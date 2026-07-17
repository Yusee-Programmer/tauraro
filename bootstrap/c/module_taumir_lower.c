#include "tauraro_types.h"

AstType** box_asttype_lir(AstType* t);
long long _f64_bits(double v);
long long _promote_f(LFunc* lf, long long v);
TrStr _print_i64_sym();
bool _is_list_tag(long long t);
bool _is_set_tag(long long t);
TrStr _set_sym(long long t, TrStr op);
bool _is_dict_tag(long long t);
bool _dict_key_is_str(long long t);
long long _dict_val_tag(long long t);
TrStr _dict_new_sym(long long t);
TrStr _dict_sym(long long t, TrStr op);
long long _list_elem_tag(long long t);
long long _list_tag_for_elem(long long et);
bool _is_cmp_op(TrStr op);
bool _is_int_typename(TrStr n);
long long _ast_type_tag(AstType* ty);
bool _is_null_str(TrStr s);
TrStr _own(TrStr s);
long long _tag_of(LModule* m, AstType* ty);
AstType* _subst_ty(LModule* m, AstType* ty);
long long _prog_generic_class_index(LModule* m, TrStr name);
TrStr _mangle_generic(LModule* m, AstType* ty);
TrStr _ensure_generic_class(LModule* m, AstType* ty);
TrStr _cls_of_ty(LModule* m, AstType* ty);
TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj);
long long _prog_class_index(HirProgram* prog, TrStr name);
bool _push_field_names_rec(HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
bool _push_field_tags_rec(LModule* m, HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
void _register_classes(LModule* m, HirProgram* prog);
bool _fn_has_iface_param(LModule* m, HirFunction* f);
bool _fn_is_specializable(LModule* m, HirFunction* f);
long long _find_generic_fn(LModule* m, TrStr name);
long long _find_generic_method(LModule* m, TrStr cls, TrStr method);
bool _is_generic_param(HirFunction* f, TrStr n);
bool _param_is_abstract(LModule* m, HirFunction* f, TrStr ptyname);
bool _lir_lower_generic(LModule* m, HirFunction* f, List_i64* argtags, List_TrStr* argcls, TrStr mangled);
void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f);
bool _register_global(LModule* m, HirStmt* s);
bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s);
void _lir_lower_function(LModule* m, HirFunction* f);
bool _field_tag_ok(long long vt, long long ftg);
void _emit_field_set(LModule* m, LFunc* lf, long long obj, long long off, long long val);
long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag);
long long _lower_enum_ctor(LModule* m, LFunc* lf, TrStr ename, TrStr vname, List_ptr* margs);
long long _lower_obj_call(LModule* m, LFunc* lf, TrStr mangled, long long self_vreg, List_ptr* margs);
bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
bool _run_defers(LModule* m, LFunc* lf);
long long _ptr_stride(LModule* m, AstType* pty);
TrStr _dunder_for_op(TrStr op);
TrStr _stmt_expr_kind(HirExpr* e);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
long long _lower_set_method(LModule* m, LFunc* lf, long long shv, long long stag, TrStr method, List_ptr* margs);
long long _lit_pat_cond(LModule* m, LFunc* lf, Pattern pat, long long subj, long long st);
bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms);
TrStr _norm_variant(TrStr ename, TrStr vn);
long long _variant_tag_cond(LFunc* lf, long long tagv, long long vidx);
bool _bind_payload(LModule* m, LFunc* lf, VariantLayout* vlay, long long subj, AstType* subj_ty, long long fldidx, TrStr bindname);
bool _lower_match_enum(LModule* m, LFunc* lf, HirExpr* expr, long long subj, List_ptr* arms);
bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body);
bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_unpack(LModule* m, LFunc* lf, List_TrStr* vars, HirExpr* iter, HirBlock* body);
bool _lower_enumerate(LModule* m, LFunc* lf, TrStr ivar, TrStr evar, HirExpr* listexpr, HirBlock* body);
void _emit_incr(LFunc* lf, TrStr name);
TrStr _ident_name(HirExpr* e);
bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val);
bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val);
TrStr _write_sym(long long t);
void _emit_call0(LModule* m, LFunc* lf, TrStr sym);
bool _lower_print(LModule* m, LFunc* lf, List_ptr* args);
bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e);
bool _int_op(TrStr op);
TrStr _lir_digit(long long d);
TrStr _lir_itoa(long long n);
void _fresh_mark(LFunc* lf, long long v);
bool _fresh_take(LFunc* lf, long long v);
void _release_str(LModule* m, LFunc* lf, long long v);
void _retain_str(LModule* m, LFunc* lf, long long v);
void _flush_fresh_strs(LModule* m, LFunc* lf);
void _secure_str(LModule* m, LFunc* lf, long long v);
bool _is_param(LFunc* lf, TrStr name);
long long _norm_bool(LFunc* lf, long long v);
long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype);
long long _heap_lit(LModule* m, LFunc* lf, TrStr s);
long long _obj_to_str(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg);
long long _reg_to_str(LModule* m, LFunc* lf, long long reg);
long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype);
long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
TrStr _float_unary_sym(TrStr method);
long long _lower_int_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
long long _lower_dict_method(LModule* m, LFunc* lf, long long _tr_v_recv, long long dtag, TrStr method, List_ptr* margs);
long long _lower_float_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
bool _is_const_int(HirExpr* e);
long long _const_int_val(HirExpr* e);
void _emit_add_const(LFunc* lf, TrStr name, long long delta);
long long _list_call1(LModule* m, LFunc* lf, TrStr sym, long long handle, long long restype);
long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx);
long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx);
long long lower_expr(LModule* m, LFunc* lf, HirExpr* e);

__attribute__((hot)) AstType** box_asttype_lir(AstType* t) {
    /* pass */
    /* unsafe block */
    /* pass */
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
    /* pass */
    (*p = t);
    /* pass */
    return p;
}

__attribute__((hot)) long long _f64_bits(double v) {
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

__attribute__((hot)) long long _promote_f(LFunc* lf, long long v) {
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IIToF(d, v));
    /* pass */
    LFunc_set_vreg_type(lf, d, 5LL);
    /* pass */
    return d;
}

__attribute__((hot)) TrStr _print_i64_sym() {
    /* pass */
    return _tr_str_lit("_tr_rt_print_i64");
}

__attribute__((hot)) bool _is_list_tag(long long t) {
    /* pass */
    return ((((t == 2LL) || (t == 3LL)) || (t == 14LL)) || (t == 19LL));
}

__attribute__((hot)) bool _is_set_tag(long long t) {
    /* pass */
    return ((t == 13LL) || (t == 16LL));
}

__attribute__((hot)) TrStr _set_sym(long long t, TrStr op) {
    /* pass */
    if ((t == 16LL)) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("_tr_rt_sdict_")), _tr_strz(op));
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(_tr_str_lit("_tr_rt_idict_")), _tr_strz(op));
}

__attribute__((hot)) bool _is_dict_tag(long long t) {
    /* pass */
    return ((((((t == 6LL) || (t == 7LL)) || (t == 8LL)) || (t == 9LL)) || (t == 17LL)) || (t == 18LL));
}

__attribute__((hot)) bool _dict_key_is_str(long long t) {
    /* pass */
    return (((t == 6LL) || (t == 8LL)) || (t == 17LL));
}

__attribute__((hot)) long long _dict_val_tag(long long t) {
    /* pass */
    if (((t == 8LL) || (t == 9LL))) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if (((t == 17LL) || (t == 18LL))) {
        /* pass */
        return 5LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) TrStr _dict_new_sym(long long t) {
    /* pass */
    if (_dict_key_is_str(t)) {
        /* pass */
        return _tr_str_lit("_tr_rt_sdict_new");
    }
    /* pass */
    return _tr_str_lit("_tr_rt_idict_new");
}

__attribute__((hot)) TrStr _dict_sym(long long t, TrStr op) {
    /* pass */
    TrStr pfx = _tr_str_lit("_tr_rt_idict_");
    /* pass */
    if (_dict_key_is_str(t)) {
        /* pass */
        TrStr _strtmp_t2249 = _tr_str_lit("_tr_rt_sdict_");
        _tr_str_release(pfx);
        pfx = _strtmp_t2249;
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(pfx), _tr_strz(op));
}

__attribute__((hot)) long long _list_elem_tag(long long t) {
    /* pass */
    if ((t == 3LL)) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if ((t == 14LL)) {
        /* pass */
        return 5LL;
    }
    /* pass */
    if ((t == 19LL)) {
        /* pass */
        return 10LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long _list_tag_for_elem(long long et) {
    /* pass */
    if ((et == 1LL)) {
        /* pass */
        return 3LL;
    }
    /* pass */
    if ((et == 5LL)) {
        /* pass */
        return 14LL;
    }
    /* pass */
    if ((et == 10LL)) {
        /* pass */
        return 19LL;
    }
    /* pass */
    return 2LL;
}

__attribute__((hot)) bool _is_cmp_op(TrStr op) {
    /* pass */
    return ((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0));
}

__attribute__((hot)) bool _is_int_typename(TrStr n) {
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("usize"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("isize"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long _ast_type_tag(AstType* ty) {
    /* pass */
    TrStr n = ty->name;
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("String"))) == 0))) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return 4LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* et = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if (((strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("String"))) == 0))) {
                /* pass */
                return 3LL;
            }
            /* pass */
            if (_is_int_typename(et->name)) {
                /* pass */
                return 2LL;
            }
            /* pass */
            if (((strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("f64"))) == 0))) {
                /* pass */
                return 14LL;
            }
            /* pass */
            return (-1LL);
        }
        /* pass */
        return 2LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0)) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* se = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if (((strcmp(_tr_strz(se->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(se->name), _tr_strz(_tr_str_lit("String"))) == 0))) {
                /* pass */
                return 16LL;
            }
            /* pass */
            if (_is_int_typename(se->name)) {
                /* pass */
                return 13LL;
            }
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Tuple"))) == 0)) {
        /* pass */
        return 15LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0))) {
        /* pass */
        if ((ty->args->len >= 2LL)) {
            /* pass */
            AstType* kt = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            AstType* vt = (*((AstType**)List_ptr_get(ty->args, 1LL)));
            /* pass */
            bool kstr = ((strcmp(_tr_strz(kt->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(kt->name), _tr_strz(_tr_str_lit("String"))) == 0));
            /* pass */
            bool kint = _is_int_typename(kt->name);
            /* pass */
            bool vstr = ((strcmp(_tr_strz(vt->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(vt->name), _tr_strz(_tr_str_lit("String"))) == 0));
            /* pass */
            bool vint = _is_int_typename(vt->name);
            /* pass */
            bool vflt = ((strcmp(_tr_strz(vt->name), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(vt->name), _tr_strz(_tr_str_lit("f64"))) == 0));
            /* pass */
            if ((kstr && vint)) {
                /* pass */
                return 6LL;
            }
            /* pass */
            if ((kint && vint)) {
                /* pass */
                return 7LL;
            }
            /* pass */
            if ((kstr && vstr)) {
                /* pass */
                return 8LL;
            }
            /* pass */
            if ((kint && vstr)) {
                /* pass */
                return 9LL;
            }
            /* pass */
            if ((kstr && vflt)) {
                /* pass */
                return 17LL;
            }
            /* pass */
            if ((kint && vflt)) {
                /* pass */
                return 18LL;
            }
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (_is_int_typename(n)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return 5LL;
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit(""))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_null_str(TrStr s) {
    /* pass */
    return (((unsigned long long)(((char*)(_tr_strz(s))))) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _own(TrStr s) {
    /* pass */
    if (_is_null_str(s)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("")));
}

__attribute__((hot)) long long _tag_of(LModule* m, AstType* ty) {
    /* pass */
    if (_is_null_str(ty->name)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long sbi = 0LL;
    /* pass */
    while ((sbi < m->subst_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(m->subst_names, sbi)), _tr_strz(ty->name)) == 0)) {
            /* pass */
            return _tag_of(m, (*((AstType**)List_ptr_get(m->subst_tys, sbi))));
        }
        /* pass */
        sbi = (sbi + 1LL);
    }
    /* pass */
    if (LModule_is_class(m, ty->name)) {
        /* pass */
        return 10LL;
    }
    /* pass */
    if (LModule_is_enum(m, ty->name)) {
        /* pass */
        return 11LL;
    }
    /* pass */
    if (((ty->args->len > 0LL) && (_prog_generic_class_index(m, ty->name) >= 0LL))) {
        /* pass */
        if ((strcmp(_tr_strz(_ensure_generic_class(m, ty)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            return 10LL;
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (ty->args->len > 0LL))) {
        /* pass */
        AstType* lelem = (*((AstType**)List_ptr_get(ty->args, 0LL)));
        /* pass */
        if (((!_is_null_str(lelem->name)) && LModule_is_class(m, lelem->name))) {
            /* pass */
            return 19LL;
        }
    }
    /* pass */
    return _ast_type_tag(ty);
}

__attribute__((hot)) AstType* _subst_ty(LModule* m, AstType* ty) {
    /* pass */
    if (_is_null_str(ty->name)) {
        /* pass */
        return ty;
    }
    /* pass */
    long long sbi = 0LL;
    /* pass */
    while ((sbi < m->subst_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(m->subst_names, sbi)), _tr_strz(ty->name)) == 0)) {
            /* pass */
            return (*((AstType**)List_ptr_get(m->subst_tys, sbi)));
        }
        /* pass */
        sbi = (sbi + 1LL);
    }
    /* pass */
    return ty;
}

__attribute__((hot)) long long _prog_generic_class_index(LModule* m, TrStr name) {
    /* pass */
    if (_is_null_str(name)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->hir_prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, i));
        /* pass */
        if (((strcmp(_tr_strz(c->name), _tr_strz(name)) == 0) && (c->generics->len > 0LL))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) TrStr _mangle_generic(LModule* m, AstType* ty) {
    /* pass */
    TrStr n = _own(ty->name);
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < ty->args->len)) {
        /* pass */
        AstType* at = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, ai))));
        /* pass */
        TrStr _strtmp_t2250 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(n), _tr_strz(_tr_str_lit("__g")))); TrStr _cr = (_own(at->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(n);
        n = _strtmp_t2250;
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    return n;
}

__attribute__((hot)) TrStr _ensure_generic_class(LModule* m, AstType* ty) {
    /* pass */
    long long gci = _prog_generic_class_index(m, ty->name);
    /* pass */
    if ((gci < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, gci));
    /* pass */
    if ((c->generics->len != ty->args->len)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    if ((c->base_classes->len > 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr mangled = _mangle_generic(m, ty);
    /* pass */
    if (LModule_is_class(m, mangled)) {
        /* pass */
        return mangled;
    }
    /* pass */
    long long sb = 0LL;
    /* pass */
    while ((sb < c->generics->len)) {
        /* pass */
        ({ TrStr _at_t2251 = (List_TrStr_get(c->generics, sb)); TrStr _at_t2252 = (List_TrStr_get(c->generics, sb)); TrStr _at_t2253 = (_own(_at_t2252)); List_TrStr_append(m->subst_names, _at_t2253); _tr_str_release(_at_t2251); _tr_str_release(_at_t2252); _tr_str_release(_at_t2253); });
        /* pass */
        List_ptr_append(m->subst_tys, box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(ty->args, sb))))));
        /* pass */
        sb = (sb + 1LL);
    }
    /* pass */
    ClassLayout* lay = ({ TrStr _at_t2254 = (_own(mangled)); __auto_type _wr = (ClassLayout_init(_at_t2254)); _tr_str_release(_at_t2254); _wr; });
    /* pass */
    bool ok = true;
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < c->fields->len)) {
        /* pass */
        long long ftg = _tag_of(m, ((HirField*)List_ptr_get(c->fields, fi))->ty);
        /* pass */
        if ((ftg < 0LL)) {
            /* pass */
            ok = false;
        }
        /* pass */
        ({ TrStr _at_t2255 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _at_t2255); _tr_str_release(_at_t2255); });
        /* pass */
        List_i64_append(lay->ftags, ftg);
        /* pass */
        ({ TrStr _at_t2256 = (_cls_of_ty(m, _subst_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty))); List_TrStr_append(lay->fcls, _at_t2256); _tr_str_release(_at_t2256); });
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    if (ok) {
        /* pass */
        LModule_add_class(m, lay);
        /* pass */
        long long prereg = 0LL;
        /* pass */
        while ((prereg < m->hir_prog->functions->len)) {
            /* pass */
            HirFunction* pf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, prereg));
            /* pass */
            if ((((strcmp(_tr_strz(pf->class_name), _tr_strz(ty->name)) == 0) && (!pf->is_extern)) && (pf->generics->len == 0LL))) {
                /* pass */
                ({ TrStr _at_t2257 = (({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(pf->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(m->fn_names, _at_t2257); _tr_str_release(_at_t2257); });
                /* pass */
                List_i64_append(m->fn_ret, _tag_of(m, pf->ret_ty));
            }
            /* pass */
            prereg = (prereg + 1LL);
        }
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < m->hir_prog->functions->len)) {
            /* pass */
            HirFunction* mf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, mi));
            /* pass */
            if ((((strcmp(_tr_strz(mf->class_name), _tr_strz(ty->name)) == 0) && (!mf->is_extern)) && (mf->generics->len == 0LL))) {
                /* pass */
                _lir_lower_method(m, mangled, mf);
                /* pass */
                if ((!m->ok)) {
                    /* pass */
                    ({ TrStr _at_t2258 = (({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(mf->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(m->unavail_names, _at_t2258); _tr_str_release(_at_t2258); });
                    /* pass */
                    ({ TrStr _at_t2259 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t2259); _tr_str_release(_at_t2259); });
                    /* pass */
                    m->ok = true;
                    /* pass */
                    m->fail_note = _tr_str_lit("");
                }
            }
            /* pass */
            mi = (mi + 1LL);
        }
    }
    /* pass */
    sb = 0LL;
    /* pass */
    while ((sb < c->generics->len)) {
        /* pass */
        List_TrStr_pop(m->subst_names);
        /* pass */
        List_ptr_pop(m->subst_tys);
        /* pass */
        sb = (sb + 1LL);
    }
    /* pass */
    if ((!ok)) {
        /* pass */
        _tr_str_release(mangled);
        _tr_obj_release(lay, _trdrop_ClassLayout);
        return _tr_str_lit("");
    }
    /* pass */
    _tr_obj_release(lay, _trdrop_ClassLayout);
    return mangled;
}

