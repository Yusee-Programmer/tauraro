#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) CppExporter* CppExporter_init(CGenerator* gen) {
#line 35 "src/codegen/cpp_export.tr"
    CppExporter* e = ((CppExporter*)_tr_obj_alloc(sizeof(CppExporter)));
#line 36 "src/codegen/cpp_export.tr"
    CGenerator* _cltmp_t6454 = _tr_obj_retain(gen);
    _tr_obj_release(e->gen, _trdrop_CGenerator);
    e->gen = _cltmp_t6454;
#line 37 "src/codegen/cpp_export.tr"
    e->cls_set = _tr_dict_new(16LL);
#line 38 "src/codegen/cpp_export.tr"
    e->list_elems = _tr_dict_new(8LL);
#line 39 "src/codegen/cpp_export.tr"
    e->dict_variants = _tr_dict_new(8LL);
#line 40 "src/codegen/cpp_export.tr"
    e->needs_tuple = false;
#line 41 "src/codegen/cpp_export.tr"
    e->needs_list_ptr = false;
#line 42 "src/codegen/cpp_export.tr"
    return e;
}

__attribute__((hot)) TrStr CppExporter__cpp_ty_kind(CppExporter* self, AstType* ty) {
#line 50 "src/codegen/cpp_export.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 51 "src/codegen/cpp_export.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))))) {
#line 51 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("void", 4LL);
    }
#line 52 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL)))) {
#line 52 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("str", 3LL);
    }
#line 53 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 53 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("scalar", 6LL);
    }
#line 54 "src/codegen/cpp_export.tr"
    if ((_is_int_type(n) || _is_float_type(n))) {
#line 54 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("scalar", 6LL);
    }
#line 55 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) && CppExporter__cpp_list_ok(self, ty))) {
#line 55 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("list", 4LL);
    }
#line 56 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && CppExporter__cpp_dict_ok(self, ty))) {
#line 56 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("dict", 4LL);
    }
#line 57 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) && CppExporter__cpp_tuple_ok(self, ty))) {
#line 57 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("tuple", 5LL);
    }
#line 58 "src/codegen/cpp_export.tr"
    if (_tr_dict_contains(self->cls_set, _tr_strz(n))) {
#line 58 "src/codegen/cpp_export.tr"
        _tr_str_release(n);
        return _tr_str_lit_len("class", 5LL);
    }
#line 59 "src/codegen/cpp_export.tr"
    _tr_str_release(n);
    return _tr_str_lit_len("no", 2LL);
}

