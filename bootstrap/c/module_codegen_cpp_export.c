#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) CppExporter* CppExporter_init(CGenerator* gen) {
    /* pass */
    CppExporter* e = ((CppExporter*)_tr_obj_alloc(sizeof(CppExporter)));
    /* pass */
    CGenerator* _cltmp_t3016 = _tr_obj_retain(gen);
    _tr_obj_release(e->gen, _trdrop_CGenerator);
    e->gen = _cltmp_t3016;
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
    TrStr n = _tr_str_retain(ty->name);
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("str", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("scalar", 6LL);
    }
    /* pass */
    if ((_is_int_type(n) || _is_float_type(n))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("scalar", 6LL);
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) && CppExporter__cpp_list_ok(self, ty))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("list", 4LL);
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && CppExporter__cpp_dict_ok(self, ty))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("dict", 4LL);
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) && CppExporter__cpp_tuple_ok(self, ty))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("tuple", 5LL);
    }
    /* pass */
    if (_tr_dict_contains(self->cls_set, _tr_strz(n))) {
        /* pass */
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
    /* pass */
    _tr_str_release(n);
    return _tr_str_lit_len("no", 2LL);
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
    return ((_tr_str_eqv((ek), (_tr_str_lit_len("scalar", 6LL))) || _tr_str_eqv((ek), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((ek), (_tr_str_lit_len("class", 5LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_kind(CppExporter* self, AstType* ty) {
    /* pass */
    return CppExporter__cpp_ty_kind(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_sfx(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _at_t3017 = (CGenerator_list_elem_suffix(self->gen, (*((AstType**)List_ptr_get(ty->args, 0LL)))->name)); __auto_type _wr = (CGenerator_list_sfx(self->gen, _at_t3017)); _tr_str_release(_at_t3017); _wr; });
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_c(CppExporter* self, AstType* ty) {
    /* pass */
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
    /* pass */
    if (_tr_str_eqv((elem->name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("TrStr", 5LL);
    }
    /* pass */
    return CGenerator_type_to_c(self->gen, elem);
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
    /* pass */
    if (_tr_str_eqv((elem->name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("std::string", 11LL);
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
    return (((_is_int_type(name) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) || _is_float_type(name)) || _tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_kind(CppExporter* self, TrStr name) {
    /* pass */
    if (_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("str", 3LL);
    }
    /* pass */
    if (_is_float_type(name)) {
        /* pass */
        return _tr_str_lit_len("f64", 3LL);
    }
    /* pass */
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_c(CppExporter* self, TrStr kind) {
    /* pass */
    if (_tr_str_eqv((kind), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("TrStr", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((kind), (_tr_str_lit_len("f64", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("double", 6LL);
    }
    /* pass */
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_cpp(CppExporter* self, TrStr kind) {
    /* pass */
    if (_tr_str_eqv((kind), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("std::string", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((kind), (_tr_str_lit_len("f64", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("double", 6LL);
    }
    /* pass */
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) bool CppExporter__cpp_dict_ok(CppExporter* self, AstType* ty) {
    /* pass */
    if ((ty->args->len < 2LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr kn = _tr_str_retain((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
    /* pass */
    if (((!_tr_str_eqv((kn), (_tr_str_lit_len("str", 3LL)))) && (!_is_int_type(kn)))) {
        /* pass */
        _tr_str_release(kn);
        return false;
    }
    /* pass */
    _tr_str_release(kn);
    return CppExporter__cpp_slot_ok(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_kkind(CppExporter* self, AstType* ty) {
    /* pass */
    if (_tr_str_eqv(((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("s", 1LL);
    }
    /* pass */
    return _tr_str_lit_len("i", 1LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_vkind(CppExporter* self, AstType* ty) {
    /* pass */
    return CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_variant(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_dict_kkind(self, ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_key_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    if (_tr_str_eqv(((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("std::string", 11LL);
    }
    /* pass */
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_v_cpp(CppExporter* self, AstType* ty) {
    /* pass */
    return ({ TrStr _at_t3018 = (CppExporter__cpp_dict_vkind(self, ty)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t3018)); _tr_str_release(_at_t3018); _wr; });
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
    return ({ TrStr _at_t3019 = (CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, i)))->name)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t3019)); _tr_str_release(_at_t3019); _wr; });
}

__attribute__((hot)) void CppExporter__note_export_ty(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr n = _tr_str_retain(ty->name);
    /* pass */
    if (((((!_tr_str_eqv((n), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->gen->classes, _tr_strz(n))) && (!_tr_dict_contains(self->gen->value_types, _tr_strz(n)))) && (!_tr_dict_contains(self->gen->enums, _tr_strz(n))))) {
        /* pass */
        _tr_dict_set(self->cls_set, _tr_strz(n), true);
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            CppExporter__note_export_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        }
        /* pass */
        if (CppExporter__cpp_list_ok(self, ty)) {
            /* pass */
            if (_tr_str_eqv((CppExporter__cpp_list_elem_kind(self, ty)), (_tr_str_lit_len("class", 5LL)))) {
                /* pass */
                self->needs_list_ptr = true;
            } else {
                /* pass */
                ({ TrStr _dkt_t3020 = (CppExporter__cpp_list_sfx(self, ty)); TrStr _dvt_t3021 = (CppExporter__cpp_list_elem_c(self, ty)); _tr_dict_set(self->list_elems, _tr_strz(_dkt_t3020), _tr_str_box(_tr_str_retain(_dvt_t3021))); _tr_str_release(_dkt_t3020); _tr_str_release(_dvt_t3021); });
            }
        }
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && CppExporter__cpp_dict_ok(self, ty))) {
        /* pass */
        ({ TrStr _dkt_t3022 = (CppExporter__cpp_dict_variant(self, ty)); TrStr _at_t3023 = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _dvt_t3024 = (CppExporter__cpp_slot_c(self, _at_t3023)); _tr_dict_set(self->dict_variants, _tr_strz(_dkt_t3022), _tr_str_box(_tr_str_retain(_dvt_t3024))); _tr_str_release(_dkt_t3022); _tr_str_release(_at_t3023); _tr_str_release(_dvt_t3024); });
        /* pass */
        if (_tr_str_eqv((CppExporter__cpp_dict_kkind(self, ty)), (_tr_str_lit_len("s", 1LL)))) {
            /* pass */
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit_len("TrStr", 5LL)), _tr_str_box(_tr_str_retain(_tr_str_lit_len("TrStr", 5LL))));
        } else {
            /* pass */
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit_len("i64", 3LL)), _tr_str_box(_tr_str_retain(_tr_str_lit_len("long long", 9LL))));
        }
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) && CppExporter__cpp_tuple_ok(self, ty))) {
        /* pass */
        self->needs_tuple = true;
    }
    _tr_str_release(n);
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
    return (!_tr_str_eqv((CppExporter__cpp_ty_kind(self, ty)), (_tr_str_lit_len("no", 2LL))));
}

__attribute__((hot)) bool CppExporter__cpp_is_class_list(CppExporter* self, AstType* ty) {
    /* pass */
    return (((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) && CppExporter__cpp_list_ok(self, ty)) && _tr_str_eqv((CppExporter__cpp_list_elem_kind(self, ty)), (_tr_str_lit_len("class", 5LL))));
}

__attribute__((hot)) bool CppExporter__cpp_export_fn_ok(CppExporter* self, HirFunction* f) {
    /* pass */
    if ((!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
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
    return (((_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL))));
}

__attribute__((hot)) bool CppExporter__cpp_fn_needs_bridge(CppExporter* self, HirFunction* f) {
    /* pass */
    if (({ TrStr _at_t3025 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t3025)); _tr_str_release(_at_t3025); _wr; })) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if (({ TrStr _at_t3026 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t3026)); _tr_str_release(_at_t3026); _wr; })) {
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
    if ((!_tr_str_eqv((m->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rk = CppExporter__cpp_ty_kind(self, m->ret_ty);
    /* pass */
    if ((((!_tr_str_eqv((rk), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((rk), (_tr_str_lit_len("str", 3LL))))) && (!_tr_str_eqv((rk), (_tr_str_lit_len("scalar", 6LL)))))) {
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
        if (_tr_str_eqv((((HirParam*)List_ptr_get(m->params, i))->name), (_tr_str_lit_len("self", 4LL)))) {
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
        if (((!_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((k), (_tr_str_lit_len("scalar", 6LL)))))) {
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
    TrStr s = _tr_str_lit_len("std::tuple<", 11LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t3027 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t3027;
        }
        /* pass */
        TrStr _strtmp_t3028 = ({ TrStr _cr = (CppExporter__cpp_tuple_elem_cpp(self, ty, i)); TrStr _cres = _tr_strx_concatv((s), _cr); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t3028;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concatv((s), (_tr_str_lit_len(">", 1LL)));
}

__attribute__((hot)) TrStr CppExporter__cpp_c_ty(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (((_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL))))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("void*", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("TrTuple", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("TrStr", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    _tr_str_release(k);
    return CGenerator_type_to_c(self->gen, ty);
}

__attribute__((hot)) TrStr CppExporter__cpp_c_arg(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_sfx(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("(List_", 6LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concatv((_tr_str_lit_len("(TrMap*)", 8LL)), (name));
    }
    /* pass */
    _tr_str_release(k);
    return _tr_str_retain(name);
}

__attribute__((hot)) TrStr CppExporter__cpp_param_decl(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concatv((_tr_str_lit_len("const std::string& ", 19LL)), (name));
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("const std::vector<", 18LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">& ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("const std::map<", 15LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">& ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_tuple_type_cpp(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(k);
    return ({ TrStr _cl = (({ TrStr _cl = (CGenerator_type_to_c(self->gen, ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr CppExporter__cpp_call_arg(CppExporter* self, AstType* ty, TrStr name) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TrStr{ (char*)", 14LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".c_str(), (long*)0 }", 20LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concatv((name), (_tr_str_lit_len(".raw()", 6LL)));
    }
    /* pass */
    _tr_str_release(k);
    return _tr_str_retain(name);
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_type(CppExporter* self, AstType* ty) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("std::string", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_retain(ty->name);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("std::vector<", 12LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">", 1LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("std::map<", 9LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">", 1LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return CppExporter__cpp_tuple_type_cpp(self, ty);
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    _tr_str_release(k);
    return CGenerator_type_to_c(self->gen, ty);
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_stmt(CppExporter* self, AstType* ty, TrStr call) {
    /* pass */
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        _tr_str_release(k);
        return _tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)));
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TrStr _r = ", 11LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; return std::string(_r.data ? _r.data : \"\");", 45LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
        /* pass */
        _tr_str_release(k);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("::_wrap(", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(k);
    return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) bool CppExporter__cpp_kind_marshal(CppExporter* self, TrStr k) {
    /* pass */
    return ((_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL))));
}

__attribute__((hot)) bool CppExporter__cpp_fn_has_list(CppExporter* self, HirFunction* f) {
    /* pass */
    if (({ TrStr _at_t3029 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t3029)); _tr_str_release(_at_t3029); _wr; })) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if (({ TrStr _at_t3030 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t3030)); _tr_str_release(_at_t3030); _wr; })) {
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
    TrStr target = _tr_strx_concatv((_tr_str_lit_len("::", 2LL)), (f->name));
    /* pass */
    if (CppExporter__cpp_fn_needs_bridge(self, f)) {
        /* pass */
        TrStr _strtmp_t3031 = _tr_strx_concatv((_tr_str_lit_len("tr__", 4LL)), (f->name));
        _tr_str_release(target);
        target = _strtmp_t3031;
    }
    /* pass */
    StringBuilder* wb = StringBuilder_init(256LL);
    /* pass */
    ({ TrStr _sbt_t3032 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_type(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("    inline ", 11LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3032); _tr_str_release(_sbt_t3032); });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        TrStr pn = _tr_str_retain(p->name);
        /* pass */
        if (_tr_str_eqv((pn), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t3033 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(pn);
            pn = _strtmp_t3033;
        }
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
        }
        /* pass */
        ({ TrStr _sbt_t3034 = (CppExporter__cpp_param_decl(self, p->ty, pn)); StringBuilder_append(wb, _sbt_t3034); _tr_str_release(_sbt_t3034); });
        /* pass */
        i = (i + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    if ((!CppExporter__cpp_fn_has_list(self, f))) {
        /* pass */
        TrStr call_args = _tr_str_lit_len("", 0LL);
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < f->params->len)) {
            /* pass */
            HirParam* p2 = ((HirParam*)List_ptr_get(f->params, i));
            /* pass */
            TrStr pn2 = _tr_str_retain(p2->name);
            /* pass */
            if (_tr_str_eqv((pn2), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                TrStr _strtmp_t3035 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
                _tr_str_release(pn2);
                pn2 = _strtmp_t3035;
            }
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t3036 = _tr_strx_concatv((call_args), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(call_args);
                call_args = _strtmp_t3036;
            }
            /* pass */
            TrStr _strtmp_t3037 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p2->ty, pn2)); TrStr _cres = _tr_strx_concatv((call_args), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(call_args);
            call_args = _strtmp_t3037;
            /* pass */
            i = (i + 1LL);
            _tr_str_release(pn2);
        }
        /* pass */
        ({ TrStr _at_t3038 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((target), (_tr_str_lit_len("(", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (call_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t3039 = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_stmt(self, f->ret_ty, _at_t3038)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len(") { ", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3039); _tr_str_release(_at_t3038); _tr_str_release(_sbt_t3039); });
        /* pass */
        _tr_str_release(target);
        _tr_str_release(call_args);
        return StringObj_as_str(StringBuilder_to_string(wb));
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len(") {\n", 4LL));
    /* pass */
    TrStr call_args = _tr_str_lit_len("", 0LL);
    /* pass */
    TrStr frees = _tr_str_lit_len("", 0LL);
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p3 = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        TrStr pn3 = _tr_str_retain(p3->name);
        /* pass */
        if (_tr_str_eqv((pn3), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t3040 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(pn3);
            pn3 = _strtmp_t3040;
        }
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t3041 = _tr_strx_concatv((call_args), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(call_args);
            call_args = _strtmp_t3041;
        }
        /* pass */
        TrStr pk = CppExporter__cpp_ty_kind(self, p3->ty);
        /* pass */
        if (_tr_str_eqv((pk), (_tr_str_lit_len("list", 4LL)))) {
            /* pass */
            TrStr sfx = CppExporter__cpp_list_sfx(self, p3->ty);
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_l", 2LL)), _cr); _tr_str_release(_cr); _cres; });
            /* pass */
            TrStr econv = _tr_str_lit_len("_e", 2LL);
            /* pass */
            if (_tr_str_eqv(((*((AstType**)List_ptr_get(p3->ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3042 = _tr_str_lit_len("TrStr{ (char*)_e.c_str(), (long*)0 }", 36LL);
                _tr_str_release(econv);
                econv = _strtmp_t3042;
            }
            /* pass */
            ({ TrStr _sbt_t3043 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        void* ", 14LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = tr__list_", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new();\n", 8LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3043); _tr_str_release(_sbt_t3043); });
            /* pass */
            ({ TrStr _sbt_t3044 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        for (const auto& _e : ", 30LL)), (pn3))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") tr__list_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_push(", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (econv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3044); _tr_str_release(_sbt_t3044); });
            /* pass */
            TrStr _strtmp_t3045 = _tr_strx_concatv((call_args), (tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t3045;
            /* pass */
            TrStr _strtmp_t3046 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((frees), (_tr_str_lit_len("        tr__list_", 17LL)))); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(frees);
            frees = _strtmp_t3046;
            _tr_str_release(sfx);
            _tr_str_release(tmp);
            _tr_str_release(econv);
        } else if (_tr_str_eqv((pk), (_tr_str_lit_len("dict", 4LL)))) {
            /* pass */
            TrStr vt = CppExporter__cpp_dict_variant(self, p3->ty);
            /* pass */
            TrStr vk = CppExporter__cpp_dict_vkind(self, p3->ty);
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_d", 2LL)), _cr); _tr_str_release(_cr); _cres; });
            /* pass */
            TrStr kconv = _tr_str_lit_len("_kv.first", 9LL);
            /* pass */
            if (_tr_str_eqv((CppExporter__cpp_dict_kkind(self, p3->ty)), (_tr_str_lit_len("s", 1LL)))) {
                /* pass */
                TrStr _strtmp_t3047 = _tr_str_lit_len("TrStr{ (char*)_kv.first.c_str(), (long*)0 }", 43LL);
                _tr_str_release(kconv);
                kconv = _strtmp_t3047;
            }
            /* pass */
            TrStr vconv = _tr_str_lit_len("_kv.second", 10LL);
            /* pass */
            if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3048 = _tr_str_lit_len("TrStr{ (char*)_kv.second.c_str(), (long*)0 }", 44LL);
                _tr_str_release(vconv);
                vconv = _strtmp_t3048;
            }
            /* pass */
            ({ TrStr _sbt_t3049 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        void* ", 14LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = tr__dict_", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new();\n", 8LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3049); _tr_str_release(_sbt_t3049); });
            /* pass */
            ({ TrStr _sbt_t3050 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        for (const auto& _kv : ", 31LL)), (pn3))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") tr__dict_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_set(", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kconv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vconv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3050); _tr_str_release(_sbt_t3050); });
            /* pass */
            TrStr _strtmp_t3051 = _tr_strx_concatv((call_args), (tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t3051;
            /* pass */
            TrStr _strtmp_t3052 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((frees), (_tr_str_lit_len("        tr__dict_", 17LL)))); TrStr _cres = _tr_strx_concatv(_cl, (vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(frees);
            frees = _strtmp_t3052;
            _tr_str_release(vk);
            _tr_str_release(vt);
            _tr_str_release(tmp);
            _tr_str_release(kconv);
            _tr_str_release(vconv);
        } else if (_tr_str_eqv((pk), (_tr_str_lit_len("tuple", 5LL)))) {
            /* pass */
            TrStr tmp = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_t", 2LL)), _cr); _tr_str_release(_cr); _cres; });
            /* pass */
            ({ TrStr _sbt_t3053 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        TrTuple ", 16LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = {};\n", 7LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3053); _tr_str_release(_sbt_t3053); });
            /* pass */
            long long ti = 0LL;
            /* pass */
            while ((ti < p3->ty->args->len)) {
                /* pass */
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(p3->ty->args, ti)))->name);
                /* pass */
                TrStr g = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("std::get<", 9LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">(", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn3)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if (_tr_str_eqv((sk), (_tr_str_lit_len("str", 3LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3054 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] = tr__box_str(TrStr{ (char*)", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".c_str(), (long*)0 });\n", 23LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3054); _tr_str_release(_sbt_t3054); });
                } else if (_tr_str_eqv((sk), (_tr_str_lit_len("f64", 3LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3055 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] = tr__box_f64(", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3055); _tr_str_release(_sbt_t3055); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t3056 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] = (long long)", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (g)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3056); _tr_str_release(_sbt_t3056); });
                }
                /* pass */
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(g);
            }
            /* pass */
            TrStr _strtmp_t3057 = _tr_strx_concatv((call_args), (tmp));
            _tr_str_release(call_args);
            call_args = _strtmp_t3057;
            _tr_str_release(tmp);
        } else {
            /* pass */
            TrStr _strtmp_t3058 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p3->ty, pn3)); TrStr _cres = _tr_strx_concatv((call_args), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(call_args);
            call_args = _strtmp_t3058;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(pn3);
        _tr_str_release(pk);
    }
    /* pass */
    TrStr call_expr = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((target), (_tr_str_lit_len("(", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (call_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
    /* pass */
    TrStr rk = CppExporter__cpp_ty_kind(self, f->ret_ty);
    /* pass */
    if (_tr_str_eqv((rk), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        ({ TrStr _sbt_t3059 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (call_expr))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (frees)); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3059); _tr_str_release(_sbt_t3059); });
    } else {
        /* pass */
        ({ TrStr _sbt_t3060 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" _r = ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call_expr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (frees)); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3060); _tr_str_release(_sbt_t3060); });
        /* pass */
        if (_tr_str_eqv((rk), (_tr_str_lit_len("list", 4LL)))) {
            /* pass */
            TrStr ecpp = CppExporter__cpp_list_elem_cpp(self, f->ret_ty);
            /* pass */
            if (_tr_str_eqv((CppExporter__cpp_list_elem_kind(self, f->ret_ty)), (_tr_str_lit_len("class", 5LL)))) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len("        long long _n = tr__list_ptr_len(_r);\n", 45LL));
                /* pass */
                ({ TrStr _sbt_t3061 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        std::vector<", 20LL)), (ecpp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("> _v; _v.reserve((size_t)_n);\n", 30LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3061); _tr_str_release(_sbt_t3061); });
                /* pass */
                ({ TrStr _sbt_t3062 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(", 59LL)), (ecpp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("::_wrap((void*)tr__list_ptr_get(_r, _i)));\n", 43LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3062); _tr_str_release(_sbt_t3062); });
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len("        tr__list_ptr_free_shallow(_r);\n        return _v;\n", 58LL));
            } else {
                /* pass */
                TrStr rsfx = CppExporter__cpp_list_sfx(self, f->ret_ty);
                /* pass */
                ({ TrStr _sbt_t3063 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        long long _n = tr__list_", 32LL)), (rsfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_len(_r);\n", 10LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3063); _tr_str_release(_sbt_t3063); });
                /* pass */
                ({ TrStr _sbt_t3064 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        std::vector<", 20LL)), (ecpp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("> _v; _v.reserve((size_t)_n);\n", 30LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3064); _tr_str_release(_sbt_t3064); });
                /* pass */
                if (_tr_str_eqv(((*((AstType**)List_ptr_get(f->ret_ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3065 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) { TrStr _s = tr__list_", 68LL)), (rsfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(_r, _i); _v.push_back(std::string(_s.data ? _s.data : \"\")); }\n", 67LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3065); _tr_str_release(_sbt_t3065); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t3066 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(tr__list_", 68LL)), (rsfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(_r, _i));\n", 15LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3066); _tr_str_release(_sbt_t3066); });
                }
                /* pass */
                ({ TrStr _sbt_t3067 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        tr__list_", 17LL)), (rsfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(_r);\n        return _v;\n", 30LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3067); _tr_str_release(_sbt_t3067); });
                _tr_str_release(rsfx);
            }
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("dict", 4LL)))) {
            /* pass */
            TrStr vt = CppExporter__cpp_dict_variant(self, f->ret_ty);
            /* pass */
            TrStr vk = CppExporter__cpp_dict_vkind(self, f->ret_ty);
            /* pass */
            TrStr ksfx = _tr_str_lit_len("TrStr", 5LL);
            /* pass */
            if (_tr_str_eqv((CppExporter__cpp_dict_kkind(self, f->ret_ty)), (_tr_str_lit_len("i", 1LL)))) {
                /* pass */
                TrStr _strtmp_t3068 = _tr_str_lit_len("i64", 3LL);
                _tr_str_release(ksfx);
                ksfx = _strtmp_t3068;
            }
            /* pass */
            ({ TrStr _sbt_t3069 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        void* _ks = tr__dict_", 29LL)), (vt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_keys(_r);\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3069); _tr_str_release(_sbt_t3069); });
            /* pass */
            ({ TrStr _sbt_t3070 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        long long _n = tr__list_", 32LL)), (ksfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_len(_ks);\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3070); _tr_str_release(_sbt_t3070); });
            /* pass */
            ({ TrStr _sbt_t3071 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_dict_key_cpp(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("        std::map<", 17LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (CppExporter__cpp_dict_v_cpp(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("> _m;\n", 6LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3071); _tr_str_release(_sbt_t3071); });
            /* pass */
            StringBuilder_append(wb, _tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) {\n", 48LL));
            /* pass */
            if (_tr_str_eqv((ksfx), (_tr_str_lit_len("TrStr", 5LL)))) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len("            TrStr _k = tr__list_TrStr_get(_ks, _i); std::string _kk2(_k.data ? _k.data : \"\");\n", 94LL));
            } else {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len("            long long _k = tr__list_i64_get(_ks, _i); long long _kk2 = _k;\n", 75LL));
            }
            /* pass */
            if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
                /* pass */
                ({ TrStr _sbt_t3072 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("            TrStr _vv = tr__dict_", 33LL)), (vt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(_r, _k); _m[_kk2] = std::string(_vv.data ? _vv.data : \"\");\n", 64LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3072); _tr_str_release(_sbt_t3072); });
            } else {
                /* pass */
                ({ TrStr _sbt_t3073 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("            _m[_kk2] = tr__dict_", 32LL)), (vt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(_r, _k);\n", 14LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3073); _tr_str_release(_sbt_t3073); });
            }
            /* pass */
            StringBuilder_append(wb, _tr_str_lit_len("        }\n", 10LL));
            /* pass */
            ({ TrStr _sbt_t3074 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        tr__list_", 17LL)), (ksfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(_ks); tr__dict_", 21LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(_r);\n        return _m;\n", 30LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3074); _tr_str_release(_sbt_t3074); });
            _tr_str_release(vt);
            _tr_str_release(ksfx);
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("tuple", 5LL)))) {
            /* pass */
            TrStr parts = _tr_str_lit_len("", 0LL);
            /* pass */
            long long ti = 0LL;
            /* pass */
            while ((ti < f->ret_ty->args->len)) {
                /* pass */
                TrStr ev = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_te", 3LL)), _cr); _tr_str_release(_cr); _cres; });
                /* pass */
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(f->ret_ty->args, ti)))->name);
                /* pass */
                TrStr slot = ({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_r.data[", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if (_tr_str_eqv((sk), (_tr_str_lit_len("str", 3LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3075 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("        TrStr _ts", 17LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = tr__unbox_str(", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); std::string ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ev)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(_ts", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".data ? _ts", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".data : \"\");\n", 13LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3075); _tr_str_release(_sbt_t3075); });
                } else if (_tr_str_eqv((sk), (_tr_str_lit_len("f64", 3LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3076 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        double ", 15LL)), (ev))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = tr__unbox_f64(", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3076); _tr_str_release(_sbt_t3076); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t3077 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        long long ", 18LL)), (ev))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = (long long)", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3077); _tr_str_release(_sbt_t3077); });
                }
                /* pass */
                if ((ti > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t3078 = _tr_strx_concatv((parts), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(parts);
                    parts = _strtmp_t3078;
                }
                /* pass */
                TrStr _strtmp_t3079 = _tr_strx_concatv((parts), (ev));
                _tr_str_release(parts);
                parts = _strtmp_t3079;
                /* pass */
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(ev);
                _tr_str_release(slot);
            }
            /* pass */
            ({ TrStr _sbt_t3080 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        return std::make_tuple(", 31LL)), (parts))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3080); _tr_str_release(_sbt_t3080); });
            _tr_str_release(parts);
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit_len("        return std::string(_r.data ? _r.data : \"\");\n", 52LL));
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("class", 5LL)))) {
            /* pass */
            ({ TrStr _sbt_t3081 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        return ", 15LL)), (f->ret_ty->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("::_wrap(_r);\n", 13LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3081); _tr_str_release(_sbt_t3081); });
        } else {
            /* pass */
            StringBuilder_append(wb, _tr_str_lit_len("        return _r;\n", 19LL));
        }
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len("    }\n", 6LL));
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
    if ((!_tr_str_eqv((m->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!_tr_str_eqv((CppExporter__cpp_ty_kind(self, m->ret_ty)), (_tr_str_lit_len("class", 5LL))))) {
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
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr k = CppExporter__cpp_ty_kind(self, p->ty);
        /* pass */
        if (((!_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((k), (_tr_str_lit_len("scalar", 6LL)))))) {
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
        if ((((!_is_invalid_ptr(((unsigned long long)(m)))) && _tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL)))) && CppExporter__cpp_init_ok(self, m))) {
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
    TrStr C = _tr_str_retain(c->name);
    /* pass */
    StringBuilder* wb = StringBuilder_init(512LL);
    /* pass */
    ({ TrStr _sbt_t3082 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    class ", 10LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" {\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3082); _tr_str_release(_sbt_t3082); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len("        void* h_;\n", 18LL));
    /* pass */
    ({ TrStr _sbt_t3083 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        explicit ", 17LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(void* _p): h_(_p) {}\n", 22LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3083); _tr_str_release(_sbt_t3083); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len("    public:\n", 12LL));
    /* pass */
    ({ TrStr _sbt_t3084 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        static ", 15LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" _wrap(void* _p) { return ", 26LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(_p); }\n", 8LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3084); _tr_str_release(_sbt_t3084); });
    /* pass */
    ({ TrStr _sbt_t3085 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(const ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& o): h_(o.h_ ? tr__", 20LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_retain(o.h_) : nullptr) {}\n", 28LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3085); _tr_str_release(_sbt_t3085); });
    /* pass */
    ({ TrStr _sbt_t3086 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("&& o) noexcept : h_(o.h_) { o.h_ = nullptr; }\n", 46LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3086); _tr_str_release(_sbt_t3086); });
    /* pass */
    ({ TrStr _sbt_t3087 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& operator=(const ", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& o) { if (this != &o) { if (h_) tr__", 37LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_release(h_); h_ = o.h_ ? tr__", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_retain(o.h_) : nullptr; } return *this; }\n", 43LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3087); _tr_str_release(_sbt_t3087); });
    /* pass */
    ({ TrStr _sbt_t3088 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("& operator=(", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("&& o) noexcept { if (this != &o) { if (h_) tr__", 47LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_release(h_); h_ = o.h_; o.h_ = nullptr; } return *this; }\n", 59LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3088); _tr_str_release(_sbt_t3088); });
    /* pass */
    ({ TrStr _sbt_t3089 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        ~", 9LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("() { if (h_) tr__", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_release(h_); }\n", 16LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3089); _tr_str_release(_sbt_t3089); });
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len("        void* raw() const { return h_; }\n", 41LL));
    /* pass */
    long long iidx = CppExporter__cpp_init_idx(self, c);
    /* pass */
    if ((iidx >= 0LL)) {
        /* pass */
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
        /* pass */
        ({ TrStr _sbt_t3090 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("        explicit ", 17LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3090); _tr_str_release(_sbt_t3090); });
        /* pass */
        TrStr cargs = _tr_str_lit_len("", 0LL);
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
            if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((emitted > 0LL)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
                /* pass */
                TrStr _strtmp_t3091 = _tr_strx_concatv((cargs), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(cargs);
                cargs = _strtmp_t3091;
            }
            /* pass */
            ({ TrStr _sbt_t3092 = (CppExporter__cpp_param_decl(self, p->ty, p->name)); StringBuilder_append(wb, _sbt_t3092); _tr_str_release(_sbt_t3092); });
            /* pass */
            TrStr _strtmp_t3093 = ({ TrStr _cr = (CppExporter__cpp_call_arg(self, p->ty, p->name)); TrStr _cres = _tr_strx_concatv((cargs), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(cargs);
            cargs = _strtmp_t3093;
            /* pass */
            emitted = (emitted + 1LL);
        }
        /* pass */
        ({ TrStr _sbt_t3094 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("): h_(tr__", 10LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")) {}\n", 6LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3094); _tr_str_release(_sbt_t3094); });
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
        if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_with((m->name).data, (_tr_str_lit_len("__", 2LL)).data))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!CppExporter__cpp_method_ok(self, m))) {
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _sbt_t3095 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_type(self, m->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3095); _tr_str_release(_sbt_t3095); });
        /* pass */
        TrStr margs = _tr_str_lit_len("h_", 2LL);
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
            if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((me > 0LL)) {
                /* pass */
                StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
            }
            /* pass */
            ({ TrStr _sbt_t3096 = (CppExporter__cpp_param_decl(self, mp->ty, mp->name)); StringBuilder_append(wb, _sbt_t3096); _tr_str_release(_sbt_t3096); });
            /* pass */
            TrStr _strtmp_t3097 = ({ TrStr _cl = (_tr_strx_concatv((margs), (_tr_str_lit_len(", ", 2LL)))); TrStr _cr = (CppExporter__cpp_call_arg(self, mp->ty, mp->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(margs);
            margs = _strtmp_t3097;
            /* pass */
            me = (me + 1LL);
        }
        /* pass */
        ({ TrStr _at_t3098 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("tr__", 4LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t3099 = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_ret_stmt(self, m->ret_ty, _at_t3098)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len(") { ", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(wb, _sbt_t3099); _tr_str_release(_at_t3098); _tr_str_release(_sbt_t3099); });
        _tr_str_release(margs);
    }
    /* pass */
    StringBuilder_append(wb, _tr_str_lit_len("    };\n", 7LL));
    /* pass */
    _tr_str_release(C);
    return StringObj_as_str(StringBuilder_to_string(wb));
}

__attribute__((hot)) TrStr CppExporter_generate_cpp_class_bridges(CppExporter* self, HirProgram* prog) {
    /* pass */
    CppExporter_collect_export_classes(self, prog);
    /* pass */
    StringBuilder* b = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder_append(b, _tr_str_lit_len("\n/* --export-cpp: stable extern-\"C\" bridges over the ARC runtime (for the C++ header). */\n", 90LL));
    /* pass */
    StringBuilder_append(b, _tr_str_lit_len("#ifdef __cplusplus\nextern \"C\" {\n#endif\n", 39LL));
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
            TrStr C = _tr_str_retain(c->name);
            /* pass */
            ({ TrStr _sbt_t3100 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__", 20LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_retain(void* p){ return _tr_obj_retain(p); }\n", 46LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3100); _tr_str_release(_sbt_t3100); });
            /* pass */
            ({ TrStr _sbt_t3101 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__", 19LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_release(void* p){ _tr_obj_release(p, _trdrop_", 46LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3101); _tr_str_release(_sbt_t3101); });
            /* pass */
            long long iidx = CppExporter__cpp_init_idx(self, c);
            /* pass */
            if ((iidx >= 0LL)) {
                /* pass */
                HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
                /* pass */
                ({ TrStr _sbt_t3102 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__", 20LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3102); _tr_str_release(_sbt_t3102); });
                /* pass */
                long long ip = 0LL;
                /* pass */
                long long ie = 0LL;
                /* pass */
                TrStr iargs = _tr_str_lit_len("", 0LL);
                /* pass */
                while ((ip < im->params->len)) {
                    /* pass */
                    HirParam* p = ((HirParam*)List_ptr_get(im->params, ip));
                    /* pass */
                    ip = (ip + 1LL);
                    /* pass */
                    if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
                        /* pass */
                        continue;
                    }
                    /* pass */
                    if ((ie > 0LL)) {
                        /* pass */
                        StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
                        /* pass */
                        TrStr _strtmp_t3103 = _tr_strx_concatv((iargs), (_tr_str_lit_len(", ", 2LL)));
                        _tr_str_release(iargs);
                        iargs = _strtmp_t3103;
                    }
                    /* pass */
                    ({ TrStr _sbt_t3104 = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (p->name)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3104); _tr_str_release(_sbt_t3104); });
                    /* pass */
                    TrStr _strtmp_t3105 = _tr_strx_concatv((iargs), (p->name));
                    _tr_str_release(iargs);
                    iargs = _strtmp_t3105;
                    /* pass */
                    ie = (ie + 1LL);
                }
                /* pass */
                ({ TrStr _sbt_t3106 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("){ return (void*)", 17LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_init(", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (iargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3106); _tr_str_release(_sbt_t3106); });
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
                if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_with((m->name).data, (_tr_str_lit_len("__", 2LL)).data))) {
                    /* pass */
                    continue;
                }
                /* pass */
                if ((!CppExporter__cpp_method_ok(self, m))) {
                    /* pass */
                    continue;
                }
                /* pass */
                ({ TrStr _sbt_t3107 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(void* s", 8LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3107); _tr_str_release(_sbt_t3107); });
                /* pass */
                TrStr margs = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)s", 3LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                long long pj = 0LL;
                /* pass */
                while ((pj < m->params->len)) {
                    /* pass */
                    HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
                    /* pass */
                    pj = (pj + 1LL);
                    /* pass */
                    if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
                        /* pass */
                        continue;
                    }
                    /* pass */
                    ({ TrStr _sbt_t3108 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len(", ", 2LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (mp->name)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3108); _tr_str_release(_sbt_t3108); });
                    /* pass */
                    TrStr _strtmp_t3109 = ({ TrStr _cl = (_tr_strx_concatv((margs), (_tr_str_lit_len(", ", 2LL)))); TrStr _cr = (CppExporter__cpp_c_arg(self, mp->ty, mp->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(margs);
                    margs = _strtmp_t3109;
                }
                /* pass */
                StringBuilder_append(b, _tr_str_lit_len("){ ", 3LL));
                /* pass */
                if (_tr_str_eqv((CppExporter__cpp_ty_kind(self, m->ret_ty)), (_tr_str_lit_len("void", 4LL)))) {
                    /* pass */
                    ({ TrStr _sbt_t3110 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((C), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3110); _tr_str_release(_sbt_t3110); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t3111 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (margs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3111); _tr_str_release(_sbt_t3111); });
                }
                _tr_str_release(margs);
            }
            _tr_str_release(C);
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
            ({ TrStr _sbt_t3112 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3112); _tr_str_release(_sbt_t3112); });
            /* pass */
            TrStr fargs = _tr_str_lit_len("", 0LL);
            /* pass */
            long long pi2 = 0LL;
            /* pass */
            while ((pi2 < f->params->len)) {
                /* pass */
                HirParam* p = ((HirParam*)List_ptr_get(f->params, pi2));
                /* pass */
                TrStr pn = _tr_str_retain(p->name);
                /* pass */
                if (_tr_str_eqv((pn), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    TrStr _strtmp_t3113 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi2)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
                    _tr_str_release(pn);
                    pn = _strtmp_t3113;
                }
                /* pass */
                if ((pi2 > 0LL)) {
                    /* pass */
                    StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
                    /* pass */
                    TrStr _strtmp_t3114 = _tr_strx_concatv((fargs), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(fargs);
                    fargs = _strtmp_t3114;
                }
                /* pass */
                ({ TrStr _sbt_t3115 = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3115); _tr_str_release(_sbt_t3115); });
                /* pass */
                TrStr _strtmp_t3116 = ({ TrStr _cr = (CppExporter__cpp_c_arg(self, p->ty, pn)); TrStr _cres = _tr_strx_concatv((fargs), _cr); _tr_str_release(_cr); _cres; });
                _tr_str_release(fargs);
                fargs = _strtmp_t3116;
                /* pass */
                pi2 = (pi2 + 1LL);
                _tr_str_release(pn);
            }
            /* pass */
            StringBuilder_append(b, _tr_str_lit_len("){ ", 3LL));
            /* pass */
            TrStr frk = CppExporter__cpp_ty_kind(self, f->ret_ty);
            /* pass */
            if (_tr_str_eqv((frk), (_tr_str_lit_len("void", 4LL)))) {
                /* pass */
                ({ TrStr _sbt_t3117 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((f->name), (_tr_str_lit_len("(", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3117); _tr_str_release(_sbt_t3117); });
            } else if (((_tr_str_eqv((frk), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((frk), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((frk), (_tr_str_lit_len("dict", 4LL))))) {
                /* pass */
                ({ TrStr _sbt_t3118 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (void*)", 14LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3118); _tr_str_release(_sbt_t3118); });
            } else {
                /* pass */
                ({ TrStr _sbt_t3119 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3119); _tr_str_release(_sbt_t3119); });
            }
            _tr_str_release(fargs);
            _tr_str_release(frk);
        }
    }
    /* pass */
    ({ TrStr _sbt_t3120 = (CppExporter__cpp_list_bridges(self)); StringBuilder_append(b, _sbt_t3120); _tr_str_release(_sbt_t3120); });
    /* pass */
    StringBuilder_append(b, _tr_str_lit_len("#ifdef __cplusplus\n}\n#endif\n", 28LL));
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
        TrStr L = _tr_strx_concatv((_tr_str_lit_len("List_", 5LL)), (sfx));
        /* pass */
        ({ TrStr _sbt_t3121 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT long long tr__list_", 29LL)), (sfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_len(void* l){ return (long long)((", 35LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)l)->len; }\n", 13LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3121); _tr_str_release(_sbt_t3121); });
        /* pass */
        ({ TrStr _sbt_t3122 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), (ec))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__list_", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(void* l, long long i){ return ", 35LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get((", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)l, i); }\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3122); _tr_str_release(_sbt_t3122); });
        /* pass */
        ({ TrStr _sbt_t3123 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__list_", 25LL)), (sfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(void){ return (void*)", 26LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(); }\n", 10LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3123); _tr_str_release(_sbt_t3123); });
        /* pass */
        ({ TrStr _sbt_t3124 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__list_", 24LL)), (sfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_push(void* l, ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" v){ ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_append((", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)l, v); }\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3124); _tr_str_release(_sbt_t3124); });
        /* pass */
        ({ TrStr _sbt_t3125 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__list_", 24LL)), (sfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(void* l){ ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free((", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (L)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)l); }\n", 8LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3125); _tr_str_release(_sbt_t3125); });
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
        TrStr kk = _tr_str_wrap(_tr_str_slice((V).data, 0LL, 1LL));
        /* pass */
        TrStr vk = _tr_str_wrap(_tr_str_slice((V).data, 2LL, _tr_str_lenv((V))));
        /* pass */
        TrStr kc = _tr_str_lit_len("long long", 9LL);
        /* pass */
        TrStr newfn = _tr_str_lit_len("_tr_idict_new(8)", 16LL);
        /* pass */
        TrStr keysfn = _tr_str_lit_len("_tr_idict_keys((TrIDict*)d)", 27LL);
        /* pass */
        TrStr getraw = _tr_str_lit_len("_tr_idict_get((TrIDict*)d, k)", 29LL);
        /* pass */
        TrStr setpre = _tr_str_lit_len("_tr_idict_set((TrIDict*)d, k, ", 30LL);
        /* pass */
        if (_tr_str_eqv((kk), (_tr_str_lit_len("s", 1LL)))) {
            /* pass */
            TrStr _strtmp_t3126 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(kc);
            kc = _strtmp_t3126;
            /* pass */
            TrStr _strtmp_t3127 = _tr_str_lit_len("_tr_dict_new(8)", 15LL);
            _tr_str_release(newfn);
            newfn = _strtmp_t3127;
            /* pass */
            TrStr _strtmp_t3128 = _tr_str_lit_len("_tr_dict_keys((TrMap*)d)", 24LL);
            _tr_str_release(keysfn);
            keysfn = _strtmp_t3128;
            /* pass */
            TrStr _strtmp_t3129 = _tr_str_lit_len("_tr_dict_get((TrMap*)d, _tr_strz(k))", 36LL);
            _tr_str_release(getraw);
            getraw = _strtmp_t3129;
            /* pass */
            TrStr _strtmp_t3130 = _tr_str_lit_len("_tr_dict_set((TrMap*)d, _tr_strz(k), ", 37LL);
            _tr_str_release(setpre);
            setpre = _strtmp_t3130;
        }
        /* pass */
        ({ TrStr _sbt_t3131 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__dict_", 25LL)), (V))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(void){ return (void*)", 26LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (newfn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3131); _tr_str_release(_sbt_t3131); });
        /* pass */
        ({ TrStr _sbt_t3132 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__dict_", 25LL)), (V))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_keys(void* d){ return (void*)", 30LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (keysfn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3132); _tr_str_release(_sbt_t3132); });
        /* pass */
        TrStr vc = _tr_str_lit_len("long long", 9LL);
        /* pass */
        TrStr getval = _tr_strx_concatv((_tr_str_lit_len("(long long)(uintptr_t)", 22LL)), (getraw));
        /* pass */
        TrStr setval = _tr_strx_concatv((setpre), (_tr_str_lit_len("v)", 2LL)));
        /* pass */
        if (_tr_str_eqv((vk), (_tr_str_lit_len("f64", 3LL)))) {
            /* pass */
            TrStr _strtmp_t3133 = _tr_str_lit_len("double", 6LL);
            _tr_str_release(vc);
            vc = _strtmp_t3133;
            /* pass */
            TrStr _strtmp_t3134 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("_tr_ptr_to_f64(", 15LL)), (getraw))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(getval);
            getval = _strtmp_t3134;
            /* pass */
            TrStr _strtmp_t3135 = _tr_strx_concatv((setpre), (_tr_str_lit_len("_tr_f64_to_ptr(v))", 18LL)));
            _tr_str_release(setval);
            setval = _strtmp_t3135;
        } else if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
            /* pass */
            TrStr _strtmp_t3136 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(vc);
            vc = _strtmp_t3136;
            /* pass */
            TrStr _strtmp_t3137 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("_tr_str_unbox(", 14LL)), (getraw))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(getval);
            getval = _strtmp_t3137;
            /* pass */
            TrStr _strtmp_t3138 = _tr_strx_concatv((setpre), (_tr_str_lit_len("_tr_str_box(v))", 15LL)));
            _tr_str_release(setval);
            setval = _strtmp_t3138;
        }
        /* pass */
        ({ TrStr _sbt_t3139 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), (vc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__dict_", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(void* d, ", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" k){ return ", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (getval)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3139); _tr_str_release(_sbt_t3139); });
        /* pass */
        ({ TrStr _sbt_t3140 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__dict_", 24LL)), (V))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_set(void* d, ", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" k, ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" v){ ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (setval)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3140); _tr_str_release(_sbt_t3140); });
        /* pass */
        TrStr freefn = _tr_str_lit_len("Dict_free((Dict*)d)", 19LL);
        /* pass */
        if ((_tr_str_eqv((kk), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL))))) {
            /* pass */
            TrStr _strtmp_t3141 = _tr_str_lit_len("Dict_free_strval((Dict*)d)", 26LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t3141;
        } else if ((_tr_str_eqv((kk), (_tr_str_lit_len("i", 1LL))) && _tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL))))) {
            /* pass */
            TrStr _strtmp_t3142 = _tr_str_lit_len("_tr_idict_free_strval((TrIDict*)d)", 34LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t3142;
        } else if (_tr_str_eqv((kk), (_tr_str_lit_len("i", 1LL)))) {
            /* pass */
            TrStr _strtmp_t3143 = _tr_str_lit_len("_tr_idict_free((TrIDict*)d)", 27LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t3143;
        }
        /* pass */
        ({ TrStr _sbt_t3144 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__dict_", 24LL)), (V))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(void* d){ ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (freefn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3144); _tr_str_release(_sbt_t3144); });
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
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__list_ptr_len(void* l){ return (long long)((List_ptr*)l)->len; }\n", 88LL));
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT void* tr__list_ptr_get(void* l, long long i){ return List_ptr_get((List_ptr*)l, i); }\n", 96LL));
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT void tr__list_ptr_free_shallow(void* l){ List_ptr_free((List_ptr*)l); }\n", 82LL));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__box_str(TrStr s){ return (long long)(uintptr_t)_tr_str_box(s); }\n", 89LL));
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT TrStr tr__unbox_str(long long p){ return _tr_str_unbox((void*)(uintptr_t)p); }\n", 89LL));
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__box_f64(double d){ return (long long)(uintptr_t)_tr_f64_to_ptr(d); }\n", 93LL));
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT double tr__unbox_f64(long long p){ return _tr_ptr_to_f64((void*)(uintptr_t)p); }\n", 91LL));
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
    StringBuilder_append(sb, _tr_str_lit_len("/* Generated by `tauraroc --export-cpp`. Call Tauraro from C++: include this,\n", 78LL));
    /* pass */
    ({ TrStr _sbt_t3145 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("   link the Tauraro library, and use the `", 42LL)), (ns))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("::` wrappers. */\n", 17LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3145); _tr_str_release(_sbt_t3145); });
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#ifndef TAURARO_CPP_EXPORTS_H\n#define TAURARO_CPP_EXPORTS_H\n\n", 61LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#include <string>\n#include <cstdint>\n", 37LL));
    /* pass */
    if ((_tr_dict_keys(self->list_elems)->len > 0LL)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("#include <vector>\n", 18LL));
    }
    /* pass */
    if ((_tr_dict_keys(self->dict_variants)->len > 0LL)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("#include <map>\n", 15LL));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("#include <tuple>\n", 17LL));
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("extern \"C\" {\n", 13LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("typedef struct { char* data; long* rc; } TrStr;\n", 48LL));
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("typedef struct { long long data[8]; } TrTuple;\n", 47LL));
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
        ({ TrStr _sbt_t3146 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("long long tr__list_", 19LL)), (sfx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_len(void*); ", 13LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__list_", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(void*, long long); void* tr__list_", 39LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(void); void tr__list_", 26LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_push(void*, ", 13LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ec)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); void tr__list_", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sfx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(void*);\n", 14LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3146); _tr_str_release(_sbt_t3146); });
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
        TrStr kc = _tr_str_lit_len("long long", 9LL);
        /* pass */
        if (({ TrStr _wt_t3147 = (_tr_str_wrap(_tr_str_slice((V).data, 0LL, 1LL))); __auto_type _wr = (_tr_str_eqv(_wt_t3147, (_tr_str_lit_len("s", 1LL)))); _tr_str_release(_wt_t3147); _wr; })) {
            /* pass */
            TrStr _strtmp_t3148 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(kc);
            kc = _strtmp_t3148;
        }
        /* pass */
        ({ TrStr _sbt_t3149 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__dict_", 15LL)), (V))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(void); void* tr__dict_", 27LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_keys(void*); ", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__dict_", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_get(void*, ", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); void tr__dict_", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_set(void*, ", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); void tr__dict_", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (V)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_free(void*);\n", 14LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3149); _tr_str_release(_sbt_t3149); });
        _tr_str_release(V);
        _tr_str_release(vc);
        _tr_str_release(kc);
    }
    /* pass */
    if (self->needs_list_ptr) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("long long tr__list_ptr_len(void*); void* tr__list_ptr_get(void*, long long); void tr__list_ptr_free_shallow(void*);\n", 116LL));
    }
    /* pass */
    if (self->needs_tuple) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("long long tr__box_str(TrStr); TrStr tr__unbox_str(long long); long long tr__box_f64(double); double tr__unbox_f64(long long);\n", 126LL));
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
            ({ TrStr _sbt_t3150 = (CppExporter__cpp_class_bridge_protos(self, c)); StringBuilder_append(sb, _sbt_t3150); _tr_str_release(_sbt_t3150); });
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
                        ({ TrStr _sbt_t3151 = (CppExporter__cpp_free_fn_bridge_proto(self, f)); StringBuilder_append(sb, _sbt_t3151); _tr_str_release(_sbt_t3151); });
                    } else {
                        /* pass */
                        TrStr sig = CGenerator_gen_func_sig(self->gen, f, _tr_str_lit_len("", 0LL));
                        /* pass */
                        if (_tr_str_starts_with((sig).data, (_tr_str_lit_len("TR_EXPORT ", 10LL)).data)) {
                            /* pass */
                            TrStr _strtmp_t3152 = _tr_str_wrap(_tr_str_slice((sig).data, 10LL, _tr_str_lenv((sig))));
                            _tr_str_release(sig);
                            sig = _strtmp_t3152;
                        }
                        /* pass */
                        ({ TrStr _sbt_t3153 = (_tr_strx_concatv((sig), (_tr_str_lit_len(";\n", 2LL)))); StringBuilder_append(sb, _sbt_t3153); _tr_str_release(_sbt_t3153); });
                        _tr_str_release(sig);
                    }
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n\n", 3LL));
    /* pass */
    ({ TrStr _sbt_t3154 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("namespace ", 10LL)), (ns))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" {\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3154); _tr_str_release(_sbt_t3154); });
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
            ({ TrStr _sbt_t3155 = (CppExporter__cpp_class_decl(self, c)); StringBuilder_append(sb, _sbt_t3155); _tr_str_release(_sbt_t3155); });
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
                    ({ TrStr _sbt_t3156 = (CppExporter__cpp_wrap_fn(self, f)); StringBuilder_append(sb, _sbt_t3156); _tr_str_release(_sbt_t3156); });
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    ({ TrStr _sbt_t3157 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("}  // namespace ", 16LL)), (ns))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n\n#endif\n", 9LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3157); _tr_str_release(_sbt_t3157); });
    /* pass */
    List_TrStr_free(lk);
    List_TrStr_free(dkk);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr CppExporter__cpp_class_bridge_protos(CppExporter* self, HirClass* c) {
    /* pass */
    TrStr C = _tr_str_retain(c->name);
    /* pass */
    StringBuilder* b = StringBuilder_init(256LL);
    /* pass */
    ({ TrStr _sbt_t3158 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__", 10LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_retain(void*); void tr__", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_release(void*);\n", 17LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3158); _tr_str_release(_sbt_t3158); });
    /* pass */
    long long iidx = CppExporter__cpp_init_idx(self, c);
    /* pass */
    if ((iidx >= 0LL)) {
        /* pass */
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
        /* pass */
        ({ TrStr _sbt_t3159 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__", 10LL)), (C))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3159); _tr_str_release(_sbt_t3159); });
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
            if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
                /* pass */
                continue;
            }
            /* pass */
            if ((ie > 0LL)) {
                /* pass */
                StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
            }
            /* pass */
            ({ TrStr _sbt_t3160 = (CppExporter__cpp_c_ty(self, p->ty)); StringBuilder_append(b, _sbt_t3160); _tr_str_release(_sbt_t3160); });
            /* pass */
            ie = (ie + 1LL);
        }
        /* pass */
        if ((ie == 0LL)) {
            /* pass */
            StringBuilder_append(b, _tr_str_lit_len("void", 4LL));
        }
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
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
        if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_with((m->name).data, (_tr_str_lit_len("__", 2LL)).data))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!CppExporter__cpp_method_ok(self, m))) {
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _sbt_t3161 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (C)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(void*", 6LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3161); _tr_str_release(_sbt_t3161); });
        /* pass */
        long long pj = 0LL;
        /* pass */
        while ((pj < m->params->len)) {
            /* pass */
            HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
            /* pass */
            pj = (pj + 1LL);
            /* pass */
            if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
                /* pass */
                continue;
            }
            /* pass */
            ({ TrStr _sbt_t3162 = (({ TrStr _cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len(", ", 2LL)), _cr); _tr_str_release(_cr); _cres; })); StringBuilder_append(b, _sbt_t3162); _tr_str_release(_sbt_t3162); });
        }
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
    }
    /* pass */
    _tr_str_release(C);
    return StringObj_as_str(StringBuilder_to_string(b));
}

__attribute__((hot)) TrStr CppExporter__cpp_free_fn_bridge_proto(CppExporter* self, HirFunction* f) {
    /* pass */
    StringBuilder* b = StringBuilder_init(128LL);
    /* pass */
    ({ TrStr _sbt_t3163 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(b, _sbt_t3163); _tr_str_release(_sbt_t3163); });
    /* pass */
    long long pi = 0LL;
    /* pass */
    long long e = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        if ((e > 0LL)) {
            /* pass */
            StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
        }
        /* pass */
        ({ TrStr _sbt_t3164 = (CppExporter__cpp_c_ty(self, ((HirParam*)List_ptr_get(f->params, pi))->ty)); StringBuilder_append(b, _sbt_t3164); _tr_str_release(_sbt_t3164); });
        /* pass */
        e = (e + 1LL);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    if ((e == 0LL)) {
        /* pass */
        StringBuilder_append(b, _tr_str_lit_len("void", 4LL));
    }
    /* pass */
    StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(b));
}