__attribute__((hot)) TrStr _cls_of_ty(LModule* m, AstType* ty) {
    /* pass */
    if (_is_null_str(ty->name)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    if ((LModule_is_class(m, ty->name) || LModule_is_enum(m, ty->name))) {
        /* pass */
        return _own(ty->name);
    }
    /* pass */
    if (((ty->args->len > 0LL) && (_prog_generic_class_index(m, ty->name) >= 0LL))) {
        /* pass */
        TrStr gm = _ensure_generic_class(m, ty);
        /* pass */
        if ((strcmp(_tr_strz(gm), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            return gm;
        }
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj) {
    /* pass */
    TrStr cn = hir_expr_type(obj)->name;
    /* pass */
    if (((!_is_null_str(cn)) && (LModule_is_class(m, cn) || LModule_is_enum(m, cn)))) {
        /* pass */
        return _tr_str_retain(cn);
    }
    /* pass */
    if ((((!_is_null_str(cn)) && (hir_expr_type(obj)->args->len > 0LL)) && (_prog_generic_class_index(m, cn) >= 0LL))) {
        /* pass */
        TrStr grc = _ensure_generic_class(m, hir_expr_type(obj));
        /* pass */
        if ((strcmp(_tr_strz(grc), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            return grc;
        }
    }
    /* pass */
    __auto_type _t2260 = (*obj);
    if (_t2260.tag == HirExpr_EIdent) {
        __auto_type nm = _t2260.data.EIdent.name;
        /* pass */
        TrStr vc = LFunc_var_cls_of(lf, nm);
        /* pass */
        if (((!_is_null_str(vc)) && (LModule_is_class(m, vc) || LModule_is_enum(m, vc)))) {
            /* pass */
            return vc;
        }
        _tr_str_release(vc);
    } else if (_t2260.tag == HirExpr_EPropAccess) {
        __auto_type inner = _t2260.data.EPropAccess.obj;
__auto_type prop = _t2260.data.EPropAccess.prop;
        /* pass */
        TrStr icls = _recv_class(m, lf, inner);
        /* pass */
        if (((strcmp(_tr_strz(icls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, icls))) {
            /* pass */
            TrStr fc = LModule_field_cls(m, icls, prop);
            /* pass */
            if (((!_is_null_str(fc)) && (strcmp(_tr_strz(fc), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                _tr_str_release(icls);
                return fc;
            }
        }
        _tr_str_release(icls);
    } else if (1) {
        __auto_type _ = _t2260;
        /* pass */
        /* pass */
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) long long _prog_class_index(HirProgram* prog, TrStr name) {
    /* pass */
    if (_is_null_str(name)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((HirClass*)List_ptr_get(prog->classes, i))->name), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _push_field_names_rec(HirProgram* prog, long long ci, ClassLayout* lay, long long depth) {
    /* pass */
    if ((depth > 32LL)) {
        /* pass */
        return false;
    }
    /* pass */
    HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
    /* pass */
    if ((c->base_classes->len > 1LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((c->base_classes->len == 1LL)) {
        /* pass */
        long long bi = ({ TrStr _at_t2261 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t2261)); _tr_str_release(_at_t2261); _wr; });
        /* pass */
        if ((bi < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((!_push_field_names_rec(prog, bi, lay, (depth + 1LL)))) {
            /* pass */
            return false;
        }
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < c->fields->len)) {
        /* pass */
        ({ TrStr _at_t2262 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _at_t2262); _tr_str_release(_at_t2262); });
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool _push_field_tags_rec(LModule* m, HirProgram* prog, long long ci, ClassLayout* lay, long long depth) {
    /* pass */
    if ((depth > 32LL)) {
        /* pass */
        return false;
    }
    /* pass */
    HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
    /* pass */
    if ((c->base_classes->len > 1LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((c->base_classes->len == 1LL)) {
        /* pass */
        long long bi = ({ TrStr _at_t2263 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t2263)); _tr_str_release(_at_t2263); _wr; });
        /* pass */
        if ((bi < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((!_push_field_tags_rec(m, prog, bi, lay, (depth + 1LL)))) {
            /* pass */
            return false;
        }
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < c->fields->len)) {
        /* pass */
        List_i64_append(lay->ftags, _tag_of(m, ((HirField*)List_ptr_get(c->fields, fi))->ty));
        /* pass */
        ({ TrStr _at_t2264 = (_cls_of_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty)); List_TrStr_append(lay->fcls, _at_t2264); _tr_str_release(_at_t2264); });
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) void _register_classes(LModule* m, HirProgram* prog) {
    /* pass */
    EnumLayout* opt = EnumLayout_init(_tr_str_lit("Option"));
    /* pass */
    VariantLayout* opt_some = VariantLayout_init(_tr_str_lit("Some"));
    /* pass */
    List_TrStr_append(opt_some->fields, _tr_str_lit("val"));
    /* pass */
    List_i64_append(opt_some->ftags, (0LL - 2LL));
    /* pass */
    List_TrStr_append(opt_some->fcls, _tr_str_lit(""));
    /* pass */
    List_ptr_append(opt->variants, _tr_obj_retain(opt_some));
    /* pass */
    List_ptr_append(opt->variants, VariantLayout_init(_tr_str_lit("None")));
    /* pass */
    LModule_add_enum(m, opt);
    /* pass */
    EnumLayout* res = EnumLayout_init(_tr_str_lit("Result"));
    /* pass */
    VariantLayout* res_ok = VariantLayout_init(_tr_str_lit("Ok"));
    /* pass */
    List_TrStr_append(res_ok->fields, _tr_str_lit("val"));
    /* pass */
    List_i64_append(res_ok->ftags, (0LL - 2LL));
    /* pass */
    List_TrStr_append(res_ok->fcls, _tr_str_lit(""));
    /* pass */
    List_ptr_append(res->variants, _tr_obj_retain(res_ok));
    /* pass */
    VariantLayout* res_err = VariantLayout_init(_tr_str_lit("Err"));
    /* pass */
    List_TrStr_append(res_err->fields, _tr_str_lit("err"));
    /* pass */
    List_i64_append(res_err->ftags, (0LL - 3LL));
    /* pass */
    List_TrStr_append(res_err->fcls, _tr_str_lit(""));
    /* pass */
    List_ptr_append(res->variants, _tr_obj_retain(res_err));
    /* pass */
    LModule_add_enum(m, res);
    /* pass */
    long long ifi = 0LL;
    /* pass */
    while ((ifi < prog->interfaces->len)) {
        /* pass */
        ({ TrStr _at_t2265 = (_own(((HirInterface*)List_ptr_get(prog->interfaces, ifi))->name)); LModule_add_iface(m, _at_t2265); _tr_str_release(_at_t2265); });
        /* pass */
        ifi = (ifi + 1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        if ((c->generics->len > 0LL)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        ClassLayout* lay = ({ TrStr _at_t2266 = (_own(c->name)); __auto_type _wr = (ClassLayout_init(_at_t2266)); _tr_str_release(_at_t2266); _wr; });
        /* pass */
        if ((c->base_classes->len == 1LL)) {
            /* pass */
            lay->base = ({ TrStr _at_t2267 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_own(_at_t2267)); _tr_str_release(_at_t2267); _wr; });
        }
        /* pass */
        if (_push_field_names_rec(prog, i, lay, 0LL)) {
            /* pass */
            LModule_add_class(m, lay);
        }
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(lay, _trdrop_ClassLayout);
    }
    /* pass */
    long long ei = 0LL;
    /* pass */
    while ((ei < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, ei));
        /* pass */
        EnumLayout* elay = ({ TrStr _at_t2268 = (_own(e->name)); __auto_type _wr = (EnumLayout_init(_at_t2268)); _tr_str_release(_at_t2268); _wr; });
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < e->variants->len)) {
            /* pass */
            HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, vi));
            /* pass */
            VariantLayout* vlay = ({ TrStr _at_t2269 = (_own(v->name)); __auto_type _wr = (VariantLayout_init(_at_t2269)); _tr_str_release(_at_t2269); _wr; });
            /* pass */
            long long pf = 0LL;
            /* pass */
            while ((pf < v->fields->len)) {
                /* pass */
                ({ TrStr _at_t2270 = (_own(((HirParam*)List_ptr_get(v->fields, pf))->name)); List_TrStr_append(vlay->fields, _at_t2270); _tr_str_release(_at_t2270); });
                /* pass */
                pf = (pf + 1LL);
            }
            /* pass */
            List_ptr_append(elay->variants, _tr_obj_retain(vlay));
            /* pass */
            vi = (vi + 1LL);
            _tr_obj_release(vlay, _trdrop_VariantLayout);
        }
        /* pass */
        LModule_add_enum(m, elay);
        /* pass */
        ei = (ei + 1LL);
        _tr_obj_release(elay, _trdrop_EnumLayout);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c2 = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        long long li = LModule_class_index(m, c2->name);
        /* pass */
        if ((li >= 0LL)) {
            /* pass */
            ClassLayout* lay2 = ((ClassLayout*)List_ptr_get(m->classes, li));
            /* pass */
            if ((!_push_field_tags_rec(m, prog, i, lay2, 0LL))) {
                /* pass */
                m->ok = false;
                /* pass */
                m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(((HirClass*)List_ptr_get(prog->classes, i))->name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("class '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': field-tag registration diverged"))); _tr_str_release(_cl); _cres; });
                /* pass */
                _tr_obj_release(opt, _trdrop_EnumLayout);
                _tr_obj_release(opt_some, _trdrop_VariantLayout);
                _tr_obj_release(res, _trdrop_EnumLayout);
                _tr_obj_release(res_ok, _trdrop_VariantLayout);
                _tr_obj_release(res_err, _trdrop_VariantLayout);
                return;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    ei = 0LL;
    /* pass */
    while ((ei < prog->enums->len)) {
        /* pass */
        HirEnum* e2 = ((HirEnum*)List_ptr_get(prog->enums, ei));
        /* pass */
        long long e2ix = LModule_enum_index(m, e2->name);
        /* pass */
        if ((e2ix < 0LL)) {
            /* pass */
            ei = (ei + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        EnumLayout* elay2 = ((EnumLayout*)List_ptr_get(m->enums, e2ix));
        /* pass */
        long long v2 = 0LL;
        /* pass */
        while ((v2 < e2->variants->len)) {
            /* pass */
            HirVariant* vdef = ((HirVariant*)List_ptr_get(e2->variants, v2));
            /* pass */
            VariantLayout* vlay2 = ((VariantLayout*)List_ptr_get(elay2->variants, v2));
            /* pass */
            long long p2 = 0LL;
            /* pass */
            while ((p2 < vdef->fields->len)) {
                /* pass */
                AstType* p2ty = ((HirParam*)List_ptr_get(vdef->fields, p2))->ty;
                /* pass */
                long long p2gi = (-1LL);
                /* pass */
                if ((!_is_null_str(p2ty->name))) {
                    /* pass */
                    long long egk = 0LL;
                    /* pass */
                    while ((egk < e2->generics->len)) {
                        /* pass */
                        if ((strcmp(_tr_strz(List_TrStr_get(e2->generics, egk)), _tr_strz(p2ty->name)) == 0)) {
                            /* pass */
                            p2gi = egk;
                        }
                        /* pass */
                        egk = (egk + 1LL);
                    }
                }
                /* pass */
                if ((p2gi >= 0LL)) {
                    /* pass */
                    List_i64_append(vlay2->ftags, (0LL - (2LL + p2gi)));
                    /* pass */
                    List_TrStr_append(vlay2->fcls, _tr_str_lit(""));
                } else {
                    /* pass */
                    List_i64_append(vlay2->ftags, _tag_of(m, p2ty));
                    /* pass */
                    ({ TrStr _at_t2271 = (_cls_of_ty(m, p2ty)); List_TrStr_append(vlay2->fcls, _at_t2271); _tr_str_release(_at_t2271); });
                }
                /* pass */
                p2 = (p2 + 1LL);
            }
            /* pass */
            v2 = (v2 + 1LL);
        }
        /* pass */
        ei = (ei + 1LL);
    }
    _tr_obj_release(opt, _trdrop_EnumLayout);
    _tr_obj_release(opt_some, _trdrop_VariantLayout);
    _tr_obj_release(res, _trdrop_EnumLayout);
    _tr_obj_release(res_ok, _trdrop_VariantLayout);
    _tr_obj_release(res_err, _trdrop_VariantLayout);
}

__attribute__((hot)) LModule* lower_to_lir(HirProgram* prog) {
    /* pass */
    LModule* m = LModule_init();
    /* pass */
    HirProgram* _cltmp_t2272 = _tr_obj_retain(prog);
    _tr_obj_release(m->hir_prog, _trdrop_HirProgram);
    m->hir_prog = _cltmp_t2272;
    /* pass */
    _register_classes(m, prog);
    /* pass */
    long long efx = 0LL;
    /* pass */
    while ((efx < prog->extern_funcs->len)) {
        /* pass */
        HirFunction* ef = ((HirFunction*)List_ptr_get(prog->extern_funcs, efx));
        /* pass */
        bool ef_ok = true;
        /* pass */
        long long epi = 0LL;
        /* pass */
        while ((epi < ef->params->len)) {
            /* pass */
            long long eptag = _tag_of(m, ((HirParam*)List_ptr_get(ef->params, epi))->ty);
            /* pass */
            if ((((eptag != 0LL) && (eptag != 1LL)) && (eptag != 4LL))) {
                /* pass */
                ef_ok = false;
            }
            /* pass */
            epi = (epi + 1LL);
        }
        /* pass */
        long long ertag = _tag_of(m, ef->ret_ty);
        /* pass */
        if ((((ertag != 0LL) && (ertag != 1LL)) && (ertag != 4LL))) {
            /* pass */
            ef_ok = false;
        }
        /* pass */
        if (ef_ok) {
            /* pass */
            ({ TrStr _at_t2273 = (_own(ef->name)); List_TrStr_append(m->extfn_names, _at_t2273); _tr_str_release(_at_t2273); });
            /* pass */
            List_i64_append(m->extfn_ret, ertag);
        }
        /* pass */
        efx = (efx + 1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f0 = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((!f0->is_extern) && (!_fn_is_specializable(m, f0)))) {
            /* pass */
            if (((strcmp(_tr_strz(f0->class_name), _tr_strz(_tr_str_lit(""))) != 0) && (_prog_generic_class_index(m, f0->class_name) >= 0LL))) {
                /* pass */
                i = (i + 1LL);
                /* pass */
                continue;
            }
            /* pass */
            if ((strcmp(_tr_strz(f0->class_name), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                List_TrStr_append(m->fn_names, f0->name);
            } else {
                /* pass */
                ({ TrStr _at_t2274 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(f0->class_name), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f0->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(m->fn_names, _at_t2274); _tr_str_release(_at_t2274); });
            }
            /* pass */
            List_i64_append(m->fn_ret, _tag_of(m, f0->ret_ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long ti = 0LL;
    /* pass */
    while ((ti < prog->top_level_stmts->len)) {
        /* pass */
        if ((!_register_global(m, ((HirStmt*)List_ptr_get(prog->top_level_stmts, ti))))) {
            /* pass */
            m->ok = false;
            /* pass */
            m->fail_note = _tr_str_lit("unsupported top-level statement");
            /* pass */
            return m;
        }
        /* pass */
        ti = (ti + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((!f->is_extern) && (!_fn_is_specializable(m, f)))) {
            /* pass */
            if (((strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) != 0) && (_prog_generic_class_index(m, f->class_name) >= 0LL))) {
                /* pass */
                i = (i + 1LL);
                /* pass */
                continue;
            }
            /* pass */
            if ((strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                _lir_lower_function(m, f);
            } else {
                /* pass */
                _lir_lower_method(m, f->class_name, f);
            }
            /* pass */
            if ((!m->ok)) {
                /* pass */
                if (((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) == 0) && (strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) == 0))) {
                    /* pass */
                    return m;
                }
                /* pass */
                TrStr un = f->name;
                /* pass */
                if ((strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    un = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(f->class_name), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; });
                }
                /* pass */
                ({ TrStr _at_t2275 = (_own(un)); List_TrStr_append(m->unavail_names, _at_t2275); _tr_str_release(_at_t2275); });
                /* pass */
                ({ TrStr _at_t2276 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t2276); _tr_str_release(_at_t2276); });
                /* pass */
                m->ok = true;
                /* pass */
                m->fail_note = _tr_str_lit("");
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((m->unavail_names->len > 0LL)) {
        /* pass */
        List_i64* dropped = (void*)List_i64_new();
        /* pass */
        long long di0 = 0LL;
        /* pass */
        while ((di0 < m->funcs->len)) {
            /* pass */
            List_i64_append(dropped, 0LL);
            /* pass */
            di0 = (di0 + 1LL);
        }
        /* pass */
        bool changed = true;
        /* pass */
        while (changed) {
            /* pass */
            changed = false;
            /* pass */
            long long fi2 = 0LL;
            /* pass */
            while ((fi2 < m->funcs->len)) {
                /* pass */
                if ((List_i64_get(dropped, fi2) == 0LL)) {
                    /* pass */
                    LFunc* lfc = ((LFunc*)List_ptr_get(m->funcs, fi2));
                    /* pass */
                    long long hit = (-1LL);
                    /* pass */
                    long long bi2 = 0LL;
                    /* pass */
                    while (((bi2 < lfc->blocks->len) && (hit < 0LL))) {
                        /* pass */
                        LBlock* blk2 = ((LBlock*)List_ptr_get(lfc->blocks, bi2));
                        /* pass */
                        long long ii2 = 0LL;
                        /* pass */
                        while (((ii2 < blk2->insts->len) && (hit < 0LL))) {
                            /* pass */
                            __auto_type _t2277 = (*((LInst*)List_ptr_get(blk2->insts, ii2)));
                            if (_t2277.tag == LInst_ICall) {
                                __auto_type ucallee = _t2277.data.ICall.callee;
                                /* pass */
                                hit = LModule_unavail_index(m, ucallee);
                            } else if (_t2277.tag == LInst_IFuncAddr) {
                                __auto_type ufname = _t2277.data.IFuncAddr.fname;
                                /* pass */
                                hit = LModule_unavail_index(m, ufname);
                            } else if (1) {
                                __auto_type _ = _t2277;
                                /* pass */
                            }
                            /* pass */
                            ii2 = (ii2 + 1LL);
                        }
                        /* pass */
                        bi2 = (bi2 + 1LL);
                    }
                    /* pass */
                    if ((hit >= 0LL)) {
                        /* pass */
                        TrStr unote = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(lfc->name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("'")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' calls '"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(m->unavail_names, hit)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' — "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(m->unavail_notes, hit)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        /* pass */
                        if (lfc->is_main) {
                            /* pass */
                            m->ok = false;
                            /* pass */
                            m->fail_note = _tr_str_retain(unote);
                            /* pass */
                            List_i64_free(dropped);
                            _tr_str_release(unote);
                            return m;
                        }
                        /* pass */
                        ({ TrStr _at_t2278 = (_own(lfc->name)); List_TrStr_append(m->unavail_names, _at_t2278); _tr_str_release(_at_t2278); });
                        /* pass */
                        List_TrStr_append(m->unavail_notes, unote);
                        /* pass */
                        List_i64_set(dropped, fi2, 1LL);
                        /* pass */
                        changed = true;
                    }
                }
                /* pass */
                fi2 = (fi2 + 1LL);
            }
        }
        /* pass */
        List_ptr* kept = (void*)List_ptr_new();
        /* pass */
        long long ki = 0LL;
        /* pass */
        while ((ki < m->funcs->len)) {
            /* pass */
            if ((List_i64_get(dropped, ki) == 0LL)) {
                /* pass */
                List_ptr_append(kept, _tr_obj_retain(((LFunc*)List_ptr_get(m->funcs, ki))));
            }
            /* pass */
            ki = (ki + 1LL);
        }
        /* pass */
        m->funcs = kept;
    }
    /* pass */
    return m;
}

__attribute__((hot)) bool _fn_has_iface_param(LModule* m, HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        TrStr pn = ((HirParam*)List_ptr_get(f->params, i))->ty->name;
        /* pass */
        if (((!_is_null_str(pn)) && LModule_is_iface(m, pn))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _fn_is_specializable(LModule* m, HirFunction* f) {
    /* pass */
    if ((f->generics->len > 0LL)) {
        /* pass */
        return true;
    }
    /* pass */
    return _fn_has_iface_param(m, f);
}

__attribute__((hot)) long long _find_generic_fn(LModule* m, TrStr name) {
    /* pass */
    if (_is_null_str(name)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->hir_prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
        /* pass */
        if ((((strcmp(_tr_strz(f->name), _tr_strz(name)) == 0) && (strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) == 0)) && (!f->is_extern))) {
            /* pass */
            if (_fn_is_specializable(m, f)) {
                /* pass */
                return i;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _find_generic_method(LModule* m, TrStr cls, TrStr method) {
    /* pass */
    if ((_is_null_str(cls) || _is_null_str(method))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->hir_prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
        /* pass */
        if (((((strcmp(_tr_strz(f->class_name), _tr_strz(cls)) == 0) && (strcmp(_tr_strz(f->name), _tr_strz(method)) == 0)) && (f->generics->len > 0LL)) && (!f->is_extern))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_generic_param(HirFunction* f, TrStr n) {
    /* pass */
    if (_is_null_str(n)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->generics->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(f->generics, i)), _tr_strz(n)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _param_is_abstract(LModule* m, HirFunction* f, TrStr ptyname) {
    /* pass */
    if (_is_generic_param(f, ptyname)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((!_is_null_str(ptyname)) && LModule_is_iface(m, ptyname))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _lir_lower_generic(LModule* m, HirFunction* f, List_i64* argtags, List_TrStr* argcls, TrStr mangled) {
    /* pass */
    if ((f->params->len != argtags->len)) {
        /* pass */
        return false;
    }
    /* pass */
    long long rtag = (-1LL);
    /* pass */
    if (_param_is_abstract(m, f, f->ret_ty->name)) {
        /* pass */
        long long ri = 0LL;
        /* pass */
        while ((ri < f->params->len)) {
            /* pass */
            if ((!_is_null_str(((HirParam*)List_ptr_get(f->params, ri))->ty->name))) {
                /* pass */
                if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(f->params, ri))->ty->name), _tr_strz(f->ret_ty->name)) == 0)) {
                    /* pass */
                    rtag = List_i64_get(argtags, ri);
                }
            }
            /* pass */
            ri = (ri + 1LL);
        }
    } else {
        /* pass */
        rtag = _tag_of(m, f->ret_ty);
    }
    /* pass */
    if ((rtag < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    ({ TrStr _at_t2279 = (_own(mangled)); List_TrStr_append(m->fn_names, _at_t2279); _tr_str_release(_at_t2279); });
    /* pass */
    List_i64_append(m->fn_ret, rtag);
    /* pass */
    LFunc* lf = ({ TrStr _at_t2280 = (_own(mangled)); __auto_type _wr = (LFunc_init(_at_t2280)); _tr_str_release(_at_t2280); _wr; });
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        long long ptag = List_i64_get(argtags, pi);
        /* pass */
        if ((!_param_is_abstract(m, f, p->ty->name))) {
            /* pass */
            long long want = _tag_of(m, p->ty);
            /* pass */
            if (((want < 0LL) || (!_field_tag_ok(ptag, want)))) {
                /* pass */
                _tr_obj_release(lf, _trdrop_LFunc);
                return false;
            }
        }
        /* pass */
        List_TrStr_append(lf->params, p->name);
        /* pass */
        LFunc_add_var(lf, p->name);
        /* pass */
        LFunc_set_var_type(lf, p->name, ptag);
        /* pass */
        if ((((ptag == 10LL) || (ptag == 11LL)) && (strcmp(_tr_strz(List_TrStr_get(argcls, pi)), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            ({ TrStr _at_t2281 = (List_TrStr_get(argcls, pi)); TrStr _at_t2282 = (_own(_at_t2281)); LFunc_set_var_cls(lf, p->name, _at_t2282); _tr_str_release(_at_t2281); _tr_str_release(_at_t2282); });
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    if ((!lower_block(m, lf, f->body))) {
        /* pass */
        _tr_obj_release(lf, _trdrop_LFunc);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    /* pass */
    _tr_obj_release(lf, _trdrop_LFunc);
    return true;
}

__attribute__((hot)) void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f) {
    /* pass */
    LFunc* lf = ({ TrStr _at_t2283 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_name), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (LFunc_init(_at_t2283)); _tr_str_release(_at_t2283); _wr; });
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        long long ptag = 0LL;
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            ptag = 10LL;
            /* pass */
            if (LModule_is_enum(m, class_name)) {
                /* pass */
                ptag = 11LL;
            }
        } else {
            /* pass */
            ptag = _tag_of(m, p->ty);
        }
        /* pass */
        if ((ptag < 0LL)) {
            /* pass */
            m->ok = false;
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("method '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': unsupported param type"))); _tr_str_release(_cl); _cres; });
            /* pass */
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
        /* pass */
        List_TrStr_append(lf->params, p->name);
        /* pass */
        LFunc_add_var(lf, p->name);
        /* pass */
        LFunc_set_var_type(lf, p->name, ptag);
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            ({ TrStr _at_t2284 = (_own(class_name)); LFunc_set_var_cls(lf, p->name, _at_t2284); _tr_str_release(_at_t2284); });
        } else if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
            /* pass */
            ({ TrStr _at_t2285 = (_own(p->ty->name)); LFunc_set_var_cls(lf, p->name, _at_t2285); _tr_str_release(_at_t2285); });
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    if ((!lower_block(m, lf, f->body))) {
        /* pass */
        m->ok = false;
        /* pass */
        m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("method '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': body uses an unsupported construct"))); _tr_str_release(_cl); _cres; });
        /* pass */
        _tr_obj_release(lf, _trdrop_LFunc);
        return;
    }
    /* pass */
    if ((!LFunc_cur_terminated(lf))) {
        /* pass */
        if ((!_run_defers(m, lf))) {
            /* pass */
            m->ok = false;
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("method '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': deferred statement not lowerable"))); _tr_str_release(_cl); _cres; });
            /* pass */
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool _register_global(LModule* m, HirStmt* s) {
    /* pass */
    __auto_type _t2286 = (*s);
    if (_t2286.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2286.data.SLineMarker.n;
        return true;
    } else if (_t2286.tag == HirStmt_SPass) {
        return true;
    } else if (_t2286.tag == HirStmt_SLet) {
        __auto_type name = _t2286.data.SLet.name;
__auto_type val = _t2286.data.SLet.val;
        /* pass */
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        long long tag = _ast_type_tag(hir_expr_type(val));
        /* pass */
        if ((tag < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LModule_add_global(m, name, tag);
        /* pass */
        List_ptr_append(m->global_inits, s);
        /* pass */
        return true;
    } else if (_t2286.tag == HirStmt_SAssign) {
        __auto_type target = _t2286.data.SAssign.target;
__auto_type val = _t2286.data.SAssign.val;
        /* pass */
        TrStr nm = _ident_name(target);
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(nm);
            return false;
        }
        /* pass */
        long long tag2 = _ast_type_tag(hir_expr_type(val));
        /* pass */
        if ((tag2 < 0LL)) {
            /* pass */
            _tr_str_release(nm);
            return false;
        }
        /* pass */
        LModule_add_global(m, nm, tag2);
        /* pass */
        List_ptr_append(m->global_inits, s);
        /* pass */
        _tr_str_release(nm);
        return true;
    } else if (1) {
        __auto_type _ = _t2286;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t2287 = (*s);
    if (_t2287.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2287.data.SLineMarker.n;
        return true;
    } else if (_t2287.tag == HirStmt_SPass) {
        return true;
    } else if (_t2287.tag == HirStmt_SLet) {
        __auto_type name = _t2287.data.SLet.name;
__auto_type val = _t2287.data.SLet.val;
        /* pass */
        long long v = lower_expr(m, lf, val);
        /* pass */
        if ((v < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long gidx = LModule_global_index(m, name);
        /* pass */
        if ((gidx < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((LFunc_vreg_type(lf, v) == 1LL)) {
            /* pass */
            _secure_str(m, lf, v);
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx, v));
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        return true;
    } else if (_t2287.tag == HirStmt_SAssign) {
        __auto_type target = _t2287.data.SAssign.target;
__auto_type val = _t2287.data.SAssign.val;
        /* pass */
        long long v2 = lower_expr(m, lf, val);
        /* pass */
        if ((v2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long gidx2 = ({ TrStr _at_t2288 = (_ident_name(target)); __auto_type _wr = (LModule_global_index(m, _at_t2288)); _tr_str_release(_at_t2288); _wr; });
        /* pass */
        if ((gidx2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((LFunc_vreg_type(lf, v2) == 1LL)) {
            /* pass */
            _secure_str(m, lf, v2);
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx2, v2));
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t2287;
        /* pass */
        return false;
    }
}

__attribute__((hot)) void _lir_lower_function(LModule* m, HirFunction* f) {
    /* pass */
    LFunc* lf = LFunc_init(f->name);
    /* pass */
    if ((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) == 0)) {
        /* pass */
        lf->is_main = true;
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
            /* pass */
            long long ptag = _tag_of(m, p->ty);
            /* pass */
            if ((ptag < 0LL)) {
                /* pass */
                m->ok = false;
                /* pass */
                m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("fn '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': unsupported param type '"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(p->ty->name)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; });
                /* pass */
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
            /* pass */
            List_TrStr_append(lf->params, p->name);
            /* pass */
            LFunc_add_var(lf, p->name);
            /* pass */
            LFunc_set_var_type(lf, p->name, ptag);
            /* pass */
            if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
                /* pass */
                ({ TrStr _at_t2289 = (_own(p->ty->name)); LFunc_set_var_cls(lf, p->name, _at_t2289); _tr_str_release(_at_t2289); });
            }
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    if (lf->is_main) {
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < m->global_inits->len)) {
            /* pass */
            if ((!_lower_global_init(m, lf, ((HirStmt*)List_ptr_get(m->global_inits, gi))))) {
                /* pass */
                m->ok = false;
                /* pass */
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
            /* pass */
            gi = (gi + 1LL);
        }
    }
    /* pass */
    if ((!lower_block(m, lf, f->body))) {
        /* pass */
        m->ok = false;
        /* pass */
        m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("fn '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': body uses an unsupported construct"))); _tr_str_release(_cl); _cres; });
        /* pass */
        _tr_obj_release(lf, _trdrop_LFunc);
        return;
    }
    /* pass */
    if ((!LFunc_cur_terminated(lf))) {
        /* pass */
        if ((!_run_defers(m, lf))) {
            /* pass */
            m->ok = false;
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("fn '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': deferred statement not lowerable"))); _tr_str_release(_cl); _cres; });
            /* pass */
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool _field_tag_ok(long long vt, long long ftg) {
    /* pass */
    if ((vt == ftg)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((vt == 0LL) || (vt == 4LL)) && ((ftg == 0LL) || (ftg == 4LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) void _emit_field_set(LModule* m, LFunc* lf, long long obj, long long off, long long val) {
    /* pass */
    long long offc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(offc, off));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_field_set_i"));
    /* pass */
    List_i64* sa = (void*)List_i64_new();
    /* pass */
    List_i64_append(sa, obj);
    /* pass */
    List_i64_append(sa, offc);
    /* pass */
    List_i64_append(sa, val);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_field_set_i"), sa));
}

__attribute__((hot)) long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag) {
    /* pass */
    long long offc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(offc, off));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_field_get_i"));
    /* pass */
    List_i64* fgargs = (void*)List_i64_new();
    /* pass */
    List_i64_append(fgargs, obj);
    /* pass */
    List_i64_append(fgargs, offc);
    /* pass */
    long long gd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(gd, _tr_str_lit("_tr_rt_field_get_i"), fgargs));
    /* pass */
    LFunc_set_vreg_type(lf, gd, tag);
    /* pass */
    return gd;
}

__attribute__((hot)) long long _lower_enum_ctor(LModule* m, LFunc* lf, TrStr ename, TrStr vname, List_ptr* margs) {
    /* pass */
    long long vidx = LModule_enum_variant_index(m, ename, vname);
    /* pass */
    if ((vidx < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    EnumLayout* elay = ((EnumLayout*)List_ptr_get(m->enums, LModule_enum_index(m, ename)));
    /* pass */
    VariantLayout* vlay = ((VariantLayout*)List_ptr_get(elay->variants, vidx));
    /* pass */
    if ((margs->len != vlay->fields->len)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    List_i64* vals = (void*)List_i64_new();
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < margs->len)) {
        /* pass */
        long long ftg = List_i64_get(vlay->ftags, ai);
        /* pass */
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
        /* pass */
        if ((av < 0LL)) {
            /* pass */
            List_i64_free(vals);
            return (-1LL);
        }
        /* pass */
        long long avt = LFunc_vreg_type(lf, av);
        /* pass */
        if ((ftg <= (0LL - 2LL))) {
            /* pass */
            ftg = avt;
            /* pass */
            if ((((_is_list_tag(ftg) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
                /* pass */
                List_i64_free(vals);
                return (-1LL);
            }
        }
        /* pass */
        if ((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg))) {
            /* pass */
            List_i64_free(vals);
            return (-1LL);
        }
        /* pass */
        if ((ftg == 5LL)) {
            /* pass */
            if ((avt == 0LL)) {
                /* pass */
                av = _promote_f(lf, av);
            } else if ((avt != 5LL)) {
                /* pass */
                List_i64_free(vals);
                return (-1LL);
            }
            /* pass */
            long long pfb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFBits(pfb, av));
            /* pass */
            av = pfb;
        } else if ((ftg == 1LL)) {
            /* pass */
            if ((avt != 1LL)) {
                /* pass */
                List_i64_free(vals);
                return (-1LL);
            }
            /* pass */
            _secure_str(m, lf, av);
        } else if ((!_field_tag_ok(avt, ftg))) {
            /* pass */
            List_i64_free(vals);
            return (-1LL);
        }
        /* pass */
        List_i64_append(vals, av);
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    long long szc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_enum_size(m, ename)));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_obj_alloc"));
    /* pass */
    List_i64* oaa = (void*)List_i64_new();
    /* pass */
    List_i64_append(oaa, szc);
    /* pass */
    long long od = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit("_tr_rt_obj_alloc"), oaa));
    /* pass */
    LFunc_set_vreg_type(lf, od, 11LL);
    /* pass */
    long long tgc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(tgc, vidx));
    /* pass */
    _emit_field_set(m, lf, od, 0LL, tgc);
    /* pass */
    ai = 0LL;
    /* pass */
    while ((ai < vals->len)) {
        /* pass */
        _emit_field_set(m, lf, od, ((1LL + ai) * 8LL), List_i64_get(vals, ai));
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    List_i64_free(vals);
    return od;
}

__attribute__((hot)) long long _lower_obj_call(LModule* m, LFunc* lf, TrStr mangled, long long self_vreg, List_ptr* margs) {
    /* pass */
    if ((!LModule_is_user_fn(m, mangled))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long rtag = LModule_fn_ret_tag(m, mangled);
    /* pass */
    if ((rtag < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    List_i64* cargs = (void*)List_i64_new();
    /* pass */
    if ((self_vreg >= 0LL)) {
        /* pass */
        List_i64_append(cargs, self_vreg);
    }
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < margs->len)) {
        /* pass */
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
        /* pass */
        if ((av < 0LL)) {
            /* pass */
            List_i64_free(cargs);
            return (-1LL);
        }
        /* pass */
        List_i64_append(cargs, av);
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    if ((cargs->len > 6LL)) {
        /* pass */
        List_i64_free(cargs);
        return (-1LL);
    }
    /* pass */
    long long rd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(rd, mangled, cargs));
    /* pass */
    LFunc_set_vreg_type(lf, rd, rtag);
    /* pass */
    if ((rtag == 1LL)) {
        /* pass */
        _fresh_mark(lf, rd);
    }
    /* pass */
    return rd;
}

__attribute__((hot)) bool lower_block(LModule* m, LFunc* lf, HirBlock* hb) {
    /* pass */
    lf->blk_depth = (lf->blk_depth + 1LL);
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < hb->stmts->len)) {
        /* pass */
        if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(hb->stmts, si))))) {
            /* pass */
            lf->blk_depth = (lf->blk_depth - 1LL);
            /* pass */
            return false;
        }
        /* pass */
        si = (si + 1LL);
    }
    /* pass */
    lf->blk_depth = (lf->blk_depth - 1LL);
    /* pass */
    return true;
}

__attribute__((hot)) bool _run_defers(LModule* m, LFunc* lf) {
    /* pass */
    if ((lf->defers->len == 0LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if (lf->in_defer) {
        /* pass */
        return false;
    }
    /* pass */
    lf->in_defer = true;
    /* pass */
    long long di = (lf->defers->len - 1LL);
    /* pass */
    while ((di >= 0LL)) {
        /* pass */
        if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(lf->defers, di))))) {
            /* pass */
            lf->in_defer = false;
            /* pass */
            return false;
        }
        /* pass */
        di = (di - 1LL);
    }
    /* pass */
    lf->in_defer = false;
    /* pass */
    return true;
}

__attribute__((hot)) long long _ptr_stride(LModule* m, AstType* pty) {
    /* pass */
    if ((pty->args->len == 0LL)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    TrStr en = (*((AstType**)List_ptr_get(pty->args, 0LL)))->name;
    /* pass */
    if (_is_null_str(en)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("usize"))) == 0)) || (strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("isize"))) == 0))) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((LModule_is_class(m, en) || LModule_is_enum(m, en))) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) TrStr _dunder_for_op(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        return _tr_str_lit("__add__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        return _tr_str_lit("__sub__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        return _tr_str_lit("__mul__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        return _tr_str_lit("__truediv__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        return _tr_str_lit("__mod__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("**"))) == 0)) {
        /* pass */
        return _tr_str_lit("__pow__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("//"))) == 0)) {
        /* pass */
        return _tr_str_lit("__floordiv__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return _tr_str_lit("__eq__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return _tr_str_lit("__ne__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        return _tr_str_lit("__lt__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        return _tr_str_lit("__le__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        return _tr_str_lit("__gt__");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        return _tr_str_lit("__ge__");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr _stmt_expr_kind(HirExpr* e) {
    /* pass */
    __auto_type _t2290 = (*e);
    if (_t2290.tag == HirExpr_ECall) {
        __auto_type callee = _t2290.data.ECall.callee;
        return ({ TrStr _at_t2291 = (_ident_name(callee)); __auto_type _wr = (({ TrStr _cl = (({ TrStr _cr = (_own(_at_t2291)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("call ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()"))); _tr_str_release(_cl); _cres; })); _tr_str_release(_at_t2291); _wr; });
    } else if (_t2290.tag == HirExpr_EMethodCall) {
        __auto_type meth = _t2290.data.EMethodCall.method;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(meth)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("method .")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()"))); _tr_str_release(_cl); _cres; });
    } else if (1) {
        __auto_type _ = _t2290;
        return _tr_str_lit("expression");
    }
}

__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t2292 = (*s);
    if (_t2292.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2292.data.SLineMarker.n;
        return true;
    } else if (_t2292.tag == HirStmt_SPass) {
        return true;
    } else if (_t2292.tag == HirStmt_SAutoDrop) {
        __auto_type name = _t2292.data.SAutoDrop.name;
        /* pass */
        if ((((LFunc_var_index(lf, name) >= 0LL) && (LFunc_var_type(lf, name) == 1LL)) && (!_is_param(lf, name)))) {
            /* pass */
            long long dv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(dv, name));
            /* pass */
            _release_str(m, lf, dv);
        }
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SFree) {
        __auto_type _ = _t2292.data.SFree.name;
        return true;
    } else if (_t2292.tag == HirStmt_SReturn) {
        __auto_type val = _t2292.data.SReturn.val;
        /* pass */
        if (lf->in_defer) {
            /* pass */
            return false;
        }
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            long long rv = lower_expr(m, lf, val);
            /* pass */
            if ((rv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            if ((LFunc_vreg_type(lf, rv) == 1LL)) {
                /* pass */
                _secure_str(m, lf, rv);
            }
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            if ((!_run_defers(m, lf))) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetVal(rv));
        } else {
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            if ((!_run_defers(m, lf))) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
        }
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SLet) {
        __auto_type name = _t2292.data.SLet.name;
__auto_type val = _t2292.data.SLet.val;
        /* pass */
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        long long v = lower_expr(m, lf, val);
        /* pass */
        if ((v < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((LFunc_vreg_type(lf, v) == 1LL)) {
            /* pass */
            _secure_str(m, lf, v);
        }
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        LFunc_set_var_type(lf, name, LFunc_vreg_type(lf, v));
        /* pass */
        if (((LFunc_vreg_type(lf, v) == 10LL) || (LFunc_vreg_type(lf, v) == 11LL))) {
            /* pass */
            TrStr lvcn = hir_expr_type(val)->name;
            /* pass */
            if (((!_is_null_str(lvcn)) && (LModule_is_class(m, lvcn) || LModule_is_enum(m, lvcn)))) {
                /* pass */
                ({ TrStr _at_t2293 = (_own(lvcn)); LFunc_set_var_cls(lf, name, _at_t2293); _tr_str_release(_at_t2293); });
            }
        }
        /* pass */
        if ((LFunc_vreg_type(lf, v) == 12LL)) {
            /* pass */
            LFunc_set_var_xret(lf, name, LFunc_vreg_xret_of(lf, v));
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(name, v));
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SAssign) {
        __auto_type target = _t2292.data.SAssign.target;
__auto_type val = _t2292.data.SAssign.val;
        /* pass */
        __auto_type _t2294 = (*target);
        if (_t2294.tag == HirExpr_EMethodCall) {
            __auto_type mobj = _t2294.data.EMethodCall.obj;
__auto_type mmeth = _t2294.data.EMethodCall.method;
__auto_type midx = _t2294.data.EMethodCall.args;
            /* pass */
            if (((strcmp(_tr_strz(mmeth), _tr_strz(_tr_str_lit("get_index"))) == 0) && (midx->len == 1LL))) {
                /* pass */
                return _lower_index_set(m, lf, mobj, ((HirExpr*)List_ptr_get(midx, 0LL)), val);
            }
            /* pass */
            return false;
        } else if (_t2294.tag == HirExpr_EIndex) {
            __auto_type iobj = _t2294.data.EIndex.obj;
__auto_type iidx = _t2294.data.EIndex._tr_v_index;
            /* pass */
            return _lower_index_set(m, lf, iobj, iidx, val);
        } else if (_t2294.tag == HirExpr_EPropAccess) {
            __auto_type pobj = _t2294.data.EPropAccess.obj;
__auto_type pprop = _t2294.data.EPropAccess.prop;
            /* pass */
            return _lower_field_set(m, lf, pobj, pprop, val);
        } else if (1) {
            __auto_type _ = _t2294;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr tn = _ident_name(target);
        /* pass */
        if ((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((LFunc_var_index(lf, tn) < 0LL) && (LFunc_capture_index(lf, tn) >= 0LL))) {
            /* pass */
            long long wcix = LFunc_capture_index(lf, tn);
            /* pass */
            long long wtag = List_i64_get(lf->cap_tags, wcix);
            /* pass */
            if ((((wtag != 0LL) && (wtag != 4LL)) && (wtag != 5LL))) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            long long wv = lower_expr(m, lf, val);
            /* pass */
            if ((wv < 0LL)) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            long long wvt = LFunc_vreg_type(lf, wv);
            /* pass */
            if ((wtag == 5LL)) {
                /* pass */
                if ((wvt == 0LL)) {
                    /* pass */
                    wv = _promote_f(lf, wv);
                } else if ((wvt != 5LL)) {
                    /* pass */
                    _tr_str_release(tn);
                    return false;
                }
                /* pass */
                long long wb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(wb, wv));
                /* pass */
                wv = wb;
            } else if ((!_field_tag_ok(wvt, wtag))) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            long long wenv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(wenv, _tr_str_lit("__env")));
            /* pass */
            long long waddr = _emit_field_get(m, lf, wenv, ((1LL + wcix) * 8LL), 0LL);
            /* pass */
            _emit_field_set(m, lf, waddr, 0LL, wv);
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            _tr_str_release(tn);
            return true;
        }
        /* pass */
        long long v2 = lower_expr(m, lf, val);
        /* pass */
        if ((v2 < 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        bool had_old_str = ((LFunc_var_index(lf, tn) >= 0LL) && (LFunc_var_type(lf, tn) == 1LL));
        /* pass */
        if ((LFunc_vreg_type(lf, v2) == 1LL)) {
            /* pass */
            _secure_str(m, lf, v2);
        }
        /* pass */
        if (((LFunc_var_index(lf, tn) < 0LL) && LModule_is_global(m, tn))) {
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreGlobal(LModule_global_index(m, tn), v2));
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            _tr_str_release(tn);
            return true;
        }
        /* pass */
        if (had_old_str) {
            /* pass */
            long long oldv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(oldv, tn));
            /* pass */
            _release_str(m, lf, oldv);
        }
        /* pass */
        LFunc_add_var(lf, tn);
        /* pass */
        LFunc_set_var_type(lf, tn, LFunc_vreg_type(lf, v2));
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(tn, v2));
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        _tr_str_release(tn);
        return true;
    } else if (_t2292.tag == HirStmt_SIf) {
        __auto_type cond = _t2292.data.SIf.cond;
__auto_type then_b = _t2292.data.SIf.then_b;
__auto_type else_b = _t2292.data.SIf.else_b;
        /* pass */
        long long cv = lower_expr(m, lf, cond);
        /* pass */
        if ((cv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        long long then_id = LFunc_new_block(lf);
        /* pass */
        long long else_id = LFunc_new_block(lf);
        /* pass */
        long long end_id = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv, then_id, else_id));
        /* pass */
        LFunc_set_cur(lf, then_id);
        /* pass */
        if ((!lower_block(m, lf, then_b))) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, else_id);
        /* pass */
        if ((!lower_block(m, lf, else_b))) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, end_id);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SWhile) {
        __auto_type cond = _t2292.data.SWhile.cond;
__auto_type body = _t2292.data.SWhile.body;
        /* pass */
        long long hdr = LFunc_new_block(lf);
        /* pass */
        long long bdy = LFunc_new_block(lf);
        /* pass */
        long long ext = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
        /* pass */
        LFunc_set_cur(lf, hdr);
        /* pass */
        long long cv2 = lower_expr(m, lf, cond);
        /* pass */
        if ((cv2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv2, bdy, ext));
        /* pass */
        LFunc_set_cur(lf, bdy);
        /* pass */
        List_i64_append(lf->loop_cont, hdr);
        /* pass */
        List_i64_append(lf->loop_brk, ext);
        /* pass */
        bool wok = lower_block(m, lf, body);
        /* pass */
        List_i64_pop(lf->loop_cont);
        /* pass */
        List_i64_pop(lf->loop_brk);
        /* pass */
        if ((!wok)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
        /* pass */
        LFunc_set_cur(lf, ext);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SBreak) {
        __auto_type bval = _t2292.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(bval)) != ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        if ((lf->loop_brk->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_brk, (lf->loop_brk->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SContinue) {
        /* pass */
        if ((lf->loop_cont->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_cont, (lf->loop_cont->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SAssert) {
        __auto_type acond = _t2292.data.SAssert.cond;
        /* pass */
        long long acv = lower_expr(m, lf, acond);
        /* pass */
        if ((acv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        long long a_ok = LFunc_new_block(lf);
        /* pass */
        long long a_fail = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(acv, a_ok, a_fail));
        /* pass */
        LFunc_set_cur(lf, a_fail);
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_assert_fail"));
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_assert_fail"), (void*)List_i64_new()));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(a_ok));
        /* pass */
        LFunc_set_cur(lf, a_ok);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SMultiLet) {
        __auto_type mnames = _t2292.data.SMultiLet.names;
__auto_type mval = _t2292.data.SMultiLet.val;
        /* pass */
        long long mtv = lower_expr(m, lf, mval);
        /* pass */
        if ((mtv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((LFunc_vreg_type(lf, mtv) != 15LL)) {
            /* pass */
            return false;
        }
        /* pass */
        AstType* mty = hir_expr_type(mval);
        /* pass */
        if ((mty->args->len != mnames->len)) {
            /* pass */
            return false;
        }
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < mnames->len)) {
            /* pass */
            long long mtag = _tag_of(m, (*((AstType**)List_ptr_get(mty->args, mi))));
            /* pass */
            if (((((mtag < 0LL) || _is_list_tag(mtag)) || _is_dict_tag(mtag)) || _is_set_tag(mtag))) {
                /* pass */
                return false;
            }
            /* pass */
            long long mv = (-1LL);
            /* pass */
            if ((mtag == 5LL)) {
                /* pass */
                long long mraw = _emit_field_get(m, lf, mtv, (mi * 8LL), 0LL);
                /* pass */
                mv = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(mv, mraw));
                /* pass */
                LFunc_set_vreg_type(lf, mv, 5LL);
            } else {
                /* pass */
                mv = _emit_field_get(m, lf, mtv, (mi * 8LL), mtag);
                /* pass */
                if ((mtag == 1LL)) {
                    /* pass */
                    _secure_str(m, lf, mv);
                }
            }
            /* pass */
            TrStr mnm = List_TrStr_get(mnames, mi);
            /* pass */
            LFunc_add_var(lf, mnm);
            /* pass */
            LFunc_set_var_type(lf, mnm, mtag);
            /* pass */
            if (((mtag == 10LL) || (mtag == 11LL))) {
                /* pass */
                ({ TrStr _at_t2295 = (_cls_of_ty(m, (*((AstType**)List_ptr_get(mty->args, mi))))); LFunc_set_var_cls(lf, mnm, _at_t2295); _tr_str_release(_at_t2295); });
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(mnm, mv));
            /* pass */
            mi = (mi + 1LL);
            _tr_str_release(mnm);
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SDefer) {
        __auto_type dstmt = _t2292.data.SDefer.stmt;
        /* pass */
        if (lf->in_defer) {
            /* pass */
            return false;
        }
        /* pass */
        if ((lf->blk_depth != 1LL)) {
            /* pass */
            return false;
        }
        /* pass */
        List_ptr_append(lf->defers, dstmt);
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SWith) {
        __auto_type witems = _t2292.data.SWith.items;
__auto_type waliases = _t2292.data.SWith.aliases;
__auto_type wbody = _t2292.data.SWith.body;
        /* pass */
        if ((witems->len != 1LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long wctx = lower_expr(m, lf, ((HirExpr*)List_ptr_get(witems, 0LL)));
        /* pass */
        if ((wctx < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr wcls = _recv_class(m, lf, ((HirExpr*)List_ptr_get(witems, 0LL)));
        /* pass */
        if (((strcmp(_tr_strz(wcls), _tr_strz(_tr_str_lit(""))) == 0) || (!LModule_is_class(m, wcls)))) {
            /* pass */
            _tr_str_release(wcls);
            return false;
        }
        /* pass */
        TrStr w_enter = LModule_resolve_method(m, wcls, _tr_str_lit("__enter__"));
        /* pass */
        TrStr w_exit = LModule_resolve_method(m, wcls, _tr_str_lit("__exit__"));
        /* pass */
        if (((strcmp(_tr_strz(w_enter), _tr_strz(_tr_str_lit(""))) == 0) || (strcmp(_tr_strz(w_exit), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            _tr_str_release(wcls);
            _tr_str_release(w_enter);
            _tr_str_release(w_exit);
            return false;
        }
        /* pass */
        TrStr w_var = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__with")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, w_var);
        /* pass */
        LFunc_set_var_type(lf, w_var, 10LL);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(w_var, wctx));
        /* pass */
        List_i64* w_ea = (void*)List_i64_new();
        /* pass */
        List_i64_append(w_ea, wctx);
        /* pass */
        long long w_rd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(w_rd, w_enter, w_ea));
        /* pass */
        LFunc_set_vreg_type(lf, w_rd, LModule_fn_ret_tag(m, w_enter));
        /* pass */
        if (({ TrStr _at_t2296 = (List_TrStr_get(waliases, 0LL)); __auto_type _wr = ((((waliases->len > 0LL) && (!_is_null_str(_at_t2296))) && (strcmp(_tr_strz(List_TrStr_get(waliases, 0LL)), _tr_strz(_tr_str_lit(""))) != 0))); _tr_str_release(_at_t2296); _wr; })) {
            /* pass */
            TrStr w_an = List_TrStr_get(waliases, 0LL);
            /* pass */
            LFunc_add_var(lf, w_an);
            /* pass */
            LFunc_set_var_type(lf, w_an, LFunc_vreg_type(lf, w_rd));
            /* pass */
            if (((LFunc_vreg_type(lf, w_rd) == 10LL) || (LFunc_vreg_type(lf, w_rd) == 11LL))) {
                /* pass */
                ({ TrStr _at_t2297 = (_own(wcls)); LFunc_set_var_cls(lf, w_an, _at_t2297); _tr_str_release(_at_t2297); });
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(w_an, w_rd));
        }
        /* pass */
        if ((!lower_block(m, lf, wbody))) {
            /* pass */
            _tr_str_release(wcls);
            _tr_str_release(w_enter);
            _tr_str_release(w_exit);
            _tr_str_release(w_var);
            return false;
        }
        /* pass */
        long long w_c2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(w_c2, w_var));
        /* pass */
        LFunc_set_vreg_type(lf, w_c2, 10LL);
        /* pass */
        List_i64* w_xa = (void*)List_i64_new();
        /* pass */
        List_i64_append(w_xa, w_c2);
        /* pass */
        long long w_ei = 0LL;
        /* pass */
        while ((w_ei < 3LL)) {
            /* pass */
            long long w_es = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStr(w_es, LModule_add_string(m, _tr_str_lit(""))));
            /* pass */
            LFunc_set_vreg_type(lf, w_es, 1LL);
            /* pass */
            List_i64_append(w_xa, w_es);
            /* pass */
            w_ei = (w_ei + 1LL);
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), w_exit, w_xa));
        /* pass */
        _tr_str_release(wcls);
        _tr_str_release(w_enter);
        _tr_str_release(w_exit);
        _tr_str_release(w_var);
        return true;
    } else if (_t2292.tag == HirStmt_STry) {
        __auto_type try_body = _t2292.data.STry.try_body;
__auto_type catches = _t2292.data.STry.catches;
__auto_type finally_b = _t2292.data.STry.finally_b;
        /* pass */
        if ((catches->len != 1LL)) {
            /* pass */
            return false;
        }
        /* pass */
        HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, 0LL)));
        /* pass */
        if ((!_is_null_str(tcc->err_type->name))) {
            /* pass */
            if ((((strcmp(_tr_strz(tcc->err_type->name), _tr_strz(_tr_str_lit(""))) != 0) && (strcmp(_tr_strz(tcc->err_type->name), _tr_strz(_tr_str_lit("void"))) != 0)) && (strcmp(_tr_strz(tcc->err_type->name), _tr_strz(_tr_str_lit("str"))) != 0))) {
                /* pass */
                return false;
            }
        }
        /* pass */
        long long t_exc = LFunc_new_block(lf);
        /* pass */
        long long t_end = LFunc_new_block(lf);
        /* pass */
        TrStr t_msg = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__exmsg")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, t_msg);
        /* pass */
        LFunc_set_var_type(lf, t_msg, 1LL);
        /* pass */
        long long t_seed = _heap_lit(m, lf, _tr_str_lit(""));
        /* pass */
        _fresh_take(lf, t_seed);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(t_msg, t_seed));
        /* pass */
        List_i64_append(lf->try_blks, t_exc);
        /* pass */
        List_TrStr_append(lf->try_msgs, t_msg);
        /* pass */
        bool t_ok = lower_block(m, lf, try_body);
        /* pass */
        List_i64_pop(lf->try_blks);
        /* pass */
        List_TrStr_pop(lf->try_msgs);
        /* pass */
        if ((!t_ok)) {
            /* pass */
            _tr_str_release(t_msg);
            return false;
        }
        /* pass */
        if ((!lower_block(m, lf, finally_b))) {
            /* pass */
            _tr_str_release(t_msg);
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(t_end));
        /* pass */
        LFunc_set_cur(lf, t_exc);
        /* pass */
        if (((!_is_null_str(tcc->err_name)) && (strcmp(_tr_strz(tcc->err_name), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            long long t_ev = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(t_ev, t_msg));
            /* pass */
            LFunc_set_vreg_type(lf, t_ev, 1LL);
            /* pass */
            _retain_str(m, lf, t_ev);
            /* pass */
            LFunc_add_var(lf, tcc->err_name);
            /* pass */
            LFunc_set_var_type(lf, tcc->err_name, 1LL);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(tcc->err_name, t_ev));
        }
        /* pass */
        if ((!lower_block(m, lf, tcc->body))) {
            /* pass */
            _tr_str_release(t_msg);
            return false;
        }
        /* pass */
        if ((!lower_block(m, lf, finally_b))) {
            /* pass */
            _tr_str_release(t_msg);
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(t_end));
        /* pass */
        LFunc_set_cur(lf, t_end);
        /* pass */
        _tr_str_release(t_msg);
        return true;
    } else if (_t2292.tag == HirStmt_SRaise) {
        __auto_type rval = _t2292.data.SRaise.val;
        /* pass */
        if ((lf->try_blks->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
            /* pass */
            long long rv = lower_expr(m, lf, rval);
            /* pass */
            if ((rv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            if ((LFunc_vreg_type(lf, rv) != 1LL)) {
                /* pass */
                return false;
            }
            /* pass */
            _secure_str(m, lf, rv);
            /* pass */
            ({ TrStr _at_t2298 = (List_TrStr_get(lf->try_msgs, (lf->try_msgs->len - 1LL))); LFunc_emit(lf, LInst_ctor_IStoreVar(_at_t2298, rv)); _tr_str_release(_at_t2298); });
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->try_blks, (lf->try_blks->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t2292.tag == HirStmt_SMatch) {
        __auto_type mexpr = _t2292.data.SMatch.expr;
__auto_type marms = _t2292.data.SMatch.arms;
        /* pass */
        return _lower_match(m, lf, mexpr, marms);
    } else if (_t2292.tag == HirStmt_SFor) {
        __auto_type var = _t2292.data.SFor.var;
__auto_type iter = _t2292.data.SFor.iter;
__auto_type body = _t2292.data.SFor.body;
        /* pass */
        return _lower_for(m, lf, var, iter, body);
    } else if (_t2292.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t2292.data.SForUnpack.vars;
__auto_type iter = _t2292.data.SForUnpack.iter;
__auto_type body = _t2292.data.SForUnpack.body;
        /* pass */
        return _lower_for_unpack(m, lf, vars, iter, body);
    } else if (_t2292.tag == HirStmt_SExpr) {
        __auto_type e = _t2292.data.SExpr.expr;
        /* pass */
        bool se_ok = lower_expr_stmt(m, lf, e);
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        if (((!se_ok) && (strcmp(_tr_strz(m->fail_note), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_stmt_expr_kind(e)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("unsupported expression statement (")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
        }
        /* pass */
        return se_ok;
    } else if (_t2292.tag == HirStmt_SUnsafe) {
        __auto_type ubody = _t2292.data.SUnsafe.body;
        /* pass */
        return lower_block(m, lf, ubody);
    } else if (1) {
        __auto_type _ = _t2292;
        /* pass */
        if ((strcmp(_tr_strz(m->fail_note), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            m->fail_note = _tr_str_lit("unsupported statement kind");
        }
        /* pass */
        return false;
    }
}

__attribute__((hot)) long long _lower_set_method(LModule* m, LFunc* lf, long long shv, long long stag, TrStr method, List_ptr* margs) {
    /* pass */
    long long want_e = 0LL;
    /* pass */
    if ((stag == 16LL)) {
        /* pass */
        want_e = 1LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("length"))) == 0))) {
        /* pass */
        TrStr slsym = _set_sym(stag, _tr_str_lit("len"));
        /* pass */
        LModule_add_extern(m, slsym);
        /* pass */
        List_i64* sla = (void*)List_i64_new();
        /* pass */
        List_i64_append(sla, shv);
        /* pass */
        long long sld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sld, slsym, sla));
        /* pass */
        _tr_str_release(slsym);
        return sld;
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0)) {
        /* pass */
        TrStr sesym = _set_sym(stag, _tr_str_lit("len"));
        /* pass */
        LModule_add_extern(m, sesym);
        /* pass */
        List_i64* sea = (void*)List_i64_new();
        /* pass */
        List_i64_append(sea, shv);
        /* pass */
        long long sed = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sed, sesym, sea));
        /* pass */
        long long sez = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(sez, 0LL));
        /* pass */
        long long ser = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(ser, _tr_str_lit("=="), sed, sez));
        /* pass */
        LFunc_set_vreg_type(lf, ser, 4LL);
        /* pass */
        _tr_str_release(sesym);
        return ser;
    }
    /* pass */
    if ((margs->len != 1LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long sav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
    /* pass */
    if ((sav < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((LFunc_vreg_type(lf, sav) != want_e)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add"))) == 0)) {
        /* pass */
        TrStr sasym = _set_sym(stag, _tr_str_lit("set"));
        /* pass */
        LModule_add_extern(m, sasym);
        /* pass */
        long long saone = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(saone, 1LL));
        /* pass */
        List_i64* saa = (void*)List_i64_new();
        /* pass */
        List_i64_append(saa, shv);
        /* pass */
        List_i64_append(saa, sav);
        /* pass */
        List_i64_append(saa, saone);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sasym, saa));
        /* pass */
        _tr_str_release(sasym);
        return shv;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("has"))) == 0))) {
        /* pass */
        TrStr scsym = _set_sym(stag, _tr_str_lit("has"));
        /* pass */
        LModule_add_extern(m, scsym);
        /* pass */
        List_i64* sca = (void*)List_i64_new();
        /* pass */
        List_i64_append(sca, shv);
        /* pass */
        List_i64_append(sca, sav);
        /* pass */
        long long scd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(scd, scsym, sca));
        /* pass */
        LFunc_set_vreg_type(lf, scd, 4LL);
        /* pass */
        _tr_str_release(scsym);
        return scd;
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove"))) == 0)) {
        /* pass */
        TrStr srsym = _set_sym(stag, _tr_str_lit("remove"));
        /* pass */
        LModule_add_extern(m, srsym);
        /* pass */
        List_i64* sra = (void*)List_i64_new();
        /* pass */
        List_i64_append(sra, shv);
        /* pass */
        List_i64_append(sra, sav);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), srsym, sra));
        /* pass */
        _tr_str_release(srsym);
        return shv;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _lit_pat_cond(LModule* m, LFunc* lf, Pattern pat, long long subj, long long st) {
    /* pass */
    __auto_type _t2299 = pat;
    if (_t2299.tag == Pattern_PLitInt) {
        __auto_type v = _t2299.data.PLitInt.val;
        /* pass */
        if ((st != 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long cv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(cv, v));
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(d, _tr_str_lit("=="), subj, cv));
        /* pass */
        LFunc_set_vreg_type(lf, d, 4LL);
        /* pass */
        return d;
    } else if (_t2299.tag == Pattern_PLitBool) {
        __auto_type bv = _t2299.data.PLitBool.val;
        /* pass */
        if ((st != 4LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long bc = 0LL;
        /* pass */
        if (bv) {
            /* pass */
            bc = 1LL;
        }
        /* pass */
        long long cvb = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(cvb, bc));
        /* pass */
        long long db = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(db, _tr_str_lit("=="), subj, cvb));
        /* pass */
        LFunc_set_vreg_type(lf, db, 4LL);
        /* pass */
        return db;
    } else if (_t2299.tag == Pattern_PLitStr) {
        __auto_type sv = _t2299.data.PLitStr.val;
        /* pass */
        if ((st != 1LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long idx = LModule_add_string(m, sv);
        /* pass */
        long long lit = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(lit, idx));
        /* pass */
        LFunc_set_vreg_type(lf, lit, 1LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_cmp"));
        /* pass */
        List_i64* sa = (void*)List_i64_new();
        /* pass */
        List_i64_append(sa, subj);
        /* pass */
        List_i64_append(sa, lit);
        /* pass */
        long long cmpv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit("_tr_rt_str_cmp"), sa));
        /* pass */
        long long z = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
        /* pass */
        long long ds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(ds, _tr_str_lit("=="), cmpv, z));
        /* pass */
        LFunc_set_vreg_type(lf, ds, 4LL);
        /* pass */
        return ds;
    } else if (1) {
        __auto_type _ = _t2299;
        /* pass */
        return (-1LL);
    }
}

__attribute__((hot)) bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms) {
    /* pass */
    TrStr subj_ty = hir_expr_type(expr)->name;
    /* pass */
    bool subj_is_str = false;
    /* pass */
    if ((!_is_null_str(subj_ty))) {
        /* pass */
        subj_is_str = (strcmp(_tr_strz(subj_ty), _tr_strz(_tr_str_lit("str"))) == 0);
    }
    /* pass */
    if (subj_is_str) {
        /* pass */
        __auto_type _t2300 = (*expr);
        if (_t2300.tag == HirExpr_EIdent) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t2300;
            return false;
        }
    }
    /* pass */
    long long subj = lower_expr(m, lf, expr);
    /* pass */
    if ((subj < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long st = LFunc_vreg_type(lf, subj);
    /* pass */
    if ((st == 11LL)) {
        /* pass */
        return _lower_match_enum(m, lf, expr, subj, arms);
    }
    /* pass */
    if ((((st != 0LL) && (st != 1LL)) && (st != 4LL))) {
        /* pass */
        return false;
    }
    /* pass */
    _flush_fresh_strs(m, lf);
    /* pass */
    long long end_id = LFunc_new_block(lf);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < arms->len)) {
        /* pass */
        HirMatchArm* arm = ((HirMatchArm*)List_ptr_get(arms, i));
        /* pass */
        bool is_default = false;
        /* pass */
        TrStr bind_name = _tr_str_lit("");
        /* pass */
        long long cond = (-1LL);
        /* pass */
        __auto_type _t2301 = arm->pat;
        if (_t2301.tag == Pattern_PWild) {
            /* pass */
            is_default = true;
        } else if (_t2301.tag == Pattern_PBind) {
            __auto_type nm = _t2301.data.PBind.name;
            /* pass */
            is_default = true;
            /* pass */
            TrStr _strtmp_t2302 = _tr_str_retain(nm);
            _tr_str_release(bind_name);
            bind_name = _strtmp_t2302;
        } else if (_t2301.tag == Pattern_POr) {
            __auto_type pats = _t2301.data.POr.patterns;
            /* pass */
            long long oi = 0LL;
            /* pass */
            while ((oi < pats->len)) {
                /* pass */
                long long sc = _lit_pat_cond(m, lf, List_Pattern_get(pats, oi), subj, st);
                /* pass */
                if ((sc < 0LL)) {
                    /* pass */
                    return false;
                }
                /* pass */
                if ((cond < 0LL)) {
                    /* pass */
                    cond = sc;
                } else {
                    /* pass */
                    long long merged = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit("+"), cond, sc));
                    /* pass */
                    cond = _norm_bool(lf, merged);
                }
                /* pass */
                oi = (oi + 1LL);
            }
            /* pass */
            if ((cond < 0LL)) {
                /* pass */
                return false;
            }
        } else if (1) {
            __auto_type _ = _t2301;
            /* pass */
            cond = _lit_pat_cond(m, lf, arm->pat, subj, st);
            /* pass */
            if ((cond < 0LL)) {
                /* pass */
                return false;
            }
        }
        /* pass */
        long long body_id = LFunc_new_block(lf);
        /* pass */
        long long next_id = LFunc_new_block(lf);
        /* pass */
        if (is_default) {
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TBr(body_id));
        } else {
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, body_id, next_id));
        }
        /* pass */
        LFunc_set_cur(lf, body_id);
        /* pass */
        if ((strcmp(_tr_strz(bind_name), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            LFunc_add_var(lf, bind_name);
            /* pass */
            LFunc_set_var_type(lf, bind_name, st);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(bind_name, subj));
        }
        /* pass */
        if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
            /* pass */
            long long gv = lower_expr(m, lf, arm->guard);
            /* pass */
            if ((gv < 0LL)) {
                /* pass */
                _tr_str_release(bind_name);
                return false;
            }
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            long long gbody = LFunc_new_block(lf);
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TCondBr(gv, gbody, next_id));
            /* pass */
            LFunc_set_cur(lf, gbody);
        }
        /* pass */
        if ((!lower_block(m, lf, arm->body))) {
            /* pass */
            _tr_str_release(bind_name);
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, next_id);
        /* pass */
        i = (i + 1LL);
        _tr_str_release(bind_name);
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
    /* pass */
    LFunc_set_cur(lf, end_id);
    /* pass */
    return true;
}

__attribute__((hot)) TrStr _norm_variant(TrStr ename, TrStr vn) {
    /* pass */
    if ((strcmp(_tr_strz(ename), _tr_strz(_tr_str_lit("Option"))) == 0)) {
        /* pass */
        if (((_is_null_str(vn) || (strcmp(_tr_strz(vn), _tr_strz(_tr_str_lit(""))) == 0)) || (strcmp(_tr_strz(vn), _tr_strz(_tr_str_lit("none"))) == 0))) {
            /* pass */
            return _tr_str_lit("None");
        }
    }
    /* pass */
    return _tr_str_retain(vn);
}

__attribute__((hot)) long long _variant_tag_cond(LFunc* lf, long long tagv, long long vidx) {
    /* pass */
    long long cv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(cv, vidx));
    /* pass */
    long long dv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(dv, _tr_str_lit("=="), tagv, cv));
    /* pass */
    LFunc_set_vreg_type(lf, dv, 4LL);
    /* pass */
    return dv;
}

__attribute__((hot)) bool _bind_payload(LModule* m, LFunc* lf, VariantLayout* vlay, long long subj, AstType* subj_ty, long long fldidx, TrStr bindname) {
    /* pass */
    if ((fldidx >= vlay->ftags->len)) {
        /* pass */
        return false;
    }
    /* pass */
    long long ftg = List_i64_get(vlay->ftags, fldidx);
    /* pass */
    TrStr bcls = List_TrStr_get(vlay->fcls, fldidx);
    /* pass */
    if ((ftg <= (0LL - 2LL))) {
        /* pass */
        long long bgi = ((0LL - ftg) - 2LL);
        /* pass */
        if ((bgi >= subj_ty->args->len)) {
            /* pass */
            _tr_str_release(bcls);
            return false;
        }
        /* pass */
        ftg = _tag_of(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        /* pass */
        TrStr _strtmp_t2303 = _cls_of_ty(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        _tr_str_release(bcls);
        bcls = _strtmp_t2303;
    }
    /* pass */
    if ((((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
        /* pass */
        _tr_str_release(bcls);
        return false;
    }
    /* pass */
    long long pv = (-1LL);
    /* pass */
    if ((ftg == 5LL)) {
        /* pass */
        long long praw = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), 0LL);
        /* pass */
        pv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(pv, praw));
        /* pass */
        LFunc_set_vreg_type(lf, pv, 5LL);
    } else {
        /* pass */
        pv = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), ftg);
        /* pass */
        if ((ftg == 1LL)) {
            /* pass */
            _secure_str(m, lf, pv);
        }
    }
    /* pass */
    LFunc_add_var(lf, bindname);
    /* pass */
    LFunc_set_var_type(lf, bindname, ftg);
    /* pass */
    if (((ftg == 10LL) || (ftg == 11LL))) {
        /* pass */
        if (((!_is_null_str(bcls)) && (strcmp(_tr_strz(bcls), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            ({ TrStr _at_t2304 = (_own(bcls)); LFunc_set_var_cls(lf, bindname, _at_t2304); _tr_str_release(_at_t2304); });
        }
    }
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(bindname, pv));
    /* pass */
    _tr_str_release(bcls);
    return true;
}

__attribute__((hot)) bool _lower_match_enum(LModule* m, LFunc* lf, HirExpr* expr, long long subj, List_ptr* arms) {
    /* pass */
    TrStr ename = _recv_class(m, lf, expr);
    /* pass */
    if (((strcmp(_tr_strz(ename), _tr_strz(_tr_str_lit(""))) == 0) || (!LModule_is_enum(m, ename)))) {
        /* pass */
        _tr_str_release(ename);
        return false;
    }
    /* pass */
    AstType* subj_hty = hir_expr_type(expr);
    /* pass */
    EnumLayout* elay = ((EnumLayout*)List_ptr_get(m->enums, LModule_enum_index(m, ename)));
    /* pass */
    long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
    /* pass */
    long long end_id = LFunc_new_block(lf);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < arms->len)) {
        /* pass */
        HirMatchArm* arm = ((HirMatchArm*)List_ptr_get(arms, i));
        /* pass */
        bool is_default = false;
        /* pass */
        TrStr bind_subj = _tr_str_lit("");
        /* pass */
        long long cond = (-1LL);
        /* pass */
        __auto_type _t2305 = arm->pat;
        if (_t2305.tag == Pattern_PWild) {
            /* pass */
            is_default = true;
        } else if (_t2305.tag == Pattern_PBind) {
            __auto_type nm = _t2305.data.PBind.name;
            /* pass */
            is_default = true;
            /* pass */
            TrStr _strtmp_t2306 = _tr_str_retain(nm);
            _tr_str_release(bind_subj);
            bind_subj = _strtmp_t2306;
        } else if (_t2305.tag == Pattern_PVariant) {
            __auto_type vn = _t2305.data.PVariant.variant;
            /* pass */
            long long vix = ({ TrStr _at_t2307 = (_norm_variant(ename, vn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t2307)); _tr_str_release(_at_t2307); _wr; });
            /* pass */
            if ((vix < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vix);
        } else if (_t2305.tag == Pattern_PVariantBind) {
            __auto_type vnb = _t2305.data.PVariantBind.variant;
            /* pass */
            long long vixb = ({ TrStr _at_t2308 = (_norm_variant(ename, vnb)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t2308)); _tr_str_release(_at_t2308); _wr; });
            /* pass */
            if ((vixb < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vixb);
        } else if (_t2305.tag == Pattern_PVariantBindMany) {
            __auto_type vnm = _t2305.data.PVariantBindMany.variant;
            /* pass */
            long long vixm = ({ TrStr _at_t2309 = (_norm_variant(ename, vnm)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t2309)); _tr_str_release(_at_t2309); _wr; });
            /* pass */
            if ((vixm < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vixm);
        } else if (_t2305.tag == Pattern_POr) {
            __auto_type orpats = _t2305.data.POr.patterns;
            /* pass */
            long long oi = 0LL;
            /* pass */
            while ((oi < orpats->len)) {
                /* pass */
                long long oc = (-1LL);
                /* pass */
                __auto_type _t2310 = List_Pattern_get(orpats, oi);
                if (_t2310.tag == Pattern_PVariant) {
                    __auto_type ovn = _t2310.data.PVariant.variant;
                    /* pass */
                    long long ovix = ({ TrStr _at_t2311 = (_norm_variant(ename, ovn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t2311)); _tr_str_release(_at_t2311); _wr; });
                    /* pass */
                    if ((ovix < 0LL)) {
                        /* pass */
                        _tr_str_release(ename);
                        return false;
                    }
                    /* pass */
                    oc = _variant_tag_cond(lf, tagv, ovix);
                } else if (1) {
                    __auto_type _ = _t2310;
                    /* pass */
                    _tr_str_release(ename);
                    return false;
                }
                /* pass */
                if ((cond < 0LL)) {
                    /* pass */
                    cond = oc;
                } else {
                    /* pass */
                    long long merged = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit("+"), cond, oc));
                    /* pass */
                    cond = _norm_bool(lf, merged);
                }
                /* pass */
                oi = (oi + 1LL);
            }
            /* pass */
            if ((cond < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
        } else if (1) {
            __auto_type _ = _t2305;
            /* pass */
            _tr_str_release(ename);
            return false;
        }
        /* pass */
        long long body_id = LFunc_new_block(lf);
        /* pass */
        long long next_id = LFunc_new_block(lf);
        /* pass */
        if (is_default) {
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TBr(body_id));
        } else {
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, body_id, next_id));
        }
        /* pass */
        LFunc_set_cur(lf, body_id);
        /* pass */
        if ((strcmp(_tr_strz(bind_subj), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            LFunc_add_var(lf, bind_subj);
            /* pass */
            LFunc_set_var_type(lf, bind_subj, 11LL);
            /* pass */
            ({ TrStr _at_t2312 = (_own(ename)); LFunc_set_var_cls(lf, bind_subj, _at_t2312); _tr_str_release(_at_t2312); });
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(bind_subj, subj));
        }
        /* pass */
        __auto_type _t2313 = arm->pat;
        if (_t2313.tag == Pattern_PVariantBind) {
            __auto_type vnb2 = _t2313.data.PVariantBind.variant;
__auto_type bnm = _t2313.data.PVariantBind.field;
            /* pass */
            VariantLayout* vlay1 = ({ TrStr _at_t2314 = (_norm_variant(ename, vnb2)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, EnumLayout_variant_index(elay, _at_t2314)))); _tr_str_release(_at_t2314); _wr; });
            /* pass */
            if ((strcmp(_tr_strz(bnm), _tr_strz(_tr_str_lit("_"))) != 0)) {
                /* pass */
                if ((!_bind_payload(m, lf, vlay1, subj, subj_hty, 0LL, bnm))) {
                    /* pass */
                    _tr_str_release(ename);
                    return false;
                }
            }
        } else if (_t2313.tag == Pattern_PVariantBindMany) {
            __auto_type vnm2 = _t2313.data.PVariantBindMany.variant;
__auto_type bnames = _t2313.data.PVariantBindMany.fields;
            /* pass */
            VariantLayout* vlay2 = ({ TrStr _at_t2315 = (_norm_variant(ename, vnm2)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, EnumLayout_variant_index(elay, _at_t2315)))); _tr_str_release(_at_t2315); _wr; });
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < bnames->len)) {
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(bnames, bi)), _tr_strz(_tr_str_lit("_"))) != 0)) {
                    /* pass */
                    if (({ TrStr _at_t2316 = (List_TrStr_get(bnames, bi)); __auto_type _wr = ((!_bind_payload(m, lf, vlay2, subj, subj_hty, bi, _at_t2316))); _tr_str_release(_at_t2316); _wr; })) {
                        /* pass */
                        _tr_str_release(ename);
                        return false;
                    }
                }
                /* pass */
                bi = (bi + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t2313;
            /* pass */
            /* pass */
        }
        /* pass */
        if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
            /* pass */
            long long egv = lower_expr(m, lf, arm->guard);
            /* pass */
            if ((egv < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                _tr_str_release(bind_subj);
                return false;
            }
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            long long egbody = LFunc_new_block(lf);
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TCondBr(egv, egbody, next_id));
            /* pass */
            LFunc_set_cur(lf, egbody);
        }
        /* pass */
        if ((!lower_block(m, lf, arm->body))) {
            /* pass */
            _tr_str_release(ename);
            _tr_str_release(bind_subj);
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, next_id);
        /* pass */
        i = (i + 1LL);
        _tr_str_release(bind_subj);
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
    /* pass */
    LFunc_set_cur(lf, end_id);
    /* pass */
    _tr_str_release(ename);
    return true;
}