__attribute__((hot)) bool CppExporter__cpp_list_ok(CppExporter* self, AstType* ty) {
#line 64 "src/codegen/cpp_export.tr"
    if ((ty->args->len == 0LL)) {
#line 64 "src/codegen/cpp_export.tr"
        return false;
    }
#line 65 "src/codegen/cpp_export.tr"
    TrStr ek = CppExporter__cpp_ty_kind(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 66 "src/codegen/cpp_export.tr"
    bool _trret1516 = ((_tr_str_eqv((ek), (_tr_str_lit_len("scalar", 6LL))) || _tr_str_eqv((ek), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((ek), (_tr_str_lit_len("class", 5LL))));
    _tr_str_release(ek);
    return _trret1516;
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_kind(CppExporter* self, AstType* ty) {
#line 68 "src/codegen/cpp_export.tr"
    return CppExporter__cpp_ty_kind(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_sfx(CppExporter* self, AstType* ty) {
#line 70 "src/codegen/cpp_export.tr"
    return ({ __auto_type _ord_t6455 = ((*((AstType**)List_ptr_get(ty->args, 0LL)))->name); TrStr _ord_t6456 = (CGenerator_list_elem_suffix(self->gen, _ord_t6455)); __auto_type _wr = (CGenerator_list_sfx(self->gen, _ord_t6456)); _tr_str_release(_ord_t6456); _wr; });
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_c(CppExporter* self, AstType* ty) {
#line 72 "src/codegen/cpp_export.tr"
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 73 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((elem->name), (_tr_str_lit_len("str", 3LL)))) {
#line 73 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("TrStr", 5LL);
    }
#line 74 "src/codegen/cpp_export.tr"
    TrStr _trret1517 = CGenerator_type_to_c(self->gen, elem);
    return _trret1517;
}

__attribute__((hot)) TrStr CppExporter__cpp_list_elem_cpp(CppExporter* self, AstType* ty) {
#line 76 "src/codegen/cpp_export.tr"
    AstType* elem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 77 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((elem->name), (_tr_str_lit_len("str", 3LL)))) {
#line 77 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("std::string", 11LL);
    }
#line 78 "src/codegen/cpp_export.tr"
    if (_tr_dict_contains(self->cls_set, _tr_strz(elem->name))) {
#line 78 "src/codegen/cpp_export.tr"
        TrStr _trret1518 = _tr_str_retain(elem->name);
        return _trret1518;
    }
#line 79 "src/codegen/cpp_export.tr"
    TrStr _trret1519 = CGenerator_type_to_c(self->gen, elem);
    return _trret1519;
}

__attribute__((hot)) bool CppExporter__cpp_slot_ok(CppExporter* self, TrStr name) {
#line 84 "src/codegen/cpp_export.tr"
    return (((_is_int_type(name) || _tr_str_eqv((name), (_tr_str_lit_len("bool", 4LL)))) || _is_float_type(name)) || _tr_str_eqv((name), (_tr_str_lit_len("str", 3LL))));
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_kind(CppExporter* self, TrStr name) {
#line 87 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("str", 3LL)))) {
#line 87 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("str", 3LL);
    }
#line 88 "src/codegen/cpp_export.tr"
    if (_is_float_type(name)) {
#line 88 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("f64", 3LL);
    }
#line 89 "src/codegen/cpp_export.tr"
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_c(CppExporter* self, TrStr kind) {
#line 91 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((kind), (_tr_str_lit_len("str", 3LL)))) {
#line 91 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("TrStr", 5LL);
    }
#line 92 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((kind), (_tr_str_lit_len("f64", 3LL)))) {
#line 92 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 93 "src/codegen/cpp_export.tr"
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_slot_cpp(CppExporter* self, TrStr kind) {
#line 95 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((kind), (_tr_str_lit_len("str", 3LL)))) {
#line 95 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("std::string", 11LL);
    }
#line 96 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((kind), (_tr_str_lit_len("f64", 3LL)))) {
#line 96 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 97 "src/codegen/cpp_export.tr"
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) bool CppExporter__cpp_dict_ok(CppExporter* self, AstType* ty) {
#line 102 "src/codegen/cpp_export.tr"
    if ((ty->args->len < 2LL)) {
#line 102 "src/codegen/cpp_export.tr"
        return false;
    }
#line 103 "src/codegen/cpp_export.tr"
    TrStr kn = _tr_str_retain((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
#line 104 "src/codegen/cpp_export.tr"
    if (((!_tr_str_eqv((kn), (_tr_str_lit_len("str", 3LL)))) && (!_is_int_type(kn)))) {
#line 104 "src/codegen/cpp_export.tr"
        _tr_str_release(kn);
        return false;
    }
#line 105 "src/codegen/cpp_export.tr"
    bool _trret1520 = CppExporter__cpp_slot_ok(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
    _tr_str_release(kn);
    return _trret1520;
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_kkind(CppExporter* self, AstType* ty) {
#line 107 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv(((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
#line 107 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("s", 1LL);
    }
#line 108 "src/codegen/cpp_export.tr"
    return _tr_str_lit_len("i", 1LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_vkind(CppExporter* self, AstType* ty) {
#line 110 "src/codegen/cpp_export.tr"
    return CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, 1LL)))->name);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_variant(CppExporter* self, AstType* ty) {
#line 112 "src/codegen/cpp_export.tr"
    return ({ __auto_type _ord_t6457 = (({ TrStr _tr_cl = (CppExporter__cpp_dict_kkind(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6457); TrStr _tr_cr = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_key_cpp(CppExporter* self, AstType* ty) {
#line 114 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv(((*((AstType**)List_ptr_get(ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
#line 114 "src/codegen/cpp_export.tr"
        return _tr_str_lit_len("std::string", 11LL);
    }
#line 115 "src/codegen/cpp_export.tr"
    return _tr_str_lit_len("long long", 9LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_dict_v_cpp(CppExporter* self, AstType* ty) {
#line 117 "src/codegen/cpp_export.tr"
    return ({ TrStr _at_t6458 = (CppExporter__cpp_dict_vkind(self, ty)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t6458)); _tr_str_release(_at_t6458); _wr; });
}

__attribute__((hot)) bool CppExporter__cpp_tuple_ok(CppExporter* self, AstType* ty) {
#line 121 "src/codegen/cpp_export.tr"
    if ((ty->args->len == 0LL)) {
#line 121 "src/codegen/cpp_export.tr"
        return false;
    }
#line 122 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 123 "src/codegen/cpp_export.tr"
    while ((i < ty->args->len)) {
#line 124 "src/codegen/cpp_export.tr"
        if ((!CppExporter__cpp_slot_ok(self, (*((AstType**)List_ptr_get(ty->args, i)))->name))) {
#line 124 "src/codegen/cpp_export.tr"
            return false;
        }
#line 125 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 126 "src/codegen/cpp_export.tr"
    return true;
}

__attribute__((hot)) TrStr CppExporter__cpp_tuple_elem_cpp(CppExporter* self, AstType* ty, long long i) {
#line 128 "src/codegen/cpp_export.tr"
    return ({ TrStr _at_t6459 = (CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(ty->args, i)))->name)); __auto_type _wr = (CppExporter__cpp_slot_cpp(self, _at_t6459)); _tr_str_release(_at_t6459); _wr; });
}

__attribute__((hot)) void CppExporter__note_export_ty(CppExporter* self, AstType* ty) {
#line 133 "src/codegen/cpp_export.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 134 "src/codegen/cpp_export.tr"
    if (((((!_tr_str_eqv((n), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(self->gen->classes, _tr_strz(n))) && (!_tr_dict_contains(self->gen->value_types, _tr_strz(n)))) && (!_tr_dict_contains(self->gen->enums, _tr_strz(n))))) {
#line 135 "src/codegen/cpp_export.tr"
        _tr_dict_set(self->cls_set, _tr_strz(n), true);
    }
#line 136 "src/codegen/cpp_export.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 137 "src/codegen/cpp_export.tr"
        if ((ty->args->len > 0LL)) {
#line 137 "src/codegen/cpp_export.tr"
            CppExporter__note_export_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        }
#line 138 "src/codegen/cpp_export.tr"
        if (CppExporter__cpp_list_ok(self, ty)) {
#line 139 "src/codegen/cpp_export.tr"
            if (({ TrStr _cmpa_t6460 = (CppExporter__cpp_list_elem_kind(self, ty)); TrStr _cmpb_t6461 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t6462 = _tr_str_eqv(_cmpa_t6460, _cmpb_t6461); _tr_str_release(_cmpa_t6460); _cmpr_t6462; })) {
#line 140 "src/codegen/cpp_export.tr"
                self->needs_list_ptr = true;
            } else {
#line 142 "src/codegen/cpp_export.tr"
                ({ TrStr _ord_t6463 = (CppExporter__cpp_list_sfx(self, ty)); TrStr _ord_t6464 = (CppExporter__cpp_list_elem_c(self, ty)); _tr_dict_set(self->list_elems, _tr_strz(_ord_t6463), _tr_str_box(_tr_str_retain(_ord_t6464))); _tr_str_release(_ord_t6463); _tr_str_release(_ord_t6464); });
            }
        }
    }
#line 143 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) && CppExporter__cpp_dict_ok(self, ty))) {
#line 144 "src/codegen/cpp_export.tr"
        ({ TrStr _ord_t6465 = (CppExporter__cpp_dict_variant(self, ty)); TrStr _at_t6466 = (CppExporter__cpp_dict_vkind(self, ty)); TrStr _ord_t6467 = (CppExporter__cpp_slot_c(self, _at_t6466)); _tr_dict_set(self->dict_variants, _tr_strz(_ord_t6465), _tr_str_box(_tr_str_retain(_ord_t6467))); _tr_str_release(_ord_t6465); _tr_str_release(_at_t6466); _tr_str_release(_ord_t6467); });
#line 145 "src/codegen/cpp_export.tr"
        if (({ TrStr _cmpa_t6468 = (CppExporter__cpp_dict_kkind(self, ty)); TrStr _cmpb_t6469 = (_tr_str_lit_len("s", 1LL)); _Bool _cmpr_t6470 = _tr_str_eqv(_cmpa_t6468, _cmpb_t6469); _tr_str_release(_cmpa_t6468); _cmpr_t6470; })) {
#line 146 "src/codegen/cpp_export.tr"
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit_len("TrStr", 5LL)), _tr_str_box(_tr_str_retain(_tr_str_lit_len("TrStr", 5LL))));
        } else {
#line 148 "src/codegen/cpp_export.tr"
            _tr_dict_set(self->list_elems, _tr_strz(_tr_str_lit_len("i64", 3LL)), _tr_str_box(_tr_str_retain(_tr_str_lit_len("long long", 9LL))));
        }
    }
#line 149 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) && CppExporter__cpp_tuple_ok(self, ty))) {
#line 150 "src/codegen/cpp_export.tr"
        self->needs_tuple = true;
    }
    _tr_str_release(n);
}

__attribute__((hot)) void CppExporter_collect_export_classes(CppExporter* self, HirProgram* prog) {
#line 153 "src/codegen/cpp_export.tr"
    self->cls_set = _tr_dict_new(16LL);
#line 154 "src/codegen/cpp_export.tr"
    self->list_elems = _tr_dict_new(8LL);
#line 155 "src/codegen/cpp_export.tr"
    self->dict_variants = _tr_dict_new(8LL);
#line 156 "src/codegen/cpp_export.tr"
    self->needs_tuple = false;
#line 157 "src/codegen/cpp_export.tr"
    self->needs_list_ptr = false;
#line 158 "src/codegen/cpp_export.tr"
    if ((_is_invalid_ptr(((unsigned long long)(prog))) || _is_invalid_ptr(((unsigned long long)(prog->functions))))) {
#line 158 "src/codegen/cpp_export.tr"
        return;
    }
#line 159 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 160 "src/codegen/cpp_export.tr"
    while ((i < prog->functions->len)) {
#line 161 "src/codegen/cpp_export.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
#line 162 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
#line 163 "src/codegen/cpp_export.tr"
        if (_is_invalid_ptr(((unsigned long long)(f)))) {
#line 163 "src/codegen/cpp_export.tr"
            continue;
        }
#line 164 "src/codegen/cpp_export.tr"
        if ((!(f->is_export && (!f->is_extern)))) {
#line 164 "src/codegen/cpp_export.tr"
            continue;
        }
#line 165 "src/codegen/cpp_export.tr"
        CppExporter__note_export_ty(self, f->ret_ty);
#line 166 "src/codegen/cpp_export.tr"
        long long j = 0LL;
#line 167 "src/codegen/cpp_export.tr"
        while ((j < f->params->len)) {
#line 168 "src/codegen/cpp_export.tr"
            CppExporter__note_export_ty(self, ((HirParam*)List_ptr_get(f->params, j))->ty);
#line 169 "src/codegen/cpp_export.tr"
            j = (j + 1LL);
        }
    }
}

__attribute__((hot)) bool CppExporter__cpp_export_ty_ok(CppExporter* self, AstType* ty) {
#line 172 "src/codegen/cpp_export.tr"
    return ({ TrStr _cmpa_t6471 = (CppExporter__cpp_ty_kind(self, ty)); TrStr _cmpb_t6472 = (_tr_str_lit_len("no", 2LL)); _Bool _cmpr_t6473 = !_tr_str_eqv(_cmpa_t6471, _cmpb_t6472); _tr_str_release(_cmpa_t6471); _cmpr_t6473; });
}

__attribute__((hot)) bool CppExporter__cpp_is_class_list(CppExporter* self, AstType* ty) {
#line 176 "src/codegen/cpp_export.tr"
    return (((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) && CppExporter__cpp_list_ok(self, ty)) && ({ TrStr _cmpa_t6474 = (CppExporter__cpp_list_elem_kind(self, ty)); TrStr _cmpb_t6475 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t6476 = _tr_str_eqv(_cmpa_t6474, _cmpb_t6475); _tr_str_release(_cmpa_t6474); _cmpr_t6476; }));
}

__attribute__((hot)) bool CppExporter__cpp_export_fn_ok(CppExporter* self, HirFunction* f) {
#line 179 "src/codegen/cpp_export.tr"
    if ((!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
#line 179 "src/codegen/cpp_export.tr"
        return false;
    }
#line 180 "src/codegen/cpp_export.tr"
    if ((!CppExporter__cpp_export_ty_ok(self, f->ret_ty))) {
#line 180 "src/codegen/cpp_export.tr"
        return false;
    }
#line 181 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 182 "src/codegen/cpp_export.tr"
    while ((i < f->params->len)) {
#line 183 "src/codegen/cpp_export.tr"
        AstType* pty = ((HirParam*)List_ptr_get(f->params, i))->ty;
#line 184 "src/codegen/cpp_export.tr"
        if ((!CppExporter__cpp_export_ty_ok(self, pty))) {
#line 184 "src/codegen/cpp_export.tr"
            return false;
        }
#line 187 "src/codegen/cpp_export.tr"
        if (CppExporter__cpp_is_class_list(self, pty)) {
#line 187 "src/codegen/cpp_export.tr"
            return false;
        }
#line 188 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 189 "src/codegen/cpp_export.tr"
    return true;
}

__attribute__((hot)) bool CppExporter__cpp_kind_bridged(CppExporter* self, TrStr k) {
#line 195 "src/codegen/cpp_export.tr"
    return (((_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL))));
}

__attribute__((hot)) bool CppExporter__cpp_fn_needs_bridge(CppExporter* self, HirFunction* f) {
#line 197 "src/codegen/cpp_export.tr"
    if (({ TrStr _at_t6477 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t6477)); _tr_str_release(_at_t6477); _wr; })) {
#line 197 "src/codegen/cpp_export.tr"
        return true;
    }
#line 198 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 199 "src/codegen/cpp_export.tr"
    while ((i < f->params->len)) {
#line 200 "src/codegen/cpp_export.tr"
        if (({ TrStr _at_t6478 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_bridged(self, _at_t6478)); _tr_str_release(_at_t6478); _wr; })) {
#line 200 "src/codegen/cpp_export.tr"
            return true;
        }
#line 201 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 202 "src/codegen/cpp_export.tr"
    return false;
}

__attribute__((hot)) bool CppExporter__cpp_method_ok(CppExporter* self, HirFunction* m) {
#line 207 "src/codegen/cpp_export.tr"
    if ((!_tr_str_eqv((m->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
#line 207 "src/codegen/cpp_export.tr"
        return false;
    }
#line 208 "src/codegen/cpp_export.tr"
    TrStr rk = CppExporter__cpp_ty_kind(self, m->ret_ty);
#line 209 "src/codegen/cpp_export.tr"
    if ((((!_tr_str_eqv((rk), (_tr_str_lit_len("void", 4LL)))) && (!_tr_str_eqv((rk), (_tr_str_lit_len("str", 3LL))))) && (!_tr_str_eqv((rk), (_tr_str_lit_len("scalar", 6LL)))))) {
#line 209 "src/codegen/cpp_export.tr"
        _tr_str_release(rk);
        return false;
    }
#line 210 "src/codegen/cpp_export.tr"
    bool has_self = false;
#line 211 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 212 "src/codegen/cpp_export.tr"
    while ((i < m->params->len)) {
#line 213 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((((HirParam*)List_ptr_get(m->params, i))->name), (_tr_str_lit_len("self", 4LL)))) {
#line 214 "src/codegen/cpp_export.tr"
            has_self = true;
#line 215 "src/codegen/cpp_export.tr"
            i = (i + 1LL);
#line 216 "src/codegen/cpp_export.tr"
            continue;
        }
#line 217 "src/codegen/cpp_export.tr"
        TrStr k = CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(m->params, i))->ty);
#line 218 "src/codegen/cpp_export.tr"
        if (((!_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((k), (_tr_str_lit_len("scalar", 6LL)))))) {
#line 218 "src/codegen/cpp_export.tr"
            _tr_str_release(rk);
            _tr_str_release(k);
            return false;
        }
#line 219 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
        _tr_str_release(k);
    }
#line 220 "src/codegen/cpp_export.tr"
    bool _trret1521 = has_self;
    _tr_str_release(rk);
    return _trret1521;
}

__attribute__((hot)) TrStr CppExporter__cpp_tuple_type_cpp(CppExporter* self, AstType* ty) {
#line 224 "src/codegen/cpp_export.tr"
    TrStr s = _tr_str_lit_len("std::tuple<", 11LL);
#line 225 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 226 "src/codegen/cpp_export.tr"
    while ((i < ty->args->len)) {
#line 227 "src/codegen/cpp_export.tr"
        if ((i > 0LL)) {
#line 227 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6480 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t6480;
        }
#line 228 "src/codegen/cpp_export.tr"
        TrStr _strtmp_t6482 = ({ TrStr _tr_cr = (CppExporter__cpp_tuple_elem_cpp(self, ty, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t6482;
#line 229 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 230 "src/codegen/cpp_export.tr"
    TrStr _trret1522 = _tr_strx_concatv((s), (_tr_str_lit_len(">", 1LL)));
    _tr_str_release(s);
    return _trret1522;
}

__attribute__((hot)) TrStr CppExporter__cpp_c_ty(CppExporter* self, AstType* ty) {
#line 236 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 237 "src/codegen/cpp_export.tr"
    if (((_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL))))) {
#line 237 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("void*", 5LL);
    }
#line 238 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
#line 238 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("TrTuple", 7LL);
    }
#line 239 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
#line 239 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("TrStr", 5LL);
    }
#line 240 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
#line 240 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("void", 4LL);
    }
#line 241 "src/codegen/cpp_export.tr"
    TrStr _trret1523 = CGenerator_type_to_c(self->gen, ty);
    _tr_str_release(k);
    return _trret1523;
}

__attribute__((hot)) TrStr CppExporter__cpp_c_arg(CppExporter* self, AstType* ty, TrStr name) {
#line 244 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 245 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
#line 245 "src/codegen/cpp_export.tr"
        TrStr _trret1524 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1524;
    }
#line 246 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
#line 246 "src/codegen/cpp_export.tr"
        TrStr _trret1525 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_list_sfx(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(List_", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1525;
    }
#line 247 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
#line 247 "src/codegen/cpp_export.tr"
        TrStr _trret1526 = _tr_strx_concatv((_tr_str_lit_len("(TrMap*)", 8LL)), (name));
        _tr_str_release(k);
        return _trret1526;
    }
#line 248 "src/codegen/cpp_export.tr"
    TrStr _trret1527 = _tr_str_retain(name);
    _tr_str_release(k);
    return _trret1527;
}

__attribute__((hot)) TrStr CppExporter__cpp_param_decl(CppExporter* self, AstType* ty, TrStr name) {
#line 251 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 252 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
#line 252 "src/codegen/cpp_export.tr"
        TrStr _trret1528 = _tr_strx_concatv((_tr_str_lit_len("const std::string& ", 19LL)), (name));
        _tr_str_release(k);
        return _trret1528;
    }
#line 253 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
#line 253 "src/codegen/cpp_export.tr"
        TrStr _trret1529 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1529;
    }
#line 254 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
#line 254 "src/codegen/cpp_export.tr"
        TrStr _trret1530 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("const std::vector<", 18LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">& ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1530;
    }
#line 255 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
#line 255 "src/codegen/cpp_export.tr"
        TrStr _trret1531 = ({ __auto_type _ord_t6483 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("const std::map<", 15LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6483); TrStr _tr_cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">& ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        _tr_str_release(k);
        return _trret1531;
    }
#line 256 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
#line 256 "src/codegen/cpp_export.tr"
        TrStr _trret1532 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_tuple_type_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1532;
    }
#line 257 "src/codegen/cpp_export.tr"
    TrStr _trret1533 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (CGenerator_type_to_c(self->gen, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(k);
    return _trret1533;
}

__attribute__((hot)) TrStr CppExporter__cpp_call_arg(CppExporter* self, AstType* ty, TrStr name) {
#line 260 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 261 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
#line 261 "src/codegen/cpp_export.tr"
        TrStr _trret1534 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TrStr{ (char*)", 14LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".c_str(), (long*)0 }", 20LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1534;
    }
#line 262 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
#line 262 "src/codegen/cpp_export.tr"
        TrStr _trret1535 = _tr_strx_concatv((name), (_tr_str_lit_len(".raw()", 6LL)));
        _tr_str_release(k);
        return _trret1535;
    }
#line 263 "src/codegen/cpp_export.tr"
    TrStr _trret1536 = _tr_str_retain(name);
    _tr_str_release(k);
    return _trret1536;
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_type(CppExporter* self, AstType* ty) {
#line 265 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 266 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
#line 266 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("std::string", 11LL);
    }
#line 267 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
#line 267 "src/codegen/cpp_export.tr"
        TrStr _trret1537 = _tr_str_retain(ty->name);
        _tr_str_release(k);
        return _trret1537;
    }
#line 268 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL)))) {
#line 268 "src/codegen/cpp_export.tr"
        TrStr _trret1538 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_list_elem_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("std::vector<", 12LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1538;
    }
#line 269 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) {
#line 269 "src/codegen/cpp_export.tr"
        TrStr _trret1539 = ({ __auto_type _ord_t6484 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_dict_key_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("std::map<", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6484); TrStr _tr_cr = (CppExporter__cpp_dict_v_cpp(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        _tr_str_release(k);
        return _trret1539;
    }
#line 270 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL)))) {
#line 270 "src/codegen/cpp_export.tr"
        TrStr _trret1540 = CppExporter__cpp_tuple_type_cpp(self, ty);
        _tr_str_release(k);
        return _trret1540;
    }
#line 271 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
#line 271 "src/codegen/cpp_export.tr"
        _tr_str_release(k);
        return _tr_str_lit_len("void", 4LL);
    }
#line 272 "src/codegen/cpp_export.tr"
    TrStr _trret1541 = CGenerator_type_to_c(self->gen, ty);
    _tr_str_release(k);
    return _trret1541;
}

__attribute__((hot)) TrStr CppExporter__cpp_ret_stmt(CppExporter* self, AstType* ty, TrStr call) {
#line 276 "src/codegen/cpp_export.tr"
    TrStr k = CppExporter__cpp_ty_kind(self, ty);
#line 277 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("void", 4LL)))) {
#line 277 "src/codegen/cpp_export.tr"
        TrStr _trret1542 = _tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)));
        _tr_str_release(k);
        return _trret1542;
    }
#line 278 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) {
#line 278 "src/codegen/cpp_export.tr"
        TrStr _trret1543 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TrStr _r = ", 11LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; return std::string(_r.data ? _r.data : \"\");", 45LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1543;
    }
#line 279 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((k), (_tr_str_lit_len("class", 5LL)))) {
#line 279 "src/codegen/cpp_export.tr"
        TrStr _trret1544 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("::_wrap(", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(k);
        return _trret1544;
    }
#line 280 "src/codegen/cpp_export.tr"
    TrStr _trret1545 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(k);
    return _trret1545;
}

