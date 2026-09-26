#include "tauraro_types.h"

AstType** box_asttype_lir(AstType* t);
long long _f64_bits(double v);
long long _promote_f(LFunc* lf, long long v);
long long _int_cast_width(TrStr tn);
bool _int_cast_signed(TrStr tn);
long long _narrow_int(LFunc* lf, long long v, TrStr tn);
bool _is_int_cast_target(TrStr tn);
TrStr _print_i64_sym();
bool _is_list_tag(long long t);
long long _list_stride(long long ltag);
bool _is_set_tag(long long t);
TrStr _set_sym(long long t, TrStr op);
bool _is_dict_tag(long long t);
bool _dict_key_is_str(long long t);
long long _dict_val_tag(long long t);
TrStr _dict_new_sym(long long t);
TrStr _dict_sym(long long t, TrStr op);
long long _list_elem_tag(long long t);
long long _list_tag_for_elem(long long et);
long long _list_tag_from_ann(LModule* m, AstType* ty);
long long _dict_tag_from_ann(LModule* m, AstType* ty);
bool _is_cmp_op(TrStr op);
bool _is_int_typename(TrStr n);
long long _ast_type_tag(AstType* ty);
bool _is_null_str(TrStr s);
TrStr _own(TrStr s);
long long _tag_of(LModule* m, AstType* ty);
AstType* _subst_args(LModule* m, AstType* ty);
AstType* _subst_ty(LModule* m, AstType* ty);
long long _prog_generic_class_index(LModule* m, TrStr name);
bool _is_real_generic_class_arity_match(LModule* m, TrStr name, long long arg_count);
TrStr _mangle_generic(LModule* m, AstType* ty);
TrStr _ensure_generic_class(LModule* m, AstType* ty);
TrStr _val_obj_cls(LModule* m, LFunc* lf, HirExpr* val);
TrStr _cls_of_ty(LModule* m, AstType* ty);
TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj);
long long _lower_list_comp(LModule* m, LFunc* lf, HirExpr* element, List_ptr* generators, AstType* lty);
AstType* _hir_method_ret_ty(LModule* m, TrStr cls, TrStr method);
long long _prog_class_index(HirProgram* prog, TrStr name);
bool _push_field_names_rec(HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
bool _push_field_tags_rec(LModule* m, HirProgram* prog, long long ci, ClassLayout* lay, long long depth);
void _register_classes(LModule* m, HirProgram* prog);
bool _class_needs_drop(ClassLayout* lay);
bool _class_needs_drop_by_name(LModule* m, TrStr cname);
void _attach_class_drop(LModule* m, LFunc* lf, long long obj, TrStr cname);
void _build_class_drop(LModule* m, ClassLayout* lay);
void _gen_class_drops(LModule* m);
bool _method_in_prog_functions(HirProgram* prog, TrStr cls, TrStr name);
long long _method_nonself_pcount(HirFunction* f);
TrStr _ov_method_name(LModule* m, TrStr cls, HirFunction* f);
void _detect_overloads(LModule* m, HirProgram* prog);
long long _arg_limit(LModule* m);
bool _is_extern_c_int(TrStr n);
bool _is_extern_c_float(TrStr n);
long long _extern_sig_tag(LModule* m, AstType* ty);
LModule* _lower_to_lir_body(LModule* m, HirProgram* prog);
bool _fn_has_iface_param(LModule* m, HirFunction* f);
bool _fn_is_specializable(LModule* m, HirFunction* f);
long long _find_generic_fn(LModule* m, TrStr name);
long long _find_generic_method(LModule* m, TrStr cls, TrStr method);
bool _is_generic_param(HirFunction* f, TrStr n);
bool _param_is_abstract(LModule* m, HirFunction* f, TrStr ptyname);
bool _lir_lower_generic(LModule* m, HirFunction* f, List_i64* argtags, List_TrStr* argcls, TrStr mangled);
TrStr _mono_base(TrStr name);
TrStr _mono_concrete(TrStr name);
bool _lir_lower_mono_fn(LModule* m, HirFunction* f, TrStr mangled, AstType* concrete);
void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f);
bool _register_global(LModule* m, HirStmt* s);
bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s);
void _lir_lower_function(LModule* m, HirFunction* f);
bool _field_tag_ok(long long vt, long long ftg);
void _emit_field_set(LModule* m, LFunc* lf, long long obj, long long off, long long val);
long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag);
long long _lower_enum_ctor(LModule* m, LFunc* lf, TrStr ename, TrStr vname, List_ptr* margs);
long long _lir_fn_ret_tag(LModule* m, HirFunction* f);
long long _wrap_result(LModule* m, LFunc* lf, long long vidx, long long payv, long long paytag);
long long _find_free_fn_idx(LModule* m, TrStr name);
bool _ensure_await_wrapper(LModule* m, TrStr fn);
long long _lower_await(LModule* m, LFunc* lf, HirExpr* awexpr);
bool _lower_spawn(LModule* m, LFunc* lf, HirExpr* expr);
long long _box_call(LModule* m, LFunc* lf, long long selfv, TrStr rtfn, List_ptr* margs, long long nargs, long long rtag);
long long _pack_spawn_args(LModule* m, LFunc* lf, List_ptr* margs, long long start);
long long _lower_pool_spawn(LModule* m, LFunc* lf, long long poolv, List_ptr* margs);
bool _is_runtime_fn(TrStr fn);
bool _is_rc_str_runtime_fn(TrStr fn);
void _track_mutex_unlock(LFunc* lf, HirExpr* obj);
long long _atomic_delta(LModule* m, LFunc* lf, long long selfv, TrStr rtfn);
long long _lower_box_method(LModule* m, LFunc* lf, HirExpr* obj, TrStr method, List_ptr* margs);
bool _lower_taskgroup(LModule* m, LFunc* lf, HirBlock* body);
long long _lower_obj_call(LModule* m, LFunc* lf, TrStr mangled, long long self_vreg, List_ptr* margs);
bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
bool _run_defers(LModule* m, LFunc* lf);
long long _sizeof_tyname(TrStr n);
long long _ptr_stride(LModule* m, AstType* pty);
AstType* _class_field_ty(LModule* m, TrStr cls, TrStr fld);
AstType* _ptr_chain_ty(LModule* m, HirExpr* e);
TrStr _dunder_for_op(TrStr op);
TrStr _stmt_expr_kind(HirExpr* e);
TrStr _expr_kind(HirExpr* e);
TrStr _stmt_kind(HirStmt* s);
bool _stmt_has_cf(HirStmt* s);
bool _block_has_cf(HirBlock* b);
bool _lower_try_setjmp(LModule* m, LFunc* lf, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b);
bool _is_vstruct_lvalue(HirExpr* e);
long long _copy_value_struct(LModule* m, LFunc* lf, long long src, TrStr cls);
long long _value_struct_generic_size(LModule* m, AstType* ty);
bool _lower_stmt_impl(LModule* m, LFunc* lf, HirStmt* s);
long long _lower_set_method(LModule* m, LFunc* lf, long long shv, long long stag, TrStr method, List_ptr* margs);
long long _lit_pat_cond(LModule* m, LFunc* lf, Pattern pat, long long subj, long long st);
bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms);
TrStr _norm_variant(TrStr ename, TrStr vn);
long long _variant_tag_cond(LFunc* lf, long long tagv, long long vidx);
long long _load_enum_payload_field(LModule* m, LFunc* lf, long long subj, AstType* subj_ty, VariantLayout* vlay, long long fldidx);
long long _lower_enum_prop(LModule* m, LFunc* lf, HirExpr* obj, TrStr ename, TrStr prop);
bool _bind_payload(LModule* m, LFunc* lf, VariantLayout* vlay, long long subj, AstType* subj_ty, long long fldidx, TrStr bindname);
bool _lower_match_enum(LModule* m, LFunc* lf, HirExpr* expr, long long subj, List_ptr* arms);
bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _sel_recv_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm);
bool _sel_send_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm);
bool _sel_uncond_arm(LModule* m, LFunc* lf, TrStr donev, HirBlock* body);
bool _sel_timeout_arm(LModule* m, LFunc* lf, TrStr donev, TrStr tstart, HirChanSelectArm* arm);
bool _lower_chan_select(LModule* m, LFunc* lf, List_ptr* cases);
bool _lower_for_chan(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_iterproto(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_erange(LModule* m, LFunc* lf, TrStr var, HirExpr* start, HirExpr* end, bool inclusive, HirBlock* body);
bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body);
bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_unpack(LModule* m, LFunc* lf, List_TrStr* vars, HirExpr* iter, HirBlock* body);
bool _lower_enumerate(LModule* m, LFunc* lf, TrStr ivar, TrStr evar, HirExpr* listexpr, HirBlock* body);
bool _lower_zip(LModule* m, LFunc* lf, TrStr v0, TrStr v1, HirExpr* aexpr, HirExpr* bexpr, HirBlock* body);
bool _lower_dict_items_unpack(LModule* m, LFunc* lf, TrStr kvar, TrStr vvar, HirExpr* dictexpr, HirBlock* body);
void _emit_incr(LFunc* lf, TrStr name);
TrStr _ident_name(HirExpr* e);
bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val);
bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val);
TrStr _write_sym(long long t);
void _emit_call0(LModule* m, LFunc* lf, TrStr sym);
bool _lower_print(LModule* m, LFunc* lf, List_ptr* args);
bool _lower_assert_cmp(LModule* m, LFunc* lf, TrStr fname, List_ptr* args);
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
void _fresh_mark_obj(LFunc* lf, long long v);
bool _fresh_take_obj(LFunc* lf, long long v);
void _release_obj(LModule* m, LFunc* lf, long long v);
void _retain_obj(LModule* m, LFunc* lf, long long v);
void _flush_fresh_objs(LModule* m, LFunc* lf);
bool _is_owned_local_return(LFunc* lf, HirExpr* val);
void _secure_obj(LModule* m, LFunc* lf, long long v);
bool _is_param(LFunc* lf, TrStr name);
long long _norm_bool(LFunc* lf, long long v);
long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype);
long long _heap_lit(LModule* m, LFunc* lf, TrStr s);
long long _heap_lit_len(LModule* m, LFunc* lf, TrStr s, long long blen);
long long _obj_to_str(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg);
long long _obj_repr(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg);
long long _reg_to_str(LModule* m, LFunc* lf, long long reg);
long long _str_concat2(LModule* m, LFunc* lf, long long a, long long b);
long long _tuple_to_str(LModule* m, LFunc* lf, long long tup, AstType* ty);
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
long long _inline_list_addr(LModule* m, LFunc* lf, long long handle, long long idx, long long stride);
long long _fixed_arr_len(LModule* m, LFunc* lf, TrStr name);
long long _fixed_arr_elem_tag(LModule* m, LFunc* lf, TrStr name);
long long _array_elem_addr(LModule* m, LFunc* lf, TrStr name, long long idx);
long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx);
long long _list_get_raw(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx);
long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx);
long long _lower_expr_impl(LModule* m, LFunc* lf, HirExpr* e);

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

__attribute__((hot)) long long _int_cast_width(TrStr tn) {
    /* pass */
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL))))) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL))))) {
        /* pass */
        return 16LL;
    }
    /* pass */
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL))))) {
        /* pass */
        return 32LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) bool _int_cast_signed(TrStr tn) {
    /* pass */
    return ((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL))));
}

__attribute__((hot)) long long _narrow_int(LFunc* lf, long long v, TrStr tn) {
    /* pass */
    long long bits = _int_cast_width(tn);
    /* pass */
    if ((bits == 0LL)) {
        /* pass */
        return v;
    }
    /* pass */
    long long maskc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(maskc, ((1LL << bits) - 1LL)));
    /* pass */
    long long masked = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(masked, _tr_str_lit_len("&", 1LL), v, maskc));
    /* pass */
    LFunc_set_vreg_type(lf, masked, 0LL);
    /* pass */
    if ((!_int_cast_signed(tn))) {
        /* pass */
        return masked;
    }
    /* pass */
    long long sbc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(sbc, (1LL << (bits - 1LL))));
    /* pass */
    long long xored = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(xored, _tr_str_lit_len("^", 1LL), masked, sbc));
    /* pass */
    long long res = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(res, _tr_str_lit_len("-", 1LL), xored, sbc));
    /* pass */
    LFunc_set_vreg_type(lf, res, 0LL);
    /* pass */
    return res;
}

__attribute__((hot)) bool _is_int_cast_target(TrStr tn) {
    /* pass */
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("isize", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv((tn), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _print_i64_sym() {
    /* pass */
    return _tr_str_lit_len("_tr_rt_print_i64", 16LL);
}

__attribute__((hot)) bool _is_list_tag(long long t) {
    /* pass */
    return ((((((t == 2LL) || (t == 3LL)) || (t == 14LL)) || (t == 19LL)) || (t == 21LL)) || (t == 22LL));
}

__attribute__((hot)) long long _list_stride(long long ltag) {
    /* pass */
    if ((ltag == 22LL)) {
        /* pass */
        return 1LL;
    }
    /* pass */
    return 8LL;
}

__attribute__((hot)) bool _is_set_tag(long long t) {
    /* pass */
    return ((t == 13LL) || (t == 16LL));
}

__attribute__((hot)) TrStr _set_sym(long long t, TrStr op) {
    /* pass */
    if ((t == 16LL)) {
        /* pass */
        return _tr_strx_concatv((_tr_str_lit_len("_tr_rt_sdict_", 13LL)), (op));
    }
    /* pass */
    return _tr_strx_concatv((_tr_str_lit_len("_tr_rt_idict_", 13LL)), (op));
}

__attribute__((hot)) bool _is_dict_tag(long long t) {
    /* pass */
    return ((((((((((t == 6LL) || (t == 7LL)) || (t == 8LL)) || (t == 9LL)) || (t == 17LL)) || (t == 18LL)) || (t == 24LL)) || (t == 25LL)) || (t == 26LL)) || (t == 27LL));
}

__attribute__((hot)) bool _dict_key_is_str(long long t) {
    /* pass */
    return (((((t == 6LL) || (t == 8LL)) || (t == 17LL)) || (t == 24LL)) || (t == 26LL));
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
    if (((t == 24LL) || (t == 25LL))) {
        /* pass */
        return 10LL;
    }
    /* pass */
    if (((t == 26LL) || (t == 27LL))) {
        /* pass */
        return 4LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) TrStr _dict_new_sym(long long t) {
    /* pass */
    if (_dict_key_is_str(t)) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_sdict_new", 16LL);
    }
    /* pass */
    return _tr_str_lit_len("_tr_rt_idict_new", 16LL);
}

__attribute__((hot)) TrStr _dict_sym(long long t, TrStr op) {
    /* pass */
    TrStr pfx = _tr_str_lit_len("_tr_rt_idict_", 13LL);
    /* pass */
    if (_dict_key_is_str(t)) {
        /* pass */
        TrStr _strtmp_t3170 = _tr_str_lit_len("_tr_rt_sdict_", 13LL);
        _tr_str_release(pfx);
        pfx = _strtmp_t3170;
    }
    /* pass */
    return _tr_strx_concatv((pfx), (op));
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
    if ((t == 21LL)) {
        /* pass */
        return 15LL;
    }
    /* pass */
    if ((t == 22LL)) {
        /* pass */
        return 4LL;
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
    if ((et == 15LL)) {
        /* pass */
        return 21LL;
    }
    /* pass */
    if ((et == 4LL)) {
        /* pass */
        return 22LL;
    }
    /* pass */
    return 2LL;
}

__attribute__((hot)) long long _list_tag_from_ann(LModule* m, AstType* ty) {
    /* pass */
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long t = _tag_of(m, ty);
    /* pass */
    if ((((((t == 2LL) || (t == 3LL)) || (t == 14LL)) || (t == 19LL)) || (t == 21LL))) {
        /* pass */
        return t;
    }
    /* pass */
    if ((ty->args->len > 0LL)) {
        /* pass */
        TrStr en = _tr_str_retain((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
        /* pass */
        if (_tr_str_eqv((en), (_tr_str_lit_len("bool", 4LL)))) {
            /* pass */
            _tr_str_release(en);
            return 2LL;
        }
        /* pass */
        if ((_tr_str_eqv((en), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((en), (_tr_str_lit_len("tuple", 5LL))))) {
            /* pass */
            _tr_str_release(en);
            return 21LL;
        }
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _dict_tag_from_ann(LModule* m, AstType* ty) {
    /* pass */
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if ((ty->args->len != 2LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    AstType* kt = (*((AstType**)List_ptr_get(ty->args, 0LL)));
    /* pass */
    AstType* vt = (*((AstType**)List_ptr_get(ty->args, 1LL)));
    /* pass */
    bool kstr = (_tr_str_eqv((kt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((kt->name), (_tr_str_lit_len("String", 6LL))));
    /* pass */
    bool kint = _is_int_typename(kt->name);
    /* pass */
    bool vstr = (_tr_str_eqv((vt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("String", 6LL))));
    /* pass */
    bool vint = _is_int_typename(vt->name);
    /* pass */
    bool vflt = (_tr_str_eqv((vt->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("f64", 3LL))));
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
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_cmp_op(TrStr op) {
    /* pass */
    return (((((_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))));
}

__attribute__((hot)) bool _is_int_typename(TrStr n) {
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long _ast_type_tag(AstType* ty) {
    /* pass */
    TrStr n = _tr_str_retain(ty->name);
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("String", 6LL))))) {
        /* pass */
        _tr_str_release(n);
        return 1LL;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        _tr_str_release(n);
        return 4LL;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* et = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if ((_tr_str_eqv((et->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((et->name), (_tr_str_lit_len("String", 6LL))))) {
                /* pass */
                _tr_str_release(n);
                return 3LL;
            }
            /* pass */
            if (_is_int_typename(et->name)) {
                /* pass */
                _tr_str_release(n);
                return 2LL;
            }
            /* pass */
            if (_tr_str_eqv((et->name), (_tr_str_lit_len("bool", 4LL)))) {
                /* pass */
                _tr_str_release(n);
                return 22LL;
            }
            /* pass */
            if ((_tr_str_eqv((et->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((et->name), (_tr_str_lit_len("f64", 3LL))))) {
                /* pass */
                _tr_str_release(n);
                return 14LL;
            }
            /* pass */
            _tr_str_release(n);
            return (-1LL);
        }
        /* pass */
        _tr_str_release(n);
        return 2LL;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* se = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if ((_tr_str_eqv((se->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((se->name), (_tr_str_lit_len("String", 6LL))))) {
                /* pass */
                _tr_str_release(n);
                return 16LL;
            }
            /* pass */
            if (_is_int_typename(se->name)) {
                /* pass */
                _tr_str_release(n);
                return 13LL;
            }
        }
        /* pass */
        _tr_str_release(n);
        return (-1LL);
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL)))) {
        /* pass */
        _tr_str_release(n);
        return 15LL;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("def", 3LL)))) {
        /* pass */
        _tr_str_release(n);
        return 0LL;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
        /* pass */
        if ((ty->args->len >= 2LL)) {
            /* pass */
            AstType* kt = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            AstType* vt = (*((AstType**)List_ptr_get(ty->args, 1LL)));
            /* pass */
            bool kstr = (_tr_str_eqv((kt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((kt->name), (_tr_str_lit_len("String", 6LL))));
            /* pass */
            bool kint = _is_int_typename(kt->name);
            /* pass */
            bool vstr = (_tr_str_eqv((vt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("String", 6LL))));
            /* pass */
            bool vint = _is_int_typename(vt->name);
            /* pass */
            bool vflt = (_tr_str_eqv((vt->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("f64", 3LL))));
            /* pass */
            bool vbool = _tr_str_eqv((vt->name), (_tr_str_lit_len("bool", 4LL)));
            /* pass */
            if (_tr_str_eqv((vt->name), (_tr_str_lit_len("Pointer", 7LL)))) {
                /* pass */
                vint = true;
            }
            /* pass */
            if ((kstr && vint)) {
                /* pass */
                _tr_str_release(n);
                return 6LL;
            }
            /* pass */
            if ((kint && vint)) {
                /* pass */
                _tr_str_release(n);
                return 7LL;
            }
            /* pass */
            if ((kstr && vstr)) {
                /* pass */
                _tr_str_release(n);
                return 8LL;
            }
            /* pass */
            if ((kint && vstr)) {
                /* pass */
                _tr_str_release(n);
                return 9LL;
            }
            /* pass */
            if ((kstr && vflt)) {
                /* pass */
                _tr_str_release(n);
                return 17LL;
            }
            /* pass */
            if ((kint && vflt)) {
                /* pass */
                _tr_str_release(n);
                return 18LL;
            }
            /* pass */
            if ((kstr && vbool)) {
                /* pass */
                _tr_str_release(n);
                return 26LL;
            }
            /* pass */
            if ((kint && vbool)) {
                /* pass */
                _tr_str_release(n);
                return 27LL;
            }
        }
        /* pass */
        _tr_str_release(n);
        return (-1LL);
    }
    /* pass */
    if (_is_int_typename(n)) {
        /* pass */
        _tr_str_release(n);
        return 0LL;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) {
        /* pass */
        _tr_str_release(n);
        return 0LL;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
        /* pass */
        _tr_str_release(n);
        return 5LL;
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL))))) {
        /* pass */
        _tr_str_release(n);
        return 0LL;
    }
    /* pass */
    _tr_str_release(n);
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
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_strx_concatv((s), (_tr_str_lit_len("", 0LL)));
}

__attribute__((hot)) long long _tag_of(LModule* m, AstType* ty) {
    /* pass */
    if (_is_null_str(ty->name)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    if (((((((_tr_str_eqv((ty->name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long sbi = 0LL;
    /* pass */
    while ((sbi < m->subst_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(m->subst_names, sbi)), (ty->name))) {
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
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))) && (ty->args->len >= 2LL))) {
        /* pass */
        AstType* dkk = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        /* pass */
        AstType* dvv = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 1LL))));
        /* pass */
        if (((!_is_null_str(dvv->name)) && (LModule_is_class(m, dvv->name) || LModule_is_enum(m, dvv->name)))) {
            /* pass */
            if ((_tr_str_eqv((dkk->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((dkk->name), (_tr_str_lit_len("String", 6LL))))) {
                /* pass */
                return 24LL;
            }
            /* pass */
            if (_is_int_typename(dkk->name)) {
                /* pass */
                return 25LL;
            }
        }
        /* pass */
        long long _dtg = _ast_type_tag(_subst_args(m, ty));
        /* pass */
        if (_is_dict_tag(_dtg)) {
            /* pass */
            return _dtg;
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((ty->args->len > 0LL) && (_prog_generic_class_index(m, ty->name) >= 0LL))) {
        /* pass */
        if ((!_tr_str_eqv((_ensure_generic_class(m, ty)), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            return 10LL;
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (ty->args->len > 0LL))) {
        /* pass */
        AstType* lelem = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        /* pass */
        if (((!_is_null_str(lelem->name)) && LModule_is_class(m, lelem->name))) {
            /* pass */
            return 19LL;
        }
    }
    /* pass */
    if (((ty->args->len > 0LL) && (m->subst_names->len > 0LL))) {
        /* pass */
        return _ast_type_tag(_subst_args(m, ty));
    }
    /* pass */
    return _ast_type_tag(ty);
}

__attribute__((hot)) AstType* _subst_args(LModule* m, AstType* ty) {
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return ty;
    }
    /* pass */
    AstType* r = ({ TrStr _at_t3171 = (_own(ty->name)); __auto_type _wr = (AstType_init(_at_t3171)); _tr_str_release(_at_t3171); _wr; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        List_ptr_append(r->args, box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(ty->args, i))))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return r;
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
        if (_tr_str_eqv((List_TrStr_get(m->subst_names, sbi)), (ty->name))) {
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
        if ((_tr_str_eqv((c->name), (name)) && (c->generics->len > 0LL))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_real_generic_class_arity_match(LModule* m, TrStr name, long long arg_count) {
    /* pass */
    return false;
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
        TrStr _strtmp_t3172 = ({ TrStr _cl = (_tr_strx_concatv((n), (_tr_str_lit_len("__g", 3LL)))); TrStr _cr = (_own(at->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(n);
        n = _strtmp_t3172;
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
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, gci));
    /* pass */
    if ((c->generics->len != ty->args->len)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    if ((c->base_classes->len > 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
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
        ({ TrStr _at_t3173 = (List_TrStr_get(c->generics, sb)); TrStr _at_t3174 = (_own(_at_t3173)); List_TrStr_append(m->subst_names, _at_t3174); _tr_str_release(_at_t3173); _tr_str_release(_at_t3174); });
        /* pass */
        List_ptr_append(m->subst_tys, box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(ty->args, sb))))));
        /* pass */
        sb = (sb + 1LL);
    }
    /* pass */
    ClassLayout* lay = ({ TrStr _at_t3175 = (_own(mangled)); __auto_type _wr = (ClassLayout_init(_at_t3175)); _tr_str_release(_at_t3175); _wr; });
    /* pass */
    lay->is_value = (!c->is_class);
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
        ({ TrStr _at_t3176 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _at_t3176); _tr_str_release(_at_t3176); });
        /* pass */
        List_i64_append(lay->ftags, ftg);
        /* pass */
        ({ TrStr _at_t3177 = (_cls_of_ty(m, _subst_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty))); List_TrStr_append(lay->fcls, _at_t3177); _tr_str_release(_at_t3177); });
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
            if (((_tr_str_eqv((pf->class_name), (ty->name)) && (!pf->is_extern)) && (pf->generics->len == 0LL))) {
                /* pass */
                TrStr pfmang = ({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(pf->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                long long pfrt = _tag_of(m, pf->ret_ty);
                /* pass */
                List_TrStr_append(m->fn_names, pfmang);
                /* pass */
                List_i64_append(m->fn_ret, pfrt);
                /* pass */
                if ((pf->returns_owned && ((pfrt == 10LL) || (pfrt == 11LL)))) {
                    /* pass */
                    LModule_mark_fn_owned(m, pfmang);
                }
            }
            /* pass */
            prereg = (prereg + 1LL);
        }
        /* pass */
        long long prereg2 = 0LL;
        /* pass */
        while ((prereg2 < c->methods->len)) {
            /* pass */
            HirFunction* pfm = ((HirFunction*)List_ptr_get(c->methods, prereg2));
            /* pass */
            if ((((!pfm->is_extern) && (pfm->generics->len == 0LL)) && (!_method_in_prog_functions(m->hir_prog, ty->name, pfm->name)))) {
                /* pass */
                TrStr pfmang2 = ({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(pfm->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                long long pfrt2 = _tag_of(m, pfm->ret_ty);
                /* pass */
                List_TrStr_append(m->fn_names, pfmang2);
                /* pass */
                List_i64_append(m->fn_ret, pfrt2);
                /* pass */
                if ((pfm->returns_owned && ((pfrt2 == 10LL) || (pfrt2 == 11LL)))) {
                    /* pass */
                    LModule_mark_fn_owned(m, pfmang2);
                }
            }
            /* pass */
            prereg2 = (prereg2 + 1LL);
        }
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < m->hir_prog->functions->len)) {
            /* pass */
            HirFunction* mf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, mi));
            /* pass */
            if (((_tr_str_eqv((mf->class_name), (ty->name)) && (!mf->is_extern)) && (mf->generics->len == 0LL))) {
                /* pass */
                _lir_lower_method(m, mangled, mf);
                /* pass */
                if ((!m->ok)) {
                    /* pass */
                    ({ TrStr _at_t3178 = (({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(mf->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(m->unavail_names, _at_t3178); _tr_str_release(_at_t3178); });
                    /* pass */
                    ({ TrStr _at_t3179 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t3179); _tr_str_release(_at_t3179); });
                    /* pass */
                    m->ok = true;
                    /* pass */
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        long long mi2 = 0LL;
        /* pass */
        while ((mi2 < c->methods->len)) {
            /* pass */
            HirFunction* mfm = ((HirFunction*)List_ptr_get(c->methods, mi2));
            /* pass */
            if ((((!mfm->is_extern) && (mfm->generics->len == 0LL)) && (!_method_in_prog_functions(m->hir_prog, ty->name, mfm->name)))) {
                /* pass */
                _lir_lower_method(m, mangled, mfm);
                /* pass */
                if ((!m->ok)) {
                    /* pass */
                    ({ TrStr _at_t3180 = (({ TrStr _cl = (({ TrStr _cl = (_own(mangled)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(mfm->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(m->unavail_names, _at_t3180); _tr_str_release(_at_t3180); });
                    /* pass */
                    ({ TrStr _at_t3181 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t3181); _tr_str_release(_at_t3181); });
                    /* pass */
                    m->ok = true;
                    /* pass */
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
            /* pass */
            mi2 = (mi2 + 1LL);
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
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    _tr_obj_release(lay, _trdrop_ClassLayout);
    return mangled;
}

__attribute__((hot)) TrStr _val_obj_cls(LModule* m, LFunc* lf, HirExpr* val) {
    /* pass */
    AstType* t = hir_expr_type(val);
    /* pass */
    if (_is_null_str(t->name)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    if ((LModule_is_class(m, t->name) || LModule_is_enum(m, t->name))) {
        /* pass */
        return _own(t->name);
    }
    /* pass */
    if ((_prog_generic_class_index(m, t->name) < 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    AstType* ct = ({ TrStr _at_t3182 = (_own(t->name)); __auto_type _wr = (AstType_init(_at_t3182)); _tr_str_release(_at_t3182); _wr; });
    /* pass */
    if ((t->args->len > 0LL)) {
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < t->args->len)) {
            /* pass */
            List_ptr_append(ct->args, box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(t->args, ai))))));
            /* pass */
            ai = (ai + 1LL);
        }
    } else if ((m->subst_names->len > 0LL)) {
        /* pass */
        HirClass* gc = ((HirClass*)List_ptr_get(m->hir_prog->classes, _prog_generic_class_index(m, t->name)));
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < gc->generics->len)) {
            /* pass */
            ({ TrStr _at_t3183 = (List_TrStr_get(gc->generics, gi)); TrStr _at_t3184 = (_own(_at_t3183)); List_ptr_append(ct->args, box_asttype_lir(_subst_ty(m, AstType_init(_at_t3184)))); _tr_str_release(_at_t3183); _tr_str_release(_at_t3184); });
            /* pass */
            gi = (gi + 1LL);
        }
    }
    /* pass */
    if ((ct->args->len == 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _ensure_generic_class(m, ct);
}

__attribute__((hot)) TrStr _cls_of_ty(LModule* m, AstType* ty) {
    /* pass */
    if (_is_null_str(ty->name)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
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
        if ((!_tr_str_eqv((gm), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            return gm;
        }
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj) {
    /* pass */
    TrStr cn = _tr_str_retain(hir_expr_type(obj)->name);
    /* pass */
    if ((_tr_str_eqv((cn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((cn), (_tr_str_lit_len("Map", 3LL))))) {
        /* pass */
        _tr_str_release(cn);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    if (((!_is_null_str(cn)) && (LModule_is_class(m, cn) || LModule_is_enum(m, cn)))) {
        /* pass */
        return cn;
    }
    /* pass */
    if ((((!_is_null_str(cn)) && (hir_expr_type(obj)->args->len > 0LL)) && (_prog_generic_class_index(m, cn) >= 0LL))) {
        /* pass */
        TrStr grc = _ensure_generic_class(m, hir_expr_type(obj));
        /* pass */
        if ((!_tr_str_eqv((grc), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            _tr_str_release(cn);
            return grc;
        }
    }
    /* pass */
    __auto_type _t3185 = (*obj);
    if (_t3185.tag == HirExpr_EIdent) {
        __auto_type nm = _t3185.data.EIdent.name;
        /* pass */
        TrStr vc = LFunc_var_cls_of(lf, nm);
        /* pass */
        if (((!_is_null_str(vc)) && (LModule_is_class(m, vc) || LModule_is_enum(m, vc)))) {
            /* pass */
            _tr_str_release(cn);
            return vc;
        }
        _tr_str_release(vc);
    } else if (_t3185.tag == HirExpr_EPropAccess) {
        __auto_type inner = _t3185.data.EPropAccess.obj;
__auto_type prop = _t3185.data.EPropAccess.prop;
        /* pass */
        TrStr icls = _recv_class(m, lf, inner);
        /* pass */
        if (((!_tr_str_eqv((icls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, icls))) {
            /* pass */
            TrStr fc = LModule_field_cls(m, icls, prop);
            /* pass */
            if (((!_is_null_str(fc)) && (!_tr_str_eqv((fc), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                _tr_str_release(icls);
                _tr_str_release(cn);
                return fc;
            }
        }
        _tr_str_release(icls);
    } else if (1) {
        __auto_type _ = _t3185;
        /* pass */
        /* pass */
    }
    /* pass */
    _tr_str_release(cn);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _lower_list_comp(LModule* m, LFunc* lf, HirExpr* element, List_ptr* generators, AstType* lty) {
    /* pass */
    if ((generators->len != 1LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, 0LL)));
    /* pass */
    if (gen->is_async) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
    /* pass */
    long long hv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
    /* pass */
    long long ltag = _tag_of(m, lty);
    /* pass */
    if ((!_is_list_tag(ltag))) {
        /* pass */
        long long et = _tag_of(m, hir_expr_type(element));
        /* pass */
        if ((et == 4LL)) {
            /* pass */
            et = 0LL;
        }
        /* pass */
        ltag = _list_tag_for_elem(et);
        /* pass */
        if ((!_is_list_tag(ltag))) {
            /* pass */
            return (-1LL);
        }
    }
    /* pass */
    LFunc_set_vreg_type(lf, hv, ltag);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr lcname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__lc", 4LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, lcname);
    /* pass */
    LFunc_set_var_type(lf, lcname, ltag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(lcname, hv));
    /* pass */
    HirExpr* _tr_v_recv = ({ TrStr _at_t3186 = (_own(lcname)); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t3186, lty, false))); _tr_str_release(_at_t3186); _wr; });
    /* pass */
    List_ptr* aargs = (void*)List_ptr_new();
    /* pass */
    List_ptr_append(aargs, element);
    /* pass */
    HirStmt* appstmt = box_hirstmt(HirStmt_ctor_SExpr(box_hirexpr(HirExpr_ctor_EMethodCall(_tr_v_recv, _tr_str_lit_len("append", 6LL), aargs, AstType_init(_tr_str_lit_len("void", 4LL))))));
    /* pass */
    HirBlock* body = HirBlock_init();
    /* pass */
    if ((gen->ifs->len == 0LL)) {
        /* pass */
        List_ptr_append(body->stmts, appstmt);
    } else {
        /* pass */
        HirExpr* cond = ((HirExpr*)List_ptr_get(gen->ifs, 0LL));
        /* pass */
        long long ci = 1LL;
        /* pass */
        while ((ci < gen->ifs->len)) {
            /* pass */
            cond = box_hirexpr(HirExpr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), cond, ((HirExpr*)List_ptr_get(gen->ifs, ci)), AstType_init(_tr_str_lit_len("bool", 4LL))));
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        HirBlock* thenb = HirBlock_init();
        /* pass */
        List_ptr_append(thenb->stmts, appstmt);
        /* pass */
        ({ HirBlock* _aot_t3187 = (HirBlock_init()); List_ptr_append(body->stmts, box_hirstmt(HirStmt_ctor_SIf(cond, thenb, _aot_t3187))); _tr_obj_release(_aot_t3187, _trdrop_HirBlock); });
    }
    /* pass */
    if (({ TrStr _at_t3188 = (_own(gen->target)); __auto_type _wr = ((!_lower_for(m, lf, _at_t3188, gen->iter, body))); _tr_str_release(_at_t3188); _wr; })) {
        /* pass */
        _tr_str_release(lcname);
        _tr_obj_release(body, _trdrop_HirBlock);
        return (-1LL);
    }
    /* pass */
    long long res = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(res, lcname));
    /* pass */
    LFunc_set_vreg_type(lf, res, ltag);
    /* pass */
    _tr_str_release(lcname);
    _tr_obj_release(body, _trdrop_HirBlock);
    return res;
}

__attribute__((hot)) AstType* _hir_method_ret_ty(LModule* m, TrStr cls, TrStr method) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->hir_prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
        /* pass */
        if (((_tr_str_eqv((f->class_name), (cls)) && _tr_str_eqv((f->name), (method))) && (!f->is_extern))) {
            /* pass */
            return f->ret_ty;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < m->hir_prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, ci));
        /* pass */
        if (_tr_str_eqv((c->name), (cls))) {
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                if (_tr_str_eqv((((HirFunction*)List_ptr_get(c->methods, mi))->name), (method))) {
                    /* pass */
                    return ((HirFunction*)List_ptr_get(c->methods, mi))->ret_ty;
                }
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    return AstType_init(_tr_str_lit_len("", 0LL));
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
        if (_tr_str_eqv((((HirClass*)List_ptr_get(prog->classes, i))->name), (name))) {
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
        long long bi = ({ TrStr _at_t3189 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t3189)); _tr_str_release(_at_t3189); _wr; });
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
        ({ TrStr _at_t3190 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _at_t3190); _tr_str_release(_at_t3190); });
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
        long long bi = ({ TrStr _at_t3191 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t3191)); _tr_str_release(_at_t3191); _wr; });
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
        ({ TrStr _at_t3192 = (_cls_of_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty)); List_TrStr_append(lay->fcls, _at_t3192); _tr_str_release(_at_t3192); });
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) void _register_classes(LModule* m, HirProgram* prog) {
    /* pass */
    EnumLayout* opt = EnumLayout_init(_tr_str_lit_len("Option", 6LL));
    /* pass */
    VariantLayout* opt_some = VariantLayout_init(_tr_str_lit_len("Some", 4LL));
    /* pass */
    List_TrStr_append(opt_some->fields, _tr_str_lit_len("val", 3LL));
    /* pass */
    List_i64_append(opt_some->ftags, (0LL - 2LL));
    /* pass */
    List_TrStr_append(opt_some->fcls, _tr_str_lit_len("", 0LL));
    /* pass */
    List_ptr_append(opt->variants, _tr_obj_retain(opt_some));
    /* pass */
    List_ptr_append(opt->variants, VariantLayout_init(_tr_str_lit_len("None", 4LL)));
    /* pass */
    LModule_add_enum(m, opt);
    /* pass */
    EnumLayout* res = EnumLayout_init(_tr_str_lit_len("Result", 6LL));
    /* pass */
    VariantLayout* res_ok = VariantLayout_init(_tr_str_lit_len("Ok", 2LL));
    /* pass */
    List_TrStr_append(res_ok->fields, _tr_str_lit_len("val", 3LL));
    /* pass */
    List_i64_append(res_ok->ftags, (0LL - 2LL));
    /* pass */
    List_TrStr_append(res_ok->fcls, _tr_str_lit_len("", 0LL));
    /* pass */
    List_ptr_append(res->variants, _tr_obj_retain(res_ok));
    /* pass */
    VariantLayout* res_err = VariantLayout_init(_tr_str_lit_len("Err", 3LL));
    /* pass */
    List_TrStr_append(res_err->fields, _tr_str_lit_len("err", 3LL));
    /* pass */
    List_i64_append(res_err->ftags, (0LL - 3LL));
    /* pass */
    List_TrStr_append(res_err->fcls, _tr_str_lit_len("", 0LL));
    /* pass */
    List_ptr_append(res->variants, _tr_obj_retain(res_err));
    /* pass */
    LModule_add_enum(m, res);
    /* pass */
    long long ifi = 0LL;
    /* pass */
    while ((ifi < prog->interfaces->len)) {
        /* pass */
        ({ TrStr _at_t3193 = (_own(((HirInterface*)List_ptr_get(prog->interfaces, ifi))->name)); LModule_add_iface(m, _at_t3193); _tr_str_release(_at_t3193); });
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
        ClassLayout* lay = ({ TrStr _at_t3194 = (_own(c->name)); __auto_type _wr = (ClassLayout_init(_at_t3194)); _tr_str_release(_at_t3194); _wr; });
        /* pass */
        if ((c->base_classes->len == 1LL)) {
            /* pass */
            lay->base = ({ TrStr _at_t3195 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_own(_at_t3195)); _tr_str_release(_at_t3195); _wr; });
        }
        /* pass */
        lay->is_value = (!c->is_class);
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
        EnumLayout* elay = ({ TrStr _at_t3196 = (_own(e->name)); __auto_type _wr = (EnumLayout_init(_at_t3196)); _tr_str_release(_at_t3196); _wr; });
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < e->variants->len)) {
            /* pass */
            HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, vi));
            /* pass */
            VariantLayout* vlay = ({ TrStr _at_t3197 = (_own(v->name)); __auto_type _wr = (VariantLayout_init(_at_t3197)); _tr_str_release(_at_t3197); _wr; });
            /* pass */
            long long pf = 0LL;
            /* pass */
            while ((pf < v->fields->len)) {
                /* pass */
                ({ TrStr _at_t3198 = (_own(((HirParam*)List_ptr_get(v->fields, pf))->name)); List_TrStr_append(vlay->fields, _at_t3198); _tr_str_release(_at_t3198); });
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
                m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(((HirClass*)List_ptr_get(prog->classes, i))->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("class '", 7LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': field-tag registration diverged", 34LL))); _tr_str_release(_cl); _cres; });
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
                        if (_tr_str_eqv((List_TrStr_get(e2->generics, egk)), (p2ty->name))) {
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
                    List_TrStr_append(vlay2->fcls, _tr_str_lit_len("", 0LL));
                } else {
                    /* pass */
                    List_i64_append(vlay2->ftags, _tag_of(m, p2ty));
                    /* pass */
                    ({ TrStr _at_t3199 = (_cls_of_ty(m, p2ty)); List_TrStr_append(vlay2->fcls, _at_t3199); _tr_str_release(_at_t3199); });
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

__attribute__((hot)) bool _class_needs_drop(ClassLayout* lay) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lay->ftags->len)) {
        /* pass */
        long long t = List_i64_get(lay->ftags, i);
        /* pass */
        if ((((t == 1LL) || (t == 10LL)) || (t == 11LL))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _class_needs_drop_by_name(LModule* m, TrStr cname) {
    /* pass */
    long long ci = LModule_class_index(m, cname);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return _class_needs_drop(((ClassLayout*)List_ptr_get(m->classes, ci)));
}

__attribute__((hot)) void _attach_class_drop(LModule* m, LFunc* lf, long long obj, TrStr cname) {
    /* pass */
    if ((!_class_needs_drop_by_name(m, cname))) {
        /* pass */
        return;
    }
    /* pass */
    long long faddr = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, faddr, 12LL);
    /* pass */
    ({ TrStr _at_t3200 = (_tr_strx_concatv((cname), (_tr_str_lit_len("__drop", 6LL)))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t3200)); _tr_str_release(_at_t3200); });
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_set_drop", 19LL));
    /* pass */
    List_i64* sda = (void*)List_i64_new();
    /* pass */
    List_i64_append(sda, obj);
    /* pass */
    List_i64_append(sda, faddr);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_set_drop", 19LL), sda));
}

__attribute__((hot)) void _build_class_drop(LModule* m, ClassLayout* lay) {
    /* pass */
    TrStr dname = _tr_strx_concatv((lay->name), (_tr_str_lit_len("__drop", 6LL)));
    /* pass */
    if (LModule_is_user_fn(m, dname)) {
        /* pass */
        _tr_str_release(dname);
        return;
    }
    /* pass */
    List_TrStr_append(m->fn_names, dname);
    /* pass */
    List_i64_append(m->fn_ret, 0LL);
    /* pass */
    LFunc* lf = LFunc_init(dname);
    /* pass */
    List_TrStr_append(lf->params, _tr_str_lit_len("self", 4LL));
    /* pass */
    LFunc_add_var(lf, _tr_str_lit_len("self", 4LL));
    /* pass */
    LFunc_set_var_type(lf, _tr_str_lit_len("self", 4LL), 10LL);
    /* pass */
    ({ TrStr _at_t3201 = (_own(lay->name)); LFunc_set_var_cls(lf, _tr_str_lit_len("self", 4LL), _at_t3201); _tr_str_release(_at_t3201); });
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lay->ftags->len)) {
        /* pass */
        long long t = List_i64_get(lay->ftags, i);
        /* pass */
        if ((t == 1LL)) {
            /* pass */
            long long s0 = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(s0, _tr_str_lit_len("self", 4LL)));
            /* pass */
            long long fv = _emit_field_get(m, lf, s0, (i * 8LL), 1LL);
            /* pass */
            _release_str(m, lf, fv);
        } else if (((t == 10LL) || (t == 11LL))) {
            /* pass */
            long long s1 = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(s1, _tr_str_lit_len("self", 4LL)));
            /* pass */
            long long fv2 = _emit_field_get(m, lf, s1, (i * 8LL), t);
            /* pass */
            _release_obj(m, lf, fv2);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_str_release(dname);
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) void _gen_class_drops(LModule* m) {
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < m->classes->len)) {
        /* pass */
        ClassLayout* lay = ((ClassLayout*)List_ptr_get(m->classes, ci));
        /* pass */
        if (_class_needs_drop(lay)) {
            /* pass */
            _build_class_drop(m, lay);
        }
        /* pass */
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) bool _method_in_prog_functions(HirProgram* prog, TrStr cls, TrStr name) {
    /* pass */
    long long dfi = 0LL;
    /* pass */
    while ((dfi < prog->functions->len)) {
        /* pass */
        HirFunction* df = ((HirFunction*)List_ptr_get(prog->functions, dfi));
        /* pass */
        if (((_tr_str_eqv((df->class_name), (cls)) && _tr_str_eqv((df->name), (name))) && (!df->is_extern))) {
            /* pass */
            return true;
        }
        /* pass */
        dfi = (dfi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long _method_nonself_pcount(HirFunction* f) {
    /* pass */
    long long c = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if ((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, i))->name), (_tr_str_lit_len("self", 4LL))))) {
            /* pass */
            c = (c + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return c;
}

__attribute__((hot)) TrStr _ov_method_name(LModule* m, TrStr cls, HirFunction* f) {
    /* pass */
    TrStr base = ({ TrStr _cl = (({ TrStr _cl = (_own(cls)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if (LModule_is_overloaded(m, cls, f->name)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((base), (_tr_str_lit_len("_", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_method_nonself_pcount(f))))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("arg", 3LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return base;
}

__attribute__((hot)) void _detect_overloads(LModule* m, HirProgram* prog) {
    /* pass */
    long long i = 0LL;
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
                    if (_tr_str_eqv((((HirFunction*)List_ptr_get(ov_cls->methods, mj))->name), (m1->name))) {
                        /* pass */
                        LModule_mark_overloaded(m, ov_cls->name, m1->name);
                        /* pass */
                        mj = ov_cls->methods->len;
                    } else {
                        /* pass */
                        mj = (mj + 1LL);
                    }
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
                if (_tr_str_eqv((((HirFunction*)List_ptr_get(ov_enm->methods, mj))->name), (m1->name))) {
                    /* pass */
                    LModule_mark_overloaded(m, ov_enm->name, m1->name);
                    /* pass */
                    mj = ov_enm->methods->len;
                } else {
                    /* pass */
                    mj = (mj + 1LL);
                }
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) long long _arg_limit(LModule* m) {
    /* pass */
    if (m->target_llvm) {
        /* pass */
        return 24LL;
    }
    /* pass */
    return 6LL;
}

__attribute__((hot)) LModule* lower_to_lir_t(HirProgram* prog, bool llvm) {
    /* pass */
    return lower_to_lir_nh(prog, llvm, false);
}

__attribute__((hot)) LModule* lower_to_lir_nh(HirProgram* prog, bool llvm, bool no_heap) {
    /* pass */
    LModule* m = LModule_init();
    /* pass */
    m->target_llvm = llvm;
    /* pass */
    m->no_heap = no_heap;
    /* pass */
    HirProgram* _cltmp_t3202 = _tr_obj_retain(prog);
    _tr_obj_release(m->hir_prog, _trdrop_HirProgram);
    m->hir_prog = _cltmp_t3202;
    /* pass */
    _register_classes(m, prog);
    /* pass */
    return _lower_to_lir_body(m, prog);
}

__attribute__((hot)) LModule* lower_to_lir(HirProgram* prog) {
    /* pass */
    return lower_to_lir_t(prog, false);
}

__attribute__((hot)) bool _is_extern_c_int(TrStr n) {
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("c_int", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uchar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_schar", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_short", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ushort", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_long", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ulong", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_longlong", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ulonglong", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_size_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ssize_t", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_int8_t", 8LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int16_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int32_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int64_t", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_uint8_t", 9LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint16_t", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint32_t", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint64_t", 10LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("c_intptr_t", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uintptr_t", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ptrdiff_t", 11LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("c_wchar", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char16", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char32", 8LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_extern_c_float(TrStr n) {
    /* pass */
    return ((_tr_str_eqv((n), (_tr_str_lit_len("c_float", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_double", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ldouble", 9LL))));
}

__attribute__((hot)) long long _extern_sig_tag(LModule* m, AstType* ty) {
    /* pass */
    long long t = _tag_of(m, ty);
    /* pass */
    if ((t >= 0LL)) {
        /* pass */
        return t;
    }
    /* pass */
    if (_is_extern_c_int(ty->name)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    if (_is_extern_c_float(ty->name)) {
        /* pass */
        return 5LL;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) LModule* _lower_to_lir_body(LModule* m, HirProgram* prog) {
    /* pass */
    _detect_overloads(m, prog);
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
            long long eptag = _extern_sig_tag(m, ((HirParam*)List_ptr_get(ef->params, epi))->ty);
            /* pass */
            if (((((eptag != 0LL) && (eptag != 1LL)) && (eptag != 4LL)) && (eptag != 5LL))) {
                /* pass */
                ef_ok = false;
            }
            /* pass */
            epi = (epi + 1LL);
        }
        /* pass */
        long long ertag = _extern_sig_tag(m, ef->ret_ty);
        /* pass */
        if (((((ertag != 0LL) && (ertag != 1LL)) && (ertag != 4LL)) && (ertag != 5LL))) {
            /* pass */
            ef_ok = false;
        }
        /* pass */
        if (ef_ok) {
            /* pass */
            ({ TrStr _at_t3203 = (_own(ef->name)); List_TrStr_append(m->extfn_names, _at_t3203); _tr_str_release(_at_t3203); });
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
            if (((!_tr_str_eqv((f0->class_name), (_tr_str_lit_len("", 0LL)))) && (_prog_generic_class_index(m, f0->class_name) >= 0LL))) {
                /* pass */
                i = (i + 1LL);
                /* pass */
                continue;
            }
            /* pass */
            TrStr f0mang = _tr_str_retain(f0->name);
            /* pass */
            if ((!_tr_str_eqv((f0->class_name), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr _strtmp_t3204 = _ov_method_name(m, f0->class_name, f0);
                _tr_str_release(f0mang);
                f0mang = _strtmp_t3204;
            }
            /* pass */
            long long f0rt = _lir_fn_ret_tag(m, f0);
            /* pass */
            List_TrStr_append(m->fn_names, f0mang);
            /* pass */
            List_i64_append(m->fn_ret, f0rt);
            /* pass */
            if (((f0->returns_owned || LModule_is_value_class(m, f0->ret_ty->name)) && ((f0rt == 10LL) || (f0rt == 11LL)))) {
                /* pass */
                LModule_mark_fn_owned(m, f0mang);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long cmi = 0LL;
    /* pass */
    while ((cmi < prog->classes->len)) {
        /* pass */
        HirClass* cm_cls = ((HirClass*)List_ptr_get(prog->classes, cmi));
        /* pass */
        if ((cm_cls->generics->len == 0LL)) {
            /* pass */
            long long cmi2 = 0LL;
            /* pass */
            while ((cmi2 < cm_cls->methods->len)) {
                /* pass */
                HirFunction* cm_f = ((HirFunction*)List_ptr_get(cm_cls->methods, cmi2));
                /* pass */
                if ((((!cm_f->is_extern) && (!_fn_is_specializable(m, cm_f))) && (!_method_in_prog_functions(prog, cm_cls->name, cm_f->name)))) {
                    /* pass */
                    TrStr cm_mang = _ov_method_name(m, cm_cls->name, cm_f);
                    /* pass */
                    long long cm_rt = _lir_fn_ret_tag(m, cm_f);
                    /* pass */
                    ({ TrStr _at_t3205 = (_own(cm_mang)); List_TrStr_append(m->fn_names, _at_t3205); _tr_str_release(_at_t3205); });
                    /* pass */
                    List_i64_append(m->fn_ret, cm_rt);
                    /* pass */
                    if ((cm_f->returns_owned && ((cm_rt == 10LL) || (cm_rt == 11LL)))) {
                        /* pass */
                        LModule_mark_fn_owned(m, cm_mang);
                    }
                }
                /* pass */
                cmi2 = (cmi2 + 1LL);
            }
        }
        /* pass */
        cmi = (cmi + 1LL);
    }
    /* pass */
    long long emi = 0LL;
    /* pass */
    while ((emi < prog->enums->len)) {
        /* pass */
        HirEnum* em_en = ((HirEnum*)List_ptr_get(prog->enums, emi));
        /* pass */
        long long emi2 = 0LL;
        /* pass */
        while ((emi2 < em_en->methods->len)) {
            /* pass */
            HirFunction* em_f = ((HirFunction*)List_ptr_get(em_en->methods, emi2));
            /* pass */
            if ((((!em_f->is_extern) && (!_fn_is_specializable(m, em_f))) && (!_method_in_prog_functions(prog, em_en->name, em_f->name)))) {
                /* pass */
                TrStr em_mang = _ov_method_name(m, em_en->name, em_f);
                /* pass */
                long long em_rt = _tag_of(m, em_f->ret_ty);
                /* pass */
                ({ TrStr _at_t3206 = (_own(em_mang)); List_TrStr_append(m->fn_names, _at_t3206); _tr_str_release(_at_t3206); });
                /* pass */
                List_i64_append(m->fn_ret, em_rt);
                /* pass */
                if ((em_f->returns_owned && ((em_rt == 10LL) || (em_rt == 11LL)))) {
                    /* pass */
                    LModule_mark_fn_owned(m, em_mang);
                }
            }
            /* pass */
            emi2 = (emi2 + 1LL);
        }
        /* pass */
        emi = (emi + 1LL);
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
            m->fail_note = _tr_str_lit_len("unsupported top-level statement", 31LL);
            /* pass */
            return _tr_obj_retain(m);
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
            if (((!_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (_prog_generic_class_index(m, f->class_name) >= 0LL))) {
                /* pass */
                i = (i + 1LL);
                /* pass */
                continue;
            }
            /* pass */
            if (_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                _lir_lower_function(m, f);
            } else {
                /* pass */
                _lir_lower_method(m, f->class_name, f);
            }
            /* pass */
            if ((!m->ok)) {
                /* pass */
                if ((_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    return _tr_obj_retain(m);
                }
                /* pass */
                TrStr un = _tr_str_retain(f->name);
                /* pass */
                if ((!_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t3207 = _ov_method_name(m, f->class_name, f);
                    _tr_str_release(un);
                    un = _strtmp_t3207;
                }
                /* pass */
                ({ TrStr _at_t3208 = (_own(un)); List_TrStr_append(m->unavail_names, _at_t3208); _tr_str_release(_at_t3208); });
                /* pass */
                ({ TrStr _at_t3209 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t3209); _tr_str_release(_at_t3209); });
                /* pass */
                m->ok = true;
                /* pass */
                m->fail_note = _tr_str_lit_len("", 0LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long cml = 0LL;
    /* pass */
    while ((cml < prog->classes->len)) {
        /* pass */
        HirClass* cml_cls = ((HirClass*)List_ptr_get(prog->classes, cml));
        /* pass */
        if ((cml_cls->generics->len == 0LL)) {
            /* pass */
            long long cml2 = 0LL;
            /* pass */
            while ((cml2 < cml_cls->methods->len)) {
                /* pass */
                HirFunction* cml_f = ((HirFunction*)List_ptr_get(cml_cls->methods, cml2));
                /* pass */
                if ((((!cml_f->is_extern) && (!_fn_is_specializable(m, cml_f))) && (!_method_in_prog_functions(prog, cml_cls->name, cml_f->name)))) {
                    /* pass */
                    _lir_lower_method(m, cml_cls->name, cml_f);
                    /* pass */
                    if ((!m->ok)) {
                        /* pass */
                        TrStr cun = _ov_method_name(m, cml_cls->name, cml_f);
                        /* pass */
                        ({ TrStr _at_t3210 = (_own(cun)); List_TrStr_append(m->unavail_names, _at_t3210); _tr_str_release(_at_t3210); });
                        /* pass */
                        ({ TrStr _at_t3211 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t3211); _tr_str_release(_at_t3211); });
                        /* pass */
                        m->ok = true;
                        /* pass */
                        m->fail_note = _tr_str_lit_len("", 0LL);
                    }
                }
                /* pass */
                cml2 = (cml2 + 1LL);
            }
        }
        /* pass */
        cml = (cml + 1LL);
    }
    /* pass */
    long long eml = 0LL;
    /* pass */
    while ((eml < prog->enums->len)) {
        /* pass */
        HirEnum* eml_en = ((HirEnum*)List_ptr_get(prog->enums, eml));
        /* pass */
        long long eml2 = 0LL;
        /* pass */
        while ((eml2 < eml_en->methods->len)) {
            /* pass */
            HirFunction* eml_f = ((HirFunction*)List_ptr_get(eml_en->methods, eml2));
            /* pass */
            if ((((!eml_f->is_extern) && (!_fn_is_specializable(m, eml_f))) && (!_method_in_prog_functions(prog, eml_en->name, eml_f->name)))) {
                /* pass */
                _lir_lower_method(m, eml_en->name, eml_f);
                /* pass */
                if ((!m->ok)) {
                    /* pass */
                    TrStr eun = _ov_method_name(m, eml_en->name, eml_f);
                    /* pass */
                    ({ TrStr _at_t3212 = (_own(eun)); List_TrStr_append(m->unavail_names, _at_t3212); _tr_str_release(_at_t3212); });
                    /* pass */
                    ({ TrStr _at_t3213 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _at_t3213); _tr_str_release(_at_t3213); });
                    /* pass */
                    m->ok = true;
                    /* pass */
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
            /* pass */
            eml2 = (eml2 + 1LL);
        }
        /* pass */
        eml = (eml + 1LL);
    }
    /* pass */
    _gen_class_drops(m);
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
                            __auto_type _t3214 = (*((LInst*)List_ptr_get(blk2->insts, ii2)));
                            if (_t3214.tag == LInst_ICall) {
                                __auto_type ucallee = _t3214.data.ICall.callee;
                                /* pass */
                                hit = LModule_unavail_index(m, ucallee);
                            } else if (_t3214.tag == LInst_IFuncAddr) {
                                __auto_type ufname = _t3214.data.IFuncAddr.fname;
                                /* pass */
                                hit = LModule_unavail_index(m, ufname);
                            } else if (1) {
                                __auto_type _ = _t3214;
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
                        TrStr unote = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(lfc->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' calls '", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(m->unavail_names, hit)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' — ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(m->unavail_notes, hit)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        /* pass */
                        if (lfc->is_main) {
                            /* pass */
                            m->ok = false;
                            /* pass */
                            m->fail_note = _tr_str_retain(unote);
                            /* pass */
                            List_i64_free(dropped);
                            _tr_str_release(unote);
                            return _tr_obj_retain(m);
                        }
                        /* pass */
                        ({ TrStr _at_t3215 = (_own(lfc->name)); List_TrStr_append(m->unavail_names, _at_t3215); _tr_str_release(_at_t3215); });
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
    return _tr_obj_retain(m);
}

__attribute__((hot)) bool _fn_has_iface_param(LModule* m, HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, i))->ty->name);
        /* pass */
        if (((!_is_null_str(pn)) && LModule_is_iface(m, pn))) {
            /* pass */
            _tr_str_release(pn);
            return true;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(pn);
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
        if (((_tr_str_eqv((f->name), (name)) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (!f->is_extern))) {
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
        if ((((_tr_str_eqv((f->class_name), (cls)) && _tr_str_eqv((f->name), (method))) && (f->generics->len > 0LL)) && (!f->is_extern))) {
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
        if (_tr_str_eqv((List_TrStr_get(f->generics, i)), (n))) {
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
                if (_tr_str_eqv((((HirParam*)List_ptr_get(f->params, ri))->ty->name), (f->ret_ty->name))) {
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
    ({ TrStr _at_t3216 = (_own(mangled)); List_TrStr_append(m->fn_names, _at_t3216); _tr_str_release(_at_t3216); });
    /* pass */
    List_i64_append(m->fn_ret, rtag);
    /* pass */
    if (((f->returns_owned || LModule_is_value_class(m, f->ret_ty->name)) && ((rtag == 10LL) || (rtag == 11LL)))) {
        /* pass */
        ({ TrStr _at_t3217 = (_own(mangled)); LModule_mark_fn_owned(m, _at_t3217); _tr_str_release(_at_t3217); });
    }
    /* pass */
    LFunc* lf = ({ TrStr _at_t3218 = (_own(mangled)); __auto_type _wr = (LFunc_init(_at_t3218)); _tr_str_release(_at_t3218); _wr; });
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
        if ((((ptag == 10LL) || (ptag == 11LL)) && (!_tr_str_eqv((List_TrStr_get(argcls, pi)), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            ({ TrStr _at_t3219 = (List_TrStr_get(argcls, pi)); TrStr _at_t3220 = (_own(_at_t3219)); LFunc_set_var_cls(lf, p->name, _at_t3220); _tr_str_release(_at_t3219); _tr_str_release(_at_t3220); });
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

__attribute__((hot)) TrStr _mono_base(TrStr name) {
    /* pass */
    TrStr marker = _tr_str_lit_len("__MONO_", 7LL);
    /* pass */
    long long idx = _tr_str_index_of((name).data, (marker).data);
    /* pass */
    if ((idx < 0LL)) {
        /* pass */
        _tr_str_release(marker);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    _tr_str_release(marker);
    return _tr_str_wrap(_tr_str_slice((name).data, 0LL, idx));
}

__attribute__((hot)) TrStr _mono_concrete(TrStr name) {
    /* pass */
    TrStr marker = _tr_str_lit_len("__MONO_", 7LL);
    /* pass */
    long long idx = _tr_str_index_of((name).data, (marker).data);
    /* pass */
    if ((idx < 0LL)) {
        /* pass */
        _tr_str_release(marker);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice((name).data, (idx + _tr_str_lenv((marker))), _tr_str_lenv((name))));
}

__attribute__((hot)) bool _lir_lower_mono_fn(LModule* m, HirFunction* f, TrStr mangled, AstType* concrete) {
    /* pass */
    if ((f->generics->len != 1LL)) {
        /* pass */
        return false;
    }
    /* pass */
    bool ok = true;
    /* pass */
    ({ TrStr _at_t3221 = (List_TrStr_get(f->generics, 0LL)); TrStr _at_t3222 = (_own(_at_t3221)); List_TrStr_append(m->subst_names, _at_t3222); _tr_str_release(_at_t3221); _tr_str_release(_at_t3222); });
    /* pass */
    List_ptr_append(m->subst_tys, box_asttype_lir(concrete));
    /* pass */
    long long rtag = _tag_of(m, f->ret_ty);
    /* pass */
    if ((rtag < 0LL)) {
        /* pass */
        ok = false;
    }
    /* pass */
    if (ok) {
        /* pass */
        ({ TrStr _at_t3223 = (_own(mangled)); List_TrStr_append(m->fn_names, _at_t3223); _tr_str_release(_at_t3223); });
        /* pass */
        List_i64_append(m->fn_ret, rtag);
        /* pass */
        if ((f->returns_owned && ((rtag == 10LL) || (rtag == 11LL)))) {
            /* pass */
            ({ TrStr _at_t3224 = (_own(mangled)); LModule_mark_fn_owned(m, _at_t3224); _tr_str_release(_at_t3224); });
        }
        /* pass */
        LFunc* lf = ({ TrStr _at_t3225 = (_own(mangled)); __auto_type _wr = (LFunc_init(_at_t3225)); _tr_str_release(_at_t3225); _wr; });
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < f->params->len)) {
            /* pass */
            HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
            /* pass */
            long long ptag = _tag_of(m, p->ty);
            /* pass */
            if ((ptag < 0LL)) {
                /* pass */
                ok = false;
                /* pass */
                break;
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
                ({ TrStr _at_t3226 = (_cls_of_ty(m, p->ty)); LFunc_set_var_cls(lf, p->name, _at_t3226); _tr_str_release(_at_t3226); });
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        if (ok) {
            /* pass */
            LFunc_set_cur(lf, LFunc_new_block(lf));
            /* pass */
            if ((!lower_block(m, lf, f->body))) {
                /* pass */
                ok = false;
            }
            /* pass */
            if (ok) {
                /* pass */
                LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
                /* pass */
                List_ptr_append(m->funcs, _tr_obj_retain(lf));
            }
        }
    }
    /* pass */
    List_TrStr_pop(m->subst_names);
    /* pass */
    List_ptr_pop(m->subst_tys);
    /* pass */
    return ok;
}

__attribute__((hot)) void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f) {
    /* pass */
    LFunc* lf = ({ TrStr _at_t3227 = (_ov_method_name(m, class_name, f)); __auto_type _wr = (LFunc_init(_at_t3227)); _tr_str_release(_at_t3227); _wr; });
    /* pass */
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
        /* pass */
        lf->is_throws = true;
        /* pass */
        lf->throws_ok_tag = _tag_of(m, f->ret_ty);
        /* pass */
        lf->throws_err_tag = _tag_of(m, f->throws_ty);
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        long long ptag = 0LL;
        /* pass */
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
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
            m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': unsupported param type", 25LL))); _tr_str_release(_cl); _cres; });
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
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
            /* pass */
            ({ TrStr _at_t3228 = (_own(class_name)); LFunc_set_var_cls(lf, p->name, _at_t3228); _tr_str_release(_at_t3228); });
        } else if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
            /* pass */
            ({ TrStr _at_t3229 = (_own(p->ty->name)); LFunc_set_var_cls(lf, p->name, _at_t3229); _tr_str_release(_at_t3229); });
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
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': body uses an unsupported construct", 37LL))); _tr_str_release(_cl); _cres; });
        }
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
            if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(class_name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': deferred statement not lowerable", 35LL))); _tr_str_release(_cl); _cres; });
            }
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
    __auto_type _t3230 = (*s);
    if (_t3230.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t3230.data.SLineMarker.n;
        return true;
    } else if (_t3230.tag == HirStmt_SPass) {
        return true;
    } else if (_t3230.tag == HirStmt_SLet) {
        __auto_type name = _t3230.data.SLet.name;
__auto_type ty = _t3230.data.SLet.ty;
__auto_type val = _t3230.data.SLet.val;
        /* pass */
        if ((((m->target_llvm && _tr_str_eqv((ty->name), (_tr_str_lit_len("Array", 5LL)))) && (ty->array_size > 0LL)) && (ty->args->len > 0LL))) {
            /* pass */
            long long getag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
            /* pass */
            if (((getag != 0LL) && (getag != 4LL))) {
                /* pass */
                return false;
            }
            /* pass */
            long long gidx_a = LModule_add_global(m, name, getag);
            /* pass */
            LModule_set_global_arr(m, gidx_a, ty->array_size);
            /* pass */
            if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
                /* pass */
                return true;
            }
            /* pass */
            return false;
        }
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
    } else if (_t3230.tag == HirStmt_SAssign) {
        __auto_type target = _t3230.data.SAssign.target;
__auto_type val = _t3230.data.SAssign.val;
        /* pass */
        TrStr nm = _ident_name(target);
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL)))) {
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
        __auto_type _ = _t3230;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t3231 = (*s);
    if (_t3231.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t3231.data.SLineMarker.n;
        return true;
    } else if (_t3231.tag == HirStmt_SPass) {
        return true;
    } else if (_t3231.tag == HirStmt_SLet) {
        __auto_type name = _t3231.data.SLet.name;
__auto_type val = _t3231.data.SLet.val;
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
    } else if (_t3231.tag == HirStmt_SAssign) {
        __auto_type target = _t3231.data.SAssign.target;
__auto_type val = _t3231.data.SAssign.val;
        /* pass */
        long long v2 = lower_expr(m, lf, val);
        /* pass */
        if ((v2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long gidx2 = ({ TrStr _at_t3232 = (_ident_name(target)); __auto_type _wr = (LModule_global_index(m, _at_t3232)); _tr_str_release(_at_t3232); _wr; });
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
        __auto_type _ = _t3231;
        /* pass */
        return false;
    }
}

__attribute__((hot)) void _lir_lower_function(LModule* m, HirFunction* f) {
    /* pass */
    LFunc* lf = LFunc_init(f->name);
    /* pass */
    if (_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL)))) {
        /* pass */
        lf->is_main = true;
    }
    /* pass */
    if ((_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && (f->params->len > 0LL))) {
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_argv_list", 16LL));
    }
    /* pass */
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
        /* pass */
        lf->is_throws = true;
        /* pass */
        lf->throws_ok_tag = _tag_of(m, f->ret_ty);
        /* pass */
        lf->throws_err_tag = _tag_of(m, f->throws_ty);
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        if ((!_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))))) {
            /* pass */
            long long ptag = _tag_of(m, p->ty);
            /* pass */
            if (((lf->is_main && (_tr_str_eqv((p->ty->name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((p->ty->name), (_tr_str_lit_len("List", 4LL))))) && (p->ty->args->len > 0LL))) {
                /* pass */
                TrStr _mel = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(p->ty->args, 0LL))))->name);
                /* pass */
                if ((_tr_str_eqv((_mel), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_mel), (_tr_str_lit_len("String", 6LL))))) {
                    /* pass */
                    ptag = 3LL;
                }
                _tr_str_release(_mel);
            }
            /* pass */
            if ((ptag < 0LL)) {
                /* pass */
                m->ok = false;
                /* pass */
                m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': unsupported param type '", 27LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own(p->ty->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_cl); _cres; });
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
                ({ TrStr _at_t3233 = (_own(p->ty->name)); LFunc_set_var_cls(lf, p->name, _at_t3233); _tr_str_release(_at_t3233); });
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
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': body uses an unsupported construct", 37LL))); _tr_str_release(_cl); _cres; });
        }
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
            if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_own(f->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("': deferred statement not lowerable", 35LL))); _tr_str_release(_cl); _cres; });
            }
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
    LFunc_emit(lf, LInst_ctor_IStore(obj, off, val, 3LL));
}

__attribute__((hot)) long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag) {
    /* pass */
    long long gd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, gd, tag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoad(gd, obj, off, 3LL));
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
    if (m->no_heap) {
        /* pass */
        m->ok = false;
        /* pass */
        m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("[H-1] constructing enum '", 25LL)), (ename))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' heap-allocates, which --no-heap forbids on this backend (it boxes via the object allocator).\n      FIX: use the C backend for --no-heap (it keeps enums as stack tagged unions), or avoid heap enums such as Option/Result on the zero-heap path", 242LL))); _tr_str_release(_cl); _cres; });
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
        } else if (((ftg == 10LL) || (ftg == 11LL))) {
            /* pass */
            if ((!_field_tag_ok(avt, ftg))) {
                /* pass */
                List_i64_free(vals);
                return (-1LL);
            }
            /* pass */
            _secure_obj(m, lf, av);
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
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
    /* pass */
    List_i64* oaa = (void*)List_i64_new();
    /* pass */
    List_i64_append(oaa, szc);
    /* pass */
    long long od = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), oaa));
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
    _fresh_mark_obj(lf, od);
    /* pass */
    List_i64_free(vals);
    return od;
}

__attribute__((hot)) long long _lir_fn_ret_tag(LModule* m, HirFunction* f) {
    /* pass */
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
        /* pass */
        return 11LL;
    }
    /* pass */
    return _tag_of(m, f->ret_ty);
}

__attribute__((hot)) long long _wrap_result(LModule* m, LFunc* lf, long long vidx, long long payv, long long paytag) {
    /* pass */
    long long pv = payv;
    /* pass */
    if ((paytag == 5LL)) {
        /* pass */
        long long fb = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IFBits(fb, pv));
        /* pass */
        pv = fb;
    } else if ((paytag == 1LL)) {
        /* pass */
        _secure_str(m, lf, pv);
    } else if (((paytag == 10LL) || (paytag == 11LL))) {
        /* pass */
        _secure_obj(m, lf, pv);
    }
    /* pass */
    long long szc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_enum_size(m, _tr_str_lit_len("Result", 6LL))));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
    /* pass */
    List_i64* aa = (void*)List_i64_new();
    /* pass */
    List_i64_append(aa, szc);
    /* pass */
    long long od = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), aa));
    /* pass */
    LFunc_set_vreg_type(lf, od, 11LL);
    /* pass */
    long long tgc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(tgc, vidx));
    /* pass */
    _emit_field_set(m, lf, od, 0LL, tgc);
    /* pass */
    _emit_field_set(m, lf, od, 8LL, pv);
    /* pass */
    _fresh_mark_obj(lf, od);
    /* pass */
    return od;
}

__attribute__((hot)) long long _find_free_fn_idx(LModule* m, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->hir_prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
        /* pass */
        if ((((_tr_str_eqv((f->name), (name)) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (!f->is_extern)) && (f->generics->len == 0LL))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _ensure_await_wrapper(LModule* m, TrStr fn) {
    /* pass */
    TrStr wname = _tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn));
    /* pass */
    if (LModule_is_user_fn(m, wname)) {
        /* pass */
        _tr_str_release(wname);
        return true;
    }
    /* pass */
    long long fidx = _find_free_fn_idx(m, fn);
    /* pass */
    if ((fidx < 0LL)) {
        /* pass */
        _tr_str_release(wname);
        return false;
    }
    /* pass */
    HirFunction* hf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, fidx));
    /* pass */
    long long frtag = _tag_of(m, hf->ret_ty);
    /* pass */
    if ((frtag < 0LL)) {
        /* pass */
        _tr_str_release(wname);
        return false;
    }
    /* pass */
    ({ TrStr _at_t3234 = (_own(wname)); List_TrStr_append(m->fn_names, _at_t3234); _tr_str_release(_at_t3234); });
    /* pass */
    List_i64_append(m->fn_ret, 0LL);
    /* pass */
    LFunc* wlf = ({ TrStr _at_t3235 = (_own(wname)); __auto_type _wr = (LFunc_init(_at_t3235)); _tr_str_release(_at_t3235); _wr; });
    /* pass */
    List_TrStr_append(wlf->params, _tr_str_lit_len("__vp", 4LL));
    /* pass */
    LFunc_add_var(wlf, _tr_str_lit_len("__vp", 4LL));
    /* pass */
    LFunc_set_var_type(wlf, _tr_str_lit_len("__vp", 4LL), 0LL);
    /* pass */
    LFunc_set_cur(wlf, LFunc_new_block(wlf));
    /* pass */
    List_i64* callargs = (void*)List_i64_new();
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < hf->params->len)) {
        /* pass */
        long long ptag = _tag_of(m, ((HirParam*)List_ptr_get(hf->params, pi))->ty);
        /* pass */
        if ((ptag < 0LL)) {
            /* pass */
            _tr_str_release(wname);
            _tr_obj_release(wlf, _trdrop_LFunc);
            List_i64_free(callargs);
            return false;
        }
        /* pass */
        long long vpv = LFunc_new_vreg(wlf);
        /* pass */
        LFunc_emit(wlf, LInst_ctor_ILoadVar(vpv, _tr_str_lit_len("__vp", 4LL)));
        /* pass */
        LFunc_set_vreg_type(wlf, vpv, 0LL);
        /* pass */
        long long argi = _emit_field_get(m, wlf, vpv, (pi * 8LL), ptag);
        /* pass */
        if ((ptag == 5LL)) {
            /* pass */
            long long fb = LFunc_new_vreg(wlf);
            /* pass */
            LFunc_emit(wlf, LInst_ctor_IBitsF(fb, argi));
            /* pass */
            LFunc_set_vreg_type(wlf, fb, 5LL);
            /* pass */
            argi = fb;
        }
        /* pass */
        List_i64_append(callargs, argi);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    if ((callargs->len > 6LL)) {
        /* pass */
        _tr_str_release(wname);
        _tr_obj_release(wlf, _trdrop_LFunc);
        List_i64_free(callargs);
        return false;
    }
    /* pass */
    long long rd = LFunc_new_vreg(wlf);
    /* pass */
    ({ TrStr _at_t3236 = (_own(fn)); LFunc_emit(wlf, LInst_ctor_ICall(rd, _at_t3236, callargs)); _tr_str_release(_at_t3236); });
    /* pass */
    LFunc_set_vreg_type(wlf, rd, frtag);
    /* pass */
    long long vpf = LFunc_new_vreg(wlf);
    /* pass */
    LFunc_emit(wlf, LInst_ctor_ILoadVar(vpf, _tr_str_lit_len("__vp", 4LL)));
    /* pass */
    LFunc_set_vreg_type(wlf, vpf, 0LL);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_free", 15LL));
    /* pass */
    List_i64* fa = (void*)List_i64_new();
    /* pass */
    List_i64_append(fa, vpf);
    /* pass */
    LFunc_emit(wlf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_raw_free", 15LL), fa));
    /* pass */
    long long ret = rd;
    /* pass */
    if ((frtag == 5LL)) {
        /* pass */
        long long rb = LFunc_new_vreg(wlf);
        /* pass */
        LFunc_emit(wlf, LInst_ctor_IFBits(rb, rd));
        /* pass */
        ret = rb;
    }
    /* pass */
    LFunc_set_term(wlf, LTerm_ctor_TRetVal(ret));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(wlf));
    /* pass */
    _tr_str_release(wname);
    _tr_obj_release(wlf, _trdrop_LFunc);
    return true;
}

__attribute__((hot)) long long _lower_await(LModule* m, LFunc* lf, HirExpr* awexpr) {
    /* pass */
    __auto_type _t3237 = (*awexpr);
    if (_t3237.tag == HirExpr_ECall) {
        __auto_type callee = _t3237.data.ECall.callee;
__auto_type args = _t3237.data.ECall.args;
__auto_type callty = _t3237.data.ECall.ty;
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if (((_is_null_str(fn) || _tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, fn)))) {
            /* pass */
            _tr_str_release(fn);
            return lower_expr(m, lf, awexpr);
        }
        /* pass */
        if ((!_ensure_await_wrapper(m, fn))) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        long long nargs = args->len;
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
        /* pass */
        long long packsz = (nargs * 8LL);
        /* pass */
        if ((packsz < 8LL)) {
            /* pass */
            packsz = 8LL;
        }
        /* pass */
        long long szc = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
        /* pass */
        List_i64* aa = (void*)List_i64_new();
        /* pass */
        List_i64_append(aa, szc);
        /* pass */
        long long ab = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ab, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
        /* pass */
        LFunc_set_vreg_type(lf, ab, 0LL);
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < nargs)) {
            /* pass */
            long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            if ((argv < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long argt = LFunc_vreg_type(lf, argv);
            /* pass */
            if ((argt == 5LL)) {
                /* pass */
                long long afb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
                /* pass */
                argv = afb;
            } else if ((argt == 1LL)) {
                /* pass */
                _secure_str(m, lf, argv);
            } else if (((argt == 10LL) || (argt == 11LL))) {
                /* pass */
                _secure_obj(m, lf, argv);
            }
            /* pass */
            _emit_field_set(m, lf, ab, (ai * 8LL), argv);
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        long long faddr = LFunc_new_vreg(lf);
        /* pass */
        LFunc_set_vreg_type(lf, faddr, 12LL);
        /* pass */
        ({ TrStr _at_t3238 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t3238)); _tr_str_release(_at_t3238); });
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_go_h", 11LL));
        /* pass */
        List_i64* ga = (void*)List_i64_new();
        /* pass */
        List_i64_append(ga, faddr);
        /* pass */
        List_i64_append(ga, ab);
        /* pass */
        long long co = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(co, _tr_str_lit_len("_tr_co_go_h", 11LL), ga));
        /* pass */
        LFunc_set_vreg_type(lf, co, 0LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_await_h", 14LL));
        /* pass */
        List_i64* wa = (void*)List_i64_new();
        /* pass */
        List_i64_append(wa, co);
        /* pass */
        long long r = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(r, _tr_str_lit_len("_tr_co_await_h", 14LL), wa));
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_free_h", 13LL));
        /* pass */
        List_i64* fca = (void*)List_i64_new();
        /* pass */
        List_i64_append(fca, co);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_free_h", 13LL), fca));
        /* pass */
        long long rtag = _tag_of(m, callty);
        /* pass */
        if ((rtag < 0LL)) {
            /* pass */
            rtag = 0LL;
        }
        /* pass */
        if ((rtag == 5LL)) {
            /* pass */
            long long rf = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBitsF(rf, r));
            /* pass */
            LFunc_set_vreg_type(lf, rf, 5LL);
            /* pass */
            _tr_str_release(fn);
            return rf;
        }
        /* pass */
        LFunc_set_vreg_type(lf, r, rtag);
        /* pass */
        _tr_str_release(fn);
        return r;
    } else if (1) {
        __auto_type _ = _t3237;
        /* pass */
        return lower_expr(m, lf, awexpr);
    }
}

__attribute__((hot)) bool _lower_spawn(LModule* m, LFunc* lf, HirExpr* expr) {
    /* pass */
    __auto_type _t3239 = (*expr);
    if (_t3239.tag == HirExpr_ECall) {
        __auto_type callee = _t3239.data.ECall.callee;
__auto_type args = _t3239.data.ECall.args;
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if (((_is_null_str(fn) || _tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, fn)))) {
            /* pass */
            _tr_str_release(fn);
            return false;
        }
        /* pass */
        if ((!_ensure_await_wrapper(m, fn))) {
            /* pass */
            _tr_str_release(fn);
            return false;
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
        /* pass */
        long long packsz = (args->len * 8LL);
        /* pass */
        if ((packsz < 8LL)) {
            /* pass */
            packsz = 8LL;
        }
        /* pass */
        long long szc = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
        /* pass */
        List_i64* aa = (void*)List_i64_new();
        /* pass */
        List_i64_append(aa, szc);
        /* pass */
        long long buf = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(buf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
        /* pass */
        LFunc_set_vreg_type(lf, buf, 0LL);
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            if ((argv < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return false;
            }
            /* pass */
            long long argt = LFunc_vreg_type(lf, argv);
            /* pass */
            if ((argt == 5LL)) {
                /* pass */
                long long afb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
                /* pass */
                argv = afb;
            } else if ((argt == 1LL)) {
                /* pass */
                _secure_str(m, lf, argv);
            } else if (((argt == 10LL) || (argt == 11LL))) {
                /* pass */
                _secure_obj(m, lf, argv);
            }
            /* pass */
            _emit_field_set(m, lf, buf, (ai * 8LL), argv);
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        long long faddr = LFunc_new_vreg(lf);
        /* pass */
        LFunc_set_vreg_type(lf, faddr, 12LL);
        /* pass */
        ({ TrStr _at_t3240 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t3240)); _tr_str_release(_at_t3240); });
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_thread_start", 16LL));
        /* pass */
        List_i64* sa = (void*)List_i64_new();
        /* pass */
        List_i64_append(sa, faddr);
        /* pass */
        List_i64_append(sa, buf);
        /* pass */
        long long handle = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(handle, _tr_str_lit_len("_tr_thread_start", 16LL), sa));
        /* pass */
        LFunc_set_vreg_type(lf, handle, 0LL);
        /* pass */
        if ((lf->in_taskgroup > 0LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_tg_push", 11LL));
            /* pass */
            List_i64* pa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pa, handle);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_push", 11LL), pa));
        } else {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_thread_detach", 17LL));
            /* pass */
            List_i64* da = (void*)List_i64_new();
            /* pass */
            List_i64_append(da, handle);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_thread_detach", 17LL), da));
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        _tr_str_release(fn);
        return true;
    } else if (1) {
        __auto_type _ = _t3239;
        /* pass */
        return false;
    }
}

__attribute__((hot)) long long _box_call(LModule* m, LFunc* lf, long long selfv, TrStr rtfn, List_ptr* margs, long long nargs, long long rtag) {
    /* pass */
    if ((margs->len != nargs)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    LModule_add_extern(m, rtfn);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, selfv);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < nargs)) {
        /* pass */
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, i)));
        /* pass */
        if (((av < 0LL) || (LFunc_vreg_type(lf, av) != 0LL))) {
            /* pass */
            List_i64_free(a);
            return (-1LL);
        }
        /* pass */
        List_i64_append(a, av);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((rtag < 0LL)) {
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), rtfn, a));
        /* pass */
        return selfv;
    }
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, rtfn, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, rtag);
    /* pass */
    return d;
}

__attribute__((hot)) long long _pack_spawn_args(LModule* m, LFunc* lf, List_ptr* margs, long long start) {
    /* pass */
    long long nargs = (margs->len - start);
    /* pass */
    long long packsz = (nargs * 8LL);
    /* pass */
    if ((packsz < 8LL)) {
        /* pass */
        packsz = 8LL;
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
    /* pass */
    long long szc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
    /* pass */
    List_i64* aa = (void*)List_i64_new();
    /* pass */
    List_i64_append(aa, szc);
    /* pass */
    long long buf = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(buf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
    /* pass */
    LFunc_set_vreg_type(lf, buf, 0LL);
    /* pass */
    long long ai = start;
    /* pass */
    while ((ai < margs->len)) {
        /* pass */
        long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
        /* pass */
        if ((argv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long argt = LFunc_vreg_type(lf, argv);
        /* pass */
        if ((argt == 5LL)) {
            /* pass */
            long long afb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
            /* pass */
            argv = afb;
        } else if ((argt == 1LL)) {
            /* pass */
            _secure_str(m, lf, argv);
        } else if (((argt == 10LL) || (argt == 11LL))) {
            /* pass */
            _secure_obj(m, lf, argv);
        }
        /* pass */
        _emit_field_set(m, lf, buf, ((ai - start) * 8LL), argv);
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    return buf;
}

__attribute__((hot)) long long _lower_pool_spawn(LModule* m, LFunc* lf, long long poolv, List_ptr* margs) {
    /* pass */
    if ((margs->len != 2LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    TrStr pfn = _ident_name(((HirExpr*)List_ptr_get(margs, 0LL)));
    /* pass */
    if (((_is_null_str(pfn) || _tr_str_eqv((pfn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, pfn)))) {
        /* pass */
        _tr_str_release(pfn);
        return (-1LL);
    }
    /* pass */
    if ((!_ensure_await_wrapper(m, pfn))) {
        /* pass */
        _tr_str_release(pfn);
        return (-1LL);
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
    /* pass */
    long long pszc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(pszc, 8LL));
    /* pass */
    List_i64* paa = (void*)List_i64_new();
    /* pass */
    List_i64_append(paa, pszc);
    /* pass */
    long long pbuf = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(pbuf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), paa));
    /* pass */
    LFunc_set_vreg_type(lf, pbuf, 0LL);
    /* pass */
    long long parg = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
    /* pass */
    if (((parg < 0LL) || (LFunc_vreg_type(lf, parg) != 0LL))) {
        /* pass */
        _tr_str_release(pfn);
        return (-1LL);
    }
    /* pass */
    _emit_field_set(m, lf, pbuf, 0LL, parg);
    /* pass */
    long long pfaddr = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, pfaddr, 12LL);
    /* pass */
    ({ TrStr _at_t3241 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (pfn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(pfaddr, _at_t3241)); _tr_str_release(_at_t3241); });
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_threadpool_spawn", 20LL));
    /* pass */
    List_i64* psa = (void*)List_i64_new();
    /* pass */
    List_i64_append(psa, poolv);
    /* pass */
    List_i64_append(psa, pfaddr);
    /* pass */
    List_i64_append(psa, pbuf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_threadpool_spawn", 20LL), psa));
    /* pass */
    _tr_str_release(pfn);
    return poolv;
}

__attribute__((hot)) bool _is_runtime_fn(TrStr fn) {
    /* pass */
    if ((_is_null_str(fn) || (((long long)_tr_str_lenv((fn))) < 4LL))) {
        /* pass */
        return false;
    }
    /* pass */
    return ({ TrStr _wt_t3242 = (_tr_str_wrap(_tr_str_slice((fn).data, 0LL, 4LL))); __auto_type _wr = (_tr_str_eqv(_wt_t3242, (_tr_str_lit_len("_tr_", 4LL)))); _tr_str_release(_wt_t3242); _wr; });
}

__attribute__((hot)) bool _is_rc_str_runtime_fn(TrStr fn) {
    /* pass */
    if ((_is_null_str(fn) || (((long long)_tr_str_lenv((fn))) < 7LL))) {
        /* pass */
        return false;
    }
    /* pass */
    return ({ TrStr _wt_t3243 = (_tr_str_wrap(_tr_str_slice((fn).data, 0LL, 7LL))); __auto_type _wr = (_tr_str_eqv(_wt_t3243, (_tr_str_lit_len("_tr_rt_", 7LL)))); _tr_str_release(_wt_t3243); _wr; });
}

__attribute__((hot)) void _track_mutex_unlock(LFunc* lf, HirExpr* obj) {
    /* pass */
    TrStr nm = _ident_name(obj);
    /* pass */
    if ((_is_null_str(nm) || _tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        _tr_str_release(nm);
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lf->mutex_unlocks->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(lf->mutex_unlocks, i)), (nm))) {
            /* pass */
            _tr_str_release(nm);
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    ({ TrStr _at_t3244 = (_own(nm)); List_TrStr_append(lf->mutex_unlocks, _at_t3244); _tr_str_release(_at_t3244); });
    _tr_str_release(nm);
}

__attribute__((hot)) long long _atomic_delta(LModule* m, LFunc* lf, long long selfv, TrStr rtfn) {
    /* pass */
    LModule_add_extern(m, rtfn);
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, selfv);
    /* pass */
    List_i64_append(a, one);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, rtfn, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, 0LL);
    /* pass */
    return d;
}

__attribute__((hot)) long long _lower_box_method(LModule* m, LFunc* lf, HirExpr* obj, TrStr method, List_ptr* margs) {
    /* pass */
    TrStr btn = _tr_str_retain(hir_expr_type(obj)->name);
    /* pass */
    if (((((((!_tr_str_eqv((btn), (_tr_str_lit_len("Mutex", 5LL)))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("RwLock", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("Atomic", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("ThreadPool", 10LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("ThreadLocal", 11LL)))))) {
        /* pass */
        _tr_str_release(btn);
        return (-2LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) {
        /* pass */
        _tr_str_release(btn);
        return (-2LL);
    }
    /* pass */
    if ((_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL))) && ((_tr_str_eqv((method), (_tr_str_lit_len("id", 2LL))) || _tr_str_eqv((method), (_tr_str_lit_len("current_id", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL)))))) {
        /* pass */
        _tr_str_release(btn);
        return (-2LL);
    }
    /* pass */
    long long bself = lower_expr(m, lf, obj);
    /* pass */
    if ((bself < 0LL)) {
        /* pass */
        _tr_str_release(btn);
        return (-1LL);
    }
    /* pass */
    if (_tr_str_eqv((btn), (_tr_str_lit_len("Mutex", 5LL)))) {
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("lock", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
            /* pass */
            _track_mutex_unlock(lf, obj);
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_lock_get", 21LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_set_unlock", 23LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("unlock", 6LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_unlock", 19LL), margs, 0LL, (-1LL));
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("RwLock", 6LL)))) {
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_read_get", 19LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("read_unlock", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_read_unlock", 22LL), margs, 0LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_write_get", 20LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("write_set", 9LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_write_set_unlock", 27LL), margs, 1LL, (-1LL));
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("Atomic", 6LL)))) {
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("load", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load", 15LL), margs, 0LL, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("store", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store", 16LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_add", 9LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add", 14LL), margs, 1LL, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_sub", 9LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub", 14LL), margs, 1LL, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("compare_and_swap", 16LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas", 14LL), margs, 2LL, 4LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("increment", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("inc", 3LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _atomic_delta(m, lf, bself, _tr_str_lit_len("_tr_atomic_add", 14LL));
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("decrement", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("dec", 3LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _atomic_delta(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub", 14LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_acquire", 12LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_acquire", 23LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_relaxed", 12LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_relaxed", 23LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_seqcst", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_seqcst", 22LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_release", 13LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_release", 24LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_relaxed", 13LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_relaxed", 24LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_seqcst", 12LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_seqcst", 23LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_relaxed", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_relaxed", 22LL), margs, 1LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_release", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_release", 22LL), margs, 1LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_acqrel", 10LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_acqrel", 21LL), margs, 1LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("sub_relaxed", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub_relaxed", 22LL), margs, 1LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("sub_release", 11LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub_release", 22LL), margs, 1LL, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("exchange", 8LL))))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_swap", 15LL), margs, 1LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("cas_weak", 8LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas_weak", 19LL), margs, 2LL, 4LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("cas_acqrel", 10LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas_acqrel", 21LL), margs, 2LL, 4LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_free", 15LL), margs, 0LL, (-1LL));
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("ThreadPool", 10LL)))) {
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("wait", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadpool_wait", 19LL), margs, 0LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadpool_free", 19LL), margs, 0LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _lower_pool_spawn(m, lf, bself, margs);
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL)))) {
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_join", 18LL), margs, 0LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("detach", 6LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_detach", 20LL), margs, 0LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_free", 18LL), margs, 0LL, (-1LL));
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_get", 11LL), margs, 0LL, 0LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_set", 11LL), margs, 1LL, (-1LL));
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
            /* pass */
            _tr_str_release(btn);
            return _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_free", 12LL), margs, 0LL, (-1LL));
        }
    }
    /* pass */
    _tr_str_release(btn);
    return (-1LL);
}

__attribute__((hot)) bool _lower_taskgroup(LModule* m, LFunc* lf, HirBlock* body) {
    /* pass */
    if ((lf->in_taskgroup > 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_tg_begin", 12LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_begin", 12LL), (void*)List_i64_new()));
    /* pass */
    lf->in_taskgroup = (lf->in_taskgroup + 1LL);
    /* pass */
    bool ok = lower_block(m, lf, body);
    /* pass */
    lf->in_taskgroup = (lf->in_taskgroup - 1LL);
    /* pass */
    if ((!ok)) {
        /* pass */
        return false;
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_taskgroup_wait", 18LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_taskgroup_wait", 18LL), (void*)List_i64_new()));
    /* pass */
    return true;
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
    if ((cargs->len > _arg_limit(m))) {
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
    if ((((rtag == 10LL) || (rtag == 11LL)) && LModule_fn_ret_owned(m, mangled))) {
        /* pass */
        _fresh_mark_obj(lf, rd);
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
    if ((lf->defers->len > 0LL)) {
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
    }
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < lf->mutex_unlocks->len)) {
        /* pass */
        TrStr mn = List_TrStr_get(lf->mutex_unlocks, mi);
        /* pass */
        if ((LFunc_var_index(lf, mn) >= 0LL)) {
            /* pass */
            long long ma = LFunc_new_vreg(lf);
            /* pass */
            LFunc_set_vreg_type(lf, ma, 12LL);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IAddrVar(ma, mn));
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_mutexbox_cleanup", 20LL));
            /* pass */
            List_i64* mca = (void*)List_i64_new();
            /* pass */
            List_i64_append(mca, ma);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_mutexbox_cleanup", 20LL), mca));
        }
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(mn);
    }
    /* pass */
    return true;
}

__attribute__((hot)) long long _sizeof_tyname(TrStr n) {
    /* pass */
    if (_is_null_str(n)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))))) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL))))) {
        /* pass */
        return 2LL;
    }
    /* pass */
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("float32", 7LL))))) {
        /* pass */
        return 4LL;
    }
    /* pass */
    return 8LL;
}

__attribute__((hot)) long long _ptr_stride(LModule* m, AstType* pty) {
    /* pass */
    if ((pty->args->len == 0LL)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    TrStr en = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL))))->name);
    /* pass */
    if (_is_null_str(en)) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    if (((((_tr_str_eqv((en), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("isize", 5LL))))) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    if ((_tr_str_eqv((en), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((en), (_tr_str_lit_len("f64", 3LL))))) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    if ((((_tr_str_eqv((en), (_tr_str_lit_len("char", 4LL))) || _tr_str_eqv((en), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("bool", 4LL))))) {
        /* pass */
        _tr_str_release(en);
        return 1LL;
    }
    /* pass */
    if (((_tr_str_eqv((en), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("f32", 3LL))))) {
        /* pass */
        _tr_str_release(en);
        return 4LL;
    }
    /* pass */
    if ((_tr_str_eqv((en), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("String", 6LL))))) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    if (LModule_is_value_class(m, en)) {
        /* pass */
        return LModule_class_size(m, en);
    }
    /* pass */
    long long _pg = _value_struct_generic_size(m, (*((AstType**)List_ptr_get(pty->args, 0LL))));
    /* pass */
    if ((_pg > 0LL)) {
        /* pass */
        _tr_str_release(en);
        return _pg;
    }
    /* pass */
    if ((LModule_is_class(m, en) || LModule_is_enum(m, en))) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    if (_tr_str_eqv((en), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        _tr_str_release(en);
        return 8LL;
    }
    /* pass */
    _tr_str_release(en);
    return 0LL;
}

__attribute__((hot)) AstType* _class_field_ty(LModule* m, TrStr cls, TrStr fld) {
    /* pass */
    TrStr cur = _tr_str_retain(cls);
    /* pass */
    long long depth = 0LL;
    /* pass */
    while (((depth < 32LL) && (!_is_null_str(cur)))) {
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < m->hir_prog->classes->len)) {
            /* pass */
            HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, ci));
            /* pass */
            if (_tr_str_eqv((c->name), (cur))) {
                /* pass */
                long long fi = 0LL;
                /* pass */
                while ((fi < c->fields->len)) {
                    /* pass */
                    if (_tr_str_eqv((((HirField*)List_ptr_get(c->fields, fi))->name), (fld))) {
                        /* pass */
                        _tr_str_release(cur);
                        return ((HirField*)List_ptr_get(c->fields, fi))->ty;
                    }
                    /* pass */
                    fi = (fi + 1LL);
                }
                /* pass */
                if ((c->base_classes->len == 1LL)) {
                    /* pass */
                    TrStr _strtmp_t3246 = ({ TrStr _at_t3245 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_own(_at_t3245)); _tr_str_release(_at_t3245); _wr; });
                    _tr_str_release(cur);
                    cur = _strtmp_t3246;
                } else {
                    /* pass */
                    _tr_str_release(cur);
                    return AstType_init(_tr_str_lit_len("", 0LL));
                }
            }
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        depth = (depth + 1LL);
    }
    /* pass */
    _tr_str_release(cur);
    return AstType_init(_tr_str_lit_len("", 0LL));
}

__attribute__((hot)) AstType* _ptr_chain_ty(LModule* m, HirExpr* e) {
    /* pass */
    __auto_type _t3247 = (*e);
    if (_t3247.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t3247.data.EMethodCall.obj;
__auto_type mname = _t3247.data.EMethodCall.method;
__auto_type mty = _t3247.data.EMethodCall.ty;
        /* pass */
        if (((_tr_str_eqv((mname), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((mname), (_tr_str_lit_len("add", 3LL)))) && _tr_str_eqv((hir_expr_type(mobj)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
            /* pass */
            AstType* inner = _ptr_chain_ty(m, mobj);
            /* pass */
            if ((inner->args->len > 0LL)) {
                /* pass */
                return inner;
            }
        }
        /* pass */
        return mty;
    } else if (_t3247.tag == HirExpr_EPropAccess) {
        __auto_type pobj = _t3247.data.EPropAccess.obj;
__auto_type pfld = _t3247.data.EPropAccess.prop;
__auto_type pty = _t3247.data.EPropAccess.ty;
        /* pass */
        if ((pty->args->len > 0LL)) {
            /* pass */
            return pty;
        }
        /* pass */
        AstType* ft = _class_field_ty(m, hir_expr_type(pobj)->name, pfld);
        /* pass */
        if ((_tr_str_eqv((ft->name), (_tr_str_lit_len("Pointer", 7LL))) && (ft->args->len > 0LL))) {
            /* pass */
            return ft;
        }
        /* pass */
        return pty;
    } else if (1) {
        __auto_type _ = _t3247;
        /* pass */
        return hir_expr_type(e);
    }
}

__attribute__((hot)) TrStr _dunder_for_op(TrStr op) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__add__", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__sub__", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__mul__", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__truediv__", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__mod__", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__pow__", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__floordiv__", 12LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__eq__", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__ne__", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__lt__", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__le__", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("__gt__", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("__ge__", 6LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _stmt_expr_kind(HirExpr* e) {
    /* pass */
    __auto_type _t3248 = (*e);
    if (_t3248.tag == HirExpr_ECall) {
        __auto_type callee = _t3248.data.ECall.callee;
        return ({ TrStr _at_t3249 = (_ident_name(callee)); __auto_type _wr = (({ TrStr _cl = (({ TrStr _cr = (_own(_at_t3249)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("call ", 5LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_cl); _cres; })); _tr_str_release(_at_t3249); _wr; });
    } else if (_t3248.tag == HirExpr_EMethodCall) {
        __auto_type meth = _t3248.data.EMethodCall.method;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(meth)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("method .", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_cl); _cres; });
    } else if (1) {
        __auto_type _ = _t3248;
        return _tr_str_lit_len("expression", 10LL);
    }
}

__attribute__((hot)) TrStr _expr_kind(HirExpr* e) {
    /* pass */
    __auto_type _t3250 = (*e);
    if (_t3250.tag == HirExpr_ELitInt) {
        return _tr_str_lit_len("int literal", 11LL);
    } else if (_t3250.tag == HirExpr_ELitStr) {
        return _tr_str_lit_len("str literal", 11LL);
    } else if (_t3250.tag == HirExpr_ELitBool) {
        return _tr_str_lit_len("bool literal", 12LL);
    } else if (_t3250.tag == HirExpr_ELitChar) {
        return _tr_str_lit_len("char literal", 12LL);
    } else if (_t3250.tag == HirExpr_ELitFloat) {
        return _tr_str_lit_len("float literal", 13LL);
    } else if (_t3250.tag == HirExpr_ERawStr) {
        return _tr_str_lit_len("raw str literal", 15LL);
    } else if (_t3250.tag == HirExpr_EIdent) {
        __auto_type n = _t3250.data.EIdent.name;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(n)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("ident '", 7LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_cl); _cres; });
    } else if (_t3250.tag == HirExpr_ECall) {
        __auto_type callee = _t3250.data.ECall.callee;
        return ({ TrStr _at_t3251 = (_ident_name(callee)); __auto_type _wr = (({ TrStr _cl = (({ TrStr _cr = (_own(_at_t3251)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("call ", 5LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_cl); _cres; })); _tr_str_release(_at_t3251); _wr; });
    } else if (_t3250.tag == HirExpr_EMethodCall) {
        __auto_type meth = _t3250.data.EMethodCall.method;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(meth)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("method .", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_cl); _cres; });
    } else if (_t3250.tag == HirExpr_EIndex) {
        return _tr_str_lit_len("index expr", 10LL);
    } else if (_t3250.tag == HirExpr_EPropAccess) {
        __auto_type p = _t3250.data.EPropAccess.prop;
        return ({ TrStr _cr = (_own(p)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("prop .", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    } else if (_t3250.tag == HirExpr_EBinOp) {
        __auto_type op = _t3250.data.EBinOp.op;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(op)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("binary op '", 11LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_cl); _cres; });
    } else if (_t3250.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3250.data.EUnaryOp.op;
        return ({ TrStr _cl = (({ TrStr _cr = (_own(op)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("unary op '", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_cl); _cres; });
    } else if (_t3250.tag == HirExpr_ECast) {
        __auto_type ctty = _t3250.data.ECast.target_ty;
        return ({ TrStr _cr = (_own(ctty->name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("cast to ", 8LL)), _cr); _tr_str_release(_cr); _cres; });
    } else if (_t3250.tag == HirExpr_EIfElse) {
        return _tr_str_lit_len("if-else expr", 12LL);
    } else if (_t3250.tag == HirExpr_EClosure) {
        return _tr_str_lit_len("closure", 7LL);
    } else if (_t3250.tag == HirExpr_ETuple) {
        return _tr_str_lit_len("tuple", 5LL);
    } else if (_t3250.tag == HirExpr_EList) {
        return _tr_str_lit_len("list literal", 12LL);
    } else if (_t3250.tag == HirExpr_ESet) {
        return _tr_str_lit_len("set literal", 11LL);
    } else if (_t3250.tag == HirExpr_EDict) {
        return _tr_str_lit_len("dict literal", 12LL);
    } else if (_t3250.tag == HirExpr_EFString) {
        return _tr_str_lit_len("f-string", 8LL);
    } else if (_t3250.tag == HirExpr_ESuperMethodCall) {
        __auto_type meth = _t3250.data.ESuperMethodCall.method;
        return ({ TrStr _cr = (_own(meth)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    } else if (_t3250.tag == HirExpr_ESuperPropAccess) {
        __auto_type p = _t3250.data.ESuperPropAccess.prop;
        return ({ TrStr _cr = (_own(p)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    } else if (1) {
        __auto_type _ = _t3250;
        return _tr_str_lit_len("expression", 10LL);
    }
}

__attribute__((hot)) TrStr _stmt_kind(HirStmt* s) {
    /* pass */
    __auto_type _t3252 = (*s);
    if (_t3252.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t3252.data.SLineMarker.n;
        return _tr_str_lit_len("line marker", 11LL);
    } else if (_t3252.tag == HirStmt_SPass) {
        return _tr_str_lit_len("pass", 4LL);
    } else if (_t3252.tag == HirStmt_SAutoDrop) {
        return _tr_str_lit_len("auto-drop", 9LL);
    } else if (_t3252.tag == HirStmt_SFree) {
        __auto_type _ = _t3252.data.SFree.name;
        return _tr_str_lit_len("free", 4LL);
    } else if (_t3252.tag == HirStmt_SReturn) {
        __auto_type _ = _t3252.data.SReturn.val;
        return _tr_str_lit_len("return", 6LL);
    } else if (_t3252.tag == HirStmt_SLet) {
        __auto_type n = _t3252.data.SLet.name;
        return ({ TrStr _cr = (_own(n)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("let ", 4LL)), _cr); _tr_str_release(_cr); _cres; });
    } else if (_t3252.tag == HirStmt_SAssign) {
        return _tr_str_lit_len("assign", 6LL);
    } else if (_t3252.tag == HirStmt_SIf) {
        return _tr_str_lit_len("if", 2LL);
    } else if (_t3252.tag == HirStmt_SWhile) {
        return _tr_str_lit_len("while", 5LL);
    } else if (_t3252.tag == HirStmt_SBreak) {
        __auto_type _ = _t3252.data.SBreak.val;
        return _tr_str_lit_len("break", 5LL);
    } else if (_t3252.tag == HirStmt_SContinue) {
        return _tr_str_lit_len("continue", 8LL);
    } else if (_t3252.tag == HirStmt_SAssert) {
        return _tr_str_lit_len("assert", 6LL);
    } else if (_t3252.tag == HirStmt_SMultiLet) {
        return _tr_str_lit_len("multi-let", 9LL);
    } else if (_t3252.tag == HirStmt_SDefer) {
        __auto_type _ = _t3252.data.SDefer.stmt;
        return _tr_str_lit_len("defer", 5LL);
    } else if (_t3252.tag == HirStmt_SWith) {
        return _tr_str_lit_len("with", 4LL);
    } else if (_t3252.tag == HirStmt_STry) {
        return _tr_str_lit_len("try", 3LL);
    } else if (_t3252.tag == HirStmt_SRaise) {
        __auto_type _ = _t3252.data.SRaise.val;
        return _tr_str_lit_len("raise", 5LL);
    } else if (_t3252.tag == HirStmt_SMatch) {
        return _tr_str_lit_len("match", 5LL);
    } else if (_t3252.tag == HirStmt_SFor) {
        return _tr_str_lit_len("for", 3LL);
    } else if (_t3252.tag == HirStmt_SForUnpack) {
        return _tr_str_lit_len("for-unpack", 10LL);
    } else if (_t3252.tag == HirStmt_SExpr) {
        __auto_type _ = _t3252.data.SExpr.expr;
        return _tr_str_lit_len("expr", 4LL);
    } else if (_t3252.tag == HirStmt_SUnsafe) {
        __auto_type _ = _t3252.data.SUnsafe.body;
        return _tr_str_lit_len("unsafe", 6LL);
    } else if (1) {
        __auto_type _ = _t3252;
        return _tr_str_lit_len("statement", 9LL);
    }
}

__attribute__((hot)) bool _stmt_has_cf(HirStmt* s) {
    /* pass */
    __auto_type _t3253 = (*s);
    if (_t3253.tag == HirStmt_SReturn) {
        __auto_type _ = _t3253.data.SReturn.val;
        return true;
    } else if (_t3253.tag == HirStmt_SBreak) {
        __auto_type _ = _t3253.data.SBreak.val;
        return true;
    } else if (_t3253.tag == HirStmt_SContinue) {
        return true;
    } else if (_t3253.tag == HirStmt_SIf) {
        __auto_type tb = _t3253.data.SIf.then_b;
__auto_type eb = _t3253.data.SIf.else_b;
        return (_block_has_cf(tb) || _block_has_cf(eb));
    } else if (_t3253.tag == HirStmt_SWhile) {
        __auto_type wb = _t3253.data.SWhile.body;
        return _block_has_cf(wb);
    } else if (_t3253.tag == HirStmt_SFor) {
        __auto_type fb = _t3253.data.SFor.body;
        return _block_has_cf(fb);
    } else if (_t3253.tag == HirStmt_SForUnpack) {
        __auto_type fb = _t3253.data.SForUnpack.body;
        return _block_has_cf(fb);
    } else if (_t3253.tag == HirStmt_SMatch) {
        __auto_type arms = _t3253.data.SMatch.arms;
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < arms->len)) {
            /* pass */
            if (_block_has_cf(((HirMatchArm*)List_ptr_get(arms, j))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        return false;
    } else if (_t3253.tag == HirStmt_SWith) {
        __auto_type wb = _t3253.data.SWith.body;
        return _block_has_cf(wb);
    } else if (_t3253.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t3253.data.SUnsafe.body;
        return _block_has_cf(ub);
    } else if (_t3253.tag == HirStmt_SDefer) {
        __auto_type ds = _t3253.data.SDefer.stmt;
        return _stmt_has_cf(ds);
    } else if (_t3253.tag == HirStmt_STry) {
        __auto_type tb = _t3253.data.STry.try_body;
__auto_type cs = _t3253.data.STry.catches;
__auto_type fb = _t3253.data.STry.finally_b;
        /* pass */
        if ((_block_has_cf(tb) || _block_has_cf(fb))) {
            /* pass */
            return true;
        }
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < cs->len)) {
            /* pass */
            if (_block_has_cf((*((HirCatchClause**)List_ptr_get(cs, k)))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t3253;
        return false;
    }
}

__attribute__((hot)) bool _block_has_cf(HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if (_stmt_has_cf(((HirStmt*)List_ptr_get(b->stmts, i)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _lower_try_setjmp(LModule* m, LFunc* lf, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b) {
    /* pass */
    bool has_catch = (catches->len == 1LL);
    /* pass */
    TrStr ename = _tr_str_lit_len("__exmsg", 7LL);
    /* pass */
    if (has_catch) {
        /* pass */
        HirCatchClause* tcc0 = (*((HirCatchClause**)List_ptr_get(catches, 0LL)));
        /* pass */
        if (((!_is_null_str(tcc0->err_name)) && (!_tr_str_eqv((tcc0->err_name), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            TrStr _strtmp_t3254 = _tr_str_retain(tcc0->err_name);
            _tr_str_release(ename);
            ename = _strtmp_t3254;
        }
    }
    /* pass */
    List_TrStr* capn = (void*)List_TrStr_new();
    /* pass */
    List_i64* capt = (void*)List_i64_new();
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < lf->vars->len)) {
        /* pass */
        TrStr vn = List_TrStr_get(lf->vars, vi);
        /* pass */
        ({ TrStr _at_t3255 = (_own(vn)); List_TrStr_append(capn, _at_t3255); _tr_str_release(_at_t3255); });
        /* pass */
        List_i64_append(capt, LFunc_var_type(lf, vn));
        /* pass */
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
    /* pass */
    TrStr tname = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_trytry_", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lf->name)); _tr_str_release(_cl); _cres; });
    /* pass */
    LFunc* tlf = ({ TrStr _at_t3256 = (_own(tname)); __auto_type _wr = (LFunc_init(_at_t3256)); _tr_str_release(_at_t3256); _wr; });
    /* pass */
    List_TrStr_append(tlf->params, _tr_str_lit_len("__env", 5LL));
    /* pass */
    LFunc_add_var(tlf, _tr_str_lit_len("__env", 5LL));
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < capn->len)) {
        /* pass */
        ({ TrStr _at_t3257 = (List_TrStr_get(capn, ci)); TrStr _at_t3258 = (_own(_at_t3257)); List_TrStr_append(tlf->captures, _at_t3258); _tr_str_release(_at_t3257); _tr_str_release(_at_t3258); });
        /* pass */
        List_i64_append(tlf->cap_tags, List_i64_get(capt, ci));
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    ({ TrStr _at_t3259 = (_own(tname)); List_TrStr_append(m->fn_names, _at_t3259); _tr_str_release(_at_t3259); });
    /* pass */
    List_i64_append(m->fn_ret, 0LL);
    /* pass */
    LFunc_set_cur(tlf, LFunc_new_block(tlf));
    /* pass */
    if ((!lower_block(m, tlf, try_body))) {
        /* pass */
        _tr_str_release(ename);
        List_TrStr_free(capn);
        List_i64_free(capt);
        _tr_str_release(tname);
        _tr_obj_release(tlf, _trdrop_LFunc);
        return false;
    }
    /* pass */
    LFunc_set_term(tlf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(tlf));
    /* pass */
    TrStr cname = _tr_str_lit_len("", 0LL);
    /* pass */
    if (has_catch) {
        /* pass */
        TrStr _strtmp_t3260 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_trycatch_", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lf->name)); _tr_str_release(_cl); _cres; });
        _tr_str_release(cname);
        cname = _strtmp_t3260;
        /* pass */
        LFunc* clf = ({ TrStr _at_t3261 = (_own(cname)); __auto_type _wr = (LFunc_init(_at_t3261)); _tr_str_release(_at_t3261); _wr; });
        /* pass */
        List_TrStr_append(clf->params, _tr_str_lit_len("__env", 5LL));
        /* pass */
        LFunc_add_var(clf, _tr_str_lit_len("__env", 5LL));
        /* pass */
        ({ TrStr _at_t3262 = (_own(ename)); List_TrStr_append(clf->params, _at_t3262); _tr_str_release(_at_t3262); });
        /* pass */
        ({ TrStr _at_t3263 = (_own(ename)); LFunc_add_var(clf, _at_t3263); _tr_str_release(_at_t3263); });
        /* pass */
        LFunc_set_var_type(clf, ename, 1LL);
        /* pass */
        long long ci2 = 0LL;
        /* pass */
        while ((ci2 < capn->len)) {
            /* pass */
            ({ TrStr _at_t3264 = (List_TrStr_get(capn, ci2)); TrStr _at_t3265 = (_own(_at_t3264)); List_TrStr_append(clf->captures, _at_t3265); _tr_str_release(_at_t3264); _tr_str_release(_at_t3265); });
            /* pass */
            List_i64_append(clf->cap_tags, List_i64_get(capt, ci2));
            /* pass */
            ci2 = (ci2 + 1LL);
        }
        /* pass */
        ({ TrStr _at_t3266 = (_own(cname)); List_TrStr_append(m->fn_names, _at_t3266); _tr_str_release(_at_t3266); });
        /* pass */
        List_i64_append(m->fn_ret, 0LL);
        /* pass */
        LFunc_set_cur(clf, LFunc_new_block(clf));
        /* pass */
        if ((!lower_block(m, clf, (*((HirCatchClause**)List_ptr_get(catches, 0LL)))->body))) {
            /* pass */
            _tr_str_release(ename);
            List_TrStr_free(capn);
            List_i64_free(capt);
            _tr_str_release(tname);
            _tr_obj_release(tlf, _trdrop_LFunc);
            _tr_str_release(cname);
            _tr_obj_release(clf, _trdrop_LFunc);
            return false;
        }
        /* pass */
        LFunc_set_term(clf, LTerm_ctor_TRetInt(0LL));
        /* pass */
        List_ptr_append(m->funcs, _tr_obj_retain(clf));
    }
    /* pass */
    long long envsz = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(envsz, ((1LL + capn->len) * 8LL)));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
    /* pass */
    List_i64* ea = (void*)List_i64_new();
    /* pass */
    List_i64_append(ea, envsz);
    /* pass */
    long long env = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(env, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), ea));
    /* pass */
    LFunc_set_vreg_type(lf, env, 12LL);
    /* pass */
    long long ci3 = 0LL;
    /* pass */
    while ((ci3 < capn->len)) {
        /* pass */
        long long av = LFunc_new_vreg(lf);
        /* pass */
        ({ TrStr _at_t3267 = (List_TrStr_get(capn, ci3)); LFunc_emit(lf, LInst_ctor_IAddrVar(av, _at_t3267)); _tr_str_release(_at_t3267); });
        /* pass */
        LFunc_set_vreg_type(lf, av, 12LL);
        /* pass */
        _emit_field_set(m, lf, env, ((1LL + ci3) * 8LL), av);
        /* pass */
        ci3 = (ci3 + 1LL);
    }
    /* pass */
    long long tfa = LFunc_new_vreg(lf);
    /* pass */
    ({ TrStr _at_t3268 = (_own(tname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(tfa, _at_t3268)); _tr_str_release(_at_t3268); });
    /* pass */
    LFunc_set_vreg_type(lf, tfa, 12LL);
    /* pass */
    long long cfa = LFunc_new_vreg(lf);
    /* pass */
    if (has_catch) {
        /* pass */
        ({ TrStr _at_t3269 = (_own(cname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(cfa, _at_t3269)); _tr_str_release(_at_t3269); });
    } else {
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(cfa, 0LL));
    }
    /* pass */
    LFunc_set_vreg_type(lf, cfa, 12LL);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_try_catch", 16LL));
    /* pass */
    List_i64* ta = (void*)List_i64_new();
    /* pass */
    List_i64_append(ta, tfa);
    /* pass */
    List_i64_append(ta, cfa);
    /* pass */
    List_i64_append(ta, env);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_try_catch", 16LL), ta));
    /* pass */
    _release_obj(m, lf, env);
    /* pass */
    if ((!lower_block(m, lf, finally_b))) {
        /* pass */
        _tr_str_release(ename);
        List_TrStr_free(capn);
        List_i64_free(capt);
        _tr_str_release(tname);
        _tr_obj_release(tlf, _trdrop_LFunc);
        _tr_str_release(cname);
        return false;
    }
    /* pass */
    _tr_str_release(ename);
    List_TrStr_free(capn);
    List_i64_free(capt);
    _tr_str_release(tname);
    _tr_obj_release(tlf, _trdrop_LFunc);
    _tr_str_release(cname);
    return true;
}

__attribute__((hot)) bool _is_vstruct_lvalue(HirExpr* e) {
    /* pass */
    __auto_type _t3270 = (*e);
    if (_t3270.tag == HirExpr_EIdent) {
        return true;
    } else if (_t3270.tag == HirExpr_EPropAccess) {
        return true;
    } else if (1) {
        __auto_type _ = _t3270;
        return false;
    }
}

__attribute__((hot)) long long _copy_value_struct(LModule* m, LFunc* lf, long long src, TrStr cls) {
    /* pass */
    long long ci = LModule_class_index(m, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    ClassLayout* lay = ((ClassLayout*)List_ptr_get(m->classes, ci));
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < lay->ftags->len)) {
        /* pass */
        long long ft = List_i64_get(lay->ftags, fi);
        /* pass */
        if (((((((ft != 0LL) && (ft != 4LL)) && (ft != 5LL)) && (ft != 1LL)) && (ft != 10LL)) && (ft != 11LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long szc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_class_size(m, cls)));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
    /* pass */
    List_i64* aa = (void*)List_i64_new();
    /* pass */
    List_i64_append(aa, szc);
    /* pass */
    long long dst = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(dst, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), aa));
    /* pass */
    LFunc_set_vreg_type(lf, dst, 10LL);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_c_memcpy", 12LL));
    /* pass */
    List_i64* cpa = (void*)List_i64_new();
    /* pass */
    List_i64_append(cpa, dst);
    /* pass */
    List_i64_append(cpa, src);
    /* pass */
    List_i64_append(cpa, szc);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_c_memcpy", 12LL), cpa));
    /* pass */
    _attach_class_drop(m, lf, dst, cls);
    /* pass */
    long long fi2 = 0LL;
    /* pass */
    while ((fi2 < lay->ftags->len)) {
        /* pass */
        long long ft2 = List_i64_get(lay->ftags, fi2);
        /* pass */
        if ((((ft2 == 1LL) || (ft2 == 10LL)) || (ft2 == 11LL))) {
            /* pass */
            long long fv = _emit_field_get(m, lf, dst, (fi2 * 8LL), ft2);
            /* pass */
            if ((ft2 == 1LL)) {
                /* pass */
                _retain_str(m, lf, fv);
            } else {
                /* pass */
                _retain_obj(m, lf, fv);
            }
        }
        /* pass */
        fi2 = (fi2 + 1LL);
    }
    /* pass */
    _fresh_mark_obj(lf, dst);
    /* pass */
    return dst;
}

__attribute__((hot)) long long _value_struct_generic_size(LModule* m, AstType* ty) {
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < m->hir_prog->classes->len)) {
        /* pass */
        HirClass* gc = ((HirClass*)List_ptr_get(m->hir_prog->classes, gi));
        /* pass */
        if (((_tr_str_eqv((gc->name), (ty->name)) && (gc->generics->len > 0LL)) && (!gc->is_class))) {
            /* pass */
            long long n = gc->fields->len;
            /* pass */
            if ((n < 1LL)) {
                /* pass */
                n = 1LL;
            }
            /* pass */
            return (n * 8LL);
        }
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) bool _lower_stmt_impl(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t3271 = (*s);
    if (_t3271.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t3271.data.SLineMarker.n;
        return true;
    } else if (_t3271.tag == HirStmt_SPass) {
        return true;
    } else if (_t3271.tag == HirStmt_SAutoDrop) {
        __auto_type name = _t3271.data.SAutoDrop.name;
        /* pass */
        if (_tr_str_eqv((name), (_tr_str_lit_len("_", 1LL)))) {
            /* pass */
            return true;
        }
        /* pass */
        if (((LFunc_var_index(lf, name) >= 0LL) && (!_is_param(lf, name)))) {
            /* pass */
            long long dvt = LFunc_var_type(lf, name);
            /* pass */
            if ((dvt == 1LL)) {
                /* pass */
                long long dv = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ILoadVar(dv, name));
                /* pass */
                _release_str(m, lf, dv);
            } else if (((dvt == 10LL) || (dvt == 11LL))) {
                /* pass */
                long long ov = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ILoadVar(ov, name));
                /* pass */
                _release_obj(m, lf, ov);
            }
        }
        /* pass */
        return true;
    } else if (_t3271.tag == HirStmt_SFree) {
        __auto_type _ = _t3271.data.SFree.name;
        return true;
    } else if (_t3271.tag == HirStmt_SReturn) {
        __auto_type val = _t3271.data.SReturn.val;
        /* pass */
        if (lf->in_defer) {
            /* pass */
            return false;
        }
        /* pass */
        if ((lf->is_throws && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
            /* pass */
            long long orv = lower_expr(m, lf, val);
            /* pass */
            if ((orv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            long long okr = _wrap_result(m, lf, 0LL, orv, lf->throws_ok_tag);
            /* pass */
            _fresh_take_obj(lf, okr);
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            if ((!_run_defers(m, lf))) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetVal(okr));
            /* pass */
            return true;
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
            if (((LFunc_vreg_type(lf, rv) == 10LL) || (LFunc_vreg_type(lf, rv) == 11LL))) {
                /* pass */
                if ((!_is_owned_local_return(lf, val))) {
                    /* pass */
                    _secure_obj(m, lf, rv);
                } else {
                    /* pass */
                    _fresh_take_obj(lf, rv);
                }
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
    } else if (_t3271.tag == HirStmt_SLet) {
        __auto_type name = _t3271.data.SLet.name;
__auto_type ty = _t3271.data.SLet.ty;
__auto_type val = _t3271.data.SLet.val;
        /* pass */
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
            /* pass */
            if ((((m->target_llvm && _tr_str_eqv((ty->name), (_tr_str_lit_len("Array", 5LL)))) && (ty->array_size > 0LL)) && (ty->args->len > 0LL))) {
                /* pass */
                long long aetag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
                /* pass */
                if (((aetag != 0LL) && (aetag != 4LL))) {
                    /* pass */
                    return false;
                }
                /* pass */
                long long alen = ty->array_size;
                /* pass */
                LFunc_add_var(lf, name);
                /* pass */
                LFunc_set_var_type(lf, name, aetag);
                /* pass */
                LFunc_set_var_arr(lf, name, alen);
                /* pass */
                long long zc = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
                /* pass */
                long long zi = 0LL;
                /* pass */
                while ((zi < alen)) {
                    /* pass */
                    long long zidx = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IConst(zidx, zi));
                    /* pass */
                    long long zaddr = _array_elem_addr(m, lf, name, zidx);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IStore(zaddr, 0LL, zc, 2LL));
                    /* pass */
                    zi = (zi + 1LL);
                }
                /* pass */
                return true;
            }
            /* pass */
            long long dtag = _tag_of(m, ty);
            /* pass */
            if ((((dtag != 0LL) && (dtag != 4LL)) && (dtag != 5LL))) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_add_var(lf, name);
            /* pass */
            LFunc_set_var_type(lf, name, dtag);
            /* pass */
            return true;
        }
        /* pass */
        long long v = lower_expr(m, lf, val);
        /* pass */
        if ((v < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long vtag = LFunc_vreg_type(lf, v);
        /* pass */
        if (((((vtag == 2LL) || (vtag == 3LL)) || (vtag == 14LL)) || (vtag == 19LL))) {
            /* pass */
            long long ann_lt = _list_tag_from_ann(m, ty);
            /* pass */
            if (((ann_lt >= 0LL) && (ann_lt != vtag))) {
                /* pass */
                LFunc_set_vreg_type(lf, v, ann_lt);
                /* pass */
                vtag = ann_lt;
            }
        } else if (_is_dict_tag(vtag)) {
            /* pass */
            long long ann_dt = _dict_tag_from_ann(m, ty);
            /* pass */
            if (((ann_dt >= 0LL) && (ann_dt != vtag))) {
                /* pass */
                LFunc_set_vreg_type(lf, v, ann_dt);
                /* pass */
                vtag = ann_dt;
            }
        } else if (((vtag == 4LL) && _is_int_typename(ty->name))) {
            /* pass */
            LFunc_set_vreg_type(lf, v, 0LL);
            /* pass */
            vtag = 0LL;
        } else if (((vtag == 0LL) && _tr_str_eqv((ty->name), (_tr_str_lit_len("bool", 4LL))))) {
            /* pass */
            long long nbv = _norm_bool(lf, v);
            /* pass */
            v = nbv;
            /* pass */
            vtag = 4LL;
        }
        /* pass */
        if ((vtag == 1LL)) {
            /* pass */
            _secure_str(m, lf, v);
        }
        /* pass */
        TrStr _vs_cls = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((vtag == 10LL)) {
            /* pass */
            TrStr _strtmp_t3272 = _val_obj_cls(m, lf, val);
            _tr_str_release(_vs_cls);
            _vs_cls = _strtmp_t3272;
        }
        /* pass */
        bool _is_vstruct = (((vtag == 10LL) && (!_is_null_str(_vs_cls))) && LModule_is_value_class(m, _vs_cls));
        /* pass */
        if ((_is_vstruct && _is_vstruct_lvalue(val))) {
            /* pass */
            long long _cpv = _copy_value_struct(m, lf, v, _vs_cls);
            /* pass */
            if ((_cpv < 0LL)) {
                /* pass */
                _tr_str_release(_vs_cls);
                return false;
            }
            /* pass */
            v = _cpv;
            /* pass */
            LFunc_set_vreg_type(lf, v, 10LL);
        }
        /* pass */
        if (((vtag == 10LL) || (vtag == 11LL))) {
            /* pass */
            _secure_obj(m, lf, v);
        }
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        LFunc_set_var_type(lf, name, vtag);
        /* pass */
        if (((LFunc_vreg_type(lf, v) == 10LL) || (LFunc_vreg_type(lf, v) == 11LL))) {
            /* pass */
            TrStr lvcn = _val_obj_cls(m, lf, val);
            /* pass */
            if ((((!_is_null_str(lvcn)) && (!_tr_str_eqv((lvcn), (_tr_str_lit_len("", 0LL))))) && (LModule_is_class(m, lvcn) || LModule_is_enum(m, lvcn)))) {
                /* pass */
                ({ TrStr _at_t3273 = (_own(lvcn)); LFunc_set_var_cls(lf, name, _at_t3273); _tr_str_release(_at_t3273); });
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
        _tr_str_release(_vs_cls);
        return true;
    } else if (_t3271.tag == HirStmt_SAssign) {
        __auto_type target = _t3271.data.SAssign.target;
__auto_type val = _t3271.data.SAssign.val;
        /* pass */
        __auto_type _t3274 = (*target);
        if (_t3274.tag == HirExpr_EMethodCall) {
            __auto_type mobj = _t3274.data.EMethodCall.obj;
__auto_type mmeth = _t3274.data.EMethodCall.method;
__auto_type midx = _t3274.data.EMethodCall.args;
            /* pass */
            if ((_tr_str_eqv((mmeth), (_tr_str_lit_len("get_index", 9LL))) && (midx->len == 1LL))) {
                /* pass */
                return _lower_index_set(m, lf, mobj, ((HirExpr*)List_ptr_get(midx, 0LL)), val);
            }
            /* pass */
            return false;
        } else if (_t3274.tag == HirExpr_EIndex) {
            __auto_type iobj = _t3274.data.EIndex.obj;
__auto_type iidx = _t3274.data.EIndex._tr_v_index;
            /* pass */
            return _lower_index_set(m, lf, iobj, iidx, val);
        } else if (_t3274.tag == HirExpr_EPropAccess) {
            __auto_type pobj = _t3274.data.EPropAccess.obj;
__auto_type pprop = _t3274.data.EPropAccess.prop;
            /* pass */
            return _lower_field_set(m, lf, pobj, pprop, val);
        } else if (1) {
            __auto_type _ = _t3274;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr tn = _ident_name(target);
        /* pass */
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
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
            LFunc_emit(lf, LInst_ctor_ILoadVar(wenv, _tr_str_lit_len("__env", 5LL)));
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
        long long oldot = 0LL;
        /* pass */
        if ((LFunc_var_index(lf, tn) >= 0LL)) {
            /* pass */
            oldot = LFunc_var_type(lf, tn);
        }
        /* pass */
        bool had_old_obj = ((oldot == 10LL) || (oldot == 11LL));
        /* pass */
        if ((LFunc_vreg_type(lf, v2) == 1LL)) {
            /* pass */
            _secure_str(m, lf, v2);
        }
        /* pass */
        if ((LFunc_vreg_type(lf, v2) == 10LL)) {
            /* pass */
            TrStr _av_cls = _val_obj_cls(m, lf, val);
            /* pass */
            if ((((!_is_null_str(_av_cls)) && LModule_is_value_class(m, _av_cls)) && _is_vstruct_lvalue(val))) {
                /* pass */
                long long _acp = _copy_value_struct(m, lf, v2, _av_cls);
                /* pass */
                if ((_acp < 0LL)) {
                    /* pass */
                    _tr_str_release(tn);
                    _tr_str_release(_av_cls);
                    return false;
                }
                /* pass */
                v2 = _acp;
                /* pass */
                LFunc_set_vreg_type(lf, v2, 10LL);
            }
        }
        /* pass */
        if (((LFunc_vreg_type(lf, v2) == 10LL) || (LFunc_vreg_type(lf, v2) == 11LL))) {
            /* pass */
            _secure_obj(m, lf, v2);
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
        if (had_old_obj) {
            /* pass */
            long long oldov = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(oldov, tn));
            /* pass */
            _release_obj(m, lf, oldov);
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
    } else if (_t3271.tag == HirStmt_SIf) {
        __auto_type cond = _t3271.data.SIf.cond;
__auto_type then_b = _t3271.data.SIf.then_b;
__auto_type else_b = _t3271.data.SIf.else_b;
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
    } else if (_t3271.tag == HirStmt_SWhile) {
        __auto_type cond = _t3271.data.SWhile.cond;
__auto_type body = _t3271.data.SWhile.body;
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
    } else if (_t3271.tag == HirStmt_SBreak) {
        __auto_type bval = _t3271.data.SBreak.val;
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
    } else if (_t3271.tag == HirStmt_SContinue) {
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
    } else if (_t3271.tag == HirStmt_SAssert) {
        __auto_type acond = _t3271.data.SAssert.cond;
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_assert_fail", 18LL));
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_assert_fail", 18LL), (void*)List_i64_new()));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(a_ok));
        /* pass */
        LFunc_set_cur(lf, a_ok);
        /* pass */
        return true;
    } else if (_t3271.tag == HirStmt_SMultiLet) {
        __auto_type mnames = _t3271.data.SMultiLet.names;
__auto_type mval = _t3271.data.SMultiLet.val;
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
        if (((mty->args->len != mnames->len) && (mty->args->len != 0LL))) {
            /* pass */
            return false;
        }
        /* pass */
        bool use_decl = (mty->args->len == mnames->len);
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < mnames->len)) {
            /* pass */
            long long mtag = 0LL;
            /* pass */
            TrStr mcls = _tr_str_lit_len("", 0LL);
            /* pass */
            if (use_decl) {
                /* pass */
                AstType* dty = (*((AstType**)List_ptr_get(mty->args, mi)));
                /* pass */
                mtag = _tag_of(m, dty);
                /* pass */
                if (((((mtag < 0LL) || _is_list_tag(mtag)) || _is_dict_tag(mtag)) || _is_set_tag(mtag))) {
                    /* pass */
                    _tr_str_release(mcls);
                    return false;
                }
                /* pass */
                if (((mtag == 10LL) || (mtag == 11LL))) {
                    /* pass */
                    TrStr _strtmp_t3275 = _cls_of_ty(m, dty);
                    _tr_str_release(mcls);
                    mcls = _strtmp_t3275;
                }
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
                /* pass */
                if (((mtag == 10LL) || (mtag == 11LL))) {
                    /* pass */
                    _secure_obj(m, lf, mv);
                }
            }
            /* pass */
            TrStr mnm = List_TrStr_get(mnames, mi);
            /* pass */
            LFunc_add_var(lf, mnm);
            /* pass */
            LFunc_set_var_type(lf, mnm, mtag);
            /* pass */
            if ((((mtag == 10LL) || (mtag == 11LL)) && (!_tr_str_eqv((mcls), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                LFunc_set_var_cls(lf, mnm, mcls);
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(mnm, mv));
            /* pass */
            mi = (mi + 1LL);
            _tr_str_release(mcls);
            _tr_str_release(mnm);
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        return true;
    } else if (_t3271.tag == HirStmt_SDefer) {
        __auto_type dstmt = _t3271.data.SDefer.stmt;
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
    } else if (_t3271.tag == HirStmt_SWith) {
        __auto_type witems = _t3271.data.SWith.items;
__auto_type waliases = _t3271.data.SWith.aliases;
__auto_type wbody = _t3271.data.SWith.body;
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
        if ((_tr_str_eqv((wcls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, wcls)))) {
            /* pass */
            _tr_str_release(wcls);
            return false;
        }
        /* pass */
        TrStr w_enter = LModule_resolve_method(m, wcls, _tr_str_lit_len("__enter__", 9LL));
        /* pass */
        TrStr w_exit = LModule_resolve_method(m, wcls, _tr_str_lit_len("__exit__", 8LL));
        /* pass */
        if ((_tr_str_eqv((w_enter), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((w_exit), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            _tr_str_release(wcls);
            _tr_str_release(w_enter);
            _tr_str_release(w_exit);
            return false;
        }
        /* pass */
        TrStr w_var = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__with", 6LL)), _cr); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, w_var);
        /* pass */
        LFunc_set_var_type(lf, w_var, 10LL);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(w_var, wctx));
        /* pass */
        bool w_owns = _fresh_take_obj(lf, wctx);
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
        if (({ TrStr _at_t3276 = (List_TrStr_get(waliases, 0LL)); __auto_type _wr = ((((waliases->len > 0LL) && (!_is_null_str(_at_t3276))) && (!_tr_str_eqv((List_TrStr_get(waliases, 0LL)), (_tr_str_lit_len("", 0LL)))))); _tr_str_release(_at_t3276); _wr; })) {
            /* pass */
            TrStr w_an = List_TrStr_get(waliases, 0LL);
            /* pass */
            LFunc_add_var(lf, w_an);
            /* pass */
            LFunc_set_var_type(lf, w_an, LFunc_vreg_type(lf, w_rd));
            /* pass */
            if (((LFunc_vreg_type(lf, w_rd) == 10LL) || (LFunc_vreg_type(lf, w_rd) == 11LL))) {
                /* pass */
                ({ TrStr _at_t3277 = (_own(wcls)); LFunc_set_var_cls(lf, w_an, _at_t3277); _tr_str_release(_at_t3277); });
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
            LFunc_emit(lf, LInst_ctor_IStr(w_es, LModule_add_string(m, _tr_str_lit_len("", 0LL), 0LL)));
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
        if (w_owns) {
            /* pass */
            _release_obj(m, lf, w_c2);
        }
        /* pass */
        _tr_str_release(wcls);
        _tr_str_release(w_enter);
        _tr_str_release(w_exit);
        _tr_str_release(w_var);
        return true;
    } else if (_t3271.tag == HirStmt_STry) {
        __auto_type try_body = _t3271.data.STry.try_body;
__auto_type catches = _t3271.data.STry.catches;
__auto_type finally_b = _t3271.data.STry.finally_b;
        /* pass */
        if ((catches->len == 0LL)) {
            /* pass */
            if (((!_block_has_cf(try_body)) && (!_block_has_cf(finally_b)))) {
                /* pass */
                return _lower_try_setjmp(m, lf, try_body, catches, finally_b);
            }
            /* pass */
            return false;
        }
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
            if ((((!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("str", 3LL)))))) {
                /* pass */
                return false;
            }
        }
        /* pass */
        if ((((!_block_has_cf(try_body)) && (!_block_has_cf(tcc->body))) && (!_block_has_cf(finally_b)))) {
            /* pass */
            return _lower_try_setjmp(m, lf, try_body, catches, finally_b);
        }
        /* pass */
        long long t_exc = LFunc_new_block(lf);
        /* pass */
        long long t_end = LFunc_new_block(lf);
        /* pass */
        TrStr t_msg = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__exmsg", 7LL)), _cr); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, t_msg);
        /* pass */
        LFunc_set_var_type(lf, t_msg, 1LL);
        /* pass */
        long long t_seed = _heap_lit(m, lf, _tr_str_lit_len("", 0LL));
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
        if (((!_is_null_str(tcc->err_name)) && (!_tr_str_eqv((tcc->err_name), (_tr_str_lit_len("", 0LL)))))) {
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
    } else if (_t3271.tag == HirStmt_SRaise) {
        __auto_type rval = _t3271.data.SRaise.val;
        /* pass */
        if ((((lf->try_blks->len == 0LL) && lf->is_throws) && (((unsigned long long)(rval)) != ((unsigned long long)(0LL))))) {
            /* pass */
            long long erv = lower_expr(m, lf, rval);
            /* pass */
            if ((erv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            long long errr = _wrap_result(m, lf, 1LL, erv, lf->throws_err_tag);
            /* pass */
            _fresh_take_obj(lf, errr);
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            if ((!_run_defers(m, lf))) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetVal(errr));
            /* pass */
            LFunc_set_cur(lf, LFunc_new_block(lf));
            /* pass */
            return true;
        }
        /* pass */
        if ((lf->try_blks->len == 0LL)) {
            /* pass */
            if ((((unsigned long long)(rval)) == ((unsigned long long)(0LL)))) {
                /* pass */
                return false;
            }
            /* pass */
            long long pv = lower_expr(m, lf, rval);
            /* pass */
            if ((pv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            if ((LFunc_vreg_type(lf, pv) != 1LL)) {
                /* pass */
                return false;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_exc_raise", 13LL));
            /* pass */
            List_i64* pa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pa, pv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_exc_raise", 13LL), pa));
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            return true;
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
            ({ TrStr _at_t3278 = (List_TrStr_get(lf->try_msgs, (lf->try_msgs->len - 1LL))); LFunc_emit(lf, LInst_ctor_IStoreVar(_at_t3278, rv)); _tr_str_release(_at_t3278); });
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->try_blks, (lf->try_blks->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t3271.tag == HirStmt_SMatch) {
        __auto_type mexpr = _t3271.data.SMatch.expr;
__auto_type marms = _t3271.data.SMatch.arms;
        /* pass */
        return _lower_match(m, lf, mexpr, marms);
    } else if (_t3271.tag == HirStmt_SFor) {
        __auto_type var = _t3271.data.SFor.var;
__auto_type iter = _t3271.data.SFor.iter;
__auto_type body = _t3271.data.SFor.body;
        /* pass */
        return _lower_for(m, lf, var, iter, body);
    } else if (_t3271.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t3271.data.SForUnpack.vars;
__auto_type iter = _t3271.data.SForUnpack.iter;
__auto_type body = _t3271.data.SForUnpack.body;
        /* pass */
        return _lower_for_unpack(m, lf, vars, iter, body);
    } else if (_t3271.tag == HirStmt_SExpr) {
        __auto_type e = _t3271.data.SExpr.expr;
        /* pass */
        bool se_ok = lower_expr_stmt(m, lf, e);
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        if (((!se_ok) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cr = (_stmt_expr_kind(e)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("unsupported expression statement (", 34LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
        }
        /* pass */
        return se_ok;
    } else if (_t3271.tag == HirStmt_SUnsafe) {
        __auto_type ubody = _t3271.data.SUnsafe.body;
        /* pass */
        return lower_block(m, lf, ubody);
    } else if (_t3271.tag == HirStmt_SSpawn) {
        __auto_type sexpr = _t3271.data.SSpawn.expr;
        /* pass */
        return _lower_spawn(m, lf, sexpr);
    } else if (_t3271.tag == HirStmt_STaskGroup) {
        __auto_type tgbody = _t3271.data.STaskGroup.body;
        /* pass */
        return _lower_taskgroup(m, lf, tgbody);
    } else if (_t3271.tag == HirStmt_SChanSelect) {
        __auto_type scases = _t3271.data.SChanSelect.cases;
        /* pass */
        return _lower_chan_select(m, lf, scases);
    } else if (_t3271.tag == HirStmt_SAsm) {
        __auto_type code = _t3271.data.SAsm.code;
__auto_type outputs = _t3271.data.SAsm.outputs;
__auto_type inputs = _t3271.data.SAsm.inputs;
__auto_type clobbers = _t3271.data.SAsm.clobbers;
        /* pass */
        if (((!_is_null_str(outputs)) && (!_tr_str_eqv((outputs), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            return false;
        }
        /* pass */
        if (((!_is_null_str(inputs)) && (!_tr_str_eqv((inputs), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr cons = _tr_str_lit_len("", 0LL);
        /* pass */
        if (((!_is_null_str(clobbers)) && (!_tr_str_eqv((clobbers), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            if (_tr_str_contains((clobbers).data, (_tr_str_lit_len(",", 1LL)).data)) {
                /* pass */
                _tr_str_release(cons);
                return false;
            }
            /* pass */
            TrStr _strtmp_t3279 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("~{", 2LL)), (clobbers))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("}", 1LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(cons);
            cons = _strtmp_t3279;
        }
        /* pass */
        ({ TrStr _at_t3280 = (_own(code)); LFunc_emit(lf, LInst_ctor_IAsm(_at_t3280, cons)); _tr_str_release(_at_t3280); });
        /* pass */
        _tr_str_release(cons);
        return true;
    } else if (1) {
        __auto_type _ = _t3271;
        /* pass */
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            m->fail_note = ({ TrStr _cr = (_stmt_kind(s)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("unsupported statement kind: ", 28LL)), _cr); _tr_str_release(_cr); _cres; });
        }
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    bool ok = _lower_stmt_impl(m, lf, s);
    /* pass */
    if (((!ok) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        m->fail_note = ({ TrStr _cr = (_stmt_kind(s)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("unsupported statement: ", 23LL)), _cr); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    return ok;
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
    if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
        /* pass */
        TrStr slsym = _set_sym(stag, _tr_str_lit_len("len", 3LL));
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
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) {
        /* pass */
        TrStr sesym = _set_sym(stag, _tr_str_lit_len("len", 3LL));
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
        LFunc_emit(lf, LInst_ctor_IBinOp(ser, _tr_str_lit_len("==", 2LL), sed, sez));
        /* pass */
        LFunc_set_vreg_type(lf, ser, 4LL);
        /* pass */
        _tr_str_release(sesym);
        return ser;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL))) && (margs->len == 0LL))) {
        /* pass */
        TrStr tlsym = _tr_str_lit_len("_tr_rt_iset_to_list", 19LL);
        /* pass */
        long long tltag = 2LL;
        /* pass */
        if ((stag == 16LL)) {
            /* pass */
            TrStr _strtmp_t3281 = _tr_str_lit_len("_tr_rt_sset_to_list", 19LL);
            _tr_str_release(tlsym);
            tlsym = _strtmp_t3281;
            /* pass */
            tltag = 3LL;
        }
        /* pass */
        LModule_add_extern(m, tlsym);
        /* pass */
        List_i64* tla = (void*)List_i64_new();
        /* pass */
        List_i64_append(tla, shv);
        /* pass */
        long long tld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(tld, tlsym, tla));
        /* pass */
        LFunc_set_vreg_type(lf, tld, tltag);
        /* pass */
        _tr_str_release(tlsym);
        return tld;
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
    if (_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) {
        /* pass */
        TrStr sasym = _set_sym(stag, _tr_str_lit_len("set", 3LL));
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
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))))) {
        /* pass */
        TrStr scsym = _set_sym(stag, _tr_str_lit_len("has", 3LL));
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
    if (_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) {
        /* pass */
        TrStr srsym = _set_sym(stag, _tr_str_lit_len("remove", 6LL));
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
    __auto_type _t3282 = pat;
    if (_t3282.tag == Pattern_PLitInt) {
        __auto_type v = _t3282.data.PLitInt.val;
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
        LFunc_emit(lf, LInst_ctor_IBinOp(d, _tr_str_lit_len("==", 2LL), subj, cv));
        /* pass */
        LFunc_set_vreg_type(lf, d, 4LL);
        /* pass */
        return d;
    } else if (_t3282.tag == Pattern_PLitBool) {
        __auto_type bv = _t3282.data.PLitBool.val;
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
        LFunc_emit(lf, LInst_ctor_IBinOp(db, _tr_str_lit_len("==", 2LL), subj, cvb));
        /* pass */
        LFunc_set_vreg_type(lf, db, 4LL);
        /* pass */
        return db;
    } else if (_t3282.tag == Pattern_PLitStr) {
        __auto_type sv = _t3282.data.PLitStr.val;
__auto_type sv_blen = _t3282.data.PLitStr.blen;
        /* pass */
        if ((st != 1LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long idx = LModule_add_string(m, sv, sv_blen);
        /* pass */
        long long lit = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(lit, idx));
        /* pass */
        LFunc_set_vreg_type(lf, lit, 1LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
        /* pass */
        List_i64* sa = (void*)List_i64_new();
        /* pass */
        List_i64_append(sa, subj);
        /* pass */
        List_i64_append(sa, lit);
        /* pass */
        long long cmpv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sa));
        /* pass */
        long long z = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
        /* pass */
        long long ds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(ds, _tr_str_lit_len("==", 2LL), cmpv, z));
        /* pass */
        LFunc_set_vreg_type(lf, ds, 4LL);
        /* pass */
        return ds;
    } else if (1) {
        __auto_type _ = _t3282;
        /* pass */
        return (-1LL);
    }
}

__attribute__((hot)) bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms) {
    /* pass */
    TrStr subj_ty = _tr_str_retain(hir_expr_type(expr)->name);
    /* pass */
    bool subj_is_str = false;
    /* pass */
    if ((!_is_null_str(subj_ty))) {
        /* pass */
        subj_is_str = _tr_str_eqv((subj_ty), (_tr_str_lit_len("str", 3LL)));
    }
    /* pass */
    if (subj_is_str) {
        /* pass */
        __auto_type _t3283 = (*expr);
        if (_t3283.tag == HirExpr_EIdent) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t3283;
            _tr_str_release(subj_ty);
            return false;
        }
    }
    /* pass */
    long long subj = lower_expr(m, lf, expr);
    /* pass */
    if ((subj < 0LL)) {
        /* pass */
        _tr_str_release(subj_ty);
        return false;
    }
    /* pass */
    long long st = LFunc_vreg_type(lf, subj);
    /* pass */
    if ((st == 11LL)) {
        /* pass */
        _tr_str_release(subj_ty);
        return _lower_match_enum(m, lf, expr, subj, arms);
    }
    /* pass */
    if ((((st != 0LL) && (st != 1LL)) && (st != 4LL))) {
        /* pass */
        _tr_str_release(subj_ty);
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
        TrStr bind_name = _tr_str_lit_len("", 0LL);
        /* pass */
        long long cond = (-1LL);
        /* pass */
        __auto_type _t3284 = arm->pat;
        if (_t3284.tag == Pattern_PWild) {
            /* pass */
            is_default = true;
        } else if (_t3284.tag == Pattern_PBind) {
            __auto_type nm = _t3284.data.PBind.name;
            /* pass */
            is_default = true;
            /* pass */
            TrStr _strtmp_t3285 = _tr_str_retain(nm);
            _tr_str_release(bind_name);
            bind_name = _strtmp_t3285;
        } else if (_t3284.tag == Pattern_POr) {
            __auto_type pats = _t3284.data.POr.patterns;
            /* pass */
            long long oi = 0LL;
            /* pass */
            while ((oi < pats->len)) {
                /* pass */
                long long sc = _lit_pat_cond(m, lf, List_Pattern_get(pats, oi), subj, st);
                /* pass */
                if ((sc < 0LL)) {
                    /* pass */
                    _tr_str_release(subj_ty);
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
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit_len("+", 1LL), cond, sc));
                    /* pass */
                    cond = _norm_bool(lf, merged);
                }
                /* pass */
                oi = (oi + 1LL);
            }
            /* pass */
            if ((cond < 0LL)) {
                /* pass */
                _tr_str_release(subj_ty);
                return false;
            }
        } else if (1) {
            __auto_type _ = _t3284;
            /* pass */
            cond = _lit_pat_cond(m, lf, arm->pat, subj, st);
            /* pass */
            if ((cond < 0LL)) {
                /* pass */
                _tr_str_release(subj_ty);
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
        if ((!_tr_str_eqv((bind_name), (_tr_str_lit_len("", 0LL))))) {
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
                _tr_str_release(subj_ty);
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
            _tr_str_release(subj_ty);
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
    _tr_str_release(subj_ty);
    return true;
}

__attribute__((hot)) TrStr _norm_variant(TrStr ename, TrStr vn) {
    /* pass */
    if (_tr_str_eqv((ename), (_tr_str_lit_len("Option", 6LL)))) {
        /* pass */
        if (((_is_null_str(vn) || _tr_str_eqv((vn), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((vn), (_tr_str_lit_len("none", 4LL))))) {
            /* pass */
            return _tr_str_lit_len("None", 4LL);
        }
        /* pass */
        if (_tr_str_eqv((vn), (_tr_str_lit_len("some", 4LL)))) {
            /* pass */
            return _tr_str_lit_len("Some", 4LL);
        }
    }
    /* pass */
    if (_tr_str_eqv((ename), (_tr_str_lit_len("Result", 6LL)))) {
        /* pass */
        if (_tr_str_eqv((vn), (_tr_str_lit_len("ok", 2LL)))) {
            /* pass */
            return _tr_str_lit_len("Ok", 2LL);
        }
        /* pass */
        if (_tr_str_eqv((vn), (_tr_str_lit_len("err", 3LL)))) {
            /* pass */
            return _tr_str_lit_len("Err", 3LL);
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
    LFunc_emit(lf, LInst_ctor_IBinOp(dv, _tr_str_lit_len("==", 2LL), tagv, cv));
    /* pass */
    LFunc_set_vreg_type(lf, dv, 4LL);
    /* pass */
    return dv;
}

__attribute__((hot)) long long _load_enum_payload_field(LModule* m, LFunc* lf, long long subj, AstType* subj_ty, VariantLayout* vlay, long long fldidx) {
    /* pass */
    if ((fldidx >= vlay->ftags->len)) {
        /* pass */
        return (-1LL);
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
            return (-1LL);
        }
        /* pass */
        ftg = _tag_of(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        /* pass */
        TrStr _strtmp_t3286 = _cls_of_ty(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        _tr_str_release(bcls);
        bcls = _strtmp_t3286;
    }
    /* pass */
    if ((((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
        /* pass */
        _tr_str_release(bcls);
        return (-1LL);
    }
    /* pass */
    if ((ftg == 5LL)) {
        /* pass */
        long long praw = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), 0LL);
        /* pass */
        long long pv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(pv, praw));
        /* pass */
        LFunc_set_vreg_type(lf, pv, 5LL);
        /* pass */
        _tr_str_release(bcls);
        return pv;
    }
    /* pass */
    long long pv2 = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), ftg);
    /* pass */
    _tr_str_release(bcls);
    return pv2;
}

__attribute__((hot)) long long _lower_enum_prop(LModule* m, LFunc* lf, HirExpr* obj, TrStr ename, TrStr prop) {
    /* pass */
    long long subj = lower_expr(m, lf, obj);
    /* pass */
    if ((subj < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long eidx = LModule_enum_index(m, ename);
    /* pass */
    if ((eidx < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    EnumLayout* elay = ((EnumLayout*)List_ptr_get(m->enums, eidx));
    /* pass */
    AstType* subj_ty = hir_expr_type(obj);
    /* pass */
    if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL))))) {
        /* pass */
        long long vix = ({ TrStr _at_t3287 = (_norm_variant(ename, _tr_str_lit_len("Ok", 2LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3287)); _tr_str_release(_at_t3287); _wr; });
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            vix = ({ TrStr _at_t3288 = (_norm_variant(ename, _tr_str_lit_len("Some", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3288)); _tr_str_release(_at_t3288); _wr; });
        }
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
        /* pass */
        return _variant_tag_cond(lf, tagv, vix);
    }
    /* pass */
    if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL))))) {
        /* pass */
        long long vix = ({ TrStr _at_t3289 = (_norm_variant(ename, _tr_str_lit_len("Err", 3LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3289)); _tr_str_release(_at_t3289); _wr; });
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            vix = ({ TrStr _at_t3290 = (_norm_variant(ename, _tr_str_lit_len("None", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3290)); _tr_str_release(_at_t3290); _wr; });
        }
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
        /* pass */
        return _variant_tag_cond(lf, tagv, vix);
    }
    /* pass */
    if (((_tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("some", 4LL))))) {
        /* pass */
        long long vix = ({ TrStr _at_t3291 = (_norm_variant(ename, _tr_str_lit_len("Ok", 2LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3291)); _tr_str_release(_at_t3291); _wr; });
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            vix = ({ TrStr _at_t3292 = (_norm_variant(ename, _tr_str_lit_len("Some", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3292)); _tr_str_release(_at_t3292); _wr; });
        }
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _load_enum_payload_field(m, lf, subj, subj_ty, ((VariantLayout*)List_ptr_get(elay->variants, vix)), 0LL);
    }
    /* pass */
    if (_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL)))) {
        /* pass */
        long long vix = ({ TrStr _at_t3293 = (_norm_variant(ename, _tr_str_lit_len("Err", 3LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3293)); _tr_str_release(_at_t3293); _wr; });
        /* pass */
        if ((vix < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _load_enum_payload_field(m, lf, subj, subj_ty, ((VariantLayout*)List_ptr_get(elay->variants, vix)), 0LL);
    }
    /* pass */
    return (-1LL);
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
        TrStr _strtmp_t3294 = _cls_of_ty(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        _tr_str_release(bcls);
        bcls = _strtmp_t3294;
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
        /* pass */
        if (((ftg == 10LL) || (ftg == 11LL))) {
            /* pass */
            _secure_obj(m, lf, pv);
        }
    }
    /* pass */
    LFunc_add_var(lf, bindname);
    /* pass */
    LFunc_set_var_type(lf, bindname, ftg);
    /* pass */
    if (((ftg == 10LL) || (ftg == 11LL))) {
        /* pass */
        if (((!_is_null_str(bcls)) && (!_tr_str_eqv((bcls), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            ({ TrStr _at_t3295 = (_own(bcls)); LFunc_set_var_cls(lf, bindname, _at_t3295); _tr_str_release(_at_t3295); });
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
    if ((_tr_str_eqv((ename), (_tr_str_lit_len("", 0LL))) || (!LModule_is_enum(m, ename)))) {
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
        TrStr bind_subj = _tr_str_lit_len("", 0LL);
        /* pass */
        long long cond = (-1LL);
        /* pass */
        __auto_type _t3296 = arm->pat;
        if (_t3296.tag == Pattern_PWild) {
            /* pass */
            is_default = true;
        } else if (_t3296.tag == Pattern_PBind) {
            __auto_type nm = _t3296.data.PBind.name;
            /* pass */
            is_default = true;
            /* pass */
            TrStr _strtmp_t3297 = _tr_str_retain(nm);
            _tr_str_release(bind_subj);
            bind_subj = _strtmp_t3297;
        } else if (_t3296.tag == Pattern_PVariant) {
            __auto_type vn = _t3296.data.PVariant.variant;
            /* pass */
            long long vix = ({ TrStr _at_t3298 = (_norm_variant(ename, vn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3298)); _tr_str_release(_at_t3298); _wr; });
            /* pass */
            if ((vix < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vix);
        } else if (_t3296.tag == Pattern_PVariantBind) {
            __auto_type vnb = _t3296.data.PVariantBind.variant;
            /* pass */
            long long vixb = ({ TrStr _at_t3299 = (_norm_variant(ename, vnb)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3299)); _tr_str_release(_at_t3299); _wr; });
            /* pass */
            if ((vixb < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vixb);
        } else if (_t3296.tag == Pattern_PVariantBindMany) {
            __auto_type vnm = _t3296.data.PVariantBindMany.variant;
            /* pass */
            long long vixm = ({ TrStr _at_t3300 = (_norm_variant(ename, vnm)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3300)); _tr_str_release(_at_t3300); _wr; });
            /* pass */
            if ((vixm < 0LL)) {
                /* pass */
                _tr_str_release(ename);
                return false;
            }
            /* pass */
            cond = _variant_tag_cond(lf, tagv, vixm);
        } else if (_t3296.tag == Pattern_POr) {
            __auto_type orpats = _t3296.data.POr.patterns;
            /* pass */
            long long oi = 0LL;
            /* pass */
            while ((oi < orpats->len)) {
                /* pass */
                long long oc = (-1LL);
                /* pass */
                __auto_type _t3301 = List_Pattern_get(orpats, oi);
                if (_t3301.tag == Pattern_PVariant) {
                    __auto_type ovn = _t3301.data.PVariant.variant;
                    /* pass */
                    long long ovix = ({ TrStr _at_t3302 = (_norm_variant(ename, ovn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t3302)); _tr_str_release(_at_t3302); _wr; });
                    /* pass */
                    if ((ovix < 0LL)) {
                        /* pass */
                        _tr_str_release(ename);
                        return false;
                    }
                    /* pass */
                    oc = _variant_tag_cond(lf, tagv, ovix);
                } else if (1) {
                    __auto_type _ = _t3301;
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
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit_len("+", 1LL), cond, oc));
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
            __auto_type _ = _t3296;
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
        if (((!_tr_str_eqv((bind_subj), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((bind_subj), (_tr_str_lit_len("_", 1LL)))))) {
            /* pass */
            LFunc_add_var(lf, bind_subj);
            /* pass */
            LFunc_set_var_type(lf, bind_subj, 11LL);
            /* pass */
            ({ TrStr _at_t3303 = (_own(ename)); LFunc_set_var_cls(lf, bind_subj, _at_t3303); _tr_str_release(_at_t3303); });
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(bind_subj, subj));
        }
        /* pass */
        __auto_type _t3304 = arm->pat;
        if (_t3304.tag == Pattern_PVariantBind) {
            __auto_type vnb2 = _t3304.data.PVariantBind.variant;
__auto_type bnm = _t3304.data.PVariantBind.field;
            /* pass */
            VariantLayout* vlay1 = ({ TrStr _at_t3305 = (_norm_variant(ename, vnb2)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, EnumLayout_variant_index(elay, _at_t3305)))); _tr_str_release(_at_t3305); _wr; });
            /* pass */
            if ((!_tr_str_eqv((bnm), (_tr_str_lit_len("_", 1LL))))) {
                /* pass */
                if ((!_bind_payload(m, lf, vlay1, subj, subj_hty, 0LL, bnm))) {
                    /* pass */
                    _tr_str_release(ename);
                    return false;
                }
            }
        } else if (_t3304.tag == Pattern_PVariantBindMany) {
            __auto_type vnm2 = _t3304.data.PVariantBindMany.variant;
__auto_type bnames = _t3304.data.PVariantBindMany.fields;
            /* pass */
            VariantLayout* vlay2 = ({ TrStr _at_t3306 = (_norm_variant(ename, vnm2)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, EnumLayout_variant_index(elay, _at_t3306)))); _tr_str_release(_at_t3306); _wr; });
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < bnames->len)) {
                /* pass */
                if ((!_tr_str_eqv((List_TrStr_get(bnames, bi)), (_tr_str_lit_len("_", 1LL))))) {
                    /* pass */
                    if (({ TrStr _at_t3307 = (List_TrStr_get(bnames, bi)); __auto_type _wr = ((!_bind_payload(m, lf, vlay2, subj, subj_hty, bi, _at_t3307))); _tr_str_release(_at_t3307); _wr; })) {
                        /* pass */
                        _tr_str_release(ename);
                        return false;
                    }
                }
                /* pass */
                bi = (bi + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3304;
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
    __auto_type _t3308 = (*iter);
    if (_t3308.tag == HirExpr_ECall) {
        __auto_type callee = _t3308.data.ECall.callee;
__auto_type args = _t3308.data.ECall.args;
        /* pass */
        if (_tr_str_eqv((_ident_name(callee)), (_tr_str_lit_len("range", 5LL)))) {
            /* pass */
            return _lower_for_range(m, lf, var, args, body);
        }
    } else if (_t3308.tag == HirExpr_ERange) {
        __auto_type rstart = _t3308.data.ERange.start;
__auto_type rend = _t3308.data.ERange.end;
__auto_type rincl = _t3308.data.ERange.inclusive;
        /* pass */
        return _lower_for_erange(m, lf, var, rstart, rend, rincl, body);
    } else if (1) {
        __auto_type _ = _t3308;
        /* pass */
        /* pass */
    }
    /* pass */
    if (_tr_str_eqv((hir_expr_type(iter)->name), (_tr_str_lit_len("Chan", 4LL)))) {
        /* pass */
        return _lower_for_chan(m, lf, var, iter, body);
    }
    /* pass */
    TrStr fic = _recv_class(m, lf, iter);
    /* pass */
    if ((((!_tr_str_eqv((fic), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, fic)) && (!_tr_str_eqv((LModule_resolve_method(m, fic, _tr_str_lit_len("__iter__", 8LL))), (_tr_str_lit_len("", 0LL)))))) {
        /* pass */
        _tr_str_release(fic);
        return _lower_for_iterproto(m, lf, var, iter, body);
    }
    /* pass */
    _tr_str_release(fic);
    return _lower_for_list(m, lf, var, iter, body);
}

__attribute__((hot)) bool _sel_recv_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm) {
    /* pass */
    long long dov = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
    /* pass */
    LFunc_set_vreg_type(lf, dov, 0LL);
    /* pass */
    long long armb = LFunc_new_block(lf);
    /* pass */
    long long skipb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
    /* pass */
    LFunc_set_cur(lf, armb);
    /* pass */
    HirExpr* chexpr = arm->chan_expr;
    /* pass */
    __auto_type _t3309 = (*chexpr);
    if (_t3309.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t3309.data.EMethodCall.obj;
        chexpr = mobj;
    } else if (1) {
        __auto_type _ = _t3309;
        /* pass */
    }
    /* pass */
    long long chv = lower_expr(m, lf, chexpr);
    /* pass */
    if ((chv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_recv_val", 21LL));
    /* pass */
    List_i64* ra = (void*)List_i64_new();
    /* pass */
    List_i64_append(ra, chv);
    /* pass */
    long long rv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(rv, _tr_str_lit_len("_tr_chan_try_recv_val", 21LL), ra));
    /* pass */
    LFunc_set_vreg_type(lf, rv, 0LL);
    /* pass */
    long long minc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(minc, ((-9223372036854775807LL) - 1LL)));
    /* pass */
    long long cmp = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cmp, _tr_str_lit_len("!=", 2LL), rv, minc));
    /* pass */
    LFunc_set_vreg_type(lf, cmp, 4LL);
    /* pass */
    long long gotb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmp, gotb, skipb));
    /* pass */
    LFunc_set_cur(lf, gotb);
    /* pass */
    if (((!_is_null_str(arm->var_name)) && (!_tr_str_eqv((arm->var_name), (_tr_str_lit_len("", 0LL)))))) {
        /* pass */
        LFunc_add_var(lf, arm->var_name);
        /* pass */
        LFunc_set_var_type(lf, arm->var_name, 0LL);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(arm->var_name, rv));
    }
    /* pass */
    if ((!lower_block(m, lf, arm->body))) {
        /* pass */
        return false;
    }
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
    /* pass */
    LFunc_set_cur(lf, skipb);
    /* pass */
    return true;
}

__attribute__((hot)) bool _sel_send_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm) {
    /* pass */
    long long dov = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
    /* pass */
    LFunc_set_vreg_type(lf, dov, 0LL);
    /* pass */
    long long armb = LFunc_new_block(lf);
    /* pass */
    long long skipb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
    /* pass */
    LFunc_set_cur(lf, armb);
    /* pass */
    HirExpr* chexpr = arm->chan_expr;
    /* pass */
    HirExpr* valexpr = arm->val_expr;
    /* pass */
    __auto_type _t3310 = (*chexpr);
    if (_t3310.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t3310.data.EMethodCall.obj;
__auto_type sargs = _t3310.data.EMethodCall.args;
        /* pass */
        chexpr = mobj;
        /* pass */
        if ((sargs->len == 1LL)) {
            /* pass */
            valexpr = ((HirExpr*)List_ptr_get(sargs, 0LL));
        }
    } else if (1) {
        __auto_type _ = _t3310;
        /* pass */
    }
    /* pass */
    long long chv = lower_expr(m, lf, chexpr);
    /* pass */
    if ((chv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long sv = lower_expr(m, lf, valexpr);
    /* pass */
    if (((sv < 0LL) || (LFunc_vreg_type(lf, sv) != 0LL))) {
        /* pass */
        return false;
    }
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_send", 17LL));
    /* pass */
    List_i64* sa = (void*)List_i64_new();
    /* pass */
    List_i64_append(sa, chv);
    /* pass */
    List_i64_append(sa, sv);
    /* pass */
    long long okv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(okv, _tr_str_lit_len("_tr_chan_try_send", 17LL), sa));
    /* pass */
    LFunc_set_vreg_type(lf, okv, 4LL);
    /* pass */
    long long gotb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(okv, gotb, skipb));
    /* pass */
    LFunc_set_cur(lf, gotb);
    /* pass */
    if ((!lower_block(m, lf, arm->body))) {
        /* pass */
        return false;
    }
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
    /* pass */
    LFunc_set_cur(lf, skipb);
    /* pass */
    return true;
}

__attribute__((hot)) bool _sel_uncond_arm(LModule* m, LFunc* lf, TrStr donev, HirBlock* body) {
    /* pass */
    long long dov = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
    /* pass */
    LFunc_set_vreg_type(lf, dov, 0LL);
    /* pass */
    long long armb = LFunc_new_block(lf);
    /* pass */
    long long skipb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
    /* pass */
    LFunc_set_cur(lf, armb);
    /* pass */
    if ((!lower_block(m, lf, body))) {
        /* pass */
        return false;
    }
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
    /* pass */
    LFunc_set_cur(lf, skipb);
    /* pass */
    return true;
}

__attribute__((hot)) bool _sel_timeout_arm(LModule* m, LFunc* lf, TrStr donev, TrStr tstart, HirChanSelectArm* arm) {
    /* pass */
    long long dov = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
    /* pass */
    LFunc_set_vreg_type(lf, dov, 0LL);
    /* pass */
    long long chkb = LFunc_new_block(lf);
    /* pass */
    long long skipb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, chkb));
    /* pass */
    LFunc_set_cur(lf, chkb);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_time_ms", 11LL));
    /* pass */
    long long nowv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(nowv, _tr_str_lit_len("_tr_time_ms", 11LL), (void*)List_i64_new()));
    /* pass */
    LFunc_set_vreg_type(lf, nowv, 0LL);
    /* pass */
    long long sv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(sv2, tstart));
    /* pass */
    LFunc_set_vreg_type(lf, sv2, 0LL);
    /* pass */
    long long elapsed = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(elapsed, _tr_str_lit_len("-", 1LL), nowv, sv2));
    /* pass */
    long long msv = lower_expr(m, lf, arm->timeout_ms);
    /* pass */
    if (((msv < 0LL) || (LFunc_vreg_type(lf, msv) != 0LL))) {
        /* pass */
        return false;
    }
    /* pass */
    long long cmp = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cmp, _tr_str_lit_len(">=", 2LL), elapsed, msv));
    /* pass */
    LFunc_set_vreg_type(lf, cmp, 4LL);
    /* pass */
    long long fireb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmp, fireb, skipb));
    /* pass */
    LFunc_set_cur(lf, fireb);
    /* pass */
    if ((!lower_block(m, lf, arm->body))) {
        /* pass */
        return false;
    }
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
    /* pass */
    LFunc_set_cur(lf, skipb);
    /* pass */
    return true;
}

__attribute__((hot)) bool _lower_chan_select(LModule* m, LFunc* lf, List_ptr* cases) {
    /* pass */
    TrStr donev = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__seldone", 9LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, donev);
    /* pass */
    LFunc_set_var_type(lf, donev, 0LL);
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, z));
    /* pass */
    bool has_timeout = false;
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < cases->len)) {
        /* pass */
        if (((*((HirChanSelectArm**)List_ptr_get(cases, ci)))->kind == 2LL)) {
            /* pass */
            has_timeout = true;
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    TrStr tstart = _tr_str_lit_len("", 0LL);
    /* pass */
    if (has_timeout) {
        /* pass */
        TrStr _strtmp_t3311 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__selstart", 10LL)), _cr); _tr_str_release(_cr); _cres; });
        _tr_str_release(tstart);
        tstart = _strtmp_t3311;
        /* pass */
        LFunc_add_var(lf, tstart);
        /* pass */
        LFunc_set_var_type(lf, tstart, 0LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_time_ms", 11LL));
        /* pass */
        long long tm = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(tm, _tr_str_lit_len("_tr_time_ms", 11LL), (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, tm, 0LL);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(tstart, tm));
    }
    /* pass */
    long long loopb = LFunc_new_block(lf);
    /* pass */
    long long endb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
    /* pass */
    LFunc_set_cur(lf, loopb);
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < cases->len)) {
        /* pass */
        HirChanSelectArm* arm = (*((HirChanSelectArm**)List_ptr_get(cases, ai)));
        /* pass */
        if ((arm->kind == 0LL)) {
            /* pass */
            if ((!_sel_recv_arm(m, lf, donev, arm))) {
                /* pass */
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        } else if ((arm->kind == 1LL)) {
            /* pass */
            if ((!_sel_send_arm(m, lf, donev, arm))) {
                /* pass */
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        }
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    long long aj = 0LL;
    /* pass */
    while ((aj < cases->len)) {
        /* pass */
        HirChanSelectArm* arm2 = (*((HirChanSelectArm**)List_ptr_get(cases, aj)));
        /* pass */
        if ((arm2->kind == 3LL)) {
            /* pass */
            if ((!_sel_uncond_arm(m, lf, donev, arm2->body))) {
                /* pass */
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        } else if ((arm2->kind == 2LL)) {
            /* pass */
            if ((!_sel_timeout_arm(m, lf, donev, tstart, arm2))) {
                /* pass */
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        }
        /* pass */
        aj = (aj + 1LL);
    }
    /* pass */
    long long dv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(dv, donev));
    /* pass */
    LFunc_set_vreg_type(lf, dv, 0LL);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dv, endb, loopb));
    /* pass */
    LFunc_set_cur(lf, endb);
    /* pass */
    _tr_str_release(donev);
    _tr_str_release(tstart);
    return true;
}

__attribute__((hot)) bool _lower_for_chan(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    long long chv = lower_expr(m, lf, iter);
    /* pass */
    if ((chv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr okname = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__chok", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, okname);
    /* pass */
    LFunc_set_var_type(lf, okname, 0LL);
    /* pass */
    long long zc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(okname, zc));
    /* pass */
    LFunc_add_var(lf, var);
    /* pass */
    LFunc_set_var_type(lf, var, 0LL);
    /* pass */
    long long loopb = LFunc_new_block(lf);
    /* pass */
    long long bodyb = LFunc_new_block(lf);
    /* pass */
    long long endb = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
    /* pass */
    LFunc_set_cur(lf, loopb);
    /* pass */
    long long okaddr = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, okaddr, 12LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IAddrVar(okaddr, okname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_recv_ok", 16LL));
    /* pass */
    List_i64* ra = (void*)List_i64_new();
    /* pass */
    List_i64_append(ra, chv);
    /* pass */
    List_i64_append(ra, okaddr);
    /* pass */
    long long rv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(rv, _tr_str_lit_len("_tr_chan_recv_ok", 16LL), ra));
    /* pass */
    LFunc_set_vreg_type(lf, rv, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, rv));
    /* pass */
    long long okv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(okv, okname));
    /* pass */
    LFunc_set_vreg_type(lf, okv, 0LL);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(okv, bodyb, endb));
    /* pass */
    LFunc_set_cur(lf, bodyb);
    /* pass */
    if ((!lower_block(m, lf, body))) {
        /* pass */
        _tr_str_release(okname);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
    /* pass */
    LFunc_set_cur(lf, endb);
    /* pass */
    _tr_str_release(okname);
    return true;
}

__attribute__((hot)) bool _lower_for_iterproto(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    TrStr icls = _recv_class(m, lf, iter);
    /* pass */
    if ((_tr_str_eqv((icls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, icls)))) {
        /* pass */
        _tr_str_release(icls);
        return false;
    }
    /* pass */
    TrStr iterm = LModule_resolve_method(m, icls, _tr_str_lit_len("__iter__", 8LL));
    /* pass */
    if (_tr_str_eqv((iterm), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        return false;
    }
    /* pass */
    TrStr itcls = _tr_str_retain(_hir_method_ret_ty(m, icls, _tr_str_lit_len("__iter__", 8LL))->name);
    /* pass */
    if ((((_is_null_str(itcls) || _tr_str_eqv((itcls), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((itcls), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((itcls), (_tr_str_lit_len("None", 4LL))))) {
        /* pass */
        TrStr _strtmp_t3312 = _tr_str_retain(icls);
        _tr_str_release(itcls);
        itcls = _strtmp_t3312;
    }
    /* pass */
    if ((!LModule_is_class(m, itcls))) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        return false;
    }
    /* pass */
    TrStr nextm = LModule_resolve_method(m, itcls, _tr_str_lit_len("__next__", 8LL));
    /* pass */
    if (_tr_str_eqv((nextm), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    AstType* nextret = _hir_method_ret_ty(m, itcls, _tr_str_lit_len("__next__", 8LL));
    /* pass */
    if ((!_tr_str_eqv((nextret->name), (_tr_str_lit_len("Option", 6LL))))) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    long long oidx = LModule_enum_index(m, _tr_str_lit_len("Option", 6LL));
    /* pass */
    if ((oidx < 0LL)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    VariantLayout* some_vlay = ((VariantLayout*)List_ptr_get(((EnumLayout*)List_ptr_get(m->enums, oidx))->variants, 0LL));
    /* pass */
    long long none_idx = EnumLayout_variant_index(((EnumLayout*)List_ptr_get(m->enums, oidx)), _tr_str_lit_len("None", 4LL));
    /* pass */
    if ((none_idx < 0LL)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    long long objv = lower_expr(m, lf, iter);
    /* pass */
    if ((objv < 0LL)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    long long itv = _lower_obj_call(m, lf, iterm, objv, (void*)List_ptr_new());
    /* pass */
    if ((itv < 0LL)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
    /* pass */
    bool fdiscard = _fresh_take_obj(lf, objv);
    /* pass */
    fdiscard = _fresh_take_obj(lf, itv);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr itname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__iter", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr nxname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__nx", 4LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, itname);
    /* pass */
    LFunc_set_var_type(lf, itname, 10LL);
    /* pass */
    ({ TrStr _at_t3313 = (_own(itcls)); LFunc_set_var_cls(lf, itname, _at_t3313); _tr_str_release(_at_t3313); });
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(itname, itv));
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
    long long itload = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(itload, itname));
    /* pass */
    LFunc_set_vreg_type(lf, itload, 10LL);
    /* pass */
    long long nx = _lower_obj_call(m, lf, nextm, itload, (void*)List_ptr_new());
    /* pass */
    if ((nx < 0LL)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
    /* pass */
    LFunc_add_var(lf, nxname);
    /* pass */
    LFunc_set_var_type(lf, nxname, 11LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(nxname, nx));
    /* pass */
    long long nxl = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(nxl, nxname));
    /* pass */
    LFunc_set_vreg_type(lf, nxl, 11LL);
    /* pass */
    long long tagv = _emit_field_get(m, lf, nxl, 0LL, 0LL);
    /* pass */
    long long isnone = _variant_tag_cond(lf, tagv, none_idx);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(isnone, ext, bdy));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long nxl2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(nxl2, nxname));
    /* pass */
    LFunc_set_vreg_type(lf, nxl2, 11LL);
    /* pass */
    if ((!_bind_payload(m, lf, some_vlay, nxl2, nextret, 0LL, var))) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
    /* pass */
    List_i64_append(lf->loop_cont, hdr);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool bok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!bok)) {
        /* pass */
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
    /* pass */
    _flush_fresh_strs(m, lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(icls);
    _tr_str_release(iterm);
    _tr_str_release(itcls);
    _tr_str_release(nextm);
    _tr_str_release(itname);
    _tr_str_release(nxname);
    return true;
}

__attribute__((hot)) bool _lower_for_erange(LModule* m, LFunc* lf, TrStr var, HirExpr* start, HirExpr* end, bool inclusive, HirBlock* body) {
    /* pass */
    long long sv = lower_expr(m, lf, start);
    /* pass */
    if (((sv < 0LL) || (LFunc_vreg_type(lf, sv) != 0LL))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr cmp = _tr_str_lit_len("<", 1LL);
    /* pass */
    if (inclusive) {
        /* pass */
        TrStr _strtmp_t3314 = _tr_str_lit_len("<=", 2LL);
        _tr_str_release(cmp);
        cmp = _strtmp_t3314;
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
    long long ev = lower_expr(m, lf, end);
    /* pass */
    if (((ev < 0LL) || (LFunc_vreg_type(lf, ev) != 0LL))) {
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
    _emit_add_const(lf, var, 1LL);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(cmp);
    return true;
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
    TrStr cmp = _tr_str_lit_len("<", 1LL);
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
            TrStr _strtmp_t3315 = _tr_str_lit_len(">", 1LL);
            _tr_str_release(cmp);
            cmp = _strtmp_t3315;
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
    TrStr hname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__forlist", 9LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr iname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__foridx", 8LL)), _cr); _tr_str_release(_cr); _cres; });
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
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
    /* pass */
    List_i64* la = (void*)List_i64_new();
    /* pass */
    List_i64_append(la, hv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
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
    long long xval = _list_get_raw(m, lf, ltag, hv2, iv2);
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
                ({ TrStr _at_t3316 = (_own((*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)); LFunc_set_var_cls(lf, var, _at_t3316); _tr_str_release(_at_t3316); });
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
    __auto_type _t3317 = (*iter);
    if (_t3317.tag == HirExpr_ECall) {
        __auto_type callee = _t3317.data.ECall.callee;
__auto_type args = _t3317.data.ECall.args;
        /* pass */
        if ((_tr_str_eqv((_ident_name(callee)), (_tr_str_lit_len("enumerate", 9LL))) && (args->len == 1LL))) {
            /* pass */
            return ({ TrStr _at_t3318 = (List_TrStr_get(vars, 0LL)); TrStr _at_t3319 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_enumerate(m, lf, _at_t3318, _at_t3319, ((HirExpr*)List_ptr_get(args, 0LL)), body)); _tr_str_release(_at_t3318); _tr_str_release(_at_t3319); _wr; });
        }
        /* pass */
        if ((_tr_str_eqv((_ident_name(callee)), (_tr_str_lit_len("zip", 3LL))) && (args->len == 2LL))) {
            /* pass */
            return ({ TrStr _at_t3320 = (List_TrStr_get(vars, 0LL)); TrStr _at_t3321 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_zip(m, lf, _at_t3320, _at_t3321, ((HirExpr*)List_ptr_get(args, 0LL)), ((HirExpr*)List_ptr_get(args, 1LL)), body)); _tr_str_release(_at_t3320); _tr_str_release(_at_t3321); _wr; });
        }
    } else if (_t3317.tag == HirExpr_EMethodCall) {
        __auto_type dobj = _t3317.data.EMethodCall.obj;
__auto_type dmeth = _t3317.data.EMethodCall.method;
        /* pass */
        if (_tr_str_eqv((dmeth), (_tr_str_lit_len("items", 5LL)))) {
            /* pass */
            return ({ TrStr _at_t3322 = (List_TrStr_get(vars, 0LL)); TrStr _at_t3323 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_dict_items_unpack(m, lf, _at_t3322, _at_t3323, dobj, body)); _tr_str_release(_at_t3322); _tr_str_release(_at_t3323); _wr; });
        }
    } else if (1) {
        __auto_type _ = _t3317;
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
    TrStr hname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__enumlist", 10LL)), _cr); _tr_str_release(_cr); _cres; });
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
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
    /* pass */
    List_i64* la = (void*)List_i64_new();
    /* pass */
    List_i64_append(la, hv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, ivar));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
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
    long long xval = _list_get_raw(m, lf, ltag, hv2, iv2);
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

__attribute__((hot)) bool _lower_zip(LModule* m, LFunc* lf, TrStr v0, TrStr v1, HirExpr* aexpr, HirExpr* bexpr, HirBlock* body) {
    /* pass */
    long long av = lower_expr(m, lf, aexpr);
    /* pass */
    if ((av < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long atag = LFunc_vreg_type(lf, av);
    /* pass */
    if ((!_is_list_tag(atag))) {
        /* pass */
        return false;
    }
    /* pass */
    long long bv = lower_expr(m, lf, bexpr);
    /* pass */
    if ((bv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long btag = LFunc_vreg_type(lf, bv);
    /* pass */
    if ((!_is_list_tag(btag))) {
        /* pass */
        return false;
    }
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr aname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__zipa", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr bname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__zipb", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, aname);
    /* pass */
    LFunc_set_var_type(lf, aname, atag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(aname, av));
    /* pass */
    LFunc_add_var(lf, bname);
    /* pass */
    LFunc_set_var_type(lf, bname, btag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(bname, bv));
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    ({ TrStr _at_t3324 = (({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__zipi", 6LL)), _cr); _tr_str_release(_cr); _cres; })); LFunc_add_var(lf, _at_t3324); _tr_str_release(_at_t3324); });
    /* pass */
    TrStr iname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__zipi", 6LL)), _cr); _tr_str_release(_cr); _cres; });
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
    long long ahv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(ahv, aname));
    /* pass */
    long long bhv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(bhv, bname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
    /* pass */
    List_i64* ala = (void*)List_i64_new();
    /* pass */
    List_i64_append(ala, ahv);
    /* pass */
    long long alenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(alenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), ala));
    /* pass */
    List_i64* bla = (void*)List_i64_new();
    /* pass */
    List_i64_append(bla, bhv);
    /* pass */
    long long blenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(blenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), bla));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
    /* pass */
    long long c1 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(c1, _tr_str_lit_len("<", 1LL), iv, alenv));
    /* pass */
    long long c2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(c2, _tr_str_lit_len("<", 1LL), iv, blenv));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("&", 1LL), c1, c2));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long ahv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(ahv2, aname));
    /* pass */
    long long bhv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(bhv2, bname));
    /* pass */
    long long iv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
    /* pass */
    long long aelem = _list_get_elem(m, lf, atag, ahv2, iv2);
    /* pass */
    long long belem = _list_get_elem(m, lf, btag, bhv2, iv2);
    /* pass */
    long long aet = _list_elem_tag(atag);
    /* pass */
    long long bet = _list_elem_tag(btag);
    /* pass */
    if ((aet == 1LL)) {
        /* pass */
        _retain_str(m, lf, aelem);
    }
    /* pass */
    if ((bet == 1LL)) {
        /* pass */
        _retain_str(m, lf, belem);
    }
    /* pass */
    LFunc_add_var(lf, v0);
    /* pass */
    LFunc_set_var_type(lf, v0, aet);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(v0, aelem));
    /* pass */
    LFunc_add_var(lf, v1);
    /* pass */
    LFunc_set_var_type(lf, v1, bet);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(v1, belem));
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
        _tr_str_release(aname);
        _tr_str_release(bname);
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
    _tr_str_release(aname);
    _tr_str_release(bname);
    _tr_str_release(iname);
    return true;
}

__attribute__((hot)) bool _lower_dict_items_unpack(LModule* m, LFunc* lf, TrStr kvar, TrStr vvar, HirExpr* dictexpr, HirBlock* body) {
    /* pass */
    long long dv = lower_expr(m, lf, dictexpr);
    /* pass */
    if ((dv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long dtag = LFunc_vreg_type(lf, dv);
    /* pass */
    if ((!_is_dict_tag(dtag))) {
        /* pass */
        return false;
    }
    /* pass */
    bool kstr = _dict_key_is_str(dtag);
    /* pass */
    long long vtag = _dict_val_tag(dtag);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr dname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__ditd", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, dname);
    /* pass */
    LFunc_set_var_type(lf, dname, dtag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(dname, dv));
    /* pass */
    TrStr isym = _tr_str_lit_len("_tr_rt_idict_items", 18LL);
    /* pass */
    if (kstr) {
        /* pass */
        TrStr _strtmp_t3325 = _tr_str_lit_len("_tr_rt_dict_items", 17LL);
        _tr_str_release(isym);
        isym = _strtmp_t3325;
    }
    /* pass */
    LModule_add_extern(m, isym);
    /* pass */
    List_i64* ia = (void*)List_i64_new();
    /* pass */
    List_i64_append(ia, dv);
    /* pass */
    long long items = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(items, isym, ia));
    /* pass */
    LFunc_set_vreg_type(lf, items, 3LL);
    /* pass */
    TrStr iname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__diti", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr itemsname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__ditl", 6LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, itemsname);
    /* pass */
    LFunc_set_var_type(lf, itemsname, 3LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(itemsname, items));
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
    long long lhv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(lhv, itemsname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
    /* pass */
    List_i64* lla = (void*)List_i64_new();
    /* pass */
    List_i64_append(lla, lhv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), lla));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long lhv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(lhv2, itemsname));
    /* pass */
    long long iv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
    /* pass */
    long long raw = _list_get(m, lf, lhv2, iv2);
    /* pass */
    long long koff = 0LL;
    /* pass */
    long long voff = 8LL;
    /* pass */
    long long keyv = _emit_field_get(m, lf, raw, koff, 0LL);
    /* pass */
    if (kstr) {
        /* pass */
        LFunc_set_vreg_type(lf, keyv, 1LL);
    }
    /* pass */
    long long valv = _emit_field_get(m, lf, raw, voff, 0LL);
    /* pass */
    if ((vtag == 5LL)) {
        /* pass */
        long long vf = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(vf, valv));
        /* pass */
        LFunc_set_vreg_type(lf, vf, 5LL);
        /* pass */
        valv = vf;
    } else {
        /* pass */
        LFunc_set_vreg_type(lf, valv, vtag);
    }
    /* pass */
    long long ktag = 0LL;
    /* pass */
    if (kstr) {
        /* pass */
        ktag = 1LL;
    }
    /* pass */
    LFunc_add_var(lf, kvar);
    /* pass */
    LFunc_set_var_type(lf, kvar, ktag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(kvar, keyv));
    /* pass */
    LFunc_add_var(lf, vvar);
    /* pass */
    LFunc_set_var_type(lf, vvar, vtag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(vvar, valv));
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
        _tr_str_release(dname);
        _tr_str_release(isym);
        _tr_str_release(iname);
        _tr_str_release(itemsname);
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
    _tr_str_release(dname);
    _tr_str_release(isym);
    _tr_str_release(iname);
    _tr_str_release(itemsname);
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
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit_len("+", 1LL), cur, one));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
    /* pass */
    __auto_type _t3326 = (*e);
    if (_t3326.tag == HirExpr_EIdent) {
        __auto_type n = _t3326.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t3326;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val) {
    /* pass */
    TrStr cls = _recv_class(m, lf, obj);
    /* pass */
    if ((_tr_str_eqv((cls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, cls)))) {
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
    if ((ftg < 0LL)) {
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
    if ((_is_list_tag(ftg) || _is_dict_tag(ftg))) {
        /* pass */
        if ((_is_list_tag(ftg) && (!_is_list_tag(vt)))) {
            /* pass */
            _tr_str_release(cls);
            return false;
        }
        /* pass */
        if ((_is_dict_tag(ftg) && (!_is_dict_tag(vt)))) {
            /* pass */
            _tr_str_release(cls);
            return false;
        }
        /* pass */
        LFunc_set_vreg_type(lf, vv, ftg);
    } else if ((ftg == 5LL)) {
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
    } else if (((ftg == 10LL) || (ftg == 11LL))) {
        /* pass */
        if ((!_field_tag_ok(vt, ftg))) {
            /* pass */
            _tr_str_release(cls);
            return false;
        }
        /* pass */
        _secure_obj(m, lf, vv);
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
    _flush_fresh_strs(m, lf);
    /* pass */
    _tr_str_release(cls);
    return true;
}

__attribute__((hot)) bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val) {
    /* pass */
    TrStr sicls = _recv_class(m, lf, obj);
    /* pass */
    if (((!_tr_str_eqv((sicls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, sicls))) {
        /* pass */
        TrStr sim = LModule_resolve_method(m, sicls, _tr_str_lit_len("__setitem__", 11LL));
        /* pass */
        if ((!_tr_str_eqv((sim), (_tr_str_lit_len("", 0LL))))) {
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
    TrStr awn = _ident_name(obj);
    /* pass */
    if (((!_tr_str_eqv((awn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, awn) > 0LL))) {
        /* pass */
        long long aiv = lower_expr(m, lf, idx);
        /* pass */
        if (((aiv < 0LL) || (LFunc_vreg_type(lf, aiv) != 0LL))) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long avv = lower_expr(m, lf, val);
        /* pass */
        if ((avv < 0LL)) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long avt = LFunc_vreg_type(lf, avv);
        /* pass */
        long long aet = _fixed_arr_elem_tag(m, lf, awn);
        /* pass */
        if ((avt == 4LL)) {
            /* pass */
            avt = 0LL;
        }
        /* pass */
        if ((aet == 4LL)) {
            /* pass */
            aet = 0LL;
        }
        /* pass */
        if ((avt != aet)) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long aaddr = _array_elem_addr(m, lf, awn, aiv);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStore(aaddr, 0LL, avv, 2LL));
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        _tr_str_release(sicls);
        _tr_str_release(awn);
        return true;
    }
    /* pass */
    long long ov = lower_expr(m, lf, obj);
    /* pass */
    if ((ov < 0LL)) {
        /* pass */
        _tr_str_release(sicls);
        _tr_str_release(awn);
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
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, kv) != 1LL))) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, kv) != 0LL))) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long vv = lower_expr(m, lf, val);
        /* pass */
        if (((vv < 0LL) || (LFunc_vreg_type(lf, vv) != _dict_val_tag(ovt)))) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        if ((_dict_val_tag(ovt) == 1LL)) {
            /* pass */
            _secure_str(m, lf, vv);
        }
        /* pass */
        if (((_dict_val_tag(ovt) == 10LL) || (_dict_val_tag(ovt) == 11LL))) {
            /* pass */
            _secure_obj(m, lf, vv);
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
        TrStr ssym = _dict_sym(ovt, _tr_str_lit_len("set", 3LL));
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
        _flush_fresh_strs(m, lf);
        /* pass */
        _tr_str_release(sicls);
        _tr_str_release(awn);
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
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long lvv = lower_expr(m, lf, val);
        /* pass */
        if ((lvv < 0LL)) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        long long lvt = LFunc_vreg_type(lf, lvv);
        /* pass */
        if (((_list_elem_tag(ovt) == 0LL) && (lvt == 4LL))) {
            /* pass */
            lvt = 0LL;
        }
        /* pass */
        if ((lvt != _list_elem_tag(ovt))) {
            /* pass */
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
        /* pass */
        if ((_list_elem_tag(ovt) == 1LL)) {
            /* pass */
            _secure_str(m, lf, lvv);
        }
        /* pass */
        if ((_list_elem_tag(ovt) == 10LL)) {
            /* pass */
            _secure_obj(m, lf, lvv);
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
        long long la_addr = _inline_list_addr(m, lf, ov, iv, _list_stride(ovt));
        /* pass */
        if ((ovt == 22LL)) {
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreB(la_addr, 0LL, lvv));
        } else {
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStore(la_addr, 0LL, lvv, 2LL));
        }
        /* pass */
        _flush_fresh_strs(m, lf);
        /* pass */
        _tr_str_release(sicls);
        _tr_str_release(awn);
        return true;
    }
    /* pass */
    _tr_str_release(sicls);
    _tr_str_release(awn);
    return false;
}

__attribute__((hot)) TrStr _write_sym(long long t) {
    /* pass */
    if ((t == 1LL)) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_write_cstr", 17LL);
    }
    /* pass */
    if ((t == 4LL)) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_write_bool", 17LL);
    }
    /* pass */
    return _tr_str_lit_len("_tr_rt_write_i64", 16LL);
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
        _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_nl", 15LL));
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
            TrStr plsym = _tr_str_lit_len("_tr_rt_print_list_i64", 21LL);
            /* pass */
            if ((avt == 3LL)) {
                /* pass */
                TrStr _strtmp_t3327 = _tr_str_lit_len("_tr_rt_print_list_str", 21LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t3327;
            }
            /* pass */
            if ((avt == 14LL)) {
                /* pass */
                TrStr _strtmp_t3328 = _tr_str_lit_len("_tr_rt_print_list_f64", 21LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t3328;
            }
            /* pass */
            if ((avt == 22LL)) {
                /* pass */
                TrStr _strtmp_t3329 = _tr_str_lit_len("_tr_rt_print_list_bool", 22LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t3329;
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
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_cstr", 17LL));
                /* pass */
                List_i64* opa = (void*)List_i64_new();
                /* pass */
                List_i64_append(opa, ostr);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_print_cstr", 17LL), opa));
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
        if ((avt == 15LL)) {
            /* pass */
            long long tstr = _tuple_to_str(m, lf, av, hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL))));
            /* pass */
            if ((tstr < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_cstr", 17LL));
            /* pass */
            List_i64* tpa = (void*)List_i64_new();
            /* pass */
            List_i64_append(tpa, tstr);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_print_cstr", 17LL), tpa));
            /* pass */
            return true;
        }
        /* pass */
        if ((avt == 5LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_f64", 16LL));
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit_len("_tr_rt_print_f64", 16LL), av));
            /* pass */
            return true;
        }
        /* pass */
        TrStr sym = _print_i64_sym();
        /* pass */
        if ((avt == 1LL)) {
            /* pass */
            TrStr _strtmp_t3330 = _tr_str_lit_len("_tr_rt_print_cstr", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t3330;
        } else if ((avt == 4LL)) {
            /* pass */
            TrStr _strtmp_t3331 = _tr_str_lit_len("_tr_rt_print_bool", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t3331;
        } else if (((avt == 0LL) && _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name), (_tr_str_lit_len("char", 4LL))))) {
            /* pass */
            TrStr _strtmp_t3332 = _tr_str_lit_len("_tr_rt_print_char", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t3332;
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
            _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_sp", 15LL));
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
            TrStr wlsym = _tr_str_lit_len("_tr_rt_write_list_i64", 21LL);
            /* pass */
            if ((pvt == 3LL)) {
                /* pass */
                TrStr _strtmp_t3333 = _tr_str_lit_len("_tr_rt_write_list_str", 21LL);
                _tr_str_release(wlsym);
                wlsym = _strtmp_t3333;
            }
            /* pass */
            if ((pvt == 14LL)) {
                /* pass */
                TrStr _strtmp_t3334 = _tr_str_lit_len("_tr_rt_write_list_f64", 21LL);
                _tr_str_release(wlsym);
                wlsym = _strtmp_t3334;
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_cstr", 17LL));
            /* pass */
            List_i64* pwa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pwa, postr);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_write_cstr", 17LL), pwa));
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
        if ((pvt == 15LL)) {
            /* pass */
            long long ptstr = _tuple_to_str(m, lf, pv, hir_expr_type(((HirExpr*)List_ptr_get(args, pi))));
            /* pass */
            if ((ptstr < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_cstr", 17LL));
            /* pass */
            List_i64* ptwa = (void*)List_i64_new();
            /* pass */
            List_i64_append(ptwa, ptstr);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_write_cstr", 17LL), ptwa));
            /* pass */
            pi = (pi + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((pvt == 5LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_f64", 16LL));
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit_len("_tr_rt_write_f64", 16LL), pv));
        } else {
            /* pass */
            TrStr wsym = _write_sym(pvt);
            /* pass */
            if (((pvt == 0LL) && _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(args, pi)))->name), (_tr_str_lit_len("char", 4LL))))) {
                /* pass */
                TrStr _strtmp_t3335 = _tr_str_lit_len("_tr_rt_write_char", 17LL);
                _tr_str_release(wsym);
                wsym = _strtmp_t3335;
            }
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
    _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_nl", 15LL));
    /* pass */
    return true;
}

__attribute__((hot)) bool _lower_assert_cmp(LModule* m, LFunc* lf, TrStr fname, List_ptr* args) {
    /* pass */
    if ((args->len < 2LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
    /* pass */
    if ((av < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long bv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 1LL)));
    /* pass */
    if ((bv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr op = _tr_str_lit_len("==", 2LL);
    /* pass */
    if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_ne", 9LL)))) {
        /* pass */
        TrStr _strtmp_t3336 = _tr_str_lit_len("!=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t3336;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_lt", 9LL)))) {
        /* pass */
        TrStr _strtmp_t3337 = _tr_str_lit_len("<", 1LL);
        _tr_str_release(op);
        op = _strtmp_t3337;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_le", 9LL)))) {
        /* pass */
        TrStr _strtmp_t3338 = _tr_str_lit_len("<=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t3338;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_gt", 9LL)))) {
        /* pass */
        TrStr _strtmp_t3339 = _tr_str_lit_len(">", 1LL);
        _tr_str_release(op);
        op = _strtmp_t3339;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_ge", 9LL)))) {
        /* pass */
        TrStr _strtmp_t3340 = _tr_str_lit_len(">=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t3340;
    }
    /* pass */
    long long at = LFunc_vreg_type(lf, av);
    /* pass */
    long long cmpv = (-1LL);
    /* pass */
    if ((at == 1LL)) {
        /* pass */
        if ((LFunc_vreg_type(lf, bv) != 1LL)) {
            /* pass */
            _tr_str_release(op);
            return false;
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
        /* pass */
        List_i64* sa = (void*)List_i64_new();
        /* pass */
        List_i64_append(sa, av);
        /* pass */
        List_i64_append(sa, bv);
        /* pass */
        long long cr = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cr, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sa));
        /* pass */
        long long z = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
        /* pass */
        cmpv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(cmpv, op, cr, z));
    } else if ((((at == 0LL) || (at == 4LL)) || (at == 5LL))) {
        /* pass */
        cmpv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(cmpv, op, av, bv));
    } else {
        /* pass */
        _tr_str_release(op);
        return false;
    }
    /* pass */
    LFunc_set_vreg_type(lf, cmpv, 4LL);
    /* pass */
    _flush_fresh_strs(m, lf);
    /* pass */
    long long ok = LFunc_new_block(lf);
    /* pass */
    long long fail = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmpv, ok, fail));
    /* pass */
    LFunc_set_cur(lf, fail);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_assert_fail", 18LL));
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_assert_fail", 18LL), (void*)List_i64_new()));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(ok));
    /* pass */
    LFunc_set_cur(lf, ok);
    /* pass */
    _tr_str_release(op);
    return true;
}

__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t3341 = (*e);
    if (_t3341.tag == HirExpr_ECall) {
        __auto_type callee = _t3341.data.ECall.callee;
__auto_type args = _t3341.data.ECall.args;
        /* pass */
        TrStr fname = _ident_name(callee);
        /* pass */
        if (_tr_str_eqv((fname), (_tr_str_lit_len("print", 5LL)))) {
            /* pass */
            _tr_str_release(fname);
            return _lower_print(m, lf, args);
        }
        /* pass */
        if ((((((_tr_str_eqv((fname), (_tr_str_lit_len("assert_eq", 9LL))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_ne", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_lt", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_le", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_gt", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_ge", 9LL))))) {
            /* pass */
            return _lower_assert_cmp(m, lf, fname, args);
        }
        /* pass */
        long long r = lower_expr(m, lf, e);
        /* pass */
        _tr_str_release(fname);
        return (r >= 0LL);
    } else if (_t3341.tag == HirExpr_EMethodCall) {
        /* pass */
        long long rm = lower_expr(m, lf, e);
        /* pass */
        return (rm >= 0LL);
    } else if (1) {
        __auto_type _ = _t3341;
        /* pass */
        long long rx = lower_expr(m, lf, e);
        /* pass */
        return (rx >= 0LL);
    }
}

__attribute__((hot)) bool _int_op(TrStr op) {
    /* pass */
    if ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("|", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))))) {
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
        return _tr_str_lit_len("0", 1LL);
    }
    /* pass */
    if ((d == 1LL)) {
        /* pass */
        return _tr_str_lit_len("1", 1LL);
    }
    /* pass */
    if ((d == 2LL)) {
        /* pass */
        return _tr_str_lit_len("2", 1LL);
    }
    /* pass */
    if ((d == 3LL)) {
        /* pass */
        return _tr_str_lit_len("3", 1LL);
    }
    /* pass */
    if ((d == 4LL)) {
        /* pass */
        return _tr_str_lit_len("4", 1LL);
    }
    /* pass */
    if ((d == 5LL)) {
        /* pass */
        return _tr_str_lit_len("5", 1LL);
    }
    /* pass */
    if ((d == 6LL)) {
        /* pass */
        return _tr_str_lit_len("6", 1LL);
    }
    /* pass */
    if ((d == 7LL)) {
        /* pass */
        return _tr_str_lit_len("7", 1LL);
    }
    /* pass */
    if ((d == 8LL)) {
        /* pass */
        return _tr_str_lit_len("8", 1LL);
    }
    /* pass */
    return _tr_str_lit_len("9", 1LL);
}

__attribute__((hot)) TrStr _lir_itoa(long long n) {
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return _tr_str_lit_len("0", 1LL);
    }
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    long long x = n;
    /* pass */
    while ((x > 0LL)) {
        /* pass */
        TrStr _strtmp_t3342 = ({ TrStr _cl = (_lir_digit((x % 10LL))); TrStr _cres = _tr_strx_concatv(_cl, (s)); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t3342;
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
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_release", 18LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_str_release", 18LL), a));
}

__attribute__((hot)) void _retain_str(LModule* m, LFunc* lf, long long v) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_retain", 17LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_str_retain", 17LL), a));
}

__attribute__((hot)) void _flush_fresh_strs(LModule* m, LFunc* lf) {
    /* pass */
    while ((lf->fresh_strs->len > 0LL)) {
        /* pass */
        long long v = List_i64_pop(lf->fresh_strs);
        /* pass */
        _release_str(m, lf, v);
    }
    /* pass */
    while ((lf->fresh_objs->len > 0LL)) {
        /* pass */
        long long ov = List_i64_pop(lf->fresh_objs);
        /* pass */
        _release_obj(m, lf, ov);
    }
}

__attribute__((hot)) void _secure_str(LModule* m, LFunc* lf, long long v) {
    /* pass */
    if ((!_fresh_take(lf, v))) {
        /* pass */
        _retain_str(m, lf, v);
    }
}

__attribute__((hot)) void _fresh_mark_obj(LFunc* lf, long long v) {
    /* pass */
    List_i64_append(lf->fresh_objs, v);
}

__attribute__((hot)) bool _fresh_take_obj(LFunc* lf, long long v) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lf->fresh_objs->len)) {
        /* pass */
        if ((List_i64_get(lf->fresh_objs, i) == v)) {
            /* pass */
            List_i64_remove(lf->fresh_objs, i);
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void _release_obj(LModule* m, LFunc* lf, long long v) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_release", 18LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_release", 18LL), a));
}

__attribute__((hot)) void _retain_obj(LModule* m, LFunc* lf, long long v) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_retain", 17LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, v);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_retain", 17LL), a));
}

__attribute__((hot)) void _flush_fresh_objs(LModule* m, LFunc* lf) {
    /* pass */
    while ((lf->fresh_objs->len > 0LL)) {
        /* pass */
        long long v = List_i64_pop(lf->fresh_objs);
        /* pass */
        _release_obj(m, lf, v);
    }
}

__attribute__((hot)) bool _is_owned_local_return(LFunc* lf, HirExpr* val) {
    /* pass */
    __auto_type _t3343 = (*val);
    if (_t3343.tag == HirExpr_EIdent) {
        __auto_type vn = _t3343.data.EIdent.name;
        /* pass */
        if ((LFunc_var_index(lf, vn) < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if (_is_param(lf, vn)) {
            /* pass */
            return false;
        }
        /* pass */
        long long vt = LFunc_var_type(lf, vn);
        /* pass */
        return ((vt == 10LL) || (vt == 11LL));
    } else if (1) {
        __auto_type _ = _t3343;
        /* pass */
        return false;
    }
}

__attribute__((hot)) void _secure_obj(LModule* m, LFunc* lf, long long v) {
    /* pass */
    if ((!_fresh_take_obj(lf, v))) {
        /* pass */
        _retain_obj(m, lf, v);
    }
}

__attribute__((hot)) bool _is_param(LFunc* lf, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lf->params->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(lf->params, i)), (name))) {
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
    LFunc_emit(lf, LInst_ctor_IBinOp(r, _tr_str_lit_len("!=", 2LL), v, z));
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
    return _heap_lit_len(m, lf, s, _tr_str_lenv((s)));
}

__attribute__((hot)) long long _heap_lit_len(LModule* m, LFunc* lf, TrStr s, long long blen) {
    /* pass */
    long long idx = LModule_add_string(m, s, blen);
    /* pass */
    long long ds = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
    /* pass */
    LFunc_set_vreg_type(lf, ds, 1LL);
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, ds);
    /* pass */
    long long h = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(h, _tr_str_lit_len("_tr_rt_str_new", 14LL), a));
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
    if ((_tr_str_eqv((ocls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, ocls)))) {
        /* pass */
        _tr_str_release(ocls);
        return (-1LL);
    }
    /* pass */
    TrStr osm = LModule_resolve_method(m, ocls, _tr_str_lit_len("__str__", 7LL));
    /* pass */
    if (_tr_str_eqv((osm), (_tr_str_lit_len("", 0LL)))) {
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

__attribute__((hot)) long long _obj_repr(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg) {
    /* pass */
    TrStr rcls = _recv_class(m, lf, objexpr);
    /* pass */
    if ((_tr_str_eqv((rcls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, rcls)))) {
        /* pass */
        _tr_str_release(rcls);
        return (-1LL);
    }
    /* pass */
    TrStr rm = LModule_resolve_method(m, rcls, _tr_str_lit_len("__repr__", 8LL));
    /* pass */
    if (_tr_str_eqv((rm), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        _tr_str_release(rcls);
        _tr_str_release(rm);
        return _obj_to_str(m, lf, objexpr, objreg);
    }
    /* pass */
    long long rself = objreg;
    /* pass */
    if ((rself < 0LL)) {
        /* pass */
        rself = lower_expr(m, lf, objexpr);
        /* pass */
        if ((rself < 0LL)) {
            /* pass */
            _tr_str_release(rcls);
            _tr_str_release(rm);
            return (-1LL);
        }
    }
    /* pass */
    _tr_str_release(rcls);
    return _lower_obj_call(m, lf, rm, rself, (void*)List_ptr_new());
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
        /* pass */
        long long fd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IFCall1(fd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), reg));
        /* pass */
        LFunc_set_vreg_type(lf, fd, 1LL);
        /* pass */
        _fresh_mark(lf, fd);
        /* pass */
        return fd;
    }
    /* pass */
    TrStr sym = _tr_str_lit_len("_tr_rt_i64_to_str", 17LL);
    /* pass */
    if ((t == 4LL)) {
        /* pass */
        TrStr _strtmp_t3344 = _tr_str_lit_len("_tr_rt_bool_to_str", 18LL);
        _tr_str_release(sym);
        sym = _strtmp_t3344;
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

__attribute__((hot)) long long _str_concat2(LModule* m, LFunc* lf, long long a, long long b) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
    /* pass */
    List_i64* ca = (void*)List_i64_new();
    /* pass */
    List_i64_append(ca, a);
    /* pass */
    List_i64_append(ca, b);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, _tr_str_lit_len("_tr_rt_str_concat", 17LL), ca));
    /* pass */
    LFunc_set_vreg_type(lf, d, 1LL);
    /* pass */
    _fresh_mark(lf, d);
    /* pass */
    return d;
}

__attribute__((hot)) long long _tuple_to_str(LModule* m, LFunc* lf, long long tup, AstType* ty) {
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return _heap_lit(m, lf, _tr_str_lit_len("()", 2LL));
    }
    /* pass */
    long long acc = _heap_lit(m, lf, _tr_str_lit_len("(", 1LL));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ty->args->len)) {
        /* pass */
        long long etag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, i))));
        /* pass */
        if (((((((etag < 0LL) || _is_list_tag(etag)) || _is_dict_tag(etag)) || _is_set_tag(etag)) || (etag == 12LL)) || (etag == 15LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long ev = _emit_field_get(m, lf, tup, (i * 8LL), 0LL);
        /* pass */
        if ((etag == 5LL)) {
            /* pass */
            long long fv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBitsF(fv, ev));
            /* pass */
            LFunc_set_vreg_type(lf, fv, 5LL);
            /* pass */
            ev = fv;
        } else {
            /* pass */
            LFunc_set_vreg_type(lf, ev, etag);
        }
        /* pass */
        long long es = _reg_to_str(m, lf, ev);
        /* pass */
        if ((es < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((etag == 1LL)) {
            /* pass */
            es = _str_concat2(m, lf, _heap_lit(m, lf, _tr_str_lit_len("'", 1LL)), es);
            /* pass */
            es = _str_concat2(m, lf, es, _heap_lit(m, lf, _tr_str_lit_len("'", 1LL)));
        }
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            acc = _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(", ", 2LL)));
        }
        /* pass */
        acc = _str_concat2(m, lf, acc, es);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((ty->args->len == 1LL)) {
        /* pass */
        acc = _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(",", 1LL)));
    }
    /* pass */
    return _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(")", 1LL)));
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
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, _tr_v_recv);
        /* pass */
        long long ld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit_len("_tr_rt_strlen", 13LL), la));
        /* pass */
        return ld;
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_upper", 16LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_lower", 16LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_strip", 16LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), _tr_v_recv, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("split_once", 10LL))) && (margs->len == 1LL))) {
        /* pass */
        long long soa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((soa < 0LL) || (LFunc_vreg_type(lf, soa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_split_once_left", 22LL));
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_split_once_right", 23LL));
        /* pass */
        List_i64* sola = (void*)List_i64_new();
        /* pass */
        List_i64_append(sola, _tr_v_recv);
        /* pass */
        List_i64_append(sola, soa);
        /* pass */
        long long sol = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sol, _tr_str_lit_len("_tr_rt_split_once_left", 22LL), sola));
        /* pass */
        LFunc_set_vreg_type(lf, sol, 1LL);
        /* pass */
        _fresh_mark(lf, sol);
        /* pass */
        _secure_str(m, lf, sol);
        /* pass */
        List_i64* sora = (void*)List_i64_new();
        /* pass */
        List_i64_append(sora, _tr_v_recv);
        /* pass */
        List_i64_append(sora, soa);
        /* pass */
        long long sor = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sor, _tr_str_lit_len("_tr_rt_split_once_right", 23LL), sora));
        /* pass */
        LFunc_set_vreg_type(lf, sor, 1LL);
        /* pass */
        _fresh_mark(lf, sor);
        /* pass */
        _secure_str(m, lf, sor);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
        /* pass */
        long long sols = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(sols, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, sols, 3LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_push_i64", 20LL));
        /* pass */
        List_i64* sop1 = (void*)List_i64_new();
        /* pass */
        List_i64_append(sop1, sols);
        /* pass */
        List_i64_append(sop1, sol);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_push_i64", 20LL), sop1));
        /* pass */
        List_i64* sop2 = (void*)List_i64_new();
        /* pass */
        List_i64_append(sop2, sols);
        /* pass */
        List_i64_append(sop2, sor);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_push_i64", 20LL), sop2));
        /* pass */
        return sols;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) && (margs->len == 0LL))) {
        /* pass */
        long long tf0 = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL), _tr_v_recv, 0LL);
        /* pass */
        long long tff = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBitsF(tff, tf0));
        /* pass */
        LFunc_set_vreg_type(lf, tff, 5LL);
        /* pass */
        return tff;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL))) && (margs->len == 2LL))) {
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_replace", 18LL));
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
        LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit_len("_tr_rt_str_replace", 18LL), ra));
        /* pass */
        LFunc_set_vreg_type(lf, rd, 1LL);
        /* pass */
        _fresh_mark(lf, rd);
        /* pass */
        return rd;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("find", 4LL))) && (margs->len == 1LL))) {
        /* pass */
        long long fa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((fa < 0LL) || (LFunc_vreg_type(lf, fa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), _tr_v_recv, fa, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL))) && (margs->len == 1LL))) {
        /* pass */
        long long sa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((sa < 0LL) || (LFunc_vreg_type(lf, sa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_starts_with", 22LL), _tr_v_recv, sa, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ea < 0LL) || (LFunc_vreg_type(lf, ea) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_ends_with", 20LL), _tr_v_recv, ea, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ka = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ka < 0LL) || (LFunc_vreg_type(lf, ka) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_count", 16LL), _tr_v_recv, ka, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) && (margs->len == 1LL))) {
        /* pass */
        long long na = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((na < 0LL) || (LFunc_vreg_type(lf, na) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_contains", 19LL), _tr_v_recv, na, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("char_at", 7LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ia < 0LL) || (LFunc_vreg_type(lf, ia) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_char_at", 18LL), _tr_v_recv, ia, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL))) && (margs->len == 1LL))) {
        /* pass */
        long long pa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pa < 0LL) || (LFunc_vreg_type(lf, pa) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_repeat", 17LL), _tr_v_recv, pa, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))) && (margs->len == 2LL))) {
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
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
        LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_str_slice", 16LL), sla));
        /* pass */
        LFunc_set_vreg_type(lf, sld, 1LL);
        /* pass */
        _fresh_mark(lf, sld);
        /* pass */
        return sld;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_capitalize", 21LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("title", 5LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_title", 16LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("lstrip", 6LL)))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_trim_left", 20LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("rstrip", 6LL)))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_trim_right", 21LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("zfill", 5LL))) && (margs->len == 1LL))) {
        /* pass */
        long long zfa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((zfa < 0LL) || (LFunc_vreg_type(lf, zfa) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_zfill", 16LL), _tr_v_recv, zfa, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("pad_left", 8LL))) && (margs->len == 1LL))) {
        /* pass */
        long long pla = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pla < 0LL) || (LFunc_vreg_type(lf, pla) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_pad_left", 19LL), _tr_v_recv, pla, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("pad_right", 9LL))) && (margs->len == 1LL))) {
        /* pass */
        long long pra = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pra < 0LL) || (LFunc_vreg_type(lf, pra) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_pad_right", 20LL), _tr_v_recv, pra, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains_char", 13LL))) && (margs->len == 1LL))) {
        /* pass */
        long long cca = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((cca < 0LL) || (LFunc_vreg_type(lf, cca) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_contains_char", 24LL), _tr_v_recv, cca, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("center", 6LL))) && (margs->len == 1LL))) {
        /* pass */
        long long cea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((cea < 0LL) || (LFunc_vreg_type(lf, cea) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_center", 17LL), _tr_v_recv, cea, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("chars", 5LL))) && (margs->len == 0LL))) {
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_chars", 16LL));
        /* pass */
        List_i64* cha = (void*)List_i64_new();
        /* pass */
        List_i64_append(cha, _tr_v_recv);
        /* pass */
        long long chd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(chd, _tr_str_lit_len("_tr_rt_str_chars", 16LL), cha));
        /* pass */
        LFunc_set_vreg_type(lf, chd, 3LL);
        /* pass */
        return chd;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("split", 5LL))) && (margs->len == 1LL))) {
        /* pass */
        long long spa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((spa < 0LL) || (LFunc_vreg_type(lf, spa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_split", 16LL));
        /* pass */
        List_i64* spargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(spargs, _tr_v_recv);
        /* pass */
        List_i64_append(spargs, spa);
        /* pass */
        long long spd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(spd, _tr_str_lit_len("_tr_rt_str_split", 16LL), spargs));
        /* pass */
        LFunc_set_vreg_type(lf, spd, 3LL);
        /* pass */
        return spd;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_reverse", 18LL), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_is_empty", 19LL), _tr_v_recv, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_parse_bool", 21LL), _tr_v_recv, 4LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("parse_int", 9LL))) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), _tr_v_recv, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ida = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ida < 0LL) || (LFunc_vreg_type(lf, ida) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), _tr_v_recv, ida, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL))) && (margs->len == 1LL))) {
        /* pass */
        long long lia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((lia < 0LL) || (LFunc_vreg_type(lf, lia) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_last_index_of", 24LL), _tr_v_recv, lia, 0LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ppa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ppa < 0LL) || (LFunc_vreg_type(lf, ppa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_strip_prefix", 23LL), _tr_v_recv, ppa, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL))) && (margs->len == 1LL))) {
        /* pass */
        long long ssa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ssa < 0LL) || (LFunc_vreg_type(lf, ssa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_strip_suffix", 23LL), _tr_v_recv, ssa, 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("replace_first", 13LL))) && (margs->len == 2LL))) {
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_replace_first", 24LL));
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
        LFunc_emit(lf, LInst_ctor_ICall(rfd, _tr_str_lit_len("_tr_rt_str_replace_first", 24LL), rfa));
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
    if (_tr_str_eqv((method), (_tr_str_lit_len("sqrt", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_sqrt", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("floor", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_floor", 12LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("ceil", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_ceil", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("round", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_round", 12LL);
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fabs", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_fabs", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("log", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_log", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("log2", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_log2", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("log10", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_log10", 12LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("exp", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_exp", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("sin", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_sin", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("cos", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_cos", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("tan", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_tan", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("asin", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_asin", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("acos", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_acos", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((method), (_tr_str_lit_len("atan", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("_tr_rt_atan", 11LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _lower_int_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
    /* pass */
    if ((margs->len == 0LL)) {
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
            /* pass */
            return _promote_f(lf, _tr_v_recv);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("to_hex", 6LL)))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_hex", 17LL), _tr_v_recv, 1LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_hex_upper", 12LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_HEX", 6LL))))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_hex_upper", 23LL), _tr_v_recv, 1LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_oct", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_octal", 8LL))))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_oct", 17LL), _tr_v_recv, 1LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_bin", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_binary", 9LL))))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_bin", 17LL), _tr_v_recv, 1LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
            /* pass */
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_str", 17LL), _tr_v_recv, 1LL);
        }
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("sign", 4LL)))) {
            /* pass */
            long long z = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
            /* pass */
            long long gt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(gt, _tr_str_lit_len(">", 1LL), _tr_v_recv, z));
            /* pass */
            long long lt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(lt, _tr_str_lit_len("<", 1LL), _tr_v_recv, z));
            /* pass */
            long long sd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(sd, _tr_str_lit_len("-", 1LL), gt, lt));
            /* pass */
            return sd;
        }
        /* pass */
        return (-1LL);
    }
    /* pass */
    if (((margs->len == 1LL) && _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))))) {
        /* pass */
        long long pe = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((pe < 0LL) || (LFunc_vreg_type(lf, pe) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_int_pow", 14LL));
        /* pass */
        List_i64* ppa = (void*)List_i64_new();
        /* pass */
        List_i64_append(ppa, _tr_v_recv);
        /* pass */
        List_i64_append(ppa, pe);
        /* pass */
        long long ppd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ppd, _tr_str_lit_len("_tr_rt_int_pow", 14LL), ppa));
        /* pass */
        return ppd;
    }
    /* pass */
    if (((margs->len == 1LL) && (_tr_str_eqv((method), (_tr_str_lit_len("gcd", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))))) {
        /* pass */
        long long y = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((y < 0LL) || (LFunc_vreg_type(lf, y) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr gsym = _tr_str_lit_len("_tr_rt_gcd_i64", 14LL);
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))) {
            /* pass */
            TrStr _strtmp_t3345 = _tr_str_lit_len("_tr_rt_lcm_i64", 14LL);
            _tr_str_release(gsym);
            gsym = _strtmp_t3345;
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
    if (((margs->len == 2LL) && _tr_str_eqv((method), (_tr_str_lit_len("clamp", 5LL))))) {
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_clamp_i64", 16LL));
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
        LFunc_emit(lf, LInst_ctor_ICall(cd, _tr_str_lit_len("_tr_rt_clamp_i64", 16LL), ca));
        /* pass */
        return cd;
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long _lower_dict_method(LModule* m, LFunc* lf, long long _tr_v_recv, long long dtag, TrStr method, List_ptr* margs) {
    /* pass */
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
        /* pass */
        TrStr lsym = _dict_sym(dtag, _tr_str_lit_len("len", 3LL));
        /* pass */
        LModule_add_extern(m, lsym);
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, _tr_v_recv);
        /* pass */
        long long ld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ld, lsym, la));
        /* pass */
        LFunc_set_vreg_type(lf, ld, 0LL);
        /* pass */
        _tr_str_release(lsym);
        return ld;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))) && (margs->len == 1LL))) {
        /* pass */
        long long rk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if ((rk < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, rk) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, rk) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        TrStr rsym = _dict_sym(dtag, _tr_str_lit_len("remove", 6LL));
        /* pass */
        LModule_add_extern(m, rsym);
        /* pass */
        List_i64* ra = (void*)List_i64_new();
        /* pass */
        List_i64_append(ra, _tr_v_recv);
        /* pass */
        List_i64_append(ra, rk);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), rsym, ra));
        /* pass */
        _tr_str_release(rsym);
        return _tr_v_recv;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL))) && (margs->len == 0LL))) {
        /* pass */
        TrStr ksym = _tr_str_lit_len("_tr_rt_idict_keys", 17LL);
        /* pass */
        long long ktag = 2LL;
        /* pass */
        if (_dict_key_is_str(dtag)) {
            /* pass */
            TrStr _strtmp_t3346 = _tr_str_lit_len("_tr_rt_sdict_keys", 17LL);
            _tr_str_release(ksym);
            ksym = _strtmp_t3346;
            /* pass */
            ktag = 3LL;
        }
        /* pass */
        LModule_add_extern(m, ksym);
        /* pass */
        List_i64* ka = (void*)List_i64_new();
        /* pass */
        List_i64_append(ka, _tr_v_recv);
        /* pass */
        long long kd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(kd, ksym, ka));
        /* pass */
        LFunc_set_vreg_type(lf, kd, ktag);
        /* pass */
        _tr_str_release(ksym);
        return kd;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))) && (margs->len == 2LL))) {
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
        TrStr gosym = _dict_sym(dtag, _tr_str_lit_len("get_or", 6LL));
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
    if (((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) && (margs->len == 2LL))) {
        /* pass */
        long long sk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if ((sk < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, sk) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, sk) != 0LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long sv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if ((sv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long vtag = _dict_val_tag(dtag);
        /* pass */
        if ((LFunc_vreg_type(lf, sv) != vtag)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((vtag == 1LL)) {
            /* pass */
            _secure_str(m, lf, sv);
        }
        /* pass */
        if (((vtag == 10LL) || (vtag == 11LL))) {
            /* pass */
            _secure_obj(m, lf, sv);
        }
        /* pass */
        if ((vtag == 5LL)) {
            /* pass */
            long long svfb = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFBits(svfb, sv));
            /* pass */
            sv = svfb;
        }
        /* pass */
        ({ TrStr _at_t3347 = (_dict_sym(dtag, _tr_str_lit_len("set", 3LL))); LModule_add_extern(m, _at_t3347); _tr_str_release(_at_t3347); });
        /* pass */
        List_i64* ssa = (void*)List_i64_new();
        /* pass */
        List_i64_append(ssa, _tr_v_recv);
        /* pass */
        List_i64_append(ssa, sk);
        /* pass */
        List_i64_append(ssa, sv);
        /* pass */
        ({ TrStr _at_t3348 = (_dict_sym(dtag, _tr_str_lit_len("set", 3LL))); LFunc_emit(lf, LInst_ctor_ICall((-1LL), _at_t3348, ssa)); _tr_str_release(_at_t3348); });
        /* pass */
        return _tr_v_recv;
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
    if ((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))))) {
        /* pass */
        TrStr gsym = _dict_sym(dtag, _tr_str_lit_len("get", 3LL));
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
        if (((_dict_val_tag(dtag) == 10LL) || (_dict_val_tag(dtag) == 11LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_retain", 17LL));
            /* pass */
            List_i64* _gra = (void*)List_i64_new();
            /* pass */
            List_i64_append(_gra, gd);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_retain", 17LL), _gra));
            /* pass */
            _fresh_mark_obj(lf, gd);
        }
        /* pass */
        _tr_str_release(gsym);
        return gd;
    }
    /* pass */
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))))) {
        /* pass */
        TrStr hsym = _dict_sym(dtag, _tr_str_lit_len("has", 3LL));
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
        if ((!_tr_str_eqv((usym), (_tr_str_lit_len("", 0LL))))) {
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
        if ((_tr_str_eqv((method), (_tr_str_lit_len("is_nan", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL))))) {
            /* pass */
            TrStr nsym = _tr_str_lit_len("_tr_rt_f64_is_nan", 17LL);
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL)))) {
                /* pass */
                TrStr _strtmp_t3349 = _tr_str_lit_len("_tr_rt_f64_is_inf", 17LL);
                _tr_str_release(nsym);
                nsym = _strtmp_t3349;
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
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
            /* pass */
            long long fsd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCall1(fsd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), _tr_v_recv));
            /* pass */
            LFunc_set_vreg_type(lf, fsd, 1LL);
            /* pass */
            _fresh_mark(lf, fsd);
            /* pass */
            _tr_str_release(usym);
            return fsd;
        }
        /* pass */
        _tr_str_release(usym);
        return (-1LL);
    }
    /* pass */
    if (((margs->len == 1LL) && (_tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))))) {
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
        TrStr psym = _tr_str_lit_len("_tr_rt_pow", 10LL);
        /* pass */
        if (_tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))) {
            /* pass */
            TrStr _strtmp_t3350 = _tr_str_lit_len("_tr_rt_atan2", 12LL);
            _tr_str_release(psym);
            psym = _strtmp_t3350;
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
    __auto_type _t3351 = (*e);
    if (_t3351.tag == HirExpr_ELitInt) {
        return true;
    } else if (_t3351.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3351.data.EUnaryOp.op;
__auto_type sub = _t3351.data.EUnaryOp.expr;
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
            /* pass */
            __auto_type _t3352 = (*sub);
            if (_t3352.tag == HirExpr_ELitInt) {
                return true;
            } else if (1) {
                __auto_type _ = _t3352;
                return false;
            }
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t3351;
        return false;
    }
}

__attribute__((hot)) long long _const_int_val(HirExpr* e) {
    /* pass */
    __auto_type _t3353 = (*e);
    if (_t3353.tag == HirExpr_ELitInt) {
        __auto_type v = _t3353.data.ELitInt.val;
        return v;
    } else if (_t3353.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3353.data.EUnaryOp.op;
__auto_type sub = _t3353.data.EUnaryOp.expr;
        /* pass */
        __auto_type _t3354 = (*sub);
        if (_t3354.tag == HirExpr_ELitInt) {
            __auto_type v2 = _t3354.data.ELitInt.val;
            return (0LL - v2);
        } else if (1) {
            __auto_type _ = _t3354;
            return 0LL;
        }
    } else if (1) {
        __auto_type _ = _t3353;
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
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit_len("+", 1LL), cur, d));
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

__attribute__((hot)) long long _inline_list_addr(LModule* m, LFunc* lf, long long handle, long long idx, long long stride) {
    /* pass */
    long long data = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, data, 10LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoad(data, handle, 0LL, 1LL));
    /* pass */
    long long off = idx;
    /* pass */
    if ((stride != 1LL)) {
        /* pass */
        long long c8 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(c8, stride));
        /* pass */
        off = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(off, _tr_str_lit_len("*", 1LL), idx, c8));
    }
    /* pass */
    long long addr = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, addr, 10LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(addr, _tr_str_lit_len("+", 1LL), data, off));
    /* pass */
    return addr;
}

__attribute__((hot)) long long _fixed_arr_len(LModule* m, LFunc* lf, TrStr name) {
    /* pass */
    if ((LFunc_var_index(lf, name) >= 0LL)) {
        /* pass */
        return LFunc_var_arr_of(lf, name);
    }
    /* pass */
    return LModule_global_arr_of(m, name);
}

__attribute__((hot)) long long _fixed_arr_elem_tag(LModule* m, LFunc* lf, TrStr name) {
    /* pass */
    if ((LFunc_var_index(lf, name) >= 0LL)) {
        /* pass */
        return LFunc_var_type(lf, name);
    }
    /* pass */
    return LModule_global_type(m, name);
}

__attribute__((hot)) long long _array_elem_addr(LModule* m, LFunc* lf, TrStr name, long long idx) {
    /* pass */
    long long base = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, base, 10LL);
    /* pass */
    if ((LFunc_var_index(lf, name) >= 0LL)) {
        /* pass */
        LFunc_emit(lf, LInst_ctor_IAddrVar(base, name));
    } else {
        /* pass */
        LFunc_emit(lf, LInst_ctor_IAddrGlobal(base, LModule_global_index(m, name)));
    }
    /* pass */
    long long c8 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(c8, 8LL));
    /* pass */
    long long off = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(off, _tr_str_lit_len("*", 1LL), idx, c8));
    /* pass */
    long long addr = LFunc_new_vreg(lf);
    /* pass */
    LFunc_set_vreg_type(lf, addr, 10LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(addr, _tr_str_lit_len("+", 1LL), base, off));
    /* pass */
    return addr;
}

__attribute__((hot)) long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx) {
    /* pass */
    long long addr = _inline_list_addr(m, lf, handle, idx, 8LL);
    /* pass */
    long long gd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoad(gd, addr, 0LL, 2LL));
    /* pass */
    return gd;
}

__attribute__((hot)) long long _list_get_raw(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx) {
    /* pass */
    if ((ltag == 22LL)) {
        /* pass */
        long long baddr = _inline_list_addr(m, lf, handle, idx, 1LL);
        /* pass */
        long long bd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadB(bd, baddr, 0LL));
        /* pass */
        return bd;
    }
    /* pass */
    return _list_get(m, lf, handle, idx);
}

__attribute__((hot)) long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx) {
    /* pass */
    long long raw = _list_get_raw(m, lf, ltag, handle, idx);
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

__attribute__((hot)) long long _lower_expr_impl(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t3355 = (*e);
    if (_t3355.tag == HirExpr_ELitInt) {
        __auto_type v = _t3355.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t3355.tag == HirExpr_ELitStr) {
        __auto_type sv = _t3355.data.ELitStr.val;
__auto_type sv_blen = _t3355.data.ELitStr.blen;
        /* pass */
        long long idx = LModule_add_string(m, sv, sv_blen);
        /* pass */
        long long ds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
        /* pass */
        LFunc_set_vreg_type(lf, ds, 1LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
        /* pass */
        List_i64* lna = (void*)List_i64_new();
        /* pass */
        List_i64_append(lna, ds);
        /* pass */
        long long lheap = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(lheap, _tr_str_lit_len("_tr_rt_str_new", 14LL), lna));
        /* pass */
        LFunc_set_vreg_type(lf, lheap, 1LL);
        /* pass */
        _fresh_mark(lf, lheap);
        /* pass */
        return lheap;
    } else if (_t3355.tag == HirExpr_ELitBool) {
        __auto_type bval = _t3355.data.ELitBool.val;
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
    } else if (_t3355.tag == HirExpr_ELitChar) {
        __auto_type cval = _t3355.data.ELitChar.val;
        /* pass */
        long long cd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(cd, cval));
        /* pass */
        return cd;
    } else if (_t3355.tag == HirExpr_ESizeOf) {
        __auto_type sz_ty = _t3355.data.ESizeOf.target_ty;
        /* pass */
        TrStr _szn = _tr_str_retain(_subst_ty(m, sz_ty)->name);
        /* pass */
        long long _szval = _sizeof_tyname(_szn);
        /* pass */
        if (LModule_is_value_class(m, _szn)) {
            /* pass */
            _szval = LModule_class_size(m, _szn);
        } else {
            /* pass */
            long long _szg = _value_struct_generic_size(m, sz_ty);
            /* pass */
            if ((_szg > 0LL)) {
                /* pass */
                _szval = _szg;
            }
        }
        /* pass */
        long long szd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(szd, _szval));
        /* pass */
        _tr_str_release(_szn);
        return szd;
    } else if (_t3355.tag == HirExpr_ETryExpr) {
        __auto_type texpr = _t3355.data.ETryExpr.expr;
__auto_type tokty = _t3355.data.ETryExpr.ty;
        /* pass */
        if ((!lf->is_throws)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long trv = lower_expr(m, lf, texpr);
        /* pass */
        if (((trv < 0LL) || (LFunc_vreg_type(lf, trv) != 11LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long tuid = LFunc_fresh_id(lf);
        /* pass */
        TrStr tnm = ({ TrStr _cr = (_lir_itoa(tuid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__try", 5LL)), _cr); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, tnm);
        /* pass */
        LFunc_set_var_type(lf, tnm, 11LL);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(tnm, trv));
        /* pass */
        long long trl = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl, tnm));
        /* pass */
        LFunc_set_vreg_type(lf, trl, 11LL);
        /* pass */
        long long ttag = _emit_field_get(m, lf, trl, 0LL, 0LL);
        /* pass */
        long long terrc = _variant_tag_cond(lf, ttag, 1LL);
        /* pass */
        long long terrb = LFunc_new_block(lf);
        /* pass */
        long long tcont = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(terrc, terrb, tcont));
        /* pass */
        LFunc_set_cur(lf, terrb);
        /* pass */
        long long trl2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl2, tnm));
        /* pass */
        LFunc_set_vreg_type(lf, trl2, 11LL);
        /* pass */
        if ((!_run_defers(m, lf))) {
            /* pass */
            _tr_str_release(tnm);
            return (-1LL);
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TRetVal(trl2));
        /* pass */
        LFunc_set_cur(lf, tcont);
        /* pass */
        long long toktag = _tag_of(m, tokty);
        /* pass */
        long long trl3 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl3, tnm));
        /* pass */
        LFunc_set_vreg_type(lf, trl3, 11LL);
        /* pass */
        if ((toktag == 5LL)) {
            /* pass */
            long long tpraw = _emit_field_get(m, lf, trl3, 8LL, 0LL);
            /* pass */
            long long tpfv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBitsF(tpfv, tpraw));
            /* pass */
            LFunc_set_vreg_type(lf, tpfv, 5LL);
            /* pass */
            _tr_str_release(tnm);
            return tpfv;
        }
        /* pass */
        _tr_str_release(tnm);
        return _emit_field_get(m, lf, trl3, 8LL, toktag);
    } else if (_t3355.tag == HirExpr_EAwait) {
        __auto_type awexpr = _t3355.data.EAwait.expr;
        /* pass */
        return _lower_await(m, lf, awexpr);
    } else if (_t3355.tag == HirExpr_EAwaitTimeout) {
        __auto_type awtexpr = _t3355.data.EAwaitTimeout.expr;
        /* pass */
        return _lower_await(m, lf, awtexpr);
    } else if (_t3355.tag == HirExpr_EListComp) {
        __auto_type lcelem = _t3355.data.EListComp.element;
__auto_type lcgens = _t3355.data.EListComp.generators;
__auto_type lcty = _t3355.data.EListComp.ty;
        /* pass */
        return _lower_list_comp(m, lf, lcelem, lcgens, lcty);
    } else if (_t3355.tag == HirExpr_EGeneratorExpr) {
        __auto_type geelem = _t3355.data.EGeneratorExpr.element;
__auto_type geegens = _t3355.data.EGeneratorExpr.generators;
__auto_type geety = _t3355.data.EGeneratorExpr.ty;
        /* pass */
        return _lower_list_comp(m, lf, geelem, geegens, geety);
    } else if (_t3355.tag == HirExpr_ELitNone) {
        __auto_type nty = _t3355.data.ELitNone.ty;
        /* pass */
        if ((((_tr_str_eqv((nty->name), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((nty->name), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((nty->name), (_tr_str_lit_len("NoneType", 8LL)))) || _is_null_str(nty->name))) {
            /* pass */
            return _lower_enum_ctor(m, lf, _tr_str_lit_len("Option", 6LL), _tr_str_lit_len("None", 4LL), (void*)List_ptr_new());
        }
        /* pass */
        long long nnull = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(nnull, 0LL));
        /* pass */
        LFunc_set_vreg_type(lf, nnull, 10LL);
        /* pass */
        return nnull;
    } else if (_t3355.tag == HirExpr_ERawStr) {
        __auto_type rsv = _t3355.data.ERawStr.val;
__auto_type rsv_blen = _t3355.data.ERawStr.blen;
        /* pass */
        long long ridx = LModule_add_string(m, rsv, rsv_blen);
        /* pass */
        long long rds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(rds, ridx));
        /* pass */
        LFunc_set_vreg_type(lf, rds, 1LL);
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
        /* pass */
        List_i64* rna = (void*)List_i64_new();
        /* pass */
        List_i64_append(rna, rds);
        /* pass */
        long long rheap = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(rheap, _tr_str_lit_len("_tr_rt_str_new", 14LL), rna));
        /* pass */
        LFunc_set_vreg_type(lf, rheap, 1LL);
        /* pass */
        _fresh_mark(lf, rheap);
        /* pass */
        return rheap;
    } else if (_t3355.tag == HirExpr_ELitFloat) {
        __auto_type fval = _t3355.data.ELitFloat.val;
        /* pass */
        long long fd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(fd, _f64_bits(fval)));
        /* pass */
        LFunc_set_vreg_type(lf, fd, 5LL);
        /* pass */
        return fd;
    } else if (_t3355.tag == HirExpr_ESuperMethodCall) {
        __auto_type sbase = _t3355.data.ESuperMethodCall.base_class;
__auto_type smeth = _t3355.data.ESuperMethodCall.method;
__auto_type sargs = _t3355.data.ESuperMethodCall.args;
        /* pass */
        TrStr smang = LModule_resolve_method(m, sbase, smeth);
        /* pass */
        if (_tr_str_eqv((smang), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            _tr_str_release(smang);
            return (-1LL);
        }
        /* pass */
        long long sselfv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(sselfv, _tr_str_lit_len("self", 4LL)));
        /* pass */
        LFunc_set_vreg_type(lf, sselfv, LFunc_var_type(lf, _tr_str_lit_len("self", 4LL)));
        /* pass */
        return _lower_obj_call(m, lf, smang, sselfv, sargs);
    } else if (_t3355.tag == HirExpr_ESuperPropAccess) {
        __auto_type sbase2 = _t3355.data.ESuperPropAccess.base_class;
__auto_type sprop = _t3355.data.ESuperPropAccess.prop;
        /* pass */
        TrStr scur = LFunc_var_cls_of(lf, _tr_str_lit_len("self", 4LL));
        /* pass */
        if ((_tr_str_eqv((scur), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, scur)))) {
            /* pass */
            _tr_str_release(scur);
            return (-1LL);
        }
        /* pass */
        long long sfoff = LModule_field_offset(m, scur, sprop);
        /* pass */
        if ((sfoff < 0LL)) {
            /* pass */
            _tr_str_release(scur);
            return (-1LL);
        }
        /* pass */
        long long sftg = LModule_field_tag(m, scur, sprop);
        /* pass */
        if ((((sftg < 0LL) || _is_list_tag(sftg)) || _is_dict_tag(sftg))) {
            /* pass */
            _tr_str_release(scur);
            return (-1LL);
        }
        /* pass */
        long long sselfp = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(sselfp, _tr_str_lit_len("self", 4LL)));
        /* pass */
        LFunc_set_vreg_type(lf, sselfp, LFunc_var_type(lf, _tr_str_lit_len("self", 4LL)));
        /* pass */
        if ((sftg == 5LL)) {
            /* pass */
            long long sfraw = _emit_field_get(m, lf, sselfp, sfoff, 0LL);
            /* pass */
            long long sffv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBitsF(sffv, sfraw));
            /* pass */
            LFunc_set_vreg_type(lf, sffv, 5LL);
            /* pass */
            _tr_str_release(scur);
            return sffv;
        }
        /* pass */
        _tr_str_release(scur);
        return _emit_field_get(m, lf, sselfp, sfoff, sftg);
    } else if (_t3355.tag == HirExpr_ECast) {
        __auto_type cinner = _t3355.data.ECast.expr;
__auto_type ctty = _t3355.data.ECast.target_ty;
        /* pass */
        long long cv = lower_expr(m, lf, cinner);
        /* pass */
        if ((cv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long cvt = LFunc_vreg_type(lf, cv);
        /* pass */
        TrStr ctn = _tr_str_retain(ctty->name);
        /* pass */
        if (_is_null_str(ctn)) {
            /* pass */
            _tr_str_release(ctn);
            return (-1LL);
        }
        /* pass */
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("f32", 3LL))))) {
            /* pass */
            if ((cvt == 5LL)) {
                /* pass */
                _tr_str_release(ctn);
                return cv;
            }
            /* pass */
            if (((cvt == 0LL) || (cvt == 4LL))) {
                /* pass */
                _tr_str_release(ctn);
                return _promote_f(lf, cv);
            }
            /* pass */
            _tr_str_release(ctn);
            return (-1LL);
        }
        /* pass */
        if (_is_int_cast_target(ctn)) {
            /* pass */
            long long r = cv;
            /* pass */
            if ((cvt == 5LL)) {
                /* pass */
                r = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFToI(r, cv));
                /* pass */
                LFunc_set_vreg_type(lf, r, 0LL);
            } else if (((cvt != 0LL) && (cvt != 4LL))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL));
                /* pass */
                List_i64* cpa = (void*)List_i64_new();
                /* pass */
                List_i64_append(cpa, cv);
                /* pass */
                r = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(r, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL), cpa));
                /* pass */
                LFunc_set_vreg_type(lf, r, 0LL);
            }
            /* pass */
            r = _narrow_int(lf, r, ctn);
            /* pass */
            if (_tr_str_eqv((ctn), (_tr_str_lit_len("bool", 4LL)))) {
                /* pass */
                long long rb = _norm_bool(lf, r);
                /* pass */
                LFunc_set_vreg_type(lf, rb, 4LL);
                /* pass */
                _tr_str_release(ctn);
                return rb;
            }
            /* pass */
            LFunc_set_vreg_type(lf, r, 0LL);
            /* pass */
            _tr_str_release(ctn);
            return r;
        }
        /* pass */
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("String", 6LL)))) && (cvt == 1LL))) {
            /* pass */
            _tr_str_release(ctn);
            return cv;
        }
        /* pass */
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("String", 6LL)))) && (cvt == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
            /* pass */
            List_i64* sna = (void*)List_i64_new();
            /* pass */
            List_i64_append(sna, cv);
            /* pass */
            long long snd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(snd, _tr_str_lit_len("_tr_rt_str_new", 14LL), sna));
            /* pass */
            LFunc_set_vreg_type(lf, snd, 1LL);
            /* pass */
            _fresh_mark(lf, snd);
            /* pass */
            _tr_str_release(ctn);
            return snd;
        }
        /* pass */
        if ((_tr_str_eqv((ctn), (_tr_str_lit_len("char", 4LL))) && ((cvt == 0LL) || (cvt == 4LL)))) {
            /* pass */
            LFunc_set_vreg_type(lf, cv, 0LL);
            /* pass */
            _tr_str_release(ctn);
            return cv;
        }
        /* pass */
        if (_tr_str_eqv((ctn), (_tr_str_lit_len("Pointer", 7LL)))) {
            /* pass */
            if ((cvt == 0LL)) {
                /* pass */
                _tr_str_release(ctn);
                return cv;
            }
            /* pass */
            if (((((cvt == 1LL) || (cvt == 10LL)) || (cvt == 11LL)) || (cvt == 12LL))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL));
                /* pass */
                List_i64* paa = (void*)List_i64_new();
                /* pass */
                List_i64_append(paa, cv);
                /* pass */
                long long pad = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(pad, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL), paa));
                /* pass */
                LFunc_set_vreg_type(lf, pad, 0LL);
                /* pass */
                _tr_str_release(ctn);
                return pad;
            }
            /* pass */
            _tr_str_release(ctn);
            return (-1LL);
        }
        /* pass */
        _tr_str_release(ctn);
        return (-1LL);
    } else if (_t3355.tag == HirExpr_EIdent) {
        __auto_type name = _t3355.data.EIdent.name;
        /* pass */
        if (((LFunc_var_index(lf, name) < 0LL) && (LFunc_capture_index(lf, name) >= 0LL))) {
            /* pass */
            long long cix = LFunc_capture_index(lf, name);
            /* pass */
            long long cenv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(cenv, _tr_str_lit_len("__env", 5LL)));
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
        if (((LFunc_var_index(lf, name) < 0LL) && LModule_is_user_fn(m, name))) {
            /* pass */
            long long fav = LFunc_new_vreg(lf);
            /* pass */
            ({ TrStr _at_t3356 = (_own(name)); LFunc_emit(lf, LInst_ctor_IFuncAddr(fav, _at_t3356)); _tr_str_release(_at_t3356); });
            /* pass */
            return fav;
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
    } else if (_t3355.tag == HirExpr_EIfElse) {
        __auto_type cond = _t3355.data.EIfElse.cond;
__auto_type then_e = _t3355.data.EIfElse.then_e;
__auto_type else_e = _t3355.data.EIfElse.else_e;
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
        TrStr rname = ({ TrStr _cr = (_lir_itoa(tuid)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__tern", 6LL)), _cr); _tr_str_release(_cr); _cres; });
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
    } else if (_t3355.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3355.data.EUnaryOp.op;
__auto_type sub = _t3355.data.EUnaryOp.expr;
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
            /* pass */
            TrStr ncls = _recv_class(m, lf, sub);
            /* pass */
            if (((!_tr_str_eqv((ncls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, ncls))) {
                /* pass */
                TrStr nm2 = LModule_resolve_method(m, ncls, _tr_str_lit_len("__neg__", 7LL));
                /* pass */
                if ((!_tr_str_eqv((nm2), (_tr_str_lit_len("", 0LL))))) {
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
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
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
                LFunc_emit(lf, LInst_ctor_IFBinOp(dnf, _tr_str_lit_len("-", 1LL), zf, sv));
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
            LFunc_emit(lf, LInst_ctor_IBinOp(dneg, _tr_str_lit_len("-", 1LL), zn, sv));
            /* pass */
            return dneg;
        }
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL)))) {
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
            LFunc_emit(lf, LInst_ctor_IBinOp(dnot, _tr_str_lit_len("==", 2LL), sv, zt));
            /* pass */
            LFunc_set_vreg_type(lf, dnot, 4LL);
            /* pass */
            return dnot;
        }
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("~", 1LL)))) {
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
            LFunc_emit(lf, LInst_ctor_IBinOp(dcpl, _tr_str_lit_len("^", 1LL), sv, ones));
            /* pass */
            return dcpl;
        }
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
            /* pass */
            __auto_type _t3357 = (*sub);
            if (_t3357.tag == HirExpr_EIdent) {
                __auto_type aname = _t3357.data.EIdent.name;
                /* pass */
                long long ad = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IAddrVar(ad, aname));
                /* pass */
                LFunc_set_vreg_type(lf, ad, 0LL);
                /* pass */
                return ad;
            } else if (1) {
                __auto_type _ = _t3357;
                /* pass */
                return (-1LL);
            }
        }
        /* pass */
        return (-1LL);
    } else if (_t3355.tag == HirExpr_EBinOp) {
        __auto_type op = _t3355.data.EBinOp.op;
__auto_type l = _t3355.data.EBinOp.left;
__auto_type r = _t3355.data.EBinOp.right;
        /* pass */
        if ((_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
            /* pass */
            long long scl = lower_expr(m, lf, l);
            /* pass */
            if ((scl < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long scla = _norm_bool(lf, scl);
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            TrStr scvar = ({ TrStr _cr = (_lir_itoa(LFunc_fresh_id(lf))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("__sc", 4LL)), _cr); _tr_str_release(_cr); _cres; });
            /* pass */
            LFunc_add_var(lf, scvar);
            /* pass */
            LFunc_set_var_type(lf, scvar, 4LL);
            /* pass */
            long long sc_rhs = LFunc_new_block(lf);
            /* pass */
            long long sc_sc = LFunc_new_block(lf);
            /* pass */
            long long sc_end = LFunc_new_block(lf);
            /* pass */
            if (_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) {
                /* pass */
                LFunc_set_term(lf, LTerm_ctor_TCondBr(scla, sc_rhs, sc_sc));
            } else {
                /* pass */
                LFunc_set_term(lf, LTerm_ctor_TCondBr(scla, sc_sc, sc_rhs));
            }
            /* pass */
            LFunc_set_cur(lf, sc_rhs);
            /* pass */
            long long scr = lower_expr(m, lf, r);
            /* pass */
            if ((scr < 0LL)) {
                /* pass */
                _tr_str_release(scvar);
                return (-1LL);
            }
            /* pass */
            long long scrb = _norm_bool(lf, scr);
            /* pass */
            _flush_fresh_strs(m, lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(scvar, scrb));
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TBr(sc_end));
            /* pass */
            LFunc_set_cur(lf, sc_sc);
            /* pass */
            long long sccv = LFunc_new_vreg(lf);
            /* pass */
            long long scconst = 0LL;
            /* pass */
            if (_tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) {
                /* pass */
                scconst = 1LL;
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(sccv, scconst));
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreVar(scvar, sccv));
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TBr(sc_end));
            /* pass */
            LFunc_set_cur(lf, sc_end);
            /* pass */
            long long scd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(scd, scvar));
            /* pass */
            LFunc_set_vreg_type(lf, scd, 4LL);
            /* pass */
            _tr_str_release(scvar);
            return scd;
        }
        /* pass */
        TrStr ddn = _dunder_for_op(op);
        /* pass */
        if ((!_tr_str_eqv((ddn), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr lcls_d = _recv_class(m, lf, l);
            /* pass */
            if (((!_tr_str_eqv((lcls_d), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, lcls_d))) {
                /* pass */
                TrStr ddm = LModule_resolve_method(m, lcls_d, ddn);
                /* pass */
                if ((!_tr_str_eqv((ddm), (_tr_str_lit_len("", 0LL))))) {
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
            if ((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL))))) {
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
            if (_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_pow", 10LL));
                /* pass */
                long long fpw = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCall2F(fpw, _tr_str_lit_len("_tr_rt_pow", 10LL), fa, fb));
                /* pass */
                LFunc_set_vreg_type(lf, fpw, 5LL);
                /* pass */
                _tr_str_release(ddn);
                return fpw;
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
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && (bt == 10LL))) {
            /* pass */
            TrStr ccls = _recv_class(m, lf, r);
            /* pass */
            if (((!_tr_str_eqv((ccls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, ccls))) {
                /* pass */
                TrStr ccm = LModule_resolve_method(m, ccls, _tr_str_lit_len("__contains__", 12LL));
                /* pass */
                if ((!_tr_str_eqv((ccm), (_tr_str_lit_len("", 0LL))))) {
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
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && _is_dict_tag(bt))) {
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
            TrStr dhsym = _dict_sym(bt, _tr_str_lit_len("has", 3LL));
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
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && _is_set_tag(bt))) {
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
            TrStr shas = _set_sym(bt, _tr_str_lit_len("has", 3LL));
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
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && (bt == 1LL))) {
            /* pass */
            TrStr ssym = _tr_str_lit_len("_tr_rt_str_contains", 19LL);
            /* pass */
            if (((at == 0LL) || (at == 4LL))) {
                /* pass */
                TrStr _strtmp_t3358 = _tr_str_lit_len("_tr_rt_str_contains_char", 24LL);
                _tr_str_release(ssym);
                ssym = _strtmp_t3358;
            } else if ((at != 1LL)) {
                /* pass */
                _tr_str_release(ddn);
                _tr_str_release(ssym);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, ssym);
            /* pass */
            List_i64* ssa2 = (void*)List_i64_new();
            /* pass */
            List_i64_append(ssa2, b);
            /* pass */
            List_i64_append(ssa2, a);
            /* pass */
            long long ssd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ssd, ssym, ssa2));
            /* pass */
            LFunc_set_vreg_type(lf, ssd, 4LL);
            /* pass */
            _tr_str_release(ddn);
            _tr_str_release(ssym);
            return ssd;
        }
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL)))) {
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
            TrStr csym = _tr_str_lit_len("_tr_rt_list_contains_i64", 24LL);
            /* pass */
            if ((want_e == 1LL)) {
                /* pass */
                TrStr _strtmp_t3359 = _tr_str_lit_len("_tr_rt_list_contains_str", 24LL);
                _tr_str_release(csym);
                csym = _strtmp_t3359;
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
        if (((_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && ((at == 1LL) || (bt == 1LL))) && ((at == 0LL) || (bt == 0LL)))) {
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_repeat", 17LL));
            /* pass */
            List_i64* ra = (void*)List_i64_new();
            /* pass */
            List_i64_append(ra, sreg);
            /* pass */
            List_i64_append(ra, nreg);
            /* pass */
            long long rd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit_len("_tr_rt_str_repeat", 17LL), ra));
            /* pass */
            LFunc_set_vreg_type(lf, rd, 1LL);
            /* pass */
            _fresh_mark(lf, rd);
            /* pass */
            _tr_str_release(ddn);
            return rd;
        }
        /* pass */
        if (((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && (at == 1LL)) && (bt == 1LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
            /* pass */
            List_i64* ca = (void*)List_i64_new();
            /* pass */
            List_i64_append(ca, a);
            /* pass */
            List_i64_append(ca, b);
            /* pass */
            long long dc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dc, _tr_str_lit_len("_tr_rt_str_concat", 17LL), ca));
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
            /* pass */
            List_i64* sca = (void*)List_i64_new();
            /* pass */
            List_i64_append(sca, a);
            /* pass */
            List_i64_append(sca, b);
            /* pass */
            long long cmpv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sca));
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
            if (((at != 11LL) || (bt != 11LL))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
            /* pass */
            long long eatag = _emit_field_get(m, lf, a, 0LL, 0LL);
            /* pass */
            long long ebtag = _emit_field_get(m, lf, b, 0LL, 0LL);
            /* pass */
            long long ecd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(ecd, op, eatag, ebtag));
            /* pass */
            LFunc_set_vreg_type(lf, ecd, 4LL);
            /* pass */
            _tr_str_release(ddn);
            return ecd;
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
            if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
                /* pass */
                _tr_str_release(ddn);
                return (-1LL);
            }
        }
        /* pass */
        if ((_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
            /* pass */
            long long na = _norm_bool(lf, a);
            /* pass */
            long long nb = _norm_bool(lf, b);
            /* pass */
            if (_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) {
                /* pass */
                long long dand = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(dand, _tr_str_lit_len("*", 1LL), na, nb));
                /* pass */
                LFunc_set_vreg_type(lf, dand, 4LL);
                /* pass */
                _tr_str_release(ddn);
                return dand;
            }
            /* pass */
            long long ssum = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(ssum, _tr_str_lit_len("+", 1LL), na, nb));
            /* pass */
            long long oro = _norm_bool(lf, ssum);
            /* pass */
            LFunc_set_vreg_type(lf, oro, 4LL);
            /* pass */
            _tr_str_release(ddn);
            return oro;
        }
        /* pass */
        if (((_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL))) && ((at == 0LL) || (at == 4LL))) && ((bt == 0LL) || (bt == 4LL)))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_int_pow", 14LL));
            /* pass */
            List_i64* ipa = (void*)List_i64_new();
            /* pass */
            List_i64_append(ipa, a);
            /* pass */
            List_i64_append(ipa, b);
            /* pass */
            long long ipd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ipd, _tr_str_lit_len("_tr_rt_int_pow", 14LL), ipa));
            /* pass */
            _tr_str_release(ddn);
            return ipd;
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
    } else if (_t3355.tag == HirExpr_ECall) {
        __auto_type callee = _t3355.data.ECall.callee;
__auto_type args = _t3355.data.ECall.args;
        /* pass */
        TrStr alloc_nm = _tr_str_lit_len("", 0LL);
        /* pass */
        __auto_type _t3360 = (*callee);
        if (_t3360.tag == HirExpr_EIdent) {
            __auto_type anm = _t3360.data.EIdent.name;
            TrStr _strtmp_t3361 = _tr_str_retain(anm);
            _tr_str_release(alloc_nm);
            alloc_nm = _strtmp_t3361;
        } else if (_t3360.tag == HirExpr_EIndex) {
            __auto_type abase = _t3360.data.EIndex.obj;
            /* pass */
            __auto_type _t3362 = (*abase);
            if (_t3362.tag == HirExpr_EIdent) {
                __auto_type anm2 = _t3362.data.EIdent.name;
                TrStr _strtmp_t3363 = _tr_str_retain(anm2);
                _tr_str_release(alloc_nm);
                alloc_nm = _strtmp_t3363;
            } else if (1) {
                __auto_type _ = _t3362;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3360;
            /* pass */
        }
        /* pass */
        if (((_tr_str_eqv((alloc_nm), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((alloc_nm), (_tr_str_lit_len("core_alloc_alloc", 16LL)))) && (args->len == 1LL))) {
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
            LFunc_emit(lf, LInst_ctor_IBinOp(anb, _tr_str_lit_len("*", 1LL), acnt, astr_c));
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
            /* pass */
            List_i64* aargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(aargs, anb);
            /* pass */
            long long ad = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ad, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aargs));
            /* pass */
            LFunc_set_vreg_type(lf, ad, 0LL);
            /* pass */
            _tr_str_release(alloc_nm);
            return ad;
        }
        /* pass */
        if (((_tr_str_eqv((alloc_nm), (_tr_str_lit_len("dealloc", 7LL))) || _tr_str_eqv((alloc_nm), (_tr_str_lit_len("core_alloc_dealloc", 18LL)))) && (args->len == 1LL))) {
            /* pass */
            long long dpv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((dpv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_free", 15LL));
            /* pass */
            List_i64* dfa = (void*)List_i64_new();
            /* pass */
            List_i64_append(dfa, dpv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_raw_free", 15LL), dfa));
            /* pass */
            _tr_str_release(alloc_nm);
            return dpv;
        }
        /* pass */
        __auto_type _t3364 = (*callee);
        if (_t3364.tag == HirExpr_EPropAccess) {
            __auto_type pfo = _t3364.data.EPropAccess.obj;
__auto_type pfp = _t3364.data.EPropAccess.prop;
            /* pass */
            TrStr pfcls = _recv_class(m, lf, pfo);
            /* pass */
            if (((!_tr_str_eqv((pfcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, pfcls))) {
                /* pass */
                long long pfoff = LModule_field_offset(m, pfcls, pfp);
                /* pass */
                if (((pfoff >= 0LL) && (LModule_field_tag(m, pfcls, pfp) == 0LL))) {
                    /* pass */
                    long long pfobj = lower_expr(m, lf, pfo);
                    /* pass */
                    if ((pfobj < 0LL)) {
                        /* pass */
                        _tr_str_release(pfcls);
                        _tr_str_release(alloc_nm);
                        return (-1LL);
                    }
                    /* pass */
                    long long pfrt = _tag_of(m, hir_expr_type(e));
                    /* pass */
                    if ((pfrt < 0LL)) {
                        /* pass */
                        _tr_str_release(pfcls);
                        _tr_str_release(alloc_nm);
                        return (-1LL);
                    }
                    /* pass */
                    List_i64* pfargs = (void*)List_i64_new();
                    /* pass */
                    long long pfai = 0LL;
                    /* pass */
                    while ((pfai < args->len)) {
                        /* pass */
                        long long pfav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, pfai)));
                        /* pass */
                        if ((pfav < 0LL)) {
                            /* pass */
                            _tr_str_release(pfcls);
                            List_i64_free(pfargs);
                            _tr_str_release(alloc_nm);
                            return (-1LL);
                        }
                        /* pass */
                        List_i64_append(pfargs, pfav);
                        /* pass */
                        pfai = (pfai + 1LL);
                    }
                    /* pass */
                    long long pffn = _emit_field_get(m, lf, pfobj, pfoff, 0LL);
                    /* pass */
                    long long pfd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICallInd(pfd, pffn, pfargs));
                    /* pass */
                    LFunc_set_vreg_type(lf, pfd, pfrt);
                    /* pass */
                    if ((pfrt == 1LL)) {
                        /* pass */
                        _fresh_mark(lf, pfd);
                    }
                    /* pass */
                    _tr_str_release(pfcls);
                    _tr_str_release(alloc_nm);
                    return pfd;
                }
            }
            _tr_str_release(pfcls);
        } else if (1) {
            __auto_type _ = _t3364;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if (_tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("Pointer", 7LL))) && (args->len == 1LL))) {
            /* pass */
            long long ptv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((ptv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, ptv) != 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LFunc_set_vreg_type(lf, ptv, 0LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return ptv;
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("await_all", 9LL))) && (args->len > 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_tg_begin", 12LL));
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_begin", 12LL), (void*)List_i64_new()));
            /* pass */
            lf->in_taskgroup = (lf->in_taskgroup + 1LL);
            /* pass */
            long long awi = 0LL;
            /* pass */
            while ((awi < args->len)) {
                /* pass */
                if ((!_lower_spawn(m, lf, ((HirExpr*)List_ptr_get(args, awi))))) {
                    /* pass */
                    lf->in_taskgroup = (lf->in_taskgroup - 1LL);
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
                /* pass */
                awi = (awi + 1LL);
            }
            /* pass */
            lf->in_taskgroup = (lf->in_taskgroup - 1LL);
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_taskgroup_wait", 18LL));
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_taskgroup_wait", 18LL), (void*)List_i64_new()));
            /* pass */
            long long awz = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(awz, 0LL));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return awz;
        }
        /* pass */
        AstType* ecty = hir_expr_type(e);
        /* pass */
        if (LModule_is_enum(m, ecty->name)) {
            /* pass */
            TrStr evn = _norm_variant(ecty->name, fn);
            /* pass */
            if ((LModule_enum_variant_index(m, ecty->name, evn) >= 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _lower_enum_ctor(m, lf, ecty->name, evn, args);
            }
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
        if ((((LFunc_var_index(lf, fn) >= 0LL) && (LFunc_var_type(lf, fn) == 0LL)) && (!LModule_is_user_fn(m, fn)))) {
            /* pass */
            long long frt = _tag_of(m, hir_expr_type(e));
            /* pass */
            if ((frt < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            List_i64* fargs = (void*)List_i64_new();
            /* pass */
            long long fai = 0LL;
            /* pass */
            while ((fai < args->len)) {
                /* pass */
                long long fav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, fai)));
                /* pass */
                if ((fav < 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(fargs);
                    return (-1LL);
                }
                /* pass */
                List_i64_append(fargs, fav);
                /* pass */
                fai = (fai + 1LL);
            }
            /* pass */
            long long ffp = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadVar(ffp, fn));
            /* pass */
            long long ffd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICallInd(ffd, ffp, fargs));
            /* pass */
            LFunc_set_vreg_type(lf, ffd, frt);
            /* pass */
            if ((frt == 1LL)) {
                /* pass */
                _fresh_mark(lf, ffd);
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return ffd;
        }
        /* pass */
        if ((LFunc_var_index(lf, fn) >= 0LL)) {
            /* pass */
            TrStr callcls = _recv_class(m, lf, callee);
            /* pass */
            if (((!_tr_str_eqv((callcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, callcls))) {
                /* pass */
                TrStr callm = LModule_resolve_method(m, callcls, _tr_str_lit_len("__call__", 8LL));
                /* pass */
                if ((!_tr_str_eqv((callm), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    long long callself = lower_expr(m, lf, callee);
                    /* pass */
                    if ((callself < 0LL)) {
                        /* pass */
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(callcls);
                        _tr_str_release(callm);
                        return (-1LL);
                    }
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(callcls);
                    return _lower_obj_call(m, lf, callm, callself, args);
                }
            }
        }
        /* pass */
        if (((args->len == 0LL) && (_prog_generic_class_index(m, fn) >= 0LL))) {
            /* pass */
            AstType* gct0 = hir_expr_type(e);
            /* pass */
            AstType* gct = ({ TrStr _at_t3365 = (_own(fn)); __auto_type _wr = (AstType_init(_at_t3365)); _tr_str_release(_at_t3365); _wr; });
            /* pass */
            if ((gct0->args->len > 0LL)) {
                /* pass */
                long long gaa = 0LL;
                /* pass */
                while ((gaa < gct0->args->len)) {
                    /* pass */
                    List_ptr_append(gct->args, box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(gct0->args, gaa))))));
                    /* pass */
                    gaa = (gaa + 1LL);
                }
            } else {
                /* pass */
                HirClass* gcc2 = ((HirClass*)List_ptr_get(m->hir_prog->classes, _prog_generic_class_index(m, fn)));
                /* pass */
                if (((gcc2->generics->len == 0LL) || (m->subst_names->len == 0LL))) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
                /* pass */
                long long gpi = 0LL;
                /* pass */
                while ((gpi < gcc2->generics->len)) {
                    /* pass */
                    ({ TrStr _at_t3366 = (List_TrStr_get(gcc2->generics, gpi)); TrStr _at_t3367 = (_own(_at_t3366)); List_ptr_append(gct->args, box_asttype_lir(_subst_ty(m, AstType_init(_at_t3367)))); _tr_str_release(_at_t3366); _tr_str_release(_at_t3367); });
                    /* pass */
                    gpi = (gpi + 1LL);
                }
            }
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
            if (_tr_str_eqv((gcm), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_own(fn)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("generic ctor ", 13LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_own((*((AstType**)List_ptr_get(gct->args, 0LL)))->name)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("] not instantiable", 18LL))); _tr_str_release(_cl); _cres; });
                }
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
            /* pass */
            List_i64* goa = (void*)List_i64_new();
            /* pass */
            List_i64_append(goa, gsz);
            /* pass */
            long long god = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(god, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), goa));
            /* pass */
            LFunc_set_vreg_type(lf, god, 10LL);
            /* pass */
            _attach_class_drop(m, lf, god, gcm);
            /* pass */
            _fresh_mark_obj(lf, god);
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
            /* pass */
            List_i64* oaa = (void*)List_i64_new();
            /* pass */
            List_i64_append(oaa, szc);
            /* pass */
            long long od = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), oaa));
            /* pass */
            LFunc_set_vreg_type(lf, od, 10LL);
            /* pass */
            _attach_class_drop(m, lf, od, fn);
            /* pass */
            _fresh_mark_obj(lf, od);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return od;
        }
        /* pass */
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))) && (args->len == 1LL)) && (!_tr_str_eqv((_recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)))), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            TrStr lncls = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if (LModule_is_class(m, lncls)) {
                /* pass */
                TrStr lnm = LModule_resolve_method(m, lncls, _tr_str_lit_len("__len__", 7LL));
                /* pass */
                if ((!_tr_str_eqv((lnm), (_tr_str_lit_len("", 0LL))))) {
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))) && (args->len == 1LL))) {
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
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
                /* pass */
                List_i64* lla = (void*)List_i64_new();
                /* pass */
                List_i64_append(lla, xv);
                /* pass */
                long long lld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(lld, _tr_str_lit_len("_tr_rt_list_len", 15LL), lla));
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return lld;
            }
            /* pass */
            if ((xt == 1LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
                /* pass */
                List_i64* sla = (void*)List_i64_new();
                /* pass */
                List_i64_append(sla, xv);
                /* pass */
                long long sld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_strlen", 13LL), sla));
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
        if ((((_tr_str_eqv((fn), (_tr_str_lit_len("hex", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("oct", 3LL)))) || _tr_str_eqv((fn), (_tr_str_lit_len("bin", 3LL)))) && (args->len == 1LL))) {
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
            TrStr hxsym = _tr_str_lit_len("_tr_rt_hex_str", 14LL);
            /* pass */
            if (_tr_str_eqv((fn), (_tr_str_lit_len("oct", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3368 = _tr_str_lit_len("_tr_rt_oct_str", 14LL);
                _tr_str_release(hxsym);
                hxsym = _strtmp_t3368;
            } else if (_tr_str_eqv((fn), (_tr_str_lit_len("bin", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3369 = _tr_str_lit_len("_tr_rt_bin_str", 14LL);
                _tr_str_release(hxsym);
                hxsym = _strtmp_t3369;
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("round", 5LL))) && (args->len == 1LL))) {
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_round", 12LL));
            /* pass */
            long long rrd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IFCallF(rrd, _tr_str_lit_len("_tr_rt_round", 12LL), rv0));
            /* pass */
            LFunc_set_vreg_type(lf, rrd, 5LL);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return rrd;
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("abs", 3LL))) && (args->len == 1LL))) {
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
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_fabs", 11LL));
                /* pass */
                long long fabd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCallF(fabd, _tr_str_lit_len("_tr_rt_fabs", 11LL), xv2));
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_abs_i64", 14LL));
            /* pass */
            List_i64* aba = (void*)List_i64_new();
            /* pass */
            List_i64_append(aba, xv2);
            /* pass */
            long long abd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(abd, _tr_str_lit_len("_tr_rt_abs_i64", 14LL), aba));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return abd;
        }
        /* pass */
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) && (args->len == 2LL))) {
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
                TrStr fmsym = _tr_str_lit_len("_tr_rt_min_f64", 14LL);
                /* pass */
                if (_tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) {
                    /* pass */
                    TrStr _strtmp_t3370 = _tr_str_lit_len("_tr_rt_max_f64", 14LL);
                    _tr_str_release(fmsym);
                    fmsym = _strtmp_t3370;
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
            TrStr msym = _tr_str_lit_len("_tr_rt_min_i64", 14LL);
            /* pass */
            if (_tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3371 = _tr_str_lit_len("_tr_rt_max_i64", 14LL);
                _tr_str_release(msym);
                msym = _strtmp_t3371;
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("repr", 4LL))) && (args->len == 1LL))) {
            /* pass */
            TrStr rprc = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if (((!_tr_str_eqv((rprc), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, rprc))) {
                /* pass */
                long long rprr = _obj_repr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), (-1LL));
                /* pass */
                if ((rprr >= 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(rprc);
                    return rprr;
                }
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return (-1LL);
            }
            /* pass */
            long long rpv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((rpv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, rpv) == 1LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return rpv;
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(rprc);
            return _reg_to_str(m, lf, rpv);
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("str", 3LL))) && (args->len == 1LL))) {
            /* pass */
            TrStr sobjc = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((((!_tr_str_eqv((sobjc), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, sobjc)) && (!_tr_str_eqv((LModule_resolve_method(m, sobjc, _tr_str_lit_len("__str__", 7LL))), (_tr_str_lit_len("", 0LL)))))) {
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
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
                /* pass */
                long long fsd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFCall1(fsd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), cv0));
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
            TrStr ssym = _tr_str_lit_len("_tr_rt_i64_to_str", 17LL);
            /* pass */
            if ((cvt == 4LL)) {
                /* pass */
                TrStr _strtmp_t3372 = _tr_str_lit_len("_tr_rt_bool_to_str", 18LL);
                _tr_str_release(ssym);
                ssym = _strtmp_t3372;
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("float", 5LL))) && (args->len == 1LL))) {
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
            if ((fvt == 1LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL));
                /* pass */
                List_i64* fsa = (void*)List_i64_new();
                /* pass */
                List_i64_append(fsa, fv0);
                /* pass */
                long long fsr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(fsr, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL), fsa));
                /* pass */
                long long fsf = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(fsf, fsr));
                /* pass */
                LFunc_set_vreg_type(lf, fsf, 5LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return fsf;
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("_tr_fn_int", 10LL)))) && (args->len == 1LL))) {
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL));
            /* pass */
            List_i64* i2a = (void*)List_i64_new();
            /* pass */
            List_i64_append(i2a, iv0);
            /* pass */
            long long i2d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(i2d, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), i2a));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return i2d;
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("sum", 3LL))) && (args->len == 1LL))) {
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
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL));
            /* pass */
            List_i64* sua = (void*)List_i64_new();
            /* pass */
            List_i64_append(sua, suv);
            /* pass */
            long long sud = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(sud, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL), sua));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return sud;
        }
        /* pass */
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("any", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("all", 3LL)))) && (args->len == 1LL))) {
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
            TrStr ansym = _tr_str_lit_len("_tr_rt_list_any_i64", 19LL);
            /* pass */
            if (_tr_str_eqv((fn), (_tr_str_lit_len("all", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3373 = _tr_str_lit_len("_tr_rt_list_all_i64", 19LL);
                _tr_str_release(ansym);
                ansym = _strtmp_t3373;
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("chr", 3LL))) && (args->len == 1LL))) {
            /* pass */
            long long chv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((chv < 0LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long cht = LFunc_vreg_type(lf, chv);
            /* pass */
            if (((cht != 0LL) && (cht != 4LL))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_chr", 10LL));
            /* pass */
            List_i64* cha = (void*)List_i64_new();
            /* pass */
            List_i64_append(cha, chv);
            /* pass */
            long long chd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(chd, _tr_str_lit_len("_tr_rt_chr", 10LL), cha));
            /* pass */
            LFunc_set_vreg_type(lf, chd, 1LL);
            /* pass */
            _fresh_mark(lf, chd);
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return chd;
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("ord", 3LL))) && (args->len == 1LL))) {
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
            long long ordt = LFunc_vreg_type(lf, ordv);
            /* pass */
            if (((ordt == 0LL) || (ordt == 4LL))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return ordv;
            }
            /* pass */
            if ((ordt != 1LL)) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_ord", 14LL));
            /* pass */
            List_i64* orda = (void*)List_i64_new();
            /* pass */
            List_i64_append(orda, ordv);
            /* pass */
            long long ordd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(ordd, _tr_str_lit_len("_tr_rt_str_ord", 14LL), orda));
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return ordd;
        }
        /* pass */
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("bool", 4LL))) && (args->len == 1LL))) {
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
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("_tr_str_lit", 11LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("_tr_strx_join_trstr", 19LL))))) {
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (_tr_str_eqv((fn), (_tr_str_lit_len("_tr_str_wrap", 12LL)))) {
            /* pass */
            TrStr _strtmp_t3374 = _tr_str_lit_len("_tr_rt_str_new", 14LL);
            _tr_str_release(fn);
            fn = _strtmp_t3374;
        }
        /* pass */
        if (_tr_str_eqv((fn), (_tr_str_lit_len("_tr_getenv", 10LL)))) {
            /* pass */
            TrStr _strtmp_t3375 = _tr_str_lit_len("_tr_rt_getenv", 13LL);
            _tr_str_release(fn);
            fn = _strtmp_t3375;
        }
        /* pass */
        if (LModule_is_extern_fn(m, fn)) {
            /* pass */
            if ((args->len > _arg_limit(m))) {
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
                if (((((ext != 0LL) && (ext != 1LL)) && (ext != 4LL)) && (ext != 5LL))) {
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
            if ((extret == 1LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
                /* pass */
                List_i64* exwr = (void*)List_i64_new();
                /* pass */
                List_i64_append(exwr, exd);
                /* pass */
                long long exwd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(exwd, _tr_str_lit_len("_tr_rt_str_new", 14LL), exwr));
                /* pass */
                LFunc_set_vreg_type(lf, exwd, 1LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return exwd;
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return exd;
        }
        /* pass */
        if ((_is_runtime_fn(fn) && (!LModule_is_user_fn(m, fn)))) {
            /* pass */
            if ((args->len > _arg_limit(m))) {
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            List_i64* rtargs = (void*)List_i64_new();
            /* pass */
            long long rti = 0LL;
            /* pass */
            while ((rti < args->len)) {
                /* pass */
                long long rtv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, rti)));
                /* pass */
                if ((rtv < 0LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(rtargs);
                    return (-1LL);
                }
                /* pass */
                if ((LFunc_vreg_type(lf, rtv) == 5LL)) {
                    /* pass */
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(rtargs);
                    return (-1LL);
                }
                /* pass */
                List_i64_append(rtargs, rtv);
                /* pass */
                rti = (rti + 1LL);
            }
            /* pass */
            LModule_add_extern(m, fn);
            /* pass */
            long long rtret = _tag_of(m, hir_expr_type(e));
            /* pass */
            if ((rtret < 0LL)) {
                /* pass */
                rtret = 0LL;
            }
            /* pass */
            long long rtd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(rtd, fn, rtargs));
            /* pass */
            LFunc_set_vreg_type(lf, rtd, rtret);
            /* pass */
            if (((rtret == 1LL) && (!_is_rc_str_runtime_fn(fn)))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
                /* pass */
                List_i64* rtwr = (void*)List_i64_new();
                /* pass */
                List_i64_append(rtwr, rtd);
                /* pass */
                long long rtwd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(rtwd, _tr_str_lit_len("_tr_rt_str_new", 14LL), rtwr));
                /* pass */
                LFunc_set_vreg_type(lf, rtwd, 1LL);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return rtwd;
            }
            /* pass */
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return rtd;
        }
        /* pass */
        if ((!LModule_is_user_fn(m, fn))) {
            /* pass */
            TrStr mbase = _mono_base(fn);
            /* pass */
            if ((!_tr_str_eqv((mbase), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                long long mgfi = _find_generic_fn(m, mbase);
                /* pass */
                if ((mgfi >= 0LL)) {
                    /* pass */
                    AstType* mct = ({ TrStr _at_t3376 = (_mono_concrete(fn)); __auto_type _wr = (AstType_init(_at_t3376)); _tr_str_release(_at_t3376); _wr; });
                    /* pass */
                    if ((!_lir_lower_mono_fn(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, mgfi)), fn, mct))) {
                        /* pass */
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(mbase);
                        return (-1LL);
                    }
                }
            }
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
            if ((args->len > _arg_limit(m))) {
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
            TrStr gmangled = _tr_strx_concatv((fn), (_tr_str_lit_len("__m", 3LL)));
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
                TrStr gcn = _tr_str_lit_len("", 0LL);
                /* pass */
                if (((gat == 10LL) || (gat == 11LL))) {
                    /* pass */
                    TrStr _strtmp_t3377 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, gi)));
                    _tr_str_release(gcn);
                    gcn = _strtmp_t3377;
                    /* pass */
                    if (_tr_str_eqv((gcn), (_tr_str_lit_len("", 0LL)))) {
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
                    TrStr _strtmp_t3378 = _tr_strx_concatv((gmangled), (_tr_str_lit_len("_", 1LL)));
                    _tr_str_release(gmangled);
                    gmangled = _strtmp_t3378;
                }
                /* pass */
                TrStr _strtmp_t3379 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gat)))); TrStr _cres = _tr_strx_concatv((gmangled), _cr); _tr_str_release(_cr); _cres; });
                _tr_str_release(gmangled);
                gmangled = _strtmp_t3379;
                /* pass */
                if ((!_tr_str_eqv((gcn), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t3380 = _tr_strx_concatv((gmangled), (gcn));
                    _tr_str_release(gmangled);
                    gmangled = _strtmp_t3380;
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
        if ((args->len > _arg_limit(m))) {
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
            if ((_is_runtime_fn(fn) && (!_is_rc_str_runtime_fn(fn)))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
                /* pass */
                List_i64* d4wr = (void*)List_i64_new();
                /* pass */
                List_i64_append(d4wr, d4);
                /* pass */
                long long d4wd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(d4wd, _tr_str_lit_len("_tr_rt_str_new", 14LL), d4wr));
                /* pass */
                LFunc_set_vreg_type(lf, d4wd, 1LL);
                /* pass */
                _fresh_mark(lf, d4wd);
                /* pass */
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return d4wd;
            }
            /* pass */
            _fresh_mark(lf, d4);
        }
        /* pass */
        _tr_str_release(alloc_nm);
        _tr_str_release(fn);
        return d4;
    } else if (_t3355.tag == HirExpr_ETuple) {
        __auto_type titems = _t3355.data.ETuple.items;
        /* pass */
        if ((titems->len == 0LL)) {
            /* pass */
            long long zsz = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zsz, 8LL));
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
            /* pass */
            List_i64* zba = (void*)List_i64_new();
            /* pass */
            List_i64_append(zba, zsz);
            /* pass */
            long long zblk = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(zblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), zba));
            /* pass */
            LFunc_set_vreg_type(lf, zblk, 15LL);
            /* pass */
            _fresh_mark_obj(lf, zblk);
            /* pass */
            return zblk;
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
            } else if (((tevt == 10LL) || (tevt == 11LL))) {
                /* pass */
                _secure_obj(m, lf, tev);
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
        /* pass */
        List_i64* tba = (void*)List_i64_new();
        /* pass */
        List_i64_append(tba, tsz);
        /* pass */
        long long tblk = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(tblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), tba));
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
    } else if (_t3355.tag == HirExpr_ESet) {
        __auto_type sitems = _t3355.data.ESet.items;
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
        TrStr snew = _set_sym(setag, _tr_str_lit_len("new", 3LL));
        /* pass */
        LModule_add_extern(m, snew);
        /* pass */
        long long shv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(shv, snew, (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, shv, setag);
        /* pass */
        TrStr sadd = _set_sym(setag, _tr_str_lit_len("set", 3LL));
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
    } else if (_t3355.tag == HirExpr_EList) {
        __auto_type items = _t3355.data.EList.items;
__auto_type lty = _t3355.data.EList.ty;
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
        /* pass */
        long long hv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
        /* pass */
        long long elem_t = 0LL;
        /* pass */
        long long list_tag = 2LL;
        /* pass */
        if ((items->len == 0LL)) {
            /* pass */
            long long ann_tag = _tag_of(m, lty);
            /* pass */
            if ((((((ann_tag == 2LL) || (ann_tag == 3LL)) || (ann_tag == 14LL)) || (ann_tag == 19LL)) || (ann_tag == 22LL))) {
                /* pass */
                list_tag = ann_tag;
                /* pass */
                elem_t = _list_elem_tag(ann_tag);
            } else if ((_tr_str_eqv((lty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lty->name), (_tr_str_lit_len("Vec", 3LL))))) {
                /* pass */
                if (((lty->args->len > 0LL) && _tr_str_eqv(((*((AstType**)List_ptr_get(lty->args, 0LL)))->name), (_tr_str_lit_len("bool", 4LL))))) {
                    /* pass */
                    list_tag = 22LL;
                    /* pass */
                    elem_t = 4LL;
                }
            }
        }
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
            if ((((((evt != 0LL) && (evt != 1LL)) && (evt != 4LL)) && (evt != 5LL)) && (evt != 10LL))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long eff_t = evt;
            /* pass */
            if ((li == 0LL)) {
                /* pass */
                elem_t = eff_t;
                /* pass */
                list_tag = _list_tag_for_elem(elem_t);
            } else if ((eff_t != elem_t)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((evt == 1LL)) {
                /* pass */
                _secure_str(m, lf, ev);
            }
            /* pass */
            if ((((evt == 10LL) || (evt == 11LL)) || (evt == 15LL))) {
                /* pass */
                _secure_obj(m, lf, ev);
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
            TrStr push_sym = _tr_str_lit_len("_tr_rt_list_push_i64", 20LL);
            /* pass */
            if ((list_tag == 22LL)) {
                /* pass */
                TrStr _strtmp_t3381 = _tr_str_lit_len("_tr_rt_blist_push", 17LL);
                _tr_str_release(push_sym);
                push_sym = _strtmp_t3381;
            }
            /* pass */
            LModule_add_extern(m, push_sym);
            /* pass */
            List_i64* pa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pa, hv);
            /* pass */
            List_i64_append(pa, ev);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), push_sym, pa));
            /* pass */
            li = (li + 1LL);
            _tr_str_release(push_sym);
        }
        /* pass */
        LFunc_set_vreg_type(lf, hv, list_tag);
        /* pass */
        return hv;
    } else if (_t3355.tag == HirExpr_EDict) {
        __auto_type keys = _t3355.data.EDict.keys;
__auto_type vals = _t3355.data.EDict.vals;
__auto_type dty = _t3355.data.EDict.ty;
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
        } else if (((_tr_str_eqv((dty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((dty->name), (_tr_str_lit_len("Map", 3LL)))) && (dty->args->len == 0LL))) {
            /* pass */
            dtag = 8LL;
        }
        /* pass */
        if ((!_is_dict_tag(dtag))) {
            /* pass */
            TrStr kn = _tr_str_lit_len("?", 1LL);
            /* pass */
            TrStr vn = _tr_str_lit_len("?", 1LL);
            /* pass */
            if ((dty->args->len > 0LL)) {
                /* pass */
                TrStr _strtmp_t3382 = _tr_str_retain((*((AstType**)List_ptr_get(dty->args, 0LL)))->name);
                _tr_str_release(kn);
                kn = _strtmp_t3382;
            }
            /* pass */
            if ((dty->args->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t3383 = _tr_str_retain((*((AstType**)List_ptr_get(dty->args, 1LL)))->name);
                _tr_str_release(vn);
                vn = _strtmp_t3383;
            }
            /* pass */
            m->fail_note = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(dtag)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("bad dict tag dtag=", 18LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ty=", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (dty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" k=", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (kn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" v=", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vn)); _tr_str_release(_cl); _cres; });
            /* pass */
            _tr_str_release(kn);
            _tr_str_release(vn);
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
            TrStr dset = _dict_sym(dtag, _tr_str_lit_len("set", 3LL));
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
    } else if (_t3355.tag == HirExpr_EFString) {
        __auto_type parts = _t3355.data.EFString.parts;
        /* pass */
        long long acc = _heap_lit(m, lf, _tr_str_lit_len("", 0LL));
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
                if ((!_tr_str_eqv((fp->fmt_spec), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    long long spev = lower_expr(m, lf, fp->expr);
                    /* pass */
                    if ((spev < 0LL)) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long spidx = LModule_add_string(m, fp->fmt_spec, _tr_str_lenv((fp->fmt_spec)));
                    /* pass */
                    long long specv = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IStr(specv, spidx));
                    /* pass */
                    long long spvt = LFunc_vreg_type(lf, spev);
                    /* pass */
                    if ((spvt == 5LL)) {
                        /* pass */
                        long long spbits = LFunc_new_vreg(lf);
                        /* pass */
                        LFunc_emit(lf, LInst_ctor_IFBits(spbits, spev));
                        /* pass */
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_f64", 19LL), spbits, specv, 1LL);
                    } else if ((spvt == 1LL)) {
                        /* pass */
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_str", 19LL), spev, specv, 1LL);
                    } else if (((spvt == 0LL) || (spvt == 4LL))) {
                        /* pass */
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_i64", 19LL), spev, specv, 1LL);
                    } else {
                        /* pass */
                        return (-1LL);
                    }
                } else {
                    /* pass */
                    TrStr fcls = _recv_class(m, lf, fp->expr);
                    /* pass */
                    if ((((!_tr_str_eqv((fcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, fcls)) && (!_tr_str_eqv((LModule_resolve_method(m, fcls, _tr_str_lit_len("__str__", 7LL))), (_tr_str_lit_len("", 0LL)))))) {
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
                        if ((_tr_str_eqv((hir_expr_type(fp->expr)->name), (_tr_str_lit_len("char", 4LL))) && (LFunc_vreg_type(lf, fev) == 0LL))) {
                            /* pass */
                            pr = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_char_to_str", 18LL), fev, 1LL);
                        } else {
                            /* pass */
                            pr = _reg_to_str(m, lf, fev);
                        }
                        /* pass */
                        if ((pr < 0LL)) {
                            /* pass */
                            _tr_str_release(fcls);
                            return (-1LL);
                        }
                    }
                }
            } else {
                /* pass */
                pr = _heap_lit_len(m, lf, fp->text, fp->text_len);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
            /* pass */
            List_i64* fca = (void*)List_i64_new();
            /* pass */
            List_i64_append(fca, acc);
            /* pass */
            List_i64_append(fca, pr);
            /* pass */
            long long fdc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(fdc, _tr_str_lit_len("_tr_rt_str_concat", 17LL), fca));
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
    } else if (_t3355.tag == HirExpr_EClosure) {
        __auto_type cparams = _t3355.data.EClosure.params;
__auto_type cret = _t3355.data.EClosure.ret_ty;
__auto_type cbody = _t3355.data.EClosure.body;
__auto_type cis_async = _t3355.data.EClosure.is_async;
__auto_type ccaps = _t3355.data.EClosure.captures;
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
        TrStr cname = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("_tr_clo_", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lf->name)); _tr_str_release(_cl); _cres; });
        /* pass */
        LFunc* clf = ({ TrStr _at_t3384 = (_own(cname)); __auto_type _wr = (LFunc_init(_at_t3384)); _tr_str_release(_at_t3384); _wr; });
        /* pass */
        List_TrStr_append(clf->params, _tr_str_lit_len("__env", 5LL));
        /* pass */
        LFunc_add_var(clf, _tr_str_lit_len("__env", 5LL));
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
                ({ TrStr _at_t3385 = (_own(cp->ty->name)); LFunc_set_var_cls(clf, cp->name, _at_t3385); _tr_str_release(_at_t3385); });
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
            ({ TrStr _at_t3386 = (_own(cc->name)); List_TrStr_append(clf->captures, _at_t3386); _tr_str_release(_at_t3386); });
            /* pass */
            List_i64_append(clf->cap_tags, cctag);
            /* pass */
            ccj = (ccj + 1LL);
        }
        /* pass */
        ({ TrStr _at_t3387 = (_own(cname)); List_TrStr_append(m->fn_names, _at_t3387); _tr_str_release(_at_t3387); });
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
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
        /* pass */
        List_i64* cba = (void*)List_i64_new();
        /* pass */
        List_i64_append(cba, cblk_sz);
        /* pass */
        long long cblk = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(cblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), cba));
        /* pass */
        LFunc_set_vreg_type(lf, cblk, 12LL);
        /* pass */
        long long cfa = LFunc_new_vreg(lf);
        /* pass */
        ({ TrStr _at_t3388 = (_own(cname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(cfa, _at_t3388)); _tr_str_release(_at_t3388); });
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
    } else if (_t3355.tag == HirExpr_EIndex) {
        __auto_type obj = _t3355.data.EIndex.obj;
__auto_type idx = _t3355.data.EIndex._tr_v_index;
        /* pass */
        TrStr gicls = _recv_class(m, lf, obj);
        /* pass */
        if (((!_tr_str_eqv((gicls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, gicls))) {
            /* pass */
            TrStr gim = LModule_resolve_method(m, gicls, _tr_str_lit_len("__getitem__", 11LL));
            /* pass */
            if ((!_tr_str_eqv((gim), (_tr_str_lit_len("", 0LL))))) {
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
        TrStr arn = _ident_name(obj);
        /* pass */
        if (((!_tr_str_eqv((arn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, arn) > 0LL))) {
            /* pass */
            long long ariv = lower_expr(m, lf, idx);
            /* pass */
            if (((ariv < 0LL) || (LFunc_vreg_type(lf, ariv) != 0LL))) {
                /* pass */
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
            /* pass */
            long long araddr = _array_elem_addr(m, lf, arn, ariv);
            /* pass */
            long long ard = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoad(ard, araddr, 0LL, 2LL));
            /* pass */
            LFunc_set_vreg_type(lf, ard, _fixed_arr_elem_tag(m, lf, arn));
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return ard;
        }
        /* pass */
        long long ov = lower_expr(m, lf, obj);
        /* pass */
        if ((ov < 0LL)) {
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
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
                _tr_str_release(arn);
                return (-1LL);
            }
            /* pass */
            if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, dkv) != 1LL))) {
                /* pass */
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
            /* pass */
            if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, dkv) != 0LL))) {
                /* pass */
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
            /* pass */
            TrStr dget = _dict_sym(ovt, _tr_str_lit_len("get", 3LL));
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
                _tr_str_release(arn);
                _tr_str_release(dget);
                return dgf;
            }
            /* pass */
            LFunc_set_vreg_type(lf, dgd, _dict_val_tag(ovt));
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
            _tr_str_release(dget);
            return dgd;
        }
        /* pass */
        if ((ovt == 1LL)) {
            /* pass */
            long long siv = lower_expr(m, lf, idx);
            /* pass */
            if ((siv < 0LL)) {
                /* pass */
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_char_at", 18LL), ov, siv, 0LL);
        }
        /* pass */
        if ((!_is_list_tag(ovt))) {
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return (-1LL);
        }
        /* pass */
        long long iv = lower_expr(m, lf, idx);
        /* pass */
        if ((iv < 0LL)) {
            /* pass */
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return (-1LL);
        }
        /* pass */
        _tr_str_release(gicls);
        _tr_str_release(arn);
        return _list_get_elem(m, lf, ovt, ov, iv);
    } else if (_t3355.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t3355.data.EPropAccess.obj;
__auto_type prop = _t3355.data.EPropAccess.prop;
        /* pass */
        __auto_type _t3389 = (*obj);
        if (_t3389.tag == HirExpr_EIdent) {
            __auto_type uvn = _t3389.data.EIdent.name;
            /* pass */
            if (({ TrStr _at_t3390 = (_norm_variant(uvn, prop)); __auto_type _wr = ((LModule_is_enum(m, uvn) && (LModule_enum_variant_index(m, uvn, _at_t3390) >= 0LL))); _tr_str_release(_at_t3390); _wr; })) {
                /* pass */
                return ({ TrStr _at_t3391 = (_norm_variant(uvn, prop)); __auto_type _wr = (_lower_enum_ctor(m, lf, uvn, _at_t3391, (void*)List_ptr_new())); _tr_str_release(_at_t3391); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t3389;
            /* pass */
            /* pass */
        }
        /* pass */
        if (_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL)))) {
            /* pass */
            TrStr aln = _ident_name(obj);
            /* pass */
            if (((!_tr_str_eqv((aln), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, aln) > 0LL))) {
                /* pass */
                long long ald = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(ald, _fixed_arr_len(m, lf, aln)));
                /* pass */
                _tr_str_release(aln);
                return ald;
            }
        }
        /* pass */
        TrStr pcls = _recv_class(m, lf, obj);
        /* pass */
        if (((!_tr_str_eqv((pcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, pcls))) {
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
            if ((ftg < 0LL)) {
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
        if (((!_tr_str_eqv((pcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_enum(m, pcls))) {
            /* pass */
            long long ep = _lower_enum_prop(m, lf, obj, pcls, prop);
            /* pass */
            if ((ep >= 0LL)) {
                /* pass */
                _tr_str_release(pcls);
                return ep;
            }
        }
        /* pass */
        if ((!_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))))) {
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
            TrStr pslsym = _set_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit_len("len", 3LL));
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
            TrStr dlsym = _dict_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit_len("len", 3LL));
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
        if ((LFunc_vreg_type(lf, ovl) == 1LL)) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
            /* pass */
            List_i64* sla = (void*)List_i64_new();
            /* pass */
            List_i64_append(sla, ovl);
            /* pass */
            long long sld = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_strlen", 13LL), sla));
            /* pass */
            _tr_str_release(pcls);
            return sld;
        }
        /* pass */
        if ((!_is_list_tag(LFunc_vreg_type(lf, ovl)))) {
            /* pass */
            _tr_str_release(pcls);
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, ovl);
        /* pass */
        long long ld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
        /* pass */
        _tr_str_release(pcls);
        return ld;
    } else if (_t3355.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t3355.data.EMethodCall.obj;
__auto_type method = _t3355.data.EMethodCall.method;
__auto_type margs = _t3355.data.EMethodCall.args;
        /* pass */
        __auto_type _t3392 = (*obj);
        if (_t3392.tag == HirExpr_EIdent) {
            __auto_type styn = _t3392.data.EIdent.name;
            /* pass */
            if (_tr_str_eqv((styn), (_tr_str_lit_len("Coro", 4LL)))) {
                /* pass */
                if (_tr_str_eqv((method), (_tr_str_lit_len("yield_now", 9LL)))) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_yield_h", 14LL));
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_yield_h", 14LL), (void*)List_i64_new()));
                    /* pass */
                    long long cyd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IConst(cyd, 0LL));
                    /* pass */
                    return cyd;
                }
                /* pass */
                if (((_tr_str_eqv((method), (_tr_str_lit_len("sleep_ms", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL)))) && (margs->len >= 1LL))) {
                    /* pass */
                    long long msv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if ((msv < 0LL)) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_sleep_h", 14LL));
                    /* pass */
                    List_i64* sma = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(sma, msv);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_sleep_h", 14LL), sma));
                    /* pass */
                    long long csd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IConst(csd, 0LL));
                    /* pass */
                    return csd;
                }
            }
            /* pass */
            if (_tr_str_eqv((styn), (_tr_str_lit_len("Thread", 6LL)))) {
                /* pass */
                if (((_tr_str_eqv((method), (_tr_str_lit_len("current_id", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("id", 2LL)))) && (margs->len == 0LL))) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_thread_current_id", 21LL));
                    /* pass */
                    long long tcd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(tcd, _tr_str_lit_len("_tr_thread_current_id", 21LL), (void*)List_i64_new()));
                    /* pass */
                    LFunc_set_vreg_type(lf, tcd, 0LL);
                    /* pass */
                    return tcd;
                }
                /* pass */
                if ((_tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL))) && (margs->len >= 1LL))) {
                    /* pass */
                    long long tmsv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if ((tmsv < 0LL)) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_sleep_h", 14LL));
                    /* pass */
                    List_i64* tsa = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(tsa, tmsv);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_sleep_h", 14LL), tsa));
                    /* pass */
                    long long tsd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IConst(tsd, 0LL));
                    /* pass */
                    return tsd;
                }
                /* pass */
                if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && (margs->len >= 1LL))) {
                    /* pass */
                    TrStr tsfn = _ident_name(((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if (((_is_null_str(tsfn) || _tr_str_eqv((tsfn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, tsfn)))) {
                        /* pass */
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
                    /* pass */
                    if ((!_ensure_await_wrapper(m, tsfn))) {
                        /* pass */
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
                    /* pass */
                    long long tsbuf = _pack_spawn_args(m, lf, margs, 1LL);
                    /* pass */
                    if ((tsbuf < 0LL)) {
                        /* pass */
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
                    /* pass */
                    long long tsfa = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_set_vreg_type(lf, tsfa, 12LL);
                    /* pass */
                    ({ TrStr _at_t3393 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (tsfn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(tsfa, _at_t3393)); _tr_str_release(_at_t3393); });
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_threadobj_spawn", 19LL));
                    /* pass */
                    List_i64* toa = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(toa, tsfa);
                    /* pass */
                    List_i64_append(toa, tsbuf);
                    /* pass */
                    long long tod = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(tod, _tr_str_lit_len("_tr_threadobj_spawn", 19LL), toa));
                    /* pass */
                    LFunc_set_vreg_type(lf, tod, 0LL);
                    /* pass */
                    _tr_str_release(tsfn);
                    return tod;
                }
            }
            /* pass */
            if (((_tr_str_eqv((styn), (_tr_str_lit_len("Str", 3LL))) || _tr_str_eqv((styn), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((styn), (_tr_str_lit_len("String", 6LL))))) {
                /* pass */
                if ((_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))) && (margs->len == 2LL))) {
                    /* pass */
                    long long sjl = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if (((sjl < 0LL) || (LFunc_vreg_type(lf, sjl) != 3LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long sjs = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
                    /* pass */
                    if (((sjs < 0LL) || (LFunc_vreg_type(lf, sjs) != 1LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_list_join", 16LL), sjl, sjs, 1LL);
                }
                /* pass */
                if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 1LL))) {
                    /* pass */
                    long long sll = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if (((sll < 0LL) || (LFunc_vreg_type(lf, sll) != 1LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_strlen", 13LL), sll, 0LL);
                }
                /* pass */
                if ((_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))) && (margs->len == 3LL))) {
                    /* pass */
                    long long scv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if (((scv < 0LL) || (LFunc_vreg_type(lf, scv) != 1LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long sca = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
                    /* pass */
                    if (((sca < 0LL) || (LFunc_vreg_type(lf, sca) != 0LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long scb = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 2LL)));
                    /* pass */
                    if (((scb < 0LL) || (LFunc_vreg_type(lf, scb) != 0LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
                    /* pass */
                    List_i64* scargs = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(scargs, scv);
                    /* pass */
                    List_i64_append(scargs, sca);
                    /* pass */
                    List_i64_append(scargs, scb);
                    /* pass */
                    long long scd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(scd, _tr_str_lit_len("_tr_rt_str_slice", 16LL), scargs));
                    /* pass */
                    LFunc_set_vreg_type(lf, scd, 1LL);
                    /* pass */
                    _fresh_mark(lf, scd);
                    /* pass */
                    return scd;
                }
                /* pass */
                if (((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("find", 4LL)))) && (margs->len == 2LL))) {
                    /* pass */
                    long long sfv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                    /* pass */
                    if (((sfv < 0LL) || (LFunc_vreg_type(lf, sfv) != 1LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    long long sfb = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
                    /* pass */
                    if (((sfb < 0LL) || (LFunc_vreg_type(lf, sfb) != 1LL))) {
                        /* pass */
                        return (-1LL);
                    }
                    /* pass */
                    return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), sfv, sfb, 0LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t3392;
            /* pass */
            /* pass */
        }
        /* pass */
        long long boxr = _lower_box_method(m, lf, obj, method, margs);
        /* pass */
        if ((boxr != (-2LL))) {
            /* pass */
            return boxr;
        }
        /* pass */
        if ((_tr_str_eqv((hir_expr_type(obj)->name), (_tr_str_lit_len("Chan", 4LL))) && ((((((_tr_str_eqv((method), (_tr_str_lit_len("send", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("close", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))))) {
            /* pass */
            long long cho = lower_expr(m, lf, obj);
            /* pass */
            if ((cho < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("send", 4LL))) && (margs->len == 1LL))) {
                /* pass */
                long long chv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((chv < 0LL) || (LFunc_vreg_type(lf, chv) != 0LL))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_send", 13LL));
                /* pass */
                List_i64* csa = (void*)List_i64_new();
                /* pass */
                List_i64_append(csa, cho);
                /* pass */
                List_i64_append(csa, chv);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_chan_send", 13LL), csa));
                /* pass */
                return cho;
            }
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) && (margs->len == 0LL))) {
                /* pass */
                TrStr crs = _tr_str_lit_len("_tr_chan_recv", 13LL);
                /* pass */
                if (_tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) {
                    /* pass */
                    TrStr _strtmp_t3394 = _tr_str_lit_len("_tr_chan_try_recv_val", 21LL);
                    _tr_str_release(crs);
                    crs = _strtmp_t3394;
                }
                /* pass */
                LModule_add_extern(m, crs);
                /* pass */
                List_i64* cra = (void*)List_i64_new();
                /* pass */
                List_i64_append(cra, cho);
                /* pass */
                long long crd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(crd, crs, cra));
                /* pass */
                LFunc_set_vreg_type(lf, crd, 0LL);
                /* pass */
                _tr_str_release(crs);
                return crd;
            }
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL))) && (margs->len == 1LL))) {
                /* pass */
                long long ctv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((ctv < 0LL) || (LFunc_vreg_type(lf, ctv) != 0LL))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_send", 17LL));
                /* pass */
                List_i64* cta = (void*)List_i64_new();
                /* pass */
                List_i64_append(cta, cho);
                /* pass */
                List_i64_append(cta, ctv);
                /* pass */
                long long ctd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(ctd, _tr_str_lit_len("_tr_chan_try_send", 17LL), cta));
                /* pass */
                LFunc_set_vreg_type(lf, ctd, 4LL);
                /* pass */
                return ctd;
            }
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("close", 5LL))) && (margs->len == 0LL))) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_close", 14LL));
                /* pass */
                List_i64* cca = (void*)List_i64_new();
                /* pass */
                List_i64_append(cca, cho);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_chan_close", 14LL), cca));
                /* pass */
                return cho;
            }
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) && (margs->len == 0LL))) {
                /* pass */
                TrStr cqs = _tr_str_lit_len("_tr_chan_len", 12LL);
                /* pass */
                long long cqtag = 0LL;
                /* pass */
                if (_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL)))) {
                    /* pass */
                    TrStr _strtmp_t3395 = _tr_str_lit_len("_tr_chan_is_closed", 18LL);
                    _tr_str_release(cqs);
                    cqs = _strtmp_t3395;
                    /* pass */
                    cqtag = 4LL;
                }
                /* pass */
                LModule_add_extern(m, cqs);
                /* pass */
                List_i64* cqa = (void*)List_i64_new();
                /* pass */
                List_i64_append(cqa, cho);
                /* pass */
                long long cqd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(cqd, cqs, cqa));
                /* pass */
                LFunc_set_vreg_type(lf, cqd, cqtag);
                /* pass */
                _tr_str_release(cqs);
                return cqd;
            }
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (_tr_str_eqv((hir_expr_type(obj)->name), (_tr_str_lit_len("Pointer", 7LL)))) {
            /* pass */
            AstType* pty = _ptr_chain_ty(m, obj);
            /* pass */
            long long pstride = _ptr_stride(m, pty);
            /* pass */
            if ((pstride == 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            bool pbyte = (pstride == 1LL);
            /* pass */
            bool pword = (pstride == 4LL);
            /* pass */
            long long petag = 0LL;
            /* pass */
            TrStr pvcls = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((pty->args->len > 0LL)) {
                /* pass */
                TrStr petn = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL))))->name);
                /* pass */
                if ((_tr_str_eqv((petn), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((petn), (_tr_str_lit_len("f64", 3LL))))) {
                    /* pass */
                    petag = 5LL;
                } else if ((_tr_str_eqv((petn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((petn), (_tr_str_lit_len("String", 6LL))))) {
                    /* pass */
                    petag = 1LL;
                } else if (_tr_str_eqv((petn), (_tr_str_lit_len("bool", 4LL)))) {
                    /* pass */
                    petag = 4LL;
                } else if (LModule_is_value_class(m, petn)) {
                    /* pass */
                    petag = 10LL;
                    /* pass */
                    TrStr _strtmp_t3396 = _own(petn);
                    _tr_str_release(pvcls);
                    pvcls = _strtmp_t3396;
                } else if (LModule_is_class(m, petn)) {
                    /* pass */
                    petag = 10LL;
                } else if (LModule_is_enum(m, petn)) {
                    /* pass */
                    petag = 11LL;
                }
                /* pass */
                if ((_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))) && (_value_struct_generic_size(m, (*((AstType**)List_ptr_get(pty->args, 0LL)))) > 0LL))) {
                    /* pass */
                    petag = 10LL;
                    /* pass */
                    TrStr _pmg = _ensure_generic_class(m, _subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL)))));
                    /* pass */
                    if ((!_tr_str_eqv((_pmg), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t3397 = _tr_str_retain(_pmg);
                        _tr_str_release(pvcls);
                        pvcls = _strtmp_t3397;
                    } else {
                        /* pass */
                        TrStr _strtmp_t3398 = _own(petn);
                        _tr_str_release(pvcls);
                        pvcls = _strtmp_t3398;
                    }
                }
            }
            /* pass */
            long long pbase = lower_expr(m, lf, obj);
            /* pass */
            if ((pbase < 0LL)) {
                /* pass */
                _tr_str_release(pvcls);
                return (-1LL);
            }
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) && (margs->len == 1LL))) {
                /* pass */
                long long poff = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((poff < 0LL) || (LFunc_vreg_type(lf, poff) != 0LL))) {
                    /* pass */
                    _tr_str_release(pvcls);
                    return (-1LL);
                }
                /* pass */
                long long pstr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(pstr, pstride));
                /* pass */
                long long pscaled = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(pscaled, _tr_str_lit_len("*", 1LL), poff, pstr));
                /* pass */
                long long padr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(padr, _tr_str_lit_len("+", 1LL), pbase, pscaled));
                /* pass */
                LFunc_set_vreg_type(lf, padr, 0LL);
                /* pass */
                _tr_str_release(pvcls);
                return padr;
            }
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("deref", 5LL)))) && (margs->len == 0LL))) {
                /* pass */
                if (pbyte) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_load_u8", 14LL));
                    /* pass */
                    List_i64* lba = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(lba, pbase);
                    /* pass */
                    long long lbd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(lbd, _tr_str_lit_len("_tr_rt_load_u8", 14LL), lba));
                    /* pass */
                    _tr_str_release(pvcls);
                    return lbd;
                }
                /* pass */
                if (pword) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_load_i32", 15LL));
                    /* pass */
                    List_i64* lwa = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(lwa, pbase);
                    /* pass */
                    long long lwd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(lwd, _tr_str_lit_len("_tr_rt_load_i32", 15LL), lwa));
                    /* pass */
                    _tr_str_release(pvcls);
                    return lwd;
                }
                /* pass */
                if ((!_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    long long rcp = _copy_value_struct(m, lf, pbase, pvcls);
                    /* pass */
                    if ((rcp < 0LL)) {
                        /* pass */
                        _tr_str_release(pvcls);
                        return (-1LL);
                    }
                    /* pass */
                    _tr_str_release(pvcls);
                    return rcp;
                }
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
                    _tr_str_release(pvcls);
                    return prf;
                }
                /* pass */
                _tr_str_release(pvcls);
                return _emit_field_get(m, lf, pbase, 0LL, petag);
            }
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL))) && (margs->len == 1LL))) {
                /* pass */
                long long pwv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if ((pwv < 0LL)) {
                    /* pass */
                    _tr_str_release(pvcls);
                    return (-1LL);
                }
                /* pass */
                if ((!_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    long long wsz = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IConst(wsz, pstride));
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_c_memcpy", 12LL));
                    /* pass */
                    List_i64* wca = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(wca, pbase);
                    /* pass */
                    List_i64_append(wca, pwv);
                    /* pass */
                    List_i64_append(wca, wsz);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_c_memcpy", 12LL), wca));
                    /* pass */
                    _tr_str_release(pvcls);
                    return pbase;
                }
                /* pass */
                if (pbyte) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_store_u8", 15LL));
                    /* pass */
                    List_i64* sba = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(sba, pbase);
                    /* pass */
                    List_i64_append(sba, pwv);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_store_u8", 15LL), sba));
                    /* pass */
                    _tr_str_release(pvcls);
                    return pbase;
                }
                /* pass */
                if (pword) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_store_i32", 16LL));
                    /* pass */
                    List_i64* swa = (void*)List_i64_new();
                    /* pass */
                    List_i64_append(swa, pbase);
                    /* pass */
                    List_i64_append(swa, pwv);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_store_i32", 16LL), swa));
                    /* pass */
                    _tr_str_release(pvcls);
                    return pbase;
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
                        _tr_str_release(pvcls);
                        return (-1LL);
                    }
                    /* pass */
                    long long pwb = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_IFBits(pwb, pwv));
                    /* pass */
                    pwv = pwb;
                } else if ((petag == 1LL)) {
                    /* pass */
                    _secure_str(m, lf, pwv);
                } else if (((petag == 10LL) || (petag == 11LL))) {
                    /* pass */
                    _secure_obj(m, lf, pwv);
                }
                /* pass */
                _emit_field_set(m, lf, pbase, 0LL, pwv);
                /* pass */
                _tr_str_release(pvcls);
                return pbase;
            }
            /* pass */
            _tr_str_release(pvcls);
            return (-1LL);
        }
        /* pass */
        __auto_type _t3399 = (*obj);
        if (_t3399.tag == HirExpr_EIdent) {
            __auto_type inm = _t3399.data.EIdent.name;
__auto_type ity_g = _t3399.data.EIdent.ty;
            /* pass */
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((inm), (_tr_str_lit_len("Dict", 4LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))))) {
                /* pass */
                long long mtag = _tag_of(m, ity_g);
                /* pass */
                if ((!_is_dict_tag(mtag))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                TrStr mnewsym = _dict_new_sym(mtag);
                /* pass */
                LModule_add_extern(m, mnewsym);
                /* pass */
                long long mnd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(mnd, mnewsym, (void*)List_i64_new()));
                /* pass */
                LFunc_set_vreg_type(lf, mnd, mtag);
                /* pass */
                _tr_str_release(mnewsym);
                return mnd;
            }
            /* pass */
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Set", 3LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 0LL))) {
                /* pass */
                long long stag = (-1LL);
                /* pass */
                if ((ity_g->args->len > 0LL)) {
                    /* pass */
                    TrStr se = _tr_str_retain((*((AstType**)List_ptr_get(ity_g->args, 0LL)))->name);
                    /* pass */
                    if ((_tr_str_eqv((se), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((se), (_tr_str_lit_len("String", 6LL))))) {
                        /* pass */
                        stag = 16LL;
                    } else if (_is_int_typename(se)) {
                        /* pass */
                        stag = 13LL;
                    }
                }
                /* pass */
                if ((stag < 0LL)) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                TrStr newsym = _set_sym(stag, _tr_str_lit_len("new", 3LL));
                /* pass */
                LModule_add_extern(m, newsym);
                /* pass */
                long long snd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(snd, newsym, (void*)List_i64_new()));
                /* pass */
                LFunc_set_vreg_type(lf, snd, stag);
                /* pass */
                _tr_str_release(newsym);
                return snd;
            }
            /* pass */
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Vec", 3LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (ity_g->args->len > 0LL))) {
                /* pass */
                TrStr ven = _tr_str_retain((*((AstType**)List_ptr_get(ity_g->args, 0LL)))->name);
                /* pass */
                if ((_tr_str_eqv((ven), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((ven), (_tr_str_lit_len("tuple", 5LL))))) {
                    /* pass */
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
                    /* pass */
                    long long vnd = LFunc_new_vreg(lf);
                    /* pass */
                    LFunc_emit(lf, LInst_ctor_ICall(vnd, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
                    /* pass */
                    LFunc_set_vreg_type(lf, vnd, 21LL);
                    /* pass */
                    _tr_str_release(ven);
                    return vnd;
                }
            }
            /* pass */
            if ((_tr_str_eqv((inm), (_tr_str_lit_len("List", 4LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))))) {
                /* pass */
                long long ltag2 = (-1LL);
                /* pass */
                if ((ity_g->args->len > 0LL)) {
                    /* pass */
                    long long letag = _tag_of(m, _subst_ty(m, (*((AstType**)List_ptr_get(ity_g->args, 0LL)))));
                    /* pass */
                    ltag2 = _list_tag_for_elem(letag);
                }
                /* pass */
                if (((ltag2 < 0LL) || (!_is_list_tag(ltag2)))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
                /* pass */
                long long lnd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(lnd, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
                /* pass */
                LFunc_set_vreg_type(lf, lnd, ltag2);
                /* pass */
                return lnd;
            }
            /* pass */
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Chan", 4LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 1LL))) {
                /* pass */
                long long ccap = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((ccap < 0LL) || (LFunc_vreg_type(lf, ccap) != 0LL))) {
                    /* pass */
                    return (-1LL);
                }
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_new", 12LL));
                /* pass */
                List_i64* cna = (void*)List_i64_new();
                /* pass */
                List_i64_append(cna, ccap);
                /* pass */
                long long cnd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(cnd, _tr_str_lit_len("_tr_chan_new", 12LL), cna));
                /* pass */
                LFunc_set_vreg_type(lf, cnd, 0LL);
                /* pass */
                return cnd;
            }
            /* pass */
            if (((((((_tr_str_eqv((inm), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((inm), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("ThreadLocal", 11LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 1LL))) {
                /* pass */
                TrStr boxrt = _tr_str_lit_len("_tr_mutexbox_new", 16LL);
                /* pass */
                if (_tr_str_eqv((inm), (_tr_str_lit_len("RwLock", 6LL)))) {
                    /* pass */
                    TrStr _strtmp_t3400 = _tr_str_lit_len("_tr_rwlbox_new", 14LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t3400;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("Atomic", 6LL)))) {
                    /* pass */
                    TrStr _strtmp_t3401 = _tr_str_lit_len("_tr_atomic_new", 14LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t3401;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("ThreadPool", 10LL)))) {
                    /* pass */
                    TrStr _strtmp_t3402 = _tr_str_lit_len("_tr_threadpool_new", 18LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t3402;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
                    /* pass */
                    TrStr _strtmp_t3403 = _tr_str_lit_len("_tr_tls_new", 11LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t3403;
                }
                /* pass */
                long long bxv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((bxv < 0LL) || (LFunc_vreg_type(lf, bxv) != 0LL))) {
                    /* pass */
                    _tr_str_release(boxrt);
                    return (-1LL);
                }
                /* pass */
                LModule_add_extern(m, boxrt);
                /* pass */
                List_i64* bxa = (void*)List_i64_new();
                /* pass */
                List_i64_append(bxa, bxv);
                /* pass */
                long long bxd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(bxd, boxrt, bxa));
                /* pass */
                LFunc_set_vreg_type(lf, bxd, 0LL);
                /* pass */
                _tr_str_release(boxrt);
                return bxd;
            }
            /* pass */
            if (((_prog_generic_class_index(m, inm) >= 0LL) && (ity_g->args->len > 0LL))) {
                /* pass */
                TrStr gsm = _ensure_generic_class(m, ity_g);
                /* pass */
                if (_tr_str_eqv((gsm), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    _tr_str_release(gsm);
                    return (-1LL);
                }
                /* pass */
                return ({ TrStr _at_t3404 = (({ TrStr _cl = (_tr_strx_concatv((gsm), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t3404, (-1LL), margs)); _tr_str_release(_at_t3404); _wr; });
            }
            /* pass */
            if (LModule_is_class(m, inm)) {
                /* pass */
                TrStr smang = LModule_resolve_method_ov(m, inm, method, margs->len);
                /* pass */
                if (_tr_str_eqv((smang), (_tr_str_lit_len("", 0LL)))) {
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
                return ({ TrStr _at_t3405 = (({ TrStr _cl = (_tr_strx_concatv((inm), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t3405, (-1LL), margs)); _tr_str_release(_at_t3405); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t3399;
            /* pass */
            /* pass */
        }
        /* pass */
        TrStr recv_cls = _recv_class(m, lf, obj);
        /* pass */
        TrStr amn = _ident_name(obj);
        /* pass */
        if (((!_tr_str_eqv((amn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, amn) > 0LL))) {
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) && (margs->len == 1LL))) {
                /* pass */
                long long amiv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
                /* pass */
                if (((amiv < 0LL) || (LFunc_vreg_type(lf, amiv) != 0LL))) {
                    /* pass */
                    _tr_str_release(recv_cls);
                    _tr_str_release(amn);
                    return (-1LL);
                }
                /* pass */
                long long amaddr = _array_elem_addr(m, lf, amn, amiv);
                /* pass */
                long long amd = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ILoad(amd, amaddr, 0LL, 2LL));
                /* pass */
                LFunc_set_vreg_type(lf, amd, _fixed_arr_elem_tag(m, lf, amn));
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return amd;
            }
            /* pass */
            if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
                /* pass */
                long long amld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IConst(amld, _fixed_arr_len(m, lf, amn)));
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return amld;
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
        /* pass */
        if (((_tr_str_eqv((recv_cls), (_tr_str_lit_len("StrView", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL)))) && (margs->len == 0LL))) {
            /* pass */
            long long svo = lower_expr(m, lf, obj);
            /* pass */
            if ((svo < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long svdata = _emit_field_get(m, lf, svo, 0LL, 0LL);
            /* pass */
            long long svlen = _emit_field_get(m, lf, svo, 8LL, 0LL);
            /* pass */
            long long svz = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(svz, 0LL));
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
            /* pass */
            List_i64* sva = (void*)List_i64_new();
            /* pass */
            List_i64_append(sva, svdata);
            /* pass */
            List_i64_append(sva, svz);
            /* pass */
            List_i64_append(sva, svlen);
            /* pass */
            long long svd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(svd, _tr_str_lit_len("_tr_rt_str_slice", 16LL), sva));
            /* pass */
            LFunc_set_vreg_type(lf, svd, 1LL);
            /* pass */
            _fresh_mark(lf, svd);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return svd;
        }
        /* pass */
        if (((_tr_str_eqv((recv_cls), (_tr_str_lit_len("StringObj", 9LL))) && _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL)))) && (margs->len == 0LL))) {
            /* pass */
            long long soo = lower_expr(m, lf, obj);
            /* pass */
            if ((soo < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long sodata = _emit_field_get(m, lf, soo, 0LL, 0LL);
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
            /* pass */
            List_i64* soa = (void*)List_i64_new();
            /* pass */
            List_i64_append(soa, sodata);
            /* pass */
            long long sod = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(sod, _tr_str_lit_len("_tr_rt_str_new", 14LL), soa));
            /* pass */
            LFunc_set_vreg_type(lf, sod, 1LL);
            /* pass */
            _fresh_mark(lf, sod);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return sod;
        }
        /* pass */
        if ((!_tr_str_eqv((recv_cls), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr dmethod = _tr_str_retain(method);
            /* pass */
            if (LModule_is_class(m, recv_cls)) {
                /* pass */
                if (((_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) && (!_tr_str_eqv((LModule_resolve_method(m, recv_cls, _tr_str_lit_len("__getitem__", 11LL))), (_tr_str_lit_len("", 0LL)))))) {
                    /* pass */
                    TrStr _strtmp_t3406 = _tr_str_lit_len("__getitem__", 11LL);
                    _tr_str_release(dmethod);
                    dmethod = _strtmp_t3406;
                }
            }
            /* pass */
            TrStr mangled = ({ TrStr _cl = (_tr_strx_concatv((recv_cls), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (dmethod)); _tr_str_release(_cl); _cres; });
            /* pass */
            if (LModule_is_class(m, recv_cls)) {
                /* pass */
                TrStr _strtmp_t3407 = LModule_resolve_method_ov(m, recv_cls, dmethod, margs->len);
                _tr_str_release(mangled);
                mangled = _strtmp_t3407;
                /* pass */
                if (_tr_str_eqv((mangled), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    long long gmix = _find_generic_method(m, recv_cls, method);
                    /* pass */
                    if ((gmix < 0LL)) {
                        /* pass */
                        _tr_str_release(recv_cls);
                        _tr_str_release(amn);
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
                        _tr_str_release(amn);
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
                    ({ TrStr _at_t3408 = (_own(recv_cls)); List_TrStr_append(gm_cls, _at_t3408); _tr_str_release(_at_t3408); });
                    /* pass */
                    List_i64_append(gm_regs, gm_self);
                    /* pass */
                    TrStr gm_name = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((recv_cls), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("__m", 3LL))); _tr_str_release(_cl); _cres; });
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
                            _tr_str_release(amn);
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
                        TrStr gmc = _tr_str_lit_len("", 0LL);
                        /* pass */
                        if (((gmt == 10LL) || (gmt == 11LL))) {
                            /* pass */
                            TrStr _strtmp_t3409 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(margs, gmj)));
                            _tr_str_release(gmc);
                            gmc = _strtmp_t3409;
                            /* pass */
                            if (_tr_str_eqv((gmc), (_tr_str_lit_len("", 0LL)))) {
                                /* pass */
                                _tr_str_release(recv_cls);
                                _tr_str_release(amn);
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
                            TrStr _strtmp_t3410 = _tr_strx_concatv((gm_name), (_tr_str_lit_len("_", 1LL)));
                            _tr_str_release(gm_name);
                            gm_name = _strtmp_t3410;
                        }
                        /* pass */
                        TrStr _strtmp_t3411 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gmt)))); TrStr _cres = _tr_strx_concatv((gm_name), _cr); _tr_str_release(_cr); _cres; });
                        _tr_str_release(gm_name);
                        gm_name = _strtmp_t3411;
                        /* pass */
                        if ((!_tr_str_eqv((gmc), (_tr_str_lit_len("", 0LL))))) {
                            /* pass */
                            TrStr _strtmp_t3412 = _tr_strx_concatv((gm_name), (gmc));
                            _tr_str_release(gm_name);
                            gm_name = _strtmp_t3412;
                        }
                        /* pass */
                        gmj = (gmj + 1LL);
                        _tr_str_release(gmc);
                    }
                    /* pass */
                    if ((gm_regs->len > 6LL)) {
                        /* pass */
                        _tr_str_release(recv_cls);
                        _tr_str_release(amn);
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
                            _tr_str_release(amn);
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
                    _tr_str_release(amn);
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
                _tr_str_release(amn);
                _tr_str_release(dmethod);
                _tr_str_release(mangled);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(dmethod);
            return _lower_obj_call(m, lf, mangled, rself, margs);
        }
        /* pass */
        long long ovm = lower_expr(m, lf, obj);
        /* pass */
        if ((ovm < 0LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
        /* pass */
        long long ovmt = LFunc_vreg_type(lf, ovm);
        /* pass */
        if ((ovmt == 1LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _lower_str_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if ((ovmt == 5LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _lower_float_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if ((ovmt == 0LL)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _lower_int_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if (_is_dict_tag(ovmt)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _lower_dict_method(m, lf, ovm, ovmt, method, margs);
        }
        /* pass */
        if (_is_set_tag(ovmt)) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _lower_set_method(m, lf, ovm, ovmt, method, margs);
        }
        /* pass */
        if ((((ovmt == 15LL) && (_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) && (margs->len == 1LL))) {
            /* pass */
            long long tgt_idx = (-1LL);
            /* pass */
            __auto_type _t3413 = (*((HirExpr*)List_ptr_get(margs, 0LL)));
            if (_t3413.tag == HirExpr_ELitInt) {
                __auto_type tgt_v = _t3413.data.ELitInt.val;
                tgt_idx = tgt_v;
            } else if (1) {
                __auto_type _ = _t3413;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            AstType* tup_hty = hir_expr_type(obj);
            /* pass */
            if (((tgt_idx < 0LL) || (tgt_idx >= tup_hty->args->len))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long tgt_tag = _tag_of(m, (*((AstType**)List_ptr_get(tup_hty->args, tgt_idx))));
            /* pass */
            if (((((tgt_tag < 0LL) || _is_list_tag(tgt_tag)) || _is_dict_tag(tgt_tag)) || _is_set_tag(tgt_tag))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
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
                _tr_str_release(amn);
                return tgf;
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _emit_field_get(m, lf, ovm, (tgt_idx * 8LL), tgt_tag);
        }
        /* pass */
        if ((!_is_list_tag(ovmt))) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
        /* pass */
        long long want_elem = _list_elem_tag(ovmt);
        /* pass */
        if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_list_len", 15LL), ovm, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))))) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((av < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long avt = LFunc_vreg_type(lf, av);
            /* pass */
            if (((want_elem == 0LL) && (avt == 4LL))) {
                /* pass */
                avt = 0LL;
            }
            /* pass */
            if ((avt != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                _secure_str(m, lf, av);
            }
            /* pass */
            if ((((want_elem == 10LL) || (want_elem == 11LL)) || (want_elem == 15LL))) {
                /* pass */
                _secure_obj(m, lf, av);
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
            TrStr apush = _tr_str_lit_len("_tr_rt_list_push_i64", 20LL);
            /* pass */
            if ((ovmt == 22LL)) {
                /* pass */
                TrStr _strtmp_t3414 = _tr_str_lit_len("_tr_rt_blist_push", 17LL);
                _tr_str_release(apush);
                apush = _strtmp_t3414;
            }
            /* pass */
            LModule_add_extern(m, apush);
            /* pass */
            List_i64* ppa = (void*)List_i64_new();
            /* pass */
            List_i64_append(ppa, ovm);
            /* pass */
            List_i64_append(ppa, av);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), apush, ppa));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(apush);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))))) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long giv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((giv < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _list_get_elem(m, lf, ovmt, ovm, giv);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) && (margs->len == 2LL))) {
            /* pass */
            long long sti = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((sti < 0LL) || (LFunc_vreg_type(lf, sti) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long stv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
            /* pass */
            if ((stv < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long stvt = LFunc_vreg_type(lf, stv);
            /* pass */
            if (((want_elem == 0LL) && (stvt == 4LL))) {
                /* pass */
                stvt = 0LL;
            }
            /* pass */
            if ((stvt != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                _secure_str(m, lf, stv);
            }
            /* pass */
            if ((((want_elem == 10LL) || (want_elem == 11LL)) || (want_elem == 15LL))) {
                /* pass */
                _secure_obj(m, lf, stv);
            }
            /* pass */
            if ((want_elem == 5LL)) {
                /* pass */
                long long stfb = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IFBits(stfb, stv));
                /* pass */
                stv = stfb;
            }
            /* pass */
            long long st_addr = _inline_list_addr(m, lf, ovm, sti, _list_stride(ovmt));
            /* pass */
            if ((ovmt == 22LL)) {
                /* pass */
                LFunc_emit(lf, LInst_ctor_IStoreB(st_addr, 0LL, stv));
            } else {
                /* pass */
                LFunc_emit(lf, LInst_ctor_IStore(st_addr, 0LL, stv, 2LL));
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL))) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_pop_i64", 19LL));
            /* pass */
            List_i64* poa = (void*)List_i64_new();
            /* pass */
            List_i64_append(poa, ovm);
            /* pass */
            long long pod = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(pod, _tr_str_lit_len("_tr_rt_list_pop_i64", 19LL), poa));
            /* pass */
            LFunc_set_vreg_type(lf, pod, want_elem);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return pod;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) && (margs->len == 1LL))) {
            /* pass */
            long long ixa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((ixa < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long ixat = LFunc_vreg_type(lf, ixa);
            /* pass */
            if (((want_elem == 0LL) && (ixat == 4LL))) {
                /* pass */
                ixat = 0LL;
            }
            /* pass */
            if ((ixat != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            TrStr ixsym = _tr_str_lit_len("_tr_rt_list_index_i64", 21LL);
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t3415 = _tr_str_lit_len("_tr_rt_list_index_str", 21LL);
                _tr_str_release(ixsym);
                ixsym = _strtmp_t3415;
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
            _tr_str_release(amn);
            _tr_str_release(ixsym);
            return ixd;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) && (margs->len == 1LL))) {
            /* pass */
            long long cxa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((cxa < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long cxat = LFunc_vreg_type(lf, cxa);
            /* pass */
            if (((want_elem == 0LL) && (cxat == 4LL))) {
                /* pass */
                cxat = 0LL;
            }
            /* pass */
            if ((cxat != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            TrStr cxsym = _tr_str_lit_len("_tr_rt_list_contains_i64", 24LL);
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t3416 = _tr_str_lit_len("_tr_rt_list_contains_str", 24LL);
                _tr_str_release(cxsym);
                cxsym = _strtmp_t3416;
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
            _tr_str_release(amn);
            _tr_str_release(cxsym);
            return cxd;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) && (margs->len == 1LL))) {
            /* pass */
            long long cta = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((cta < 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long ctat = LFunc_vreg_type(lf, cta);
            /* pass */
            if (((want_elem == 0LL) && (ctat == 4LL))) {
                /* pass */
                ctat = 0LL;
            }
            /* pass */
            if ((ctat != want_elem)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            TrStr ctsym = _tr_str_lit_len("_tr_rt_list_count_i64", 21LL);
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t3417 = _tr_str_lit_len("_tr_rt_list_count_str", 21LL);
                _tr_str_release(ctsym);
                ctsym = _strtmp_t3417;
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
            _tr_str_release(amn);
            _tr_str_release(ctsym);
            return ctd;
        }
        /* pass */
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))) && (margs->len == 0LL))) {
            /* pass */
            if ((want_elem != 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            TrStr mmsym = _tr_str_lit_len("_tr_rt_list_min_i64", 19LL);
            /* pass */
            if ((_tr_str_eqv((method), (_tr_str_lit_len("max", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL))))) {
                /* pass */
                TrStr _strtmp_t3418 = _tr_str_lit_len("_tr_rt_list_max_i64", 19LL);
                _tr_str_release(mmsym);
                mmsym = _strtmp_t3418;
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _list_call1(m, lf, mmsym, ovm, 0LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL))) && (margs->len == 0LL))) {
            /* pass */
            if ((want_elem == 0LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL), ovm, 0LL);
            }
            /* pass */
            if ((want_elem == 5LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_sum_f64", 19LL));
                /* pass */
                long long fsum = LFunc_new_vreg(lf);
                /* pass */
                List_i64* fsa = (void*)List_i64_new();
                /* pass */
                List_i64_append(fsa, ovm);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(fsum, _tr_str_lit_len("_tr_rt_list_sum_f64", 19LL), fsa));
                /* pass */
                long long fsr = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBitsF(fsr, fsum));
                /* pass */
                LFunc_set_vreg_type(lf, fsr, 5LL);
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return fsr;
            }
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
        /* pass */
        if (((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_clone", 17LL));
            /* pass */
            List_i64* cla = (void*)List_i64_new();
            /* pass */
            List_i64_append(cla, ovm);
            /* pass */
            long long cld = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cld, _tr_str_lit_len("_tr_rt_list_clone", 17LL), cla));
            /* pass */
            LFunc_set_vreg_type(lf, cld, ovmt);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return cld;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))) && (margs->len == 1LL))) {
            /* pass */
            long long rmi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((rmi < 0LL) || (LFunc_vreg_type(lf, rmi) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_remove", 18LL));
            /* pass */
            List_i64* rma = (void*)List_i64_new();
            /* pass */
            List_i64_append(rma, ovm);
            /* pass */
            List_i64_append(rma, rmi);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_remove", 18LL), rma));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL))) && (margs->len == 2LL))) {
            /* pass */
            long long swi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((swi < 0LL) || (LFunc_vreg_type(lf, swi) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long swj = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
            /* pass */
            if (((swj < 0LL) || (LFunc_vreg_type(lf, swj) != 0LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_swap", 16LL));
            /* pass */
            List_i64* swa = (void*)List_i64_new();
            /* pass */
            List_i64_append(swa, ovm);
            /* pass */
            List_i64_append(swa, swi);
            /* pass */
            List_i64_append(swa, swj);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_swap", 16LL), swa));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))) && (margs->len == 1LL))) {
            /* pass */
            if ((ovmt != 3LL)) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long jsep = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((jsep < 0LL) || (LFunc_vreg_type(lf, jsep) != 1LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_join", 16LL));
            /* pass */
            List_i64* jargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(jargs, ovm);
            /* pass */
            List_i64_append(jargs, jsep);
            /* pass */
            long long jd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(jd, _tr_str_lit_len("_tr_rt_list_join", 16LL), jargs));
            /* pass */
            LFunc_set_vreg_type(lf, jd, 1LL);
            /* pass */
            _fresh_mark(lf, jd);
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return jd;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_is_empty", 20LL), ovm, 4LL);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("first", 5LL))) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_first_i64", 21LL), ovm, want_elem);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("last", 4LL))) && (margs->len == 0LL))) {
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_last_i64", 20LL), ovm, want_elem);
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_reverse", 19LL));
            /* pass */
            List_i64* rva = (void*)List_i64_new();
            /* pass */
            List_i64_append(rva, ovm);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_reverse", 19LL), rva));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL))) && (margs->len == 1LL))) {
            /* pass */
            long long exo = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if (((exo < 0LL) || (LFunc_vreg_type(lf, exo) != ovmt))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_extend", 18LL));
            /* pass */
            List_i64* exa = (void*)List_i64_new();
            /* pass */
            List_i64_append(exa, ovm);
            /* pass */
            List_i64_append(exa, exo);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_extend", 18LL), exa));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL))) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_clear", 17LL));
            /* pass */
            List_i64* cla = (void*)List_i64_new();
            /* pass */
            List_i64_append(cla, ovm);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_clear", 17LL), cla));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return ovm;
        }
        /* pass */
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("sort", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) && (margs->len == 0LL))) {
            /* pass */
            if (((want_elem != 0LL) && (want_elem != 1LL))) {
                /* pass */
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
            /* pass */
            long long dirv = LFunc_new_vreg(lf);
            /* pass */
            long long dir = 1LL;
            /* pass */
            if (_tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) {
                /* pass */
                dir = (0LL - 1LL);
            }
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(dirv, dir));
            /* pass */
            TrStr sortsym = _tr_str_lit_len("_tr_rt_list_sort", 16LL);
            /* pass */
            if ((want_elem == 1LL)) {
                /* pass */
                TrStr _strtmp_t3419 = _tr_str_lit_len("_tr_rt_list_sort_str", 20LL);
                _tr_str_release(sortsym);
                sortsym = _strtmp_t3419;
            }
            /* pass */
            LModule_add_extern(m, sortsym);
            /* pass */
            List_i64* soa = (void*)List_i64_new();
            /* pass */
            List_i64_append(soa, ovm);
            /* pass */
            List_i64_append(soa, dirv);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), sortsym, soa));
            /* pass */
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(sortsym);
            return ovm;
        }
        /* pass */
        _tr_str_release(recv_cls);
        _tr_str_release(amn);
        return (-1LL);
    } else if (1) {
        __auto_type _ = _t3355;
        /* pass */
        return (-1LL);
    }
}

__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    long long r = _lower_expr_impl(m, lf, e);
    /* pass */
    if (((r < 0LL) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        m->fail_note = ({ TrStr _cr = (_expr_kind(e)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("unsupported expression: ", 24LL)), _cr); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    return r;
}