__attribute__((hot)) bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    __auto_type _t2317 = (*iter);
    if (_t2317.tag == HirExpr_ECall) {
        __auto_type callee = _t2317.data.ECall.callee;
__auto_type args = _t2317.data.ECall.args;
        /* pass */
        if ((strcmp(_tr_strz(_ident_name(callee)), _tr_strz(_tr_str_lit("range"))) == 0)) {
            /* pass */
            return _lower_for_range(m, lf, var, args, body);
        }
    } else if (1) {
        __auto_type _ = _t2317;
        /* pass */
        /* pass */
    }
    /* pass */
    return _lower_for_list(m, lf, var, iter, body);
}

__attribute__((hot)) bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body) {
    /* pass */
    if (((args->len < 1LL) || (args->len > 3LL))) {
        /* pass */
        return false;
    }
    /* pass */
    long long stepv = 1LL;
    /* pass */
    TrStr cmp = _tr_str_lit("<");
    /* pass */
    if ((args->len == 3LL)) {
        /* pass */
        if ((!_is_const_int(((HirExpr*)List_ptr_get(args, 2LL))))) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        stepv = _const_int_val(((HirExpr*)List_ptr_get(args, 2LL)));
        /* pass */
        if ((stepv == 0LL)) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        if ((stepv < 0LL)) {
            /* pass */
            TrStr _strtmp_t2318 = _tr_str_lit(">");
            _tr_str_release(cmp);
            cmp = _strtmp_t2318;
        }
    }
    /* pass */
    long long sv = (-1LL);
    /* pass */
    HirExpr* end_expr = ((HirExpr*)List_ptr_get(args, 0LL));
    /* pass */
    if ((args->len == 1LL)) {
        /* pass */
        long long z = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
        /* pass */
        sv = z;
    } else {
        /* pass */
        sv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
        /* pass */
        if ((sv < 0LL)) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        end_expr = ((HirExpr*)List_ptr_get(args, 1LL));
    }
    /* pass */
    LFunc_add_var(lf, var);
    /* pass */
    LFunc_set_var_type(lf, var, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, sv));
    /* pass */
    long long hdr = LFunc_new_block(lf);
    /* pass */
    long long bdy = LFunc_new_block(lf);
    /* pass */
    long long latch = LFunc_new_block(lf);
    /* pass */
    long long ext = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, hdr);
    /* pass */
    long long vv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(vv, var));
    /* pass */
    long long ev = lower_expr(m, lf, end_expr);
    /* pass */
    if ((ev < 0LL)) {
        /* pass */
        _tr_str_release(cmp);
        return false;
    }
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, cmp, vv, ev));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    List_i64_append(lf->loop_cont, latch);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool rok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!rok)) {
        /* pass */
        _tr_str_release(cmp);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
    /* pass */
    LFunc_set_cur(lf, latch);
    /* pass */
    _emit_add_const(lf, var, stepv);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(cmp);
    return true;
}