__attribute__((hot)) bool CppExporter__cpp_kind_marshal(CppExporter* self, TrStr k) {
#line 284 "src/codegen/cpp_export.tr"
    return ((_tr_str_eqv((k), (_tr_str_lit_len("list", 4LL))) || _tr_str_eqv((k), (_tr_str_lit_len("dict", 4LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("tuple", 5LL))));
}

__attribute__((hot)) bool CppExporter__cpp_fn_has_list(CppExporter* self, HirFunction* f) {
#line 286 "src/codegen/cpp_export.tr"
    if (({ TrStr _at_t6485 = (CppExporter__cpp_ty_kind(self, f->ret_ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t6485)); _tr_str_release(_at_t6485); _wr; })) {
#line 286 "src/codegen/cpp_export.tr"
        return true;
    }
#line 287 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 288 "src/codegen/cpp_export.tr"
    while ((i < f->params->len)) {
#line 289 "src/codegen/cpp_export.tr"
        if (({ TrStr _at_t6486 = (CppExporter__cpp_ty_kind(self, ((HirParam*)List_ptr_get(f->params, i))->ty)); __auto_type _wr = (CppExporter__cpp_kind_marshal(self, _at_t6486)); _tr_str_release(_at_t6486); _wr; })) {
#line 289 "src/codegen/cpp_export.tr"
            return true;
        }
#line 290 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 291 "src/codegen/cpp_export.tr"
    return false;
}

__attribute__((hot)) TrStr CppExporter__cpp_wrap_fn(CppExporter* self, HirFunction* f) {
#line 297 "src/codegen/cpp_export.tr"
    TrStr target = _tr_strx_concatv((_tr_str_lit_len("::", 2LL)), (f->name));
#line 298 "src/codegen/cpp_export.tr"
    if (CppExporter__cpp_fn_needs_bridge(self, f)) {
#line 298 "src/codegen/cpp_export.tr"
        TrStr _strtmp_t6487 = _tr_strx_concatv((_tr_str_lit_len("tr__", 4LL)), (f->name));
        _tr_str_release(target);
        target = _strtmp_t6487;
    }
#line 299 "src/codegen/cpp_export.tr"
    StringBuilder* wb = StringBuilder_init(256LL);
#line 300 "src/codegen/cpp_export.tr"
    ({ __auto_type _ord_t6488 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_ret_type(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("    inline ", 11LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6489 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6488); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6489); _tr_str_release(_sbt_t6489); });
#line 301 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 302 "src/codegen/cpp_export.tr"
    while ((i < f->params->len)) {
#line 303 "src/codegen/cpp_export.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, i));
#line 304 "src/codegen/cpp_export.tr"
        TrStr pn = _tr_str_retain(p->name);
#line 305 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((pn), (_tr_str_lit_len("", 0LL)))) {
#line 305 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6490 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(pn);
            pn = _strtmp_t6490;
        }
#line 306 "src/codegen/cpp_export.tr"
        if ((i > 0LL)) {
#line 306 "src/codegen/cpp_export.tr"
            StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
        }
#line 307 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6491 = (CppExporter__cpp_param_decl(self, p->ty, pn)); StringBuilder_append(wb, _sbt_t6491); _tr_str_release(_sbt_t6491); });
#line 308 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
        _tr_str_release(pn);
    }
#line 310 "src/codegen/cpp_export.tr"
    if ((!CppExporter__cpp_fn_has_list(self, f))) {
#line 311 "src/codegen/cpp_export.tr"
        TrStr call_args = _tr_str_lit_len("", 0LL);
#line 312 "src/codegen/cpp_export.tr"
        i = 0LL;
#line 313 "src/codegen/cpp_export.tr"
        while ((i < f->params->len)) {
#line 314 "src/codegen/cpp_export.tr"
            HirParam* p2 = ((HirParam*)List_ptr_get(f->params, i));
#line 315 "src/codegen/cpp_export.tr"
            TrStr pn2 = _tr_str_retain(p2->name);
#line 316 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((pn2), (_tr_str_lit_len("", 0LL)))) {
#line 316 "src/codegen/cpp_export.tr"
                TrStr _strtmp_t6492 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(pn2);
                pn2 = _strtmp_t6492;
            }
#line 317 "src/codegen/cpp_export.tr"
            if ((i > 0LL)) {
#line 317 "src/codegen/cpp_export.tr"
                ({ TrStr _ap_t6493 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&call_args, _ap_t6493); });
            }
#line 318 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6494 = (CppExporter__cpp_call_arg(self, p2->ty, pn2)); _tr_str_append(&call_args, _ap_t6494); _tr_str_release(_ap_t6494); });
#line 319 "src/codegen/cpp_export.tr"
            i = (i + 1LL);
            _tr_str_release(pn2);
        }
#line 320 "src/codegen/cpp_export.tr"
        ({ __auto_type _ord_t6496 = (f->ret_ty); TrStr _at_t6495 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((target), (_tr_str_lit_len("(", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6497 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_ret_stmt(self, _ord_t6496, _at_t6495)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(") { ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6497); _tr_str_release(_at_t6495); _tr_str_release(_sbt_t6497); });
#line 321 "src/codegen/cpp_export.tr"
        TrStr _trret1546 = StringObj_as_str(StringBuilder_to_string(wb));
        _tr_str_release(target);
        _tr_str_release(call_args);
        return _trret1546;
    }
#line 323 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len(") {\n", 4LL));
#line 324 "src/codegen/cpp_export.tr"
    TrStr call_args = _tr_str_lit_len("", 0LL);
#line 325 "src/codegen/cpp_export.tr"
    TrStr frees = _tr_str_lit_len("", 0LL);
#line 326 "src/codegen/cpp_export.tr"
    i = 0LL;
#line 327 "src/codegen/cpp_export.tr"
    while ((i < f->params->len)) {
#line 328 "src/codegen/cpp_export.tr"
        HirParam* p3 = ((HirParam*)List_ptr_get(f->params, i));
#line 329 "src/codegen/cpp_export.tr"
        TrStr pn3 = _tr_str_retain(p3->name);
#line 330 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((pn3), (_tr_str_lit_len("", 0LL)))) {
#line 330 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6498 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(pn3);
            pn3 = _strtmp_t6498;
        }
#line 331 "src/codegen/cpp_export.tr"
        if ((i > 0LL)) {
#line 331 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6499 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&call_args, _ap_t6499); });
        }
#line 332 "src/codegen/cpp_export.tr"
        TrStr pk = CppExporter__cpp_ty_kind(self, p3->ty);
#line 333 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((pk), (_tr_str_lit_len("list", 4LL)))) {
#line 334 "src/codegen/cpp_export.tr"
            TrStr sfx = CppExporter__cpp_list_sfx(self, p3->ty);
#line 335 "src/codegen/cpp_export.tr"
            TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_l", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 336 "src/codegen/cpp_export.tr"
            TrStr econv = _tr_str_lit_len("_e", 2LL);
#line 337 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv(((*((AstType**)List_ptr_get(p3->ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
#line 337 "src/codegen/cpp_export.tr"
                TrStr _strtmp_t6500 = _tr_str_lit_len("TrStr{ (char*)_e.c_str(), (long*)0 }", 36LL);
                _tr_str_release(econv);
                econv = _strtmp_t6500;
            }
#line 338 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6501 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        void* ", 14LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = tr__list_", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new();\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6501); _tr_str_release(_sbt_t6501); });
#line 339 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6502 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        for (const auto& _e : ", 30LL)), (pn3))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") tr__list_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_push(", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (econv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6502); _tr_str_release(_sbt_t6502); });
#line 340 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6503 = (tmp); _tr_str_append(&call_args, _ap_t6503); });
#line 341 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6504 = (_tr_str_lit_len("        tr__list_", 17LL)); TrStr _ap_t6505 = (sfx); TrStr _ap_t6506 = (_tr_str_lit_len("_free(", 6LL)); TrStr _ap_t6507 = (tmp); TrStr _ap_t6508 = (_tr_str_lit_len(");\n", 3LL)); _tr_str_append(&frees, _ap_t6504); _tr_str_append(&frees, _ap_t6505); _tr_str_append(&frees, _ap_t6506); _tr_str_append(&frees, _ap_t6507); _tr_str_append(&frees, _ap_t6508); });
            _tr_str_release(sfx);
            _tr_str_release(tmp);
            _tr_str_release(econv);
        } else if (_tr_str_eqv((pk), (_tr_str_lit_len("dict", 4LL)))) {
#line 343 "src/codegen/cpp_export.tr"
            TrStr vt = CppExporter__cpp_dict_variant(self, p3->ty);
#line 344 "src/codegen/cpp_export.tr"
            TrStr vk = CppExporter__cpp_dict_vkind(self, p3->ty);
#line 345 "src/codegen/cpp_export.tr"
            TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_d", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 346 "src/codegen/cpp_export.tr"
            TrStr kconv = _tr_str_lit_len("_kv.first", 9LL);
#line 347 "src/codegen/cpp_export.tr"
            if (({ TrStr _cmpa_t6509 = (CppExporter__cpp_dict_kkind(self, p3->ty)); TrStr _cmpb_t6510 = (_tr_str_lit_len("s", 1LL)); _Bool _cmpr_t6511 = _tr_str_eqv(_cmpa_t6509, _cmpb_t6510); _tr_str_release(_cmpa_t6509); _cmpr_t6511; })) {
#line 347 "src/codegen/cpp_export.tr"
                TrStr _strtmp_t6512 = _tr_str_lit_len("TrStr{ (char*)_kv.first.c_str(), (long*)0 }", 43LL);
                _tr_str_release(kconv);
                kconv = _strtmp_t6512;
            }
#line 348 "src/codegen/cpp_export.tr"
            TrStr vconv = _tr_str_lit_len("_kv.second", 10LL);
#line 349 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
#line 349 "src/codegen/cpp_export.tr"
                TrStr _strtmp_t6513 = _tr_str_lit_len("TrStr{ (char*)_kv.second.c_str(), (long*)0 }", 44LL);
                _tr_str_release(vconv);
                vconv = _strtmp_t6513;
            }
#line 350 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6514 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        void* ", 14LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = tr__dict_", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new();\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6514); _tr_str_release(_sbt_t6514); });
#line 351 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6515 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        for (const auto& _kv : ", 31LL)), (pn3))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") tr__dict_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_set(", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kconv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vconv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6515); _tr_str_release(_sbt_t6515); });
#line 352 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6516 = (tmp); _tr_str_append(&call_args, _ap_t6516); });
#line 353 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6517 = (_tr_str_lit_len("        tr__dict_", 17LL)); TrStr _ap_t6518 = (vt); TrStr _ap_t6519 = (_tr_str_lit_len("_free(", 6LL)); TrStr _ap_t6520 = (tmp); TrStr _ap_t6521 = (_tr_str_lit_len(");\n", 3LL)); _tr_str_append(&frees, _ap_t6517); _tr_str_append(&frees, _ap_t6518); _tr_str_append(&frees, _ap_t6519); _tr_str_append(&frees, _ap_t6520); _tr_str_append(&frees, _ap_t6521); });
            _tr_str_release(vk);
            _tr_str_release(vt);
            _tr_str_release(tmp);
            _tr_str_release(kconv);
            _tr_str_release(vconv);
        } else if (_tr_str_eqv((pk), (_tr_str_lit_len("tuple", 5LL)))) {
#line 355 "src/codegen/cpp_export.tr"
            TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_t", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 356 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6522 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        TrTuple ", 16LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = {};\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6522); _tr_str_release(_sbt_t6522); });
#line 357 "src/codegen/cpp_export.tr"
            long long ti = 0LL;
#line 358 "src/codegen/cpp_export.tr"
            while ((ti < p3->ty->args->len)) {
#line 359 "src/codegen/cpp_export.tr"
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(p3->ty->args, ti)))->name);
#line 360 "src/codegen/cpp_export.tr"
                TrStr g = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("std::get<", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">(", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn3)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 361 "src/codegen/cpp_export.tr"
                if (_tr_str_eqv((sk), (_tr_str_lit_len("str", 3LL)))) {
#line 362 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6523 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] = tr__box_str(TrStr{ (char*)", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".c_str(), (long*)0 });\n", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6523); _tr_str_release(_sbt_t6523); });
                } else if (_tr_str_eqv((sk), (_tr_str_lit_len("f64", 3LL)))) {
#line 364 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6524 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] = tr__box_f64(", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6524); _tr_str_release(_sbt_t6524); });
                } else {
#line 366 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6525 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (tmp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".data[", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] = (long long)", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6525); _tr_str_release(_sbt_t6525); });
                }
