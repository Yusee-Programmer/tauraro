#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) CppExporter* CppExporter_init(CGenerator* gen) {
    /* pass */
    CppExporter* e = ((CppExporter*)_tr_obj_alloc(sizeof(CppExporter)));
    /* pass */
    CGenerator* _cltmp_t2724 = _tr_obj_retain(gen);
    _tr_obj_release(e->gen, _trdrop_CGenerator);
    e->gen = _cltmp_t2724;
    /* pass */
    e->cls_set = _tr_dict_new(16LL);
    /* pass */
    e->list_elems = _tr_dict_new(8LL);
    /* pass */
    e->dict_variants = _tr_dict_new(8LL);
    /* pass */
    e->needs_tuple = false;
    /* pass */
    e->needs_list_ptr = false;
    /* pass */
    return e;
}

__attribute__((hot)) TrStr CppExporter__cpp_ty_kind(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr n = ty->name;
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit(""))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0))) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("str");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("scalar");
    }
    /* pass */
    if ((_is_int_type(n) || _is_float_type(n))) {
        /* pass */
        return _tr_str_lit("scalar");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0)) && CppExporter__cpp_list_ok(self, ty))) {
        /* pass */
        return _tr_str_lit("list");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) && CppExporter__cpp_dict_ok(self, ty))) {
        /* pass */
        return _tr_str_lit("dict");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Tuple"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("tuple"))) == 0)) && CppExporter__cpp_tuple_ok(self, ty))) {
        /* pass */
        return _tr_str_lit("tuple");
    }
    /* pass */
    if (_tr_dict_contains(self->cls_set, _tr_strz(n))) {
        /* pass */
        return _tr_str_lit("class");
    }
    /* pass */
    return _tr_str_lit("no");
}

__attribute__((hot)) bool CppExporter__cpp_list_ok(CppExporter* self, AstType* ty) {
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr ek = CppExporter__cpp_ty_kind(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
    /* pass */
    return (((strcmp(_tr_strz(ek), _tr_strz(_tr_str_lit("scalar"))) == 0) || (strcmp(_tr_strz(ek), _tr_strz(_tr_str_lit("str"))) == 0)) || (strcmp(_tr_strz(ek), _tr_strz(_tr_str_lit("class"))) == 0));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_kind(CppExporter* self, AstType* ty) {
    /* pass */
    return CppExporter__cpp_ty_kind(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_sfx(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _at_t2725 = (CGenerator_list_elem_suffix(self->gen, (*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); __auto_type _wr = (CGenerator_list_sfx(self->gen, _at_t2725)); _tr_str_release(_at_t2725); _wr; });
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_c(CppExporter* self, AstType* ty) {
    /* pass */
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
    /* pass */
    if ((strcmp(_tr_strz(elem->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("TrStr");
    }
    /* pass */
    return CGenerator_type_to_c(self->gen, elem);
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
    /* pass */
    if ((strcmp(_tr_strz(elem->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("std::string");
    }
    /* pass */
    if (_tr_dict_contains(self->cls_set, _tr_strz(elem->name))) {
        /* pass */
        return _tr_str_retain(elem->name);
    }
    /* pass */
    return CGenerator_type_to_c(self->gen, elem);
}

__attribute__((hot)) bool CppExporter__cpp_slot_ok(CppExporter* self, TrStr name) {
    /* pass */
    return (((_is_int_type(name) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("bool"))) == 0)) || _is_float_type(name)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0));
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_kind(CppExporter* self, TrStr name) {
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("str");
    }
    /* pass */
    if (_is_float_type(name)) {
        /* pass */
        return _tr_str_lit("f64");
    }
    /* pass */
    return _tr_str_lit("i64");
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_c(CppExporter* self, TrStr kind) {
    /* pass */
    if ((strcmp(_tr_strz(kind), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("TrStr");
    }
    /* pass */
    if ((strcmp(_tr_strz(kind), _tr_strz(_tr_str_lit("f64"))) == 0)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    return _tr_str_lit("long long");
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_cpp(CppExporter* self, TrStr kind) {
    /* pass */
    if ((strcmp(_tr_strz(kind), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("std::string");
    }
    /* pass */
    if ((strcmp(_tr_strz(kind), _tr_strz(_tr_str_lit("f64"))) == 0)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    return _tr_str_lit("long long");
}

__attribute__((hot)) bool CppExporter__cpp_dict_ok(CppExporter* self, AstType* ty) {
    /* pass */
    if ((ty->args->len < 2LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr kn = (*((AstType**)List_ptr_get(ty->args, 0LL)))->name;
    /* pass */
    if (((strcmp(_tr_strz(kn), _tr_strz(_tr_str_lit("str"))) != 0) && (!_is_int_type(kn)))) {
        /* pass */
        return false;
    }
    /* pass */
    return CppExporter__cpp_slot_ok(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_kkind(CppExporter* self, AstType* ty) {
    /* pass */
    if ((strcmp(_tr_strz((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("s");
    }
    /* pass */
    return _tr_str_lit("i");
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_vkind(CppExporter* self, AstType* ty) {
    /* pass */
    return CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_variant(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_dict_kkind(self, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_key_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    if ((strcmp(_tr_strz((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("std::string");
    }
    /* pass */
    return _tr_str_lit("long long");
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_v_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _at_t2726 = (CppExporter__cpp_dict_vkind(self, ty)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t2726)); _tr_str_release(_at_t2726); _wr; });
}

__attribute__((hot)) bool CppExporter__cpp_tuple_ok(CppExporter* self, AstType* ty) {
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        if ((!CppExporter__cpp_slot_ok(self, (*((AstType**)List_ptr_get(ty->args, i)))->name))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) TrStr CppExporter__cpp_tuple_elem_cpp(CppExporter* self, AstType* ty, long long i) {
    /* pass */
    return ({ TrStr _at_t2727 = (CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, i)))->name)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t2727)); _tr_str_release(_at_t2727); _wr; });
}

__attribute__((hot)) void CppExporter__note_export_ty(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr n = ty->name;
    /* pass */
    if (((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit(""))) != 0) && _tr_dict_contains(self->gen->classes, _tr_strz(n))) && (!_tr_dict_contains(self->gen->value_types, _tr_strz(n)))) && (!_tr_dict_contains(self->gen->enums, _tr_strz(n))))) {
        /* pass */
        _tr_dict_set(self->cls_set, _tr_strz(n), true);
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            CppExporter__note_export_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        }
        /* pass */
        if (CppExporter__cpp_list_ok(self, ty)) {
            /* pass */
            if ((strcmp(_tr_strz(CppExporter__cpp_list_elem_kind(self, ty)), _tr_strz(_tr_str_lit("class"))) == 0)) {
                /* pass */
                self->needs_list_ptr = true;
            } else {
                /* pass */
                ({ TrStr _dkt_t2728 = (CppExporter__cpp_list_sfx(self, ty)); TrStr _dvt_t2729 = (CppExporter__cpp_list_elem_c(self, ty)); _tr_dict_set(self->list_elems, _tr_strz(_dkt_t2728), _tr_str_box(_tr_str_retain(_dvt_t2729))); _tr_str_release(_dkt_t2728); _tr_str_release(_dvt_t2729); });
            }
        }
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) && CppExporter__cpp_dict_ok(self, ty))) {
        /* pass */
        ({ TrStr _dkt_t2730 = (CppExporter__cpp_dict_variant(self, ty)); TrStr _at_t2731 = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _dvt_t2732 = (CppExporter__cpp_slot_c(self, _at_t2731)); _tr_dict_set(self->dict_variants, _tr_strz(_dkt_t2730), _tr_str_box(_tr_str_retain(_dvt_t2732))); _tr_str_release(_dkt_t2730); _tr_str_release(_at_t2731); _tr_str_release(_dvt_t2732); });
        /* pass */
        if ((strcmp(_tr_strz(CppExporter__cpp_dict_kkind(self, ty)), _tr_strz(_tr_str_lit("s"))) == 0)) {
            /* pass */
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit("TrStr")), _tr_str_box(_tr_str_retain(_tr_str_lit("TrStr"))));
        } else {
            /* pass */
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit("i64")), _tr_str_box(_tr_str_retain(_tr_str_lit("long long"))));
        }
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Tuple"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("tuple"))) == 0)) && CppExporter__cpp_tuple_ok(self, ty))) {
        /* pass */
        self->needs_tuple = true;
    }
}

__attribute__((hot)) void CppExporter_collect_export_classes(CppExporter* self, HirProgram* prog) {
    /* pass */
    self->cls_set = _tr_dict_new(16LL);
    /* pass */
    self->list_elems = _tr_dict_new(8LL);
    /* pass */
    self->dict_variants = _tr_dict_new(8LL);
    /* pass */
    self->needs_tuple = false;
    /* pass */
    self->needs_list_ptr = false;
    /* pass */
    if ((_is_invalid_ptr(((unsigned long long)(prog))) || _is_invalid_ptr(((unsigned long long)(prog->functions))))) {
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
        i = (i + 1LL);
        /* pass */
        if (_is_invalid_ptr(((unsigned long long)(f)))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!(f->is_export && (!f->is_extern)))) {
            /* pass */
            continue;
        }
        /* pass */
        CppExporter__note_export_ty(self, f->ret_ty);
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < f->params->len)) {
            /* pass */
            CppExporter__note_export_ty(self, ((HirParam*)List_ptr_get(f->params, j))->ty);
            /* pass */
            j = (j + 1LL);
        }
    }
}

__attribute__((hot)) bool CppExporter__cpp_export_ty_ok(CppExporter* self, AstType* ty) {
    /* pass */
    return (strcmp(_tr_strz(CppExporter__cpp_ty_kind(self, ty)), _tr_strz(_tr_str_lit("no"))) != 0);
}

__attribute__((hot)) bool CppExporter__cpp_is_class_list(CppExporter* self, AstType* ty) {
    /* pass */
    return ((((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && CppExporter__cpp_list_ok(self, ty)) && (strcmp(_tr_strz(CppExporter__cpp_list_elem_kind(self, ty)), _tr_strz(_tr_str_lit("class"))) == 0));
}

__attribute__((hot)) bool CppExporter__cpp_export_fn_ok(CppExporter* self, HirFunction* f) {
    /* pass */
    if ((strcmp(_tr_strz(f->throws_ty->name), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!CppExporter__cpp_export_ty_ok(self, f->ret_ty))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        AstType* pty = ((HirParam*)List_ptr_get(f->params, i))->ty;
        /* pass */
        if ((!CppExporter__cpp_export_ty_ok(self, pty))) {
            /* pass */
            return false;
        }
        /* pass */
        if (CppExporter__cpp_is_class_list(self, pty)) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool CppExporter__cpp_kind_bridged(CppExporter* self, TrStr k) {
    /* pass */
    return ((((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0)) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0)) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("tuple"))) == 0));
}

__attribute__((hot)) bool CppExporter__cpp_fn_needs_bridge(CppExporter* self, HirFunction* f) {
    /* pass */
    if (({ TrStr _at_t2733 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t2733)); _tr_str_release(_at_t2733); _wr; })) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if (({ TrStr _at_t2734 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t2734)); _tr_str_release(_at_t2734); _wr; })) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool CppExporter__cpp_method_ok(CppExporter* self, HirFunction* m) {
    /* pass */
    if ((strcmp(_tr_strz(m->throws_ty->name), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rk = CppExporter__cpp_ty_kind(self, m->ret_ty);
    /* pass */
    if ((((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("void"))) != 0) && (strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("str"))) != 0)) && (strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("scalar"))) != 0))) {
        /* pass */
        _tr_str_release(rk);
        return false;
    }
    /* pass */
    bool has_self = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->params->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(m->params, i))->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            has_self = true;
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        TrStr k = CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(m->params, i))->ty);
        /* pass */
        if (((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) != 0) && (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("scalar"))) != 0))) {
            /* pass */
            _tr_str_release(rk);
            _tr_str_release(k);
            return false;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(k);
    }
    /* pass */
    _tr_str_release(rk);
    return has_self;
}