__attribute__((hot)) bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    long long lv = lower_expr(m, lf, iter);
    /* pass */
    if ((lv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long ltag = LFunc_vreg_type(lf, lv);
    /* pass */
    if ((!_is_list_tag(ltag))) {
        /* pass */
        return false;
    }
    /* pass */
    long long elem_t = _list_elem_tag(ltag);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr hname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__forlist")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr iname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__foridx")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, hname);
    /* pass */
    LFunc_set_var_type(lf, hname, ltag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(hname, lv));
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    LFunc_add_var(lf, iname);
    /* pass */
    LFunc_set_var_type(lf, iname, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(iname, z));
    /* pass */
    long long hdr = LFunc_new_block(lf);
    /* pass */
    long long bdy = LFunc_new_block(lf);
    /* pass */
    long long latch = LFunc_new_block(lf);
    /* pass */
    long long ext = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, hdr);
    /* pass */
    long long hv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv, hname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
    /* pass */
    List_i64* la = (void*)List_i64_new();
    /* pass */
    List_i64_append(la, hv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit("_tr_rt_list_len"), la));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit("<"), iv, lenv));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long hv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv2, hname));
    /* pass */
    long long iv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
    /* pass */
    long long xval = _list_get(m, lf, hv2, iv2);
    /* pass */
    if ((elem_t == 5LL)) {
        /* pass */
        long long xfb = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(xfb, xval));
        /* pass */
        LFunc_set_vreg_type(lf, xfb, 5LL);
        /* pass */
        xval = xfb;
    } else {
        /* pass */
        LFunc_set_vreg_type(lf, xval, elem_t);
    }
    /* pass */
    if ((elem_t == 1LL)) {
        /* pass */
        _retain_str(m, lf, xval);
    }
    /* pass */
    LFunc_add_var(lf, var);
    /* pass */
    LFunc_set_var_type(lf, var, elem_t);
    /* pass */
    if ((elem_t == 10LL)) {
        /* pass */
        AstType* it_ty = hir_expr_type(iter);
        /* pass */
        if (((it_ty->args->len > 0LL) && (!_is_null_str((*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)))) {
            /* pass */
            if (LModule_is_class(m, (*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)) {
                /* pass */
                ({ TrStr _at_t2319 = (_own((*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)); LFunc_set_var_cls(lf, var, _at_t2319); _tr_str_release(_at_t2319); });
            }
        }
    }
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, xval));
    /* pass */
    List_i64_append(lf->loop_cont, latch);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool fok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!fok)) {
        /* pass */
        _tr_str_release(hname);
        _tr_str_release(iname);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
    /* pass */
    LFunc_set_cur(lf, latch);
    /* pass */
    _emit_incr(lf, iname);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(hname);
    _tr_str_release(iname);
    return true;
}

__attribute__((hot)) bool _lower_for_unpack(LModule* m, LFunc* lf, List_TrStr* vars, HirExpr* iter, HirBlock* body) {
    /* pass */
    if ((vars->len != 2LL)) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t2320 = (*iter);
    if (_t2320.tag == HirExpr_ECall) {
        __auto_type callee = _t2320.data.ECall.callee;
__auto_type args = _t2320.data.ECall.args;
        /* pass */
        if (((strcmp(_tr_strz(_ident_name(callee)), _tr_strz(_tr_str_lit("enumerate"))) == 0) && (args->len == 1LL))) {
            /* pass */
            return ({ TrStr _at_t2321 = (List_TrStr_get(vars, 0LL)); TrStr _at_t2322 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_enumerate(m, lf, _at_t2321, _at_t2322, ((HirExpr*)List_ptr_get(args, 0LL)), body)); _tr_str_release(_at_t2321); _tr_str_release(_at_t2322); _wr; });
        }
    } else if (1) {
        __auto_type _ = _t2320;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _lower_enumerate(LModule* m, LFunc* lf, TrStr ivar, TrStr evar, HirExpr* listexpr, HirBlock* body) {
    /* pass */
    long long lv = lower_expr(m, lf, listexpr);
    /* pass */
    if ((lv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long ltag = LFunc_vreg_type(lf, lv);
    /* pass */
    if ((!_is_list_tag(ltag))) {
        /* pass */
        return false;
    }
    /* pass */
    long long elem_t = _list_elem_tag(ltag);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr hname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__enumlist")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, hname);
    /* pass */
    LFunc_set_var_type(lf, hname, ltag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(hname, lv));
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    LFunc_add_var(lf, ivar);
    /* pass */
    LFunc_set_var_type(lf, ivar, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(ivar, z));
    /* pass */
    long long hdr = LFunc_new_block(lf);
    /* pass */
    long long bdy = LFunc_new_block(lf);
    /* pass */
    long long latch = LFunc_new_block(lf);
    /* pass */
    long long ext = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, hdr);
    /* pass */
    long long hv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv, hname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
    /* pass */
    List_i64* la = (void*)List_i64_new();
    /* pass */
    List_i64_append(la, hv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit("_tr_rt_list_len"), la));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, ivar));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit("<"), iv, lenv));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long hv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv2, hname));
    /* pass */
    long long iv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, ivar));
    /* pass */
    long long xval = _list_get(m, lf, hv2, iv2);
    /* pass */
    if ((elem_t == 5LL)) {
        /* pass */
        long long xfb2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(xfb2, xval));
        /* pass */
        LFunc_set_vreg_type(lf, xfb2, 5LL);
        /* pass */
        xval = xfb2;
    } else {
        /* pass */
        LFunc_set_vreg_type(lf, xval, elem_t);
    }
    /* pass */
    if ((elem_t == 1LL)) {
        /* pass */
        _retain_str(m, lf, xval);
    }
    /* pass */
    LFunc_add_var(lf, evar);
    /* pass */
    LFunc_set_var_type(lf, evar, elem_t);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(evar, xval));
    /* pass */
    List_i64_append(lf->loop_cont, latch);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool ok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!ok)) {
        /* pass */
        _tr_str_release(hname);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
    /* pass */
    LFunc_set_cur(lf, latch);
    /* pass */
    _emit_incr(lf, ivar);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(hname);
    return true;
}

__attribute__((hot)) void _emit_incr(LFunc* lf, TrStr name) {
    /* pass */
    long long cur = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    long long inc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit("+"), cur, one));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
    /* pass */
    __auto_type _t2323 = (*e);
    if (_t2323.tag == HirExpr_EIdent) {
        __auto_type n = _t2323.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t2323;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val) {
    /* pass */
    TrStr cls = _recv_class(m, lf, obj);
    /* pass */
    if (((strcmp(_tr_strz(cls), _tr_strz(_tr_str_lit(""))) == 0) || (!LModule_is_class(m, cls)))) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    long long off = LModule_field_offset(m, cls, prop);
    /* pass */
    if ((off < 0LL)) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    long long ftg = LModule_field_tag(m, cls, prop);
    /* pass */
    if ((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg))) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    long long vv = lower_expr(m, lf, val);
    /* pass */
    if ((vv < 0LL)) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    long long vt = LFunc_vreg_type(lf, vv);
    /* pass */
    if ((ftg == 5LL)) {
        /* pass */
        if ((vt == 0LL)) {
            /* pass */
            vv = _promote_f(lf, vv);
        } else if ((vt != 5LL)) {
            /* pass */
            _tr_str_release(cls);
            return false;
        }
        /* pass */
        long long fbits = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IFBits(fbits, vv));
        /* pass */
        vv = fbits;
    } else if ((ftg == 1LL)) {
        /* pass */
        if ((vt != 1LL)) {
            /* pass */
            _tr_str_release(cls);
            return false;
        }
        /* pass */
        _secure_str(m, lf, vv);
    } else if ((!_field_tag_ok(vt, ftg))) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    long long ov0 = lower_expr(m, lf, obj);
    /* pass */
    if ((ov0 < 0LL)) {
        /* pass */
        _tr_str_release(cls);
        return false;
    }
    /* pass */
    _emit_field_set(m, lf, ov0, off, vv);
    /* pass */
    _tr_str_release(cls);
    return true;
}

__attribute__((hot)) bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val) {
    /* pass */
    TrStr sicls = _recv_class(m, lf, obj);
    /* pass */
    if (((strcmp(_tr_strz(sicls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, sicls))) {
        /* pass */
        TrStr sim = LModule_resolve_method(m, sicls, _tr_str_lit("__setitem__"));
        /* pass */
        if ((strcmp(_tr_strz(sim), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            long long siself = lower_expr(m, lf, obj);
            /* pass */
            if ((siself < 0LL)) {
                /* pass */
                _tr_str_release(sicls);
                _tr_str_release(sim);
                return false;
            }
            /* pass */
            List_ptr* siargs = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(siargs, idx);
            /* pass */
            List_ptr_append(siargs, val);
            /* pass */
            _tr_str_release(sicls);
            return (_lower_obj_call(m, lf, sim, siself, siargs) >= 0LL);
        }
    }
    /* pass */
    long long ov = lower_expr(m, lf, obj);
    /* pass */
    if ((ov < 0LL)) {
        /* pass */
        _tr_str_release(sicls);
        return false;
    }
    /* pass */
    long long ovt = LFunc_vreg_type(lf, ov);
    /* pass */
    if (_is_dict_tag(ovt)) {
        /* pass */
        long long kv = lower_expr(m, lf, idx);
        /* pass */
        if ((kv < 0LL)) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, kv) != 1LL))) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, kv) != 0LL))) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        long long vv = lower_expr(m, lf, val);
        /* pass */
        if (((vv < 0LL) || (LFunc_vreg_type(lf, vv) != _dict_val_tag(ovt)))) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        if ((_dict_val_tag(ovt) == 1LL)) {
            /* pass */
            _secure_str(m, lf, vv);
        }
        /* pass */
        if ((_dict_val_tag(ovt) == 5LL)) {
            /* pass */
            long long ivb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFBits(ivb, vv));
            /* pass */
            vv = ivb;
        }
        /* pass */
        TrStr ssym = _dict_sym(ovt, _tr_str_lit("set"));
        /* pass */
        LModule_add_extern(m, ssym);
        /* pass */
        List_i64* sa = (void*)List_i64_new();
        /* pass */
        List_i64_append(sa, ov);
        /* pass */
        List_i64_append(sa, kv);
        /* pass */
        List_i64_append(sa, vv);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), ssym, sa));
        /* pass */
        _tr_str_release(sicls);
        _tr_str_release(ssym);
        return true;
    }
    /* pass */
    if (_is_list_tag(ovt)) {
        /* pass */
        long long iv = lower_expr(m, lf, idx);
        /* pass */
        if (((iv < 0LL) || (LFunc_vreg_type(lf, iv) != 0LL))) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        long long lvv = lower_expr(m, lf, val);
        /* pass */
        if (((lvv < 0LL) || (LFunc_vreg_type(lf, lvv) != _list_elem_tag(ovt)))) {
            /* pass */
            _tr_str_release(sicls);
            return false;
        }
        /* pass */
        if ((_list_elem_tag(ovt) == 1LL)) {
            /* pass */
            _secure_str(m, lf, lvv);
        }
        /* pass */
        if ((_list_elem_tag(ovt) == 5LL)) {
            /* pass */
            long long lvfb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFBits(lvfb, lvv));
            /* pass */
            lvv = lvfb;
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_set_i64"));
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, ov);
        /* pass */
        List_i64_append(la, iv);
        /* pass */
        List_i64_append(la, lvv);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_set_i64"), la));
        /* pass */
        _tr_str_release(sicls);
        return true;
    }
    /* pass */
    _tr_str_release(sicls);
    return false;
}