#line 367 "src/codegen/cpp_export.tr"
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(g);
            }
#line 368 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6526 = (tmp); _tr_str_append(&call_args, _ap_t6526); });
            _tr_str_release(tmp);
        } else {
#line 370 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6527 = (CppExporter__cpp_call_arg(self, p3->ty, pn3)); _tr_str_append(&call_args, _ap_t6527); _tr_str_release(_ap_t6527); });
        }
#line 371 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
        _tr_str_release(pn3);
        _tr_str_release(pk);
    }
#line 372 "src/codegen/cpp_export.tr"
    TrStr call_expr = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((target), (_tr_str_lit_len("(", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 373 "src/codegen/cpp_export.tr"
    TrStr rk = CppExporter__cpp_ty_kind(self, f->ret_ty);
#line 374 "src/codegen/cpp_export.tr"
    if (_tr_str_eqv((rk), (_tr_str_lit_len("void", 4LL)))) {
#line 375 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6528 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (call_expr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (frees)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6528); _tr_str_release(_sbt_t6528); });
    } else {
#line 377 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6529 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" _r = ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call_expr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (frees)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6529); _tr_str_release(_sbt_t6529); });
#line 378 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((rk), (_tr_str_lit_len("list", 4LL)))) {
#line 379 "src/codegen/cpp_export.tr"
            TrStr ecpp = CppExporter__cpp_list_elem_cpp(self, f->ret_ty);
#line 380 "src/codegen/cpp_export.tr"
            if (({ TrStr _cmpa_t6530 = (CppExporter__cpp_list_elem_kind(self, f->ret_ty)); TrStr _cmpb_t6531 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t6532 = _tr_str_eqv(_cmpa_t6530, _cmpb_t6531); _tr_str_release(_cmpa_t6530); _cmpr_t6532; })) {
#line 383 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len("        long long _n = tr__list_ptr_len(_r);\n", 45LL));
#line 384 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6533 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        std::vector<", 20LL)), (ecpp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("> _v; _v.reserve((size_t)_n);\n", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6533); _tr_str_release(_sbt_t6533); });
#line 385 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6534 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(", 59LL)), (ecpp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("::_wrap((void*)tr__list_ptr_get(_r, _i)));\n", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6534); _tr_str_release(_sbt_t6534); });
#line 386 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len("        tr__list_ptr_free_shallow(_r);\n        return _v;\n", 58LL));
            } else {
#line 388 "src/codegen/cpp_export.tr"
                TrStr rsfx = CppExporter__cpp_list_sfx(self, f->ret_ty);
#line 389 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6535 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        long long _n = tr__list_", 32LL)), (rsfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_len(_r);\n", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6535); _tr_str_release(_sbt_t6535); });
#line 390 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6536 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        std::vector<", 20LL)), (ecpp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("> _v; _v.reserve((size_t)_n);\n", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6536); _tr_str_release(_sbt_t6536); });
#line 391 "src/codegen/cpp_export.tr"
                if (_tr_str_eqv(((*((AstType**)List_ptr_get(f->ret_ty->args, 0LL)))->name), (_tr_str_lit_len("str", 3LL)))) {
#line 392 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6537 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) { TrStr _s = tr__list_", 68LL)), (rsfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(_r, _i); _v.push_back(std::string(_s.data ? _s.data : \"\")); }\n", 67LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6537); _tr_str_release(_sbt_t6537); });
                } else {
#line 394 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6538 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) _v.push_back(tr__list_", 68LL)), (rsfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(_r, _i));\n", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6538); _tr_str_release(_sbt_t6538); });
                }
#line 395 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6539 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        tr__list_", 17LL)), (rsfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(_r);\n        return _v;\n", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6539); _tr_str_release(_sbt_t6539); });
                _tr_str_release(rsfx);
            }
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("dict", 4LL)))) {
#line 397 "src/codegen/cpp_export.tr"
            TrStr vt = CppExporter__cpp_dict_variant(self, f->ret_ty);
#line 398 "src/codegen/cpp_export.tr"
            TrStr vk = CppExporter__cpp_dict_vkind(self, f->ret_ty);
#line 399 "src/codegen/cpp_export.tr"
            TrStr ksfx = _tr_str_lit_len("TrStr", 5LL);
#line 400 "src/codegen/cpp_export.tr"
            if (({ TrStr _cmpa_t6540 = (CppExporter__cpp_dict_kkind(self, f->ret_ty)); TrStr _cmpb_t6541 = (_tr_str_lit_len("i", 1LL)); _Bool _cmpr_t6542 = _tr_str_eqv(_cmpa_t6540, _cmpb_t6541); _tr_str_release(_cmpa_t6540); _cmpr_t6542; })) {
#line 400 "src/codegen/cpp_export.tr"
                TrStr _strtmp_t6543 = _tr_str_lit_len("i64", 3LL);
                _tr_str_release(ksfx);
                ksfx = _strtmp_t6543;
            }
#line 401 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6544 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        void* _ks = tr__dict_", 29LL)), (vt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_keys(_r);\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6544); _tr_str_release(_sbt_t6544); });
#line 402 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6545 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        long long _n = tr__list_", 32LL)), (ksfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_len(_ks);\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6545); _tr_str_release(_sbt_t6545); });
#line 403 "src/codegen/cpp_export.tr"
            ({ __auto_type _ord_t6546 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_dict_key_cpp(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("        std::map<", 17LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6547 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6546); TrStr _tr_cr = (CppExporter__cpp_dict_v_cpp(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("> _m;\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6547); _tr_str_release(_sbt_t6547); });
#line 404 "src/codegen/cpp_export.tr"
            StringBuilder_append(wb, _tr_str_lit_len("        for (long long _i = 0; _i < _n; ++_i) {\n", 48LL));
#line 405 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((ksfx), (_tr_str_lit_len("TrStr", 5LL)))) {
#line 406 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len("            TrStr _k = tr__list_TrStr_get(_ks, _i); std::string _kk2(_k.data ? _k.data : \"\");\n", 94LL));
            } else {
#line 408 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len("            long long _k = tr__list_i64_get(_ks, _i); long long _kk2 = _k;\n", 75LL));
            }
#line 409 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
#line 410 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6548 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("            TrStr _vv = tr__dict_", 33LL)), (vt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(_r, _k); _m[_kk2] = std::string(_vv.data ? _vv.data : \"\");\n", 64LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6548); _tr_str_release(_sbt_t6548); });
            } else {
#line 412 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6549 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("            _m[_kk2] = tr__dict_", 32LL)), (vt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(_r, _k);\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6549); _tr_str_release(_sbt_t6549); });
            }
#line 413 "src/codegen/cpp_export.tr"
            StringBuilder_append(wb, _tr_str_lit_len("        }\n", 10LL));
#line 414 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6550 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        tr__list_", 17LL)), (ksfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(_ks); tr__dict_", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(_r);\n        return _m;\n", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6550); _tr_str_release(_sbt_t6550); });
            _tr_str_release(vt);
            _tr_str_release(ksfx);
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("tuple", 5LL)))) {
#line 416 "src/codegen/cpp_export.tr"
            TrStr parts = _tr_str_lit_len("", 0LL);
#line 417 "src/codegen/cpp_export.tr"
            long long ti = 0LL;
#line 418 "src/codegen/cpp_export.tr"
            while ((ti < f->ret_ty->args->len)) {
#line 419 "src/codegen/cpp_export.tr"
                TrStr ev = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_te", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 420 "src/codegen/cpp_export.tr"
                TrStr sk = CppExporter__cpp_slot_kind(self, (*((AstType**)List_ptr_get(f->ret_ty->args, ti)))->name);
#line 421 "src/codegen/cpp_export.tr"
                TrStr slot = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_r.data[", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 422 "src/codegen/cpp_export.tr"
                if (_tr_str_eqv((sk), (_tr_str_lit_len("str", 3LL)))) {
#line 423 "src/codegen/cpp_export.tr"
                    ({ __auto_type _ord_t6551 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("        TrStr _ts", 17LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = tr__unbox_str(", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (slot)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); std::string ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ev)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(_ts", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t6552 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6551); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".data ? _ts", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6553 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6552); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ti)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".data : \"\");\n", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6553); _tr_str_release(_sbt_t6553); });
                } else if (_tr_str_eqv((sk), (_tr_str_lit_len("f64", 3LL)))) {
#line 425 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6554 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        double ", 15LL)), (ev))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = tr__unbox_f64(", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (slot)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6554); _tr_str_release(_sbt_t6554); });
                } else {
#line 427 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6555 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        long long ", 18LL)), (ev))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = (long long)", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (slot)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6555); _tr_str_release(_sbt_t6555); });
                }
#line 428 "src/codegen/cpp_export.tr"
                if ((ti > 0LL)) {
#line 428 "src/codegen/cpp_export.tr"
                    ({ TrStr _ap_t6556 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&parts, _ap_t6556); });
                }
#line 429 "src/codegen/cpp_export.tr"
                ({ TrStr _ap_t6557 = (ev); _tr_str_append(&parts, _ap_t6557); });
#line 430 "src/codegen/cpp_export.tr"
                ti = (ti + 1LL);
                _tr_str_release(sk);
                _tr_str_release(ev);
                _tr_str_release(slot);
            }
#line 431 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6558 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        return std::make_tuple(", 31LL)), (parts))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6558); _tr_str_release(_sbt_t6558); });
            _tr_str_release(parts);
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("str", 3LL)))) {
#line 433 "src/codegen/cpp_export.tr"
            StringBuilder_append(wb, _tr_str_lit_len("        return std::string(_r.data ? _r.data : \"\");\n", 52LL));
        } else if (_tr_str_eqv((rk), (_tr_str_lit_len("class", 5LL)))) {
#line 435 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6559 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        return ", 15LL)), (f->ret_ty->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("::_wrap(_r);\n", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6559); _tr_str_release(_sbt_t6559); });
        } else {
#line 437 "src/codegen/cpp_export.tr"
            StringBuilder_append(wb, _tr_str_lit_len("        return _r;\n", 19LL));
        }
    }
