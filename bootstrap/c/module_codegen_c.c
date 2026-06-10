#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) CGenerator* CGenerator_init() {
    /* pass */
    CGenerator* g = ((CGenerator*)_tr_checked_alloc(sizeof(CGenerator)));
    /* pass */
    g->buf = StringBuilder_init(65536LL);
    /* pass */
    g->fwd_buf = StringBuilder_init(4096LL);
    /* pass */
    g->struct_buf = StringBuilder_init(16384LL);
    /* pass */
    g->list_types_buf = StringBuilder_init(2048LL);
    /* pass */
    g->proto_buf = StringBuilder_init(8192LL);
    /* pass */
    g->mono_buf = StringBuilder_init(8192LL);
    /* pass */
    g->temp_count = 0LL;
    /* pass */
    g->classes = _tr_dict_new(128LL);
    /* pass */
    g->enums = _tr_dict_new(64LL);
    /* pass */
    g->interfaces = _tr_dict_new(32LL);
    /* pass */
    g->functions = _tr_dict_new(256LL);
    /* pass */
    g->decl_vars = _tr_dict_new(64LL);
    /* pass */
    g->type_subst = _tr_dict_new(8LL);
    /* pass */
    g->mono_done = _tr_dict_new(32LL);
    /* pass */
    g->list_type_done = _tr_dict_new(16LL);
    /* pass */
    g->list_fwd_done = _tr_dict_new(16LL);
    /* pass */
    g->cur_class = "";
    /* pass */
    g->cur_func = "";
    /* pass */
    g->closure_count = 0LL;
    /* pass */
    g->emitted_fns = _tr_dict_new(256LL);
    /* pass */
    g->spawn_wrappers = _tr_dict_new(16LL);
    /* pass */
    g->async_wrappers = _tr_dict_new(16LL);
    /* pass */
    g->prescanned_fns = _tr_dict_new(256LL);
    /* pass */
    g->shared_vars = _tr_dict_new(16LL);
    /* pass */
    g->cur_throws_ty = "";
    /* pass */
    g->in_task_group = 0LL;
    /* pass */
    g->in_gpu_block = 0LL;
    /* pass */
    g->value_types = _tr_dict_new(32LL);
    /* pass */
    g->global_vars = _tr_dict_new(32LL);
    /* pass */
    g->closure_cap_set = _tr_dict_new(0LL);
    /* pass */
    g->closure_env_var = "";
    /* pass */
    g->decorator_defs = _tr_dict_new(16LL);
    /* pass */
    g->overloaded_sigs = _tr_dict_new(32LL);
    /* pass */
    g->type_alias_map = _tr_dict_new(16LL);
    /* pass */
    g->defer_stack = (void*)List_str_new();
    /* pass */
    return g;
}

__attribute__((hot)) char* CGenerator_next_temp(CGenerator* self) {
    /* pass */
    self->temp_count = (self->temp_count + 1LL);
    /* pass */
    return _tr_str_concat("_t", _tr_int_to_str((long long)(self->temp_count)));
}

__attribute__((hot)) void CGenerator_reset_defer_stack(CGenerator* self) {
    /* pass */
    self->defer_stack = (void*)List_str_new();
}

__attribute__((hot)) void CGenerator_gen_func_body(CGenerator* self, HirBlock* body, long long indent) {
    /* pass */
    CGenerator_reset_defer_stack(self);
    /* pass */
    CGenerator_gen_block(self, body, indent);
    /* pass */
    if ((self->defer_stack->len > 0LL)) {
        /* pass */
        bool ends_in_return = false;
        /* pass */
        if ((body->stmts->len > 0LL)) {
            /* pass */
            __auto_type _t179 = (*((HirStmt*)List_ptr_get(body->stmts, (body->stmts->len - 1LL))));
            if (_t179.tag == HirStmt_SReturn) {
                __auto_type _ = _t179.data.SReturn.val;
                ends_in_return = true;
            } else if (1) {
                __auto_type _ = _t179;
                /* pass */
            }
        }
        /* pass */
        if ((!ends_in_return)) {
            /* pass */
            long long fdi = (self->defer_stack->len - 1LL);
            /* pass */
            while ((fdi >= 0LL)) {
                /* pass */
                CGenerator_w(self, List_str_get(self->defer_stack, fdi));
                /* pass */
                if ((fdi == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                fdi = (fdi - 1LL);
            }
        }
    }
}

__attribute__((hot)) void CGenerator_seed_params(CGenerator* self, HirFunction* f) {
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        _tr_dict_set(self->decl_vars, ((HirParam*)List_ptr_get(f->params, pi))->name, true);
        /* pass */
        pi = (pi + 1LL);
    }
}

__attribute__((hot)) void CGenerator_w(CGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->buf, s);
}

__attribute__((hot)) void CGenerator_wf(CGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->fwd_buf, s);
}

__attribute__((hot)) void CGenerator_ws(CGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->struct_buf, s);
}

__attribute__((hot)) void CGenerator_wp(CGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->proto_buf, s);
}

__attribute__((hot)) void CGenerator_wlt(CGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->list_types_buf, s);
}

__attribute__((hot)) void CGenerator_ensure_list_type(CGenerator* self, char* n) {
    /* pass */
    if (_tr_dict_contains(self->list_type_done, n)) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->list_type_done, n, true);
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct List_", n), " { "), n), "* data; size_t len; size_t capacity; } List_"), n), ";\n"));
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline List_", n), "* List_"), n), "_new(void) { List_"), n), "* l=(List_"), n), "*)malloc(sizeof(List_"), n), ")); l->data=("), n), "*)malloc(sizeof("), n), ")*8); l->len=0; l->capacity=8; return l; }\n"));
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline void List_", n), "_append(List_"), n), "* l, "), n), " val) { if(l->len==l->capacity){ l->capacity*=2; l->data=("), n), "*)realloc(l->data,sizeof("), n), ")*l->capacity); } l->data[l->len++]=val; }\n"));
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline ", n), " List_"), n), "_get(List_"), n), "* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }\n"));
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline ", n), " List_"), n), "_pop(List_"), n), "* l) { if(!l||l->len==0) return ("), n), "){0}; l->len--; return l->data[l->len]; }\n"));
    /* pass */
    CGenerator_wlt(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline void List_", n), "_free(List_"), n), "* l) { if(l){ free(l->data); free(l); } }\n"));
}

__attribute__((hot)) void CGenerator_check_and_emit_list_fwd(CGenerator* self, AstType* ty) {
    /* pass */
    if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (((strcmp((char*)ty->name, (char*)"List") == 0) || (strcmp((char*)ty->name, (char*)"Vec") == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if (_tr_dict_contains(self->enums, elem->name)) {
                /* pass */
                if ((!_tr_dict_contains(self->list_fwd_done, elem->name))) {
                    /* pass */
                    _tr_dict_set(self->list_fwd_done, elem->name, true);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct List_", elem->name), " List_"), elem->name), ";\n"));
                }
            }
        }
    }
}

__attribute__((hot)) void CGenerator_emit_list_fwd_decls(CGenerator* self, HirProgram* prog) {
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < prog->classes->len)) {
        /* pass */
        HirClass* cls = ((HirClass*)List_ptr_get(prog->classes, ci));
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < cls->fields->len)) {
            /* pass */
            CGenerator_check_and_emit_list_fwd(self, ((HirField*)List_ptr_get(cls->fields, fi))->ty);
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long ei = 0LL;
    /* pass */
    while ((ei < prog->enums->len)) {
        /* pass */
        HirEnum* enm = ((HirEnum*)List_ptr_get(prog->enums, ei));
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < enm->variants->len)) {
            /* pass */
            HirVariant* v = ((HirVariant*)List_ptr_get(enm->variants, vi));
            /* pass */
            long long vfi = 0LL;
            /* pass */
            while ((vfi < v->fields->len)) {
                /* pass */
                CGenerator_check_and_emit_list_fwd(self, ((HirParam*)List_ptr_get(v->fields, vfi))->ty);
                /* pass */
                vfi = (vfi + 1LL);
            }
            /* pass */
            vi = (vi + 1LL);
        }
        /* pass */
        ei = (ei + 1LL);
    }
}

__attribute__((hot)) char* CGenerator_type_to_c(CGenerator* self, AstType* ty) {
    /* pass */
    if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return "void";
    }
    /* pass */
    char* n = ty->name;
    /* pass */
    if ((((strcmp((char*)n, (char*)"") == 0) || (strcmp((char*)n, (char*)"void") == 0)) || (strcmp((char*)n, (char*)"None") == 0))) {
        /* pass */
        return "void";
    }
    /* pass */
    if ((((strcmp((char*)n, (char*)"ref") == 0) || (strcmp((char*)n, (char*)"mut_ref") == 0)) && (ty->args->len > 0LL))) {
        /* pass */
        return CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"def") == 0)) {
        /* pass */
        return "void*";
    }
    /* pass */
    if (_tr_dict_contains(self->type_subst, n)) {
        /* pass */
        return ((char*)(uintptr_t)_tr_dict_get(self->type_subst, n));
    }
    /* pass */
    if (_tr_dict_contains(self->type_alias_map, n)) {
        /* pass */
        return ((char*)(uintptr_t)_tr_dict_get(self->type_alias_map, n));
    }
    /* pass */
    if ((((strcmp((char*)n, (char*)"int") == 0) || (strcmp((char*)n, (char*)"i64") == 0)) || (strcmp((char*)n, (char*)"isize") == 0))) {
        /* pass */
        return "long long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i32") == 0)) {
        /* pass */
        return "int";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i16") == 0)) {
        /* pass */
        return "short";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i8") == 0)) {
        /* pass */
        return "signed char";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"u64") == 0) || (strcmp((char*)n, (char*)"usize") == 0))) {
        /* pass */
        return "unsigned long long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u32") == 0)) {
        /* pass */
        return "unsigned int";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u16") == 0)) {
        /* pass */
        return "unsigned short";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u8") == 0)) {
        /* pass */
        return "unsigned char";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"f64") == 0) || (strcmp((char*)n, (char*)"float") == 0))) {
        /* pass */
        return "double";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"f32") == 0)) {
        /* pass */
        return "float";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"bool") == 0)) {
        /* pass */
        return "bool";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"char") == 0)) {
        /* pass */
        return "char";
    }
    /* pass */
    if (_is_str_type(n)) {
        /* pass */
        return "char*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_int") == 0)) {
        /* pass */
        return "int";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_long") == 0)) {
        /* pass */
        return "long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_longlong") == 0)) {
        /* pass */
        return "long long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uint") == 0)) {
        /* pass */
        return "unsigned int";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ulong") == 0)) {
        /* pass */
        return "unsigned long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ulonglong") == 0)) {
        /* pass */
        return "unsigned long long";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_short") == 0)) {
        /* pass */
        return "short";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ushort") == 0)) {
        /* pass */
        return "unsigned short";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_char") == 0)) {
        /* pass */
        return "char";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uchar") == 0)) {
        /* pass */
        return "unsigned char";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_schar") == 0)) {
        /* pass */
        return "signed char";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_float") == 0)) {
        /* pass */
        return "float";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_double") == 0)) {
        /* pass */
        return "double";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ldouble") == 0)) {
        /* pass */
        return "long double";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"c_void_ptr") == 0) || (strcmp((char*)n, (char*)"RawPtr") == 0))) {
        /* pass */
        return "void*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_size_t") == 0)) {
        /* pass */
        return "size_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ssize_t") == 0)) {
        /* pass */
        return "ssize_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_ptrdiff_t") == 0)) {
        /* pass */
        return "ptrdiff_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_intptr_t") == 0)) {
        /* pass */
        return "intptr_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uintptr_t") == 0)) {
        /* pass */
        return "uintptr_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_int8_t") == 0)) {
        /* pass */
        return "int8_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_int16_t") == 0)) {
        /* pass */
        return "int16_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_int32_t") == 0)) {
        /* pass */
        return "int32_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_int64_t") == 0)) {
        /* pass */
        return "int64_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uint8_t") == 0)) {
        /* pass */
        return "uint8_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uint16_t") == 0)) {
        /* pass */
        return "uint16_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uint32_t") == 0)) {
        /* pass */
        return "uint32_t";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"c_uint64_t") == 0)) {
        /* pass */
        return "uint64_t";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"CFile") == 0) || (strcmp((char*)n, (char*)"c_FILE") == 0))) {
        /* pass */
        return "FILE*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"CString") == 0)) {
        /* pass */
        return "char*";
    }
    /* pass */
    if ((strcmp((char*)self->cur_func, (char*)"") != 0)) {
        /* pass */
        if (_tr_dict_contains(self->functions, self->cur_func)) {
            /* pass */
            HirFunction* f = ((HirFunction*)(uintptr_t)_tr_dict_get(self->functions, self->cur_func));
            /* pass */
            long long gi = 0LL;
            /* pass */
            while ((gi < f->generics->len)) {
                /* pass */
                if ((strcmp((char*)List_str_get(f->generics, gi), (char*)n) == 0)) {
                    /* pass */
                    return "void*";
                }
                /* pass */
                gi = (gi + 1LL);
            }
        }
    }
    /* pass */
    if ((strcmp((char*)self->cur_class, (char*)"") != 0)) {
        /* pass */
        char* base_cls_n = self->cur_class;
        /* pass */
        long long u_idx = 0LL;
        /* pass */
        char* cp = ((char*)(self->cur_class));
        /* pass */
        while ((((long long)((*(cp + u_idx)))) != 0LL)) {
            /* pass */
            if ((((long long)((*(cp + u_idx)))) == 95LL)) {
                /* pass */
                base_cls_n = _tr_str_slice(self->cur_class, 0LL, u_idx);
                /* pass */
                break;
            }
            /* pass */
            u_idx = (u_idx + 1LL);
        }
        /* pass */
        if (_tr_dict_contains(self->classes, base_cls_n)) {
            /* pass */
            HirClass* cls_def = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, base_cls_n));
            /* pass */
            long long gi = 0LL;
            /* pass */
            while ((gi < cls_def->generics->len)) {
                /* pass */
                if ((strcmp((char*)List_str_get(cls_def->generics, gi), (char*)n) == 0)) {
                    /* pass */
                    return "void*";
                }
                /* pass */
                gi = (gi + 1LL);
            }
        }
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Pointer") == 0)) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* inner = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            char* it = CGenerator_type_to_c(self, inner);
            /* pass */
            if (((strcmp((char*)it, (char*)"void") == 0) || (strcmp((char*)it, (char*)"void*") == 0))) {
                /* pass */
                return "void*";
            }
            /* pass */
            return _tr_str_concat(it, "*");
        }
        /* pass */
        return "void*";
    }
    /* pass */
    if ((((((_tr_dict_contains(self->classes, n) && (strcmp((char*)n, (char*)"Vec") != 0)) && (strcmp((char*)n, (char*)"Map") != 0)) && (strcmp((char*)n, (char*)"Dict") != 0)) && (strcmp((char*)n, (char*)"List") != 0)) && (!CGenerator_is_rt_concurrency_type(self, n)))) {
        /* pass */
        HirClass* ucls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, n));
        /* pass */
        bool is_val = _tr_dict_contains(self->value_types, n);
        /* pass */
        if (((ucls->generics->len > 0LL) && (ty->args->len > 0LL))) {
            /* pass */
            char* sfx = CGenerator_type_args_suffix(self, ty->args);
            /* pass */
            CGenerator_ensure_mono(self, ucls, ty->args);
            /* pass */
            if (is_val) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(n, "_"), sfx);
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(n, "_"), sfx), "*");
        }
        /* pass */
        if ((ucls->generics->len > 0LL)) {
            /* pass */
            char* sfx2 = CGenerator_synth_class_suffix(self, ucls);
            /* pass */
            if ((strcmp((char*)sfx2, (char*)"") != 0)) {
                /* pass */
                if (is_val) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(n, "_"), sfx2);
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(n, "_"), sfx2), "*");
            }
        }
        /* pass */
        if (is_val) {
            /* pass */
            return n;
        }
        /* pass */
        return _tr_str_concat(n, "*");
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"List") == 0)) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            char* sfx = CGenerator_list_elem_suffix(self, elem->name);
            /* pass */
            if ((strcmp((char*)sfx, (char*)"ptr") == 0)) {
                /* pass */
                return "List_ptr*";
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("List_", sfx), "*");
        }
        /* pass */
        return "List_ptr*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Vec") == 0)) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            char* sfx = CGenerator_list_elem_suffix(self, elem->name);
            /* pass */
            if ((strcmp((char*)sfx, (char*)"ptr") == 0)) {
                /* pass */
                return "List_ptr*";
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("List_", sfx), "*");
        }
        /* pass */
        return "List_ptr*";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"Map") == 0) || (strcmp((char*)n, (char*)"Dict") == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* ka = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if ((((_is_int_type(ka->name) || (strcmp((char*)ka->name, (char*)"int") == 0)) || (strcmp((char*)ka->name, (char*)"i64") == 0)) || (strcmp((char*)ka->name, (char*)"usize") == 0))) {
                /* pass */
                return "TrIDict*";
            }
        }
        /* pass */
        return "TrMap*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Set") == 0)) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* se = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if ((((_is_int_type(se->name) || (strcmp((char*)se->name, (char*)"int") == 0)) || (strcmp((char*)se->name, (char*)"i64") == 0)) || (strcmp((char*)se->name, (char*)"usize") == 0))) {
                /* pass */
                return "_TrISet*";
            }
        }
        /* pass */
        return "_TrSet*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Chan") == 0)) {
        /* pass */
        return "_TrChan*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Mutex") == 0)) {
        /* pass */
        return "_TrMutexBox*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"RwLock") == 0)) {
        /* pass */
        return "_TrRWLBox*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"ThreadPool") == 0)) {
        /* pass */
        return "_TrThreadPool*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Thread") == 0)) {
        /* pass */
        return "_TrThreadObj*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Atomic") == 0)) {
        /* pass */
        return "_TrAtomic*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"ThreadLocal") == 0)) {
        /* pass */
        return "_TrTLS*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Shared") == 0)) {
        /* pass */
        return "_TrSharedBox*";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Weak") == 0)) {
        /* pass */
        return "_TrWeakBox*";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"Tuple") == 0) || (strcmp((char*)n, (char*)"tuple") == 0))) {
        /* pass */
        return "TrTuple";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Result") == 0)) {
        /* pass */
        return "Result";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"Option") == 0)) {
        /* pass */
        return "Option";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"lambda") == 0)) {
        /* pass */
        return "void*";
    }
    /* pass */
    if (_tr_dict_contains(self->enums, n)) {
        /* pass */
        return n;
    }
    /* pass */
    if (_tr_dict_contains(self->interfaces, n)) {
        /* pass */
        HirInterface* gi_iface = ((HirInterface*)(uintptr_t)_tr_dict_get(self->interfaces, n));
        /* pass */
        if (((gi_iface->generics->len > 0LL) && (ty->args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(n, "_"), CGenerator_type_args_suffix(self, ty->args)), "_obj");
        }
        /* pass */
        return _tr_str_concat(n, "_obj");
    }
    /* pass */
    return _tr_str_concat(n, "*");
}

__attribute__((hot)) char* CGenerator_type_suffix(CGenerator* self, char* n) {
    /* pass */
    if (((strcmp((char*)n, (char*)"int") == 0) || (strcmp((char*)n, (char*)"i64") == 0))) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i32") == 0)) {
        /* pass */
        return "i32";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i16") == 0)) {
        /* pass */
        return "i16";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i8") == 0)) {
        /* pass */
        return "i8";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"u64") == 0) || (strcmp((char*)n, (char*)"usize") == 0))) {
        /* pass */
        return "u64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u32") == 0)) {
        /* pass */
        return "u32";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u16") == 0)) {
        /* pass */
        return "u16";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u8") == 0)) {
        /* pass */
        return "u8";
    }
    /* pass */
    if (((strcmp((char*)n, (char*)"f64") == 0) || (strcmp((char*)n, (char*)"float") == 0))) {
        /* pass */
        return "f64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"f32") == 0)) {
        /* pass */
        return "f32";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"bool") == 0)) {
        /* pass */
        return "bool";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"char") == 0)) {
        /* pass */
        return "char";
    }
    /* pass */
    if (_is_str_type(n)) {
        /* pass */
        return "str";
    }
    /* pass */
    char* np = ((char*)(n));
    /* pass */
    long long nlen = 0LL;
    /* pass */
    while ((((long long)((*(np + nlen)))) != 0LL)) {
        /* pass */
        nlen = (nlen + 1LL);
    }
    /* pass */
    if ((nlen > 0LL)) {
        /* pass */
        if ((((long long)((*(np + (nlen - 1LL))))) == 42LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_slice(n, 0LL, (nlen - 1LL)), "_ptr");
        }
    }
    /* pass */
    return n;
}

__attribute__((hot)) char* CGenerator_list_elem_suffix(CGenerator* self, char* n) {
    /* pass */
    char* actual_n = n;
    /* pass */
    if (_tr_dict_contains(self->type_subst, n)) {
        /* pass */
        char* cv = ((char*)(uintptr_t)_tr_dict_get(self->type_subst, n));
        /* pass */
        if ((strcmp((char*)cv, (char*)"long long") == 0)) {
            /* pass */
            actual_n = "int";
        } else if ((strcmp((char*)cv, (char*)"double") == 0)) {
            /* pass */
            actual_n = "float";
        } else if ((strcmp((char*)cv, (char*)"char*") == 0)) {
            /* pass */
            actual_n = "str";
        } else if ((strcmp((char*)cv, (char*)"bool") == 0)) {
            /* pass */
            actual_n = "bool";
        } else if ((strcmp((char*)cv, (char*)"char") == 0)) {
            /* pass */
            actual_n = "char";
        } else if ((strcmp((char*)cv, (char*)"unsigned long long") == 0)) {
            /* pass */
            actual_n = "u64";
        } else if ((strcmp((char*)cv, (char*)"unsigned int") == 0)) {
            /* pass */
            actual_n = "u32";
        } else if ((strcmp((char*)cv, (char*)"int") == 0)) {
            /* pass */
            actual_n = "i32";
        } else if ((strcmp((char*)cv, (char*)"short") == 0)) {
            /* pass */
            actual_n = "i16";
        } else if ((strcmp((char*)cv, (char*)"signed char") == 0)) {
            /* pass */
            actual_n = "i8";
        } else if ((strcmp((char*)cv, (char*)"float") == 0)) {
            /* pass */
            actual_n = "f32";
        }
    }
    /* pass */
    char* sfx = CGenerator_type_suffix(self, actual_n);
    /* pass */
    if ((((((((((strcmp((char*)sfx, (char*)"i64") == 0) || (strcmp((char*)sfx, (char*)"f64") == 0)) || (strcmp((char*)sfx, (char*)"str") == 0)) || (strcmp((char*)sfx, (char*)"bool") == 0)) || (strcmp((char*)sfx, (char*)"i8") == 0)) || (strcmp((char*)sfx, (char*)"i32") == 0)) || (strcmp((char*)sfx, (char*)"char") == 0)) || (strcmp((char*)sfx, (char*)"u8") == 0)) || (strcmp((char*)sfx, (char*)"u32") == 0))) {
        /* pass */
        return sfx;
    }
    /* pass */
    if (_tr_dict_contains(self->enums, actual_n)) {
        /* pass */
        CGenerator_ensure_list_type(self, actual_n);
        /* pass */
        return actual_n;
    }
    /* pass */
    return "ptr";
}

__attribute__((hot)) char* CGenerator_type_args_suffix(CGenerator* self, List_ptr* args) {
    /* pass */
    char* sfx = "";
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            sfx = _tr_str_concat(sfx, "_");
        }
        /* pass */
        char* arg_name = (*((AstType**)List_ptr_get(args, i)))->name;
        /* pass */
        if (_tr_dict_contains(self->type_subst, arg_name)) {
            /* pass */
            char* concrete = ((char*)(uintptr_t)_tr_dict_get(self->type_subst, arg_name));
            /* pass */
            if ((strcmp((char*)concrete, (char*)"long long") == 0)) {
                /* pass */
                arg_name = "int";
            } else if ((strcmp((char*)concrete, (char*)"double") == 0)) {
                /* pass */
                arg_name = "float";
            } else if ((strcmp((char*)concrete, (char*)"char*") == 0)) {
                /* pass */
                arg_name = "str";
            } else if ((strcmp((char*)concrete, (char*)"bool") == 0)) {
                /* pass */
                arg_name = "bool";
            } else if ((strcmp((char*)concrete, (char*)"char") == 0)) {
                /* pass */
                arg_name = "char";
            } else if ((strcmp((char*)concrete, (char*)"unsigned long long") == 0)) {
                /* pass */
                arg_name = "u64";
            } else if ((strcmp((char*)concrete, (char*)"unsigned int") == 0)) {
                /* pass */
                arg_name = "u32";
            } else if ((strcmp((char*)concrete, (char*)"int") == 0)) {
                /* pass */
                arg_name = "i32";
            } else if ((strcmp((char*)concrete, (char*)"short") == 0)) {
                /* pass */
                arg_name = "i16";
            } else if ((strcmp((char*)concrete, (char*)"signed char") == 0)) {
                /* pass */
                arg_name = "i8";
            } else if ((strcmp((char*)concrete, (char*)"float") == 0)) {
                /* pass */
                arg_name = "f32";
            } else {
                /* pass */
                arg_name = concrete;
            }
        }
        /* pass */
        sfx = _tr_str_concat(sfx, CGenerator_type_suffix(self, arg_name));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return sfx;
}

__attribute__((hot)) char* CGenerator_synth_class_suffix(CGenerator* self, HirClass* ucls) {
    /* pass */
    char* sfx = "";
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < ucls->generics->len)) {
        /* pass */
        char* gp = List_str_get(ucls->generics, gi);
        /* pass */
        if ((!_tr_dict_contains(self->type_subst, gp))) {
            /* pass */
            return "";
        }
        /* pass */
        if ((gi > 0LL)) {
            /* pass */
            sfx = _tr_str_concat(sfx, "_");
        }
        /* pass */
        char* cv = ((char*)(uintptr_t)_tr_dict_get(self->type_subst, gp));
        /* pass */
        char* tn = cv;
        /* pass */
        if ((strcmp((char*)cv, (char*)"long long") == 0)) {
            /* pass */
            tn = "int";
        } else if ((strcmp((char*)cv, (char*)"double") == 0)) {
            /* pass */
            tn = "float";
        } else if ((strcmp((char*)cv, (char*)"char*") == 0)) {
            /* pass */
            tn = "str";
        } else if ((strcmp((char*)cv, (char*)"bool") == 0)) {
            /* pass */
            tn = "bool";
        } else if ((strcmp((char*)cv, (char*)"char") == 0)) {
            /* pass */
            tn = "char";
        } else if ((strcmp((char*)cv, (char*)"unsigned long long") == 0)) {
            /* pass */
            tn = "u64";
        } else if ((strcmp((char*)cv, (char*)"unsigned int") == 0)) {
            /* pass */
            tn = "u32";
        } else if ((strcmp((char*)cv, (char*)"int") == 0)) {
            /* pass */
            tn = "i32";
        } else if ((strcmp((char*)cv, (char*)"short") == 0)) {
            /* pass */
            tn = "i16";
        } else if ((strcmp((char*)cv, (char*)"signed char") == 0)) {
            /* pass */
            tn = "i8";
        } else if ((strcmp((char*)cv, (char*)"float") == 0)) {
            /* pass */
            tn = "f32";
        }
        /* pass */
        sfx = _tr_str_concat(sfx, CGenerator_type_suffix(self, tn));
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    return sfx;
}

__attribute__((hot)) void CGenerator_ensure_mono(CGenerator* self, HirClass* cls, List_ptr* type_args) {
    /* pass */
    char* sfx = CGenerator_type_args_suffix(self, type_args);
    /* pass */
    char* mono_key = _tr_str_concat(_tr_str_concat(cls->name, "_"), sfx);
    /* pass */
    if (_tr_dict_contains(self->mono_done, mono_key)) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->mono_done, mono_key, true);
    /* pass */
    char* mono_name = _tr_str_concat(_tr_str_concat(cls->name, "_"), sfx);
    /* pass */
    List_str* resolved_args = (void*)List_str_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < type_args->len)) {
        /* pass */
        List_str_append(resolved_args, CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(type_args, i)))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrMap* old_subst = self->type_subst;
    /* pass */
    self->type_subst = _tr_dict_new(8LL);
    /* pass */
    i = 0LL;
    /* pass */
    while (((i < cls->generics->len) && (i < resolved_args->len))) {
        /* pass */
        char* param = List_str_get(cls->generics, i);
        /* pass */
        char* concrete = List_str_get(resolved_args, i);
        /* pass */
        _tr_dict_set(self->type_subst, param, concrete);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder* old_buf = self->buf;
    /* pass */
    StringBuilder* old_sbuf = self->struct_buf;
    /* pass */
    StringBuilder* old_pbuf = self->proto_buf;
    /* pass */
    self->buf = self->mono_buf;
    /* pass */
    self->struct_buf = self->mono_buf;
    /* pass */
    self->proto_buf = self->mono_buf;
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct ", mono_name), " "), mono_name), ";\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat("typedef struct ", mono_name), " {\n"));
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < cls->fields->len)) {
        /* pass */
        HirField* f = ((HirField*)List_ptr_get(cls->fields, fi));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", CGenerator_type_to_c(self, f->ty)), " "), f->name), ";\n"));
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    if ((cls->fields->len == 0LL)) {
        /* pass */
        CGenerator_w(self, "    char _pad;\n");
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat("} ", mono_name), ";\n\n"));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(cls->methods, mi));
        /* pass */
        char* old_class = self->cur_class;
        /* pass */
        char* old_func = self->cur_func;
        /* pass */
        self->cur_class = mono_name;
        /* pass */
        self->cur_func = m->name;
        /* pass */
        char* sig = _tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, mono_name));
        /* pass */
        CGenerator_w(self, _tr_str_concat(sig, ";\n"));
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    mi = 0LL;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(cls->methods, mi));
        /* pass */
        char* old_class = self->cur_class;
        /* pass */
        char* old_func = self->cur_func;
        /* pass */
        self->cur_class = mono_name;
        /* pass */
        self->cur_func = m->name;
        /* pass */
        self->decl_vars = _tr_dict_new(64LL);
        /* pass */
        CGenerator_seed_params(self, m);
        /* pass */
        char* sig = _tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, mono_name));
        /* pass */
        CGenerator_w(self, _tr_str_concat(sig, " {\n"));
        /* pass */
        CGenerator_gen_func_body(self, m->body, 1LL);
        /* pass */
        CGenerator_w(self, "}\n\n");
        /* pass */
        self->cur_class = old_class;
        /* pass */
        self->cur_func = old_func;
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    if (((((unsigned long long)(cls->iface_names)) != ((unsigned long long)(0LL))) && (!_is_invalid_ptr(((unsigned long long)(cls->iface_names)))))) {
        /* pass */
        long long ifw_i = 0LL;
        /* pass */
        while ((ifw_i < cls->iface_names->len)) {
            /* pass */
            char* iface_base = List_str_get(cls->iface_names, ifw_i);
            /* pass */
            if (_tr_dict_contains(self->interfaces, iface_base)) {
                /* pass */
                HirInterface* m_iface = ((HirInterface*)(uintptr_t)_tr_dict_get(self->interfaces, iface_base));
                /* pass */
                if ((!_is_invalid_ptr(((unsigned long long)(m_iface))))) {
                    /* pass */
                    char* mono_iface_name = iface_base;
                    /* pass */
                    if ((m_iface->generics->len > 0LL)) {
                        /* pass */
                        mono_iface_name = _tr_str_concat(_tr_str_concat(iface_base, "_"), sfx);
                    }
                    /* pass */
                    char* vtbl_done_key = _tr_str_concat("iface_obj_", mono_iface_name);
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, vtbl_done_key))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, vtbl_done_key, true);
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat("typedef struct _", mono_iface_name), "_vtable {\n"));
                        /* pass */
                        long long vmi = 0LL;
                        /* pass */
                        while ((vmi < m_iface->methods->len)) {
                            /* pass */
                            HirFunction* vm2 = ((HirFunction*)List_ptr_get(m_iface->methods, vmi));
                            /* pass */
                            char* vret2 = CGenerator_type_to_c(self, vm2->ret_ty);
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", vret2), " (*"), vm2->name), ")(void* self"));
                            /* pass */
                            long long vpi = 0LL;
                            /* pass */
                            while ((vpi < vm2->params->len)) {
                                /* pass */
                                HirParam* vp2 = ((HirParam*)List_ptr_get(vm2->params, vpi));
                                /* pass */
                                if ((strcmp((char*)vp2->name, (char*)"self") != 0)) {
                                    /* pass */
                                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(", ", CGenerator_type_to_c(self, vp2->ty)), " "), vp2->name));
                                }
                                /* pass */
                                vpi = (vpi + 1LL);
                            }
                            /* pass */
                            CGenerator_w(self, ");\n");
                            /* pass */
                            vmi = (vmi + 1LL);
                        }
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat("} ", mono_iface_name), "_vtable;\n\n"));
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct { ", mono_iface_name), "_vtable* vtable; void* data; } "), mono_iface_name), "_obj;\n\n"));
                    }
                    /* pass */
                    char* wrap_done_key = _tr_str_concat(_tr_str_concat(_tr_str_concat("iface_wrap_", mono_name), "_"), mono_iface_name);
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, wrap_done_key))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, wrap_done_key, true);
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline ", mono_iface_name), "_obj "), mono_name), "_as_"), mono_iface_name), "("), mono_name), "* self) {\n"));
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    static const ", mono_iface_name), "_vtable _vtbl_"), mono_name), "_"), mono_iface_name), " = {\n"));
                        /* pass */
                        long long wm_i = 0LL;
                        /* pass */
                        while ((wm_i < m_iface->methods->len)) {
                            /* pass */
                            HirFunction* wm2 = ((HirFunction*)List_ptr_get(m_iface->methods, wm_i));
                            /* pass */
                            char* wret_c2 = CGenerator_type_to_c(self, wm2->ret_ty);
                            /* pass */
                            char* wcast2 = _tr_str_concat(wret_c2, "(*)(void*");
                            /* pass */
                            long long wp_i = 0LL;
                            /* pass */
                            while ((wp_i < wm2->params->len)) {
                                /* pass */
                                HirParam* wp2 = ((HirParam*)List_ptr_get(wm2->params, wp_i));
                                /* pass */
                                if ((strcmp((char*)wp2->name, (char*)"self") != 0)) {
                                    /* pass */
                                    wcast2 = _tr_str_concat(_tr_str_concat(wcast2, ", "), CGenerator_type_to_c(self, wp2->ty));
                                }
                                /* pass */
                                wp_i = (wp_i + 1LL);
                            }
                            /* pass */
                            wcast2 = _tr_str_concat(wcast2, ")");
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("        .", wm2->name), " = ("), wcast2), ")("), mono_name), "_"), wm2->name), "),\n"));
                            /* pass */
                            wm_i = (wm_i + 1LL);
                        }
                        /* pass */
                        CGenerator_w(self, "    };\n");
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    return (", mono_iface_name), "_obj){ .vtable = ("), mono_iface_name), "_vtable*)&_vtbl_"), mono_name), "_"), mono_iface_name), ", .data = (void*)self };\n"));
                        /* pass */
                        CGenerator_w(self, "}\n\n");
                    }
                }
            }
            /* pass */
            ifw_i = (ifw_i + 1LL);
        }
    }
    /* pass */
    self->mono_buf = self->buf;
    /* pass */
    self->buf = old_buf;
    /* pass */
    self->struct_buf = old_sbuf;
    /* pass */
    self->proto_buf = old_pbuf;
    /* pass */
    self->type_subst = old_subst;
}

__attribute__((hot)) void CGenerator_ensure_mono_func(CGenerator* self, char* fname, char* targ) {
    /* pass */
    char* mono_key = _tr_str_concat(_tr_str_concat(_tr_str_concat("func_", fname), "__MONO_"), targ);
    /* pass */
    if (_tr_dict_contains(self->mono_done, mono_key)) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->mono_done, mono_key, true);
    /* pass */
    if ((!_tr_dict_contains(self->functions, fname))) {
        /* pass */
        return;
    }
    /* pass */
    HirFunction* f = ((HirFunction*)(uintptr_t)_tr_dict_get(self->functions, fname));
    /* pass */
    if ((f->generics->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    char* type_param = List_str_get(f->generics, 0LL);
    /* pass */
    char* concrete_c = CGenerator_type_to_c(self, AstType_init(targ));
    /* pass */
    TrMap* old_subst = self->type_subst;
    /* pass */
    self->type_subst = _tr_dict_new(4LL);
    /* pass */
    _tr_dict_set(self->type_subst, type_param, concrete_c);
    /* pass */
    StringBuilder* old_buf = self->buf;
    /* pass */
    StringBuilder* old_sbuf = self->struct_buf;
    /* pass */
    StringBuilder* old_pbuf = self->proto_buf;
    /* pass */
    self->buf = self->mono_buf;
    /* pass */
    self->struct_buf = self->mono_buf;
    /* pass */
    self->proto_buf = self->mono_buf;
    /* pass */
    char* mono_fname = _tr_str_concat(_tr_str_concat(fname, "__MONO_"), targ);
    /* pass */
    char* ret_s = CGenerator_type_to_c(self, f->ret_ty);
    /* pass */
    char* sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(ret_s, " "), mono_fname), "(");
    /* pass */
    bool first = true;
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        if ((strcmp((char*)p->name, (char*)"self") != 0)) {
            /* pass */
            if ((!first)) {
                /* pass */
                sig = _tr_str_concat(sig, ", ");
            }
            /* pass */
            sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(sig, CGenerator_type_to_c(self, p->ty)), " "), _safe_c_varname(p->name));
            /* pass */
            first = false;
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    sig = _tr_str_concat(sig, ")");
    /* pass */
    CGenerator_w(self, _tr_str_concat(sig, ";\n"));
    /* pass */
    char* old_class = self->cur_class;
    /* pass */
    char* old_func = self->cur_func;
    /* pass */
    self->cur_class = "";
    /* pass */
    self->cur_func = mono_fname;
    /* pass */
    self->decl_vars = _tr_dict_new(64LL);
    /* pass */
    CGenerator_seed_params(self, f);
    /* pass */
    CGenerator_w(self, _tr_str_concat(sig, " {\n"));
    /* pass */
    CGenerator_gen_func_body(self, f->body, 1LL);
    /* pass */
    CGenerator_w(self, "}\n\n");
    /* pass */
    self->cur_class = old_class;
    /* pass */
    self->cur_func = old_func;
    /* pass */
    self->mono_buf = self->buf;
    /* pass */
    self->buf = old_buf;
    /* pass */
    self->struct_buf = old_sbuf;
    /* pass */
    self->proto_buf = old_pbuf;
    /* pass */
    self->type_subst = old_subst;
}

__attribute__((hot)) char* CGenerator_get_user_decorator_attr(CGenerator* self, char* name) {
    /* pass */
    if ((!_tr_dict_contains(self->decorator_defs, name))) {
        /* pass */
        return "";
    }
    /* pass */
    HirFunction* df = ((HirFunction*)(uintptr_t)_tr_dict_get(self->decorator_defs, name));
    /* pass */
    if ((df->body->stmts->len == 1LL)) {
        /* pass */
        __auto_type _t180 = (*((HirStmt*)List_ptr_get(df->body->stmts, 0LL)));
        if (_t180.tag == HirStmt_SReturn) {
            __auto_type rv = _t180.data.SReturn.val;
            /* pass */
            if ((((unsigned long long)(rv)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t181 = (*rv);
                if (_t181.tag == HirExpr_ELitStr) {
                    __auto_type attr_s = _t181.data.ELitStr.val;
                    /* pass */
                    return _tr_str_concat(_tr_str_concat("__attribute__((", attr_s), ")) ");
                } else if (1) {
                    __auto_type _ = _t181;
                    /* pass */
                }
            }
        } else if (1) {
            __auto_type _ = _t180;
            /* pass */
        }
    }
    /* pass */
    return "";
}

__attribute__((hot)) char* CGenerator_get_inline_attrs(CGenerator* self, HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"inline") == 0)) {
            /* pass */
            return "static inline __attribute__((always_inline,hot)) ";
        }
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"hot") == 0)) {
            /* pass */
            return "static __attribute__((hot)) ";
        }
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"noinline") == 0)) {
            /* pass */
            return "static __attribute__((noinline)) ";
        }
        /* pass */
        char* user_attr = CGenerator_get_user_decorator_attr(self, ((Decorator*)List_ptr_get(f->decorators, i))->name);
        /* pass */
        if ((strcmp((char*)user_attr, (char*)"") != 0)) {
            /* pass */
            return _tr_str_concat("static ", user_attr);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((strcmp((char*)f->name, (char*)"main") == 0) || (strcmp((char*)f->name, (char*)"_tr_main") == 0))) {
        /* pass */
        return "__attribute__((hot)) ";
    }
    /* pass */
    if ((strcmp((char*)f->name, (char*)"init") == 0)) {
        /* pass */
        return "static __attribute__((malloc,returns_nonnull,hot)) ";
    }
    /* pass */
    return "static __attribute__((hot)) ";
}

__attribute__((hot)) bool CGenerator_is_rt_concurrency_type(CGenerator* self, char* name) {
    /* pass */
    if ((strcmp((char*)name, (char*)"Atomic") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Thread") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Mutex") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"RwLock") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"Chan") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"ThreadPool") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"ThreadLocal") == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) char* CGenerator_get_proto_attrs(CGenerator* self, HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"inline") == 0)) {
            /* pass */
            return "inline __attribute__((always_inline,hot)) ";
        }
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"hot") == 0)) {
            /* pass */
            return "__attribute__((hot)) ";
        }
        /* pass */
        if ((strcmp((char*)((Decorator*)List_ptr_get(f->decorators, i))->name, (char*)"noinline") == 0)) {
            /* pass */
            return "__attribute__((noinline)) ";
        }
        /* pass */
        char* user_attr_p = CGenerator_get_user_decorator_attr(self, ((Decorator*)List_ptr_get(f->decorators, i))->name);
        /* pass */
        if ((strcmp((char*)user_attr_p, (char*)"") != 0)) {
            /* pass */
            return user_attr_p;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((strcmp((char*)f->name, (char*)"main") == 0) || (strcmp((char*)f->name, (char*)"_tr_main") == 0))) {
        /* pass */
        return "__attribute__((hot)) ";
    }
    /* pass */
    if ((strcmp((char*)f->name, (char*)"init") == 0)) {
        /* pass */
        return "__attribute__((malloc,returns_nonnull,hot)) ";
    }
    /* pass */
    return "__attribute__((hot)) ";
}

__attribute__((hot)) char* CGenerator_gen_func_sig(CGenerator* self, HirFunction* f, char* class_name) {
    /* pass */
    char* old_f = self->cur_func;
    /* pass */
    self->cur_func = f->name;
    /* pass */
    char* ret_ty_str = CGenerator_type_to_c(self, f->ret_ty);
    /* pass */
    if ((strcmp((char*)f->throws_ty->name, (char*)"") != 0)) {
        /* pass */
        ret_ty_str = "Result";
    }
    /* pass */
    char* fname = f->name;
    /* pass */
    if ((strcmp((char*)fname, (char*)"main") == 0)) {
        /* pass */
        fname = "_tr_main";
    }
    /* pass */
    if (_is_c_keyword(fname)) {
        /* pass */
        fname = _tr_str_concat("_tr_fn_", fname);
    }
    /* pass */
    if ((strcmp((char*)class_name, (char*)"") != 0)) {
        /* pass */
        char* base_sig_key = _tr_str_concat(_tr_str_concat(class_name, "_"), fname);
        /* pass */
        if (_tr_dict_contains(self->overloaded_sigs, base_sig_key)) {
            /* pass */
            long long param_count = 0LL;
            /* pass */
            long long opi = 0LL;
            /* pass */
            while ((opi < f->params->len)) {
                /* pass */
                if ((strcmp((char*)((HirParam*)List_ptr_get(f->params, opi))->name, (char*)"self") != 0)) {
                    /* pass */
                    param_count = (param_count + 1LL);
                }
                /* pass */
                opi = (opi + 1LL);
            }
            /* pass */
            fname = _tr_str_concat(_tr_str_concat(_tr_str_concat(base_sig_key, "_"), _tr_int_to_str((long long)(param_count))), "arg");
        } else {
            /* pass */
            fname = base_sig_key;
        }
    }
    /* pass */
    char* sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(ret_ty_str, " "), fname), "(");
    /* pass */
    bool first = true;
    /* pass */
    if (((strcmp((char*)class_name, (char*)"") != 0) && (!f->is_static))) {
        /* pass */
        if (_tr_dict_contains(self->enums, class_name)) {
            /* pass */
            sig = _tr_str_concat(_tr_str_concat(sig, class_name), " self");
        } else {
            /* pass */
            sig = _tr_str_concat(_tr_str_concat(sig, class_name), "* self");
        }
        /* pass */
        first = false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        if ((strcmp((char*)p->name, (char*)"self") != 0)) {
            /* pass */
            if ((!first)) {
                /* pass */
                sig = _tr_str_concat(sig, ", ");
            }
            /* pass */
            sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(sig, CGenerator_type_to_c(self, p->ty)), " "), _safe_c_varname(p->name));
            /* pass */
            first = false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (f->is_variadic) {
        /* pass */
        if ((!first)) {
            /* pass */
            sig = _tr_str_concat(sig, ", ");
        }
        /* pass */
        sig = _tr_str_concat(sig, "...");
    }
    /* pass */
    sig = _tr_str_concat(sig, ")");
    /* pass */
    self->cur_func = old_f;
    /* pass */
    return sig;
}

__attribute__((hot)) void CGenerator_emit_base_fields(CGenerator* self, char* base_name) {
    /* pass */
    if (_tr_dict_contains(self->classes, base_name)) {
        /* pass */
        HirClass* base_cls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, base_name));
        /* pass */
        long long bfi = 0LL;
        /* pass */
        while ((bfi < base_cls->fields->len)) {
            /* pass */
            HirField* bf = ((HirField*)List_ptr_get(base_cls->fields, bfi));
            /* pass */
            CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", CGenerator_type_to_c(self, bf->ty)), " "), bf->name), ";\n"));
            /* pass */
            bfi = (bfi + 1LL);
        }
    }
}

__attribute__((hot)) void CGenerator_gen_class_struct(CGenerator* self, HirClass* c) {
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("#ifndef ", c->name), "_STRUCT_DEFINED\n"));
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("#define ", c->name), "_STRUCT_DEFINED\n"));
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("typedef struct ", c->name), " {\n"));
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < c->base_classes->len)) {
        /* pass */
        char* base_name = List_str_get(c->base_classes, bi);
        /* pass */
        CGenerator_emit_base_fields(self, base_name);
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        HirField* f = ((HirField*)List_ptr_get(c->fields, i));
        /* pass */
        CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", CGenerator_type_to_c(self, f->ty)), " "), f->name), ";\n"));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((c->fields->len == 0LL) && (c->base_classes->len == 0LL))) {
        /* pass */
        CGenerator_ws(self, "    char _pad;\n");
    }
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("} ", c->name), ";\n"));
    /* pass */
    CGenerator_ws(self, "#endif\n\n");
}

__attribute__((hot)) void CGenerator_gen_enum_struct(CGenerator* self, HirEnum* e) {
    /* pass */
    CGenerator_ws(self, "typedef enum {\n");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < e->variants->len)) {
        /* pass */
        HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, i));
        /* pass */
        CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat("    ", e->name), "_"), v->name));
        /* pass */
        if ((i < (e->variants->len - 1LL))) {
            /* pass */
            CGenerator_ws(self, ",");
        }
        /* pass */
        CGenerator_ws(self, "\n");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((e->variants->len == 0LL)) {
        /* pass */
        CGenerator_ws(self, _tr_str_concat(_tr_str_concat("    ", e->name), "_EMPTY\n"));
    }
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("} ", e->name), "_tag;\n\n"));
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("typedef struct ", e->name), " {\n"));
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("    ", e->name), "_tag tag;\n"));
    /* pass */
    bool has_data = false;
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < e->variants->len)) {
        /* pass */
        if ((((HirVariant*)List_ptr_get(e->variants, i))->fields->len > 0LL)) {
            /* pass */
            has_data = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (has_data) {
        /* pass */
        CGenerator_ws(self, "    union {\n");
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < e->variants->len)) {
            /* pass */
            HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, i));
            /* pass */
            if ((v->fields->len > 0LL)) {
                /* pass */
                CGenerator_ws(self, "        struct {\n");
                /* pass */
                long long j = 0LL;
                /* pass */
                while ((j < v->fields->len)) {
                    /* pass */
                    HirParam* fld = ((HirParam*)List_ptr_get(v->fields, j));
                    /* pass */
                    CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("            ", CGenerator_type_to_c(self, fld->ty)), " "), fld->name), ";\n"));
                    /* pass */
                    j = (j + 1LL);
                }
                /* pass */
                CGenerator_ws(self, _tr_str_concat(_tr_str_concat("        } ", v->name), ";\n"));
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        CGenerator_ws(self, "    } data;\n");
    }
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("} ", e->name), ";\n\n"));
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < e->variants->len)) {
        /* pass */
        HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, i));
        /* pass */
        if ((v->fields->len == 0LL)) {
            /* pass */
            CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("#define ", e->name), "_make_"), v->name), "() (("), e->name), "){.tag="), e->name), "_"), v->name), "})\n"));
        } else {
            /* pass */
            CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline __attribute__((always_inline)) ", e->name), " "), e->name), "_ctor_"), v->name), "("));
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < v->fields->len)) {
                /* pass */
                if ((j > 0LL)) {
                    /* pass */
                    CGenerator_ws(self, ", ");
                }
                /* pass */
                CGenerator_ws(self, _tr_str_concat(_tr_str_concat(CGenerator_type_to_c(self, ((HirParam*)List_ptr_get(v->fields, j))->ty), " "), ((HirParam*)List_ptr_get(v->fields, j))->name));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(") { ", e->name), " _r = {.tag="), e->name), "_"), v->name), "}; "));
            /* pass */
            j = 0LL;
            /* pass */
            while ((j < v->fields->len)) {
                /* pass */
                CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_r.data.", v->name), "."), ((HirParam*)List_ptr_get(v->fields, j))->name), " = "), ((HirParam*)List_ptr_get(v->fields, j))->name), "; "));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            CGenerator_ws(self, "return _r; }\n");
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_ws(self, "\n");
    /* pass */
    if ((!_tr_dict_contains(self->list_type_done, e->name))) {
        /* pass */
        CGenerator_ensure_list_type(self, e->name);
    }
}

__attribute__((hot)) void CGenerator_gen_interface_vtable(CGenerator* self, HirInterface* iface) {
    /* pass */
    if ((iface->generics->len > 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("typedef struct _", iface->name), "_vtable {\n"));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < iface->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(iface->methods, i));
        /* pass */
        char* ret = CGenerator_type_to_c(self, m->ret_ty);
        /* pass */
        CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", ret), " (*"), m->name), ")(void* self"));
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < m->params->len)) {
            /* pass */
            HirParam* p = ((HirParam*)List_ptr_get(m->params, j));
            /* pass */
            if ((strcmp((char*)p->name, (char*)"self") != 0)) {
                /* pass */
                CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(", ", CGenerator_type_to_c(self, p->ty)), " "), p->name));
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        CGenerator_ws(self, ");\n");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_ws(self, _tr_str_concat(_tr_str_concat("} ", iface->name), "_vtable;\n\n"));
    /* pass */
    if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("iface_obj_", iface->name)))) {
        /* pass */
        _tr_dict_set(self->emitted_fns, _tr_str_concat("iface_obj_", iface->name), true);
        /* pass */
        CGenerator_ws(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct { ", iface->name), "_vtable* vtable; void* data; } "), iface->name), "_obj;\n\n"));
    }
}

__attribute__((hot)) char* CGenerator_gen_one_iface_wrap(CGenerator* self, char* cls_name, HirInterface* iface) {
    /* pass */
    char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static inline ", iface->name), "_obj "), cls_name), "_as_"), iface->name), "("), cls_name), "* self) {\n");
    /* pass */
    s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "    static const "), iface->name), "_vtable _vtbl_"), cls_name), "_"), iface->name), " = {\n");
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < iface->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(iface->methods, mi));
        /* pass */
        char* ret_c = CGenerator_type_to_c(self, m->ret_ty);
        /* pass */
        char* cast_sig = _tr_str_concat(ret_c, "(*)(void*");
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < m->params->len)) {
            /* pass */
            HirParam* p = ((HirParam*)List_ptr_get(m->params, pi));
            /* pass */
            if ((strcmp((char*)p->name, (char*)"self") != 0)) {
                /* pass */
                cast_sig = _tr_str_concat(_tr_str_concat(cast_sig, ", "), CGenerator_type_to_c(self, p->ty));
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        cast_sig = _tr_str_concat(cast_sig, ")");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "        ."), m->name), " = ("), cast_sig), ")("), cls_name), "_"), m->name), "),\n");
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    s = _tr_str_concat(s, "    };\n");
    /* pass */
    s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "    return ("), iface->name), "_obj){ .vtable = ("), iface->name), "_vtable*)&_vtbl_"), cls_name), "_"), iface->name), ", .data = (void*)self };\n");
    /* pass */
    s = _tr_str_concat(s, "}\n");
    /* pass */
    return s;
}

__attribute__((hot)) char* CGenerator_gen_expr(CGenerator* self, HirExpr* e_ptr) {
    /* pass */
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return "NULL";
    }
    /* pass */
    __auto_type e = (*e_ptr);
    /* pass */
    __auto_type _t182 = e;
    if (_t182.tag == HirExpr_ELitInt) {
        __auto_type v = _t182.data.ELitInt.val;
        return _tr_str_concat(_tr_int_to_str((long long)(v)), "LL");
    } else if (_t182.tag == HirExpr_ELitFloat) {
        __auto_type v = _t182.data.ELitFloat.val;
        return _tr_float_to_c_lit(v);
    } else if (_t182.tag == HirExpr_ELitStr) {
        __auto_type v = _t182.data.ELitStr.val;
        return _tr_str_concat(_tr_str_concat("\"", _escape_str_for_c(v)), "\"");
    } else if (_t182.tag == HirExpr_ERawStr) {
        __auto_type v = _t182.data.ERawStr.val;
        return _tr_str_concat(_tr_str_concat("\"", _escape_str_for_c(v)), "\"");
    } else if (_t182.tag == HirExpr_ELitBytes) {
        __auto_type v = _t182.data.ELitBytes.val;
        return _tr_str_concat(_tr_str_concat("\"", _escape_str_for_c(v)), "\"");
    } else if (_t182.tag == HirExpr_ELitChar) {
        __auto_type v = _t182.data.ELitChar.val;
        /* pass */
        if ((v == 0LL)) {
            /* pass */
            return "'\\0'";
        }
        /* pass */
        if ((v == 10LL)) {
            /* pass */
            return "'\\n'";
        }
        /* pass */
        if ((v == 9LL)) {
            /* pass */
            return "'\\t'";
        }
        /* pass */
        if ((v == 13LL)) {
            /* pass */
            return "'\\r'";
        }
        /* pass */
        if ((v == 92LL)) {
            /* pass */
            return "'\\\\'";
        }
        /* pass */
        if ((v == 39LL)) {
            /* pass */
            return "'\\''";
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("'", _tr_char_to_str(v)), "'");
    } else if (_t182.tag == HirExpr_ELitBool) {
        __auto_type v = _t182.data.ELitBool.val;
        /* pass */
        if (v) {
            /* pass */
            return "true";
        } else {
            /* pass */
            return "false";
        }
    } else if (_t182.tag == HirExpr_ELitNone) {
        __auto_type _ = _t182.data.ELitNone.ty;
        return "NULL";
    } else if (_t182.tag == HirExpr_EIdent) {
        __auto_type n = _t182.data.EIdent.name;
        /* pass */
        if ((_is_c_keyword(n) && _tr_dict_contains(self->decl_vars, n))) {
            /* pass */
            return _tr_str_concat("_tr_v_", n);
        }
        /* pass */
        if (((strcmp((char*)n, (char*)"argv") == 0) && (strcmp((char*)self->cur_func, (char*)"main") == 0))) {
            /* pass */
            return "_tr_main_argv";
        }
        /* pass */
        if ((_tr_dict_contains(self->functions, n) && (!_tr_dict_contains(self->decl_vars, n)))) {
            /* pass */
            return _tr_str_concat("(void*)", n);
        }
        /* pass */
        return n;
    } else if (_t182.tag == HirExpr_EBinOp) {
        __auto_type op = _t182.data.EBinOp.op;
__auto_type l = _t182.data.EBinOp.left;
__auto_type r = _t182.data.EBinOp.right;
        return CGenerator_gen_binop(self, op, l, r);
    } else if (_t182.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t182.data.EUnaryOp.op;
__auto_type expr = _t182.data.EUnaryOp.expr;
        return CGenerator_gen_unary(self, op, expr);
    } else if (_t182.tag == HirExpr_ECall) {
        __auto_type callee = _t182.data.ECall.callee;
__auto_type args = _t182.data.ECall.args;
__auto_type call_ty = _t182.data.ECall.ty;
        return CGenerator_gen_call(self, callee, args, call_ty);
    } else if (_t182.tag == HirExpr_EMethodCall) {
        __auto_type o = _t182.data.EMethodCall.obj;
__auto_type m = _t182.data.EMethodCall.method;
__auto_type a = _t182.data.EMethodCall.args;
__auto_type ty = _t182.data.EMethodCall.ty;
        return CGenerator_gen_method_call(self, o, m, a, ty);
    } else if (_t182.tag == HirExpr_EPropAccess) {
        __auto_type o = _t182.data.EPropAccess.obj;
__auto_type p = _t182.data.EPropAccess.prop;
        return CGenerator_gen_prop_access(self, o, p);
    } else if (_t182.tag == HirExpr_EIndex) {
        __auto_type o = _t182.data.EIndex.obj;
__auto_type idx = _t182.data.EIndex.index;
        return CGenerator_gen_index(self, o, idx);
    } else if (_t182.tag == HirExpr_ESizeOf) {
        __auto_type target_ty = _t182.data.ESizeOf.target_ty;
        /* pass */
        char* sizeof_cty = CGenerator_type_to_c(self, target_ty);
        /* pass */
        long long sizeof_len = _tr_str_len(sizeof_cty);
        /* pass */
        if ((sizeof_len > 0LL)) {
            /* pass */
            if ((((long long)((*(((char*)(sizeof_cty)) + (sizeof_len - 1LL))))) == 42LL)) {
                /* pass */
                sizeof_cty = _tr_str_slice(sizeof_cty, 0LL, (sizeof_len - 1LL));
            }
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("((long long)sizeof(", sizeof_cty), "))");
    } else if (_t182.tag == HirExpr_ECast) {
        __auto_type expr = _t182.data.ECast.expr;
__auto_type target_ty = _t182.data.ECast.target_ty;
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", CGenerator_type_to_c(self, target_ty)), ")("), CGenerator_gen_expr(self, expr)), "))");
    } else if (_t182.tag == HirExpr_EFString) {
        __auto_type parts = _t182.data.EFString.parts;
        return CGenerator_gen_fstring(self, parts);
    } else if (_t182.tag == HirExpr_ETryExpr) {
        __auto_type expr = _t182.data.ETryExpr.expr;
__auto_type ok_ty = _t182.data.ETryExpr.ty;
        /* pass */
        char* inner_s2 = CGenerator_gen_expr(self, expr);
        /* pass */
        char* ok_c = CGenerator_type_to_c(self, ok_ty);
        /* pass */
        if (((strcmp((char*)ok_c, (char*)"void") == 0) || (strcmp((char*)ok_c, (char*)"__auto_type") == 0))) {
            /* pass */
            ok_c = "long long";
        }
        /* pass */
        if ((_is_int_type(ok_ty->name) || (strcmp((char*)ok_ty->name, (char*)"bool") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", ok_c), ")(uintptr_t)(("), inner_s2), ").data.Ok.val))");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", ok_c), "*)(("), inner_s2), ").data.Ok.val))");
    } else if (_t182.tag == HirExpr_ETuple) {
        __auto_type items = _t182.data.ETuple.items;
        return CGenerator_gen_tuple(self, items);
    } else if (_t182.tag == HirExpr_EList) {
        __auto_type items = _t182.data.EList.items;
__auto_type ty = _t182.data.EList.ty;
        return CGenerator_gen_list_literal(self, items, ty);
    } else if (_t182.tag == HirExpr_EDict) {
        __auto_type keys = _t182.data.EDict.keys;
__auto_type vals = _t182.data.EDict.vals;
__auto_type dict_ty = _t182.data.EDict.ty;
        return CGenerator_gen_dict_literal(self, keys, vals, dict_ty);
    } else if (_t182.tag == HirExpr_EClosure) {
        __auto_type params = _t182.data.EClosure.params;
__auto_type ret_ty = _t182.data.EClosure.ret_ty;
__auto_type body = _t182.data.EClosure.body;
__auto_type captures = _t182.data.EClosure.captures;
        /* pass */
        return CGenerator_gen_closure(self, params, ret_ty, body, captures);
    } else if (_t182.tag == HirExpr_ERange) {
        __auto_type start = _t182.data.ERange.start;
__auto_type end = _t182.data.ERange.end;
__auto_type inclusive = _t182.data.ERange.inclusive;
        /* pass */
        return CGenerator_gen_expr(self, start);
    } else if (_t182.tag == HirExpr_ESuperMethodCall) {
        __auto_type base = _t182.data.ESuperMethodCall.base_class;
__auto_type method = _t182.data.ESuperMethodCall.method;
__auto_type args = _t182.data.ESuperMethodCall.args;
        /* pass */
        char* super_mc = method;
        /* pass */
        if (_is_c_keyword(super_mc)) {
            /* pass */
            super_mc = _tr_str_concat("_tr_fn_", super_mc);
        }
        /* pass */
        char* sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(base, "_"), super_mc), "(("), base), "*)self");
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            sig = _tr_str_concat(_tr_str_concat(sig, ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, i))));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _tr_str_concat(sig, ")");
    } else if (_t182.tag == HirExpr_ESuperPropAccess) {
        __auto_type base = _t182.data.ESuperPropAccess.base_class;
__auto_type prop = _t182.data.ESuperPropAccess.prop;
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(((", base), "*)self)->"), prop), ")");
    } else if (_t182.tag == HirExpr_EAwait) {
        __auto_type expr = _t182.data.EAwait.expr;
        /* pass */
        return CGenerator_gen_await_call(self, expr);
    } else if (_t182.tag == HirExpr_EAwaitTimeout) {
        __auto_type at_expr = _t182.data.EAwaitTimeout.expr;
__auto_type at_ms = _t182.data.EAwaitTimeout.timeout_ms;
        /* pass */
        return CGenerator_gen_await_timeout_call(self, at_expr, at_ms);
    } else if (_t182.tag == HirExpr_EYield) {
        __auto_type expr = _t182.data.EYield.expr;
        /* pass */
        return CGenerator_gen_expr(self, expr);
    } else if (_t182.tag == HirExpr_ESet) {
        __auto_type items = _t182.data.ESet.items;
        /* pass */
        AstType* set_ty = AstType_init("ptr");
        /* pass */
        if ((items->len > 0LL)) {
            /* pass */
            char* first_ty_n = hir_expr_type(((HirExpr*)List_ptr_get(items, 0LL)))->name;
            /* pass */
            set_ty = AstType_init(first_ty_n);
        }
        /* pass */
        return CGenerator_gen_list_literal(self, items, set_ty);
    } else if (_t182.tag == HirExpr_EListComp) {
        __auto_type element = _t182.data.EListComp.element;
__auto_type generators = _t182.data.EListComp.generators;
        /* pass */
        return CGenerator_gen_list_comp(self, element, generators);
    } else if (_t182.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t182.data.EGeneratorExpr.element;
__auto_type generators = _t182.data.EGeneratorExpr.generators;
        /* pass */
        return CGenerator_gen_list_comp(self, element, generators);
    } else if (_t182.tag == HirExpr_ESlice) {
        __auto_type start = _t182.data.ESlice.start;
__auto_type stop = _t182.data.ESlice.stop;
__auto_type step = _t182.data.ESlice.step;
        /* pass */
        if ((((unsigned long long)(start)) != ((unsigned long long)(0LL)))) {
            /* pass */
            return CGenerator_gen_expr(self, start);
        }
        /* pass */
        return "0LL";
    } else if (_t182.tag == HirExpr_EIfElse) {
        __auto_type cond = _t182.data.EIfElse.cond;
__auto_type then_e = _t182.data.EIfElse.then_e;
__auto_type else_e = _t182.data.EIfElse.else_e;
__auto_type ty = _t182.data.EIfElse.ty;
        /* pass */
        char* ts = CGenerator_gen_expr(self, then_e);
        /* pass */
        char* es = CGenerator_gen_expr(self, else_e);
        /* pass */
        if (_is_int_type(ty->name)) {
            /* pass */
            if ((strcmp((char*)ts, (char*)"NULL") == 0)) {
                /* pass */
                ts = "0LL";
            }
            /* pass */
            if ((strcmp((char*)es, (char*)"NULL") == 0)) {
                /* pass */
                es = "0LL";
            }
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", CGenerator_gen_expr(self, cond)), ") ? ("), ts), ") : ("), es), "))");
    } else if (1) {
        __auto_type _ = _t182;
        return "0 /* unsupported expr */";
    }
}

__attribute__((hot)) bool CGenerator_has_method(CGenerator* self, char* cls_name, char* method) {
    /* pass */
    if (((strcmp((char*)cls_name, (char*)"") == 0) || (strcmp((char*)cls_name, (char*)"void") == 0))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, cls_name)) {
        /* pass */
        HirClass* cls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, cls_name));
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < cls->methods->len)) {
            /* pass */
            if ((strcmp((char*)((HirFunction*)List_ptr_get(cls->methods, i))->name, (char*)method) == 0)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return false;
    }
    /* pass */
    char* cp = ((char*)(cls_name));
    /* pass */
    long long ui = 0LL;
    /* pass */
    while ((((long long)((*(cp + ui)))) != 0LL)) {
        /* pass */
        if ((((long long)((*(cp + ui)))) == 95LL)) {
            /* pass */
            __auto_type candidate = _tr_str_slice(cls_name, 0LL, ui);
            /* pass */
            if (_tr_dict_contains(self->classes, candidate)) {
                /* pass */
                HirClass* base_cls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, candidate));
                /* pass */
                long long bmi = 0LL;
                /* pass */
                while ((bmi < base_cls->methods->len)) {
                    /* pass */
                    if ((strcmp((char*)((HirFunction*)List_ptr_get(base_cls->methods, bmi))->name, (char*)method) == 0)) {
                        /* pass */
                        return true;
                    }
                    /* pass */
                    bmi = (bmi + 1LL);
                }
                /* pass */
                return false;
            }
        }
        /* pass */
        ui = (ui + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) AstType* CGenerator_cls_method_ret_ty(CGenerator* self, char* cls_name, char* method) {
    /* pass */
    if (((strcmp((char*)cls_name, (char*)"") == 0) || (strcmp((char*)cls_name, (char*)"void") == 0))) {
        /* pass */
        return AstType_init("void");
    }
    /* pass */
    if (_tr_dict_contains(self->classes, cls_name)) {
        /* pass */
        HirClass* cls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, cls_name));
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < cls->methods->len)) {
            /* pass */
            if ((strcmp((char*)((HirFunction*)List_ptr_get(cls->methods, i))->name, (char*)method) == 0)) {
                /* pass */
                return ((HirFunction*)List_ptr_get(cls->methods, i))->ret_ty;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return AstType_init("void");
    }
    /* pass */
    char* cp2 = ((char*)(cls_name));
    /* pass */
    long long ui2 = 0LL;
    /* pass */
    while ((((long long)((*(cp2 + ui2)))) != 0LL)) {
        /* pass */
        if ((((long long)((*(cp2 + ui2)))) == 95LL)) {
            /* pass */
            __auto_type candidate2 = _tr_str_slice(cls_name, 0LL, ui2);
            /* pass */
            if (_tr_dict_contains(self->classes, candidate2)) {
                /* pass */
                HirClass* base_cls2 = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, candidate2));
                /* pass */
                long long i2 = 0LL;
                /* pass */
                while ((i2 < base_cls2->methods->len)) {
                    /* pass */
                    if ((strcmp((char*)((HirFunction*)List_ptr_get(base_cls2->methods, i2))->name, (char*)method) == 0)) {
                        /* pass */
                        return ((HirFunction*)List_ptr_get(base_cls2->methods, i2))->ret_ty;
                    }
                    /* pass */
                    i2 = (i2 + 1LL);
                }
                /* pass */
                return AstType_init("void");
            }
        }
        /* pass */
        ui2 = (ui2 + 1LL);
    }
    /* pass */
    return AstType_init("void");
}

__attribute__((hot)) char* CGenerator_cls_method_c_call(CGenerator* self, char* cls_name, char* method, char* obj_s, char* extra_args) {
    /* pass */
    char* safe_m = method;
    /* pass */
    if (_is_c_keyword(safe_m)) {
        /* pass */
        safe_m = _tr_str_concat("_tr_fn_", safe_m);
    }
    /* pass */
    if ((strcmp((char*)extra_args, (char*)"") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(cls_name, "_"), safe_m), "("), obj_s), ")");
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(cls_name, "_"), safe_m), "("), obj_s), ", "), extra_args), ")");
}

__attribute__((hot)) char* CGenerator_gen_cond_expr(CGenerator* self, HirExpr* cond) {
    /* pass */
    char* cond_s = CGenerator_gen_expr(self, cond);
    /* pass */
    char* ty_n = hir_expr_type(cond)->name;
    /* pass */
    if (CGenerator_has_method(self, ty_n, "__bool__")) {
        /* pass */
        return CGenerator_cls_method_c_call(self, ty_n, "__bool__", cond_s, "");
    }
    /* pass */
    return cond_s;
}

__attribute__((hot)) char* CGenerator_gen_binop(CGenerator* self, char* op, HirExpr* l, HirExpr* r) {
    /* pass */
    char* ls = CGenerator_gen_expr(self, l);
    /* pass */
    char* rs = CGenerator_gen_expr(self, r);
    /* pass */
    char* lt_n = hir_expr_type(l)->name;
    /* pass */
    char* rt_n = hir_expr_type(r)->name;
    /* pass */
    if (((strcmp((char*)op, (char*)"+") == 0) && (_is_str_type(lt_n) || _is_str_type(rt_n)))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_concat(", ls), ", "), rs), ")");
    }
    /* pass */
    if (((strcmp((char*)op, (char*)"*") == 0) && (_is_str_type(lt_n) || _is_str_type(rt_n)))) {
        /* pass */
        if (_is_str_type(lt_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_repeat(", ls), ", (long long)("), rs), "))");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_repeat(", rs), ", (long long)("), ls), "))");
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"and") == 0)) {
        /* pass */
        char* lsa = ls;
        /* pass */
        char* rsa = rs;
        /* pass */
        if (CGenerator_has_method(self, lt_n, "__bool__")) {
            /* pass */
            lsa = CGenerator_cls_method_c_call(self, lt_n, "__bool__", ls, "");
        }
        /* pass */
        if (CGenerator_has_method(self, rt_n, "__bool__")) {
            /* pass */
            rsa = CGenerator_cls_method_c_call(self, rt_n, "__bool__", rs, "");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", lsa), " && "), rsa), ")");
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"or") == 0)) {
        /* pass */
        char* lso = ls;
        /* pass */
        char* rso = rs;
        /* pass */
        if (CGenerator_has_method(self, lt_n, "__bool__")) {
            /* pass */
            lso = CGenerator_cls_method_c_call(self, lt_n, "__bool__", ls, "");
        }
        /* pass */
        if (CGenerator_has_method(self, rt_n, "__bool__")) {
            /* pass */
            rso = CGenerator_cls_method_c_call(self, rt_n, "__bool__", rs, "");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", lso), " || "), rso), ")");
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"is") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ls), " == "), rs), ")");
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"in") == 0)) {
        /* pass */
        if (((strcmp((char*)rt_n, (char*)"List") == 0) || (strcmp((char*)rt_n, (char*)"Vec") == 0))) {
            /* pass */
            char* in_sfx = CGenerator_list_elem_suffix(self, lt_n);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", in_sfx), "_contains("), rs), ", "), ls), ")");
        }
        /* pass */
        if (((strcmp((char*)rt_n, (char*)"Map") == 0) || (strcmp((char*)rt_n, (char*)"Dict") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_contains(", rs), ", "), ls), ")");
        }
        /* pass */
        if (CGenerator_has_method(self, rt_n, "__contains__")) {
            /* pass */
            return CGenerator_cls_method_c_call(self, rt_n, "__contains__", rs, ls);
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_contains(", rs), ", "), ls), ")");
    }
    /* pass */
    char* dunder = "";
    /* pass */
    if ((strcmp((char*)op, (char*)"+") == 0)) {
        /* pass */
        dunder = "__add__";
    } else if ((strcmp((char*)op, (char*)"-") == 0)) {
        /* pass */
        dunder = "__sub__";
    } else if ((strcmp((char*)op, (char*)"*") == 0)) {
        /* pass */
        dunder = "__mul__";
    } else if ((strcmp((char*)op, (char*)"/") == 0)) {
        /* pass */
        dunder = "__truediv__";
    } else if ((strcmp((char*)op, (char*)"%") == 0)) {
        /* pass */
        dunder = "__mod__";
    } else if ((strcmp((char*)op, (char*)"**") == 0)) {
        /* pass */
        dunder = "__pow__";
    } else if ((strcmp((char*)op, (char*)"//") == 0)) {
        /* pass */
        dunder = "__floordiv__";
    } else if ((strcmp((char*)op, (char*)"==") == 0)) {
        /* pass */
        dunder = "__eq__";
    } else if ((strcmp((char*)op, (char*)"!=") == 0)) {
        /* pass */
        dunder = "__ne__";
    } else if ((strcmp((char*)op, (char*)"<") == 0)) {
        /* pass */
        dunder = "__lt__";
    } else if ((strcmp((char*)op, (char*)"<=") == 0)) {
        /* pass */
        dunder = "__le__";
    } else if ((strcmp((char*)op, (char*)">") == 0)) {
        /* pass */
        dunder = "__gt__";
    } else if ((strcmp((char*)op, (char*)">=") == 0)) {
        /* pass */
        dunder = "__ge__";
    }
    /* pass */
    if (((strcmp((char*)dunder, (char*)"") != 0) && CGenerator_has_method(self, lt_n, dunder))) {
        /* pass */
        return CGenerator_cls_method_c_call(self, lt_n, dunder, ls, rs);
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"**") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((long long)pow((double)(", ls), "), (double)("), rs), ")))");
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"//") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((long long)(", ls), ") / (long long)("), rs), "))");
    }
    /* pass */
    if (((_is_str_type(lt_n) || _is_str_type(rt_n)) && ((strcmp((char*)op, (char*)"==") == 0) || (strcmp((char*)op, (char*)"!=") == 0)))) {
        /* pass */
        if ((strcmp((char*)op, (char*)"==") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(strcmp((char*)", ls), ", (char*)"), rs), ") == 0)");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(strcmp((char*)", ls), ", (char*)"), rs), ") != 0)");
    }
    /* pass */
    if ((_tr_dict_contains(self->enums, lt_n) && ((strcmp((char*)op, (char*)"==") == 0) || (strcmp((char*)op, (char*)"!=") == 0)))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ls), ".tag "), op), " "), rs), ".tag)");
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ls), " "), op), " "), rs), ")");
}

__attribute__((hot)) char* CGenerator_gen_unary(CGenerator* self, char* op, HirExpr* expr) {
    /* pass */
    char* inner = CGenerator_gen_expr(self, expr);
    /* pass */
    char* ty_n = hir_expr_type(expr)->name;
    /* pass */
    if (((strcmp((char*)op, (char*)"not") == 0) || (strcmp((char*)op, (char*)"!") == 0))) {
        /* pass */
        if (CGenerator_has_method(self, ty_n, "__bool__")) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(!(", CGenerator_cls_method_c_call(self, ty_n, "__bool__", inner, "")), "))");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("(!", inner), ")");
    }
    /* pass */
    if (((strcmp((char*)op, (char*)"-") == 0) && CGenerator_has_method(self, ty_n, "__neg__"))) {
        /* pass */
        return CGenerator_cls_method_c_call(self, ty_n, "__neg__", inner, "");
    }
    /* pass */
    if (((strcmp((char*)op, (char*)"+") == 0) && CGenerator_has_method(self, ty_n, "__pos__"))) {
        /* pass */
        return CGenerator_cls_method_c_call(self, ty_n, "__pos__", inner, "");
    }
    /* pass */
    if (((strcmp((char*)op, (char*)"*") == 0) || (strcmp((char*)op, (char*)"deref") == 0))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("(*", inner), ")");
    }
    /* pass */
    if (((strcmp((char*)op, (char*)"&") == 0) || (strcmp((char*)op, (char*)"ref") == 0))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("(&", inner), ")");
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat("(", op), inner), ")");
}

__attribute__((hot)) char* CGenerator_gen_prop_access(CGenerator* self, HirExpr* o, char* p) {
    /* pass */
    char* t_n = hir_expr_type(o)->name;
    /* pass */
    char* obj_s = CGenerator_gen_expr(self, o);
    /* pass */
    if (_tr_dict_contains(self->shared_vars, obj_s)) {
        /* pass */
        char* sh_ty = ((char*)(uintptr_t)_tr_dict_get(self->shared_vars, obj_s));
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", sh_ty), "*)("), obj_s), "->data))->"), p);
    }
    /* pass */
    if (_tr_dict_contains(self->enums, t_n)) {
        /* pass */
        if ((strcmp((char*)obj_s, (char*)t_n) == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(t_n, "_make_"), p), "()");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(obj_s, ".data."), p);
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"Pointer") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat("(*", obj_s), ")."), p);
    }
    /* pass */
    if ((_is_str_type(t_n) && ((strcmp((char*)p, (char*)"len") == 0) || (strcmp((char*)p, (char*)"length") == 0)))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("((long long)_tr_strlen(", obj_s), "))");
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"StringBuilder") == 0) && ((strcmp((char*)p, (char*)"len") == 0) || (strcmp((char*)p, (char*)"length") == 0)))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("(", obj_s), "->buf->len)");
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"Result") == 0)) {
        /* pass */
        if ((strcmp((char*)p, (char*)"is_err") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Result_Err)");
        }
        /* pass */
        if ((strcmp((char*)p, (char*)"is_ok") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Result_Ok)");
        }
        /* pass */
        if ((strcmp((char*)p, (char*)"ok") == 0)) {
            /* pass */
            AstType* full_ty = hir_expr_type(o);
            /* pass */
            if ((full_ty->args->len > 0LL)) {
                /* pass */
                char* ok_ct = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(full_ty->args, 0LL))));
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", ok_ct), ")Result_unwrap("), obj_s), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("Result_unwrap(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)p, (char*)"err") == 0)) {
            /* pass */
            AstType* full_ty2 = hir_expr_type(o);
            /* pass */
            if ((full_ty2->args->len > 1LL)) {
                /* pass */
                char* err_ct = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(full_ty2->args, 1LL))));
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", err_ct), ")Result_unwrap_err("), obj_s), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(char*)Result_unwrap_err(", obj_s), ")");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(obj_s, "."), p);
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"Option") == 0)) {
        /* pass */
        if ((strcmp((char*)p, (char*)"is_some") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Option_Some)");
        }
        /* pass */
        if ((strcmp((char*)p, (char*)"is_none") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Option_None)");
        }
        /* pass */
        if (((strcmp((char*)p, (char*)"value") == 0) || (strcmp((char*)p, (char*)"val") == 0))) {
            /* pass */
            AstType* full_opt_ty = hir_expr_type(o);
            /* pass */
            if ((full_opt_ty->args->len > 0LL)) {
                /* pass */
                char* opt_ct = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(full_opt_ty->args, 0LL))));
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", opt_ct), ")Option_unwrap("), obj_s), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("Option_unwrap(", obj_s), ")");
        }
        /* pass */
        if ((((strcmp((char*)p, (char*)"") == 0) || (strcmp((char*)p, (char*)"None") == 0)) || (strcmp((char*)p, (char*)"none") == 0))) {
            /* pass */
            return "((Option){.tag=Option_None})";
        }
        /* pass */
        if (((strcmp((char*)p, (char*)"Some") == 0) || (strcmp((char*)p, (char*)"some") == 0))) {
            /* pass */
            return "((Option){.tag=Option_Some})";
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(obj_s, "."), p);
    }
    /* pass */
    if ((_tr_dict_contains(self->interfaces, t_n) && (((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(obj_s, "."), p);
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(obj_s, "->"), p);
}

__attribute__((hot)) char* CGenerator_gen_index(CGenerator* self, HirExpr* o, HirExpr* idx) {
    /* pass */
    char* os = CGenerator_gen_expr(self, o);
    /* pass */
    char* is_idx = CGenerator_gen_expr(self, idx);
    /* pass */
    char* ty_n = hir_expr_type(o)->name;
    /* pass */
    if ((strcmp((char*)ty_n, (char*)"Pointer") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(os, "["), is_idx), "]");
    }
    /* pass */
    if (((strcmp((char*)ty_n, (char*)"List") == 0) || (strcmp((char*)ty_n, (char*)"Vec") == 0))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(os, "->data[(_tr_bounds_check("), is_idx), ", "), os), "->len), "), is_idx), ")]");
    }
    /* pass */
    if (((((_tr_dict_contains(self->classes, ty_n) && (strcmp((char*)ty_n, (char*)"Vec") != 0)) && (strcmp((char*)ty_n, (char*)"List") != 0)) && (strcmp((char*)ty_n, (char*)"Map") != 0)) && (strcmp((char*)ty_n, (char*)"Dict") != 0))) {
        /* pass */
        if (CGenerator_has_method(self, ty_n, "__getitem__")) {
            /* pass */
            return CGenerator_cls_method_c_call(self, ty_n, "__getitem__", os, is_idx);
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(ty_n, "_get_index("), os), ", "), is_idx), ")");
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(os, "["), is_idx), "]");
}

__attribute__((hot)) char* CGenerator_gen_call(CGenerator* self, HirExpr* callee, List_ptr* args, AstType* call_ty) {
    /* pass */
    char* callee_s = CGenerator_gen_expr(self, callee);
    /* pass */
    char* base_callee = callee_s;
    /* pass */
    bool has_generic = false;
    /* pass */
    AstType* generic_ty = AstType_init("void");
    /* pass */
    __auto_type _t183 = (*callee);
    if (_t183.tag == HirExpr_EPropAccess) {
        __auto_type inner_obj = _t183.data.EPropAccess.obj;
__auto_type variant = _t183.data.EPropAccess.prop;
        /* pass */
        if (((strcmp((char*)variant, (char*)"init") == 0) || (strcmp((char*)variant, (char*)"new") == 0))) {
            /* pass */
            __auto_type _t184 = (*inner_obj);
            if (_t184.tag == HirExpr_EIndex) {
                __auto_type base_expr = _t184.data.EIndex.obj;
__auto_type idx_expr = _t184.data.EIndex.index;
                /* pass */
                __auto_type _t185 = (*base_expr);
                if (_t185.tag == HirExpr_EIdent) {
                    __auto_type gn = _t185.data.EIdent.name;
                    /* pass */
                    if (((strcmp((char*)gn, (char*)"Vec") == 0) || (strcmp((char*)gn, (char*)"List") == 0))) {
                        /* pass */
                        __auto_type _t186 = (*idx_expr);
                        if (_t186.tag == HirExpr_EIdent) {
                            __auto_type elem_n = _t186.data.EIdent.name;
                            /* pass */
                            char* sfx = CGenerator_list_elem_suffix(self, elem_n);
                            /* pass */
                            return _tr_str_concat(_tr_str_concat("(void*)List_", sfx), "_new()");
                        } else if (1) {
                            __auto_type _ = _t186;
                            /* pass */
                        }
                    }
                } else if (1) {
                    __auto_type _ = _t185;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t184;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t183;
        /* pass */
    }
    /* pass */
    __auto_type _t187 = (*callee);
    if (_t187.tag == HirExpr_EIdent) {
        __auto_type raw_n = _t187.data.EIdent.name;
        /* pass */
        base_callee = raw_n;
    } else if (_t187.tag == HirExpr_EIndex) {
        __auto_type base_expr = _t187.data.EIndex.obj;
__auto_type idx_expr = _t187.data.EIndex.index;
__auto_type idx_ty = _t187.data.EIndex.ty;
        /* pass */
        __auto_type _t188 = (*base_expr);
        if (_t188.tag == HirExpr_EIdent) {
            __auto_type n = _t188.data.EIdent.name;
            /* pass */
            if (((((((((strcmp((char*)n, (char*)"alloc") == 0) || (strcmp((char*)n, (char*)"core_alloc_alloc") == 0)) || (strcmp((char*)n, (char*)"dealloc") == 0)) || (strcmp((char*)n, (char*)"Pointer") == 0)) || (strcmp((char*)n, (char*)"resize") == 0)) || (strcmp((char*)n, (char*)"core_alloc_resize") == 0)) || (strcmp((char*)n, (char*)"copy") == 0)) || (strcmp((char*)n, (char*)"core_alloc_copy") == 0))) {
                /* pass */
                base_callee = n;
                /* pass */
                has_generic = true;
                /* pass */
                __auto_type _t189 = (*idx_expr);
                if (_t189.tag == HirExpr_EIdent) {
                    __auto_type tname = _t189.data.EIdent.name;
                    /* pass */
                    generic_ty = AstType_init(tname);
                } else if (1) {
                    __auto_type _ = _t189;
                    /* pass */
                }
            }
        } else if (1) {
            __auto_type _ = _t188;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t187;
        /* pass */
    }
    /* pass */
    if (_is_c_keyword(base_callee)) {
        /* pass */
        base_callee = _tr_str_concat("_tr_fn_", base_callee);
    }
    /* pass */
    __auto_type _t190 = (*callee);
    if (_t190.tag == HirExpr_EIdent) {
        __auto_type _dc_n = _t190.data.EIdent.name;
        /* pass */
        if (_tr_dict_contains(self->functions, _dc_n)) {
            /* pass */
            callee_s = base_callee;
        }
    } else if (1) {
        __auto_type _ = _t190;
        /* pass */
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"print") == 0)) {
        /* pass */
        return CGenerator_gen_print_call(self, args);
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"len") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            HirExpr* arg0 = ((HirExpr*)List_ptr_get(args, 0LL));
            /* pass */
            char* arg0_ty_n = hir_expr_type(arg0)->name;
            /* pass */
            if (_is_str_type(arg0_ty_n)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_strlen(", CGenerator_gen_expr(self, arg0)), ")");
            }
            /* pass */
            if (CGenerator_has_method(self, arg0_ty_n, "__len__")) {
                /* pass */
                return CGenerator_cls_method_c_call(self, arg0_ty_n, "__len__", CGenerator_gen_expr(self, arg0), "");
            }
            /* pass */
            return _tr_str_concat(CGenerator_gen_expr(self, arg0), "->len");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"range") == 0)) {
        /* pass */
        if ((args->len == 1LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_range(0, ", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", 1)");
        } else if ((args->len == 2LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_range(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ", 1)");
        } else if ((args->len == 3LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_range(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 2LL)))), ")");
        }
        /* pass */
        return "_tr_range(0, 0, 1)";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"_tr_exit") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("exit((int)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "exit(0)";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"ord") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* ord_t_n = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
            /* pass */
            if ((strcmp((char*)ord_t_n, (char*)"char") == 0)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("((long long)(unsigned char)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("((long long)(unsigned char)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")[0])");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"chr") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((char)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "'\\0'";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"abs") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* abs_a = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* abs_t_n = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
            /* pass */
            if (_is_float_type(abs_t_n)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("fabs((double)(", abs_a), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("llabs((long long)(", abs_a), "))");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"max") == 0)) {
        /* pass */
        if ((args->len == 2LL)) {
            /* pass */
            char* ma = CGenerator_next_temp(self);
            /* pass */
            char* mb = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ __auto_type ", ma), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "; __auto_type "), mb), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), "; "), ma), " > "), mb), " ? "), ma), " : "), mb), "; })");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"min") == 0)) {
        /* pass */
        if ((args->len == 2LL)) {
            /* pass */
            char* mna = CGenerator_next_temp(self);
            /* pass */
            char* mnb = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ __auto_type ", mna), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "; __auto_type "), mnb), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), "; "), mna), " < "), mnb), " ? "), mna), " : "), mnb), "; })");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if (((((((strcmp((char*)base_callee, (char*)"assert_eq") == 0) || (strcmp((char*)base_callee, (char*)"assert_ne") == 0)) || (strcmp((char*)base_callee, (char*)"assert_lt") == 0)) || (strcmp((char*)base_callee, (char*)"assert_le") == 0)) || (strcmp((char*)base_callee, (char*)"assert_gt") == 0)) || (strcmp((char*)base_callee, (char*)"assert_ge") == 0))) {
        /* pass */
        if ((args->len >= 2LL)) {
            /* pass */
            char* _aa = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _ab = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            char* _ta = CGenerator_next_temp(self);
            /* pass */
            char* _tb = CGenerator_next_temp(self);
            /* pass */
            char* _op = "==";
            /* pass */
            char* _op_msg = "expected equal";
            /* pass */
            if ((strcmp((char*)base_callee, (char*)"assert_ne") == 0)) {
                /* pass */
                _op = "!=";
                /* pass */
                _op_msg = "expected not-equal";
            } else if ((strcmp((char*)base_callee, (char*)"assert_lt") == 0)) {
                /* pass */
                _op = "<";
                /* pass */
                _op_msg = "expected less-than";
            } else if ((strcmp((char*)base_callee, (char*)"assert_le") == 0)) {
                /* pass */
                _op = "<=";
                /* pass */
                _op_msg = "expected less-or-equal";
            } else if ((strcmp((char*)base_callee, (char*)"assert_gt") == 0)) {
                /* pass */
                _op = ">";
                /* pass */
                _op_msg = "expected greater-than";
            } else if ((strcmp((char*)base_callee, (char*)"assert_ge") == 0)) {
                /* pass */
                _op = ">=";
                /* pass */
                _op_msg = "expected greater-or-equal";
            }
            /* pass */
            char* _ty_a = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
            /* pass */
            char* _cvt = "(long long)";
            /* pass */
            if (_is_float_type(_ty_a)) {
                /* pass */
                _cvt = "(double)";
            } else if ((!_is_int_type(_ty_a))) {
                /* pass */
                _cvt = "(long long)(uintptr_t)";
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ __auto_type ", _ta), " = "), _aa), "; __auto_type "), _tb), " = "), _ab), "; if (!("), _ta), " "), _op), " "), _tb), ")) { fprintf(stderr, \"[assert_"), _op), "] "), _op_msg), ": lhs=%lld rhs=%lld\\n\", (long long)"), _ta), ", (long long)"), _tb), "); abort(); } })");
        }
        /* pass */
        return "(void)0";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"checked_add") == 0) || (strcmp((char*)base_callee, (char*)"checked_mul") == 0))) {
        /* pass */
        if ((args->len == 2LL)) {
            /* pass */
            char* _ca = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _cb = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            char* _tres = CGenerator_next_temp(self);
            /* pass */
            char* _op_c = "+";
            /* pass */
            if ((strcmp((char*)base_callee, (char*)"checked_mul") == 0)) {
                /* pass */
                _op_c = "*";
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _tres), "; _Bool _ov = __builtin_add_overflow((long long)("), _ca), "), (long long)("), _cb), "), &"), _tres), "); _ov ? ((Option){.tag=Option_None}) : ((Option){.tag=Option_Some,.data.Some.val=(void*)(uintptr_t)"), _tres), "}); })");
        }
        /* pass */
        return "((Option){.tag=Option_None})";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"checked_sub") == 0)) {
        /* pass */
        if ((args->len == 2LL)) {
            /* pass */
            char* _ca2 = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _cb2 = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            char* _tres2 = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _tres2), "; _Bool _ov = __builtin_sub_overflow((long long)("), _ca2), "), (long long)("), _cb2), "), &"), _tres2), "); _ov ? ((Option){.tag=Option_None}) : ((Option){.tag=Option_Some,.data.Some.val=(void*)(uintptr_t)"), _tres2), "}); })");
        }
        /* pass */
        return "((Option){.tag=Option_None})";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"round") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(double)round((double)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "0.0";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"str") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            HirExpr* str_arg = ((HirExpr*)List_ptr_get(args, 0LL));
            /* pass */
            char* str_t_n = hir_expr_type(str_arg)->name;
            /* pass */
            if (_is_int_type(str_t_n)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_int_to_str((long long)(", CGenerator_gen_expr(self, str_arg)), "))");
            }
            /* pass */
            if (_is_float_type(str_t_n)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_float_to_str((double)(", CGenerator_gen_expr(self, str_arg)), "))");
            }
            /* pass */
            if ((strcmp((char*)str_t_n, (char*)"bool") == 0)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("((", CGenerator_gen_expr(self, str_arg)), ") ? \"true\" : \"false\")");
            }
            /* pass */
            if (CGenerator_has_method(self, str_t_n, "__str__")) {
                /* pass */
                return CGenerator_cls_method_c_call(self, str_t_n, "__str__", CGenerator_gen_expr(self, str_arg), "");
            }
            /* pass */
            if (CGenerator_has_method(self, str_t_n, "__repr__")) {
                /* pass */
                return CGenerator_cls_method_c_call(self, str_t_n, "__repr__", CGenerator_gen_expr(self, str_arg), "");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(char*)(", CGenerator_gen_expr(self, str_arg)), ")");
        }
        /* pass */
        return "\"\"";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"repr") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            HirExpr* repr_arg = ((HirExpr*)List_ptr_get(args, 0LL));
            /* pass */
            char* repr_t_n = hir_expr_type(repr_arg)->name;
            /* pass */
            if (CGenerator_has_method(self, repr_t_n, "__repr__")) {
                /* pass */
                return CGenerator_cls_method_c_call(self, repr_t_n, "__repr__", CGenerator_gen_expr(self, repr_arg), "");
            }
            /* pass */
            if (CGenerator_has_method(self, repr_t_n, "__str__")) {
                /* pass */
                return CGenerator_cls_method_c_call(self, repr_t_n, "__str__", CGenerator_gen_expr(self, repr_arg), "");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(char*)(", CGenerator_gen_expr(self, repr_arg)), ")");
        }
        /* pass */
        return "\"\"";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"int") == 0) || (strcmp((char*)base_callee, (char*)"_tr_fn_int") == 0))) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            if (_is_str_type(hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_str_to_int(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"float") == 0) || (strcmp((char*)base_callee, (char*)"_tr_fn_float") == 0))) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            if (_is_str_type(hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_str_to_float(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(double)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        return "0.0";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"bool") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(_Bool)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        return "0";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"type") == 0)) {
        /* pass */
        return "\"<object>\"";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"await_all") == 0)) {
        /* pass */
        if ((args->len == 0LL)) {
            /* pass */
            return "0LL";
        }
        /* pass */
        char* aa_code = "({ _tr_tg_begin(); ";
        /* pass */
        long long aa_i = 0LL;
        /* pass */
        while ((aa_i < args->len)) {
            /* pass */
            char* aa_fn = "";
            /* pass */
            List_ptr* aa_call_args = ({ List_ptr* _l__t191 = List_ptr_new(); _l__t191; });
            /* pass */
            __auto_type _t192 = (*((HirExpr*)List_ptr_get(args, aa_i)));
            if (_t192.tag == HirExpr_ECall) {
                __auto_type aa_c = _t192.data.ECall.callee;
__auto_type aa_a = _t192.data.ECall.args;
                /* pass */
                __auto_type _t193 = (*aa_c);
                if (_t193.tag == HirExpr_EIdent) {
                    __auto_type aa_n = _t193.data.EIdent.name;
                    aa_fn = aa_n;
                } else if (1) {
                    __auto_type _ = _t193;
                    /* pass */
                }
                /* pass */
                aa_call_args = aa_a;
            } else if (1) {
                __auto_type _ = _t192;
                /* pass */
            }
            /* pass */
            if ((strcmp((char*)aa_fn, (char*)"") != 0)) {
                /* pass */
                if ((aa_call_args->len == 0LL)) {
                    /* pass */
                    aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "_tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), aa_fn), ", NULL)); ");
                } else if ((aa_call_args->len == 1LL)) {
                    /* pass */
                    char* aa_aty = hir_expr_type(((HirExpr*)List_ptr_get(aa_call_args, 0LL)))->name;
                    /* pass */
                    char* aa_vcast = _tr_str_concat(_tr_str_concat("(void*)(uintptr_t)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(aa_call_args, 0LL)))), ")");
                    /* pass */
                    if ((((!_is_int_type(aa_aty)) && (strcmp((char*)aa_aty, (char*)"bool") != 0)) && (strcmp((char*)aa_aty, (char*)"char") != 0))) {
                        /* pass */
                        aa_vcast = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(aa_call_args, 0LL)))), ")");
                    }
                    /* pass */
                    aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "_tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), aa_fn), ", "), aa_vcast), ")); ");
                } else {
                    /* pass */
                    long long aa_na = aa_call_args->len;
                    /* pass */
                    aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "{ long long* _aab"), _tr_int_to_str((long long)(aa_i))), " = (long long*)malloc("), _tr_int_to_str((long long)((aa_na + 1LL)))), " * sizeof(long long)); ");
                    /* pass */
                    long long aa_ai2 = 0LL;
                    /* pass */
                    while ((aa_ai2 < aa_na)) {
                        /* pass */
                        char* aa_atn = hir_expr_type(((HirExpr*)List_ptr_get(aa_call_args, aa_ai2)))->name;
                        /* pass */
                        if ((((!_is_int_type(aa_atn)) && (strcmp((char*)aa_atn, (char*)"bool") != 0)) && (strcmp((char*)aa_atn, (char*)"char") != 0))) {
                            /* pass */
                            aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "_aab"), _tr_int_to_str((long long)(aa_i))), "["), _tr_int_to_str((long long)((aa_ai2 + 1LL)))), "] = (long long)"), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(aa_call_args, aa_ai2)))), "; ");
                        } else {
                            /* pass */
                            aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "_aab"), _tr_int_to_str((long long)(aa_i))), "["), _tr_int_to_str((long long)((aa_ai2 + 1LL)))), "] = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(aa_call_args, aa_ai2)))), "); ");
                        }
                        /* pass */
                        aa_ai2 = (aa_ai2 + 1LL);
                    }
                    /* pass */
                    aa_code = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(aa_code, "_tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), aa_fn), ", _aab"), _tr_int_to_str((long long)(aa_i))), ")); } ");
                }
            } else {
                /* pass */
                aa_code = _tr_str_concat(_tr_str_concat(aa_code, CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, aa_i)))), "; ");
            }
            /* pass */
            aa_i = (aa_i + 1LL);
        }
        /* pass */
        aa_code = _tr_str_concat(aa_code, "_tr_taskgroup_wait(); 0LL; })");
        /* pass */
        return aa_code;
    }
    /* pass */
    if (((((strcmp((char*)base_callee, (char*)"iter") == 0) || (strcmp((char*)base_callee, (char*)"enumerate") == 0)) || (strcmp((char*)base_callee, (char*)"sorted") == 0)) || (strcmp((char*)base_callee, (char*)"reversed") == 0))) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
        }
        /* pass */
        return "NULL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"sum") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* sum_tmp = CGenerator_next_temp(self);
            /* pass */
            char* sum_l = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", sum_tmp), " = 0; __auto_type "), sum_l), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "; for (long long _si = 0; _si < "), sum_l), "->len; _si++) "), sum_tmp), " += (long long)"), sum_l), "->data[_si]; "), sum_tmp), "; })");
        }
        /* pass */
        return "0LL";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"any") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* any_l = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ _Bool _any_r = 0; __auto_type ", any_l), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "; for (long long _ai = 0; _ai < "), any_l), "->len; _ai++) if ("), any_l), "->data[_ai]) { _any_r = 1; break; } _any_r; })");
        }
        /* pass */
        return "0";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"all") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* all_l = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ _Bool _all_r = 1; __auto_type ", all_l), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "; for (long long _ai = 0; _ai < "), all_l), "->len; _ai++) if (!"), all_l), "->data[_ai]) { _all_r = 0; break; } _all_r; })");
        }
        /* pass */
        return "1";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"Some") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_option_some((void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "_tr_option_none()";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"Ok") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_result_ok((void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "_tr_result_ok(NULL)";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"Err") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_result_err((void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "_tr_result_err(NULL)";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"alloc") == 0) || (strcmp((char*)base_callee, (char*)"core_alloc_alloc") == 0))) {
        /* pass */
        char* count_s = "1LL";
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            count_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
        }
        /* pass */
        char* alloc_ty = "void";
        /* pass */
        if ((has_generic && (strcmp((char*)generic_ty->name, (char*)"void") != 0))) {
            /* pass */
            alloc_ty = CGenerator_type_to_c(self, generic_ty);
        } else if (((strcmp((char*)call_ty->name, (char*)"Pointer") == 0) && (call_ty->args->len > 0LL))) {
            /* pass */
            alloc_ty = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(call_ty->args, 0LL))));
        } else {
            /* pass */
            __auto_type _t194 = (*callee);
            if (_t194.tag == HirExpr_EIdent) {
                __auto_type ident_ty = _t194.data.EIdent.ty;
                /* pass */
                if ((ident_ty->args->len > 0LL)) {
                    /* pass */
                    alloc_ty = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(ident_ty->args, 0LL))));
                }
            } else if (1) {
                __auto_type _ = _t194;
                /* pass */
            }
        }
        /* pass */
        if (((strcmp((char*)alloc_ty, (char*)"void") == 0) || (strcmp((char*)alloc_ty, (char*)"void*") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("malloc((size_t)(", count_s), "))");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", alloc_ty), "*)_tr_c_calloc((size_t)("), count_s), "), sizeof("), alloc_ty), ")))");
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"dealloc") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_free((void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        return "";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"resize") == 0) || (strcmp((char*)base_callee, (char*)"core_alloc_resize") == 0))) {
        /* pass */
        if ((args->len >= 2LL)) {
            /* pass */
            char* rptr_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* rcnt_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            char* resize_ty = "void";
            /* pass */
            if ((has_generic && (strcmp((char*)generic_ty->name, (char*)"void") != 0))) {
                /* pass */
                resize_ty = CGenerator_type_to_c(self, generic_ty);
            }
            /* pass */
            if (((strcmp((char*)resize_ty, (char*)"void") == 0) || (strcmp((char*)resize_ty, (char*)"void*") == 0))) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(void*)_tr_c_realloc((void*)(", rptr_s), "), (size_t)("), rcnt_s), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", resize_ty), "*)_tr_c_realloc((void*)("), rptr_s), "), (size_t)("), rcnt_s), ") * sizeof("), resize_ty), ")))");
        }
        /* pass */
        return "NULL";
    }
    /* pass */
    if (((strcmp((char*)base_callee, (char*)"copy") == 0) || (strcmp((char*)base_callee, (char*)"core_alloc_copy") == 0))) {
        /* pass */
        if ((args->len >= 3LL)) {
            /* pass */
            char* cdst_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* csrc_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            char* ccnt_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 2LL)));
            /* pass */
            char* copy_ty = "void";
            /* pass */
            if ((has_generic && (strcmp((char*)generic_ty->name, (char*)"void") != 0))) {
                /* pass */
                copy_ty = CGenerator_type_to_c(self, generic_ty);
            }
            /* pass */
            if (((strcmp((char*)copy_ty, (char*)"void") == 0) || (strcmp((char*)copy_ty, (char*)"void*") == 0))) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_c_memcpy((void*)(", cdst_s), "), (void*)("), csrc_s), "), (size_t)("), ccnt_s), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_c_memcpy((void*)(", cdst_s), "), (void*)("), csrc_s), "), (size_t)("), ccnt_s), ") * sizeof("), copy_ty), "))");
        }
        /* pass */
        return "";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"input") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_input(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        return "_tr_input(NULL)";
    }
    /* pass */
    if ((strcmp((char*)base_callee, (char*)"Pointer") == 0)) {
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            char* ptr_arg = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((has_generic && (strcmp((char*)generic_ty->name, (char*)"void") != 0))) {
                /* pass */
                char* ct = CGenerator_type_to_c(self, generic_ty);
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ct), "*)("), ptr_arg), ")");
            }
            /* pass */
            __auto_type _t195 = (*callee);
            if (_t195.tag == HirExpr_EIdent) {
                __auto_type ident_ty = _t195.data.EIdent.ty;
                /* pass */
                if ((ident_ty->args->len > 0LL)) {
                    /* pass */
                    AstType* elem = (*((AstType**)List_ptr_get(ident_ty->args, 0LL)));
                    /* pass */
                    char* ct = CGenerator_type_to_c(self, elem);
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ct), "*)("), ptr_arg), ")");
                }
            } else if (1) {
                __auto_type _ = _t195;
                /* pass */
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(void*)(", ptr_arg), ")");
        }
        /* pass */
        return "NULL";
    }
    /* pass */
    __auto_type _t196 = (*callee);
    if (_t196.tag == HirExpr_EIdent) {
        __auto_type n = _t196.data.EIdent.name;
__auto_type ident_ty = _t196.data.EIdent.ty;
        /* pass */
        if (((((_tr_dict_contains(self->classes, n) && (strcmp((char*)n, (char*)"Vec") != 0)) && (strcmp((char*)n, (char*)"Map") != 0)) && (strcmp((char*)n, (char*)"Dict") != 0)) && (strcmp((char*)n, (char*)"List") != 0))) {
            /* pass */
            HirClass* ucls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, n));
            /* pass */
            if (((ucls->generics->len > 0LL) && (ident_ty->args->len > 0LL))) {
                /* pass */
                char* sfx = CGenerator_type_args_suffix(self, ident_ty->args);
                /* pass */
                CGenerator_ensure_mono(self, ucls, ident_ty->args);
                /* pass */
                char* mn = _tr_str_concat(_tr_str_concat(n, "_"), sfx);
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", mn), "*)_tr_checked_alloc(sizeof("), mn), ")))");
            }
            /* pass */
            if ((ucls->generics->len > 0LL)) {
                /* pass */
                char* sfx2 = CGenerator_synth_class_suffix(self, ucls);
                /* pass */
                if ((strcmp((char*)sfx2, (char*)"") != 0)) {
                    /* pass */
                    char* mn2 = _tr_str_concat(_tr_str_concat(n, "_"), sfx2);
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", mn2), "*)_tr_checked_alloc(sizeof("), mn2), ")))");
                }
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", n), "*)_tr_checked_alloc(sizeof("), n), ")))");
        }
        /* pass */
        if (_tr_dict_contains(self->enums, n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", n), "){.tag="), n), "_"), n), "}");
        }
    } else if (_t196.tag == HirExpr_EPropAccess) {
        __auto_type obj_inner = _t196.data.EPropAccess.obj;
__auto_type variant = _t196.data.EPropAccess.prop;
        /* pass */
        char* oi_ctor_name = "";
        /* pass */
        __auto_type _t197 = (*obj_inner);
        if (_t197.tag == HirExpr_EIdent) {
            __auto_type oin = _t197.data.EIdent.name;
            oi_ctor_name = oin;
        } else if (1) {
            __auto_type _ = _t197;
            /* pass */
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"Chan") == 0)) {
            /* pass */
            if ((strcmp((char*)variant, (char*)"init") == 0)) {
                /* pass */
                char* ch_cap = "1LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    ch_cap = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_chan_new(", ch_cap), ")");
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"Mutex") == 0)) {
            /* pass */
            if ((strcmp((char*)variant, (char*)"init") == 0)) {
                /* pass */
                char* mx_val = "0LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    mx_val = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_mutexbox_new(", mx_val), ")");
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"RwLock") == 0)) {
            /* pass */
            if ((strcmp((char*)variant, (char*)"init") == 0)) {
                /* pass */
                char* rw_val = "0LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    rw_val = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_rwlbox_new(", rw_val), ")");
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"ThreadPool") == 0)) {
            /* pass */
            if ((strcmp((char*)variant, (char*)"new") == 0)) {
                /* pass */
                char* tp_n = "4LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    tp_n = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_threadpool_new(", tp_n), ")");
            }
            /* pass */
            if ((strcmp((char*)variant, (char*)"auto") == 0)) {
                /* pass */
                return "_tr_threadpool_auto()";
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"Thread") == 0)) {
            /* pass */
            if (((strcmp((char*)variant, (char*)"spawn") == 0) || (strcmp((char*)variant, (char*)"new") == 0))) {
                /* pass */
                if ((args->len >= 2LL)) {
                    /* pass */
                    char* th2_fn_nm = "";
                    /* pass */
                    __auto_type _t198 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                    if (_t198.tag == HirExpr_EIdent) {
                        __auto_type thn3 = _t198.data.EIdent.name;
                        th2_fn_nm = thn3;
                    } else if (1) {
                        __auto_type _ = _t198;
                        /* pass */
                    }
                    /* pass */
                    char* th2_arg_s = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                    /* pass */
                    char* th2_aty = hir_expr_type(((HirExpr*)List_ptr_get(args, 1LL)))->name;
                    /* pass */
                    if (((_is_int_type(th2_aty) || (strcmp((char*)th2_aty, (char*)"bool") == 0)) || (strcmp((char*)th2_aty, (char*)"char") == 0))) {
                        /* pass */
                        th2_arg_s = _tr_str_concat(_tr_str_concat("(void*)(uintptr_t)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                    }
                    /* pass */
                    if ((strcmp((char*)th2_fn_nm, (char*)"") != 0)) {
                        /* pass */
                        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadobj_spawn(_tr_spawn_wrap_", th2_fn_nm), ", "), th2_arg_s), ")");
                    }
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadobj_spawn((void*(*)(void*))(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), "), th2_arg_s), ")");
                } else if ((args->len == 1LL)) {
                    /* pass */
                    char* th2_fn_nm2 = "";
                    /* pass */
                    __auto_type _t199 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                    if (_t199.tag == HirExpr_EIdent) {
                        __auto_type thn4 = _t199.data.EIdent.name;
                        th2_fn_nm2 = thn4;
                    } else if (1) {
                        __auto_type _ = _t199;
                        /* pass */
                    }
                    /* pass */
                    if ((strcmp((char*)th2_fn_nm2, (char*)"") != 0)) {
                        /* pass */
                        return _tr_str_concat(_tr_str_concat("_tr_threadobj_spawn(_tr_spawn_wrap_", th2_fn_nm2), ", NULL)");
                    }
                    /* pass */
                    return _tr_str_concat(_tr_str_concat("_tr_threadobj_spawn((void*(*)(void*))(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), NULL)");
                }
            }
            /* pass */
            if (((strcmp((char*)variant, (char*)"sleep") == 0) || (strcmp((char*)variant, (char*)"sleep_ms") == 0))) {
                /* pass */
                char* th2_ms = "0LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    th2_ms = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_thread_sleep_ms((long long)(", th2_ms), "))");
            }
            /* pass */
            if (((strcmp((char*)variant, (char*)"id") == 0) || (strcmp((char*)variant, (char*)"current_id") == 0))) {
                /* pass */
                return "_tr_thread_current_id()";
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"Atomic") == 0)) {
            /* pass */
            if (((strcmp((char*)variant, (char*)"new") == 0) || (strcmp((char*)variant, (char*)"init") == 0))) {
                /* pass */
                char* at2_v = "0LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    at2_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_atomic_new(", at2_v), ")");
            }
        }
        /* pass */
        if ((strcmp((char*)oi_ctor_name, (char*)"ThreadLocal") == 0)) {
            /* pass */
            if (((strcmp((char*)variant, (char*)"new") == 0) || (strcmp((char*)variant, (char*)"init") == 0))) {
                /* pass */
                char* tl2_v = "0LL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    tl2_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_tls_new(", tl2_v), ")");
            }
        }
        /* pass */
        char* oi_type_n = hir_expr_type(obj_inner)->name;
        /* pass */
        if ((strcmp((char*)oi_type_n, (char*)"Result") == 0)) {
            /* pass */
            if ((strcmp((char*)variant, (char*)"ok") == 0)) {
                /* pass */
                char* ok_val = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    ok_val = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Result){.tag=Result_Ok, .data.Ok.val=", ok_val), "})");
            } else if ((strcmp((char*)variant, (char*)"err") == 0)) {
                /* pass */
                char* err_val = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    err_val = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Result){.tag=Result_Err, .data.Err.err=", err_val), "})");
            }
        } else if ((strcmp((char*)oi_type_n, (char*)"Option") == 0)) {
            /* pass */
            if (((strcmp((char*)variant, (char*)"some") == 0) || (strcmp((char*)variant, (char*)"Some") == 0))) {
                /* pass */
                char* sv = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    sv = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Option){.tag=Option_Some, .data.Some.val=", sv), "})");
            } else if (((strcmp((char*)variant, (char*)"none") == 0) || (strcmp((char*)variant, (char*)"None") == 0))) {
                /* pass */
                return "((Option){.tag=Option_None})";
            }
        }
        /* pass */
        if (_tr_dict_contains(self->enums, oi_type_n)) {
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(oi_type_n, "_ctor_"), variant), "("), CGenerator_gen_args(self, args)), ")");
            } else {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(oi_type_n, "_make_"), variant), "()");
            }
        }
    } else if (1) {
        __auto_type _ = _t196;
        /* pass */
    }
    /* pass */
    if (_tr_dict_contains(self->functions, callee_s)) {
        /* pass */
        HirFunction* fn_def = ((HirFunction*)(uintptr_t)_tr_dict_get(self->functions, callee_s));
        /* pass */
        long long self_skip = 0LL;
        /* pass */
        if ((((((unsigned long long)(fn_def->params)) != ((unsigned long long)(0LL))) && (fn_def->params->len > 0LL)) && (strcmp((char*)((HirParam*)List_ptr_get(fn_def->params, 0LL))->name, (char*)"self") == 0))) {
            /* pass */
            self_skip = 1LL;
        }
        /* pass */
        bool any_wrapped = false;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            long long pi = (ai + self_skip);
            /* pass */
            if (((((unsigned long long)(fn_def->params)) != ((unsigned long long)(0LL))) && (pi < fn_def->params->len))) {
                /* pass */
                if (_tr_dict_contains(self->interfaces, ((HirParam*)List_ptr_get(fn_def->params, pi))->ty->name)) {
                    /* pass */
                    char* actual_ty_n = hir_expr_type(((HirExpr*)List_ptr_get(args, ai)))->name;
                    /* pass */
                    if (_tr_dict_contains(self->classes, actual_ty_n)) {
                        /* pass */
                        any_wrapped = true;
                    }
                }
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        if (any_wrapped) {
            /* pass */
            char* wrapped_args = "";
            /* pass */
            ai = 0LL;
            /* pass */
            while ((ai < args->len)) {
                /* pass */
                if ((ai > 0LL)) {
                    /* pass */
                    wrapped_args = _tr_str_concat(wrapped_args, ", ");
                }
                /* pass */
                long long pi = (ai + self_skip);
                /* pass */
                char* arg_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, ai)));
                /* pass */
                if (((((unsigned long long)(fn_def->params)) != ((unsigned long long)(0LL))) && (pi < fn_def->params->len))) {
                    /* pass */
                    AstType* _p_iface_ty = ((HirParam*)List_ptr_get(fn_def->params, pi))->ty;
                    /* pass */
                    char* expected_iface = _p_iface_ty->name;
                    /* pass */
                    if (_tr_dict_contains(self->interfaces, expected_iface)) {
                        /* pass */
                        HirInterface* _ep_iface = ((HirInterface*)(uintptr_t)_tr_dict_get(self->interfaces, expected_iface));
                        /* pass */
                        if (((_ep_iface->generics->len > 0LL) && (_p_iface_ty->args->len > 0LL))) {
                            /* pass */
                            expected_iface = _tr_str_concat(_tr_str_concat(expected_iface, "_"), CGenerator_type_args_suffix(self, _p_iface_ty->args));
                        }
                        /* pass */
                        AstType* actual_ty_e = hir_expr_type(((HirExpr*)List_ptr_get(args, ai)));
                        /* pass */
                        char* actual_ty_n = actual_ty_e->name;
                        /* pass */
                        if (_tr_dict_contains(self->classes, actual_ty_n)) {
                            /* pass */
                            HirClass* _acls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, actual_ty_n));
                            /* pass */
                            if (((_acls->generics->len > 0LL) && (actual_ty_e->args->len > 0LL))) {
                                /* pass */
                                actual_ty_n = _tr_str_concat(_tr_str_concat(actual_ty_n, "_"), CGenerator_type_args_suffix(self, actual_ty_e->args));
                            }
                            /* pass */
                            arg_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(actual_ty_n, "_as_"), expected_iface), "("), arg_s), ")");
                        }
                    }
                }
                /* pass */
                wrapped_args = _tr_str_concat(wrapped_args, arg_s);
                /* pass */
                ai = (ai + 1LL);
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(callee_s, "("), wrapped_args), ")");
        }
    }
    /* pass */
    char* callee_ty_n = hir_expr_type(callee)->name;
    /* pass */
    bool _callee_direct_fn = false;
    /* pass */
    __auto_type _t200 = (*callee);
    if (_t200.tag == HirExpr_EIdent) {
        __auto_type _lc_n = _t200.data.EIdent.name;
        /* pass */
        if (_tr_dict_contains(self->functions, _lc_n)) {
            /* pass */
            _callee_direct_fn = true;
        }
    } else if (1) {
        __auto_type _ = _t200;
        /* pass */
    }
    /* pass */
    if (((((strcmp((char*)callee_ty_n, (char*)"lambda") == 0) || (strcmp((char*)callee_ty_n, (char*)"void*") == 0)) || (strcmp((char*)callee_ty_n, (char*)"def") == 0)) && (!_callee_direct_fn))) {
        /* pass */
        char* ret_cty = CGenerator_type_to_c(self, call_ty);
        /* pass */
        if (((strcmp((char*)ret_cty, (char*)"void") == 0) || (strcmp((char*)ret_cty, (char*)"void*") == 0))) {
            /* pass */
            ret_cty = "long long";
        }
        /* pass */
        char* arg_types = "";
        /* pass */
        long long ai2 = 0LL;
        /* pass */
        while ((ai2 < args->len)) {
            /* pass */
            if ((ai2 > 0LL)) {
                /* pass */
                arg_types = _tr_str_concat(arg_types, ", ");
            }
            /* pass */
            arg_types = _tr_str_concat(arg_types, CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(args, ai2)))));
            /* pass */
            ai2 = (ai2 + 1LL);
        }
        /* pass */
        if ((strcmp((char*)arg_types, (char*)"") == 0)) {
            /* pass */
            arg_types = "void";
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", ret_cty), "(*)("), arg_types), "))("), callee_s), "))("), CGenerator_gen_args(self, args)), ")");
    }
    /* pass */
    char* callee_cls_ty = hir_expr_type(callee)->name;
    /* pass */
    if ((_tr_dict_contains(self->classes, callee_cls_ty) && CGenerator_has_method(self, callee_cls_ty, "__call__"))) {
        /* pass */
        bool is_fn_or_cls = false;
        /* pass */
        __auto_type _t201 = (*callee);
        if (_t201.tag == HirExpr_EIdent) {
            __auto_type cname = _t201.data.EIdent.name;
            /* pass */
            if (_tr_dict_contains(self->functions, cname)) {
                /* pass */
                is_fn_or_cls = true;
            }
            /* pass */
            if (_tr_dict_contains(self->classes, cname)) {
                /* pass */
                is_fn_or_cls = true;
            }
        } else if (1) {
            __auto_type _ = _t201;
            /* pass */
        }
        /* pass */
        if ((!is_fn_or_cls)) {
            /* pass */
            return CGenerator_cls_method_c_call(self, callee_cls_ty, "__call__", callee_s, CGenerator_gen_args(self, args));
        }
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(callee_s, "("), CGenerator_gen_args(self, args)), ")");
}

__attribute__((hot)) char* CGenerator_gen_print_call(CGenerator* self, List_ptr* args) {
    /* pass */
    if ((args->len == 0LL)) {
        /* pass */
        return "printf(\"\\n\")";
    }
    /* pass */
    HirExpr* arg0 = ((HirExpr*)List_ptr_get(args, 0LL));
    /* pass */
    char* ty0_n = hir_expr_type(arg0)->name;
    /* pass */
    char* s0 = CGenerator_gen_expr(self, arg0);
    /* pass */
    if (_is_int_type(ty0_n)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%lld\\n\", (long long)(", s0), "))");
    }
    /* pass */
    if (_is_float_type(ty0_n)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%g\\n\", (double)(", s0), "))");
    }
    /* pass */
    if ((strcmp((char*)ty0_n, (char*)"bool") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%s\\n\", (", s0), ") ? \"true\" : \"false\")");
    }
    /* pass */
    if ((strcmp((char*)ty0_n, (char*)"char") == 0)) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%c\\n\", ", s0), ")");
    }
    /* pass */
    if (CGenerator_has_method(self, ty0_n, "__str__")) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%s\\n\", ", CGenerator_cls_method_c_call(self, ty0_n, "__str__", s0, "")), ")");
    }
    /* pass */
    if (CGenerator_has_method(self, ty0_n, "__repr__")) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%s\\n\", ", CGenerator_cls_method_c_call(self, ty0_n, "__repr__", s0, "")), ")");
    }
    /* pass */
    if ((((strcmp((char*)ty0_n, (char*)"void") == 0) || (strcmp((char*)ty0_n, (char*)"") == 0)) || (strcmp((char*)ty0_n, (char*)"lambda") == 0))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat("printf(\"%s\\n\", _TR_AUTO_STR(", s0), "))");
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat("printf(\"%s\\n\", (char*)(", s0), "))");
}

__attribute__((hot)) char* CGenerator_gen_args(CGenerator* self, List_ptr* args) {
    /* pass */
    char* s = "";
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            s = _tr_str_concat(s, ", ");
        }
        /* pass */
        s = _tr_str_concat(s, CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) char* CGenerator_gen_method_call(CGenerator* self, HirExpr* obj, char* method, List_ptr* args, AstType* call_ty) {
    /* pass */
    char* obj_s = CGenerator_gen_expr(self, obj);
    /* pass */
    char* t_n = hir_expr_type(obj)->name;
    /* pass */
    if (((strcmp((char*)t_n, (char*)"void") == 0) || (strcmp((char*)t_n, (char*)"") == 0))) {
        /* pass */
        __auto_type _t202 = (*obj);
        if (_t202.tag == HirExpr_EPropAccess) {
            __auto_type inner_obj_fb = _t202.data.EPropAccess.obj;
__auto_type field_name_fb = _t202.data.EPropAccess.prop;
            /* pass */
            char* inner_t_n_fb = hir_expr_type(inner_obj_fb)->name;
            /* pass */
            if (((strcmp((char*)inner_t_n_fb, (char*)"void") == 0) || (strcmp((char*)inner_t_n_fb, (char*)"") == 0))) {
                /* pass */
                inner_t_n_fb = self->cur_class;
            }
            /* pass */
            if (_tr_dict_contains(self->classes, inner_t_n_fb)) {
                /* pass */
                HirClass* parent_cls_fb = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, inner_t_n_fb));
                /* pass */
                long long fi_fb = 0LL;
                /* pass */
                while ((fi_fb < parent_cls_fb->fields->len)) {
                    /* pass */
                    HirField* fld_fb = ((HirField*)List_ptr_get(parent_cls_fb->fields, fi_fb));
                    /* pass */
                    if ((((strcmp((char*)fld_fb->name, (char*)field_name_fb) == 0) && (strcmp((char*)fld_fb->ty->name, (char*)"") != 0)) && (strcmp((char*)fld_fb->ty->name, (char*)"void") != 0))) {
                        /* pass */
                        t_n = fld_fb->ty->name;
                    }
                    /* pass */
                    fi_fb = (fi_fb + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t202;
            /* pass */
        }
    }
    /* pass */
    if (_tr_dict_contains(self->shared_vars, obj_s)) {
        /* pass */
        char* sh_ty = ((char*)(uintptr_t)_tr_dict_get(self->shared_vars, obj_s));
        /* pass */
        if ((strcmp((char*)method, (char*)"clone") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_shared_clone(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"drop") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_shared_drop(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_null") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", obj_s), " == NULL || "), obj_s), "->data == NULL)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"downgrade") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_weak_new(", obj_s), ")");
        }
        /* pass */
        char* sh_unwrapped = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", sh_ty), "*)("), obj_s), "->data))");
        /* pass */
        char* sh_sig = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sh_ty, "_"), method), "("), sh_unwrapped);
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            sh_sig = _tr_str_concat(_tr_str_concat(sh_sig, ", "), CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(sh_sig, ")");
    }
    /* pass */
    if (_tr_dict_contains(self->interfaces, t_n)) {
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(obj_s, ".vtable->"), method), "("), obj_s), ".data");
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            s = _tr_str_concat(_tr_str_concat(s, ", "), CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(s, ")");
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"Result") == 0)) {
        /* pass */
        if ((strcmp((char*)obj_s, (char*)"Result") == 0)) {
            /* pass */
            if ((strcmp((char*)method, (char*)"ok") == 0)) {
                /* pass */
                char* ok_cv = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    ok_cv = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Result){.tag=Result_Ok, .data.Ok.val=", ok_cv), "})");
            } else if ((strcmp((char*)method, (char*)"err") == 0)) {
                /* pass */
                char* err_cv = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    err_cv = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Result){.tag=Result_Err, .data.Err.err=", err_cv), "})");
            }
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_err") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Result_Err)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_ok") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Result_Ok)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"unwrap") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("Result_unwrap(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"unwrap_err") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("Result_unwrap_err(", obj_s), ")");
        }
        /* pass */
        char* res_extra = "";
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            res_extra = _tr_str_concat(", ", CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("Result_", method), "("), obj_s), res_extra), ")");
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"Option") == 0)) {
        /* pass */
        if ((strcmp((char*)obj_s, (char*)"Option") == 0)) {
            /* pass */
            if (((strcmp((char*)method, (char*)"some") == 0) || (strcmp((char*)method, (char*)"Some") == 0))) {
                /* pass */
                char* sv2 = "NULL";
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    sv2 = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("((Option){.tag=Option_Some, .data.Some.val=", sv2), "})");
            } else if (((strcmp((char*)method, (char*)"none") == 0) || (strcmp((char*)method, (char*)"None") == 0))) {
                /* pass */
                return "((Option){.tag=Option_None})";
            }
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_some") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Option_Some)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_none") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(", obj_s), ".tag == Option_None)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"unwrap") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("Option_unwrap(", obj_s), ")");
        }
        /* pass */
        char* opt_extra = "";
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            opt_extra = _tr_str_concat(", ", CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("Option_", method), "("), obj_s), opt_extra), ")");
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"Str") == 0) && (strcmp((char*)method, (char*)"join") == 0))) {
        /* pass */
        if ((args->len >= 2LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_join((List_str*)", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if ((args->len == 1LL)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_join((List_str*)", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", \"\")");
        }
    }
    /* pass */
    if ((strcmp((char*)t_n, (char*)"str") == 0)) {
        /* pass */
        char* str_arg0 = "";
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            str_arg0 = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
        }
        /* pass */
        char* str_arg1 = "";
        /* pass */
        if ((args->len > 1LL)) {
            /* pass */
            str_arg1 = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"split") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_split(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"split_to_vec") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_split(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"trim") == 0) || (strcmp((char*)method, (char*)"strip") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_strip(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_left") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_left(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_right") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_right(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"to_upper") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_upper(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"to_lower") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_lower(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"capitalize") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_capitalize(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"title") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_title(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"reverse") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_reverse(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"repeat") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_repeat(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"replace") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_replace(", obj_s), ", "), str_arg0), ", "), str_arg1), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"replace_first") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_replace_first(", obj_s), ", "), str_arg0), ", "), str_arg1), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"slice") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_slice(", obj_s), ", "), str_arg0), ", "), str_arg1), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"strip_prefix") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_strip_prefix(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"strip_suffix") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_strip_suffix(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"remove_char") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_remove_char(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"join") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_join((List_str*)(void*)", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"len") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_strlen((char*)", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"index_of") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_index_of(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"last_index_of") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_last_index_of(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"count") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_count_occ(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"char_at") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_char_at_code(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"starts_with") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_starts_with(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"ends_with") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_ends_with(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"contains") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_contains(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"contains_char") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_contains_char(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"eq") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_eq(", obj_s), ", "), str_arg0), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_digit") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_digit(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_alpha") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_alpha(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_alnum") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_alnum(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_space") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_space(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_upper") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_upper(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_lower") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_is_lower(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_int") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_int(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_float") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_float(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_bool") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_parse_bool(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"lines") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_lines(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"words") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_words(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"lpad") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_lpad(", obj_s), ", "), str_arg0), ", "), str_arg1), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"rpad") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_rpad(", obj_s), ", "), str_arg0), ", "), str_arg1), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"center") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_center(", obj_s), ", "), str_arg0), ")");
        }
    }
    /* pass */
    bool _ptr_intercept = ((strcmp((char*)t_n, (char*)"Pointer") == 0) || (strcmp((char*)method, (char*)"offset") == 0));
    /* pass */
    if (((!_ptr_intercept) && (((strcmp((char*)method, (char*)"read") == 0) || (strcmp((char*)method, (char*)"write") == 0)) || (strcmp((char*)method, (char*)"as_ptr") == 0)))) {
        /* pass */
        if (((((((((((((((strcmp((char*)t_n, (char*)"Chan") != 0) && (strcmp((char*)t_n, (char*)"Mutex") != 0)) && (strcmp((char*)t_n, (char*)"RwLock") != 0)) && (strcmp((char*)t_n, (char*)"ThreadPool") != 0)) && (strcmp((char*)t_n, (char*)"Thread") != 0)) && (strcmp((char*)t_n, (char*)"Atomic") != 0)) && (strcmp((char*)t_n, (char*)"ThreadLocal") != 0)) && (strcmp((char*)obj_s, (char*)"Chan") != 0)) && (strcmp((char*)obj_s, (char*)"Mutex") != 0)) && (strcmp((char*)obj_s, (char*)"RwLock") != 0)) && (strcmp((char*)obj_s, (char*)"ThreadPool") != 0)) && (strcmp((char*)obj_s, (char*)"Thread") != 0)) && (strcmp((char*)obj_s, (char*)"Atomic") != 0)) && (strcmp((char*)obj_s, (char*)"ThreadLocal") != 0))) {
            /* pass */
            if ((!_tr_dict_contains(self->classes, t_n))) {
                /* pass */
                _ptr_intercept = true;
            }
        }
    }
    /* pass */
    if (_ptr_intercept) {
        /* pass */
        if ((strcmp((char*)method, (char*)"offset") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", obj_s), " + "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"read") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(*", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"write") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(*", obj_s), " = "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"as_ptr") == 0)) {
            /* pass */
            return obj_s;
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"Chan") == 0) || (strcmp((char*)obj_s, (char*)"Chan") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"init") == 0)) {
            /* pass */
            char* ch_cap_s = "1LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                ch_cap_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_new(", ch_cap_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"send") == 0)) {
            /* pass */
            char* ch_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                ch_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_chan_send(", obj_s), ", "), ch_v), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"recv") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_recv(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"try_send") == 0)) {
            /* pass */
            char* ch_tv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                ch_tv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_chan_try_send(", obj_s), ", "), ch_tv), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"try_recv") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_try_recv_val(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"close") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_close(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_closed") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_is_closed(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"len") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_len(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"cap") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_cap(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_chan_free(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"send_timeout") == 0) && (args->len >= 2LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_chan_send_timeout(", obj_s), ", (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"recv_timeout") == 0) && (args->len >= 1LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_chan_recv_timeout_val(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"Mutex") == 0) || (strcmp((char*)obj_s, (char*)"Mutex") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"init") == 0)) {
            /* pass */
            char* mx_init_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                mx_init_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_mutexbox_new(", mx_init_v), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"lock") == 0) || (strcmp((char*)method, (char*)"get") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_mutexbox_lock_get(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"set") == 0)) {
            /* pass */
            char* mx_sv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                mx_sv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_mutexbox_set_unlock(", obj_s), ", "), mx_sv), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"unlock") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_mutexbox_unlock(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_mutexbox_free(", obj_s), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"RwLock") == 0) || (strcmp((char*)obj_s, (char*)"RwLock") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"init") == 0)) {
            /* pass */
            char* rw_init_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                rw_init_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_rwlbox_new(", rw_init_v), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"read") == 0) || (strcmp((char*)method, (char*)"read_lock") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_rwlbox_read_get(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"read_unlock") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_rwlbox_read_unlock(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"write") == 0) || (strcmp((char*)method, (char*)"write_lock") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_rwlbox_write_get(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"write_set") == 0) || (strcmp((char*)method, (char*)"set") == 0))) {
            /* pass */
            char* rw_sv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                rw_sv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_rwlbox_write_set_unlock(", obj_s), ", "), rw_sv), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_rwlbox_free(", obj_s), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"ThreadPool") == 0) || (strcmp((char*)obj_s, (char*)"ThreadPool") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"new") == 0)) {
            /* pass */
            char* tp_nw = "4LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                tp_nw = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadpool_new(", tp_nw), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"auto") == 0)) {
            /* pass */
            return "_tr_threadpool_auto()";
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"spawn") == 0)) {
            /* pass */
            if ((args->len >= 2LL)) {
                /* pass */
                char* tp_fn_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                /* pass */
                char* tp_fn_nm = "";
                /* pass */
                __auto_type _t203 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                if (_t203.tag == HirExpr_EIdent) {
                    __auto_type tpn = _t203.data.EIdent.name;
                    tp_fn_nm = tpn;
                } else if (1) {
                    __auto_type _ = _t203;
                    /* pass */
                }
                /* pass */
                char* tp_arg_s = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                /* pass */
                char* tp_aty = hir_expr_type(((HirExpr*)List_ptr_get(args, 1LL)))->name;
                /* pass */
                if (((_is_int_type(tp_aty) || (strcmp((char*)tp_aty, (char*)"bool") == 0)) || (strcmp((char*)tp_aty, (char*)"char") == 0))) {
                    /* pass */
                    tp_arg_s = _tr_str_concat(_tr_str_concat("(void*)(uintptr_t)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                }
                /* pass */
                if ((strcmp((char*)tp_fn_nm, (char*)"") != 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadpool_spawn(", obj_s), ", _tr_spawn_wrap_"), tp_fn_nm), ", "), tp_arg_s), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadpool_spawn(", obj_s), ", (void*(*)(void*))("), tp_fn_s), "), "), tp_arg_s), ")");
            } else if ((args->len == 1LL)) {
                /* pass */
                char* tp_fn2_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                /* pass */
                char* tp_fn2_nm = "";
                /* pass */
                __auto_type _t204 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                if (_t204.tag == HirExpr_EIdent) {
                    __auto_type tpn2 = _t204.data.EIdent.name;
                    tp_fn2_nm = tpn2;
                } else if (1) {
                    __auto_type _ = _t204;
                    /* pass */
                }
                /* pass */
                if ((strcmp((char*)tp_fn2_nm, (char*)"") != 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadpool_spawn(", obj_s), ", _tr_spawn_wrap_"), tp_fn2_nm), ", NULL)");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadpool_spawn(", obj_s), ", (void*(*)(void*))("), tp_fn2_s), "), NULL)");
            }
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"wait") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadpool_wait(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadpool_free(", obj_s), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"Thread") == 0) || (strcmp((char*)obj_s, (char*)"Thread") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"spawn") == 0)) {
            /* pass */
            if ((args->len >= 3LL)) {
                /* pass */
                char* th_ma_fn = "";
                /* pass */
                __auto_type _t205 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                if (_t205.tag == HirExpr_EIdent) {
                    __auto_type thn_ma = _t205.data.EIdent.name;
                    th_ma_fn = thn_ma;
                } else if (1) {
                    __auto_type _ = _t205;
                    /* pass */
                }
                /* pass */
                long long th_ma_n = (args->len - 1LL);
                /* pass */
                char* th_ma_tmp = CGenerator_next_temp(self);
                /* pass */
                char* th_ma_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long* _thab", th_ma_tmp), " = (long long*)_tr_checked_alloc("), _tr_int_to_str((long long)(th_ma_n))), " * sizeof(long long)); ");
                /* pass */
                long long th_ma_ai = 1LL;
                /* pass */
                long long th_ma_di = 0LL;
                /* pass */
                while ((th_ma_ai < args->len)) {
                    /* pass */
                    char* th_ma_aty = hir_expr_type(((HirExpr*)List_ptr_get(args, th_ma_ai)))->name;
                    /* pass */
                    if (((_is_int_type(th_ma_aty) || (strcmp((char*)th_ma_aty, (char*)"bool") == 0)) || (strcmp((char*)th_ma_aty, (char*)"char") == 0))) {
                        /* pass */
                        th_ma_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(th_ma_s, "_thab"), th_ma_tmp), "["), _tr_int_to_str((long long)(th_ma_di))), "] = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, th_ma_ai)))), "); ");
                    } else {
                        /* pass */
                        th_ma_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(th_ma_s, "_thab"), th_ma_tmp), "["), _tr_int_to_str((long long)(th_ma_di))), "] = (long long)(uintptr_t)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, th_ma_ai)))), "); ");
                    }
                    /* pass */
                    th_ma_ai = (th_ma_ai + 1LL);
                    /* pass */
                    th_ma_di = (th_ma_di + 1LL);
                }
                /* pass */
                if ((strcmp((char*)th_ma_fn, (char*)"") != 0)) {
                    /* pass */
                    th_ma_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(th_ma_s, "_tr_threadobj_spawn(_tr_spawn_wrap_"), th_ma_fn), ", _thab"), th_ma_tmp), "); })");
                } else {
                    /* pass */
                    th_ma_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(th_ma_s, "_tr_threadobj_spawn((void*(*)(void*))("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), _thab"), th_ma_tmp), "); })");
                }
                /* pass */
                return th_ma_s;
            } else if ((args->len == 2LL)) {
                /* pass */
                char* th_fn_nm = "";
                /* pass */
                __auto_type _t206 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                if (_t206.tag == HirExpr_EIdent) {
                    __auto_type thn = _t206.data.EIdent.name;
                    th_fn_nm = thn;
                } else if (1) {
                    __auto_type _ = _t206;
                    /* pass */
                }
                /* pass */
                char* th_arg_s = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                /* pass */
                char* th_aty = hir_expr_type(((HirExpr*)List_ptr_get(args, 1LL)))->name;
                /* pass */
                if (((_is_int_type(th_aty) || (strcmp((char*)th_aty, (char*)"bool") == 0)) || (strcmp((char*)th_aty, (char*)"char") == 0))) {
                    /* pass */
                    th_arg_s = _tr_str_concat(_tr_str_concat("(void*)(uintptr_t)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
                }
                /* pass */
                if ((strcmp((char*)th_fn_nm, (char*)"") != 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadobj_spawn(_tr_spawn_wrap_", th_fn_nm), ", "), th_arg_s), ")");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_threadobj_spawn((void*(*)(void*))(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), "), th_arg_s), ")");
            } else if ((args->len == 1LL)) {
                /* pass */
                char* th_fn_nm2 = "";
                /* pass */
                __auto_type _t207 = (*((HirExpr*)List_ptr_get(args, 0LL)));
                if (_t207.tag == HirExpr_EIdent) {
                    __auto_type thn2 = _t207.data.EIdent.name;
                    th_fn_nm2 = thn2;
                } else if (1) {
                    __auto_type _ = _t207;
                    /* pass */
                }
                /* pass */
                if ((strcmp((char*)th_fn_nm2, (char*)"") != 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat("_tr_threadobj_spawn(_tr_spawn_wrap_", th_fn_nm2), ", NULL)");
                }
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_threadobj_spawn((void*(*)(void*))(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "), NULL)");
            }
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"join") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadobj_join(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"detach") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadobj_detach(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_threadobj_free(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"id") == 0) || (strcmp((char*)method, (char*)"current_id") == 0))) {
            /* pass */
            return "_tr_thread_current_id()";
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sleep") == 0) || (strcmp((char*)method, (char*)"sleep_ms") == 0))) {
            /* pass */
            char* th_ms = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                th_ms = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_thread_sleep_ms((long long)(", th_ms), "))");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"Atomic") == 0) || (strcmp((char*)obj_s, (char*)"Atomic") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"new") == 0) || (strcmp((char*)method, (char*)"init") == 0))) {
            /* pass */
            char* at_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_new(", at_v), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"load") == 0) || (strcmp((char*)method, (char*)"get") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_load(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"store") == 0) || (strcmp((char*)method, (char*)"set") == 0))) {
            /* pass */
            char* at_sv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_sv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_store(", obj_s), ", "), at_sv), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"add") == 0) || (strcmp((char*)method, (char*)"fetch_add") == 0))) {
            /* pass */
            char* at_av = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_av = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_add(", obj_s), ", "), at_av), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sub") == 0) || (strcmp((char*)method, (char*)"fetch_sub") == 0))) {
            /* pass */
            char* at_bv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_bv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_sub(", obj_s), ", "), at_bv), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"swap") == 0) || (strcmp((char*)method, (char*)"exchange") == 0))) {
            /* pass */
            char* at_xv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_xv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_swap(", obj_s), ", "), at_xv), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"cas") == 0) || (strcmp((char*)method, (char*)"compare_exchange") == 0))) {
            /* pass */
            char* at_exp = "0LL";
            /* pass */
            char* at_des = "0LL";
            /* pass */
            if ((args->len >= 1LL)) {
                /* pass */
                at_exp = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            if ((args->len >= 2LL)) {
                /* pass */
                at_des = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_cas(", obj_s), ", "), at_exp), ", "), at_des), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"increment") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_add(", obj_s), ", 1LL)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"decrement") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_sub(", obj_s), ", 1LL)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_free(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"load_relaxed") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_load_relaxed(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"load_acquire") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_load_acquire(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"load_seqcst") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_atomic_load_seqcst(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"store_relaxed") == 0)) {
            /* pass */
            char* at_or_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_store_relaxed(", obj_s), ", "), at_or_v), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"store_release") == 0)) {
            /* pass */
            char* at_or_v2 = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_v2 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_store_release(", obj_s), ", "), at_or_v2), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"store_seqcst") == 0)) {
            /* pass */
            char* at_or_v3 = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_v3 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_store_seqcst(", obj_s), ", "), at_or_v3), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"add_relaxed") == 0) || (strcmp((char*)method, (char*)"fetch_add_relaxed") == 0))) {
            /* pass */
            char* at_or_av = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_av = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_add_relaxed(", obj_s), ", "), at_or_av), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"add_release") == 0) || (strcmp((char*)method, (char*)"fetch_add_release") == 0))) {
            /* pass */
            char* at_or_av2 = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_av2 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_add_release(", obj_s), ", "), at_or_av2), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"add_acqrel") == 0) || (strcmp((char*)method, (char*)"fetch_add_acqrel") == 0))) {
            /* pass */
            char* at_or_av3 = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_av3 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_add_acqrel(", obj_s), ", "), at_or_av3), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sub_relaxed") == 0) || (strcmp((char*)method, (char*)"fetch_sub_relaxed") == 0))) {
            /* pass */
            char* at_or_sv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_sv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_sub_relaxed(", obj_s), ", "), at_or_sv), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sub_release") == 0) || (strcmp((char*)method, (char*)"fetch_sub_release") == 0))) {
            /* pass */
            char* at_or_sv2 = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                at_or_sv2 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_sub_release(", obj_s), ", "), at_or_sv2), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"cas_weak") == 0)) {
            /* pass */
            char* at_or_exp = "0LL";
            /* pass */
            char* at_or_des = "0LL";
            /* pass */
            if ((args->len >= 1LL)) {
                /* pass */
                at_or_exp = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            if ((args->len >= 2LL)) {
                /* pass */
                at_or_des = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_cas_weak(", obj_s), ", "), at_or_exp), ", "), at_or_des), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"cas_acqrel") == 0)) {
            /* pass */
            char* at_or_exp2 = "0LL";
            /* pass */
            char* at_or_des2 = "0LL";
            /* pass */
            if ((args->len >= 1LL)) {
                /* pass */
                at_or_exp2 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            if ((args->len >= 2LL)) {
                /* pass */
                at_or_des2 = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_atomic_cas_acqrel(", obj_s), ", "), at_or_exp2), ", "), at_or_des2), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"ThreadLocal") == 0) || (strcmp((char*)obj_s, (char*)"ThreadLocal") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"new") == 0) || (strcmp((char*)method, (char*)"init") == 0))) {
            /* pass */
            char* tl_v = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                tl_v = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_tls_new(", tl_v), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"get") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_tls_get(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"set") == 0)) {
            /* pass */
            char* tl_sv = "0LL";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                tl_sv = _tr_str_concat(_tr_str_concat("(long long)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_tls_set(", obj_s), ", "), tl_sv), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_tls_free(", obj_s), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)method, (char*)"to_str") == 0) || (strcmp((char*)method, (char*)"to_string") == 0))) {
        /* pass */
        if (_is_int_type(t_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_int_to_str((long long)(", obj_s), "))");
        }
        /* pass */
        if (_is_float_type(t_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_float_to_str((double)(", obj_s), "))");
        }
        /* pass */
        if ((strcmp((char*)t_n, (char*)"bool") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((", obj_s), ") ? \"true\" : \"false\")");
        }
        /* pass */
        if ((strcmp((char*)t_n, (char*)"char") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_char_to_str_alloc(", obj_s), ")");
        }
        /* pass */
        if (((!((((_tr_dict_contains(self->classes, t_n) && (strcmp((char*)t_n, (char*)"Vec") != 0)) && (strcmp((char*)t_n, (char*)"Map") != 0)) && (strcmp((char*)t_n, (char*)"Dict") != 0)) && (strcmp((char*)t_n, (char*)"List") != 0))) && (strcmp((char*)t_n, (char*)"StringBuilder") != 0))) {
            /* pass */
            return obj_s;
        }
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"to_float") == 0)) {
        /* pass */
        if (_is_str_type(t_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_float(", obj_s), ")");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("(double)(", obj_s), ")");
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"to_int") == 0)) {
        /* pass */
        if (_is_str_type(t_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_int(", obj_s), ")");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("(long long)(", obj_s), ")");
    }
    /* pass */
    if (_is_float_type(t_n)) {
        /* pass */
        if ((strcmp((char*)method, (char*)"floor") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)floor((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"ceil") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)ceil((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"round") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)round((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sqrt") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)sqrt((double)(", obj_s), ")))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"abs") == 0) || (strcmp((char*)method, (char*)"fabs") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)fabs((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"log") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)log((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"log2") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)log2((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"log10") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)log10((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"exp") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)exp((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sin") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)sin((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"cos") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)cos((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"tan") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)tan((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"asin") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)asin((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"acos") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)acos((double)(", obj_s), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"atan") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((double)atan((double)(", obj_s), ")))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"atan2") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((double)atan2((double)(", obj_s), "), (double)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"pow") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((double)pow((double)(", obj_s), "), (double)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sign") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((double)(((", obj_s), ") > 0.0) - (("), obj_s), ") < 0.0)))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"clamp") == 0) && (args->len == 2LL))) {
            /* pass */
            char* _ca = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _cb = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ double _v = (double)(", obj_s), "); double _lo = (double)("), _ca), "); double _hi = (double)("), _cb), "); _v < _lo ? _lo : (_v > _hi ? _hi : _v); })");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_nan") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_float_is_nan((double)(", obj_s), "))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_inf") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_float_is_inf((double)(", obj_s), "))");
        }
    }
    /* pass */
    if (_is_int_type(t_n)) {
        /* pass */
        if ((strcmp((char*)method, (char*)"to_hex") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("({ char* _hb = (char*)_tr_checked_alloc(32); snprintf(_hb, 32, \"%llx\", (long long)(", obj_s), ")); _hb; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"to_HEX") == 0) || (strcmp((char*)method, (char*)"to_hex_upper") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("({ char* _hb = (char*)_tr_checked_alloc(32); snprintf(_hb, 32, \"%llX\", (long long)(", obj_s), ")); _hb; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"to_octal") == 0) || (strcmp((char*)method, (char*)"to_oct") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("({ char* _ob = (char*)_tr_checked_alloc(32); snprintf(_ob, 32, \"%llo\", (long long)(", obj_s), ")); _ob; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"to_binary") == 0) || (strcmp((char*)method, (char*)"to_bin") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_int_to_binary((long long)(", obj_s), "))");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sign") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((long long)(((", obj_s), ") > 0LL) - (("), obj_s), ") < 0LL)))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"clamp") == 0) && (args->len == 2LL))) {
            /* pass */
            char* _ia = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _ib = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long _v = (long long)(", obj_s), "); long long _lo = (long long)("), _ia), "); long long _hi = (long long)("), _ib), "); _v < _lo ? _lo : (_v > _hi ? _hi : _v); })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"gcd") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_int_gcd((long long)(", obj_s), "), (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"lcm") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_int_lcm((long long)(", obj_s), "), (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "))");
        }
    }
    /* pass */
    if ((_is_int_type(t_n) || _is_int_type(t_n))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"checked_add") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _ca = obj_s;
            /* pass */
            char* _cb = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _cr = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _cr), "; __builtin_add_overflow((long long)("), _ca), "),(long long)("), _cb), "),&"), _cr), ") ? ((Option){.tag=Option_None}) : ((Option){.tag=Option_Some,.data.Some.val=(void*)(uintptr_t)"), _cr), "}); })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"checked_sub") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _sa = obj_s;
            /* pass */
            char* _sb = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _sr = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _sr), "; __builtin_sub_overflow((long long)("), _sa), "),(long long)("), _sb), "),&"), _sr), ") ? ((Option){.tag=Option_None}) : ((Option){.tag=Option_Some,.data.Some.val=(void*)(uintptr_t)"), _sr), "}); })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"checked_mul") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _ma = obj_s;
            /* pass */
            char* _mb = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _mr = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _mr), "; __builtin_mul_overflow((long long)("), _ma), "),(long long)("), _mb), "),&"), _mr), ") ? ((Option){.tag=Option_None}) : ((Option){.tag=Option_Some,.data.Some.val=(void*)(uintptr_t)"), _mr), "}); })");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"abs") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("llabs((long long)(", obj_s), "))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"min") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _mn = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _mn), " = (long long)("), obj_s), "); long long _b = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "); "), _mn), " < _b ? "), _mn), " : _b; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"max") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _mx = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long ", _mx), " = (long long)("), obj_s), "); long long _b = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "); "), _mx), " > _b ? "), _mx), " : _b; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"pow") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((long long)pow((double)(", obj_s), "),(double)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")))");
        }
    }
    /* pass */
    if ((strcmp((char*)method, (char*)"as_str") == 0)) {
        /* pass */
        if ((strcmp((char*)t_n, (char*)"StringObj") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("StringObj_as_str(", obj_s), ")");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("(char*)(", obj_s), ")");
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"starts_with") == 0) && (args->len > 0LL)) && _is_str_type(t_n))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_starts_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"ends_with") == 0) && (args->len > 0LL)) && _is_str_type(t_n))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_ends_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"contains") == 0) && (args->len > 0LL)) && _is_str_type(t_n))) {
        /* pass */
        return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_contains(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
    }
    /* pass */
    if (_is_str_type(t_n)) {
        /* pass */
        if (((strcmp((char*)method, (char*)"upper") == 0) || (strcmp((char*)method, (char*)"to_upper") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_upper(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"lower") == 0) || (strcmp((char*)method, (char*)"to_lower") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_lower(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"strip") == 0) || (strcmp((char*)method, (char*)"trim") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_strip(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_left") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_left(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_right") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_right(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"replace") == 0) && (args->len == 2LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_replace(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"split") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_split(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"find") == 0) && (args->len > 0LL))) {
            /* pass */
            char* tmp_find = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ char* ", tmp_find), " = strstr("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), "); "), tmp_find), " ? (long long)("), tmp_find), " - ("), obj_s), ")) : -1LL; })");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"slice") == 0) && (args->len == 2LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_slice(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"index_of") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_index_of(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"last_index_of") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_last_index_of(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_strlen(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"starts_with") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_starts_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"ends_with") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_ends_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"contains") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_contains(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"strip_prefix") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_strip_prefix(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"strip_suffix") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_strip_suffix(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"repeat") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_repeat(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"reverse") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_reverse(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_int") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_int(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_float") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_to_float(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"parse_bool") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_parse_bool(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"char_at") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_char_at_code(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"split_once") == 0) && (args->len > 0LL))) {
            /* pass */
            char* _so_sep = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _so_t = CGenerator_next_temp(self);
            /* pass */
            char* _so_p = CGenerator_next_temp(self);
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ char* ", _so_p), " = strstr("), obj_s), ", "), _so_sep), "); "), _so_p), " ? _tr_str_tuple2(_tr_str_slice("), obj_s), ", 0LL, (long long)("), _so_p), " - ("), obj_s), "))), _tr_str_slice("), _so_p), ", _tr_strlen("), _so_sep), "), _tr_strlen("), _so_p), "))) : _tr_str_tuple2("), obj_s), ", \"\"); })");
        }
    }
    /* pass */
    if (((strcmp((char*)t_n, (char*)"void") == 0) || (strcmp((char*)t_n, (char*)"") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"strip") == 0) || (strcmp((char*)method, (char*)"trim") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_strip(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"upper") == 0) || (strcmp((char*)method, (char*)"to_upper") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_upper(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"lower") == 0) || (strcmp((char*)method, (char*)"to_lower") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_lower(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_left") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_left(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"trim_right") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_str_trim_right(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"slice") == 0) && (args->len == 2LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_slice(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"index_of") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_index_of(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"starts_with") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_starts_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"ends_with") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_ends_with(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"contains") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_contains(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"split") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_split(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"replace") == 0) && (args->len == 2LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_replace(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_strlen(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"char_at") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_char_at_code(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    char* class_name = t_n;
    /* pass */
    if (_is_str_type(class_name)) {
        /* pass */
        class_name = "TrStr";
    }
    /* pass */
    if ((((strcmp((char*)class_name, (char*)"") != 0) && (strcmp((char*)class_name, (char*)"void") != 0)) && _tr_dict_contains(self->type_alias_map, class_name))) {
        /* pass */
        char* resolved_c = ((char*)(uintptr_t)_tr_dict_get(self->type_alias_map, class_name));
        /* pass */
        if (_tr_str_starts_with(resolved_c, "List_")) {
            /* pass */
            class_name = "List";
        } else if ((_tr_str_starts_with(resolved_c, "Map_") || (strcmp((char*)resolved_c, (char*)"TrMap*") == 0))) {
            /* pass */
            class_name = "Map";
        }
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"OS") == 0) || (strcmp((char*)class_name, (char*)"OS") == 0))) {
        /* pass */
        if ((strcmp((char*)method, (char*)"cwd") == 0)) {
            /* pass */
            return "_tr_cwd()";
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_windows") == 0)) {
            /* pass */
            return "_tr_is_windows()";
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_linux") == 0)) {
            /* pass */
            return "(!_tr_is_windows())";
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"platform") == 0)) {
            /* pass */
            return "(_tr_is_windows() ? \"windows\" : \"linux\")";
        }
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"Process") == 0) || (strcmp((char*)class_name, (char*)"Process") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"system") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_system(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"shell_output") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_popen_read(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"exit") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_exit(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"Env") == 0) || (strcmp((char*)class_name, (char*)"Env") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"get_var") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_getenv(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"set_var") == 0) && (args->len > 1LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_setenv(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)))), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"Hash") == 0) || (strcmp((char*)class_name, (char*)"Hash") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"sha256") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_sha256_hex(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"md5") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_md5_hex(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    if (((strcmp((char*)obj_s, (char*)"File") == 0) || (strcmp((char*)class_name, (char*)"File") == 0))) {
        /* pass */
        if (((strcmp((char*)method, (char*)"dir_exists") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dir_exists(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    if ((((strcmp((char*)class_name, (char*)"void") == 0) || (strcmp((char*)class_name, (char*)"") == 0)) && _tr_dict_contains(self->classes, obj_s))) {
        /* pass */
        class_name = obj_s;
    }
    /* pass */
    if ((((strcmp((char*)class_name, (char*)"void") == 0) || (strcmp((char*)class_name, (char*)"") == 0)) && ((((strcmp((char*)obj_s, (char*)"Vec") == 0) || (strcmp((char*)obj_s, (char*)"List") == 0)) || (strcmp((char*)obj_s, (char*)"Map") == 0)) || (strcmp((char*)obj_s, (char*)"Dict") == 0)))) {
        /* pass */
        class_name = obj_s;
    }
    /* pass */
    if ((strcmp((char*)class_name, (char*)"StringBuilder") == 0)) {
        /* pass */
        if ((strcmp((char*)method, (char*)"init") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("StringBuilder_init(", CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"append") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("StringBuilder_append(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"append_char") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("StringBuilder_append_char(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"to_string") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("StringBuilder_to_string(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            return _tr_str_concat(obj_s, "->buf->len");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("StringBuilder_free(", obj_s), ")");
        }
    }
    /* pass */
    if ((((((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"__len__") == 0)) || (strcmp((char*)method, (char*)"length") == 0)) && (!((((_tr_dict_contains(self->classes, t_n) && (strcmp((char*)t_n, (char*)"Vec") != 0)) && (strcmp((char*)t_n, (char*)"Map") != 0)) && (strcmp((char*)t_n, (char*)"Dict") != 0)) && (strcmp((char*)t_n, (char*)"List") != 0)))) && (!_tr_dict_contains(self->enums, t_n)))) {
        /* pass */
        if (_is_str_type(t_n)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_strlen(", obj_s), ")");
        }
        /* pass */
        return _tr_str_concat(_tr_str_concat("((long long)", obj_s), "->len)");
    }
    /* pass */
    if ((((strcmp((char*)method, (char*)"get_index") == 0) && CGenerator_has_method(self, class_name, "__getitem__")) && (args->len > 0LL))) {
        /* pass */
        return CGenerator_cls_method_c_call(self, class_name, "__getitem__", obj_s, CGenerator_gen_args(self, args));
    }
    /* pass */
    if (((((_tr_dict_contains(self->classes, class_name) && (strcmp((char*)class_name, (char*)"Vec") != 0)) && (strcmp((char*)class_name, (char*)"Map") != 0)) && (strcmp((char*)class_name, (char*)"Dict") != 0)) && (strcmp((char*)class_name, (char*)"List") != 0))) {
        /* pass */
        HirClass* ucls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, class_name));
        /* pass */
        if ((ucls->generics->len > 0LL)) {
            /* pass */
            if ((hir_expr_type(obj)->args->len > 0LL)) {
                /* pass */
                char* sfx = CGenerator_type_args_suffix(self, hir_expr_type(obj)->args);
                /* pass */
                char* mono_name = _tr_str_concat(_tr_str_concat(class_name, "_"), sfx);
                /* pass */
                CGenerator_ensure_mono(self, ucls, hir_expr_type(obj)->args);
                /* pass */
                char* gmethod_c = method;
                /* pass */
                if (_is_c_keyword(gmethod_c)) {
                    /* pass */
                    gmethod_c = _tr_str_concat("_tr_fn_", gmethod_c);
                }
                /* pass */
                if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mono_name, "_"), gmethod_c), "("), CGenerator_gen_args(self, args)), ")");
                }
                /* pass */
                if ((strcmp((char*)obj_s, (char*)class_name) == 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mono_name, "_"), gmethod_c), "("), CGenerator_gen_args(self, args)), ")");
                }
                /* pass */
                char* ms = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mono_name, "_"), gmethod_c), "("), obj_s);
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    ms = _tr_str_concat(_tr_str_concat(ms, ", "), CGenerator_gen_args(self, args));
                }
                /* pass */
                return _tr_str_concat(ms, ")");
            }
            /* pass */
            __auto_type _t208 = (*obj);
            if (_t208.tag == HirExpr_EPropAccess) {
                __auto_type inner_obj2 = _t208.data.EPropAccess.obj;
__auto_type fld_name2 = _t208.data.EPropAccess.prop;
                /* pass */
                char* parent_ty_n2 = hir_expr_type(inner_obj2)->name;
                /* pass */
                if (_tr_dict_contains(self->classes, parent_ty_n2)) {
                    /* pass */
                    HirClass* parent_cls2 = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, parent_ty_n2));
                    /* pass */
                    long long fld_i2 = 0LL;
                    /* pass */
                    while ((fld_i2 < parent_cls2->fields->len)) {
                        /* pass */
                        HirField* fld2 = ((HirField*)List_ptr_get(parent_cls2->fields, fld_i2));
                        /* pass */
                        if ((((strcmp((char*)fld2->name, (char*)fld_name2) == 0) && (strcmp((char*)fld2->ty->name, (char*)class_name) == 0)) && (fld2->ty->args->len > 0LL))) {
                            /* pass */
                            char* sfx3 = CGenerator_type_args_suffix(self, fld2->ty->args);
                            /* pass */
                            char* mn3 = _tr_str_concat(_tr_str_concat(class_name, "_"), sfx3);
                            /* pass */
                            CGenerator_ensure_mono(self, ucls, fld2->ty->args);
                            /* pass */
                            char* method_c3 = method;
                            /* pass */
                            if (_is_c_keyword(method_c3)) {
                                /* pass */
                                method_c3 = _tr_str_concat("_tr_fn_", method_c3);
                            }
                            /* pass */
                            if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
                                /* pass */
                                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mn3, "_"), method_c3), "("), CGenerator_gen_args(self, args)), ")");
                            }
                            /* pass */
                            if ((strcmp((char*)obj_s, (char*)class_name) == 0)) {
                                /* pass */
                                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mn3, "_"), method_c3), "("), CGenerator_gen_args(self, args)), ")");
                            }
                            /* pass */
                            char* ms3 = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(mn3, "_"), method_c3), "("), obj_s);
                            /* pass */
                            if ((args->len > 0LL)) {
                                /* pass */
                                ms3 = _tr_str_concat(_tr_str_concat(ms3, ", "), CGenerator_gen_args(self, args));
                            }
                            /* pass */
                            return _tr_str_concat(ms3, ")");
                        }
                        /* pass */
                        fld_i2 = (fld_i2 + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t208;
                /* pass */
            }
            /* pass */
            if ((strcmp((char*)self->cur_class, (char*)"") != 0)) {
                /* pass */
                char* method_c_g = method;
                /* pass */
                if (_is_c_keyword(method_c_g)) {
                    /* pass */
                    method_c_g = _tr_str_concat("_tr_fn_", method_c_g);
                }
                /* pass */
                if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(self->cur_class, "_"), method_c_g), "("), CGenerator_gen_args(self, args)), ")");
                }
                /* pass */
                if ((strcmp((char*)obj_s, (char*)class_name) == 0)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(self->cur_class, "_"), method_c_g), "("), CGenerator_gen_args(self, args)), ")");
                }
                /* pass */
                char* ms_g = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(self->cur_class, "_"), method_c_g), "("), obj_s);
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    ms_g = _tr_str_concat(_tr_str_concat(ms_g, ", "), CGenerator_gen_args(self, args));
                }
                /* pass */
                return _tr_str_concat(ms_g, ")");
            }
        }
    }
    /* pass */
    if (((((_tr_dict_contains(self->classes, class_name) && (strcmp((char*)class_name, (char*)"Vec") != 0)) && (strcmp((char*)class_name, (char*)"Map") != 0)) && (strcmp((char*)class_name, (char*)"Dict") != 0)) && (strcmp((char*)class_name, (char*)"List") != 0))) {
        /* pass */
        char* method_c = method;
        /* pass */
        if (_is_c_keyword(method_c)) {
            /* pass */
            method_c = _tr_str_concat("_tr_fn_", method_c);
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(class_name, "_"), method_c), "("), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)obj_s, (char*)class_name) == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(class_name, "_"), method_c), "("), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        HirClass* ucls_inh = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, class_name));
        /* pass */
        bool has_own_method = false;
        /* pass */
        long long mci = 0LL;
        /* pass */
        while ((mci < ucls_inh->methods->len)) {
            /* pass */
            HirFunction* mdef = ((HirFunction*)List_ptr_get(ucls_inh->methods, mci));
            /* pass */
            if ((strcmp((char*)mdef->name, (char*)method) == 0)) {
                /* pass */
                has_own_method = true;
            }
            /* pass */
            mci = (mci + 1LL);
        }
        /* pass */
        if (((!has_own_method) && (ucls_inh->base_classes->len > 0LL))) {
            /* pass */
            char* base_cls_n = List_str_get(ucls_inh->base_classes, 0LL);
            /* pass */
            if (_tr_dict_contains(self->classes, base_cls_n)) {
                /* pass */
                char* s_base = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(base_cls_n, "_"), method_c), "(("), base_cls_n), "*)"), obj_s);
                /* pass */
                if ((args->len > 0LL)) {
                    /* pass */
                    s_base = _tr_str_concat(_tr_str_concat(s_base, ", "), CGenerator_gen_args(self, args));
                }
                /* pass */
                return _tr_str_concat(s_base, ")");
            }
        }
        /* pass */
        char* dispatch_name = _tr_str_concat(_tr_str_concat(class_name, "_"), method_c);
        /* pass */
        if (_tr_dict_contains(self->overloaded_sigs, dispatch_name)) {
            /* pass */
            dispatch_name = _tr_str_concat(_tr_str_concat(_tr_str_concat(dispatch_name, "_"), _tr_int_to_str((long long)(args->len))), "arg");
        }
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(dispatch_name, "("), obj_s);
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            s = _tr_str_concat(_tr_str_concat(s, ", "), CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(s, ")");
    }
    /* pass */
    if (_tr_dict_contains(self->enums, class_name)) {
        /* pass */
        HirEnum* _evdef = ((HirEnum*)(uintptr_t)_tr_dict_get(self->enums, class_name));
        /* pass */
        bool _is_var = false;
        /* pass */
        long long _vi2 = 0LL;
        /* pass */
        while ((_vi2 < _evdef->variants->len)) {
            /* pass */
            if ((strcmp((char*)((HirVariant*)List_ptr_get(_evdef->variants, _vi2))->name, (char*)method) == 0)) {
                /* pass */
                _is_var = true;
            }
            /* pass */
            _vi2 = (_vi2 + 1LL);
        }
        /* pass */
        if (_is_var) {
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(class_name, "_ctor_"), method), "("), CGenerator_gen_args(self, args)), ")");
            } else {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(class_name, "_make_"), method), "()");
            }
        }
        /* pass */
        char* enum_dispatch_name = _tr_str_concat(_tr_str_concat(class_name, "_"), method);
        /* pass */
        if (_tr_dict_contains(self->overloaded_sigs, enum_dispatch_name)) {
            /* pass */
            enum_dispatch_name = _tr_str_concat(_tr_str_concat(_tr_str_concat(enum_dispatch_name, "_"), _tr_int_to_str((long long)(args->len))), "arg");
        }
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(enum_dispatch_name, "("), obj_s);
        /* pass */
        if ((args->len > 0LL)) {
            /* pass */
            s = _tr_str_concat(_tr_str_concat(s, ", "), CGenerator_gen_args(self, args));
        }
        /* pass */
        return _tr_str_concat(s, ")");
    }
    /* pass */
    char* elem_sfx = "ptr";
    /* pass */
    __auto_type _t209 = (*obj);
    if (_t209.tag == HirExpr_EIdent) {
        __auto_type ity = _t209.data.EIdent.ty;
        /* pass */
        if ((ity->args->len > 0LL)) {
            /* pass */
            elem_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(ity->args, 0LL)))->name);
        } else if (_tr_dict_contains(self->type_alias_map, ity->name)) {
            /* pass */
            char* _alias_raw = ((char*)(uintptr_t)_tr_dict_get(self->type_alias_map, ity->name));
            /* pass */
            char* _alias_c = _tr_str_strip(_alias_raw);
            /* pass */
            if (_tr_str_ends_with(_alias_c, "*")) {
                /* pass */
                char* _alias_sl = _tr_str_slice(_alias_c, 0LL, (_tr_strlen((char*)_alias_c) - 1LL));
                /* pass */
                _alias_c = _tr_str_strip(_alias_sl);
            }
            /* pass */
            if (_tr_str_starts_with(_alias_c, "List_")) {
                /* pass */
                elem_sfx = _tr_str_slice(_alias_c, 5LL, _tr_strlen((char*)_alias_c));
            }
        }
    } else if (_t209.tag == HirExpr_EMethodCall) {
        __auto_type mty = _t209.data.EMethodCall.ty;
        /* pass */
        if ((mty->args->len > 0LL)) {
            /* pass */
            elem_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(mty->args, 0LL)))->name);
        }
    } else if (_t209.tag == HirExpr_ECall) {
        __auto_type cty = _t209.data.ECall.ty;
        /* pass */
        if ((cty->args->len > 0LL)) {
            /* pass */
            elem_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(cty->args, 0LL)))->name);
        }
    } else if (_t209.tag == HirExpr_EPropAccess) {
        __auto_type pty = _t209.data.EPropAccess.ty;
        /* pass */
        if ((pty->args->len > 0LL)) {
            /* pass */
            elem_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(pty->args, 0LL)))->name);
        }
    } else if (1) {
        __auto_type _ = _t209;
        /* pass */
    }
    /* pass */
    bool is_list_vec = (((strcmp((char*)class_name, (char*)"List") == 0) || (strcmp((char*)class_name, (char*)"Vec") == 0)) || ((strcmp((char*)class_name, (char*)"int") == 0) && (((((((strcmp((char*)method, (char*)"append") == 0) || (strcmp((char*)method, (char*)"push") == 0)) || (strcmp((char*)method, (char*)"pop") == 0)) || (strcmp((char*)method, (char*)"set") == 0)) || (strcmp((char*)method, (char*)"get") == 0)) || (strcmp((char*)method, (char*)"free") == 0)) || (strcmp((char*)method, (char*)"contains") == 0))));
    /* pass */
    if (((!is_list_vec) && (((unsigned long long)(class_name)) != ((unsigned long long)(0LL))))) {
        /* pass */
        char* cp2 = ((char*)(class_name));
        /* pass */
        if (((((((long long)((*(cp2 + 0LL)))) == 86LL) && (((long long)((*(cp2 + 1LL)))) == 101LL)) && (((long long)((*(cp2 + 2LL)))) == 99LL)) && (((long long)((*(cp2 + 3LL)))) == 95LL))) {
            /* pass */
            is_list_vec = true;
        } else if ((((((((long long)((*(cp2 + 0LL)))) == 76LL) && (((long long)((*(cp2 + 1LL)))) == 105LL)) && (((long long)((*(cp2 + 2LL)))) == 115LL)) && (((long long)((*(cp2 + 3LL)))) == 116LL)) && (((long long)((*(cp2 + 4LL)))) == 95LL))) {
            /* pass */
            is_list_vec = true;
        }
    }
    /* pass */
    if (is_list_vec) {
        /* pass */
        if (((strcmp((char*)method, (char*)"append") == 0) || (strcmp((char*)method, (char*)"push") == 0))) {
            /* pass */
            if (((strcmp((char*)elem_sfx, (char*)"ptr") == 0) && (args->len > 0LL))) {
                /* pass */
                char* push_arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
                /* pass */
                if ((_is_int_type(push_arg_ty) || (strcmp((char*)push_arg_ty, (char*)"bool") == 0))) {
                    /* pass */
                    elem_sfx = "i64";
                } else if (_is_float_type(push_arg_ty)) {
                    /* pass */
                    elem_sfx = "f64";
                } else if (((strcmp((char*)push_arg_ty, (char*)"str") == 0) || (strcmp((char*)push_arg_ty, (char*)"char*") == 0))) {
                    /* pass */
                    elem_sfx = "str";
                }
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_append("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"free") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_free("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"pop") == 0)) {
            /* pass */
            char* pop_r = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_pop("), obj_s), ")");
            /* pass */
            if (((((strcmp((char*)elem_sfx, (char*)"ptr") == 0) && (strcmp((char*)call_ty->name, (char*)"") != 0)) && (strcmp((char*)call_ty->name, (char*)"void") != 0)) && (strcmp((char*)call_ty->name, (char*)"Pointer") != 0))) {
                /* pass */
                if ((_tr_strlen(call_ty->name) > 1LL)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", call_ty->name), "*)"), pop_r), ")");
                }
            }
            /* pass */
            return pop_r;
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"set") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_set("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"contains") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_contains("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"get") == 0)) {
            /* pass */
            char* get_r = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_get("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
            /* pass */
            if ((((strcmp((char*)elem_sfx, (char*)"ptr") == 0) && (strcmp((char*)call_ty->name, (char*)"Pointer") == 0)) && (call_ty->args->len > 0LL))) {
                /* pass */
                char* inner_ct = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(call_ty->args, 0LL))));
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", inner_ct), "*)"), get_r), ")");
            }
            /* pass */
            if ((((((((((strcmp((char*)elem_sfx, (char*)"ptr") == 0) && (strcmp((char*)call_ty->name, (char*)"") != 0)) && (strcmp((char*)call_ty->name, (char*)"void") != 0)) && (strcmp((char*)call_ty->name, (char*)"Pointer") != 0)) && (strcmp((char*)call_ty->name, (char*)"List") != 0)) && (strcmp((char*)call_ty->name, (char*)"Vec") != 0)) && (strcmp((char*)call_ty->name, (char*)"Map") != 0)) && (strcmp((char*)call_ty->name, (char*)"Dict") != 0)) && (strcmp((char*)call_ty->name, (char*)"str") != 0))) {
                /* pass */
                if ((_tr_strlen(call_ty->name) > 1LL)) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", call_ty->name), "*)"), get_r), ")");
                }
            }
            /* pass */
            return get_r;
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("((long long)", obj_s), "->len)");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"new") == 0) || (strcmp((char*)method, (char*)"init") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat("(void*)List_", elem_sfx), "_new()");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"remove") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_remove("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"swap") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_swap("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"clear") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_clear("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_empty") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_is_empty("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"extend") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_extend("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"index_of") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_index_of("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sort") == 0) || (strcmp((char*)method, (char*)"sort_asc") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_sort_", elem_sfx), "("), obj_s), ", 1)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sort_desc") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_sort_", elem_sfx), "("), obj_s), ", -1)");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"sort_by") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_sort_by_", elem_sfx), "("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"first") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_get("), obj_s), ", 0LL)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"last") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("List_", elem_sfx), "_get("), obj_s), ", (long long)("), obj_s), "->len - 1LL))");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"any") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_any_", elem_sfx), "("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"all") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_all_", elem_sfx), "("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"sum") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_sum_", elem_sfx), "("), obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"min_val") == 0) || (strcmp((char*)method, (char*)"min") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_min_", elem_sfx), "("), obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"max_val") == 0) || (strcmp((char*)method, (char*)"max") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_max_", elem_sfx), "("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"join") == 0)) {
            /* pass */
            char* sep_s = "\"\"";
            /* pass */
            if ((args->len > 0LL)) {
                /* pass */
                sep_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_join((List_str*)", obj_s), ", "), sep_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"reversed") == 0) || (strcmp((char*)method, (char*)"reversed_copy") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_reversed_", elem_sfx), "("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"reverse") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_reverse_", elem_sfx), "("), obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"clone") == 0) || (strcmp((char*)method, (char*)"copy") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_list_clone_", elem_sfx), "("), obj_s), ")");
        }
    }
    /* pass */
    if ((strcmp((char*)class_name, (char*)"Set") == 0)) {
        /* pass */
        char* set_elem = elem_sfx;
        /* pass */
        if ((strcmp((char*)set_elem, (char*)"ptr") == 0)) {
            /* pass */
            set_elem = "str";
        }
        /* pass */
        char* sfn = "_tr_set_";
        /* pass */
        if ((strcmp((char*)set_elem, (char*)"i64") == 0)) {
            /* pass */
            sfn = "_tr_iset_";
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
            /* pass */
            if ((args->len == 0LL)) {
                /* pass */
                return _tr_str_concat(sfn, "new(16)");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "new("), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"add") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "add("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"contains") == 0) || (strcmp((char*)method, (char*)"has") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "contains("), obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"remove") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "remove("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "len("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_empty") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", sfn), "len("), obj_s), ") == 0LL)");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"clear") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "clear("), obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"to_list") == 0)) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "to_list("), obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"union") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "union("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"intersection") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "intersection("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"difference") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "difference("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"is_subset") == 0) && (args->len > 0LL))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sfn, "is_subset("), obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
    }
    /* pass */
    bool is_map_dict = (((strcmp((char*)class_name, (char*)"Map") == 0) || (strcmp((char*)class_name, (char*)"Dict") == 0)) || (strcmp((char*)class_name, (char*)"TrMap") == 0));
    /* pass */
    if (((!is_map_dict) && (((unsigned long long)(class_name)) != ((unsigned long long)(0LL))))) {
        /* pass */
        char* cp = ((char*)(class_name));
        /* pass */
        if (((((((long long)((*(cp + 0LL)))) == 77LL) && (((long long)((*(cp + 1LL)))) == 97LL)) && (((long long)((*(cp + 2LL)))) == 112LL)) && (((long long)((*(cp + 3LL)))) == 95LL))) {
            /* pass */
            is_map_dict = true;
        } else if ((((((((long long)((*(cp + 0LL)))) == 68LL) && (((long long)((*(cp + 1LL)))) == 105LL)) && (((long long)((*(cp + 2LL)))) == 99LL)) && (((long long)((*(cp + 3LL)))) == 116LL)) && (((long long)((*(cp + 4LL)))) == 95LL))) {
            /* pass */
            is_map_dict = true;
        }
    }
    /* pass */
    if (is_map_dict) {
        /* pass */
        AstType* obj_full_ty = hir_expr_type(obj);
        /* pass */
        char* dict_key_ty = "str";
        /* pass */
        if ((obj_full_ty->args->len > 0LL)) {
            /* pass */
            AstType* ka = (*((AstType**)List_ptr_get(obj_full_ty->args, 0LL)));
            /* pass */
            if (((((_is_int_type(ka->name) || (strcmp((char*)ka->name, (char*)"int") == 0)) || (strcmp((char*)ka->name, (char*)"i64") == 0)) || (strcmp((char*)ka->name, (char*)"i32") == 0)) || (strcmp((char*)ka->name, (char*)"usize") == 0))) {
                /* pass */
                dict_key_ty = "int";
            }
        }
        /* pass */
        bool is_idict = (strcmp((char*)dict_key_ty, (char*)"int") == 0);
        /* pass */
        if (((strcmp((char*)method, (char*)"init") == 0) || (strcmp((char*)method, (char*)"new") == 0))) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_new(", CGenerator_gen_args(self, args)), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_new(", CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"get") == 0)) {
            /* pass */
            char* c_ty_c = CGenerator_type_to_c(self, call_ty);
            /* pass */
            if (is_idict) {
                /* pass */
                char* iget_r = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_idict_get(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
                /* pass */
                if (((strcmp((char*)c_ty_c, (char*)"void") != 0) && (strcmp((char*)c_ty_c, (char*)"") != 0))) {
                    /* pass */
                    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", c_ty_c), ")(uintptr_t)"), iget_r), ")");
                }
                /* pass */
                return iget_r;
            }
            /* pass */
            char* mget_r = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_get(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
            /* pass */
            if (((strcmp((char*)c_ty_c, (char*)"void") != 0) && (strcmp((char*)c_ty_c, (char*)"") != 0))) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("((", c_ty_c), ")(uintptr_t)"), mget_r), ")");
            }
            /* pass */
            return mget_r;
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"set") == 0) || (strcmp((char*)method, (char*)"insert") == 0))) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_idict_set(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_set(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"contains") == 0) || (strcmp((char*)method, (char*)"has") == 0))) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_idict_contains(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_contains(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"remove") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_idict_remove(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_remove(", obj_s), ", "), CGenerator_gen_args(self, args)), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"len") == 0) || (strcmp((char*)method, (char*)"length") == 0))) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_len(", obj_s), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_len(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"keys") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_keys(", obj_s), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_keys(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"values") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_values(", obj_s), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_values(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"items") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_items(", obj_s), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_items(", obj_s), ")");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"update") == 0) && (args->len > 0LL))) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_idict_update(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_dict_update(", obj_s), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"clear") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("_tr_idict_clear(", obj_s), ")");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("_tr_dict_clear(", obj_s), ")");
        }
        /* pass */
        if ((strcmp((char*)method, (char*)"is_empty") == 0)) {
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat("(_tr_idict_len(", obj_s), ") == 0LL)");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat("(_tr_dict_len(", obj_s), ") == 0LL)");
        }
        /* pass */
        if (((strcmp((char*)method, (char*)"get_or") == 0) && (args->len >= 2LL))) {
            /* pass */
            char* _k = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            char* _d = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            if (is_idict) {
                /* pass */
                return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(_tr_idict_contains(", obj_s), ", "), _k), ") ? _tr_idict_get("), obj_s), ", "), _k), ") : (void*)(uintptr_t)("), _d), "))");
            }
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(_tr_dict_contains(", obj_s), ", "), _k), ") ? _tr_dict_get("), obj_s), ", "), _k), ") : (void*)(uintptr_t)("), _d), "))");
        }
    }
    /* pass */
    char* extra_args = "";
    /* pass */
    if ((args->len > 0LL)) {
        /* pass */
        extra_args = _tr_str_concat(", ", CGenerator_gen_args(self, args));
    }
    /* pass */
    char* safe_method = method;
    /* pass */
    if (_is_c_keyword(safe_method)) {
        /* pass */
        safe_method = _tr_str_concat("_tr_fn_", safe_method);
    }
    /* pass */
    if ((((((((strcmp((char*)class_name, (char*)"") != 0) && (strcmp((char*)class_name, (char*)"void") != 0)) && (strcmp((char*)class_name, (char*)"int") != 0)) && (strcmp((char*)class_name, (char*)"float") != 0)) && (strcmp((char*)class_name, (char*)"bool") != 0)) && (strcmp((char*)class_name, (char*)"char") != 0)) && (strcmp((char*)class_name, (char*)"str") != 0))) {
        /* pass */
        if (((!_is_primitive(class_name)) && (!_is_str_type(class_name)))) {
            /* pass */
            return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(class_name, "_"), safe_method), "("), obj_s), extra_args), ")");
        }
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(safe_method, "("), obj_s), extra_args), ")");
}

__attribute__((hot)) char* CGenerator_gen_fstring(CGenerator* self, List_ptr* parts) {
    /* pass */
    if ((parts->len == 0LL)) {
        /* pass */
        return "\"\"";
    }
    /* pass */
    char* fmt = "";
    /* pass */
    char* fargs = "";
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
        /* pass */
        if ((!part->is_expr)) {
            /* pass */
            fmt = _tr_str_concat(fmt, _escape_str_for_c(part->text));
        } else {
            /* pass */
            char* s = CGenerator_gen_expr(self, part->expr);
            /* pass */
            char* ty_n = hir_expr_type(part->expr)->name;
            /* pass */
            char* spec = part->fmt_spec;
            /* pass */
            if ((_tr_strlen((char*)spec) > 0LL)) {
                /* pass */
                long long spec_n = _tr_strlen((char*)spec);
                /* pass */
                long long last_c = _tr_str_char_at_code(spec, (spec_n - 1LL));
                /* pass */
                if ((((((last_c == 102LL) || (last_c == 101LL)) || (last_c == 69LL)) || (last_c == 103LL)) || (last_c == 71LL))) {
                    /* pass */
                    fmt = _tr_str_concat(_tr_str_concat(fmt, "%"), spec);
                    /* pass */
                    fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (double)("), s), ")");
                } else if (((((((last_c == 100LL) || (last_c == 105LL)) || (last_c == 117LL)) || (last_c == 111LL)) || (last_c == 120LL)) || (last_c == 88LL))) {
                    /* pass */
                    fmt = _tr_str_concat(_tr_str_concat(fmt, "%"), spec);
                    /* pass */
                    fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (long long)("), s), ")");
                } else if ((last_c == 115LL)) {
                    /* pass */
                    fmt = _tr_str_concat(_tr_str_concat(fmt, "%"), spec);
                    /* pass */
                    if (_is_int_type(ty_n)) {
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", _tr_int_to_str("), s), ")");
                    } else if (_is_float_type(ty_n)) {
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", _tr_float_to_str("), s), ")");
                    } else {
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (char*)("), s), ")");
                    }
                } else {
                    /* pass */
                    char* _align_spec = spec;
                    /* pass */
                    bool _left_align = false;
                    /* pass */
                    if (_tr_str_starts_with(_align_spec, ">")) {
                        /* pass */
                        _align_spec = _tr_str_slice(_align_spec, 1LL, _tr_strlen((char*)_align_spec));
                    } else if (_tr_str_starts_with(_align_spec, "<")) {
                        /* pass */
                        _align_spec = _tr_str_slice(_align_spec, 1LL, _tr_strlen((char*)_align_spec));
                        /* pass */
                        _left_align = true;
                    } else if (_tr_str_starts_with(_align_spec, "^")) {
                        /* pass */
                        _align_spec = _tr_str_slice(_align_spec, 1LL, _tr_strlen((char*)_align_spec));
                    }
                    /* pass */
                    if (_is_int_type(ty_n)) {
                        /* pass */
                        if (_left_align) {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%-"), _align_spec), "lld");
                        } else {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%"), _align_spec), "lld");
                        }
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (long long)("), s), ")");
                    } else if (_is_float_type(ty_n)) {
                        /* pass */
                        if (_left_align) {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%-"), _align_spec), "g");
                        } else {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%"), _align_spec), "g");
                        }
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (double)("), s), ")");
                    } else {
                        /* pass */
                        if (_left_align) {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%-"), _align_spec), "s");
                        } else {
                            /* pass */
                            fmt = _tr_str_concat(_tr_str_concat(_tr_str_concat(fmt, "%"), _align_spec), "s");
                        }
                        /* pass */
                        fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (char*)("), s), ")");
                    }
                }
            } else if (_is_int_type(ty_n)) {
                /* pass */
                fmt = _tr_str_concat(fmt, "%lld");
                /* pass */
                fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (long long)("), s), ")");
            } else if (_is_float_type(ty_n)) {
                /* pass */
                fmt = _tr_str_concat(fmt, "%g");
                /* pass */
                fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (double)("), s), ")");
            } else if ((strcmp((char*)ty_n, (char*)"bool") == 0)) {
                /* pass */
                fmt = _tr_str_concat(fmt, "%s");
                /* pass */
                fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (("), s), ") ? \"true\" : \"false\")");
            } else if ((strcmp((char*)ty_n, (char*)"char") == 0)) {
                /* pass */
                fmt = _tr_str_concat(fmt, "%c");
                /* pass */
                fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (char)("), s), ")");
            } else if (((strcmp((char*)ty_n, (char*)"void") == 0) || (strcmp((char*)ty_n, (char*)"") == 0))) {
                /* pass */
                fmt = _tr_str_concat(fmt, "%s");
                /* pass */
                fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", _TR_AUTO_STR("), s), ")");
            } else {
                /* pass */
                fmt = _tr_str_concat(fmt, "%s");
                /* pass */
                if (CGenerator_has_method(self, ty_n, "__str__")) {
                    /* pass */
                    fargs = _tr_str_concat(_tr_str_concat(fargs, ", "), CGenerator_cls_method_c_call(self, ty_n, "__str__", s, ""));
                } else if (CGenerator_has_method(self, ty_n, "__repr__")) {
                    /* pass */
                    fargs = _tr_str_concat(_tr_str_concat(fargs, ", "), CGenerator_cls_method_c_call(self, ty_n, "__repr__", s, ""));
                } else {
                    /* pass */
                    fargs = _tr_str_concat(_tr_str_concat(_tr_str_concat(fargs, ", (char*)("), s), ")");
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ int _fz = snprintf(NULL,0,\"", fmt), "\""), fargs), "); char* _fr=(char*)_tr_checked_alloc(_fz+1); snprintf(_fr,_fz+1,\""), fmt), "\""), fargs), "); _fr; })");
}

__attribute__((hot)) char* CGenerator_gen_tuple(CGenerator* self, List_ptr* items) {
    /* pass */
    if ((items->len == 0LL)) {
        /* pass */
        return "((TrTuple){.data={0}})";
    }
    /* pass */
    char* s = "((TrTuple){.data={";
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < items->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            s = _tr_str_concat(s, ", ");
        }
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "(long long)(uintptr_t)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(items, i)))), ")");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_concat(s, "}})");
}

__attribute__((hot)) void CGenerator_emit_async_wrapper_for_call(CGenerator* self, char* fn_name, List_ptr* args, char* ret_name) {
    /* pass */
    if (_tr_dict_contains(self->async_wrappers, fn_name)) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->async_wrappers, fn_name, true);
    /* pass */
    long long nargs = args->len;
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat("static void* _tr_async_wrap_", fn_name), "(void* _vp) {\n"));
    /* pass */
    CGenerator_w(self, "    long long* _a = (long long*)_vp;\n");
    /* pass */
    CGenerator_w(self, "    _TrTaskState* _ts = (_TrTaskState*)(uintptr_t)_a[0];\n");
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < nargs)) {
        /* pass */
        char* arg_ty_name = hir_expr_type(((HirExpr*)List_ptr_get(args, ci)))->name;
        /* pass */
        char* c_arg_ty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(args, ci))));
        /* pass */
        if (((strcmp((char*)c_arg_ty, (char*)"void") == 0) || (strcmp((char*)c_arg_ty, (char*)"") == 0))) {
            /* pass */
            c_arg_ty = "long long";
        }
        /* pass */
        bool is_int = ((_is_int_type(arg_ty_name) || (strcmp((char*)arg_ty_name, (char*)"bool") == 0)) || (strcmp((char*)arg_ty_name, (char*)"char") == 0));
        /* pass */
        if (is_int) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", c_arg_ty), " _arg"), _tr_int_to_str((long long)(ci))), " = ("), c_arg_ty), ")_a["), _tr_int_to_str((long long)((ci + 1LL)))), "];\n"));
        } else {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", c_arg_ty), " _arg"), _tr_int_to_str((long long)(ci))), " = ("), c_arg_ty), ")(uintptr_t)_a["), _tr_int_to_str((long long)((ci + 1LL)))), "];\n"));
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    char* call_s = _tr_str_concat(fn_name, "(");
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < nargs)) {
        /* pass */
        if ((ai > 0LL)) {
            /* pass */
            call_s = _tr_str_concat(call_s, ", ");
        }
        /* pass */
        call_s = _tr_str_concat(_tr_str_concat(call_s, "_arg"), _tr_int_to_str((long long)(ai)));
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    call_s = _tr_str_concat(call_s, ")");
    /* pass */
    if (((strcmp((char*)ret_name, (char*)"void") == 0) || (strcmp((char*)ret_name, (char*)"") == 0))) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    ", call_s), ";\n"));
        /* pass */
        CGenerator_w(self, "    _tr_task_complete(_ts, 0LL);\n");
    } else {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    long long _r = (long long)(uintptr_t)(", call_s), ");\n"));
        /* pass */
        CGenerator_w(self, "    _tr_task_complete(_ts, _r);\n");
    }
    /* pass */
    CGenerator_w(self, "    free(_a);\n");
    /* pass */
    CGenerator_w(self, "    return NULL;\n");
    /* pass */
    CGenerator_w(self, "}\n");
}

__attribute__((hot)) char* CGenerator_gen_await_call(CGenerator* self, HirExpr* expr) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(expr)))) {
        /* pass */
        return "0LL";
    }
    /* pass */
    __auto_type _t210 = (*expr);
    if (_t210.tag == HirExpr_ECall) {
        __auto_type callee = _t210.data.ECall.callee;
__auto_type args = _t210.data.ECall.args;
__auto_type call_ty = _t210.data.ECall.ty;
        /* pass */
        char* fn_name = "";
        /* pass */
        __auto_type _t211 = (*callee);
        if (_t211.tag == HirExpr_EIdent) {
            __auto_type n = _t211.data.EIdent.name;
            fn_name = n;
        } else if (1) {
            __auto_type _ = _t211;
            /* pass */
        }
        /* pass */
        if ((strcmp((char*)fn_name, (char*)"") == 0)) {
            /* pass */
            return CGenerator_gen_expr(self, expr);
        }
        /* pass */
        char* ret_name = call_ty->name;
        /* pass */
        if ((strcmp((char*)ret_name, (char*)"") == 0)) {
            /* pass */
            ret_name = "void";
        }
        /* pass */
        CGenerator_emit_async_wrapper_for_call(self, fn_name, args, ret_name);
        /* pass */
        long long nargs = args->len;
        /* pass */
        char* tmp = CGenerator_next_temp(self);
        /* pass */
        char* buf_size = _tr_int_to_str((long long)((nargs + 1LL)));
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long* _ab", tmp), " = (long long*)_tr_checked_alloc("), buf_size), " * sizeof(long long)); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_TrTaskState* _ts"), tmp), " = _tr_task_new(); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "[0] = (long long)(uintptr_t)_ts"), tmp), "; ");
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < nargs)) {
            /* pass */
            char* arg_ty_name = hir_expr_type(((HirExpr*)List_ptr_get(args, ai)))->name;
            /* pass */
            bool is_int = ((_is_int_type(arg_ty_name) || (strcmp((char*)arg_ty_name, (char*)"bool") == 0)) || (strcmp((char*)arg_ty_name, (char*)"char") == 0));
            /* pass */
            if (is_int) {
                /* pass */
                s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "["), _tr_int_to_str((long long)((ai + 1LL)))), "] = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, ai)))), "); ");
            } else {
                /* pass */
                s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "["), _tr_int_to_str((long long)((ai + 1LL)))), "] = (long long)(uintptr_t)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, ai)))), "); ");
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_async_pool_submit(_tr_global_async_pool, _tr_async_wrap_"), fn_name), ", _ab"), tmp), "); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "long long _r"), tmp), " = _tr_task_await(_ts"), tmp), "); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_task_free(_ts"), tmp), "); ");
        /* pass */
        if ((strcmp((char*)ret_name, (char*)"void") == 0)) {
            /* pass */
            s = _tr_str_concat(s, "0LL; })");
        } else if (((_is_int_type(ret_name) || (strcmp((char*)ret_name, (char*)"bool") == 0)) || (strcmp((char*)ret_name, (char*)"char") == 0))) {
            /* pass */
            char* c_ret = CGenerator_type_to_c(self, call_ty);
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "("), c_ret), ")_r"), tmp), "; })");
        } else {
            /* pass */
            char* c_ret2 = CGenerator_type_to_c(self, call_ty);
            /* pass */
            if (((strcmp((char*)c_ret2, (char*)"void") == 0) || (strcmp((char*)c_ret2, (char*)"") == 0))) {
                /* pass */
                c_ret2 = "void*";
            }
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "("), c_ret2), ")(uintptr_t)_r"), tmp), "; })");
        }
        /* pass */
        return s;
    } else if (1) {
        __auto_type _ = _t210;
        /* pass */
        return CGenerator_gen_expr(self, expr);
    }
}

__attribute__((hot)) char* CGenerator_gen_await_timeout_call(CGenerator* self, HirExpr* expr, HirExpr* ms_expr) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(expr)))) {
        /* pass */
        return "0LL";
    }
    /* pass */
    __auto_type _t212 = (*expr);
    if (_t212.tag == HirExpr_ECall) {
        __auto_type callee = _t212.data.ECall.callee;
__auto_type args = _t212.data.ECall.args;
__auto_type call_ty = _t212.data.ECall.ty;
        /* pass */
        char* fn_name = "";
        /* pass */
        __auto_type _t213 = (*callee);
        if (_t213.tag == HirExpr_EIdent) {
            __auto_type n = _t213.data.EIdent.name;
            fn_name = n;
        } else if (1) {
            __auto_type _ = _t213;
            /* pass */
        }
        /* pass */
        if ((strcmp((char*)fn_name, (char*)"") == 0)) {
            /* pass */
            return CGenerator_gen_expr(self, expr);
        }
        /* pass */
        char* ret_name = call_ty->name;
        /* pass */
        if ((strcmp((char*)ret_name, (char*)"") == 0)) {
            /* pass */
            ret_name = "void";
        }
        /* pass */
        CGenerator_emit_async_wrapper_for_call(self, fn_name, args, ret_name);
        /* pass */
        long long nargs = args->len;
        /* pass */
        char* tmp = CGenerator_next_temp(self);
        /* pass */
        char* buf_size = _tr_int_to_str((long long)((nargs + 1LL)));
        /* pass */
        char* ms_s = CGenerator_gen_expr(self, ms_expr);
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ long long* _ab", tmp), " = (long long*)_tr_checked_alloc("), buf_size), " * sizeof(long long)); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_TrTaskState* _ts"), tmp), " = _tr_task_new(); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "[0] = (long long)(uintptr_t)_ts"), tmp), "; ");
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < nargs)) {
            /* pass */
            char* arg_ty_name = hir_expr_type(((HirExpr*)List_ptr_get(args, ai)))->name;
            /* pass */
            bool is_int = ((_is_int_type(arg_ty_name) || (strcmp((char*)arg_ty_name, (char*)"bool") == 0)) || (strcmp((char*)arg_ty_name, (char*)"char") == 0));
            /* pass */
            if (is_int) {
                /* pass */
                s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "["), _tr_int_to_str((long long)((ai + 1LL)))), "] = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, ai)))), "); ");
            } else {
                /* pass */
                s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_ab"), tmp), "["), _tr_int_to_str((long long)((ai + 1LL)))), "] = (long long)(uintptr_t)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, ai)))), "); ");
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_async_pool_submit(_tr_global_async_pool, _tr_async_wrap_"), fn_name), ", _ab"), tmp), "); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "long long _rto"), tmp), " = 0LL; ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_task_await_timeout(_ts"), tmp), ", (long long)("), ms_s), "), &_rto"), tmp), "); ");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_task_free(_ts"), tmp), "); ");
        /* pass */
        if ((strcmp((char*)ret_name, (char*)"void") == 0)) {
            /* pass */
            s = _tr_str_concat(s, "0LL; })");
        } else if (((_is_int_type(ret_name) || (strcmp((char*)ret_name, (char*)"bool") == 0)) || (strcmp((char*)ret_name, (char*)"char") == 0))) {
            /* pass */
            char* c_ret = CGenerator_type_to_c(self, call_ty);
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "("), c_ret), ")_rto"), tmp), "; })");
        } else {
            /* pass */
            char* c_ret2 = CGenerator_type_to_c(self, call_ty);
            /* pass */
            if (((strcmp((char*)c_ret2, (char*)"void") == 0) || (strcmp((char*)c_ret2, (char*)"") == 0))) {
                /* pass */
                c_ret2 = "void*";
            }
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "("), c_ret2), ")(uintptr_t)_rto"), tmp), "; })");
        }
        /* pass */
        return s;
    } else if (1) {
        __auto_type _ = _t212;
        /* pass */
        return CGenerator_gen_expr(self, expr);
    }
}

__attribute__((hot)) void CGenerator_gen_multi_let(CGenerator* self, List_str* names, bool is_mut, HirExpr* val, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* tmp = _tr_str_concat("_tup", CGenerator_next_temp(self));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "TrTuple "), tmp), " = "), CGenerator_gen_expr(self, val)), ";\n"));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < names->len)) {
        /* pass */
        char* vname = _safe_c_varname(List_str_get(names, i));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "long long "), vname), " = "), tmp), ".data["), _tr_int_to_str((long long)(i))), "];\n"));
        /* pass */
        _tr_dict_set(self->decl_vars, List_str_get(names, i), true);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) char* CGenerator_gen_list_literal(CGenerator* self, List_ptr* items, AstType* ty) {
    /* pass */
    char* sfx = "ptr";
    /* pass */
    if ((ty->args->len > 0LL)) {
        /* pass */
        sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
    } else if ((items->len > 0LL)) {
        /* pass */
        sfx = CGenerator_list_elem_suffix(self, hir_expr_type(((HirExpr*)List_ptr_get(items, 0LL)))->name);
    }
    /* pass */
    char* l = _tr_str_concat("_l_", CGenerator_next_temp(self));
    /* pass */
    char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ List_", sfx), "* "), l), " = List_"), sfx), "_new(); ");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < items->len)) {
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "List_"), sfx), "_append("), l), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(items, i)))), "); ");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(s, l), "; })");
}

__attribute__((hot)) char* CGenerator_gen_dict_literal(CGenerator* self, List_ptr* keys, List_ptr* vals, AstType* hint_ty) {
    /* pass */
    bool is_int_key = false;
    /* pass */
    if ((hint_ty->args->len > 0LL)) {
        /* pass */
        AstType* ka_h = (*((AstType**)List_ptr_get(hint_ty->args, 0LL)));
        /* pass */
        if (((((_is_int_type(ka_h->name) || (strcmp((char*)ka_h->name, (char*)"int") == 0)) || (strcmp((char*)ka_h->name, (char*)"i64") == 0)) || (strcmp((char*)ka_h->name, (char*)"i32") == 0)) || (strcmp((char*)ka_h->name, (char*)"usize") == 0))) {
            /* pass */
            is_int_key = true;
        }
    }
    /* pass */
    if ((keys->len == 0LL)) {
        /* pass */
        if (is_int_key) {
            /* pass */
            return "_tr_idict_new(0LL)";
        }
        /* pass */
        return "_tr_dict_new(0LL)";
    }
    /* pass */
    if (((!is_int_key) && (keys->len > 0LL))) {
        /* pass */
        char* k0_ty = hir_expr_type(((HirExpr*)List_ptr_get(keys, 0LL)))->name;
        /* pass */
        if (((_is_int_type(k0_ty) || (strcmp((char*)k0_ty, (char*)"int") == 0)) || (strcmp((char*)k0_ty, (char*)"i64") == 0))) {
            /* pass */
            is_int_key = true;
        }
    }
    /* pass */
    char* tmp = CGenerator_next_temp(self);
    /* pass */
    if (is_int_key) {
        /* pass */
        char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ TrIDict* ", tmp), " = _tr_idict_new("), _tr_int_to_str((long long)(keys->len))), "LL); ");
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_idict_set("), tmp), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(keys, i)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(vals, i)))), "); ");
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        s = _tr_str_concat(_tr_str_concat(s, tmp), "; })");
        /* pass */
        return s;
    }
    /* pass */
    char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ TrMap* ", tmp), " = _tr_dict_new("), _tr_int_to_str((long long)(keys->len))), "LL); ");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < keys->len)) {
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_tr_dict_set("), tmp), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(keys, i)))), ", "), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(vals, i)))), "); ");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    s = _tr_str_concat(_tr_str_concat(s, tmp), "; })");
    /* pass */
    return s;
}

__attribute__((hot)) char* CGenerator_gen_list_comp(CGenerator* self, HirExpr* element, List_ptr* generators) {
    /* pass */
    char* tmp = CGenerator_next_temp(self);
    /* pass */
    char* elem_ty_n = hir_expr_type(element)->name;
    /* pass */
    char* sfx = CGenerator_list_elem_suffix(self, elem_ty_n);
    /* pass */
    char* s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("({ List_", sfx), "* "), tmp), " = List_"), sfx), "_new(); ");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < generators->len)) {
        /* pass */
        HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
        /* pass */
        char* iter_s = CGenerator_gen_expr(self, gen->iter);
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "{ __auto_type _gc"), _tr_int_to_str((long long)(i))), " = "), iter_s), "; long long _gi"), _tr_int_to_str((long long)(i))), " = 0;");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, " while (_gi"), _tr_int_to_str((long long)(i))), " < _gc"), _tr_int_to_str((long long)(i))), "->len) {");
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, " __auto_type "), gen->target), " = _gc"), _tr_int_to_str((long long)(i))), "->data[_gi"), _tr_int_to_str((long long)(i))), "]; ");
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < gen->ifs->len)) {
            /* pass */
            s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "if ("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(gen->ifs, fi)))), ") { ");
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(s, "List_"), sfx), "_append("), tmp), ", "), CGenerator_gen_expr(self, element)), "); ");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < generators->len)) {
        /* pass */
        HirComprehension* gen2 = (*((HirComprehension**)List_ptr_get(generators, i)));
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < gen2->ifs->len)) {
            /* pass */
            s = _tr_str_concat(s, "} ");
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        s = _tr_str_concat(_tr_str_concat(_tr_str_concat(s, "_gi"), _tr_int_to_str((long long)(i))), "++; } } ");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_concat(_tr_str_concat(s, tmp), "; })");
}

__attribute__((hot)) char* CGenerator_gen_closure(CGenerator* self, List_ptr* params, AstType* ret_ty, HirBlock* body, List_ptr* captures) {
    /* pass */
    self->closure_count = (self->closure_count + 1LL);
    /* pass */
    char* cname = _tr_str_concat("_closure_", _tr_int_to_str((long long)(self->closure_count)));
    /* pass */
    char* ret_c = CGenerator_type_to_c(self, ret_ty);
    /* pass */
    if ((strcmp((char*)ret_c, (char*)"void") == 0)) {
        /* pass */
        ret_c = "long long";
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < params->len)) {
        /* pass */
        _tr_dict_set(self->decl_vars, ((HirParam*)List_ptr_get(params, pi))->name, true);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < captures->len)) {
        /* pass */
        _tr_dict_set(self->decl_vars, ((HirParam*)List_ptr_get(captures, ci))->name, true);
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", ret_c), " "), cname), "("));
    /* pass */
    bool first = true;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(params, i));
        /* pass */
        if ((!first)) {
            /* pass */
            CGenerator_w(self, ", ");
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_type_to_c(self, p->ty), " "), _safe_c_varname(p->name)));
        /* pass */
        first = false;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_w(self, ") {\n");
    /* pass */
    CGenerator_gen_block(self, body, 2LL);
    /* pass */
    CGenerator_w(self, "    }\n");
    /* pass */
    return _tr_str_concat(_tr_str_concat("(void*)(&", cname), ")");
}

__attribute__((hot)) void CGenerator_emit_spawn_wrapper_for_expr(CGenerator* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t214 = (*e);
    if (_t214.tag == HirExpr_ECall) {
        __auto_type callee = _t214.data.ECall.callee;
__auto_type args = _t214.data.ECall.args;
__auto_type ew_ret_ty = _t214.data.ECall.ty;
        /* pass */
        char* fn_name = "";
        /* pass */
        __auto_type _t215 = (*callee);
        if (_t215.tag == HirExpr_EIdent) {
            __auto_type n = _t215.data.EIdent.name;
            fn_name = n;
        } else if (1) {
            __auto_type _ = _t215;
            return;
        }
        /* pass */
        if ((strcmp((char*)fn_name, (char*)"") == 0)) {
            /* pass */
            return;
        }
        /* pass */
        if (_tr_dict_contains(self->spawn_wrappers, fn_name)) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(self->spawn_wrappers, fn_name, true);
        /* pass */
        if ((args->len == 0LL)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", fn_name), "(void* _vp) { (void)_vp; "), fn_name), "(); return NULL; }\n"));
        } else if ((args->len == 1LL)) {
            /* pass */
            char* arg_ty_n = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
            /* pass */
            char* cast_back = "(long long)(uintptr_t)_vp";
            /* pass */
            if ((((!_is_int_type(arg_ty_n)) && (strcmp((char*)arg_ty_n, (char*)"bool") != 0)) && (strcmp((char*)arg_ty_n, (char*)"char") != 0))) {
                /* pass */
                char* c_arg_ty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL))));
                /* pass */
                cast_back = _tr_str_concat(_tr_str_concat("(", c_arg_ty), ")_vp");
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", fn_name), "(void* _vp) { "), fn_name), "("), cast_back), "); return NULL; }\n"));
        } else {
            /* pass */
            long long ma_nargs = args->len;
            /* pass */
            bool ew_is_void = ((strcmp((char*)ew_ret_ty->name, (char*)"void") == 0) || (strcmp((char*)ew_ret_ty->name, (char*)"") == 0));
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", fn_name), "(void* _vp) {\n"));
            /* pass */
            CGenerator_w(self, "    long long* _ab = (long long*)_vp;\n");
            /* pass */
            char* ma_call_s = "";
            /* pass */
            long long ma_ai = 0LL;
            /* pass */
            while ((ma_ai < ma_nargs)) {
                /* pass */
                if ((ma_ai > 0LL)) {
                    /* pass */
                    ma_call_s = _tr_str_concat(ma_call_s, ", ");
                }
                /* pass */
                char* ma_atn = hir_expr_type(((HirExpr*)List_ptr_get(args, ma_ai)))->name;
                /* pass */
                if ((((!_is_int_type(ma_atn)) && (strcmp((char*)ma_atn, (char*)"bool") != 0)) && (strcmp((char*)ma_atn, (char*)"char") != 0))) {
                    /* pass */
                    char* ma_cty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(args, ma_ai))));
                    /* pass */
                    ma_call_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(ma_call_s, "("), ma_cty), ")_ab["), _tr_int_to_str((long long)((ma_ai + 1LL)))), "]");
                } else {
                    /* pass */
                    ma_call_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(ma_call_s, "_ab["), _tr_int_to_str((long long)((ma_ai + 1LL)))), "]");
                }
                /* pass */
                ma_ai = (ma_ai + 1LL);
            }
            /* pass */
            if (ew_is_void) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", fn_name), "("), ma_call_s), ");\n"));
            } else {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    _ab[0] = (long long)", fn_name), "("), ma_call_s), ");\n"));
            }
            /* pass */
            CGenerator_w(self, "    free(_ab); return NULL;\n");
            /* pass */
            CGenerator_w(self, "}\n");
        }
    } else if (1) {
        __auto_type _ = _t214;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_prescan_block_spawns(CGenerator* self, HirBlock* block) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block->stmts)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block->stmts->data)))) {
        /* pass */
        return;
    }
    /* pass */
    long long blen = block->stmts->len;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < blen)) {
        /* pass */
        HirStmt* sp = ((HirStmt*)List_ptr_get(block->stmts, i));
        /* pass */
        CGenerator_prescan_stmt_spawns(self, sp);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_prescan_stmt_spawns(CGenerator* self, HirStmt* s) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(s)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t216 = (*s);
    if (_t216.tag == HirStmt_SSpawn) {
        __auto_type e = _t216.data.SSpawn.expr;
        /* pass */
        CGenerator_emit_spawn_wrapper_for_expr(self, e);
    } else if (_t216.tag == HirStmt_STaskGroup) {
        __auto_type body = _t216.data.STaskGroup.body;
        /* pass */
        CGenerator_prescan_block_spawns(self, body);
    } else if (_t216.tag == HirStmt_SIf) {
        __auto_type t = _t216.data.SIf.then_b;
__auto_type e = _t216.data.SIf.else_b;
        /* pass */
        CGenerator_prescan_block_spawns(self, t);
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(e))))) {
            /* pass */
            CGenerator_prescan_block_spawns(self, e);
        }
    } else if (_t216.tag == HirStmt_SWhile) {
        __auto_type body = _t216.data.SWhile.body;
        /* pass */
        CGenerator_prescan_block_spawns(self, body);
    } else if (_t216.tag == HirStmt_SFor) {
        __auto_type body = _t216.data.SFor.body;
        /* pass */
        CGenerator_prescan_block_spawns(self, body);
    } else if (_t216.tag == HirStmt_SForUnpack) {
        __auto_type body = _t216.data.SForUnpack.body;
        /* pass */
        CGenerator_prescan_block_spawns(self, body);
    } else if (_t216.tag == HirStmt_STry) {
        __auto_type tb = _t216.data.STry.try_body;
__auto_type catches = _t216.data.STry.catches;
__auto_type fin = _t216.data.STry.finally_b;
        /* pass */
        CGenerator_prescan_block_spawns(self, tb);
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(fin))))) {
            /* pass */
            CGenerator_prescan_block_spawns(self, fin);
        }
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            HirCatchClause* cc = (*((HirCatchClause**)List_ptr_get(catches, ci)));
            /* pass */
            CGenerator_prescan_block_spawns(self, cc->body);
            /* pass */
            ci = (ci + 1LL);
        }
    } else if (_t216.tag == HirStmt_SExpr) {
        __auto_type se = _t216.data.SExpr.expr;
        /* pass */
        CGenerator_prescan_await_all_in_expr(self, se);
    } else if (_t216.tag == HirStmt_SLet) {
        __auto_type se_v = _t216.data.SLet.val;
        /* pass */
        CGenerator_prescan_await_all_in_expr(self, se_v);
    } else if (_t216.tag == HirStmt_SReturn) {
        __auto_type se_r = _t216.data.SReturn.val;
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(se_r))))) {
            /* pass */
            CGenerator_prescan_await_all_in_expr(self, se_r);
        }
    } else if (1) {
        __auto_type _ = _t216;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_prescan_await_all_in_expr(CGenerator* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t217 = (*e);
    if (_t217.tag == HirExpr_ECall) {
        __auto_type aa_callee = _t217.data.ECall.callee;
__auto_type aa_args = _t217.data.ECall.args;
        /* pass */
        char* aa_fn_n = "";
        /* pass */
        __auto_type _t218 = (*aa_callee);
        if (_t218.tag == HirExpr_EIdent) {
            __auto_type nn = _t218.data.EIdent.name;
            aa_fn_n = nn;
        } else if (_t218.tag == HirExpr_EPropAccess) {
            __auto_type th_obj2 = _t218.data.EPropAccess.obj;
__auto_type th_var2 = _t218.data.EPropAccess.prop;
            /* pass */
            char* th_ctor2 = "";
            /* pass */
            __auto_type _t219 = (*th_obj2);
            if (_t219.tag == HirExpr_EIdent) {
                __auto_type thn2 = _t219.data.EIdent.name;
                th_ctor2 = thn2;
            } else if (1) {
                __auto_type _ = _t219;
                /* pass */
            }
            /* pass */
            if (((strcmp((char*)th_ctor2, (char*)"Thread") == 0) && ((strcmp((char*)th_var2, (char*)"spawn") == 0) || (strcmp((char*)th_var2, (char*)"new") == 0)))) {
                /* pass */
                if ((aa_args->len >= 1LL)) {
                    /* pass */
                    char* th2_fn_nm = "";
                    /* pass */
                    __auto_type _t220 = (*((HirExpr*)List_ptr_get(aa_args, 0LL)));
                    if (_t220.tag == HirExpr_EIdent) {
                        __auto_type tspn2 = _t220.data.EIdent.name;
                        th2_fn_nm = tspn2;
                    } else if (1) {
                        __auto_type _ = _t220;
                        /* pass */
                    }
                    /* pass */
                    if (((strcmp((char*)th2_fn_nm, (char*)"") != 0) && (!_tr_dict_contains(self->spawn_wrappers, th2_fn_nm)))) {
                        /* pass */
                        _tr_dict_set(self->spawn_wrappers, th2_fn_nm, true);
                        /* pass */
                        if ((aa_args->len == 1LL)) {
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", th2_fn_nm), "(void* _vp) { (void)_vp; "), th2_fn_nm), "(); return NULL; }\n"));
                        } else {
                            /* pass */
                            char* th2_sp_aty = hir_expr_type(((HirExpr*)List_ptr_get(aa_args, 1LL)))->name;
                            /* pass */
                            char* th2_sp_cast = "(long long)(uintptr_t)_vp";
                            /* pass */
                            if ((((!_is_int_type(th2_sp_aty)) && (strcmp((char*)th2_sp_aty, (char*)"bool") != 0)) && (strcmp((char*)th2_sp_aty, (char*)"char") != 0))) {
                                /* pass */
                                char* th2_sp_cty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(aa_args, 1LL))));
                                /* pass */
                                th2_sp_cast = _tr_str_concat(_tr_str_concat("(", th2_sp_cty), ")_vp");
                            }
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", th2_fn_nm), "(void* _vp) { "), th2_fn_nm), "("), th2_sp_cast), "); return NULL; }\n"));
                        }
                    }
                }
            }
        } else if (1) {
            __auto_type _ = _t218;
            /* pass */
        }
        /* pass */
        if ((strcmp((char*)aa_fn_n, (char*)"await_all") == 0)) {
            /* pass */
            long long aa_pi = 0LL;
            /* pass */
            while ((aa_pi < aa_args->len)) {
                /* pass */
                CGenerator_emit_spawn_wrapper_for_expr(self, ((HirExpr*)List_ptr_get(aa_args, aa_pi)));
                /* pass */
                aa_pi = (aa_pi + 1LL);
            }
        }
    } else if (_t217.tag == HirExpr_EMethodCall) {
        __auto_type pool_obj = _t217.data.EMethodCall.obj;
__auto_type pool_meth = _t217.data.EMethodCall.method;
__auto_type pool_args = _t217.data.EMethodCall.args;
        /* pass */
        if ((strcmp((char*)pool_meth, (char*)"spawn") == 0)) {
            /* pass */
            char* th_ps_nm = "";
            /* pass */
            __auto_type _t221 = (*pool_obj);
            if (_t221.tag == HirExpr_EIdent) {
                __auto_type tpn2 = _t221.data.EIdent.name;
                th_ps_nm = tpn2;
            } else if (1) {
                __auto_type _ = _t221;
                /* pass */
            }
            /* pass */
            if (((strcmp((char*)th_ps_nm, (char*)"Thread") == 0) && (pool_args->len >= 1LL))) {
                /* pass */
                char* th_ps_fn = "";
                /* pass */
                __auto_type _t222 = (*((HirExpr*)List_ptr_get(pool_args, 0LL)));
                if (_t222.tag == HirExpr_EIdent) {
                    __auto_type tpfn = _t222.data.EIdent.name;
                    th_ps_fn = tpfn;
                } else if (1) {
                    __auto_type _ = _t222;
                    /* pass */
                }
                /* pass */
                if (((strcmp((char*)th_ps_fn, (char*)"") != 0) && (!_tr_dict_contains(self->spawn_wrappers, th_ps_fn)))) {
                    /* pass */
                    _tr_dict_set(self->spawn_wrappers, th_ps_fn, true);
                    /* pass */
                    if ((pool_args->len == 1LL)) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", th_ps_fn), "(void* _vp) { (void)_vp; "), th_ps_fn), "(); return NULL; }\n"));
                    } else if ((pool_args->len == 2LL)) {
                        /* pass */
                        char* th_ps_aty = hir_expr_type(((HirExpr*)List_ptr_get(pool_args, 1LL)))->name;
                        /* pass */
                        char* th_ps_cast = "(long long)(uintptr_t)_vp";
                        /* pass */
                        if ((((!_is_int_type(th_ps_aty)) && (strcmp((char*)th_ps_aty, (char*)"bool") != 0)) && (strcmp((char*)th_ps_aty, (char*)"char") != 0))) {
                            /* pass */
                            char* th_ps_cty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(pool_args, 1LL))));
                            /* pass */
                            th_ps_cast = _tr_str_concat(_tr_str_concat("(", th_ps_cty), ")_vp");
                        }
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", th_ps_fn), "(void* _vp) { "), th_ps_fn), "("), th_ps_cast), "); return NULL; }\n"));
                    } else {
                        /* pass */
                        long long th_ps_nfnargs = (pool_args->len - 1LL);
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", th_ps_fn), "(void* _vp) {\n"));
                        /* pass */
                        CGenerator_w(self, "    long long* _ab = (long long*)_vp;\n");
                        /* pass */
                        char* th_ps_cs = "";
                        /* pass */
                        long long th_ps_ii = 0LL;
                        /* pass */
                        while ((th_ps_ii < th_ps_nfnargs)) {
                            /* pass */
                            if ((th_ps_ii > 0LL)) {
                                /* pass */
                                th_ps_cs = _tr_str_concat(th_ps_cs, ", ");
                            }
                            /* pass */
                            char* th_ps_atn = hir_expr_type(((HirExpr*)List_ptr_get(pool_args, (th_ps_ii + 1LL))))->name;
                            /* pass */
                            if ((((!_is_int_type(th_ps_atn)) && (strcmp((char*)th_ps_atn, (char*)"bool") != 0)) && (strcmp((char*)th_ps_atn, (char*)"char") != 0))) {
                                /* pass */
                                char* th_ps_cty2 = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(pool_args, (th_ps_ii + 1LL)))));
                                /* pass */
                                th_ps_cs = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(th_ps_cs, "("), th_ps_cty2), ")_ab["), _tr_int_to_str((long long)(th_ps_ii))), "]");
                            } else {
                                /* pass */
                                th_ps_cs = _tr_str_concat(_tr_str_concat(_tr_str_concat(th_ps_cs, "_ab["), _tr_int_to_str((long long)(th_ps_ii))), "]");
                            }
                            /* pass */
                            th_ps_ii = (th_ps_ii + 1LL);
                        }
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", th_ps_fn), "("), th_ps_cs), ");\n"));
                        /* pass */
                        CGenerator_w(self, "    free(_ab); return NULL;\n");
                        /* pass */
                        CGenerator_w(self, "}\n");
                    }
                }
            }
        }
        /* pass */
        if (((strcmp((char*)pool_meth, (char*)"spawn") == 0) && (strcmp((char*)hir_expr_type(pool_obj)->name, (char*)"ThreadPool") == 0))) {
            /* pass */
            if ((pool_args->len >= 1LL)) {
                /* pass */
                char* sp_fn_nm = "";
                /* pass */
                __auto_type _t223 = (*((HirExpr*)List_ptr_get(pool_args, 0LL)));
                if (_t223.tag == HirExpr_EIdent) {
                    __auto_type spn = _t223.data.EIdent.name;
                    sp_fn_nm = spn;
                } else if (1) {
                    __auto_type _ = _t223;
                    /* pass */
                }
                /* pass */
                if ((strcmp((char*)sp_fn_nm, (char*)"") != 0)) {
                    /* pass */
                    if ((!_tr_dict_contains(self->spawn_wrappers, sp_fn_nm))) {
                        /* pass */
                        _tr_dict_set(self->spawn_wrappers, sp_fn_nm, true);
                        /* pass */
                        if ((pool_args->len == 1LL)) {
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", sp_fn_nm), "(void* _vp) { (void)_vp; "), sp_fn_nm), "(); return NULL; }\n"));
                        } else if ((pool_args->len == 2LL)) {
                            /* pass */
                            char* sp_aty = hir_expr_type(((HirExpr*)List_ptr_get(pool_args, 1LL)))->name;
                            /* pass */
                            char* sp_cast = "(long long)(uintptr_t)_vp";
                            /* pass */
                            if ((((!_is_int_type(sp_aty)) && (strcmp((char*)sp_aty, (char*)"bool") != 0)) && (strcmp((char*)sp_aty, (char*)"char") != 0))) {
                                /* pass */
                                char* sp_cty = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(pool_args, 1LL))));
                                /* pass */
                                sp_cast = _tr_str_concat(_tr_str_concat("(", sp_cty), ")_vp");
                            }
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", sp_fn_nm), "(void* _vp) { "), sp_fn_nm), "("), sp_cast), "); return NULL; }\n"));
                        } else {
                            /* pass */
                            long long sp_na = (pool_args->len - 1LL);
                            /* pass */
                            bool sp_is_void = true;
                            /* pass */
                            if (_tr_dict_contains(self->functions, sp_fn_nm)) {
                                /* pass */
                                HirFunction* sp_hf = ((HirFunction*)(uintptr_t)_tr_dict_get(self->functions, sp_fn_nm));
                                /* pass */
                                sp_is_void = ((strcmp((char*)sp_hf->ret_ty->name, (char*)"void") == 0) || (strcmp((char*)sp_hf->ret_ty->name, (char*)"") == 0));
                            }
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat("static void* _tr_spawn_wrap_", sp_fn_nm), "(void* _vp) {\n"));
                            /* pass */
                            CGenerator_w(self, "    long long* _ab = (long long*)_vp;\n");
                            /* pass */
                            char* sp_cs = "";
                            /* pass */
                            long long sp_ii = 0LL;
                            /* pass */
                            while ((sp_ii < sp_na)) {
                                /* pass */
                                if ((sp_ii > 0LL)) {
                                    /* pass */
                                    sp_cs = _tr_str_concat(sp_cs, ", ");
                                }
                                /* pass */
                                char* sp_atn2 = hir_expr_type(((HirExpr*)List_ptr_get(pool_args, (sp_ii + 1LL))))->name;
                                /* pass */
                                if ((((!_is_int_type(sp_atn2)) && (strcmp((char*)sp_atn2, (char*)"bool") != 0)) && (strcmp((char*)sp_atn2, (char*)"char") != 0))) {
                                    /* pass */
                                    char* sp_ct2 = CGenerator_type_to_c(self, hir_expr_type(((HirExpr*)List_ptr_get(pool_args, (sp_ii + 1LL)))));
                                    /* pass */
                                    sp_cs = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(sp_cs, "("), sp_ct2), ")_ab["), _tr_int_to_str((long long)((sp_ii + 1LL)))), "]");
                                } else {
                                    /* pass */
                                    sp_cs = _tr_str_concat(_tr_str_concat(_tr_str_concat(sp_cs, "_ab["), _tr_int_to_str((long long)((sp_ii + 1LL)))), "]");
                                }
                                /* pass */
                                sp_ii = (sp_ii + 1LL);
                            }
                            /* pass */
                            if (sp_is_void) {
                                /* pass */
                                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", sp_fn_nm), "("), sp_cs), ");\n"));
                            } else {
                                /* pass */
                                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    _ab[0] = (long long)", sp_fn_nm), "("), sp_cs), ");\n"));
                            }
                            /* pass */
                            CGenerator_w(self, "    free(_ab); return NULL;\n");
                            /* pass */
                            CGenerator_w(self, "}\n");
                        }
                    }
                }
            }
        }
    } else if (1) {
        __auto_type _ = _t217;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_prescan_spawns(CGenerator* self, HirProgram* prog) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->functions)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->classes)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
            /* pass */
            if ((!_tr_dict_contains(self->prescanned_fns, f->name))) {
                /* pass */
                _tr_dict_set(self->prescanned_fns, f->name, true);
                /* pass */
                HirBlock* fb = f->body;
                /* pass */
                if ((!_is_invalid_ptr(((unsigned long long)(fb))))) {
                    /* pass */
                    CGenerator_prescan_block_spawns(self, fb);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(c))))) {
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(c->methods))))) {
                /* pass */
                long long mi = 0LL;
                /* pass */
                while ((mi < c->methods->len)) {
                    /* pass */
                    HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                        /* pass */
                        char* mk = _tr_str_concat(_tr_str_concat(c->name, "::"), m->name);
                        /* pass */
                        if ((!_tr_dict_contains(self->prescanned_fns, mk))) {
                            /* pass */
                            _tr_dict_set(self->prescanned_fns, mk, true);
                            /* pass */
                            if ((!_is_invalid_ptr(((unsigned long long)(m->body))))) {
                                /* pass */
                                CGenerator_prescan_block_spawns(self, m->body);
                            }
                        }
                    }
                    /* pass */
                    mi = (mi + 1LL);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_prescan_expr_awaits(CGenerator* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t224 = (*e);
    if (_t224.tag == HirExpr_EAwait) {
        __auto_type inner = _t224.data.EAwait.expr;
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(inner))))) {
            /* pass */
            __auto_type _t225 = (*inner);
            if (_t225.tag == HirExpr_ECall) {
                __auto_type callee = _t225.data.ECall.callee;
__auto_type args = _t225.data.ECall.args;
__auto_type call_ty = _t225.data.ECall.ty;
                /* pass */
                char* fn_name = "";
                /* pass */
                __auto_type _t226 = (*callee);
                if (_t226.tag == HirExpr_EIdent) {
                    __auto_type n = _t226.data.EIdent.name;
                    fn_name = n;
                } else if (1) {
                    __auto_type _ = _t226;
                    /* pass */
                }
                /* pass */
                if ((strcmp((char*)fn_name, (char*)"") != 0)) {
                    /* pass */
                    CGenerator_emit_async_wrapper_for_call(self, fn_name, args, call_ty->name);
                }
            } else if (1) {
                __auto_type _ = _t225;
                /* pass */
            }
        }
        /* pass */
        CGenerator_prescan_expr_awaits(self, inner);
    } else if (_t224.tag == HirExpr_EAwaitTimeout) {
        __auto_type ato_inner = _t224.data.EAwaitTimeout.expr;
__auto_type ato_ms = _t224.data.EAwaitTimeout.timeout_ms;
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(ato_inner))))) {
            /* pass */
            __auto_type _t227 = (*ato_inner);
            if (_t227.tag == HirExpr_ECall) {
                __auto_type ato_callee = _t227.data.ECall.callee;
__auto_type ato_args = _t227.data.ECall.args;
__auto_type ato_call_ty = _t227.data.ECall.ty;
                /* pass */
                char* ato_fn_nm = "";
                /* pass */
                __auto_type _t228 = (*ato_callee);
                if (_t228.tag == HirExpr_EIdent) {
                    __auto_type nn = _t228.data.EIdent.name;
                    ato_fn_nm = nn;
                } else if (1) {
                    __auto_type _ = _t228;
                    /* pass */
                }
                /* pass */
                if ((strcmp((char*)ato_fn_nm, (char*)"") != 0)) {
                    /* pass */
                    CGenerator_emit_async_wrapper_for_call(self, ato_fn_nm, ato_args, ato_call_ty->name);
                }
            } else if (1) {
                __auto_type _ = _t227;
                /* pass */
            }
        }
        /* pass */
        CGenerator_prescan_expr_awaits(self, ato_ms);
    } else if (_t224.tag == HirExpr_ECall) {
        __auto_type callee = _t224.data.ECall.callee;
__auto_type args = _t224.data.ECall.args;
        /* pass */
        CGenerator_prescan_expr_awaits(self, callee);
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            CGenerator_prescan_expr_awaits(self, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t224.tag == HirExpr_EBinOp) {
        __auto_type l = _t224.data.EBinOp.left;
__auto_type r = _t224.data.EBinOp.right;
        /* pass */
        CGenerator_prescan_expr_awaits(self, l);
        /* pass */
        CGenerator_prescan_expr_awaits(self, r);
    } else if (_t224.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t224.data.EMethodCall.obj;
__auto_type margs = _t224.data.EMethodCall.args;
        /* pass */
        CGenerator_prescan_expr_awaits(self, obj);
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < margs->len)) {
            /* pass */
            CGenerator_prescan_expr_awaits(self, ((HirExpr*)List_ptr_get(margs, mi)));
            /* pass */
            mi = (mi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t224;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_prescan_block_awaits(CGenerator* self, HirBlock* block) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < block->stmts->len)) {
        /* pass */
        CGenerator_prescan_stmt_awaits(self, ((HirStmt*)List_ptr_get(block->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_prescan_stmt_awaits(CGenerator* self, HirStmt* s) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(s)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t229 = (*s);
    if (_t229.tag == HirStmt_SExpr) {
        __auto_type e = _t229.data.SExpr.expr;
        CGenerator_prescan_expr_awaits(self, e);
    } else if (_t229.tag == HirStmt_SLet) {
        __auto_type v = _t229.data.SLet.val;
        CGenerator_prescan_expr_awaits(self, v);
    } else if (_t229.tag == HirStmt_SReturn) {
        __auto_type v = _t229.data.SReturn.val;
        CGenerator_prescan_expr_awaits(self, v);
    } else if (_t229.tag == HirStmt_SIf) {
        __auto_type cond = _t229.data.SIf.cond;
__auto_type t = _t229.data.SIf.then_b;
__auto_type e = _t229.data.SIf.else_b;
        /* pass */
        CGenerator_prescan_expr_awaits(self, cond);
        /* pass */
        CGenerator_prescan_block_awaits(self, t);
        /* pass */
        CGenerator_prescan_block_awaits(self, e);
    } else if (_t229.tag == HirStmt_SWhile) {
        __auto_type cond = _t229.data.SWhile.cond;
__auto_type body = _t229.data.SWhile.body;
        /* pass */
        CGenerator_prescan_expr_awaits(self, cond);
        /* pass */
        CGenerator_prescan_block_awaits(self, body);
    } else if (_t229.tag == HirStmt_SFor) {
        __auto_type iter = _t229.data.SFor.iter;
__auto_type body = _t229.data.SFor.body;
        /* pass */
        CGenerator_prescan_expr_awaits(self, iter);
        /* pass */
        CGenerator_prescan_block_awaits(self, body);
    } else if (_t229.tag == HirStmt_SForUnpack) {
        __auto_type iter = _t229.data.SForUnpack.iter;
__auto_type body = _t229.data.SForUnpack.body;
        /* pass */
        CGenerator_prescan_expr_awaits(self, iter);
        /* pass */
        CGenerator_prescan_block_awaits(self, body);
    } else if (_t229.tag == HirStmt_STry) {
        __auto_type tb = _t229.data.STry.try_body;
__auto_type catches = _t229.data.STry.catches;
__auto_type fin = _t229.data.STry.finally_b;
        /* pass */
        CGenerator_prescan_block_awaits(self, tb);
        /* pass */
        CGenerator_prescan_block_awaits(self, fin);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            HirCatchClause* cc = (*((HirCatchClause**)List_ptr_get(catches, ci)));
            /* pass */
            CGenerator_prescan_block_awaits(self, cc->body);
            /* pass */
            ci = (ci + 1LL);
        }
    } else if (_t229.tag == HirStmt_STaskGroup) {
        __auto_type body = _t229.data.STaskGroup.body;
        CGenerator_prescan_block_awaits(self, body);
    } else if (_t229.tag == HirStmt_SMultiLet) {
        __auto_type v = _t229.data.SMultiLet.val;
        CGenerator_prescan_expr_awaits(self, v);
    } else if (_t229.tag == HirStmt_SAssign) {
        __auto_type tgt = _t229.data.SAssign.target;
__auto_type v = _t229.data.SAssign.val;
        /* pass */
        CGenerator_prescan_expr_awaits(self, tgt);
        /* pass */
        CGenerator_prescan_expr_awaits(self, v);
    } else if (1) {
        __auto_type _ = _t229;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_prescan_awaits(CGenerator* self, HirProgram* prog) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(f->body))))) {
                /* pass */
                CGenerator_prescan_block_awaits(self, f->body);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(c))))) {
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(c->methods))))) {
                /* pass */
                long long mi = 0LL;
                /* pass */
                while ((mi < c->methods->len)) {
                    /* pass */
                    HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                        /* pass */
                        if ((!_is_invalid_ptr(((unsigned long long)(m->body))))) {
                            /* pass */
                            CGenerator_prescan_block_awaits(self, m->body);
                        }
                    }
                    /* pass */
                    mi = (mi + 1LL);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_gen_stmt(CGenerator* self, HirStmt* s_ptr, long long indent) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(s_ptr)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    __auto_type _t230 = (*s_ptr);
    if (_t230.tag == HirStmt_SExpr) {
        __auto_type e = _t230.data.SExpr.expr;
        /* pass */
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t231 = (*e);
            if (_t231.tag == HirExpr_ETryExpr) {
                __auto_type inner_try = _t231.data.ETryExpr.expr;
                /* pass */
                char* tmp_qr = _tr_str_concat("_qr", CGenerator_next_temp(self));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "Result "), tmp_qr), " = ("), CGenerator_gen_expr(self, inner_try)), ");\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), tmp_qr), ".tag == Result_Err) return "), tmp_qr), ";\n"));
                /* pass */
                return;
            } else if (1) {
                __auto_type _ = _t231;
                /* pass */
            }
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_gen_expr(self, e)), ";\n"));
    } else if (_t230.tag == HirStmt_SDefer) {
        __auto_type inner_defer = _t230.data.SDefer.stmt;
        /* pass */
        StringBuilder* saved_buf = self->buf;
        /* pass */
        self->buf = StringBuilder_init(256LL);
        /* pass */
        CGenerator_gen_stmt(self, inner_defer, indent);
        /* pass */
        char* deferred_c = StringObj_as_str(StringBuilder_to_string(self->buf));
        /* pass */
        self->buf = saved_buf;
        /* pass */
        List_str_append(self->defer_stack, deferred_c);
    } else if (_t230.tag == HirStmt_SReturn) {
        __auto_type e = _t230.data.SReturn.val;
        /* pass */
        if ((self->defer_stack->len > 0LL)) {
            /* pass */
            long long di2 = (self->defer_stack->len - 1LL);
            /* pass */
            while ((di2 >= 0LL)) {
                /* pass */
                CGenerator_w(self, List_str_get(self->defer_stack, di2));
                /* pass */
                if ((di2 == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                di2 = (di2 - 1LL);
            }
        }
        /* pass */
        if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
            /* pass */
            if ((strcmp((char*)self->cur_throws_ty, (char*)"") != 0)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "return ((Result){.tag=Result_Ok});\n"));
            } else if ((strcmp((char*)self->cur_func, (char*)"main") == 0)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "return 0;\n"));
            } else {
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "return;\n"));
            }
        } else {
            /* pass */
            __auto_type _t232 = (*e);
            if (_t232.tag == HirExpr_ETryExpr) {
                __auto_type inner_try2 = _t232.data.ETryExpr.expr;
__auto_type ok_ty2 = _t232.data.ETryExpr.ty;
                /* pass */
                char* tmp_qr2 = _tr_str_concat("_qr", CGenerator_next_temp(self));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "Result "), tmp_qr2), " = ("), CGenerator_gen_expr(self, inner_try2)), ");\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), tmp_qr2), ".tag == Result_Err) return "), tmp_qr2), ";\n"));
                /* pass */
                if ((strcmp((char*)self->cur_throws_ty, (char*)"") != 0)) {
                    /* pass */
                    char* ok_c2 = CGenerator_type_to_c(self, ok_ty2);
                    /* pass */
                    if ((_is_int_type(ok_ty2->name) || (strcmp((char*)ok_ty2->name, (char*)"bool") == 0))) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ((Result){.tag=Result_Ok, .data.Ok.val=(void*)(uintptr_t)(("), ok_c2), ")(uintptr_t)"), tmp_qr2), ".data.Ok.val)});\n"));
                    } else {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ((Result){.tag=Result_Ok, .data.Ok.val="), tmp_qr2), ".data.Ok.val});\n"));
                    }
                } else {
                    /* pass */
                    char* ok_c2b = CGenerator_type_to_c(self, ok_ty2);
                    /* pass */
                    if ((_is_int_type(ok_ty2->name) || (strcmp((char*)ok_ty2->name, (char*)"bool") == 0))) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ("), ok_c2b), ")(uintptr_t)"), tmp_qr2), ".data.Ok.val;\n"));
                    } else {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ("), ok_c2b), "*)"), tmp_qr2), ".data.Ok.val;\n"));
                    }
                }
                /* pass */
                return;
            } else if (1) {
                __auto_type _ = _t232;
                /* pass */
            }
            /* pass */
            if ((strcmp((char*)self->cur_throws_ty, (char*)"") != 0)) {
                /* pass */
                char* ret_s = CGenerator_gen_expr(self, e);
                /* pass */
                char* ret_ty_n = hir_expr_type(e)->name;
                /* pass */
                if (((_is_int_type(ret_ty_n) || (strcmp((char*)ret_ty_n, (char*)"bool") == 0)) || (strcmp((char*)ret_ty_n, (char*)"char") == 0))) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ((Result){.tag=Result_Ok, .data.Ok.val=(void*)(uintptr_t)("), ret_s), ")});\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ((Result){.tag=Result_Ok, .data.Ok.val=(void*)("), ret_s), ")});\n"));
                }
            } else {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return "), CGenerator_gen_expr(self, e)), ";\n"));
            }
        }
    } else if (_t230.tag == HirStmt_SLet) {
        __auto_type n = _t230.data.SLet.name;
__auto_type is_const = _t230.data.SLet.is_const;
__auto_type is_shared = _t230.data.SLet.is_shared;
__auto_type ty = _t230.data.SLet.ty;
__auto_type v = _t230.data.SLet.val;
        /* pass */
        char* c_ty = CGenerator_type_to_c(self, ty);
        /* pass */
        if (((strcmp((char*)c_ty, (char*)"List_ptr*") == 0) && (((unsigned long long)(v)) != ((unsigned long long)(0LL))))) {
            /* pass */
            char* _init_sfx = "ptr";
            /* pass */
            __auto_type _t233 = (*v);
            if (_t233.tag == HirExpr_ECall) {
                __auto_type call_ty = _t233.data.ECall.ty;
                /* pass */
                if ((((strcmp((char*)call_ty->name, (char*)"Vec") == 0) || (strcmp((char*)call_ty->name, (char*)"List") == 0)) && (call_ty->args->len > 0LL))) {
                    /* pass */
                    _init_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(call_ty->args, 0LL)))->name);
                }
            } else if (_t233.tag == HirExpr_EMethodCall) {
                __auto_type hobj_mc = _t233.data.EMethodCall.obj;
__auto_type _m = _t233.data.EMethodCall.method;
__auto_type iargs = _t233.data.EMethodCall.args;
__auto_type call_ty = _t233.data.EMethodCall.ty;
                /* pass */
                if ((((strcmp((char*)call_ty->name, (char*)"Vec") == 0) || (strcmp((char*)call_ty->name, (char*)"List") == 0)) && (call_ty->args->len > 0LL))) {
                    /* pass */
                    _init_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(call_ty->args, 0LL)))->name);
                } else if (((((unsigned long long)(hobj_mc)) != ((unsigned long long)(0LL))) && ((strcmp((char*)_m, (char*)"init") == 0) || (strcmp((char*)_m, (char*)"new") == 0)))) {
                    /* pass */
                    AstType* hobj_ty_mc = hir_expr_type(hobj_mc);
                    /* pass */
                    if ((((strcmp((char*)hobj_ty_mc->name, (char*)"Vec") == 0) || (strcmp((char*)hobj_ty_mc->name, (char*)"List") == 0)) && (hobj_ty_mc->args->len > 0LL))) {
                        /* pass */
                        _init_sfx = CGenerator_list_elem_suffix(self, (*((AstType**)List_ptr_get(hobj_ty_mc->args, 0LL)))->name);
                    } else if ((iargs->len > 0LL)) {
                        /* pass */
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t233;
                /* pass */
            }
            /* pass */
            if ((strcmp((char*)_init_sfx, (char*)"ptr") != 0)) {
                /* pass */
                c_ty = _tr_str_concat(_tr_str_concat("List_", _init_sfx), "*");
            }
        }
        /* pass */
        if (((strcmp((char*)c_ty, (char*)"void") == 0) && (((unsigned long long)(v)) != ((unsigned long long)(0LL))))) {
            /* pass */
            c_ty = "__auto_type";
        }
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t234 = (*v);
            if (_t234.tag == HirExpr_ETryExpr) {
                __auto_type inner_try3 = _t234.data.ETryExpr.expr;
__auto_type ok_ty3 = _t234.data.ETryExpr.ty;
                /* pass */
                char* tmp_qr3 = _tr_str_concat("_qr", CGenerator_next_temp(self));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "Result "), tmp_qr3), " = ("), CGenerator_gen_expr(self, inner_try3)), ");\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), tmp_qr3), ".tag == Result_Err) return "), tmp_qr3), ";\n"));
                /* pass */
                char* ok_c3 = CGenerator_type_to_c(self, ok_ty3);
                /* pass */
                if (((strcmp((char*)ok_c3, (char*)"void") == 0) || (strcmp((char*)ok_c3, (char*)"__auto_type") == 0))) {
                    /* pass */
                    ok_c3 = "long long";
                }
                /* pass */
                char* ok_val3 = "";
                /* pass */
                if (((_is_int_type(ok_ty3->name) || (strcmp((char*)ok_ty3->name, (char*)"bool") == 0)) || (strcmp((char*)ok_ty3->name, (char*)"char") == 0))) {
                    /* pass */
                    ok_val3 = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ok_c3), ")(uintptr_t)"), tmp_qr3), ".data.Ok.val");
                } else {
                    /* pass */
                    ok_val3 = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("(", ok_c3), "*)"), tmp_qr3), ".data.Ok.val");
                }
                /* pass */
                if (is_const) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "const "), ok_c3), " "), _safe_c_varname(n)), " = "), ok_val3), ";\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, ok_c3), " "), _safe_c_varname(n)), " = "), ok_val3), ";\n"));
                }
                /* pass */
                _tr_dict_set(self->decl_vars, n, true);
                /* pass */
                return;
            } else if (1) {
                __auto_type _ = _t234;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t235 = (*v);
            if (_t235.tag == HirExpr_EClosure) {
                __auto_type cl_params = _t235.data.EClosure.params;
__auto_type cl_ret_ty = _t235.data.EClosure.ret_ty;
__auto_type cl_body = _t235.data.EClosure.body;
__auto_type cl_caps = _t235.data.EClosure.captures;
                /* pass */
                self->closure_count = (self->closure_count + 1LL);
                /* pass */
                char* cl_cname = _tr_str_concat("_closure_", _tr_int_to_str((long long)(self->closure_count)));
                /* pass */
                char* cl_ret_c = CGenerator_type_to_c(self, cl_ret_ty);
                /* pass */
                if ((strcmp((char*)cl_ret_c, (char*)"void") == 0)) {
                    /* pass */
                    cl_ret_c = "long long";
                }
                /* pass */
                long long cl_pi = 0LL;
                /* pass */
                while ((cl_pi < cl_params->len)) {
                    /* pass */
                    _tr_dict_set(self->decl_vars, ((HirParam*)List_ptr_get(cl_params, cl_pi))->name, true);
                    /* pass */
                    cl_pi = (cl_pi + 1LL);
                }
                /* pass */
                long long cl_ci = 0LL;
                /* pass */
                while ((cl_ci < cl_caps->len)) {
                    /* pass */
                    _tr_dict_set(self->decl_vars, ((HirParam*)List_ptr_get(cl_caps, cl_ci))->name, true);
                    /* pass */
                    cl_ci = (cl_ci + 1LL);
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, cl_ret_c), " "), cl_cname), "("));
                /* pass */
                bool cl_first = true;
                /* pass */
                long long cl_ii = 0LL;
                /* pass */
                while ((cl_ii < cl_params->len)) {
                    /* pass */
                    HirParam* cl_p = ((HirParam*)List_ptr_get(cl_params, cl_ii));
                    /* pass */
                    if ((!cl_first)) {
                        /* pass */
                        CGenerator_w(self, ", ");
                    }
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_type_to_c(self, cl_p->ty), " "), _safe_c_varname(cl_p->name)));
                    /* pass */
                    cl_first = false;
                    /* pass */
                    cl_ii = (cl_ii + 1LL);
                }
                /* pass */
                CGenerator_w(self, ") {\n");
                /* pass */
                CGenerator_gen_block(self, cl_body, (indent + 1LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "void* "), _safe_c_varname(n)), " = (void*)(&"), cl_cname), ");\n"));
                /* pass */
                _tr_dict_set(self->decl_vars, n, true);
                /* pass */
                return;
            } else if (1) {
                __auto_type _ = _t235;
                /* pass */
            }
        }
        /* pass */
        char* _sn = _safe_c_varname(n);
        /* pass */
        if (is_const) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "const "), c_ty), " "), _sn));
        } else if (is_shared) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, "_TrSharedBox* "), _sn));
        } else {
            /* pass */
            if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t236 = (*v);
                if (_t236.tag == HirExpr_EMethodCall) {
                    __auto_type au_obj = _t236.data.EMethodCall.obj;
__auto_type au_method = _t236.data.EMethodCall.method;
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(au_obj))))) {
                        /* pass */
                        AstType* au_oty = hir_expr_type(au_obj);
                        /* pass */
                        if (((strcmp((char*)au_oty->name, (char*)"Mutex") == 0) && ((strcmp((char*)au_method, (char*)"get") == 0) || (strcmp((char*)au_method, (char*)"lock") == 0)))) {
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__attribute__((cleanup(_tr_mutexbox_cleanup))) _TrMutexBox* __g_"), _sn), " = "), CGenerator_gen_expr(self, au_obj)), ";\n"));
                        } else if (((strcmp((char*)au_oty->name, (char*)"RwLock") == 0) && ((strcmp((char*)au_method, (char*)"read") == 0) || (strcmp((char*)au_method, (char*)"read_lock") == 0)))) {
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__attribute__((cleanup(_tr_rwlbox_cleanup_r))) _TrRWLBox* __g_"), _sn), " = "), CGenerator_gen_expr(self, au_obj)), ";\n"));
                        } else if (((strcmp((char*)au_oty->name, (char*)"RwLock") == 0) && ((strcmp((char*)au_method, (char*)"write") == 0) || (strcmp((char*)au_method, (char*)"write_lock") == 0)))) {
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__attribute__((cleanup(_tr_rwlbox_cleanup_w))) _TrRWLBox* __g_"), _sn), " = "), CGenerator_gen_expr(self, au_obj)), ";\n"));
                        }
                    }
                } else if (1) {
                    __auto_type _ = _t236;
                    /* pass */
                }
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, c_ty), " "), _sn));
        }
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (is_shared) {
                /* pass */
                bool is_clone = false;
                /* pass */
                __auto_type _t237 = (*v);
                if (_t237.tag == HirExpr_EIdent) {
                    __auto_type vn = _t237.data.EIdent.name;
                    /* pass */
                    if (_tr_dict_contains(self->shared_vars, vn)) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(" = _tr_shared_clone(", vn), ");\n"));
                        /* pass */
                        is_clone = true;
                    }
                } else if (1) {
                    __auto_type _ = _t237;
                    /* pass */
                }
                /* pass */
                if ((!is_clone)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(" = _tr_shared_new(", CGenerator_gen_expr(self, v)), ");\n"));
                }
                /* pass */
                char* sh_type_name = ty->name;
                /* pass */
                if (((strcmp((char*)sh_type_name, (char*)"void") == 0) || (strcmp((char*)sh_type_name, (char*)"None") == 0))) {
                    /* pass */
                    sh_type_name = hir_expr_type(v)->name;
                }
                /* pass */
                _tr_dict_set(self->shared_vars, n, sh_type_name);
            } else {
                /* pass */
                char* init_s = "";
                /* pass */
                if (_tr_dict_contains(self->interfaces, ty->name)) {
                    /* pass */
                    HirInterface* _slet_iface = ((HirInterface*)(uintptr_t)_tr_dict_get(self->interfaces, ty->name));
                    /* pass */
                    char* _slet_iface_name = ty->name;
                    /* pass */
                    if (((_slet_iface->generics->len > 0LL) && (ty->args->len > 0LL))) {
                        /* pass */
                        _slet_iface_name = _tr_str_concat(_tr_str_concat(ty->name, "_"), CGenerator_type_args_suffix(self, ty->args));
                    }
                    /* pass */
                    AstType* _slet_val_ty = hir_expr_type(v);
                    /* pass */
                    char* val_ty_n = _slet_val_ty->name;
                    /* pass */
                    if (_tr_dict_contains(self->classes, val_ty_n)) {
                        /* pass */
                        HirClass* _slet_cls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, val_ty_n));
                        /* pass */
                        if (((_slet_cls->generics->len > 0LL) && (_slet_val_ty->args->len > 0LL))) {
                            /* pass */
                            val_ty_n = _tr_str_concat(_tr_str_concat(val_ty_n, "_"), CGenerator_type_args_suffix(self, _slet_val_ty->args));
                        }
                        /* pass */
                        init_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(val_ty_n, "_as_"), _slet_iface_name), "("), CGenerator_gen_expr(self, v)), ")");
                    }
                }
                /* pass */
                if ((strcmp((char*)init_s, (char*)"") == 0)) {
                    /* pass */
                    __auto_type _t238 = (*v);
                    if (_t238.tag == HirExpr_EList) {
                        __auto_type items = _t238.data.EList.items;
__auto_type list_ty = _t238.data.EList.ty;
                        /* pass */
                        if ((((list_ty->args->len == 0LL) && (strcmp((char*)ty->name, (char*)"List") == 0)) && (ty->args->len > 0LL))) {
                            /* pass */
                            init_s = CGenerator_gen_list_literal(self, items, ty);
                        } else {
                            /* pass */
                            init_s = CGenerator_gen_list_literal(self, items, list_ty);
                        }
                    } else if (_t238.tag == HirExpr_EDict) {
                        __auto_type d_keys = _t238.data.EDict.keys;
__auto_type d_vals = _t238.data.EDict.vals;
__auto_type d_ty = _t238.data.EDict.ty;
                        /* pass */
                        if (((((d_ty->args->len == 0LL) || (strcmp((char*)d_ty->name, (char*)"Dict") == 0)) && ((strcmp((char*)ty->name, (char*)"Dict") == 0) || (strcmp((char*)ty->name, (char*)"Map") == 0))) && (ty->args->len > 0LL))) {
                            /* pass */
                            init_s = CGenerator_gen_dict_literal(self, d_keys, d_vals, ty);
                        } else {
                            /* pass */
                            init_s = CGenerator_gen_dict_literal(self, d_keys, d_vals, d_ty);
                        }
                    } else if (1) {
                        __auto_type _ = _t238;
                        /* pass */
                        init_s = CGenerator_gen_expr(self, v);
                    }
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(" = ", init_s), ";\n"));
            }
        } else {
            /* pass */
            if (is_shared) {
                /* pass */
                CGenerator_w(self, " = NULL;\n");
                /* pass */
                _tr_dict_set(self->shared_vars, n, ty->name);
            } else if (((strcmp((char*)c_ty, (char*)"Result") == 0) || (strcmp((char*)c_ty, (char*)"Option") == 0))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(" = (", c_ty), "){0};\n"));
            } else {
                /* pass */
                CGenerator_w(self, " = 0;\n");
            }
        }
        /* pass */
        _tr_dict_set(self->decl_vars, n, true);
    } else if (_t230.tag == HirStmt_SAssign) {
        __auto_type t = _t230.data.SAssign.target;
__auto_type v = _t230.data.SAssign.val;
        /* pass */
        __auto_type _t239 = (*t);
        if (_t239.tag == HirExpr_EIdent) {
            __auto_type n = _t239.data.EIdent.name;
            /* pass */
            if (((!_tr_dict_contains(self->decl_vars, n)) && (!_tr_dict_contains(self->global_vars, n)))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__auto_type "), _safe_c_varname(n)), " = "), CGenerator_gen_expr(self, v)), ";\n"));
                /* pass */
                _tr_dict_set(self->decl_vars, n, true);
                /* pass */
                return;
            }
        } else if (_t239.tag == HirExpr_EIndex) {
            __auto_type idx_obj = _t239.data.EIndex.obj;
__auto_type idx_key = _t239.data.EIndex.index;
            /* pass */
            char* idx_ty_n = hir_expr_type(idx_obj)->name;
            /* pass */
            if (CGenerator_has_method(self, idx_ty_n, "__setitem__")) {
                /* pass */
                char* _si_obj = CGenerator_gen_expr(self, idx_obj);
                /* pass */
                char* _si_key = CGenerator_gen_expr(self, idx_key);
                /* pass */
                char* _si_val = CGenerator_gen_expr(self, v);
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_cls_method_c_call(self, idx_ty_n, "__setitem__", _si_obj, _tr_str_concat(_tr_str_concat(_si_key, ", "), _si_val))), ";\n"));
                /* pass */
                return;
            }
        } else if (_t239.tag == HirExpr_EMethodCall) {
            __auto_type mc_obj = _t239.data.EMethodCall.obj;
__auto_type mc_meth = _t239.data.EMethodCall.method;
__auto_type mc_args = _t239.data.EMethodCall.args;
            /* pass */
            if (((strcmp((char*)mc_meth, (char*)"get_index") == 0) && (mc_args->len > 0LL))) {
                /* pass */
                char* mc_ty_n = hir_expr_type(mc_obj)->name;
                /* pass */
                if (CGenerator_has_method(self, mc_ty_n, "__setitem__")) {
                    /* pass */
                    char* _si2_obj = CGenerator_gen_expr(self, mc_obj);
                    /* pass */
                    char* _si2_key = CGenerator_gen_args(self, mc_args);
                    /* pass */
                    char* _si2_val = CGenerator_gen_expr(self, v);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_cls_method_c_call(self, mc_ty_n, "__setitem__", _si2_obj, _tr_str_concat(_tr_str_concat(_si2_key, ", "), _si2_val))), ";\n"));
                    /* pass */
                    return;
                }
            }
        } else if (1) {
            __auto_type _ = _t239;
            /* pass */
        }
        /* pass */
        char* _assign_rhs = "";
        /* pass */
        __auto_type _t240 = (*v);
        if (_t240.tag == HirExpr_EList) {
            __auto_type litems = _t240.data.EList.items;
__auto_type list_ty = _t240.data.EList.ty;
            /* pass */
            if ((list_ty->args->len == 0LL)) {
                /* pass */
                AstType* tgt_ty = hir_expr_type(t);
                /* pass */
                if ((((strcmp((char*)tgt_ty->name, (char*)"List") == 0) || (strcmp((char*)tgt_ty->name, (char*)"Vec") == 0)) && (tgt_ty->args->len > 0LL))) {
                    /* pass */
                    _assign_rhs = CGenerator_gen_list_literal(self, litems, tgt_ty);
                }
            }
        } else if (1) {
            __auto_type _ = _t240;
            /* pass */
        }
        /* pass */
        if ((strcmp((char*)_assign_rhs, (char*)"") == 0)) {
            /* pass */
            _assign_rhs = CGenerator_gen_expr(self, v);
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, CGenerator_gen_expr(self, t)), " = "), _assign_rhs), ";\n"));
    } else if (_t230.tag == HirStmt_SIf) {
        __auto_type c = _t230.data.SIf.cond;
__auto_type t = _t230.data.SIf.then_b;
__auto_type e = _t230.data.SIf.else_b;
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), CGenerator_gen_cond_expr(self, c)), ") {\n"));
        /* pass */
        CGenerator_gen_block(self, t, (indent + 1LL));
        /* pass */
        HirBlock* cur = e;
        /* pass */
        bool going = (!_is_invalid_ptr(((unsigned long long)(cur))));
        /* pass */
        while (going) {
            /* pass */
            if (_is_invalid_ptr(((unsigned long long)(cur->stmts)))) {
                /* pass */
                going = false;
            } else if ((cur->stmts->len == 1LL)) {
                /* pass */
                __auto_type _t241 = (*((HirStmt*)List_ptr_get(cur->stmts, 0LL)));
                if (_t241.tag == HirStmt_SIf) {
                    __auto_type ic = _t241.data.SIf.cond;
__auto_type it = _t241.data.SIf.then_b;
__auto_type ie = _t241.data.SIf.else_b;
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "} else if ("), CGenerator_gen_cond_expr(self, ic)), ") {\n"));
                    /* pass */
                    CGenerator_gen_block(self, it, (indent + 1LL));
                    /* pass */
                    cur = ie;
                    /* pass */
                    going = (!_is_invalid_ptr(((unsigned long long)(cur))));
                } else if (1) {
                    __auto_type _ = _t241;
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(pad, "} else {\n"));
                    /* pass */
                    CGenerator_gen_block(self, cur, (indent + 1LL));
                    /* pass */
                    going = false;
                }
            } else if ((cur->stmts->len > 0LL)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "} else {\n"));
                /* pass */
                CGenerator_gen_block(self, cur, (indent + 1LL));
                /* pass */
                going = false;
            } else {
                /* pass */
                going = false;
            }
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
    } else if (_t230.tag == HirStmt_SWhile) {
        __auto_type c = _t230.data.SWhile.cond;
__auto_type b = _t230.data.SWhile.body;
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "while ("), CGenerator_gen_cond_expr(self, c)), ") {\n"));
        /* pass */
        CGenerator_gen_block(self, b, (indent + 1LL));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
    } else if (_t230.tag == HirStmt_SFor) {
        __auto_type var = _t230.data.SFor.var;
__auto_type iter = _t230.data.SFor.iter;
__auto_type body = _t230.data.SFor.body;
        /* pass */
        if ((self->in_gpu_block > 0LL)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(pad, "#pragma omp for\n"));
        }
        /* pass */
        CGenerator_gen_for_loop(self, var, iter, body, indent);
    } else if (_t230.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t230.data.SForUnpack.vars;
__auto_type iter = _t230.data.SForUnpack.iter;
__auto_type body = _t230.data.SForUnpack.body;
        /* pass */
        CGenerator_gen_for_unpack(self, vars, iter, body, indent);
    } else if (_t230.tag == HirStmt_SMatch) {
        __auto_type e = _t230.data.SMatch.expr;
__auto_type arms = _t230.data.SMatch.arms;
        /* pass */
        CGenerator_gen_match(self, e, arms, indent);
    } else if (_t230.tag == HirStmt_STry) {
        __auto_type try_body = _t230.data.STry.try_body;
__auto_type catches = _t230.data.STry.catches;
__auto_type finally_b = _t230.data.STry.finally_b;
        /* pass */
        CGenerator_gen_try(self, try_body, catches, finally_b, indent);
    } else if (_t230.tag == HirStmt_SRaise) {
        __auto_type e = _t230.data.SRaise.val;
        /* pass */
        if ((strcmp((char*)self->cur_throws_ty, (char*)"") != 0)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "return ((Result){.tag=Result_Err, .data.Err.err=(void*)("), CGenerator_gen_expr(self, e)), ")});\n"));
        } else {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_tr_exc_raise((char*)("), CGenerator_gen_expr(self, e)), "));\n"));
        }
    } else if (_t230.tag == HirStmt_SAssert) {
        __auto_type cond = _t230.data.SAssert.cond;
__auto_type msg = _t230.data.SAssert.msg;
        /* pass */
        if ((((unsigned long long)(msg)) == ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_TR_ASSERT("), CGenerator_gen_expr(self, cond)), ");\n"));
        } else {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_TR_ASSERT_MSG("), CGenerator_gen_expr(self, cond)), ", "), CGenerator_gen_expr(self, msg)), ");\n"));
        }
    } else if (_t230.tag == HirStmt_SUnsafe) {
        __auto_type b = _t230.data.SUnsafe.body;
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "/* unsafe block */\n"));
        /* pass */
        CGenerator_gen_block(self, b, indent);
    } else if (_t230.tag == HirStmt_SWith) {
        __auto_type items = _t230.data.SWith.items;
__auto_type aliases = _t230.data.SWith.aliases;
__auto_type body = _t230.data.SWith.body;
        /* pass */
        char* with_base = CGenerator_next_temp(self);
        /* pass */
        long long wi = 0LL;
        /* pass */
        while ((wi < items->len)) {
            /* pass */
            HirExpr* with_item = ((HirExpr*)List_ptr_get(items, wi));
            /* pass */
            HirExpr* actual_wi = with_item;
            /* pass */
            char* cast_alias = "";
            /* pass */
            __auto_type _t242 = (*with_item);
            if (_t242.tag == HirExpr_ECast) {
                __auto_type wi_inner = _t242.data.ECast.expr;
__auto_type wi_cast_ty = _t242.data.ECast.target_ty;
                /* pass */
                actual_wi = wi_inner;
                /* pass */
                cast_alias = wi_cast_ty->name;
            } else if (1) {
                __auto_type _ = _t242;
                /* pass */
            }
            /* pass */
            char* item_s = CGenerator_gen_expr(self, actual_wi);
            /* pass */
            char* item_ty_n = hir_expr_type(actual_wi)->name;
            /* pass */
            char* alias_n = "";
            /* pass */
            if (((wi < aliases->len) && (strcmp((char*)List_str_get(aliases, wi), (char*)"") != 0))) {
                /* pass */
                alias_n = List_str_get(aliases, wi);
            } else if ((strcmp((char*)cast_alias, (char*)"") != 0)) {
                /* pass */
                alias_n = cast_alias;
            }
            /* pass */
            char* ctx_name = _tr_str_concat(_tr_str_concat(with_base, "_ctx"), _tr_int_to_str((long long)(wi)));
            /* pass */
            if (CGenerator_has_method(self, item_ty_n, "__enter__")) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__auto_type "), ctx_name), " = "), item_s), ";\n"));
                /* pass */
                char* enter_val = CGenerator_cls_method_c_call(self, item_ty_n, "__enter__", ctx_name, "");
                /* pass */
                if ((strcmp((char*)alias_n, (char*)"") != 0)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__auto_type "), alias_n), " = "), enter_val), ";\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, enter_val), ";\n"));
                }
            } else {
                /* pass */
                if ((strcmp((char*)alias_n, (char*)"") != 0)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__auto_type "), alias_n), " = "), item_s), ";\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, item_s), ";\n"));
                }
            }
            /* pass */
            wi = (wi + 1LL);
        }
        /* pass */
        CGenerator_gen_block(self, body, indent);
        /* pass */
        long long rei = (items->len - 1LL);
        /* pass */
        while ((rei >= 0LL)) {
            /* pass */
            HirExpr* with_item_exit = ((HirExpr*)List_ptr_get(items, rei));
            /* pass */
            HirExpr* exit_actual = with_item_exit;
            /* pass */
            __auto_type _t243 = (*with_item_exit);
            if (_t243.tag == HirExpr_ECast) {
                __auto_type we_inner = _t243.data.ECast.expr;
                /* pass */
                exit_actual = we_inner;
            } else if (1) {
                __auto_type _ = _t243;
                /* pass */
            }
            /* pass */
            char* exit_ty_n = hir_expr_type(exit_actual)->name;
            /* pass */
            if (CGenerator_has_method(self, exit_ty_n, "__exit__")) {
                /* pass */
                char* ctx_exit = _tr_str_concat(_tr_str_concat(with_base, "_ctx"), _tr_int_to_str((long long)(rei)));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_cls_method_c_call(self, exit_ty_n, "__exit__", ctx_exit, "NULL, NULL, NULL")), ";\n"));
            }
            /* pass */
            rei = (rei - 1LL);
        }
    } else if (_t230.tag == HirStmt_SAsm) {
        __auto_type code = _t230.data.SAsm.code;
__auto_type outputs = _t230.data.SAsm.outputs;
__auto_type inputs = _t230.data.SAsm.inputs;
__auto_type clobbers = _t230.data.SAsm.clobbers;
        /* pass */
        char* asm_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__asm__ volatile(\""), _escape_str_for_c(code)), "\"");
        /* pass */
        if ((((strcmp((char*)outputs, (char*)"") != 0) || (strcmp((char*)inputs, (char*)"") != 0)) || (strcmp((char*)clobbers, (char*)"") != 0))) {
            /* pass */
            asm_s = _tr_str_concat(_tr_str_concat(asm_s, " : "), outputs);
            /* pass */
            if (((strcmp((char*)inputs, (char*)"") != 0) || (strcmp((char*)clobbers, (char*)"") != 0))) {
                /* pass */
                asm_s = _tr_str_concat(_tr_str_concat(asm_s, " : "), inputs);
                /* pass */
                if ((strcmp((char*)clobbers, (char*)"") != 0)) {
                    /* pass */
                    asm_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(asm_s, " : \""), clobbers), "\"");
                }
            }
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(asm_s, ");\n"));
    } else if (_t230.tag == HirStmt_SSpawn) {
        __auto_type e = _t230.data.SSpawn.expr;
        /* pass */
        __auto_type _t244 = (*e);
        if (_t244.tag == HirExpr_ECall) {
            __auto_type callee = _t244.data.ECall.callee;
__auto_type args = _t244.data.ECall.args;
            /* pass */
            char* sp_fn = "";
            /* pass */
            __auto_type _t245 = (*callee);
            if (_t245.tag == HirExpr_EIdent) {
                __auto_type n = _t245.data.EIdent.name;
                sp_fn = n;
            } else if (1) {
                __auto_type _ = _t245;
                /* pass */
            }
            /* pass */
            if ((strcmp((char*)sp_fn, (char*)"") == 0)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_gen_expr(self, e)), "; /* spawn: inline */\n"));
            } else if ((args->len == 0LL)) {
                /* pass */
                if ((self->in_task_group > 0LL)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", NULL));\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_tr_thread_detach(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", NULL));\n"));
                }
            } else if ((args->len == 1LL)) {
                /* pass */
                char* arg_ty_n = hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name;
                /* pass */
                char* void_cast = _tr_str_concat(_tr_str_concat("(void*)(uintptr_t)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                /* pass */
                if ((((!_is_int_type(arg_ty_n)) && (strcmp((char*)arg_ty_n, (char*)"bool") != 0)) && (strcmp((char*)arg_ty_n, (char*)"char") != 0))) {
                    /* pass */
                    void_cast = _tr_str_concat(_tr_str_concat("(void*)(", CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)))), ")");
                }
                /* pass */
                if ((self->in_task_group > 0LL)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", "), void_cast), "));\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "_tr_thread_detach(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", "), void_cast), "));\n"));
                }
            } else {
                /* pass */
                long long sp_nargs = args->len;
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ long long* _sa = (long long*)malloc("), _tr_int_to_str((long long)((sp_nargs + 1LL)))), " * sizeof(long long));\n"));
                /* pass */
                long long sp_ai = 0LL;
                /* pass */
                while ((sp_ai < sp_nargs)) {
                    /* pass */
                    char* sp_atn = hir_expr_type(((HirExpr*)List_ptr_get(args, sp_ai)))->name;
                    /* pass */
                    if ((((!_is_int_type(sp_atn)) && (strcmp((char*)sp_atn, (char*)"bool") != 0)) && (strcmp((char*)sp_atn, (char*)"char") != 0))) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  _sa["), _tr_int_to_str((long long)((sp_ai + 1LL)))), "] = (long long)"), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, sp_ai)))), ";\n"));
                    } else {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  _sa["), _tr_int_to_str((long long)((sp_ai + 1LL)))), "] = (long long)("), CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, sp_ai)))), ");\n"));
                    }
                    /* pass */
                    sp_ai = (sp_ai + 1LL);
                }
                /* pass */
                if ((self->in_task_group > 0LL)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  _tr_tg_push(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", _sa)); }\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  _tr_thread_detach(_tr_thread_start(_tr_spawn_wrap_"), sp_fn), ", _sa)); }\n"));
                }
            }
        } else if (1) {
            __auto_type _ = _t244;
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, CGenerator_gen_expr(self, e)), "; /* spawn: non-call sync */\n"));
        }
    } else if (_t230.tag == HirStmt_STaskGroup) {
        __auto_type body = _t230.data.STaskGroup.body;
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "_tr_tg_begin();\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "{\n"));
        /* pass */
        self->in_task_group = (self->in_task_group + 1LL);
        /* pass */
        CGenerator_gen_block(self, body, (indent + 1LL));
        /* pass */
        self->in_task_group = (self->in_task_group - 1LL);
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "_tr_taskgroup_wait();\n"));
    } else if (_t230.tag == HirStmt_SGpuBlock) {
        __auto_type body = _t230.data.SGpuBlock.body;
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "/* deprecated gpu: block - use std.gpu for GPU dispatch */\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "{\n"));
        /* pass */
        self->in_gpu_block = (self->in_gpu_block + 1LL);
        /* pass */
        CGenerator_gen_block(self, body, (indent + 1LL));
        /* pass */
        self->in_gpu_block = (self->in_gpu_block - 1LL);
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
    } else if (_t230.tag == HirStmt_SBreak) {
        CGenerator_w(self, _tr_str_concat(pad, "break;\n"));
    } else if (_t230.tag == HirStmt_SContinue) {
        CGenerator_w(self, _tr_str_concat(pad, "continue;\n"));
    } else if (_t230.tag == HirStmt_SPass) {
        CGenerator_w(self, _tr_str_concat(pad, "/* pass */\n"));
    } else if (_t230.tag == HirStmt_SFree) {
        __auto_type name = _t230.data.SFree.name;
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), name), ") { free("), name), "); "), name), " = NULL; }\n"));
    } else if (_t230.tag == HirStmt_SMultiLet) {
        __auto_type names = _t230.data.SMultiLet.names;
__auto_type is_mut = _t230.data.SMultiLet.is_mut;
__auto_type val = _t230.data.SMultiLet.val;
        /* pass */
        CGenerator_gen_multi_let(self, names, is_mut, val, indent);
    } else if (_t230.tag == HirStmt_SChanSelect) {
        __auto_type cs_arms = _t230.data.SChanSelect.cases;
        /* pass */
        CGenerator_gen_chan_select(self, cs_arms, indent);
    } else if (1) {
        __auto_type _ = _t230;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_gen_for_loop(CGenerator* self, char* var, HirExpr* iter, HirBlock* body, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* iter_s = CGenerator_gen_expr(self, iter);
    /* pass */
    __auto_type _t246 = (*iter);
    if (_t246.tag == HirExpr_ECall) {
        __auto_type callee = _t246.data.ECall.callee;
__auto_type args = _t246.data.ECall.args;
        /* pass */
        __auto_type _t247 = (*callee);
        if (_t247.tag == HirExpr_EIdent) {
            __auto_type n = _t247.data.EIdent.name;
            /* pass */
            if ((strcmp((char*)n, (char*)"range") == 0)) {
                /* pass */
                char* start_s = "0LL";
                /* pass */
                char* end_s = "0LL";
                /* pass */
                char* step_s = "1LL";
                /* pass */
                if ((args->len == 1LL)) {
                    /* pass */
                    end_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                } else if ((args->len == 2LL)) {
                    /* pass */
                    start_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    end_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
                } else if ((args->len >= 3LL)) {
                    /* pass */
                    start_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    end_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 1LL)));
                    /* pass */
                    step_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, 2LL)));
                }
                /* pass */
                char* cmp = "<";
                /* pass */
                char* step_p = ((char*)(step_s));
                /* pass */
                if ((((long long)((*(step_p + 0LL)))) == 45LL)) {
                    /* pass */
                    cmp = ">";
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "for (long long "), var), " = "), start_s), "; "), var), " "), cmp), " "), end_s), "; "), var), " += "), step_s), ") {\n"));
                /* pass */
                CGenerator_gen_block(self, body, (indent + 1LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
                /* pass */
                return;
            }
        } else if (1) {
            __auto_type _ = _t247;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t246;
        /* pass */
    }
    /* pass */
    char* tmp = CGenerator_next_temp(self);
    /* pass */
    AstType* iter_ty = hir_expr_type(iter);
    /* pass */
    char* iter_ty_n = iter_ty->name;
    /* pass */
    if (CGenerator_has_method(self, iter_ty_n, "__iter__")) {
        /* pass */
        AstType* iter_ret = CGenerator_cls_method_ret_ty(self, iter_ty_n, "__iter__");
        /* pass */
        char* it_cls = iter_ret->name;
        /* pass */
        if (((strcmp((char*)it_cls, (char*)"") == 0) || (strcmp((char*)it_cls, (char*)"void") == 0))) {
            /* pass */
            it_cls = iter_ty_n;
        }
        /* pass */
        if (CGenerator_has_method(self, it_cls, "__next__")) {
            /* pass */
            AstType* next_ret = CGenerator_cls_method_ret_ty(self, it_cls, "__next__");
            /* pass */
            char* elem_ct = "void*";
            /* pass */
            if (((strcmp((char*)next_ret->name, (char*)"Option") == 0) && (next_ret->args->len > 0LL))) {
                /* pass */
                elem_ct = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(next_ret->args, 0LL))));
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), tmp), "_it = "), CGenerator_cls_method_c_call(self, iter_ty_n, "__iter__", iter_s, "")), ";\n"));
            /* pass */
            CGenerator_w(self, _tr_str_concat(pad, "  while (1) {\n"));
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), tmp), "_nx = "), CGenerator_cls_method_c_call(self, it_cls, "__next__", _tr_str_concat(tmp, "_it"), "")), ";\n"));
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    if ("), tmp), "_nx.tag == Option_None) break;\n"));
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), elem_ct), " "), var), " = ("), elem_ct), ")"), tmp), "_nx.data.Some.val;\n"));
            /* pass */
            CGenerator_gen_block(self, body, (indent + 2LL));
            /* pass */
            CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
            /* pass */
            CGenerator_w(self, _tr_str_concat(pad, "}\n"));
            /* pass */
            return;
        }
    }
    /* pass */
    if ((CGenerator_has_method(self, iter_ty_n, "__len__") && CGenerator_has_method(self, iter_ty_n, "__getitem__"))) {
        /* pass */
        char* col_tmp = CGenerator_next_temp(self);
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), col_tmp), " = "), iter_s), ";\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  long long "), tmp), "_len = (long long)"), CGenerator_cls_method_c_call(self, iter_ty_n, "__len__", col_tmp, "")), ";\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (long long "), tmp), "_i = 0; "), tmp), "_i < "), tmp), "_len; "), tmp), "_i++) {\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), var), " = "), CGenerator_cls_method_c_call(self, iter_ty_n, "__getitem__", col_tmp, _tr_str_concat(tmp, "_i"))), ";\n"));
        /* pass */
        CGenerator_gen_block(self, body, (indent + 2LL));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
        /* pass */
        return;
    }
    /* pass */
    if ((strcmp((char*)iter_ty_n, (char*)"Chan") == 0)) {
        /* pass */
        char* ch_elem_c = "long long";
        /* pass */
        if ((iter_ty->args->len > 0LL)) {
            /* pass */
            ch_elem_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(iter_ty->args, 0LL))));
        }
        /* pass */
        char* ch_ok_v = CGenerator_next_temp(self);
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ int "), ch_ok_v), " = 1;\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  while ("), ch_ok_v), ") {\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), ch_elem_c), " "), var), " = ("), ch_elem_c), ")_tr_chan_recv_ok("), iter_s), ", &"), ch_ok_v), ");\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    if (!"), ch_ok_v), ") break;\n"));
        /* pass */
        CGenerator_gen_block(self, body, (indent + 2LL));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
        /* pass */
        return;
    }
    /* pass */
    char* elem_c = "__auto_type";
    /* pass */
    if (((strcmp((char*)iter_ty->name, (char*)"List") == 0) || (strcmp((char*)iter_ty->name, (char*)"Vec") == 0))) {
        /* pass */
        if ((iter_ty->args->len > 0LL)) {
            /* pass */
            elem_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(iter_ty->args, 0LL))));
        }
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), tmp), "_col = "), iter_s), ";\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (long long "), tmp), "_i = 0; "), tmp), "_i < "), tmp), "_col->len; "), tmp), "_i++) {\n"));
    /* pass */
    if ((strcmp((char*)elem_c, (char*)"__auto_type") == 0)) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), var), " = "), tmp), "_col->data["), tmp), "_i];\n"));
    } else {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), elem_c), " "), var), " = ("), elem_c), ")"), tmp), "_col->data["), tmp), "_i];\n"));
    }
    /* pass */
    CGenerator_gen_block(self, body, (indent + 2LL));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "}\n"));
}

__attribute__((hot)) void CGenerator_gen_for_unpack(CGenerator* self, List_str* vars, HirExpr* iter, HirBlock* body, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* tmp = CGenerator_next_temp(self);
    /* pass */
    __auto_type _t248 = (*iter);
    if (_t248.tag == HirExpr_ECall) {
        __auto_type eu_callee = _t248.data.ECall.callee;
__auto_type eu_args = _t248.data.ECall.args;
        /* pass */
        __auto_type _t249 = (*eu_callee);
        if (_t249.tag == HirExpr_EIdent) {
            __auto_type eu_n = _t249.data.EIdent.name;
            /* pass */
            if ((((strcmp((char*)eu_n, (char*)"enumerate") == 0) && (eu_args->len == 1LL)) && (vars->len >= 2LL))) {
                /* pass */
                HirExpr* eu_col = ((HirExpr*)List_ptr_get(eu_args, 0LL));
                /* pass */
                char* eu_col_s = CGenerator_gen_expr(self, eu_col);
                /* pass */
                AstType* eu_ty = hir_expr_type(eu_col);
                /* pass */
                char* eu_elem_c = "__auto_type";
                /* pass */
                if ((((strcmp((char*)eu_ty->name, (char*)"List") == 0) || (strcmp((char*)eu_ty->name, (char*)"Vec") == 0)) && (eu_ty->args->len > 0LL))) {
                    /* pass */
                    eu_elem_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(eu_ty->args, 0LL))));
                }
                /* pass */
                char* eu_v0 = _safe_c_varname(List_str_get(vars, 0LL));
                /* pass */
                char* eu_v1 = _safe_c_varname(List_str_get(vars, 1LL));
                /* pass */
                _tr_dict_set(self->decl_vars, List_str_get(vars, 0LL), true);
                /* pass */
                _tr_dict_set(self->decl_vars, List_str_get(vars, 1LL), true);
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), tmp), "_c = "), eu_col_s), ";\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (long long "), tmp), "_i = 0; "), tmp), "_i < (long long)"), tmp), "_c->len; "), tmp), "_i++) {\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    long long "), eu_v0), " = "), tmp), "_i;\n"));
                /* pass */
                if ((strcmp((char*)eu_elem_c, (char*)"__auto_type") == 0)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), eu_v1), " = "), tmp), "_c->data["), tmp), "_i];\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), eu_elem_c), " "), eu_v1), " = ("), eu_elem_c), ")"), tmp), "_c->data["), tmp), "_i];\n"));
                }
                /* pass */
                CGenerator_gen_block(self, body, (indent + 2LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
                /* pass */
                return;
            }
            /* pass */
            if ((((strcmp((char*)eu_n, (char*)"zip") == 0) && (eu_args->len == 2LL)) && (vars->len >= 2LL))) {
                /* pass */
                HirExpr* za_col = ((HirExpr*)List_ptr_get(eu_args, 0LL));
                /* pass */
                HirExpr* zb_col = ((HirExpr*)List_ptr_get(eu_args, 1LL));
                /* pass */
                char* za_s = CGenerator_gen_expr(self, za_col);
                /* pass */
                char* zb_s = CGenerator_gen_expr(self, zb_col);
                /* pass */
                AstType* za_ty = hir_expr_type(za_col);
                /* pass */
                AstType* zb_ty = hir_expr_type(zb_col);
                /* pass */
                char* za_c = "__auto_type";
                /* pass */
                char* zb_c = "__auto_type";
                /* pass */
                if ((((strcmp((char*)za_ty->name, (char*)"List") == 0) || (strcmp((char*)za_ty->name, (char*)"Vec") == 0)) && (za_ty->args->len > 0LL))) {
                    /* pass */
                    za_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(za_ty->args, 0LL))));
                }
                /* pass */
                if ((((strcmp((char*)zb_ty->name, (char*)"List") == 0) || (strcmp((char*)zb_ty->name, (char*)"Vec") == 0)) && (zb_ty->args->len > 0LL))) {
                    /* pass */
                    zb_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(zb_ty->args, 0LL))));
                }
                /* pass */
                char* zv0 = _safe_c_varname(List_str_get(vars, 0LL));
                /* pass */
                char* zv1 = _safe_c_varname(List_str_get(vars, 1LL));
                /* pass */
                _tr_dict_set(self->decl_vars, List_str_get(vars, 0LL), true);
                /* pass */
                _tr_dict_set(self->decl_vars, List_str_get(vars, 1LL), true);
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), tmp), "_a = "), za_s), "; __auto_type "), tmp), "_b = "), zb_s), ";\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  long long "), tmp), "_n = (long long)("), tmp), "_a->len < "), tmp), "_b->len ? "), tmp), "_a->len : "), tmp), "_b->len);\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (long long "), tmp), "_i = 0; "), tmp), "_i < "), tmp), "_n; "), tmp), "_i++) {\n"));
                /* pass */
                if ((strcmp((char*)za_c, (char*)"__auto_type") == 0)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), zv0), " = "), tmp), "_a->data["), tmp), "_i];\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), za_c), " "), zv0), " = ("), za_c), ")"), tmp), "_a->data["), tmp), "_i];\n"));
                }
                /* pass */
                if ((strcmp((char*)zb_c, (char*)"__auto_type") == 0)) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    __auto_type "), zv1), " = "), tmp), "_b->data["), tmp), "_i];\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    "), zb_c), " "), zv1), " = ("), zb_c), ")"), tmp), "_b->data["), tmp), "_i];\n"));
                }
                /* pass */
                CGenerator_gen_block(self, body, (indent + 2LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
                /* pass */
                return;
            }
        } else if (1) {
            __auto_type _ = _t249;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t248;
        /* pass */
    }
    /* pass */
    __auto_type _t250 = (*iter);
    if (_t250.tag == HirExpr_EMethodCall) {
        __auto_type di_obj = _t250.data.EMethodCall.obj;
__auto_type di_meth = _t250.data.EMethodCall.method;
        /* pass */
        if ((strcmp((char*)di_meth, (char*)"items") == 0)) {
            /* pass */
            char* di_s = CGenerator_gen_expr(self, di_obj);
            /* pass */
            AstType* di_ty = hir_expr_type(di_obj);
            /* pass */
            bool di_idict = false;
            /* pass */
            if ((di_ty->args->len > 0LL)) {
                /* pass */
                AstType* di_ka = (*((AstType**)List_ptr_get(di_ty->args, 0LL)));
                /* pass */
                if (((((_is_int_type(di_ka->name) || (strcmp((char*)di_ka->name, (char*)"int") == 0)) || (strcmp((char*)di_ka->name, (char*)"i64") == 0)) || (strcmp((char*)di_ka->name, (char*)"i32") == 0)) || (strcmp((char*)di_ka->name, (char*)"usize") == 0))) {
                    /* pass */
                    di_idict = true;
                }
            }
            /* pass */
            char* di_v0 = _safe_c_varname(List_str_get(vars, 0LL));
            /* pass */
            char* di_v1 = "_di_ign";
            /* pass */
            if ((vars->len > 1LL)) {
                /* pass */
                di_v1 = _safe_c_varname(List_str_get(vars, 1LL));
            }
            /* pass */
            _tr_dict_set(self->decl_vars, List_str_get(vars, 0LL), true);
            /* pass */
            if ((vars->len > 1LL)) {
                /* pass */
                _tr_dict_set(self->decl_vars, List_str_get(vars, 1LL), true);
            }
            /* pass */
            if (di_idict) {
                /* pass */
                char* di_val_c = "void*";
                /* pass */
                if ((di_ty->args->len > 1LL)) {
                    /* pass */
                    di_val_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(di_ty->args, 1LL))));
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ TrIDict* "), tmp), "_d = "), di_s), ";\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (size_t "), tmp), "_bi = 0; "), tmp), "_bi < "), tmp), "_d->cap; "), tmp), "_bi++) {\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    _TrIDictNode* "), tmp), "_nd = "), tmp), "_d->buckets["), tmp), "_bi];\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    while ("), tmp), "_nd) {\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      long long "), di_v0), " = "), tmp), "_nd->key;\n"));
                /* pass */
                if (((strcmp((char*)di_val_c, (char*)"void*") == 0) || (strcmp((char*)di_val_c, (char*)"void") == 0))) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      void* "), di_v1), " = "), tmp), "_nd->value;\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      "), di_val_c), " "), di_v1), " = ("), di_val_c), ")(uintptr_t)"), tmp), "_nd->value;\n"));
                }
                /* pass */
                CGenerator_gen_block(self, body, (indent + 3LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      "), tmp), "_nd = "), tmp), "_nd->next;\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
            } else {
                /* pass */
                char* di_val_c = "void*";
                /* pass */
                if ((di_ty->args->len > 1LL)) {
                    /* pass */
                    di_val_c = CGenerator_type_to_c(self, (*((AstType**)List_ptr_get(di_ty->args, 1LL))));
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ TrMap* "), tmp), "_d = "), di_s), ";\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (size_t "), tmp), "_bi = 0; "), tmp), "_bi < "), tmp), "_d->cap; "), tmp), "_bi++) {\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    _DictNode* "), tmp), "_nd = "), tmp), "_d->buckets["), tmp), "_bi];\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    while ("), tmp), "_nd) {\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      char* "), di_v0), " = "), tmp), "_nd->key;\n"));
                /* pass */
                if (((strcmp((char*)di_val_c, (char*)"void*") == 0) || (strcmp((char*)di_val_c, (char*)"void") == 0))) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      void* "), di_v1), " = "), tmp), "_nd->value;\n"));
                } else {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      "), di_val_c), " "), di_v1), " = ("), di_val_c), ")(uintptr_t)"), tmp), "_nd->value;\n"));
                }
                /* pass */
                CGenerator_gen_block(self, body, (indent + 3LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      "), tmp), "_nd = "), tmp), "_nd->next;\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "}\n"));
            }
            /* pass */
            return;
        }
    } else if (1) {
        __auto_type _ = _t250;
        /* pass */
    }
    /* pass */
    char* fu_iter_s = CGenerator_gen_expr(self, iter);
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "{ __auto_type "), tmp), "_col = "), fu_iter_s), ";\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  for (long long "), tmp), "_i = 0; "), tmp), "_i < (long long)"), tmp), "_col->len; "), tmp), "_i++) {\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    TrTuple* "), tmp), "_pair = (TrTuple*)"), tmp), "_col->data["), tmp), "_i];\n"));
    /* pass */
    long long fu_pi = 0LL;
    /* pass */
    while ((fu_pi < vars->len)) {
        /* pass */
        char* fu_vn = _safe_c_varname(List_str_get(vars, fu_pi));
        /* pass */
        _tr_dict_set(self->decl_vars, List_str_get(vars, fu_pi), true);
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    long long "), fu_vn), " = "), tmp), "_pair->data["), _tr_int_to_str((long long)(fu_pi))), "];\n"));
        /* pass */
        fu_pi = (fu_pi + 1LL);
    }
    /* pass */
    CGenerator_gen_block(self, body, (indent + 2LL));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "}\n"));
}

__attribute__((hot)) void CGenerator_gen_try(CGenerator* self, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* jb = CGenerator_next_temp(self);
    /* pass */
    char* em = CGenerator_next_temp(self);
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "{\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    jmp_buf "), jb), "; char* "), em), " = NULL;\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    _tr_exc_push(&"), jb), ", &"), em), ");\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    if (setjmp("), jb), ") == 0) {\n"));
    /* pass */
    CGenerator_gen_block(self, try_body, (indent + 2LL));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "        _tr_exc_pop();\n"));
    /* pass */
    if ((catches->len > 0LL)) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "    } else {\n"));
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < catches->len)) {
            /* pass */
            HirCatchClause* cc = (*((HirCatchClause**)List_ptr_get(catches, i)));
            /* pass */
            if ((strcmp((char*)cc->err_name, (char*)"") != 0)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "        char* "), cc->err_name), " = "), em), ";\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "        (void)"), cc->err_name), ";\n"));
            }
            /* pass */
            CGenerator_gen_block(self, cc->body, (indent + 2LL));
            /* pass */
            i = (i + 1LL);
        }
    } else {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "    } else {\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "        /* exception not caught */\n"));
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
    /* pass */
    if (((((unsigned long long)(finally_b)) != ((unsigned long long)(0LL))) && (finally_b->stmts->len > 0LL))) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "    /* finally */\n"));
        /* pass */
        CGenerator_gen_block(self, finally_b, (indent + 1LL));
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "}\n"));
}

__attribute__((hot)) void CGenerator_gen_chan_select(CGenerator* self, List_ptr* arms, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* tmp = CGenerator_next_temp(self);
    /* pass */
    long long n = arms->len;
    /* pass */
    bool has_default = false;
    /* pass */
    bool has_timeout = false;
    /* pass */
    char* timeout_ms_s = "5000LL";
    /* pass */
    long long default_idx = (-1LL);
    /* pass */
    long long timeout_idx = (-1LL);
    /* pass */
    long long active_n = 0LL;
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < n)) {
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(((HirChanSelectArm**)List_ptr_get(arms, ai))))))) {
            /* pass */
            HirChanSelectArm* ak = (*((HirChanSelectArm**)List_ptr_get(arms, ai)));
            /* pass */
            if ((ak->kind == 3LL)) {
                /* pass */
                has_default = true;
                /* pass */
                default_idx = ai;
            } else if ((ak->kind == 2LL)) {
                /* pass */
                has_timeout = true;
                /* pass */
                timeout_idx = ai;
                /* pass */
                if ((!_is_invalid_ptr(((unsigned long long)(ak->timeout_ms))))) {
                    /* pass */
                    timeout_ms_s = CGenerator_gen_expr(self, ak->timeout_ms);
                }
            } else {
                /* pass */
                active_n = (active_n + 1LL);
            }
        }
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    if ((active_n == 0LL)) {
        /* pass */
        active_n = 1LL;
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "{ /* chan_select */\n"));
    /* pass */
    if (has_timeout) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  long long _cst_dl_"), tmp), " = _tr_monotonic_ms() + (long long)("), timeout_ms_s), ");\n"));
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  int _cst_start_"), tmp), " = (int)(rand() % "), _tr_int_to_str((long long)(active_n))), ");\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  int _cst_done_"), tmp), " = 0;\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  while (!_cst_done_"), tmp), ") {\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    int _cst_active_"), tmp), "["), _tr_int_to_str((long long)(n))), "]; int _cst_na_"), tmp), " = 0;\n"));
    /* pass */
    long long idx2 = 0LL;
    /* pass */
    while ((idx2 < n)) {
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(((HirChanSelectArm**)List_ptr_get(arms, idx2))))))) {
            /* pass */
            HirChanSelectArm* ak2 = (*((HirChanSelectArm**)List_ptr_get(arms, idx2)));
            /* pass */
            if (((ak2->kind != 2LL) && (ak2->kind != 3LL))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    _cst_active_"), tmp), "[_cst_na_"), tmp), "++] = "), _tr_int_to_str((long long)(idx2))), ";\n"));
            }
        }
        /* pass */
        idx2 = (idx2 + 1LL);
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    for (int _csi_"), tmp), " = 0; _csi_"), tmp), " < _cst_na_"), tmp), " && !_cst_done_"), tmp), "; _csi_"), tmp), "++) {\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      int _csc_"), tmp), " = _cst_active_"), tmp), "[(_csi_"), tmp), " + _cst_start_"), tmp), ") % (_cst_na_"), tmp), " > 0 ? _cst_na_"), tmp), " : 1)];\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      switch (_csc_"), tmp), ") {\n"));
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < n)) {
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(((HirChanSelectArm**)List_ptr_get(arms, bi))))))) {
            /* pass */
            HirChanSelectArm* arm = (*((HirChanSelectArm**)List_ptr_get(arms, bi)));
            /* pass */
            if (((arm->kind != 2LL) && (arm->kind != 3LL))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "        case "), _tr_int_to_str((long long)(bi))), ": {\n"));
                /* pass */
                if ((arm->kind == 0LL)) {
                    /* pass */
                    char* chan_s = "";
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(arm->chan_expr))))) {
                        /* pass */
                        __auto_type _t251 = (*arm->chan_expr);
                        if (_t251.tag == HirExpr_EMethodCall) {
                            __auto_type rc_obj = _t251.data.EMethodCall.obj;
__auto_type rc_meth = _t251.data.EMethodCall.method;
                            /* pass */
                            if (((strcmp((char*)rc_meth, (char*)"recv") == 0) || (strcmp((char*)rc_meth, (char*)"receive") == 0))) {
                                /* pass */
                                chan_s = CGenerator_gen_expr(self, rc_obj);
                            } else {
                                /* pass */
                                chan_s = CGenerator_gen_expr(self, arm->chan_expr);
                            }
                        } else if (1) {
                            __auto_type _ = _t251;
                            /* pass */
                            chan_s = CGenerator_gen_expr(self, arm->chan_expr);
                        }
                    }
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "          long long _crv_"), tmp), "_"), _tr_int_to_str((long long)(bi))), " = _tr_chan_try_recv_val("), chan_s), ");\n"));
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "          if (_crv_"), tmp), "_"), _tr_int_to_str((long long)(bi))), " != LLONG_MIN) {\n"));
                    /* pass */
                    if ((strcmp((char*)arm->var_name, (char*)"") != 0)) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "            long long "), _safe_c_varname(arm->var_name)), " = _crv_"), tmp), "_"), _tr_int_to_str((long long)(bi))), ";\n"));
                    }
                    /* pass */
                    CGenerator_gen_block(self, arm->body, (indent + 3LL));
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "            _cst_done_"), tmp), " = 1;\n"));
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(pad, "          }\n"));
                } else if ((arm->kind == 1LL)) {
                    /* pass */
                    char* send_chan_s = "";
                    /* pass */
                    char* send_val_s = "0LL";
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(arm->chan_expr))))) {
                        /* pass */
                        __auto_type _t252 = (*arm->chan_expr);
                        if (_t252.tag == HirExpr_EMethodCall) {
                            __auto_type sc_obj = _t252.data.EMethodCall.obj;
__auto_type sc_meth = _t252.data.EMethodCall.method;
__auto_type sc_args = _t252.data.EMethodCall.args;
                            /* pass */
                            send_chan_s = CGenerator_gen_expr(self, sc_obj);
                            /* pass */
                            if ((sc_args->len > 0LL)) {
                                /* pass */
                                send_val_s = CGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(sc_args, 0LL)));
                            }
                        } else if (1) {
                            __auto_type _ = _t252;
                            /* pass */
                            send_chan_s = CGenerator_gen_expr(self, arm->chan_expr);
                        }
                    }
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(arm->val_expr))))) {
                        /* pass */
                        send_val_s = CGenerator_gen_expr(self, arm->val_expr);
                    }
                    /* pass */
                    if ((strcmp((char*)send_chan_s, (char*)"") != 0)) {
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "          if (_tr_chan_try_send("), send_chan_s), ", "), send_val_s), ")) {\n"));
                        /* pass */
                        CGenerator_gen_block(self, arm->body, (indent + 3LL));
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "            _cst_done_"), tmp), " = 1;\n"));
                        /* pass */
                        CGenerator_w(self, _tr_str_concat(pad, "          }\n"));
                    }
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "          break;\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "        }\n"));
            }
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "        default: break;\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "      }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    if (!_cst_done_"), tmp), ") {\n"));
    /* pass */
    if (has_timeout) {
        /* pass */
        HirChanSelectArm* to_arm = (*((HirChanSelectArm**)List_ptr_get(arms, timeout_idx)));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "      if (_tr_monotonic_ms() >= _cst_dl_"), tmp), ") {\n"));
        /* pass */
        CGenerator_gen_block(self, to_arm->body, (indent + 3LL));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "        _cst_done_"), tmp), " = 1;\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "      } else "));
    } else {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "      "));
    }
    /* pass */
    if (has_default) {
        /* pass */
        HirChanSelectArm* def_arm = (*((HirChanSelectArm**)List_ptr_get(arms, default_idx)));
        /* pass */
        if (has_timeout) {
            /* pass */
            CGenerator_w(self, "{\n");
        } else {
            /* pass */
            CGenerator_w(self, "{\n");
        }
        /* pass */
        CGenerator_gen_block(self, def_arm->body, (indent + 3LL));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "        _cst_done_"), tmp), " = 1;\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "      }\n"));
    } else if ((!has_timeout)) {
        /* pass */
        CGenerator_w(self, "{ _tr_thread_sleep_ms(0); }\n");
    } else {
        /* pass */
        CGenerator_w(self, "{ _tr_thread_sleep_ms(0); }\n");
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
    /* pass */
    CGenerator_w(self, _tr_str_concat(pad, "} /* end chan_select */\n"));
}

__attribute__((hot)) void CGenerator_gen_block(CGenerator* self, HirBlock* b, long long indent) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(b)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(b->stmts)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        CGenerator_gen_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)), indent);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_gen_match(CGenerator* self, HirExpr* expr, List_ptr* arms, long long indent) {
    /* pass */
    __auto_type pad = _indent_str(indent);
    /* pass */
    char* subj = CGenerator_next_temp(self);
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "__auto_type "), subj), " = "), CGenerator_gen_expr(self, expr)), ";\n"));
    /* pass */
    bool has_any_guard = false;
    /* pass */
    long long gk = 0LL;
    /* pass */
    while ((gk < arms->len)) {
        /* pass */
        if ((((unsigned long long)(((HirMatchArm*)List_ptr_get(arms, gk))->guard)) != ((unsigned long long)(0LL)))) {
            /* pass */
            has_any_guard = true;
        }
        /* pass */
        gk = (gk + 1LL);
    }
    /* pass */
    if (has_any_guard) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "do {\n"));
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < arms->len)) {
            /* pass */
            HirMatchArm* garm = ((HirMatchArm*)List_ptr_get(arms, gi));
            /* pass */
            char* gcond = "1 /* wildcard */";
            /* pass */
            char* gbindings = "";
            /* pass */
            __auto_type _t253 = garm->pat;
            if (_t253.tag == Pattern_PLitInt) {
                __auto_type v = _t253.data.PLitInt.val;
                gcond = _tr_str_concat(_tr_str_concat(_tr_str_concat(subj, " == "), _tr_int_to_str((long long)(v))), "LL");
            } else if (_t253.tag == Pattern_PLitBool) {
                __auto_type v = _t253.data.PLitBool.val;
                /* pass */
                if (v) {
                    /* pass */
                    gcond = subj;
                } else {
                    /* pass */
                    gcond = _tr_str_concat("!", subj);
                }
            } else if (_t253.tag == Pattern_PLitStr) {
                __auto_type v = _t253.data.PLitStr.val;
                gcond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_eq(", subj), ", \""), _escape_str_for_c(v)), "\")");
            } else if (_t253.tag == Pattern_PVariant) {
                __auto_type tn = _t253.data.PVariant.type_name;
__auto_type vn = _t253.data.PVariant.variant;
                /* pass */
                char* gpv = vn;
                /* pass */
                if (((strcmp((char*)tn, (char*)"Option") == 0) && ((strcmp((char*)vn, (char*)"") == 0) || (strcmp((char*)vn, (char*)"none") == 0)))) {
                    /* pass */
                    gpv = "None";
                }
                /* pass */
                gcond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), gpv);
            } else if (_t253.tag == Pattern_PVariantBind) {
                __auto_type tn = _t253.data.PVariantBind.type_name;
__auto_type vn = _t253.data.PVariantBind.variant;
__auto_type field = _t253.data.PVariantBind.field;
                /* pass */
                gcond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), vn);
                /* pass */
                char* gaf = field;
                /* pass */
                if (((strcmp((char*)tn, (char*)"Option") == 0) && (strcmp((char*)vn, (char*)"Some") == 0))) {
                    /* pass */
                    gaf = "val";
                }
                /* pass */
                if (((strcmp((char*)tn, (char*)"Result") == 0) && (strcmp((char*)vn, (char*)"Ok") == 0))) {
                    /* pass */
                    gaf = "val";
                }
                /* pass */
                if (((strcmp((char*)tn, (char*)"Result") == 0) && (strcmp((char*)vn, (char*)"Err") == 0))) {
                    /* pass */
                    gaf = "err";
                }
                /* pass */
                if (_tr_dict_contains(self->enums, tn)) {
                    /* pass */
                    HirEnum* _gev = ((HirEnum*)(uintptr_t)_tr_dict_get(self->enums, tn));
                    /* pass */
                    long long _gvi = 0LL;
                    /* pass */
                    while ((_gvi < _gev->variants->len)) {
                        /* pass */
                        HirVariant* _gv = ((HirVariant*)List_ptr_get(_gev->variants, _gvi));
                        /* pass */
                        if (((strcmp((char*)_gv->name, (char*)vn) == 0) && (_gv->fields->len > 0LL))) {
                            /* pass */
                            gaf = ((HirParam*)List_ptr_get(_gv->fields, 0LL))->name;
                        }
                        /* pass */
                        _gvi = (_gvi + 1LL);
                    }
                }
                /* pass */
                _tr_dict_set(self->decl_vars, field, true);
                /* pass */
                gbindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("__auto_type ", _safe_c_varname(field)), " = "), subj), ".data."), vn), "."), gaf), ";\n");
            } else if (_t253.tag == Pattern_PVariantBindMany) {
                __auto_type tn = _t253.data.PVariantBindMany.type_name;
__auto_type vn = _t253.data.PVariantBindMany.variant;
__auto_type fields = _t253.data.PVariantBindMany.fields;
                /* pass */
                gcond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), vn);
                /* pass */
                long long _gbi = 0LL;
                /* pass */
                while ((_gbi < fields->len)) {
                    /* pass */
                    char* _gbf = List_str_get(fields, _gbi);
                    /* pass */
                    if ((strcmp((char*)_gbf, (char*)"_") != 0)) {
                        /* pass */
                        char* _gact = _gbf;
                        /* pass */
                        if (_tr_dict_contains(self->enums, tn)) {
                            /* pass */
                            HirEnum* _gev2 = ((HirEnum*)(uintptr_t)_tr_dict_get(self->enums, tn));
                            /* pass */
                            long long _gvi2 = 0LL;
                            /* pass */
                            while ((_gvi2 < _gev2->variants->len)) {
                                /* pass */
                                HirVariant* _gv2 = ((HirVariant*)List_ptr_get(_gev2->variants, _gvi2));
                                /* pass */
                                if (((strcmp((char*)_gv2->name, (char*)vn) == 0) && (_gbi < _gv2->fields->len))) {
                                    /* pass */
                                    _gact = ((HirParam*)List_ptr_get(_gv2->fields, _gbi))->name;
                                }
                                /* pass */
                                _gvi2 = (_gvi2 + 1LL);
                            }
                        }
                        /* pass */
                        _tr_dict_set(self->decl_vars, _gbf, true);
                        /* pass */
                        gbindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(gbindings, "__auto_type "), _safe_c_varname(_gbf)), " = "), subj), ".data."), vn), "."), _gact), ";\n");
                    }
                    /* pass */
                    _gbi = (_gbi + 1LL);
                }
            } else if (_t253.tag == Pattern_PBind) {
                __auto_type gn = _t253.data.PBind.name;
                /* pass */
                gcond = "1";
                /* pass */
                _tr_dict_set(self->decl_vars, gn, true);
                /* pass */
                gbindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("__auto_type ", _safe_c_varname(gn)), " = "), subj), ";\n");
            } else if (_t253.tag == Pattern_PWild) {
                gcond = "1";
            } else if (1) {
                __auto_type _ = _t253;
                gcond = "1";
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "  if ("), gcond), ") {\n"));
            /* pass */
            if ((strcmp((char*)gbindings, (char*)"") != 0)) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, "    "), gbindings));
            }
            /* pass */
            if ((((unsigned long long)(garm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "    if ("), CGenerator_gen_expr(self, garm->guard)), ") {\n"));
                /* pass */
                CGenerator_gen_block(self, garm->body, (indent + 3LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "      break;\n"));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "    }\n"));
            } else {
                /* pass */
                CGenerator_gen_block(self, garm->body, (indent + 2LL));
                /* pass */
                CGenerator_w(self, _tr_str_concat(pad, "    break;\n"));
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(pad, "  }\n"));
            /* pass */
            gi = (gi + 1LL);
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "} while(0);\n"));
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < arms->len)) {
        /* pass */
        HirMatchArm* arm = ((HirMatchArm*)List_ptr_get(arms, i));
        /* pass */
        char* cond = "1 /* wildcard */";
        /* pass */
        char* bindings = "";
        /* pass */
        __auto_type _t254 = arm->pat;
        if (_t254.tag == Pattern_PLitInt) {
            __auto_type v = _t254.data.PLitInt.val;
            cond = _tr_str_concat(_tr_str_concat(_tr_str_concat(subj, " == "), _tr_int_to_str((long long)(v))), "LL");
        } else if (_t254.tag == Pattern_PLitBool) {
            __auto_type v = _t254.data.PLitBool.val;
            /* pass */
            if (v) {
                /* pass */
                cond = subj;
            } else {
                /* pass */
                cond = _tr_str_concat("!", subj);
            }
        } else if (_t254.tag == Pattern_PLitStr) {
            __auto_type v = _t254.data.PLitStr.val;
            cond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_eq(", subj), ", \""), _escape_str_for_c(v)), "\")");
        } else if (_t254.tag == Pattern_PVariant) {
            __auto_type tn = _t254.data.PVariant.type_name;
__auto_type vn = _t254.data.PVariant.variant;
            /* pass */
            char* pv_vn = vn;
            /* pass */
            if (((strcmp((char*)tn, (char*)"Option") == 0) && ((strcmp((char*)vn, (char*)"") == 0) || (strcmp((char*)vn, (char*)"none") == 0)))) {
                /* pass */
                pv_vn = "None";
            }
            /* pass */
            cond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), pv_vn);
        } else if (_t254.tag == Pattern_PVariantBind) {
            __auto_type tn = _t254.data.PVariantBind.type_name;
__auto_type vn = _t254.data.PVariantBind.variant;
__auto_type field = _t254.data.PVariantBind.field;
            /* pass */
            cond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), vn);
            /* pass */
            char* actual_fld = field;
            /* pass */
            if (((strcmp((char*)tn, (char*)"Option") == 0) && (strcmp((char*)vn, (char*)"Some") == 0))) {
                /* pass */
                actual_fld = "val";
            }
            /* pass */
            if (((strcmp((char*)tn, (char*)"Result") == 0) && (strcmp((char*)vn, (char*)"Ok") == 0))) {
                /* pass */
                actual_fld = "val";
            }
            /* pass */
            if (((strcmp((char*)tn, (char*)"Result") == 0) && (strcmp((char*)vn, (char*)"Err") == 0))) {
                /* pass */
                actual_fld = "err";
            }
            /* pass */
            if (_tr_dict_contains(self->enums, tn)) {
                /* pass */
                HirEnum* _ev_def = ((HirEnum*)(uintptr_t)_tr_dict_get(self->enums, tn));
                /* pass */
                long long _vi = 0LL;
                /* pass */
                while ((_vi < _ev_def->variants->len)) {
                    /* pass */
                    HirVariant* _ev = ((HirVariant*)List_ptr_get(_ev_def->variants, _vi));
                    /* pass */
                    if (((strcmp((char*)_ev->name, (char*)vn) == 0) && (_ev->fields->len > 0LL))) {
                        /* pass */
                        actual_fld = ((HirParam*)List_ptr_get(_ev->fields, 0LL))->name;
                    }
                    /* pass */
                    _vi = (_vi + 1LL);
                }
            }
            /* pass */
            _tr_dict_set(self->decl_vars, field, true);
            /* pass */
            bindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("__auto_type ", _safe_c_varname(field)), " = "), subj), ".data."), vn), "."), actual_fld), ";\n");
        } else if (_t254.tag == Pattern_PVariantBindMany) {
            __auto_type tn = _t254.data.PVariantBindMany.type_name;
__auto_type vn = _t254.data.PVariantBindMany.variant;
__auto_type fields = _t254.data.PVariantBindMany.fields;
            /* pass */
            cond = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), vn);
            /* pass */
            long long _bi = 0LL;
            /* pass */
            while ((_bi < fields->len)) {
                /* pass */
                char* _bf = List_str_get(fields, _bi);
                /* pass */
                if ((strcmp((char*)_bf, (char*)"_") != 0)) {
                    /* pass */
                    char* _actual = _bf;
                    /* pass */
                    if (_tr_dict_contains(self->enums, tn)) {
                        /* pass */
                        HirEnum* _ev_def2 = ((HirEnum*)(uintptr_t)_tr_dict_get(self->enums, tn));
                        /* pass */
                        long long _vi2 = 0LL;
                        /* pass */
                        while ((_vi2 < _ev_def2->variants->len)) {
                            /* pass */
                            HirVariant* _ev2 = ((HirVariant*)List_ptr_get(_ev_def2->variants, _vi2));
                            /* pass */
                            if (((strcmp((char*)_ev2->name, (char*)vn) == 0) && (_bi < _ev2->fields->len))) {
                                /* pass */
                                _actual = ((HirParam*)List_ptr_get(_ev2->fields, _bi))->name;
                            }
                            /* pass */
                            _vi2 = (_vi2 + 1LL);
                        }
                    }
                    /* pass */
                    _tr_dict_set(self->decl_vars, _bf, true);
                    /* pass */
                    bindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(bindings, "__auto_type "), _safe_c_varname(_bf)), " = "), subj), ".data."), vn), "."), _actual), ";\n");
                }
                /* pass */
                _bi = (_bi + 1LL);
            }
        } else if (_t254.tag == Pattern_PBind) {
            __auto_type n = _t254.data.PBind.name;
            /* pass */
            cond = "1";
            /* pass */
            _tr_dict_set(self->decl_vars, n, true);
            /* pass */
            bindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("__auto_type ", _safe_c_varname(n)), " = "), subj), ";\n");
        } else if (_t254.tag == Pattern_PWild) {
            cond = "1";
        } else if (_t254.tag == Pattern_POr) {
            __auto_type pats = _t254.data.POr.patterns;
            /* pass */
            char* or_conds = "";
            /* pass */
            long long oi = 0LL;
            /* pass */
            while ((oi < pats->len)) {
                /* pass */
                Pattern pp = List_Pattern_get(pats, oi);
                /* pass */
                char* pc = "1";
                /* pass */
                __auto_type _t255 = pp;
                if (_t255.tag == Pattern_PVariant) {
                    __auto_type tn = _t255.data.PVariant.type_name;
__auto_type vn = _t255.data.PVariant.variant;
                    /* pass */
                    char* or_vn = vn;
                    /* pass */
                    if (((strcmp((char*)tn, (char*)"Option") == 0) && ((strcmp((char*)vn, (char*)"") == 0) || (strcmp((char*)vn, (char*)"none") == 0)))) {
                        /* pass */
                        or_vn = "None";
                    }
                    /* pass */
                    pc = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(subj, ".tag == "), tn), "_"), or_vn);
                } else if (_t255.tag == Pattern_PLitInt) {
                    __auto_type vi = _t255.data.PLitInt.val;
                    pc = _tr_str_concat(_tr_str_concat(_tr_str_concat(subj, " == "), _tr_int_to_str((long long)(vi))), "LL");
                } else if (_t255.tag == Pattern_PLitStr) {
                    __auto_type vs = _t255.data.PLitStr.val;
                    pc = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("_tr_str_eq(", subj), ", \""), _escape_str_for_c(vs)), "\")");
                } else if (_t255.tag == Pattern_PLitBool) {
                    __auto_type bv = _t255.data.PLitBool.val;
                    /* pass */
                    if (bv) {
                        /* pass */
                        pc = subj;
                    } else {
                        /* pass */
                        pc = _tr_str_concat("!", subj);
                    }
                } else if (_t255.tag == Pattern_PBind) {
                    __auto_type bn = _t255.data.PBind.name;
                    /* pass */
                    pc = "1";
                    /* pass */
                    _tr_dict_set(self->decl_vars, bn, true);
                    /* pass */
                    bindings = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(bindings, "__auto_type "), _safe_c_varname(bn)), " = "), subj), ";\n");
                } else if (1) {
                    __auto_type _ = _t255;
                    pc = "1";
                }
                /* pass */
                if ((oi == 0LL)) {
                    /* pass */
                    or_conds = pc;
                } else {
                    /* pass */
                    or_conds = _tr_str_concat(_tr_str_concat(or_conds, " || "), pc);
                }
                /* pass */
                oi = (oi + 1LL);
            }
            /* pass */
            cond = _tr_str_concat(_tr_str_concat("(", or_conds), ")");
        } else if (1) {
            __auto_type _ = _t254;
            cond = "1";
        }
        /* pass */
        if ((i == 0LL)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "if ("), cond), ") {\n"));
        } else {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(pad, "} else if ("), cond), ") {\n"));
        }
        /* pass */
        if ((strcmp((char*)bindings, (char*)"") != 0)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(pad, "    "), bindings));
        }
        /* pass */
        CGenerator_gen_block(self, arm->body, (indent + 1LL));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((arms->len > 0LL)) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(pad, "}\n"));
    }
}

__attribute__((hot)) void CGenerator_register_program(CGenerator* self, HirProgram* prog) {
    /* pass */
    if ((!_is_invalid_ptr(((unsigned long long)(prog->type_alias_names))))) {
        /* pass */
        long long tai = 0LL;
        /* pass */
        while ((tai < prog->type_alias_names->len)) {
            /* pass */
            char* ta_name = List_str_get(prog->type_alias_names, tai);
            /* pass */
            AstType** ta_ty_p = ((AstType**)List_ptr_get(prog->type_alias_types, tai));
            /* pass */
            if ((((unsigned long long)(ta_ty_p)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _tr_dict_set(self->type_alias_map, ta_name, CGenerator_type_to_c(self, (*ta_ty_p)));
            }
            /* pass */
            tai = (tai + 1LL);
        }
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        _tr_dict_set(self->classes, ((HirClass*)List_ptr_get(prog->classes, i))->name, ((HirClass*)List_ptr_get(prog->classes, i)));
        /* pass */
        if ((!((HirClass*)List_ptr_get(prog->classes, i))->is_class)) {
            /* pass */
            _tr_dict_set(self->value_types, ((HirClass*)List_ptr_get(prog->classes, i))->name, true);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        _tr_dict_set(self->enums, ((HirEnum*)List_ptr_get(prog->enums, i))->name, ((HirEnum*)List_ptr_get(prog->enums, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->interfaces->len)) {
        /* pass */
        _tr_dict_set(self->interfaces, ((HirInterface*)List_ptr_get(prog->interfaces, i))->name, ((HirInterface*)List_ptr_get(prog->interfaces, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* pf = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp((char*)pf->class_name, (char*)"") == 0) || (!_tr_dict_contains(self->functions, pf->name)))) {
            /* pass */
            _tr_dict_set(self->functions, pf->name, pf);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->decorator_defs->len)) {
        /* pass */
        HirFunction* _dd = ((HirFunction*)List_ptr_get(prog->decorator_defs, i));
        /* pass */
        _tr_dict_set(self->decorator_defs, _dd->name, _dd);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* ov_cls = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((ov_cls->generics->len == 0LL)) {
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < ov_cls->methods->len)) {
                /* pass */
                HirFunction* m1 = ((HirFunction*)List_ptr_get(ov_cls->methods, mi));
                /* pass */
                long long mj = (mi + 1LL);
                /* pass */
                while ((mj < ov_cls->methods->len)) {
                    /* pass */
                    if ((strcmp((char*)((HirFunction*)List_ptr_get(ov_cls->methods, mj))->name, (char*)m1->name) == 0)) {
                        /* pass */
                        _tr_dict_set(self->overloaded_sigs, _tr_str_concat(_tr_str_concat(ov_cls->name, "_"), m1->name), true);
                        /* pass */
                        break;
                    }
                    /* pass */
                    mj = (mj + 1LL);
                }
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* ov_enm = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < ov_enm->methods->len)) {
            /* pass */
            HirFunction* m1 = ((HirFunction*)List_ptr_get(ov_enm->methods, mi));
            /* pass */
            long long mj = (mi + 1LL);
            /* pass */
            while ((mj < ov_enm->methods->len)) {
                /* pass */
                if ((strcmp((char*)((HirFunction*)List_ptr_get(ov_enm->methods, mj))->name, (char*)m1->name) == 0)) {
                    /* pass */
                    _tr_dict_set(self->overloaded_sigs, _tr_str_concat(_tr_str_concat(ov_enm->name, "_"), m1->name), true);
                    /* pass */
                    break;
                }
                /* pass */
                mj = (mj + 1LL);
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_scan_mono_ty(CGenerator* self, AstType* ty) {
    /* pass */
    if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if ((strcmp((char*)ty->name, (char*)"") == 0)) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        CGenerator_scan_mono_ty(self, (*((AstType**)List_ptr_get(ty->args, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((((((ty->args->len > 0LL) && _tr_dict_contains(self->classes, ty->name)) && (strcmp((char*)ty->name, (char*)"Vec") != 0)) && (strcmp((char*)ty->name, (char*)"Map") != 0)) && (strcmp((char*)ty->name, (char*)"Dict") != 0)) && (strcmp((char*)ty->name, (char*)"List") != 0))) {
        /* pass */
        HirClass* ucls = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, ty->name));
        /* pass */
        if ((ucls->generics->len > 0LL)) {
            /* pass */
            CGenerator_ensure_mono(self, ucls, ty->args);
        }
    }
}

__attribute__((hot)) void CGenerator_scan_mono_block(CGenerator* self, HirBlock* block) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(block->stmts)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < block->stmts->len)) {
        /* pass */
        CGenerator_scan_mono_stmt(self, ((HirStmt*)List_ptr_get(block->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void CGenerator_scan_mono_expr(CGenerator* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t256 = (*e);
    if (_t256.tag == HirExpr_ECall) {
        __auto_type callee = _t256.data.ECall.callee;
__auto_type args = _t256.data.ECall.args;
        /* pass */
        CGenerator_scan_mono_expr(self, callee);
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < args->len)) {
            /* pass */
            CGenerator_scan_mono_expr(self, ((HirExpr*)List_ptr_get(args, ei)));
            /* pass */
            ei = (ei + 1LL);
        }
        /* pass */
        __auto_type _t257 = (*callee);
        if (_t257.tag == HirExpr_EIdent) {
            __auto_type mn = _t257.data.EIdent.name;
            /* pass */
            if (_tr_str_contains(mn, "__MONO_")) {
                /* pass */
                long long nlen = _tr_strlen(mn);
                /* pass */
                long long si2 = 0LL;
                /* pass */
                bool found_s = false;
                /* pass */
                long long sep_p = 0LL;
                /* pass */
                while (((si2 <= (nlen - 7LL)) && (!found_s))) {
                    /* pass */
                    if ((strcmp((char*)_tr_str_slice(mn, si2, (si2 + 7LL)), (char*)"__MONO_") == 0)) {
                        /* pass */
                        found_s = true;
                        /* pass */
                        sep_p = si2;
                    }
                    /* pass */
                    si2 = (si2 + 1LL);
                }
                /* pass */
                if (found_s) {
                    /* pass */
                    __auto_type gfunc_n = _tr_str_slice(mn, 0LL, sep_p);
                    /* pass */
                    __auto_type gfunc_t = _tr_str_slice(mn, (sep_p + 7LL), nlen);
                    /* pass */
                    if (_tr_dict_contains(self->functions, gfunc_n)) {
                        /* pass */
                        CGenerator_ensure_mono_func(self, gfunc_n, gfunc_t);
                    }
                }
            }
        } else if (1) {
            __auto_type _ = _t257;
            /* pass */
        }
    } else if (_t256.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t256.data.EMethodCall.obj;
__auto_type margs = _t256.data.EMethodCall.args;
        /* pass */
        CGenerator_scan_mono_expr(self, obj);
        /* pass */
        long long ei2 = 0LL;
        /* pass */
        while ((ei2 < margs->len)) {
            /* pass */
            CGenerator_scan_mono_expr(self, ((HirExpr*)List_ptr_get(margs, ei2)));
            /* pass */
            ei2 = (ei2 + 1LL);
        }
    } else if (_t256.tag == HirExpr_EBinOp) {
        __auto_type bl = _t256.data.EBinOp.left;
__auto_type br = _t256.data.EBinOp.right;
        /* pass */
        CGenerator_scan_mono_expr(self, bl);
        /* pass */
        CGenerator_scan_mono_expr(self, br);
    } else if (_t256.tag == HirExpr_EUnaryOp) {
        __auto_type ue = _t256.data.EUnaryOp.expr;
        /* pass */
        CGenerator_scan_mono_expr(self, ue);
    } else if (1) {
        __auto_type _ = _t256;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_scan_mono_stmt(CGenerator* self, HirStmt* s_ptr) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(s_ptr)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t258 = (*s_ptr);
    if (_t258.tag == HirStmt_SLet) {
        __auto_type ty = _t258.data.SLet.ty;
__auto_type val = _t258.data.SLet.val;
        /* pass */
        CGenerator_scan_mono_ty(self, ty);
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, val);
        }
    } else if (_t258.tag == HirStmt_SExpr) {
        __auto_type expr = _t258.data.SExpr.expr;
        /* pass */
        if ((((unsigned long long)(expr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, expr);
        }
    } else if (_t258.tag == HirStmt_SReturn) {
        __auto_type val = _t258.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, val);
        }
    } else if (_t258.tag == HirStmt_SIf) {
        __auto_type cond_s = _t258.data.SIf.cond;
__auto_type then_b = _t258.data.SIf.then_b;
__auto_type else_b = _t258.data.SIf.else_b;
        /* pass */
        if ((((unsigned long long)(cond_s)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, cond_s);
        }
        /* pass */
        CGenerator_scan_mono_block(self, then_b);
        /* pass */
        if ((((unsigned long long)(else_b)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_block(self, else_b);
        }
    } else if (_t258.tag == HirStmt_SWhile) {
        __auto_type cond_s2 = _t258.data.SWhile.cond;
__auto_type body = _t258.data.SWhile.body;
        /* pass */
        if ((((unsigned long long)(cond_s2)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, cond_s2);
        }
        /* pass */
        CGenerator_scan_mono_block(self, body);
    } else if (_t258.tag == HirStmt_SFor) {
        __auto_type body = _t258.data.SFor.body;
        /* pass */
        CGenerator_scan_mono_block(self, body);
    } else if (_t258.tag == HirStmt_SForUnpack) {
        __auto_type body = _t258.data.SForUnpack.body;
        /* pass */
        CGenerator_scan_mono_block(self, body);
    } else if (_t258.tag == HirStmt_SMatch) {
        __auto_type subj_s = _t258.data.SMatch.expr;
__auto_type arms = _t258.data.SMatch.arms;
        /* pass */
        if ((((unsigned long long)(subj_s)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_expr(self, subj_s);
        }
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            CGenerator_scan_mono_block(self, ((HirMatchArm*)List_ptr_get(arms, ai))->body);
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t258.tag == HirStmt_STry) {
        __auto_type try_body = _t258.data.STry.try_body;
__auto_type catches = _t258.data.STry.catches;
__auto_type finally_b = _t258.data.STry.finally_b;
        /* pass */
        CGenerator_scan_mono_block(self, try_body);
        /* pass */
        if ((((unsigned long long)(finally_b)) != ((unsigned long long)(0LL)))) {
            /* pass */
            CGenerator_scan_mono_block(self, finally_b);
        }
    } else if (_t258.tag == HirStmt_SUnsafe) {
        __auto_type body = _t258.data.SUnsafe.body;
        /* pass */
        CGenerator_scan_mono_block(self, body);
    } else if (_t258.tag == HirStmt_STaskGroup) {
        __auto_type body = _t258.data.STaskGroup.body;
        /* pass */
        CGenerator_scan_mono_block(self, body);
    } else if (1) {
        __auto_type _ = _t258;
        /* pass */
    }
}

__attribute__((hot)) void CGenerator_scan_mono_func(CGenerator* self, HirFunction* f) {
    /* pass */
    CGenerator_scan_mono_ty(self, f->ret_ty);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        CGenerator_scan_mono_ty(self, ((HirParam*)List_ptr_get(f->params, i))->ty);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_scan_mono_block(self, f->body);
}

__attribute__((hot)) void CGenerator_scan_mono_prog(CGenerator* self, HirProgram* prog) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* cls = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((cls->generics->len == 0LL)) {
            /* pass */
            long long fi = 0LL;
            /* pass */
            while ((fi < cls->fields->len)) {
                /* pass */
                CGenerator_scan_mono_ty(self, ((HirField*)List_ptr_get(cls->fields, fi))->ty);
                /* pass */
                fi = (fi + 1LL);
            }
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < cls->methods->len)) {
                /* pass */
                CGenerator_scan_mono_func(self, ((HirFunction*)List_ptr_get(cls->methods, mi)));
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* fn = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        bool skip = false;
        /* pass */
        if ((strcmp((char*)fn->class_name, (char*)"") != 0)) {
            /* pass */
            if (_tr_dict_contains(self->classes, fn->class_name)) {
                /* pass */
                HirClass* owner = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, fn->class_name));
                /* pass */
                if ((owner->generics->len > 0LL)) {
                    /* pass */
                    skip = true;
                }
            }
        }
        /* pass */
        if ((fn->generics->len > 0LL)) {
            /* pass */
            skip = true;
        }
        /* pass */
        if ((!skip)) {
            /* pass */
            CGenerator_scan_mono_func(self, fn);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->top_level_stmts->len)) {
        /* pass */
        CGenerator_scan_mono_stmt(self, ((HirStmt*)List_ptr_get(prog->top_level_stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) char* CGenerator_generate(CGenerator* self, HirProgram* prog) {
    /* pass */
    CGenerator_register_program(self, prog);
    /* pass */
    CGenerator_w(self, "#define _TR_MAIN\n");
    /* pass */
    CGenerator_w(self, "#include \"tauraro_rt.h\"\n");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* cls2 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((cls2->generics->len == 0LL)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct ", cls2->name), " "), cls2->name), ";\n"));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct ", ((HirEnum*)List_ptr_get(prog->enums, i))->name), " "), ((HirEnum*)List_ptr_get(prog->enums, i))->name), ";\n"));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->interfaces->len)) {
        /* pass */
        HirInterface* iface_fwd = ((HirInterface*)List_ptr_get(prog->interfaces, i));
        /* pass */
        if ((iface_fwd->generics->len == 0LL)) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct _", iface_fwd->name), "_vtable "), iface_fwd->name), "_vtable;\n"));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_emit_list_fwd_decls(self, prog);
    /* pass */
    CGenerator_w(self, "\n");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->extern_funcs->len)) {
        /* pass */
        HirFunction* ef = ((HirFunction*)List_ptr_get(prog->extern_funcs, i));
        /* pass */
        char* ef_ret = CGenerator_type_to_c(self, ef->ret_ty);
        /* pass */
        char* ef_name = ef->name;
        /* pass */
        if ((!_starts_with_tr(ef_name))) {
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("extern ", ef_ret), " "), ef_name), "("));
            /* pass */
            bool ef_first = true;
            /* pass */
            long long epi = 0LL;
            /* pass */
            while ((epi < ef->params->len)) {
                /* pass */
                HirParam* ep = ((HirParam*)List_ptr_get(ef->params, epi));
                /* pass */
                if ((strcmp((char*)ep->name, (char*)"self") != 0)) {
                    /* pass */
                    if ((!ef_first)) {
                        /* pass */
                        CGenerator_w(self, ", ");
                    }
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_type_to_c(self, ep->ty), " "), ep->name));
                    /* pass */
                    ef_first = false;
                }
                /* pass */
                epi = (epi + 1LL);
            }
            /* pass */
            if (ef->is_variadic) {
                /* pass */
                if ((!ef_first)) {
                    /* pass */
                    CGenerator_w(self, ", ");
                }
                /* pass */
                CGenerator_w(self, "...");
            }
            /* pass */
            CGenerator_w(self, ");\n");
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((prog->extern_funcs->len > 0LL)) {
        /* pass */
        CGenerator_w(self, "\n");
    }
    /* pass */
    CGenerator_scan_mono_prog(self, prog);
    /* pass */
    char* mono_out = StringObj_as_str(StringBuilder_to_string(self->mono_buf));
    /* pass */
    if ((strcmp((char*)mono_out, (char*)"") != 0)) {
        /* pass */
        CGenerator_w(self, mono_out);
        /* pass */
        CGenerator_w(self, "\n");
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        CGenerator_gen_enum_struct(self, ((HirEnum*)List_ptr_get(prog->enums, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* cls3 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((cls3->generics->len == 0LL)) {
            /* pass */
            CGenerator_gen_class_struct(self, cls3);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->interfaces->len)) {
        /* pass */
        CGenerator_gen_interface_vtable(self, ((HirInterface*)List_ptr_get(prog->interfaces, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_w(self, StringObj_as_str(StringBuilder_to_string(self->struct_buf)));
    /* pass */
    self->struct_buf = StringBuilder_init(16LL);
    /* pass */
    char* lt_out = StringObj_as_str(StringBuilder_to_string(self->list_types_buf));
    /* pass */
    if ((strcmp((char*)lt_out, (char*)"") != 0)) {
        /* pass */
        CGenerator_w(self, lt_out);
    }
    /* pass */
    long long gvi = 0LL;
    /* pass */
    while ((gvi < prog->top_level_stmts->len)) {
        /* pass */
        __auto_type _t259 = (*((HirStmt*)List_ptr_get(prog->top_level_stmts, gvi)));
        if (_t259.tag == HirStmt_SLet) {
            __auto_type gvn = _t259.data.SLet.name;
__auto_type gvty = _t259.data.SLet.ty;
__auto_type gvval = _t259.data.SLet.val;
            /* pass */
            char* gv_cty = CGenerator_type_to_c(self, gvty);
            /* pass */
            if (((strcmp((char*)gv_cty, (char*)"void") == 0) || (strcmp((char*)gv_cty, (char*)"__auto_type") == 0))) {
                /* pass */
                if ((((unsigned long long)(gvval)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    gv_cty = CGenerator_type_to_c(self, hir_expr_type(gvval));
                }
            }
            /* pass */
            if (((strcmp((char*)gv_cty, (char*)"void") == 0) || (strcmp((char*)gv_cty, (char*)"__auto_type") == 0))) {
                /* pass */
                gv_cty = "long long";
            }
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(gv_cty, " "), gvn), ";\n"));
            /* pass */
            _tr_dict_set(self->global_vars, gvn, true);
        } else if (1) {
            __auto_type _ = _t259;
            /* pass */
        }
        /* pass */
        gvi = (gvi + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((c->generics->len == 0LL)) {
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, c->name)), ";\n"));
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < e->methods->len)) {
            /* pass */
            HirFunction* m = ((HirFunction*)List_ptr_get(e->methods, mi));
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, e->name)), ";\n"));
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp((char*)f->name, (char*)"main") != 0) && (!f->is_extern))) {
            /* pass */
            bool skip4 = false;
            /* pass */
            if ((strcmp((char*)f->class_name, (char*)"") != 0)) {
                /* pass */
                skip4 = true;
            }
            /* pass */
            if ((!skip4)) {
                /* pass */
                if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("p_", f->name)))) {
                    /* pass */
                    _tr_dict_set(self->emitted_fns, _tr_str_concat("p_", f->name), true);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, f), CGenerator_gen_func_sig(self, f, f->class_name)), ";\n"));
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_w(self, "\n");
    /* pass */
    CGenerator_prescan_spawns(self, prog);
    /* pass */
    CGenerator_prescan_awaits(self, prog);
    /* pass */
    CGenerator_w(self, "\n");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((c->generics->len == 0LL)) {
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                /* pass */
                self->cur_class = c->name;
                /* pass */
                self->cur_func = m->name;
                /* pass */
                self->cur_throws_ty = m->throws_ty->name;
                /* pass */
                self->decl_vars = _tr_dict_new(64LL);
                /* pass */
                self->shared_vars = _tr_dict_new(8LL);
                /* pass */
                CGenerator_seed_params(self, m);
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, c->name)), " {\n"));
                /* pass */
                CGenerator_gen_func_body(self, m->body, 1LL);
                /* pass */
                CGenerator_w(self, "}\n\n");
                /* pass */
                self->cur_class = "";
                /* pass */
                self->cur_func = "";
                /* pass */
                self->cur_throws_ty = "";
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < e->methods->len)) {
            /* pass */
            HirFunction* m = ((HirFunction*)List_ptr_get(e->methods, mi));
            /* pass */
            self->cur_class = e->name;
            /* pass */
            self->cur_func = m->name;
            /* pass */
            self->cur_throws_ty = m->throws_ty->name;
            /* pass */
            self->decl_vars = _tr_dict_new(64LL);
            /* pass */
            self->shared_vars = _tr_dict_new(8LL);
            /* pass */
            CGenerator_seed_params(self, m);
            /* pass */
            CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, m), CGenerator_gen_func_sig(self, m, e->name)), " {\n"));
            /* pass */
            CGenerator_gen_func_body(self, m->body, 1LL);
            /* pass */
            CGenerator_w(self, "}\n\n");
            /* pass */
            self->cur_class = "";
            /* pass */
            self->cur_func = "";
            /* pass */
            self->cur_throws_ty = "";
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp((char*)f->name, (char*)"main") != 0) && (!f->is_extern))) {
            /* pass */
            bool skip6 = false;
            /* pass */
            if ((strcmp((char*)f->class_name, (char*)"") != 0)) {
                /* pass */
                skip6 = true;
            }
            /* pass */
            if ((!skip6)) {
                /* pass */
                if (_tr_dict_contains(self->emitted_fns, _tr_str_concat("i_", f->name))) {
                    /* pass */
                    skip6 = true;
                } else {
                    /* pass */
                    _tr_dict_set(self->emitted_fns, _tr_str_concat("i_", f->name), true);
                }
            }
            /* pass */
            if ((!skip6)) {
                /* pass */
                self->cur_func = f->name;
                /* pass */
                self->cur_class = f->class_name;
                /* pass */
                self->cur_throws_ty = f->throws_ty->name;
                /* pass */
                self->decl_vars = _tr_dict_new(64LL);
                /* pass */
                self->shared_vars = _tr_dict_new(8LL);
                /* pass */
                CGenerator_seed_params(self, f);
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_inline_attrs(self, f), CGenerator_gen_func_sig(self, f, f->class_name)), " {\n"));
                /* pass */
                CGenerator_gen_func_body(self, f->body, 1LL);
                /* pass */
                CGenerator_w(self, "}\n\n");
                /* pass */
                self->cur_func = "";
                /* pass */
                self->cur_class = "";
                /* pass */
                self->cur_throws_ty = "";
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_w(self, "__attribute__((hot)) int main(int argc, char** argv) {\n");
    /* pass */
    CGenerator_w(self, "    _tr_argc = argc; _tr_argv = argv;\n");
    /* pass */
    CGenerator_w(self, "    _tr_init_console();\n");
    /* pass */
    CGenerator_w(self, "#ifndef TAURARO_BARE\n");
    /* pass */
    CGenerator_w(self, "    _tr_global_async_pool = _tr_threadpool_auto();\n");
    /* pass */
    CGenerator_w(self, "#endif\n");
    /* pass */
    bool main_has_args = false;
    /* pass */
    char* main_args_name = "args";
    /* pass */
    long long mi2 = 0LL;
    /* pass */
    while ((mi2 < prog->functions->len)) {
        /* pass */
        HirFunction* mf = ((HirFunction*)List_ptr_get(prog->functions, mi2));
        /* pass */
        if (((strcmp((char*)mf->name, (char*)"main") == 0) && (mf->params->len > 0LL))) {
            /* pass */
            main_has_args = true;
            /* pass */
            main_args_name = ((HirParam*)List_ptr_get(mf->params, 0LL))->name;
        }
        /* pass */
        mi2 = (mi2 + 1LL);
    }
    /* pass */
    if (main_has_args) {
        /* pass */
        if ((strcmp((char*)main_args_name, (char*)"argv") == 0)) {
            /* pass */
            main_args_name = "_tr_main_argv";
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    List_str* ", main_args_name), " = List_str_new();\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    for (int _ai = 0; _ai < argc; _ai++) { List_str_append(", main_args_name), ", argv[_ai]); }\n"));
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->top_level_stmts->len)) {
        /* pass */
        bool _tl_global = false;
        /* pass */
        __auto_type _t260 = (*((HirStmt*)List_ptr_get(prog->top_level_stmts, i)));
        if (_t260.tag == HirStmt_SLet) {
            __auto_type tln = _t260.data.SLet.name;
__auto_type tlv = _t260.data.SLet.val;
            /* pass */
            if ((_tr_dict_contains(self->global_vars, tln) && (((unsigned long long)(tlv)) != ((unsigned long long)(0LL))))) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", tln), " = "), CGenerator_gen_expr(self, tlv)), ";\n"));
                /* pass */
                _tl_global = true;
            }
        } else if (1) {
            __auto_type _ = _t260;
            /* pass */
        }
        /* pass */
        if ((!_tl_global)) {
            /* pass */
            CGenerator_gen_stmt(self, ((HirStmt*)List_ptr_get(prog->top_level_stmts, i)), 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((strcmp((char*)f->name, (char*)"main") == 0)) {
            /* pass */
            self->cur_func = "main";
            /* pass */
            self->decl_vars = _tr_dict_new(64LL);
            /* pass */
            CGenerator_gen_func_body(self, f->body, 1LL);
            /* pass */
            self->cur_func = "";
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_w(self, "#ifndef TAURARO_BARE\n");
    /* pass */
    CGenerator_w(self, "    if (_tr_global_async_pool) { _tr_threadpool_free(_tr_global_async_pool); _tr_global_async_pool = NULL; }\n");
    /* pass */
    CGenerator_w(self, "#endif\n");
    /* pass */
    CGenerator_w(self, "    return 0;\n}\n");
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->buf));
}

__attribute__((hot)) char* CGenerator_generate_types_header(CGenerator* self, HirProgram* prog) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog)))) {
        /* pass */
        return "";
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->classes)))) {
        /* pass */
        return "";
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->enums)))) {
        /* pass */
        return "";
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->interfaces)))) {
        /* pass */
        return "";
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(prog->functions)))) {
        /* pass */
        return "";
    }
    /* pass */
    if ((self->classes->len == 0LL)) {
        /* pass */
        CGenerator_register_program(self, prog);
        /* pass */
        CGenerator_scan_mono_prog(self, prog);
    }
    /* pass */
    StringBuilder* old_buf = self->buf;
    /* pass */
    StringBuilder* old_struct = self->struct_buf;
    /* pass */
    StringBuilder* old_list = self->list_types_buf;
    /* pass */
    self->buf = StringBuilder_init(4096LL);
    /* pass */
    self->struct_buf = StringBuilder_init(16384LL);
    /* pass */
    self->list_types_buf = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder* out = StringBuilder_init(65536LL);
    /* pass */
    StringBuilder_append(out, "#pragma once\n");
    /* pass */
    bool has_std_lib = false;
    /* pass */
    long long sbi = 0LL;
    /* pass */
    while ((sbi < prog->classes->len)) {
        /* pass */
        HirClass* sbc = ((HirClass*)List_ptr_get(prog->classes, sbi));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(sbc))))) {
            /* pass */
            if ((strcmp((char*)sbc->name, (char*)"StringBuilder") == 0)) {
                /* pass */
                has_std_lib = true;
                /* pass */
                break;
            }
        }
        /* pass */
        sbi = (sbi + 1LL);
    }
    /* pass */
    if ((!has_std_lib)) {
        /* pass */
        long long fi2 = 0LL;
        /* pass */
        while ((fi2 < prog->functions->len)) {
            /* pass */
            HirFunction* fc = ((HirFunction*)List_ptr_get(prog->functions, fi2));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(fc))))) {
                /* pass */
                if (((strcmp((char*)fc->name, (char*)"read_file") == 0) || (strcmp((char*)fc->name, (char*)"write_file") == 0))) {
                    /* pass */
                    has_std_lib = true;
                    /* pass */
                    break;
                }
            }
            /* pass */
            fi2 = (fi2 + 1LL);
        }
    }
    /* pass */
    if (has_std_lib) {
        /* pass */
        StringBuilder_append(out, "#define TAURARO_STD_LIB\n");
        /* pass */
        StringBuilder_append(out, "#define TAURARO_RT_NO_STRINGBUILDER\n");
    }
    /* pass */
    StringBuilder_append(out, "#include \"tauraro_rt.h\"\n\n");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c2 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(c2))))) {
            /* pass */
            if ((c2->generics->len == 0LL)) {
                /* pass */
                char* fwd_key = _tr_str_concat("fwd_", c2->name);
                /* pass */
                if ((!_tr_dict_contains(self->emitted_fns, fwd_key))) {
                    /* pass */
                    _tr_dict_set(self->emitted_fns, fwd_key, true);
                    /* pass */
                    StringBuilder_append(out, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct ", c2->name), " "), c2->name), ";\n"));
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e2 = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(e2))))) {
            /* pass */
            char* fwd_key2 = _tr_str_concat("fwd_", e2->name);
            /* pass */
            if ((!_tr_dict_contains(self->emitted_fns, fwd_key2))) {
                /* pass */
                _tr_dict_set(self->emitted_fns, fwd_key2, true);
                /* pass */
                StringBuilder_append(out, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct ", e2->name), " "), e2->name), ";\n"));
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->interfaces->len)) {
        /* pass */
        HirInterface* iface2 = ((HirInterface*)List_ptr_get(prog->interfaces, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(iface2))))) {
            /* pass */
            if ((iface2->generics->len == 0LL)) {
                /* pass */
                StringBuilder_append(out, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct _", iface2->name), "_vtable "), iface2->name), "_vtable;\n"));
                /* pass */
                StringBuilder_append(out, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef struct { ", iface2->name), "_vtable* vtable; void* data; } "), iface2->name), "_obj;\n"));
                /* pass */
                _tr_dict_set(self->emitted_fns, _tr_str_concat("iface_obj_", iface2->name), true);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    CGenerator_emit_list_fwd_decls(self, prog);
    /* pass */
    StringBuilder_append(out, StringObj_as_str(StringBuilder_to_string(self->buf)));
    /* pass */
    self->buf = StringBuilder_init(256LL);
    /* pass */
    StringBuilder_append(out, "\n");
    /* pass */
    char* mono_early = StringObj_as_str(StringBuilder_to_string(self->mono_buf));
    /* pass */
    if ((strcmp((char*)mono_early, (char*)"") != 0)) {
        /* pass */
        StringBuilder_append(out, _tr_str_concat(mono_early, "\n"));
    }
    /* pass */
    if ((!_is_invalid_ptr(((unsigned long long)(prog->extern_funcs))))) {
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < prog->extern_funcs->len)) {
            /* pass */
            HirFunction* ef = ((HirFunction*)List_ptr_get(prog->extern_funcs, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(ef))))) {
                /* pass */
                if ((!_starts_with_tr(ef->name))) {
                    /* pass */
                    char* ef_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("extern ", CGenerator_type_to_c(self, ef->ret_ty)), " "), ef->name), "(");
                    /* pass */
                    bool ef_first = true;
                    /* pass */
                    long long epi = 0LL;
                    /* pass */
                    while ((epi < ef->params->len)) {
                        /* pass */
                        HirParam* ep = ((HirParam*)List_ptr_get(ef->params, epi));
                        /* pass */
                        if ((strcmp((char*)ep->name, (char*)"self") != 0)) {
                            /* pass */
                            if ((!ef_first)) {
                                /* pass */
                                ef_s = _tr_str_concat(ef_s, ", ");
                            }
                            /* pass */
                            ef_s = _tr_str_concat(_tr_str_concat(_tr_str_concat(ef_s, CGenerator_type_to_c(self, ep->ty)), " "), ep->name);
                            /* pass */
                            ef_first = false;
                        }
                        /* pass */
                        epi = (epi + 1LL);
                    }
                    /* pass */
                    if (ef->is_variadic) {
                        /* pass */
                        if ((!ef_first)) {
                            /* pass */
                            ef_s = _tr_str_concat(ef_s, ", ");
                        }
                        /* pass */
                        ef_s = _tr_str_concat(ef_s, "...");
                    }
                    /* pass */
                    StringBuilder_append(out, _tr_str_concat(ef_s, ");\n"));
                }
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if ((prog->extern_funcs->len > 0LL)) {
            /* pass */
            StringBuilder_append(out, "\n");
        }
    }
    /* pass */
    if ((!_is_invalid_ptr(((unsigned long long)(prog->type_alias_names))))) {
        /* pass */
        long long tai = 0LL;
        /* pass */
        while ((tai < prog->type_alias_names->len)) {
            /* pass */
            char* ta_name = List_str_get(prog->type_alias_names, tai);
            /* pass */
            AstType** ta_ty_p = ((AstType**)List_ptr_get(prog->type_alias_types, tai));
            /* pass */
            if ((((unsigned long long)(ta_ty_p)) != ((unsigned long long)(0LL)))) {
                /* pass */
                char* ta_c = CGenerator_type_to_c(self, (*ta_ty_p));
                /* pass */
                StringBuilder_append(out, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("typedef ", ta_c), " "), ta_name), ";\n"));
            }
            /* pass */
            tai = (tai + 1LL);
        }
        /* pass */
        if ((prog->type_alias_names->len > 0LL)) {
            /* pass */
            StringBuilder_append(out, "\n");
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e3 = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(e3))))) {
            /* pass */
            CGenerator_gen_enum_struct(self, e3);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c3 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(c3))))) {
            /* pass */
            if ((c3->generics->len == 0LL)) {
                /* pass */
                CGenerator_gen_class_struct(self, c3);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->interfaces->len)) {
        /* pass */
        HirInterface* iface3 = ((HirInterface*)List_ptr_get(prog->interfaces, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(iface3))))) {
            /* pass */
            CGenerator_gen_interface_vtable(self, iface3);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder_append(out, StringObj_as_str(StringBuilder_to_string(self->struct_buf)));
    /* pass */
    StringBuilder_append(out, StringObj_as_str(StringBuilder_to_string(self->list_types_buf)));
    /* pass */
    StringBuilder_append(out, "\n");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f4 = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(f4))))) {
            /* pass */
            if (((strcmp((char*)f4->name, (char*)"main") != 0) && (!f4->is_extern))) {
                /* pass */
                if ((strcmp((char*)f4->class_name, (char*)"") != 0)) {
                    /* pass */
                    bool skip4_generic = false;
                    /* pass */
                    if (_tr_dict_contains(self->classes, f4->class_name)) {
                        /* pass */
                        HirClass* cls4g = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, f4->class_name));
                        /* pass */
                        if ((cls4g->generics->len > 0LL)) {
                            /* pass */
                            skip4_generic = true;
                        }
                    }
                    /* pass */
                    if ((!skip4_generic)) {
                        /* pass */
                        char* ep_key = _tr_str_concat(_tr_str_concat(_tr_str_concat("p_", f4->class_name), "_"), f4->name);
                        /* pass */
                        if ((!_tr_dict_contains(self->emitted_fns, ep_key))) {
                            /* pass */
                            _tr_dict_set(self->emitted_fns, ep_key, true);
                            /* pass */
                            StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f4), CGenerator_gen_func_sig(self, f4, f4->class_name)), ";\n"));
                        }
                    }
                } else {
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("p_", f4->name)))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, _tr_str_concat("p_", f4->name), true);
                        /* pass */
                        StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f4), CGenerator_gen_func_sig(self, f4, f4->class_name)), ";\n"));
                    }
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder_append(out, "\n");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c4 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(c4))))) {
            /* pass */
            if ((c4->generics->len == 0LL)) {
                /* pass */
                long long mi = 0LL;
                /* pass */
                if ((!_is_invalid_ptr(((unsigned long long)(c4->methods))))) {
                    /* pass */
                    while ((mi < c4->methods->len)) {
                        /* pass */
                        HirFunction* m = ((HirFunction*)List_ptr_get(c4->methods, mi));
                        /* pass */
                        if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                            /* pass */
                            char* ep_key3 = _tr_str_concat(_tr_str_concat(_tr_str_concat("p_", c4->name), "_"), m->name);
                            /* pass */
                            if ((!_tr_dict_contains(self->emitted_fns, ep_key3))) {
                                /* pass */
                                _tr_dict_set(self->emitted_fns, ep_key3, true);
                                /* pass */
                                StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, c4->name)), ";\n"));
                            }
                        }
                        /* pass */
                        mi = (mi + 1LL);
                    }
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e4 = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(e4))))) {
            /* pass */
            long long mi = 0LL;
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(e4->methods))))) {
                /* pass */
                while ((mi < e4->methods->len)) {
                    /* pass */
                    HirFunction* m = ((HirFunction*)List_ptr_get(e4->methods, mi));
                    /* pass */
                    if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                        /* pass */
                        StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, e4->name)), ";\n"));
                    }
                    /* pass */
                    mi = (mi + 1LL);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f4 = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(f4))))) {
            /* pass */
            if (((strcmp((char*)f4->name, (char*)"main") != 0) && (!f4->is_extern))) {
                /* pass */
                if ((strcmp((char*)f4->class_name, (char*)"") != 0)) {
                    /* pass */
                    bool skip4b_generic = false;
                    /* pass */
                    if (_tr_dict_contains(self->classes, f4->class_name)) {
                        /* pass */
                        HirClass* cls4bg = ((HirClass*)(uintptr_t)_tr_dict_get(self->classes, f4->class_name));
                        /* pass */
                        if ((cls4bg->generics->len > 0LL)) {
                            /* pass */
                            skip4b_generic = true;
                        }
                    }
                    /* pass */
                    if ((!skip4b_generic)) {
                        /* pass */
                        char* ep_key2 = _tr_str_concat(_tr_str_concat(_tr_str_concat("p_", f4->class_name), "_"), f4->name);
                        /* pass */
                        if ((!_tr_dict_contains(self->emitted_fns, ep_key2))) {
                            /* pass */
                            _tr_dict_set(self->emitted_fns, ep_key2, true);
                            /* pass */
                            StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f4), CGenerator_gen_func_sig(self, f4, f4->class_name)), ";\n"));
                        }
                    }
                } else {
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("p_", f4->name)))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, _tr_str_concat("p_", f4->name), true);
                        /* pass */
                        StringBuilder_append(out, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f4), CGenerator_gen_func_sig(self, f4, f4->class_name)), ";\n"));
                    }
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder_append(out, "\n");
    /* pass */
    self->buf = StringBuilder_init(1024LL);
    /* pass */
    CGenerator_prescan_spawns(self, prog);
    /* pass */
    CGenerator_prescan_awaits(self, prog);
    /* pass */
    char* sw_out = StringObj_as_str(StringBuilder_to_string(self->buf));
    /* pass */
    self->buf = StringBuilder_init(256LL);
    /* pass */
    if ((strcmp((char*)sw_out, (char*)"") != 0)) {
        /* pass */
        StringBuilder_append(out, sw_out);
        /* pass */
        StringBuilder_append(out, "\n");
    }
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < prog->classes->len)) {
        /* pass */
        HirClass* wcls = ((HirClass*)List_ptr_get(prog->classes, wi));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(wcls))))) {
            /* pass */
            if ((((wcls->generics->len == 0LL) && (((unsigned long long)(wcls->iface_names)) != ((unsigned long long)(0LL)))) && (!_is_invalid_ptr(((unsigned long long)(wcls->iface_names)))))) {
                /* pass */
                long long wii = 0LL;
                /* pass */
                while ((wii < wcls->iface_names->len)) {
                    /* pass */
                    char* wiface_name = List_str_get(wcls->iface_names, wii);
                    /* pass */
                    if (_tr_dict_contains(self->interfaces, wiface_name)) {
                        /* pass */
                        HirInterface* wiface = ((HirInterface*)(uintptr_t)_tr_dict_get(self->interfaces, wiface_name));
                        /* pass */
                        if ((!_is_invalid_ptr(((unsigned long long)(wiface))))) {
                            /* pass */
                            StringBuilder_append(out, CGenerator_gen_one_iface_wrap(self, wcls->name, wiface));
                            /* pass */
                            StringBuilder_append(out, "\n");
                        }
                    }
                    /* pass */
                    wii = (wii + 1LL);
                }
            }
        }
        /* pass */
        wi = (wi + 1LL);
    }
    /* pass */
    self->buf = old_buf;
    /* pass */
    self->struct_buf = old_struct;
    /* pass */
    self->list_types_buf = old_list;
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(out));
}

__attribute__((hot)) char* CGenerator_generate_module_compat(CGenerator* self, List_str* all_decl_modules, List_ptr* all_decls) {
    /* pass */
    char* out = "\n/* === Module-prefixed typedef aliases (auto-generated) === */\n";
    /* pass */
    out = _tr_str_concat(out, "/* Maps module-qualified C names to short-name types in tauraro_types.h */\n\n");
    /* pass */
    out = _tr_str_concat(out, "#ifndef TAURARO_RT_NO_STRINGBUILDER\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_string_StringObj core_string_StringObj;\n");
    /* pass */
    out = _tr_str_concat(out, "typedef core_string_StringObj StringObj;\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_string_StringBuilder core_string_StringBuilder;\n");
    /* pass */
    out = _tr_str_concat(out, "typedef core_string_StringBuilder StringBuilder;\n");
    /* pass */
    out = _tr_str_concat(out, "#endif\n\n");
    /* pass */
    TrMap* seen = _tr_dict_new(128LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < all_decl_modules->len)) {
        /* pass */
        char* mod_dp = List_str_get(all_decl_modules, i);
        /* pass */
        if ((strcmp((char*)mod_dp, (char*)"") != 0)) {
            /* pass */
            char* sm = _c_dot_to_safe(mod_dp);
            /* pass */
            __auto_type _t261 = (*((Decl*)List_ptr_get(all_decls, i)));
            if (_t261.tag == Decl_DClass) {
                __auto_type c = _t261.data.DClass.cls;
                /* pass */
                if ((c->generics->len == 0LL)) {
                    /* pass */
                    char* key = _tr_str_concat(_tr_str_concat(sm, "_"), c->name);
                    /* pass */
                    if ((!_tr_dict_contains(seen, key))) {
                        /* pass */
                        _tr_dict_set(seen, key, true);
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef "), c->name), " "), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_vec_Vec_"), key), " { "), key), "** data; long long len; long long capacity; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_vec_Vec_"), key), " core_vec_Vec_"), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_vec_Vec_"), key), "_ptr { "), key), "*** data; long long len; long long capacity; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_vec_Vec_"), key), "_ptr core_vec_Vec_"), key), "_ptr;\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_map_MapNode_str_"), key), " { char* key; "), key), "* value; struct core_map_MapNode_str_"), key), "* next; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_map_MapNode_str_"), key), " core_map_MapNode_str_"), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_map_Map_str_"), key), " { core_map_MapNode_str_"), key), "** buckets; long long capacity; long long len; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_map_Map_str_"), key), " core_map_Map_str_"), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "** core_alloc_alloc_"), key), "(long long count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "** core_alloc_resize_"), key), "("), key), "** ptr, long long new_count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_"), key), "("), key), "** ptr);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "*** core_alloc_alloc_"), key), "_ptr(long long count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "*** core_alloc_resize_"), key), "_ptr("), key), "*** ptr, long long new_count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_"), key), "_ptr("), key), "*** ptr);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) core_map_MapNode_str_"), key), "** core_alloc_alloc_core_map_MapNode_str_"), key), "(long long count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_"), key), "(core_map_MapNode_str_"), key), "** ptr);\n");
                        /* pass */
                        out = _tr_str_concat(out, "\n");
                    }
                }
            } else if (_t261.tag == Decl_DEnum) {
                __auto_type e = _t261.data.DEnum.enm;
                /* pass */
                if ((e->generics->len == 0LL)) {
                    /* pass */
                    char* key = _tr_str_concat(_tr_str_concat(sm, "_"), e->name);
                    /* pass */
                    if ((!_tr_dict_contains(seen, key))) {
                        /* pass */
                        _tr_dict_set(seen, key, true);
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef "), e->name), " "), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_vec_Vec_"), key), " { "), key), "* data; long long len; long long capacity; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_vec_Vec_"), key), " core_vec_Vec_"), key), ";\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "struct core_vec_Vec_"), key), "_ptr { "), key), "** data; long long len; long long capacity; };\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef struct core_vec_Vec_"), key), "_ptr core_vec_Vec_"), key), "_ptr;\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "* core_alloc_alloc_"), key), "(long long count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "* core_alloc_resize_"), key), "("), key), "* ptr, long long new_count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_"), key), "("), key), "* ptr);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "** core_alloc_alloc_"), key), "_ptr(long long count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) "), key), "** core_alloc_resize_"), key), "_ptr("), key), "** ptr, long long new_count);\n");
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_"), key), "_ptr("), key), "** ptr);\n");
                        /* pass */
                        out = _tr_str_concat(out, "\n");
                    }
                }
            } else if (_t261.tag == Decl_DInterface) {
                __auto_type iface = _t261.data.DInterface.iface;
                /* pass */
                if ((iface->generics->len == 0LL)) {
                    /* pass */
                    char* key = _tr_str_concat(_tr_str_concat(sm, "_"), iface->name);
                    /* pass */
                    if ((!_tr_dict_contains(seen, key))) {
                        /* pass */
                        _tr_dict_set(seen, key, true);
                        /* pass */
                        out = _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(out, "typedef "), iface->name), "_obj "), key), ";\n\n");
                    }
                }
            } else if (1) {
                __auto_type _ = _t261;
                /* pass */
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    out = _tr_str_concat(out, "/* Primitive vec/map types for core modules */\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_vec_Vec_str { char** data; long long len; long long capacity; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_vec_Vec_str core_vec_Vec_str;\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) char** core_alloc_alloc_str(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) char** core_alloc_resize_str(char** ptr, long long new_count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_str(char** ptr);\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_vec_Vec_i64 { long long* data; long long len; long long capacity; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_vec_Vec_i64 core_vec_Vec_i64;\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) long long* core_alloc_alloc_i64(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) long long* core_alloc_resize_i64(long long* ptr, long long new_count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_i64(long long* ptr);\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_MapNode_str_bool { char* key; bool value; struct core_map_MapNode_str_bool* next; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_MapNode_str_bool core_map_MapNode_str_bool;\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_Map_str_bool { core_map_MapNode_str_bool** buckets; long long capacity; long long len; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_Map_str_bool core_map_Map_str_bool;\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) core_map_MapNode_str_bool** core_alloc_alloc_core_map_MapNode_str_bool(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bool(core_map_MapNode_str_bool** ptr);\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_MapNode_str_str { char* key; char* value; struct core_map_MapNode_str_str* next; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_MapNode_str_str core_map_MapNode_str_str;\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_Map_str_str { core_map_MapNode_str_str** buckets; long long capacity; long long len; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_Map_str_str core_map_Map_str_str;\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) core_map_MapNode_str_str** core_alloc_alloc_core_map_MapNode_str_str(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_str(core_map_MapNode_str_str** ptr);\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_MapNode_str_i64 { char* key; long long value; struct core_map_MapNode_str_i64* next; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_MapNode_str_i64 core_map_MapNode_str_i64;\n");
    /* pass */
    out = _tr_str_concat(out, "struct core_map_Map_str_i64 { core_map_MapNode_str_i64** buckets; long long capacity; long long len; };\n");
    /* pass */
    out = _tr_str_concat(out, "typedef struct core_map_Map_str_i64 core_map_Map_str_i64;\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) core_map_MapNode_str_i64** core_alloc_alloc_core_map_MapNode_str_i64(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_i64(core_map_MapNode_str_i64** ptr);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) char* core_alloc_alloc_char(long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_copy_char(char* dst, char* src, long long count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) char* core_alloc_resize_char(char* ptr, long long new_count);\n");
    /* pass */
    out = _tr_str_concat(out, "__attribute__((hot)) void core_alloc_dealloc_char(char* ptr);\n");
    /* pass */
    return out;
}

__attribute__((hot)) char* CGenerator_generate_module_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set, long long depth) {
    /* pass */
    self->buf = StringBuilder_init(65536LL);
    /* pass */
    char* inc_prefix = "";
    /* pass */
    long long d = 0LL;
    /* pass */
    while ((d < depth)) {
        /* pass */
        inc_prefix = _tr_str_concat(inc_prefix, "../");
        /* pass */
        d = (d + 1LL);
    }
    /* pass */
    CGenerator_w(self, _tr_str_concat(_tr_str_concat("#include \"", inc_prefix), "tauraro_types.h\"\n\n"));
    /* pass */
    long long fwd_i = 0LL;
    /* pass */
    while ((fwd_i < prog->functions->len)) {
        /* pass */
        HirFunction* fwd_f = ((HirFunction*)List_ptr_get(prog->functions, fwd_i));
        /* pass */
        if ((((strcmp((char*)fwd_f->name, (char*)"main") != 0) && (!fwd_f->is_extern)) && (!fwd_f->is_public))) {
            /* pass */
            bool in_this_mod = false;
            /* pass */
            if (((strcmp((char*)fwd_f->class_name, (char*)"") == 0) && _tr_dict_contains(fn_set, fwd_f->name))) {
                /* pass */
                in_this_mod = true;
            } else if (((strcmp((char*)fwd_f->class_name, (char*)"") != 0) && _tr_dict_contains(class_set, fwd_f->class_name))) {
                /* pass */
                if (_tr_dict_contains(self->classes, fwd_f->class_name)) {
                    /* pass */
                    if ((((HirClass*)(uintptr_t)_tr_dict_get(self->classes, fwd_f->class_name))->generics->len == 0LL)) {
                        /* pass */
                        in_this_mod = true;
                    }
                }
            }
            /* pass */
            if (in_this_mod) {
                /* pass */
                CGenerator_w(self, _tr_str_concat(CGenerator_gen_func_sig(self, fwd_f, fwd_f->class_name), ";\n"));
            }
        }
        /* pass */
        fwd_i = (fwd_i + 1LL);
    }
    /* pass */
    CGenerator_w(self, "\n");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if (((c->generics->len == 0LL) && _tr_dict_contains(class_set, c->name))) {
            /* pass */
            if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("cls_", c->name)))) {
                /* pass */
                _tr_dict_set(self->emitted_fns, _tr_str_concat("cls_", c->name), true);
                /* pass */
                long long mi = 0LL;
                /* pass */
                while ((mi < c->methods->len)) {
                    /* pass */
                    HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                    /* pass */
                    _tr_dict_set(self->emitted_fns, _tr_str_concat(_tr_str_concat(_tr_str_concat("i_", c->name), "_"), m->name), true);
                    /* pass */
                    self->cur_class = c->name;
                    /* pass */
                    self->cur_func = m->name;
                    /* pass */
                    self->decl_vars = _tr_dict_new(64LL);
                    /* pass */
                    CGenerator_seed_params(self, m);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, c->name)), " {\n"));
                    /* pass */
                    CGenerator_gen_func_body(self, m->body, 1LL);
                    /* pass */
                    CGenerator_w(self, "}\n\n");
                    /* pass */
                    self->cur_class = "";
                    /* pass */
                    self->cur_func = "";
                    /* pass */
                    mi = (mi + 1LL);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        if (_tr_dict_contains(class_set, e->name)) {
            /* pass */
            if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("cls_", e->name)))) {
                /* pass */
                _tr_dict_set(self->emitted_fns, _tr_str_concat("cls_", e->name), true);
                /* pass */
                long long mi = 0LL;
                /* pass */
                while ((mi < e->methods->len)) {
                    /* pass */
                    HirFunction* m = ((HirFunction*)List_ptr_get(e->methods, mi));
                    /* pass */
                    self->cur_class = e->name;
                    /* pass */
                    self->cur_func = m->name;
                    /* pass */
                    self->decl_vars = _tr_dict_new(64LL);
                    /* pass */
                    CGenerator_seed_params(self, m);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, e->name)), " {\n"));
                    /* pass */
                    CGenerator_gen_func_body(self, m->body, 1LL);
                    /* pass */
                    CGenerator_w(self, "}\n\n");
                    /* pass */
                    self->cur_class = "";
                    /* pass */
                    self->cur_func = "";
                    /* pass */
                    mi = (mi + 1LL);
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp((char*)f->name, (char*)"main") != 0) && (!f->is_extern))) {
            /* pass */
            bool in_mod = false;
            /* pass */
            if (((strcmp((char*)f->class_name, (char*)"") == 0) && _tr_dict_contains(fn_set, f->name))) {
                /* pass */
                in_mod = true;
            } else if (((strcmp((char*)f->class_name, (char*)"") != 0) && _tr_dict_contains(class_set, f->class_name))) {
                /* pass */
                bool is_generic_cls = false;
                /* pass */
                if (_tr_dict_contains(self->classes, f->class_name)) {
                    /* pass */
                    if ((((HirClass*)(uintptr_t)_tr_dict_get(self->classes, f->class_name))->generics->len > 0LL)) {
                        /* pass */
                        is_generic_cls = true;
                    }
                }
                /* pass */
                if ((!is_generic_cls)) {
                    /* pass */
                    in_mod = true;
                }
            }
            /* pass */
            if (in_mod) {
                /* pass */
                char* em_key = _tr_str_concat(_tr_str_concat(_tr_str_concat("i_", f->class_name), "_"), f->name);
                /* pass */
                if ((!_tr_dict_contains(self->emitted_fns, em_key))) {
                    /* pass */
                    _tr_dict_set(self->emitted_fns, em_key, true);
                    /* pass */
                    self->cur_func = f->name;
                    /* pass */
                    self->cur_class = f->class_name;
                    /* pass */
                    self->decl_vars = _tr_dict_new(64LL);
                    /* pass */
                    CGenerator_seed_params(self, f);
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f), CGenerator_gen_func_sig(self, f, f->class_name)), " {\n"));
                    /* pass */
                    CGenerator_gen_func_body(self, f->body, 1LL);
                    /* pass */
                    CGenerator_w(self, "}\n\n");
                    /* pass */
                    self->cur_func = "";
                    /* pass */
                    self->cur_class = "";
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->buf));
}

__attribute__((hot)) char* CGenerator_generate_main_c(CGenerator* self, HirProgram* prog, TrMap* class_set, TrMap* fn_set) {
    /* pass */
    self->buf = StringBuilder_init(65536LL);
    /* pass */
    CGenerator_w(self, "#define _TR_MAIN\n");
    /* pass */
    CGenerator_w(self, "#include \"tauraro_types.h\"\n\n");
    /* pass */
    CGenerator_prescan_spawns(self, prog);
    /* pass */
    CGenerator_prescan_awaits(self, prog);
    /* pass */
    CGenerator_w(self, "\n");
    /* pass */
    long long i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
        /* pass */
        while ((i < prog->classes->len)) {
            /* pass */
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(c))))) {
                /* pass */
                if (((c->generics->len == 0LL) && _tr_dict_contains(class_set, c->name))) {
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("cls_", c->name)))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, _tr_str_concat("cls_", c->name), true);
                        /* pass */
                        long long mi = 0LL;
                        /* pass */
                        if ((!_is_invalid_ptr(((unsigned long long)(c->methods))))) {
                            /* pass */
                            while ((mi < c->methods->len)) {
                                /* pass */
                                HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                                /* pass */
                                if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                                    /* pass */
                                    _tr_dict_set(self->emitted_fns, _tr_str_concat(_tr_str_concat(_tr_str_concat("i_", c->name), "_"), m->name), true);
                                    /* pass */
                                    self->cur_class = c->name;
                                    /* pass */
                                    self->cur_func = m->name;
                                    /* pass */
                                    self->cur_throws_ty = m->throws_ty->name;
                                    /* pass */
                                    self->decl_vars = _tr_dict_new(64LL);
                                    /* pass */
                                    self->shared_vars = _tr_dict_new(8LL);
                                    /* pass */
                                    CGenerator_seed_params(self, m);
                                    /* pass */
                                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, c->name)), " {\n"));
                                    /* pass */
                                    CGenerator_gen_func_body(self, m->body, 1LL);
                                    /* pass */
                                    CGenerator_w(self, "}\n\n");
                                    /* pass */
                                    self->cur_class = "";
                                    /* pass */
                                    self->cur_func = "";
                                    /* pass */
                                    self->cur_throws_ty = "";
                                }
                                /* pass */
                                mi = (mi + 1LL);
                            }
                        }
                    }
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->enums)))))) {
        /* pass */
        while ((i < prog->enums->len)) {
            /* pass */
            HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(e))))) {
                /* pass */
                if (_tr_dict_contains(class_set, e->name)) {
                    /* pass */
                    if ((!_tr_dict_contains(self->emitted_fns, _tr_str_concat("cls_", e->name)))) {
                        /* pass */
                        _tr_dict_set(self->emitted_fns, _tr_str_concat("cls_", e->name), true);
                        /* pass */
                        long long mi = 0LL;
                        /* pass */
                        if ((!_is_invalid_ptr(((unsigned long long)(e->methods))))) {
                            /* pass */
                            while ((mi < e->methods->len)) {
                                /* pass */
                                HirFunction* m = ((HirFunction*)List_ptr_get(e->methods, mi));
                                /* pass */
                                if ((!_is_invalid_ptr(((unsigned long long)(m))))) {
                                    /* pass */
                                    self->cur_class = e->name;
                                    /* pass */
                                    self->cur_func = m->name;
                                    /* pass */
                                    self->cur_throws_ty = m->throws_ty->name;
                                    /* pass */
                                    self->decl_vars = _tr_dict_new(64LL);
                                    /* pass */
                                    self->shared_vars = _tr_dict_new(8LL);
                                    /* pass */
                                    CGenerator_seed_params(self, m);
                                    /* pass */
                                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, m), CGenerator_gen_func_sig(self, m, e->name)), " {\n"));
                                    /* pass */
                                    CGenerator_gen_func_body(self, m->body, 1LL);
                                    /* pass */
                                    CGenerator_w(self, "}\n\n");
                                    /* pass */
                                    self->cur_class = "";
                                    /* pass */
                                    self->cur_func = "";
                                    /* pass */
                                    self->cur_throws_ty = "";
                                }
                                /* pass */
                                mi = (mi + 1LL);
                            }
                        }
                    }
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    long long gvi2 = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->top_level_stmts)))))) {
        /* pass */
        while ((gvi2 < prog->top_level_stmts->len)) {
            /* pass */
            __auto_type _t262 = (*((HirStmt*)List_ptr_get(prog->top_level_stmts, gvi2)));
            if (_t262.tag == HirStmt_SLet) {
                __auto_type gvn2 = _t262.data.SLet.name;
__auto_type gvty2 = _t262.data.SLet.ty;
__auto_type gvval2 = _t262.data.SLet.val;
                /* pass */
                char* gv_cty2 = CGenerator_type_to_c(self, gvty2);
                /* pass */
                if (((strcmp((char*)gv_cty2, (char*)"void") == 0) || (strcmp((char*)gv_cty2, (char*)"__auto_type") == 0))) {
                    /* pass */
                    if ((((unsigned long long)(gvval2)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        gv_cty2 = CGenerator_type_to_c(self, hir_expr_type(gvval2));
                    }
                }
                /* pass */
                if (((strcmp((char*)gv_cty2, (char*)"void") == 0) || (strcmp((char*)gv_cty2, (char*)"__auto_type") == 0))) {
                    /* pass */
                    gv_cty2 = "long long";
                }
                /* pass */
                CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(gv_cty2, " "), gvn2), ";\n"));
                /* pass */
                _tr_dict_set(self->global_vars, gvn2, true);
            } else if (1) {
                __auto_type _ = _t262;
                /* pass */
            }
            /* pass */
            gvi2 = (gvi2 + 1LL);
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
        /* pass */
        while ((i < prog->functions->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
                /* pass */
                if (((strcmp((char*)f->name, (char*)"main") != 0) && (!f->is_extern))) {
                    /* pass */
                    bool in_mod = false;
                    /* pass */
                    if (((strcmp((char*)f->class_name, (char*)"") == 0) && _tr_dict_contains(fn_set, f->name))) {
                        /* pass */
                        in_mod = true;
                    } else if (((strcmp((char*)f->class_name, (char*)"") != 0) && _tr_dict_contains(class_set, f->class_name))) {
                        /* pass */
                        bool is_generic_cls2 = false;
                        /* pass */
                        if (_tr_dict_contains(self->classes, f->class_name)) {
                            /* pass */
                            if ((((HirClass*)(uintptr_t)_tr_dict_get(self->classes, f->class_name))->generics->len > 0LL)) {
                                /* pass */
                                is_generic_cls2 = true;
                            }
                        }
                        /* pass */
                        if ((!is_generic_cls2)) {
                            /* pass */
                            in_mod = true;
                        }
                    }
                    /* pass */
                    if (in_mod) {
                        /* pass */
                        char* em_key2 = _tr_str_concat(_tr_str_concat(_tr_str_concat("i_", f->class_name), "_"), f->name);
                        /* pass */
                        if ((!_tr_dict_contains(self->emitted_fns, em_key2))) {
                            /* pass */
                            _tr_dict_set(self->emitted_fns, em_key2, true);
                            /* pass */
                            self->cur_func = f->name;
                            /* pass */
                            self->cur_class = f->class_name;
                            /* pass */
                            self->cur_throws_ty = f->throws_ty->name;
                            /* pass */
                            self->decl_vars = _tr_dict_new(64LL);
                            /* pass */
                            self->shared_vars = _tr_dict_new(8LL);
                            /* pass */
                            CGenerator_seed_params(self, f);
                            /* pass */
                            CGenerator_w(self, _tr_str_concat(_tr_str_concat(CGenerator_get_proto_attrs(self, f), CGenerator_gen_func_sig(self, f, f->class_name)), " {\n"));
                            /* pass */
                            CGenerator_gen_func_body(self, f->body, 1LL);
                            /* pass */
                            CGenerator_w(self, "}\n\n");
                            /* pass */
                            self->cur_func = "";
                            /* pass */
                            self->cur_class = "";
                            /* pass */
                            self->cur_throws_ty = "";
                        }
                    }
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    CGenerator_w(self, "__attribute__((hot)) int main(int argc, char** argv) {\n");
    /* pass */
    CGenerator_w(self, "    _tr_argc = argc; _tr_argv = argv;\n");
    /* pass */
    CGenerator_w(self, "    _tr_init_console();\n");
    /* pass */
    CGenerator_w(self, "#ifndef TAURARO_BARE\n");
    /* pass */
    CGenerator_w(self, "    _tr_global_async_pool = _tr_threadpool_auto();\n");
    /* pass */
    CGenerator_w(self, "#endif\n");
    /* pass */
    bool main_has_args = false;
    /* pass */
    char* main_args_name = "args";
    /* pass */
    long long mi2 = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
        /* pass */
        while ((mi2 < prog->functions->len)) {
            /* pass */
            HirFunction* mf = ((HirFunction*)List_ptr_get(prog->functions, mi2));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(mf))))) {
                /* pass */
                if (((strcmp((char*)mf->name, (char*)"main") == 0) && (mf->params->len > 0LL))) {
                    /* pass */
                    main_has_args = true;
                    /* pass */
                    main_args_name = ((HirParam*)List_ptr_get(mf->params, 0LL))->name;
                }
            }
            /* pass */
            mi2 = (mi2 + 1LL);
        }
    }
    /* pass */
    if (main_has_args) {
        /* pass */
        if ((strcmp((char*)main_args_name, (char*)"argv") == 0)) {
            /* pass */
            main_args_name = "_tr_main_argv";
        }
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    List_str* ", main_args_name), " = List_str_new();\n"));
        /* pass */
        CGenerator_w(self, _tr_str_concat(_tr_str_concat("    for (int _ai = 0; _ai < argc; _ai++) { List_str_append(", main_args_name), ", argv[_ai]); }\n"));
    }
    /* pass */
    i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->top_level_stmts)))))) {
        /* pass */
        while ((i < prog->top_level_stmts->len)) {
            /* pass */
            bool _tlg = false;
            /* pass */
            __auto_type _t263 = (*((HirStmt*)List_ptr_get(prog->top_level_stmts, i)));
            if (_t263.tag == HirStmt_SLet) {
                __auto_type tln2 = _t263.data.SLet.name;
__auto_type tlv2 = _t263.data.SLet.val;
                /* pass */
                if ((_tr_dict_contains(self->global_vars, tln2) && (((unsigned long long)(tlv2)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    CGenerator_w(self, _tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat("    ", tln2), " = "), CGenerator_gen_expr(self, tlv2)), ";\n"));
                    /* pass */
                    _tlg = true;
                }
            } else if (1) {
                __auto_type _ = _t263;
                /* pass */
            }
            /* pass */
            if ((!_tlg)) {
                /* pass */
                CGenerator_gen_stmt(self, ((HirStmt*)List_ptr_get(prog->top_level_stmts, i)), 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
        /* pass */
        while ((i < prog->functions->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
                /* pass */
                if ((strcmp((char*)f->name, (char*)"main") == 0)) {
                    /* pass */
                    self->cur_func = "main";
                    /* pass */
                    self->decl_vars = _tr_dict_new(64LL);
                    /* pass */
                    CGenerator_gen_func_body(self, f->body, 1LL);
                    /* pass */
                    self->cur_func = "";
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    CGenerator_w(self, "#ifndef TAURARO_BARE\n");
    /* pass */
    CGenerator_w(self, "    if (_tr_global_async_pool) { _tr_threadpool_free(_tr_global_async_pool); _tr_global_async_pool = NULL; }\n");
    /* pass */
    CGenerator_w(self, "#endif\n");
    /* pass */
    CGenerator_w(self, "    return 0;\n}\n");
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->buf));
}

__attribute__((hot)) char* _c_dot_to_safe(char* s) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    __auto_type buf = _tr_str_slice(s, 0LL, n);
    /* pass */
    char* bp = ((char*)(buf));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 46LL)) {
            /* pass */
            (*(bp + j) = ((char)(95LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) char* _indent_str(long long n) {
    /* pass */
    char* s = "";
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        s = _tr_str_concat(s, "    ");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) bool _is_invalid_ptr(unsigned long long addr) {
    /* pass */
    if ((addr < ((unsigned long long)(65536LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((addr > ((unsigned long long)(281474976710655LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr == ((unsigned long long)(2880154539LL))) || (addr == ((unsigned long long)(4277075694LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr == ((unsigned long long)(3131961357LL))) || (addr == ((unsigned long long)(3435973836LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr == ((unsigned long long)(3452816845LL))) || (addr == ((unsigned long long)(3722304989LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr == ((unsigned long long)(-76843841185972498LL))) || (addr == ((unsigned long long)(-6076574518398440533LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr == ((unsigned long long)(-4995072469926809587LL))) || (addr == ((unsigned long long)(-3689348814741910324LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((addr % ((unsigned long long)(8LL))) != ((unsigned long long)(0LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_str_type(char* n) {
    /* pass */
    return ((strcmp((char*)n, (char*)"str") == 0) || (strcmp((char*)n, (char*)"String") == 0));
}

__attribute__((hot)) bool _is_int_type(char* n) {
    /* pass */
    if ((((((strcmp((char*)n, (char*)"int") == 0) || (strcmp((char*)n, (char*)"i64") == 0)) || (strcmp((char*)n, (char*)"i32") == 0)) || (strcmp((char*)n, (char*)"i16") == 0)) || (strcmp((char*)n, (char*)"i8") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"u64") == 0) || (strcmp((char*)n, (char*)"u32") == 0)) || (strcmp((char*)n, (char*)"u16") == 0)) || (strcmp((char*)n, (char*)"u8") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)n, (char*)"usize") == 0) || (strcmp((char*)n, (char*)"isize") == 0)) || (strcmp((char*)n, (char*)"long long") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_float_type(char* n) {
    /* pass */
    return ((((strcmp((char*)n, (char*)"float") == 0) || (strcmp((char*)n, (char*)"f64") == 0)) || (strcmp((char*)n, (char*)"f32") == 0)) || (strcmp((char*)n, (char*)"double") == 0));
}

__attribute__((hot)) char* _safe_c_varname(char* n) {
    /* pass */
    if (_is_c_keyword(n)) {
        /* pass */
        return _tr_str_concat("_tr_v_", n);
    }
    /* pass */
    return n;
}

__attribute__((hot)) bool _is_c_keyword(char* n) {
    /* pass */
    if (((((strcmp((char*)n, (char*)"double") == 0) || (strcmp((char*)n, (char*)"float") == 0)) || (strcmp((char*)n, (char*)"int") == 0)) || (strcmp((char*)n, (char*)"char") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"long") == 0) || (strcmp((char*)n, (char*)"short") == 0)) || (strcmp((char*)n, (char*)"void") == 0)) || (strcmp((char*)n, (char*)"unsigned") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"signed") == 0) || (strcmp((char*)n, (char*)"register") == 0)) || (strcmp((char*)n, (char*)"inline") == 0)) || (strcmp((char*)n, (char*)"auto") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"struct") == 0) || (strcmp((char*)n, (char*)"union") == 0)) || (strcmp((char*)n, (char*)"enum") == 0)) || (strcmp((char*)n, (char*)"extern") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"static") == 0) || (strcmp((char*)n, (char*)"volatile") == 0)) || (strcmp((char*)n, (char*)"const") == 0)) || (strcmp((char*)n, (char*)"restrict") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"typedef") == 0) || (strcmp((char*)n, (char*)"sizeof") == 0)) || (strcmp((char*)n, (char*)"alignof") == 0)) || (strcmp((char*)n, (char*)"typeof") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"goto") == 0) || (strcmp((char*)n, (char*)"switch") == 0)) || (strcmp((char*)n, (char*)"case") == 0)) || (strcmp((char*)n, (char*)"default") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp((char*)n, (char*)"do") == 0) || (strcmp((char*)n, (char*)"for") == 0)) || (strcmp((char*)n, (char*)"while") == 0)) || (strcmp((char*)n, (char*)"if") == 0)) || (strcmp((char*)n, (char*)"else") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)n, (char*)"break") == 0) || (strcmp((char*)n, (char*)"continue") == 0)) || (strcmp((char*)n, (char*)"return") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"exit") == 0) || (strcmp((char*)n, (char*)"abort") == 0)) || (strcmp((char*)n, (char*)"remove") == 0)) || (strcmp((char*)n, (char*)"rename") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"read") == 0) || (strcmp((char*)n, (char*)"write") == 0)) || (strcmp((char*)n, (char*)"open") == 0)) || (strcmp((char*)n, (char*)"close") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"send") == 0) || (strcmp((char*)n, (char*)"recv") == 0)) || (strcmp((char*)n, (char*)"bind") == 0)) || (strcmp((char*)n, (char*)"connect") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp((char*)n, (char*)"accept") == 0) || (strcmp((char*)n, (char*)"listen") == 0)) || (strcmp((char*)n, (char*)"socket") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"time") == 0) || (strcmp((char*)n, (char*)"sleep") == 0)) || (strcmp((char*)n, (char*)"signal") == 0)) || (strcmp((char*)n, (char*)"raise") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"stat") == 0) || (strcmp((char*)n, (char*)"mkdir") == 0)) || (strcmp((char*)n, (char*)"rmdir") == 0)) || (strcmp((char*)n, (char*)"unlink") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"malloc") == 0) || (strcmp((char*)n, (char*)"free") == 0)) || (strcmp((char*)n, (char*)"calloc") == 0)) || (strcmp((char*)n, (char*)"realloc") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"printf") == 0) || (strcmp((char*)n, (char*)"scanf") == 0)) || (strcmp((char*)n, (char*)"puts") == 0)) || (strcmp((char*)n, (char*)"gets") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"strlen") == 0) || (strcmp((char*)n, (char*)"strcpy") == 0)) || (strcmp((char*)n, (char*)"strcat") == 0)) || (strcmp((char*)n, (char*)"strcmp") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"system") == 0) || (strcmp((char*)n, (char*)"getenv") == 0)) || (strcmp((char*)n, (char*)"setenv") == 0)) || (strcmp((char*)n, (char*)"putenv") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"fopen") == 0) || (strcmp((char*)n, (char*)"fclose") == 0)) || (strcmp((char*)n, (char*)"fread") == 0)) || (strcmp((char*)n, (char*)"fwrite") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"fseek") == 0) || (strcmp((char*)n, (char*)"ftell") == 0)) || (strcmp((char*)n, (char*)"rewind") == 0)) || (strcmp((char*)n, (char*)"feof") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"index") == 0) || (strcmp((char*)n, (char*)"count") == 0)) || (strcmp((char*)n, (char*)"new") == 0)) || (strcmp((char*)n, (char*)"delete") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _starts_with_tr(char* s) {
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    if ((((long long)(p)) == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((long long)((*(p + 0LL)))) == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((long long)((*(p + 0LL)))) == 95LL)) {
        /* pass */
        if ((((long long)((*(p + 1LL)))) == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((((long long)((*(p + 1LL)))) == 116LL)) {
            /* pass */
            if ((((long long)((*(p + 2LL)))) == 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            if ((((long long)((*(p + 2LL)))) == 114LL)) {
                /* pass */
                if ((((long long)((*(p + 3LL)))) == 0LL)) {
                    /* pass */
                    return false;
                }
                /* pass */
                if ((((long long)((*(p + 3LL)))) == 95LL)) {
                    /* pass */
                    return true;
                }
            }
        }
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_primitive(char* n) {
    /* pass */
    if ((_is_int_type(n) || _is_float_type(n))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp((char*)n, (char*)"bool") == 0) || (strcmp((char*)n, (char*)"char") == 0)) || (strcmp((char*)n, (char*)"void") == 0)) || (strcmp((char*)n, (char*)"None") == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) char* _escape_str_for_c(char* s) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    char* p = ((char*)(s));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if ((c == 34LL)) {
            /* pass */
            StringBuilder_append(sb, "\\\"");
        } else if ((c == 92LL)) {
            /* pass */
            StringBuilder_append(sb, "\\\\");
        } else if ((c == 10LL)) {
            /* pass */
            StringBuilder_append(sb, "\\n");
        } else if ((c == 13LL)) {
            /* pass */
            StringBuilder_append(sb, "\\r");
        } else if ((c == 9LL)) {
            /* pass */
            StringBuilder_append(sb, "\\t");
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