__attribute__((hot)) TrStr _write_sym(long long t) {
    /* pass */
    if ((t == 1LL)) {
        /* pass */
        return _tr_str_lit("_tr_rt_write_cstr");
    }
    /* pass */
    if ((t == 4LL)) {
        /* pass */
        return _tr_str_lit("_tr_rt_write_bool");
    }
    /* pass */
    return _tr_str_lit("_tr_rt_write_i64");
}

__attribute__((hot)) void _emit_call0(LModule* m, LFunc* lf, TrStr sym) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, (void*)List_i64_new()));
}

__attribute__((hot)) bool _lower_print(LModule* m, LFunc* lf, List_ptr* args) {
    /* pass */
    if ((args->len == 0LL)) {
        /* pass */
        _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_nl"));
        /* pass */
        return true;
    }
    /* pass */
    if ((args->len == 1LL)) {
        /* pass */
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
        /* pass */
        if ((av < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long avt = LFunc_vreg_type(lf, av);
        /* pass */
        if (_is_list_tag(avt)) {
            /* pass */
            TrStr plsym = _tr_str_lit("_tr_rt_print_list_i64");
            /* pass */
            if ((avt == 3LL)) {
                /* pass */
                TrStr _strtmp_t2324 = _tr_str_lit("_tr_rt_print_list_str");
                _tr_str_release(plsym);
                plsym = _strtmp_t2324;
            }
            /* pass */
            if ((avt == 14LL)) {
                /* pass */
                TrStr _strtmp_t2325 = _tr_str_lit("_tr_rt_print_list_f64");
                _tr_str_release(plsym);
                plsym = _strtmp_t2325;
            }
            /* pass */
            LModule_add_extern(m, plsym);
            /* pass */
            List_i64* pla = (void*)List_i64_new();
            /* pass */
            List_i64_append(pla, av);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), plsym, pla));
            /* pass */
            _tr_str_release(plsym);
            return true;
        }
        /* pass */
        if ((avt == 10LL)) {
            /* pass */
            long long ostr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), av);
            /* pass */
            if ((ostr >= 0LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_print_cstr"));
                /* pass */
                List_i64* opa = (void*)List_i64_new();
                /* pass */
                List_i64_append(opa, ostr);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_print_cstr"), opa));
                /* pass */
                return true;
            }
            /* pass */
            return false;
        }
        /* pass */
        if (((avt == 11LL) || _is_set_tag(avt))) {
            /* pass */
            return false;
        }
        /* pass */
        if ((avt == 5LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_print_f64"));
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit("_tr_rt_print_f64"), av));
            /* pass */
            return true;
        }
        /* pass */
        TrStr sym = _print_i64_sym();
        /* pass */
        if ((avt == 1LL)) {
            /* pass */
            TrStr _strtmp_t2326 = _tr_str_lit("_tr_rt_print_cstr");
            _tr_str_release(sym);
            sym = _strtmp_t2326;
        } else if ((avt == 4LL)) {
            /* pass */
            TrStr _strtmp_t2327 = _tr_str_lit("_tr_rt_print_bool");
            _tr_str_release(sym);
            sym = _strtmp_t2327;
        }
        /* pass */
        LModule_add_extern(m, sym);
        /* pass */
        List_i64* cargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(cargs, av);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, cargs));
        /* pass */
        _tr_str_release(sym);
        return true;
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < args->len)) {
        /* pass */
        if ((pi > 0LL)) {
            /* pass */
            _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_sp"));
        }
        /* pass */
        long long pv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, pi)));
        /* pass */
        if ((pv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long pvt = LFunc_vreg_type(lf, pv);
        /* pass */
        if (_is_list_tag(pvt)) {
            /* pass */
            TrStr wlsym = _tr_str_lit("_tr_rt_write_list_i64");
            /* pass */
            if ((pvt == 3LL)) {
                /* pass */
                TrStr _strtmp_t2328 = _tr_str_lit("_tr_rt_write_list_str");
                _tr_str_release(wlsym);
                wlsym = _strtmp_t2328;
            }
            /* pass */
            if ((pvt == 14LL)) {
                /* pass */
                TrStr _strtmp_t2329 = _tr_str_lit("_tr_rt_write_list_f64");
                _tr_str_release(wlsym);
                wlsym = _strtmp_t2329;
            }
            /* pass */
            LModule_add_extern(m, wlsym);
            /* pass */
            List_i64* wla = (void*)List_i64_new();
            /* pass */
            List_i64_append(wla, pv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), wlsym, wla));
            /* pass */
            pi = (pi + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((pvt == 10LL)) {
            /* pass */
            long long postr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, pi)), pv);
            /* pass */
            if ((postr < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_write_cstr"));
            /* pass */
            List_i64* pwa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pwa, postr);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_write_cstr"), pwa));
            /* pass */
            pi = (pi + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if (((pvt == 11LL) || _is_set_tag(pvt))) {
            /* pass */
            return false;
        }
        /* pass */
        if ((pvt == 5LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_write_f64"));
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit("_tr_rt_write_f64"), pv));
        } else {
            /* pass */
            TrStr wsym = _write_sym(pvt);
            /* pass */
            LModule_add_extern(m, wsym);
            /* pass */
            List_i64* wargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(wargs, pv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), wsym, wargs));
            _tr_str_release(wsym);
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_nl"));
    /* pass */
    return true;
}

__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2330 = (*e);
    if (_t2330.tag == HirExpr_ECall) {
        __auto_type callee = _t2330.data.ECall.callee;
__auto_type args = _t2330.data.ECall.args;
        /* pass */
        TrStr fname = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit("print"))) == 0)) {
            /* pass */
            _tr_str_release(fname);
            return _lower_print(m, lf, args);
        }
        /* pass */
        long long r = lower_expr(m, lf, e);
        /* pass */
        _tr_str_release(fname);
        return (r >= 0LL);
    } else if (_t2330.tag == HirExpr_EMethodCall) {
        /* pass */
        long long rm = lower_expr(m, lf, e);
        /* pass */
        return (rm >= 0LL);
    } else if (1) {
        __auto_type _ = _t2330;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _int_op(TrStr op) {
    /* pass */
    if (((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("//"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("|"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("^"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<<"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">>"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _lir_digit(long long d) {
    /* pass */
    if ((d == 0LL)) {
        /* pass */
        return _tr_str_lit("0");
    }
    /* pass */
    if ((d == 1LL)) {
        /* pass */
        return _tr_str_lit("1");
    }
    /* pass */
    if ((d == 2LL)) {
        /* pass */
        return _tr_str_lit("2");
    }
    /* pass */
    if ((d == 3LL)) {
        /* pass */
        return _tr_str_lit("3");
    }
    /* pass */
    if ((d == 4LL)) {
        /* pass */
        return _tr_str_lit("4");
    }
    /* pass */
    if ((d == 5LL)) {
        /* pass */
        return _tr_str_lit("5");
    }
    /* pass */
    if ((d == 6LL)) {
        /* pass */
        return _tr_str_lit("6");
    }
    /* pass */
    if ((d == 7LL)) {
        /* pass */
        return _tr_str_lit("7");
    }
    /* pass */
    if ((d == 8LL)) {
        /* pass */
        return _tr_str_lit("8");
    }
    /* pass */
    return _tr_str_lit("9");
}

__attribute__((hot)) TrStr _lir_itoa(long long n) {
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return _tr_str_lit("0");
    }
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long x = n;
    /* pass */
    while ((x > 0LL)) {
        /* pass */
        TrStr _strtmp_t2331 = ({ TrStr _cl = (_lir_digit((x % 10LL))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(s)); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t2331;
        /* pass */
        x = (x / 10LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) void _fresh_mark(LFunc* lf, long long v) {
    /* pass */
    List_i64_append(lf->fresh_strs, v);
}

__attribute__((hot)) bool _fresh_take(LFunc* lf, long long v) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lf->fresh_strs->len)) {
        /* pass */
        if ((List_i64_get(lf->fresh_strs, i) == v)) {
            /* pass */
            List_i64_remove(lf->fresh_strs, i);
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void _release_str(LModule* m, LFunc* lf, long long v) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_str_release"));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_str_release"), a));
}

__attribute__((hot)) void _retain_str(LModule* m, LFunc* lf, long long v) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_str_retain"));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_str_retain"), a));
}

__attribute__((hot)) void _flush_fresh_strs(LModule* m, LFunc* lf) {
    /* pass */
    while ((lf->fresh_strs->len > 0LL)) {
        /* pass */
        long long v = List_i64_pop(lf->fresh_strs);
        /* pass */
        _release_str(m, lf, v);
    }
}

__attribute__((hot)) void _secure_str(LModule* m, LFunc* lf, long long v) {
    /* pass */
    if ((!_fresh_take(lf, v))) {
        /* pass */
        _retain_str(m, lf, v);
    }
}

__attribute__((hot)) bool _is_param(LFunc* lf, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lf->params->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(lf->params, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long _norm_bool(LFunc* lf, long long v) {
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    long long r = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(r, _tr_str_lit("!="), v, z));
    /* pass */
    return r;
}

__attribute__((hot)) long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, _tr_v_recv);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, restype);
    /* pass */
    if ((restype == 1LL)) {
        /* pass */
        _fresh_mark(lf, d);
    }
    /* pass */
    return d;
}

__attribute__((hot)) long long _heap_lit(LModule* m, LFunc* lf, TrStr s) {
    /* pass */
    long long idx = LModule_add_string(m, s);
    /* pass */
    long long ds = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
    /* pass */
    LFunc_set_vreg_type(lf, ds, 1LL);
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_str_new"));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, ds);
    /* pass */
    long long h = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(h, _tr_str_lit("_tr_rt_str_new"), a));
    /* pass */
    LFunc_set_vreg_type(lf, h, 1LL);
    /* pass */
    _fresh_mark(lf, h);
    /* pass */
    return h;
}

__attribute__((hot)) long long _obj_to_str(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg) {
    /* pass */
    TrStr ocls = _recv_class(m, lf, objexpr);
    /* pass */
    if (((strcmp(_tr_strz(ocls), _tr_strz(_tr_str_lit(""))) == 0) || (!LModule_is_class(m, ocls)))) {
        /* pass */
        _tr_str_release(ocls);
        return (-1LL);
    }
    /* pass */
    TrStr osm = LModule_resolve_method(m, ocls, _tr_str_lit("__str__"));
    /* pass */
    if ((strcmp(_tr_strz(osm), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        _tr_str_release(ocls);
        _tr_str_release(osm);
        return (-1LL);
    }
    /* pass */
    long long oself = objreg;
    /* pass */
    if ((oself < 0LL)) {
        /* pass */
        oself = lower_expr(m, lf, objexpr);
        /* pass */
        if ((oself < 0LL)) {
            /* pass */
            _tr_str_release(ocls);
            _tr_str_release(osm);
            return (-1LL);
        }
    }
    /* pass */
    long long sd = _lower_obj_call(m, lf, osm, oself, (void*)List_ptr_new());
    /* pass */
    _tr_str_release(ocls);
    _tr_str_release(osm);
    return sd;
}

__attribute__((hot)) long long _reg_to_str(LModule* m, LFunc* lf, long long reg) {
    /* pass */
    long long t = LFunc_vreg_type(lf, reg);
    /* pass */
    if ((t == 1LL)) {
        /* pass */
        return reg;
    }
    /* pass */
    if ((t == 5LL)) {
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_f64_to_str"));
        /* pass */
        long long fd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IFCall1(fd, _tr_str_lit("_tr_rt_f64_to_str"), reg));
        /* pass */
        LFunc_set_vreg_type(lf, fd, 1LL);
        /* pass */
        _fresh_mark(lf, fd);
        /* pass */
        return fd;
    }
    /* pass */
    TrStr sym = _tr_str_lit("_tr_rt_i64_to_str");
    /* pass */
    if ((t == 4LL)) {
        /* pass */
        TrStr _strtmp_t2332 = _tr_str_lit("_tr_rt_bool_to_str");
        _tr_str_release(sym);
        sym = _strtmp_t2332;
    } else if ((t != 0LL)) {
        /* pass */
        _tr_str_release(sym);
        return (-1LL);
    }
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, reg);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, 1LL);
    /* pass */
    _fresh_mark(lf, d);
    /* pass */
    _tr_str_release(sym);
    return d;
}

__attribute__((hot)) long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, _tr_v_recv);
    /* pass */
    List_i64_append(a, arg);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, restype);
    /* pass */
    if ((restype == 1LL)) {
        /* pass */
        _fresh_mark(lf, d);
    }
    /* pass */
    return d;
}

__attribute__((hot)) long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("upper"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_upper"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_upper"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lower"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_lower"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_lower"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_strip"), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("replace"))) == 0) && (margs->len == 2LL))) {
        /* pass */
        long long a0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((a0 < 0LL) || (LFunc_vreg_type(lf, a0) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long a1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((a1 < 0LL) || (LFunc_vreg_type(lf, a1) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_replace"));
        /* pass */
        List_i64* ra = (void*)List_i64_new();
        /* pass */
        List_i64_append(ra, _tr_v_recv);
        /* pass */
        List_i64_append(ra, a0);
        /* pass */
        List_i64_append(ra, a1);
        /* pass */
        long long rd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit("_tr_rt_str_replace"), ra));
        /* pass */
        LFunc_set_vreg_type(lf, rd, 1LL);
        /* pass */
        _fresh_mark(lf, rd);
        /* pass */
        return rd;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("find"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long fa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((fa < 0LL) || (LFunc_vreg_type(lf, fa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_find"), _tr_v_recv, fa, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("starts_with"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long sa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((sa < 0LL) || (LFunc_vreg_type(lf, sa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_starts_with"), _tr_v_recv, sa, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ends_with"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ea < 0LL) || (LFunc_vreg_type(lf, ea) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_ends_with"), _tr_v_recv, ea, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("count"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ka = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ka < 0LL) || (LFunc_vreg_type(lf, ka) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_count"), _tr_v_recv, ka, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long na = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((na < 0LL) || (LFunc_vreg_type(lf, na) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_contains"), _tr_v_recv, na, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("char_at"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ia < 0LL) || (LFunc_vreg_type(lf, ia) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_char_at"), _tr_v_recv, ia, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("repeat"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long pa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pa < 0LL) || (LFunc_vreg_type(lf, pa) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_repeat"), _tr_v_recv, pa, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("slice"))) == 0) && (margs->len == 2LL))) {
        /* pass */
        long long s0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((s0 < 0LL) || (LFunc_vreg_type(lf, s0) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long s1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((s1 < 0LL) || (LFunc_vreg_type(lf, s1) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_slice"));
        /* pass */
        List_i64* sla = (void*)List_i64_new();
        /* pass */
        List_i64_append(sla, _tr_v_recv);
        /* pass */
        List_i64_append(sla, s0);
        /* pass */
        List_i64_append(sla, s1);
        /* pass */
        long long sld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit("_tr_rt_str_slice"), sla));
        /* pass */
        LFunc_set_vreg_type(lf, sld, 1LL);
        /* pass */
        _fresh_mark(lf, sld);
        /* pass */
        return sld;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("capitalize"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_capitalize"), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("title"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_title"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim_left"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lstrip"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_trim_left"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim_right"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("rstrip"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_trim_right"), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("zfill"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long zfa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((zfa < 0LL) || (LFunc_vreg_type(lf, zfa) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_zfill"), _tr_v_recv, zfa, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pad_left"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long pla = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pla < 0LL) || (LFunc_vreg_type(lf, pla) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_pad_left"), _tr_v_recv, pla, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pad_right"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long pra = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pra < 0LL) || (LFunc_vreg_type(lf, pra) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_pad_right"), _tr_v_recv, pra, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains_char"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long cca = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((cca < 0LL) || (LFunc_vreg_type(lf, cca) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_contains_char"), _tr_v_recv, cca, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("center"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long cea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((cea < 0LL) || (LFunc_vreg_type(lf, cea) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_center"), _tr_v_recv, cea, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("chars"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_chars"));
        /* pass */
        List_i64* cha = (void*)List_i64_new();
        /* pass */
        List_i64_append(cha, _tr_v_recv);
        /* pass */
        long long chd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(chd, _tr_str_lit("_tr_rt_str_chars"), cha));
        /* pass */
        LFunc_set_vreg_type(lf, chd, 3LL);
        /* pass */
        return chd;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("split"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long spa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((spa < 0LL) || (LFunc_vreg_type(lf, spa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_split"));
        /* pass */
        List_i64* spargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(spargs, _tr_v_recv);
        /* pass */
        List_i64_append(spargs, spa);
        /* pass */
        long long spd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(spd, _tr_str_lit("_tr_rt_str_split"), spargs));
        /* pass */
        LFunc_set_vreg_type(lf, spd, 3LL);
        /* pass */
        return spd;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reverse"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_reverse"), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_is_empty"), _tr_v_recv, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_bool"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_parse_bool"), _tr_v_recv, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_int"))) == 0) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_to_i64"), _tr_v_recv, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("index_of"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ida = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ida < 0LL) || (LFunc_vreg_type(lf, ida) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_find"), _tr_v_recv, ida, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("last_index_of"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long lia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((lia < 0LL) || (LFunc_vreg_type(lf, lia) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_last_index_of"), _tr_v_recv, lia, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip_prefix"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ppa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ppa < 0LL) || (LFunc_vreg_type(lf, ppa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_strip_prefix"), _tr_v_recv, ppa, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip_suffix"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ssa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ssa < 0LL) || (LFunc_vreg_type(lf, ssa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_strip_suffix"), _tr_v_recv, ssa, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("replace_first"))) == 0) && (margs->len == 2LL))) {
        /* pass */
        long long rf0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((rf0 < 0LL) || (LFunc_vreg_type(lf, rf0) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long rf1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((rf1 < 0LL) || (LFunc_vreg_type(lf, rf1) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_replace_first"));
        /* pass */
        List_i64* rfa = (void*)List_i64_new();
        /* pass */
        List_i64_append(rfa, _tr_v_recv);
        /* pass */
        List_i64_append(rfa, rf0);
        /* pass */
        List_i64_append(rfa, rf1);
        /* pass */
        long long rfd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(rfd, _tr_str_lit("_tr_rt_str_replace_first"), rfa));
        /* pass */
        LFunc_set_vreg_type(lf, rfd, 1LL);
        /* pass */
        _fresh_mark(lf, rfd);
        /* pass */
        return rfd;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) TrStr _float_unary_sym(TrStr method) {
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sqrt"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_sqrt");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("floor"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_floor");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ceil"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_ceil");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("round"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_round");
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("abs"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("fabs"))) == 0))) {
        /* pass */
        return _tr_str_lit("_tr_rt_fabs");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_log");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log2"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_log2");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log10"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_log10");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("exp"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_exp");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sin"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_sin");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cos"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_cos");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("tan"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_tan");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("asin"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_asin");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("acos"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_acos");
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("atan"))) == 0)) {
        /* pass */
        return _tr_str_lit("_tr_rt_atan");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) long long _lower_int_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
    /* pass */
    if ((margs->len == 0LL)) {
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_hex"))) == 0)) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit("_tr_rt_i64_to_hex"), _tr_v_recv, 1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_hex_upper"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_HEX"))) == 0))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit("_tr_rt_i64_to_hex_upper"), _tr_v_recv, 1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_oct"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_octal"))) == 0))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit("_tr_rt_i64_to_oct"), _tr_v_recv, 1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_bin"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_binary"))) == 0))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit("_tr_rt_i64_to_bin"), _tr_v_recv, 1LL);
        }
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sign"))) == 0)) {
            /* pass */
            long long z = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
            /* pass */
            long long gt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(gt, _tr_str_lit(">"), _tr_v_recv, z));
            /* pass */
            long long lt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(lt, _tr_str_lit("<"), _tr_v_recv, z));
            /* pass */
            long long sd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(sd, _tr_str_lit("-"), gt, lt));
            /* pass */
            return sd;
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((margs->len == 1LL) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pow"))) == 0))) {
        /* pass */
        long long pe = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pe < 0LL) || (LFunc_vreg_type(lf, pe) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_int_pow"));
        /* pass */
        List_i64* ppa = (void*)List_i64_new();
        /* pass */
        List_i64_append(ppa, _tr_v_recv);
        /* pass */
        List_i64_append(ppa, pe);
        /* pass */
        long long ppd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ppd, _tr_str_lit("_tr_rt_int_pow"), ppa));
        /* pass */
        return ppd;
    }
    /* pass */
    if (((margs->len == 1LL) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("gcd"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lcm"))) == 0)))) {
        /* pass */
        long long y = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((y < 0LL) || (LFunc_vreg_type(lf, y) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr gsym = _tr_str_lit("_tr_rt_gcd_i64");
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lcm"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2333 = _tr_str_lit("_tr_rt_lcm_i64");
            _tr_str_release(gsym);
            gsym = _strtmp_t2333;
        }
        /* pass */
        LModule_add_extern(m, gsym);
        /* pass */
        List_i64* glargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(glargs, _tr_v_recv);
        /* pass */
        List_i64_append(glargs, y);
        /* pass */
        long long gd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(gd, gsym, glargs));
        /* pass */
        _tr_str_release(gsym);
        return gd;
    }
    /* pass */
    if (((margs->len == 2LL) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clamp"))) == 0))) {
        /* pass */
        long long lo = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((lo < 0LL) || (LFunc_vreg_type(lf, lo) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long hi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((hi < 0LL) || (LFunc_vreg_type(lf, hi) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_clamp_i64"));
        /* pass */
        List_i64* ca = (void*)List_i64_new();
        /* pass */
        List_i64_append(ca, _tr_v_recv);
        /* pass */
        List_i64_append(ca, lo);
        /* pass */
        List_i64_append(ca, hi);
        /* pass */
        long long cd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cd, _tr_str_lit("_tr_rt_clamp_i64"), ca));
        /* pass */
        return cd;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _lower_dict_method(LModule* m, LFunc* lf, long long _tr_v_recv, long long dtag, TrStr method, List_ptr* margs) {
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_or"))) == 0) && (margs->len == 2LL))) {
        /* pass */
        if ((_dict_val_tag(dtag) != 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long gk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if ((gk < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, gk) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, gk) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long gdef = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((gdef < 0LL) || (LFunc_vreg_type(lf, gdef) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr gosym = _dict_sym(dtag, _tr_str_lit("get_or"));
        /* pass */
        LModule_add_extern(m, gosym);
        /* pass */
        List_i64* goa = (void*)List_i64_new();
        /* pass */
        List_i64_append(goa, _tr_v_recv);
        /* pass */
        List_i64_append(goa, gk);
        /* pass */
        List_i64_append(goa, gdef);
        /* pass */
        long long godd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(godd, gosym, goa));
        /* pass */
        _tr_str_release(gosym);
        return godd;
    }
    /* pass */
    if ((margs->len != 1LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long kv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
    /* pass */
    if ((kv < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, kv) != 1LL))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, kv) != 0LL))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_index"))) == 0))) {
        /* pass */
        TrStr gsym = _dict_sym(dtag, _tr_str_lit("get"));
        /* pass */
        LModule_add_extern(m, gsym);
        /* pass */
        List_i64* getargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(getargs, _tr_v_recv);
        /* pass */
        List_i64_append(getargs, kv);
        /* pass */
        long long gd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(gd, gsym, getargs));
        /* pass */
        if ((_dict_val_tag(dtag) == 5LL)) {
            /* pass */
            long long gdf = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBitsF(gdf, gd));
            /* pass */
            LFunc_set_vreg_type(lf, gdf, 5LL);
            /* pass */
            _tr_str_release(gsym);
            return gdf;
        }
        /* pass */
        LFunc_set_vreg_type(lf, gd, _dict_val_tag(dtag));
        /* pass */
        _tr_str_release(gsym);
        return gd;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("has"))) == 0))) {
        /* pass */
        TrStr hsym = _dict_sym(dtag, _tr_str_lit("has"));
        /* pass */
        LModule_add_extern(m, hsym);
        /* pass */
        List_i64* ha = (void*)List_i64_new();
        /* pass */
        List_i64_append(ha, _tr_v_recv);
        /* pass */
        List_i64_append(ha, kv);
        /* pass */
        long long hd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(hd, hsym, ha));
        /* pass */
        LFunc_set_vreg_type(lf, hd, 4LL);
        /* pass */
        _tr_str_release(hsym);
        return hd;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _lower_float_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
    /* pass */
    if ((margs->len == 0LL)) {
        /* pass */
        TrStr usym = _float_unary_sym(method);
        /* pass */
        if ((strcmp(_tr_strz(usym), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            LModule_add_extern(m, usym);
            /* pass */
            long long d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCallF(d, usym, _tr_v_recv));
            /* pass */
            LFunc_set_vreg_type(lf, d, 5LL);
            /* pass */
            _tr_str_release(usym);
            return d;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_nan"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_inf"))) == 0))) {
            /* pass */
            TrStr nsym = _tr_str_lit("_tr_rt_f64_is_nan");
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_inf"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2334 = _tr_str_lit("_tr_rt_f64_is_inf");
                _tr_str_release(nsym);
                nsym = _strtmp_t2334;
            }
            /* pass */
            LModule_add_extern(m, nsym);
            /* pass */
            long long nd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1(nd, nsym, _tr_v_recv));
            /* pass */
            LFunc_set_vreg_type(lf, nd, 4LL);
            /* pass */
            _tr_str_release(usym);
            _tr_str_release(nsym);
            return nd;
        }
        /* pass */
        _tr_str_release(usym);
        return (-1LL);
    }
    /* pass */
    if (((margs->len == 1LL) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pow"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("atan2"))) == 0)))) {
        /* pass */
        long long arg = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if ((arg < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long argt = LFunc_vreg_type(lf, arg);
        /* pass */
        if ((argt == 0LL)) {
            /* pass */
            arg = _promote_f(lf, arg);
        } else if ((argt != 5LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr psym = _tr_str_lit("_tr_rt_pow");
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("atan2"))) == 0)) {
            /* pass */
            TrStr _strtmp_t2335 = _tr_str_lit("_tr_rt_atan2");
            _tr_str_release(psym);
            psym = _strtmp_t2335;
        }
        /* pass */
        LModule_add_extern(m, psym);
        /* pass */
        long long pd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IFCall2F(pd, psym, _tr_v_recv, arg));
        /* pass */
        LFunc_set_vreg_type(lf, pd, 5LL);
        /* pass */
        _tr_str_release(psym);
        return pd;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_const_int(HirExpr* e) {
    /* pass */
    __auto_type _t2336 = (*e);
    if (_t2336.tag == HirExpr_ELitInt) {
        return true;
    } else if (_t2336.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2336.data.EUnaryOp.op;
__auto_type sub = _t2336.data.EUnaryOp.expr;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            __auto_type _t2337 = (*sub);
            if (_t2337.tag == HirExpr_ELitInt) {
                return true;
            } else if (1) {
                __auto_type _ = _t2337;
                return false;
            }
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t2336;
        return false;
    }
}

__attribute__((hot)) long long _const_int_val(HirExpr* e) {
    /* pass */
    __auto_type _t2338 = (*e);
    if (_t2338.tag == HirExpr_ELitInt) {
        __auto_type v = _t2338.data.ELitInt.val;
        return v;
    } else if (_t2338.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2338.data.EUnaryOp.op;
__auto_type sub = _t2338.data.EUnaryOp.expr;
        /* pass */
        __auto_type _t2339 = (*sub);
        if (_t2339.tag == HirExpr_ELitInt) {
            __auto_type v2 = _t2339.data.ELitInt.val;
            return (0LL - v2);
        } else if (1) {
            __auto_type _ = _t2339;
            return 0LL;
        }
    } else if (1) {
        __auto_type _ = _t2338;
        return 0LL;
    }
}

__attribute__((hot)) void _emit_add_const(LFunc* lf, TrStr name, long long delta) {
    /* pass */
    long long cur = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(d, delta));
    /* pass */
    long long inc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit("+"), cur, d));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) long long _list_call1(LModule* m, LFunc* lf, TrStr sym, long long handle, long long restype) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, handle);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, restype);
    /* pass */
    return d;
}