#line 438 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len("    }\n", 6LL));
#line 439 "src/codegen/cpp_export.tr"
    TrStr _trret1547 = StringObj_as_str(StringBuilder_to_string(wb));
    _tr_str_release(target);
    _tr_str_release(call_args);
    _tr_str_release(frees);
    _tr_str_release(call_expr);
    _tr_str_release(rk);
    StringBuilder__tr_fn_free(wb);
    return _trret1547;
}

__attribute__((hot)) bool CppExporter__cpp_init_ok(CppExporter* self, HirFunction* m) {
#line 445 "src/codegen/cpp_export.tr"
    if ((!_tr_str_eqv((m->throws_ty->name), (_tr_str_lit_len("", 0LL))))) {
#line 445 "src/codegen/cpp_export.tr"
        return false;
    }
#line 446 "src/codegen/cpp_export.tr"
    if (({ TrStr _cmpa_t6560 = (CppExporter__cpp_ty_kind(self, m->ret_ty)); TrStr _cmpb_t6561 = (_tr_str_lit_len("class", 5LL)); _Bool _cmpr_t6562 = !_tr_str_eqv(_cmpa_t6560, _cmpb_t6561); _tr_str_release(_cmpa_t6560); _cmpr_t6562; })) {
#line 446 "src/codegen/cpp_export.tr"
        return false;
    }
#line 447 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 448 "src/codegen/cpp_export.tr"
    while ((i < m->params->len)) {
#line 449 "src/codegen/cpp_export.tr"
        HirParam* p = ((HirParam*)List_ptr_get(m->params, i));
#line 450 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 450 "src/codegen/cpp_export.tr"
            return false;
        }
#line 451 "src/codegen/cpp_export.tr"
        TrStr k = CppExporter__cpp_ty_kind(self, p->ty);
#line 452 "src/codegen/cpp_export.tr"
        if (((!_tr_str_eqv((k), (_tr_str_lit_len("str", 3LL)))) && (!_tr_str_eqv((k), (_tr_str_lit_len("scalar", 6LL)))))) {
#line 452 "src/codegen/cpp_export.tr"
            _tr_str_release(k);
            return false;
        }
#line 453 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
        _tr_str_release(k);
    }
#line 454 "src/codegen/cpp_export.tr"
    return true;
}

__attribute__((hot)) long long CppExporter__cpp_init_idx(CppExporter* self, HirClass* c) {
#line 458 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 459 "src/codegen/cpp_export.tr"
    while ((i < c->methods->len)) {
#line 460 "src/codegen/cpp_export.tr"
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, i));
#line 461 "src/codegen/cpp_export.tr"
        if ((((!_is_invalid_ptr(((unsigned long long)(m)))) && _tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL)))) && CppExporter__cpp_init_ok(self, m))) {
#line 462 "src/codegen/cpp_export.tr"
            long long _trret1548 = i;
            return _trret1548;
        }
#line 463 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
    }
#line 464 "src/codegen/cpp_export.tr"
    return (-1LL);
}

__attribute__((hot)) TrStr CppExporter__cpp_class_decl(CppExporter* self, HirClass* c) {
#line 470 "src/codegen/cpp_export.tr"
    TrStr C = _tr_str_retain(c->name);
#line 471 "src/codegen/cpp_export.tr"
    StringBuilder* wb = StringBuilder_init(512LL);
#line 472 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6563 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    class ", 10LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" {\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6563); _tr_str_release(_sbt_t6563); });
#line 473 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len("        void* h_;\n", 18LL));
#line 474 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6564 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        explicit ", 17LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(void* _p): h_(_p) {}\n", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6564); _tr_str_release(_sbt_t6564); });
#line 475 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len("    public:\n", 12LL));
#line 476 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6565 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        static ", 15LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" _wrap(void* _p) { return ", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(_p); }\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6565); _tr_str_release(_sbt_t6565); });
#line 477 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6566 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(const ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& o): h_(o.h_ ? tr__", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_retain(o.h_) : nullptr) {}\n", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6566); _tr_str_release(_sbt_t6566); });
#line 478 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6567 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("&& o) noexcept : h_(o.h_) { o.h_ = nullptr; }\n", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6567); _tr_str_release(_sbt_t6567); });
#line 479 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6568 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& operator=(const ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& o) { if (this != &o) { if (h_) tr__", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_release(h_); h_ = o.h_ ? tr__", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_retain(o.h_) : nullptr; } return *this; }\n", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6568); _tr_str_release(_sbt_t6568); });
#line 480 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6569 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("& operator=(", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("&& o) noexcept { if (this != &o) { if (h_) tr__", 47LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_release(h_); h_ = o.h_; o.h_ = nullptr; } return *this; }\n", 59LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6569); _tr_str_release(_sbt_t6569); });
#line 481 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6570 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        ~", 9LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() { if (h_) tr__", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_release(h_); }\n", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6570); _tr_str_release(_sbt_t6570); });
#line 482 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len("        void* raw() const { return h_; }\n", 41LL));
#line 484 "src/codegen/cpp_export.tr"
    long long iidx = CppExporter__cpp_init_idx(self, c);
#line 485 "src/codegen/cpp_export.tr"
    if ((iidx >= 0LL)) {
#line 486 "src/codegen/cpp_export.tr"
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
#line 487 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6571 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("        explicit ", 17LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6571); _tr_str_release(_sbt_t6571); });
#line 488 "src/codegen/cpp_export.tr"
        TrStr cargs = _tr_str_lit_len("", 0LL);
#line 489 "src/codegen/cpp_export.tr"
        long long pi = 0LL;
#line 490 "src/codegen/cpp_export.tr"
        long long emitted = 0LL;
#line 491 "src/codegen/cpp_export.tr"
        while ((pi < im->params->len)) {
#line 492 "src/codegen/cpp_export.tr"
            HirParam* p = ((HirParam*)List_ptr_get(im->params, pi));
#line 493 "src/codegen/cpp_export.tr"
            pi = (pi + 1LL);
#line 494 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 494 "src/codegen/cpp_export.tr"
                continue;
            }
#line 495 "src/codegen/cpp_export.tr"
            if ((emitted > 0LL)) {
#line 496 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
#line 497 "src/codegen/cpp_export.tr"
                ({ TrStr _ap_t6572 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&cargs, _ap_t6572); });
            }
#line 498 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6573 = (CppExporter__cpp_param_decl(self, p->ty, p->name)); StringBuilder_append(wb, _sbt_t6573); _tr_str_release(_sbt_t6573); });
#line 499 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6574 = (CppExporter__cpp_call_arg(self, p->ty, p->name)); _tr_str_append(&cargs, _ap_t6574); _tr_str_release(_ap_t6574); });
#line 500 "src/codegen/cpp_export.tr"
            emitted = (emitted + 1LL);
        }
#line 501 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6575 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("): h_(tr__", 10LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")) {}\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6575); _tr_str_release(_sbt_t6575); });
        _tr_str_release(cargs);
    }
#line 503 "src/codegen/cpp_export.tr"
    long long mi = 0LL;
#line 504 "src/codegen/cpp_export.tr"
    while ((mi < c->methods->len)) {
#line 505 "src/codegen/cpp_export.tr"
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
#line 506 "src/codegen/cpp_export.tr"
        mi = (mi + 1LL);
#line 507 "src/codegen/cpp_export.tr"
        if (_is_invalid_ptr(((unsigned long long)(m)))) {
#line 507 "src/codegen/cpp_export.tr"
            continue;
        }
#line 508 "src/codegen/cpp_export.tr"
        if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_withv((m->name), (_tr_str_lit_len("__", 2LL))))) {
#line 508 "src/codegen/cpp_export.tr"
            continue;
        }
#line 509 "src/codegen/cpp_export.tr"
        if ((!CppExporter__cpp_method_ok(self, m))) {
#line 509 "src/codegen/cpp_export.tr"
            continue;
        }