__attribute__((hot)) TrStr CppExporter__cpp_tuple_type_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr s = _tr_str_lit("std::tuple<");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t2735 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t2735;
        }
        /* pass */
        TrStr _strtmp_t2736 = ({ TrStr _cr = (CppExporter__cpp_tuple_elem_cpp(self, ty, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t2736;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(">")));
}

__attribute__((hot)) TrStr CppExporter__cpp_c_ty(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0)) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("void*");
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("tuple"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("TrTuple");
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("TrStr");
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("void");
    }
    /* pass */
    _tr_str_release(k);
    return CGenerator_type_to_c(self->gen, ty);
}

__attribute__((hot)) TrStr CppExporter__cpp_c_arg(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("(")), _tr_strz(ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_sfx(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("(List_")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concat(_tr_strz(_tr_str_lit("(TrMap*)")), _tr_strz(name));
    }
    /* pass */
    _tr_str_release(k);
    return _tr_str_retain(name);
}

__attribute__((hot)) TrStr CppExporter__cpp_param_decl(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concat(_tr_strz(_tr_str_lit("const std::string& ")), _tr_strz(name));
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("const std::vector<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">& "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("const std::map<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">& "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("tuple"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_tuple_type_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(k);
    return ({ TrStr _cl = (({ TrStr _cl = (CGenerator_type_to_c(self->gen, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr CppExporter__cpp_call_arg(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TrStr{ (char*)")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c_str(), (long*)0 }"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concat(_tr_strz(name), _tr_strz(_tr_str_lit(".raw()")));
    }
    /* pass */
    _tr_str_release(k);
    return _tr_str_retain(name);
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_type(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("std::string");
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_retain(ty->name);
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("std::vector<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("std::map<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("tuple"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return CppExporter__cpp_tuple_type_cpp(self, ty);
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit("void");
    }
    /* pass */
    _tr_str_release(k);
    return CGenerator_type_to_c(self->gen, ty);
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_stmt(CppExporter* self, AstType* ty, TrStr call) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concat(_tr_strz(call), _tr_strz(_tr_str_lit(";")));
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TrStr _r = ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; return std::string(_r.data ? _r.data : \"\");"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("::_wrap("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(k);
    return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) bool CppExporter__cpp_kind_marshal(CppExporter* self, TrStr k) {
    /* pass */
    return (((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("list"))) == 0) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("dict"))) == 0)) || (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("tuple"))) == 0));
}

__attribute__((hot)) bool CppExporter__cpp_fn_has_list(CppExporter* self, HirFunction* f) {
    /* pass */
    if (({ TrStr _at_t2737 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t2737)); _tr_str_release(_at_t2737); _wr; })) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if (({ TrStr _at_t2738 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t2738)); _tr_str_release(_at_t2738); _wr; })) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr CppExporter__cpp_wrap_fn(CppExporter* self, HirFunction* f) {
    /* pass */
    TrStr target = _tr_strx_concat(_tr_strz(_tr_str_lit("::")), _tr_strz(f->name));
    /* pass */
    if (CppExporter__cpp_fn_needs_bridge(self, f)) {
        /* pass */
        TrStr _strtmp_t2739 = _tr_strx_concat(_tr_strz(_tr_str_lit("tr__")), _tr_strz(f->name));
        _tr_str_release(target);
        target = _strtmp_t2739;
    }
    /* pass */
    StringBuilder* wb = StringBuilder_init(256LL);
    /* pass */
    ({ TrStr _sbt_t2740 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_type(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("    inline ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2740); _tr_str_release(_sbt_t2740); });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        TrStr pn = p->name;
        /* pass */
        if ((strcmp(_tr_strz(pn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            pn = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
        }
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit(", "));
        }
        /* pass */
        ({ TrStr _sbt_t2741 = (CppExporter__cpp_param_decl(self, p->ty, pn)); StringBuilder_append(wb, _sbt_t2741); _tr_str_release(_sbt_t2741); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((!CppExporter__cpp_fn_has_list(self, f))) {
        /* pass */
        TrStr call_args = _tr_str_lit("");
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < f->params->len)) {
            /* pass */
            HirParam* p2 = ((HirParam*)List_ptr_get(f->params, i));
            /* pass */
            TrStr pn2 = p2->name;
            /* pass */
            if ((strcmp(_tr_strz(pn2), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                pn2 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
            }
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t2742 = _tr_strx_concat(_tr_strz(call_args), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(call_args);
                call_args = _strtmp_t2742;
            }
            /* pass */
            TrStr _strtmp_t2743 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p2->ty, pn2)); TrStr _cres = _tr_strx_concat(_tr_strz(call_args), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(call_args);
            call_args = _strtmp_t2743;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        ({ TrStr _at_t2744 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(target), _tr_strz(_tr_str_lit("(")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t2745 = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_stmt(self, f->ret_ty, _at_t2744)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit(") { ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2745); _tr_str_release(_at_t2744); _tr_str_release(_sbt_t2745); });
        /* pass */
        _tr_str_release(target);
        _tr_str_release(call_args);
        return StringObj_as_str(StringBuilder_to_string(wb));
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit(") {\n"));
    /* pass */
    TrStr call_args = _tr_str_lit("");
    /* pass */
    TrStr frees = _tr_str_lit("");
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p3 = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        TrStr pn3 = p3->name;
        /* pass */
        if ((strcmp(_tr_strz(pn3), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            pn3 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
        }
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t2746 = _tr_strx_concat(_tr_strz(call_args), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(call_args);
            call_args = _strtmp_t2746;
        }
        /* pass */
        TrStr pk = CppExporter__cpp_ty_kind(self, p3->ty);
        /* pass */
        if ((strcmp(_tr_strz(pk), _tr_strz(_tr_str_lit("list"))) == 0)) {
            /* pass */
            TrStr sfx = CppExporter__cpp_list_sfx(self, p3->ty);
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_l")), _cr.data); _tr_str_release(_cr); _cres; });
            /* pass */
            TrStr econv = _tr_str_lit("_e");
            /* pass */
            if ((strcmp(_tr_strz((*((AstType**)List_ptr_get(p3->ty->args, 0LL)))->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2747 = _tr_str_lit("TrStr{ (char*)_e.c_str(), (long*)0 }");
                _tr_str_release(econv);
                econv = _strtmp_t2747;
            }
            /* pass */
            ({ TrStr _sbt_t2748 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        void* ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new();\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2748); _tr_str_release(_sbt_t2748); });
            /* pass */
            ({ TrStr _sbt_t2749 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        for (const auto& _e : ")), _tr_strz(pn3))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_push("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(econv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2749); _tr_str_release(_sbt_t2749); });
            /* pass */
            TrStr _strtmp_t2750 = _tr_strx_concat(_tr_strz(call_args), _tr_strz(tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t2750;
            /* pass */
            TrStr _strtmp_t2751 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(frees), _tr_strz(_tr_str_lit("        tr__list_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(frees);
            frees = _strtmp_t2751;
            _tr_str_release(sfx);
            _tr_str_release(tmp);
            _tr_str_release(econv);
        } else if ((strcmp(_tr_strz(pk), _tr_strz(_tr_str_lit("dict"))) == 0)) {
            /* pass */
            TrStr vt = CppExporter__cpp_dict_variant(self, p3->ty);
            /* pass */
            TrStr vk = CppExporter__cpp_dict_vkind(self, p3->ty);
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_d")), _cr.data); _tr_str_release(_cr); _cres; });
            /* pass */
            TrStr kconv = _tr_str_lit("_kv.first");
            /* pass */
            if ((strcmp(_tr_strz(CppExporter__cpp_dict_kkind(self, p3->ty)), _tr_strz(_tr_str_lit("s"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2752 = _tr_str_lit("TrStr{ (char*)_kv.first.c_str(), (long*)0 }");
                _tr_str_release(kconv);
                kconv = _strtmp_t2752;
            }
            /* pass */
            TrStr vconv = _tr_str_lit("_kv.second");
            /* pass */
            if ((strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2753 = _tr_str_lit("TrStr{ (char*)_kv.second.c_str(), (long*)0 }");
                _tr_str_release(vconv);
                vconv = _strtmp_t2753;
            }
            /* pass */
            ({ TrStr _sbt_t2754 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        void* ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new();\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2754); _tr_str_release(_sbt_t2754); });
            /* pass */
            ({ TrStr _sbt_t2755 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        for (const auto& _kv : ")), _tr_strz(pn3))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_set("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kconv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vconv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2755); _tr_str_release(_sbt_t2755); });
            /* pass */
            TrStr _strtmp_t2756 = _tr_strx_concat(_tr_strz(call_args), _tr_strz(tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t2756;
            /* pass */
            TrStr _strtmp_t2757 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(frees), _tr_strz(_tr_str_lit("        tr__dict_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(frees);
            frees = _strtmp_t2757;
            _tr_str_release(vk);
            _tr_str_release(vt);
            _tr_str_release(tmp);
            _tr_str_release(kconv);
            _tr_str_release(vconv);
        } else if ((strcmp(_tr_strz(pk), _tr_strz(_tr_str_lit("tuple"))) == 0)) {
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_t")), _cr.data); _tr_str_release(_cr); _cres; });
            /* pass */
            ({ TrStr _sbt_t2758 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        TrTuple ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = {};\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2758); _tr_str_release(_sbt_t2758); });
            /* pass */
            long long ti = 0LL;
            /* pass */
            while ((ti < p3->ty->args->len)) {
                /* pass */
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(p3->ty->args, ti)))->name);
                /* pass */
                TrStr g = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("std::get<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn3)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(sk), _tr_strz(_tr_str_lit("str"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2759 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".data["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] = tr__box_str(TrStr{ (char*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".c_str(), (long*)0 });\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2759); _tr_str_release(_sbt_t2759); });
                } else if ((strcmp(_tr_strz(sk), _tr_strz(_tr_str_lit("f64"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2760 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".data["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] = tr__box_f64("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2760); _tr_str_release(_sbt_t2760); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t2761 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(tmp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".data["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] = (long long)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2761); _tr_str_release(_sbt_t2761); });
                }
                /* pass */
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(g);
            }
            /* pass */
            TrStr _strtmp_t2762 = _tr_strx_concat(_tr_strz(call_args), _tr_strz(tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t2762;
            _tr_str_release(tmp);
        } else {
            /* pass */
            TrStr _strtmp_t2763 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p3->ty, pn3)); TrStr _cres = _tr_strx_concat(_tr_strz(call_args), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(call_args);
            call_args = _strtmp_t2763;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(pk);
    }
    /* pass */
    TrStr call_expr = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(target), _tr_strz(_tr_str_lit("(")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    /* pass */
    TrStr rk = CppExporter__cpp_ty_kind(self, f->ret_ty);
    /* pass */
    if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        ({ TrStr _sbt_t2764 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(call_expr))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(frees)); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2764); _tr_str_release(_sbt_t2764); });
    } else {
        /* pass */
        ({ TrStr _sbt_t2765 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" _r = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call_expr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(frees)); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2765); _tr_str_release(_sbt_t2765); });
        /* pass */
        if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("list"))) == 0)) {
            /* pass */
            TrStr ecpp = CppExporter__cpp_list_elem_cpp(self, f->ret_ty);
            /* pass */
            if ((strcmp(_tr_strz(CppExporter__cpp_list_elem_kind(self, f->ret_ty)), _tr_strz(_tr_str_lit("class"))) == 0)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit("        long long _n = tr__list_ptr_len(_r);\n"));
                /* pass */
                ({ TrStr _sbt_t2766 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        std::vector<")), _tr_strz(ecpp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("> _v; _v.reserve((size_t)_n);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2766); _tr_str_release(_sbt_t2766); });
                /* pass */
                ({ TrStr _sbt_t2767 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(")), _tr_strz(ecpp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("::_wrap((void*)tr__list_ptr_get(_r, _i)));\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2767); _tr_str_release(_sbt_t2767); });
                /* pass */
                StringBuilder_append(wb, _tr_str_lit("        tr__list_ptr_free_shallow(_r);\n        return _v;\n"));
            } else {
                /* pass */
                TrStr rsfx = CppExporter__cpp_list_sfx(self, f->ret_ty);
                /* pass */
                ({ TrStr _sbt_t2768 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        long long _n = tr__list_")), _tr_strz(rsfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_len(_r);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2768); _tr_str_release(_sbt_t2768); });
                /* pass */
                ({ TrStr _sbt_t2769 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        std::vector<")), _tr_strz(ecpp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("> _v; _v.reserve((size_t)_n);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2769); _tr_str_release(_sbt_t2769); });
                /* pass */
                if ((strcmp(_tr_strz((*((AstType**)List_ptr_get(f->ret_ty->args, 0LL)))->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2770 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        for (long long _i = 0; _i < _n; ++_i) { TrStr _s = tr__list_")), _tr_strz(rsfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(_r, _i); _v.push_back(std::string(_s.data ? _s.data : \"\")); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2770); _tr_str_release(_sbt_t2770); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t2771 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(tr__list_")), _tr_strz(rsfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(_r, _i));\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2771); _tr_str_release(_sbt_t2771); });
                }
                /* pass */
                ({ TrStr _sbt_t2772 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        tr__list_")), _tr_strz(rsfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(_r);\n        return _v;\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2772); _tr_str_release(_sbt_t2772); });
                _tr_str_release(rsfx);
            }
        } else if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("dict"))) == 0)) {
            /* pass */
            TrStr vt = CppExporter__cpp_dict_variant(self, f->ret_ty);
            /* pass */
            TrStr vk = CppExporter__cpp_dict_vkind(self, f->ret_ty);
            /* pass */
            TrStr ksfx = _tr_str_lit("TrStr");
            /* pass */
            if ((strcmp(_tr_strz(CppExporter__cpp_dict_kkind(self, f->ret_ty)), _tr_strz(_tr_str_lit("i"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2773 = _tr_str_lit("i64");
                _tr_str_release(ksfx);
                ksfx = _strtmp_t2773;
            }
            /* pass */
            ({ TrStr _sbt_t2774 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        void* _ks = tr__dict_")), _tr_strz(vt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_keys(_r);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2774); _tr_str_release(_sbt_t2774); });
            /* pass */
            ({ TrStr _sbt_t2775 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        long long _n = tr__list_")), _tr_strz(ksfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_len(_ks);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2775); _tr_str_release(_sbt_t2775); });
            /* pass */
            ({ TrStr _sbt_t2776 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("        std::map<")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("> _m;\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2776); _tr_str_release(_sbt_t2776); });
            /* pass */
            StringBuilder_append(wb, _tr_str_lit("        for (long long _i = 0; _i < _n; ++_i) {\n"));
            /* pass */
            if ((strcmp(_tr_strz(ksfx), _tr_strz(_tr_str_lit("TrStr"))) == 0)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit("            TrStr _k = tr__list_TrStr_get(_ks, _i); std::string _kk2(_k.data ? _k.data : \"\");\n"));
            } else {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit("            long long _k = tr__list_i64_get(_ks, _i); long long _kk2 = _k;\n"));
            }
            /* pass */
            if ((strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                ({ TrStr _sbt_t2777 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("            TrStr _vv = tr__dict_")), _tr_strz(vt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(_r, _k); _m[_kk2] = std::string(_vv.data ? _vv.data : \"\");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2777); _tr_str_release(_sbt_t2777); });
            } else {
                /* pass */
                ({ TrStr _sbt_t2778 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("            _m[_kk2] = tr__dict_")), _tr_strz(vt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(_r, _k);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2778); _tr_str_release(_sbt_t2778); });
            }
            /* pass */
            StringBuilder_append(wb, _tr_str_lit("        }\n"));
            /* pass */
            ({ TrStr _sbt_t2779 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        tr__list_")), _tr_strz(ksfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(_ks); tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(_r);\n        return _m;\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2779); _tr_str_release(_sbt_t2779); });
            _tr_str_release(vt);
            _tr_str_release(ksfx);
        } else if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("tuple"))) == 0)) {
            /* pass */
            TrStr parts = _tr_str_lit("");
            /* pass */
            long long ti = 0LL;
            /* pass */
            while ((ti < f->ret_ty->args->len)) {
                /* pass */
                TrStr ev = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_te")), _cr.data); _tr_str_release(_cr); _cres; });
                /* pass */
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(f->ret_ty->args, ti)))->name);
                /* pass */
                TrStr slot = ({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_r.data[")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(sk), _tr_strz(_tr_str_lit("str"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2780 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("        TrStr _ts")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = tr__unbox_str("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); std::string "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(_ts"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".data ? _ts"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".data : \"\");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2780); _tr_str_release(_sbt_t2780); });
                } else if ((strcmp(_tr_strz(sk), _tr_strz(_tr_str_lit("f64"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2781 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        double ")), _tr_strz(ev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = tr__unbox_f64("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2781); _tr_str_release(_sbt_t2781); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t2782 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        long long ")), _tr_strz(ev))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = (long long)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2782); _tr_str_release(_sbt_t2782); });
                }
                /* pass */
                if ((ti > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t2783 = _tr_strx_concat(_tr_strz(parts), _tr_strz(_tr_str_lit(", ")));
                    _tr_str_release(parts);
                    parts = _strtmp_t2783;
                }
                /* pass */
                TrStr _strtmp_t2784 = _tr_strx_concat(_tr_strz(parts), _tr_strz(ev));
                _tr_str_release(parts);
                parts = _strtmp_t2784;
                /* pass */
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(ev);
                _tr_str_release(slot);
            }
            /* pass */
            ({ TrStr _sbt_t2785 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        return std::make_tuple(")), _tr_strz(parts))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2785); _tr_str_release(_sbt_t2785); });
            _tr_str_release(parts);
        } else if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit("        return std::string(_r.data ? _r.data : \"\");\n"));
        } else if ((strcmp(_tr_strz(rk), _tr_strz(_tr_str_lit("class"))) == 0)) {
            /* pass */
            ({ TrStr _sbt_t2786 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        return ")), _tr_strz(f->ret_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("::_wrap(_r);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2786); _tr_str_release(_sbt_t2786); });
        } else {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit("        return _r;\n"));
        }
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit("    }\n"));
    /* pass */
    _tr_str_release(target);
    _tr_str_release(call_args);
    _tr_str_release(frees);
    _tr_str_release(call_expr);
    _tr_str_release(rk);
    return StringObj_as_str(StringBuilder_to_string(wb));
}

__attribute__((hot)) bool CppExporter__cpp_init_ok(CppExporter* self, HirFunction* m) {
    /* pass */
    if ((strcmp(_tr_strz(m->throws_ty->name), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((strcmp(_tr_strz(CppExporter__cpp_ty_kind(self, m->ret_ty)), _tr_strz(_tr_str_lit("class"))) != 0)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(m->params, i));
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr k = CppExporter__cpp_ty_kind(self, p->ty);
        /* pass */
        if (((strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("str"))) != 0) && (strcmp(_tr_strz(k), _tr_strz(_tr_str_lit("scalar"))) != 0))) {
            /* pass */
            _tr_str_release(k);
            return false;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(k);
    }
    /* pass */
    return true;
}

__attribute__((hot)) long long CppExporter__cpp_init_idx(CppExporter* self, HirClass* c) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, i));
        /* pass */
        if ((((!_is_invalid_ptr(((unsigned long long)(m)))) && (strcmp(_tr_strz(m->name), _tr_strz(_tr_str_lit("init"))) == 0)) && CppExporter__cpp_init_ok(self, m))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_class_decl(CppExporter* self, HirClass* c) {
    /* pass */
    TrStr C = c->name;
    /* pass */
    StringBuilder* wb = StringBuilder_init(512LL);
    /* pass */
    ({ TrStr _sbt_t2787 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    class ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" {\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2787); _tr_str_release(_sbt_t2787); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit("        void* h_;\n"));
    /* pass */
    ({ TrStr _sbt_t2788 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        explicit ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(void* _p): h_(_p) {}\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2788); _tr_str_release(_sbt_t2788); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit("    public:\n"));
    /* pass */
    ({ TrStr _sbt_t2789 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        static ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" _wrap(void* _p) { return "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(_p); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2789); _tr_str_release(_sbt_t2789); });
    /* pass */
    ({ TrStr _sbt_t2790 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(const "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& o): h_(o.h_ ? tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_retain(o.h_) : nullptr) {}\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2790); _tr_str_release(_sbt_t2790); });
    /* pass */
    ({ TrStr _sbt_t2791 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("&& o) noexcept : h_(o.h_) { o.h_ = nullptr; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2791); _tr_str_release(_sbt_t2791); });
    /* pass */
    ({ TrStr _sbt_t2792 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& operator=(const "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& o) { if (this != &o) { if (h_) tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_release(h_); h_ = o.h_ ? tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_retain(o.h_) : nullptr; } return *this; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2792); _tr_str_release(_sbt_t2792); });
    /* pass */
    ({ TrStr _sbt_t2793 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("& operator=("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("&& o) noexcept { if (this != &o) { if (h_) tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_release(h_); h_ = o.h_; o.h_ = nullptr; } return *this; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2793); _tr_str_release(_sbt_t2793); });
    /* pass */
    ({ TrStr _sbt_t2794 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        ~")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("() { if (h_) tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_release(h_); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2794); _tr_str_release(_sbt_t2794); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit("        void* raw() const { return h_; }\n"));
    /* pass */
    long long iidx = CppExporter__cpp_init_idx(self, c);
    /* pass */
    if ((iidx >= 0LL)) {
        /* pass */
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
        /* pass */
        ({ TrStr _sbt_t2795 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("        explicit ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2795); _tr_str_release(_sbt_t2795); });
        /* pass */
        TrStr cargs = _tr_str_lit("");
        /* pass */
        long long pi = 0LL;
        /* pass */
        long long emitted = 0LL;
        /* pass */
        while ((pi < im->params->len)) {
            /* pass */
            HirParam* p = ((HirParam*)List_ptr_get(im->params, pi));
            /* pass */
            pi = (pi + 1LL);
            /* pass */
            if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                /* pass */
                continue;
            }
            /* pass */
            if ((emitted > 0LL)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit(", "));
                /* pass */
                TrStr _strtmp_t2796 = _tr_strx_concat(_tr_strz(cargs), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(cargs);
                cargs = _strtmp_t2796;
            }
            /* pass */
            ({ TrStr _sbt_t2797 = (CppExporter__cpp_param_decl(self, p->ty, p->name)); StringBuilder_append(wb, _sbt_t2797); _tr_str_release(_sbt_t2797); });
            /* pass */
            TrStr _strtmp_t2798 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p->ty, p->name)); TrStr _cres = _tr_strx_concat(_tr_strz(cargs), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(cargs);
            cargs = _strtmp_t2798;
            /* pass */
            emitted = (emitted + 1LL);
        }
        /* pass */
        ({ TrStr _sbt_t2799 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("): h_(tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")) {}\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2799); _tr_str_release(_sbt_t2799); });
        _tr_str_release(cargs);
    }
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < c->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
        /* pass */
        mi = (mi + 1LL);
        /* pass */
        if (_is_invalid_ptr(((unsigned long long)(m)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (((strcmp(_tr_strz(m->name), _tr_strz(_tr_str_lit("init"))) == 0) || _tr_str_starts_with(_tr_strz(m->name), _tr_strz(_tr_str_lit("__"))))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!CppExporter__cpp_method_ok(self, m))) {
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _sbt_t2800 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_type(self, m->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("        ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2800); _tr_str_release(_sbt_t2800); });
        /* pass */
        TrStr margs = _tr_str_lit("h_");
        /* pass */
        long long pj = 0LL;
        /* pass */
        long long me = 0LL;
        /* pass */
        while ((pj < m->params->len)) {
            /* pass */
            HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
            /* pass */
            pj = (pj + 1LL);
            /* pass */
            if ((strcmp(_tr_strz(mp->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                /* pass */
                continue;
            }
            /* pass */
            if ((me > 0LL)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit(", "));
            }
            /* pass */
            ({ TrStr _sbt_t2801 = (CppExporter__cpp_param_decl(self, mp->ty, mp->name)); StringBuilder_append(wb, _sbt_t2801); _tr_str_release(_sbt_t2801); });
            /* pass */
            TrStr _strtmp_t2802 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(margs), _tr_strz(_tr_str_lit(", ")))); TrStr _cr = (CppExporter__cpp_call_arg(self, mp->ty, mp->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(margs);
            margs = _strtmp_t2802;
            /* pass */
            me = (me + 1LL);
        }
        /* pass */
        ({ TrStr _at_t2803 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t2804 = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_stmt(self, m->ret_ty, _at_t2803)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit(") { ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t2804); _tr_str_release(_at_t2803); _tr_str_release(_sbt_t2804); });
        _tr_str_release(margs);
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit("    };\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(wb));
}

__attribute__((hot)) TrStr CppExporter_generate_cpp_class_bridges(CppExporter* self, HirProgram* prog) {
    /* pass */
    CppExporter_collect_export_classes(self, prog);
    /* pass */
    StringBuilder* b = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder_append(b, _tr_str_lit("\n/* --export-cpp: stable extern-\"C\" bridges over the ARC runtime (for the C++ header). */\n"));
    /* pass */
    StringBuilder_append(b, _tr_str_lit("#ifdef __cplusplus\nextern \"C\" {\n#endif\n"));
    /* pass */
    long long ci = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
        /* pass */
        while ((ci < prog->classes->len)) {
            /* pass */
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
            /* pass */
            ci = (ci + 1LL);
            /* pass */
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
                /* pass */
                continue;
            }
            /* pass */
            TrStr C = c->name;
            /* pass */
            ({ TrStr _sbt_t2805 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void* tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_retain(void* p){ return _tr_obj_retain(p); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2805); _tr_str_release(_sbt_t2805); });
            /* pass */
            ({ TrStr _sbt_t2806 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_release(void* p){ _tr_obj_release(p, _trdrop_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2806); _tr_str_release(_sbt_t2806); });
            /* pass */
            long long iidx = CppExporter__cpp_init_idx(self, c);
            /* pass */
            if ((iidx >= 0LL)) {
                /* pass */
                HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
                /* pass */
                ({ TrStr _sbt_t2807 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void* tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2807); _tr_str_release(_sbt_t2807); });
                /* pass */
                long long ip = 0LL;
                /* pass */
                long long ie = 0LL;
                /* pass */
                TrStr iargs = _tr_str_lit("");
                /* pass */
                while ((ip < im->params->len)) {
                    /* pass */
                    HirParam* p = ((HirParam*)List_ptr_get(im->params, ip));
                    /* pass */
                    ip = (ip + 1LL);
                    /* pass */
                    if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                        /* pass */
                        continue;
                    }
                    /* pass */
                    if ((ie > 0LL)) {
                        /* pass */
                        StringBuilder_append(b, _tr_str_lit(", "));
                        /* pass */
                        TrStr _strtmp_t2808 = _tr_strx_concat(_tr_strz(iargs), _tr_strz(_tr_str_lit(", ")));
                        _tr_str_release(iargs);
                        iargs = _strtmp_t2808;
                    }
                    /* pass */
                    ({ TrStr _sbt_t2809 = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(p->name)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2809); _tr_str_release(_sbt_t2809); });
                    /* pass */
                    TrStr _strtmp_t2810 = _tr_strx_concat(_tr_strz(iargs), _tr_strz(p->name));
                    _tr_str_release(iargs);
                    iargs = _strtmp_t2810;
                    /* pass */
                    ie = (ie + 1LL);
                }
                /* pass */
                ({ TrStr _sbt_t2811 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("){ return (void*)")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_init("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2811); _tr_str_release(_sbt_t2811); });
                _tr_str_release(iargs);
            }
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
                /* pass */
                mi = (mi + 1LL);
                /* pass */
                if (_is_invalid_ptr(((unsigned long long)(m)))) {
                    /* pass */
                    continue;
                }
                /* pass */
                if (((strcmp(_tr_strz(m->name), _tr_strz(_tr_str_lit("init"))) == 0) || _tr_str_starts_with(_tr_strz(m->name), _tr_strz(_tr_str_lit("__"))))) {
                    /* pass */
                    continue;
                }
                /* pass */
                if ((!CppExporter__cpp_method_ok(self, m))) {
                    /* pass */
                    continue;
                }
                /* pass */
                ({ TrStr _sbt_t2812 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(void* s"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2812); _tr_str_release(_sbt_t2812); });
                /* pass */
                TrStr margs = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("(")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)s"))); _tr_str_release(_cl); _cres; });
                /* pass */
                long long pj = 0LL;
                /* pass */
                while ((pj < m->params->len)) {
                    /* pass */
                    HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
                    /* pass */
                    pj = (pj + 1LL);
                    /* pass */
                    if ((strcmp(_tr_strz(mp->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                        /* pass */
                        continue;
                    }
                    /* pass */
                    ({ TrStr _sbt_t2813 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit(", ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(mp->name)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2813); _tr_str_release(_sbt_t2813); });
                    /* pass */
                    TrStr _strtmp_t2814 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(margs), _tr_strz(_tr_str_lit(", ")))); TrStr _cr = (CppExporter__cpp_c_arg(self, mp->ty, mp->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(margs);
                    margs = _strtmp_t2814;
                }
                /* pass */
                StringBuilder_append(b, _tr_str_lit("){ "));
                /* pass */
                if ((strcmp(_tr_strz(CppExporter__cpp_ty_kind(self, m->ret_ty)), _tr_strz(_tr_str_lit("void"))) == 0)) {
                    /* pass */
                    ({ TrStr _sbt_t2815 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(C), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2815); _tr_str_release(_sbt_t2815); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t2816 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2816); _tr_str_release(_sbt_t2816); });
                }
                _tr_str_release(margs);
            }
        }
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
        /* pass */
        while ((fi < prog->functions->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, fi));
            /* pass */
            fi = (fi + 1LL);
            /* pass */
            if (_is_invalid_ptr(((unsigned long long)(f)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((!(f->is_export && (!f->is_extern)))) {
                /* pass */
                continue;
            }
            /* pass */
            if (((!CppExporter__cpp_export_fn_ok(self, f)) || (!CppExporter__cpp_fn_needs_bridge(self, f)))) {
                /* pass */
                continue;
            }
            /* pass */
            ({ TrStr _sbt_t2817 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2817); _tr_str_release(_sbt_t2817); });
            /* pass */
            TrStr fargs = _tr_str_lit("");
            /* pass */
            long long pi2 = 0LL;
            /* pass */
            while ((pi2 < f->params->len)) {
                /* pass */
                HirParam* p = ((HirParam*)List_ptr_get(f->params, pi2));
                /* pass */
                TrStr pn = p->name;
                /* pass */
                if ((strcmp(_tr_strz(pn), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    pn = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi2)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
                }
                /* pass */
                if ((pi2 > 0LL)) {
                    /* pass */
                    StringBuilder_append(b, _tr_str_lit(", "));
                    /* pass */
                    TrStr _strtmp_t2818 = _tr_strx_concat(_tr_strz(fargs), _tr_strz(_tr_str_lit(", ")));
                    _tr_str_release(fargs);
                    fargs = _strtmp_t2818;
                }
                /* pass */
                ({ TrStr _sbt_t2819 = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2819); _tr_str_release(_sbt_t2819); });
                /* pass */
                TrStr _strtmp_t2820 = ({ TrStr _cr = (CppExporter__cpp_c_arg(self, p->ty, pn)); TrStr _cres = _tr_strx_concat(_tr_strz(fargs), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(fargs);
                fargs = _strtmp_t2820;
                /* pass */
                pi2 = (pi2 + 1LL);
            }
            /* pass */
            StringBuilder_append(b, _tr_str_lit("){ "));
            /* pass */
            TrStr frk = CppExporter__cpp_ty_kind(self, f->ret_ty);
            /* pass */
            if ((strcmp(_tr_strz(frk), _tr_strz(_tr_str_lit("void"))) == 0)) {
                /* pass */
                ({ TrStr _sbt_t2821 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(f->name), _tr_strz(_tr_str_lit("(")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2821); _tr_str_release(_sbt_t2821); });
            } else if ((((strcmp(_tr_strz(frk), _tr_strz(_tr_str_lit("class"))) == 0) || (strcmp(_tr_strz(frk), _tr_strz(_tr_str_lit("list"))) == 0)) || (strcmp(_tr_strz(frk), _tr_strz(_tr_str_lit("dict"))) == 0))) {
                /* pass */
                ({ TrStr _sbt_t2822 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (void*)")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2822); _tr_str_release(_sbt_t2822); });
            } else {
                /* pass */
                ({ TrStr _sbt_t2823 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2823); _tr_str_release(_sbt_t2823); });
            }
            _tr_str_release(fargs);
            _tr_str_release(frk);
        }
    }
    /* pass */
    ({ TrStr _sbt_t2824 = (CppExporter__cpp_list_bridges(self)); StringBuilder_append(b, _sbt_t2824); _tr_str_release(_sbt_t2824); });
    /* pass */
    StringBuilder_append(b, _tr_str_lit("#ifdef __cplusplus\n}\n#endif\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(b));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_bridges(CppExporter* self) {
    /* pass */
    StringBuilder* b = StringBuilder_init(512LL);
    /* pass */
    List_TrStr* keys = _tr_dict_keys(self->list_elems);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < keys->len)) {
        /* pass */
        TrStr sfx = List_TrStr_get(keys, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        TrStr ec = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->list_elems, _tr_strz(sfx))));
        /* pass */
        TrStr L = _tr_strx_concat(_tr_strz(_tr_str_lit("List_")), _tr_strz(sfx));
        /* pass */
        ({ TrStr _sbt_t2825 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT long long tr__list_")), _tr_strz(sfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_len(void* l){ return (long long)(("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)l)->len; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2825); _tr_str_release(_sbt_t2825); });
        /* pass */
        ({ TrStr _sbt_t2826 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT ")), _tr_strz(ec))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(void* l, long long i){ return "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)l, i); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2826); _tr_str_release(_sbt_t2826); });
        /* pass */
        ({ TrStr _sbt_t2827 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void* tr__list_")), _tr_strz(sfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new(void){ return (void*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new(); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2827); _tr_str_release(_sbt_t2827); });
        /* pass */
        ({ TrStr _sbt_t2828 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void tr__list_")), _tr_strz(sfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_push(void* l, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" v){ "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_append(("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)l, v); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2828); _tr_str_release(_sbt_t2828); });
        /* pass */
        ({ TrStr _sbt_t2829 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void tr__list_")), _tr_strz(sfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(void* l){ "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)l); }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2829); _tr_str_release(_sbt_t2829); });
        _tr_str_release(sfx);
        _tr_str_release(ec);
        _tr_str_release(L);
    }
    /* pass */
    List_TrStr* dk = _tr_dict_keys(self->dict_variants);
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < dk->len)) {
        /* pass */
        TrStr V = List_TrStr_get(dk, di);
        /* pass */
        di = (di + 1LL);
        /* pass */
        TrStr kk = _tr_str_wrap(_tr_str_slice(_tr_strz(V), 0LL, 1LL));
        /* pass */
        TrStr vk = _tr_str_wrap(_tr_str_slice(_tr_strz(V), 2LL, _tr_strlen(_tr_strz(V))));
        /* pass */
        TrStr kc = _tr_str_lit("long long");
        /* pass */
        TrStr newfn = _tr_str_lit("_tr_idict_new(8)");
        /* pass */
        TrStr keysfn = _tr_str_lit("_tr_idict_keys((TrIDict*)d)");
        /* pass */
        TrStr getraw = _tr_str_lit("_tr_idict_get((TrIDict*)d, k)");
        /* pass */
        TrStr setpre = _tr_str_lit("_tr_idict_set((TrIDict*)d, k, ");
        /* pass */
        if ((strcmp(_tr_strz(kk), _tr_strz(_tr_str_lit("s"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2830 = _tr_str_lit("TrStr");
            _tr_str_release(kc);
            kc = _strtmp_t2830;
            /* pass */
            TrStr _strtmp_t2831 = _tr_str_lit("_tr_dict_new(8)");
            _tr_str_release(newfn);
            newfn = _strtmp_t2831;
            /* pass */
            TrStr _strtmp_t2832 = _tr_str_lit("_tr_dict_keys((TrMap*)d)");
            _tr_str_release(keysfn);
            keysfn = _strtmp_t2832;
            /* pass */
            TrStr _strtmp_t2833 = _tr_str_lit("_tr_dict_get((TrMap*)d, _tr_strz(k))");
            _tr_str_release(getraw);
            getraw = _strtmp_t2833;
            /* pass */
            TrStr _strtmp_t2834 = _tr_str_lit("_tr_dict_set((TrMap*)d, _tr_strz(k), ");
            _tr_str_release(setpre);
            setpre = _strtmp_t2834;
        }
        /* pass */
        ({ TrStr _sbt_t2835 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void* tr__dict_")), _tr_strz(V))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new(void){ return (void*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(newfn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2835); _tr_str_release(_sbt_t2835); });
        /* pass */
        ({ TrStr _sbt_t2836 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void* tr__dict_")), _tr_strz(V))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_keys(void* d){ return (void*)"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(keysfn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2836); _tr_str_release(_sbt_t2836); });
        /* pass */
        TrStr vc = _tr_str_lit("long long");
        /* pass */
        TrStr getval = _tr_strx_concat(_tr_strz(_tr_str_lit("(long long)(uintptr_t)")), _tr_strz(getraw));
        /* pass */
        TrStr setval = _tr_strx_concat(_tr_strz(setpre), _tr_strz(_tr_str_lit("v)")));
        /* pass */
        if ((strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("f64"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2837 = _tr_str_lit("double");
            _tr_str_release(vc);
            vc = _strtmp_t2837;
            /* pass */
            TrStr _strtmp_t2838 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("_tr_ptr_to_f64(")), _tr_strz(getraw))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(getval);
            getval = _strtmp_t2838;
            /* pass */
            TrStr _strtmp_t2839 = _tr_strx_concat(_tr_strz(setpre), _tr_strz(_tr_str_lit("_tr_f64_to_ptr(v))")));
            _tr_str_release(setval);
            setval = _strtmp_t2839;
        } else if ((strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2840 = _tr_str_lit("TrStr");
            _tr_str_release(vc);
            vc = _strtmp_t2840;
            /* pass */
            TrStr _strtmp_t2841 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("_tr_str_unbox(")), _tr_strz(getraw))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(getval);
            getval = _strtmp_t2841;
            /* pass */
            TrStr _strtmp_t2842 = _tr_strx_concat(_tr_strz(setpre), _tr_strz(_tr_str_lit("_tr_str_box(v))")));
            _tr_str_release(setval);
            setval = _strtmp_t2842;
        }
        /* pass */
        ({ TrStr _sbt_t2843 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT ")), _tr_strz(vc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(void* d, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" k){ return "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(getval)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2843); _tr_str_release(_sbt_t2843); });
        /* pass */
        ({ TrStr _sbt_t2844 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void tr__dict_")), _tr_strz(V))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_set(void* d, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" k, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" v){ "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(setval)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2844); _tr_str_release(_sbt_t2844); });
        /* pass */
        TrStr freefn = _tr_str_lit("Dict_free((Dict*)d)");
        /* pass */
        if (((strcmp(_tr_strz(kk), _tr_strz(_tr_str_lit("s"))) == 0) && (strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("str"))) == 0))) {
            /* pass */
            TrStr _strtmp_t2845 = _tr_str_lit("Dict_free_strval((Dict*)d)");
            _tr_str_release(freefn);
            freefn = _strtmp_t2845;
        } else if (((strcmp(_tr_strz(kk), _tr_strz(_tr_str_lit("i"))) == 0) && (strcmp(_tr_strz(vk), _tr_strz(_tr_str_lit("str"))) == 0))) {
            /* pass */
            TrStr _strtmp_t2846 = _tr_str_lit("_tr_idict_free_strval((TrIDict*)d)");
            _tr_str_release(freefn);
            freefn = _strtmp_t2846;
        } else if ((strcmp(_tr_strz(kk), _tr_strz(_tr_str_lit("i"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2847 = _tr_str_lit("_tr_idict_free((TrIDict*)d)");
            _tr_str_release(freefn);
            freefn = _strtmp_t2847;
        }
        /* pass */
        ({ TrStr _sbt_t2848 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("TR_EXPORT void tr__dict_")), _tr_strz(V))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(void* d){ "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(freefn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2848); _tr_str_release(_sbt_t2848); });
        _tr_str_release(V);
        _tr_str_release(kk);
        _tr_str_release(vk);
        _tr_str_release(kc);
        _tr_str_release(newfn);
        _tr_str_release(keysfn);
        _tr_str_release(getraw);
        _tr_str_release(setpre);
        _tr_str_release(vc);
        _tr_str_release(getval);
        _tr_str_release(setval);
        _tr_str_release(freefn);
    }
    /* pass */
    if (self->needs_list_ptr) {
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT long long tr__list_ptr_len(void* l){ return (long long)((List_ptr*)l)->len; }\n"));
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT void* tr__list_ptr_get(void* l, long long i){ return List_ptr_get((List_ptr*)l, i); }\n"));
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT void tr__list_ptr_free_shallow(void* l){ List_ptr_free((List_ptr*)l); }\n"));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT long long tr__box_str(TrStr s){ return (long long)(uintptr_t)_tr_str_box(s); }\n"));
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT TrStr tr__unbox_str(long long p){ return _tr_str_unbox((void*)(uintptr_t)p); }\n"));
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT long long tr__box_f64(double d){ return (long long)(uintptr_t)_tr_f64_to_ptr(d); }\n"));
        /* pass */
        StringBuilder_append(b, _tr_str_lit("TR_EXPORT double tr__unbox_f64(long long p){ return _tr_ptr_to_f64((void*)(uintptr_t)p); }\n"));
    }
    /* pass */
    List_TrStr_free(keys);
    List_TrStr_free(dk);
    return StringObj_as_str(StringBuilder_to_string(b));
}

__attribute__((hot)) TrStr CppExporter_generate_export_cpp_header(CppExporter* self, HirProgram* prog, TrStr ns) {
    /* pass */
    CppExporter_collect_export_classes(self, prog);
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* Generated by `tauraroc --export-cpp`. Call Tauraro from C++: include this,\n"));
    /* pass */
    ({ TrStr _sbt_t2849 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("   link the Tauraro library, and use the `")), _tr_strz(ns))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("::` wrappers. */\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t2849); _tr_str_release(_sbt_t2849); });
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#ifndef TAURARO_CPP_EXPORTS_H\n#define TAURARO_CPP_EXPORTS_H\n\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <string>\n#include <cstdint>\n"));
    /* pass */
    if ((_tr_dict_keys(self->list_elems)->len > 0LL)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("#include <vector>\n"));
    }
    /* pass */
    if ((_tr_dict_keys(self->dict_variants)->len > 0LL)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("#include <map>\n"));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("#include <tuple>\n"));
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("extern \"C\" {\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("typedef struct { char* data; long* rc; } TrStr;\n"));
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("typedef struct { long long data[8]; } TrTuple;\n"));
    }
    /* pass */
    List_TrStr* lk = _tr_dict_keys(self->list_elems);
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lk->len)) {
        /* pass */
        TrStr sfx = List_TrStr_get(lk, li);
        /* pass */
        li = (li + 1LL);
        /* pass */
        TrStr ec = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->list_elems, _tr_strz(sfx))));
        /* pass */
        ({ TrStr _sbt_t2850 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("long long tr__list_")), _tr_strz(sfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_len(void*); "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(void*, long long); void* tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new(void); void tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_push(void*, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); void tr__list_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(void*);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t2850); _tr_str_release(_sbt_t2850); });
        _tr_str_release(sfx);
        _tr_str_release(ec);
    }
    /* pass */
    List_TrStr* dkk = _tr_dict_keys(self->dict_variants);
    /* pass */
    long long dii = 0LL;
    /* pass */
    while ((dii < dkk->len)) {
        /* pass */
        TrStr V = List_TrStr_get(dkk, dii);
        /* pass */
        dii = (dii + 1LL);
        /* pass */
        TrStr vc = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->dict_variants, _tr_strz(V))));
        /* pass */
        TrStr kc = _tr_str_lit("long long");
        /* pass */
        if (({ TrStr _wt_t2851 = (_tr_str_wrap(_tr_str_slice(_tr_strz(V), 0LL, 1LL))); __auto_type _wr = ((strcmp(_wt_t2851.data, _tr_strz(_tr_str_lit("s"))) == 0)); _tr_str_release(_wt_t2851); _wr; })) {
            /* pass */
            TrStr _strtmp_t2852 = _tr_str_lit("TrStr");
            _tr_str_release(kc);
            kc = _strtmp_t2852;
        }
        /* pass */
        ({ TrStr _sbt_t2853 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void* tr__dict_")), _tr_strz(V))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new(void); void* tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_keys(void*); "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_get(void*, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); void tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_set(void*, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); void tr__dict_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_free(void*);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t2853); _tr_str_release(_sbt_t2853); });
        _tr_str_release(V);
        _tr_str_release(vc);
        _tr_str_release(kc);
    }
    /* pass */
    if (self->needs_list_ptr) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("long long tr__list_ptr_len(void*); void* tr__list_ptr_get(void*, long long); void tr__list_ptr_free_shallow(void*);\n"));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("long long tr__box_str(TrStr); TrStr tr__unbox_str(long long); long long tr__box_f64(double); double tr__unbox_f64(long long);\n"));
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
        /* pass */
        while ((ci < prog->classes->len)) {
            /* pass */
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
            /* pass */
            ci = (ci + 1LL);
            /* pass */
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
                /* pass */
                continue;
            }
            /* pass */
            ({ TrStr _sbt_t2854 = (CppExporter__cpp_class_bridge_protos(self, c)); StringBuilder_append(sb, _sbt_t2854); _tr_str_release(_sbt_t2854); });
        }
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
        /* pass */
        while ((i < prog->functions->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
            /* pass */
            if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
                /* pass */
                if (((f->is_export && (!f->is_extern)) && CppExporter__cpp_export_fn_ok(self, f))) {
                    /* pass */
                    if (CppExporter__cpp_fn_needs_bridge(self, f)) {
                        /* pass */
                        ({ TrStr _sbt_t2855 = (CppExporter__cpp_free_fn_bridge_proto(self, f)); StringBuilder_append(sb, _sbt_t2855); _tr_str_release(_sbt_t2855); });
                    } else {
                        /* pass */
                        TrStr sig = CGenerator_gen_func_sig(self->gen, f, _tr_str_lit(""));
                        /* pass */
                        if (_tr_str_starts_with(_tr_strz(sig), _tr_strz(_tr_str_lit("TR_EXPORT ")))) {
                            /* pass */
                            TrStr _strtmp_t2856 = _tr_str_wrap(_tr_str_slice(_tr_strz(sig), 10LL, _tr_strlen(_tr_strz(sig))));
                            _tr_str_release(sig);
                            sig = _strtmp_t2856;
                        }
                        /* pass */
                        ({ TrStr _sbt_t2857 = (_tr_strx_concat(_tr_strz(sig), _tr_strz(_tr_str_lit(";\n")))); StringBuilder_append(sb, _sbt_t2857); _tr_str_release(_sbt_t2857); });
                        _tr_str_release(sig);
                    }
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n\n"));
    /* pass */
    ({ TrStr _sbt_t2858 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("namespace ")), _tr_strz(ns))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" {\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t2858); _tr_str_release(_sbt_t2858); });
    /* pass */
    ci = 0LL;
    /* pass */
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
        /* pass */
        while ((ci < prog->classes->len)) {
            /* pass */
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
            /* pass */
            ci = (ci + 1LL);
            /* pass */
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
                /* pass */
                continue;
            }
            /* pass */
            ({ TrStr _sbt_t2859 = (CppExporter__cpp_class_decl(self, c)); StringBuilder_append(sb, _sbt_t2859); _tr_str_release(_sbt_t2859); });
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
                if (((f->is_export && (!f->is_extern)) && CppExporter__cpp_export_fn_ok(self, f))) {
                    /* pass */
                    ({ TrStr _sbt_t2860 = (CppExporter__cpp_wrap_fn(self, f)); StringBuilder_append(sb, _sbt_t2860); _tr_str_release(_sbt_t2860); });
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    ({ TrStr _sbt_t2861 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("}  // namespace ")), _tr_strz(ns))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n\n#endif\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t2861); _tr_str_release(_sbt_t2861); });
    /* pass */
    List_TrStr_free(lk);
    List_TrStr_free(dkk);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr CppExporter__cpp_class_bridge_protos(CppExporter* self, HirClass* c) {
    /* pass */
    TrStr C = c->name;
    /* pass */
    StringBuilder* b = StringBuilder_init(256LL);
    /* pass */
    ({ TrStr _sbt_t2862 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void* tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_retain(void*); void tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_release(void*);\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2862); _tr_str_release(_sbt_t2862); });
    /* pass */
    long long iidx = CppExporter__cpp_init_idx(self, c);
    /* pass */
    if ((iidx >= 0LL)) {
        /* pass */
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
        /* pass */
        ({ TrStr _sbt_t2863 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void* tr__")), _tr_strz(C))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_new("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2863); _tr_str_release(_sbt_t2863); });
        /* pass */
        long long ip = 0LL;
        /* pass */
        long long ie = 0LL;
        /* pass */
        while ((ip < im->params->len)) {
            /* pass */
            HirParam* p = ((HirParam*)List_ptr_get(im->params, ip));
            /* pass */
            ip = (ip + 1LL);
            /* pass */
            if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                /* pass */
                continue;
            }
            /* pass */
            if ((ie > 0LL)) {
                /* pass */
                StringBuilder_append(b, _tr_str_lit(", "));
            }
            /* pass */
            ({ TrStr _sbt_t2864 = (CppExporter__cpp_c_ty(self, p->ty)); StringBuilder_append(b, _sbt_t2864); _tr_str_release(_sbt_t2864); });
            /* pass */
            ie = (ie + 1LL);
        }
        /* pass */
        if ((ie == 0LL)) {
            /* pass */
            StringBuilder_append(b, _tr_str_lit("void"));
        }
        /* pass */
        StringBuilder_append(b, _tr_str_lit(");\n"));
    }
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < c->methods->len)) {
        /* pass */
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
        /* pass */
        mi = (mi + 1LL);
        /* pass */
        if (_is_invalid_ptr(((unsigned long long)(m)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (((strcmp(_tr_strz(m->name), _tr_strz(_tr_str_lit("init"))) == 0) || _tr_str_starts_with(_tr_strz(m->name), _tr_strz(_tr_str_lit("__"))))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!CppExporter__cpp_method_ok(self, m))) {
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _sbt_t2865 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(void*"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2865); _tr_str_release(_sbt_t2865); });
        /* pass */
        long long pj = 0LL;
        /* pass */
        while ((pj < m->params->len)) {
            /* pass */
            HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
            /* pass */
            pj = (pj + 1LL);
            /* pass */
            if ((strcmp(_tr_strz(mp->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                /* pass */
                continue;
            }
            /* pass */
            ({ TrStr _sbt_t2866 = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit(", ")), _cr.data); _tr_str_release(_cr); _cres; })); StringBuilder_append(b, _sbt_t2866); _tr_str_release(_sbt_t2866); });
        }
        /* pass */
        StringBuilder_append(b, _tr_str_lit(");\n"));
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(b));
}

__attribute__((hot)) TrStr CppExporter__cpp_free_fn_bridge_proto(CppExporter* self, HirFunction* f) {
    /* pass */
    StringBuilder* b = StringBuilder_init(128LL);
    /* pass */
    ({ TrStr _sbt_t2867 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" tr__"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t2867); _tr_str_release(_sbt_t2867); });
    /* pass */
    long long pi = 0LL;
    /* pass */
    long long e = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        if ((e > 0LL)) {
            /* pass */
            StringBuilder_append(b, _tr_str_lit(", "));
        }
        /* pass */
        ({ TrStr _sbt_t2868 = (CppExporter__cpp_c_ty(self, ((HirParam*)List_ptr_get(f->params, pi))->ty)); StringBuilder_append(b, _sbt_t2868); _tr_str_release(_sbt_t2868); });
        /* pass */
        e = (e + 1LL);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    if ((e == 0LL)) {
        /* pass */
        StringBuilder_append(b, _tr_str_lit("void"));
    }
    /* pass */
    StringBuilder_append(b, _tr_str_lit(");\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(b));
}