__attribute__((hot)) long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_list_get_i64"));
    /* pass */
    List_i64* gargs = (void*)List_i64_new();
    /* pass */
    List_i64_append(gargs, handle);
    /* pass */
    List_i64_append(gargs, idx);
    /* pass */
    long long gd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(gd, _tr_str_lit("_tr_rt_list_get_i64"), gargs));
    /* pass */
    return gd;
}

__attribute__((hot)) long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx) {
    /* pass */
    long long raw = _list_get(m, lf, handle, idx);
    /* pass */
    if ((ltag == 14LL)) {
        /* pass */
        long long fv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(fv, raw));
        /* pass */
        LFunc_set_vreg_type(lf, fv, 5LL);
        /* pass */
        return fv;
    }
    /* pass */
    LFunc_set_vreg_type(lf, raw, _list_elem_tag(ltag));
    /* pass */
    return raw;
}

__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2340 = (*e);
    if (_t2340.tag == HirExpr_ELitInt) {
        __auto_type v = _t2340.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t2340.tag == HirExpr_ELitStr) {
        __auto_type sv = _t2340.data.ELitStr.val;
        /* pass */
        long long idx = LModule_add_string(m, sv);
        /* pass */
        long long ds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
        /* pass */
        LFunc_set_vreg_type(lf, ds, 1LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_new"));
        /* pass */
        List_i64* lna = (void*)List_i64_new();
        /* pass */
        List_i64_append(lna, ds);
        /* pass */
        long long lheap = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(lheap, _tr_str_lit("_tr_rt_str_new"), lna));
        /* pass */
        LFunc_set_vreg_type(lf, lheap, 1LL);
        /* pass */
        _fresh_mark(lf, lheap);
        /* pass */
        return lheap;
    } else if (_t2340.tag == HirExpr_ELitBool) {
        __auto_type bval = _t2340.data.ELitBool.val;
        /* pass */
        long long db = LFunc_new_vreg(lf);
        /* pass */
        long long bconst = 0LL;
        /* pass */
        if (bval) {
            /* pass */
            bconst = 1LL;
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(db, bconst));
        /* pass */
        LFunc_set_vreg_type(lf, db, 4LL);
        /* pass */
        return db;
    } else if (_t2340.tag == HirExpr_ELitFloat) {
        __auto_type fval = _t2340.data.ELitFloat.val;
        /* pass */
        long long fd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(fd, _f64_bits(fval)));
        /* pass */
        LFunc_set_vreg_type(lf, fd, 5LL);
        /* pass */
        return fd;
    } else if (_t2340.tag == HirExpr_EIdent) {
        __auto_type name = _t2340.data.EIdent.name;
        /* pass */
        if (((LFunc_var_index(lf, name) < 0LL) && (LFunc_capture_index(lf, name) >= 0LL))) {
            /* pass */
            long long cix = LFunc_capture_index(lf, name);
            /* pass */
            long long cenv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(cenv, _tr_str_lit("__env")));
            /* pass */
            long long caddr = _emit_field_get(m, lf, cenv, ((1LL + cix) * 8LL), 0LL);
            /* pass */
            long long ctag = List_i64_get(lf->cap_tags, cix);
            /* pass */
            if ((ctag == 5LL)) {
                /* pass */
                long long craw = _emit_field_get(m, lf, caddr, 0LL, 0LL);
                /* pass */
                long long cfv = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(cfv, craw));
                /* pass */
                LFunc_set_vreg_type(lf, cfv, 5LL);
                /* pass */
                return cfv;
            }
            /* pass */
            return _emit_field_get(m, lf, caddr, 0LL, ctag);
        }
        /* pass */
        if (((LFunc_var_index(lf, name) < 0LL) && LModule_is_global(m, name))) {
            /* pass */
            long long gd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadGlobal(gd, LModule_global_index(m, name)));
            /* pass */
            LFunc_set_vreg_type(lf, gd, LModule_global_type(m, name));
            /* pass */
            return gd;
        }
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        long long d2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(d2, name));
        /* pass */
        LFunc_set_vreg_type(lf, d2, LFunc_var_type(lf, name));
        /* pass */
        return d2;
    } else if (_t2340.tag == HirExpr_EIfElse) {
        __auto_type cond = _t2340.data.EIfElse.cond;
__auto_type then_e = _t2340.data.EIfElse.then_e;
__auto_type else_e = _t2340.data.EIfElse.else_e;
        /* pass */
        long long tcv = lower_expr(m, lf, cond);
        /* pass */
        if ((tcv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long tuid = LFunc_fresh_id(lf);
        /* pass */
        TrStr rname = ({ TrStr _cr = (_lir_itoa(tuid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__tern")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, rname);
        /* pass */
        long long then_b = LFunc_new_block(lf);
        /* pass */
        long long else_b = LFunc_new_block(lf);
        /* pass */
        long long end_b = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(tcv, then_b, else_b));
        /* pass */
        LFunc_set_cur(lf, then_b);
        /* pass */
        long long tv = lower_expr(m, lf, then_e);
        /* pass */
        if ((tv < 0LL)) {
            /* pass */
            _tr_str_release(rname);
            return (-1LL);
        }
        /* pass */
        long long rtype = LFunc_vreg_type(lf, tv);
        /* pass */
        if ((rtype == 1LL)) {
            /* pass */
            _secure_str(m, lf, tv);
        }
        /* pass */
        LFunc_set_var_type(lf, rname, rtype);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, tv));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
        /* pass */
        LFunc_set_cur(lf, else_b);
        /* pass */
        long long ev2 = lower_expr(m, lf, else_e);
        /* pass */
        if ((ev2 < 0LL)) {
            /* pass */
            _tr_str_release(rname);
            return (-1LL);
        }
        /* pass */
        if ((rtype == 1LL)) {
            /* pass */
            _secure_str(m, lf, ev2);
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, ev2));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
        /* pass */
        LFunc_set_cur(lf, end_b);
        /* pass */
        long long trd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(trd, rname));
        /* pass */
        LFunc_set_vreg_type(lf, trd, rtype);
        /* pass */
        if ((rtype == 1LL)) {
            /* pass */
            _fresh_mark(lf, trd);
        }
        /* pass */
        _tr_str_release(rname);
        return trd;
    } else if (_t2340.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2340.data.EUnaryOp.op;
__auto_type sub = _t2340.data.EUnaryOp.expr;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            TrStr ncls = _recv_class(m, lf, sub);
            /* pass */
            if (((strcmp(_tr_strz(ncls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, ncls))) {
                /* pass */
                TrStr nm2 = LModule_resolve_method(m, ncls, _tr_str_lit("__neg__"));
                /* pass */
                if ((strcmp(_tr_strz(nm2), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    long long nself = lower_expr(m, lf, sub);
                    /* pass */
                    if ((nself < 0LL)) {
                        /* pass */
                        _tr_str_release(ncls);
                        _tr_str_release(nm2);
                        return (-1LL);
                    }
                    /* pass */
                    _tr_str_release(ncls);
                    return _lower_obj_call(m, lf, nm2, nself, (void*)List_ptr_new());
                }
            }
        }
        /* pass */
        long long sv = lower_expr(m, lf, sub);
        /* pass */
        if ((sv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long svt = LFunc_vreg_type(lf, sv);
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            if ((svt == 5LL)) {
                /* pass */
                long long zf = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(zf, _f64_bits(0.0)));
                /* pass */
                LFunc_set_vreg_type(lf, zf, 5LL);
                /* pass */
                long long dnf = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBinOp(dnf, _tr_str_lit("-"), zf, sv));
                /* pass */
                LFunc_set_vreg_type(lf, dnf, 5LL);
                /* pass */
                return dnf;
            }
            /* pass */
            if ((svt != 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long zn = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zn, 0LL));
            /* pass */
            long long dneg = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(dneg, _tr_str_lit("-"), zn, sv));
            /* pass */
            return dneg;
        }
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("not"))) == 0)) {
            /* pass */
            if (((svt != 0LL) && (svt != 4LL))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long zt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zt, 0LL));
            /* pass */
            long long dnot = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(dnot, _tr_str_lit("=="), sv, zt));
            /* pass */
            LFunc_set_vreg_type(lf, dnot, 4LL);
            /* pass */
            return dnot;
        }
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("~"))) == 0)) {
            /* pass */
            if ((svt != 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long ones = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(ones, (0LL - 1LL)));
            /* pass */
            long long dcpl = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(dcpl, _tr_str_lit("^"), sv, ones));
            /* pass */
            return dcpl;
        }
        /* pass */
        return (-1LL);
    } else if (_t2340.tag == HirExpr_EBinOp) {
        __auto_type op = _t2340.data.EBinOp.op;
__auto_type l = _t2340.data.EBinOp.left;
__auto_type r = _t2340.data.EBinOp.right;
        /* pass */
        TrStr ddn = _dunder_for_op(op);
        /* pass */
        if ((strcmp(_tr_strz(ddn), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr lcls_d = _recv_class(m, lf, l);
            /* pass */
            if (((strcmp(_tr_strz(lcls_d), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, lcls_d))) {
                /* pass */
                TrStr ddm = LModule_resolve_method(m, lcls_d, ddn);
                /* pass */
                if ((strcmp(_tr_strz(ddm), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    long long da = lower_expr(m, lf, l);
                    /* pass */
                    if ((da < 0LL)) {
                        /* pass */
                        _tr_str_release(ddn);
                        _tr_str_release(lcls_d);
                        _tr_str_release(ddm);
                        return (-1LL);
                    }
                    /* pass */
                    List_ptr* dargs = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(dargs, r);
                    /* pass */
                    _tr_str_release(ddn);
                    _tr_str_release(lcls_d);
                    return _lower_obj_call(m, lf, ddm, da, dargs);
                }
            }
        }
        /* pass */
        long long a = lower_expr(m, lf, l);
        /* pass */
        if ((a < 0LL)) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        long long b = lower_expr(m, lf, r);
        /* pass */
        if ((b < 0LL)) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        long long at = LFunc_vreg_type(lf, a);
        /* pass */
        long long bt = LFunc_vreg_type(lf, b);
        /* pass */
        if (((at == 5LL) || (bt == 5LL))) {
            /* pass */
            if (((at != 5LL) && (at != 0LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if (((bt != 5LL) && (bt != 0LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            long long fa = a;
            /* pass */
            long long fb = b;
            /* pass */
            if ((at == 0LL)) {
                /* pass */
                fa = _promote_f(lf, a);
            }
            /* pass */
            if ((bt == 0LL)) {
                /* pass */
                fb = _promote_f(lf, b);
            }
            /* pass */
            if (((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0))) {
                /* pass */
                long long fdd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBinOp(fdd, op, fa, fb));
                /* pass */
                LFunc_set_vreg_type(lf, fdd, 5LL);
                /* pass */
                _tr_str_release(ddn);
                return fdd;
            }
            /* pass */
            if (_is_cmp_op(op)) {
                /* pass */
                long long fcd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBinOp(fcd, op, fa, fb));
                /* pass */
                LFunc_set_vreg_type(lf, fcd, 4LL);
                /* pass */
                _tr_str_release(ddn);
                return fcd;
            }
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0) && (bt == 10LL))) {
            /* pass */
            TrStr ccls = _recv_class(m, lf, r);
            /* pass */
            if (((strcmp(_tr_strz(ccls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, ccls))) {
                /* pass */
                TrStr ccm = LModule_resolve_method(m, ccls, _tr_str_lit("__contains__"));
                /* pass */
                if ((strcmp(_tr_strz(ccm), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    List_ptr* cca = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(cca, l);
                    /* pass */
                    _tr_str_release(ddn);
                    _tr_str_release(ccls);
                    return _lower_obj_call(m, lf, ccm, b, cca);
                }
            }
            /* pass */
            _tr_str_release(ddn);
            _tr_str_release(ccls);
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0) && _is_dict_tag(bt))) {
            /* pass */
            if ((_dict_key_is_str(bt) && (at != 1LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if (((!_dict_key_is_str(bt)) && (at != 0LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            TrStr dhsym = _dict_sym(bt, _tr_str_lit("has"));
            /* pass */
            LModule_add_extern(m, dhsym);
            /* pass */
            List_i64* dha = (void*)List_i64_new();
            /* pass */
            List_i64_append(dha, b);
            /* pass */
            List_i64_append(dha, a);
            /* pass */
            long long dhd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dhd, dhsym, dha));
            /* pass */
            LFunc_set_vreg_type(lf, dhd, 4LL);
            /* pass */
            _tr_str_release(ddn);
            _tr_str_release(dhsym);
            return dhd;
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0) && _is_set_tag(bt))) {
            /* pass */
            if (((bt == 16LL) && (at != 1LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if (((bt == 13LL) && (at != 0LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            TrStr shas = _set_sym(bt, _tr_str_lit("has"));
            /* pass */
            LModule_add_extern(m, shas);
            /* pass */
            List_i64* sha = (void*)List_i64_new();
            /* pass */
            List_i64_append(sha, b);
            /* pass */
            List_i64_append(sha, a);
            /* pass */
            long long shd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(shd, shas, sha));
            /* pass */
            LFunc_set_vreg_type(lf, shd, 4LL);
            /* pass */
            _tr_str_release(ddn);
            _tr_str_release(shas);
            return shd;
        }
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0)) {
            /* pass */
            if ((!_is_list_tag(bt))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if ((bt == 14LL)) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            long long want_e = _list_elem_tag(bt);
            /* pass */
            if ((at != want_e)) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            TrStr csym = _tr_str_lit("_tr_rt_list_contains_i64");
            /* pass */
            if ((want_e == 1LL)) {
                /* pass */
                TrStr _strtmp_t2341 = _tr_str_lit("_tr_rt_list_contains_str");
                _tr_str_release(csym);
                csym = _strtmp_t2341;
            }
            /* pass */
            LModule_add_extern(m, csym);
            /* pass */
            List_i64* cca = (void*)List_i64_new();
            /* pass */
            List_i64_append(cca, b);
            /* pass */
            List_i64_append(cca, a);
            /* pass */
            long long cd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cd, csym, cca));
            /* pass */
            LFunc_set_vreg_type(lf, cd, 4LL);
            /* pass */
            _tr_str_release(ddn);
            _tr_str_release(csym);
            return cd;
        }
        /* pass */
        if ((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0) && ((at == 1LL) || (bt == 1LL))) && ((at == 0LL) || (bt == 0LL)))) {
            /* pass */
            long long sreg = a;
            /* pass */
            long long nreg = b;
            /* pass */
            if ((at == 0LL)) {
                /* pass */
                sreg = b;
                /* pass */
                nreg = a;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_repeat"));
            /* pass */
            List_i64* ra = (void*)List_i64_new();
            /* pass */
            List_i64_append(ra, sreg);
            /* pass */
            List_i64_append(ra, nreg);
            /* pass */
            long long rd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit("_tr_rt_str_repeat"), ra));
            /* pass */
            LFunc_set_vreg_type(lf, rd, 1LL);
            /* pass */
            _fresh_mark(lf, rd);
            /* pass */
            _tr_str_release(ddn);
            return rd;
        }
        /* pass */
        if ((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) && (at == 1LL)) && (bt == 1LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_concat"));
            /* pass */
            List_i64* ca = (void*)List_i64_new();
            /* pass */
            List_i64_append(ca, a);
            /* pass */
            List_i64_append(ca, b);
            /* pass */
            long long dc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dc, _tr_str_lit("_tr_rt_str_concat"), ca));
            /* pass */
            LFunc_set_vreg_type(lf, dc, 1LL);
            /* pass */
            _fresh_mark(lf, dc);
            /* pass */
            _tr_str_release(ddn);
            return dc;
        }
        /* pass */
        if (((_is_cmp_op(op) && (at == 1LL)) && (bt == 1LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_cmp"));
            /* pass */
            List_i64* sca = (void*)List_i64_new();
            /* pass */
            List_i64_append(sca, a);
            /* pass */
            List_i64_append(sca, b);
            /* pass */
            long long cmpv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit("_tr_rt_str_cmp"), sca));
            /* pass */
            long long zc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
            /* pass */
            long long rc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(rc, op, cmpv, zc));
            /* pass */
            LFunc_set_vreg_type(lf, rc, 4LL);
            /* pass */
            _tr_str_release(ddn);
            return rc;
        }
        /* pass */
        if (((at == 1LL) || (bt == 1LL))) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        if ((_is_list_tag(at) || _is_list_tag(bt))) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        if (((at == 11LL) || (bt == 11LL))) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        if (((at == 10LL) || (bt == 10LL))) {
            /* pass */
            if (((at != 10LL) || (bt != 10LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) != 0) && (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) != 0))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("or"))) == 0))) {
            /* pass */
            long long na = _norm_bool(lf, a);
            /* pass */
            long long nb = _norm_bool(lf, b);
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0)) {
                /* pass */
                long long dand = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(dand, _tr_str_lit("*"), na, nb));
                /* pass */
                LFunc_set_vreg_type(lf, dand, 4LL);
                /* pass */
                _tr_str_release(ddn);
                return dand;
            }
            /* pass */
            long long ssum = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(ssum, _tr_str_lit("+"), na, nb));
            /* pass */
            long long oro = _norm_bool(lf, ssum);
            /* pass */
            LFunc_set_vreg_type(lf, oro, 4LL);
            /* pass */
            _tr_str_release(ddn);
            return oro;
        }
        /* pass */
        if ((!_int_op(op))) {
            /* pass */
            _tr_str_release(ddn);
            return (-1LL);
        }
        /* pass */
        long long d3 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(d3, op, a, b));
        /* pass */
        if (_is_cmp_op(op)) {
            /* pass */
            LFunc_set_vreg_type(lf, d3, 4LL);
        }
        /* pass */
        _tr_str_release(ddn);
        return d3;
    } else if (_t2340.tag == HirExpr_ECall) {
        __auto_type callee = _t2340.data.ECall.callee;
__auto_type args = _t2340.data.ECall.args;
        /* pass */
        TrStr alloc_nm = _tr_str_lit("");
        /* pass */
        __auto_type _t2342 = (*callee);
        if (_t2342.tag == HirExpr_EIdent) {
            __auto_type anm = _t2342.data.EIdent.name;
            TrStr _strtmp_t2343 = _tr_str_retain(anm);
            _tr_str_release(alloc_nm);
            alloc_nm = _strtmp_t2343;
        } else if (_t2342.tag == HirExpr_EIndex) {
            __auto_type abase = _t2342.data.EIndex.obj;
            /* pass */
            __auto_type _t2344 = (*abase);
            if (_t2344.tag == HirExpr_EIdent) {
                __auto_type anm2 = _t2344.data.EIdent.name;
                TrStr _strtmp_t2345 = _tr_str_retain(anm2);
                _tr_str_release(alloc_nm);
                alloc_nm = _strtmp_t2345;
            } else if (1) {
                __auto_type _ = _t2344;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t2342;
            /* pass */
        }
        /* pass */
        if ((((strcmp(_tr_strz(alloc_nm), _tr_strz(_tr_str_lit("alloc"))) == 0) || (strcmp(_tr_strz(alloc_nm), _tr_strz(_tr_str_lit("core_alloc_alloc"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            AstType* aptr_ty = hir_expr_type(e);
            /* pass */
            long long astride = _ptr_stride(m, aptr_ty);
            /* pass */
            if ((astride == 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
            /* pass */
            long long acnt = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if (((acnt < 0LL) || (LFunc_vreg_type(lf, acnt) != 0LL))) {
                /* pass */
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
            /* pass */
            long long astr_c = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(astr_c, astride));
            /* pass */
            long long anb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(anb, _tr_str_lit("*"), acnt, astr_c));
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_raw_alloc"));
            /* pass */
            List_i64* aargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(aargs, anb);
            /* pass */
            long long ad = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ad, _tr_str_lit("_tr_rt_raw_alloc"), aargs));
            /* pass */
            LFunc_set_vreg_type(lf, ad, 0LL);
            /* pass */
            _tr_str_release(alloc_nm);
            return ad;
        }
        /* pass */
        if ((((strcmp(_tr_strz(alloc_nm), _tr_strz(_tr_str_lit("dealloc"))) == 0) || (strcmp(_tr_strz(alloc_nm), _tr_strz(_tr_str_lit("core_alloc_dealloc"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            long long dpv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((dpv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_raw_free"));
            /* pass */
            List_i64* dfa = (void*)List_i64_new();
            /* pass */
            List_i64_append(dfa, dpv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_raw_free"), dfa));
            /* pass */
            _tr_str_release(alloc_nm);
            return dpv;
        }
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (((LFunc_var_index(lf, fn) >= 0LL) && (LFunc_var_type(lf, fn) == 12LL))) {
            /* pass */
            long long kret = LFunc_var_xret_of(lf, fn);
            /* pass */
            if ((kret < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if (((args->len + 1LL) > 6LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long kblk = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(kblk, fn));
            /* pass */
            List_i64* kargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(kargs, kblk);
            /* pass */
            long long ki = 0LL;
            /* pass */
            while ((ki < args->len)) {
                /* pass */
                long long kav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ki)));
                /* pass */
                if ((kav < 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(kargs);
                    return (-1LL);
                }
                /* pass */
                List_i64_append(kargs, kav);
                /* pass */
                ki = (ki + 1LL);
            }
            /* pass */
            long long kfp = _emit_field_get(m, lf, kblk, 0LL, 0LL);
            /* pass */
            long long kd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICallInd(kd, kfp, kargs));
            /* pass */
            LFunc_set_vreg_type(lf, kd, kret);
            /* pass */
            if ((kret == 1LL)) {
                /* pass */
                _fresh_mark(lf, kd);
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return kd;
        }
        /* pass */
        if (((args->len == 0LL) && (_prog_generic_class_index(m, fn) >= 0LL))) {
            /* pass */
            AstType* gct = hir_expr_type(e);
            /* pass */
            if ((gct->args->len == 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            TrStr gcm = _ensure_generic_class(m, gct);
            /* pass */
            if ((strcmp(_tr_strz(gcm), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(gcm);
                return (-1LL);
            }
            /* pass */
            long long gsz = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(gsz, LModule_class_size(m, gcm)));
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_obj_alloc"));
            /* pass */
            List_i64* goa = (void*)List_i64_new();
            /* pass */
            List_i64_append(goa, gsz);
            /* pass */
            long long god = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(god, _tr_str_lit("_tr_rt_obj_alloc"), goa));
            /* pass */
            LFunc_set_vreg_type(lf, god, 10LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(gcm);
            return god;
        }
        /* pass */
        if ((LModule_is_class(m, fn) && (args->len == 0LL))) {
            /* pass */
            long long szc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_class_size(m, fn)));
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_obj_alloc"));
            /* pass */
            List_i64* oaa = (void*)List_i64_new();
            /* pass */
            List_i64_append(oaa, szc);
            /* pass */
            long long od = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit("_tr_rt_obj_alloc"), oaa));
            /* pass */
            LFunc_set_vreg_type(lf, od, 10LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return od;
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("len"))) == 0) && (args->len == 1LL)) && (strcmp(_tr_strz(_recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)))), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            TrStr lncls = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if (LModule_is_class(m, lncls)) {
                /* pass */
                TrStr lnm = LModule_resolve_method(m, lncls, _tr_str_lit("__len__"));
                /* pass */
                if ((strcmp(_tr_strz(lnm), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    long long lnself = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    if ((lnself < 0LL)) {
                        /* pass */
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(lncls);
                        _tr_str_release(lnm);
                        return (-1LL);
                    }
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(lncls);
                    return _lower_obj_call(m, lf, lnm, lnself, (void*)List_ptr_new());
                }
            }
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("len"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long xv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((xv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long xt = LFunc_vreg_type(lf, xv);
            /* pass */
            if (_is_list_tag(xt)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
                /* pass */
                List_i64* lla = (void*)List_i64_new();
                /* pass */
                List_i64_append(lla, xv);
                /* pass */
                long long lld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(lld, _tr_str_lit("_tr_rt_list_len"), lla));
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return lld;
            }
            /* pass */
            if ((xt == 1LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_strlen"));
                /* pass */
                List_i64* sla = (void*)List_i64_new();
                /* pass */
                List_i64_append(sla, xv);
                /* pass */
                long long sld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit("_tr_rt_strlen"), sla));
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return sld;
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("hex"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("oct"))) == 0)) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("bin"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            long long hxv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if (((hxv < 0LL) || (LFunc_vreg_type(lf, hxv) != 0LL))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            TrStr hxsym = _tr_str_lit("_tr_rt_hex_str");
            /* pass */
            if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("oct"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2346 = _tr_str_lit("_tr_rt_oct_str");
                _tr_str_release(hxsym);
                hxsym = _strtmp_t2346;
            } else if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("bin"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2347 = _tr_str_lit("_tr_rt_bin_str");
                _tr_str_release(hxsym);
                hxsym = _strtmp_t2347;
            }
            /* pass */
            LModule_add_extern(m, hxsym);
            /* pass */
            List_i64* hxa = (void*)List_i64_new();
            /* pass */
            List_i64_append(hxa, hxv);
            /* pass */
            long long hxd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(hxd, hxsym, hxa));
            /* pass */
            LFunc_set_vreg_type(lf, hxd, 1LL);
            /* pass */
            _fresh_mark(lf, hxd);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(hxsym);
            return hxd;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("round"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long rv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((rv0 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long rvt = LFunc_vreg_type(lf, rv0);
            /* pass */
            if ((rvt == 0LL)) {
                /* pass */
                rv0 = _promote_f(lf, rv0);
            } else if ((rvt != 5LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_round"));
            /* pass */
            long long rrd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCallF(rrd, _tr_str_lit("_tr_rt_round"), rv0));
            /* pass */
            LFunc_set_vreg_type(lf, rrd, 5LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return rrd;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("abs"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long xv2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((xv2 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long xvt = LFunc_vreg_type(lf, xv2);
            /* pass */
            if ((xvt == 5LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_fabs"));
                /* pass */
                long long fabd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCallF(fabd, _tr_str_lit("_tr_rt_fabs"), xv2));
                /* pass */
                LFunc_set_vreg_type(lf, fabd, 5LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return fabd;
            }
            /* pass */
            if ((xvt != 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_abs_i64"));
            /* pass */
            List_i64* aba = (void*)List_i64_new();
            /* pass */
            List_i64_append(aba, xv2);
            /* pass */
            long long abd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(abd, _tr_str_lit("_tr_rt_abs_i64"), aba));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return abd;
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("min"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("max"))) == 0)) && (args->len == 2LL))) {
            /* pass */
            long long mm1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((mm1 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long mm2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            if ((mm2 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long mt1 = LFunc_vreg_type(lf, mm1);
            /* pass */
            long long mt2 = LFunc_vreg_type(lf, mm2);
            /* pass */
            if (((mt1 == 5LL) || (mt2 == 5LL))) {
                /* pass */
                if ((mt1 == 0LL)) {
                    /* pass */
                    mm1 = _promote_f(lf, mm1);
                } else if ((mt1 != 5LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
                /* pass */
                if ((mt2 == 0LL)) {
                    /* pass */
                    mm2 = _promote_f(lf, mm2);
                } else if ((mt2 != 5LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
                /* pass */
                TrStr fmsym = _tr_str_lit("_tr_rt_min_f64");
                /* pass */
                if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("max"))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t2348 = _tr_str_lit("_tr_rt_max_f64");
                    _tr_str_release(fmsym);
                    fmsym = _strtmp_t2348;
                }
                /* pass */
                LModule_add_extern(m, fmsym);
                /* pass */
                long long fmd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCall2F(fmd, fmsym, mm1, mm2));
                /* pass */
                LFunc_set_vreg_type(lf, fmd, 5LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(fmsym);
                return fmd;
            }
            /* pass */
            if (((mt1 != 0LL) || (mt2 != 0LL))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            TrStr msym = _tr_str_lit("_tr_rt_min_i64");
            /* pass */
            if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("max"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2349 = _tr_str_lit("_tr_rt_max_i64");
                _tr_str_release(msym);
                msym = _strtmp_t2349;
            }
            /* pass */
            LModule_add_extern(m, msym);
            /* pass */
            List_i64* mma = (void*)List_i64_new();
            /* pass */
            List_i64_append(mma, mm1);
            /* pass */
            List_i64_append(mma, mm2);
            /* pass */
            long long mmd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(mmd, msym, mma));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(msym);
            return mmd;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("str"))) == 0) && (args->len == 1LL))) {
            /* pass */
            TrStr sobjc = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((((strcmp(_tr_strz(sobjc), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, sobjc)) && (strcmp(_tr_strz(LModule_resolve_method(m, sobjc, _tr_str_lit("__str__"))), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                long long sobjr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), (-1LL));
                /* pass */
                if ((sobjr >= 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(sobjc);
                    return sobjr;
                }
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return (-1LL);
            }
            /* pass */
            long long cv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((cv0 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return (-1LL);
            }
            /* pass */
            long long cvt = LFunc_vreg_type(lf, cv0);
            /* pass */
            if ((cvt == 1LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return cv0;
            }
            /* pass */
            if ((cvt == 5LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_f64_to_str"));
                /* pass */
                long long fsd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCall1(fsd, _tr_str_lit("_tr_rt_f64_to_str"), cv0));
                /* pass */
                LFunc_set_vreg_type(lf, fsd, 1LL);
                /* pass */
                _fresh_mark(lf, fsd);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return fsd;
            }
            /* pass */
            TrStr ssym = _tr_str_lit("_tr_rt_i64_to_str");
            /* pass */
            if ((cvt == 4LL)) {
                /* pass */
                TrStr _strtmp_t2350 = _tr_str_lit("_tr_rt_bool_to_str");
                _tr_str_release(ssym);
                ssym = _strtmp_t2350;
            } else if ((cvt != 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                _tr_str_release(ssym);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, ssym);
            /* pass */
            List_i64* s2a = (void*)List_i64_new();
            /* pass */
            List_i64_append(s2a, cv0);
            /* pass */
            long long s2d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(s2d, ssym, s2a));
            /* pass */
            LFunc_set_vreg_type(lf, s2d, 1LL);
            /* pass */
            _fresh_mark(lf, s2d);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(sobjc);
            _tr_str_release(ssym);
            return s2d;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("float"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long fv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((fv0 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long fvt = LFunc_vreg_type(lf, fv0);
            /* pass */
            if ((fvt == 5LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return fv0;
            }
            /* pass */
            if ((fvt == 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _promote_f(lf, fv0);
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("_tr_fn_int"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            long long iv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((iv0 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long ivt = LFunc_vreg_type(lf, iv0);
            /* pass */
            if ((ivt == 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return iv0;
            }
            /* pass */
            if ((ivt == 4LL)) {
                /* pass */
                LFunc_set_vreg_type(lf, iv0, 0LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return iv0;
            }
            /* pass */
            if ((ivt == 5LL)) {
                /* pass */
                long long itd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFToI(itd, iv0));
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return itd;
            }
            /* pass */
            if ((ivt != 1LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_to_i64"));
            /* pass */
            List_i64* i2a = (void*)List_i64_new();
            /* pass */
            List_i64_append(i2a, iv0);
            /* pass */
            long long i2d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(i2d, _tr_str_lit("_tr_rt_str_to_i64"), i2a));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return i2d;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("sum"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long suv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((suv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, suv) != 2LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_sum_i64"));
            /* pass */
            List_i64* sua = (void*)List_i64_new();
            /* pass */
            List_i64_append(sua, suv);
            /* pass */
            long long sud = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(sud, _tr_str_lit("_tr_rt_list_sum_i64"), sua));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return sud;
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("any"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("all"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            long long anv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((anv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, anv) != 2LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            TrStr ansym = _tr_str_lit("_tr_rt_list_any_i64");
            /* pass */
            if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("all"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2351 = _tr_str_lit("_tr_rt_list_all_i64");
                _tr_str_release(ansym);
                ansym = _strtmp_t2351;
            }
            /* pass */
            LModule_add_extern(m, ansym);
            /* pass */
            List_i64* ana = (void*)List_i64_new();
            /* pass */
            List_i64_append(ana, anv);
            /* pass */
            long long and2 = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(and2, ansym, ana));
            /* pass */
            LFunc_set_vreg_type(lf, and2, 4LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(ansym);
            return and2;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("ord"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long ordv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((ordv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, ordv) != 1LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_ord"));
            /* pass */
            List_i64* orda = (void*)List_i64_new();
            /* pass */
            List_i64_append(orda, ordv);
            /* pass */
            long long ordd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ordd, _tr_str_lit("_tr_rt_str_ord"), orda));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return ordd;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("bool"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long bv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((bv0 < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if (_is_list_tag(LFunc_vreg_type(lf, bv0))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long bnorm = _norm_bool(lf, bv0);
            /* pass */
            LFunc_set_vreg_type(lf, bnorm, 4LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return bnorm;
        }
        /* pass */
        if (LModule_is_extern_fn(m, fn)) {
            /* pass */
            if ((args->len > 6LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            List_i64* exargs = (void*)List_i64_new();
            /* pass */
            long long exi = 0LL;
            /* pass */
            while ((exi < args->len)) {
                /* pass */
                long long exv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, exi)));
                /* pass */
                if ((exv < 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(exargs);
                    return (-1LL);
                }
                /* pass */
                long long ext = LFunc_vreg_type(lf, exv);
                /* pass */
                if ((((ext != 0LL) && (ext != 1LL)) && (ext != 4LL))) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(exargs);
                    return (-1LL);
                }
                /* pass */
                List_i64_append(exargs, exv);
                /* pass */
                exi = (exi + 1LL);
            }
            /* pass */
            LModule_add_extern(m, fn);
            /* pass */
            long long extret = LModule_extern_ret_tag(m, fn);
            /* pass */
            long long exd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(exd, fn, exargs));
            /* pass */
            LFunc_set_vreg_type(lf, exd, extret);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return exd;
        }
        /* pass */
        if ((!LModule_is_user_fn(m, fn))) {
            /* pass */
            long long gfi = _find_generic_fn(m, fn);
            /* pass */
            if ((gfi < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((args->len > 6LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            List_i64* gvregs = (void*)List_i64_new();
            /* pass */
            List_i64* gtags = (void*)List_i64_new();
            /* pass */
            List_TrStr* gcls = (void*)List_TrStr_new();
            /* pass */
            TrStr gmangled = _tr_strx_concat(_tr_strz(fn), _tr_strz(_tr_str_lit("__m")));
            /* pass */
            long long gi = 0LL;
            /* pass */
            while ((gi < args->len)) {
                /* pass */
                long long gav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, gi)));
                /* pass */
                if ((gav < 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(gvregs);
                    List_i64_free(gtags);
                    List_TrStr_free(gcls);
                    _tr_str_release(gmangled);
                    return (-1LL);
                }
                /* pass */
                long long gat = LFunc_vreg_type(lf, gav);
                /* pass */
                List_i64_append(gvregs, gav);
                /* pass */
                List_i64_append(gtags, gat);
                /* pass */
                TrStr gcn = _tr_str_lit("");
                /* pass */
                if (((gat == 10LL) || (gat == 11LL))) {
                    /* pass */
                    TrStr _strtmp_t2352 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, gi)));
                    _tr_str_release(gcn);
                    gcn = _strtmp_t2352;
                    /* pass */
                    if ((strcmp(_tr_strz(gcn), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        List_i64_free(gvregs);
                        List_i64_free(gtags);
                        List_TrStr_free(gcls);
                        _tr_str_release(gmangled);
                        _tr_str_release(gcn);
                        return (-1LL);
                    }
                }
                /* pass */
                List_TrStr_append(gcls, gcn);
                /* pass */
                if ((gi > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t2353 = _tr_strx_concat(_tr_strz(gmangled), _tr_strz(_tr_str_lit("_")));
                    _tr_str_release(gmangled);
                    gmangled = _strtmp_t2353;
                }
                /* pass */
                TrStr _strtmp_t2354 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gat)))); TrStr _cres = _tr_strx_concat(_tr_strz(gmangled), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(gmangled);
                gmangled = _strtmp_t2354;
                /* pass */
                if ((strcmp(_tr_strz(gcn), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t2355 = _tr_strx_concat(_tr_strz(gmangled), _tr_strz(gcn));
                    _tr_str_release(gmangled);
                    gmangled = _strtmp_t2355;
                }
                /* pass */
                gi = (gi + 1LL);
                _tr_str_release(gcn);
            }
            /* pass */
            if ((!LModule_is_user_fn(m, gmangled))) {
                /* pass */
                if ((!_lir_lower_generic(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, gfi)), gtags, gcls, gmangled))) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(gvregs);
                    List_i64_free(gtags);
                    List_TrStr_free(gcls);
                    _tr_str_release(gmangled);
                    return (-1LL);
                }
            }
            /* pass */
            long long grtag = LModule_fn_ret_tag(m, gmangled);
            /* pass */
            long long gd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(gd, gmangled, gvregs));
            /* pass */
            LFunc_set_vreg_type(lf, gd, grtag);
            /* pass */
            if ((grtag == 1LL)) {
                /* pass */
                _fresh_mark(lf, gd);
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(gmangled);
            return gd;
        }
        /* pass */
        if ((args->len > 6LL)) {
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        long long rtag = LModule_fn_ret_tag(m, fn);
        /* pass */
        if ((rtag < 0LL)) {
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        List_i64* argvregs = (void*)List_i64_new();
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            long long avr = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            if ((avr < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                List_i64_free(argvregs);
                return (-1LL);
            }
            /* pass */
            List_i64_append(argvregs, avr);
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        long long d4 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(d4, fn, argvregs));
        /* pass */
        LFunc_set_vreg_type(lf, d4, rtag);
        /* pass */
        if ((rtag == 1LL)) {
            /* pass */
            _fresh_mark(lf, d4);
        }
        /* pass */
        _tr_str_release(alloc_nm);
        _tr_str_release(fn);
        return d4;
    } else if (_t2340.tag == HirExpr_ETuple) {
        __auto_type titems = _t2340.data.ETuple.items;
        /* pass */
        if ((titems->len == 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        List_i64* tvals = (void*)List_i64_new();
        /* pass */
        long long tj = 0LL;
        /* pass */
        while ((tj < titems->len)) {
            /* pass */
            long long tev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(titems, tj)));
            /* pass */
            if ((tev < 0LL)) {
                /* pass */
                List_i64_free(tvals);
                return (-1LL);
            }
            /* pass */
            long long tevt = LFunc_vreg_type(lf, tev);
            /* pass */
            if (((((_is_list_tag(tevt) || _is_dict_tag(tevt)) || _is_set_tag(tevt)) || (tevt == 12LL)) || (tevt == 15LL))) {
                /* pass */
                List_i64_free(tvals);
                return (-1LL);
            }
            /* pass */
            if ((tevt == 5LL)) {
                /* pass */
                long long tfb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(tfb, tev));
                /* pass */
                tev = tfb;
            } else if ((tevt == 1LL)) {
                /* pass */
                _secure_str(m, lf, tev);
            }
            /* pass */
            List_i64_append(tvals, tev);
            /* pass */
            tj = (tj + 1LL);
        }
        /* pass */
        long long tsz = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(tsz, (titems->len * 8LL)));
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_obj_alloc"));
        /* pass */
        List_i64* tba = (void*)List_i64_new();
        /* pass */
        List_i64_append(tba, tsz);
        /* pass */
        long long tblk = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(tblk, _tr_str_lit("_tr_rt_obj_alloc"), tba));
        /* pass */
        LFunc_set_vreg_type(lf, tblk, 15LL);
        /* pass */
        long long tk = 0LL;
        /* pass */
        while ((tk < tvals->len)) {
            /* pass */
            _emit_field_set(m, lf, tblk, (tk * 8LL), List_i64_get(tvals, tk));
            /* pass */
            tk = (tk + 1LL);
        }
        /* pass */
        List_i64_free(tvals);
        return tblk;
    } else if (_t2340.tag == HirExpr_ESet) {
        __auto_type sitems = _t2340.data.ESet.items;
        /* pass */
        if ((sitems->len == 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long sev0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(sitems, 0LL)));
        /* pass */
        if ((sev0 < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long setag = 13LL;
        /* pass */
        if ((LFunc_vreg_type(lf, sev0) == 1LL)) {
            /* pass */
            setag = 16LL;
        } else if ((LFunc_vreg_type(lf, sev0) != 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr snew = _set_sym(setag, _tr_str_lit("new"));
        /* pass */
        LModule_add_extern(m, snew);
        /* pass */
        long long shv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(shv, snew, (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, shv, setag);
        /* pass */
        TrStr sadd = _set_sym(setag, _tr_str_lit("set"));
        /* pass */
        LModule_add_extern(m, sadd);
        /* pass */
        long long sone = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(sone, 1LL));
        /* pass */
        List_i64* sfirst = (void*)List_i64_new();
        /* pass */
        List_i64_append(sfirst, shv);
        /* pass */
        List_i64_append(sfirst, sev0);
        /* pass */
        List_i64_append(sfirst, sone);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sadd, sfirst));
        /* pass */
        long long sli = 1LL;
        /* pass */
        while ((sli < sitems->len)) {
            /* pass */
            long long sev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(sitems, sli)));
            /* pass */
            if ((sev < 0LL)) {
                /* pass */
                _tr_str_release(snew);
                _tr_str_release(sadd);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, sev) != LFunc_vreg_type(lf, sev0))) {
                /* pass */
                _tr_str_release(snew);
                _tr_str_release(sadd);
                return (-1LL);
            }
            /* pass */
            List_i64* sargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(sargs, shv);
            /* pass */
            List_i64_append(sargs, sev);
            /* pass */
            List_i64_append(sargs, sone);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), sadd, sargs));
            /* pass */
            sli = (sli + 1LL);
        }
        /* pass */
        _tr_str_release(snew);
        _tr_str_release(sadd);
        return shv;
    } else if (_t2340.tag == HirExpr_EList) {
        __auto_type items = _t2340.data.EList.items;
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_new"));
        /* pass */
        long long hv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit("_tr_rt_list_new"), (void*)List_i64_new()));
        /* pass */
        long long elem_t = 0LL;
        /* pass */
        long long list_tag = 2LL;
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < items->len)) {
            /* pass */
            long long ev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(items, li)));
            /* pass */
            if ((ev < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long evt = LFunc_vreg_type(lf, ev);
            /* pass */
            if (((((evt != 0LL) && (evt != 1LL)) && (evt != 5LL)) && (evt != 10LL))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((li == 0LL)) {
                /* pass */
                elem_t = evt;
                /* pass */
                list_tag = _list_tag_for_elem(evt);
            } else if ((evt != elem_t)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((evt == 1LL)) {
                /* pass */
                _secure_str(m, lf, ev);
            }
            /* pass */
            if ((evt == 5LL)) {
                /* pass */
                long long evb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(evb, ev));
                /* pass */
                ev = evb;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_push_i64"));
            /* pass */
            List_i64* pa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pa, hv);
            /* pass */
            List_i64_append(pa, ev);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_push_i64"), pa));
            /* pass */
            li = (li + 1LL);
        }
        /* pass */
        LFunc_set_vreg_type(lf, hv, list_tag);
        /* pass */
        return hv;
    } else if (_t2340.tag == HirExpr_EDict) {
        __auto_type keys = _t2340.data.EDict.keys;
__auto_type vals = _t2340.data.EDict.vals;
__auto_type dty = _t2340.data.EDict.ty;
        /* pass */
        long long dtag = _ast_type_tag(dty);
        /* pass */
        if ((keys->len > 0LL)) {
            /* pass */
            long long k0 = _ast_type_tag(hir_expr_type(((HirExpr*)List_ptr_get(keys, 0LL))));
            /* pass */
            long long v0 = _ast_type_tag(hir_expr_type(((HirExpr*)List_ptr_get(vals, 0LL))));
            /* pass */
            if (((k0 == 1LL) && (v0 == 0LL))) {
                /* pass */
                dtag = 6LL;
            } else if (((k0 == 0LL) && (v0 == 0LL))) {
                /* pass */
                dtag = 7LL;
            } else if (((k0 == 1LL) && (v0 == 1LL))) {
                /* pass */
                dtag = 8LL;
            } else if (((k0 == 0LL) && (v0 == 1LL))) {
                /* pass */
                dtag = 9LL;
            } else if (((k0 == 1LL) && (v0 == 5LL))) {
                /* pass */
                dtag = 17LL;
            } else if (((k0 == 0LL) && (v0 == 5LL))) {
                /* pass */
                dtag = 18LL;
            } else {
                /* pass */
                return (-1LL);
            }
        }
        /* pass */
        if ((!_is_dict_tag(dtag))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr dnew = _dict_new_sym(dtag);
        /* pass */
        LModule_add_extern(m, dnew);
        /* pass */
        long long dhv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(dhv, dnew, (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, dhv, dtag);
        /* pass */
        bool kstr = _dict_key_is_str(dtag);
        /* pass */
        long long vtag = _dict_val_tag(dtag);
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < keys->len)) {
            /* pass */
            long long kv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(keys, di)));
            /* pass */
            if ((kv < 0LL)) {
                /* pass */
                _tr_str_release(dnew);
                return (-1LL);
            }
            /* pass */
            if ((kstr && (LFunc_vreg_type(lf, kv) != 1LL))) {
                /* pass */
                _tr_str_release(dnew);
                return (-1LL);
            }
            /* pass */
            if (((!kstr) && (LFunc_vreg_type(lf, kv) != 0LL))) {
                /* pass */
                _tr_str_release(dnew);
                return (-1LL);
            }
            /* pass */
            long long vv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(vals, di)));
            /* pass */
            if ((vv < 0LL)) {
                /* pass */
                _tr_str_release(dnew);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, vv) != vtag)) {
                /* pass */
                _tr_str_release(dnew);
                return (-1LL);
            }
            /* pass */
            if ((vtag == 1LL)) {
                /* pass */
                _secure_str(m, lf, vv);
            }
            /* pass */
            if ((vtag == 5LL)) {
                /* pass */
                long long dvb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(dvb, vv));
                /* pass */
                vv = dvb;
            }
            /* pass */
            TrStr dset = _dict_sym(dtag, _tr_str_lit("set"));
            /* pass */
            LModule_add_extern(m, dset);
            /* pass */
            List_i64* dsa = (void*)List_i64_new();
            /* pass */
            List_i64_append(dsa, dhv);
            /* pass */
            List_i64_append(dsa, kv);
            /* pass */
            List_i64_append(dsa, vv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), dset, dsa));
            /* pass */
            di = (di + 1LL);
            _tr_str_release(dset);
        }
        /* pass */
        _tr_str_release(dnew);
        return dhv;
    } else if (_t2340.tag == HirExpr_EFString) {
        __auto_type parts = _t2340.data.EFString.parts;
        /* pass */
        long long acc = _heap_lit(m, lf, _tr_str_lit(""));
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < parts->len)) {
            /* pass */
            HirFStringPart* fp = ((HirFStringPart*)List_ptr_get(parts, fi));
            /* pass */
            long long pr = (-1LL);
            /* pass */
            if (fp->is_expr) {
                /* pass */
                if ((strcmp(_tr_strz(fp->fmt_spec), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                TrStr fcls = _recv_class(m, lf, fp->expr);
                /* pass */
                if ((((strcmp(_tr_strz(fcls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, fcls)) && (strcmp(_tr_strz(LModule_resolve_method(m, fcls, _tr_str_lit("__str__"))), _tr_strz(_tr_str_lit(""))) != 0))) {
                    /* pass */
                    pr = _obj_to_str(m, lf, fp->expr, (-1LL));
                    /* pass */
                    if ((pr < 0LL)) {
                        /* pass */
                        _tr_str_release(fcls);
                        return (-1LL);
                    }
                } else {
                    /* pass */
                    long long fev = lower_expr(m, lf, fp->expr);
                    /* pass */
                    if ((fev < 0LL)) {
                        /* pass */
                        _tr_str_release(fcls);
                        return (-1LL);
                    }
                    /* pass */
                    pr = _reg_to_str(m, lf, fev);
                    /* pass */
                    if ((pr < 0LL)) {
                        /* pass */
                        _tr_str_release(fcls);
                        return (-1LL);
                    }
                }
            } else {
                /* pass */
                pr = _heap_lit(m, lf, fp->text);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_concat"));
            /* pass */
            List_i64* fca = (void*)List_i64_new();
            /* pass */
            List_i64_append(fca, acc);
            /* pass */
            List_i64_append(fca, pr);
            /* pass */
            long long fdc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(fdc, _tr_str_lit("_tr_rt_str_concat"), fca));
            /* pass */
            LFunc_set_vreg_type(lf, fdc, 1LL);
            /* pass */
            _fresh_mark(lf, fdc);
            /* pass */
            acc = fdc;
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        return acc;
    } else if (_t2340.tag == HirExpr_EClosure) {
        __auto_type cparams = _t2340.data.EClosure.params;
__auto_type cret = _t2340.data.EClosure.ret_ty;
__auto_type cbody = _t2340.data.EClosure.body;
__auto_type cis_async = _t2340.data.EClosure.is_async;
__auto_type ccaps = _t2340.data.EClosure.captures;
        /* pass */
        if (cis_async) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long crtag = _tag_of(m, cret);
        /* pass */
        if ((crtag < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((cparams->len + 1LL) > 6LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr cname = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("_tr_clo_")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lf->name)); _tr_str_release(_cl); _cres; });
        /* pass */
        LFunc* clf = ({ TrStr _at_t2356 = (_own(cname)); __auto_type _wr = (LFunc_init(_at_t2356)); _tr_str_release(_at_t2356); _wr; });
        /* pass */
        List_TrStr_append(clf->params, _tr_str_lit("__env"));
        /* pass */
        LFunc_add_var(clf, _tr_str_lit("__env"));
        /* pass */
        long long cpj = 0LL;
        /* pass */
        while ((cpj < cparams->len)) {
            /* pass */
            HirParam* cp = ((HirParam*)List_ptr_get(cparams, cpj));
            /* pass */
            long long cptag = _tag_of(m, cp->ty);
            /* pass */
            if ((cptag < 0LL)) {
                /* pass */
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
            /* pass */
            List_TrStr_append(clf->params, cp->name);
            /* pass */
            LFunc_add_var(clf, cp->name);
            /* pass */
            LFunc_set_var_type(clf, cp->name, cptag);
            /* pass */
            if ((((cptag == 10LL) || (cptag == 11LL)) && (!_is_null_str(cp->ty->name)))) {
                /* pass */
                ({ TrStr _at_t2357 = (_own(cp->ty->name)); LFunc_set_var_cls(clf, cp->name, _at_t2357); _tr_str_release(_at_t2357); });
            }
            /* pass */
            cpj = (cpj + 1LL);
        }
        /* pass */
        long long ccj = 0LL;
        /* pass */
        while ((ccj < ccaps->len)) {
            /* pass */
            HirParam* cc = ((HirParam*)List_ptr_get(ccaps, ccj));
            /* pass */
            if ((LFunc_var_index(lf, cc->name) < 0LL)) {
                /* pass */
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
            /* pass */
            long long cctag = LFunc_var_type(lf, cc->name);
            /* pass */
            if (((((cctag != 0LL) && (cctag != 1LL)) && (cctag != 4LL)) && (cctag != 5LL))) {
                /* pass */
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
            /* pass */
            ({ TrStr _at_t2358 = (_own(cc->name)); List_TrStr_append(clf->captures, _at_t2358); _tr_str_release(_at_t2358); });
            /* pass */
            List_i64_append(clf->cap_tags, cctag);
            /* pass */
            ccj = (ccj + 1LL);
        }
        /* pass */
        ({ TrStr _at_t2359 = (_own(cname)); List_TrStr_append(m->fn_names, _at_t2359); _tr_str_release(_at_t2359); });
        /* pass */
        List_i64_append(m->fn_ret, crtag);
        /* pass */
        LFunc_set_cur(clf, LFunc_new_block(clf));
        /* pass */
        if ((!lower_block(m, clf, cbody))) {
            /* pass */
            _tr_str_release(cname);
            _tr_obj_release(clf, _trdrop_LFunc);
            return (-1LL);
        }
        /* pass */
        LFunc_set_term(clf, LTerm_ctor_TRetInt(0LL));
        /* pass */
        List_ptr_append(m->funcs, _tr_obj_retain(clf));
        /* pass */
        long long cblk_sz = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(cblk_sz, ((1LL + ccaps->len) * 8LL)));
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_obj_alloc"));
        /* pass */
        List_i64* cba = (void*)List_i64_new();
        /* pass */
        List_i64_append(cba, cblk_sz);
        /* pass */
        long long cblk = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cblk, _tr_str_lit("_tr_rt_obj_alloc"), cba));
        /* pass */
        LFunc_set_vreg_type(lf, cblk, 12LL);
        /* pass */
        long long cfa = LFunc_new_vreg(lf);
        /* pass */
        ({ TrStr _at_t2360 = (_own(cname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(cfa, _at_t2360)); _tr_str_release(_at_t2360); });
        /* pass */
        _emit_field_set(m, lf, cblk, 0LL, cfa);
        /* pass */
        long long cbi = 0LL;
        /* pass */
        while ((cbi < ccaps->len)) {
            /* pass */
            long long cav = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IAddrVar(cav, ((HirParam*)List_ptr_get(ccaps, cbi))->name));
            /* pass */
            _emit_field_set(m, lf, cblk, ((1LL + cbi) * 8LL), cav);
            /* pass */
            cbi = (cbi + 1LL);
        }
        /* pass */
        LFunc_set_vreg_xret(lf, cblk, crtag);
        /* pass */
        _tr_str_release(cname);
        _tr_obj_release(clf, _trdrop_LFunc);
        return cblk;
    } else if (_t2340.tag == HirExpr_EIndex) {
        __auto_type obj = _t2340.data.EIndex.obj;
__auto_type idx = _t2340.data.EIndex._tr_v_index;
        /* pass */
        TrStr gicls = _recv_class(m, lf, obj);
        /* pass */
        if (((strcmp(_tr_strz(gicls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, gicls))) {
            /* pass */
            TrStr gim = LModule_resolve_method(m, gicls, _tr_str_lit("__getitem__"));
            /* pass */
            if ((strcmp(_tr_strz(gim), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                long long giself = lower_expr(m, lf, obj);
                /* pass */
                if ((giself < 0LL)) {
                    /* pass */
                    _tr_str_release(gicls);
                    _tr_str_release(gim);
                    return (-1LL);
                }
                /* pass */
                List_ptr* giargs = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(giargs, idx);
                /* pass */
                _tr_str_release(gicls);
                return _lower_obj_call(m, lf, gim, giself, giargs);
            }
        }
        /* pass */
        long long ov = lower_expr(m, lf, obj);
        /* pass */
        if ((ov < 0LL)) {
            /* pass */
            _tr_str_release(gicls);
            return (-1LL);
        }
        /* pass */
        long long ovt = LFunc_vreg_type(lf, ov);
        /* pass */
        if (_is_dict_tag(ovt)) {
            /* pass */
            long long dkv = lower_expr(m, lf, idx);
            /* pass */
            if ((dkv < 0LL)) {
                /* pass */
                _tr_str_release(gicls);
                return (-1LL);
            }
            /* pass */
            if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, dkv) != 1LL))) {
                /* pass */
                _tr_str_release(gicls);
                return (-1LL);
            }
            /* pass */
            if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, dkv) != 0LL))) {
                /* pass */
                _tr_str_release(gicls);
                return (-1LL);
            }
            /* pass */
            TrStr dget = _dict_sym(ovt, _tr_str_lit("get"));
            /* pass */
            LModule_add_extern(m, dget);
            /* pass */
            List_i64* dga = (void*)List_i64_new();
            /* pass */
            List_i64_append(dga, ov);
            /* pass */
            List_i64_append(dga, dkv);
            /* pass */
            long long dgd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dgd, dget, dga));
            /* pass */
            if ((_dict_val_tag(ovt) == 5LL)) {
                /* pass */
                long long dgf = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(dgf, dgd));
                /* pass */
                LFunc_set_vreg_type(lf, dgf, 5LL);
                /* pass */
                _tr_str_release(gicls);
                _tr_str_release(dget);
                return dgf;
            }
            /* pass */
            LFunc_set_vreg_type(lf, dgd, _dict_val_tag(ovt));
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(dget);
            return dgd;
        }
        /* pass */
        if ((!_is_list_tag(ovt))) {
            /* pass */
            _tr_str_release(gicls);
            return (-1LL);
        }
        /* pass */
        long long iv = lower_expr(m, lf, idx);
        /* pass */
        if ((iv < 0LL)) {
            /* pass */
            _tr_str_release(gicls);
            return (-1LL);
        }
        /* pass */
        _tr_str_release(gicls);
        return _list_get_elem(m, lf, ovt, ov, iv);
    } else if (_t2340.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t2340.data.EPropAccess.obj;
__auto_type prop = _t2340.data.EPropAccess.prop;
        /* pass */
        __auto_type _t2361 = (*obj);
        if (_t2361.tag == HirExpr_EIdent) {
            __auto_type uvn = _t2361.data.EIdent.name;
            /* pass */
            if (({ TrStr _at_t2362 = (_norm_variant(uvn, prop)); __auto_type _wr = ((LModule_is_enum(m, uvn) && (LModule_enum_variant_index(m, uvn, _at_t2362) >= 0LL))); _tr_str_release(_at_t2362); _wr; })) {
                /* pass */
                return ({ TrStr _at_t2363 = (_norm_variant(uvn, prop)); __auto_type _wr = (_lower_enum_ctor(m, lf, uvn, _at_t2363, (void*)List_ptr_new())); _tr_str_release(_at_t2363); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t2361;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr pcls = _recv_class(m, lf, obj);
        /* pass */
        if (((strcmp(_tr_strz(pcls), _tr_strz(_tr_str_lit(""))) != 0) && LModule_is_class(m, pcls))) {
            /* pass */
            long long foff = LModule_field_offset(m, pcls, prop);
            /* pass */
            if ((foff < 0LL)) {
                /* pass */
                _tr_str_release(pcls);
                return (-1LL);
            }
            /* pass */
            long long ftg = LModule_field_tag(m, pcls, prop);
            /* pass */
            if ((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg))) {
                /* pass */
                _tr_str_release(pcls);
                return (-1LL);
            }
            /* pass */
            long long fobj = lower_expr(m, lf, obj);
            /* pass */
            if ((fobj < 0LL)) {
                /* pass */
                _tr_str_release(pcls);
                return (-1LL);
            }
            /* pass */
            if ((ftg == 5LL)) {
                /* pass */
                long long fraw = _emit_field_get(m, lf, fobj, foff, 0LL);
                /* pass */
                long long ffv = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(ffv, fraw));
                /* pass */
                LFunc_set_vreg_type(lf, ffv, 5LL);
                /* pass */
                _tr_str_release(pcls);
                return ffv;
            }
            /* pass */
            _tr_str_release(pcls);
            return _emit_field_get(m, lf, fobj, foff, ftg);
        }
        /* pass */
        if ((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("len"))) != 0)) {
            /* pass */
            _tr_str_release(pcls);
            return (-1LL);
        }
        /* pass */
        long long ovl = lower_expr(m, lf, obj);
        /* pass */
        if ((ovl < 0LL)) {
            /* pass */
            _tr_str_release(pcls);
            return (-1LL);
        }
        /* pass */
        if (_is_set_tag(LFunc_vreg_type(lf, ovl))) {
            /* pass */
            TrStr pslsym = _set_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit("len"));
            /* pass */
            LModule_add_extern(m, pslsym);
            /* pass */
            List_i64* psla = (void*)List_i64_new();
            /* pass */
            List_i64_append(psla, ovl);
            /* pass */
            long long psld = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(psld, pslsym, psla));
            /* pass */
            _tr_str_release(pcls);
            _tr_str_release(pslsym);
            return psld;
        }
        /* pass */
        if (_is_dict_tag(LFunc_vreg_type(lf, ovl))) {
            /* pass */
            TrStr dlsym = _dict_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit("len"));
            /* pass */
            LModule_add_extern(m, dlsym);
            /* pass */
            List_i64* dla = (void*)List_i64_new();
            /* pass */
            List_i64_append(dla, ovl);
            /* pass */
            long long dld = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dld, dlsym, dla));
            /* pass */
            _tr_str_release(pcls);
            _tr_str_release(dlsym);
            return dld;
        }
        /* pass */
        if ((!_is_list_tag(LFunc_vreg_type(lf, ovl)))) {
            /* pass */
            _tr_str_release(pcls);
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, ovl);
        /* pass */
        long long ld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit("_tr_rt_list_len"), la));
        /* pass */
        _tr_str_release(pcls);
        return ld;
    } else if (_t2340.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t2340.data.EMethodCall.obj;
__auto_type method = _t2340.data.EMethodCall.method;
__auto_type margs = _t2340.data.EMethodCall.args;
        /* pass */
        if ((strcmp(_tr_strz(hir_expr_type(obj)->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
            /* pass */
            AstType* pty = hir_expr_type(obj);
            /* pass */
            long long pstride = _ptr_stride(m, pty);
            /* pass */
            if ((pstride == 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long petag = 0LL;
            /* pass */
            if ((pty->args->len > 0LL)) {
                /* pass */
                TrStr petn = (*((AstType**)List_ptr_get(pty->args, 0LL)))->name;
                /* pass */
                if (((strcmp(_tr_strz(petn), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(petn), _tr_strz(_tr_str_lit("f64"))) == 0))) {
                    /* pass */
                    petag = 5LL;
                } else if (LModule_is_class(m, petn)) {
                    /* pass */
                    petag = 10LL;
                } else if (LModule_is_enum(m, petn)) {
                    /* pass */
                    petag = 11LL;
                }
            }
            /* pass */
            long long pbase = lower_expr(m, lf, obj);
            /* pass */
            if ((pbase < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("offset"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add"))) == 0)) && (margs->len == 1LL))) {
                /* pass */
                long long poff = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((poff < 0LL) || (LFunc_vreg_type(lf, poff) != 0LL))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                long long pstr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(pstr, pstride));
                /* pass */
                long long pscaled = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(pscaled, _tr_str_lit("*"), poff, pstr));
                /* pass */
                long long padr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(padr, _tr_str_lit("+"), pbase, pscaled));
                /* pass */
                LFunc_set_vreg_type(lf, padr, 0LL);
                /* pass */
                return padr;
            }
            /* pass */
            if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("read"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("deref"))) == 0)) && (margs->len == 0LL))) {
                /* pass */
                if ((petag == 5LL)) {
                    /* pass */
                    long long prraw = _emit_field_get(m, lf, pbase, 0LL, 0LL);
                    /* pass */
                    long long prf = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IBitsF(prf, prraw));
                    /* pass */
                    LFunc_set_vreg_type(lf, prf, 5LL);
                    /* pass */
                    return prf;
                }
                /* pass */
                return _emit_field_get(m, lf, pbase, 0LL, petag);
            }
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("write"))) == 0) && (margs->len == 1LL))) {
                /* pass */
                long long pwv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if ((pwv < 0LL)) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                if ((petag == 5LL)) {
                    /* pass */
                    long long pwvt = LFunc_vreg_type(lf, pwv);
                    /* pass */
                    if ((pwvt == 0LL)) {
                        /* pass */
                        pwv = _promote_f(lf, pwv);
                    } else if ((pwvt != 5LL)) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long pwb = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IFBits(pwb, pwv));
                    /* pass */
                    pwv = pwb;
                }
                /* pass */
                _emit_field_set(m, lf, pbase, 0LL, pwv);
                /* pass */
                return pbase;
            }
            /* pass */
            return (-1LL);
        }
        /* pass */
        __auto_type _t2364 = (*obj);
        if (_t2364.tag == HirExpr_EIdent) {
            __auto_type inm = _t2364.data.EIdent.name;
__auto_type ity_g = _t2364.data.EIdent.ty;
            /* pass */
            if (((_prog_generic_class_index(m, inm) >= 0LL) && (ity_g->args->len > 0LL))) {
                /* pass */
                TrStr gsm = _ensure_generic_class(m, ity_g);
                /* pass */
                if ((strcmp(_tr_strz(gsm), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    _tr_str_release(gsm);
                    return (-1LL);
                }
                /* pass */
                return ({ TrStr _at_t2365 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(gsm), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t2365, (-1LL), margs)); _tr_str_release(_at_t2365); _wr; });
            }
            /* pass */
            if (LModule_is_class(m, inm)) {
                /* pass */
                TrStr smang = LModule_resolve_method(m, inm, method);
                /* pass */
                if ((strcmp(_tr_strz(smang), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    _tr_str_release(smang);
                    return (-1LL);
                }
                /* pass */
                return _lower_obj_call(m, lf, smang, (-1LL), margs);
            }
            /* pass */
            if (LModule_is_enum(m, inm)) {
                /* pass */
                TrStr nvm = _norm_variant(inm, method);
                /* pass */
                if ((LModule_enum_variant_index(m, inm, nvm) >= 0LL)) {
                    /* pass */
                    return _lower_enum_ctor(m, lf, inm, nvm, margs);
                }
                /* pass */
                _tr_str_release(nvm);
                return ({ TrStr _at_t2366 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(inm), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t2366, (-1LL), margs)); _tr_str_release(_at_t2366); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t2364;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr recv_cls = _recv_class(m, lf, obj);
        /* pass */
        if ((strcmp(_tr_strz(recv_cls), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr dmethod = _tr_str_retain(method);
            /* pass */
            if (LModule_is_class(m, recv_cls)) {
                /* pass */
                if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_index"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0)) && (strcmp(_tr_strz(LModule_resolve_method(m, recv_cls, _tr_str_lit("__getitem__"))), _tr_strz(_tr_str_lit(""))) != 0))) {
                    /* pass */
                    TrStr _strtmp_t2367 = _tr_str_lit("__getitem__");
                    _tr_str_release(dmethod);
                    dmethod = _strtmp_t2367;
                }
            }
            /* pass */
            TrStr mangled = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(recv_cls), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dmethod)); _tr_str_release(_cl); _cres; });
            /* pass */
            if (LModule_is_class(m, recv_cls)) {
                /* pass */
                TrStr _strtmp_t2368 = LModule_resolve_method(m, recv_cls, dmethod);
                _tr_str_release(mangled);
                mangled = _strtmp_t2368;
                /* pass */
                if ((strcmp(_tr_strz(mangled), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    long long gmix = _find_generic_method(m, recv_cls, method);
                    /* pass */
                    if ((gmix < 0LL)) {
                        /* pass */
                        _tr_str_release(recv_cls);
                        _tr_str_release(dmethod);
                        _tr_str_release(mangled);
                        return (-1LL);
                    }
                    /* pass */
                    long long gm_self = lower_expr(m, lf, obj);
                    /* pass */
                    if ((gm_self < 0LL)) {
                        /* pass */
                        _tr_str_release(recv_cls);
                        _tr_str_release(dmethod);
                        _tr_str_release(mangled);
                        return (-1LL);
                    }
                    /* pass */
                    List_i64* gm_tags = (void*)List_i64_new();
                    /* pass */
                    List_TrStr* gm_cls = (void*)List_TrStr_new();
                    /* pass */
                    List_i64* gm_regs = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(gm_tags, LFunc_vreg_type(lf, gm_self));
                    /* pass */
                    ({ TrStr _at_t2369 = (_own(recv_cls)); List_TrStr_append(gm_cls, _at_t2369); _tr_str_release(_at_t2369); });
                    /* pass */
                    List_i64_append(gm_regs, gm_self);
                    /* pass */
                    TrStr gm_name = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(recv_cls), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("__m"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    long long gmj = 0LL;
                    /* pass */
                    while ((gmj < margs->len)) {
                        /* pass */
                        long long gmv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, gmj)));
                        /* pass */
                        if ((gmv < 0LL)) {
                            /* pass */
                            _tr_str_release(recv_cls);
                            _tr_str_release(dmethod);
                            _tr_str_release(mangled);
                            List_i64_free(gm_tags);
                            List_TrStr_free(gm_cls);
                            List_i64_free(gm_regs);
                            _tr_str_release(gm_name);
                            return (-1LL);
                        }
                        /* pass */
                        long long gmt = LFunc_vreg_type(lf, gmv);
                        /* pass */
                        TrStr gmc = _tr_str_lit("");
                        /* pass */
                        if (((gmt == 10LL) || (gmt == 11LL))) {
                            /* pass */
                            TrStr _strtmp_t2370 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(margs, gmj)));
                            _tr_str_release(gmc);
                            gmc = _strtmp_t2370;
                            /* pass */
                            if ((strcmp(_tr_strz(gmc), _tr_strz(_tr_str_lit(""))) == 0)) {
                                /* pass */
                                _tr_str_release(recv_cls);
                                _tr_str_release(dmethod);
                                _tr_str_release(mangled);
                                List_i64_free(gm_tags);
                                List_TrStr_free(gm_cls);
                                List_i64_free(gm_regs);
                                _tr_str_release(gm_name);
                                _tr_str_release(gmc);
                                return (-1LL);
                            }
                        }
                        /* pass */
                        List_i64_append(gm_regs, gmv);
                        /* pass */
                        List_i64_append(gm_tags, gmt);
                        /* pass */
                        List_TrStr_append(gm_cls, gmc);
                        /* pass */
                        if ((gmj > 0LL)) {
                            /* pass */
                            TrStr _strtmp_t2371 = _tr_strx_concat(_tr_strz(gm_name), _tr_strz(_tr_str_lit("_")));
                            _tr_str_release(gm_name);
                            gm_name = _strtmp_t2371;
                        }
                        /* pass */
                        TrStr _strtmp_t2372 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gmt)))); TrStr _cres = _tr_strx_concat(_tr_strz(gm_name), _cr.data); _tr_str_release(_cr); _cres; });
                        _tr_str_release(gm_name);
                        gm_name = _strtmp_t2372;
                        /* pass */
                        if ((strcmp(_tr_strz(gmc), _tr_strz(_tr_str_lit(""))) != 0)) {
                            /* pass */
                            TrStr _strtmp_t2373 = _tr_strx_concat(_tr_strz(gm_name), _tr_strz(gmc));
                            _tr_str_release(gm_name);
                            gm_name = _strtmp_t2373;
                        }
                        /* pass */
                        gmj = (gmj + 1LL);
                        _tr_str_release(gmc);
                    }
                    /* pass */
                    if ((gm_regs->len > 6LL)) {
                        /* pass */
                        _tr_str_release(recv_cls);
                        _tr_str_release(dmethod);
                        _tr_str_release(mangled);
                        List_i64_free(gm_tags);
                        List_TrStr_free(gm_cls);
                        List_i64_free(gm_regs);
                        _tr_str_release(gm_name);
                        return (-1LL);
                    }
                    /* pass */
                    if ((!LModule_is_user_fn(m, gm_name))) {
                        /* pass */
                        if ((!_lir_lower_generic(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, gmix)), gm_tags, gm_cls, gm_name))) {
                            /* pass */
                            _tr_str_release(recv_cls);
                            _tr_str_release(dmethod);
                            _tr_str_release(mangled);
                            List_i64_free(gm_tags);
                            List_TrStr_free(gm_cls);
                            List_i64_free(gm_regs);
                            _tr_str_release(gm_name);
                            return (-1LL);
                        }
                    }
                    /* pass */
                    long long gm_ret = LModule_fn_ret_tag(m, gm_name);
                    /* pass */
                    long long gm_d = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(gm_d, gm_name, gm_regs));
                    /* pass */
                    LFunc_set_vreg_type(lf, gm_d, gm_ret);
                    /* pass */
                    if ((gm_ret == 1LL)) {
                        /* pass */
                        _fresh_mark(lf, gm_d);
                    }
                    /* pass */
                    _tr_str_release(recv_cls);
                    _tr_str_release(dmethod);
                    _tr_str_release(mangled);
                    _tr_str_release(gm_name);
                    return gm_d;
                }
            }
            /* pass */
            long long rself = lower_expr(m, lf, obj);
            /* pass */
            if ((rself < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(dmethod);
                _tr_str_release(mangled);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(dmethod);
            return _lower_obj_call(m, lf, mangled, rself, margs);
        }
        /* pass */
        long long ovm = lower_expr(m, lf, obj);
        /* pass */
        if ((ovm < 0LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            return (-1LL);
        }
        /* pass */
        long long ovmt = LFunc_vreg_type(lf, ovm);
        /* pass */
        if ((ovmt == 1LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            return _lower_str_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if ((ovmt == 5LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            return _lower_float_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if ((ovmt == 0LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            return _lower_int_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if (_is_dict_tag(ovmt)) {
            /* pass */
            _tr_str_release(recv_cls);
            return _lower_dict_method(m, lf, ovm, ovmt, method, margs);
        }
        /* pass */
        if (_is_set_tag(ovmt)) {
            /* pass */
            _tr_str_release(recv_cls);
            return _lower_set_method(m, lf, ovm, ovmt, method, margs);
        }
        /* pass */
        if ((((ovmt == 15LL) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_index"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0))) && (margs->len == 1LL))) {
            /* pass */
            long long tgt_idx = (-1LL);
            /* pass */
            __auto_type _t2374 = (*((HirExpr*)List_ptr_get(margs, 0LL)));
            if (_t2374.tag == HirExpr_ELitInt) {
                __auto_type tgt_v = _t2374.data.ELitInt.val;
                tgt_idx = tgt_v;
            } else if (1) {
                __auto_type _ = _t2374;
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            AstType* tup_hty = hir_expr_type(obj);
            /* pass */
            if (((tgt_idx < 0LL) || (tgt_idx >= tup_hty->args->len))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long tgt_tag = _tag_of(m, (*((AstType**)List_ptr_get(tup_hty->args, tgt_idx))));
            /* pass */
            if (((((tgt_tag < 0LL) || _is_list_tag(tgt_tag)) || _is_dict_tag(tgt_tag)) || _is_set_tag(tgt_tag))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            if ((tgt_tag == 5LL)) {
                /* pass */
                long long tgr = _emit_field_get(m, lf, ovm, (tgt_idx * 8LL), 0LL);
                /* pass */
                long long tgf = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(tgf, tgr));
                /* pass */
                LFunc_set_vreg_type(lf, tgf, 5LL);
                /* pass */
                _tr_str_release(recv_cls);
                return tgf;
            }
            /* pass */
            _tr_str_release(recv_cls);
            return _emit_field_get(m, lf, ovm, (tgt_idx * 8LL), tgt_tag);
        }
        /* pass */
        if ((!_is_list_tag(ovmt))) {
            /* pass */
            _tr_str_release(recv_cls);
            return (-1LL);
        }
        /* pass */
        long long want_elem = _list_elem_tag(ovmt);
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("push"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("append"))) == 0))) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((av < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, av) != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                _secure_str(m, lf, av);
            }
            /* pass */
            if ((want_elem == 5LL)) {
                /* pass */
                long long avfb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(avfb, av));
                /* pass */
                av = avfb;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_push_i64"));
            /* pass */
            List_i64* ppa = (void*)List_i64_new();
            /* pass */
            List_i64_append(ppa, ovm);
            /* pass */
            List_i64_append(ppa, av);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_push_i64"), ppa));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_index"))) == 0))) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long giv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((giv < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(recv_cls);
            return _list_get_elem(m, lf, ovmt, ovm, giv);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pop"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_pop_i64"));
            /* pass */
            List_i64* poa = (void*)List_i64_new();
            /* pass */
            List_i64_append(poa, ovm);
            /* pass */
            long long pod = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(pod, _tr_str_lit("_tr_rt_list_pop_i64"), poa));
            /* pass */
            LFunc_set_vreg_type(lf, pod, want_elem);
            /* pass */
            _tr_str_release(recv_cls);
            return pod;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("index_of"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            long long ixa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((ixa < 0LL) || (LFunc_vreg_type(lf, ixa) != want_elem))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            TrStr ixsym = _tr_str_lit("_tr_rt_list_index_i64");
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t2375 = _tr_str_lit("_tr_rt_list_index_str");
                _tr_str_release(ixsym);
                ixsym = _strtmp_t2375;
            }
            /* pass */
            LModule_add_extern(m, ixsym);
            /* pass */
            List_i64* ixargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(ixargs, ovm);
            /* pass */
            List_i64_append(ixargs, ixa);
            /* pass */
            long long ixd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ixd, ixsym, ixargs));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(ixsym);
            return ixd;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            long long cxa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((cxa < 0LL) || (LFunc_vreg_type(lf, cxa) != want_elem))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            TrStr cxsym = _tr_str_lit("_tr_rt_list_contains_i64");
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t2376 = _tr_str_lit("_tr_rt_list_contains_str");
                _tr_str_release(cxsym);
                cxsym = _strtmp_t2376;
            }
            /* pass */
            LModule_add_extern(m, cxsym);
            /* pass */
            List_i64* cxargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(cxargs, ovm);
            /* pass */
            List_i64_append(cxargs, cxa);
            /* pass */
            long long cxd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cxd, cxsym, cxargs));
            /* pass */
            LFunc_set_vreg_type(lf, cxd, 4LL);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(cxsym);
            return cxd;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("count"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            long long cta = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((cta < 0LL) || (LFunc_vreg_type(lf, cta) != want_elem))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            TrStr ctsym = _tr_str_lit("_tr_rt_list_count_i64");
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t2377 = _tr_str_lit("_tr_rt_list_count_str");
                _tr_str_release(ctsym);
                ctsym = _strtmp_t2377;
            }
            /* pass */
            LModule_add_extern(m, ctsym);
            /* pass */
            List_i64* ctargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(ctargs, ovm);
            /* pass */
            List_i64_append(ctargs, cta);
            /* pass */
            long long ctd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ctd, ctsym, ctargs));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(ctsym);
            return ctd;
        }
        /* pass */
        if ((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("min"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("min_val"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max_val"))) == 0)) && (margs->len == 0LL))) {
            /* pass */
            if ((want_elem != 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            TrStr mmsym = _tr_str_lit("_tr_rt_list_min_i64");
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max_val"))) == 0))) {
                /* pass */
                TrStr _strtmp_t2378 = _tr_str_lit("_tr_rt_list_max_i64");
                _tr_str_release(mmsym);
                mmsym = _strtmp_t2378;
            }
            /* pass */
            _tr_str_release(recv_cls);
            return _list_call1(m, lf, mmsym, ovm, 0LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sum"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            if ((want_elem != 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(recv_cls);
            return _list_call1(m, lf, _tr_str_lit("_tr_rt_list_sum_i64"), ovm, 0LL);
        }
        /* pass */
        if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("copy"))) == 0)) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_clone"));
            /* pass */
            List_i64* cla = (void*)List_i64_new();
            /* pass */
            List_i64_append(cla, ovm);
            /* pass */
            long long cld = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cld, _tr_str_lit("_tr_rt_list_clone"), cla));
            /* pass */
            LFunc_set_vreg_type(lf, cld, ovmt);
            /* pass */
            _tr_str_release(recv_cls);
            return cld;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            long long rmi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((rmi < 0LL) || (LFunc_vreg_type(lf, rmi) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_remove"));
            /* pass */
            List_i64* rma = (void*)List_i64_new();
            /* pass */
            List_i64_append(rma, ovm);
            /* pass */
            List_i64_append(rma, rmi);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_remove"), rma));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("swap"))) == 0) && (margs->len == 2LL))) {
            /* pass */
            long long swi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((swi < 0LL) || (LFunc_vreg_type(lf, swi) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long swj = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
            /* pass */
            if (((swj < 0LL) || (LFunc_vreg_type(lf, swj) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_swap"));
            /* pass */
            List_i64* swa = (void*)List_i64_new();
            /* pass */
            List_i64_append(swa, ovm);
            /* pass */
            List_i64_append(swa, swi);
            /* pass */
            List_i64_append(swa, swj);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_swap"), swa));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("join"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            if ((ovmt != 3LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long jsep = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((jsep < 0LL) || (LFunc_vreg_type(lf, jsep) != 1LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_join"));
            /* pass */
            List_i64* jargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(jargs, ovm);
            /* pass */
            List_i64_append(jargs, jsep);
            /* pass */
            long long jd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(jd, _tr_str_lit("_tr_rt_list_join"), jargs));
            /* pass */
            LFunc_set_vreg_type(lf, jd, 1LL);
            /* pass */
            _fresh_mark(lf, jd);
            /* pass */
            _tr_str_release(recv_cls);
            return jd;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            return _list_call1(m, lf, _tr_str_lit("_tr_rt_list_is_empty"), ovm, 4LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("first"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            return _list_call1(m, lf, _tr_str_lit("_tr_rt_list_first_i64"), ovm, want_elem);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("last"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            return _list_call1(m, lf, _tr_str_lit("_tr_rt_list_last_i64"), ovm, want_elem);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reverse"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_reverse"));
            /* pass */
            List_i64* rva = (void*)List_i64_new();
            /* pass */
            List_i64_append(rva, ovm);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_reverse"), rva));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("extend"))) == 0) && (margs->len == 1LL))) {
            /* pass */
            long long exo = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((exo < 0LL) || (LFunc_vreg_type(lf, exo) != ovmt))) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_extend"));
            /* pass */
            List_i64* exa = (void*)List_i64_new();
            /* pass */
            List_i64_append(exa, ovm);
            /* pass */
            List_i64_append(exa, exo);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_extend"), exa));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clear"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_clear"));
            /* pass */
            List_i64* cla = (void*)List_i64_new();
            /* pass */
            List_i64_append(cla, ovm);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_clear"), cla));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sort"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sort_asc"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sort_desc"))) == 0)) && (margs->len == 0LL))) {
            /* pass */
            if ((want_elem != 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                return (-1LL);
            }
            /* pass */
            long long dirv = LFunc_new_vreg(lf);
            /* pass */
            long long dir = 1LL;
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sort_desc"))) == 0)) {
                /* pass */
                dir = (0LL - 1LL);
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(dirv, dir));
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_sort"));
            /* pass */
            List_i64* soa = (void*)List_i64_new();
            /* pass */
            List_i64_append(soa, ovm);
            /* pass */
            List_i64_append(soa, dirv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_sort"), soa));
            /* pass */
            _tr_str_release(recv_cls);
            return ovm;
        }
        /* pass */
        _tr_str_release(recv_cls);
        return (-1LL);
    } else if (1) {
        __auto_type _ = _t2340;
        /* pass */
        return (-1LL);
    }
}