#line 510 "src/codegen/cpp_export.tr"
        ({ __auto_type _ord_t6576 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_ret_type(self, m->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("        ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6577 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6576); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6577); _tr_str_release(_sbt_t6577); });
#line 511 "src/codegen/cpp_export.tr"
        TrStr margs = _tr_str_lit_len("h_", 2LL);
#line 512 "src/codegen/cpp_export.tr"
        long long pj = 0LL;
#line 513 "src/codegen/cpp_export.tr"
        long long me = 0LL;
#line 514 "src/codegen/cpp_export.tr"
        while ((pj < m->params->len)) {
#line 515 "src/codegen/cpp_export.tr"
            HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
#line 516 "src/codegen/cpp_export.tr"
            pj = (pj + 1LL);
#line 517 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
#line 517 "src/codegen/cpp_export.tr"
                continue;
            }
#line 518 "src/codegen/cpp_export.tr"
            if ((me > 0LL)) {
#line 518 "src/codegen/cpp_export.tr"
                StringBuilder_append(wb, _tr_str_lit_len(", ", 2LL));
            }
#line 519 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6578 = (CppExporter__cpp_param_decl(self, mp->ty, mp->name)); StringBuilder_append(wb, _sbt_t6578); _tr_str_release(_sbt_t6578); });
#line 520 "src/codegen/cpp_export.tr"
            ({ TrStr _ap_t6579 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t6580 = (CppExporter__cpp_call_arg(self, mp->ty, mp->name)); _tr_str_append(&margs, _ap_t6579); _tr_str_append(&margs, _ap_t6580); _tr_str_release(_ap_t6580); });
#line 521 "src/codegen/cpp_export.tr"
            me = (me + 1LL);
        }
#line 522 "src/codegen/cpp_export.tr"
        ({ __auto_type _ord_t6582 = (m->ret_ty); TrStr _at_t6581 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("tr__", 4LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (margs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6583 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_ret_stmt(self, _ord_t6582, _at_t6581)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(") { ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(wb, _sbt_t6583); _tr_str_release(_at_t6581); _tr_str_release(_sbt_t6583); });
        _tr_str_release(margs);
    }
#line 523 "src/codegen/cpp_export.tr"
    StringBuilder_append(wb, _tr_str_lit_len("    };\n", 7LL));
#line 524 "src/codegen/cpp_export.tr"
    TrStr _trret1549 = StringObj_as_str(StringBuilder_to_string(wb));
    _tr_str_release(C);
    StringBuilder__tr_fn_free(wb);
    return _trret1549;
}

__attribute__((hot)) TrStr CppExporter_generate_cpp_class_bridges(CppExporter* self, HirProgram* prog) {
#line 530 "src/codegen/cpp_export.tr"
    CppExporter_collect_export_classes(self, prog);
#line 531 "src/codegen/cpp_export.tr"
    StringBuilder* b = StringBuilder_init(2048LL);
#line 532 "src/codegen/cpp_export.tr"
    StringBuilder_append(b, _tr_str_lit_len("\n/* --export-cpp: stable extern-\"C\" bridges over the ARC runtime (for the C++ header). */\n", 90LL));
#line 533 "src/codegen/cpp_export.tr"
    StringBuilder_append(b, _tr_str_lit_len("#ifdef __cplusplus\nextern \"C\" {\n#endif\n", 39LL));
#line 534 "src/codegen/cpp_export.tr"
    long long ci = 0LL;
#line 535 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
#line 536 "src/codegen/cpp_export.tr"
        while ((ci < prog->classes->len)) {
#line 537 "src/codegen/cpp_export.tr"
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
#line 538 "src/codegen/cpp_export.tr"
            ci = (ci + 1LL);
#line 539 "src/codegen/cpp_export.tr"
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
#line 539 "src/codegen/cpp_export.tr"
                continue;
            }
#line 540 "src/codegen/cpp_export.tr"
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
#line 540 "src/codegen/cpp_export.tr"
                continue;
            }
#line 541 "src/codegen/cpp_export.tr"
            TrStr C = _tr_str_retain(c->name);
#line 542 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6584 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__", 20LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_retain(void* p){ return _tr_obj_retain(p); }\n", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6584); _tr_str_release(_sbt_t6584); });
#line 543 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6585 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__", 19LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_release(void* p){ _tr_obj_release(p, _trdrop_", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6585); _tr_str_release(_sbt_t6585); });
#line 544 "src/codegen/cpp_export.tr"
            long long iidx = CppExporter__cpp_init_idx(self, c);
#line 545 "src/codegen/cpp_export.tr"
            if ((iidx >= 0LL)) {
#line 546 "src/codegen/cpp_export.tr"
                HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
#line 547 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6586 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__", 20LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6586); _tr_str_release(_sbt_t6586); });
#line 548 "src/codegen/cpp_export.tr"
                long long ip = 0LL;
#line 549 "src/codegen/cpp_export.tr"
                long long ie = 0LL;
#line 550 "src/codegen/cpp_export.tr"
                TrStr iargs = _tr_str_lit_len("", 0LL);
#line 551 "src/codegen/cpp_export.tr"
                while ((ip < im->params->len)) {
#line 552 "src/codegen/cpp_export.tr"
                    HirParam* p = ((HirParam*)List_ptr_get(im->params, ip));
#line 553 "src/codegen/cpp_export.tr"
                    ip = (ip + 1LL);
#line 554 "src/codegen/cpp_export.tr"
                    if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 554 "src/codegen/cpp_export.tr"
                        continue;
                    }
#line 555 "src/codegen/cpp_export.tr"
                    if ((ie > 0LL)) {
#line 556 "src/codegen/cpp_export.tr"
                        StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
#line 557 "src/codegen/cpp_export.tr"
                        ({ TrStr _ap_t6587 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&iargs, _ap_t6587); });
                    }
#line 558 "src/codegen/cpp_export.tr"
                    ({ __auto_type _ord_t6588 = (({ TrStr _tr_cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6589 = (({ TrStr _tr_cl = (_ord_t6588); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p->name)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6589); _tr_str_release(_sbt_t6589); });
#line 559 "src/codegen/cpp_export.tr"
                    ({ TrStr _ap_t6590 = (p->name); _tr_str_append(&iargs, _ap_t6590); });
#line 560 "src/codegen/cpp_export.tr"
                    ie = (ie + 1LL);
                }
#line 561 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6591 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("){ return (void*)", 17LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_init(", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (iargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6591); _tr_str_release(_sbt_t6591); });
                _tr_str_release(iargs);
            }
#line 562 "src/codegen/cpp_export.tr"
            long long mi = 0LL;
#line 563 "src/codegen/cpp_export.tr"
            while ((mi < c->methods->len)) {
#line 564 "src/codegen/cpp_export.tr"
                HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
#line 565 "src/codegen/cpp_export.tr"
                mi = (mi + 1LL);
#line 566 "src/codegen/cpp_export.tr"
                if (_is_invalid_ptr(((unsigned long long)(m)))) {
#line 566 "src/codegen/cpp_export.tr"
                    continue;
                }
#line 567 "src/codegen/cpp_export.tr"
                if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_withv((m->name), (_tr_str_lit_len("__", 2LL))))) {
#line 567 "src/codegen/cpp_export.tr"
                    continue;
                }
#line 568 "src/codegen/cpp_export.tr"
                if ((!CppExporter__cpp_method_ok(self, m))) {
#line 568 "src/codegen/cpp_export.tr"
                    continue;
                }
#line 569 "src/codegen/cpp_export.tr"
                ({ __auto_type _ord_t6592 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6593 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6592); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(void* s", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6593); _tr_str_release(_sbt_t6593); });
#line 570 "src/codegen/cpp_export.tr"
                TrStr margs = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)s", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 571 "src/codegen/cpp_export.tr"
                long long pj = 0LL;
#line 572 "src/codegen/cpp_export.tr"
                while ((pj < m->params->len)) {
#line 573 "src/codegen/cpp_export.tr"
                    HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
#line 574 "src/codegen/cpp_export.tr"
                    pj = (pj + 1LL);
#line 575 "src/codegen/cpp_export.tr"
                    if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
#line 575 "src/codegen/cpp_export.tr"
                        continue;
                    }
#line 576 "src/codegen/cpp_export.tr"
                    ({ __auto_type _ord_t6594 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(", ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6595 = (({ TrStr _tr_cl = (_ord_t6594); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mp->name)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6595); _tr_str_release(_sbt_t6595); });
#line 577 "src/codegen/cpp_export.tr"
                    ({ TrStr _ap_t6596 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t6597 = (CppExporter__cpp_c_arg(self, mp->ty, mp->name)); _tr_str_append(&margs, _ap_t6596); _tr_str_append(&margs, _ap_t6597); _tr_str_release(_ap_t6597); });
                }
#line 578 "src/codegen/cpp_export.tr"
                StringBuilder_append(b, _tr_str_lit_len("){ ", 3LL));
#line 579 "src/codegen/cpp_export.tr"
                if (({ TrStr _cmpa_t6598 = (CppExporter__cpp_ty_kind(self, m->ret_ty)); TrStr _cmpb_t6599 = (_tr_str_lit_len("void", 4LL)); _Bool _cmpr_t6600 = _tr_str_eqv(_cmpa_t6598, _cmpb_t6599); _tr_str_release(_cmpa_t6598); _cmpr_t6600; })) {
#line 580 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6601 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((C), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (margs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6601); _tr_str_release(_sbt_t6601); });
                } else {
#line 582 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6602 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (margs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6602); _tr_str_release(_sbt_t6602); });
                }
                _tr_str_release(margs);
            }
            _tr_str_release(C);
        }
    }
#line 584 "src/codegen/cpp_export.tr"
    long long fi = 0LL;
#line 585 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
#line 586 "src/codegen/cpp_export.tr"
        while ((fi < prog->functions->len)) {
#line 587 "src/codegen/cpp_export.tr"
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, fi));
#line 588 "src/codegen/cpp_export.tr"
            fi = (fi + 1LL);
#line 589 "src/codegen/cpp_export.tr"
            if (_is_invalid_ptr(((unsigned long long)(f)))) {
#line 589 "src/codegen/cpp_export.tr"
                continue;
            }
#line 590 "src/codegen/cpp_export.tr"
            if ((!(f->is_export && (!f->is_extern)))) {
#line 590 "src/codegen/cpp_export.tr"
                continue;
            }
#line 591 "src/codegen/cpp_export.tr"
            if (((!CppExporter__cpp_export_fn_ok(self, f)) || (!CppExporter__cpp_fn_needs_bridge(self, f)))) {
#line 591 "src/codegen/cpp_export.tr"
                continue;
            }
#line 592 "src/codegen/cpp_export.tr"
            ({ __auto_type _ord_t6603 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6604 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6603); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6604); _tr_str_release(_sbt_t6604); });
#line 593 "src/codegen/cpp_export.tr"
            TrStr fargs = _tr_str_lit_len("", 0LL);
#line 594 "src/codegen/cpp_export.tr"
            long long pi2 = 0LL;
#line 595 "src/codegen/cpp_export.tr"
            while ((pi2 < f->params->len)) {
#line 596 "src/codegen/cpp_export.tr"
                HirParam* p = ((HirParam*)List_ptr_get(f->params, pi2));
#line 597 "src/codegen/cpp_export.tr"
                TrStr pn = _tr_str_retain(p->name);
#line 598 "src/codegen/cpp_export.tr"
                if (_tr_str_eqv((pn), (_tr_str_lit_len("", 0LL)))) {
#line 598 "src/codegen/cpp_export.tr"
                    TrStr _strtmp_t6605 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi2)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                    _tr_str_release(pn);
                    pn = _strtmp_t6605;
                }
#line 599 "src/codegen/cpp_export.tr"
                if ((pi2 > 0LL)) {
#line 600 "src/codegen/cpp_export.tr"
                    StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
#line 601 "src/codegen/cpp_export.tr"
                    ({ TrStr _ap_t6606 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&fargs, _ap_t6606); });
                }
#line 602 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6607 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (CppExporter__cpp_c_ty(self, p->ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6607); _tr_str_release(_sbt_t6607); });
#line 603 "src/codegen/cpp_export.tr"
                ({ TrStr _ap_t6608 = (CppExporter__cpp_c_arg(self, p->ty, pn)); _tr_str_append(&fargs, _ap_t6608); _tr_str_release(_ap_t6608); });
#line 604 "src/codegen/cpp_export.tr"
                pi2 = (pi2 + 1LL);
                _tr_str_release(pn);
            }
#line 605 "src/codegen/cpp_export.tr"
            StringBuilder_append(b, _tr_str_lit_len("){ ", 3LL));
#line 606 "src/codegen/cpp_export.tr"
            TrStr frk = CppExporter__cpp_ty_kind(self, f->ret_ty);
#line 607 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((frk), (_tr_str_lit_len("void", 4LL)))) {
#line 608 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6609 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((f->name), (_tr_str_lit_len("(", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6609); _tr_str_release(_sbt_t6609); });
            } else if (((_tr_str_eqv((frk), (_tr_str_lit_len("class", 5LL))) || _tr_str_eqv((frk), (_tr_str_lit_len("list", 4LL)))) || _tr_str_eqv((frk), (_tr_str_lit_len("dict", 4LL))))) {
#line 610 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6610 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (void*)", 14LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6610); _tr_str_release(_sbt_t6610); });
            } else {
#line 612 "src/codegen/cpp_export.tr"
                ({ TrStr _sbt_t6611 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6611); _tr_str_release(_sbt_t6611); });
            }
            _tr_str_release(fargs);
            _tr_str_release(frk);
        }
    }
#line 613 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6612 = (CppExporter__cpp_list_bridges(self)); StringBuilder_append(b, _sbt_t6612); _tr_str_release(_sbt_t6612); });
#line 614 "src/codegen/cpp_export.tr"
    StringBuilder_append(b, _tr_str_lit_len("#ifdef __cplusplus\n}\n#endif\n", 28LL));
#line 615 "src/codegen/cpp_export.tr"
    return StringObj_as_str(StringBuilder_to_string(b));
}

__attribute__((hot)) TrStr CppExporter__cpp_list_bridges(CppExporter* self) {
#line 620 "src/codegen/cpp_export.tr"
    StringBuilder* b = StringBuilder_init(512LL);
#line 621 "src/codegen/cpp_export.tr"
    List_TrStr* keys = _tr_dict_keys(self->list_elems);
#line 622 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 623 "src/codegen/cpp_export.tr"
    while ((i < keys->len)) {
#line 624 "src/codegen/cpp_export.tr"
        TrStr sfx = List_TrStr_get(keys, i);
#line 625 "src/codegen/cpp_export.tr"
        i = (i + 1LL);
#line 626 "src/codegen/cpp_export.tr"
        TrStr ec = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->list_elems, _tr_strz(sfx))));
#line 627 "src/codegen/cpp_export.tr"
        TrStr L = _tr_strx_concatv((_tr_str_lit_len("List_", 5LL)), (sfx));
#line 628 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6613 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT long long tr__list_", 29LL)), (sfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_len(void* l){ return (long long)((", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)l)->len; }\n", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6613); _tr_str_release(_sbt_t6613); });
#line 629 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6614 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), (ec))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__list_", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(void* l, long long i){ return ", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get((", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)l, i); }\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6614); _tr_str_release(_sbt_t6614); });
#line 630 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6615 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__list_", 25LL)), (sfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(void){ return (void*)", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(); }\n", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6615); _tr_str_release(_sbt_t6615); });
#line 631 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6616 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__list_", 24LL)), (sfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_push(void* l, ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ec)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" v){ ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_append((", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)l, v); }\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6616); _tr_str_release(_sbt_t6616); });
#line 632 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6617 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__list_", 24LL)), (sfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(void* l){ ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free((", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (L)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)l); }\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6617); _tr_str_release(_sbt_t6617); });
        _tr_str_release(sfx);
        _tr_str_release(ec);
        _tr_str_release(L);
    }
#line 635 "src/codegen/cpp_export.tr"
    List_TrStr* dk = _tr_dict_keys(self->dict_variants);
#line 636 "src/codegen/cpp_export.tr"
    long long di = 0LL;
#line 637 "src/codegen/cpp_export.tr"
    while ((di < dk->len)) {
#line 638 "src/codegen/cpp_export.tr"
        TrStr V = List_TrStr_get(dk, di);
#line 639 "src/codegen/cpp_export.tr"
        di = (di + 1LL);
#line 640 "src/codegen/cpp_export.tr"
        TrStr kk = _tr_str_slicev((V), 0LL, 1LL);
#line 641 "src/codegen/cpp_export.tr"
        TrStr vk = _tr_str_slicev((V), 2LL, _tr_str_lenv((V)));
#line 642 "src/codegen/cpp_export.tr"
        TrStr kc = _tr_str_lit_len("long long", 9LL);
#line 643 "src/codegen/cpp_export.tr"
        TrStr newfn = _tr_str_lit_len("_tr_idict_new(8)", 16LL);
#line 644 "src/codegen/cpp_export.tr"
        TrStr keysfn = _tr_str_lit_len("_tr_idict_keys((TrIDict*)d)", 27LL);
#line 645 "src/codegen/cpp_export.tr"
        TrStr getraw = _tr_str_lit_len("_tr_idict_get((TrIDict*)d, k)", 29LL);
#line 646 "src/codegen/cpp_export.tr"
        TrStr setpre = _tr_str_lit_len("_tr_idict_set((TrIDict*)d, k, ", 30LL);
#line 647 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((kk), (_tr_str_lit_len("s", 1LL)))) {
#line 648 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6618 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(kc);
            kc = _strtmp_t6618;
#line 649 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6619 = _tr_str_lit_len("_tr_dict_new(8)", 15LL);
            _tr_str_release(newfn);
            newfn = _strtmp_t6619;
#line 650 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6620 = _tr_str_lit_len("_tr_dict_keys((TrMap*)d)", 24LL);
            _tr_str_release(keysfn);
            keysfn = _strtmp_t6620;
#line 651 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6621 = _tr_str_lit_len("_tr_dict_get((TrMap*)d, _tr_strz(k))", 36LL);
            _tr_str_release(getraw);
            getraw = _strtmp_t6621;
#line 652 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6622 = _tr_str_lit_len("_tr_dict_set((TrMap*)d, _tr_strz(k), ", 37LL);
            _tr_str_release(setpre);
            setpre = _strtmp_t6622;
        }
#line 653 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6623 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__dict_", 25LL)), (V))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(void){ return (void*)", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (newfn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6623); _tr_str_release(_sbt_t6623); });
#line 654 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6624 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void* tr__dict_", 25LL)), (V))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_keys(void* d){ return (void*)", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (keysfn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6624); _tr_str_release(_sbt_t6624); });
#line 655 "src/codegen/cpp_export.tr"
        TrStr vc = _tr_str_lit_len("long long", 9LL);
#line 656 "src/codegen/cpp_export.tr"
        TrStr getval = _tr_strx_concatv((_tr_str_lit_len("(long long)(uintptr_t)", 22LL)), (getraw));
#line 657 "src/codegen/cpp_export.tr"
        TrStr setval = _tr_strx_concatv((setpre), (_tr_str_lit_len("v)", 2LL)));
#line 658 "src/codegen/cpp_export.tr"
        if (_tr_str_eqv((vk), (_tr_str_lit_len("f64", 3LL)))) {
#line 659 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6625 = _tr_str_lit_len("double", 6LL);
            _tr_str_release(vc);
            vc = _strtmp_t6625;
#line 660 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6626 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("_tr_ptr_to_f64(", 15LL)), (getraw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(getval);
            getval = _strtmp_t6626;
#line 661 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6627 = _tr_strx_concatv((setpre), (_tr_str_lit_len("_tr_f64_to_ptr(v))", 18LL)));
            _tr_str_release(setval);
            setval = _strtmp_t6627;
        } else if (_tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL)))) {
#line 663 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6628 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(vc);
            vc = _strtmp_t6628;
#line 664 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6629 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("_tr_str_unbox(", 14LL)), (getraw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(getval);
            getval = _strtmp_t6629;
#line 665 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6630 = _tr_strx_concatv((setpre), (_tr_str_lit_len("_tr_str_box(v))", 15LL)));
            _tr_str_release(setval);
            setval = _strtmp_t6630;
        }
#line 666 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6631 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT ", 10LL)), (vc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__dict_", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (V)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(void* d, ", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" k){ return ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (getval)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6631); _tr_str_release(_sbt_t6631); });
#line 667 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6632 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__dict_", 24LL)), (V))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_set(void* d, ", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" k, ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" v){ ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (setval)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6632); _tr_str_release(_sbt_t6632); });
#line 668 "src/codegen/cpp_export.tr"
        TrStr freefn = _tr_str_lit_len("Dict_free((Dict*)d)", 19LL);
#line 669 "src/codegen/cpp_export.tr"
        if ((_tr_str_eqv((kk), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL))))) {
#line 669 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6633 = _tr_str_lit_len("Dict_free_strval((Dict*)d)", 26LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t6633;
        } else if ((_tr_str_eqv((kk), (_tr_str_lit_len("i", 1LL))) && _tr_str_eqv((vk), (_tr_str_lit_len("str", 3LL))))) {
#line 670 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6634 = _tr_str_lit_len("_tr_idict_free_strval((TrIDict*)d)", 34LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t6634;
        } else if (_tr_str_eqv((kk), (_tr_str_lit_len("i", 1LL)))) {
#line 671 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6635 = _tr_str_lit_len("_tr_idict_free((TrIDict*)d)", 27LL);
            _tr_str_release(freefn);
            freefn = _strtmp_t6635;
        }
#line 672 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6636 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("TR_EXPORT void tr__dict_", 24LL)), (V))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(void* d){ ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (freefn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; }\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6636); _tr_str_release(_sbt_t6636); });
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
#line 675 "src/codegen/cpp_export.tr"
    if (self->needs_list_ptr) {
#line 676 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__list_ptr_len(void* l){ return (long long)((List_ptr*)l)->len; }\n", 88LL));
#line 677 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT void* tr__list_ptr_get(void* l, long long i){ return List_ptr_get((List_ptr*)l, i); }\n", 96LL));
#line 678 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT void tr__list_ptr_free_shallow(void* l){ List_ptr_free((List_ptr*)l); }\n", 82LL));
    }
#line 681 "src/codegen/cpp_export.tr"
    if (self->needs_tuple) {
#line 682 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__box_str(TrStr s){ return (long long)(uintptr_t)_tr_str_box(s); }\n", 89LL));
#line 683 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT TrStr tr__unbox_str(long long p){ return _tr_str_unbox((void*)(uintptr_t)p); }\n", 89LL));
#line 684 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT long long tr__box_f64(double d){ return (long long)(uintptr_t)_tr_f64_to_ptr(d); }\n", 93LL));
#line 685 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("TR_EXPORT double tr__unbox_f64(long long p){ return _tr_ptr_to_f64((void*)(uintptr_t)p); }\n", 91LL));
    }
#line 686 "src/codegen/cpp_export.tr"
    TrStr _trret1550 = StringObj_as_str(StringBuilder_to_string(b));
    List_TrStr_free(keys);
    List_TrStr_free(dk);
    StringBuilder__tr_fn_free(b);
    return _trret1550;
}

__attribute__((hot)) TrStr CppExporter_generate_export_cpp_header(CppExporter* self, HirProgram* prog, TrStr ns) {
#line 694 "src/codegen/cpp_export.tr"
    CppExporter_collect_export_classes(self, prog);
#line 695 "src/codegen/cpp_export.tr"
    StringBuilder* sb = StringBuilder_init(4096LL);
#line 696 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* Generated by `tauraroc --export-cpp`. Call Tauraro from C++: include this,\n", 78LL));
#line 697 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6637 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("   link the Tauraro library, and use the `", 42LL)), (ns))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("::` wrappers. */\n", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6637); _tr_str_release(_sbt_t6637); });
#line 698 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#ifndef TAURARO_CPP_EXPORTS_H\n#define TAURARO_CPP_EXPORTS_H\n\n", 61LL));
#line 699 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#include <string>\n#include <cstdint>\n", 37LL));
#line 700 "src/codegen/cpp_export.tr"
    if ((_tr_dict_keys(self->list_elems)->len > 0LL)) {
#line 700 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("#include <vector>\n", 18LL));
    }
#line 701 "src/codegen/cpp_export.tr"
    if ((_tr_dict_keys(self->dict_variants)->len > 0LL)) {
#line 701 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("#include <map>\n", 15LL));
    }
#line 702 "src/codegen/cpp_export.tr"
    if (self->needs_tuple) {
#line 702 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("#include <tuple>\n", 17LL));
    }
#line 703 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
#line 706 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("extern \"C\" {\n", 13LL));
#line 707 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("typedef struct { char* data; long* rc; } TrStr;\n", 48LL));
#line 708 "src/codegen/cpp_export.tr"
    if (self->needs_tuple) {
#line 708 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("typedef struct { long long data[8]; } TrTuple;\n", 47LL));
    }
#line 710 "src/codegen/cpp_export.tr"
    List_TrStr* lk = _tr_dict_keys(self->list_elems);
#line 711 "src/codegen/cpp_export.tr"
    long long li = 0LL;
#line 712 "src/codegen/cpp_export.tr"
    while ((li < lk->len)) {
#line 713 "src/codegen/cpp_export.tr"
        TrStr sfx = List_TrStr_get(lk, li);
#line 714 "src/codegen/cpp_export.tr"
        li = (li + 1LL);
#line 715 "src/codegen/cpp_export.tr"
        TrStr ec = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->list_elems, _tr_strz(sfx))));
#line 716 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6638 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("long long tr__list_", 19LL)), (sfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_len(void*); ", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ec)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__list_", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(void*, long long); void* tr__list_", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(void); void tr__list_", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_push(void*, ", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ec)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); void tr__list_", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sfx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(void*);\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6638); _tr_str_release(_sbt_t6638); });
        _tr_str_release(sfx);
        _tr_str_release(ec);
    }
#line 718 "src/codegen/cpp_export.tr"
    List_TrStr* dkk = _tr_dict_keys(self->dict_variants);
#line 719 "src/codegen/cpp_export.tr"
    long long dii = 0LL;
#line 720 "src/codegen/cpp_export.tr"
    while ((dii < dkk->len)) {
#line 721 "src/codegen/cpp_export.tr"
        TrStr V = List_TrStr_get(dkk, dii);
#line 722 "src/codegen/cpp_export.tr"
        dii = (dii + 1LL);
#line 723 "src/codegen/cpp_export.tr"
        TrStr vc = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->dict_variants, _tr_strz(V))));
#line 724 "src/codegen/cpp_export.tr"
        TrStr kc = _tr_str_lit_len("long long", 9LL);
#line 725 "src/codegen/cpp_export.tr"
        if (({ TrStr _cmpa_t6639 = (_tr_str_slicev((V), 0LL, 1LL)); TrStr _cmpb_t6640 = (_tr_str_lit_len("s", 1LL)); _Bool _cmpr_t6641 = _tr_str_eqv(_cmpa_t6639, _cmpb_t6640); _tr_str_release(_cmpa_t6639); _cmpr_t6641; })) {
#line 725 "src/codegen/cpp_export.tr"
            TrStr _strtmp_t6642 = _tr_str_lit_len("TrStr", 5LL);
            _tr_str_release(kc);
            kc = _strtmp_t6642;
        }
#line 726 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6643 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__dict_", 15LL)), (V))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(void); void* tr__dict_", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (V)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_keys(void*); ", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__dict_", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (V)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_get(void*, ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); void tr__dict_", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (V)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_set(void*, ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); void tr__dict_", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (V)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_free(void*);\n", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6643); _tr_str_release(_sbt_t6643); });
        _tr_str_release(V);
        _tr_str_release(vc);
        _tr_str_release(kc);
    }
#line 728 "src/codegen/cpp_export.tr"
    if (self->needs_list_ptr) {
#line 729 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("long long tr__list_ptr_len(void*); void* tr__list_ptr_get(void*, long long); void tr__list_ptr_free_shallow(void*);\n", 116LL));
    }
#line 731 "src/codegen/cpp_export.tr"
    if (self->needs_tuple) {
#line 732 "src/codegen/cpp_export.tr"
        StringBuilder_append(sb, _tr_str_lit_len("long long tr__box_str(TrStr); TrStr tr__unbox_str(long long); long long tr__box_f64(double); double tr__unbox_f64(long long);\n", 126LL));
    }
#line 734 "src/codegen/cpp_export.tr"
    long long ci = 0LL;
#line 735 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
#line 736 "src/codegen/cpp_export.tr"
        while ((ci < prog->classes->len)) {
#line 737 "src/codegen/cpp_export.tr"
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
#line 738 "src/codegen/cpp_export.tr"
            ci = (ci + 1LL);
#line 739 "src/codegen/cpp_export.tr"
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
#line 739 "src/codegen/cpp_export.tr"
                continue;
            }
#line 740 "src/codegen/cpp_export.tr"
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
#line 740 "src/codegen/cpp_export.tr"
                continue;
            }
#line 741 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6644 = (CppExporter__cpp_class_bridge_protos(self, c)); StringBuilder_append(sb, _sbt_t6644); _tr_str_release(_sbt_t6644); });
        }
    }
#line 743 "src/codegen/cpp_export.tr"
    long long i = 0LL;
#line 744 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
#line 745 "src/codegen/cpp_export.tr"
        while ((i < prog->functions->len)) {
#line 746 "src/codegen/cpp_export.tr"
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
#line 747 "src/codegen/cpp_export.tr"
            if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
#line 748 "src/codegen/cpp_export.tr"
                if (((f->is_export && (!f->is_extern)) && CppExporter__cpp_export_fn_ok(self, f))) {
#line 749 "src/codegen/cpp_export.tr"
                    if (CppExporter__cpp_fn_needs_bridge(self, f)) {
#line 750 "src/codegen/cpp_export.tr"
                        ({ TrStr _sbt_t6645 = (CppExporter__cpp_free_fn_bridge_proto(self, f)); StringBuilder_append(sb, _sbt_t6645); _tr_str_release(_sbt_t6645); });
                    } else {
#line 752 "src/codegen/cpp_export.tr"
                        TrStr sig = CGenerator_gen_func_sig(self->gen, f, _tr_str_lit_len("", 0LL));
#line 753 "src/codegen/cpp_export.tr"
                        if (_tr_str_starts_withv((sig), (_tr_str_lit_len("TR_EXPORT ", 10LL)))) {
#line 753 "src/codegen/cpp_export.tr"
                            TrStr _strtmp_t6646 = _tr_str_slicev((sig), 10LL, _tr_str_lenv((sig)));
                            _tr_str_release(sig);
                            sig = _strtmp_t6646;
                        }
#line 754 "src/codegen/cpp_export.tr"
                        ({ TrStr _sbt_t6647 = (_tr_strx_concatv((sig), (_tr_str_lit_len(";\n", 2LL)))); StringBuilder_append(sb, _sbt_t6647); _tr_str_release(_sbt_t6647); });
                        _tr_str_release(sig);
                    }
                }
            }
#line 755 "src/codegen/cpp_export.tr"
            i = (i + 1LL);
        }
    }
#line 756 "src/codegen/cpp_export.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n\n", 3LL));
#line 758 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6648 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("namespace ", 10LL)), (ns))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" {\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6648); _tr_str_release(_sbt_t6648); });
#line 759 "src/codegen/cpp_export.tr"
    ci = 0LL;
#line 760 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->classes)))))) {
#line 761 "src/codegen/cpp_export.tr"
        while ((ci < prog->classes->len)) {
#line 762 "src/codegen/cpp_export.tr"
            HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
#line 763 "src/codegen/cpp_export.tr"
            ci = (ci + 1LL);
#line 764 "src/codegen/cpp_export.tr"
            if (_is_invalid_ptr(((unsigned long long)(c)))) {
#line 764 "src/codegen/cpp_export.tr"
                continue;
            }
#line 765 "src/codegen/cpp_export.tr"
            if ((!_tr_dict_contains(self->cls_set, _tr_strz(c->name)))) {
#line 765 "src/codegen/cpp_export.tr"
                continue;
            }
#line 766 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6649 = (CppExporter__cpp_class_decl(self, c)); StringBuilder_append(sb, _sbt_t6649); _tr_str_release(_sbt_t6649); });
        }
    }
#line 767 "src/codegen/cpp_export.tr"
    i = 0LL;
#line 768 "src/codegen/cpp_export.tr"
    if (((!_is_invalid_ptr(((unsigned long long)(prog)))) && (!_is_invalid_ptr(((unsigned long long)(prog->functions)))))) {
#line 769 "src/codegen/cpp_export.tr"
        while ((i < prog->functions->len)) {
#line 770 "src/codegen/cpp_export.tr"
            HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
#line 771 "src/codegen/cpp_export.tr"
            if ((!_is_invalid_ptr(((unsigned long long)(f))))) {
#line 772 "src/codegen/cpp_export.tr"
                if (((f->is_export && (!f->is_extern)) && CppExporter__cpp_export_fn_ok(self, f))) {
#line 773 "src/codegen/cpp_export.tr"
                    ({ TrStr _sbt_t6650 = (CppExporter__cpp_wrap_fn(self, f)); StringBuilder_append(sb, _sbt_t6650); _tr_str_release(_sbt_t6650); });
                }
            }
#line 774 "src/codegen/cpp_export.tr"
            i = (i + 1LL);
        }
    }
#line 775 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6651 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("}  // namespace ", 16LL)), (ns))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n\n#endif\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t6651); _tr_str_release(_sbt_t6651); });
#line 776 "src/codegen/cpp_export.tr"
    TrStr _trret1551 = StringObj_as_str(StringBuilder_to_string(sb));
    List_TrStr_free(lk);
    List_TrStr_free(dkk);
    StringBuilder__tr_fn_free(sb);
    return _trret1551;
}

__attribute__((hot)) TrStr CppExporter__cpp_class_bridge_protos(CppExporter* self, HirClass* c) {
#line 780 "src/codegen/cpp_export.tr"
    TrStr C = _tr_str_retain(c->name);
#line 781 "src/codegen/cpp_export.tr"
    StringBuilder* b = StringBuilder_init(256LL);
#line 782 "src/codegen/cpp_export.tr"
    ({ TrStr _sbt_t6652 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__", 10LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_retain(void*); void tr__", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_release(void*);\n", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6652); _tr_str_release(_sbt_t6652); });
#line 783 "src/codegen/cpp_export.tr"
    long long iidx = CppExporter__cpp_init_idx(self, c);
#line 784 "src/codegen/cpp_export.tr"
    if ((iidx >= 0LL)) {
#line 785 "src/codegen/cpp_export.tr"
        HirFunction* im = ((HirFunction*)List_ptr_get(c->methods, iidx));
#line 786 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6653 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void* tr__", 10LL)), (C))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_new(", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6653); _tr_str_release(_sbt_t6653); });
#line 787 "src/codegen/cpp_export.tr"
        long long ip = 0LL;
#line 788 "src/codegen/cpp_export.tr"
        long long ie = 0LL;
#line 789 "src/codegen/cpp_export.tr"
        while ((ip < im->params->len)) {
#line 790 "src/codegen/cpp_export.tr"
            HirParam* p = ((HirParam*)List_ptr_get(im->params, ip));
#line 791 "src/codegen/cpp_export.tr"
            ip = (ip + 1LL);
#line 792 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 792 "src/codegen/cpp_export.tr"
                continue;
            }
#line 793 "src/codegen/cpp_export.tr"
            if ((ie > 0LL)) {
#line 793 "src/codegen/cpp_export.tr"
                StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
            }
#line 794 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6654 = (CppExporter__cpp_c_ty(self, p->ty)); StringBuilder_append(b, _sbt_t6654); _tr_str_release(_sbt_t6654); });
#line 795 "src/codegen/cpp_export.tr"
            ie = (ie + 1LL);
        }
#line 796 "src/codegen/cpp_export.tr"
        if ((ie == 0LL)) {
#line 796 "src/codegen/cpp_export.tr"
            StringBuilder_append(b, _tr_str_lit_len("void", 4LL));
        }
#line 797 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
    }
#line 798 "src/codegen/cpp_export.tr"
    long long mi = 0LL;
#line 799 "src/codegen/cpp_export.tr"
    while ((mi < c->methods->len)) {
#line 800 "src/codegen/cpp_export.tr"
        HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
#line 801 "src/codegen/cpp_export.tr"
        mi = (mi + 1LL);
#line 802 "src/codegen/cpp_export.tr"
        if (_is_invalid_ptr(((unsigned long long)(m)))) {
#line 802 "src/codegen/cpp_export.tr"
            continue;
        }
#line 803 "src/codegen/cpp_export.tr"
        if ((_tr_str_eqv((m->name), (_tr_str_lit_len("init", 4LL))) || _tr_str_starts_withv((m->name), (_tr_str_lit_len("__", 2LL))))) {
#line 803 "src/codegen/cpp_export.tr"
            continue;
        }
#line 804 "src/codegen/cpp_export.tr"
        if ((!CppExporter__cpp_method_ok(self, m))) {
#line 804 "src/codegen/cpp_export.tr"
            continue;
        }
#line 805 "src/codegen/cpp_export.tr"
        ({ __auto_type _ord_t6655 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (CppExporter__cpp_c_ty(self, m->ret_ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (C)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6656 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6655); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(void*", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6656); _tr_str_release(_sbt_t6656); });
#line 806 "src/codegen/cpp_export.tr"
        long long pj = 0LL;
#line 807 "src/codegen/cpp_export.tr"
        while ((pj < m->params->len)) {
#line 808 "src/codegen/cpp_export.tr"
            HirParam* mp = ((HirParam*)List_ptr_get(m->params, pj));
#line 809 "src/codegen/cpp_export.tr"
            pj = (pj + 1LL);
#line 810 "src/codegen/cpp_export.tr"
            if (_tr_str_eqv((mp->name), (_tr_str_lit_len("self", 4LL)))) {
#line 810 "src/codegen/cpp_export.tr"
                continue;
            }
#line 811 "src/codegen/cpp_export.tr"
            ({ TrStr _sbt_t6657 = (({ TrStr _tr_cr = (CppExporter__cpp_c_ty(self, mp->ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(", ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(b, _sbt_t6657); _tr_str_release(_sbt_t6657); });
        }
#line 812 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
    }
#line 813 "src/codegen/cpp_export.tr"
    TrStr _trret1552 = StringObj_as_str(StringBuilder_to_string(b));
    _tr_str_release(C);
    StringBuilder__tr_fn_free(b);
    return _trret1552;
}

__attribute__((hot)) TrStr CppExporter__cpp_free_fn_bridge_proto(CppExporter* self, HirFunction* f) {
#line 816 "src/codegen/cpp_export.tr"
    StringBuilder* b = StringBuilder_init(128LL);
#line 817 "src/codegen/cpp_export.tr"
    ({ __auto_type _ord_t6658 = (({ TrStr _tr_cl = (CppExporter__cpp_c_ty(self, f->ret_ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" tr__", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t6659 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6658); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(b, _sbt_t6659); _tr_str_release(_sbt_t6659); });
#line 818 "src/codegen/cpp_export.tr"
    long long pi = 0LL;
#line 819 "src/codegen/cpp_export.tr"
    long long e = 0LL;
#line 820 "src/codegen/cpp_export.tr"
    while ((pi < f->params->len)) {
#line 821 "src/codegen/cpp_export.tr"
        if ((e > 0LL)) {
#line 821 "src/codegen/cpp_export.tr"
            StringBuilder_append(b, _tr_str_lit_len(", ", 2LL));
        }
#line 822 "src/codegen/cpp_export.tr"
        ({ TrStr _sbt_t6660 = (CppExporter__cpp_c_ty(self, ((HirParam*)List_ptr_get(f->params, pi))->ty)); StringBuilder_append(b, _sbt_t6660); _tr_str_release(_sbt_t6660); });
#line 823 "src/codegen/cpp_export.tr"
        e = (e + 1LL);
#line 824 "src/codegen/cpp_export.tr"
        pi = (pi + 1LL);
    }
#line 825 "src/codegen/cpp_export.tr"
    if ((e == 0LL)) {
#line 825 "src/codegen/cpp_export.tr"
        StringBuilder_append(b, _tr_str_lit_len("void", 4LL));
    }
#line 826 "src/codegen/cpp_export.tr"
    StringBuilder_append(b, _tr_str_lit_len(");\n", 3LL));
#line 827 "src/codegen/cpp_export.tr"
    return StringObj_as_str(StringBuilder_to_string(b));
}

