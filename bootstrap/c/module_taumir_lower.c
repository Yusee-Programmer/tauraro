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
#line 22 "src/taumir/lower.tr"
    /* unsafe block */
#line 23 "src/taumir/lower.tr"
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
#line 24 "src/taumir/lower.tr"
    (*p = t);
#line 25 "src/taumir/lower.tr"
    return p;
}

__attribute__((hot)) long long _f64_bits(double v) {
#line 31 "src/taumir/lower.tr"
    /* unsafe block */
#line 32 "src/taumir/lower.tr"
    double* p = ((double*)_tr_c_calloc((size_t)(1LL), sizeof(double)));
#line 33 "src/taumir/lower.tr"
    (*p = v);
#line 34 "src/taumir/lower.tr"
    long long* ip = ((long long*)(p));
#line 35 "src/taumir/lower.tr"
    return (*ip);
}

__attribute__((hot)) long long _promote_f(LFunc* lf, long long v) {
#line 39 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 40 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IIToF(d, v));
#line 41 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, 5LL);
#line 42 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _int_cast_width(TrStr tn) {
#line 48 "src/taumir/lower.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL))))) {
#line 48 "src/taumir/lower.tr"
        return 8LL;
    }
#line 49 "src/taumir/lower.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL))))) {
#line 49 "src/taumir/lower.tr"
        return 16LL;
    }
#line 50 "src/taumir/lower.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL))))) {
#line 50 "src/taumir/lower.tr"
        return 32LL;
    }
#line 51 "src/taumir/lower.tr"
    return 0LL;
}

__attribute__((hot)) bool _int_cast_signed(TrStr tn) {
#line 54 "src/taumir/lower.tr"
    return ((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL))));
}

__attribute__((hot)) long long _narrow_int(LFunc* lf, long long v, TrStr tn) {
#line 57 "src/taumir/lower.tr"
    long long bits = _int_cast_width(tn);
#line 58 "src/taumir/lower.tr"
    if ((bits == 0LL)) {
#line 58 "src/taumir/lower.tr"
        return v;
    }
#line 60 "src/taumir/lower.tr"
    long long maskc = LFunc_new_vreg(lf);
#line 61 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(maskc, (((long long)((unsigned long long)(1LL) << (bits))) - 1LL)));
#line 62 "src/taumir/lower.tr"
    long long masked = LFunc_new_vreg(lf);
#line 63 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(masked, _tr_str_lit_len("&", 1LL), v, maskc));
#line 64 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, masked, 0LL);
#line 65 "src/taumir/lower.tr"
    if ((!_int_cast_signed(tn))) {
#line 65 "src/taumir/lower.tr"
        return masked;
    }
#line 67 "src/taumir/lower.tr"
    long long sbc = LFunc_new_vreg(lf);
#line 68 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(sbc, ((long long)((unsigned long long)(1LL) << ((bits - 1LL))))));
#line 69 "src/taumir/lower.tr"
    long long xored = LFunc_new_vreg(lf);
#line 70 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(xored, _tr_str_lit_len("^", 1LL), masked, sbc));
#line 71 "src/taumir/lower.tr"
    long long res = LFunc_new_vreg(lf);
#line 72 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(res, _tr_str_lit_len("-", 1LL), xored, sbc));
#line 73 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, res, 0LL);
#line 74 "src/taumir/lower.tr"
    return res;
}

__attribute__((hot)) bool _is_int_cast_target(TrStr tn) {
#line 78 "src/taumir/lower.tr"
    if (((((_tr_str_eqv((tn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("isize", 5LL))))) {
#line 78 "src/taumir/lower.tr"
        return true;
    }
#line 79 "src/taumir/lower.tr"
    if ((((((_tr_str_eqv((tn), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("u32", 3LL))))) {
#line 79 "src/taumir/lower.tr"
        return true;
    }
#line 80 "src/taumir/lower.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("bool", 4LL)))) {
#line 80 "src/taumir/lower.tr"
        return true;
    }
#line 81 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) TrStr _print_i64_sym() {
#line 84 "src/taumir/lower.tr"
    return _tr_str_lit_len("_tr_rt_print_i64", 16LL);
}

__attribute__((hot)) bool _is_list_tag(long long t) {
#line 89 "src/taumir/lower.tr"
    return ((((((t == 2LL) || (t == 3LL)) || (t == 14LL)) || (t == 19LL)) || (t == 21LL)) || (t == 22LL));
}

__attribute__((hot)) long long _list_stride(long long ltag) {
#line 94 "src/taumir/lower.tr"
    if ((ltag == 22LL)) {
#line 94 "src/taumir/lower.tr"
        return 1LL;
    }
#line 95 "src/taumir/lower.tr"
    return 8LL;
}

__attribute__((hot)) bool _is_set_tag(long long t) {
#line 98 "src/taumir/lower.tr"
    return ((t == 13LL) || (t == 16LL));
}

__attribute__((hot)) TrStr _set_sym(long long t, TrStr op) {
#line 102 "src/taumir/lower.tr"
    if ((t == 16LL)) {
#line 102 "src/taumir/lower.tr"
        return _tr_strx_concatv((_tr_str_lit_len("_tr_rt_sdict_", 13LL)), (op));
    }
#line 103 "src/taumir/lower.tr"
    return _tr_strx_concatv((_tr_str_lit_len("_tr_rt_idict_", 13LL)), (op));
}

__attribute__((hot)) bool _is_dict_tag(long long t) {
#line 106 "src/taumir/lower.tr"
    return ((((((((((t == 6LL) || (t == 7LL)) || (t == 8LL)) || (t == 9LL)) || (t == 17LL)) || (t == 18LL)) || (t == 24LL)) || (t == 25LL)) || (t == 26LL)) || (t == 27LL));
}

__attribute__((hot)) bool _dict_key_is_str(long long t) {
#line 109 "src/taumir/lower.tr"
    return (((((t == 6LL) || (t == 8LL)) || (t == 17LL)) || (t == 24LL)) || (t == 26LL));
}

__attribute__((hot)) long long _dict_val_tag(long long t) {
#line 112 "src/taumir/lower.tr"
    if (((t == 8LL) || (t == 9LL))) {
#line 112 "src/taumir/lower.tr"
        return 1LL;
    }
#line 113 "src/taumir/lower.tr"
    if (((t == 17LL) || (t == 18LL))) {
#line 113 "src/taumir/lower.tr"
        return 5LL;
    }
#line 114 "src/taumir/lower.tr"
    if (((t == 24LL) || (t == 25LL))) {
#line 114 "src/taumir/lower.tr"
        return 10LL;
    }
#line 115 "src/taumir/lower.tr"
    if (((t == 26LL) || (t == 27LL))) {
#line 115 "src/taumir/lower.tr"
        return 4LL;
    }
#line 116 "src/taumir/lower.tr"
    return 0LL;
}

__attribute__((hot)) TrStr _dict_new_sym(long long t) {
#line 119 "src/taumir/lower.tr"
    if (_dict_key_is_str(t)) {
#line 119 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_sdict_new", 16LL);
    }
#line 120 "src/taumir/lower.tr"
    return _tr_str_lit_len("_tr_rt_idict_new", 16LL);
}

__attribute__((hot)) TrStr _dict_sym(long long t, TrStr op) {
#line 124 "src/taumir/lower.tr"
    TrStr pfx = _tr_str_lit_len("_tr_rt_idict_", 13LL);
#line 125 "src/taumir/lower.tr"
    if (_dict_key_is_str(t)) {
#line 125 "src/taumir/lower.tr"
        TrStr _strtmp_t6096 = _tr_str_lit_len("_tr_rt_sdict_", 13LL);
        _tr_str_release(pfx);
        pfx = _strtmp_t6096;
    }
#line 126 "src/taumir/lower.tr"
    TrStr _trret1489 = _tr_strx_concatv((pfx), (op));
    _tr_str_release(pfx);
    return _trret1489;
}

__attribute__((hot)) long long _list_elem_tag(long long t) {
#line 129 "src/taumir/lower.tr"
    if ((t == 3LL)) {
#line 129 "src/taumir/lower.tr"
        return 1LL;
    }
#line 130 "src/taumir/lower.tr"
    if ((t == 14LL)) {
#line 130 "src/taumir/lower.tr"
        return 5LL;
    }
#line 131 "src/taumir/lower.tr"
    if ((t == 19LL)) {
#line 131 "src/taumir/lower.tr"
        return 10LL;
    }
#line 132 "src/taumir/lower.tr"
    if ((t == 21LL)) {
#line 132 "src/taumir/lower.tr"
        return 15LL;
    }
#line 133 "src/taumir/lower.tr"
    if ((t == 22LL)) {
#line 133 "src/taumir/lower.tr"
        return 4LL;
    }
#line 134 "src/taumir/lower.tr"
    return 0LL;
}

__attribute__((hot)) long long _list_tag_for_elem(long long et) {
#line 137 "src/taumir/lower.tr"
    if ((et == 1LL)) {
#line 137 "src/taumir/lower.tr"
        return 3LL;
    }
#line 138 "src/taumir/lower.tr"
    if ((et == 5LL)) {
#line 138 "src/taumir/lower.tr"
        return 14LL;
    }
#line 139 "src/taumir/lower.tr"
    if ((et == 10LL)) {
#line 139 "src/taumir/lower.tr"
        return 19LL;
    }
#line 140 "src/taumir/lower.tr"
    if ((et == 15LL)) {
#line 140 "src/taumir/lower.tr"
        return 21LL;
    }
#line 141 "src/taumir/lower.tr"
    if ((et == 4LL)) {
#line 141 "src/taumir/lower.tr"
        return 22LL;
    }
#line 142 "src/taumir/lower.tr"
    return 2LL;
}

__attribute__((hot)) long long _list_tag_from_ann(LModule* m, AstType* ty) {
#line 146 "src/taumir/lower.tr"
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))))) {
#line 146 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 147 "src/taumir/lower.tr"
    long long t = _tag_of(m, ty);
#line 148 "src/taumir/lower.tr"
    if ((((((t == 2LL) || (t == 3LL)) || (t == 14LL)) || (t == 19LL)) || (t == 21LL))) {
#line 148 "src/taumir/lower.tr"
        return t;
    }
#line 149 "src/taumir/lower.tr"
    if ((ty->args->len > 0LL)) {
#line 150 "src/taumir/lower.tr"
        TrStr en = _tr_str_retain((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
#line 151 "src/taumir/lower.tr"
        if (_tr_str_eqv((en), (_tr_str_lit_len("bool", 4LL)))) {
#line 151 "src/taumir/lower.tr"
            _tr_str_release(en);
            return 2LL;
        }
#line 152 "src/taumir/lower.tr"
        if ((_tr_str_eqv((en), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((en), (_tr_str_lit_len("tuple", 5LL))))) {
#line 152 "src/taumir/lower.tr"
            _tr_str_release(en);
            return 21LL;
        }
    }
#line 153 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) long long _dict_tag_from_ann(LModule* m, AstType* ty) {
#line 157 "src/taumir/lower.tr"
    if (((!_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL)))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))))) {
#line 157 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 158 "src/taumir/lower.tr"
    if ((ty->args->len != 2LL)) {
#line 158 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 159 "src/taumir/lower.tr"
    AstType* kt = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 160 "src/taumir/lower.tr"
    AstType* vt = (*((AstType**)List_ptr_get(ty->args, 1LL)));
#line 161 "src/taumir/lower.tr"
    bool kstr = (_tr_str_eqv((kt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((kt->name), (_tr_str_lit_len("String", 6LL))));
#line 162 "src/taumir/lower.tr"
    bool kint = _is_int_typename(kt->name);
#line 163 "src/taumir/lower.tr"
    bool vstr = (_tr_str_eqv((vt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("String", 6LL))));
#line 164 "src/taumir/lower.tr"
    bool vint = _is_int_typename(vt->name);
#line 165 "src/taumir/lower.tr"
    bool vflt = (_tr_str_eqv((vt->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("f64", 3LL))));
#line 166 "src/taumir/lower.tr"
    if ((kstr && vint)) {
#line 166 "src/taumir/lower.tr"
        return 6LL;
    }
#line 167 "src/taumir/lower.tr"
    if ((kint && vint)) {
#line 167 "src/taumir/lower.tr"
        return 7LL;
    }
#line 168 "src/taumir/lower.tr"
    if ((kstr && vstr)) {
#line 168 "src/taumir/lower.tr"
        return 8LL;
    }
#line 169 "src/taumir/lower.tr"
    if ((kint && vstr)) {
#line 169 "src/taumir/lower.tr"
        return 9LL;
    }
#line 170 "src/taumir/lower.tr"
    if ((kstr && vflt)) {
#line 170 "src/taumir/lower.tr"
        return 17LL;
    }
#line 171 "src/taumir/lower.tr"
    if ((kint && vflt)) {
#line 171 "src/taumir/lower.tr"
        return 18LL;
    }
#line 172 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _is_cmp_op(TrStr op) {
#line 175 "src/taumir/lower.tr"
    return (((((_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))));
}

__attribute__((hot)) bool _is_int_typename(TrStr n) {
#line 178 "src/taumir/lower.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))))) {
#line 178 "src/taumir/lower.tr"
        return true;
    }
#line 179 "src/taumir/lower.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))))) {
#line 179 "src/taumir/lower.tr"
        return true;
    }
#line 180 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) long long _ast_type_tag(AstType* ty) {
#line 185 "src/taumir/lower.tr"
    TrStr n = _tr_str_retain(ty->name);
#line 186 "src/taumir/lower.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("String", 6LL))))) {
#line 186 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 1LL;
    }
#line 187 "src/taumir/lower.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 187 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 4LL;
    }
#line 188 "src/taumir/lower.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL))))) {
#line 189 "src/taumir/lower.tr"
        if ((ty->args->len > 0LL)) {
#line 190 "src/taumir/lower.tr"
            AstType* et = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 191 "src/taumir/lower.tr"
            if ((_tr_str_eqv((et->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((et->name), (_tr_str_lit_len("String", 6LL))))) {
#line 191 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 3LL;
            }
#line 192 "src/taumir/lower.tr"
            if (_is_int_typename(et->name)) {
#line 192 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 2LL;
            }
#line 193 "src/taumir/lower.tr"
            if (_tr_str_eqv((et->name), (_tr_str_lit_len("bool", 4LL)))) {
#line 193 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 22LL;
            }
#line 194 "src/taumir/lower.tr"
            if ((_tr_str_eqv((et->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((et->name), (_tr_str_lit_len("f64", 3LL))))) {
#line 194 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 14LL;
            }
#line 195 "src/taumir/lower.tr"
            _tr_str_release(n);
            return (-1LL);
        }
#line 196 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 2LL;
    }
#line 197 "src/taumir/lower.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) {
#line 198 "src/taumir/lower.tr"
        if ((ty->args->len > 0LL)) {
#line 199 "src/taumir/lower.tr"
            AstType* se = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 200 "src/taumir/lower.tr"
            if ((_tr_str_eqv((se->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((se->name), (_tr_str_lit_len("String", 6LL))))) {
#line 200 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 16LL;
            }
#line 201 "src/taumir/lower.tr"
            if (_is_int_typename(se->name)) {
#line 201 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 13LL;
            }
        }
#line 202 "src/taumir/lower.tr"
        _tr_str_release(n);
        return (-1LL);
    }
#line 203 "src/taumir/lower.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL)))) {
#line 203 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 15LL;
    }
#line 204 "src/taumir/lower.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("def", 3LL)))) {
#line 204 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 0LL;
    }
#line 205 "src/taumir/lower.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL))))) {
#line 206 "src/taumir/lower.tr"
        if ((ty->args->len >= 2LL)) {
#line 207 "src/taumir/lower.tr"
            AstType* kt = (*((AstType**)List_ptr_get(ty->args, 0LL)));
#line 208 "src/taumir/lower.tr"
            AstType* vt = (*((AstType**)List_ptr_get(ty->args, 1LL)));
#line 209 "src/taumir/lower.tr"
            bool kstr = (_tr_str_eqv((kt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((kt->name), (_tr_str_lit_len("String", 6LL))));
#line 210 "src/taumir/lower.tr"
            bool kint = _is_int_typename(kt->name);
#line 211 "src/taumir/lower.tr"
            bool vstr = (_tr_str_eqv((vt->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("String", 6LL))));
#line 212 "src/taumir/lower.tr"
            bool vint = _is_int_typename(vt->name);
#line 213 "src/taumir/lower.tr"
            bool vflt = (_tr_str_eqv((vt->name), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((vt->name), (_tr_str_lit_len("f64", 3LL))));
#line 214 "src/taumir/lower.tr"
            bool vbool = _tr_str_eqv((vt->name), (_tr_str_lit_len("bool", 4LL)));
#line 215 "src/taumir/lower.tr"
            if (_tr_str_eqv((vt->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 215 "src/taumir/lower.tr"
                vint = true;
            }
#line 216 "src/taumir/lower.tr"
            if ((kstr && vint)) {
#line 216 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 6LL;
            }
#line 217 "src/taumir/lower.tr"
            if ((kint && vint)) {
#line 217 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 7LL;
            }
#line 218 "src/taumir/lower.tr"
            if ((kstr && vstr)) {
#line 218 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 8LL;
            }
#line 219 "src/taumir/lower.tr"
            if ((kint && vstr)) {
#line 219 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 9LL;
            }
#line 220 "src/taumir/lower.tr"
            if ((kstr && vflt)) {
#line 220 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 17LL;
            }
#line 221 "src/taumir/lower.tr"
            if ((kint && vflt)) {
#line 221 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 18LL;
            }
#line 222 "src/taumir/lower.tr"
            if ((kstr && vbool)) {
#line 222 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 26LL;
            }
#line 223 "src/taumir/lower.tr"
            if ((kint && vbool)) {
#line 223 "src/taumir/lower.tr"
                _tr_str_release(n);
                return 27LL;
            }
        }
#line 224 "src/taumir/lower.tr"
        _tr_str_release(n);
        return (-1LL);
    }
#line 225 "src/taumir/lower.tr"
    if (_is_int_typename(n)) {
#line 225 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 0LL;
    }
#line 226 "src/taumir/lower.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) {
#line 226 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 0LL;
    }
#line 232 "src/taumir/lower.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))))) {
#line 232 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 5LL;
    }
#line 233 "src/taumir/lower.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL))))) {
#line 233 "src/taumir/lower.tr"
        _tr_str_release(n);
        return 0LL;
    }
#line 234 "src/taumir/lower.tr"
    _tr_str_release(n);
    return (-1LL);
}

__attribute__((hot)) bool _is_null_str(TrStr s) {
#line 239 "src/taumir/lower.tr"
    return (((unsigned long long)(((char*)(_tr_strz(s))))) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _own(TrStr s) {
#line 245 "src/taumir/lower.tr"
    if (_is_null_str(s)) {
#line 245 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 246 "src/taumir/lower.tr"
    return _tr_strx_concatv((s), (_tr_str_lit_len("", 0LL)));
}

__attribute__((hot)) long long _tag_of(LModule* m, AstType* ty) {
#line 253 "src/taumir/lower.tr"
    if (_is_null_str(ty->name)) {
#line 253 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 254 "src/taumir/lower.tr"
    if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 254 "src/taumir/lower.tr"
        return 0LL;
    }
#line 257 "src/taumir/lower.tr"
    if (((((((_tr_str_eqv((ty->name), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("ThreadLocal", 11LL))))) {
#line 257 "src/taumir/lower.tr"
        return 0LL;
    }
#line 259 "src/taumir/lower.tr"
    long long sbi = 0LL;
#line 260 "src/taumir/lower.tr"
    while ((sbi < m->subst_names->len)) {
#line 261 "src/taumir/lower.tr"
        if (({ __auto_type _ord_t6097 = (List_TrStr_get(m->subst_names, sbi)); __auto_type _wr = (({ TrStr _cmpa_t6098 = (_ord_t6097); TrStr _cmpb_t6099 = (ty->name); _Bool _cmpr_t6100 = _tr_str_eqv(_cmpa_t6098, _cmpb_t6099); _tr_str_release(_cmpa_t6098); _cmpr_t6100; })); _wr; })) {
#line 262 "src/taumir/lower.tr"
            return _tag_of(m, (*((AstType**)List_ptr_get(m->subst_tys, sbi))));
        }
#line 263 "src/taumir/lower.tr"
        sbi = (sbi + 1LL);
    }
#line 264 "src/taumir/lower.tr"
    if (LModule_is_class(m, ty->name)) {
#line 264 "src/taumir/lower.tr"
        return 10LL;
    }
#line 265 "src/taumir/lower.tr"
    if (LModule_is_enum(m, ty->name)) {
#line 265 "src/taumir/lower.tr"
        return 11LL;
    }
#line 287 "src/taumir/lower.tr"
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL)))) && (ty->args->len >= 2LL))) {
#line 288 "src/taumir/lower.tr"
        AstType* dkk = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 289 "src/taumir/lower.tr"
        AstType* dvv = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 1LL))));
#line 290 "src/taumir/lower.tr"
        if (((!_is_null_str(dvv->name)) && (LModule_is_class(m, dvv->name) || LModule_is_enum(m, dvv->name)))) {
#line 291 "src/taumir/lower.tr"
            if ((_tr_str_eqv((dkk->name), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((dkk->name), (_tr_str_lit_len("String", 6LL))))) {
#line 291 "src/taumir/lower.tr"
                return 24LL;
            }
#line 292 "src/taumir/lower.tr"
            if (_is_int_typename(dkk->name)) {
#line 292 "src/taumir/lower.tr"
                return 25LL;
            }
        }
#line 293 "src/taumir/lower.tr"
        long long _dtg = _ast_type_tag(_subst_args(m, ty));
#line 294 "src/taumir/lower.tr"
        if (_is_dict_tag(_dtg)) {
#line 294 "src/taumir/lower.tr"
            long long _trret1490 = _dtg;
            return _trret1490;
        }
#line 295 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 297 "src/taumir/lower.tr"
    if (((ty->args->len > 0LL) && (_prog_generic_class_index(m, ty->name) >= 0LL))) {
#line 298 "src/taumir/lower.tr"
        if (({ TrStr _cmpa_t6101 = (_ensure_generic_class(m, ty)); TrStr _cmpb_t6102 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6103 = !_tr_str_eqv(_cmpa_t6101, _cmpb_t6102); _tr_str_release(_cmpa_t6101); _cmpr_t6103; })) {
#line 298 "src/taumir/lower.tr"
            return 10LL;
        }
#line 299 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 300 "src/taumir/lower.tr"
    if (((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) && (ty->args->len > 0LL))) {
#line 301 "src/taumir/lower.tr"
        AstType* lelem = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 302 "src/taumir/lower.tr"
        if (((!_is_null_str(lelem->name)) && LModule_is_class(m, lelem->name))) {
#line 302 "src/taumir/lower.tr"
            return 19LL;
        }
    }
#line 305 "src/taumir/lower.tr"
    if (((ty->args->len > 0LL) && (m->subst_names->len > 0LL))) {
#line 306 "src/taumir/lower.tr"
        return _ast_type_tag(_subst_args(m, ty));
    }
#line 307 "src/taumir/lower.tr"
    return _ast_type_tag(ty);
}

__attribute__((hot)) AstType* _subst_args(LModule* m, AstType* ty) {
#line 312 "src/taumir/lower.tr"
    if ((ty->args->len == 0LL)) {
#line 312 "src/taumir/lower.tr"
        return ty;
    }
#line 313 "src/taumir/lower.tr"
    AstType* r = ({ TrStr _at_t6104 = (_own(ty->name)); __auto_type _wr = (AstType_init(_at_t6104)); _tr_str_release(_at_t6104); _wr; });
#line 314 "src/taumir/lower.tr"
    long long i = 0LL;
#line 315 "src/taumir/lower.tr"
    while ((i < ty->args->len)) {
#line 316 "src/taumir/lower.tr"
        ({ __auto_type _ord_t6105 = (box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(ty->args, i)))))); List_ptr_append(r->args, _ord_t6105); });
#line 317 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 318 "src/taumir/lower.tr"
    return r;
}

__attribute__((hot)) AstType* _subst_ty(LModule* m, AstType* ty) {
#line 322 "src/taumir/lower.tr"
    if (_is_null_str(ty->name)) {
#line 322 "src/taumir/lower.tr"
        return ty;
    }
#line 323 "src/taumir/lower.tr"
    long long sbi = 0LL;
#line 324 "src/taumir/lower.tr"
    while ((sbi < m->subst_names->len)) {
#line 325 "src/taumir/lower.tr"
        if (({ __auto_type _ord_t6106 = (List_TrStr_get(m->subst_names, sbi)); __auto_type _wr = (({ TrStr _cmpa_t6107 = (_ord_t6106); TrStr _cmpb_t6108 = (ty->name); _Bool _cmpr_t6109 = _tr_str_eqv(_cmpa_t6107, _cmpb_t6108); _tr_str_release(_cmpa_t6107); _cmpr_t6109; })); _wr; })) {
#line 325 "src/taumir/lower.tr"
            return (*((AstType**)List_ptr_get(m->subst_tys, sbi)));
        }
#line 326 "src/taumir/lower.tr"
        sbi = (sbi + 1LL);
    }
#line 327 "src/taumir/lower.tr"
    return ty;
}

__attribute__((hot)) long long _prog_generic_class_index(LModule* m, TrStr name) {
#line 331 "src/taumir/lower.tr"
    if (_is_null_str(name)) {
#line 331 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 332 "src/taumir/lower.tr"
    long long i = 0LL;
#line 333 "src/taumir/lower.tr"
    while ((i < m->hir_prog->classes->len)) {
#line 334 "src/taumir/lower.tr"
        HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, i));
#line 335 "src/taumir/lower.tr"
        if ((_tr_str_eqv((c->name), (name)) && (c->generics->len > 0LL))) {
#line 335 "src/taumir/lower.tr"
            long long _trret1491 = i;
            return _trret1491;
        }
#line 336 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 337 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _is_real_generic_class_arity_match(LModule* m, TrStr name, long long arg_count) {
#line 359 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) TrStr _mangle_generic(LModule* m, AstType* ty) {
#line 363 "src/taumir/lower.tr"
    TrStr n = _own(ty->name);
#line 364 "src/taumir/lower.tr"
    long long ai = 0LL;
#line 365 "src/taumir/lower.tr"
    while ((ai < ty->args->len)) {
#line 366 "src/taumir/lower.tr"
        AstType* at = _subst_ty(m, (*((AstType**)List_ptr_get(ty->args, ai))));
#line 367 "src/taumir/lower.tr"
        TrStr _strtmp_t6112 = ({ TrStr _tr_cl = (_tr_strx_concatv((n), (_tr_str_lit_len("__g", 3LL)))); TrStr _tr_cr = (_own(at->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(n);
        n = _strtmp_t6112;
#line 368 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 369 "src/taumir/lower.tr"
    TrStr _trret1492 = _tr_str_retain(n);
    _tr_str_release(n);
    return _trret1492;
}

__attribute__((hot)) TrStr _ensure_generic_class(LModule* m, AstType* ty) {
#line 375 "src/taumir/lower.tr"
    long long gci = _prog_generic_class_index(m, ty->name);
#line 376 "src/taumir/lower.tr"
    if ((gci < 0LL)) {
#line 376 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 377 "src/taumir/lower.tr"
    HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, gci));
#line 378 "src/taumir/lower.tr"
    if ((c->generics->len != ty->args->len)) {
#line 378 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 379 "src/taumir/lower.tr"
    if ((c->base_classes->len > 0LL)) {
#line 379 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 380 "src/taumir/lower.tr"
    TrStr mangled = _mangle_generic(m, ty);
#line 381 "src/taumir/lower.tr"
    if (LModule_is_class(m, mangled)) {
#line 381 "src/taumir/lower.tr"
        TrStr _trret1493 = _tr_str_retain(mangled);
        _tr_str_release(mangled);
        return _trret1493;
    }
#line 383 "src/taumir/lower.tr"
    long long sb = 0LL;
#line 384 "src/taumir/lower.tr"
    while ((sb < c->generics->len)) {
#line 385 "src/taumir/lower.tr"
        ({ TrStr _at_t6113 = (List_TrStr_get(c->generics, sb)); TrStr _ord_t6114 = (_own(_at_t6113)); List_TrStr_append(m->subst_names, _ord_t6114); _tr_str_release(_at_t6113); _tr_str_release(_ord_t6114); });
#line 386 "src/taumir/lower.tr"
        ({ __auto_type _ord_t6115 = (box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(ty->args, sb)))))); List_ptr_append(m->subst_tys, _ord_t6115); });
#line 387 "src/taumir/lower.tr"
        sb = (sb + 1LL);
    }
#line 389 "src/taumir/lower.tr"
    ClassLayout* lay = ({ TrStr _at_t6116 = (_own(mangled)); __auto_type _wr = (ClassLayout_init(_at_t6116)); _tr_str_release(_at_t6116); _wr; });
#line 390 "src/taumir/lower.tr"
    lay->is_value = (!c->is_class);
#line 391 "src/taumir/lower.tr"
    bool ok = true;
#line 392 "src/taumir/lower.tr"
    long long fi = 0LL;
#line 393 "src/taumir/lower.tr"
    while ((fi < c->fields->len)) {
#line 394 "src/taumir/lower.tr"
        long long ftg = _tag_of(m, ((HirField*)List_ptr_get(c->fields, fi))->ty);
#line 395 "src/taumir/lower.tr"
        if ((ftg < 0LL)) {
#line 396 "src/taumir/lower.tr"
            ok = false;
        }
#line 397 "src/taumir/lower.tr"
        ({ TrStr _ord_t6117 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _ord_t6117); _tr_str_release(_ord_t6117); });
#line 398 "src/taumir/lower.tr"
        List_i64_append(lay->ftags, ftg);
#line 399 "src/taumir/lower.tr"
        ({ TrStr _ord_t6118 = (_cls_of_ty(m, _subst_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty))); List_TrStr_append(lay->fcls, _ord_t6118); _tr_str_release(_ord_t6118); });
#line 400 "src/taumir/lower.tr"
        fi = (fi + 1LL);
    }
#line 401 "src/taumir/lower.tr"
    if (ok) {
#line 402 "src/taumir/lower.tr"
        LModule_add_class(m, lay);
#line 405 "src/taumir/lower.tr"
        long long prereg = 0LL;
#line 406 "src/taumir/lower.tr"
        while ((prereg < m->hir_prog->functions->len)) {
#line 407 "src/taumir/lower.tr"
            HirFunction* pf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, prereg));
#line 408 "src/taumir/lower.tr"
            if (((_tr_str_eqv((pf->class_name), (ty->name)) && (!pf->is_extern)) && (pf->generics->len == 0LL))) {
#line 409 "src/taumir/lower.tr"
                TrStr pfmang = ({ __auto_type _ord_t6119 = (({ TrStr _tr_cl = (_own(mangled)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6119); TrStr _tr_cr = (_own(pf->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 410 "src/taumir/lower.tr"
                long long pfrt = _tag_of(m, pf->ret_ty);
#line 411 "src/taumir/lower.tr"
                List_TrStr_append(m->fn_names, pfmang);
#line 412 "src/taumir/lower.tr"
                List_i64_append(m->fn_ret, pfrt);
#line 413 "src/taumir/lower.tr"
                if ((pf->returns_owned && ((pfrt == 10LL) || (pfrt == 11LL)))) {
#line 413 "src/taumir/lower.tr"
                    LModule_mark_fn_owned(m, pfmang);
                }
            }
#line 414 "src/taumir/lower.tr"
            prereg = (prereg + 1LL);
        }
#line 415 "src/taumir/lower.tr"
        long long prereg2 = 0LL;
#line 416 "src/taumir/lower.tr"
        while ((prereg2 < c->methods->len)) {
#line 417 "src/taumir/lower.tr"
            HirFunction* pfm = ((HirFunction*)List_ptr_get(c->methods, prereg2));
#line 418 "src/taumir/lower.tr"
            if ((((!pfm->is_extern) && (pfm->generics->len == 0LL)) && (!_method_in_prog_functions(m->hir_prog, ty->name, pfm->name)))) {
#line 419 "src/taumir/lower.tr"
                TrStr pfmang2 = ({ __auto_type _ord_t6120 = (({ TrStr _tr_cl = (_own(mangled)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6120); TrStr _tr_cr = (_own(pfm->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 420 "src/taumir/lower.tr"
                long long pfrt2 = _tag_of(m, pfm->ret_ty);
#line 421 "src/taumir/lower.tr"
                List_TrStr_append(m->fn_names, pfmang2);
#line 422 "src/taumir/lower.tr"
                List_i64_append(m->fn_ret, pfrt2);
#line 423 "src/taumir/lower.tr"
                if ((pfm->returns_owned && ((pfrt2 == 10LL) || (pfrt2 == 11LL)))) {
#line 423 "src/taumir/lower.tr"
                    LModule_mark_fn_owned(m, pfmang2);
                }
            }
#line 424 "src/taumir/lower.tr"
            prereg2 = (prereg2 + 1LL);
        }
#line 425 "src/taumir/lower.tr"
        long long mi = 0LL;
#line 426 "src/taumir/lower.tr"
        while ((mi < m->hir_prog->functions->len)) {
#line 427 "src/taumir/lower.tr"
            HirFunction* mf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, mi));
#line 428 "src/taumir/lower.tr"
            if (((_tr_str_eqv((mf->class_name), (ty->name)) && (!mf->is_extern)) && (mf->generics->len == 0LL))) {
#line 429 "src/taumir/lower.tr"
                _lir_lower_method(m, mangled, mf);
#line 430 "src/taumir/lower.tr"
                if ((!m->ok)) {
#line 432 "src/taumir/lower.tr"
                    ({ __auto_type _ord_t6121 = (({ TrStr _tr_cl = (_own(mangled)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t6122 = (({ TrStr _tr_cl = (_ord_t6121); TrStr _tr_cr = (_own(mf->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(m->unavail_names, _ord_t6122); _tr_str_release(_ord_t6122); });
#line 433 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6123 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _ord_t6123); _tr_str_release(_ord_t6123); });
#line 434 "src/taumir/lower.tr"
                    m->ok = true;
#line 435 "src/taumir/lower.tr"
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
#line 436 "src/taumir/lower.tr"
            mi = (mi + 1LL);
        }
#line 437 "src/taumir/lower.tr"
        long long mi2 = 0LL;
#line 438 "src/taumir/lower.tr"
        while ((mi2 < c->methods->len)) {
#line 439 "src/taumir/lower.tr"
            HirFunction* mfm = ((HirFunction*)List_ptr_get(c->methods, mi2));
#line 440 "src/taumir/lower.tr"
            if ((((!mfm->is_extern) && (mfm->generics->len == 0LL)) && (!_method_in_prog_functions(m->hir_prog, ty->name, mfm->name)))) {
#line 441 "src/taumir/lower.tr"
                _lir_lower_method(m, mangled, mfm);
#line 442 "src/taumir/lower.tr"
                if ((!m->ok)) {
#line 443 "src/taumir/lower.tr"
                    ({ __auto_type _ord_t6124 = (({ TrStr _tr_cl = (_own(mangled)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t6125 = (({ TrStr _tr_cl = (_ord_t6124); TrStr _tr_cr = (_own(mfm->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(m->unavail_names, _ord_t6125); _tr_str_release(_ord_t6125); });
#line 444 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6126 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _ord_t6126); _tr_str_release(_ord_t6126); });
#line 445 "src/taumir/lower.tr"
                    m->ok = true;
#line 446 "src/taumir/lower.tr"
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
#line 447 "src/taumir/lower.tr"
            mi2 = (mi2 + 1LL);
        }
    }
#line 449 "src/taumir/lower.tr"
    sb = 0LL;
#line 450 "src/taumir/lower.tr"
    while ((sb < c->generics->len)) {
#line 451 "src/taumir/lower.tr"
        List_TrStr_pop(m->subst_names);
#line 452 "src/taumir/lower.tr"
        List_ptr_pop(m->subst_tys);
#line 453 "src/taumir/lower.tr"
        sb = (sb + 1LL);
    }
#line 454 "src/taumir/lower.tr"
    if ((!ok)) {
#line 454 "src/taumir/lower.tr"
        _tr_str_release(mangled);
        _tr_obj_release(lay, _trdrop_ClassLayout);
        return _tr_str_lit_len("", 0LL);
    }
#line 455 "src/taumir/lower.tr"
    TrStr _trret1494 = _tr_str_retain(mangled);
    _tr_str_release(mangled);
    _tr_obj_release(lay, _trdrop_ClassLayout);
    return _trret1494;
}

__attribute__((hot)) TrStr _val_obj_cls(LModule* m, LFunc* lf, HirExpr* val) {
#line 462 "src/taumir/lower.tr"
    AstType* t = hir_expr_type(val);
#line 463 "src/taumir/lower.tr"
    if (_is_null_str(t->name)) {
#line 463 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 464 "src/taumir/lower.tr"
    if ((LModule_is_class(m, t->name) || LModule_is_enum(m, t->name))) {
#line 464 "src/taumir/lower.tr"
        TrStr _trret1495 = _own(t->name);
        return _trret1495;
    }
#line 465 "src/taumir/lower.tr"
    if ((_prog_generic_class_index(m, t->name) < 0LL)) {
#line 465 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 466 "src/taumir/lower.tr"
    AstType* ct = ({ TrStr _at_t6127 = (_own(t->name)); __auto_type _wr = (AstType_init(_at_t6127)); _tr_str_release(_at_t6127); _wr; });
#line 467 "src/taumir/lower.tr"
    if ((t->args->len > 0LL)) {
#line 468 "src/taumir/lower.tr"
        long long ai = 0LL;
#line 469 "src/taumir/lower.tr"
        while ((ai < t->args->len)) {
#line 470 "src/taumir/lower.tr"
            ({ __auto_type _ord_t6128 = (box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(t->args, ai)))))); List_ptr_append(ct->args, _ord_t6128); });
#line 471 "src/taumir/lower.tr"
            ai = (ai + 1LL);
        }
    } else if ((m->subst_names->len > 0LL)) {
#line 473 "src/taumir/lower.tr"
        HirClass* gc = ({ __auto_type _ord_t6129 = (_prog_generic_class_index(m, t->name)); __auto_type _wr = (((HirClass*)List_ptr_get(m->hir_prog->classes, _ord_t6129))); _wr; });
#line 474 "src/taumir/lower.tr"
        long long gi = 0LL;
#line 475 "src/taumir/lower.tr"
        while ((gi < gc->generics->len)) {
#line 476 "src/taumir/lower.tr"
            ({ TrStr _at_t6130 = (List_TrStr_get(gc->generics, gi)); TrStr _at_t6131 = (_own(_at_t6130)); __auto_type _ord_t6132 = (box_asttype_lir(_subst_ty(m, AstType_init(_at_t6131)))); List_ptr_append(ct->args, _ord_t6132); _tr_str_release(_at_t6130); _tr_str_release(_at_t6131); });
#line 477 "src/taumir/lower.tr"
            gi = (gi + 1LL);
        }
    }
#line 478 "src/taumir/lower.tr"
    if ((ct->args->len == 0LL)) {
#line 478 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 479 "src/taumir/lower.tr"
    TrStr _trret1496 = _ensure_generic_class(m, ct);
    return _trret1496;
}

__attribute__((hot)) TrStr _cls_of_ty(LModule* m, AstType* ty) {
#line 483 "src/taumir/lower.tr"
    if (_is_null_str(ty->name)) {
#line 483 "src/taumir/lower.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 484 "src/taumir/lower.tr"
    if ((LModule_is_class(m, ty->name) || LModule_is_enum(m, ty->name))) {
#line 484 "src/taumir/lower.tr"
        return _own(ty->name);
    }
#line 485 "src/taumir/lower.tr"
    if (((ty->args->len > 0LL) && (_prog_generic_class_index(m, ty->name) >= 0LL))) {
#line 486 "src/taumir/lower.tr"
        TrStr gm = _ensure_generic_class(m, ty);
#line 487 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((gm), (_tr_str_lit_len("", 0LL))))) {
#line 487 "src/taumir/lower.tr"
            TrStr _trret1497 = _tr_str_retain(gm);
            _tr_str_release(gm);
            return _trret1497;
        }
    }
#line 488 "src/taumir/lower.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _recv_class(LModule* m, LFunc* lf, HirExpr* obj) {
#line 494 "src/taumir/lower.tr"
    TrStr cn = _tr_str_retain(hir_expr_type(obj)->name);
#line 495 "src/taumir/lower.tr"
    if ((_tr_str_eqv((cn), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((cn), (_tr_str_lit_len("Map", 3LL))))) {
#line 495 "src/taumir/lower.tr"
        _tr_str_release(cn);
        return _tr_str_lit_len("", 0LL);
    }
#line 496 "src/taumir/lower.tr"
    if (((!_is_null_str(cn)) && (LModule_is_class(m, cn) || LModule_is_enum(m, cn)))) {
#line 496 "src/taumir/lower.tr"
        TrStr _trret1498 = _tr_str_retain(cn);
        _tr_str_release(cn);
        return _trret1498;
    }
#line 497 "src/taumir/lower.tr"
    if ((((!_is_null_str(cn)) && (hir_expr_type(obj)->args->len > 0LL)) && (_prog_generic_class_index(m, cn) >= 0LL))) {
#line 498 "src/taumir/lower.tr"
        TrStr grc = _ensure_generic_class(m, hir_expr_type(obj));
#line 499 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((grc), (_tr_str_lit_len("", 0LL))))) {
#line 499 "src/taumir/lower.tr"
            TrStr _trret1499 = _tr_str_retain(grc);
            _tr_str_release(cn);
            _tr_str_release(grc);
            return _trret1499;
        }
    }
#line 500 "src/taumir/lower.tr"
    __auto_type _t6133 = (*obj);
    if (_t6133.tag == HirExpr_EIdent) {
        __auto_type nm = _t6133.data.EIdent.name;
#line 502 "src/taumir/lower.tr"
        TrStr vc = LFunc_var_cls_of(lf, nm);
#line 503 "src/taumir/lower.tr"
        if (((!_is_null_str(vc)) && (LModule_is_class(m, vc) || LModule_is_enum(m, vc)))) {
#line 503 "src/taumir/lower.tr"
            TrStr _trret1500 = _tr_str_retain(vc);
            _tr_str_release(vc);
            _tr_str_release(cn);
            return _trret1500;
        }
        _tr_str_release(vc);
    } else if (_t6133.tag == HirExpr_EPropAccess) {
        __auto_type inner = _t6133.data.EPropAccess.obj;
__auto_type prop = _t6133.data.EPropAccess.prop;
#line 505 "src/taumir/lower.tr"
        TrStr icls = _recv_class(m, lf, inner);
#line 506 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((icls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, icls))) {
#line 507 "src/taumir/lower.tr"
            TrStr fc = LModule_field_cls(m, icls, prop);
#line 508 "src/taumir/lower.tr"
            if (((!_is_null_str(fc)) && (!_tr_str_eqv((fc), (_tr_str_lit_len("", 0LL)))))) {
#line 508 "src/taumir/lower.tr"
                TrStr _trret1501 = _tr_str_retain(fc);
                _tr_str_release(icls);
                _tr_str_release(fc);
                _tr_str_release(cn);
                return _trret1501;
            }
        }
        _tr_str_release(icls);
    } else if (1) {
        __auto_type _ = _t6133;
#line 510 "src/taumir/lower.tr"
        /* pass */
    }
#line 511 "src/taumir/lower.tr"
    _tr_str_release(cn);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _lower_list_comp(LModule* m, LFunc* lf, HirExpr* element, List_ptr* generators, AstType* lty) {
#line 517 "src/taumir/lower.tr"
    if ((generators->len != 1LL)) {
#line 517 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 518 "src/taumir/lower.tr"
    HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, 0LL)));
#line 519 "src/taumir/lower.tr"
    if (gen->is_async) {
#line 519 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 520 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
#line 521 "src/taumir/lower.tr"
    long long hv = LFunc_new_vreg(lf);
#line 522 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
#line 523 "src/taumir/lower.tr"
    long long ltag = _tag_of(m, lty);
#line 524 "src/taumir/lower.tr"
    if ((!_is_list_tag(ltag))) {
#line 525 "src/taumir/lower.tr"
        long long et = _tag_of(m, hir_expr_type(element));
#line 526 "src/taumir/lower.tr"
        if ((et == 4LL)) {
#line 526 "src/taumir/lower.tr"
            et = 0LL;
        }
#line 527 "src/taumir/lower.tr"
        ltag = _list_tag_for_elem(et);
#line 528 "src/taumir/lower.tr"
        if ((!_is_list_tag(ltag))) {
#line 528 "src/taumir/lower.tr"
            return (-1LL);
        }
    }
#line 529 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, hv, ltag);
#line 530 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 531 "src/taumir/lower.tr"
    TrStr lcname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__lc", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 532 "src/taumir/lower.tr"
    LFunc_add_var(lf, lcname);
#line 533 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, lcname, ltag);
#line 534 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(lcname, hv));
#line 536 "src/taumir/lower.tr"
    HirExpr* _tr_v_recv = ({ TrStr _at_t6134 = (_own(lcname)); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t6134, lty, false))); _tr_str_release(_at_t6134); _wr; });
#line 537 "src/taumir/lower.tr"
    List_ptr* aargs = (void*)List_ptr_new();
#line 538 "src/taumir/lower.tr"
    List_ptr_append(aargs, element);
#line 539 "src/taumir/lower.tr"
    HirStmt* appstmt = box_hirstmt(HirStmt_ctor_SExpr(box_hirexpr(HirExpr_ctor_EMethodCall(_tr_v_recv, _tr_str_lit_len("append", 6LL), aargs, AstType_init(_tr_str_lit_len("void", 4LL))))));
#line 540 "src/taumir/lower.tr"
    HirBlock* body = HirBlock_init();
#line 541 "src/taumir/lower.tr"
    if ((gen->ifs->len == 0LL)) {
#line 542 "src/taumir/lower.tr"
        List_ptr_append(body->stmts, appstmt);
    } else {
#line 544 "src/taumir/lower.tr"
        HirExpr* cond = ((HirExpr*)List_ptr_get(gen->ifs, 0LL));
#line 545 "src/taumir/lower.tr"
        long long ci = 1LL;
#line 546 "src/taumir/lower.tr"
        while ((ci < gen->ifs->len)) {
#line 547 "src/taumir/lower.tr"
            cond = ({ __auto_type _ord_t6135 = (((HirExpr*)List_ptr_get(gen->ifs, ci))); __auto_type _ord_t6136 = (AstType_init(_tr_str_lit_len("bool", 4LL))); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), cond, _ord_t6135, _ord_t6136))); _wr; });
#line 548 "src/taumir/lower.tr"
            ci = (ci + 1LL);
        }
#line 549 "src/taumir/lower.tr"
        HirBlock* thenb = HirBlock_init();
#line 550 "src/taumir/lower.tr"
        List_ptr_append(thenb->stmts, appstmt);
#line 551 "src/taumir/lower.tr"
        ({ HirBlock* _aot_t6137 = (HirBlock_init()); __auto_type _ord_t6138 = (box_hirstmt(HirStmt_ctor_SIf(cond, thenb, _aot_t6137))); List_ptr_append(body->stmts, _ord_t6138); _tr_obj_release(_aot_t6137, _trdrop_HirBlock); });
    }
#line 552 "src/taumir/lower.tr"
    if (({ TrStr _at_t6139 = (_own(gen->target)); __auto_type _wr = ((!_lower_for(m, lf, _at_t6139, gen->iter, body))); _tr_str_release(_at_t6139); _wr; })) {
#line 552 "src/taumir/lower.tr"
        _tr_str_release(lcname);
        _tr_obj_release(body, _trdrop_HirBlock);
        return (-1LL);
    }
#line 553 "src/taumir/lower.tr"
    long long res = LFunc_new_vreg(lf);
#line 554 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(res, lcname));
#line 555 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, res, ltag);
#line 556 "src/taumir/lower.tr"
    long long _trret1502 = res;
    _tr_str_release(lcname);
    _tr_obj_release(body, _trdrop_HirBlock);
    return _trret1502;
}

__attribute__((hot)) AstType* _hir_method_ret_ty(LModule* m, TrStr cls, TrStr method) {
#line 561 "src/taumir/lower.tr"
    long long i = 0LL;
#line 562 "src/taumir/lower.tr"
    while ((i < m->hir_prog->functions->len)) {
#line 563 "src/taumir/lower.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
#line 564 "src/taumir/lower.tr"
        if (((_tr_str_eqv((f->class_name), (cls)) && _tr_str_eqv((f->name), (method))) && (!f->is_extern))) {
#line 565 "src/taumir/lower.tr"
            return f->ret_ty;
        }
#line 566 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 567 "src/taumir/lower.tr"
    long long ci = 0LL;
#line 568 "src/taumir/lower.tr"
    while ((ci < m->hir_prog->classes->len)) {
#line 569 "src/taumir/lower.tr"
        HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, ci));
#line 570 "src/taumir/lower.tr"
        if (_tr_str_eqv((c->name), (cls))) {
#line 571 "src/taumir/lower.tr"
            long long mi = 0LL;
#line 572 "src/taumir/lower.tr"
            while ((mi < c->methods->len)) {
#line 573 "src/taumir/lower.tr"
                if (_tr_str_eqv((((HirFunction*)List_ptr_get(c->methods, mi))->name), (method))) {
#line 573 "src/taumir/lower.tr"
                    return ((HirFunction*)List_ptr_get(c->methods, mi))->ret_ty;
                }
#line 574 "src/taumir/lower.tr"
                mi = (mi + 1LL);
            }
        }
#line 575 "src/taumir/lower.tr"
        ci = (ci + 1LL);
    }
#line 576 "src/taumir/lower.tr"
    return AstType_init(_tr_str_lit_len("", 0LL));
}

__attribute__((hot)) long long _prog_class_index(HirProgram* prog, TrStr name) {
#line 580 "src/taumir/lower.tr"
    if (_is_null_str(name)) {
#line 580 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 581 "src/taumir/lower.tr"
    long long i = 0LL;
#line 582 "src/taumir/lower.tr"
    while ((i < prog->classes->len)) {
#line 583 "src/taumir/lower.tr"
        if (_tr_str_eqv((((HirClass*)List_ptr_get(prog->classes, i))->name), (name))) {
#line 583 "src/taumir/lower.tr"
            return i;
        }
#line 584 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 585 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _push_field_names_rec(HirProgram* prog, long long ci, ClassLayout* lay, long long depth) {
#line 592 "src/taumir/lower.tr"
    if ((depth > 32LL)) {
#line 592 "src/taumir/lower.tr"
        return false;
    }
#line 593 "src/taumir/lower.tr"
    HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
#line 594 "src/taumir/lower.tr"
    if ((c->base_classes->len > 1LL)) {
#line 594 "src/taumir/lower.tr"
        return false;
    }
#line 595 "src/taumir/lower.tr"
    if ((c->base_classes->len == 1LL)) {
#line 596 "src/taumir/lower.tr"
        long long bi = ({ TrStr _at_t6140 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t6140)); _tr_str_release(_at_t6140); _wr; });
#line 597 "src/taumir/lower.tr"
        if ((bi < 0LL)) {
#line 597 "src/taumir/lower.tr"
            return false;
        }
#line 598 "src/taumir/lower.tr"
        if ((!_push_field_names_rec(prog, bi, lay, (depth + 1LL)))) {
#line 598 "src/taumir/lower.tr"
            return false;
        }
    }
#line 599 "src/taumir/lower.tr"
    long long fi = 0LL;
#line 600 "src/taumir/lower.tr"
    while ((fi < c->fields->len)) {
#line 601 "src/taumir/lower.tr"
        ({ TrStr _ord_t6141 = (_own(((HirField*)List_ptr_get(c->fields, fi))->name)); List_TrStr_append(lay->fields, _ord_t6141); _tr_str_release(_ord_t6141); });
#line 602 "src/taumir/lower.tr"
        fi = (fi + 1LL);
    }
#line 603 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _push_field_tags_rec(LModule* m, HirProgram* prog, long long ci, ClassLayout* lay, long long depth) {
#line 608 "src/taumir/lower.tr"
    if ((depth > 32LL)) {
#line 608 "src/taumir/lower.tr"
        return false;
    }
#line 609 "src/taumir/lower.tr"
    HirClass* c = ((HirClass*)List_ptr_get(prog->classes, ci));
#line 610 "src/taumir/lower.tr"
    if ((c->base_classes->len > 1LL)) {
#line 610 "src/taumir/lower.tr"
        return false;
    }
#line 611 "src/taumir/lower.tr"
    if ((c->base_classes->len == 1LL)) {
#line 612 "src/taumir/lower.tr"
        long long bi = ({ TrStr _at_t6142 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_prog_class_index(prog, _at_t6142)); _tr_str_release(_at_t6142); _wr; });
#line 613 "src/taumir/lower.tr"
        if ((bi < 0LL)) {
#line 613 "src/taumir/lower.tr"
            return false;
        }
#line 614 "src/taumir/lower.tr"
        if ((!_push_field_tags_rec(m, prog, bi, lay, (depth + 1LL)))) {
#line 614 "src/taumir/lower.tr"
            return false;
        }
    }
#line 615 "src/taumir/lower.tr"
    long long fi = 0LL;
#line 616 "src/taumir/lower.tr"
    while ((fi < c->fields->len)) {
#line 617 "src/taumir/lower.tr"
        ({ __auto_type _ord_t6143 = (_tag_of(m, ((HirField*)List_ptr_get(c->fields, fi))->ty)); List_i64_append(lay->ftags, _ord_t6143); });
#line 618 "src/taumir/lower.tr"
        ({ TrStr _ord_t6144 = (_cls_of_ty(m, ((HirField*)List_ptr_get(c->fields, fi))->ty)); List_TrStr_append(lay->fcls, _ord_t6144); _tr_str_release(_ord_t6144); });
#line 619 "src/taumir/lower.tr"
        fi = (fi + 1LL);
    }
#line 620 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) void _register_classes(LModule* m, HirProgram* prog) {
#line 630 "src/taumir/lower.tr"
    EnumLayout* opt = EnumLayout_init(_tr_str_lit_len("Option", 6LL));
#line 631 "src/taumir/lower.tr"
    VariantLayout* opt_some = VariantLayout_init(_tr_str_lit_len("Some", 4LL));
#line 632 "src/taumir/lower.tr"
    List_TrStr_append(opt_some->fields, _tr_str_lit_len("val", 3LL));
#line 633 "src/taumir/lower.tr"
    List_i64_append(opt_some->ftags, (0LL - 2LL));
#line 634 "src/taumir/lower.tr"
    List_TrStr_append(opt_some->fcls, _tr_str_lit_len("", 0LL));
#line 635 "src/taumir/lower.tr"
    List_ptr_append(opt->variants, _tr_obj_retain(opt_some));
#line 636 "src/taumir/lower.tr"
    ({ VariantLayout* _ord_t6145 = (VariantLayout_init(_tr_str_lit_len("None", 4LL))); List_ptr_append(opt->variants, _tr_obj_retain(_ord_t6145)); _tr_obj_release(_ord_t6145, _trdrop_VariantLayout); });
#line 637 "src/taumir/lower.tr"
    LModule_add_enum(m, opt);
#line 638 "src/taumir/lower.tr"
    EnumLayout* res = EnumLayout_init(_tr_str_lit_len("Result", 6LL));
#line 639 "src/taumir/lower.tr"
    VariantLayout* res_ok = VariantLayout_init(_tr_str_lit_len("Ok", 2LL));
#line 640 "src/taumir/lower.tr"
    List_TrStr_append(res_ok->fields, _tr_str_lit_len("val", 3LL));
#line 641 "src/taumir/lower.tr"
    List_i64_append(res_ok->ftags, (0LL - 2LL));
#line 642 "src/taumir/lower.tr"
    List_TrStr_append(res_ok->fcls, _tr_str_lit_len("", 0LL));
#line 643 "src/taumir/lower.tr"
    List_ptr_append(res->variants, _tr_obj_retain(res_ok));
#line 644 "src/taumir/lower.tr"
    VariantLayout* res_err = VariantLayout_init(_tr_str_lit_len("Err", 3LL));
#line 645 "src/taumir/lower.tr"
    List_TrStr_append(res_err->fields, _tr_str_lit_len("err", 3LL));
#line 646 "src/taumir/lower.tr"
    List_i64_append(res_err->ftags, (0LL - 3LL));
#line 647 "src/taumir/lower.tr"
    List_TrStr_append(res_err->fcls, _tr_str_lit_len("", 0LL));
#line 648 "src/taumir/lower.tr"
    List_ptr_append(res->variants, _tr_obj_retain(res_err));
#line 649 "src/taumir/lower.tr"
    LModule_add_enum(m, res);
#line 652 "src/taumir/lower.tr"
    long long ifi = 0LL;
#line 653 "src/taumir/lower.tr"
    while ((ifi < prog->interfaces->len)) {
#line 654 "src/taumir/lower.tr"
        ({ TrStr _at_t6146 = (_own(((HirInterface*)List_ptr_get(prog->interfaces, ifi))->name)); LModule_add_iface(m, _at_t6146); _tr_str_release(_at_t6146); });
#line 655 "src/taumir/lower.tr"
        ifi = (ifi + 1LL);
    }
#line 656 "src/taumir/lower.tr"
    long long i = 0LL;
#line 657 "src/taumir/lower.tr"
    while ((i < prog->classes->len)) {
#line 658 "src/taumir/lower.tr"
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
#line 659 "src/taumir/lower.tr"
        if ((c->generics->len > 0LL)) {
#line 660 "src/taumir/lower.tr"
            i = (i + 1LL);
#line 661 "src/taumir/lower.tr"
            continue;
        }
#line 662 "src/taumir/lower.tr"
        ClassLayout* lay = ({ TrStr _at_t6147 = (_own(c->name)); __auto_type _wr = (ClassLayout_init(_at_t6147)); _tr_str_release(_at_t6147); _wr; });
#line 663 "src/taumir/lower.tr"
        if ((c->base_classes->len == 1LL)) {
#line 663 "src/taumir/lower.tr"
            lay->base = ({ TrStr _at_t6148 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_own(_at_t6148)); _tr_str_release(_at_t6148); _wr; });
        }
#line 664 "src/taumir/lower.tr"
        lay->is_value = (!c->is_class);
#line 665 "src/taumir/lower.tr"
        if (_push_field_names_rec(prog, i, lay, 0LL)) {
#line 666 "src/taumir/lower.tr"
            LModule_add_class(m, lay);
        }
#line 667 "src/taumir/lower.tr"
        i = (i + 1LL);
        _tr_obj_release(lay, _trdrop_ClassLayout);
    }
#line 668 "src/taumir/lower.tr"
    long long ei = 0LL;
#line 669 "src/taumir/lower.tr"
    while ((ei < prog->enums->len)) {
#line 670 "src/taumir/lower.tr"
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, ei));
#line 671 "src/taumir/lower.tr"
        EnumLayout* elay = ({ TrStr _at_t6149 = (_own(e->name)); __auto_type _wr = (EnumLayout_init(_at_t6149)); _tr_str_release(_at_t6149); _wr; });
#line 672 "src/taumir/lower.tr"
        long long vi = 0LL;
#line 673 "src/taumir/lower.tr"
        while ((vi < e->variants->len)) {
#line 674 "src/taumir/lower.tr"
            HirVariant* v = ((HirVariant*)List_ptr_get(e->variants, vi));
#line 675 "src/taumir/lower.tr"
            VariantLayout* vlay = ({ TrStr _at_t6150 = (_own(v->name)); __auto_type _wr = (VariantLayout_init(_at_t6150)); _tr_str_release(_at_t6150); _wr; });
#line 676 "src/taumir/lower.tr"
            long long pf = 0LL;
#line 677 "src/taumir/lower.tr"
            while ((pf < v->fields->len)) {
#line 678 "src/taumir/lower.tr"
                ({ TrStr _ord_t6151 = (_own(((HirParam*)List_ptr_get(v->fields, pf))->name)); List_TrStr_append(vlay->fields, _ord_t6151); _tr_str_release(_ord_t6151); });
#line 679 "src/taumir/lower.tr"
                pf = (pf + 1LL);
            }
#line 680 "src/taumir/lower.tr"
            List_ptr_append(elay->variants, _tr_obj_retain(vlay));
#line 681 "src/taumir/lower.tr"
            vi = (vi + 1LL);
            _tr_obj_release(vlay, _trdrop_VariantLayout);
        }
#line 682 "src/taumir/lower.tr"
        LModule_add_enum(m, elay);
#line 683 "src/taumir/lower.tr"
        ei = (ei + 1LL);
        _tr_obj_release(elay, _trdrop_EnumLayout);
    }
#line 686 "src/taumir/lower.tr"
    i = 0LL;
#line 687 "src/taumir/lower.tr"
    while ((i < prog->classes->len)) {
#line 688 "src/taumir/lower.tr"
        HirClass* c2 = ((HirClass*)List_ptr_get(prog->classes, i));
#line 689 "src/taumir/lower.tr"
        long long li = LModule_class_index(m, c2->name);
#line 690 "src/taumir/lower.tr"
        if ((li >= 0LL)) {
#line 691 "src/taumir/lower.tr"
            ClassLayout* lay2 = ((ClassLayout*)List_ptr_get(m->classes, li));
#line 692 "src/taumir/lower.tr"
            if ((!_push_field_tags_rec(m, prog, i, lay2, 0LL))) {
#line 693 "src/taumir/lower.tr"
                m->ok = false;
#line 694 "src/taumir/lower.tr"
                m->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(((HirClass*)List_ptr_get(prog->classes, i))->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("class '", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': field-tag registration diverged", 34LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 695 "src/taumir/lower.tr"
                _tr_obj_release(opt, _trdrop_EnumLayout);
                _tr_obj_release(opt_some, _trdrop_VariantLayout);
                _tr_obj_release(res, _trdrop_EnumLayout);
                _tr_obj_release(res_ok, _trdrop_VariantLayout);
                _tr_obj_release(res_err, _trdrop_VariantLayout);
                return;
            }
        }
#line 696 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 697 "src/taumir/lower.tr"
    ei = 0LL;
#line 698 "src/taumir/lower.tr"
    while ((ei < prog->enums->len)) {
#line 699 "src/taumir/lower.tr"
        HirEnum* e2 = ((HirEnum*)List_ptr_get(prog->enums, ei));
#line 702 "src/taumir/lower.tr"
        long long e2ix = LModule_enum_index(m, e2->name);
#line 703 "src/taumir/lower.tr"
        if ((e2ix < 0LL)) {
#line 704 "src/taumir/lower.tr"
            ei = (ei + 1LL);
#line 705 "src/taumir/lower.tr"
            continue;
        }
#line 706 "src/taumir/lower.tr"
        EnumLayout* elay2 = ((EnumLayout*)List_ptr_get(m->enums, e2ix));
#line 707 "src/taumir/lower.tr"
        long long v2 = 0LL;
#line 708 "src/taumir/lower.tr"
        while ((v2 < e2->variants->len)) {
#line 709 "src/taumir/lower.tr"
            HirVariant* vdef = ((HirVariant*)List_ptr_get(e2->variants, v2));
#line 710 "src/taumir/lower.tr"
            VariantLayout* vlay2 = ((VariantLayout*)List_ptr_get(elay2->variants, v2));
#line 711 "src/taumir/lower.tr"
            long long p2 = 0LL;
#line 712 "src/taumir/lower.tr"
            while ((p2 < vdef->fields->len)) {
#line 717 "src/taumir/lower.tr"
                AstType* p2ty = ((HirParam*)List_ptr_get(vdef->fields, p2))->ty;
#line 718 "src/taumir/lower.tr"
                long long p2gi = (-1LL);
#line 719 "src/taumir/lower.tr"
                if ((!_is_null_str(p2ty->name))) {
#line 720 "src/taumir/lower.tr"
                    long long egk = 0LL;
#line 721 "src/taumir/lower.tr"
                    while ((egk < e2->generics->len)) {
#line 722 "src/taumir/lower.tr"
                        if (({ __auto_type _ord_t6152 = (List_TrStr_get(e2->generics, egk)); __auto_type _wr = (({ TrStr _cmpa_t6153 = (_ord_t6152); TrStr _cmpb_t6154 = (p2ty->name); _Bool _cmpr_t6155 = _tr_str_eqv(_cmpa_t6153, _cmpb_t6154); _tr_str_release(_cmpa_t6153); _cmpr_t6155; })); _wr; })) {
#line 722 "src/taumir/lower.tr"
                            p2gi = egk;
                        }
#line 723 "src/taumir/lower.tr"
                        egk = (egk + 1LL);
                    }
                }
#line 724 "src/taumir/lower.tr"
                if ((p2gi >= 0LL)) {
#line 725 "src/taumir/lower.tr"
                    List_i64_append(vlay2->ftags, (0LL - (2LL + p2gi)));
#line 726 "src/taumir/lower.tr"
                    List_TrStr_append(vlay2->fcls, _tr_str_lit_len("", 0LL));
                } else {
#line 728 "src/taumir/lower.tr"
                    ({ __auto_type _ord_t6156 = (_tag_of(m, p2ty)); List_i64_append(vlay2->ftags, _ord_t6156); });
#line 729 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6157 = (_cls_of_ty(m, p2ty)); List_TrStr_append(vlay2->fcls, _ord_t6157); _tr_str_release(_ord_t6157); });
                }
#line 730 "src/taumir/lower.tr"
                p2 = (p2 + 1LL);
            }
#line 731 "src/taumir/lower.tr"
            v2 = (v2 + 1LL);
        }
#line 732 "src/taumir/lower.tr"
        ei = (ei + 1LL);
    }
    _tr_obj_release(opt, _trdrop_EnumLayout);
    _tr_obj_release(opt_some, _trdrop_VariantLayout);
    _tr_obj_release(res, _trdrop_EnumLayout);
    _tr_obj_release(res_ok, _trdrop_VariantLayout);
    _tr_obj_release(res_err, _trdrop_VariantLayout);
}

__attribute__((hot)) bool _class_needs_drop(ClassLayout* lay) {
#line 741 "src/taumir/lower.tr"
    long long i = 0LL;
#line 742 "src/taumir/lower.tr"
    while ((i < lay->ftags->len)) {
#line 743 "src/taumir/lower.tr"
        long long t = List_i64_get(lay->ftags, i);
#line 744 "src/taumir/lower.tr"
        if ((((t == 1LL) || (t == 10LL)) || (t == 11LL))) {
#line 744 "src/taumir/lower.tr"
            return true;
        }
#line 745 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 746 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _class_needs_drop_by_name(LModule* m, TrStr cname) {
#line 749 "src/taumir/lower.tr"
    long long ci = LModule_class_index(m, cname);
#line 750 "src/taumir/lower.tr"
    if ((ci < 0LL)) {
#line 750 "src/taumir/lower.tr"
        return false;
    }
#line 751 "src/taumir/lower.tr"
    return _class_needs_drop(((ClassLayout*)List_ptr_get(m->classes, ci)));
}

__attribute__((hot)) void _attach_class_drop(LModule* m, LFunc* lf, long long obj, TrStr cname) {
#line 756 "src/taumir/lower.tr"
    if ((!_class_needs_drop_by_name(m, cname))) {
#line 756 "src/taumir/lower.tr"
        return;
    }
#line 757 "src/taumir/lower.tr"
    long long faddr = LFunc_new_vreg(lf);
#line 758 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, faddr, 12LL);
#line 759 "src/taumir/lower.tr"
    ({ TrStr _at_t6158 = (_tr_strx_concatv((cname), (_tr_str_lit_len("__drop", 6LL)))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t6158)); _tr_str_release(_at_t6158); });
#line 760 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_set_drop", 19LL));
#line 761 "src/taumir/lower.tr"
    List_i64* sda = (void*)List_i64_new();
#line 762 "src/taumir/lower.tr"
    List_i64_append(sda, obj);
#line 763 "src/taumir/lower.tr"
    List_i64_append(sda, faddr);
#line 764 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_set_drop", 19LL), sda));
}

__attribute__((hot)) void _build_class_drop(LModule* m, ClassLayout* lay) {
#line 769 "src/taumir/lower.tr"
    TrStr dname = _tr_strx_concatv((lay->name), (_tr_str_lit_len("__drop", 6LL)));
#line 770 "src/taumir/lower.tr"
    if (LModule_is_user_fn(m, dname)) {
#line 770 "src/taumir/lower.tr"
        _tr_str_release(dname);
        return;
    }
#line 771 "src/taumir/lower.tr"
    List_TrStr_append(m->fn_names, dname);
#line 772 "src/taumir/lower.tr"
    List_i64_append(m->fn_ret, 0LL);
#line 773 "src/taumir/lower.tr"
    LFunc* lf = LFunc_init(dname);
#line 774 "src/taumir/lower.tr"
    List_TrStr_append(lf->params, _tr_str_lit_len("self", 4LL));
#line 775 "src/taumir/lower.tr"
    LFunc_add_var(lf, _tr_str_lit_len("self", 4LL));
#line 776 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, _tr_str_lit_len("self", 4LL), 10LL);
#line 777 "src/taumir/lower.tr"
    ({ TrStr _at_t6159 = (_own(lay->name)); LFunc_set_var_cls(lf, _tr_str_lit_len("self", 4LL), _at_t6159); _tr_str_release(_at_t6159); });
#line 778 "src/taumir/lower.tr"
    LFunc_set_cur(lf, LFunc_new_block(lf));
#line 779 "src/taumir/lower.tr"
    long long i = 0LL;
#line 780 "src/taumir/lower.tr"
    while ((i < lay->ftags->len)) {
#line 781 "src/taumir/lower.tr"
        long long t = List_i64_get(lay->ftags, i);
#line 782 "src/taumir/lower.tr"
        if ((t == 1LL)) {
#line 783 "src/taumir/lower.tr"
            long long s0 = LFunc_new_vreg(lf);
#line 784 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(s0, _tr_str_lit_len("self", 4LL)));
#line 785 "src/taumir/lower.tr"
            long long fv = _emit_field_get(m, lf, s0, (i * 8LL), 1LL);
#line 786 "src/taumir/lower.tr"
            _release_str(m, lf, fv);
        } else if (((t == 10LL) || (t == 11LL))) {
#line 788 "src/taumir/lower.tr"
            long long s1 = LFunc_new_vreg(lf);
#line 789 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(s1, _tr_str_lit_len("self", 4LL)));
#line 790 "src/taumir/lower.tr"
            long long fv2 = _emit_field_get(m, lf, s1, (i * 8LL), t);
#line 791 "src/taumir/lower.tr"
            _release_obj(m, lf, fv2);
        }
#line 792 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 793 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
#line 794 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_str_release(dname);
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) void _gen_class_drops(LModule* m) {
#line 798 "src/taumir/lower.tr"
    long long ci = 0LL;
#line 799 "src/taumir/lower.tr"
    while ((ci < m->classes->len)) {
#line 800 "src/taumir/lower.tr"
        ClassLayout* lay = ((ClassLayout*)List_ptr_get(m->classes, ci));
#line 801 "src/taumir/lower.tr"
        if (_class_needs_drop(lay)) {
#line 801 "src/taumir/lower.tr"
            _build_class_drop(m, lay);
        }
#line 802 "src/taumir/lower.tr"
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) bool _method_in_prog_functions(HirProgram* prog, TrStr cls, TrStr name) {
#line 807 "src/taumir/lower.tr"
    long long dfi = 0LL;
#line 808 "src/taumir/lower.tr"
    while ((dfi < prog->functions->len)) {
#line 809 "src/taumir/lower.tr"
        HirFunction* df = ((HirFunction*)List_ptr_get(prog->functions, dfi));
#line 810 "src/taumir/lower.tr"
        if (((_tr_str_eqv((df->class_name), (cls)) && _tr_str_eqv((df->name), (name))) && (!df->is_extern))) {
#line 810 "src/taumir/lower.tr"
            return true;
        }
#line 811 "src/taumir/lower.tr"
        dfi = (dfi + 1LL);
    }
#line 812 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) long long _method_nonself_pcount(HirFunction* f) {
#line 816 "src/taumir/lower.tr"
    long long c = 0LL;
#line 817 "src/taumir/lower.tr"
    long long i = 0LL;
#line 818 "src/taumir/lower.tr"
    while ((i < f->params->len)) {
#line 819 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, i))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 819 "src/taumir/lower.tr"
            c = (c + 1LL);
        }
#line 820 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 821 "src/taumir/lower.tr"
    return c;
}

__attribute__((hot)) TrStr _ov_method_name(LModule* m, TrStr cls, HirFunction* f) {
#line 827 "src/taumir/lower.tr"
    TrStr base = ({ __auto_type _ord_t6160 = (({ TrStr _tr_cl = (_own(cls)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6160); TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 828 "src/taumir/lower.tr"
    if (LModule_is_overloaded(m, cls, f->name)) {
#line 829 "src/taumir/lower.tr"
        TrStr _trret1503 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((base), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_method_nonself_pcount(f))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("arg", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(base);
        return _trret1503;
    }
#line 830 "src/taumir/lower.tr"
    TrStr _trret1504 = _tr_str_retain(base);
    _tr_str_release(base);
    return _trret1504;
}

__attribute__((hot)) void _detect_overloads(LModule* m, HirProgram* prog) {
#line 835 "src/taumir/lower.tr"
    long long i = 0LL;
#line 836 "src/taumir/lower.tr"
    while ((i < prog->classes->len)) {
#line 837 "src/taumir/lower.tr"
        HirClass* ov_cls = ((HirClass*)List_ptr_get(prog->classes, i));
#line 838 "src/taumir/lower.tr"
        if ((ov_cls->generics->len == 0LL)) {
#line 839 "src/taumir/lower.tr"
            long long mi = 0LL;
#line 840 "src/taumir/lower.tr"
            while ((mi < ov_cls->methods->len)) {
#line 841 "src/taumir/lower.tr"
                HirFunction* m1 = ((HirFunction*)List_ptr_get(ov_cls->methods, mi));
#line 842 "src/taumir/lower.tr"
                long long mj = (mi + 1LL);
#line 843 "src/taumir/lower.tr"
                while ((mj < ov_cls->methods->len)) {
#line 844 "src/taumir/lower.tr"
                    if (({ __auto_type _ord_t6161 = (((HirFunction*)List_ptr_get(ov_cls->methods, mj))->name); __auto_type _wr = (_tr_str_eqv((_ord_t6161), (m1->name))); _wr; })) {
#line 845 "src/taumir/lower.tr"
                        LModule_mark_overloaded(m, ov_cls->name, m1->name);
#line 846 "src/taumir/lower.tr"
                        mj = ov_cls->methods->len;
                    } else {
#line 848 "src/taumir/lower.tr"
                        mj = (mj + 1LL);
                    }
                }
#line 849 "src/taumir/lower.tr"
                mi = (mi + 1LL);
            }
        }
#line 850 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 851 "src/taumir/lower.tr"
    i = 0LL;
#line 852 "src/taumir/lower.tr"
    while ((i < prog->enums->len)) {
#line 853 "src/taumir/lower.tr"
        HirEnum* ov_enm = ((HirEnum*)List_ptr_get(prog->enums, i));
#line 854 "src/taumir/lower.tr"
        long long mi = 0LL;
#line 855 "src/taumir/lower.tr"
        while ((mi < ov_enm->methods->len)) {
#line 856 "src/taumir/lower.tr"
            HirFunction* m1 = ((HirFunction*)List_ptr_get(ov_enm->methods, mi));
#line 857 "src/taumir/lower.tr"
            long long mj = (mi + 1LL);
#line 858 "src/taumir/lower.tr"
            while ((mj < ov_enm->methods->len)) {
#line 859 "src/taumir/lower.tr"
                if (({ __auto_type _ord_t6162 = (((HirFunction*)List_ptr_get(ov_enm->methods, mj))->name); __auto_type _wr = (_tr_str_eqv((_ord_t6162), (m1->name))); _wr; })) {
#line 860 "src/taumir/lower.tr"
                    LModule_mark_overloaded(m, ov_enm->name, m1->name);
#line 861 "src/taumir/lower.tr"
                    mj = ov_enm->methods->len;
                } else {
#line 863 "src/taumir/lower.tr"
                    mj = (mj + 1LL);
                }
            }
#line 864 "src/taumir/lower.tr"
            mi = (mi + 1LL);
        }
#line 865 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) long long _arg_limit(LModule* m) {
#line 870 "src/taumir/lower.tr"
    if (m->target_llvm) {
#line 870 "src/taumir/lower.tr"
        return 24LL;
    }
#line 871 "src/taumir/lower.tr"
    return 6LL;
}

__attribute__((hot)) LModule* lower_to_lir_t(HirProgram* prog, bool llvm) {
#line 874 "src/taumir/lower.tr"
    return lower_to_lir_nh(prog, llvm, false);
}

__attribute__((hot)) LModule* lower_to_lir_nh(HirProgram* prog, bool llvm, bool no_heap) {
#line 879 "src/taumir/lower.tr"
    LModule* m = LModule_init();
#line 880 "src/taumir/lower.tr"
    m->target_llvm = llvm;
#line 881 "src/taumir/lower.tr"
    m->no_heap = no_heap;
#line 882 "src/taumir/lower.tr"
    HirProgram* _cltmp_t6163 = _tr_obj_retain(prog);
    _tr_obj_release(m->hir_prog, _trdrop_HirProgram);
    m->hir_prog = _cltmp_t6163;
#line 883 "src/taumir/lower.tr"
    _register_classes(m, prog);
#line 884 "src/taumir/lower.tr"
    return _lower_to_lir_body(m, prog);
}

__attribute__((hot)) LModule* lower_to_lir(HirProgram* prog) {
#line 887 "src/taumir/lower.tr"
    return lower_to_lir_t(prog, false);
}

__attribute__((hot)) bool _is_extern_c_int(TrStr n) {
#line 899 "src/taumir/lower.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("c_int", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uchar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_schar", 7LL))))) {
#line 899 "src/taumir/lower.tr"
        return true;
    }
#line 900 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_short", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ushort", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_long", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ulong", 7LL))))) {
#line 900 "src/taumir/lower.tr"
        return true;
    }
#line 901 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_longlong", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ulonglong", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_size_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ssize_t", 9LL))))) {
#line 901 "src/taumir/lower.tr"
        return true;
    }
#line 902 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_int8_t", 8LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int16_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int32_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int64_t", 9LL))))) {
#line 902 "src/taumir/lower.tr"
        return true;
    }
#line 903 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("c_uint8_t", 9LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint16_t", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint32_t", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uint64_t", 10LL))))) {
#line 903 "src/taumir/lower.tr"
        return true;
    }
#line 904 "src/taumir/lower.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("c_intptr_t", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uintptr_t", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ptrdiff_t", 11LL))))) {
#line 904 "src/taumir/lower.tr"
        return true;
    }
#line 905 "src/taumir/lower.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("c_wchar", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char16", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char32", 8LL))))) {
#line 905 "src/taumir/lower.tr"
        return true;
    }
#line 906 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _is_extern_c_float(TrStr n) {
#line 909 "src/taumir/lower.tr"
    return ((_tr_str_eqv((n), (_tr_str_lit_len("c_float", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("c_double", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_ldouble", 9LL))));
}

__attribute__((hot)) long long _extern_sig_tag(LModule* m, AstType* ty) {
#line 915 "src/taumir/lower.tr"
    long long t = _tag_of(m, ty);
#line 916 "src/taumir/lower.tr"
    if ((t >= 0LL)) {
#line 916 "src/taumir/lower.tr"
        return t;
    }
#line 917 "src/taumir/lower.tr"
    if (_is_extern_c_int(ty->name)) {
#line 917 "src/taumir/lower.tr"
        return 0LL;
    }
#line 918 "src/taumir/lower.tr"
    if (_is_extern_c_float(ty->name)) {
#line 918 "src/taumir/lower.tr"
        return 5LL;
    }
#line 919 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) LModule* _lower_to_lir_body(LModule* m, HirProgram* prog) {
#line 922 "src/taumir/lower.tr"
    _detect_overloads(m, prog);
#line 926 "src/taumir/lower.tr"
    long long efx = 0LL;
#line 927 "src/taumir/lower.tr"
    while ((efx < prog->extern_funcs->len)) {
#line 928 "src/taumir/lower.tr"
        HirFunction* ef = ((HirFunction*)List_ptr_get(prog->extern_funcs, efx));
#line 929 "src/taumir/lower.tr"
        bool ef_ok = true;
#line 930 "src/taumir/lower.tr"
        long long epi = 0LL;
#line 931 "src/taumir/lower.tr"
        while ((epi < ef->params->len)) {
#line 932 "src/taumir/lower.tr"
            long long eptag = _extern_sig_tag(m, ((HirParam*)List_ptr_get(ef->params, epi))->ty);
#line 933 "src/taumir/lower.tr"
            if (((((eptag != 0LL) && (eptag != 1LL)) && (eptag != 4LL)) && (eptag != 5LL))) {
#line 934 "src/taumir/lower.tr"
                ef_ok = false;
            }
#line 935 "src/taumir/lower.tr"
            epi = (epi + 1LL);
        }
#line 936 "src/taumir/lower.tr"
        long long ertag = _extern_sig_tag(m, ef->ret_ty);
#line 937 "src/taumir/lower.tr"
        if (((((ertag != 0LL) && (ertag != 1LL)) && (ertag != 4LL)) && (ertag != 5LL))) {
#line 937 "src/taumir/lower.tr"
            ef_ok = false;
        }
#line 938 "src/taumir/lower.tr"
        if (ef_ok) {
#line 939 "src/taumir/lower.tr"
            ({ TrStr _ord_t6164 = (_own(ef->name)); List_TrStr_append(m->extfn_names, _ord_t6164); _tr_str_release(_ord_t6164); });
#line 940 "src/taumir/lower.tr"
            List_i64_append(m->extfn_ret, ertag);
        }
#line 941 "src/taumir/lower.tr"
        efx = (efx + 1LL);
    }
#line 945 "src/taumir/lower.tr"
    long long i = 0LL;
#line 946 "src/taumir/lower.tr"
    while ((i < prog->functions->len)) {
#line 947 "src/taumir/lower.tr"
        HirFunction* f0 = ((HirFunction*)List_ptr_get(prog->functions, i));
#line 948 "src/taumir/lower.tr"
        if (((!f0->is_extern) && (!_fn_is_specializable(m, f0)))) {
#line 949 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((f0->class_name), (_tr_str_lit_len("", 0LL)))) && (_prog_generic_class_index(m, f0->class_name) >= 0LL))) {
#line 950 "src/taumir/lower.tr"
                i = (i + 1LL);
#line 951 "src/taumir/lower.tr"
                continue;
            }
#line 952 "src/taumir/lower.tr"
            TrStr f0mang = _tr_str_retain(f0->name);
#line 953 "src/taumir/lower.tr"
            if ((!_tr_str_eqv((f0->class_name), (_tr_str_lit_len("", 0LL))))) {
#line 953 "src/taumir/lower.tr"
                TrStr _strtmp_t6165 = _ov_method_name(m, f0->class_name, f0);
                _tr_str_release(f0mang);
                f0mang = _strtmp_t6165;
            }
#line 954 "src/taumir/lower.tr"
            long long f0rt = _lir_fn_ret_tag(m, f0);
#line 955 "src/taumir/lower.tr"
            List_TrStr_append(m->fn_names, f0mang);
#line 956 "src/taumir/lower.tr"
            List_i64_append(m->fn_ret, f0rt);
#line 957 "src/taumir/lower.tr"
            if (((f0->returns_owned || LModule_is_value_class(m, f0->ret_ty->name)) && ((f0rt == 10LL) || (f0rt == 11LL)))) {
#line 957 "src/taumir/lower.tr"
                LModule_mark_fn_owned(m, f0mang);
            }
        }
#line 958 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 961 "src/taumir/lower.tr"
    long long cmi = 0LL;
#line 962 "src/taumir/lower.tr"
    while ((cmi < prog->classes->len)) {
#line 963 "src/taumir/lower.tr"
        HirClass* cm_cls = ((HirClass*)List_ptr_get(prog->classes, cmi));
#line 964 "src/taumir/lower.tr"
        if ((cm_cls->generics->len == 0LL)) {
#line 965 "src/taumir/lower.tr"
            long long cmi2 = 0LL;
#line 966 "src/taumir/lower.tr"
            while ((cmi2 < cm_cls->methods->len)) {
#line 967 "src/taumir/lower.tr"
                HirFunction* cm_f = ((HirFunction*)List_ptr_get(cm_cls->methods, cmi2));
#line 968 "src/taumir/lower.tr"
                if ((((!cm_f->is_extern) && (!_fn_is_specializable(m, cm_f))) && (!_method_in_prog_functions(prog, cm_cls->name, cm_f->name)))) {
#line 969 "src/taumir/lower.tr"
                    TrStr cm_mang = _ov_method_name(m, cm_cls->name, cm_f);
#line 970 "src/taumir/lower.tr"
                    long long cm_rt = _lir_fn_ret_tag(m, cm_f);
#line 971 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6166 = (_own(cm_mang)); List_TrStr_append(m->fn_names, _ord_t6166); _tr_str_release(_ord_t6166); });
#line 972 "src/taumir/lower.tr"
                    List_i64_append(m->fn_ret, cm_rt);
#line 973 "src/taumir/lower.tr"
                    if ((cm_f->returns_owned && ((cm_rt == 10LL) || (cm_rt == 11LL)))) {
#line 973 "src/taumir/lower.tr"
                        LModule_mark_fn_owned(m, cm_mang);
                    }
                }
#line 974 "src/taumir/lower.tr"
                cmi2 = (cmi2 + 1LL);
            }
        }
#line 975 "src/taumir/lower.tr"
        cmi = (cmi + 1LL);
    }
#line 976 "src/taumir/lower.tr"
    long long emi = 0LL;
#line 977 "src/taumir/lower.tr"
    while ((emi < prog->enums->len)) {
#line 978 "src/taumir/lower.tr"
        HirEnum* em_en = ((HirEnum*)List_ptr_get(prog->enums, emi));
#line 979 "src/taumir/lower.tr"
        long long emi2 = 0LL;
#line 980 "src/taumir/lower.tr"
        while ((emi2 < em_en->methods->len)) {
#line 981 "src/taumir/lower.tr"
            HirFunction* em_f = ((HirFunction*)List_ptr_get(em_en->methods, emi2));
#line 982 "src/taumir/lower.tr"
            if ((((!em_f->is_extern) && (!_fn_is_specializable(m, em_f))) && (!_method_in_prog_functions(prog, em_en->name, em_f->name)))) {
#line 983 "src/taumir/lower.tr"
                TrStr em_mang = _ov_method_name(m, em_en->name, em_f);
#line 984 "src/taumir/lower.tr"
                long long em_rt = _tag_of(m, em_f->ret_ty);
#line 985 "src/taumir/lower.tr"
                ({ TrStr _ord_t6167 = (_own(em_mang)); List_TrStr_append(m->fn_names, _ord_t6167); _tr_str_release(_ord_t6167); });
#line 986 "src/taumir/lower.tr"
                List_i64_append(m->fn_ret, em_rt);
#line 987 "src/taumir/lower.tr"
                if ((em_f->returns_owned && ((em_rt == 10LL) || (em_rt == 11LL)))) {
#line 987 "src/taumir/lower.tr"
                    LModule_mark_fn_owned(m, em_mang);
                }
            }
#line 988 "src/taumir/lower.tr"
            emi2 = (emi2 + 1LL);
        }
#line 989 "src/taumir/lower.tr"
        emi = (emi + 1LL);
    }
#line 992 "src/taumir/lower.tr"
    long long ti = 0LL;
#line 993 "src/taumir/lower.tr"
    while ((ti < prog->top_level_stmts->len)) {
#line 994 "src/taumir/lower.tr"
        if ((!_register_global(m, ((HirStmt*)List_ptr_get(prog->top_level_stmts, ti))))) {
#line 995 "src/taumir/lower.tr"
            m->ok = false;
#line 996 "src/taumir/lower.tr"
            m->fail_note = _tr_str_lit_len("unsupported top-level statement", 31LL);
#line 997 "src/taumir/lower.tr"
            return _tr_obj_retain(m);
        }
#line 998 "src/taumir/lower.tr"
        ti = (ti + 1LL);
    }
#line 999 "src/taumir/lower.tr"
    i = 0LL;
#line 1000 "src/taumir/lower.tr"
    while ((i < prog->functions->len)) {
#line 1001 "src/taumir/lower.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
#line 1002 "src/taumir/lower.tr"
        if (((!f->is_extern) && (!_fn_is_specializable(m, f)))) {
#line 1003 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (_prog_generic_class_index(m, f->class_name) >= 0LL))) {
#line 1004 "src/taumir/lower.tr"
                i = (i + 1LL);
#line 1005 "src/taumir/lower.tr"
                continue;
            }
#line 1006 "src/taumir/lower.tr"
            if (_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) {
#line 1007 "src/taumir/lower.tr"
                _lir_lower_function(m, f);
            } else {
#line 1009 "src/taumir/lower.tr"
                _lir_lower_method(m, f->class_name, f);
            }
#line 1010 "src/taumir/lower.tr"
            if ((!m->ok)) {
#line 1015 "src/taumir/lower.tr"
                if ((_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL))))) {
#line 1015 "src/taumir/lower.tr"
                    return _tr_obj_retain(m);
                }
#line 1016 "src/taumir/lower.tr"
                TrStr un = _tr_str_retain(f->name);
#line 1017 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL))))) {
#line 1017 "src/taumir/lower.tr"
                    TrStr _strtmp_t6168 = _ov_method_name(m, f->class_name, f);
                    _tr_str_release(un);
                    un = _strtmp_t6168;
                }
#line 1018 "src/taumir/lower.tr"
                ({ TrStr _ord_t6169 = (_own(un)); List_TrStr_append(m->unavail_names, _ord_t6169); _tr_str_release(_ord_t6169); });
#line 1019 "src/taumir/lower.tr"
                ({ TrStr _ord_t6170 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _ord_t6170); _tr_str_release(_ord_t6170); });
#line 1020 "src/taumir/lower.tr"
                m->ok = true;
#line 1021 "src/taumir/lower.tr"
                m->fail_note = _tr_str_lit_len("", 0LL);
            }
        }
#line 1022 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1024 "src/taumir/lower.tr"
    long long cml = 0LL;
#line 1025 "src/taumir/lower.tr"
    while ((cml < prog->classes->len)) {
#line 1026 "src/taumir/lower.tr"
        HirClass* cml_cls = ((HirClass*)List_ptr_get(prog->classes, cml));
#line 1027 "src/taumir/lower.tr"
        if ((cml_cls->generics->len == 0LL)) {
#line 1028 "src/taumir/lower.tr"
            long long cml2 = 0LL;
#line 1029 "src/taumir/lower.tr"
            while ((cml2 < cml_cls->methods->len)) {
#line 1030 "src/taumir/lower.tr"
                HirFunction* cml_f = ((HirFunction*)List_ptr_get(cml_cls->methods, cml2));
#line 1031 "src/taumir/lower.tr"
                if ((((!cml_f->is_extern) && (!_fn_is_specializable(m, cml_f))) && (!_method_in_prog_functions(prog, cml_cls->name, cml_f->name)))) {
#line 1032 "src/taumir/lower.tr"
                    _lir_lower_method(m, cml_cls->name, cml_f);
#line 1033 "src/taumir/lower.tr"
                    if ((!m->ok)) {
#line 1034 "src/taumir/lower.tr"
                        TrStr cun = _ov_method_name(m, cml_cls->name, cml_f);
#line 1035 "src/taumir/lower.tr"
                        ({ TrStr _ord_t6171 = (_own(cun)); List_TrStr_append(m->unavail_names, _ord_t6171); _tr_str_release(_ord_t6171); });
#line 1036 "src/taumir/lower.tr"
                        ({ TrStr _ord_t6172 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _ord_t6172); _tr_str_release(_ord_t6172); });
#line 1037 "src/taumir/lower.tr"
                        m->ok = true;
#line 1038 "src/taumir/lower.tr"
                        m->fail_note = _tr_str_lit_len("", 0LL);
                    }
                }
#line 1039 "src/taumir/lower.tr"
                cml2 = (cml2 + 1LL);
            }
        }
#line 1040 "src/taumir/lower.tr"
        cml = (cml + 1LL);
    }
#line 1041 "src/taumir/lower.tr"
    long long eml = 0LL;
#line 1042 "src/taumir/lower.tr"
    while ((eml < prog->enums->len)) {
#line 1043 "src/taumir/lower.tr"
        HirEnum* eml_en = ((HirEnum*)List_ptr_get(prog->enums, eml));
#line 1044 "src/taumir/lower.tr"
        long long eml2 = 0LL;
#line 1045 "src/taumir/lower.tr"
        while ((eml2 < eml_en->methods->len)) {
#line 1046 "src/taumir/lower.tr"
            HirFunction* eml_f = ((HirFunction*)List_ptr_get(eml_en->methods, eml2));
#line 1047 "src/taumir/lower.tr"
            if ((((!eml_f->is_extern) && (!_fn_is_specializable(m, eml_f))) && (!_method_in_prog_functions(prog, eml_en->name, eml_f->name)))) {
#line 1048 "src/taumir/lower.tr"
                _lir_lower_method(m, eml_en->name, eml_f);
#line 1049 "src/taumir/lower.tr"
                if ((!m->ok)) {
#line 1050 "src/taumir/lower.tr"
                    TrStr eun = _ov_method_name(m, eml_en->name, eml_f);
#line 1051 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6173 = (_own(eun)); List_TrStr_append(m->unavail_names, _ord_t6173); _tr_str_release(_ord_t6173); });
#line 1052 "src/taumir/lower.tr"
                    ({ TrStr _ord_t6174 = (_own(m->fail_note)); List_TrStr_append(m->unavail_notes, _ord_t6174); _tr_str_release(_ord_t6174); });
#line 1053 "src/taumir/lower.tr"
                    m->ok = true;
#line 1054 "src/taumir/lower.tr"
                    m->fail_note = _tr_str_lit_len("", 0LL);
                }
            }
#line 1055 "src/taumir/lower.tr"
            eml2 = (eml2 + 1LL);
        }
#line 1056 "src/taumir/lower.tr"
        eml = (eml + 1LL);
    }
#line 1060 "src/taumir/lower.tr"
    _gen_class_drops(m);
#line 1065 "src/taumir/lower.tr"
    if ((m->unavail_names->len > 0LL)) {
#line 1066 "src/taumir/lower.tr"
        List_i64* dropped = (void*)List_i64_new();
#line 1067 "src/taumir/lower.tr"
        long long di0 = 0LL;
#line 1068 "src/taumir/lower.tr"
        while ((di0 < m->funcs->len)) {
#line 1069 "src/taumir/lower.tr"
            List_i64_append(dropped, 0LL);
#line 1070 "src/taumir/lower.tr"
            di0 = (di0 + 1LL);
        }
#line 1071 "src/taumir/lower.tr"
        bool changed = true;
#line 1072 "src/taumir/lower.tr"
        while (changed) {
#line 1073 "src/taumir/lower.tr"
            changed = false;
#line 1074 "src/taumir/lower.tr"
            long long fi2 = 0LL;
#line 1075 "src/taumir/lower.tr"
            while ((fi2 < m->funcs->len)) {
#line 1076 "src/taumir/lower.tr"
                if ((List_i64_get(dropped, fi2) == 0LL)) {
#line 1077 "src/taumir/lower.tr"
                    LFunc* lfc = ((LFunc*)List_ptr_get(m->funcs, fi2));
#line 1078 "src/taumir/lower.tr"
                    long long hit = (-1LL);
#line 1079 "src/taumir/lower.tr"
                    long long bi2 = 0LL;
#line 1080 "src/taumir/lower.tr"
                    while (((bi2 < lfc->blocks->len) && (hit < 0LL))) {
#line 1081 "src/taumir/lower.tr"
                        LBlock* blk2 = ((LBlock*)List_ptr_get(lfc->blocks, bi2));
#line 1082 "src/taumir/lower.tr"
                        long long ii2 = 0LL;
#line 1083 "src/taumir/lower.tr"
                        while (((ii2 < blk2->insts->len) && (hit < 0LL))) {
#line 1084 "src/taumir/lower.tr"
                            __auto_type _t6175 = (*((LInst*)List_ptr_get(blk2->insts, ii2)));
                            if (_t6175.tag == LInst_ICall) {
                                __auto_type ucallee = _t6175.data.ICall.callee;
#line 1086 "src/taumir/lower.tr"
                                hit = LModule_unavail_index(m, ucallee);
                            } else if (_t6175.tag == LInst_IFuncAddr) {
                                __auto_type ufname = _t6175.data.IFuncAddr.fname;
#line 1088 "src/taumir/lower.tr"
                                hit = LModule_unavail_index(m, ufname);
                            } else if (1) {
                                __auto_type _ = _t6175;
                                /* pass */
                            }
#line 1090 "src/taumir/lower.tr"
                            ii2 = (ii2 + 1LL);
                        }
#line 1091 "src/taumir/lower.tr"
                        bi2 = (bi2 + 1LL);
                    }
#line 1092 "src/taumir/lower.tr"
                    if ((hit >= 0LL)) {
#line 1093 "src/taumir/lower.tr"
                        TrStr unote = ({ __auto_type _ord_t6176 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(lfc->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' calls '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t6177 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6176); TrStr _tr_cr = (List_TrStr_get(m->unavail_names, hit)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6177); TrStr _tr_cr = (List_TrStr_get(m->unavail_notes, hit)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 1094 "src/taumir/lower.tr"
                        if (lfc->is_main) {
#line 1095 "src/taumir/lower.tr"
                            m->ok = false;
#line 1096 "src/taumir/lower.tr"
                            m->fail_note = _tr_str_retain(unote);
#line 1097 "src/taumir/lower.tr"
                            List_i64_free(dropped);
                            _tr_str_release(unote);
                            return _tr_obj_retain(m);
                        }
#line 1098 "src/taumir/lower.tr"
                        ({ TrStr _ord_t6178 = (_own(lfc->name)); List_TrStr_append(m->unavail_names, _ord_t6178); _tr_str_release(_ord_t6178); });
#line 1099 "src/taumir/lower.tr"
                        List_TrStr_append(m->unavail_notes, unote);
#line 1100 "src/taumir/lower.tr"
                        List_i64_set(dropped, fi2, 1LL);
#line 1101 "src/taumir/lower.tr"
                        changed = true;
                    }
                }
#line 1102 "src/taumir/lower.tr"
                fi2 = (fi2 + 1LL);
            }
        }
#line 1104 "src/taumir/lower.tr"
        List_ptr* kept = (void*)List_ptr_new();
#line 1105 "src/taumir/lower.tr"
        long long ki = 0LL;
#line 1106 "src/taumir/lower.tr"
        while ((ki < m->funcs->len)) {
#line 1107 "src/taumir/lower.tr"
            if ((List_i64_get(dropped, ki) == 0LL)) {
#line 1107 "src/taumir/lower.tr"
                List_ptr_append(kept, _tr_obj_retain(((LFunc*)List_ptr_get(m->funcs, ki))));
            }
#line 1108 "src/taumir/lower.tr"
            ki = (ki + 1LL);
        }
#line 1109 "src/taumir/lower.tr"
        m->funcs = kept;
    }
#line 1110 "src/taumir/lower.tr"
    return _tr_obj_retain(m);
}

__attribute__((hot)) bool _fn_has_iface_param(LModule* m, HirFunction* f) {
#line 1115 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1116 "src/taumir/lower.tr"
    while ((i < f->params->len)) {
#line 1117 "src/taumir/lower.tr"
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, i))->ty->name);
#line 1118 "src/taumir/lower.tr"
        if (((!_is_null_str(pn)) && LModule_is_iface(m, pn))) {
#line 1118 "src/taumir/lower.tr"
            _tr_str_release(pn);
            return true;
        }
#line 1119 "src/taumir/lower.tr"
        i = (i + 1LL);
        _tr_str_release(pn);
    }
#line 1120 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _fn_is_specializable(LModule* m, HirFunction* f) {
#line 1124 "src/taumir/lower.tr"
    if ((f->generics->len > 0LL)) {
#line 1124 "src/taumir/lower.tr"
        return true;
    }
#line 1125 "src/taumir/lower.tr"
    return _fn_has_iface_param(m, f);
}

__attribute__((hot)) long long _find_generic_fn(LModule* m, TrStr name) {
#line 1129 "src/taumir/lower.tr"
    if (_is_null_str(name)) {
#line 1129 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1130 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1131 "src/taumir/lower.tr"
    while ((i < m->hir_prog->functions->len)) {
#line 1132 "src/taumir/lower.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
#line 1133 "src/taumir/lower.tr"
        if (((_tr_str_eqv((f->name), (name)) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (!f->is_extern))) {
#line 1134 "src/taumir/lower.tr"
            if (_fn_is_specializable(m, f)) {
#line 1134 "src/taumir/lower.tr"
                long long _trret1505 = i;
                return _trret1505;
            }
        }
#line 1135 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1136 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) long long _find_generic_method(LModule* m, TrStr cls, TrStr method) {
#line 1141 "src/taumir/lower.tr"
    if ((_is_null_str(cls) || _is_null_str(method))) {
#line 1141 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1142 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1143 "src/taumir/lower.tr"
    while ((i < m->hir_prog->functions->len)) {
#line 1144 "src/taumir/lower.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
#line 1145 "src/taumir/lower.tr"
        if ((((_tr_str_eqv((f->class_name), (cls)) && _tr_str_eqv((f->name), (method))) && (f->generics->len > 0LL)) && (!f->is_extern))) {
#line 1146 "src/taumir/lower.tr"
            long long _trret1506 = i;
            return _trret1506;
        }
#line 1147 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1148 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _is_generic_param(HirFunction* f, TrStr n) {
#line 1152 "src/taumir/lower.tr"
    if (_is_null_str(n)) {
#line 1152 "src/taumir/lower.tr"
        return false;
    }
#line 1153 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1154 "src/taumir/lower.tr"
    while ((i < f->generics->len)) {
#line 1155 "src/taumir/lower.tr"
        if (({ TrStr _cmpa_t6179 = (List_TrStr_get(f->generics, i)); TrStr _cmpb_t6180 = (n); _Bool _cmpr_t6181 = _tr_str_eqv(_cmpa_t6179, _cmpb_t6180); _tr_str_release(_cmpa_t6179); _cmpr_t6181; })) {
#line 1155 "src/taumir/lower.tr"
            return true;
        }
#line 1156 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1157 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _param_is_abstract(LModule* m, HirFunction* f, TrStr ptyname) {
#line 1162 "src/taumir/lower.tr"
    if (_is_generic_param(f, ptyname)) {
#line 1162 "src/taumir/lower.tr"
        return true;
    }
#line 1163 "src/taumir/lower.tr"
    if (((!_is_null_str(ptyname)) && LModule_is_iface(m, ptyname))) {
#line 1163 "src/taumir/lower.tr"
        return true;
    }
#line 1164 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _lir_lower_generic(LModule* m, HirFunction* f, List_i64* argtags, List_TrStr* argcls, TrStr mangled) {
#line 1172 "src/taumir/lower.tr"
    if ((f->params->len != argtags->len)) {
#line 1172 "src/taumir/lower.tr"
        return false;
    }
#line 1174 "src/taumir/lower.tr"
    long long rtag = (-1LL);
#line 1175 "src/taumir/lower.tr"
    if (_param_is_abstract(m, f, f->ret_ty->name)) {
#line 1176 "src/taumir/lower.tr"
        long long ri = 0LL;
#line 1177 "src/taumir/lower.tr"
        while ((ri < f->params->len)) {
#line 1178 "src/taumir/lower.tr"
            if ((!_is_null_str(((HirParam*)List_ptr_get(f->params, ri))->ty->name))) {
#line 1179 "src/taumir/lower.tr"
                if (({ __auto_type _ord_t6182 = (((HirParam*)List_ptr_get(f->params, ri))->ty->name); __auto_type _wr = (_tr_str_eqv((_ord_t6182), (f->ret_ty->name))); _wr; })) {
#line 1179 "src/taumir/lower.tr"
                    rtag = List_i64_get(argtags, ri);
                }
            }
#line 1180 "src/taumir/lower.tr"
            ri = (ri + 1LL);
        }
    } else {
#line 1182 "src/taumir/lower.tr"
        rtag = _tag_of(m, f->ret_ty);
    }
#line 1183 "src/taumir/lower.tr"
    if ((rtag < 0LL)) {
#line 1183 "src/taumir/lower.tr"
        return false;
    }
#line 1184 "src/taumir/lower.tr"
    ({ TrStr _ord_t6183 = (_own(mangled)); List_TrStr_append(m->fn_names, _ord_t6183); _tr_str_release(_ord_t6183); });
#line 1185 "src/taumir/lower.tr"
    List_i64_append(m->fn_ret, rtag);
#line 1186 "src/taumir/lower.tr"
    if (((f->returns_owned || LModule_is_value_class(m, f->ret_ty->name)) && ((rtag == 10LL) || (rtag == 11LL)))) {
#line 1186 "src/taumir/lower.tr"
        ({ TrStr _at_t6184 = (_own(mangled)); LModule_mark_fn_owned(m, _at_t6184); _tr_str_release(_at_t6184); });
    }
#line 1187 "src/taumir/lower.tr"
    LFunc* lf = ({ TrStr _at_t6185 = (_own(mangled)); __auto_type _wr = (LFunc_init(_at_t6185)); _tr_str_release(_at_t6185); _wr; });
#line 1188 "src/taumir/lower.tr"
    long long pi = 0LL;
#line 1189 "src/taumir/lower.tr"
    while ((pi < f->params->len)) {
#line 1190 "src/taumir/lower.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 1191 "src/taumir/lower.tr"
        long long ptag = List_i64_get(argtags, pi);
#line 1193 "src/taumir/lower.tr"
        if ((!_param_is_abstract(m, f, p->ty->name))) {
#line 1194 "src/taumir/lower.tr"
            long long want = _tag_of(m, p->ty);
#line 1195 "src/taumir/lower.tr"
            if (((want < 0LL) || (!_field_tag_ok(ptag, want)))) {
#line 1195 "src/taumir/lower.tr"
                _tr_obj_release(lf, _trdrop_LFunc);
                return false;
            }
        }
#line 1196 "src/taumir/lower.tr"
        List_TrStr_append(lf->params, p->name);
#line 1197 "src/taumir/lower.tr"
        LFunc_add_var(lf, p->name);
#line 1198 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, p->name, ptag);
#line 1199 "src/taumir/lower.tr"
        if ((((ptag == 10LL) || (ptag == 11LL)) && ({ TrStr _cmpa_t6186 = (List_TrStr_get(argcls, pi)); TrStr _cmpb_t6187 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6188 = !_tr_str_eqv(_cmpa_t6186, _cmpb_t6187); _tr_str_release(_cmpa_t6186); _cmpr_t6188; }))) {
#line 1200 "src/taumir/lower.tr"
            ({ __auto_type _ord_t6189 = (p->name); TrStr _at_t6190 = (List_TrStr_get(argcls, pi)); TrStr _ord_t6191 = (_own(_at_t6190)); LFunc_set_var_cls(lf, _ord_t6189, _ord_t6191); _tr_str_release(_at_t6190); _tr_str_release(_ord_t6191); });
        }
#line 1201 "src/taumir/lower.tr"
        pi = (pi + 1LL);
    }
#line 1202 "src/taumir/lower.tr"
    LFunc_set_cur(lf, LFunc_new_block(lf));
#line 1203 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, f->body))) {
#line 1203 "src/taumir/lower.tr"
        _tr_obj_release(lf, _trdrop_LFunc);
        return false;
    }
#line 1204 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
#line 1205 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
#line 1206 "src/taumir/lower.tr"
    _tr_obj_release(lf, _trdrop_LFunc);
    return true;
}

__attribute__((hot)) TrStr _mono_base(TrStr name) {
#line 1210 "src/taumir/lower.tr"
    TrStr marker = _tr_str_lit_len("__MONO_", 7LL);
#line 1211 "src/taumir/lower.tr"
    long long idx = _tr_str_index_ofv((name), (marker));
#line 1212 "src/taumir/lower.tr"
    if ((idx < 0LL)) {
#line 1212 "src/taumir/lower.tr"
        _tr_str_release(marker);
        return _tr_str_lit_len("", 0LL);
    }
#line 1213 "src/taumir/lower.tr"
    TrStr _trret1507 = _tr_str_slicev((name), 0LL, idx);
    _tr_str_release(marker);
    return _trret1507;
}

__attribute__((hot)) TrStr _mono_concrete(TrStr name) {
#line 1216 "src/taumir/lower.tr"
    TrStr marker = _tr_str_lit_len("__MONO_", 7LL);
#line 1217 "src/taumir/lower.tr"
    long long idx = _tr_str_index_ofv((name), (marker));
#line 1218 "src/taumir/lower.tr"
    if ((idx < 0LL)) {
#line 1218 "src/taumir/lower.tr"
        _tr_str_release(marker);
        return _tr_str_lit_len("", 0LL);
    }
#line 1219 "src/taumir/lower.tr"
    TrStr _trret1508 = ({ __auto_type _ord_t6192 = ((idx + _tr_str_lenv((marker)))); __auto_type _ord_t6193 = (_tr_str_lenv((name))); __auto_type _wr = (_tr_str_slicev((name), _ord_t6192, _ord_t6193)); _wr; });
    _tr_str_release(marker);
    return _trret1508;
}

__attribute__((hot)) bool _lir_lower_mono_fn(LModule* m, HirFunction* f, TrStr mangled, AstType* concrete) {
#line 1224 "src/taumir/lower.tr"
    if ((f->generics->len != 1LL)) {
#line 1224 "src/taumir/lower.tr"
        return false;
    }
#line 1225 "src/taumir/lower.tr"
    bool ok = true;
#line 1226 "src/taumir/lower.tr"
    ({ TrStr _at_t6194 = (List_TrStr_get(f->generics, 0LL)); TrStr _ord_t6195 = (_own(_at_t6194)); List_TrStr_append(m->subst_names, _ord_t6195); _tr_str_release(_at_t6194); _tr_str_release(_ord_t6195); });
#line 1227 "src/taumir/lower.tr"
    ({ __auto_type _ord_t6196 = (box_asttype_lir(concrete)); List_ptr_append(m->subst_tys, _ord_t6196); });
#line 1228 "src/taumir/lower.tr"
    long long rtag = _tag_of(m, f->ret_ty);
#line 1229 "src/taumir/lower.tr"
    if ((rtag < 0LL)) {
#line 1229 "src/taumir/lower.tr"
        ok = false;
    }
#line 1230 "src/taumir/lower.tr"
    if (ok) {
#line 1231 "src/taumir/lower.tr"
        ({ TrStr _ord_t6197 = (_own(mangled)); List_TrStr_append(m->fn_names, _ord_t6197); _tr_str_release(_ord_t6197); });
#line 1232 "src/taumir/lower.tr"
        List_i64_append(m->fn_ret, rtag);
#line 1233 "src/taumir/lower.tr"
        if ((f->returns_owned && ((rtag == 10LL) || (rtag == 11LL)))) {
#line 1233 "src/taumir/lower.tr"
            ({ TrStr _at_t6198 = (_own(mangled)); LModule_mark_fn_owned(m, _at_t6198); _tr_str_release(_at_t6198); });
        }
#line 1234 "src/taumir/lower.tr"
        LFunc* lf = ({ TrStr _at_t6199 = (_own(mangled)); __auto_type _wr = (LFunc_init(_at_t6199)); _tr_str_release(_at_t6199); _wr; });
#line 1235 "src/taumir/lower.tr"
        long long pi = 0LL;
#line 1236 "src/taumir/lower.tr"
        while ((pi < f->params->len)) {
#line 1237 "src/taumir/lower.tr"
            HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 1238 "src/taumir/lower.tr"
            long long ptag = _tag_of(m, p->ty);
#line 1239 "src/taumir/lower.tr"
            if ((ptag < 0LL)) {
#line 1240 "src/taumir/lower.tr"
                ok = false;
#line 1241 "src/taumir/lower.tr"
                break;
            }
#line 1242 "src/taumir/lower.tr"
            List_TrStr_append(lf->params, p->name);
#line 1243 "src/taumir/lower.tr"
            LFunc_add_var(lf, p->name);
#line 1244 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, p->name, ptag);
#line 1245 "src/taumir/lower.tr"
            if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
#line 1246 "src/taumir/lower.tr"
                ({ __auto_type _ord_t6200 = (p->name); TrStr _ord_t6201 = (_cls_of_ty(m, p->ty)); LFunc_set_var_cls(lf, _ord_t6200, _ord_t6201); _tr_str_release(_ord_t6201); });
            }
#line 1247 "src/taumir/lower.tr"
            pi = (pi + 1LL);
        }
#line 1248 "src/taumir/lower.tr"
        if (ok) {
#line 1249 "src/taumir/lower.tr"
            LFunc_set_cur(lf, LFunc_new_block(lf));
#line 1250 "src/taumir/lower.tr"
            if ((!lower_block(m, lf, f->body))) {
#line 1250 "src/taumir/lower.tr"
                ok = false;
            }
#line 1251 "src/taumir/lower.tr"
            if (ok) {
#line 1252 "src/taumir/lower.tr"
                LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
#line 1253 "src/taumir/lower.tr"
                List_ptr_append(m->funcs, _tr_obj_retain(lf));
            }
        }
    }
#line 1254 "src/taumir/lower.tr"
    List_TrStr_pop(m->subst_names);
#line 1255 "src/taumir/lower.tr"
    List_ptr_pop(m->subst_tys);
#line 1256 "src/taumir/lower.tr"
    return ok;
}

__attribute__((hot)) void _lir_lower_method(LModule* m, TrStr class_name, HirFunction* f) {
#line 1261 "src/taumir/lower.tr"
    LFunc* lf = ({ TrStr _at_t6202 = (_ov_method_name(m, class_name, f)); __auto_type _wr = (LFunc_init(_at_t6202)); _tr_str_release(_at_t6202); _wr; });
#line 1262 "src/taumir/lower.tr"
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
#line 1263 "src/taumir/lower.tr"
        lf->is_throws = true;
#line 1264 "src/taumir/lower.tr"
        lf->throws_ok_tag = _tag_of(m, f->ret_ty);
#line 1265 "src/taumir/lower.tr"
        lf->throws_err_tag = _tag_of(m, f->throws_ty);
    }
#line 1266 "src/taumir/lower.tr"
    long long pi = 0LL;
#line 1267 "src/taumir/lower.tr"
    while ((pi < f->params->len)) {
#line 1268 "src/taumir/lower.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 1269 "src/taumir/lower.tr"
        long long ptag = 0LL;
#line 1270 "src/taumir/lower.tr"
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 1271 "src/taumir/lower.tr"
            ptag = 10LL;
#line 1272 "src/taumir/lower.tr"
            if (LModule_is_enum(m, class_name)) {
#line 1272 "src/taumir/lower.tr"
                ptag = 11LL;
            }
        } else {
#line 1274 "src/taumir/lower.tr"
            ptag = _tag_of(m, p->ty);
        }
#line 1275 "src/taumir/lower.tr"
        if ((ptag < 0LL)) {
#line 1276 "src/taumir/lower.tr"
            m->ok = false;
#line 1277 "src/taumir/lower.tr"
            m->fail_note = ({ __auto_type _ord_t6203 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(class_name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6203); TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': unsupported param type", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 1278 "src/taumir/lower.tr"
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
#line 1279 "src/taumir/lower.tr"
        List_TrStr_append(lf->params, p->name);
#line 1280 "src/taumir/lower.tr"
        LFunc_add_var(lf, p->name);
#line 1281 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, p->name, ptag);
#line 1282 "src/taumir/lower.tr"
        if (_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL)))) {
#line 1283 "src/taumir/lower.tr"
            ({ __auto_type _ord_t6204 = (p->name); TrStr _ord_t6205 = (_own(class_name)); LFunc_set_var_cls(lf, _ord_t6204, _ord_t6205); _tr_str_release(_ord_t6205); });
        } else if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
#line 1285 "src/taumir/lower.tr"
            ({ __auto_type _ord_t6206 = (p->name); TrStr _ord_t6207 = (_own(p->ty->name)); LFunc_set_var_cls(lf, _ord_t6206, _ord_t6207); _tr_str_release(_ord_t6207); });
        }
#line 1286 "src/taumir/lower.tr"
        pi = (pi + 1LL);
    }
#line 1287 "src/taumir/lower.tr"
    LFunc_set_cur(lf, LFunc_new_block(lf));
#line 1288 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, f->body))) {
#line 1289 "src/taumir/lower.tr"
        m->ok = false;
#line 1290 "src/taumir/lower.tr"
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 1291 "src/taumir/lower.tr"
            m->fail_note = ({ __auto_type _ord_t6208 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(class_name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6208); TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': body uses an unsupported construct", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        }
#line 1292 "src/taumir/lower.tr"
        _tr_obj_release(lf, _trdrop_LFunc);
        return;
    }
#line 1293 "src/taumir/lower.tr"
    if ((!LFunc_cur_terminated(lf))) {
#line 1294 "src/taumir/lower.tr"
        if ((!_run_defers(m, lf))) {
#line 1295 "src/taumir/lower.tr"
            m->ok = false;
#line 1296 "src/taumir/lower.tr"
            if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 1297 "src/taumir/lower.tr"
                m->fail_note = ({ __auto_type _ord_t6209 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(class_name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("method '", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6209); TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': deferred statement not lowerable", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
            }
#line 1298 "src/taumir/lower.tr"
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
    }
#line 1299 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
#line 1300 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool _register_global(LModule* m, HirStmt* s) {
#line 1306 "src/taumir/lower.tr"
    __auto_type _t6210 = (*s);
    if (_t6210.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t6210.data.SLineMarker.n;
        return true;
    } else if (_t6210.tag == HirStmt_SPass) {
        return true;
    } else if (_t6210.tag == HirStmt_SLet) {
        __auto_type name = _t6210.data.SLet.name;
__auto_type ty = _t6210.data.SLet.ty;
__auto_type val = _t6210.data.SLet.val;
#line 1314 "src/taumir/lower.tr"
        if ((((m->target_llvm && _tr_str_eqv((ty->name), (_tr_str_lit_len("Array", 5LL)))) && (ty->array_size > 0LL)) && (ty->args->len > 0LL))) {
#line 1315 "src/taumir/lower.tr"
            long long getag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 1316 "src/taumir/lower.tr"
            if (((getag != 0LL) && (getag != 4LL))) {
#line 1316 "src/taumir/lower.tr"
                return false;
            }
#line 1317 "src/taumir/lower.tr"
            long long gidx_a = LModule_add_global(m, name, getag);
#line 1318 "src/taumir/lower.tr"
            LModule_set_global_arr(m, gidx_a, ty->array_size);
#line 1321 "src/taumir/lower.tr"
            if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 1321 "src/taumir/lower.tr"
                return true;
            }
#line 1322 "src/taumir/lower.tr"
            return false;
        }
#line 1323 "src/taumir/lower.tr"
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 1323 "src/taumir/lower.tr"
            return false;
        }
#line 1324 "src/taumir/lower.tr"
        long long tag = _ast_type_tag(hir_expr_type(val));
#line 1325 "src/taumir/lower.tr"
        if ((tag < 0LL)) {
#line 1325 "src/taumir/lower.tr"
            return false;
        }
#line 1326 "src/taumir/lower.tr"
        LModule_add_global(m, name, tag);
#line 1327 "src/taumir/lower.tr"
        List_ptr_append(m->global_inits, s);
#line 1328 "src/taumir/lower.tr"
        return true;
    } else if (_t6210.tag == HirStmt_SAssign) {
        __auto_type target = _t6210.data.SAssign.target;
__auto_type val = _t6210.data.SAssign.val;
#line 1330 "src/taumir/lower.tr"
        TrStr nm = _ident_name(target);
#line 1331 "src/taumir/lower.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL)))) {
#line 1331 "src/taumir/lower.tr"
            _tr_str_release(nm);
            return false;
        }
#line 1332 "src/taumir/lower.tr"
        long long tag2 = _ast_type_tag(hir_expr_type(val));
#line 1333 "src/taumir/lower.tr"
        if ((tag2 < 0LL)) {
#line 1333 "src/taumir/lower.tr"
            _tr_str_release(nm);
            return false;
        }
#line 1334 "src/taumir/lower.tr"
        LModule_add_global(m, nm, tag2);
#line 1335 "src/taumir/lower.tr"
        List_ptr_append(m->global_inits, s);
#line 1336 "src/taumir/lower.tr"
        _tr_str_release(nm);
        return true;
    } else if (1) {
        __auto_type _ = _t6210;
#line 1338 "src/taumir/lower.tr"
        return false;
    }
}

__attribute__((hot)) bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s) {
#line 1342 "src/taumir/lower.tr"
    __auto_type _t6211 = (*s);
    if (_t6211.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t6211.data.SLineMarker.n;
        return true;
    } else if (_t6211.tag == HirStmt_SPass) {
        return true;
    } else if (_t6211.tag == HirStmt_SLet) {
        __auto_type name = _t6211.data.SLet.name;
__auto_type val = _t6211.data.SLet.val;
#line 1346 "src/taumir/lower.tr"
        long long v = lower_expr(m, lf, val);
#line 1347 "src/taumir/lower.tr"
        if ((v < 0LL)) {
#line 1347 "src/taumir/lower.tr"
            return false;
        }
#line 1348 "src/taumir/lower.tr"
        long long gidx = LModule_global_index(m, name);
#line 1349 "src/taumir/lower.tr"
        if ((gidx < 0LL)) {
#line 1349 "src/taumir/lower.tr"
            return false;
        }
#line 1350 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, v) == 1LL)) {
#line 1350 "src/taumir/lower.tr"
            _secure_str(m, lf, v);
        }
#line 1351 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx, v));
#line 1352 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 1353 "src/taumir/lower.tr"
        return true;
    } else if (_t6211.tag == HirStmt_SAssign) {
        __auto_type target = _t6211.data.SAssign.target;
__auto_type val = _t6211.data.SAssign.val;
#line 1355 "src/taumir/lower.tr"
        long long v2 = lower_expr(m, lf, val);
#line 1356 "src/taumir/lower.tr"
        if ((v2 < 0LL)) {
#line 1356 "src/taumir/lower.tr"
            return false;
        }
#line 1357 "src/taumir/lower.tr"
        long long gidx2 = ({ TrStr _at_t6212 = (_ident_name(target)); __auto_type _wr = (LModule_global_index(m, _at_t6212)); _tr_str_release(_at_t6212); _wr; });
#line 1358 "src/taumir/lower.tr"
        if ((gidx2 < 0LL)) {
#line 1358 "src/taumir/lower.tr"
            return false;
        }
#line 1359 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, v2) == 1LL)) {
#line 1359 "src/taumir/lower.tr"
            _secure_str(m, lf, v2);
        }
#line 1360 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx2, v2));
#line 1361 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 1362 "src/taumir/lower.tr"
        return true;
    } else if (1) {
        __auto_type _ = _t6211;
#line 1364 "src/taumir/lower.tr"
        return false;
    }
}

__attribute__((hot)) void _lir_lower_function(LModule* m, HirFunction* f) {
#line 1367 "src/taumir/lower.tr"
    LFunc* lf = LFunc_init(f->name);
#line 1368 "src/taumir/lower.tr"
    if (_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL)))) {
#line 1368 "src/taumir/lower.tr"
        lf->is_main = true;
    }
#line 1369 "src/taumir/lower.tr"
    if ((_tr_str_eqv((f->name), (_tr_str_lit_len("main", 4LL))) && (f->params->len > 0LL))) {
#line 1369 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_argv_list", 16LL));
    }
#line 1370 "src/taumir/lower.tr"
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
#line 1371 "src/taumir/lower.tr"
        lf->is_throws = true;
#line 1372 "src/taumir/lower.tr"
        lf->throws_ok_tag = _tag_of(m, f->ret_ty);
#line 1373 "src/taumir/lower.tr"
        lf->throws_err_tag = _tag_of(m, f->throws_ty);
    }
#line 1374 "src/taumir/lower.tr"
    long long pi = 0LL;
#line 1375 "src/taumir/lower.tr"
    while ((pi < f->params->len)) {
#line 1376 "src/taumir/lower.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 1377 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((p->name), (_tr_str_lit_len("self", 4LL))))) {
#line 1378 "src/taumir/lower.tr"
            long long ptag = _tag_of(m, p->ty);
#line 1383 "src/taumir/lower.tr"
            if (((lf->is_main && (_tr_str_eqv((p->ty->name), (_tr_str_lit_len("Vec", 3LL))) || _tr_str_eqv((p->ty->name), (_tr_str_lit_len("List", 4LL))))) && (p->ty->args->len > 0LL))) {
#line 1384 "src/taumir/lower.tr"
                TrStr _mel = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(p->ty->args, 0LL))))->name);
#line 1385 "src/taumir/lower.tr"
                if ((_tr_str_eqv((_mel), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((_mel), (_tr_str_lit_len("String", 6LL))))) {
#line 1385 "src/taumir/lower.tr"
                    ptag = 3LL;
                }
                _tr_str_release(_mel);
            }
#line 1386 "src/taumir/lower.tr"
            if ((ptag < 0LL)) {
#line 1387 "src/taumir/lower.tr"
                m->ok = false;
#line 1388 "src/taumir/lower.tr"
                m->fail_note = ({ __auto_type _ord_t6213 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': unsupported param type '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6213); TrStr _tr_cr = (_own(p->ty->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 1389 "src/taumir/lower.tr"
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
#line 1390 "src/taumir/lower.tr"
            List_TrStr_append(lf->params, p->name);
#line 1391 "src/taumir/lower.tr"
            LFunc_add_var(lf, p->name);
#line 1392 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, p->name, ptag);
#line 1393 "src/taumir/lower.tr"
            if ((((ptag == 10LL) || (ptag == 11LL)) && (!_is_null_str(p->ty->name)))) {
#line 1394 "src/taumir/lower.tr"
                ({ __auto_type _ord_t6214 = (p->name); TrStr _ord_t6215 = (_own(p->ty->name)); LFunc_set_var_cls(lf, _ord_t6214, _ord_t6215); _tr_str_release(_ord_t6215); });
            }
        }
#line 1395 "src/taumir/lower.tr"
        pi = (pi + 1LL);
    }
#line 1396 "src/taumir/lower.tr"
    LFunc_set_cur(lf, LFunc_new_block(lf));
#line 1397 "src/taumir/lower.tr"
    if (lf->is_main) {
#line 1398 "src/taumir/lower.tr"
        long long gi = 0LL;
#line 1399 "src/taumir/lower.tr"
        while ((gi < m->global_inits->len)) {
#line 1400 "src/taumir/lower.tr"
            if ((!_lower_global_init(m, lf, ((HirStmt*)List_ptr_get(m->global_inits, gi))))) {
#line 1401 "src/taumir/lower.tr"
                m->ok = false;
#line 1402 "src/taumir/lower.tr"
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
#line 1403 "src/taumir/lower.tr"
            gi = (gi + 1LL);
        }
    }
#line 1404 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, f->body))) {
#line 1405 "src/taumir/lower.tr"
        m->ok = false;
#line 1406 "src/taumir/lower.tr"
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 1407 "src/taumir/lower.tr"
            m->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': body uses an unsupported construct", 37LL))); _tr_str_release(_tr_cl); _tr_cres; });
        }
#line 1408 "src/taumir/lower.tr"
        _tr_obj_release(lf, _trdrop_LFunc);
        return;
    }
#line 1409 "src/taumir/lower.tr"
    if ((!LFunc_cur_terminated(lf))) {
#line 1410 "src/taumir/lower.tr"
        if ((!_run_defers(m, lf))) {
#line 1411 "src/taumir/lower.tr"
            m->ok = false;
#line 1412 "src/taumir/lower.tr"
            if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 1413 "src/taumir/lower.tr"
                m->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(f->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("fn '", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': deferred statement not lowerable", 35LL))); _tr_str_release(_tr_cl); _tr_cres; });
            }
#line 1414 "src/taumir/lower.tr"
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
    }
#line 1415 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
#line 1416 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool _field_tag_ok(long long vt, long long ftg) {
#line 1421 "src/taumir/lower.tr"
    if ((vt == ftg)) {
#line 1421 "src/taumir/lower.tr"
        return true;
    }
#line 1422 "src/taumir/lower.tr"
    if ((((vt == 0LL) || (vt == 4LL)) && ((ftg == 0LL) || (ftg == 4LL)))) {
#line 1422 "src/taumir/lower.tr"
        return true;
    }
#line 1423 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) void _emit_field_set(LModule* m, LFunc* lf, long long obj, long long off, long long val) {
#line 1428 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStore(obj, off, val, 3LL));
}

__attribute__((hot)) long long _emit_field_get(LModule* m, LFunc* lf, long long obj, long long off, long long tag) {
#line 1432 "src/taumir/lower.tr"
    long long gd = LFunc_new_vreg(lf);
#line 1433 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, gd, tag);
#line 1434 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoad(gd, obj, off, 3LL));
#line 1435 "src/taumir/lower.tr"
    return gd;
}

__attribute__((hot)) long long _lower_enum_ctor(LModule* m, LFunc* lf, TrStr ename, TrStr vname, List_ptr* margs) {
#line 1441 "src/taumir/lower.tr"
    long long vidx = LModule_enum_variant_index(m, ename, vname);
#line 1442 "src/taumir/lower.tr"
    if ((vidx < 0LL)) {
#line 1442 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1447 "src/taumir/lower.tr"
    if (m->no_heap) {
#line 1448 "src/taumir/lower.tr"
        m->ok = false;
#line 1449 "src/taumir/lower.tr"
        m->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[H-1] constructing enum '", 25LL)), (ename))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' heap-allocates, which --no-heap forbids on this backend (it boxes via the object allocator).\n      FIX: use the C backend for --no-heap (it keeps enums as stack tagged unions), or avoid heap enums such as Option/Result on the zero-heap path", 242LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 1450 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1451 "src/taumir/lower.tr"
    EnumLayout* elay = ({ __auto_type _ord_t6216 = (LModule_enum_index(m, ename)); __auto_type _wr = (((EnumLayout*)List_ptr_get(m->enums, _ord_t6216))); _wr; });
#line 1452 "src/taumir/lower.tr"
    VariantLayout* vlay = ((VariantLayout*)List_ptr_get(elay->variants, vidx));
#line 1453 "src/taumir/lower.tr"
    if ((margs->len != vlay->fields->len)) {
#line 1453 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1455 "src/taumir/lower.tr"
    List_i64* vals = (void*)List_i64_new();
#line 1456 "src/taumir/lower.tr"
    long long ai = 0LL;
#line 1457 "src/taumir/lower.tr"
    while ((ai < margs->len)) {
#line 1458 "src/taumir/lower.tr"
        long long ftg = List_i64_get(vlay->ftags, ai);
#line 1459 "src/taumir/lower.tr"
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
#line 1460 "src/taumir/lower.tr"
        if ((av < 0LL)) {
#line 1460 "src/taumir/lower.tr"
            List_i64_free(vals);
            return (-1LL);
        }
#line 1461 "src/taumir/lower.tr"
        long long avt = LFunc_vreg_type(lf, av);
#line 1462 "src/taumir/lower.tr"
        if ((ftg <= (0LL - 2LL))) {
#line 1464 "src/taumir/lower.tr"
            ftg = avt;
#line 1465 "src/taumir/lower.tr"
            if ((((_is_list_tag(ftg) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
#line 1465 "src/taumir/lower.tr"
                List_i64_free(vals);
                return (-1LL);
            }
        }
#line 1466 "src/taumir/lower.tr"
        if ((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg))) {
#line 1466 "src/taumir/lower.tr"
            List_i64_free(vals);
            return (-1LL);
        }
#line 1467 "src/taumir/lower.tr"
        if ((ftg == 5LL)) {
#line 1468 "src/taumir/lower.tr"
            if ((avt == 0LL)) {
#line 1468 "src/taumir/lower.tr"
                av = _promote_f(lf, av);
            } else if ((avt != 5LL)) {
#line 1469 "src/taumir/lower.tr"
                List_i64_free(vals);
                return (-1LL);
            }
#line 1470 "src/taumir/lower.tr"
            long long pfb = LFunc_new_vreg(lf);
#line 1471 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFBits(pfb, av));
#line 1472 "src/taumir/lower.tr"
            av = pfb;
        } else if ((ftg == 1LL)) {
#line 1474 "src/taumir/lower.tr"
            if ((avt != 1LL)) {
#line 1474 "src/taumir/lower.tr"
                List_i64_free(vals);
                return (-1LL);
            }
#line 1475 "src/taumir/lower.tr"
            _secure_str(m, lf, av);
        } else if (((ftg == 10LL) || (ftg == 11LL))) {
#line 1477 "src/taumir/lower.tr"
            if ((!_field_tag_ok(avt, ftg))) {
#line 1477 "src/taumir/lower.tr"
                List_i64_free(vals);
                return (-1LL);
            }
#line 1478 "src/taumir/lower.tr"
            _secure_obj(m, lf, av);
        } else if ((!_field_tag_ok(avt, ftg))) {
#line 1480 "src/taumir/lower.tr"
            List_i64_free(vals);
            return (-1LL);
        }
#line 1481 "src/taumir/lower.tr"
        List_i64_append(vals, av);
#line 1482 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 1483 "src/taumir/lower.tr"
    long long szc = LFunc_new_vreg(lf);
#line 1484 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_enum_size(m, ename)));
#line 1485 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 1486 "src/taumir/lower.tr"
    List_i64* oaa = (void*)List_i64_new();
#line 1487 "src/taumir/lower.tr"
    List_i64_append(oaa, szc);
#line 1488 "src/taumir/lower.tr"
    long long od = LFunc_new_vreg(lf);
#line 1489 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), oaa));
#line 1490 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, od, 11LL);
#line 1491 "src/taumir/lower.tr"
    long long tgc = LFunc_new_vreg(lf);
#line 1492 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(tgc, vidx));
#line 1493 "src/taumir/lower.tr"
    _emit_field_set(m, lf, od, 0LL, tgc);
#line 1494 "src/taumir/lower.tr"
    ai = 0LL;
#line 1495 "src/taumir/lower.tr"
    while ((ai < vals->len)) {
#line 1496 "src/taumir/lower.tr"
        _emit_field_set(m, lf, od, ((1LL + ai) * 8LL), List_i64_get(vals, ai));
#line 1497 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 1498 "src/taumir/lower.tr"
    _fresh_mark_obj(lf, od);
#line 1499 "src/taumir/lower.tr"
    long long _trret1509 = od;
    List_i64_free(vals);
    return _trret1509;
}

__attribute__((hot)) long long _lir_fn_ret_tag(LModule* m, HirFunction* f) {
#line 1504 "src/taumir/lower.tr"
    if (((!_is_null_str(f->throws_ty->name)) && (!_tr_str_eqv((f->throws_ty->name), (_tr_str_lit_len("", 0LL)))))) {
#line 1504 "src/taumir/lower.tr"
        return 11LL;
    }
#line 1505 "src/taumir/lower.tr"
    return _tag_of(m, f->ret_ty);
}

__attribute__((hot)) long long _wrap_result(LModule* m, LFunc* lf, long long vidx, long long payv, long long paytag) {
#line 1511 "src/taumir/lower.tr"
    long long pv = payv;
#line 1512 "src/taumir/lower.tr"
    if ((paytag == 5LL)) {
#line 1513 "src/taumir/lower.tr"
        long long fb = LFunc_new_vreg(lf);
#line 1514 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IFBits(fb, pv));
#line 1515 "src/taumir/lower.tr"
        pv = fb;
    } else if ((paytag == 1LL)) {
#line 1517 "src/taumir/lower.tr"
        _secure_str(m, lf, pv);
    } else if (((paytag == 10LL) || (paytag == 11LL))) {
#line 1519 "src/taumir/lower.tr"
        _secure_obj(m, lf, pv);
    }
#line 1520 "src/taumir/lower.tr"
    long long szc = LFunc_new_vreg(lf);
#line 1521 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_enum_size(m, _tr_str_lit_len("Result", 6LL))));
#line 1522 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 1523 "src/taumir/lower.tr"
    List_i64* aa = (void*)List_i64_new();
#line 1524 "src/taumir/lower.tr"
    List_i64_append(aa, szc);
#line 1525 "src/taumir/lower.tr"
    long long od = LFunc_new_vreg(lf);
#line 1526 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), aa));
#line 1527 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, od, 11LL);
#line 1528 "src/taumir/lower.tr"
    long long tgc = LFunc_new_vreg(lf);
#line 1529 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(tgc, vidx));
#line 1530 "src/taumir/lower.tr"
    _emit_field_set(m, lf, od, 0LL, tgc);
#line 1531 "src/taumir/lower.tr"
    _emit_field_set(m, lf, od, 8LL, pv);
#line 1532 "src/taumir/lower.tr"
    _fresh_mark_obj(lf, od);
#line 1533 "src/taumir/lower.tr"
    return od;
}

__attribute__((hot)) long long _find_free_fn_idx(LModule* m, TrStr name) {
#line 1542 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1543 "src/taumir/lower.tr"
    while ((i < m->hir_prog->functions->len)) {
#line 1544 "src/taumir/lower.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(m->hir_prog->functions, i));
#line 1545 "src/taumir/lower.tr"
        if ((((_tr_str_eqv((f->name), (name)) && _tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) && (!f->is_extern)) && (f->generics->len == 0LL))) {
#line 1545 "src/taumir/lower.tr"
            long long _trret1510 = i;
            return _trret1510;
        }
#line 1546 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1547 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _ensure_await_wrapper(LModule* m, TrStr fn) {
#line 1550 "src/taumir/lower.tr"
    TrStr wname = _tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn));
#line 1551 "src/taumir/lower.tr"
    if (LModule_is_user_fn(m, wname)) {
#line 1551 "src/taumir/lower.tr"
        _tr_str_release(wname);
        return true;
    }
#line 1552 "src/taumir/lower.tr"
    long long fidx = _find_free_fn_idx(m, fn);
#line 1553 "src/taumir/lower.tr"
    if ((fidx < 0LL)) {
#line 1553 "src/taumir/lower.tr"
        _tr_str_release(wname);
        return false;
    }
#line 1554 "src/taumir/lower.tr"
    HirFunction* hf = ((HirFunction*)List_ptr_get(m->hir_prog->functions, fidx));
#line 1555 "src/taumir/lower.tr"
    long long frtag = _tag_of(m, hf->ret_ty);
#line 1556 "src/taumir/lower.tr"
    if ((frtag < 0LL)) {
#line 1556 "src/taumir/lower.tr"
        _tr_str_release(wname);
        return false;
    }
#line 1557 "src/taumir/lower.tr"
    ({ TrStr _ord_t6217 = (_own(wname)); List_TrStr_append(m->fn_names, _ord_t6217); _tr_str_release(_ord_t6217); });
#line 1558 "src/taumir/lower.tr"
    List_i64_append(m->fn_ret, 0LL);
#line 1559 "src/taumir/lower.tr"
    LFunc* wlf = ({ TrStr _at_t6218 = (_own(wname)); __auto_type _wr = (LFunc_init(_at_t6218)); _tr_str_release(_at_t6218); _wr; });
#line 1560 "src/taumir/lower.tr"
    List_TrStr_append(wlf->params, _tr_str_lit_len("__vp", 4LL));
#line 1561 "src/taumir/lower.tr"
    LFunc_add_var(wlf, _tr_str_lit_len("__vp", 4LL));
#line 1562 "src/taumir/lower.tr"
    LFunc_set_var_type(wlf, _tr_str_lit_len("__vp", 4LL), 0LL);
#line 1563 "src/taumir/lower.tr"
    LFunc_set_cur(wlf, LFunc_new_block(wlf));
#line 1564 "src/taumir/lower.tr"
    List_i64* callargs = (void*)List_i64_new();
#line 1565 "src/taumir/lower.tr"
    long long pi = 0LL;
#line 1566 "src/taumir/lower.tr"
    while ((pi < hf->params->len)) {
#line 1567 "src/taumir/lower.tr"
        long long ptag = _tag_of(m, ((HirParam*)List_ptr_get(hf->params, pi))->ty);
#line 1568 "src/taumir/lower.tr"
        if ((ptag < 0LL)) {
#line 1568 "src/taumir/lower.tr"
            _tr_str_release(wname);
            _tr_obj_release(wlf, _trdrop_LFunc);
            List_i64_free(callargs);
            return false;
        }
#line 1569 "src/taumir/lower.tr"
        long long vpv = LFunc_new_vreg(wlf);
#line 1570 "src/taumir/lower.tr"
        LFunc_emit(wlf, LInst_ctor_ILoadVar(vpv, _tr_str_lit_len("__vp", 4LL)));
#line 1571 "src/taumir/lower.tr"
        LFunc_set_vreg_type(wlf, vpv, 0LL);
#line 1572 "src/taumir/lower.tr"
        long long argi = _emit_field_get(m, wlf, vpv, (pi * 8LL), ptag);
#line 1573 "src/taumir/lower.tr"
        if ((ptag == 5LL)) {
#line 1574 "src/taumir/lower.tr"
            long long fb = LFunc_new_vreg(wlf);
#line 1575 "src/taumir/lower.tr"
            LFunc_emit(wlf, LInst_ctor_IBitsF(fb, argi));
#line 1576 "src/taumir/lower.tr"
            LFunc_set_vreg_type(wlf, fb, 5LL);
#line 1577 "src/taumir/lower.tr"
            argi = fb;
        }
#line 1578 "src/taumir/lower.tr"
        List_i64_append(callargs, argi);
#line 1579 "src/taumir/lower.tr"
        pi = (pi + 1LL);
    }
#line 1580 "src/taumir/lower.tr"
    if ((callargs->len > 6LL)) {
#line 1580 "src/taumir/lower.tr"
        _tr_str_release(wname);
        _tr_obj_release(wlf, _trdrop_LFunc);
        List_i64_free(callargs);
        return false;
    }
#line 1581 "src/taumir/lower.tr"
    long long rd = LFunc_new_vreg(wlf);
#line 1582 "src/taumir/lower.tr"
    ({ TrStr _at_t6219 = (_own(fn)); LFunc_emit(wlf, LInst_ctor_ICall(rd, _at_t6219, callargs)); _tr_str_release(_at_t6219); });
#line 1583 "src/taumir/lower.tr"
    LFunc_set_vreg_type(wlf, rd, frtag);
#line 1584 "src/taumir/lower.tr"
    long long vpf = LFunc_new_vreg(wlf);
#line 1585 "src/taumir/lower.tr"
    LFunc_emit(wlf, LInst_ctor_ILoadVar(vpf, _tr_str_lit_len("__vp", 4LL)));
#line 1586 "src/taumir/lower.tr"
    LFunc_set_vreg_type(wlf, vpf, 0LL);
#line 1587 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_free", 15LL));
#line 1588 "src/taumir/lower.tr"
    List_i64* fa = (void*)List_i64_new();
#line 1589 "src/taumir/lower.tr"
    List_i64_append(fa, vpf);
#line 1590 "src/taumir/lower.tr"
    LFunc_emit(wlf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_raw_free", 15LL), fa));
#line 1591 "src/taumir/lower.tr"
    long long ret = rd;
#line 1592 "src/taumir/lower.tr"
    if ((frtag == 5LL)) {
#line 1593 "src/taumir/lower.tr"
        long long rb = LFunc_new_vreg(wlf);
#line 1594 "src/taumir/lower.tr"
        LFunc_emit(wlf, LInst_ctor_IFBits(rb, rd));
#line 1595 "src/taumir/lower.tr"
        ret = rb;
    }
#line 1596 "src/taumir/lower.tr"
    LFunc_set_term(wlf, LTerm_ctor_TRetVal(ret));
#line 1597 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(wlf));
#line 1598 "src/taumir/lower.tr"
    _tr_str_release(wname);
    _tr_obj_release(wlf, _trdrop_LFunc);
    return true;
}

__attribute__((hot)) long long _lower_await(LModule* m, LFunc* lf, HirExpr* awexpr) {
#line 1601 "src/taumir/lower.tr"
    __auto_type _t6220 = (*awexpr);
    if (_t6220.tag == HirExpr_ECall) {
        __auto_type callee = _t6220.data.ECall.callee;
__auto_type args = _t6220.data.ECall.args;
__auto_type callty = _t6220.data.ECall.ty;
#line 1603 "src/taumir/lower.tr"
        TrStr fn = _ident_name(callee);
#line 1604 "src/taumir/lower.tr"
        if (((_is_null_str(fn) || _tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, fn)))) {
#line 1605 "src/taumir/lower.tr"
            long long _trret1511 = lower_expr(m, lf, awexpr);
            _tr_str_release(fn);
            return _trret1511;
        }
#line 1606 "src/taumir/lower.tr"
        if ((!_ensure_await_wrapper(m, fn))) {
#line 1606 "src/taumir/lower.tr"
            _tr_str_release(fn);
            return (-1LL);
        }
#line 1607 "src/taumir/lower.tr"
        long long nargs = args->len;
#line 1608 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
#line 1609 "src/taumir/lower.tr"
        long long packsz = (nargs * 8LL);
#line 1610 "src/taumir/lower.tr"
        if ((packsz < 8LL)) {
#line 1610 "src/taumir/lower.tr"
            packsz = 8LL;
        }
#line 1611 "src/taumir/lower.tr"
        long long szc = LFunc_new_vreg(lf);
#line 1612 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
#line 1613 "src/taumir/lower.tr"
        List_i64* aa = (void*)List_i64_new();
#line 1614 "src/taumir/lower.tr"
        List_i64_append(aa, szc);
#line 1615 "src/taumir/lower.tr"
        long long ab = LFunc_new_vreg(lf);
#line 1616 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(ab, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
#line 1617 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, ab, 0LL);
#line 1618 "src/taumir/lower.tr"
        long long ai = 0LL;
#line 1619 "src/taumir/lower.tr"
        while ((ai < nargs)) {
#line 1620 "src/taumir/lower.tr"
            long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
#line 1621 "src/taumir/lower.tr"
            if ((argv < 0LL)) {
#line 1621 "src/taumir/lower.tr"
                _tr_str_release(fn);
                return (-1LL);
            }
#line 1622 "src/taumir/lower.tr"
            long long argt = LFunc_vreg_type(lf, argv);
#line 1623 "src/taumir/lower.tr"
            if ((argt == 5LL)) {
#line 1624 "src/taumir/lower.tr"
                long long afb = LFunc_new_vreg(lf);
#line 1625 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
#line 1626 "src/taumir/lower.tr"
                argv = afb;
            } else if ((argt == 1LL)) {
#line 1627 "src/taumir/lower.tr"
                _secure_str(m, lf, argv);
            } else if (((argt == 10LL) || (argt == 11LL))) {
#line 1628 "src/taumir/lower.tr"
                _secure_obj(m, lf, argv);
            }
#line 1629 "src/taumir/lower.tr"
            _emit_field_set(m, lf, ab, (ai * 8LL), argv);
#line 1630 "src/taumir/lower.tr"
            ai = (ai + 1LL);
        }
#line 1631 "src/taumir/lower.tr"
        long long faddr = LFunc_new_vreg(lf);
#line 1632 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, faddr, 12LL);
#line 1633 "src/taumir/lower.tr"
        ({ TrStr _at_t6221 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t6221)); _tr_str_release(_at_t6221); });
#line 1634 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_go_h", 11LL));
#line 1635 "src/taumir/lower.tr"
        List_i64* ga = (void*)List_i64_new();
#line 1636 "src/taumir/lower.tr"
        List_i64_append(ga, faddr);
#line 1637 "src/taumir/lower.tr"
        List_i64_append(ga, ab);
#line 1638 "src/taumir/lower.tr"
        long long co = LFunc_new_vreg(lf);
#line 1639 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(co, _tr_str_lit_len("_tr_co_go_h", 11LL), ga));
#line 1640 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, co, 0LL);
#line 1641 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_await_h", 14LL));
#line 1642 "src/taumir/lower.tr"
        List_i64* wa = (void*)List_i64_new();
#line 1643 "src/taumir/lower.tr"
        List_i64_append(wa, co);
#line 1644 "src/taumir/lower.tr"
        long long r = LFunc_new_vreg(lf);
#line 1645 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(r, _tr_str_lit_len("_tr_co_await_h", 14LL), wa));
#line 1646 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_co_free_h", 13LL));
#line 1647 "src/taumir/lower.tr"
        List_i64* fca = (void*)List_i64_new();
#line 1648 "src/taumir/lower.tr"
        List_i64_append(fca, co);
#line 1649 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_free_h", 13LL), fca));
#line 1650 "src/taumir/lower.tr"
        long long rtag = _tag_of(m, callty);
#line 1651 "src/taumir/lower.tr"
        if ((rtag < 0LL)) {
#line 1651 "src/taumir/lower.tr"
            rtag = 0LL;
        }
#line 1652 "src/taumir/lower.tr"
        if ((rtag == 5LL)) {
#line 1653 "src/taumir/lower.tr"
            long long rf = LFunc_new_vreg(lf);
#line 1654 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBitsF(rf, r));
#line 1655 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, rf, 5LL);
#line 1656 "src/taumir/lower.tr"
            long long _trret1512 = rf;
            _tr_str_release(fn);
            return _trret1512;
        }
#line 1657 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, r, rtag);
#line 1658 "src/taumir/lower.tr"
        long long _trret1513 = r;
        _tr_str_release(fn);
        return _trret1513;
    } else if (1) {
        __auto_type _ = _t6220;
#line 1660 "src/taumir/lower.tr"
        return lower_expr(m, lf, awexpr);
    }
}

__attribute__((hot)) bool _lower_spawn(LModule* m, LFunc* lf, HirExpr* expr) {
#line 1668 "src/taumir/lower.tr"
    __auto_type _t6222 = (*expr);
    if (_t6222.tag == HirExpr_ECall) {
        __auto_type callee = _t6222.data.ECall.callee;
__auto_type args = _t6222.data.ECall.args;
#line 1670 "src/taumir/lower.tr"
        TrStr fn = _ident_name(callee);
#line 1671 "src/taumir/lower.tr"
        if (((_is_null_str(fn) || _tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, fn)))) {
#line 1671 "src/taumir/lower.tr"
            _tr_str_release(fn);
            return false;
        }
#line 1672 "src/taumir/lower.tr"
        if ((!_ensure_await_wrapper(m, fn))) {
#line 1672 "src/taumir/lower.tr"
            _tr_str_release(fn);
            return false;
        }
#line 1673 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
#line 1674 "src/taumir/lower.tr"
        long long packsz = (args->len * 8LL);
#line 1675 "src/taumir/lower.tr"
        if ((packsz < 8LL)) {
#line 1675 "src/taumir/lower.tr"
            packsz = 8LL;
        }
#line 1676 "src/taumir/lower.tr"
        long long szc = LFunc_new_vreg(lf);
#line 1677 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
#line 1678 "src/taumir/lower.tr"
        List_i64* aa = (void*)List_i64_new();
#line 1679 "src/taumir/lower.tr"
        List_i64_append(aa, szc);
#line 1680 "src/taumir/lower.tr"
        long long buf = LFunc_new_vreg(lf);
#line 1681 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(buf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
#line 1682 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, buf, 0LL);
#line 1683 "src/taumir/lower.tr"
        long long ai = 0LL;
#line 1684 "src/taumir/lower.tr"
        while ((ai < args->len)) {
#line 1685 "src/taumir/lower.tr"
            long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
#line 1686 "src/taumir/lower.tr"
            if ((argv < 0LL)) {
#line 1686 "src/taumir/lower.tr"
                _tr_str_release(fn);
                return false;
            }
#line 1687 "src/taumir/lower.tr"
            long long argt = LFunc_vreg_type(lf, argv);
#line 1688 "src/taumir/lower.tr"
            if ((argt == 5LL)) {
#line 1689 "src/taumir/lower.tr"
                long long afb = LFunc_new_vreg(lf);
#line 1690 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
#line 1691 "src/taumir/lower.tr"
                argv = afb;
            } else if ((argt == 1LL)) {
#line 1692 "src/taumir/lower.tr"
                _secure_str(m, lf, argv);
            } else if (((argt == 10LL) || (argt == 11LL))) {
#line 1693 "src/taumir/lower.tr"
                _secure_obj(m, lf, argv);
            }
#line 1694 "src/taumir/lower.tr"
            _emit_field_set(m, lf, buf, (ai * 8LL), argv);
#line 1695 "src/taumir/lower.tr"
            ai = (ai + 1LL);
        }
#line 1696 "src/taumir/lower.tr"
        long long faddr = LFunc_new_vreg(lf);
#line 1697 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, faddr, 12LL);
#line 1698 "src/taumir/lower.tr"
        ({ TrStr _at_t6223 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (fn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(faddr, _at_t6223)); _tr_str_release(_at_t6223); });
#line 1699 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_thread_start", 16LL));
#line 1700 "src/taumir/lower.tr"
        List_i64* sa = (void*)List_i64_new();
#line 1701 "src/taumir/lower.tr"
        List_i64_append(sa, faddr);
#line 1702 "src/taumir/lower.tr"
        List_i64_append(sa, buf);
#line 1703 "src/taumir/lower.tr"
        long long handle = LFunc_new_vreg(lf);
#line 1704 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(handle, _tr_str_lit_len("_tr_thread_start", 16LL), sa));
#line 1705 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, handle, 0LL);
#line 1706 "src/taumir/lower.tr"
        if ((lf->in_taskgroup > 0LL)) {
#line 1707 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_tg_push", 11LL));
#line 1708 "src/taumir/lower.tr"
            List_i64* pa = (void*)List_i64_new();
#line 1709 "src/taumir/lower.tr"
            List_i64_append(pa, handle);
#line 1710 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_push", 11LL), pa));
        } else {
#line 1712 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_thread_detach", 17LL));
#line 1713 "src/taumir/lower.tr"
            List_i64* da = (void*)List_i64_new();
#line 1714 "src/taumir/lower.tr"
            List_i64_append(da, handle);
#line 1715 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_thread_detach", 17LL), da));
        }
#line 1716 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 1717 "src/taumir/lower.tr"
        _tr_str_release(fn);
        return true;
    } else if (1) {
        __auto_type _ = _t6222;
#line 1719 "src/taumir/lower.tr"
        return false;
    }
}

__attribute__((hot)) long long _box_call(LModule* m, LFunc* lf, long long selfv, TrStr rtfn, List_ptr* margs, long long nargs, long long rtag) {
#line 1724 "src/taumir/lower.tr"
    if ((margs->len != nargs)) {
#line 1724 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1725 "src/taumir/lower.tr"
    LModule_add_extern(m, rtfn);
#line 1726 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 1727 "src/taumir/lower.tr"
    List_i64_append(a, selfv);
#line 1728 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1729 "src/taumir/lower.tr"
    while ((i < nargs)) {
#line 1730 "src/taumir/lower.tr"
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, i)));
#line 1731 "src/taumir/lower.tr"
        if (((av < 0LL) || (LFunc_vreg_type(lf, av) != 0LL))) {
#line 1731 "src/taumir/lower.tr"
            List_i64_free(a);
            return (-1LL);
        }
#line 1732 "src/taumir/lower.tr"
        List_i64_append(a, av);
#line 1733 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1734 "src/taumir/lower.tr"
    if ((rtag < 0LL)) {
#line 1735 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), rtfn, a));
#line 1736 "src/taumir/lower.tr"
        return selfv;
    }
#line 1737 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 1738 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, rtfn, a));
#line 1739 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, rtag);
#line 1740 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _pack_spawn_args(LModule* m, LFunc* lf, List_ptr* margs, long long start) {
#line 1745 "src/taumir/lower.tr"
    long long nargs = (margs->len - start);
#line 1746 "src/taumir/lower.tr"
    long long packsz = (nargs * 8LL);
#line 1747 "src/taumir/lower.tr"
    if ((packsz < 8LL)) {
#line 1747 "src/taumir/lower.tr"
        packsz = 8LL;
    }
#line 1748 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
#line 1749 "src/taumir/lower.tr"
    long long szc = LFunc_new_vreg(lf);
#line 1750 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(szc, packsz));
#line 1751 "src/taumir/lower.tr"
    List_i64* aa = (void*)List_i64_new();
#line 1752 "src/taumir/lower.tr"
    List_i64_append(aa, szc);
#line 1753 "src/taumir/lower.tr"
    long long buf = LFunc_new_vreg(lf);
#line 1754 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(buf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aa));
#line 1755 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, buf, 0LL);
#line 1756 "src/taumir/lower.tr"
    long long ai = start;
#line 1757 "src/taumir/lower.tr"
    while ((ai < margs->len)) {
#line 1758 "src/taumir/lower.tr"
        long long argv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
#line 1759 "src/taumir/lower.tr"
        if ((argv < 0LL)) {
#line 1759 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 1760 "src/taumir/lower.tr"
        long long argt = LFunc_vreg_type(lf, argv);
#line 1761 "src/taumir/lower.tr"
        if ((argt == 5LL)) {
#line 1762 "src/taumir/lower.tr"
            long long afb = LFunc_new_vreg(lf);
#line 1763 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFBits(afb, argv));
#line 1764 "src/taumir/lower.tr"
            argv = afb;
        } else if ((argt == 1LL)) {
#line 1765 "src/taumir/lower.tr"
            _secure_str(m, lf, argv);
        } else if (((argt == 10LL) || (argt == 11LL))) {
#line 1766 "src/taumir/lower.tr"
            _secure_obj(m, lf, argv);
        }
#line 1767 "src/taumir/lower.tr"
        _emit_field_set(m, lf, buf, ((ai - start) * 8LL), argv);
#line 1768 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 1769 "src/taumir/lower.tr"
    return buf;
}

__attribute__((hot)) long long _lower_pool_spawn(LModule* m, LFunc* lf, long long poolv, List_ptr* margs) {
#line 1774 "src/taumir/lower.tr"
    if ((margs->len != 2LL)) {
#line 1774 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1775 "src/taumir/lower.tr"
    TrStr pfn = _ident_name(((HirExpr*)List_ptr_get(margs, 0LL)));
#line 1776 "src/taumir/lower.tr"
    if (((_is_null_str(pfn) || _tr_str_eqv((pfn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, pfn)))) {
#line 1776 "src/taumir/lower.tr"
        _tr_str_release(pfn);
        return (-1LL);
    }
#line 1777 "src/taumir/lower.tr"
    if ((!_ensure_await_wrapper(m, pfn))) {
#line 1777 "src/taumir/lower.tr"
        _tr_str_release(pfn);
        return (-1LL);
    }
#line 1778 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
#line 1779 "src/taumir/lower.tr"
    long long pszc = LFunc_new_vreg(lf);
#line 1780 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(pszc, 8LL));
#line 1781 "src/taumir/lower.tr"
    List_i64* paa = (void*)List_i64_new();
#line 1782 "src/taumir/lower.tr"
    List_i64_append(paa, pszc);
#line 1783 "src/taumir/lower.tr"
    long long pbuf = LFunc_new_vreg(lf);
#line 1784 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(pbuf, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), paa));
#line 1785 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, pbuf, 0LL);
#line 1786 "src/taumir/lower.tr"
    long long parg = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 1787 "src/taumir/lower.tr"
    if (((parg < 0LL) || (LFunc_vreg_type(lf, parg) != 0LL))) {
#line 1787 "src/taumir/lower.tr"
        _tr_str_release(pfn);
        return (-1LL);
    }
#line 1788 "src/taumir/lower.tr"
    _emit_field_set(m, lf, pbuf, 0LL, parg);
#line 1789 "src/taumir/lower.tr"
    long long pfaddr = LFunc_new_vreg(lf);
#line 1790 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, pfaddr, 12LL);
#line 1791 "src/taumir/lower.tr"
    ({ TrStr _at_t6224 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (pfn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(pfaddr, _at_t6224)); _tr_str_release(_at_t6224); });
#line 1792 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_threadpool_spawn", 20LL));
#line 1793 "src/taumir/lower.tr"
    List_i64* psa = (void*)List_i64_new();
#line 1794 "src/taumir/lower.tr"
    List_i64_append(psa, poolv);
#line 1795 "src/taumir/lower.tr"
    List_i64_append(psa, pfaddr);
#line 1796 "src/taumir/lower.tr"
    List_i64_append(psa, pbuf);
#line 1797 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_threadpool_spawn", 20LL), psa));
#line 1798 "src/taumir/lower.tr"
    long long _trret1514 = poolv;
    _tr_str_release(pfn);
    return _trret1514;
}

__attribute__((hot)) bool _is_runtime_fn(TrStr fn) {
#line 1802 "src/taumir/lower.tr"
    if ((_is_null_str(fn) || (((long long)_tr_str_lenv((fn))) < 4LL))) {
#line 1802 "src/taumir/lower.tr"
        return false;
    }
#line 1803 "src/taumir/lower.tr"
    return ({ TrStr _cmpa_t6225 = (_tr_str_slicev((fn), 0LL, 4LL)); TrStr _cmpb_t6226 = (_tr_str_lit_len("_tr_", 4LL)); _Bool _cmpr_t6227 = _tr_str_eqv(_cmpa_t6225, _cmpb_t6226); _tr_str_release(_cmpa_t6225); _cmpr_t6227; });
}

__attribute__((hot)) bool _is_rc_str_runtime_fn(TrStr fn) {
#line 1809 "src/taumir/lower.tr"
    if ((_is_null_str(fn) || (((long long)_tr_str_lenv((fn))) < 7LL))) {
#line 1809 "src/taumir/lower.tr"
        return false;
    }
#line 1810 "src/taumir/lower.tr"
    return ({ TrStr _cmpa_t6228 = (_tr_str_slicev((fn), 0LL, 7LL)); TrStr _cmpb_t6229 = (_tr_str_lit_len("_tr_rt_", 7LL)); _Bool _cmpr_t6230 = _tr_str_eqv(_cmpa_t6228, _cmpb_t6229); _tr_str_release(_cmpa_t6228); _cmpr_t6230; });
}

__attribute__((hot)) void _track_mutex_unlock(LFunc* lf, HirExpr* obj) {
#line 1814 "src/taumir/lower.tr"
    TrStr nm = _ident_name(obj);
#line 1815 "src/taumir/lower.tr"
    if ((_is_null_str(nm) || _tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
#line 1815 "src/taumir/lower.tr"
        _tr_str_release(nm);
        return;
    }
#line 1816 "src/taumir/lower.tr"
    long long i = 0LL;
#line 1817 "src/taumir/lower.tr"
    while ((i < lf->mutex_unlocks->len)) {
#line 1818 "src/taumir/lower.tr"
        if (({ TrStr _cmpa_t6231 = (List_TrStr_get(lf->mutex_unlocks, i)); TrStr _cmpb_t6232 = (nm); _Bool _cmpr_t6233 = _tr_str_eqv(_cmpa_t6231, _cmpb_t6232); _tr_str_release(_cmpa_t6231); _cmpr_t6233; })) {
#line 1818 "src/taumir/lower.tr"
            _tr_str_release(nm);
            return;
        }
#line 1819 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 1820 "src/taumir/lower.tr"
    ({ TrStr _ord_t6234 = (_own(nm)); List_TrStr_append(lf->mutex_unlocks, _ord_t6234); _tr_str_release(_ord_t6234); });
    _tr_str_release(nm);
}

__attribute__((hot)) long long _atomic_delta(LModule* m, LFunc* lf, long long selfv, TrStr rtfn) {
#line 1824 "src/taumir/lower.tr"
    LModule_add_extern(m, rtfn);
#line 1825 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 1826 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 1827 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 1828 "src/taumir/lower.tr"
    List_i64_append(a, selfv);
#line 1829 "src/taumir/lower.tr"
    List_i64_append(a, one);
#line 1830 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 1831 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, rtfn, a));
#line 1832 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, 0LL);
#line 1833 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _lower_box_method(LModule* m, LFunc* lf, HirExpr* obj, TrStr method, List_ptr* margs) {
#line 1838 "src/taumir/lower.tr"
    TrStr btn = _tr_str_retain(hir_expr_type(obj)->name);
#line 1839 "src/taumir/lower.tr"
    if (((((((!_tr_str_eqv((btn), (_tr_str_lit_len("Mutex", 5LL)))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("RwLock", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("Atomic", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("ThreadPool", 10LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL))))) && (!_tr_str_eqv((btn), (_tr_str_lit_len("ThreadLocal", 11LL)))))) {
#line 1839 "src/taumir/lower.tr"
        _tr_str_release(btn);
        return (-2LL);
    }
#line 1843 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) {
#line 1843 "src/taumir/lower.tr"
        _tr_str_release(btn);
        return (-2LL);
    }
#line 1844 "src/taumir/lower.tr"
    if ((_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL))) && ((_tr_str_eqv((method), (_tr_str_lit_len("id", 2LL))) || _tr_str_eqv((method), (_tr_str_lit_len("current_id", 10LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL)))))) {
#line 1844 "src/taumir/lower.tr"
        _tr_str_release(btn);
        return (-2LL);
    }
#line 1845 "src/taumir/lower.tr"
    long long bself = lower_expr(m, lf, obj);
#line 1846 "src/taumir/lower.tr"
    if ((bself < 0LL)) {
#line 1846 "src/taumir/lower.tr"
        _tr_str_release(btn);
        return (-1LL);
    }
#line 1847 "src/taumir/lower.tr"
    if (_tr_str_eqv((btn), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 1848 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("lock", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
#line 1849 "src/taumir/lower.tr"
            _track_mutex_unlock(lf, obj);
#line 1850 "src/taumir/lower.tr"
            long long _trret1515 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_lock_get", 21LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1515;
        }
#line 1851 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) {
#line 1851 "src/taumir/lower.tr"
            long long _trret1516 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_set_unlock", 23LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1516;
        }
#line 1852 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("unlock", 6LL)))) {
#line 1852 "src/taumir/lower.tr"
            long long _trret1517 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_mutexbox_unlock", 19LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1517;
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 1854 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL)))) {
#line 1854 "src/taumir/lower.tr"
            long long _trret1549 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_read_get", 19LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1549;
        }
#line 1855 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("read_unlock", 11LL)))) {
#line 1855 "src/taumir/lower.tr"
            long long _trret1550 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_read_unlock", 22LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1550;
        }
#line 1856 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL)))) {
#line 1856 "src/taumir/lower.tr"
            long long _trret1551 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_write_get", 20LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1551;
        }
#line 1857 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("write_set", 9LL)))) {
#line 1857 "src/taumir/lower.tr"
            long long _trret1552 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_rwlbox_write_set_unlock", 27LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1552;
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 1859 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("load", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) {
#line 1859 "src/taumir/lower.tr"
            long long _trret1527 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load", 15LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1527;
        }
#line 1860 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("store", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))))) {
#line 1860 "src/taumir/lower.tr"
            long long _trret1528 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store", 16LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1528;
        }
#line 1861 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_add", 9LL))))) {
#line 1861 "src/taumir/lower.tr"
            long long _trret1529 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add", 14LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1529;
        }
#line 1862 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("sub", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fetch_sub", 9LL))))) {
#line 1862 "src/taumir/lower.tr"
            long long _trret1530 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub", 14LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1530;
        }
#line 1863 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("cas", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("compare_and_swap", 16LL))))) {
#line 1863 "src/taumir/lower.tr"
            long long _trret1531 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas", 14LL), margs, 2LL, 4LL);
            _tr_str_release(btn);
            return _trret1531;
        }
#line 1864 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("increment", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("inc", 3LL))))) {
#line 1864 "src/taumir/lower.tr"
            long long _trret1532 = _atomic_delta(m, lf, bself, _tr_str_lit_len("_tr_atomic_add", 14LL));
            _tr_str_release(btn);
            return _trret1532;
        }
#line 1865 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("decrement", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("dec", 3LL))))) {
#line 1865 "src/taumir/lower.tr"
            long long _trret1533 = _atomic_delta(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub", 14LL));
            _tr_str_release(btn);
            return _trret1533;
        }
#line 1867 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_acquire", 12LL)))) {
#line 1867 "src/taumir/lower.tr"
            long long _trret1534 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_acquire", 23LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1534;
        }
#line 1868 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_relaxed", 12LL)))) {
#line 1868 "src/taumir/lower.tr"
            long long _trret1535 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_relaxed", 23LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1535;
        }
#line 1869 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("load_seqcst", 11LL)))) {
#line 1869 "src/taumir/lower.tr"
            long long _trret1536 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_load_seqcst", 22LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1536;
        }
#line 1870 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_release", 13LL)))) {
#line 1870 "src/taumir/lower.tr"
            long long _trret1537 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_release", 24LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1537;
        }
#line 1871 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_relaxed", 13LL)))) {
#line 1871 "src/taumir/lower.tr"
            long long _trret1538 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_relaxed", 24LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1538;
        }
#line 1872 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("store_seqcst", 12LL)))) {
#line 1872 "src/taumir/lower.tr"
            long long _trret1539 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_store_seqcst", 23LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1539;
        }
#line 1873 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_relaxed", 11LL)))) {
#line 1873 "src/taumir/lower.tr"
            long long _trret1540 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_relaxed", 22LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1540;
        }
#line 1874 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_release", 11LL)))) {
#line 1874 "src/taumir/lower.tr"
            long long _trret1541 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_release", 22LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1541;
        }
#line 1875 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("add_acqrel", 10LL)))) {
#line 1875 "src/taumir/lower.tr"
            long long _trret1542 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_add_acqrel", 21LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1542;
        }
#line 1876 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("sub_relaxed", 11LL)))) {
#line 1876 "src/taumir/lower.tr"
            long long _trret1543 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub_relaxed", 22LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1543;
        }
#line 1877 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("sub_release", 11LL)))) {
#line 1877 "src/taumir/lower.tr"
            long long _trret1544 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_sub_release", 22LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1544;
        }
#line 1878 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("exchange", 8LL))))) {
#line 1878 "src/taumir/lower.tr"
            long long _trret1545 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_swap", 15LL), margs, 1LL, 0LL);
            _tr_str_release(btn);
            return _trret1545;
        }
#line 1879 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("cas_weak", 8LL)))) {
#line 1879 "src/taumir/lower.tr"
            long long _trret1546 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas_weak", 19LL), margs, 2LL, 4LL);
            _tr_str_release(btn);
            return _trret1546;
        }
#line 1880 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("cas_acqrel", 10LL)))) {
#line 1880 "src/taumir/lower.tr"
            long long _trret1547 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_cas_acqrel", 21LL), margs, 2LL, 4LL);
            _tr_str_release(btn);
            return _trret1547;
        }
#line 1881 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 1881 "src/taumir/lower.tr"
            long long _trret1548 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_atomic_free", 15LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1548;
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("ThreadPool", 10LL)))) {
#line 1883 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("wait", 4LL)))) {
#line 1883 "src/taumir/lower.tr"
            long long _trret1524 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadpool_wait", 19LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1524;
        }
#line 1884 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 1884 "src/taumir/lower.tr"
            long long _trret1525 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadpool_free", 19LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1525;
        }
#line 1885 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL)))) {
#line 1885 "src/taumir/lower.tr"
            long long _trret1526 = _lower_pool_spawn(m, lf, bself, margs);
            _tr_str_release(btn);
            return _trret1526;
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("Thread", 6LL)))) {
#line 1887 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL)))) {
#line 1887 "src/taumir/lower.tr"
            long long _trret1521 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_join", 18LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1521;
        }
#line 1888 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("detach", 6LL)))) {
#line 1888 "src/taumir/lower.tr"
            long long _trret1522 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_detach", 20LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1522;
        }
#line 1889 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 1889 "src/taumir/lower.tr"
            long long _trret1523 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_threadobj_free", 18LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1523;
        }
    } else if (_tr_str_eqv((btn), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
#line 1891 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) {
#line 1891 "src/taumir/lower.tr"
            long long _trret1518 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_get", 11LL), margs, 0LL, 0LL);
            _tr_str_release(btn);
            return _trret1518;
        }
#line 1892 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) {
#line 1892 "src/taumir/lower.tr"
            long long _trret1519 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_set", 11LL), margs, 1LL, (-1LL));
            _tr_str_release(btn);
            return _trret1519;
        }
#line 1893 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("free", 4LL)))) {
#line 1893 "src/taumir/lower.tr"
            long long _trret1520 = _box_call(m, lf, bself, _tr_str_lit_len("_tr_tls_free", 12LL), margs, 0LL, (-1LL));
            _tr_str_release(btn);
            return _trret1520;
        }
    }
#line 1894 "src/taumir/lower.tr"
    _tr_str_release(btn);
    return (-1LL);
}

__attribute__((hot)) bool _lower_taskgroup(LModule* m, LFunc* lf, HirBlock* body) {
#line 1900 "src/taumir/lower.tr"
    if ((lf->in_taskgroup > 0LL)) {
#line 1900 "src/taumir/lower.tr"
        return false;
    }
#line 1901 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_tg_begin", 12LL));
#line 1902 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_begin", 12LL), (void*)List_i64_new()));
#line 1903 "src/taumir/lower.tr"
    lf->in_taskgroup = (lf->in_taskgroup + 1LL);
#line 1904 "src/taumir/lower.tr"
    bool ok = lower_block(m, lf, body);
#line 1905 "src/taumir/lower.tr"
    lf->in_taskgroup = (lf->in_taskgroup - 1LL);
#line 1906 "src/taumir/lower.tr"
    if ((!ok)) {
#line 1906 "src/taumir/lower.tr"
        return false;
    }
#line 1907 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_taskgroup_wait", 18LL));
#line 1908 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_taskgroup_wait", 18LL), (void*)List_i64_new()));
#line 1909 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) long long _lower_obj_call(LModule* m, LFunc* lf, TrStr mangled, long long self_vreg, List_ptr* margs) {
#line 1915 "src/taumir/lower.tr"
    if ((!LModule_is_user_fn(m, mangled))) {
#line 1915 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1916 "src/taumir/lower.tr"
    long long rtag = LModule_fn_ret_tag(m, mangled);
#line 1917 "src/taumir/lower.tr"
    if ((rtag < 0LL)) {
#line 1917 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 1918 "src/taumir/lower.tr"
    List_i64* cargs = (void*)List_i64_new();
#line 1919 "src/taumir/lower.tr"
    if ((self_vreg >= 0LL)) {
#line 1919 "src/taumir/lower.tr"
        List_i64_append(cargs, self_vreg);
    }
#line 1920 "src/taumir/lower.tr"
    long long ai = 0LL;
#line 1921 "src/taumir/lower.tr"
    while ((ai < margs->len)) {
#line 1922 "src/taumir/lower.tr"
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, ai)));
#line 1923 "src/taumir/lower.tr"
        if ((av < 0LL)) {
#line 1923 "src/taumir/lower.tr"
            List_i64_free(cargs);
            return (-1LL);
        }
#line 1924 "src/taumir/lower.tr"
        List_i64_append(cargs, av);
#line 1925 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 1926 "src/taumir/lower.tr"
    if (({ __auto_type _ord_t6235 = (cargs->len); __auto_type _wr = ((_ord_t6235 > _arg_limit(m))); _wr; })) {
#line 1926 "src/taumir/lower.tr"
        List_i64_free(cargs);
        return (-1LL);
    }
#line 1927 "src/taumir/lower.tr"
    long long rd = LFunc_new_vreg(lf);
#line 1928 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(rd, mangled, cargs));
#line 1929 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, rd, rtag);
#line 1930 "src/taumir/lower.tr"
    if ((rtag == 1LL)) {
#line 1930 "src/taumir/lower.tr"
        _fresh_mark(lf, rd);
    }
#line 1931 "src/taumir/lower.tr"
    if ((((rtag == 10LL) || (rtag == 11LL)) && LModule_fn_ret_owned(m, mangled))) {
#line 1932 "src/taumir/lower.tr"
        _fresh_mark_obj(lf, rd);
    }
#line 1933 "src/taumir/lower.tr"
    return rd;
}

__attribute__((hot)) bool lower_block(LModule* m, LFunc* lf, HirBlock* hb) {
#line 1936 "src/taumir/lower.tr"
    lf->blk_depth = (lf->blk_depth + 1LL);
#line 1937 "src/taumir/lower.tr"
    long long si = 0LL;
#line 1938 "src/taumir/lower.tr"
    while ((si < hb->stmts->len)) {
#line 1939 "src/taumir/lower.tr"
        if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(hb->stmts, si))))) {
#line 1940 "src/taumir/lower.tr"
            lf->blk_depth = (lf->blk_depth - 1LL);
#line 1941 "src/taumir/lower.tr"
            return false;
        }
#line 1942 "src/taumir/lower.tr"
        si = (si + 1LL);
    }
#line 1943 "src/taumir/lower.tr"
    lf->blk_depth = (lf->blk_depth - 1LL);
#line 1944 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _run_defers(LModule* m, LFunc* lf) {
#line 1948 "src/taumir/lower.tr"
    if ((lf->defers->len > 0LL)) {
#line 1949 "src/taumir/lower.tr"
        if (lf->in_defer) {
#line 1949 "src/taumir/lower.tr"
            return false;
        }
#line 1950 "src/taumir/lower.tr"
        lf->in_defer = true;
#line 1951 "src/taumir/lower.tr"
        long long di = (lf->defers->len - 1LL);
#line 1952 "src/taumir/lower.tr"
        while ((di >= 0LL)) {
#line 1953 "src/taumir/lower.tr"
            if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(lf->defers, di))))) {
#line 1954 "src/taumir/lower.tr"
                lf->in_defer = false;
#line 1955 "src/taumir/lower.tr"
                return false;
            }
#line 1956 "src/taumir/lower.tr"
            di = (di - 1LL);
        }
#line 1957 "src/taumir/lower.tr"
        lf->in_defer = false;
    }
#line 1961 "src/taumir/lower.tr"
    long long mi = 0LL;
#line 1962 "src/taumir/lower.tr"
    while ((mi < lf->mutex_unlocks->len)) {
#line 1963 "src/taumir/lower.tr"
        TrStr mn = List_TrStr_get(lf->mutex_unlocks, mi);
#line 1964 "src/taumir/lower.tr"
        if ((LFunc_var_index(lf, mn) >= 0LL)) {
#line 1965 "src/taumir/lower.tr"
            long long ma = LFunc_new_vreg(lf);
#line 1966 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ma, 12LL);
#line 1967 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IAddrVar(ma, mn));
#line 1968 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_mutexbox_cleanup", 20LL));
#line 1969 "src/taumir/lower.tr"
            List_i64* mca = (void*)List_i64_new();
#line 1970 "src/taumir/lower.tr"
            List_i64_append(mca, ma);
#line 1971 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_mutexbox_cleanup", 20LL), mca));
        }
#line 1972 "src/taumir/lower.tr"
        mi = (mi + 1LL);
        _tr_str_release(mn);
    }
#line 1973 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) long long _sizeof_tyname(TrStr n) {
#line 1977 "src/taumir/lower.tr"
    if (_is_null_str(n)) {
#line 1977 "src/taumir/lower.tr"
        return 8LL;
    }
#line 1978 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL))))) {
#line 1978 "src/taumir/lower.tr"
        return 1LL;
    }
#line 1979 "src/taumir/lower.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL))))) {
#line 1979 "src/taumir/lower.tr"
        return 2LL;
    }
#line 1980 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("float32", 7LL))))) {
#line 1980 "src/taumir/lower.tr"
        return 4LL;
    }
#line 1981 "src/taumir/lower.tr"
    return 8LL;
}

__attribute__((hot)) long long _ptr_stride(LModule* m, AstType* pty) {
#line 1987 "src/taumir/lower.tr"
    if ((pty->args->len == 0LL)) {
#line 1987 "src/taumir/lower.tr"
        return 8LL;
    }
#line 1988 "src/taumir/lower.tr"
    TrStr en = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL))))->name);
#line 1989 "src/taumir/lower.tr"
    if (_is_null_str(en)) {
#line 1989 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 1990 "src/taumir/lower.tr"
    if (((((_tr_str_eqv((en), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("isize", 5LL))))) {
#line 1990 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 1991 "src/taumir/lower.tr"
    if ((_tr_str_eqv((en), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((en), (_tr_str_lit_len("f64", 3LL))))) {
#line 1991 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 1992 "src/taumir/lower.tr"
    if ((((_tr_str_eqv((en), (_tr_str_lit_len("char", 4LL))) || _tr_str_eqv((en), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("bool", 4LL))))) {
#line 1992 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 1LL;
    }
#line 1993 "src/taumir/lower.tr"
    if (((_tr_str_eqv((en), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((en), (_tr_str_lit_len("f32", 3LL))))) {
#line 1993 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 4LL;
    }
#line 1994 "src/taumir/lower.tr"
    if ((_tr_str_eqv((en), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((en), (_tr_str_lit_len("String", 6LL))))) {
#line 1994 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 1995 "src/taumir/lower.tr"
    if (LModule_is_value_class(m, en)) {
#line 1995 "src/taumir/lower.tr"
        long long _trret1553 = LModule_class_size(m, en);
        _tr_str_release(en);
        return _trret1553;
    }
#line 1996 "src/taumir/lower.tr"
    long long _pg = _value_struct_generic_size(m, (*((AstType**)List_ptr_get(pty->args, 0LL))));
#line 1997 "src/taumir/lower.tr"
    if ((_pg > 0LL)) {
#line 1997 "src/taumir/lower.tr"
        long long _trret1554 = _pg;
        _tr_str_release(en);
        return _trret1554;
    }
#line 1998 "src/taumir/lower.tr"
    if ((LModule_is_class(m, en) || LModule_is_enum(m, en))) {
#line 1998 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 1999 "src/taumir/lower.tr"
    if (_tr_str_eqv((en), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 1999 "src/taumir/lower.tr"
        _tr_str_release(en);
        return 8LL;
    }
#line 2000 "src/taumir/lower.tr"
    _tr_str_release(en);
    return 0LL;
}

__attribute__((hot)) AstType* _class_field_ty(LModule* m, TrStr cls, TrStr fld) {
#line 2011 "src/taumir/lower.tr"
    TrStr cur = _tr_str_retain(cls);
#line 2012 "src/taumir/lower.tr"
    long long depth = 0LL;
#line 2013 "src/taumir/lower.tr"
    while (((depth < 32LL) && (!_is_null_str(cur)))) {
#line 2014 "src/taumir/lower.tr"
        long long ci = 0LL;
#line 2015 "src/taumir/lower.tr"
        while ((ci < m->hir_prog->classes->len)) {
#line 2016 "src/taumir/lower.tr"
            HirClass* c = ((HirClass*)List_ptr_get(m->hir_prog->classes, ci));
#line 2017 "src/taumir/lower.tr"
            if (_tr_str_eqv((c->name), (cur))) {
#line 2018 "src/taumir/lower.tr"
                long long fi = 0LL;
#line 2019 "src/taumir/lower.tr"
                while ((fi < c->fields->len)) {
#line 2020 "src/taumir/lower.tr"
                    if (_tr_str_eqv((((HirField*)List_ptr_get(c->fields, fi))->name), (fld))) {
#line 2020 "src/taumir/lower.tr"
                        _tr_str_release(cur);
                        return ((HirField*)List_ptr_get(c->fields, fi))->ty;
                    }
#line 2021 "src/taumir/lower.tr"
                    fi = (fi + 1LL);
                }
#line 2022 "src/taumir/lower.tr"
                if ((c->base_classes->len == 1LL)) {
#line 2022 "src/taumir/lower.tr"
                    TrStr _strtmp_t6237 = ({ TrStr _at_t6236 = (List_TrStr_get(c->base_classes, 0LL)); __auto_type _wr = (_own(_at_t6236)); _tr_str_release(_at_t6236); _wr; });
                    _tr_str_release(cur);
                    cur = _strtmp_t6237;
                } else {
#line 2023 "src/taumir/lower.tr"
                    _tr_str_release(cur);
                    return AstType_init(_tr_str_lit_len("", 0LL));
                }
            }
#line 2024 "src/taumir/lower.tr"
            ci = (ci + 1LL);
        }
#line 2025 "src/taumir/lower.tr"
        depth = (depth + 1LL);
    }
#line 2026 "src/taumir/lower.tr"
    _tr_str_release(cur);
    return AstType_init(_tr_str_lit_len("", 0LL));
}

__attribute__((hot)) AstType* _ptr_chain_ty(LModule* m, HirExpr* e) {
#line 2029 "src/taumir/lower.tr"
    __auto_type _t6238 = (*e);
    if (_t6238.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t6238.data.EMethodCall.obj;
__auto_type mname = _t6238.data.EMethodCall.method;
__auto_type mty = _t6238.data.EMethodCall.ty;
#line 2031 "src/taumir/lower.tr"
        if (((_tr_str_eqv((mname), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((mname), (_tr_str_lit_len("add", 3LL)))) && _tr_str_eqv((hir_expr_type(mobj)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 2032 "src/taumir/lower.tr"
            AstType* inner = _ptr_chain_ty(m, mobj);
#line 2033 "src/taumir/lower.tr"
            if ((inner->args->len > 0LL)) {
#line 2033 "src/taumir/lower.tr"
                return inner;
            }
        }
#line 2034 "src/taumir/lower.tr"
        return mty;
    } else if (_t6238.tag == HirExpr_EPropAccess) {
        __auto_type pobj = _t6238.data.EPropAccess.obj;
__auto_type pfld = _t6238.data.EPropAccess.prop;
__auto_type pty = _t6238.data.EPropAccess.ty;
#line 2036 "src/taumir/lower.tr"
        if ((pty->args->len > 0LL)) {
#line 2036 "src/taumir/lower.tr"
            return pty;
        }
#line 2037 "src/taumir/lower.tr"
        AstType* ft = _class_field_ty(m, hir_expr_type(pobj)->name, pfld);
#line 2038 "src/taumir/lower.tr"
        if ((_tr_str_eqv((ft->name), (_tr_str_lit_len("Pointer", 7LL))) && (ft->args->len > 0LL))) {
#line 2038 "src/taumir/lower.tr"
            return ft;
        }
#line 2039 "src/taumir/lower.tr"
        return pty;
    } else if (1) {
        __auto_type _ = _t6238;
#line 2041 "src/taumir/lower.tr"
        return hir_expr_type(e);
    }
}

__attribute__((hot)) TrStr _dunder_for_op(TrStr op) {
#line 2045 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 2045 "src/taumir/lower.tr"
        return _tr_str_lit_len("__add__", 7LL);
    }
#line 2046 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 2046 "src/taumir/lower.tr"
        return _tr_str_lit_len("__sub__", 7LL);
    }
#line 2047 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 2047 "src/taumir/lower.tr"
        return _tr_str_lit_len("__mul__", 7LL);
    }
#line 2048 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
#line 2048 "src/taumir/lower.tr"
        return _tr_str_lit_len("__truediv__", 11LL);
    }
#line 2049 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
#line 2049 "src/taumir/lower.tr"
        return _tr_str_lit_len("__mod__", 7LL);
    }
#line 2050 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) {
#line 2050 "src/taumir/lower.tr"
        return _tr_str_lit_len("__pow__", 7LL);
    }
#line 2051 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) {
#line 2051 "src/taumir/lower.tr"
        return _tr_str_lit_len("__floordiv__", 12LL);
    }
#line 2052 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 2052 "src/taumir/lower.tr"
        return _tr_str_lit_len("__eq__", 6LL);
    }
#line 2053 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 2053 "src/taumir/lower.tr"
        return _tr_str_lit_len("__ne__", 6LL);
    }
#line 2054 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 2054 "src/taumir/lower.tr"
        return _tr_str_lit_len("__lt__", 6LL);
    }
#line 2055 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 2055 "src/taumir/lower.tr"
        return _tr_str_lit_len("__le__", 6LL);
    }
#line 2056 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 2056 "src/taumir/lower.tr"
        return _tr_str_lit_len("__gt__", 6LL);
    }
#line 2057 "src/taumir/lower.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
#line 2057 "src/taumir/lower.tr"
        return _tr_str_lit_len("__ge__", 6LL);
    }
#line 2058 "src/taumir/lower.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _stmt_expr_kind(HirExpr* e) {
#line 2062 "src/taumir/lower.tr"
    __auto_type _t6239 = (*e);
    if (_t6239.tag == HirExpr_ECall) {
        __auto_type callee = _t6239.data.ECall.callee;
        return ({ TrStr _at_t6240 = (_ident_name(callee)); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(_at_t6240)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("call ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_str_release(_at_t6240); _wr; });
    } else if (_t6239.tag == HirExpr_EMethodCall) {
        __auto_type meth = _t6239.data.EMethodCall.method;
        TrStr _trret1555 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(meth)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("method .", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret1555;
    } else if (1) {
        __auto_type _ = _t6239;
        return _tr_str_lit_len("expression", 10LL);
    }
}

__attribute__((hot)) TrStr _expr_kind(HirExpr* e) {
#line 2068 "src/taumir/lower.tr"
    __auto_type _t6241 = (*e);
    if (_t6241.tag == HirExpr_ELitInt) {
        return _tr_str_lit_len("int literal", 11LL);
    } else if (_t6241.tag == HirExpr_ELitStr) {
        return _tr_str_lit_len("str literal", 11LL);
    } else if (_t6241.tag == HirExpr_ELitBool) {
        return _tr_str_lit_len("bool literal", 12LL);
    } else if (_t6241.tag == HirExpr_ELitChar) {
        return _tr_str_lit_len("char literal", 12LL);
    } else if (_t6241.tag == HirExpr_ELitFloat) {
        return _tr_str_lit_len("float literal", 13LL);
    } else if (_t6241.tag == HirExpr_ERawStr) {
        return _tr_str_lit_len("raw str literal", 15LL);
    } else if (_t6241.tag == HirExpr_EIdent) {
        __auto_type n = _t6241.data.EIdent.name;
        TrStr _trret1556 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(n)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("ident '", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret1556;
    } else if (_t6241.tag == HirExpr_ECall) {
        __auto_type callee = _t6241.data.ECall.callee;
        return ({ TrStr _at_t6242 = (_ident_name(callee)); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(_at_t6242)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("call ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_str_release(_at_t6242); _wr; });
    } else if (_t6241.tag == HirExpr_EMethodCall) {
        __auto_type meth = _t6241.data.EMethodCall.method;
        TrStr _trret1557 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(meth)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("method .", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret1557;
    } else if (_t6241.tag == HirExpr_EIndex) {
        return _tr_str_lit_len("index expr", 10LL);
    } else if (_t6241.tag == HirExpr_EPropAccess) {
        __auto_type p = _t6241.data.EPropAccess.prop;
        TrStr _trret1558 = ({ TrStr _tr_cr = (_own(p)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("prop .", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret1558;
    } else if (_t6241.tag == HirExpr_EBinOp) {
        __auto_type op = _t6241.data.EBinOp.op;
        TrStr _trret1559 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(op)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("binary op '", 11LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret1559;
    } else if (_t6241.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t6241.data.EUnaryOp.op;
        TrStr _trret1560 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(op)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("unary op '", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret1560;
    } else if (_t6241.tag == HirExpr_ECast) {
        __auto_type ctty = _t6241.data.ECast.target_ty;
        TrStr _trret1561 = ({ TrStr _tr_cr = (_own(ctty->name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("cast to ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret1561;
    } else if (_t6241.tag == HirExpr_EIfElse) {
        return _tr_str_lit_len("if-else expr", 12LL);
    } else if (_t6241.tag == HirExpr_EClosure) {
        return _tr_str_lit_len("closure", 7LL);
    } else if (_t6241.tag == HirExpr_ETuple) {
        return _tr_str_lit_len("tuple", 5LL);
    } else if (_t6241.tag == HirExpr_EList) {
        return _tr_str_lit_len("list literal", 12LL);
    } else if (_t6241.tag == HirExpr_ESet) {
        return _tr_str_lit_len("set literal", 11LL);
    } else if (_t6241.tag == HirExpr_EDict) {
        return _tr_str_lit_len("dict literal", 12LL);
    } else if (_t6241.tag == HirExpr_EFString) {
        return _tr_str_lit_len("f-string", 8LL);
    } else if (_t6241.tag == HirExpr_ESuperMethodCall) {
        __auto_type meth = _t6241.data.ESuperMethodCall.method;
        TrStr _trret1562 = ({ TrStr _tr_cr = (_own(meth)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret1562;
    } else if (_t6241.tag == HirExpr_ESuperPropAccess) {
        __auto_type p = _t6241.data.ESuperPropAccess.prop;
        TrStr _trret1563 = ({ TrStr _tr_cr = (_own(p)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret1563;
    } else if (1) {
        __auto_type _ = _t6241;
        return _tr_str_lit_len("expression", 10LL);
    }
}

__attribute__((hot)) TrStr _stmt_kind(HirStmt* s) {
#line 2095 "src/taumir/lower.tr"
    __auto_type _t6243 = (*s);
    if (_t6243.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t6243.data.SLineMarker.n;
        return _tr_str_lit_len("line marker", 11LL);
    } else if (_t6243.tag == HirStmt_SPass) {
        return _tr_str_lit_len("pass", 4LL);
    } else if (_t6243.tag == HirStmt_SAutoDrop) {
        return _tr_str_lit_len("auto-drop", 9LL);
    } else if (_t6243.tag == HirStmt_SFree) {
        __auto_type _ = _t6243.data.SFree.name;
        return _tr_str_lit_len("free", 4LL);
    } else if (_t6243.tag == HirStmt_SReturn) {
        __auto_type _ = _t6243.data.SReturn.val;
        return _tr_str_lit_len("return", 6LL);
    } else if (_t6243.tag == HirStmt_SLet) {
        __auto_type n = _t6243.data.SLet.name;
        TrStr _trret1564 = ({ TrStr _tr_cr = (_own(n)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("let ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret1564;
    } else if (_t6243.tag == HirStmt_SAssign) {
        return _tr_str_lit_len("assign", 6LL);
    } else if (_t6243.tag == HirStmt_SIf) {
        return _tr_str_lit_len("if", 2LL);
    } else if (_t6243.tag == HirStmt_SWhile) {
        return _tr_str_lit_len("while", 5LL);
    } else if (_t6243.tag == HirStmt_SBreak) {
        __auto_type _ = _t6243.data.SBreak.val;
        return _tr_str_lit_len("break", 5LL);
    } else if (_t6243.tag == HirStmt_SContinue) {
        return _tr_str_lit_len("continue", 8LL);
    } else if (_t6243.tag == HirStmt_SAssert) {
        return _tr_str_lit_len("assert", 6LL);
    } else if (_t6243.tag == HirStmt_SMultiLet) {
        return _tr_str_lit_len("multi-let", 9LL);
    } else if (_t6243.tag == HirStmt_SDefer) {
        __auto_type _ = _t6243.data.SDefer.stmt;
        return _tr_str_lit_len("defer", 5LL);
    } else if (_t6243.tag == HirStmt_SWith) {
        return _tr_str_lit_len("with", 4LL);
    } else if (_t6243.tag == HirStmt_STry) {
        return _tr_str_lit_len("try", 3LL);
    } else if (_t6243.tag == HirStmt_SRaise) {
        __auto_type _ = _t6243.data.SRaise.val;
        return _tr_str_lit_len("raise", 5LL);
    } else if (_t6243.tag == HirStmt_SMatch) {
        return _tr_str_lit_len("match", 5LL);
    } else if (_t6243.tag == HirStmt_SFor) {
        return _tr_str_lit_len("for", 3LL);
    } else if (_t6243.tag == HirStmt_SForUnpack) {
        return _tr_str_lit_len("for-unpack", 10LL);
    } else if (_t6243.tag == HirStmt_SExpr) {
        __auto_type _ = _t6243.data.SExpr.expr;
        return _tr_str_lit_len("expr", 4LL);
    } else if (_t6243.tag == HirStmt_SUnsafe) {
        __auto_type _ = _t6243.data.SUnsafe.body;
        return _tr_str_lit_len("unsafe", 6LL);
    } else if (1) {
        __auto_type _ = _t6243;
        return _tr_str_lit_len("statement", 9LL);
    }
}

__attribute__((hot)) bool _stmt_has_cf(HirStmt* s) {
#line 2125 "src/taumir/lower.tr"
    __auto_type _t6244 = (*s);
    if (_t6244.tag == HirStmt_SReturn) {
        __auto_type _ = _t6244.data.SReturn.val;
        return true;
    } else if (_t6244.tag == HirStmt_SBreak) {
        __auto_type _ = _t6244.data.SBreak.val;
        return true;
    } else if (_t6244.tag == HirStmt_SContinue) {
        return true;
    } else if (_t6244.tag == HirStmt_SIf) {
        __auto_type tb = _t6244.data.SIf.then_b;
__auto_type eb = _t6244.data.SIf.else_b;
        bool _trret1565 = (_block_has_cf(tb) || _block_has_cf(eb));
        return _trret1565;
    } else if (_t6244.tag == HirStmt_SWhile) {
        __auto_type wb = _t6244.data.SWhile.body;
        bool _trret1566 = _block_has_cf(wb);
        return _trret1566;
    } else if (_t6244.tag == HirStmt_SFor) {
        __auto_type fb = _t6244.data.SFor.body;
        bool _trret1567 = _block_has_cf(fb);
        return _trret1567;
    } else if (_t6244.tag == HirStmt_SForUnpack) {
        __auto_type fb = _t6244.data.SForUnpack.body;
        bool _trret1568 = _block_has_cf(fb);
        return _trret1568;
    } else if (_t6244.tag == HirStmt_SMatch) {
        __auto_type arms = _t6244.data.SMatch.arms;
#line 2134 "src/taumir/lower.tr"
        long long j = 0LL;
#line 2135 "src/taumir/lower.tr"
        while ((j < arms->len)) {
#line 2136 "src/taumir/lower.tr"
            if (_block_has_cf(((HirMatchArm*)List_ptr_get(arms, j))->body)) {
#line 2136 "src/taumir/lower.tr"
                return true;
            }
#line 2137 "src/taumir/lower.tr"
            j = (j + 1LL);
        }
#line 2138 "src/taumir/lower.tr"
        return false;
    } else if (_t6244.tag == HirStmt_SWith) {
        __auto_type wb = _t6244.data.SWith.body;
        bool _trret1569 = _block_has_cf(wb);
        return _trret1569;
    } else if (_t6244.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t6244.data.SUnsafe.body;
        bool _trret1570 = _block_has_cf(ub);
        return _trret1570;
    } else if (_t6244.tag == HirStmt_SDefer) {
        __auto_type ds = _t6244.data.SDefer.stmt;
        return _stmt_has_cf(ds);
    } else if (_t6244.tag == HirStmt_STry) {
        __auto_type tb = _t6244.data.STry.try_body;
__auto_type cs = _t6244.data.STry.catches;
__auto_type fb = _t6244.data.STry.finally_b;
#line 2143 "src/taumir/lower.tr"
        if ((_block_has_cf(tb) || _block_has_cf(fb))) {
#line 2143 "src/taumir/lower.tr"
            return true;
        }
#line 2144 "src/taumir/lower.tr"
        long long k = 0LL;
#line 2145 "src/taumir/lower.tr"
        while ((k < cs->len)) {
#line 2146 "src/taumir/lower.tr"
            if (_block_has_cf((*((HirCatchClause**)List_ptr_get(cs, k)))->body)) {
#line 2146 "src/taumir/lower.tr"
                return true;
            }
#line 2147 "src/taumir/lower.tr"
            k = (k + 1LL);
        }
#line 2148 "src/taumir/lower.tr"
        return false;
    } else if (1) {
        __auto_type _ = _t6244;
        return false;
    }
}

__attribute__((hot)) bool _block_has_cf(HirBlock* b) {
#line 2152 "src/taumir/lower.tr"
    long long i = 0LL;
#line 2153 "src/taumir/lower.tr"
    while ((i < b->stmts->len)) {
#line 2154 "src/taumir/lower.tr"
        if (_stmt_has_cf(((HirStmt*)List_ptr_get(b->stmts, i)))) {
#line 2154 "src/taumir/lower.tr"
            return true;
        }
#line 2155 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 2156 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _lower_try_setjmp(LModule* m, LFunc* lf, HirBlock* try_body, List_ptr* catches, HirBlock* finally_b) {
#line 2165 "src/taumir/lower.tr"
    bool has_catch = (catches->len == 1LL);
#line 2166 "src/taumir/lower.tr"
    TrStr ename = _tr_str_lit_len("__exmsg", 7LL);
#line 2167 "src/taumir/lower.tr"
    if (has_catch) {
#line 2168 "src/taumir/lower.tr"
        HirCatchClause* tcc0 = (*((HirCatchClause**)List_ptr_get(catches, 0LL)));
#line 2169 "src/taumir/lower.tr"
        if (((!_is_null_str(tcc0->err_name)) && (!_tr_str_eqv((tcc0->err_name), (_tr_str_lit_len("", 0LL)))))) {
#line 2169 "src/taumir/lower.tr"
            TrStr _strtmp_t6245 = _tr_str_retain(tcc0->err_name);
            _tr_str_release(ename);
            ename = _strtmp_t6245;
        }
    }
#line 2172 "src/taumir/lower.tr"
    List_TrStr* capn = (void*)List_TrStr_new();
#line 2173 "src/taumir/lower.tr"
    List_i64* capt = (void*)List_i64_new();
#line 2174 "src/taumir/lower.tr"
    long long vi = 0LL;
#line 2175 "src/taumir/lower.tr"
    while ((vi < lf->vars->len)) {
#line 2176 "src/taumir/lower.tr"
        TrStr vn = List_TrStr_get(lf->vars, vi);
#line 2177 "src/taumir/lower.tr"
        ({ TrStr _at_t6246 = (_own(vn)); List_TrStr_append(capn, _at_t6246); _tr_str_release(_at_t6246); });
#line 2178 "src/taumir/lower.tr"
        List_i64_append(capt, LFunc_var_type(lf, vn));
#line 2179 "src/taumir/lower.tr"
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
#line 2181 "src/taumir/lower.tr"
    TrStr tname = ({ __auto_type _ord_t6247 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_trytry_", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6247); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lf->name)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 2182 "src/taumir/lower.tr"
    LFunc* tlf = ({ TrStr _at_t6248 = (_own(tname)); __auto_type _wr = (LFunc_init(_at_t6248)); _tr_str_release(_at_t6248); _wr; });
#line 2183 "src/taumir/lower.tr"
    List_TrStr_append(tlf->params, _tr_str_lit_len("__env", 5LL));
#line 2184 "src/taumir/lower.tr"
    LFunc_add_var(tlf, _tr_str_lit_len("__env", 5LL));
#line 2185 "src/taumir/lower.tr"
    long long ci = 0LL;
#line 2186 "src/taumir/lower.tr"
    while ((ci < capn->len)) {
#line 2187 "src/taumir/lower.tr"
        ({ TrStr _at_t6249 = (List_TrStr_get(capn, ci)); TrStr _ord_t6250 = (_own(_at_t6249)); List_TrStr_append(tlf->captures, _ord_t6250); _tr_str_release(_at_t6249); _tr_str_release(_ord_t6250); });
#line 2188 "src/taumir/lower.tr"
        ({ __auto_type _ord_t6251 = (List_i64_get(capt, ci)); List_i64_append(tlf->cap_tags, _ord_t6251); });
#line 2189 "src/taumir/lower.tr"
        ci = (ci + 1LL);
    }
#line 2190 "src/taumir/lower.tr"
    ({ TrStr _ord_t6252 = (_own(tname)); List_TrStr_append(m->fn_names, _ord_t6252); _tr_str_release(_ord_t6252); });
#line 2191 "src/taumir/lower.tr"
    List_i64_append(m->fn_ret, 0LL);
#line 2192 "src/taumir/lower.tr"
    LFunc_set_cur(tlf, LFunc_new_block(tlf));
#line 2193 "src/taumir/lower.tr"
    if ((!lower_block(m, tlf, try_body))) {
#line 2193 "src/taumir/lower.tr"
        _tr_str_release(ename);
        List_TrStr_free(capn);
        List_i64_free(capt);
        _tr_str_release(tname);
        _tr_obj_release(tlf, _trdrop_LFunc);
        return false;
    }
#line 2194 "src/taumir/lower.tr"
    LFunc_set_term(tlf, LTerm_ctor_TRetInt(0LL));
#line 2195 "src/taumir/lower.tr"
    List_ptr_append(m->funcs, _tr_obj_retain(tlf));
#line 2197 "src/taumir/lower.tr"
    TrStr cname = _tr_str_lit_len("", 0LL);
#line 2198 "src/taumir/lower.tr"
    if (has_catch) {
#line 2199 "src/taumir/lower.tr"
        TrStr _strtmp_t6254 = ({ __auto_type _ord_t6253 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_trycatch_", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6253); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lf->name)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        _tr_str_release(cname);
        cname = _strtmp_t6254;
#line 2200 "src/taumir/lower.tr"
        LFunc* clf = ({ TrStr _at_t6255 = (_own(cname)); __auto_type _wr = (LFunc_init(_at_t6255)); _tr_str_release(_at_t6255); _wr; });
#line 2201 "src/taumir/lower.tr"
        List_TrStr_append(clf->params, _tr_str_lit_len("__env", 5LL));
#line 2202 "src/taumir/lower.tr"
        LFunc_add_var(clf, _tr_str_lit_len("__env", 5LL));
#line 2203 "src/taumir/lower.tr"
        ({ TrStr _ord_t6256 = (_own(ename)); List_TrStr_append(clf->params, _ord_t6256); _tr_str_release(_ord_t6256); });
#line 2204 "src/taumir/lower.tr"
        ({ TrStr _at_t6257 = (_own(ename)); LFunc_add_var(clf, _at_t6257); _tr_str_release(_at_t6257); });
#line 2205 "src/taumir/lower.tr"
        LFunc_set_var_type(clf, ename, 1LL);
#line 2206 "src/taumir/lower.tr"
        long long ci2 = 0LL;
#line 2207 "src/taumir/lower.tr"
        while ((ci2 < capn->len)) {
#line 2208 "src/taumir/lower.tr"
            ({ TrStr _at_t6258 = (List_TrStr_get(capn, ci2)); TrStr _ord_t6259 = (_own(_at_t6258)); List_TrStr_append(clf->captures, _ord_t6259); _tr_str_release(_at_t6258); _tr_str_release(_ord_t6259); });
#line 2209 "src/taumir/lower.tr"
            ({ __auto_type _ord_t6260 = (List_i64_get(capt, ci2)); List_i64_append(clf->cap_tags, _ord_t6260); });
#line 2210 "src/taumir/lower.tr"
            ci2 = (ci2 + 1LL);
        }
#line 2211 "src/taumir/lower.tr"
        ({ TrStr _ord_t6261 = (_own(cname)); List_TrStr_append(m->fn_names, _ord_t6261); _tr_str_release(_ord_t6261); });
#line 2212 "src/taumir/lower.tr"
        List_i64_append(m->fn_ret, 0LL);
#line 2213 "src/taumir/lower.tr"
        LFunc_set_cur(clf, LFunc_new_block(clf));
#line 2214 "src/taumir/lower.tr"
        if ((!lower_block(m, clf, (*((HirCatchClause**)List_ptr_get(catches, 0LL)))->body))) {
#line 2214 "src/taumir/lower.tr"
            _tr_str_release(ename);
            List_TrStr_free(capn);
            List_i64_free(capt);
            _tr_str_release(tname);
            _tr_obj_release(tlf, _trdrop_LFunc);
            _tr_str_release(cname);
            _tr_obj_release(clf, _trdrop_LFunc);
            return false;
        }
#line 2215 "src/taumir/lower.tr"
        LFunc_set_term(clf, LTerm_ctor_TRetInt(0LL));
#line 2216 "src/taumir/lower.tr"
        List_ptr_append(m->funcs, _tr_obj_retain(clf));
    }
#line 2218 "src/taumir/lower.tr"
    long long envsz = LFunc_new_vreg(lf);
#line 2219 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(envsz, ((1LL + capn->len) * 8LL)));
#line 2220 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 2221 "src/taumir/lower.tr"
    List_i64* ea = (void*)List_i64_new();
#line 2222 "src/taumir/lower.tr"
    List_i64_append(ea, envsz);
#line 2223 "src/taumir/lower.tr"
    long long env = LFunc_new_vreg(lf);
#line 2224 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(env, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), ea));
#line 2225 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, env, 12LL);
#line 2226 "src/taumir/lower.tr"
    long long ci3 = 0LL;
#line 2227 "src/taumir/lower.tr"
    while ((ci3 < capn->len)) {
#line 2228 "src/taumir/lower.tr"
        long long av = LFunc_new_vreg(lf);
#line 2229 "src/taumir/lower.tr"
        ({ TrStr _at_t6262 = (List_TrStr_get(capn, ci3)); LFunc_emit(lf, LInst_ctor_IAddrVar(av, _at_t6262)); _tr_str_release(_at_t6262); });
#line 2230 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, av, 12LL);
#line 2231 "src/taumir/lower.tr"
        _emit_field_set(m, lf, env, ((1LL + ci3) * 8LL), av);
#line 2232 "src/taumir/lower.tr"
        ci3 = (ci3 + 1LL);
    }
#line 2233 "src/taumir/lower.tr"
    long long tfa = LFunc_new_vreg(lf);
#line 2234 "src/taumir/lower.tr"
    ({ TrStr _at_t6263 = (_own(tname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(tfa, _at_t6263)); _tr_str_release(_at_t6263); });
#line 2235 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, tfa, 12LL);
#line 2236 "src/taumir/lower.tr"
    long long cfa = LFunc_new_vreg(lf);
#line 2237 "src/taumir/lower.tr"
    if (has_catch) {
#line 2238 "src/taumir/lower.tr"
        ({ TrStr _at_t6264 = (_own(cname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(cfa, _at_t6264)); _tr_str_release(_at_t6264); });
    } else {
#line 2240 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(cfa, 0LL));
    }
#line 2241 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, cfa, 12LL);
#line 2242 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_try_catch", 16LL));
#line 2243 "src/taumir/lower.tr"
    List_i64* ta = (void*)List_i64_new();
#line 2244 "src/taumir/lower.tr"
    List_i64_append(ta, tfa);
#line 2245 "src/taumir/lower.tr"
    List_i64_append(ta, cfa);
#line 2246 "src/taumir/lower.tr"
    List_i64_append(ta, env);
#line 2247 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_try_catch", 16LL), ta));
#line 2248 "src/taumir/lower.tr"
    _release_obj(m, lf, env);
#line 2250 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, finally_b))) {
#line 2250 "src/taumir/lower.tr"
        _tr_str_release(ename);
        List_TrStr_free(capn);
        List_i64_free(capt);
        _tr_str_release(tname);
        _tr_obj_release(tlf, _trdrop_LFunc);
        _tr_str_release(cname);
        return false;
    }
#line 2251 "src/taumir/lower.tr"
    _tr_str_release(ename);
    List_TrStr_free(capn);
    List_i64_free(capt);
    _tr_str_release(tname);
    _tr_obj_release(tlf, _trdrop_LFunc);
    _tr_str_release(cname);
    return true;
}

__attribute__((hot)) bool _is_vstruct_lvalue(HirExpr* e) {
#line 2256 "src/taumir/lower.tr"
    __auto_type _t6265 = (*e);
    if (_t6265.tag == HirExpr_EIdent) {
        return true;
    } else if (_t6265.tag == HirExpr_EPropAccess) {
        return true;
    } else if (1) {
        __auto_type _ = _t6265;
        return false;
    }
}

__attribute__((hot)) long long _copy_value_struct(LModule* m, LFunc* lf, long long src, TrStr cls) {
#line 2265 "src/taumir/lower.tr"
    long long ci = LModule_class_index(m, cls);
#line 2266 "src/taumir/lower.tr"
    if ((ci < 0LL)) {
#line 2266 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 2267 "src/taumir/lower.tr"
    ClassLayout* lay = ((ClassLayout*)List_ptr_get(m->classes, ci));
#line 2268 "src/taumir/lower.tr"
    long long fi = 0LL;
#line 2269 "src/taumir/lower.tr"
    while ((fi < lay->ftags->len)) {
#line 2270 "src/taumir/lower.tr"
        long long ft = List_i64_get(lay->ftags, fi);
#line 2271 "src/taumir/lower.tr"
        if (((((((ft != 0LL) && (ft != 4LL)) && (ft != 5LL)) && (ft != 1LL)) && (ft != 10LL)) && (ft != 11LL))) {
#line 2271 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 2272 "src/taumir/lower.tr"
        fi = (fi + 1LL);
    }
#line 2273 "src/taumir/lower.tr"
    long long szc = LFunc_new_vreg(lf);
#line 2274 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_class_size(m, cls)));
#line 2275 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 2276 "src/taumir/lower.tr"
    List_i64* aa = (void*)List_i64_new();
#line 2277 "src/taumir/lower.tr"
    List_i64_append(aa, szc);
#line 2278 "src/taumir/lower.tr"
    long long dst = LFunc_new_vreg(lf);
#line 2279 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(dst, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), aa));
#line 2280 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dst, 10LL);
#line 2282 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_c_memcpy", 12LL));
#line 2283 "src/taumir/lower.tr"
    List_i64* cpa = (void*)List_i64_new();
#line 2284 "src/taumir/lower.tr"
    List_i64_append(cpa, dst);
#line 2285 "src/taumir/lower.tr"
    List_i64_append(cpa, src);
#line 2286 "src/taumir/lower.tr"
    List_i64_append(cpa, szc);
#line 2287 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_c_memcpy", 12LL), cpa));
#line 2290 "src/taumir/lower.tr"
    _attach_class_drop(m, lf, dst, cls);
#line 2291 "src/taumir/lower.tr"
    long long fi2 = 0LL;
#line 2292 "src/taumir/lower.tr"
    while ((fi2 < lay->ftags->len)) {
#line 2293 "src/taumir/lower.tr"
        long long ft2 = List_i64_get(lay->ftags, fi2);
#line 2294 "src/taumir/lower.tr"
        if ((((ft2 == 1LL) || (ft2 == 10LL)) || (ft2 == 11LL))) {
#line 2295 "src/taumir/lower.tr"
            long long fv = _emit_field_get(m, lf, dst, (fi2 * 8LL), ft2);
#line 2296 "src/taumir/lower.tr"
            if ((ft2 == 1LL)) {
#line 2296 "src/taumir/lower.tr"
                _retain_str(m, lf, fv);
            } else {
#line 2297 "src/taumir/lower.tr"
                _retain_obj(m, lf, fv);
            }
        }
#line 2298 "src/taumir/lower.tr"
        fi2 = (fi2 + 1LL);
    }
#line 2299 "src/taumir/lower.tr"
    _fresh_mark_obj(lf, dst);
#line 2300 "src/taumir/lower.tr"
    long long _trret1571 = dst;
    return _trret1571;
}

__attribute__((hot)) long long _value_struct_generic_size(LModule* m, AstType* ty) {
#line 2306 "src/taumir/lower.tr"
    long long gi = 0LL;
#line 2307 "src/taumir/lower.tr"
    while ((gi < m->hir_prog->classes->len)) {
#line 2308 "src/taumir/lower.tr"
        HirClass* gc = ((HirClass*)List_ptr_get(m->hir_prog->classes, gi));
#line 2309 "src/taumir/lower.tr"
        if (((_tr_str_eqv((gc->name), (ty->name)) && (gc->generics->len > 0LL)) && (!gc->is_class))) {
#line 2310 "src/taumir/lower.tr"
            long long n = gc->fields->len;
#line 2311 "src/taumir/lower.tr"
            if ((n < 1LL)) {
#line 2311 "src/taumir/lower.tr"
                n = 1LL;
            }
#line 2312 "src/taumir/lower.tr"
            long long _trret1572 = (n * 8LL);
            return _trret1572;
        }
#line 2313 "src/taumir/lower.tr"
        gi = (gi + 1LL);
    }
#line 2314 "src/taumir/lower.tr"
    return 0LL;
}

__attribute__((hot)) bool _lower_stmt_impl(LModule* m, LFunc* lf, HirStmt* s) {
#line 2317 "src/taumir/lower.tr"
    __auto_type _t6266 = (*s);
    if (_t6266.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t6266.data.SLineMarker.n;
        return true;
    } else if (_t6266.tag == HirStmt_SPass) {
        return true;
    } else if (_t6266.tag == HirStmt_SAutoDrop) {
        __auto_type name = _t6266.data.SAutoDrop.name;
#line 2325 "src/taumir/lower.tr"
        if (_tr_str_eqv((name), (_tr_str_lit_len("_", 1LL)))) {
#line 2325 "src/taumir/lower.tr"
            return true;
        }
#line 2330 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, name) >= 0LL) && (!_is_param(lf, name)))) {
#line 2331 "src/taumir/lower.tr"
            long long dvt = LFunc_var_type(lf, name);
#line 2332 "src/taumir/lower.tr"
            if ((dvt == 1LL)) {
#line 2333 "src/taumir/lower.tr"
                long long dv = LFunc_new_vreg(lf);
#line 2334 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ILoadVar(dv, name));
#line 2335 "src/taumir/lower.tr"
                _release_str(m, lf, dv);
            } else if (((dvt == 10LL) || (dvt == 11LL))) {
#line 2337 "src/taumir/lower.tr"
                long long ov = LFunc_new_vreg(lf);
#line 2338 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ILoadVar(ov, name));
#line 2339 "src/taumir/lower.tr"
                _release_obj(m, lf, ov);
            }
        }
#line 2340 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SFree) {
        __auto_type _ = _t6266.data.SFree.name;
        return true;
    } else if (_t6266.tag == HirStmt_SReturn) {
        __auto_type val = _t6266.data.SReturn.val;
#line 2343 "src/taumir/lower.tr"
        if (lf->in_defer) {
#line 2343 "src/taumir/lower.tr"
            return false;
        }
#line 2344 "src/taumir/lower.tr"
        if ((lf->is_throws && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 2346 "src/taumir/lower.tr"
            long long orv = lower_expr(m, lf, val);
#line 2347 "src/taumir/lower.tr"
            if ((orv < 0LL)) {
#line 2347 "src/taumir/lower.tr"
                return false;
            }
#line 2348 "src/taumir/lower.tr"
            long long okr = _wrap_result(m, lf, 0LL, orv, lf->throws_ok_tag);
#line 2349 "src/taumir/lower.tr"
            _fresh_take_obj(lf, okr);
#line 2350 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2351 "src/taumir/lower.tr"
            if ((!_run_defers(m, lf))) {
#line 2351 "src/taumir/lower.tr"
                return false;
            }
#line 2352 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TRetVal(okr));
#line 2353 "src/taumir/lower.tr"
            return true;
        }
#line 2354 "src/taumir/lower.tr"
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 2355 "src/taumir/lower.tr"
            long long rv = lower_expr(m, lf, val);
#line 2356 "src/taumir/lower.tr"
            if ((rv < 0LL)) {
#line 2356 "src/taumir/lower.tr"
                return false;
            }
#line 2357 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, rv) == 1LL)) {
#line 2357 "src/taumir/lower.tr"
                _secure_str(m, lf, rv);
            }
#line 2358 "src/taumir/lower.tr"
            if (((LFunc_vreg_type(lf, rv) == 10LL) || (LFunc_vreg_type(lf, rv) == 11LL))) {
#line 2363 "src/taumir/lower.tr"
                if ((!_is_owned_local_return(lf, val))) {
#line 2364 "src/taumir/lower.tr"
                    _secure_obj(m, lf, rv);
                } else {
#line 2366 "src/taumir/lower.tr"
                    _fresh_take_obj(lf, rv);
                }
            }
#line 2367 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2368 "src/taumir/lower.tr"
            if ((!_run_defers(m, lf))) {
#line 2368 "src/taumir/lower.tr"
                return false;
            }
#line 2369 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TRetVal(rv));
        } else {
#line 2371 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2372 "src/taumir/lower.tr"
            if ((!_run_defers(m, lf))) {
#line 2372 "src/taumir/lower.tr"
                return false;
            }
#line 2373 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
        }
#line 2374 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SLet) {
        __auto_type name = _t6266.data.SLet.name;
__auto_type ty = _t6266.data.SLet.ty;
__auto_type val = _t6266.data.SLet.val;
#line 2376 "src/taumir/lower.tr"
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 2389 "src/taumir/lower.tr"
            if ((((m->target_llvm && _tr_str_eqv((ty->name), (_tr_str_lit_len("Array", 5LL)))) && (ty->array_size > 0LL)) && (ty->args->len > 0LL))) {
#line 2390 "src/taumir/lower.tr"
                long long aetag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, 0LL))));
#line 2391 "src/taumir/lower.tr"
                if (((aetag != 0LL) && (aetag != 4LL))) {
#line 2391 "src/taumir/lower.tr"
                    return false;
                }
#line 2392 "src/taumir/lower.tr"
                long long alen = ty->array_size;
#line 2393 "src/taumir/lower.tr"
                LFunc_add_var(lf, name);
#line 2394 "src/taumir/lower.tr"
                LFunc_set_var_type(lf, name, aetag);
#line 2395 "src/taumir/lower.tr"
                LFunc_set_var_arr(lf, name, alen);
#line 2396 "src/taumir/lower.tr"
                long long zc = LFunc_new_vreg(lf);
#line 2397 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
#line 2398 "src/taumir/lower.tr"
                long long zi = 0LL;
#line 2399 "src/taumir/lower.tr"
                while ((zi < alen)) {
#line 2400 "src/taumir/lower.tr"
                    long long zidx = LFunc_new_vreg(lf);
#line 2401 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IConst(zidx, zi));
#line 2402 "src/taumir/lower.tr"
                    long long zaddr = _array_elem_addr(m, lf, name, zidx);
#line 2403 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IStore(zaddr, 0LL, zc, 2LL));
#line 2404 "src/taumir/lower.tr"
                    zi = (zi + 1LL);
                }
#line 2405 "src/taumir/lower.tr"
                return true;
            }
#line 2406 "src/taumir/lower.tr"
            long long dtag = _tag_of(m, ty);
#line 2407 "src/taumir/lower.tr"
            if ((((dtag != 0LL) && (dtag != 4LL)) && (dtag != 5LL))) {
#line 2407 "src/taumir/lower.tr"
                return false;
            }
#line 2408 "src/taumir/lower.tr"
            LFunc_add_var(lf, name);
#line 2409 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, name, dtag);
#line 2410 "src/taumir/lower.tr"
            return true;
        }
#line 2411 "src/taumir/lower.tr"
        long long v = lower_expr(m, lf, val);
#line 2412 "src/taumir/lower.tr"
        if ((v < 0LL)) {
#line 2412 "src/taumir/lower.tr"
            return false;
        }
#line 2415 "src/taumir/lower.tr"
        long long vtag = LFunc_vreg_type(lf, v);
#line 2416 "src/taumir/lower.tr"
        if (((((vtag == 2LL) || (vtag == 3LL)) || (vtag == 14LL)) || (vtag == 19LL))) {
#line 2417 "src/taumir/lower.tr"
            long long ann_lt = _list_tag_from_ann(m, ty);
#line 2418 "src/taumir/lower.tr"
            if (((ann_lt >= 0LL) && (ann_lt != vtag))) {
#line 2419 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, v, ann_lt);
#line 2420 "src/taumir/lower.tr"
                vtag = ann_lt;
            }
        } else if (_is_dict_tag(vtag)) {
#line 2422 "src/taumir/lower.tr"
            long long ann_dt = _dict_tag_from_ann(m, ty);
#line 2423 "src/taumir/lower.tr"
            if (((ann_dt >= 0LL) && (ann_dt != vtag))) {
#line 2424 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, v, ann_dt);
#line 2425 "src/taumir/lower.tr"
                vtag = ann_dt;
            }
        } else if (((vtag == 4LL) && _is_int_typename(ty->name))) {
#line 2428 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, v, 0LL);
#line 2429 "src/taumir/lower.tr"
            vtag = 0LL;
        } else if (((vtag == 0LL) && _tr_str_eqv((ty->name), (_tr_str_lit_len("bool", 4LL))))) {
#line 2432 "src/taumir/lower.tr"
            long long nbv = _norm_bool(lf, v);
#line 2433 "src/taumir/lower.tr"
            v = nbv;
#line 2434 "src/taumir/lower.tr"
            vtag = 4LL;
        }
#line 2435 "src/taumir/lower.tr"
        if ((vtag == 1LL)) {
#line 2435 "src/taumir/lower.tr"
            _secure_str(m, lf, v);
        }
#line 2439 "src/taumir/lower.tr"
        TrStr _vs_cls = _tr_str_lit_len("", 0LL);
#line 2440 "src/taumir/lower.tr"
        if ((vtag == 10LL)) {
#line 2440 "src/taumir/lower.tr"
            TrStr _strtmp_t6267 = _val_obj_cls(m, lf, val);
            _tr_str_release(_vs_cls);
            _vs_cls = _strtmp_t6267;
        }
#line 2441 "src/taumir/lower.tr"
        bool _is_vstruct = (((vtag == 10LL) && (!_is_null_str(_vs_cls))) && LModule_is_value_class(m, _vs_cls));
#line 2442 "src/taumir/lower.tr"
        if ((_is_vstruct && _is_vstruct_lvalue(val))) {
#line 2443 "src/taumir/lower.tr"
            long long _cpv = _copy_value_struct(m, lf, v, _vs_cls);
#line 2444 "src/taumir/lower.tr"
            if ((_cpv < 0LL)) {
#line 2444 "src/taumir/lower.tr"
                _tr_str_release(_vs_cls);
                return false;
            }
#line 2445 "src/taumir/lower.tr"
            v = _cpv;
#line 2446 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, v, 10LL);
        }
#line 2447 "src/taumir/lower.tr"
        if (((vtag == 10LL) || (vtag == 11LL))) {
#line 2448 "src/taumir/lower.tr"
            _secure_obj(m, lf, v);
        }
#line 2449 "src/taumir/lower.tr"
        LFunc_add_var(lf, name);
#line 2450 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, name, vtag);
#line 2451 "src/taumir/lower.tr"
        if (((LFunc_vreg_type(lf, v) == 10LL) || (LFunc_vreg_type(lf, v) == 11LL))) {
#line 2452 "src/taumir/lower.tr"
            TrStr lvcn = _val_obj_cls(m, lf, val);
#line 2453 "src/taumir/lower.tr"
            if ((((!_is_null_str(lvcn)) && (!_tr_str_eqv((lvcn), (_tr_str_lit_len("", 0LL))))) && (LModule_is_class(m, lvcn) || LModule_is_enum(m, lvcn)))) {
#line 2454 "src/taumir/lower.tr"
                ({ TrStr _at_t6268 = (_own(lvcn)); LFunc_set_var_cls(lf, name, _at_t6268); _tr_str_release(_at_t6268); });
            }
        }
#line 2455 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, v) == 12LL)) {
#line 2456 "src/taumir/lower.tr"
            LFunc_set_var_xret(lf, name, LFunc_vreg_xret_of(lf, v));
        }
#line 2457 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(name, v));
#line 2458 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2459 "src/taumir/lower.tr"
        _tr_str_release(_vs_cls);
        return true;
    } else if (_t6266.tag == HirStmt_SAssign) {
        __auto_type target = _t6266.data.SAssign.target;
__auto_type val = _t6266.data.SAssign.val;
#line 2462 "src/taumir/lower.tr"
        __auto_type _t6269 = (*target);
        if (_t6269.tag == HirExpr_EMethodCall) {
            __auto_type mobj = _t6269.data.EMethodCall.obj;
__auto_type mmeth = _t6269.data.EMethodCall.method;
__auto_type midx = _t6269.data.EMethodCall.args;
#line 2464 "src/taumir/lower.tr"
            if ((_tr_str_eqv((mmeth), (_tr_str_lit_len("get_index", 9LL))) && (midx->len == 1LL))) {
#line 2465 "src/taumir/lower.tr"
                bool _trret1573 = _lower_index_set(m, lf, mobj, ((HirExpr*)List_ptr_get(midx, 0LL)), val);
                return _trret1573;
            }
#line 2466 "src/taumir/lower.tr"
            return false;
        } else if (_t6269.tag == HirExpr_EIndex) {
            __auto_type iobj = _t6269.data.EIndex.obj;
__auto_type iidx = _t6269.data.EIndex._tr_v_index;
#line 2468 "src/taumir/lower.tr"
            return _lower_index_set(m, lf, iobj, iidx, val);
        } else if (_t6269.tag == HirExpr_EPropAccess) {
            __auto_type pobj = _t6269.data.EPropAccess.obj;
__auto_type pprop = _t6269.data.EPropAccess.prop;
#line 2470 "src/taumir/lower.tr"
            bool _trret1574 = _lower_field_set(m, lf, pobj, pprop, val);
            return _trret1574;
        } else if (1) {
            __auto_type _ = _t6269;
#line 2472 "src/taumir/lower.tr"
            /* pass */
        }
#line 2473 "src/taumir/lower.tr"
        TrStr tn = _ident_name(target);
#line 2474 "src/taumir/lower.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 2474 "src/taumir/lower.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2478 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, tn) < 0LL) && (LFunc_capture_index(lf, tn) >= 0LL))) {
#line 2479 "src/taumir/lower.tr"
            long long wcix = LFunc_capture_index(lf, tn);
#line 2480 "src/taumir/lower.tr"
            long long wtag = List_i64_get(lf->cap_tags, wcix);
#line 2481 "src/taumir/lower.tr"
            if ((((wtag != 0LL) && (wtag != 4LL)) && (wtag != 5LL))) {
#line 2481 "src/taumir/lower.tr"
                _tr_str_release(tn);
                return false;
            }
#line 2482 "src/taumir/lower.tr"
            long long wv = lower_expr(m, lf, val);
#line 2483 "src/taumir/lower.tr"
            if ((wv < 0LL)) {
#line 2483 "src/taumir/lower.tr"
                _tr_str_release(tn);
                return false;
            }
#line 2484 "src/taumir/lower.tr"
            long long wvt = LFunc_vreg_type(lf, wv);
#line 2485 "src/taumir/lower.tr"
            if ((wtag == 5LL)) {
#line 2486 "src/taumir/lower.tr"
                if ((wvt == 0LL)) {
#line 2486 "src/taumir/lower.tr"
                    wv = _promote_f(lf, wv);
                } else if ((wvt != 5LL)) {
#line 2487 "src/taumir/lower.tr"
                    _tr_str_release(tn);
                    return false;
                }
#line 2488 "src/taumir/lower.tr"
                long long wb = LFunc_new_vreg(lf);
#line 2489 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(wb, wv));
#line 2490 "src/taumir/lower.tr"
                wv = wb;
            } else if ((!_field_tag_ok(wvt, wtag))) {
#line 2492 "src/taumir/lower.tr"
                _tr_str_release(tn);
                return false;
            }
#line 2493 "src/taumir/lower.tr"
            long long wenv = LFunc_new_vreg(lf);
#line 2494 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(wenv, _tr_str_lit_len("__env", 5LL)));
#line 2495 "src/taumir/lower.tr"
            long long waddr = _emit_field_get(m, lf, wenv, ((1LL + wcix) * 8LL), 0LL);
#line 2496 "src/taumir/lower.tr"
            _emit_field_set(m, lf, waddr, 0LL, wv);
#line 2497 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2498 "src/taumir/lower.tr"
            _tr_str_release(tn);
            return true;
        }
#line 2499 "src/taumir/lower.tr"
        long long v2 = lower_expr(m, lf, val);
#line 2500 "src/taumir/lower.tr"
        if ((v2 < 0LL)) {
#line 2500 "src/taumir/lower.tr"
            _tr_str_release(tn);
            return false;
        }
#line 2501 "src/taumir/lower.tr"
        bool had_old_str = ((LFunc_var_index(lf, tn) >= 0LL) && (LFunc_var_type(lf, tn) == 1LL));
#line 2502 "src/taumir/lower.tr"
        long long oldot = 0LL;
#line 2503 "src/taumir/lower.tr"
        if ((LFunc_var_index(lf, tn) >= 0LL)) {
#line 2503 "src/taumir/lower.tr"
            oldot = LFunc_var_type(lf, tn);
        }
#line 2504 "src/taumir/lower.tr"
        bool had_old_obj = ((oldot == 10LL) || (oldot == 11LL));
#line 2505 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, v2) == 1LL)) {
#line 2505 "src/taumir/lower.tr"
            _secure_str(m, lf, v2);
        }
#line 2506 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, v2) == 10LL)) {
#line 2507 "src/taumir/lower.tr"
            TrStr _av_cls = _val_obj_cls(m, lf, val);
#line 2508 "src/taumir/lower.tr"
            if ((((!_is_null_str(_av_cls)) && LModule_is_value_class(m, _av_cls)) && _is_vstruct_lvalue(val))) {
#line 2509 "src/taumir/lower.tr"
                long long _acp = _copy_value_struct(m, lf, v2, _av_cls);
#line 2510 "src/taumir/lower.tr"
                if ((_acp < 0LL)) {
#line 2510 "src/taumir/lower.tr"
                    _tr_str_release(tn);
                    _tr_str_release(_av_cls);
                    return false;
                }
#line 2511 "src/taumir/lower.tr"
                v2 = _acp;
#line 2512 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, v2, 10LL);
            }
        }
#line 2513 "src/taumir/lower.tr"
        if (((LFunc_vreg_type(lf, v2) == 10LL) || (LFunc_vreg_type(lf, v2) == 11LL))) {
#line 2513 "src/taumir/lower.tr"
            _secure_obj(m, lf, v2);
        }
#line 2514 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, tn) < 0LL) && LModule_is_global(m, tn))) {
#line 2515 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreGlobal(LModule_global_index(m, tn), v2));
#line 2516 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2517 "src/taumir/lower.tr"
            _tr_str_release(tn);
            return true;
        }
#line 2518 "src/taumir/lower.tr"
        if (had_old_str) {
#line 2519 "src/taumir/lower.tr"
            long long oldv = LFunc_new_vreg(lf);
#line 2520 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(oldv, tn));
#line 2521 "src/taumir/lower.tr"
            _release_str(m, lf, oldv);
        }
#line 2522 "src/taumir/lower.tr"
        if (had_old_obj) {
#line 2523 "src/taumir/lower.tr"
            long long oldov = LFunc_new_vreg(lf);
#line 2524 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(oldov, tn));
#line 2525 "src/taumir/lower.tr"
            _release_obj(m, lf, oldov);
        }
#line 2526 "src/taumir/lower.tr"
        LFunc_add_var(lf, tn);
#line 2527 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, tn, LFunc_vreg_type(lf, v2));
#line 2528 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(tn, v2));
#line 2529 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2530 "src/taumir/lower.tr"
        _tr_str_release(tn);
        return true;
    } else if (_t6266.tag == HirStmt_SIf) {
        __auto_type cond = _t6266.data.SIf.cond;
__auto_type then_b = _t6266.data.SIf.then_b;
__auto_type else_b = _t6266.data.SIf.else_b;
#line 2532 "src/taumir/lower.tr"
        long long cv = lower_expr(m, lf, cond);
#line 2533 "src/taumir/lower.tr"
        if ((cv < 0LL)) {
#line 2533 "src/taumir/lower.tr"
            return false;
        }
#line 2534 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2535 "src/taumir/lower.tr"
        long long then_id = LFunc_new_block(lf);
#line 2536 "src/taumir/lower.tr"
        long long else_id = LFunc_new_block(lf);
#line 2537 "src/taumir/lower.tr"
        long long end_id = LFunc_new_block(lf);
#line 2538 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv, then_id, else_id));
#line 2539 "src/taumir/lower.tr"
        LFunc_set_cur(lf, then_id);
#line 2540 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, then_b))) {
#line 2540 "src/taumir/lower.tr"
            return false;
        }
#line 2541 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 2542 "src/taumir/lower.tr"
        LFunc_set_cur(lf, else_id);
#line 2543 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, else_b))) {
#line 2543 "src/taumir/lower.tr"
            return false;
        }
#line 2544 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 2545 "src/taumir/lower.tr"
        LFunc_set_cur(lf, end_id);
#line 2546 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SWhile) {
        __auto_type cond = _t6266.data.SWhile.cond;
__auto_type body = _t6266.data.SWhile.body;
#line 2548 "src/taumir/lower.tr"
        long long hdr = LFunc_new_block(lf);
#line 2549 "src/taumir/lower.tr"
        long long bdy = LFunc_new_block(lf);
#line 2550 "src/taumir/lower.tr"
        long long ext = LFunc_new_block(lf);
#line 2551 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 2552 "src/taumir/lower.tr"
        LFunc_set_cur(lf, hdr);
#line 2553 "src/taumir/lower.tr"
        long long cv2 = lower_expr(m, lf, cond);
#line 2554 "src/taumir/lower.tr"
        if ((cv2 < 0LL)) {
#line 2554 "src/taumir/lower.tr"
            return false;
        }
#line 2555 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2556 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv2, bdy, ext));
#line 2557 "src/taumir/lower.tr"
        LFunc_set_cur(lf, bdy);
#line 2558 "src/taumir/lower.tr"
        List_i64_append(lf->loop_cont, hdr);
#line 2559 "src/taumir/lower.tr"
        List_i64_append(lf->loop_brk, ext);
#line 2560 "src/taumir/lower.tr"
        bool wok = lower_block(m, lf, body);
#line 2561 "src/taumir/lower.tr"
        List_i64_pop(lf->loop_cont);
#line 2562 "src/taumir/lower.tr"
        List_i64_pop(lf->loop_brk);
#line 2563 "src/taumir/lower.tr"
        if ((!wok)) {
#line 2563 "src/taumir/lower.tr"
            return false;
        }
#line 2564 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 2565 "src/taumir/lower.tr"
        LFunc_set_cur(lf, ext);
#line 2566 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SBreak) {
        __auto_type bval = _t6266.data.SBreak.val;
#line 2568 "src/taumir/lower.tr"
        if ((((unsigned long long)(bval)) != ((unsigned long long)(0LL)))) {
#line 2568 "src/taumir/lower.tr"
            return false;
        }
#line 2569 "src/taumir/lower.tr"
        if ((lf->loop_brk->len == 0LL)) {
#line 2569 "src/taumir/lower.tr"
            return false;
        }
#line 2570 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_brk, (lf->loop_brk->len - 1LL))));
#line 2571 "src/taumir/lower.tr"
        LFunc_set_cur(lf, LFunc_new_block(lf));
#line 2572 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SContinue) {
#line 2574 "src/taumir/lower.tr"
        if ((lf->loop_cont->len == 0LL)) {
#line 2574 "src/taumir/lower.tr"
            return false;
        }
#line 2575 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_cont, (lf->loop_cont->len - 1LL))));
#line 2576 "src/taumir/lower.tr"
        LFunc_set_cur(lf, LFunc_new_block(lf));
#line 2577 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SAssert) {
        __auto_type acond = _t6266.data.SAssert.cond;
#line 2580 "src/taumir/lower.tr"
        long long acv = lower_expr(m, lf, acond);
#line 2581 "src/taumir/lower.tr"
        if ((acv < 0LL)) {
#line 2581 "src/taumir/lower.tr"
            return false;
        }
#line 2582 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2583 "src/taumir/lower.tr"
        long long a_ok = LFunc_new_block(lf);
#line 2584 "src/taumir/lower.tr"
        long long a_fail = LFunc_new_block(lf);
#line 2585 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TCondBr(acv, a_ok, a_fail));
#line 2586 "src/taumir/lower.tr"
        LFunc_set_cur(lf, a_fail);
#line 2587 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_assert_fail", 18LL));
#line 2588 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_assert_fail", 18LL), (void*)List_i64_new()));
#line 2589 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(a_ok));
#line 2590 "src/taumir/lower.tr"
        LFunc_set_cur(lf, a_ok);
#line 2591 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SMultiLet) {
        __auto_type mnames = _t6266.data.SMultiLet.names;
__auto_type mval = _t6266.data.SMultiLet.val;
#line 2594 "src/taumir/lower.tr"
        long long mtv = lower_expr(m, lf, mval);
#line 2595 "src/taumir/lower.tr"
        if ((mtv < 0LL)) {
#line 2595 "src/taumir/lower.tr"
            return false;
        }
#line 2596 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, mtv) != 15LL)) {
#line 2596 "src/taumir/lower.tr"
            return false;
        }
#line 2597 "src/taumir/lower.tr"
        AstType* mty = hir_expr_type(mval);
#line 2600 "src/taumir/lower.tr"
        if (((mty->args->len != mnames->len) && (mty->args->len != 0LL))) {
#line 2600 "src/taumir/lower.tr"
            return false;
        }
#line 2601 "src/taumir/lower.tr"
        bool use_decl = (mty->args->len == mnames->len);
#line 2602 "src/taumir/lower.tr"
        long long mi = 0LL;
#line 2603 "src/taumir/lower.tr"
        while ((mi < mnames->len)) {
#line 2604 "src/taumir/lower.tr"
            long long mtag = 0LL;
#line 2605 "src/taumir/lower.tr"
            TrStr mcls = _tr_str_lit_len("", 0LL);
#line 2606 "src/taumir/lower.tr"
            if (use_decl) {
#line 2607 "src/taumir/lower.tr"
                AstType* dty = (*((AstType**)List_ptr_get(mty->args, mi)));
#line 2608 "src/taumir/lower.tr"
                mtag = _tag_of(m, dty);
#line 2609 "src/taumir/lower.tr"
                if (((((mtag < 0LL) || _is_list_tag(mtag)) || _is_dict_tag(mtag)) || _is_set_tag(mtag))) {
#line 2609 "src/taumir/lower.tr"
                    _tr_str_release(mcls);
                    return false;
                }
#line 2610 "src/taumir/lower.tr"
                if (((mtag == 10LL) || (mtag == 11LL))) {
#line 2610 "src/taumir/lower.tr"
                    TrStr _strtmp_t6270 = _cls_of_ty(m, dty);
                    _tr_str_release(mcls);
                    mcls = _strtmp_t6270;
                }
            }
#line 2611 "src/taumir/lower.tr"
            long long mv = (-1LL);
#line 2612 "src/taumir/lower.tr"
            if ((mtag == 5LL)) {
#line 2613 "src/taumir/lower.tr"
                long long mraw = _emit_field_get(m, lf, mtv, (mi * 8LL), 0LL);
#line 2614 "src/taumir/lower.tr"
                mv = LFunc_new_vreg(lf);
#line 2615 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(mv, mraw));
#line 2616 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, mv, 5LL);
            } else {
#line 2618 "src/taumir/lower.tr"
                mv = _emit_field_get(m, lf, mtv, (mi * 8LL), mtag);
#line 2619 "src/taumir/lower.tr"
                if ((mtag == 1LL)) {
#line 2619 "src/taumir/lower.tr"
                    _secure_str(m, lf, mv);
                }
#line 2620 "src/taumir/lower.tr"
                if (((mtag == 10LL) || (mtag == 11LL))) {
#line 2620 "src/taumir/lower.tr"
                    _secure_obj(m, lf, mv);
                }
            }
#line 2621 "src/taumir/lower.tr"
            TrStr mnm = List_TrStr_get(mnames, mi);
#line 2622 "src/taumir/lower.tr"
            LFunc_add_var(lf, mnm);
#line 2623 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, mnm, mtag);
#line 2624 "src/taumir/lower.tr"
            if ((((mtag == 10LL) || (mtag == 11LL)) && (!_tr_str_eqv((mcls), (_tr_str_lit_len("", 0LL)))))) {
#line 2625 "src/taumir/lower.tr"
                LFunc_set_var_cls(lf, mnm, mcls);
            }
#line 2626 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(mnm, mv));
#line 2627 "src/taumir/lower.tr"
            mi = (mi + 1LL);
            _tr_str_release(mcls);
            _tr_str_release(mnm);
        }
#line 2628 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2629 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SDefer) {
        __auto_type dstmt = _t6266.data.SDefer.stmt;
#line 2633 "src/taumir/lower.tr"
        if (lf->in_defer) {
#line 2633 "src/taumir/lower.tr"
            return false;
        }
#line 2634 "src/taumir/lower.tr"
        if ((lf->blk_depth != 1LL)) {
#line 2634 "src/taumir/lower.tr"
            return false;
        }
#line 2635 "src/taumir/lower.tr"
        List_ptr_append(lf->defers, dstmt);
#line 2636 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SWith) {
        __auto_type witems = _t6266.data.SWith.items;
__auto_type waliases = _t6266.data.SWith.aliases;
__auto_type wbody = _t6266.data.SWith.body;
#line 2639 "src/taumir/lower.tr"
        if ((witems->len != 1LL)) {
#line 2639 "src/taumir/lower.tr"
            return false;
        }
#line 2640 "src/taumir/lower.tr"
        long long wctx = lower_expr(m, lf, ((HirExpr*)List_ptr_get(witems, 0LL)));
#line 2641 "src/taumir/lower.tr"
        if ((wctx < 0LL)) {
#line 2641 "src/taumir/lower.tr"
            return false;
        }
#line 2642 "src/taumir/lower.tr"
        TrStr wcls = _recv_class(m, lf, ((HirExpr*)List_ptr_get(witems, 0LL)));
#line 2643 "src/taumir/lower.tr"
        if ((_tr_str_eqv((wcls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, wcls)))) {
#line 2643 "src/taumir/lower.tr"
            _tr_str_release(wcls);
            return false;
        }
#line 2644 "src/taumir/lower.tr"
        TrStr w_enter = LModule_resolve_method(m, wcls, _tr_str_lit_len("__enter__", 9LL));
#line 2645 "src/taumir/lower.tr"
        TrStr w_exit = LModule_resolve_method(m, wcls, _tr_str_lit_len("__exit__", 8LL));
#line 2646 "src/taumir/lower.tr"
        if ((_tr_str_eqv((w_enter), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((w_exit), (_tr_str_lit_len("", 0LL))))) {
#line 2646 "src/taumir/lower.tr"
            _tr_str_release(wcls);
            _tr_str_release(w_enter);
            _tr_str_release(w_exit);
            return false;
        }
#line 2648 "src/taumir/lower.tr"
        TrStr w_var = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__with", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 2649 "src/taumir/lower.tr"
        LFunc_add_var(lf, w_var);
#line 2650 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, w_var, 10LL);
#line 2651 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(w_var, wctx));
#line 2655 "src/taumir/lower.tr"
        bool w_owns = _fresh_take_obj(lf, wctx);
#line 2656 "src/taumir/lower.tr"
        List_i64* w_ea = (void*)List_i64_new();
#line 2657 "src/taumir/lower.tr"
        List_i64_append(w_ea, wctx);
#line 2658 "src/taumir/lower.tr"
        long long w_rd = LFunc_new_vreg(lf);
#line 2659 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(w_rd, w_enter, w_ea));
#line 2660 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, w_rd, LModule_fn_ret_tag(m, w_enter));
#line 2661 "src/taumir/lower.tr"
        if ((((waliases->len > 0LL) && ({ TrStr _at_t6271 = (List_TrStr_get(waliases, 0LL)); __auto_type _wr = ((!_is_null_str(_at_t6271))); _tr_str_release(_at_t6271); _wr; })) && ({ TrStr _cmpa_t6272 = (List_TrStr_get(waliases, 0LL)); TrStr _cmpb_t6273 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6274 = !_tr_str_eqv(_cmpa_t6272, _cmpb_t6273); _tr_str_release(_cmpa_t6272); _cmpr_t6274; }))) {
#line 2662 "src/taumir/lower.tr"
            TrStr w_an = List_TrStr_get(waliases, 0LL);
#line 2663 "src/taumir/lower.tr"
            LFunc_add_var(lf, w_an);
#line 2664 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, w_an, LFunc_vreg_type(lf, w_rd));
#line 2665 "src/taumir/lower.tr"
            if (((LFunc_vreg_type(lf, w_rd) == 10LL) || (LFunc_vreg_type(lf, w_rd) == 11LL))) {
#line 2666 "src/taumir/lower.tr"
                ({ TrStr _at_t6275 = (_own(wcls)); LFunc_set_var_cls(lf, w_an, _at_t6275); _tr_str_release(_at_t6275); });
            }
#line 2667 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(w_an, w_rd));
        }
#line 2668 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, wbody))) {
#line 2668 "src/taumir/lower.tr"
            _tr_str_release(wcls);
            _tr_str_release(w_enter);
            _tr_str_release(w_exit);
            _tr_str_release(w_var);
            return false;
        }
#line 2671 "src/taumir/lower.tr"
        long long w_c2 = LFunc_new_vreg(lf);
#line 2672 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(w_c2, w_var));
#line 2673 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, w_c2, 10LL);
#line 2674 "src/taumir/lower.tr"
        List_i64* w_xa = (void*)List_i64_new();
#line 2675 "src/taumir/lower.tr"
        List_i64_append(w_xa, w_c2);
#line 2676 "src/taumir/lower.tr"
        long long w_ei = 0LL;
#line 2677 "src/taumir/lower.tr"
        while ((w_ei < 3LL)) {
#line 2678 "src/taumir/lower.tr"
            long long w_es = LFunc_new_vreg(lf);
#line 2679 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStr(w_es, LModule_add_string(m, _tr_str_lit_len("", 0LL), 0LL)));
#line 2680 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, w_es, 1LL);
#line 2681 "src/taumir/lower.tr"
            List_i64_append(w_xa, w_es);
#line 2682 "src/taumir/lower.tr"
            w_ei = (w_ei + 1LL);
        }
#line 2683 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), w_exit, w_xa));
#line 2684 "src/taumir/lower.tr"
        if (w_owns) {
#line 2684 "src/taumir/lower.tr"
            _release_obj(m, lf, w_c2);
        }
#line 2685 "src/taumir/lower.tr"
        _tr_str_release(wcls);
        _tr_str_release(w_enter);
        _tr_str_release(w_exit);
        _tr_str_release(w_var);
        return true;
    } else if (_t6266.tag == HirStmt_STry) {
        __auto_type try_body = _t6266.data.STry.try_body;
__auto_type catches = _t6266.data.STry.catches;
__auto_type finally_b = _t6266.data.STry.finally_b;
#line 2693 "src/taumir/lower.tr"
        if ((catches->len == 0LL)) {
#line 2694 "src/taumir/lower.tr"
            if (((!_block_has_cf(try_body)) && (!_block_has_cf(finally_b)))) {
#line 2695 "src/taumir/lower.tr"
                bool _trret1575 = _lower_try_setjmp(m, lf, try_body, catches, finally_b);
                return _trret1575;
            }
#line 2696 "src/taumir/lower.tr"
            return false;
        }
#line 2697 "src/taumir/lower.tr"
        if ((catches->len != 1LL)) {
#line 2697 "src/taumir/lower.tr"
            return false;
        }
#line 2698 "src/taumir/lower.tr"
        HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, 0LL)));
#line 2699 "src/taumir/lower.tr"
        if ((!_is_null_str(tcc->err_type->name))) {
#line 2700 "src/taumir/lower.tr"
            if ((((!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("void", 4LL))))) && (!_tr_str_eqv((tcc->err_type->name), (_tr_str_lit_len("str", 3LL)))))) {
#line 2700 "src/taumir/lower.tr"
                return false;
            }
        }
#line 2705 "src/taumir/lower.tr"
        if ((((!_block_has_cf(try_body)) && (!_block_has_cf(tcc->body))) && (!_block_has_cf(finally_b)))) {
#line 2706 "src/taumir/lower.tr"
            bool _trret1576 = _lower_try_setjmp(m, lf, try_body, catches, finally_b);
            return _trret1576;
        }
#line 2707 "src/taumir/lower.tr"
        long long t_exc = LFunc_new_block(lf);
#line 2708 "src/taumir/lower.tr"
        long long t_end = LFunc_new_block(lf);
#line 2709 "src/taumir/lower.tr"
        TrStr t_msg = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__exmsg", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 2710 "src/taumir/lower.tr"
        LFunc_add_var(lf, t_msg);
#line 2711 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, t_msg, 1LL);
#line 2712 "src/taumir/lower.tr"
        long long t_seed = _heap_lit(m, lf, _tr_str_lit_len("", 0LL));
#line 2713 "src/taumir/lower.tr"
        _fresh_take(lf, t_seed);
#line 2714 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(t_msg, t_seed));
#line 2715 "src/taumir/lower.tr"
        List_i64_append(lf->try_blks, t_exc);
#line 2716 "src/taumir/lower.tr"
        List_TrStr_append(lf->try_msgs, t_msg);
#line 2717 "src/taumir/lower.tr"
        bool t_ok = lower_block(m, lf, try_body);
#line 2718 "src/taumir/lower.tr"
        List_i64_pop(lf->try_blks);
#line 2719 "src/taumir/lower.tr"
        List_TrStr_pop(lf->try_msgs);
#line 2720 "src/taumir/lower.tr"
        if ((!t_ok)) {
#line 2720 "src/taumir/lower.tr"
            _tr_str_release(t_msg);
            return false;
        }
#line 2721 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, finally_b))) {
#line 2721 "src/taumir/lower.tr"
            _tr_str_release(t_msg);
            return false;
        }
#line 2722 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(t_end));
#line 2723 "src/taumir/lower.tr"
        LFunc_set_cur(lf, t_exc);
#line 2724 "src/taumir/lower.tr"
        if (((!_is_null_str(tcc->err_name)) && (!_tr_str_eqv((tcc->err_name), (_tr_str_lit_len("", 0LL)))))) {
#line 2725 "src/taumir/lower.tr"
            long long t_ev = LFunc_new_vreg(lf);
#line 2726 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(t_ev, t_msg));
#line 2727 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, t_ev, 1LL);
#line 2728 "src/taumir/lower.tr"
            _retain_str(m, lf, t_ev);
#line 2729 "src/taumir/lower.tr"
            LFunc_add_var(lf, tcc->err_name);
#line 2730 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, tcc->err_name, 1LL);
#line 2731 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(tcc->err_name, t_ev));
        }
#line 2732 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, tcc->body))) {
#line 2732 "src/taumir/lower.tr"
            _tr_str_release(t_msg);
            return false;
        }
#line 2733 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, finally_b))) {
#line 2733 "src/taumir/lower.tr"
            _tr_str_release(t_msg);
            return false;
        }
#line 2734 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(t_end));
#line 2735 "src/taumir/lower.tr"
        LFunc_set_cur(lf, t_end);
#line 2736 "src/taumir/lower.tr"
        _tr_str_release(t_msg);
        return true;
    } else if (_t6266.tag == HirStmt_SRaise) {
        __auto_type rval = _t6266.data.SRaise.val;
#line 2739 "src/taumir/lower.tr"
        if ((((lf->try_blks->len == 0LL) && lf->is_throws) && (((unsigned long long)(rval)) != ((unsigned long long)(0LL))))) {
#line 2740 "src/taumir/lower.tr"
            long long erv = lower_expr(m, lf, rval);
#line 2741 "src/taumir/lower.tr"
            if ((erv < 0LL)) {
#line 2741 "src/taumir/lower.tr"
                return false;
            }
#line 2742 "src/taumir/lower.tr"
            long long errr = _wrap_result(m, lf, 1LL, erv, lf->throws_err_tag);
#line 2743 "src/taumir/lower.tr"
            _fresh_take_obj(lf, errr);
#line 2744 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2745 "src/taumir/lower.tr"
            if ((!_run_defers(m, lf))) {
#line 2745 "src/taumir/lower.tr"
                return false;
            }
#line 2746 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TRetVal(errr));
#line 2747 "src/taumir/lower.tr"
            LFunc_set_cur(lf, LFunc_new_block(lf));
#line 2748 "src/taumir/lower.tr"
            return true;
        }
#line 2754 "src/taumir/lower.tr"
        if ((lf->try_blks->len == 0LL)) {
#line 2755 "src/taumir/lower.tr"
            if ((((unsigned long long)(rval)) == ((unsigned long long)(0LL)))) {
#line 2755 "src/taumir/lower.tr"
                return false;
            }
#line 2756 "src/taumir/lower.tr"
            long long pv = lower_expr(m, lf, rval);
#line 2757 "src/taumir/lower.tr"
            if ((pv < 0LL)) {
#line 2757 "src/taumir/lower.tr"
                return false;
            }
#line 2758 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, pv) != 1LL)) {
#line 2758 "src/taumir/lower.tr"
                return false;
            }
#line 2759 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_exc_raise", 13LL));
#line 2760 "src/taumir/lower.tr"
            List_i64* pa = (void*)List_i64_new();
#line 2761 "src/taumir/lower.tr"
            List_i64_append(pa, pv);
#line 2762 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_exc_raise", 13LL), pa));
#line 2763 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 2764 "src/taumir/lower.tr"
            return true;
        }
#line 2765 "src/taumir/lower.tr"
        if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
#line 2766 "src/taumir/lower.tr"
            long long rv = lower_expr(m, lf, rval);
#line 2767 "src/taumir/lower.tr"
            if ((rv < 0LL)) {
#line 2767 "src/taumir/lower.tr"
                return false;
            }
#line 2768 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, rv) != 1LL)) {
#line 2768 "src/taumir/lower.tr"
                return false;
            }
#line 2769 "src/taumir/lower.tr"
            _secure_str(m, lf, rv);
#line 2770 "src/taumir/lower.tr"
            ({ TrStr _at_t6276 = (List_TrStr_get(lf->try_msgs, (lf->try_msgs->len - 1LL))); LFunc_emit(lf, LInst_ctor_IStoreVar(_at_t6276, rv)); _tr_str_release(_at_t6276); });
        }
#line 2771 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2772 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->try_blks, (lf->try_blks->len - 1LL))));
#line 2773 "src/taumir/lower.tr"
        LFunc_set_cur(lf, LFunc_new_block(lf));
#line 2774 "src/taumir/lower.tr"
        return true;
    } else if (_t6266.tag == HirStmt_SMatch) {
        __auto_type mexpr = _t6266.data.SMatch.expr;
__auto_type marms = _t6266.data.SMatch.arms;
#line 2776 "src/taumir/lower.tr"
        return _lower_match(m, lf, mexpr, marms);
    } else if (_t6266.tag == HirStmt_SFor) {
        __auto_type var = _t6266.data.SFor.var;
__auto_type iter = _t6266.data.SFor.iter;
__auto_type body = _t6266.data.SFor.body;
#line 2778 "src/taumir/lower.tr"
        bool _trret1577 = _lower_for(m, lf, var, iter, body);
        return _trret1577;
    } else if (_t6266.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t6266.data.SForUnpack.vars;
__auto_type iter = _t6266.data.SForUnpack.iter;
__auto_type body = _t6266.data.SForUnpack.body;
#line 2780 "src/taumir/lower.tr"
        bool _trret1578 = _lower_for_unpack(m, lf, vars, iter, body);
        return _trret1578;
    } else if (_t6266.tag == HirStmt_SExpr) {
        __auto_type e = _t6266.data.SExpr.expr;
#line 2782 "src/taumir/lower.tr"
        bool se_ok = lower_expr_stmt(m, lf, e);
#line 2783 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 2784 "src/taumir/lower.tr"
        if (((!se_ok) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
#line 2785 "src/taumir/lower.tr"
            m->fail_note = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_stmt_expr_kind(e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("unsupported expression statement (", 34LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        }
#line 2786 "src/taumir/lower.tr"
        return se_ok;
    } else if (_t6266.tag == HirStmt_SUnsafe) {
        __auto_type ubody = _t6266.data.SUnsafe.body;
#line 2788 "src/taumir/lower.tr"
        bool _trret1579 = lower_block(m, lf, ubody);
        return _trret1579;
    } else if (_t6266.tag == HirStmt_SSpawn) {
        __auto_type sexpr = _t6266.data.SSpawn.expr;
#line 2790 "src/taumir/lower.tr"
        return _lower_spawn(m, lf, sexpr);
    } else if (_t6266.tag == HirStmt_STaskGroup) {
        __auto_type tgbody = _t6266.data.STaskGroup.body;
#line 2792 "src/taumir/lower.tr"
        bool _trret1580 = _lower_taskgroup(m, lf, tgbody);
        return _trret1580;
    } else if (_t6266.tag == HirStmt_SChanSelect) {
        __auto_type scases = _t6266.data.SChanSelect.cases;
#line 2794 "src/taumir/lower.tr"
        return _lower_chan_select(m, lf, scases);
    } else if (_t6266.tag == HirStmt_SAsm) {
        __auto_type code = _t6266.data.SAsm.code;
__auto_type outputs = _t6266.data.SAsm.outputs;
__auto_type inputs = _t6266.data.SAsm.inputs;
__auto_type clobbers = _t6266.data.SAsm.clobbers;
#line 2800 "src/taumir/lower.tr"
        if (((!_is_null_str(outputs)) && (!_tr_str_eqv((outputs), (_tr_str_lit_len("", 0LL)))))) {
#line 2800 "src/taumir/lower.tr"
            return false;
        }
#line 2801 "src/taumir/lower.tr"
        if (((!_is_null_str(inputs)) && (!_tr_str_eqv((inputs), (_tr_str_lit_len("", 0LL)))))) {
#line 2801 "src/taumir/lower.tr"
            return false;
        }
#line 2802 "src/taumir/lower.tr"
        TrStr cons = _tr_str_lit_len("", 0LL);
#line 2803 "src/taumir/lower.tr"
        if (((!_is_null_str(clobbers)) && (!_tr_str_eqv((clobbers), (_tr_str_lit_len("", 0LL)))))) {
#line 2804 "src/taumir/lower.tr"
            if (_tr_str_containsv((clobbers), (_tr_str_lit_len(",", 1LL)))) {
#line 2804 "src/taumir/lower.tr"
                _tr_str_release(cons);
                return false;
            }
#line 2805 "src/taumir/lower.tr"
            TrStr _strtmp_t6277 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("~{", 2LL)), (clobbers))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("}", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(cons);
            cons = _strtmp_t6277;
        }
#line 2806 "src/taumir/lower.tr"
        ({ TrStr _at_t6278 = (_own(code)); LFunc_emit(lf, LInst_ctor_IAsm(_at_t6278, cons)); _tr_str_release(_at_t6278); });
#line 2807 "src/taumir/lower.tr"
        _tr_str_release(cons);
        return true;
    } else if (1) {
        __auto_type _ = _t6266;
#line 2809 "src/taumir/lower.tr"
        if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 2809 "src/taumir/lower.tr"
            m->fail_note = ({ TrStr _tr_cr = (_stmt_kind(s)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("unsupported statement kind: ", 28LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        }
#line 2810 "src/taumir/lower.tr"
        return false;
    }
}

__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s) {
#line 2813 "src/taumir/lower.tr"
    bool ok = _lower_stmt_impl(m, lf, s);
#line 2814 "src/taumir/lower.tr"
    if (((!ok) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
#line 2815 "src/taumir/lower.tr"
        m->fail_note = ({ TrStr _tr_cr = (_stmt_kind(s)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("unsupported statement: ", 23LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    }
#line 2816 "src/taumir/lower.tr"
    return ok;
}

__attribute__((hot)) long long _lower_set_method(LModule* m, LFunc* lf, long long shv, long long stag, TrStr method, List_ptr* margs) {
#line 2821 "src/taumir/lower.tr"
    long long want_e = 0LL;
#line 2822 "src/taumir/lower.tr"
    if ((stag == 16LL)) {
#line 2822 "src/taumir/lower.tr"
        want_e = 1LL;
    }
#line 2823 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL))))) {
#line 2824 "src/taumir/lower.tr"
        TrStr slsym = _set_sym(stag, _tr_str_lit_len("len", 3LL));
#line 2825 "src/taumir/lower.tr"
        LModule_add_extern(m, slsym);
#line 2826 "src/taumir/lower.tr"
        List_i64* sla = (void*)List_i64_new();
#line 2827 "src/taumir/lower.tr"
        List_i64_append(sla, shv);
#line 2828 "src/taumir/lower.tr"
        long long sld = LFunc_new_vreg(lf);
#line 2829 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sld, slsym, sla));
#line 2830 "src/taumir/lower.tr"
        long long _trret1581 = sld;
        _tr_str_release(slsym);
        return _trret1581;
    }
#line 2831 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL)))) {
#line 2832 "src/taumir/lower.tr"
        TrStr sesym = _set_sym(stag, _tr_str_lit_len("len", 3LL));
#line 2833 "src/taumir/lower.tr"
        LModule_add_extern(m, sesym);
#line 2834 "src/taumir/lower.tr"
        List_i64* sea = (void*)List_i64_new();
#line 2835 "src/taumir/lower.tr"
        List_i64_append(sea, shv);
#line 2836 "src/taumir/lower.tr"
        long long sed = LFunc_new_vreg(lf);
#line 2837 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sed, sesym, sea));
#line 2838 "src/taumir/lower.tr"
        long long sez = LFunc_new_vreg(lf);
#line 2839 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(sez, 0LL));
#line 2840 "src/taumir/lower.tr"
        long long ser = LFunc_new_vreg(lf);
#line 2841 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(ser, _tr_str_lit_len("==", 2LL), sed, sez));
#line 2842 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, ser, 4LL);
#line 2843 "src/taumir/lower.tr"
        long long _trret1582 = ser;
        _tr_str_release(sesym);
        return _trret1582;
    }
#line 2844 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_list", 7LL))) && (margs->len == 0LL))) {
#line 2845 "src/taumir/lower.tr"
        TrStr tlsym = _tr_str_lit_len("_tr_rt_iset_to_list", 19LL);
#line 2846 "src/taumir/lower.tr"
        long long tltag = 2LL;
#line 2847 "src/taumir/lower.tr"
        if ((stag == 16LL)) {
#line 2848 "src/taumir/lower.tr"
            TrStr _strtmp_t6279 = _tr_str_lit_len("_tr_rt_sset_to_list", 19LL);
            _tr_str_release(tlsym);
            tlsym = _strtmp_t6279;
#line 2849 "src/taumir/lower.tr"
            tltag = 3LL;
        }
#line 2850 "src/taumir/lower.tr"
        LModule_add_extern(m, tlsym);
#line 2851 "src/taumir/lower.tr"
        List_i64* tla = (void*)List_i64_new();
#line 2852 "src/taumir/lower.tr"
        List_i64_append(tla, shv);
#line 2853 "src/taumir/lower.tr"
        long long tld = LFunc_new_vreg(lf);
#line 2854 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(tld, tlsym, tla));
#line 2855 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, tld, tltag);
#line 2856 "src/taumir/lower.tr"
        long long _trret1583 = tld;
        _tr_str_release(tlsym);
        return _trret1583;
    }
#line 2857 "src/taumir/lower.tr"
    if ((margs->len != 1LL)) {
#line 2857 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 2858 "src/taumir/lower.tr"
    long long sav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 2859 "src/taumir/lower.tr"
    if ((sav < 0LL)) {
#line 2859 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 2860 "src/taumir/lower.tr"
    if ((LFunc_vreg_type(lf, sav) != want_e)) {
#line 2860 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 2861 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) {
#line 2862 "src/taumir/lower.tr"
        TrStr sasym = _set_sym(stag, _tr_str_lit_len("set", 3LL));
#line 2863 "src/taumir/lower.tr"
        LModule_add_extern(m, sasym);
#line 2864 "src/taumir/lower.tr"
        long long saone = LFunc_new_vreg(lf);
#line 2865 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(saone, 1LL));
#line 2866 "src/taumir/lower.tr"
        List_i64* saa = (void*)List_i64_new();
#line 2867 "src/taumir/lower.tr"
        List_i64_append(saa, shv);
#line 2868 "src/taumir/lower.tr"
        List_i64_append(saa, sav);
#line 2869 "src/taumir/lower.tr"
        List_i64_append(saa, saone);
#line 2870 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sasym, saa));
#line 2871 "src/taumir/lower.tr"
        long long _trret1584 = shv;
        _tr_str_release(sasym);
        return _trret1584;
    }
#line 2872 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))))) {
#line 2873 "src/taumir/lower.tr"
        TrStr scsym = _set_sym(stag, _tr_str_lit_len("has", 3LL));
#line 2874 "src/taumir/lower.tr"
        LModule_add_extern(m, scsym);
#line 2875 "src/taumir/lower.tr"
        List_i64* sca = (void*)List_i64_new();
#line 2876 "src/taumir/lower.tr"
        List_i64_append(sca, shv);
#line 2877 "src/taumir/lower.tr"
        List_i64_append(sca, sav);
#line 2878 "src/taumir/lower.tr"
        long long scd = LFunc_new_vreg(lf);
#line 2879 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(scd, scsym, sca));
#line 2880 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, scd, 4LL);
#line 2881 "src/taumir/lower.tr"
        long long _trret1585 = scd;
        _tr_str_release(scsym);
        return _trret1585;
    }
#line 2882 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) {
#line 2883 "src/taumir/lower.tr"
        TrStr srsym = _set_sym(stag, _tr_str_lit_len("remove", 6LL));
#line 2884 "src/taumir/lower.tr"
        LModule_add_extern(m, srsym);
#line 2885 "src/taumir/lower.tr"
        List_i64* sra = (void*)List_i64_new();
#line 2886 "src/taumir/lower.tr"
        List_i64_append(sra, shv);
#line 2887 "src/taumir/lower.tr"
        List_i64_append(sra, sav);
#line 2888 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), srsym, sra));
#line 2889 "src/taumir/lower.tr"
        long long _trret1586 = shv;
        _tr_str_release(srsym);
        return _trret1586;
    }
#line 2890 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) long long _lit_pat_cond(LModule* m, LFunc* lf, Pattern pat, long long subj, long long st) {
#line 2896 "src/taumir/lower.tr"
    __auto_type _t6280 = pat;
    if (_t6280.tag == Pattern_PLitInt) {
        __auto_type v = _t6280.data.PLitInt.val;
#line 2898 "src/taumir/lower.tr"
        if ((st != 0LL)) {
#line 2898 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 2899 "src/taumir/lower.tr"
        long long cv = LFunc_new_vreg(lf);
#line 2900 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(cv, v));
#line 2901 "src/taumir/lower.tr"
        long long d = LFunc_new_vreg(lf);
#line 2902 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(d, _tr_str_lit_len("==", 2LL), subj, cv));
#line 2903 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, d, 4LL);
#line 2904 "src/taumir/lower.tr"
        return d;
    } else if (_t6280.tag == Pattern_PLitBool) {
        __auto_type bv = _t6280.data.PLitBool.val;
#line 2906 "src/taumir/lower.tr"
        if ((st != 4LL)) {
#line 2906 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 2907 "src/taumir/lower.tr"
        long long bc = 0LL;
#line 2908 "src/taumir/lower.tr"
        if (bv) {
#line 2908 "src/taumir/lower.tr"
            bc = 1LL;
        }
#line 2909 "src/taumir/lower.tr"
        long long cvb = LFunc_new_vreg(lf);
#line 2910 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(cvb, bc));
#line 2911 "src/taumir/lower.tr"
        long long db = LFunc_new_vreg(lf);
#line 2912 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(db, _tr_str_lit_len("==", 2LL), subj, cvb));
#line 2913 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, db, 4LL);
#line 2914 "src/taumir/lower.tr"
        return db;
    } else if (_t6280.tag == Pattern_PLitStr) {
        __auto_type sv = _t6280.data.PLitStr.val;
__auto_type sv_blen = _t6280.data.PLitStr.blen;
#line 2916 "src/taumir/lower.tr"
        if ((st != 1LL)) {
#line 2916 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 2917 "src/taumir/lower.tr"
        long long idx = LModule_add_string(m, sv, sv_blen);
#line 2918 "src/taumir/lower.tr"
        long long lit = LFunc_new_vreg(lf);
#line 2919 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStr(lit, idx));
#line 2920 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, lit, 1LL);
#line 2921 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
#line 2922 "src/taumir/lower.tr"
        List_i64* sa = (void*)List_i64_new();
#line 2923 "src/taumir/lower.tr"
        List_i64_append(sa, subj);
#line 2924 "src/taumir/lower.tr"
        List_i64_append(sa, lit);
#line 2925 "src/taumir/lower.tr"
        long long cmpv = LFunc_new_vreg(lf);
#line 2926 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sa));
#line 2927 "src/taumir/lower.tr"
        long long z = LFunc_new_vreg(lf);
#line 2928 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 2929 "src/taumir/lower.tr"
        long long ds = LFunc_new_vreg(lf);
#line 2930 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(ds, _tr_str_lit_len("==", 2LL), cmpv, z));
#line 2931 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, ds, 4LL);
#line 2932 "src/taumir/lower.tr"
        long long _trret1587 = ds;
        return _trret1587;
    } else if (1) {
        __auto_type _ = _t6280;
#line 2934 "src/taumir/lower.tr"
        return (-1LL);
    }
}

__attribute__((hot)) bool _lower_match(LModule* m, LFunc* lf, HirExpr* expr, List_ptr* arms) {
#line 2942 "src/taumir/lower.tr"
    TrStr subj_ty = _tr_str_retain(hir_expr_type(expr)->name);
#line 2943 "src/taumir/lower.tr"
    bool subj_is_str = false;
#line 2944 "src/taumir/lower.tr"
    if ((!_is_null_str(subj_ty))) {
#line 2944 "src/taumir/lower.tr"
        subj_is_str = _tr_str_eqv((subj_ty), (_tr_str_lit_len("str", 3LL)));
    }
#line 2945 "src/taumir/lower.tr"
    if (subj_is_str) {
#line 2947 "src/taumir/lower.tr"
        __auto_type _t6281 = (*expr);
        if (_t6281.tag == HirExpr_EIdent) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t6281;
            _tr_str_release(subj_ty);
            return false;
        }
    }
#line 2950 "src/taumir/lower.tr"
    long long subj = lower_expr(m, lf, expr);
#line 2951 "src/taumir/lower.tr"
    if ((subj < 0LL)) {
#line 2951 "src/taumir/lower.tr"
        _tr_str_release(subj_ty);
        return false;
    }
#line 2952 "src/taumir/lower.tr"
    long long st = LFunc_vreg_type(lf, subj);
#line 2953 "src/taumir/lower.tr"
    if ((st == 11LL)) {
#line 2954 "src/taumir/lower.tr"
        bool _trret1588 = _lower_match_enum(m, lf, expr, subj, arms);
        _tr_str_release(subj_ty);
        return _trret1588;
    }
#line 2955 "src/taumir/lower.tr"
    if ((((st != 0LL) && (st != 1LL)) && (st != 4LL))) {
#line 2955 "src/taumir/lower.tr"
        _tr_str_release(subj_ty);
        return false;
    }
#line 2956 "src/taumir/lower.tr"
    _flush_fresh_strs(m, lf);
#line 2957 "src/taumir/lower.tr"
    long long end_id = LFunc_new_block(lf);
#line 2958 "src/taumir/lower.tr"
    long long i = 0LL;
#line 2959 "src/taumir/lower.tr"
    while ((i < arms->len)) {
#line 2960 "src/taumir/lower.tr"
        HirMatchArm* arm = ((HirMatchArm*)List_ptr_get(arms, i));
#line 2961 "src/taumir/lower.tr"
        bool is_default = false;
#line 2962 "src/taumir/lower.tr"
        TrStr bind_name = _tr_str_lit_len("", 0LL);
#line 2963 "src/taumir/lower.tr"
        long long cond = (-1LL);
#line 2964 "src/taumir/lower.tr"
        __auto_type _t6282 = arm->pat;
        if (_t6282.tag == Pattern_PWild) {
#line 2966 "src/taumir/lower.tr"
            is_default = true;
        } else if (_t6282.tag == Pattern_PBind) {
            __auto_type nm = _t6282.data.PBind.name;
#line 2968 "src/taumir/lower.tr"
            is_default = true;
#line 2969 "src/taumir/lower.tr"
            TrStr _strtmp_t6283 = _tr_str_retain(nm);
            _tr_str_release(bind_name);
            bind_name = _strtmp_t6283;
        } else if (_t6282.tag == Pattern_POr) {
            __auto_type pats = _t6282.data.POr.patterns;
#line 2972 "src/taumir/lower.tr"
            long long oi = 0LL;
#line 2973 "src/taumir/lower.tr"
            while ((oi < pats->len)) {
#line 2974 "src/taumir/lower.tr"
                long long sc = _lit_pat_cond(m, lf, List_Pattern_get(pats, oi), subj, st);
#line 2975 "src/taumir/lower.tr"
                if ((sc < 0LL)) {
#line 2975 "src/taumir/lower.tr"
                    _tr_str_release(subj_ty);
                    return false;
                }
#line 2976 "src/taumir/lower.tr"
                if ((cond < 0LL)) {
#line 2977 "src/taumir/lower.tr"
                    cond = sc;
                } else {
#line 2979 "src/taumir/lower.tr"
                    long long merged = LFunc_new_vreg(lf);
#line 2980 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit_len("+", 1LL), cond, sc));
#line 2981 "src/taumir/lower.tr"
                    cond = _norm_bool(lf, merged);
                }
#line 2982 "src/taumir/lower.tr"
                oi = (oi + 1LL);
            }
#line 2983 "src/taumir/lower.tr"
            if ((cond < 0LL)) {
#line 2983 "src/taumir/lower.tr"
                _tr_str_release(subj_ty);
                return false;
            }
        } else if (1) {
            __auto_type _ = _t6282;
#line 2985 "src/taumir/lower.tr"
            cond = _lit_pat_cond(m, lf, arm->pat, subj, st);
#line 2986 "src/taumir/lower.tr"
            if ((cond < 0LL)) {
#line 2986 "src/taumir/lower.tr"
                _tr_str_release(subj_ty);
                return false;
            }
        }
#line 2987 "src/taumir/lower.tr"
        long long body_id = LFunc_new_block(lf);
#line 2988 "src/taumir/lower.tr"
        long long next_id = LFunc_new_block(lf);
#line 2989 "src/taumir/lower.tr"
        if (is_default) {
#line 2990 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TBr(body_id));
        } else {
#line 2992 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, body_id, next_id));
        }
#line 2993 "src/taumir/lower.tr"
        LFunc_set_cur(lf, body_id);
#line 2994 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((bind_name), (_tr_str_lit_len("", 0LL))))) {
#line 2995 "src/taumir/lower.tr"
            LFunc_add_var(lf, bind_name);
#line 2996 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, bind_name, st);
#line 2997 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(bind_name, subj));
        }
#line 2999 "src/taumir/lower.tr"
        if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
#line 3000 "src/taumir/lower.tr"
            long long gv = lower_expr(m, lf, arm->guard);
#line 3001 "src/taumir/lower.tr"
            if ((gv < 0LL)) {
#line 3001 "src/taumir/lower.tr"
                _tr_str_release(subj_ty);
                _tr_str_release(bind_name);
                return false;
            }
#line 3002 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 3003 "src/taumir/lower.tr"
            long long gbody = LFunc_new_block(lf);
#line 3004 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TCondBr(gv, gbody, next_id));
#line 3005 "src/taumir/lower.tr"
            LFunc_set_cur(lf, gbody);
        }
#line 3006 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, arm->body))) {
#line 3006 "src/taumir/lower.tr"
            _tr_str_release(subj_ty);
            _tr_str_release(bind_name);
            return false;
        }
#line 3007 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 3008 "src/taumir/lower.tr"
        LFunc_set_cur(lf, next_id);
#line 3009 "src/taumir/lower.tr"
        i = (i + 1LL);
        _tr_str_release(bind_name);
    }
#line 3010 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 3011 "src/taumir/lower.tr"
    LFunc_set_cur(lf, end_id);
#line 3012 "src/taumir/lower.tr"
    _tr_str_release(subj_ty);
    return true;
}

__attribute__((hot)) TrStr _norm_variant(TrStr ename, TrStr vn) {
#line 3018 "src/taumir/lower.tr"
    if (_tr_str_eqv((ename), (_tr_str_lit_len("Option", 6LL)))) {
#line 3019 "src/taumir/lower.tr"
        if (((_is_null_str(vn) || _tr_str_eqv((vn), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((vn), (_tr_str_lit_len("none", 4LL))))) {
#line 3019 "src/taumir/lower.tr"
            return _tr_str_lit_len("None", 4LL);
        }
#line 3020 "src/taumir/lower.tr"
        if (_tr_str_eqv((vn), (_tr_str_lit_len("some", 4LL)))) {
#line 3020 "src/taumir/lower.tr"
            return _tr_str_lit_len("Some", 4LL);
        }
    }
#line 3021 "src/taumir/lower.tr"
    if (_tr_str_eqv((ename), (_tr_str_lit_len("Result", 6LL)))) {
#line 3022 "src/taumir/lower.tr"
        if (_tr_str_eqv((vn), (_tr_str_lit_len("ok", 2LL)))) {
#line 3022 "src/taumir/lower.tr"
            return _tr_str_lit_len("Ok", 2LL);
        }
#line 3023 "src/taumir/lower.tr"
        if (_tr_str_eqv((vn), (_tr_str_lit_len("err", 3LL)))) {
#line 3023 "src/taumir/lower.tr"
            return _tr_str_lit_len("Err", 3LL);
        }
    }
#line 3024 "src/taumir/lower.tr"
    return _tr_str_retain(vn);
}

__attribute__((hot)) long long _variant_tag_cond(LFunc* lf, long long tagv, long long vidx) {
#line 3028 "src/taumir/lower.tr"
    long long cv = LFunc_new_vreg(lf);
#line 3029 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(cv, vidx));
#line 3030 "src/taumir/lower.tr"
    long long dv = LFunc_new_vreg(lf);
#line 3031 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(dv, _tr_str_lit_len("==", 2LL), tagv, cv));
#line 3032 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dv, 4LL);
#line 3033 "src/taumir/lower.tr"
    return dv;
}

__attribute__((hot)) long long _load_enum_payload_field(LModule* m, LFunc* lf, long long subj, AstType* subj_ty, VariantLayout* vlay, long long fldidx) {
#line 3037 "src/taumir/lower.tr"
    if ((fldidx >= vlay->ftags->len)) {
#line 3037 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 3038 "src/taumir/lower.tr"
    long long ftg = List_i64_get(vlay->ftags, fldidx);
#line 3039 "src/taumir/lower.tr"
    TrStr bcls = List_TrStr_get(vlay->fcls, fldidx);
#line 3040 "src/taumir/lower.tr"
    if ((ftg <= (0LL - 2LL))) {
#line 3041 "src/taumir/lower.tr"
        long long bgi = ((0LL - ftg) - 2LL);
#line 3042 "src/taumir/lower.tr"
        if ((bgi >= subj_ty->args->len)) {
#line 3042 "src/taumir/lower.tr"
            _tr_str_release(bcls);
            return (-1LL);
        }
#line 3043 "src/taumir/lower.tr"
        ftg = _tag_of(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
#line 3044 "src/taumir/lower.tr"
        TrStr _strtmp_t6284 = _cls_of_ty(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        _tr_str_release(bcls);
        bcls = _strtmp_t6284;
    }
#line 3045 "src/taumir/lower.tr"
    if ((((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
#line 3045 "src/taumir/lower.tr"
        _tr_str_release(bcls);
        return (-1LL);
    }
#line 3046 "src/taumir/lower.tr"
    if ((ftg == 5LL)) {
#line 3047 "src/taumir/lower.tr"
        long long praw = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), 0LL);
#line 3048 "src/taumir/lower.tr"
        long long pv = LFunc_new_vreg(lf);
#line 3049 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(pv, praw));
#line 3050 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, pv, 5LL);
#line 3051 "src/taumir/lower.tr"
        long long _trret1589 = pv;
        _tr_str_release(bcls);
        return _trret1589;
    }
#line 3052 "src/taumir/lower.tr"
    long long pv2 = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), ftg);
#line 3053 "src/taumir/lower.tr"
    long long _trret1590 = pv2;
    _tr_str_release(bcls);
    return _trret1590;
}

__attribute__((hot)) long long _lower_enum_prop(LModule* m, LFunc* lf, HirExpr* obj, TrStr ename, TrStr prop) {
#line 3057 "src/taumir/lower.tr"
    long long subj = lower_expr(m, lf, obj);
#line 3058 "src/taumir/lower.tr"
    if ((subj < 0LL)) {
#line 3058 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 3059 "src/taumir/lower.tr"
    long long eidx = LModule_enum_index(m, ename);
#line 3060 "src/taumir/lower.tr"
    if ((eidx < 0LL)) {
#line 3060 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 3061 "src/taumir/lower.tr"
    EnumLayout* elay = ((EnumLayout*)List_ptr_get(m->enums, eidx));
#line 3062 "src/taumir/lower.tr"
    AstType* subj_ty = hir_expr_type(obj);
#line 3063 "src/taumir/lower.tr"
    if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_ok", 5LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_some", 7LL))))) {
#line 3064 "src/taumir/lower.tr"
        long long vix = ({ TrStr _at_t6285 = (_norm_variant(ename, _tr_str_lit_len("Ok", 2LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6285)); _tr_str_release(_at_t6285); _wr; });
#line 3065 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3065 "src/taumir/lower.tr"
            vix = ({ TrStr _at_t6286 = (_norm_variant(ename, _tr_str_lit_len("Some", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6286)); _tr_str_release(_at_t6286); _wr; });
        }
#line 3066 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3066 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 3067 "src/taumir/lower.tr"
        long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
#line 3068 "src/taumir/lower.tr"
        long long _trret1591 = _variant_tag_cond(lf, tagv, vix);
        return _trret1591;
    }
#line 3069 "src/taumir/lower.tr"
    if ((_tr_str_eqv((prop), (_tr_str_lit_len("is_err", 6LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("is_none", 7LL))))) {
#line 3070 "src/taumir/lower.tr"
        long long vix = ({ TrStr _at_t6287 = (_norm_variant(ename, _tr_str_lit_len("Err", 3LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6287)); _tr_str_release(_at_t6287); _wr; });
#line 3071 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3071 "src/taumir/lower.tr"
            vix = ({ TrStr _at_t6288 = (_norm_variant(ename, _tr_str_lit_len("None", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6288)); _tr_str_release(_at_t6288); _wr; });
        }
#line 3072 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3072 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 3073 "src/taumir/lower.tr"
        long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
#line 3074 "src/taumir/lower.tr"
        long long _trret1592 = _variant_tag_cond(lf, tagv, vix);
        return _trret1592;
    }
#line 3075 "src/taumir/lower.tr"
    if (((_tr_str_eqv((prop), (_tr_str_lit_len("ok", 2LL))) || _tr_str_eqv((prop), (_tr_str_lit_len("value", 5LL)))) || _tr_str_eqv((prop), (_tr_str_lit_len("some", 4LL))))) {
#line 3076 "src/taumir/lower.tr"
        long long vix = ({ TrStr _at_t6289 = (_norm_variant(ename, _tr_str_lit_len("Ok", 2LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6289)); _tr_str_release(_at_t6289); _wr; });
#line 3077 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3077 "src/taumir/lower.tr"
            vix = ({ TrStr _at_t6290 = (_norm_variant(ename, _tr_str_lit_len("Some", 4LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6290)); _tr_str_release(_at_t6290); _wr; });
        }
#line 3078 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3078 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 3079 "src/taumir/lower.tr"
        long long _trret1593 = _load_enum_payload_field(m, lf, subj, subj_ty, ((VariantLayout*)List_ptr_get(elay->variants, vix)), 0LL);
        return _trret1593;
    }
#line 3080 "src/taumir/lower.tr"
    if (_tr_str_eqv((prop), (_tr_str_lit_len("err", 3LL)))) {
#line 3081 "src/taumir/lower.tr"
        long long vix = ({ TrStr _at_t6291 = (_norm_variant(ename, _tr_str_lit_len("Err", 3LL))); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6291)); _tr_str_release(_at_t6291); _wr; });
#line 3082 "src/taumir/lower.tr"
        if ((vix < 0LL)) {
#line 3082 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 3083 "src/taumir/lower.tr"
        long long _trret1594 = _load_enum_payload_field(m, lf, subj, subj_ty, ((VariantLayout*)List_ptr_get(elay->variants, vix)), 0LL);
        return _trret1594;
    }
#line 3084 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _bind_payload(LModule* m, LFunc* lf, VariantLayout* vlay, long long subj, AstType* subj_ty, long long fldidx, TrStr bindname) {
#line 3090 "src/taumir/lower.tr"
    if ((fldidx >= vlay->ftags->len)) {
#line 3090 "src/taumir/lower.tr"
        return false;
    }
#line 3091 "src/taumir/lower.tr"
    long long ftg = List_i64_get(vlay->ftags, fldidx);
#line 3092 "src/taumir/lower.tr"
    TrStr bcls = List_TrStr_get(vlay->fcls, fldidx);
#line 3093 "src/taumir/lower.tr"
    if ((ftg <= (0LL - 2LL))) {
#line 3094 "src/taumir/lower.tr"
        long long bgi = ((0LL - ftg) - 2LL);
#line 3095 "src/taumir/lower.tr"
        if ((bgi >= subj_ty->args->len)) {
#line 3095 "src/taumir/lower.tr"
            _tr_str_release(bcls);
            return false;
        }
#line 3096 "src/taumir/lower.tr"
        ftg = _tag_of(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
#line 3097 "src/taumir/lower.tr"
        TrStr _strtmp_t6292 = _cls_of_ty(m, (*((AstType**)List_ptr_get(subj_ty->args, bgi))));
        _tr_str_release(bcls);
        bcls = _strtmp_t6292;
    }
#line 3098 "src/taumir/lower.tr"
    if ((((((ftg < 0LL) || _is_list_tag(ftg)) || _is_dict_tag(ftg)) || _is_set_tag(ftg)) || (ftg == 12LL))) {
#line 3098 "src/taumir/lower.tr"
        _tr_str_release(bcls);
        return false;
    }
#line 3099 "src/taumir/lower.tr"
    long long pv = (-1LL);
#line 3100 "src/taumir/lower.tr"
    if ((ftg == 5LL)) {
#line 3101 "src/taumir/lower.tr"
        long long praw = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), 0LL);
#line 3102 "src/taumir/lower.tr"
        pv = LFunc_new_vreg(lf);
#line 3103 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(pv, praw));
#line 3104 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, pv, 5LL);
    } else {
#line 3106 "src/taumir/lower.tr"
        pv = _emit_field_get(m, lf, subj, ((1LL + fldidx) * 8LL), ftg);
#line 3109 "src/taumir/lower.tr"
        if ((ftg == 1LL)) {
#line 3109 "src/taumir/lower.tr"
            _secure_str(m, lf, pv);
        }
#line 3110 "src/taumir/lower.tr"
        if (((ftg == 10LL) || (ftg == 11LL))) {
#line 3110 "src/taumir/lower.tr"
            _secure_obj(m, lf, pv);
        }
    }
#line 3111 "src/taumir/lower.tr"
    LFunc_add_var(lf, bindname);
#line 3112 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, bindname, ftg);
#line 3113 "src/taumir/lower.tr"
    if (((ftg == 10LL) || (ftg == 11LL))) {
#line 3114 "src/taumir/lower.tr"
        if (((!_is_null_str(bcls)) && (!_tr_str_eqv((bcls), (_tr_str_lit_len("", 0LL)))))) {
#line 3114 "src/taumir/lower.tr"
            ({ TrStr _at_t6293 = (_own(bcls)); LFunc_set_var_cls(lf, bindname, _at_t6293); _tr_str_release(_at_t6293); });
        }
    }
#line 3115 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(bindname, pv));
#line 3116 "src/taumir/lower.tr"
    _tr_str_release(bcls);
    return true;
}

__attribute__((hot)) bool _lower_match_enum(LModule* m, LFunc* lf, HirExpr* expr, long long subj, List_ptr* arms) {
#line 3121 "src/taumir/lower.tr"
    TrStr ename = _recv_class(m, lf, expr);
#line 3122 "src/taumir/lower.tr"
    if ((_tr_str_eqv((ename), (_tr_str_lit_len("", 0LL))) || (!LModule_is_enum(m, ename)))) {
#line 3122 "src/taumir/lower.tr"
        _tr_str_release(ename);
        return false;
    }
#line 3123 "src/taumir/lower.tr"
    AstType* subj_hty = hir_expr_type(expr);
#line 3124 "src/taumir/lower.tr"
    EnumLayout* elay = ({ __auto_type _ord_t6294 = (LModule_enum_index(m, ename)); __auto_type _wr = (((EnumLayout*)List_ptr_get(m->enums, _ord_t6294))); _wr; });
#line 3125 "src/taumir/lower.tr"
    long long tagv = _emit_field_get(m, lf, subj, 0LL, 0LL);
#line 3126 "src/taumir/lower.tr"
    long long end_id = LFunc_new_block(lf);
#line 3127 "src/taumir/lower.tr"
    long long i = 0LL;
#line 3128 "src/taumir/lower.tr"
    while ((i < arms->len)) {
#line 3129 "src/taumir/lower.tr"
        HirMatchArm* arm = ((HirMatchArm*)List_ptr_get(arms, i));
#line 3130 "src/taumir/lower.tr"
        bool is_default = false;
#line 3131 "src/taumir/lower.tr"
        TrStr bind_subj = _tr_str_lit_len("", 0LL);
#line 3132 "src/taumir/lower.tr"
        long long cond = (-1LL);
#line 3133 "src/taumir/lower.tr"
        __auto_type _t6295 = arm->pat;
        if (_t6295.tag == Pattern_PWild) {
#line 3135 "src/taumir/lower.tr"
            is_default = true;
        } else if (_t6295.tag == Pattern_PBind) {
            __auto_type nm = _t6295.data.PBind.name;
#line 3137 "src/taumir/lower.tr"
            is_default = true;
#line 3138 "src/taumir/lower.tr"
            TrStr _strtmp_t6296 = _tr_str_retain(nm);
            _tr_str_release(bind_subj);
            bind_subj = _strtmp_t6296;
        } else if (_t6295.tag == Pattern_PVariant) {
            __auto_type vn = _t6295.data.PVariant.variant;
#line 3140 "src/taumir/lower.tr"
            long long vix = ({ TrStr _at_t6297 = (_norm_variant(ename, vn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6297)); _tr_str_release(_at_t6297); _wr; });
#line 3141 "src/taumir/lower.tr"
            if ((vix < 0LL)) {
#line 3141 "src/taumir/lower.tr"
                _tr_str_release(ename);
                return false;
            }
#line 3142 "src/taumir/lower.tr"
            cond = _variant_tag_cond(lf, tagv, vix);
        } else if (_t6295.tag == Pattern_PVariantBind) {
            __auto_type vnb = _t6295.data.PVariantBind.variant;
#line 3144 "src/taumir/lower.tr"
            long long vixb = ({ TrStr _at_t6298 = (_norm_variant(ename, vnb)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6298)); _tr_str_release(_at_t6298); _wr; });
#line 3145 "src/taumir/lower.tr"
            if ((vixb < 0LL)) {
#line 3145 "src/taumir/lower.tr"
                _tr_str_release(ename);
                return false;
            }
#line 3146 "src/taumir/lower.tr"
            cond = _variant_tag_cond(lf, tagv, vixb);
        } else if (_t6295.tag == Pattern_PVariantBindMany) {
            __auto_type vnm = _t6295.data.PVariantBindMany.variant;
#line 3148 "src/taumir/lower.tr"
            long long vixm = ({ TrStr _at_t6299 = (_norm_variant(ename, vnm)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6299)); _tr_str_release(_at_t6299); _wr; });
#line 3149 "src/taumir/lower.tr"
            if ((vixm < 0LL)) {
#line 3149 "src/taumir/lower.tr"
                _tr_str_release(ename);
                return false;
            }
#line 3150 "src/taumir/lower.tr"
            cond = _variant_tag_cond(lf, tagv, vixm);
        } else if (_t6295.tag == Pattern_POr) {
            __auto_type orpats = _t6295.data.POr.patterns;
#line 3152 "src/taumir/lower.tr"
            long long oi = 0LL;
#line 3153 "src/taumir/lower.tr"
            while ((oi < orpats->len)) {
#line 3154 "src/taumir/lower.tr"
                long long oc = (-1LL);
#line 3155 "src/taumir/lower.tr"
                __auto_type _t6300 = List_Pattern_get(orpats, oi);
                if (_t6300.tag == Pattern_PVariant) {
                    __auto_type ovn = _t6300.data.PVariant.variant;
#line 3157 "src/taumir/lower.tr"
                    long long ovix = ({ TrStr _at_t6301 = (_norm_variant(ename, ovn)); __auto_type _wr = (EnumLayout_variant_index(elay, _at_t6301)); _tr_str_release(_at_t6301); _wr; });
#line 3158 "src/taumir/lower.tr"
                    if ((ovix < 0LL)) {
#line 3158 "src/taumir/lower.tr"
                        _tr_str_release(ename);
                        return false;
                    }
#line 3159 "src/taumir/lower.tr"
                    oc = _variant_tag_cond(lf, tagv, ovix);
                } else if (1) {
                    __auto_type _ = _t6300;
#line 3161 "src/taumir/lower.tr"
                    _tr_str_release(ename);
                    return false;
                }
#line 3162 "src/taumir/lower.tr"
                if ((cond < 0LL)) {
#line 3163 "src/taumir/lower.tr"
                    cond = oc;
                } else {
#line 3165 "src/taumir/lower.tr"
                    long long merged = LFunc_new_vreg(lf);
#line 3166 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IBinOp(merged, _tr_str_lit_len("+", 1LL), cond, oc));
#line 3167 "src/taumir/lower.tr"
                    cond = _norm_bool(lf, merged);
                }
#line 3168 "src/taumir/lower.tr"
                oi = (oi + 1LL);
            }
#line 3169 "src/taumir/lower.tr"
            if ((cond < 0LL)) {
#line 3169 "src/taumir/lower.tr"
                _tr_str_release(ename);
                return false;
            }
        } else if (1) {
            __auto_type _ = _t6295;
#line 3171 "src/taumir/lower.tr"
            _tr_str_release(ename);
            return false;
        }
#line 3172 "src/taumir/lower.tr"
        long long body_id = LFunc_new_block(lf);
#line 3173 "src/taumir/lower.tr"
        long long next_id = LFunc_new_block(lf);
#line 3174 "src/taumir/lower.tr"
        if (is_default) {
#line 3175 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TBr(body_id));
        } else {
#line 3177 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, body_id, next_id));
        }
#line 3178 "src/taumir/lower.tr"
        LFunc_set_cur(lf, body_id);
#line 3179 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((bind_subj), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((bind_subj), (_tr_str_lit_len("_", 1LL)))))) {
#line 3180 "src/taumir/lower.tr"
            LFunc_add_var(lf, bind_subj);
#line 3181 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, bind_subj, 11LL);
#line 3182 "src/taumir/lower.tr"
            ({ TrStr _at_t6302 = (_own(ename)); LFunc_set_var_cls(lf, bind_subj, _at_t6302); _tr_str_release(_at_t6302); });
#line 3183 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(bind_subj, subj));
        }
#line 3185 "src/taumir/lower.tr"
        __auto_type _t6303 = arm->pat;
        if (_t6303.tag == Pattern_PVariantBind) {
            __auto_type vnb2 = _t6303.data.PVariantBind.variant;
__auto_type bnm = _t6303.data.PVariantBind.field;
#line 3187 "src/taumir/lower.tr"
            VariantLayout* vlay1 = ({ TrStr _at_t6304 = (_norm_variant(ename, vnb2)); __auto_type _ord_t6305 = (EnumLayout_variant_index(elay, _at_t6304)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, _ord_t6305))); _tr_str_release(_at_t6304); _wr; });
#line 3188 "src/taumir/lower.tr"
            if ((!_tr_str_eqv((bnm), (_tr_str_lit_len("_", 1LL))))) {
#line 3189 "src/taumir/lower.tr"
                if ((!_bind_payload(m, lf, vlay1, subj, subj_hty, 0LL, bnm))) {
#line 3189 "src/taumir/lower.tr"
                    _tr_str_release(ename);
                    return false;
                }
            }
        } else if (_t6303.tag == Pattern_PVariantBindMany) {
            __auto_type vnm2 = _t6303.data.PVariantBindMany.variant;
__auto_type bnames = _t6303.data.PVariantBindMany.fields;
#line 3191 "src/taumir/lower.tr"
            VariantLayout* vlay2 = ({ TrStr _at_t6306 = (_norm_variant(ename, vnm2)); __auto_type _ord_t6307 = (EnumLayout_variant_index(elay, _at_t6306)); __auto_type _wr = (((VariantLayout*)List_ptr_get(elay->variants, _ord_t6307))); _tr_str_release(_at_t6306); _wr; });
#line 3192 "src/taumir/lower.tr"
            long long bi = 0LL;
#line 3193 "src/taumir/lower.tr"
            while ((bi < bnames->len)) {
#line 3194 "src/taumir/lower.tr"
                if (({ TrStr _cmpa_t6308 = (List_TrStr_get(bnames, bi)); TrStr _cmpb_t6309 = (_tr_str_lit_len("_", 1LL)); _Bool _cmpr_t6310 = !_tr_str_eqv(_cmpa_t6308, _cmpb_t6309); _tr_str_release(_cmpa_t6308); _cmpr_t6310; })) {
#line 3195 "src/taumir/lower.tr"
                    if (({ TrStr _at_t6311 = (List_TrStr_get(bnames, bi)); __auto_type _wr = ((!_bind_payload(m, lf, vlay2, subj, subj_hty, bi, _at_t6311))); _tr_str_release(_at_t6311); _wr; })) {
#line 3195 "src/taumir/lower.tr"
                        _tr_str_release(ename);
                        return false;
                    }
                }
#line 3196 "src/taumir/lower.tr"
                bi = (bi + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t6303;
#line 3198 "src/taumir/lower.tr"
            /* pass */
        }
#line 3200 "src/taumir/lower.tr"
        if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
#line 3201 "src/taumir/lower.tr"
            long long egv = lower_expr(m, lf, arm->guard);
#line 3202 "src/taumir/lower.tr"
            if ((egv < 0LL)) {
#line 3202 "src/taumir/lower.tr"
                _tr_str_release(ename);
                _tr_str_release(bind_subj);
                return false;
            }
#line 3203 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 3204 "src/taumir/lower.tr"
            long long egbody = LFunc_new_block(lf);
#line 3205 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TCondBr(egv, egbody, next_id));
#line 3206 "src/taumir/lower.tr"
            LFunc_set_cur(lf, egbody);
        }
#line 3207 "src/taumir/lower.tr"
        if ((!lower_block(m, lf, arm->body))) {
#line 3207 "src/taumir/lower.tr"
            _tr_str_release(ename);
            _tr_str_release(bind_subj);
            return false;
        }
#line 3208 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 3209 "src/taumir/lower.tr"
        LFunc_set_cur(lf, next_id);
#line 3210 "src/taumir/lower.tr"
        i = (i + 1LL);
        _tr_str_release(bind_subj);
    }
#line 3211 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
#line 3212 "src/taumir/lower.tr"
    LFunc_set_cur(lf, end_id);
#line 3213 "src/taumir/lower.tr"
    _tr_str_release(ename);
    return true;
}

__attribute__((hot)) bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
#line 3219 "src/taumir/lower.tr"
    __auto_type _t6312 = (*iter);
    if (_t6312.tag == HirExpr_ECall) {
        __auto_type callee = _t6312.data.ECall.callee;
__auto_type args = _t6312.data.ECall.args;
#line 3221 "src/taumir/lower.tr"
        if (({ TrStr _cmpa_t6313 = (_ident_name(callee)); TrStr _cmpb_t6314 = (_tr_str_lit_len("range", 5LL)); _Bool _cmpr_t6315 = _tr_str_eqv(_cmpa_t6313, _cmpb_t6314); _tr_str_release(_cmpa_t6313); _cmpr_t6315; })) {
#line 3222 "src/taumir/lower.tr"
            return _lower_for_range(m, lf, var, args, body);
        }
    } else if (_t6312.tag == HirExpr_ERange) {
        __auto_type rstart = _t6312.data.ERange.start;
__auto_type rend = _t6312.data.ERange.end;
__auto_type rincl = _t6312.data.ERange.inclusive;
#line 3224 "src/taumir/lower.tr"
        return _lower_for_erange(m, lf, var, rstart, rend, rincl, body);
    } else if (1) {
        __auto_type _ = _t6312;
#line 3226 "src/taumir/lower.tr"
        /* pass */
    }
#line 3229 "src/taumir/lower.tr"
    if (_tr_str_eqv((hir_expr_type(iter)->name), (_tr_str_lit_len("Chan", 4LL)))) {
#line 3230 "src/taumir/lower.tr"
        return _lower_for_chan(m, lf, var, iter, body);
    }
#line 3233 "src/taumir/lower.tr"
    TrStr fic = _recv_class(m, lf, iter);
#line 3234 "src/taumir/lower.tr"
    if ((((!_tr_str_eqv((fic), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, fic)) && ({ TrStr _cmpa_t6316 = (LModule_resolve_method(m, fic, _tr_str_lit_len("__iter__", 8LL))); TrStr _cmpb_t6317 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6318 = !_tr_str_eqv(_cmpa_t6316, _cmpb_t6317); _tr_str_release(_cmpa_t6316); _cmpr_t6318; }))) {
#line 3235 "src/taumir/lower.tr"
        bool _trret1595 = _lower_for_iterproto(m, lf, var, iter, body);
        _tr_str_release(fic);
        return _trret1595;
    }
#line 3236 "src/taumir/lower.tr"
    bool _trret1596 = _lower_for_list(m, lf, var, iter, body);
    _tr_str_release(fic);
    return _trret1596;
}

__attribute__((hot)) bool _sel_recv_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm) {
#line 3243 "src/taumir/lower.tr"
    long long dov = LFunc_new_vreg(lf);
#line 3244 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
#line 3245 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dov, 0LL);
#line 3246 "src/taumir/lower.tr"
    long long armb = LFunc_new_block(lf);
#line 3247 "src/taumir/lower.tr"
    long long skipb = LFunc_new_block(lf);
#line 3248 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
#line 3249 "src/taumir/lower.tr"
    LFunc_set_cur(lf, armb);
#line 3251 "src/taumir/lower.tr"
    HirExpr* chexpr = arm->chan_expr;
#line 3252 "src/taumir/lower.tr"
    __auto_type _t6319 = (*chexpr);
    if (_t6319.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t6319.data.EMethodCall.obj;
        chexpr = mobj;
    } else if (1) {
        __auto_type _ = _t6319;
        /* pass */
    }
#line 3255 "src/taumir/lower.tr"
    long long chv = lower_expr(m, lf, chexpr);
#line 3256 "src/taumir/lower.tr"
    if ((chv < 0LL)) {
#line 3256 "src/taumir/lower.tr"
        return false;
    }
#line 3257 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_recv_val", 21LL));
#line 3258 "src/taumir/lower.tr"
    List_i64* ra = (void*)List_i64_new();
#line 3259 "src/taumir/lower.tr"
    List_i64_append(ra, chv);
#line 3260 "src/taumir/lower.tr"
    long long rv = LFunc_new_vreg(lf);
#line 3261 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(rv, _tr_str_lit_len("_tr_chan_try_recv_val", 21LL), ra));
#line 3262 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, rv, 0LL);
#line 3263 "src/taumir/lower.tr"
    long long minc = LFunc_new_vreg(lf);
#line 3264 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(minc, ((-9223372036854775807LL) - 1LL)));
#line 3265 "src/taumir/lower.tr"
    long long cmp = LFunc_new_vreg(lf);
#line 3266 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cmp, _tr_str_lit_len("!=", 2LL), rv, minc));
#line 3267 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, cmp, 4LL);
#line 3268 "src/taumir/lower.tr"
    long long gotb = LFunc_new_block(lf);
#line 3269 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmp, gotb, skipb));
#line 3270 "src/taumir/lower.tr"
    LFunc_set_cur(lf, gotb);
#line 3271 "src/taumir/lower.tr"
    if (((!_is_null_str(arm->var_name)) && (!_tr_str_eqv((arm->var_name), (_tr_str_lit_len("", 0LL)))))) {
#line 3272 "src/taumir/lower.tr"
        LFunc_add_var(lf, arm->var_name);
#line 3273 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, arm->var_name, 0LL);
#line 3274 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(arm->var_name, rv));
    }
#line 3275 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, arm->body))) {
#line 3275 "src/taumir/lower.tr"
        return false;
    }
#line 3276 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 3277 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 3278 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
#line 3279 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
#line 3280 "src/taumir/lower.tr"
    LFunc_set_cur(lf, skipb);
#line 3281 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _sel_send_arm(LModule* m, LFunc* lf, TrStr donev, HirChanSelectArm* arm) {
#line 3284 "src/taumir/lower.tr"
    long long dov = LFunc_new_vreg(lf);
#line 3285 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
#line 3286 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dov, 0LL);
#line 3287 "src/taumir/lower.tr"
    long long armb = LFunc_new_block(lf);
#line 3288 "src/taumir/lower.tr"
    long long skipb = LFunc_new_block(lf);
#line 3289 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
#line 3290 "src/taumir/lower.tr"
    LFunc_set_cur(lf, armb);
#line 3292 "src/taumir/lower.tr"
    HirExpr* chexpr = arm->chan_expr;
#line 3293 "src/taumir/lower.tr"
    HirExpr* valexpr = arm->val_expr;
#line 3294 "src/taumir/lower.tr"
    __auto_type _t6320 = (*chexpr);
    if (_t6320.tag == HirExpr_EMethodCall) {
        __auto_type mobj = _t6320.data.EMethodCall.obj;
__auto_type sargs = _t6320.data.EMethodCall.args;
#line 3296 "src/taumir/lower.tr"
        chexpr = mobj;
#line 3297 "src/taumir/lower.tr"
        if ((sargs->len == 1LL)) {
#line 3297 "src/taumir/lower.tr"
            valexpr = ((HirExpr*)List_ptr_get(sargs, 0LL));
        }
    } else if (1) {
        __auto_type _ = _t6320;
        /* pass */
    }
#line 3299 "src/taumir/lower.tr"
    long long chv = lower_expr(m, lf, chexpr);
#line 3300 "src/taumir/lower.tr"
    if ((chv < 0LL)) {
#line 3300 "src/taumir/lower.tr"
        return false;
    }
#line 3301 "src/taumir/lower.tr"
    long long sv = lower_expr(m, lf, valexpr);
#line 3302 "src/taumir/lower.tr"
    if (((sv < 0LL) || (LFunc_vreg_type(lf, sv) != 0LL))) {
#line 3302 "src/taumir/lower.tr"
        return false;
    }
#line 3303 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_send", 17LL));
#line 3304 "src/taumir/lower.tr"
    List_i64* sa = (void*)List_i64_new();
#line 3305 "src/taumir/lower.tr"
    List_i64_append(sa, chv);
#line 3306 "src/taumir/lower.tr"
    List_i64_append(sa, sv);
#line 3307 "src/taumir/lower.tr"
    long long okv = LFunc_new_vreg(lf);
#line 3308 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(okv, _tr_str_lit_len("_tr_chan_try_send", 17LL), sa));
#line 3309 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, okv, 4LL);
#line 3310 "src/taumir/lower.tr"
    long long gotb = LFunc_new_block(lf);
#line 3311 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(okv, gotb, skipb));
#line 3312 "src/taumir/lower.tr"
    LFunc_set_cur(lf, gotb);
#line 3313 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, arm->body))) {
#line 3313 "src/taumir/lower.tr"
        return false;
    }
#line 3314 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 3315 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 3316 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
#line 3317 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
#line 3318 "src/taumir/lower.tr"
    LFunc_set_cur(lf, skipb);
#line 3319 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _sel_uncond_arm(LModule* m, LFunc* lf, TrStr donev, HirBlock* body) {
#line 3323 "src/taumir/lower.tr"
    long long dov = LFunc_new_vreg(lf);
#line 3324 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
#line 3325 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dov, 0LL);
#line 3326 "src/taumir/lower.tr"
    long long armb = LFunc_new_block(lf);
#line 3327 "src/taumir/lower.tr"
    long long skipb = LFunc_new_block(lf);
#line 3328 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, armb));
#line 3329 "src/taumir/lower.tr"
    LFunc_set_cur(lf, armb);
#line 3330 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, body))) {
#line 3330 "src/taumir/lower.tr"
        return false;
    }
#line 3331 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 3332 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 3333 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
#line 3334 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
#line 3335 "src/taumir/lower.tr"
    LFunc_set_cur(lf, skipb);
#line 3336 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _sel_timeout_arm(LModule* m, LFunc* lf, TrStr donev, TrStr tstart, HirChanSelectArm* arm) {
#line 3340 "src/taumir/lower.tr"
    long long dov = LFunc_new_vreg(lf);
#line 3341 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(dov, donev));
#line 3342 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dov, 0LL);
#line 3343 "src/taumir/lower.tr"
    long long chkb = LFunc_new_block(lf);
#line 3344 "src/taumir/lower.tr"
    long long skipb = LFunc_new_block(lf);
#line 3345 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dov, skipb, chkb));
#line 3346 "src/taumir/lower.tr"
    LFunc_set_cur(lf, chkb);
#line 3347 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_time_ms", 11LL));
#line 3348 "src/taumir/lower.tr"
    long long nowv = LFunc_new_vreg(lf);
#line 3349 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(nowv, _tr_str_lit_len("_tr_time_ms", 11LL), (void*)List_i64_new()));
#line 3350 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, nowv, 0LL);
#line 3351 "src/taumir/lower.tr"
    long long sv2 = LFunc_new_vreg(lf);
#line 3352 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(sv2, tstart));
#line 3353 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, sv2, 0LL);
#line 3354 "src/taumir/lower.tr"
    long long elapsed = LFunc_new_vreg(lf);
#line 3355 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(elapsed, _tr_str_lit_len("-", 1LL), nowv, sv2));
#line 3356 "src/taumir/lower.tr"
    long long msv = lower_expr(m, lf, arm->timeout_ms);
#line 3357 "src/taumir/lower.tr"
    if (((msv < 0LL) || (LFunc_vreg_type(lf, msv) != 0LL))) {
#line 3357 "src/taumir/lower.tr"
        return false;
    }
#line 3358 "src/taumir/lower.tr"
    long long cmp = LFunc_new_vreg(lf);
#line 3359 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cmp, _tr_str_lit_len(">=", 2LL), elapsed, msv));
#line 3360 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, cmp, 4LL);
#line 3361 "src/taumir/lower.tr"
    long long fireb = LFunc_new_block(lf);
#line 3362 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmp, fireb, skipb));
#line 3363 "src/taumir/lower.tr"
    LFunc_set_cur(lf, fireb);
#line 3364 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, arm->body))) {
#line 3364 "src/taumir/lower.tr"
        return false;
    }
#line 3365 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 3366 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 3367 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, one));
#line 3368 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(skipb));
#line 3369 "src/taumir/lower.tr"
    LFunc_set_cur(lf, skipb);
#line 3370 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _lower_chan_select(LModule* m, LFunc* lf, List_ptr* cases) {
#line 3373 "src/taumir/lower.tr"
    TrStr donev = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__seldone", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3374 "src/taumir/lower.tr"
    LFunc_add_var(lf, donev);
#line 3375 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, donev, 0LL);
#line 3376 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 3377 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3378 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(donev, z));
#line 3379 "src/taumir/lower.tr"
    bool has_timeout = false;
#line 3380 "src/taumir/lower.tr"
    long long ci = 0LL;
#line 3381 "src/taumir/lower.tr"
    while ((ci < cases->len)) {
#line 3382 "src/taumir/lower.tr"
        if (((*((HirChanSelectArm**)List_ptr_get(cases, ci)))->kind == 2LL)) {
#line 3382 "src/taumir/lower.tr"
            has_timeout = true;
        }
#line 3383 "src/taumir/lower.tr"
        ci = (ci + 1LL);
    }
#line 3384 "src/taumir/lower.tr"
    TrStr tstart = _tr_str_lit_len("", 0LL);
#line 3385 "src/taumir/lower.tr"
    if (has_timeout) {
#line 3386 "src/taumir/lower.tr"
        TrStr _strtmp_t6321 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__selstart", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(tstart);
        tstart = _strtmp_t6321;
#line 3387 "src/taumir/lower.tr"
        LFunc_add_var(lf, tstart);
#line 3388 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, tstart, 0LL);
#line 3389 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_time_ms", 11LL));
#line 3390 "src/taumir/lower.tr"
        long long tm = LFunc_new_vreg(lf);
#line 3391 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(tm, _tr_str_lit_len("_tr_time_ms", 11LL), (void*)List_i64_new()));
#line 3392 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, tm, 0LL);
#line 3393 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(tstart, tm));
    }
#line 3394 "src/taumir/lower.tr"
    long long loopb = LFunc_new_block(lf);
#line 3395 "src/taumir/lower.tr"
    long long endb = LFunc_new_block(lf);
#line 3396 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
#line 3397 "src/taumir/lower.tr"
    LFunc_set_cur(lf, loopb);
#line 3398 "src/taumir/lower.tr"
    long long ai = 0LL;
#line 3399 "src/taumir/lower.tr"
    while ((ai < cases->len)) {
#line 3400 "src/taumir/lower.tr"
        HirChanSelectArm* arm = (*((HirChanSelectArm**)List_ptr_get(cases, ai)));
#line 3401 "src/taumir/lower.tr"
        if ((arm->kind == 0LL)) {
#line 3402 "src/taumir/lower.tr"
            if ((!_sel_recv_arm(m, lf, donev, arm))) {
#line 3402 "src/taumir/lower.tr"
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        } else if ((arm->kind == 1LL)) {
#line 3404 "src/taumir/lower.tr"
            if ((!_sel_send_arm(m, lf, donev, arm))) {
#line 3404 "src/taumir/lower.tr"
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        }
#line 3405 "src/taumir/lower.tr"
        ai = (ai + 1LL);
    }
#line 3406 "src/taumir/lower.tr"
    long long aj = 0LL;
#line 3407 "src/taumir/lower.tr"
    while ((aj < cases->len)) {
#line 3408 "src/taumir/lower.tr"
        HirChanSelectArm* arm2 = (*((HirChanSelectArm**)List_ptr_get(cases, aj)));
#line 3409 "src/taumir/lower.tr"
        if ((arm2->kind == 3LL)) {
#line 3410 "src/taumir/lower.tr"
            if ((!_sel_uncond_arm(m, lf, donev, arm2->body))) {
#line 3410 "src/taumir/lower.tr"
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        } else if ((arm2->kind == 2LL)) {
#line 3412 "src/taumir/lower.tr"
            if ((!_sel_timeout_arm(m, lf, donev, tstart, arm2))) {
#line 3412 "src/taumir/lower.tr"
                _tr_str_release(donev);
                _tr_str_release(tstart);
                return false;
            }
        }
#line 3413 "src/taumir/lower.tr"
        aj = (aj + 1LL);
    }
#line 3414 "src/taumir/lower.tr"
    long long dv = LFunc_new_vreg(lf);
#line 3415 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(dv, donev));
#line 3416 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, dv, 0LL);
#line 3417 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(dv, endb, loopb));
#line 3418 "src/taumir/lower.tr"
    LFunc_set_cur(lf, endb);
#line 3419 "src/taumir/lower.tr"
    _tr_str_release(donev);
    _tr_str_release(tstart);
    return true;
}

__attribute__((hot)) bool _lower_for_chan(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
#line 3424 "src/taumir/lower.tr"
    long long chv = lower_expr(m, lf, iter);
#line 3425 "src/taumir/lower.tr"
    if ((chv < 0LL)) {
#line 3425 "src/taumir/lower.tr"
        return false;
    }
#line 3426 "src/taumir/lower.tr"
    TrStr okname = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(LFunc_fresh_id(lf))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__chok", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3427 "src/taumir/lower.tr"
    LFunc_add_var(lf, okname);
#line 3428 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, okname, 0LL);
#line 3429 "src/taumir/lower.tr"
    long long zc = LFunc_new_vreg(lf);
#line 3430 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
#line 3431 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(okname, zc));
#line 3432 "src/taumir/lower.tr"
    LFunc_add_var(lf, var);
#line 3433 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, var, 0LL);
#line 3434 "src/taumir/lower.tr"
    long long loopb = LFunc_new_block(lf);
#line 3435 "src/taumir/lower.tr"
    long long bodyb = LFunc_new_block(lf);
#line 3436 "src/taumir/lower.tr"
    long long endb = LFunc_new_block(lf);
#line 3437 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
#line 3438 "src/taumir/lower.tr"
    LFunc_set_cur(lf, loopb);
#line 3439 "src/taumir/lower.tr"
    long long okaddr = LFunc_new_vreg(lf);
#line 3440 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, okaddr, 12LL);
#line 3441 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IAddrVar(okaddr, okname));
#line 3442 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_chan_recv_ok", 16LL));
#line 3443 "src/taumir/lower.tr"
    List_i64* ra = (void*)List_i64_new();
#line 3444 "src/taumir/lower.tr"
    List_i64_append(ra, chv);
#line 3445 "src/taumir/lower.tr"
    List_i64_append(ra, okaddr);
#line 3446 "src/taumir/lower.tr"
    long long rv = LFunc_new_vreg(lf);
#line 3447 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(rv, _tr_str_lit_len("_tr_chan_recv_ok", 16LL), ra));
#line 3448 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, rv, 0LL);
#line 3449 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, rv));
#line 3450 "src/taumir/lower.tr"
    long long okv = LFunc_new_vreg(lf);
#line 3451 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(okv, okname));
#line 3452 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, okv, 0LL);
#line 3453 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(okv, bodyb, endb));
#line 3454 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bodyb);
#line 3455 "src/taumir/lower.tr"
    if ((!lower_block(m, lf, body))) {
#line 3455 "src/taumir/lower.tr"
        _tr_str_release(okname);
        return false;
    }
#line 3456 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(loopb));
#line 3457 "src/taumir/lower.tr"
    LFunc_set_cur(lf, endb);
#line 3458 "src/taumir/lower.tr"
    _tr_str_release(okname);
    return true;
}

__attribute__((hot)) bool _lower_for_iterproto(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
#line 3463 "src/taumir/lower.tr"
    TrStr icls = _recv_class(m, lf, iter);
#line 3464 "src/taumir/lower.tr"
    if ((_tr_str_eqv((icls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, icls)))) {
#line 3464 "src/taumir/lower.tr"
        _tr_str_release(icls);
        return false;
    }
#line 3465 "src/taumir/lower.tr"
    TrStr iterm = LModule_resolve_method(m, icls, _tr_str_lit_len("__iter__", 8LL));
#line 3466 "src/taumir/lower.tr"
    if (_tr_str_eqv((iterm), (_tr_str_lit_len("", 0LL)))) {
#line 3466 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        return false;
    }
#line 3467 "src/taumir/lower.tr"
    TrStr itcls = _tr_str_retain(_hir_method_ret_ty(m, icls, _tr_str_lit_len("__iter__", 8LL))->name);
#line 3468 "src/taumir/lower.tr"
    if ((((_is_null_str(itcls) || _tr_str_eqv((itcls), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((itcls), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((itcls), (_tr_str_lit_len("None", 4LL))))) {
#line 3468 "src/taumir/lower.tr"
        TrStr _strtmp_t6322 = _tr_str_retain(icls);
        _tr_str_release(itcls);
        itcls = _strtmp_t6322;
    }
#line 3469 "src/taumir/lower.tr"
    if ((!LModule_is_class(m, itcls))) {
#line 3469 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        return false;
    }
#line 3470 "src/taumir/lower.tr"
    TrStr nextm = LModule_resolve_method(m, itcls, _tr_str_lit_len("__next__", 8LL));
#line 3471 "src/taumir/lower.tr"
    if (_tr_str_eqv((nextm), (_tr_str_lit_len("", 0LL)))) {
#line 3471 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3472 "src/taumir/lower.tr"
    AstType* nextret = _hir_method_ret_ty(m, itcls, _tr_str_lit_len("__next__", 8LL));
#line 3473 "src/taumir/lower.tr"
    if ((!_tr_str_eqv((nextret->name), (_tr_str_lit_len("Option", 6LL))))) {
#line 3473 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3474 "src/taumir/lower.tr"
    long long oidx = LModule_enum_index(m, _tr_str_lit_len("Option", 6LL));
#line 3475 "src/taumir/lower.tr"
    if ((oidx < 0LL)) {
#line 3475 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3476 "src/taumir/lower.tr"
    VariantLayout* some_vlay = ((VariantLayout*)List_ptr_get(((EnumLayout*)List_ptr_get(m->enums, oidx))->variants, 0LL));
#line 3477 "src/taumir/lower.tr"
    long long none_idx = EnumLayout_variant_index(((EnumLayout*)List_ptr_get(m->enums, oidx)), _tr_str_lit_len("None", 4LL));
#line 3478 "src/taumir/lower.tr"
    if ((none_idx < 0LL)) {
#line 3478 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3479 "src/taumir/lower.tr"
    long long objv = lower_expr(m, lf, iter);
#line 3480 "src/taumir/lower.tr"
    if ((objv < 0LL)) {
#line 3480 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3481 "src/taumir/lower.tr"
    long long itv = _lower_obj_call(m, lf, iterm, objv, (void*)List_ptr_new());
#line 3482 "src/taumir/lower.tr"
    if ((itv < 0LL)) {
#line 3482 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        return false;
    }
#line 3487 "src/taumir/lower.tr"
    bool fdiscard = _fresh_take_obj(lf, objv);
#line 3488 "src/taumir/lower.tr"
    fdiscard = _fresh_take_obj(lf, itv);
#line 3489 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 3490 "src/taumir/lower.tr"
    TrStr itname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__iter", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3491 "src/taumir/lower.tr"
    TrStr nxname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__nx", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3492 "src/taumir/lower.tr"
    LFunc_add_var(lf, itname);
#line 3493 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, itname, 10LL);
#line 3494 "src/taumir/lower.tr"
    ({ TrStr _at_t6323 = (_own(itcls)); LFunc_set_var_cls(lf, itname, _at_t6323); _tr_str_release(_at_t6323); });
#line 3495 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(itname, itv));
#line 3496 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3497 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3498 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3499 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3500 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3501 "src/taumir/lower.tr"
    long long itload = LFunc_new_vreg(lf);
#line 3502 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(itload, itname));
#line 3503 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, itload, 10LL);
#line 3504 "src/taumir/lower.tr"
    long long nx = _lower_obj_call(m, lf, nextm, itload, (void*)List_ptr_new());
#line 3505 "src/taumir/lower.tr"
    if ((nx < 0LL)) {
#line 3505 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
#line 3506 "src/taumir/lower.tr"
    LFunc_add_var(lf, nxname);
#line 3507 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, nxname, 11LL);
#line 3508 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(nxname, nx));
#line 3509 "src/taumir/lower.tr"
    long long nxl = LFunc_new_vreg(lf);
#line 3510 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(nxl, nxname));
#line 3511 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, nxl, 11LL);
#line 3512 "src/taumir/lower.tr"
    long long tagv = _emit_field_get(m, lf, nxl, 0LL, 0LL);
#line 3513 "src/taumir/lower.tr"
    long long isnone = _variant_tag_cond(lf, tagv, none_idx);
#line 3514 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(isnone, ext, bdy));
#line 3515 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3516 "src/taumir/lower.tr"
    long long nxl2 = LFunc_new_vreg(lf);
#line 3517 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(nxl2, nxname));
#line 3518 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, nxl2, 11LL);
#line 3519 "src/taumir/lower.tr"
    if ((!_bind_payload(m, lf, some_vlay, nxl2, nextret, 0LL, var))) {
#line 3519 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
#line 3520 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, hdr);
#line 3521 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3522 "src/taumir/lower.tr"
    bool bok = lower_block(m, lf, body);
#line 3523 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3524 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3525 "src/taumir/lower.tr"
    if ((!bok)) {
#line 3525 "src/taumir/lower.tr"
        _tr_str_release(icls);
        _tr_str_release(iterm);
        _tr_str_release(itcls);
        _tr_str_release(nextm);
        _tr_str_release(itname);
        _tr_str_release(nxname);
        return false;
    }
#line 3526 "src/taumir/lower.tr"
    _flush_fresh_strs(m, lf);
#line 3527 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3528 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3529 "src/taumir/lower.tr"
    _tr_str_release(icls);
    _tr_str_release(iterm);
    _tr_str_release(itcls);
    _tr_str_release(nextm);
    _tr_str_release(itname);
    _tr_str_release(nxname);
    return true;
}

__attribute__((hot)) bool _lower_for_erange(LModule* m, LFunc* lf, TrStr var, HirExpr* start, HirExpr* end, bool inclusive, HirBlock* body) {
#line 3534 "src/taumir/lower.tr"
    long long sv = lower_expr(m, lf, start);
#line 3535 "src/taumir/lower.tr"
    if (((sv < 0LL) || (LFunc_vreg_type(lf, sv) != 0LL))) {
#line 3535 "src/taumir/lower.tr"
        return false;
    }
#line 3536 "src/taumir/lower.tr"
    TrStr cmp = _tr_str_lit_len("<", 1LL);
#line 3537 "src/taumir/lower.tr"
    if (inclusive) {
#line 3537 "src/taumir/lower.tr"
        TrStr _strtmp_t6324 = _tr_str_lit_len("<=", 2LL);
        _tr_str_release(cmp);
        cmp = _strtmp_t6324;
    }
#line 3538 "src/taumir/lower.tr"
    LFunc_add_var(lf, var);
#line 3539 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, var, 0LL);
#line 3540 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, sv));
#line 3541 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3542 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3543 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3544 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3545 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3546 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3547 "src/taumir/lower.tr"
    long long vv = LFunc_new_vreg(lf);
#line 3548 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(vv, var));
#line 3549 "src/taumir/lower.tr"
    long long ev = lower_expr(m, lf, end);
#line 3550 "src/taumir/lower.tr"
    if (((ev < 0LL) || (LFunc_vreg_type(lf, ev) != 0LL))) {
#line 3550 "src/taumir/lower.tr"
        _tr_str_release(cmp);
        return false;
    }
#line 3551 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3552 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, cmp, vv, ev));
#line 3553 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3554 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3555 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3556 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3557 "src/taumir/lower.tr"
    bool rok = lower_block(m, lf, body);
#line 3558 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3559 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3560 "src/taumir/lower.tr"
    if ((!rok)) {
#line 3560 "src/taumir/lower.tr"
        _tr_str_release(cmp);
        return false;
    }
#line 3561 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3562 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3563 "src/taumir/lower.tr"
    _emit_add_const(lf, var, 1LL);
#line 3564 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3565 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3566 "src/taumir/lower.tr"
    _tr_str_release(cmp);
    return true;
}

__attribute__((hot)) bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body) {
#line 3569 "src/taumir/lower.tr"
    if (((args->len < 1LL) || (args->len > 3LL))) {
#line 3569 "src/taumir/lower.tr"
        return false;
    }
#line 3570 "src/taumir/lower.tr"
    long long stepv = 1LL;
#line 3571 "src/taumir/lower.tr"
    TrStr cmp = _tr_str_lit_len("<", 1LL);
#line 3572 "src/taumir/lower.tr"
    if ((args->len == 3LL)) {
#line 3573 "src/taumir/lower.tr"
        if ((!_is_const_int(((HirExpr*)List_ptr_get(args, 2LL))))) {
#line 3573 "src/taumir/lower.tr"
            _tr_str_release(cmp);
            return false;
        }
#line 3574 "src/taumir/lower.tr"
        stepv = _const_int_val(((HirExpr*)List_ptr_get(args, 2LL)));
#line 3575 "src/taumir/lower.tr"
        if ((stepv == 0LL)) {
#line 3575 "src/taumir/lower.tr"
            _tr_str_release(cmp);
            return false;
        }
#line 3576 "src/taumir/lower.tr"
        if ((stepv < 0LL)) {
#line 3576 "src/taumir/lower.tr"
            TrStr _strtmp_t6325 = _tr_str_lit_len(">", 1LL);
            _tr_str_release(cmp);
            cmp = _strtmp_t6325;
        }
    }
#line 3577 "src/taumir/lower.tr"
    long long sv = (-1LL);
#line 3578 "src/taumir/lower.tr"
    HirExpr* end_expr = ((HirExpr*)List_ptr_get(args, 0LL));
#line 3579 "src/taumir/lower.tr"
    if ((args->len == 1LL)) {
#line 3580 "src/taumir/lower.tr"
        long long z = LFunc_new_vreg(lf);
#line 3581 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3582 "src/taumir/lower.tr"
        sv = z;
    } else {
#line 3584 "src/taumir/lower.tr"
        sv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 3585 "src/taumir/lower.tr"
        if ((sv < 0LL)) {
#line 3585 "src/taumir/lower.tr"
            _tr_str_release(cmp);
            return false;
        }
#line 3586 "src/taumir/lower.tr"
        end_expr = ((HirExpr*)List_ptr_get(args, 1LL));
    }
#line 3587 "src/taumir/lower.tr"
    LFunc_add_var(lf, var);
#line 3588 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, var, 0LL);
#line 3589 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, sv));
#line 3590 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3591 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3592 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3593 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3594 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3595 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3596 "src/taumir/lower.tr"
    long long vv = LFunc_new_vreg(lf);
#line 3597 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(vv, var));
#line 3598 "src/taumir/lower.tr"
    long long ev = lower_expr(m, lf, end_expr);
#line 3599 "src/taumir/lower.tr"
    if ((ev < 0LL)) {
#line 3599 "src/taumir/lower.tr"
        _tr_str_release(cmp);
        return false;
    }
#line 3600 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3601 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, cmp, vv, ev));
#line 3602 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3603 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3604 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3605 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3606 "src/taumir/lower.tr"
    bool rok = lower_block(m, lf, body);
#line 3607 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3608 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3609 "src/taumir/lower.tr"
    if ((!rok)) {
#line 3609 "src/taumir/lower.tr"
        _tr_str_release(cmp);
        return false;
    }
#line 3610 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3611 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3612 "src/taumir/lower.tr"
    _emit_add_const(lf, var, stepv);
#line 3613 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3614 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3615 "src/taumir/lower.tr"
    _tr_str_release(cmp);
    return true;
}

__attribute__((hot)) bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
#line 3618 "src/taumir/lower.tr"
    long long lv = lower_expr(m, lf, iter);
#line 3619 "src/taumir/lower.tr"
    if ((lv < 0LL)) {
#line 3619 "src/taumir/lower.tr"
        return false;
    }
#line 3620 "src/taumir/lower.tr"
    long long ltag = LFunc_vreg_type(lf, lv);
#line 3621 "src/taumir/lower.tr"
    if ((!_is_list_tag(ltag))) {
#line 3621 "src/taumir/lower.tr"
        return false;
    }
#line 3622 "src/taumir/lower.tr"
    long long elem_t = _list_elem_tag(ltag);
#line 3623 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 3624 "src/taumir/lower.tr"
    TrStr hname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__forlist", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3625 "src/taumir/lower.tr"
    TrStr iname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__foridx", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3626 "src/taumir/lower.tr"
    LFunc_add_var(lf, hname);
#line 3627 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, hname, ltag);
#line 3628 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(hname, lv));
#line 3629 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 3630 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3631 "src/taumir/lower.tr"
    LFunc_add_var(lf, iname);
#line 3632 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, iname, 0LL);
#line 3633 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(iname, z));
#line 3634 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3635 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3636 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3637 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3638 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3639 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3640 "src/taumir/lower.tr"
    long long hv = LFunc_new_vreg(lf);
#line 3641 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv, hname));
#line 3642 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 3643 "src/taumir/lower.tr"
    List_i64* la = (void*)List_i64_new();
#line 3644 "src/taumir/lower.tr"
    List_i64_append(la, hv);
#line 3645 "src/taumir/lower.tr"
    long long lenv = LFunc_new_vreg(lf);
#line 3646 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
#line 3647 "src/taumir/lower.tr"
    long long iv = LFunc_new_vreg(lf);
#line 3648 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
#line 3649 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3650 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
#line 3651 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3652 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3653 "src/taumir/lower.tr"
    long long hv2 = LFunc_new_vreg(lf);
#line 3654 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv2, hname));
#line 3655 "src/taumir/lower.tr"
    long long iv2 = LFunc_new_vreg(lf);
#line 3656 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
#line 3657 "src/taumir/lower.tr"
    long long xval = _list_get_raw(m, lf, ltag, hv2, iv2);
#line 3658 "src/taumir/lower.tr"
    if ((elem_t == 5LL)) {
#line 3661 "src/taumir/lower.tr"
        long long xfb = LFunc_new_vreg(lf);
#line 3662 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(xfb, xval));
#line 3663 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, xfb, 5LL);
#line 3664 "src/taumir/lower.tr"
        xval = xfb;
    } else {
#line 3666 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, xval, elem_t);
    }
#line 3667 "src/taumir/lower.tr"
    if ((elem_t == 1LL)) {
#line 3667 "src/taumir/lower.tr"
        _retain_str(m, lf, xval);
    }
#line 3668 "src/taumir/lower.tr"
    LFunc_add_var(lf, var);
#line 3669 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, var, elem_t);
#line 3670 "src/taumir/lower.tr"
    if ((elem_t == 10LL)) {
#line 3671 "src/taumir/lower.tr"
        AstType* it_ty = hir_expr_type(iter);
#line 3672 "src/taumir/lower.tr"
        if (((it_ty->args->len > 0LL) && (!_is_null_str((*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)))) {
#line 3673 "src/taumir/lower.tr"
            if (LModule_is_class(m, (*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)) {
#line 3674 "src/taumir/lower.tr"
                ({ TrStr _at_t6326 = (_own((*((AstType**)List_ptr_get(it_ty->args, 0LL)))->name)); LFunc_set_var_cls(lf, var, _at_t6326); _tr_str_release(_at_t6326); });
            }
        }
    }
#line 3675 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, xval));
#line 3676 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3677 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3678 "src/taumir/lower.tr"
    bool fok = lower_block(m, lf, body);
#line 3679 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3680 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3681 "src/taumir/lower.tr"
    if ((!fok)) {
#line 3681 "src/taumir/lower.tr"
        _tr_str_release(hname);
        _tr_str_release(iname);
        return false;
    }
#line 3682 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3683 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3684 "src/taumir/lower.tr"
    _emit_incr(lf, iname);
#line 3685 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3686 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3687 "src/taumir/lower.tr"
    _tr_str_release(hname);
    _tr_str_release(iname);
    return true;
}

__attribute__((hot)) bool _lower_for_unpack(LModule* m, LFunc* lf, List_TrStr* vars, HirExpr* iter, HirBlock* body) {
#line 3691 "src/taumir/lower.tr"
    if ((vars->len != 2LL)) {
#line 3691 "src/taumir/lower.tr"
        return false;
    }
#line 3692 "src/taumir/lower.tr"
    __auto_type _t6327 = (*iter);
    if (_t6327.tag == HirExpr_ECall) {
        __auto_type callee = _t6327.data.ECall.callee;
__auto_type args = _t6327.data.ECall.args;
#line 3694 "src/taumir/lower.tr"
        if ((({ TrStr _cmpa_t6328 = (_ident_name(callee)); TrStr _cmpb_t6329 = (_tr_str_lit_len("enumerate", 9LL)); _Bool _cmpr_t6330 = _tr_str_eqv(_cmpa_t6328, _cmpb_t6329); _tr_str_release(_cmpa_t6328); _cmpr_t6330; }) && (args->len == 1LL))) {
#line 3695 "src/taumir/lower.tr"
            return ({ TrStr _at_t6331 = (List_TrStr_get(vars, 0LL)); TrStr _at_t6332 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_enumerate(m, lf, _at_t6331, _at_t6332, ((HirExpr*)List_ptr_get(args, 0LL)), body)); _tr_str_release(_at_t6331); _tr_str_release(_at_t6332); _wr; });
        }
#line 3696 "src/taumir/lower.tr"
        if ((({ TrStr _cmpa_t6333 = (_ident_name(callee)); TrStr _cmpb_t6334 = (_tr_str_lit_len("zip", 3LL)); _Bool _cmpr_t6335 = _tr_str_eqv(_cmpa_t6333, _cmpb_t6334); _tr_str_release(_cmpa_t6333); _cmpr_t6335; }) && (args->len == 2LL))) {
#line 3697 "src/taumir/lower.tr"
            return ({ TrStr _at_t6336 = (List_TrStr_get(vars, 0LL)); TrStr _at_t6337 = (List_TrStr_get(vars, 1LL)); __auto_type _ord_t6338 = (((HirExpr*)List_ptr_get(args, 0LL))); __auto_type _wr = (_lower_zip(m, lf, _at_t6336, _at_t6337, _ord_t6338, ((HirExpr*)List_ptr_get(args, 1LL)), body)); _tr_str_release(_at_t6336); _tr_str_release(_at_t6337); _wr; });
        }
    } else if (_t6327.tag == HirExpr_EMethodCall) {
        __auto_type dobj = _t6327.data.EMethodCall.obj;
__auto_type dmeth = _t6327.data.EMethodCall.method;
#line 3699 "src/taumir/lower.tr"
        if (_tr_str_eqv((dmeth), (_tr_str_lit_len("items", 5LL)))) {
#line 3700 "src/taumir/lower.tr"
            bool _trret1597 = ({ TrStr _at_t6339 = (List_TrStr_get(vars, 0LL)); TrStr _at_t6340 = (List_TrStr_get(vars, 1LL)); __auto_type _wr = (_lower_dict_items_unpack(m, lf, _at_t6339, _at_t6340, dobj, body)); _tr_str_release(_at_t6339); _tr_str_release(_at_t6340); _wr; });
            return _trret1597;
        }
    } else if (1) {
        __auto_type _ = _t6327;
#line 3702 "src/taumir/lower.tr"
        /* pass */
    }
#line 3703 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) bool _lower_enumerate(LModule* m, LFunc* lf, TrStr ivar, TrStr evar, HirExpr* listexpr, HirBlock* body) {
#line 3707 "src/taumir/lower.tr"
    long long lv = lower_expr(m, lf, listexpr);
#line 3708 "src/taumir/lower.tr"
    if ((lv < 0LL)) {
#line 3708 "src/taumir/lower.tr"
        return false;
    }
#line 3709 "src/taumir/lower.tr"
    long long ltag = LFunc_vreg_type(lf, lv);
#line 3710 "src/taumir/lower.tr"
    if ((!_is_list_tag(ltag))) {
#line 3710 "src/taumir/lower.tr"
        return false;
    }
#line 3711 "src/taumir/lower.tr"
    long long elem_t = _list_elem_tag(ltag);
#line 3712 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 3713 "src/taumir/lower.tr"
    TrStr hname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__enumlist", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3714 "src/taumir/lower.tr"
    LFunc_add_var(lf, hname);
#line 3715 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, hname, ltag);
#line 3716 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(hname, lv));
#line 3717 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 3718 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3719 "src/taumir/lower.tr"
    LFunc_add_var(lf, ivar);
#line 3720 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, ivar, 0LL);
#line 3721 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(ivar, z));
#line 3722 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3723 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3724 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3725 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3726 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3727 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3728 "src/taumir/lower.tr"
    long long hv = LFunc_new_vreg(lf);
#line 3729 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv, hname));
#line 3730 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 3731 "src/taumir/lower.tr"
    List_i64* la = (void*)List_i64_new();
#line 3732 "src/taumir/lower.tr"
    List_i64_append(la, hv);
#line 3733 "src/taumir/lower.tr"
    long long lenv = LFunc_new_vreg(lf);
#line 3734 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
#line 3735 "src/taumir/lower.tr"
    long long iv = LFunc_new_vreg(lf);
#line 3736 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, ivar));
#line 3737 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3738 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
#line 3739 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3740 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3741 "src/taumir/lower.tr"
    long long hv2 = LFunc_new_vreg(lf);
#line 3742 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv2, hname));
#line 3743 "src/taumir/lower.tr"
    long long iv2 = LFunc_new_vreg(lf);
#line 3744 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, ivar));
#line 3745 "src/taumir/lower.tr"
    long long xval = _list_get_raw(m, lf, ltag, hv2, iv2);
#line 3746 "src/taumir/lower.tr"
    if ((elem_t == 5LL)) {
#line 3747 "src/taumir/lower.tr"
        long long xfb2 = LFunc_new_vreg(lf);
#line 3748 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(xfb2, xval));
#line 3749 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, xfb2, 5LL);
#line 3750 "src/taumir/lower.tr"
        xval = xfb2;
    } else {
#line 3752 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, xval, elem_t);
    }
#line 3753 "src/taumir/lower.tr"
    if ((elem_t == 1LL)) {
#line 3753 "src/taumir/lower.tr"
        _retain_str(m, lf, xval);
    }
#line 3754 "src/taumir/lower.tr"
    LFunc_add_var(lf, evar);
#line 3755 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, evar, elem_t);
#line 3756 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(evar, xval));
#line 3757 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3758 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3759 "src/taumir/lower.tr"
    bool ok = lower_block(m, lf, body);
#line 3760 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3761 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3762 "src/taumir/lower.tr"
    if ((!ok)) {
#line 3762 "src/taumir/lower.tr"
        _tr_str_release(hname);
        return false;
    }
#line 3763 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3764 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3765 "src/taumir/lower.tr"
    _emit_incr(lf, ivar);
#line 3766 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3767 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3768 "src/taumir/lower.tr"
    _tr_str_release(hname);
    return true;
}

__attribute__((hot)) bool _lower_zip(LModule* m, LFunc* lf, TrStr v0, TrStr v1, HirExpr* aexpr, HirExpr* bexpr, HirBlock* body) {
#line 3772 "src/taumir/lower.tr"
    long long av = lower_expr(m, lf, aexpr);
#line 3773 "src/taumir/lower.tr"
    if ((av < 0LL)) {
#line 3773 "src/taumir/lower.tr"
        return false;
    }
#line 3774 "src/taumir/lower.tr"
    long long atag = LFunc_vreg_type(lf, av);
#line 3775 "src/taumir/lower.tr"
    if ((!_is_list_tag(atag))) {
#line 3775 "src/taumir/lower.tr"
        return false;
    }
#line 3776 "src/taumir/lower.tr"
    long long bv = lower_expr(m, lf, bexpr);
#line 3777 "src/taumir/lower.tr"
    if ((bv < 0LL)) {
#line 3777 "src/taumir/lower.tr"
        return false;
    }
#line 3778 "src/taumir/lower.tr"
    long long btag = LFunc_vreg_type(lf, bv);
#line 3779 "src/taumir/lower.tr"
    if ((!_is_list_tag(btag))) {
#line 3779 "src/taumir/lower.tr"
        return false;
    }
#line 3780 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 3781 "src/taumir/lower.tr"
    TrStr aname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__zipa", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3782 "src/taumir/lower.tr"
    TrStr bname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__zipb", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3783 "src/taumir/lower.tr"
    LFunc_add_var(lf, aname);
#line 3783 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, aname, atag);
#line 3783 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(aname, av));
#line 3784 "src/taumir/lower.tr"
    LFunc_add_var(lf, bname);
#line 3784 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, bname, btag);
#line 3784 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(bname, bv));
#line 3785 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 3785 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3786 "src/taumir/lower.tr"
    ({ TrStr _at_t6341 = (({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__zipi", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); LFunc_add_var(lf, _at_t6341); _tr_str_release(_at_t6341); });
#line 3786 "src/taumir/lower.tr"
    TrStr iname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__zipi", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3787 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, iname, 0LL);
#line 3787 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(iname, z));
#line 3788 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3788 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3788 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3788 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3789 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3790 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3791 "src/taumir/lower.tr"
    long long ahv = LFunc_new_vreg(lf);
#line 3791 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(ahv, aname));
#line 3792 "src/taumir/lower.tr"
    long long bhv = LFunc_new_vreg(lf);
#line 3792 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(bhv, bname));
#line 3793 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 3794 "src/taumir/lower.tr"
    List_i64* ala = (void*)List_i64_new();
#line 3794 "src/taumir/lower.tr"
    List_i64_append(ala, ahv);
#line 3795 "src/taumir/lower.tr"
    long long alenv = LFunc_new_vreg(lf);
#line 3795 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(alenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), ala));
#line 3796 "src/taumir/lower.tr"
    List_i64* bla = (void*)List_i64_new();
#line 3796 "src/taumir/lower.tr"
    List_i64_append(bla, bhv);
#line 3797 "src/taumir/lower.tr"
    long long blenv = LFunc_new_vreg(lf);
#line 3797 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(blenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), bla));
#line 3798 "src/taumir/lower.tr"
    long long iv = LFunc_new_vreg(lf);
#line 3798 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
#line 3799 "src/taumir/lower.tr"
    long long c1 = LFunc_new_vreg(lf);
#line 3799 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(c1, _tr_str_lit_len("<", 1LL), iv, alenv));
#line 3800 "src/taumir/lower.tr"
    long long c2 = LFunc_new_vreg(lf);
#line 3800 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(c2, _tr_str_lit_len("<", 1LL), iv, blenv));
#line 3801 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3801 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("&", 1LL), c1, c2));
#line 3802 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3803 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3804 "src/taumir/lower.tr"
    long long ahv2 = LFunc_new_vreg(lf);
#line 3804 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(ahv2, aname));
#line 3805 "src/taumir/lower.tr"
    long long bhv2 = LFunc_new_vreg(lf);
#line 3805 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(bhv2, bname));
#line 3806 "src/taumir/lower.tr"
    long long iv2 = LFunc_new_vreg(lf);
#line 3806 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
#line 3807 "src/taumir/lower.tr"
    long long aelem = _list_get_elem(m, lf, atag, ahv2, iv2);
#line 3808 "src/taumir/lower.tr"
    long long belem = _list_get_elem(m, lf, btag, bhv2, iv2);
#line 3809 "src/taumir/lower.tr"
    long long aet = _list_elem_tag(atag);
#line 3809 "src/taumir/lower.tr"
    long long bet = _list_elem_tag(btag);
#line 3810 "src/taumir/lower.tr"
    if ((aet == 1LL)) {
#line 3810 "src/taumir/lower.tr"
        _retain_str(m, lf, aelem);
    }
#line 3811 "src/taumir/lower.tr"
    if ((bet == 1LL)) {
#line 3811 "src/taumir/lower.tr"
        _retain_str(m, lf, belem);
    }
#line 3812 "src/taumir/lower.tr"
    LFunc_add_var(lf, v0);
#line 3812 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, v0, aet);
#line 3812 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(v0, aelem));
#line 3813 "src/taumir/lower.tr"
    LFunc_add_var(lf, v1);
#line 3813 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, v1, bet);
#line 3813 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(v1, belem));
#line 3814 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3814 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3815 "src/taumir/lower.tr"
    bool ok = lower_block(m, lf, body);
#line 3816 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3816 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3817 "src/taumir/lower.tr"
    if ((!ok)) {
#line 3817 "src/taumir/lower.tr"
        _tr_str_release(aname);
        _tr_str_release(bname);
        _tr_str_release(iname);
        return false;
    }
#line 3818 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3819 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3820 "src/taumir/lower.tr"
    _emit_incr(lf, iname);
#line 3821 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3822 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3823 "src/taumir/lower.tr"
    _tr_str_release(aname);
    _tr_str_release(bname);
    _tr_str_release(iname);
    return true;
}

__attribute__((hot)) bool _lower_dict_items_unpack(LModule* m, LFunc* lf, TrStr kvar, TrStr vvar, HirExpr* dictexpr, HirBlock* body) {
#line 3827 "src/taumir/lower.tr"
    long long dv = lower_expr(m, lf, dictexpr);
#line 3828 "src/taumir/lower.tr"
    if ((dv < 0LL)) {
#line 3828 "src/taumir/lower.tr"
        return false;
    }
#line 3829 "src/taumir/lower.tr"
    long long dtag = LFunc_vreg_type(lf, dv);
#line 3830 "src/taumir/lower.tr"
    if ((!_is_dict_tag(dtag))) {
#line 3830 "src/taumir/lower.tr"
        return false;
    }
#line 3831 "src/taumir/lower.tr"
    bool kstr = _dict_key_is_str(dtag);
#line 3832 "src/taumir/lower.tr"
    long long vtag = _dict_val_tag(dtag);
#line 3833 "src/taumir/lower.tr"
    long long uid = LFunc_fresh_id(lf);
#line 3834 "src/taumir/lower.tr"
    TrStr dname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__ditd", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3835 "src/taumir/lower.tr"
    LFunc_add_var(lf, dname);
#line 3835 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, dname, dtag);
#line 3835 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(dname, dv));
#line 3836 "src/taumir/lower.tr"
    TrStr isym = _tr_str_lit_len("_tr_rt_idict_items", 18LL);
#line 3837 "src/taumir/lower.tr"
    if (kstr) {
#line 3837 "src/taumir/lower.tr"
        TrStr _strtmp_t6342 = _tr_str_lit_len("_tr_rt_dict_items", 17LL);
        _tr_str_release(isym);
        isym = _strtmp_t6342;
    }
#line 3838 "src/taumir/lower.tr"
    LModule_add_extern(m, isym);
#line 3839 "src/taumir/lower.tr"
    List_i64* ia = (void*)List_i64_new();
#line 3839 "src/taumir/lower.tr"
    List_i64_append(ia, dv);
#line 3840 "src/taumir/lower.tr"
    long long items = LFunc_new_vreg(lf);
#line 3840 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(items, isym, ia));
#line 3841 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, items, 3LL);
#line 3842 "src/taumir/lower.tr"
    TrStr iname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__diti", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3843 "src/taumir/lower.tr"
    TrStr itemsname = ({ TrStr _tr_cr = (_lir_itoa(uid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__ditl", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 3844 "src/taumir/lower.tr"
    LFunc_add_var(lf, itemsname);
#line 3844 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, itemsname, 3LL);
#line 3844 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(itemsname, items));
#line 3845 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 3845 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 3846 "src/taumir/lower.tr"
    LFunc_add_var(lf, iname);
#line 3846 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, iname, 0LL);
#line 3846 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(iname, z));
#line 3847 "src/taumir/lower.tr"
    long long hdr = LFunc_new_block(lf);
#line 3847 "src/taumir/lower.tr"
    long long bdy = LFunc_new_block(lf);
#line 3847 "src/taumir/lower.tr"
    long long latch = LFunc_new_block(lf);
#line 3847 "src/taumir/lower.tr"
    long long ext = LFunc_new_block(lf);
#line 3848 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3849 "src/taumir/lower.tr"
    LFunc_set_cur(lf, hdr);
#line 3850 "src/taumir/lower.tr"
    long long lhv = LFunc_new_vreg(lf);
#line 3850 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(lhv, itemsname));
#line 3851 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 3852 "src/taumir/lower.tr"
    List_i64* lla = (void*)List_i64_new();
#line 3852 "src/taumir/lower.tr"
    List_i64_append(lla, lhv);
#line 3853 "src/taumir/lower.tr"
    long long lenv = LFunc_new_vreg(lf);
#line 3853 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit_len("_tr_rt_list_len", 15LL), lla));
#line 3854 "src/taumir/lower.tr"
    long long iv = LFunc_new_vreg(lf);
#line 3854 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
#line 3855 "src/taumir/lower.tr"
    long long cond = LFunc_new_vreg(lf);
#line 3855 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit_len("<", 1LL), iv, lenv));
#line 3856 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
#line 3857 "src/taumir/lower.tr"
    LFunc_set_cur(lf, bdy);
#line 3858 "src/taumir/lower.tr"
    long long lhv2 = LFunc_new_vreg(lf);
#line 3858 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(lhv2, itemsname));
#line 3859 "src/taumir/lower.tr"
    long long iv2 = LFunc_new_vreg(lf);
#line 3859 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
#line 3860 "src/taumir/lower.tr"
    long long raw = _list_get(m, lf, lhv2, iv2);
#line 3861 "src/taumir/lower.tr"
    long long koff = 0LL;
#line 3861 "src/taumir/lower.tr"
    long long voff = 8LL;
#line 3862 "src/taumir/lower.tr"
    long long keyv = _emit_field_get(m, lf, raw, koff, 0LL);
#line 3863 "src/taumir/lower.tr"
    if (kstr) {
#line 3864 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, keyv, 1LL);
    }
#line 3865 "src/taumir/lower.tr"
    long long valv = _emit_field_get(m, lf, raw, voff, 0LL);
#line 3866 "src/taumir/lower.tr"
    if ((vtag == 5LL)) {
#line 3867 "src/taumir/lower.tr"
        long long vf = LFunc_new_vreg(lf);
#line 3867 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(vf, valv));
#line 3867 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, vf, 5LL);
#line 3867 "src/taumir/lower.tr"
        valv = vf;
    } else {
#line 3869 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, valv, vtag);
    }
#line 3870 "src/taumir/lower.tr"
    long long ktag = 0LL;
#line 3871 "src/taumir/lower.tr"
    if (kstr) {
#line 3871 "src/taumir/lower.tr"
        ktag = 1LL;
    }
#line 3872 "src/taumir/lower.tr"
    LFunc_add_var(lf, kvar);
#line 3872 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, kvar, ktag);
#line 3872 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(kvar, keyv));
#line 3873 "src/taumir/lower.tr"
    LFunc_add_var(lf, vvar);
#line 3873 "src/taumir/lower.tr"
    LFunc_set_var_type(lf, vvar, vtag);
#line 3873 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(vvar, valv));
#line 3874 "src/taumir/lower.tr"
    List_i64_append(lf->loop_cont, latch);
#line 3874 "src/taumir/lower.tr"
    List_i64_append(lf->loop_brk, ext);
#line 3875 "src/taumir/lower.tr"
    bool ok = lower_block(m, lf, body);
#line 3876 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_cont);
#line 3876 "src/taumir/lower.tr"
    List_i64_pop(lf->loop_brk);
#line 3877 "src/taumir/lower.tr"
    if ((!ok)) {
#line 3877 "src/taumir/lower.tr"
        _tr_str_release(dname);
        _tr_str_release(isym);
        _tr_str_release(iname);
        _tr_str_release(itemsname);
        return false;
    }
#line 3878 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
#line 3879 "src/taumir/lower.tr"
    LFunc_set_cur(lf, latch);
#line 3880 "src/taumir/lower.tr"
    _emit_incr(lf, iname);
#line 3881 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
#line 3882 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ext);
#line 3883 "src/taumir/lower.tr"
    _tr_str_release(dname);
    _tr_str_release(isym);
    _tr_str_release(iname);
    _tr_str_release(itemsname);
    return true;
}

__attribute__((hot)) void _emit_incr(LFunc* lf, TrStr name) {
#line 3887 "src/taumir/lower.tr"
    long long cur = LFunc_new_vreg(lf);
#line 3888 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
#line 3889 "src/taumir/lower.tr"
    long long one = LFunc_new_vreg(lf);
#line 3890 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
#line 3891 "src/taumir/lower.tr"
    long long inc = LFunc_new_vreg(lf);
#line 3892 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit_len("+", 1LL), cur, one));
#line 3893 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
#line 3896 "src/taumir/lower.tr"
    __auto_type _t6343 = (*e);
    if (_t6343.tag == HirExpr_EIdent) {
        __auto_type n = _t6343.data.EIdent.name;
        TrStr _trret1598 = _tr_str_retain(n);
        return _trret1598;
    } else if (1) {
        __auto_type _ = _t6343;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool _lower_field_set(LModule* m, LFunc* lf, HirExpr* obj, TrStr prop, HirExpr* val) {
#line 3904 "src/taumir/lower.tr"
    TrStr cls = _recv_class(m, lf, obj);
#line 3905 "src/taumir/lower.tr"
    if ((_tr_str_eqv((cls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, cls)))) {
#line 3905 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3906 "src/taumir/lower.tr"
    long long off = LModule_field_offset(m, cls, prop);
#line 3907 "src/taumir/lower.tr"
    if ((off < 0LL)) {
#line 3907 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3908 "src/taumir/lower.tr"
    long long ftg = LModule_field_tag(m, cls, prop);
#line 3909 "src/taumir/lower.tr"
    if ((ftg < 0LL)) {
#line 3909 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3910 "src/taumir/lower.tr"
    long long vv = lower_expr(m, lf, val);
#line 3911 "src/taumir/lower.tr"
    if ((vv < 0LL)) {
#line 3911 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3912 "src/taumir/lower.tr"
    long long vt = LFunc_vreg_type(lf, vv);
#line 3913 "src/taumir/lower.tr"
    if ((_is_list_tag(ftg) || _is_dict_tag(ftg))) {
#line 3916 "src/taumir/lower.tr"
        if ((_is_list_tag(ftg) && (!_is_list_tag(vt)))) {
#line 3916 "src/taumir/lower.tr"
            _tr_str_release(cls);
            return false;
        }
#line 3917 "src/taumir/lower.tr"
        if ((_is_dict_tag(ftg) && (!_is_dict_tag(vt)))) {
#line 3917 "src/taumir/lower.tr"
            _tr_str_release(cls);
            return false;
        }
#line 3918 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, vv, ftg);
    } else if ((ftg == 5LL)) {
#line 3921 "src/taumir/lower.tr"
        if ((vt == 0LL)) {
#line 3921 "src/taumir/lower.tr"
            vv = _promote_f(lf, vv);
        } else if ((vt != 5LL)) {
#line 3922 "src/taumir/lower.tr"
            _tr_str_release(cls);
            return false;
        }
#line 3923 "src/taumir/lower.tr"
        long long fbits = LFunc_new_vreg(lf);
#line 3924 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IFBits(fbits, vv));
#line 3925 "src/taumir/lower.tr"
        vv = fbits;
    } else if ((ftg == 1LL)) {
#line 3929 "src/taumir/lower.tr"
        if ((vt != 1LL)) {
#line 3929 "src/taumir/lower.tr"
            _tr_str_release(cls);
            return false;
        }
#line 3930 "src/taumir/lower.tr"
        _secure_str(m, lf, vv);
    } else if (((ftg == 10LL) || (ftg == 11LL))) {
#line 3935 "src/taumir/lower.tr"
        if ((!_field_tag_ok(vt, ftg))) {
#line 3935 "src/taumir/lower.tr"
            _tr_str_release(cls);
            return false;
        }
#line 3936 "src/taumir/lower.tr"
        _secure_obj(m, lf, vv);
    } else if ((!_field_tag_ok(vt, ftg))) {
#line 3938 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3939 "src/taumir/lower.tr"
    long long ov0 = lower_expr(m, lf, obj);
#line 3940 "src/taumir/lower.tr"
    if ((ov0 < 0LL)) {
#line 3940 "src/taumir/lower.tr"
        _tr_str_release(cls);
        return false;
    }
#line 3941 "src/taumir/lower.tr"
    _emit_field_set(m, lf, ov0, off, vv);
#line 3947 "src/taumir/lower.tr"
    _flush_fresh_strs(m, lf);
#line 3948 "src/taumir/lower.tr"
    _tr_str_release(cls);
    return true;
}

__attribute__((hot)) bool _lower_index_set(LModule* m, LFunc* lf, HirExpr* obj, HirExpr* idx, HirExpr* val) {
#line 3952 "src/taumir/lower.tr"
    TrStr sicls = _recv_class(m, lf, obj);
#line 3953 "src/taumir/lower.tr"
    if (((!_tr_str_eqv((sicls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, sicls))) {
#line 3954 "src/taumir/lower.tr"
        TrStr sim = LModule_resolve_method(m, sicls, _tr_str_lit_len("__setitem__", 11LL));
#line 3955 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((sim), (_tr_str_lit_len("", 0LL))))) {
#line 3956 "src/taumir/lower.tr"
            long long siself = lower_expr(m, lf, obj);
#line 3957 "src/taumir/lower.tr"
            if ((siself < 0LL)) {
#line 3957 "src/taumir/lower.tr"
                _tr_str_release(sicls);
                _tr_str_release(sim);
                return false;
            }
#line 3958 "src/taumir/lower.tr"
            List_ptr* siargs = (void*)List_ptr_new();
#line 3959 "src/taumir/lower.tr"
            List_ptr_append(siargs, idx);
#line 3960 "src/taumir/lower.tr"
            List_ptr_append(siargs, val);
#line 3961 "src/taumir/lower.tr"
            bool _trret1599 = (_lower_obj_call(m, lf, sim, siself, siargs) >= 0LL);
            _tr_str_release(sicls);
            _tr_str_release(sim);
            return _trret1599;
        }
    }
#line 3964 "src/taumir/lower.tr"
    TrStr awn = _ident_name(obj);
#line 3965 "src/taumir/lower.tr"
    if (((!_tr_str_eqv((awn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, awn) > 0LL))) {
#line 3966 "src/taumir/lower.tr"
        long long aiv = lower_expr(m, lf, idx);
#line 3967 "src/taumir/lower.tr"
        if (((aiv < 0LL) || (LFunc_vreg_type(lf, aiv) != 0LL))) {
#line 3967 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3968 "src/taumir/lower.tr"
        long long avv = lower_expr(m, lf, val);
#line 3969 "src/taumir/lower.tr"
        if ((avv < 0LL)) {
#line 3969 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3970 "src/taumir/lower.tr"
        long long avt = LFunc_vreg_type(lf, avv);
#line 3971 "src/taumir/lower.tr"
        long long aet = _fixed_arr_elem_tag(m, lf, awn);
#line 3972 "src/taumir/lower.tr"
        if ((avt == 4LL)) {
#line 3972 "src/taumir/lower.tr"
            avt = 0LL;
        }
#line 3973 "src/taumir/lower.tr"
        if ((aet == 4LL)) {
#line 3973 "src/taumir/lower.tr"
            aet = 0LL;
        }
#line 3974 "src/taumir/lower.tr"
        if ((avt != aet)) {
#line 3974 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3975 "src/taumir/lower.tr"
        long long aaddr = _array_elem_addr(m, lf, awn, aiv);
#line 3976 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStore(aaddr, 0LL, avv, 2LL));
#line 3977 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 3978 "src/taumir/lower.tr"
        _tr_str_release(sicls);
        _tr_str_release(awn);
        return true;
    }
#line 3979 "src/taumir/lower.tr"
    long long ov = lower_expr(m, lf, obj);
#line 3980 "src/taumir/lower.tr"
    if ((ov < 0LL)) {
#line 3980 "src/taumir/lower.tr"
        _tr_str_release(sicls);
        _tr_str_release(awn);
        return false;
    }
#line 3981 "src/taumir/lower.tr"
    long long ovt = LFunc_vreg_type(lf, ov);
#line 3982 "src/taumir/lower.tr"
    if (_is_dict_tag(ovt)) {
#line 3983 "src/taumir/lower.tr"
        long long kv = lower_expr(m, lf, idx);
#line 3984 "src/taumir/lower.tr"
        if ((kv < 0LL)) {
#line 3984 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3985 "src/taumir/lower.tr"
        if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, kv) != 1LL))) {
#line 3985 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3986 "src/taumir/lower.tr"
        if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, kv) != 0LL))) {
#line 3986 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3987 "src/taumir/lower.tr"
        long long vv = lower_expr(m, lf, val);
#line 3988 "src/taumir/lower.tr"
        if (((vv < 0LL) || ({ __auto_type _ord_t6344 = (LFunc_vreg_type(lf, vv)); __auto_type _wr = ((_ord_t6344 != _dict_val_tag(ovt))); _wr; }))) {
#line 3988 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 3989 "src/taumir/lower.tr"
        if ((_dict_val_tag(ovt) == 1LL)) {
#line 3989 "src/taumir/lower.tr"
            _secure_str(m, lf, vv);
        }
#line 3990 "src/taumir/lower.tr"
        if (((_dict_val_tag(ovt) == 10LL) || (_dict_val_tag(ovt) == 11LL))) {
#line 3990 "src/taumir/lower.tr"
            _secure_obj(m, lf, vv);
        }
#line 3991 "src/taumir/lower.tr"
        if ((_dict_val_tag(ovt) == 5LL)) {
#line 3992 "src/taumir/lower.tr"
            long long ivb = LFunc_new_vreg(lf);
#line 3993 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFBits(ivb, vv));
#line 3994 "src/taumir/lower.tr"
            vv = ivb;
        }
#line 3995 "src/taumir/lower.tr"
        TrStr ssym = _dict_sym(ovt, _tr_str_lit_len("set", 3LL));
#line 3996 "src/taumir/lower.tr"
        LModule_add_extern(m, ssym);
#line 3997 "src/taumir/lower.tr"
        List_i64* sa = (void*)List_i64_new();
#line 3998 "src/taumir/lower.tr"
        List_i64_append(sa, ov);
#line 3999 "src/taumir/lower.tr"
        List_i64_append(sa, kv);
#line 4000 "src/taumir/lower.tr"
        List_i64_append(sa, vv);
#line 4001 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), ssym, sa));
#line 4002 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 4003 "src/taumir/lower.tr"
        _tr_str_release(sicls);
        _tr_str_release(awn);
        _tr_str_release(ssym);
        return true;
    }
#line 4004 "src/taumir/lower.tr"
    if (_is_list_tag(ovt)) {
#line 4005 "src/taumir/lower.tr"
        long long iv = lower_expr(m, lf, idx);
#line 4006 "src/taumir/lower.tr"
        if (((iv < 0LL) || (LFunc_vreg_type(lf, iv) != 0LL))) {
#line 4006 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 4007 "src/taumir/lower.tr"
        long long lvv = lower_expr(m, lf, val);
#line 4008 "src/taumir/lower.tr"
        if ((lvv < 0LL)) {
#line 4008 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 4009 "src/taumir/lower.tr"
        long long lvt = LFunc_vreg_type(lf, lvv);
#line 4010 "src/taumir/lower.tr"
        if (((_list_elem_tag(ovt) == 0LL) && (lvt == 4LL))) {
#line 4010 "src/taumir/lower.tr"
            lvt = 0LL;
        }
#line 4011 "src/taumir/lower.tr"
        if ((lvt != _list_elem_tag(ovt))) {
#line 4011 "src/taumir/lower.tr"
            _tr_str_release(sicls);
            _tr_str_release(awn);
            return false;
        }
#line 4012 "src/taumir/lower.tr"
        if ((_list_elem_tag(ovt) == 1LL)) {
#line 4012 "src/taumir/lower.tr"
            _secure_str(m, lf, lvv);
        }
#line 4013 "src/taumir/lower.tr"
        if ((_list_elem_tag(ovt) == 10LL)) {
#line 4013 "src/taumir/lower.tr"
            _secure_obj(m, lf, lvv);
        }
#line 4014 "src/taumir/lower.tr"
        if ((_list_elem_tag(ovt) == 5LL)) {
#line 4015 "src/taumir/lower.tr"
            long long lvfb = LFunc_new_vreg(lf);
#line 4016 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFBits(lvfb, lvv));
#line 4017 "src/taumir/lower.tr"
            lvv = lvfb;
        }
#line 4018 "src/taumir/lower.tr"
        long long la_addr = _inline_list_addr(m, lf, ov, iv, _list_stride(ovt));
#line 4019 "src/taumir/lower.tr"
        if ((ovt == 22LL)) {
#line 4020 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreB(la_addr, 0LL, lvv));
        } else {
#line 4022 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStore(la_addr, 0LL, lvv, 2LL));
        }
#line 4023 "src/taumir/lower.tr"
        _flush_fresh_strs(m, lf);
#line 4024 "src/taumir/lower.tr"
        _tr_str_release(sicls);
        _tr_str_release(awn);
        return true;
    }
#line 4025 "src/taumir/lower.tr"
    _tr_str_release(sicls);
    _tr_str_release(awn);
    return false;
}

__attribute__((hot)) TrStr _write_sym(long long t) {
#line 4029 "src/taumir/lower.tr"
    if ((t == 1LL)) {
#line 4029 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_write_cstr", 17LL);
    }
#line 4030 "src/taumir/lower.tr"
    if ((t == 4LL)) {
#line 4030 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_write_bool", 17LL);
    }
#line 4031 "src/taumir/lower.tr"
    return _tr_str_lit_len("_tr_rt_write_i64", 16LL);
}

__attribute__((hot)) void _emit_call0(LModule* m, LFunc* lf, TrStr sym) {
#line 4034 "src/taumir/lower.tr"
    LModule_add_extern(m, sym);
#line 4035 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, (void*)List_i64_new()));
}

__attribute__((hot)) bool _lower_print(LModule* m, LFunc* lf, List_ptr* args) {
#line 4040 "src/taumir/lower.tr"
    if ((args->len == 0LL)) {
#line 4041 "src/taumir/lower.tr"
        _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_nl", 15LL));
#line 4042 "src/taumir/lower.tr"
        return true;
    }
#line 4043 "src/taumir/lower.tr"
    if ((args->len == 1LL)) {
#line 4044 "src/taumir/lower.tr"
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 4045 "src/taumir/lower.tr"
        if ((av < 0LL)) {
#line 4045 "src/taumir/lower.tr"
            return false;
        }
#line 4046 "src/taumir/lower.tr"
        long long avt = LFunc_vreg_type(lf, av);
#line 4047 "src/taumir/lower.tr"
        if (_is_list_tag(avt)) {
#line 4048 "src/taumir/lower.tr"
            TrStr plsym = _tr_str_lit_len("_tr_rt_print_list_i64", 21LL);
#line 4049 "src/taumir/lower.tr"
            if ((avt == 3LL)) {
#line 4049 "src/taumir/lower.tr"
                TrStr _strtmp_t6345 = _tr_str_lit_len("_tr_rt_print_list_str", 21LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t6345;
            }
#line 4050 "src/taumir/lower.tr"
            if ((avt == 14LL)) {
#line 4050 "src/taumir/lower.tr"
                TrStr _strtmp_t6346 = _tr_str_lit_len("_tr_rt_print_list_f64", 21LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t6346;
            }
#line 4051 "src/taumir/lower.tr"
            if ((avt == 22LL)) {
#line 4051 "src/taumir/lower.tr"
                TrStr _strtmp_t6347 = _tr_str_lit_len("_tr_rt_print_list_bool", 22LL);
                _tr_str_release(plsym);
                plsym = _strtmp_t6347;
            }
#line 4052 "src/taumir/lower.tr"
            LModule_add_extern(m, plsym);
#line 4053 "src/taumir/lower.tr"
            List_i64* pla = (void*)List_i64_new();
#line 4054 "src/taumir/lower.tr"
            List_i64_append(pla, av);
#line 4055 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), plsym, pla));
#line 4056 "src/taumir/lower.tr"
            _tr_str_release(plsym);
            return true;
        }
#line 4057 "src/taumir/lower.tr"
        if ((avt == 10LL)) {
#line 4058 "src/taumir/lower.tr"
            long long ostr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), av);
#line 4059 "src/taumir/lower.tr"
            if ((ostr >= 0LL)) {
#line 4060 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_cstr", 17LL));
#line 4061 "src/taumir/lower.tr"
                List_i64* opa = (void*)List_i64_new();
#line 4062 "src/taumir/lower.tr"
                List_i64_append(opa, ostr);
#line 4063 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_print_cstr", 17LL), opa));
#line 4064 "src/taumir/lower.tr"
                return true;
            }
#line 4065 "src/taumir/lower.tr"
            return false;
        }
#line 4066 "src/taumir/lower.tr"
        if (((avt == 11LL) || _is_set_tag(avt))) {
#line 4066 "src/taumir/lower.tr"
            return false;
        }
#line 4067 "src/taumir/lower.tr"
        if ((avt == 15LL)) {
#line 4068 "src/taumir/lower.tr"
            long long tstr = _tuple_to_str(m, lf, av, hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL))));
#line 4069 "src/taumir/lower.tr"
            if ((tstr < 0LL)) {
#line 4069 "src/taumir/lower.tr"
                return false;
            }
#line 4070 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_cstr", 17LL));
#line 4071 "src/taumir/lower.tr"
            List_i64* tpa = (void*)List_i64_new();
#line 4072 "src/taumir/lower.tr"
            List_i64_append(tpa, tstr);
#line 4073 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_print_cstr", 17LL), tpa));
#line 4074 "src/taumir/lower.tr"
            return true;
        }
#line 4075 "src/taumir/lower.tr"
        if ((avt == 5LL)) {
#line 4076 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_print_f64", 16LL));
#line 4077 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit_len("_tr_rt_print_f64", 16LL), av));
#line 4078 "src/taumir/lower.tr"
            return true;
        }
#line 4079 "src/taumir/lower.tr"
        TrStr sym = _print_i64_sym();
#line 4080 "src/taumir/lower.tr"
        if ((avt == 1LL)) {
#line 4080 "src/taumir/lower.tr"
            TrStr _strtmp_t6348 = _tr_str_lit_len("_tr_rt_print_cstr", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t6348;
        } else if ((avt == 4LL)) {
#line 4081 "src/taumir/lower.tr"
            TrStr _strtmp_t6349 = _tr_str_lit_len("_tr_rt_print_bool", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t6349;
        } else if (((avt == 0LL) && _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(args, 0LL)))->name), (_tr_str_lit_len("char", 4LL))))) {
#line 4082 "src/taumir/lower.tr"
            TrStr _strtmp_t6350 = _tr_str_lit_len("_tr_rt_print_char", 17LL);
            _tr_str_release(sym);
            sym = _strtmp_t6350;
        }
#line 4083 "src/taumir/lower.tr"
        LModule_add_extern(m, sym);
#line 4084 "src/taumir/lower.tr"
        List_i64* cargs = (void*)List_i64_new();
#line 4085 "src/taumir/lower.tr"
        List_i64_append(cargs, av);
#line 4086 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, cargs));
#line 4087 "src/taumir/lower.tr"
        _tr_str_release(sym);
        return true;
    }
#line 4088 "src/taumir/lower.tr"
    long long pi = 0LL;
#line 4089 "src/taumir/lower.tr"
    while ((pi < args->len)) {
#line 4090 "src/taumir/lower.tr"
        if ((pi > 0LL)) {
#line 4090 "src/taumir/lower.tr"
            _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_sp", 15LL));
        }
#line 4091 "src/taumir/lower.tr"
        long long pv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, pi)));
#line 4092 "src/taumir/lower.tr"
        if ((pv < 0LL)) {
#line 4092 "src/taumir/lower.tr"
            return false;
        }
#line 4093 "src/taumir/lower.tr"
        long long pvt = LFunc_vreg_type(lf, pv);
#line 4094 "src/taumir/lower.tr"
        if (_is_list_tag(pvt)) {
#line 4095 "src/taumir/lower.tr"
            TrStr wlsym = _tr_str_lit_len("_tr_rt_write_list_i64", 21LL);
#line 4096 "src/taumir/lower.tr"
            if ((pvt == 3LL)) {
#line 4096 "src/taumir/lower.tr"
                TrStr _strtmp_t6351 = _tr_str_lit_len("_tr_rt_write_list_str", 21LL);
                _tr_str_release(wlsym);
                wlsym = _strtmp_t6351;
            }
#line 4097 "src/taumir/lower.tr"
            if ((pvt == 14LL)) {
#line 4097 "src/taumir/lower.tr"
                TrStr _strtmp_t6352 = _tr_str_lit_len("_tr_rt_write_list_f64", 21LL);
                _tr_str_release(wlsym);
                wlsym = _strtmp_t6352;
            }
#line 4098 "src/taumir/lower.tr"
            LModule_add_extern(m, wlsym);
#line 4099 "src/taumir/lower.tr"
            List_i64* wla = (void*)List_i64_new();
#line 4100 "src/taumir/lower.tr"
            List_i64_append(wla, pv);
#line 4101 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), wlsym, wla));
#line 4102 "src/taumir/lower.tr"
            pi = (pi + 1LL);
#line 4103 "src/taumir/lower.tr"
            _tr_str_release(wlsym);
            continue;
        }
#line 4104 "src/taumir/lower.tr"
        if ((pvt == 10LL)) {
#line 4105 "src/taumir/lower.tr"
            long long postr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, pi)), pv);
#line 4106 "src/taumir/lower.tr"
            if ((postr < 0LL)) {
#line 4106 "src/taumir/lower.tr"
                return false;
            }
#line 4107 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_cstr", 17LL));
#line 4108 "src/taumir/lower.tr"
            List_i64* pwa = (void*)List_i64_new();
#line 4109 "src/taumir/lower.tr"
            List_i64_append(pwa, postr);
#line 4110 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_write_cstr", 17LL), pwa));
#line 4111 "src/taumir/lower.tr"
            pi = (pi + 1LL);
#line 4112 "src/taumir/lower.tr"
            continue;
        }
#line 4113 "src/taumir/lower.tr"
        if (((pvt == 11LL) || _is_set_tag(pvt))) {
#line 4113 "src/taumir/lower.tr"
            return false;
        }
#line 4114 "src/taumir/lower.tr"
        if ((pvt == 15LL)) {
#line 4115 "src/taumir/lower.tr"
            long long ptstr = _tuple_to_str(m, lf, pv, hir_expr_type(((HirExpr*)List_ptr_get(args, pi))));
#line 4116 "src/taumir/lower.tr"
            if ((ptstr < 0LL)) {
#line 4116 "src/taumir/lower.tr"
                return false;
            }
#line 4117 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_cstr", 17LL));
#line 4118 "src/taumir/lower.tr"
            List_i64* ptwa = (void*)List_i64_new();
#line 4119 "src/taumir/lower.tr"
            List_i64_append(ptwa, ptstr);
#line 4120 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_write_cstr", 17LL), ptwa));
#line 4121 "src/taumir/lower.tr"
            pi = (pi + 1LL);
#line 4122 "src/taumir/lower.tr"
            continue;
        }
#line 4123 "src/taumir/lower.tr"
        if ((pvt == 5LL)) {
#line 4124 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_write_f64", 16LL));
#line 4125 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCall1((-1LL), _tr_str_lit_len("_tr_rt_write_f64", 16LL), pv));
        } else {
#line 4127 "src/taumir/lower.tr"
            TrStr wsym = _write_sym(pvt);
#line 4128 "src/taumir/lower.tr"
            if (((pvt == 0LL) && _tr_str_eqv((hir_expr_type(((HirExpr*)List_ptr_get(args, pi)))->name), (_tr_str_lit_len("char", 4LL))))) {
#line 4128 "src/taumir/lower.tr"
                TrStr _strtmp_t6353 = _tr_str_lit_len("_tr_rt_write_char", 17LL);
                _tr_str_release(wsym);
                wsym = _strtmp_t6353;
            }
#line 4129 "src/taumir/lower.tr"
            LModule_add_extern(m, wsym);
#line 4130 "src/taumir/lower.tr"
            List_i64* wargs = (void*)List_i64_new();
#line 4131 "src/taumir/lower.tr"
            List_i64_append(wargs, pv);
#line 4132 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), wsym, wargs));
            _tr_str_release(wsym);
        }
#line 4133 "src/taumir/lower.tr"
        pi = (pi + 1LL);
    }
#line 4134 "src/taumir/lower.tr"
    _emit_call0(m, lf, _tr_str_lit_len("_tr_rt_write_nl", 15LL));
#line 4135 "src/taumir/lower.tr"
    return true;
}

__attribute__((hot)) bool _lower_assert_cmp(LModule* m, LFunc* lf, TrStr fname, List_ptr* args) {
#line 4141 "src/taumir/lower.tr"
    if ((args->len < 2LL)) {
#line 4141 "src/taumir/lower.tr"
        return false;
    }
#line 4142 "src/taumir/lower.tr"
    long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 4143 "src/taumir/lower.tr"
    if ((av < 0LL)) {
#line 4143 "src/taumir/lower.tr"
        return false;
    }
#line 4144 "src/taumir/lower.tr"
    long long bv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 1LL)));
#line 4145 "src/taumir/lower.tr"
    if ((bv < 0LL)) {
#line 4145 "src/taumir/lower.tr"
        return false;
    }
#line 4146 "src/taumir/lower.tr"
    TrStr op = _tr_str_lit_len("==", 2LL);
#line 4147 "src/taumir/lower.tr"
    if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_ne", 9LL)))) {
#line 4147 "src/taumir/lower.tr"
        TrStr _strtmp_t6354 = _tr_str_lit_len("!=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t6354;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_lt", 9LL)))) {
#line 4148 "src/taumir/lower.tr"
        TrStr _strtmp_t6355 = _tr_str_lit_len("<", 1LL);
        _tr_str_release(op);
        op = _strtmp_t6355;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_le", 9LL)))) {
#line 4149 "src/taumir/lower.tr"
        TrStr _strtmp_t6356 = _tr_str_lit_len("<=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t6356;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_gt", 9LL)))) {
#line 4150 "src/taumir/lower.tr"
        TrStr _strtmp_t6357 = _tr_str_lit_len(">", 1LL);
        _tr_str_release(op);
        op = _strtmp_t6357;
    } else if (_tr_str_eqv((fname), (_tr_str_lit_len("assert_ge", 9LL)))) {
#line 4151 "src/taumir/lower.tr"
        TrStr _strtmp_t6358 = _tr_str_lit_len(">=", 2LL);
        _tr_str_release(op);
        op = _strtmp_t6358;
    }
#line 4152 "src/taumir/lower.tr"
    long long at = LFunc_vreg_type(lf, av);
#line 4153 "src/taumir/lower.tr"
    long long cmpv = (-1LL);
#line 4154 "src/taumir/lower.tr"
    if ((at == 1LL)) {
#line 4155 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, bv) != 1LL)) {
#line 4155 "src/taumir/lower.tr"
            _tr_str_release(op);
            return false;
        }
#line 4156 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
#line 4157 "src/taumir/lower.tr"
        List_i64* sa = (void*)List_i64_new();
#line 4158 "src/taumir/lower.tr"
        List_i64_append(sa, av);
#line 4159 "src/taumir/lower.tr"
        List_i64_append(sa, bv);
#line 4160 "src/taumir/lower.tr"
        long long cr = LFunc_new_vreg(lf);
#line 4161 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(cr, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sa));
#line 4162 "src/taumir/lower.tr"
        long long z = LFunc_new_vreg(lf);
#line 4163 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 4164 "src/taumir/lower.tr"
        cmpv = LFunc_new_vreg(lf);
#line 4165 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(cmpv, op, cr, z));
    } else if ((((at == 0LL) || (at == 4LL)) || (at == 5LL))) {
#line 4167 "src/taumir/lower.tr"
        cmpv = LFunc_new_vreg(lf);
#line 4168 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(cmpv, op, av, bv));
    } else {
#line 4170 "src/taumir/lower.tr"
        _tr_str_release(op);
        return false;
    }
#line 4171 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, cmpv, 4LL);
#line 4172 "src/taumir/lower.tr"
    _flush_fresh_strs(m, lf);
#line 4173 "src/taumir/lower.tr"
    long long ok = LFunc_new_block(lf);
#line 4174 "src/taumir/lower.tr"
    long long fail = LFunc_new_block(lf);
#line 4175 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cmpv, ok, fail));
#line 4176 "src/taumir/lower.tr"
    LFunc_set_cur(lf, fail);
#line 4177 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_assert_fail", 18LL));
#line 4178 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_assert_fail", 18LL), (void*)List_i64_new()));
#line 4179 "src/taumir/lower.tr"
    LFunc_set_term(lf, LTerm_ctor_TBr(ok));
#line 4180 "src/taumir/lower.tr"
    LFunc_set_cur(lf, ok);
#line 4181 "src/taumir/lower.tr"
    _tr_str_release(op);
    return true;
}

__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e) {
#line 4184 "src/taumir/lower.tr"
    __auto_type _t6359 = (*e);
    if (_t6359.tag == HirExpr_ECall) {
        __auto_type callee = _t6359.data.ECall.callee;
__auto_type args = _t6359.data.ECall.args;
#line 4186 "src/taumir/lower.tr"
        TrStr fname = _ident_name(callee);
#line 4187 "src/taumir/lower.tr"
        if (_tr_str_eqv((fname), (_tr_str_lit_len("print", 5LL)))) {
#line 4188 "src/taumir/lower.tr"
            bool _trret1600 = _lower_print(m, lf, args);
            _tr_str_release(fname);
            return _trret1600;
        }
#line 4189 "src/taumir/lower.tr"
        if ((((((_tr_str_eqv((fname), (_tr_str_lit_len("assert_eq", 9LL))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_ne", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_lt", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_le", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_gt", 9LL)))) || _tr_str_eqv((fname), (_tr_str_lit_len("assert_ge", 9LL))))) {
#line 4190 "src/taumir/lower.tr"
            bool _trret1601 = _lower_assert_cmp(m, lf, fname, args);
            _tr_str_release(fname);
            return _trret1601;
        }
#line 4191 "src/taumir/lower.tr"
        long long r = lower_expr(m, lf, e);
#line 4192 "src/taumir/lower.tr"
        bool _trret1602 = (r >= 0LL);
        _tr_str_release(fname);
        return _trret1602;
    } else if (_t6359.tag == HirExpr_EMethodCall) {
#line 4194 "src/taumir/lower.tr"
        long long rm = lower_expr(m, lf, e);
#line 4195 "src/taumir/lower.tr"
        return (rm >= 0LL);
    } else if (1) {
        __auto_type _ = _t6359;
#line 4199 "src/taumir/lower.tr"
        long long rx = lower_expr(m, lf, e);
#line 4200 "src/taumir/lower.tr"
        return (rx >= 0LL);
    }
}

__attribute__((hot)) bool _int_op(TrStr op) {
#line 4204 "src/taumir/lower.tr"
    if ((((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("%", 1LL))))) {
#line 4204 "src/taumir/lower.tr"
        return true;
    }
#line 4205 "src/taumir/lower.tr"
    if (((((_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("|", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL))))) {
#line 4205 "src/taumir/lower.tr"
        return true;
    }
#line 4206 "src/taumir/lower.tr"
    if ((((((_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))))) {
#line 4206 "src/taumir/lower.tr"
        return true;
    }
#line 4207 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) TrStr _lir_digit(long long d) {
#line 4210 "src/taumir/lower.tr"
    if ((d == 0LL)) {
#line 4210 "src/taumir/lower.tr"
        return _tr_str_lit_len("0", 1LL);
    }
#line 4211 "src/taumir/lower.tr"
    if ((d == 1LL)) {
#line 4211 "src/taumir/lower.tr"
        return _tr_str_lit_len("1", 1LL);
    }
#line 4212 "src/taumir/lower.tr"
    if ((d == 2LL)) {
#line 4212 "src/taumir/lower.tr"
        return _tr_str_lit_len("2", 1LL);
    }
#line 4213 "src/taumir/lower.tr"
    if ((d == 3LL)) {
#line 4213 "src/taumir/lower.tr"
        return _tr_str_lit_len("3", 1LL);
    }
#line 4214 "src/taumir/lower.tr"
    if ((d == 4LL)) {
#line 4214 "src/taumir/lower.tr"
        return _tr_str_lit_len("4", 1LL);
    }
#line 4215 "src/taumir/lower.tr"
    if ((d == 5LL)) {
#line 4215 "src/taumir/lower.tr"
        return _tr_str_lit_len("5", 1LL);
    }
#line 4216 "src/taumir/lower.tr"
    if ((d == 6LL)) {
#line 4216 "src/taumir/lower.tr"
        return _tr_str_lit_len("6", 1LL);
    }
#line 4217 "src/taumir/lower.tr"
    if ((d == 7LL)) {
#line 4217 "src/taumir/lower.tr"
        return _tr_str_lit_len("7", 1LL);
    }
#line 4218 "src/taumir/lower.tr"
    if ((d == 8LL)) {
#line 4218 "src/taumir/lower.tr"
        return _tr_str_lit_len("8", 1LL);
    }
#line 4219 "src/taumir/lower.tr"
    return _tr_str_lit_len("9", 1LL);
}

__attribute__((hot)) TrStr _lir_itoa(long long n) {
#line 4223 "src/taumir/lower.tr"
    if ((n == 0LL)) {
#line 4223 "src/taumir/lower.tr"
        return _tr_str_lit_len("0", 1LL);
    }
#line 4224 "src/taumir/lower.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 4225 "src/taumir/lower.tr"
    long long x = n;
#line 4226 "src/taumir/lower.tr"
    while ((x > 0LL)) {
#line 4227 "src/taumir/lower.tr"
        TrStr _strtmp_t6360 = ({ TrStr _tr_cl = (_lir_digit(_tr_imod((long long)(x), (long long)(10LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t6360;
#line 4228 "src/taumir/lower.tr"
        x = _tr_idiv((long long)(x), (long long)(10LL));
    }
#line 4229 "src/taumir/lower.tr"
    TrStr _trret1603 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret1603;
}

__attribute__((hot)) void _fresh_mark(LFunc* lf, long long v) {
#line 4236 "src/taumir/lower.tr"
    List_i64_append(lf->fresh_strs, v);
}

__attribute__((hot)) bool _fresh_take(LFunc* lf, long long v) {
#line 4239 "src/taumir/lower.tr"
    long long i = 0LL;
#line 4240 "src/taumir/lower.tr"
    while ((i < lf->fresh_strs->len)) {
#line 4241 "src/taumir/lower.tr"
        if ((List_i64_get(lf->fresh_strs, i) == v)) {
#line 4242 "src/taumir/lower.tr"
            List_i64_remove(lf->fresh_strs, i);
#line 4243 "src/taumir/lower.tr"
            return true;
        }
#line 4244 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 4245 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) void _release_str(LModule* m, LFunc* lf, long long v) {
#line 4248 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_release", 18LL));
#line 4249 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4250 "src/taumir/lower.tr"
    List_i64_append(a, v);
#line 4251 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_str_release", 18LL), a));
}

__attribute__((hot)) void _retain_str(LModule* m, LFunc* lf, long long v) {
#line 4254 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_retain", 17LL));
#line 4255 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4256 "src/taumir/lower.tr"
    List_i64_append(a, v);
#line 4257 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_str_retain", 17LL), a));
}

__attribute__((hot)) void _flush_fresh_strs(LModule* m, LFunc* lf) {
#line 4264 "src/taumir/lower.tr"
    while ((lf->fresh_strs->len > 0LL)) {
#line 4265 "src/taumir/lower.tr"
        long long v = List_i64_pop(lf->fresh_strs);
#line 4266 "src/taumir/lower.tr"
        _release_str(m, lf, v);
    }
#line 4267 "src/taumir/lower.tr"
    while ((lf->fresh_objs->len > 0LL)) {
#line 4268 "src/taumir/lower.tr"
        long long ov = List_i64_pop(lf->fresh_objs);
#line 4269 "src/taumir/lower.tr"
        _release_obj(m, lf, ov);
    }
}

__attribute__((hot)) void _secure_str(LModule* m, LFunc* lf, long long v) {
#line 4274 "src/taumir/lower.tr"
    if ((!_fresh_take(lf, v))) {
#line 4275 "src/taumir/lower.tr"
        _retain_str(m, lf, v);
    }
}

__attribute__((hot)) void _fresh_mark_obj(LFunc* lf, long long v) {
#line 4284 "src/taumir/lower.tr"
    List_i64_append(lf->fresh_objs, v);
}

__attribute__((hot)) bool _fresh_take_obj(LFunc* lf, long long v) {
#line 4287 "src/taumir/lower.tr"
    long long i = 0LL;
#line 4288 "src/taumir/lower.tr"
    while ((i < lf->fresh_objs->len)) {
#line 4289 "src/taumir/lower.tr"
        if ((List_i64_get(lf->fresh_objs, i) == v)) {
#line 4290 "src/taumir/lower.tr"
            List_i64_remove(lf->fresh_objs, i);
#line 4291 "src/taumir/lower.tr"
            return true;
        }
#line 4292 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 4293 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) void _release_obj(LModule* m, LFunc* lf, long long v) {
#line 4296 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_release", 18LL));
#line 4297 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4298 "src/taumir/lower.tr"
    List_i64_append(a, v);
#line 4299 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_release", 18LL), a));
}

__attribute__((hot)) void _retain_obj(LModule* m, LFunc* lf, long long v) {
#line 4302 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_retain", 17LL));
#line 4303 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4304 "src/taumir/lower.tr"
    List_i64_append(a, v);
#line 4305 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_retain", 17LL), a));
}

__attribute__((hot)) void _flush_fresh_objs(LModule* m, LFunc* lf) {
#line 4309 "src/taumir/lower.tr"
    while ((lf->fresh_objs->len > 0LL)) {
#line 4310 "src/taumir/lower.tr"
        long long v = List_i64_pop(lf->fresh_objs);
#line 4311 "src/taumir/lower.tr"
        _release_obj(m, lf, v);
    }
}

__attribute__((hot)) bool _is_owned_local_return(LFunc* lf, HirExpr* val) {
#line 4320 "src/taumir/lower.tr"
    __auto_type _t6361 = (*val);
    if (_t6361.tag == HirExpr_EIdent) {
        __auto_type vn = _t6361.data.EIdent.name;
#line 4322 "src/taumir/lower.tr"
        if ((LFunc_var_index(lf, vn) < 0LL)) {
#line 4322 "src/taumir/lower.tr"
            return false;
        }
#line 4323 "src/taumir/lower.tr"
        if (_is_param(lf, vn)) {
#line 4323 "src/taumir/lower.tr"
            return false;
        }
#line 4324 "src/taumir/lower.tr"
        long long vt = LFunc_var_type(lf, vn);
#line 4325 "src/taumir/lower.tr"
        bool _trret1604 = ((vt == 10LL) || (vt == 11LL));
        return _trret1604;
    } else if (1) {
        __auto_type _ = _t6361;
#line 4327 "src/taumir/lower.tr"
        return false;
    }
}

__attribute__((hot)) void _secure_obj(LModule* m, LFunc* lf, long long v) {
#line 4331 "src/taumir/lower.tr"
    if ((!_fresh_take_obj(lf, v))) {
#line 4332 "src/taumir/lower.tr"
        _retain_obj(m, lf, v);
    }
}

__attribute__((hot)) bool _is_param(LFunc* lf, TrStr name) {
#line 4335 "src/taumir/lower.tr"
    long long i = 0LL;
#line 4336 "src/taumir/lower.tr"
    while ((i < lf->params->len)) {
#line 4337 "src/taumir/lower.tr"
        if (({ TrStr _cmpa_t6362 = (List_TrStr_get(lf->params, i)); TrStr _cmpb_t6363 = (name); _Bool _cmpr_t6364 = _tr_str_eqv(_cmpa_t6362, _cmpb_t6363); _tr_str_release(_cmpa_t6362); _cmpr_t6364; })) {
#line 4337 "src/taumir/lower.tr"
            return true;
        }
#line 4338 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 4339 "src/taumir/lower.tr"
    return false;
}

__attribute__((hot)) long long _norm_bool(LFunc* lf, long long v) {
#line 4343 "src/taumir/lower.tr"
    long long z = LFunc_new_vreg(lf);
#line 4344 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 4345 "src/taumir/lower.tr"
    long long r = LFunc_new_vreg(lf);
#line 4346 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(r, _tr_str_lit_len("!=", 2LL), v, z));
#line 4347 "src/taumir/lower.tr"
    return r;
}

__attribute__((hot)) long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype) {
#line 4351 "src/taumir/lower.tr"
    LModule_add_extern(m, sym);
#line 4352 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4353 "src/taumir/lower.tr"
    List_i64_append(a, _tr_v_recv);
#line 4354 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4355 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
#line 4356 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, restype);
#line 4357 "src/taumir/lower.tr"
    if ((restype == 1LL)) {
#line 4357 "src/taumir/lower.tr"
        _fresh_mark(lf, d);
    }
#line 4358 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _heap_lit(LModule* m, LFunc* lf, TrStr s) {
#line 4365 "src/taumir/lower.tr"
    return _heap_lit_len(m, lf, s, _tr_str_lenv((s)));
}

__attribute__((hot)) long long _heap_lit_len(LModule* m, LFunc* lf, TrStr s, long long blen) {
#line 4371 "src/taumir/lower.tr"
    long long idx = LModule_add_string(m, s, blen);
#line 4372 "src/taumir/lower.tr"
    long long ds = LFunc_new_vreg(lf);
#line 4373 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
#line 4374 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, ds, 1LL);
#line 4375 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 4376 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4377 "src/taumir/lower.tr"
    List_i64_append(a, ds);
#line 4378 "src/taumir/lower.tr"
    long long h = LFunc_new_vreg(lf);
#line 4379 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(h, _tr_str_lit_len("_tr_rt_str_new", 14LL), a));
#line 4380 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, h, 1LL);
#line 4381 "src/taumir/lower.tr"
    _fresh_mark(lf, h);
#line 4382 "src/taumir/lower.tr"
    return h;
}

__attribute__((hot)) long long _obj_to_str(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg) {
#line 4390 "src/taumir/lower.tr"
    TrStr ocls = _recv_class(m, lf, objexpr);
#line 4391 "src/taumir/lower.tr"
    if ((_tr_str_eqv((ocls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, ocls)))) {
#line 4391 "src/taumir/lower.tr"
        _tr_str_release(ocls);
        return (-1LL);
    }
#line 4392 "src/taumir/lower.tr"
    TrStr osm = LModule_resolve_method(m, ocls, _tr_str_lit_len("__str__", 7LL));
#line 4393 "src/taumir/lower.tr"
    if (_tr_str_eqv((osm), (_tr_str_lit_len("", 0LL)))) {
#line 4393 "src/taumir/lower.tr"
        _tr_str_release(ocls);
        _tr_str_release(osm);
        return (-1LL);
    }
#line 4394 "src/taumir/lower.tr"
    long long oself = objreg;
#line 4395 "src/taumir/lower.tr"
    if ((oself < 0LL)) {
#line 4396 "src/taumir/lower.tr"
        oself = lower_expr(m, lf, objexpr);
#line 4397 "src/taumir/lower.tr"
        if ((oself < 0LL)) {
#line 4397 "src/taumir/lower.tr"
            _tr_str_release(ocls);
            _tr_str_release(osm);
            return (-1LL);
        }
    }
#line 4398 "src/taumir/lower.tr"
    long long sd = _lower_obj_call(m, lf, osm, oself, (void*)List_ptr_new());
#line 4399 "src/taumir/lower.tr"
    long long _trret1605 = sd;
    _tr_str_release(ocls);
    _tr_str_release(osm);
    return _trret1605;
}

__attribute__((hot)) long long _obj_repr(LModule* m, LFunc* lf, HirExpr* objexpr, long long objreg) {
#line 4403 "src/taumir/lower.tr"
    TrStr rcls = _recv_class(m, lf, objexpr);
#line 4404 "src/taumir/lower.tr"
    if ((_tr_str_eqv((rcls), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, rcls)))) {
#line 4404 "src/taumir/lower.tr"
        _tr_str_release(rcls);
        return (-1LL);
    }
#line 4405 "src/taumir/lower.tr"
    TrStr rm = LModule_resolve_method(m, rcls, _tr_str_lit_len("__repr__", 8LL));
#line 4406 "src/taumir/lower.tr"
    if (_tr_str_eqv((rm), (_tr_str_lit_len("", 0LL)))) {
#line 4406 "src/taumir/lower.tr"
        long long _trret1606 = _obj_to_str(m, lf, objexpr, objreg);
        _tr_str_release(rcls);
        _tr_str_release(rm);
        return _trret1606;
    }
#line 4407 "src/taumir/lower.tr"
    long long rself = objreg;
#line 4408 "src/taumir/lower.tr"
    if ((rself < 0LL)) {
#line 4409 "src/taumir/lower.tr"
        rself = lower_expr(m, lf, objexpr);
#line 4410 "src/taumir/lower.tr"
        if ((rself < 0LL)) {
#line 4410 "src/taumir/lower.tr"
            _tr_str_release(rcls);
            _tr_str_release(rm);
            return (-1LL);
        }
    }
#line 4411 "src/taumir/lower.tr"
    long long _trret1607 = _lower_obj_call(m, lf, rm, rself, (void*)List_ptr_new());
    _tr_str_release(rcls);
    _tr_str_release(rm);
    return _trret1607;
}

__attribute__((hot)) long long _reg_to_str(LModule* m, LFunc* lf, long long reg) {
#line 4414 "src/taumir/lower.tr"
    long long t = LFunc_vreg_type(lf, reg);
#line 4415 "src/taumir/lower.tr"
    if ((t == 1LL)) {
#line 4415 "src/taumir/lower.tr"
        return reg;
    }
#line 4416 "src/taumir/lower.tr"
    if ((t == 5LL)) {
#line 4417 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
#line 4418 "src/taumir/lower.tr"
        long long fd = LFunc_new_vreg(lf);
#line 4419 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IFCall1(fd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), reg));
#line 4420 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, fd, 1LL);
#line 4421 "src/taumir/lower.tr"
        _fresh_mark(lf, fd);
#line 4422 "src/taumir/lower.tr"
        return fd;
    }
#line 4423 "src/taumir/lower.tr"
    TrStr sym = _tr_str_lit_len("_tr_rt_i64_to_str", 17LL);
#line 4424 "src/taumir/lower.tr"
    if ((t == 4LL)) {
#line 4424 "src/taumir/lower.tr"
        TrStr _strtmp_t6365 = _tr_str_lit_len("_tr_rt_bool_to_str", 18LL);
        _tr_str_release(sym);
        sym = _strtmp_t6365;
    } else if ((t != 0LL)) {
#line 4425 "src/taumir/lower.tr"
        _tr_str_release(sym);
        return (-1LL);
    }
#line 4426 "src/taumir/lower.tr"
    LModule_add_extern(m, sym);
#line 4427 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4428 "src/taumir/lower.tr"
    List_i64_append(a, reg);
#line 4429 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4430 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
#line 4431 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, 1LL);
#line 4432 "src/taumir/lower.tr"
    _fresh_mark(lf, d);
#line 4433 "src/taumir/lower.tr"
    long long _trret1608 = d;
    _tr_str_release(sym);
    return _trret1608;
}

__attribute__((hot)) long long _str_concat2(LModule* m, LFunc* lf, long long a, long long b) {
#line 4437 "src/taumir/lower.tr"
    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
#line 4438 "src/taumir/lower.tr"
    List_i64* ca = (void*)List_i64_new();
#line 4439 "src/taumir/lower.tr"
    List_i64_append(ca, a);
#line 4440 "src/taumir/lower.tr"
    List_i64_append(ca, b);
#line 4441 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4442 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, _tr_str_lit_len("_tr_rt_str_concat", 17LL), ca));
#line 4443 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, 1LL);
#line 4444 "src/taumir/lower.tr"
    _fresh_mark(lf, d);
#line 4445 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _tuple_to_str(LModule* m, LFunc* lf, long long tup, AstType* ty) {
#line 4449 "src/taumir/lower.tr"
    if ((ty->args->len == 0LL)) {
#line 4450 "src/taumir/lower.tr"
        return _heap_lit(m, lf, _tr_str_lit_len("()", 2LL));
    }
#line 4451 "src/taumir/lower.tr"
    long long acc = _heap_lit(m, lf, _tr_str_lit_len("(", 1LL));
#line 4452 "src/taumir/lower.tr"
    long long i = 0LL;
#line 4453 "src/taumir/lower.tr"
    while ((i < ty->args->len)) {
#line 4454 "src/taumir/lower.tr"
        long long etag = _tag_of(m, (*((AstType**)List_ptr_get(ty->args, i))));
#line 4455 "src/taumir/lower.tr"
        if (((((((etag < 0LL) || _is_list_tag(etag)) || _is_dict_tag(etag)) || _is_set_tag(etag)) || (etag == 12LL)) || (etag == 15LL))) {
#line 4456 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4457 "src/taumir/lower.tr"
        long long ev = _emit_field_get(m, lf, tup, (i * 8LL), 0LL);
#line 4458 "src/taumir/lower.tr"
        if ((etag == 5LL)) {
#line 4459 "src/taumir/lower.tr"
            long long fv = LFunc_new_vreg(lf);
#line 4460 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBitsF(fv, ev));
#line 4461 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, fv, 5LL);
#line 4462 "src/taumir/lower.tr"
            ev = fv;
        } else {
#line 4464 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ev, etag);
        }
#line 4465 "src/taumir/lower.tr"
        long long es = _reg_to_str(m, lf, ev);
#line 4466 "src/taumir/lower.tr"
        if ((es < 0LL)) {
#line 4466 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4467 "src/taumir/lower.tr"
        if ((etag == 1LL)) {
#line 4468 "src/taumir/lower.tr"
            es = _str_concat2(m, lf, _heap_lit(m, lf, _tr_str_lit_len("'", 1LL)), es);
#line 4469 "src/taumir/lower.tr"
            es = _str_concat2(m, lf, es, _heap_lit(m, lf, _tr_str_lit_len("'", 1LL)));
        }
#line 4470 "src/taumir/lower.tr"
        if ((i > 0LL)) {
#line 4470 "src/taumir/lower.tr"
            acc = _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(", ", 2LL)));
        }
#line 4471 "src/taumir/lower.tr"
        acc = _str_concat2(m, lf, acc, es);
#line 4472 "src/taumir/lower.tr"
        i = (i + 1LL);
    }
#line 4473 "src/taumir/lower.tr"
    if ((ty->args->len == 1LL)) {
#line 4473 "src/taumir/lower.tr"
        acc = _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(",", 1LL)));
    }
#line 4474 "src/taumir/lower.tr"
    return _str_concat2(m, lf, acc, _heap_lit(m, lf, _tr_str_lit_len(")", 1LL)));
}

__attribute__((hot)) long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype) {
#line 4478 "src/taumir/lower.tr"
    LModule_add_extern(m, sym);
#line 4479 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4480 "src/taumir/lower.tr"
    List_i64_append(a, _tr_v_recv);
#line 4481 "src/taumir/lower.tr"
    List_i64_append(a, arg);
#line 4482 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4483 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
#line 4484 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, restype);
#line 4485 "src/taumir/lower.tr"
    if ((restype == 1LL)) {
#line 4485 "src/taumir/lower.tr"
        _fresh_mark(lf, d);
    }
#line 4486 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
#line 4490 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
#line 4491 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
#line 4492 "src/taumir/lower.tr"
        List_i64* la = (void*)List_i64_new();
#line 4493 "src/taumir/lower.tr"
        List_i64_append(la, _tr_v_recv);
#line 4494 "src/taumir/lower.tr"
        long long ld = LFunc_new_vreg(lf);
#line 4495 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit_len("_tr_rt_strlen", 13LL), la));
#line 4496 "src/taumir/lower.tr"
        return ld;
    }
#line 4497 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("upper", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_upper", 8LL)))) && (margs->len == 0LL))) {
#line 4498 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_upper", 16LL), _tr_v_recv, 1LL);
    }
#line 4499 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("lower", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_lower", 8LL)))) && (margs->len == 0LL))) {
#line 4500 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_lower", 16LL), _tr_v_recv, 1LL);
    }
#line 4501 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("strip", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("trim", 4LL)))) && (margs->len == 0LL))) {
#line 4502 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_strip", 16LL), _tr_v_recv, 1LL);
    }
#line 4503 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_int", 6LL))) && (margs->len == 0LL))) {
#line 4504 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), _tr_v_recv, 0LL);
    }
#line 4505 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("split_once", 10LL))) && (margs->len == 1LL))) {
#line 4507 "src/taumir/lower.tr"
        long long soa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4508 "src/taumir/lower.tr"
        if (((soa < 0LL) || (LFunc_vreg_type(lf, soa) != 1LL))) {
#line 4508 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4509 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_split_once_left", 22LL));
#line 4510 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_split_once_right", 23LL));
#line 4511 "src/taumir/lower.tr"
        List_i64* sola = (void*)List_i64_new();
#line 4512 "src/taumir/lower.tr"
        List_i64_append(sola, _tr_v_recv);
#line 4513 "src/taumir/lower.tr"
        List_i64_append(sola, soa);
#line 4514 "src/taumir/lower.tr"
        long long sol = LFunc_new_vreg(lf);
#line 4515 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sol, _tr_str_lit_len("_tr_rt_split_once_left", 22LL), sola));
#line 4516 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sol, 1LL);
#line 4517 "src/taumir/lower.tr"
        _fresh_mark(lf, sol);
#line 4518 "src/taumir/lower.tr"
        _secure_str(m, lf, sol);
#line 4519 "src/taumir/lower.tr"
        List_i64* sora = (void*)List_i64_new();
#line 4520 "src/taumir/lower.tr"
        List_i64_append(sora, _tr_v_recv);
#line 4521 "src/taumir/lower.tr"
        List_i64_append(sora, soa);
#line 4522 "src/taumir/lower.tr"
        long long sor = LFunc_new_vreg(lf);
#line 4523 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sor, _tr_str_lit_len("_tr_rt_split_once_right", 23LL), sora));
#line 4524 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sor, 1LL);
#line 4525 "src/taumir/lower.tr"
        _fresh_mark(lf, sor);
#line 4526 "src/taumir/lower.tr"
        _secure_str(m, lf, sor);
#line 4527 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
#line 4528 "src/taumir/lower.tr"
        long long sols = LFunc_new_vreg(lf);
#line 4529 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sols, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
#line 4530 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sols, 3LL);
#line 4531 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_push_i64", 20LL));
#line 4532 "src/taumir/lower.tr"
        List_i64* sop1 = (void*)List_i64_new();
#line 4533 "src/taumir/lower.tr"
        List_i64_append(sop1, sols);
#line 4534 "src/taumir/lower.tr"
        List_i64_append(sop1, sol);
#line 4535 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_push_i64", 20LL), sop1));
#line 4536 "src/taumir/lower.tr"
        List_i64* sop2 = (void*)List_i64_new();
#line 4537 "src/taumir/lower.tr"
        List_i64_append(sop2, sols);
#line 4538 "src/taumir/lower.tr"
        List_i64_append(sop2, sor);
#line 4539 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_push_i64", 20LL), sop2));
#line 4540 "src/taumir/lower.tr"
        return sols;
    }
#line 4541 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) && (margs->len == 0LL))) {
#line 4543 "src/taumir/lower.tr"
        long long tf0 = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL), _tr_v_recv, 0LL);
#line 4544 "src/taumir/lower.tr"
        long long tff = LFunc_new_vreg(lf);
#line 4545 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(tff, tf0));
#line 4546 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, tff, 5LL);
#line 4547 "src/taumir/lower.tr"
        return tff;
    }
#line 4548 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("replace", 7LL))) && (margs->len == 2LL))) {
#line 4549 "src/taumir/lower.tr"
        long long a0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4550 "src/taumir/lower.tr"
        if (((a0 < 0LL) || (LFunc_vreg_type(lf, a0) != 1LL))) {
#line 4550 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4551 "src/taumir/lower.tr"
        long long a1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4552 "src/taumir/lower.tr"
        if (((a1 < 0LL) || (LFunc_vreg_type(lf, a1) != 1LL))) {
#line 4552 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4553 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_replace", 18LL));
#line 4554 "src/taumir/lower.tr"
        List_i64* ra = (void*)List_i64_new();
#line 4555 "src/taumir/lower.tr"
        List_i64_append(ra, _tr_v_recv);
#line 4556 "src/taumir/lower.tr"
        List_i64_append(ra, a0);
#line 4557 "src/taumir/lower.tr"
        List_i64_append(ra, a1);
#line 4558 "src/taumir/lower.tr"
        long long rd = LFunc_new_vreg(lf);
#line 4559 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit_len("_tr_rt_str_replace", 18LL), ra));
#line 4560 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, rd, 1LL);
#line 4561 "src/taumir/lower.tr"
        _fresh_mark(lf, rd);
#line 4562 "src/taumir/lower.tr"
        return rd;
    }
#line 4563 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("find", 4LL))) && (margs->len == 1LL))) {
#line 4564 "src/taumir/lower.tr"
        long long fa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4565 "src/taumir/lower.tr"
        if (((fa < 0LL) || (LFunc_vreg_type(lf, fa) != 1LL))) {
#line 4565 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4566 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), _tr_v_recv, fa, 0LL);
    }
#line 4567 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("starts_with", 11LL))) && (margs->len == 1LL))) {
#line 4568 "src/taumir/lower.tr"
        long long sa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4569 "src/taumir/lower.tr"
        if (((sa < 0LL) || (LFunc_vreg_type(lf, sa) != 1LL))) {
#line 4569 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4570 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_starts_with", 22LL), _tr_v_recv, sa, 4LL);
    }
#line 4571 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("ends_with", 9LL))) && (margs->len == 1LL))) {
#line 4572 "src/taumir/lower.tr"
        long long ea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4573 "src/taumir/lower.tr"
        if (((ea < 0LL) || (LFunc_vreg_type(lf, ea) != 1LL))) {
#line 4573 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4574 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_ends_with", 20LL), _tr_v_recv, ea, 4LL);
    }
#line 4575 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) && (margs->len == 1LL))) {
#line 4576 "src/taumir/lower.tr"
        long long ka = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4577 "src/taumir/lower.tr"
        if (((ka < 0LL) || (LFunc_vreg_type(lf, ka) != 1LL))) {
#line 4577 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4578 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_count", 16LL), _tr_v_recv, ka, 0LL);
    }
#line 4579 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) && (margs->len == 1LL))) {
#line 4580 "src/taumir/lower.tr"
        long long na = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4581 "src/taumir/lower.tr"
        if (((na < 0LL) || (LFunc_vreg_type(lf, na) != 1LL))) {
#line 4581 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4582 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_contains", 19LL), _tr_v_recv, na, 4LL);
    }
#line 4583 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("char_at", 7LL))) && (margs->len == 1LL))) {
#line 4584 "src/taumir/lower.tr"
        long long ia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4585 "src/taumir/lower.tr"
        if (((ia < 0LL) || (LFunc_vreg_type(lf, ia) != 0LL))) {
#line 4585 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4586 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_char_at", 18LL), _tr_v_recv, ia, 0LL);
    }
#line 4587 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("repeat", 6LL))) && (margs->len == 1LL))) {
#line 4588 "src/taumir/lower.tr"
        long long pa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4589 "src/taumir/lower.tr"
        if (((pa < 0LL) || (LFunc_vreg_type(lf, pa) != 0LL))) {
#line 4589 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4590 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_repeat", 17LL), _tr_v_recv, pa, 1LL);
    }
#line 4591 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))) && (margs->len == 2LL))) {
#line 4592 "src/taumir/lower.tr"
        long long s0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4593 "src/taumir/lower.tr"
        if (((s0 < 0LL) || (LFunc_vreg_type(lf, s0) != 0LL))) {
#line 4593 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4594 "src/taumir/lower.tr"
        long long s1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4595 "src/taumir/lower.tr"
        if (((s1 < 0LL) || (LFunc_vreg_type(lf, s1) != 0LL))) {
#line 4595 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4596 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
#line 4597 "src/taumir/lower.tr"
        List_i64* sla = (void*)List_i64_new();
#line 4598 "src/taumir/lower.tr"
        List_i64_append(sla, _tr_v_recv);
#line 4599 "src/taumir/lower.tr"
        List_i64_append(sla, s0);
#line 4600 "src/taumir/lower.tr"
        List_i64_append(sla, s1);
#line 4601 "src/taumir/lower.tr"
        long long sld = LFunc_new_vreg(lf);
#line 4602 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_str_slice", 16LL), sla));
#line 4603 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sld, 1LL);
#line 4604 "src/taumir/lower.tr"
        _fresh_mark(lf, sld);
#line 4605 "src/taumir/lower.tr"
        return sld;
    }
#line 4607 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("capitalize", 10LL))) && (margs->len == 0LL))) {
#line 4608 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_capitalize", 21LL), _tr_v_recv, 1LL);
    }
#line 4609 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("title", 5LL))) && (margs->len == 0LL))) {
#line 4610 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_title", 16LL), _tr_v_recv, 1LL);
    }
#line 4611 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("trim_left", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("lstrip", 6LL)))) && (margs->len == 0LL))) {
#line 4612 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_trim_left", 20LL), _tr_v_recv, 1LL);
    }
#line 4613 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("trim_right", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("rstrip", 6LL)))) && (margs->len == 0LL))) {
#line 4614 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_trim_right", 21LL), _tr_v_recv, 1LL);
    }
#line 4615 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("zfill", 5LL))) && (margs->len == 1LL))) {
#line 4616 "src/taumir/lower.tr"
        long long zfa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4617 "src/taumir/lower.tr"
        if (((zfa < 0LL) || (LFunc_vreg_type(lf, zfa) != 0LL))) {
#line 4617 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4618 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_zfill", 16LL), _tr_v_recv, zfa, 1LL);
    }
#line 4619 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("pad_left", 8LL))) && (margs->len == 1LL))) {
#line 4620 "src/taumir/lower.tr"
        long long pla = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4621 "src/taumir/lower.tr"
        if (((pla < 0LL) || (LFunc_vreg_type(lf, pla) != 0LL))) {
#line 4621 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4622 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_pad_left", 19LL), _tr_v_recv, pla, 1LL);
    }
#line 4623 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("pad_right", 9LL))) && (margs->len == 1LL))) {
#line 4624 "src/taumir/lower.tr"
        long long pra = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4625 "src/taumir/lower.tr"
        if (((pra < 0LL) || (LFunc_vreg_type(lf, pra) != 0LL))) {
#line 4625 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4626 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_pad_right", 20LL), _tr_v_recv, pra, 1LL);
    }
#line 4627 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains_char", 13LL))) && (margs->len == 1LL))) {
#line 4628 "src/taumir/lower.tr"
        long long cca = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4629 "src/taumir/lower.tr"
        if (((cca < 0LL) || (LFunc_vreg_type(lf, cca) != 0LL))) {
#line 4629 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4630 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_contains_char", 24LL), _tr_v_recv, cca, 4LL);
    }
#line 4631 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("center", 6LL))) && (margs->len == 1LL))) {
#line 4632 "src/taumir/lower.tr"
        long long cea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4633 "src/taumir/lower.tr"
        if (((cea < 0LL) || (LFunc_vreg_type(lf, cea) != 0LL))) {
#line 4633 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4634 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_center", 17LL), _tr_v_recv, cea, 1LL);
    }
#line 4635 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("chars", 5LL))) && (margs->len == 0LL))) {
#line 4636 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_chars", 16LL));
#line 4637 "src/taumir/lower.tr"
        List_i64* cha = (void*)List_i64_new();
#line 4638 "src/taumir/lower.tr"
        List_i64_append(cha, _tr_v_recv);
#line 4639 "src/taumir/lower.tr"
        long long chd = LFunc_new_vreg(lf);
#line 4640 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(chd, _tr_str_lit_len("_tr_rt_str_chars", 16LL), cha));
#line 4641 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, chd, 3LL);
#line 4642 "src/taumir/lower.tr"
        return chd;
    }
#line 4643 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("split", 5LL))) && (margs->len == 1LL))) {
#line 4644 "src/taumir/lower.tr"
        long long spa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4645 "src/taumir/lower.tr"
        if (((spa < 0LL) || (LFunc_vreg_type(lf, spa) != 1LL))) {
#line 4645 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4646 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_split", 16LL));
#line 4647 "src/taumir/lower.tr"
        List_i64* spargs = (void*)List_i64_new();
#line 4648 "src/taumir/lower.tr"
        List_i64_append(spargs, _tr_v_recv);
#line 4649 "src/taumir/lower.tr"
        List_i64_append(spargs, spa);
#line 4650 "src/taumir/lower.tr"
        long long spd = LFunc_new_vreg(lf);
#line 4651 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(spd, _tr_str_lit_len("_tr_rt_str_split", 16LL), spargs));
#line 4652 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, spd, 3LL);
#line 4653 "src/taumir/lower.tr"
        return spd;
    }
#line 4654 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))) && (margs->len == 0LL))) {
#line 4655 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_reverse", 18LL), _tr_v_recv, 1LL);
    }
#line 4656 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))) && (margs->len == 0LL))) {
#line 4657 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_is_empty", 19LL), _tr_v_recv, 4LL);
    }
#line 4658 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("parse_bool", 10LL))) && (margs->len == 0LL))) {
#line 4659 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_parse_bool", 21LL), _tr_v_recv, 4LL);
    }
#line 4660 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("parse_int", 9LL))) && (margs->len == 0LL))) {
#line 4661 "src/taumir/lower.tr"
        return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), _tr_v_recv, 0LL);
    }
#line 4663 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) && (margs->len == 1LL))) {
#line 4664 "src/taumir/lower.tr"
        long long ida = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4665 "src/taumir/lower.tr"
        if (((ida < 0LL) || (LFunc_vreg_type(lf, ida) != 1LL))) {
#line 4665 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4666 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), _tr_v_recv, ida, 0LL);
    }
#line 4667 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("last_index_of", 13LL))) && (margs->len == 1LL))) {
#line 4668 "src/taumir/lower.tr"
        long long lia = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4669 "src/taumir/lower.tr"
        if (((lia < 0LL) || (LFunc_vreg_type(lf, lia) != 1LL))) {
#line 4669 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4670 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_last_index_of", 24LL), _tr_v_recv, lia, 0LL);
    }
#line 4671 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("strip_prefix", 12LL))) && (margs->len == 1LL))) {
#line 4672 "src/taumir/lower.tr"
        long long ppa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4673 "src/taumir/lower.tr"
        if (((ppa < 0LL) || (LFunc_vreg_type(lf, ppa) != 1LL))) {
#line 4673 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4674 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_strip_prefix", 23LL), _tr_v_recv, ppa, 1LL);
    }
#line 4675 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("strip_suffix", 12LL))) && (margs->len == 1LL))) {
#line 4676 "src/taumir/lower.tr"
        long long ssa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4677 "src/taumir/lower.tr"
        if (((ssa < 0LL) || (LFunc_vreg_type(lf, ssa) != 1LL))) {
#line 4677 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4678 "src/taumir/lower.tr"
        return _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_strip_suffix", 23LL), _tr_v_recv, ssa, 1LL);
    }
#line 4679 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("replace_first", 13LL))) && (margs->len == 2LL))) {
#line 4680 "src/taumir/lower.tr"
        long long rf0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4681 "src/taumir/lower.tr"
        if (((rf0 < 0LL) || (LFunc_vreg_type(lf, rf0) != 1LL))) {
#line 4681 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4682 "src/taumir/lower.tr"
        long long rf1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4683 "src/taumir/lower.tr"
        if (((rf1 < 0LL) || (LFunc_vreg_type(lf, rf1) != 1LL))) {
#line 4683 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4684 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_replace_first", 24LL));
#line 4685 "src/taumir/lower.tr"
        List_i64* rfa = (void*)List_i64_new();
#line 4686 "src/taumir/lower.tr"
        List_i64_append(rfa, _tr_v_recv);
#line 4687 "src/taumir/lower.tr"
        List_i64_append(rfa, rf0);
#line 4688 "src/taumir/lower.tr"
        List_i64_append(rfa, rf1);
#line 4689 "src/taumir/lower.tr"
        long long rfd = LFunc_new_vreg(lf);
#line 4690 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(rfd, _tr_str_lit_len("_tr_rt_str_replace_first", 24LL), rfa));
#line 4691 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, rfd, 1LL);
#line 4692 "src/taumir/lower.tr"
        _fresh_mark(lf, rfd);
#line 4693 "src/taumir/lower.tr"
        return rfd;
    }
#line 4694 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) TrStr _float_unary_sym(TrStr method) {
#line 4698 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("sqrt", 4LL)))) {
#line 4698 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_sqrt", 11LL);
    }
#line 4699 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("floor", 5LL)))) {
#line 4699 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_floor", 12LL);
    }
#line 4700 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("ceil", 4LL)))) {
#line 4700 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_ceil", 11LL);
    }
#line 4701 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("round", 5LL)))) {
#line 4701 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_round", 12LL);
    }
#line 4702 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("abs", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("fabs", 4LL))))) {
#line 4702 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_fabs", 11LL);
    }
#line 4703 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("log", 3LL)))) {
#line 4703 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_log", 10LL);
    }
#line 4704 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("log2", 4LL)))) {
#line 4704 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_log2", 11LL);
    }
#line 4705 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("log10", 5LL)))) {
#line 4705 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_log10", 12LL);
    }
#line 4706 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("exp", 3LL)))) {
#line 4706 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_exp", 10LL);
    }
#line 4707 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("sin", 3LL)))) {
#line 4707 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_sin", 10LL);
    }
#line 4708 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("cos", 3LL)))) {
#line 4708 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_cos", 10LL);
    }
#line 4709 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("tan", 3LL)))) {
#line 4709 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_tan", 10LL);
    }
#line 4710 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("asin", 4LL)))) {
#line 4710 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_asin", 11LL);
    }
#line 4711 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("acos", 4LL)))) {
#line 4711 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_acos", 11LL);
    }
#line 4712 "src/taumir/lower.tr"
    if (_tr_str_eqv((method), (_tr_str_lit_len("atan", 4LL)))) {
#line 4712 "src/taumir/lower.tr"
        return _tr_str_lit_len("_tr_rt_atan", 11LL);
    }
#line 4713 "src/taumir/lower.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _lower_int_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
#line 4717 "src/taumir/lower.tr"
    if ((margs->len == 0LL)) {
#line 4718 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_float", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_f64", 6LL))))) {
#line 4718 "src/taumir/lower.tr"
            return _promote_f(lf, _tr_v_recv);
        }
#line 4719 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("to_hex", 6LL)))) {
#line 4719 "src/taumir/lower.tr"
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_hex", 17LL), _tr_v_recv, 1LL);
        }
#line 4720 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_hex_upper", 12LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_HEX", 6LL))))) {
#line 4720 "src/taumir/lower.tr"
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_hex_upper", 23LL), _tr_v_recv, 1LL);
        }
#line 4721 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_oct", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_octal", 8LL))))) {
#line 4721 "src/taumir/lower.tr"
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_oct", 17LL), _tr_v_recv, 1LL);
        }
#line 4722 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_bin", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_binary", 9LL))))) {
#line 4722 "src/taumir/lower.tr"
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_bin", 17LL), _tr_v_recv, 1LL);
        }
#line 4723 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
#line 4723 "src/taumir/lower.tr"
            return _str_call0(m, lf, _tr_str_lit_len("_tr_rt_i64_to_str", 17LL), _tr_v_recv, 1LL);
        }
#line 4724 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("sign", 4LL)))) {
#line 4725 "src/taumir/lower.tr"
            long long z = LFunc_new_vreg(lf);
#line 4726 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
#line 4727 "src/taumir/lower.tr"
            long long gt = LFunc_new_vreg(lf);
#line 4728 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(gt, _tr_str_lit_len(">", 1LL), _tr_v_recv, z));
#line 4729 "src/taumir/lower.tr"
            long long lt = LFunc_new_vreg(lf);
#line 4730 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(lt, _tr_str_lit_len("<", 1LL), _tr_v_recv, z));
#line 4731 "src/taumir/lower.tr"
            long long sd = LFunc_new_vreg(lf);
#line 4732 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(sd, _tr_str_lit_len("-", 1LL), gt, lt));
#line 4733 "src/taumir/lower.tr"
            return sd;
        }
#line 4734 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 4735 "src/taumir/lower.tr"
    if (((margs->len == 1LL) && _tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))))) {
#line 4736 "src/taumir/lower.tr"
        long long pe = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4737 "src/taumir/lower.tr"
        if (((pe < 0LL) || (LFunc_vreg_type(lf, pe) != 0LL))) {
#line 4737 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4738 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_int_pow", 14LL));
#line 4739 "src/taumir/lower.tr"
        List_i64* ppa = (void*)List_i64_new();
#line 4740 "src/taumir/lower.tr"
        List_i64_append(ppa, _tr_v_recv);
#line 4741 "src/taumir/lower.tr"
        List_i64_append(ppa, pe);
#line 4742 "src/taumir/lower.tr"
        long long ppd = LFunc_new_vreg(lf);
#line 4743 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(ppd, _tr_str_lit_len("_tr_rt_int_pow", 14LL), ppa));
#line 4744 "src/taumir/lower.tr"
        return ppd;
    }
#line 4745 "src/taumir/lower.tr"
    if (((margs->len == 1LL) && (_tr_str_eqv((method), (_tr_str_lit_len("gcd", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))))) {
#line 4746 "src/taumir/lower.tr"
        long long y = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4747 "src/taumir/lower.tr"
        if (((y < 0LL) || (LFunc_vreg_type(lf, y) != 0LL))) {
#line 4747 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4748 "src/taumir/lower.tr"
        TrStr gsym = _tr_str_lit_len("_tr_rt_gcd_i64", 14LL);
#line 4749 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("lcm", 3LL)))) {
#line 4749 "src/taumir/lower.tr"
            TrStr _strtmp_t6366 = _tr_str_lit_len("_tr_rt_lcm_i64", 14LL);
            _tr_str_release(gsym);
            gsym = _strtmp_t6366;
        }
#line 4750 "src/taumir/lower.tr"
        LModule_add_extern(m, gsym);
#line 4751 "src/taumir/lower.tr"
        List_i64* glargs = (void*)List_i64_new();
#line 4752 "src/taumir/lower.tr"
        List_i64_append(glargs, _tr_v_recv);
#line 4753 "src/taumir/lower.tr"
        List_i64_append(glargs, y);
#line 4754 "src/taumir/lower.tr"
        long long gd = LFunc_new_vreg(lf);
#line 4755 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(gd, gsym, glargs));
#line 4756 "src/taumir/lower.tr"
        long long _trret1609 = gd;
        _tr_str_release(gsym);
        return _trret1609;
    }
#line 4757 "src/taumir/lower.tr"
    if (((margs->len == 2LL) && _tr_str_eqv((method), (_tr_str_lit_len("clamp", 5LL))))) {
#line 4758 "src/taumir/lower.tr"
        long long lo = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4759 "src/taumir/lower.tr"
        if (((lo < 0LL) || (LFunc_vreg_type(lf, lo) != 0LL))) {
#line 4759 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4760 "src/taumir/lower.tr"
        long long hi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4761 "src/taumir/lower.tr"
        if (((hi < 0LL) || (LFunc_vreg_type(lf, hi) != 0LL))) {
#line 4761 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4762 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_clamp_i64", 16LL));
#line 4763 "src/taumir/lower.tr"
        List_i64* ca = (void*)List_i64_new();
#line 4764 "src/taumir/lower.tr"
        List_i64_append(ca, _tr_v_recv);
#line 4765 "src/taumir/lower.tr"
        List_i64_append(ca, lo);
#line 4766 "src/taumir/lower.tr"
        List_i64_append(ca, hi);
#line 4767 "src/taumir/lower.tr"
        long long cd = LFunc_new_vreg(lf);
#line 4768 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(cd, _tr_str_lit_len("_tr_rt_clamp_i64", 16LL), ca));
#line 4769 "src/taumir/lower.tr"
        return cd;
    }
#line 4770 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) long long _lower_dict_method(LModule* m, LFunc* lf, long long _tr_v_recv, long long dtag, TrStr method, List_ptr* margs) {
#line 4774 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
#line 4775 "src/taumir/lower.tr"
        TrStr lsym = _dict_sym(dtag, _tr_str_lit_len("len", 3LL));
#line 4776 "src/taumir/lower.tr"
        LModule_add_extern(m, lsym);
#line 4777 "src/taumir/lower.tr"
        List_i64* la = (void*)List_i64_new();
#line 4778 "src/taumir/lower.tr"
        List_i64_append(la, _tr_v_recv);
#line 4779 "src/taumir/lower.tr"
        long long ld = LFunc_new_vreg(lf);
#line 4780 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(ld, lsym, la));
#line 4781 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, ld, 0LL);
#line 4782 "src/taumir/lower.tr"
        long long _trret1610 = ld;
        _tr_str_release(lsym);
        return _trret1610;
    }
#line 4783 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))) && (margs->len == 1LL))) {
#line 4784 "src/taumir/lower.tr"
        long long rk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4785 "src/taumir/lower.tr"
        if ((rk < 0LL)) {
#line 4785 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4786 "src/taumir/lower.tr"
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, rk) != 1LL))) {
#line 4786 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4787 "src/taumir/lower.tr"
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, rk) != 0LL))) {
#line 4787 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4788 "src/taumir/lower.tr"
        TrStr rsym = _dict_sym(dtag, _tr_str_lit_len("remove", 6LL));
#line 4789 "src/taumir/lower.tr"
        LModule_add_extern(m, rsym);
#line 4790 "src/taumir/lower.tr"
        List_i64* ra = (void*)List_i64_new();
#line 4791 "src/taumir/lower.tr"
        List_i64_append(ra, _tr_v_recv);
#line 4792 "src/taumir/lower.tr"
        List_i64_append(ra, rk);
#line 4793 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), rsym, ra));
#line 4794 "src/taumir/lower.tr"
        long long _trret1611 = _tr_v_recv;
        _tr_str_release(rsym);
        return _trret1611;
    }
#line 4795 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("keys", 4LL))) && (margs->len == 0LL))) {
#line 4796 "src/taumir/lower.tr"
        TrStr ksym = _tr_str_lit_len("_tr_rt_idict_keys", 17LL);
#line 4797 "src/taumir/lower.tr"
        long long ktag = 2LL;
#line 4798 "src/taumir/lower.tr"
        if (_dict_key_is_str(dtag)) {
#line 4799 "src/taumir/lower.tr"
            TrStr _strtmp_t6367 = _tr_str_lit_len("_tr_rt_sdict_keys", 17LL);
            _tr_str_release(ksym);
            ksym = _strtmp_t6367;
#line 4800 "src/taumir/lower.tr"
            ktag = 3LL;
        }
#line 4801 "src/taumir/lower.tr"
        LModule_add_extern(m, ksym);
#line 4802 "src/taumir/lower.tr"
        List_i64* ka = (void*)List_i64_new();
#line 4803 "src/taumir/lower.tr"
        List_i64_append(ka, _tr_v_recv);
#line 4804 "src/taumir/lower.tr"
        long long kd = LFunc_new_vreg(lf);
#line 4805 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(kd, ksym, ka));
#line 4806 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, kd, ktag);
#line 4807 "src/taumir/lower.tr"
        long long _trret1612 = kd;
        _tr_str_release(ksym);
        return _trret1612;
    }
#line 4808 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("get_or", 6LL))) && (margs->len == 2LL))) {
#line 4809 "src/taumir/lower.tr"
        if ((_dict_val_tag(dtag) != 0LL)) {
#line 4809 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4810 "src/taumir/lower.tr"
        long long gk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4811 "src/taumir/lower.tr"
        if ((gk < 0LL)) {
#line 4811 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4812 "src/taumir/lower.tr"
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, gk) != 1LL))) {
#line 4812 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4813 "src/taumir/lower.tr"
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, gk) != 0LL))) {
#line 4813 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4814 "src/taumir/lower.tr"
        long long gdef = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4815 "src/taumir/lower.tr"
        if (((gdef < 0LL) || (LFunc_vreg_type(lf, gdef) != 0LL))) {
#line 4815 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4816 "src/taumir/lower.tr"
        TrStr gosym = _dict_sym(dtag, _tr_str_lit_len("get_or", 6LL));
#line 4817 "src/taumir/lower.tr"
        LModule_add_extern(m, gosym);
#line 4818 "src/taumir/lower.tr"
        List_i64* goa = (void*)List_i64_new();
#line 4819 "src/taumir/lower.tr"
        List_i64_append(goa, _tr_v_recv);
#line 4820 "src/taumir/lower.tr"
        List_i64_append(goa, gk);
#line 4821 "src/taumir/lower.tr"
        List_i64_append(goa, gdef);
#line 4822 "src/taumir/lower.tr"
        long long godd = LFunc_new_vreg(lf);
#line 4823 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(godd, gosym, goa));
#line 4824 "src/taumir/lower.tr"
        long long _trret1613 = godd;
        _tr_str_release(gosym);
        return _trret1613;
    }
#line 4825 "src/taumir/lower.tr"
    if (((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) && (margs->len == 2LL))) {
#line 4826 "src/taumir/lower.tr"
        long long sk = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4827 "src/taumir/lower.tr"
        if ((sk < 0LL)) {
#line 4827 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4828 "src/taumir/lower.tr"
        if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, sk) != 1LL))) {
#line 4828 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4829 "src/taumir/lower.tr"
        if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, sk) != 0LL))) {
#line 4829 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4830 "src/taumir/lower.tr"
        long long sv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 4831 "src/taumir/lower.tr"
        if ((sv < 0LL)) {
#line 4831 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4832 "src/taumir/lower.tr"
        long long vtag = _dict_val_tag(dtag);
#line 4833 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, sv) != vtag)) {
#line 4833 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4834 "src/taumir/lower.tr"
        if ((vtag == 1LL)) {
#line 4834 "src/taumir/lower.tr"
            _secure_str(m, lf, sv);
        }
#line 4835 "src/taumir/lower.tr"
        if (((vtag == 10LL) || (vtag == 11LL))) {
#line 4835 "src/taumir/lower.tr"
            _secure_obj(m, lf, sv);
        }
#line 4836 "src/taumir/lower.tr"
        if ((vtag == 5LL)) {
#line 4837 "src/taumir/lower.tr"
            long long svfb = LFunc_new_vreg(lf);
#line 4838 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFBits(svfb, sv));
#line 4839 "src/taumir/lower.tr"
            sv = svfb;
        }
#line 4840 "src/taumir/lower.tr"
        ({ TrStr _at_t6368 = (_dict_sym(dtag, _tr_str_lit_len("set", 3LL))); LModule_add_extern(m, _at_t6368); _tr_str_release(_at_t6368); });
#line 4841 "src/taumir/lower.tr"
        List_i64* ssa = (void*)List_i64_new();
#line 4842 "src/taumir/lower.tr"
        List_i64_append(ssa, _tr_v_recv);
#line 4843 "src/taumir/lower.tr"
        List_i64_append(ssa, sk);
#line 4844 "src/taumir/lower.tr"
        List_i64_append(ssa, sv);
#line 4845 "src/taumir/lower.tr"
        ({ TrStr _at_t6369 = (_dict_sym(dtag, _tr_str_lit_len("set", 3LL))); LFunc_emit(lf, LInst_ctor_ICall((-1LL), _at_t6369, ssa)); _tr_str_release(_at_t6369); });
#line 4846 "src/taumir/lower.tr"
        return _tr_v_recv;
    }
#line 4847 "src/taumir/lower.tr"
    if ((margs->len != 1LL)) {
#line 4847 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 4848 "src/taumir/lower.tr"
    long long kv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4849 "src/taumir/lower.tr"
    if ((kv < 0LL)) {
#line 4849 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 4850 "src/taumir/lower.tr"
    if ((_dict_key_is_str(dtag) && (LFunc_vreg_type(lf, kv) != 1LL))) {
#line 4850 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 4851 "src/taumir/lower.tr"
    if (((!_dict_key_is_str(dtag)) && (LFunc_vreg_type(lf, kv) != 0LL))) {
#line 4851 "src/taumir/lower.tr"
        return (-1LL);
    }
#line 4852 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))))) {
#line 4853 "src/taumir/lower.tr"
        TrStr gsym = _dict_sym(dtag, _tr_str_lit_len("get", 3LL));
#line 4854 "src/taumir/lower.tr"
        LModule_add_extern(m, gsym);
#line 4855 "src/taumir/lower.tr"
        List_i64* getargs = (void*)List_i64_new();
#line 4856 "src/taumir/lower.tr"
        List_i64_append(getargs, _tr_v_recv);
#line 4857 "src/taumir/lower.tr"
        List_i64_append(getargs, kv);
#line 4858 "src/taumir/lower.tr"
        long long gd = LFunc_new_vreg(lf);
#line 4859 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(gd, gsym, getargs));
#line 4860 "src/taumir/lower.tr"
        if ((_dict_val_tag(dtag) == 5LL)) {
#line 4861 "src/taumir/lower.tr"
            long long gdf = LFunc_new_vreg(lf);
#line 4862 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBitsF(gdf, gd));
#line 4863 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, gdf, 5LL);
#line 4864 "src/taumir/lower.tr"
            long long _trret1614 = gdf;
            _tr_str_release(gsym);
            return _trret1614;
        }
#line 4865 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, gd, _dict_val_tag(dtag));
#line 4866 "src/taumir/lower.tr"
        if (((_dict_val_tag(dtag) == 10LL) || (_dict_val_tag(dtag) == 11LL))) {
#line 4871 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_retain", 17LL));
#line 4872 "src/taumir/lower.tr"
            List_i64* _gra = (void*)List_i64_new();
#line 4873 "src/taumir/lower.tr"
            List_i64_append(_gra, gd);
#line 4874 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_obj_retain", 17LL), _gra));
#line 4875 "src/taumir/lower.tr"
            _fresh_mark_obj(lf, gd);
        }
#line 4876 "src/taumir/lower.tr"
        long long _trret1615 = gd;
        _tr_str_release(gsym);
        return _trret1615;
    }
#line 4877 "src/taumir/lower.tr"
    if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("has", 3LL))))) {
#line 4878 "src/taumir/lower.tr"
        TrStr hsym = _dict_sym(dtag, _tr_str_lit_len("has", 3LL));
#line 4879 "src/taumir/lower.tr"
        LModule_add_extern(m, hsym);
#line 4880 "src/taumir/lower.tr"
        List_i64* ha = (void*)List_i64_new();
#line 4881 "src/taumir/lower.tr"
        List_i64_append(ha, _tr_v_recv);
#line 4882 "src/taumir/lower.tr"
        List_i64_append(ha, kv);
#line 4883 "src/taumir/lower.tr"
        long long hd = LFunc_new_vreg(lf);
#line 4884 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(hd, hsym, ha));
#line 4885 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, hd, 4LL);
#line 4886 "src/taumir/lower.tr"
        long long _trret1616 = hd;
        _tr_str_release(hsym);
        return _trret1616;
    }
#line 4887 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) long long _lower_float_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
#line 4892 "src/taumir/lower.tr"
    if ((margs->len == 0LL)) {
#line 4893 "src/taumir/lower.tr"
        TrStr usym = _float_unary_sym(method);
#line 4894 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((usym), (_tr_str_lit_len("", 0LL))))) {
#line 4895 "src/taumir/lower.tr"
            LModule_add_extern(m, usym);
#line 4896 "src/taumir/lower.tr"
            long long d = LFunc_new_vreg(lf);
#line 4897 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCallF(d, usym, _tr_v_recv));
#line 4898 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, d, 5LL);
#line 4899 "src/taumir/lower.tr"
            long long _trret1617 = d;
            _tr_str_release(usym);
            return _trret1617;
        }
#line 4900 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("is_nan", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL))))) {
#line 4901 "src/taumir/lower.tr"
            TrStr nsym = _tr_str_lit_len("_tr_rt_f64_is_nan", 17LL);
#line 4902 "src/taumir/lower.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("is_inf", 6LL)))) {
#line 4902 "src/taumir/lower.tr"
                TrStr _strtmp_t6370 = _tr_str_lit_len("_tr_rt_f64_is_inf", 17LL);
                _tr_str_release(nsym);
                nsym = _strtmp_t6370;
            }
#line 4903 "src/taumir/lower.tr"
            LModule_add_extern(m, nsym);
#line 4904 "src/taumir/lower.tr"
            long long nd = LFunc_new_vreg(lf);
#line 4905 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCall1(nd, nsym, _tr_v_recv));
#line 4906 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, nd, 4LL);
#line 4907 "src/taumir/lower.tr"
            long long _trret1618 = nd;
            _tr_str_release(usym);
            _tr_str_release(nsym);
            return _trret1618;
        }
#line 4908 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
#line 4909 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
#line 4910 "src/taumir/lower.tr"
            long long fsd = LFunc_new_vreg(lf);
#line 4911 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCall1(fsd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), _tr_v_recv));
#line 4912 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, fsd, 1LL);
#line 4913 "src/taumir/lower.tr"
            _fresh_mark(lf, fsd);
#line 4914 "src/taumir/lower.tr"
            long long _trret1619 = fsd;
            _tr_str_release(usym);
            return _trret1619;
        }
#line 4915 "src/taumir/lower.tr"
        _tr_str_release(usym);
        return (-1LL);
    }
#line 4916 "src/taumir/lower.tr"
    if (((margs->len == 1LL) && (_tr_str_eqv((method), (_tr_str_lit_len("pow", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))))) {
#line 4917 "src/taumir/lower.tr"
        long long arg = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 4918 "src/taumir/lower.tr"
        if ((arg < 0LL)) {
#line 4918 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4919 "src/taumir/lower.tr"
        long long argt = LFunc_vreg_type(lf, arg);
#line 4920 "src/taumir/lower.tr"
        if ((argt == 0LL)) {
#line 4920 "src/taumir/lower.tr"
            arg = _promote_f(lf, arg);
        } else if ((argt != 5LL)) {
#line 4921 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 4922 "src/taumir/lower.tr"
        TrStr psym = _tr_str_lit_len("_tr_rt_pow", 10LL);
#line 4923 "src/taumir/lower.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("atan2", 5LL)))) {
#line 4923 "src/taumir/lower.tr"
            TrStr _strtmp_t6371 = _tr_str_lit_len("_tr_rt_atan2", 12LL);
            _tr_str_release(psym);
            psym = _strtmp_t6371;
        }
#line 4924 "src/taumir/lower.tr"
        LModule_add_extern(m, psym);
#line 4925 "src/taumir/lower.tr"
        long long pd = LFunc_new_vreg(lf);
#line 4926 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IFCall2F(pd, psym, _tr_v_recv, arg));
#line 4927 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, pd, 5LL);
#line 4928 "src/taumir/lower.tr"
        long long _trret1620 = pd;
        _tr_str_release(psym);
        return _trret1620;
    }
#line 4929 "src/taumir/lower.tr"
    return (-1LL);
}

__attribute__((hot)) bool _is_const_int(HirExpr* e) {
#line 4933 "src/taumir/lower.tr"
    __auto_type _t6372 = (*e);
    if (_t6372.tag == HirExpr_ELitInt) {
        return true;
    } else if (_t6372.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t6372.data.EUnaryOp.op;
__auto_type sub = _t6372.data.EUnaryOp.expr;
#line 4936 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 4937 "src/taumir/lower.tr"
            __auto_type _t6373 = (*sub);
            if (_t6373.tag == HirExpr_ELitInt) {
                return true;
            } else if (1) {
                __auto_type _ = _t6373;
                return false;
            }
        }
#line 4940 "src/taumir/lower.tr"
        return false;
    } else if (1) {
        __auto_type _ = _t6372;
        return false;
    }
}

__attribute__((hot)) long long _const_int_val(HirExpr* e) {
#line 4944 "src/taumir/lower.tr"
    __auto_type _t6374 = (*e);
    if (_t6374.tag == HirExpr_ELitInt) {
        __auto_type v = _t6374.data.ELitInt.val;
        return v;
    } else if (_t6374.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t6374.data.EUnaryOp.op;
__auto_type sub = _t6374.data.EUnaryOp.expr;
#line 4947 "src/taumir/lower.tr"
        __auto_type _t6375 = (*sub);
        if (_t6375.tag == HirExpr_ELitInt) {
            __auto_type v2 = _t6375.data.ELitInt.val;
            long long _trret1621 = (0LL - v2);
            return _trret1621;
        } else if (1) {
            __auto_type _ = _t6375;
            return 0LL;
        }
    } else if (1) {
        __auto_type _ = _t6374;
        return 0LL;
    }
}

__attribute__((hot)) void _emit_add_const(LFunc* lf, TrStr name, long long delta) {
#line 4954 "src/taumir/lower.tr"
    long long cur = LFunc_new_vreg(lf);
#line 4955 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
#line 4956 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4957 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(d, delta));
#line 4958 "src/taumir/lower.tr"
    long long inc = LFunc_new_vreg(lf);
#line 4959 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit_len("+", 1LL), cur, d));
#line 4960 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) long long _list_call1(LModule* m, LFunc* lf, TrStr sym, long long handle, long long restype) {
#line 4964 "src/taumir/lower.tr"
    LModule_add_extern(m, sym);
#line 4965 "src/taumir/lower.tr"
    List_i64* a = (void*)List_i64_new();
#line 4966 "src/taumir/lower.tr"
    List_i64_append(a, handle);
#line 4967 "src/taumir/lower.tr"
    long long d = LFunc_new_vreg(lf);
#line 4968 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
#line 4969 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, d, restype);
#line 4970 "src/taumir/lower.tr"
    return d;
}

__attribute__((hot)) long long _inline_list_addr(LModule* m, LFunc* lf, long long handle, long long idx, long long stride) {
#line 4978 "src/taumir/lower.tr"
    long long data = LFunc_new_vreg(lf);
#line 4979 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, data, 10LL);
#line 4980 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoad(data, handle, 0LL, 1LL));
#line 4985 "src/taumir/lower.tr"
    long long off = idx;
#line 4986 "src/taumir/lower.tr"
    if ((stride != 1LL)) {
#line 4987 "src/taumir/lower.tr"
        long long c8 = LFunc_new_vreg(lf);
#line 4988 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(c8, stride));
#line 4989 "src/taumir/lower.tr"
        off = LFunc_new_vreg(lf);
#line 4990 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(off, _tr_str_lit_len("*", 1LL), idx, c8));
    }
#line 4991 "src/taumir/lower.tr"
    long long addr = LFunc_new_vreg(lf);
#line 4992 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, addr, 10LL);
#line 4998 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(addr, _tr_str_lit_len("+", 1LL), data, off));
#line 4999 "src/taumir/lower.tr"
    return addr;
}

__attribute__((hot)) long long _fixed_arr_len(LModule* m, LFunc* lf, TrStr name) {
#line 5009 "src/taumir/lower.tr"
    if ((LFunc_var_index(lf, name) >= 0LL)) {
#line 5009 "src/taumir/lower.tr"
        return LFunc_var_arr_of(lf, name);
    }
#line 5010 "src/taumir/lower.tr"
    return LModule_global_arr_of(m, name);
}

__attribute__((hot)) long long _fixed_arr_elem_tag(LModule* m, LFunc* lf, TrStr name) {
#line 5014 "src/taumir/lower.tr"
    if ((LFunc_var_index(lf, name) >= 0LL)) {
#line 5014 "src/taumir/lower.tr"
        return LFunc_var_type(lf, name);
    }
#line 5015 "src/taumir/lower.tr"
    return LModule_global_type(m, name);
}

__attribute__((hot)) long long _array_elem_addr(LModule* m, LFunc* lf, TrStr name, long long idx) {
#line 5018 "src/taumir/lower.tr"
    long long base = LFunc_new_vreg(lf);
#line 5019 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, base, 10LL);
#line 5020 "src/taumir/lower.tr"
    if ((LFunc_var_index(lf, name) >= 0LL)) {
#line 5021 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IAddrVar(base, name));
    } else {
#line 5023 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IAddrGlobal(base, LModule_global_index(m, name)));
    }
#line 5024 "src/taumir/lower.tr"
    long long c8 = LFunc_new_vreg(lf);
#line 5025 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IConst(c8, 8LL));
#line 5026 "src/taumir/lower.tr"
    long long off = LFunc_new_vreg(lf);
#line 5027 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(off, _tr_str_lit_len("*", 1LL), idx, c8));
#line 5028 "src/taumir/lower.tr"
    long long addr = LFunc_new_vreg(lf);
#line 5029 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, addr, 10LL);
#line 5030 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_IBinOp(addr, _tr_str_lit_len("+", 1LL), base, off));
#line 5031 "src/taumir/lower.tr"
    return addr;
}

__attribute__((hot)) long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx) {
#line 5035 "src/taumir/lower.tr"
    long long addr = _inline_list_addr(m, lf, handle, idx, 8LL);
#line 5036 "src/taumir/lower.tr"
    long long gd = LFunc_new_vreg(lf);
#line 5037 "src/taumir/lower.tr"
    LFunc_emit(lf, LInst_ctor_ILoad(gd, addr, 0LL, 2LL));
#line 5038 "src/taumir/lower.tr"
    return gd;
}

__attribute__((hot)) long long _list_get_raw(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx) {
#line 5043 "src/taumir/lower.tr"
    if ((ltag == 22LL)) {
#line 5044 "src/taumir/lower.tr"
        long long baddr = _inline_list_addr(m, lf, handle, idx, 1LL);
#line 5045 "src/taumir/lower.tr"
        long long bd = LFunc_new_vreg(lf);
#line 5046 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadB(bd, baddr, 0LL));
#line 5047 "src/taumir/lower.tr"
        return bd;
    }
#line 5048 "src/taumir/lower.tr"
    return _list_get(m, lf, handle, idx);
}

__attribute__((hot)) long long _list_get_elem(LModule* m, LFunc* lf, long long ltag, long long handle, long long idx) {
#line 5053 "src/taumir/lower.tr"
    long long raw = _list_get_raw(m, lf, ltag, handle, idx);
#line 5054 "src/taumir/lower.tr"
    if ((ltag == 14LL)) {
#line 5055 "src/taumir/lower.tr"
        long long fv = LFunc_new_vreg(lf);
#line 5056 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBitsF(fv, raw));
#line 5057 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, fv, 5LL);
#line 5058 "src/taumir/lower.tr"
        return fv;
    }
#line 5059 "src/taumir/lower.tr"
    LFunc_set_vreg_type(lf, raw, _list_elem_tag(ltag));
#line 5060 "src/taumir/lower.tr"
    return raw;
}

__attribute__((hot)) long long _lower_expr_impl(LModule* m, LFunc* lf, HirExpr* e) {
#line 5063 "src/taumir/lower.tr"
    __auto_type _t6376 = (*e);
    if (_t6376.tag == HirExpr_ELitInt) {
        __auto_type v = _t6376.data.ELitInt.val;
#line 5065 "src/taumir/lower.tr"
        long long d = LFunc_new_vreg(lf);
#line 5066 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
#line 5067 "src/taumir/lower.tr"
        return d;
    } else if (_t6376.tag == HirExpr_ELitStr) {
        __auto_type sv = _t6376.data.ELitStr.val;
__auto_type sv_blen = _t6376.data.ELitStr.blen;
#line 5069 "src/taumir/lower.tr"
        long long idx = LModule_add_string(m, sv, sv_blen);
#line 5070 "src/taumir/lower.tr"
        long long ds = LFunc_new_vreg(lf);
#line 5071 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
#line 5072 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, ds, 1LL);
#line 5073 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 5074 "src/taumir/lower.tr"
        List_i64* lna = (void*)List_i64_new();
#line 5075 "src/taumir/lower.tr"
        List_i64_append(lna, ds);
#line 5076 "src/taumir/lower.tr"
        long long lheap = LFunc_new_vreg(lf);
#line 5077 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(lheap, _tr_str_lit_len("_tr_rt_str_new", 14LL), lna));
#line 5078 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, lheap, 1LL);
#line 5079 "src/taumir/lower.tr"
        _fresh_mark(lf, lheap);
#line 5080 "src/taumir/lower.tr"
        long long _trret1622 = lheap;
        return _trret1622;
    } else if (_t6376.tag == HirExpr_ELitBool) {
        __auto_type bval = _t6376.data.ELitBool.val;
#line 5082 "src/taumir/lower.tr"
        long long db = LFunc_new_vreg(lf);
#line 5083 "src/taumir/lower.tr"
        long long bconst = 0LL;
#line 5084 "src/taumir/lower.tr"
        if (bval) {
#line 5084 "src/taumir/lower.tr"
            bconst = 1LL;
        }
#line 5085 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(db, bconst));
#line 5086 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, db, 4LL);
#line 5087 "src/taumir/lower.tr"
        return db;
    } else if (_t6376.tag == HirExpr_ELitChar) {
        __auto_type cval = _t6376.data.ELitChar.val;
#line 5089 "src/taumir/lower.tr"
        long long cd = LFunc_new_vreg(lf);
#line 5090 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(cd, cval));
#line 5091 "src/taumir/lower.tr"
        return cd;
    } else if (_t6376.tag == HirExpr_ESizeOf) {
        __auto_type sz_ty = _t6376.data.ESizeOf.target_ty;
#line 5096 "src/taumir/lower.tr"
        TrStr _szn = _tr_str_retain(_subst_ty(m, sz_ty)->name);
#line 5097 "src/taumir/lower.tr"
        long long _szval = _sizeof_tyname(_szn);
#line 5098 "src/taumir/lower.tr"
        if (LModule_is_value_class(m, _szn)) {
#line 5098 "src/taumir/lower.tr"
            _szval = LModule_class_size(m, _szn);
        } else {
#line 5100 "src/taumir/lower.tr"
            long long _szg = _value_struct_generic_size(m, sz_ty);
#line 5101 "src/taumir/lower.tr"
            if ((_szg > 0LL)) {
#line 5101 "src/taumir/lower.tr"
                _szval = _szg;
            }
        }
#line 5102 "src/taumir/lower.tr"
        long long szd = LFunc_new_vreg(lf);
#line 5103 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(szd, _szval));
#line 5104 "src/taumir/lower.tr"
        long long _trret1623 = szd;
        _tr_str_release(_szn);
        return _trret1623;
    } else if (_t6376.tag == HirExpr_ETryExpr) {
        __auto_type texpr = _t6376.data.ETryExpr.expr;
__auto_type tokty = _t6376.data.ETryExpr.ty;
#line 5108 "src/taumir/lower.tr"
        if ((!lf->is_throws)) {
#line 5108 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 5109 "src/taumir/lower.tr"
        long long trv = lower_expr(m, lf, texpr);
#line 5110 "src/taumir/lower.tr"
        if (((trv < 0LL) || (LFunc_vreg_type(lf, trv) != 11LL))) {
#line 5110 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 5111 "src/taumir/lower.tr"
        long long tuid = LFunc_fresh_id(lf);
#line 5112 "src/taumir/lower.tr"
        TrStr tnm = ({ TrStr _tr_cr = (_lir_itoa(tuid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__try", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 5113 "src/taumir/lower.tr"
        LFunc_add_var(lf, tnm);
#line 5114 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, tnm, 11LL);
#line 5115 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(tnm, trv));
#line 5116 "src/taumir/lower.tr"
        long long trl = LFunc_new_vreg(lf);
#line 5117 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl, tnm));
#line 5118 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, trl, 11LL);
#line 5119 "src/taumir/lower.tr"
        long long ttag = _emit_field_get(m, lf, trl, 0LL, 0LL);
#line 5120 "src/taumir/lower.tr"
        long long terrc = _variant_tag_cond(lf, ttag, 1LL);
#line 5121 "src/taumir/lower.tr"
        long long terrb = LFunc_new_block(lf);
#line 5122 "src/taumir/lower.tr"
        long long tcont = LFunc_new_block(lf);
#line 5123 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TCondBr(terrc, terrb, tcont));
#line 5124 "src/taumir/lower.tr"
        LFunc_set_cur(lf, terrb);
#line 5125 "src/taumir/lower.tr"
        long long trl2 = LFunc_new_vreg(lf);
#line 5126 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl2, tnm));
#line 5127 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, trl2, 11LL);
#line 5128 "src/taumir/lower.tr"
        if ((!_run_defers(m, lf))) {
#line 5128 "src/taumir/lower.tr"
            _tr_str_release(tnm);
            return (-1LL);
        }
#line 5129 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TRetVal(trl2));
#line 5130 "src/taumir/lower.tr"
        LFunc_set_cur(lf, tcont);
#line 5131 "src/taumir/lower.tr"
        long long toktag = _tag_of(m, tokty);
#line 5132 "src/taumir/lower.tr"
        long long trl3 = LFunc_new_vreg(lf);
#line 5133 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(trl3, tnm));
#line 5134 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, trl3, 11LL);
#line 5135 "src/taumir/lower.tr"
        if ((toktag == 5LL)) {
#line 5136 "src/taumir/lower.tr"
            long long tpraw = _emit_field_get(m, lf, trl3, 8LL, 0LL);
#line 5137 "src/taumir/lower.tr"
            long long tpfv = LFunc_new_vreg(lf);
#line 5138 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBitsF(tpfv, tpraw));
#line 5139 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, tpfv, 5LL);
#line 5140 "src/taumir/lower.tr"
            long long _trret1624 = tpfv;
            _tr_str_release(tnm);
            return _trret1624;
        }
#line 5141 "src/taumir/lower.tr"
        long long _trret1625 = _emit_field_get(m, lf, trl3, 8LL, toktag);
        _tr_str_release(tnm);
        return _trret1625;
    } else if (_t6376.tag == HirExpr_EAwait) {
        __auto_type awexpr = _t6376.data.EAwait.expr;
#line 5145 "src/taumir/lower.tr"
        return _lower_await(m, lf, awexpr);
    } else if (_t6376.tag == HirExpr_EAwaitTimeout) {
        __auto_type awtexpr = _t6376.data.EAwaitTimeout.expr;
#line 5147 "src/taumir/lower.tr"
        return _lower_await(m, lf, awtexpr);
    } else if (_t6376.tag == HirExpr_EListComp) {
        __auto_type lcelem = _t6376.data.EListComp.element;
__auto_type lcgens = _t6376.data.EListComp.generators;
__auto_type lcty = _t6376.data.EListComp.ty;
#line 5149 "src/taumir/lower.tr"
        long long _trret1626 = _lower_list_comp(m, lf, lcelem, lcgens, lcty);
        return _trret1626;
    } else if (_t6376.tag == HirExpr_EGeneratorExpr) {
        __auto_type geelem = _t6376.data.EGeneratorExpr.element;
__auto_type geegens = _t6376.data.EGeneratorExpr.generators;
__auto_type geety = _t6376.data.EGeneratorExpr.ty;
#line 5151 "src/taumir/lower.tr"
        long long _trret1627 = _lower_list_comp(m, lf, geelem, geegens, geety);
        return _trret1627;
    } else if (_t6376.tag == HirExpr_ELitNone) {
        __auto_type nty = _t6376.data.ELitNone.ty;
#line 5156 "src/taumir/lower.tr"
        if ((((_tr_str_eqv((nty->name), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((nty->name), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((nty->name), (_tr_str_lit_len("NoneType", 8LL)))) || _is_null_str(nty->name))) {
#line 5157 "src/taumir/lower.tr"
            long long _trret1628 = _lower_enum_ctor(m, lf, _tr_str_lit_len("Option", 6LL), _tr_str_lit_len("None", 4LL), (void*)List_ptr_new());
            return _trret1628;
        }
#line 5158 "src/taumir/lower.tr"
        long long nnull = LFunc_new_vreg(lf);
#line 5159 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(nnull, 0LL));
#line 5160 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, nnull, 10LL);
#line 5161 "src/taumir/lower.tr"
        long long _trret1629 = nnull;
        return _trret1629;
    } else if (_t6376.tag == HirExpr_ERawStr) {
        __auto_type rsv = _t6376.data.ERawStr.val;
__auto_type rsv_blen = _t6376.data.ERawStr.blen;
#line 5163 "src/taumir/lower.tr"
        long long ridx = LModule_add_string(m, rsv, rsv_blen);
#line 5164 "src/taumir/lower.tr"
        long long rds = LFunc_new_vreg(lf);
#line 5165 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStr(rds, ridx));
#line 5166 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, rds, 1LL);
#line 5167 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 5168 "src/taumir/lower.tr"
        List_i64* rna = (void*)List_i64_new();
#line 5169 "src/taumir/lower.tr"
        List_i64_append(rna, rds);
#line 5170 "src/taumir/lower.tr"
        long long rheap = LFunc_new_vreg(lf);
#line 5171 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(rheap, _tr_str_lit_len("_tr_rt_str_new", 14LL), rna));
#line 5172 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, rheap, 1LL);
#line 5173 "src/taumir/lower.tr"
        _fresh_mark(lf, rheap);
#line 5174 "src/taumir/lower.tr"
        long long _trret1630 = rheap;
        return _trret1630;
    } else if (_t6376.tag == HirExpr_ELitFloat) {
        __auto_type fval = _t6376.data.ELitFloat.val;
#line 5176 "src/taumir/lower.tr"
        long long fd = LFunc_new_vreg(lf);
#line 5177 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(fd, _f64_bits(fval)));
#line 5178 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, fd, 5LL);
#line 5179 "src/taumir/lower.tr"
        return fd;
    } else if (_t6376.tag == HirExpr_ESuperMethodCall) {
        __auto_type sbase = _t6376.data.ESuperMethodCall.base_class;
__auto_type smeth = _t6376.data.ESuperMethodCall.method;
__auto_type sargs = _t6376.data.ESuperMethodCall.args;
#line 5182 "src/taumir/lower.tr"
        TrStr smang = LModule_resolve_method(m, sbase, smeth);
#line 5183 "src/taumir/lower.tr"
        if (_tr_str_eqv((smang), (_tr_str_lit_len("", 0LL)))) {
#line 5183 "src/taumir/lower.tr"
            _tr_str_release(smang);
            return (-1LL);
        }
#line 5184 "src/taumir/lower.tr"
        long long sselfv = LFunc_new_vreg(lf);
#line 5185 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(sselfv, _tr_str_lit_len("self", 4LL)));
#line 5186 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sselfv, LFunc_var_type(lf, _tr_str_lit_len("self", 4LL)));
#line 5187 "src/taumir/lower.tr"
        long long _trret1631 = _lower_obj_call(m, lf, smang, sselfv, sargs);
        _tr_str_release(smang);
        return _trret1631;
    } else if (_t6376.tag == HirExpr_ESuperPropAccess) {
        __auto_type sbase2 = _t6376.data.ESuperPropAccess.base_class;
__auto_type sprop = _t6376.data.ESuperPropAccess.prop;
#line 5190 "src/taumir/lower.tr"
        TrStr scur = LFunc_var_cls_of(lf, _tr_str_lit_len("self", 4LL));
#line 5191 "src/taumir/lower.tr"
        if ((_tr_str_eqv((scur), (_tr_str_lit_len("", 0LL))) || (!LModule_is_class(m, scur)))) {
#line 5191 "src/taumir/lower.tr"
            _tr_str_release(scur);
            return (-1LL);
        }
#line 5192 "src/taumir/lower.tr"
        long long sfoff = LModule_field_offset(m, scur, sprop);
#line 5193 "src/taumir/lower.tr"
        if ((sfoff < 0LL)) {
#line 5193 "src/taumir/lower.tr"
            _tr_str_release(scur);
            return (-1LL);
        }
#line 5194 "src/taumir/lower.tr"
        long long sftg = LModule_field_tag(m, scur, sprop);
#line 5195 "src/taumir/lower.tr"
        if ((((sftg < 0LL) || _is_list_tag(sftg)) || _is_dict_tag(sftg))) {
#line 5195 "src/taumir/lower.tr"
            _tr_str_release(scur);
            return (-1LL);
        }
#line 5196 "src/taumir/lower.tr"
        long long sselfp = LFunc_new_vreg(lf);
#line 5197 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(sselfp, _tr_str_lit_len("self", 4LL)));
#line 5198 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, sselfp, LFunc_var_type(lf, _tr_str_lit_len("self", 4LL)));
#line 5199 "src/taumir/lower.tr"
        if ((sftg == 5LL)) {
#line 5200 "src/taumir/lower.tr"
            long long sfraw = _emit_field_get(m, lf, sselfp, sfoff, 0LL);
#line 5201 "src/taumir/lower.tr"
            long long sffv = LFunc_new_vreg(lf);
#line 5202 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBitsF(sffv, sfraw));
#line 5203 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, sffv, 5LL);
#line 5204 "src/taumir/lower.tr"
            long long _trret1632 = sffv;
            _tr_str_release(scur);
            return _trret1632;
        }
#line 5205 "src/taumir/lower.tr"
        long long _trret1633 = _emit_field_get(m, lf, sselfp, sfoff, sftg);
        _tr_str_release(scur);
        return _trret1633;
    } else if (_t6376.tag == HirExpr_ECast) {
        __auto_type cinner = _t6376.data.ECast.expr;
__auto_type ctty = _t6376.data.ECast.target_ty;
#line 5207 "src/taumir/lower.tr"
        long long cv = lower_expr(m, lf, cinner);
#line 5208 "src/taumir/lower.tr"
        if ((cv < 0LL)) {
#line 5208 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 5209 "src/taumir/lower.tr"
        long long cvt = LFunc_vreg_type(lf, cv);
#line 5210 "src/taumir/lower.tr"
        TrStr ctn = _tr_str_retain(ctty->name);
#line 5211 "src/taumir/lower.tr"
        if (_is_null_str(ctn)) {
#line 5211 "src/taumir/lower.tr"
            _tr_str_release(ctn);
            return (-1LL);
        }
#line 5213 "src/taumir/lower.tr"
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((ctn), (_tr_str_lit_len("f32", 3LL))))) {
#line 5214 "src/taumir/lower.tr"
            if ((cvt == 5LL)) {
#line 5214 "src/taumir/lower.tr"
                long long _trret1634 = cv;
                _tr_str_release(ctn);
                return _trret1634;
            }
#line 5215 "src/taumir/lower.tr"
            if (((cvt == 0LL) || (cvt == 4LL))) {
#line 5215 "src/taumir/lower.tr"
                long long _trret1635 = _promote_f(lf, cv);
                _tr_str_release(ctn);
                return _trret1635;
            }
#line 5216 "src/taumir/lower.tr"
            _tr_str_release(ctn);
            return (-1LL);
        }
#line 5218 "src/taumir/lower.tr"
        if (_is_int_cast_target(ctn)) {
#line 5219 "src/taumir/lower.tr"
            long long r = cv;
#line 5220 "src/taumir/lower.tr"
            if ((cvt == 5LL)) {
#line 5221 "src/taumir/lower.tr"
                r = LFunc_new_vreg(lf);
#line 5222 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFToI(r, cv));
#line 5223 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, r, 0LL);
            } else if (((cvt != 0LL) && (cvt != 4LL))) {
#line 5227 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL));
#line 5228 "src/taumir/lower.tr"
                List_i64* cpa = (void*)List_i64_new();
#line 5229 "src/taumir/lower.tr"
                List_i64_append(cpa, cv);
#line 5230 "src/taumir/lower.tr"
                r = LFunc_new_vreg(lf);
#line 5231 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(r, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL), cpa));
#line 5232 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, r, 0LL);
            }
#line 5233 "src/taumir/lower.tr"
            r = _narrow_int(lf, r, ctn);
#line 5234 "src/taumir/lower.tr"
            if (_tr_str_eqv((ctn), (_tr_str_lit_len("bool", 4LL)))) {
#line 5235 "src/taumir/lower.tr"
                long long rb = _norm_bool(lf, r);
#line 5236 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, rb, 4LL);
#line 5237 "src/taumir/lower.tr"
                long long _trret1636 = rb;
                _tr_str_release(ctn);
                return _trret1636;
            }
#line 5238 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, r, 0LL);
#line 5239 "src/taumir/lower.tr"
            long long _trret1637 = r;
            _tr_str_release(ctn);
            return _trret1637;
        }
#line 5241 "src/taumir/lower.tr"
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("String", 6LL)))) && (cvt == 1LL))) {
#line 5241 "src/taumir/lower.tr"
            long long _trret1638 = cv;
            _tr_str_release(ctn);
            return _trret1638;
        }
#line 5245 "src/taumir/lower.tr"
        if (((_tr_str_eqv((ctn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((ctn), (_tr_str_lit_len("String", 6LL)))) && (cvt == 0LL))) {
#line 5246 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 5247 "src/taumir/lower.tr"
            List_i64* sna = (void*)List_i64_new();
#line 5248 "src/taumir/lower.tr"
            List_i64_append(sna, cv);
#line 5249 "src/taumir/lower.tr"
            long long snd = LFunc_new_vreg(lf);
#line 5250 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(snd, _tr_str_lit_len("_tr_rt_str_new", 14LL), sna));
#line 5251 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, snd, 1LL);
#line 5252 "src/taumir/lower.tr"
            _fresh_mark(lf, snd);
#line 5253 "src/taumir/lower.tr"
            long long _trret1639 = snd;
            _tr_str_release(ctn);
            return _trret1639;
        }
#line 5255 "src/taumir/lower.tr"
        if ((_tr_str_eqv((ctn), (_tr_str_lit_len("char", 4LL))) && ((cvt == 0LL) || (cvt == 4LL)))) {
#line 5256 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, cv, 0LL);
#line 5257 "src/taumir/lower.tr"
            long long _trret1640 = cv;
            _tr_str_release(ctn);
            return _trret1640;
        }
#line 5261 "src/taumir/lower.tr"
        if (_tr_str_eqv((ctn), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 5262 "src/taumir/lower.tr"
            if ((cvt == 0LL)) {
#line 5262 "src/taumir/lower.tr"
                long long _trret1641 = cv;
                _tr_str_release(ctn);
                return _trret1641;
            }
#line 5263 "src/taumir/lower.tr"
            if (((((cvt == 1LL) || (cvt == 10LL)) || (cvt == 11LL)) || (cvt == 12LL))) {
#line 5264 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL));
#line 5265 "src/taumir/lower.tr"
                List_i64* paa = (void*)List_i64_new();
#line 5266 "src/taumir/lower.tr"
                List_i64_append(paa, cv);
#line 5267 "src/taumir/lower.tr"
                long long pad = LFunc_new_vreg(lf);
#line 5268 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(pad, _tr_str_lit_len("_tr_rt_ptr_addr", 15LL), paa));
#line 5269 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, pad, 0LL);
#line 5270 "src/taumir/lower.tr"
                long long _trret1642 = pad;
                _tr_str_release(ctn);
                return _trret1642;
            }
#line 5271 "src/taumir/lower.tr"
            _tr_str_release(ctn);
            return (-1LL);
        }
#line 5272 "src/taumir/lower.tr"
        _tr_str_release(ctn);
        return (-1LL);
    } else if (_t6376.tag == HirExpr_EIdent) {
        __auto_type name = _t6376.data.EIdent.name;
#line 5276 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, name) < 0LL) && (LFunc_capture_index(lf, name) >= 0LL))) {
#line 5277 "src/taumir/lower.tr"
            long long cix = LFunc_capture_index(lf, name);
#line 5278 "src/taumir/lower.tr"
            long long cenv = LFunc_new_vreg(lf);
#line 5279 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(cenv, _tr_str_lit_len("__env", 5LL)));
#line 5280 "src/taumir/lower.tr"
            long long caddr = _emit_field_get(m, lf, cenv, ((1LL + cix) * 8LL), 0LL);
#line 5281 "src/taumir/lower.tr"
            long long ctag = List_i64_get(lf->cap_tags, cix);
#line 5282 "src/taumir/lower.tr"
            if ((ctag == 5LL)) {
#line 5283 "src/taumir/lower.tr"
                long long craw = _emit_field_get(m, lf, caddr, 0LL, 0LL);
#line 5284 "src/taumir/lower.tr"
                long long cfv = LFunc_new_vreg(lf);
#line 5285 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(cfv, craw));
#line 5286 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, cfv, 5LL);
#line 5287 "src/taumir/lower.tr"
                long long _trret1643 = cfv;
                return _trret1643;
            }
#line 5288 "src/taumir/lower.tr"
            long long _trret1644 = _emit_field_get(m, lf, caddr, 0LL, ctag);
            return _trret1644;
        }
#line 5289 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, name) < 0LL) && LModule_is_global(m, name))) {
#line 5290 "src/taumir/lower.tr"
            long long gd = LFunc_new_vreg(lf);
#line 5291 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadGlobal(gd, LModule_global_index(m, name)));
#line 5292 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, gd, LModule_global_type(m, name));
#line 5293 "src/taumir/lower.tr"
            long long _trret1645 = gd;
            return _trret1645;
        }
#line 5294 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, name) < 0LL) && LModule_is_user_fn(m, name))) {
#line 5295 "src/taumir/lower.tr"
            long long fav = LFunc_new_vreg(lf);
#line 5296 "src/taumir/lower.tr"
            ({ TrStr _at_t6377 = (_own(name)); LFunc_emit(lf, LInst_ctor_IFuncAddr(fav, _at_t6377)); _tr_str_release(_at_t6377); });
#line 5297 "src/taumir/lower.tr"
            long long _trret1646 = fav;
            return _trret1646;
        }
#line 5298 "src/taumir/lower.tr"
        LFunc_add_var(lf, name);
#line 5299 "src/taumir/lower.tr"
        long long d2 = LFunc_new_vreg(lf);
#line 5300 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(d2, name));
#line 5301 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, d2, LFunc_var_type(lf, name));
#line 5302 "src/taumir/lower.tr"
        long long _trret1647 = d2;
        return _trret1647;
    } else if (_t6376.tag == HirExpr_EIfElse) {
        __auto_type cond = _t6376.data.EIfElse.cond;
__auto_type then_e = _t6376.data.EIfElse.then_e;
__auto_type else_e = _t6376.data.EIfElse.else_e;
#line 5305 "src/taumir/lower.tr"
        long long tcv = lower_expr(m, lf, cond);
#line 5306 "src/taumir/lower.tr"
        if ((tcv < 0LL)) {
#line 5306 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 5307 "src/taumir/lower.tr"
        long long tuid = LFunc_fresh_id(lf);
#line 5308 "src/taumir/lower.tr"
        TrStr rname = ({ TrStr _tr_cr = (_lir_itoa(tuid)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__tern", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 5309 "src/taumir/lower.tr"
        LFunc_add_var(lf, rname);
#line 5310 "src/taumir/lower.tr"
        long long then_b = LFunc_new_block(lf);
#line 5311 "src/taumir/lower.tr"
        long long else_b = LFunc_new_block(lf);
#line 5312 "src/taumir/lower.tr"
        long long end_b = LFunc_new_block(lf);
#line 5313 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TCondBr(tcv, then_b, else_b));
#line 5314 "src/taumir/lower.tr"
        LFunc_set_cur(lf, then_b);
#line 5315 "src/taumir/lower.tr"
        long long tv = lower_expr(m, lf, then_e);
#line 5316 "src/taumir/lower.tr"
        if ((tv < 0LL)) {
#line 5316 "src/taumir/lower.tr"
            _tr_str_release(rname);
            return (-1LL);
        }
#line 5317 "src/taumir/lower.tr"
        long long rtype = LFunc_vreg_type(lf, tv);
#line 5320 "src/taumir/lower.tr"
        if ((rtype == 1LL)) {
#line 5320 "src/taumir/lower.tr"
            _secure_str(m, lf, tv);
        }
#line 5321 "src/taumir/lower.tr"
        LFunc_set_var_type(lf, rname, rtype);
#line 5322 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, tv));
#line 5323 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
#line 5324 "src/taumir/lower.tr"
        LFunc_set_cur(lf, else_b);
#line 5325 "src/taumir/lower.tr"
        long long ev2 = lower_expr(m, lf, else_e);
#line 5326 "src/taumir/lower.tr"
        if ((ev2 < 0LL)) {
#line 5326 "src/taumir/lower.tr"
            _tr_str_release(rname);
            return (-1LL);
        }
#line 5327 "src/taumir/lower.tr"
        if ((rtype == 1LL)) {
#line 5327 "src/taumir/lower.tr"
            _secure_str(m, lf, ev2);
        }
#line 5328 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, ev2));
#line 5329 "src/taumir/lower.tr"
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
#line 5330 "src/taumir/lower.tr"
        LFunc_set_cur(lf, end_b);
#line 5331 "src/taumir/lower.tr"
        long long trd = LFunc_new_vreg(lf);
#line 5332 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ILoadVar(trd, rname));
#line 5333 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, trd, rtype);
#line 5334 "src/taumir/lower.tr"
        if ((rtype == 1LL)) {
#line 5334 "src/taumir/lower.tr"
            _fresh_mark(lf, trd);
        }
#line 5335 "src/taumir/lower.tr"
        long long _trret1648 = trd;
        _tr_str_release(rname);
        return _trret1648;
    } else if (_t6376.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t6376.data.EUnaryOp.op;
__auto_type sub = _t6376.data.EUnaryOp.expr;
#line 5338 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 5339 "src/taumir/lower.tr"
            TrStr ncls = _recv_class(m, lf, sub);
#line 5340 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((ncls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, ncls))) {
#line 5341 "src/taumir/lower.tr"
                TrStr nm2 = LModule_resolve_method(m, ncls, _tr_str_lit_len("__neg__", 7LL));
#line 5342 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((nm2), (_tr_str_lit_len("", 0LL))))) {
#line 5343 "src/taumir/lower.tr"
                    long long nself = lower_expr(m, lf, sub);
#line 5344 "src/taumir/lower.tr"
                    if ((nself < 0LL)) {
#line 5344 "src/taumir/lower.tr"
                        _tr_str_release(ncls);
                        _tr_str_release(nm2);
                        return (-1LL);
                    }
#line 5345 "src/taumir/lower.tr"
                    long long _trret1649 = _lower_obj_call(m, lf, nm2, nself, (void*)List_ptr_new());
                    _tr_str_release(ncls);
                    _tr_str_release(nm2);
                    return _trret1649;
                }
            }
        }
#line 5346 "src/taumir/lower.tr"
        long long sv = lower_expr(m, lf, sub);
#line 5347 "src/taumir/lower.tr"
        if ((sv < 0LL)) {
#line 5347 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 5348 "src/taumir/lower.tr"
        long long svt = LFunc_vreg_type(lf, sv);
#line 5349 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 5350 "src/taumir/lower.tr"
            if ((svt == 5LL)) {
#line 5351 "src/taumir/lower.tr"
                long long zf = LFunc_new_vreg(lf);
#line 5352 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IConst(zf, _f64_bits(0.0)));
#line 5353 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, zf, 5LL);
#line 5354 "src/taumir/lower.tr"
                long long dnf = LFunc_new_vreg(lf);
#line 5355 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBinOp(dnf, _tr_str_lit_len("-", 1LL), zf, sv));
#line 5356 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, dnf, 5LL);
#line 5357 "src/taumir/lower.tr"
                long long _trret1650 = dnf;
                return _trret1650;
            }
#line 5358 "src/taumir/lower.tr"
            if ((svt != 0LL)) {
#line 5358 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 5359 "src/taumir/lower.tr"
            long long zn = LFunc_new_vreg(lf);
#line 5360 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(zn, 0LL));
#line 5361 "src/taumir/lower.tr"
            long long dneg = LFunc_new_vreg(lf);
#line 5362 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(dneg, _tr_str_lit_len("-", 1LL), zn, sv));
#line 5363 "src/taumir/lower.tr"
            long long _trret1651 = dneg;
            return _trret1651;
        }
#line 5364 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL)))) {
#line 5365 "src/taumir/lower.tr"
            if (((svt != 0LL) && (svt != 4LL))) {
#line 5365 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 5366 "src/taumir/lower.tr"
            long long zt = LFunc_new_vreg(lf);
#line 5367 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(zt, 0LL));
#line 5368 "src/taumir/lower.tr"
            long long dnot = LFunc_new_vreg(lf);
#line 5369 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(dnot, _tr_str_lit_len("==", 2LL), sv, zt));
#line 5370 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, dnot, 4LL);
#line 5371 "src/taumir/lower.tr"
            long long _trret1652 = dnot;
            return _trret1652;
        }
#line 5372 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("~", 1LL)))) {
#line 5373 "src/taumir/lower.tr"
            if ((svt != 0LL)) {
#line 5373 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 5374 "src/taumir/lower.tr"
            long long ones = LFunc_new_vreg(lf);
#line 5375 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(ones, (0LL - 1LL)));
#line 5376 "src/taumir/lower.tr"
            long long dcpl = LFunc_new_vreg(lf);
#line 5377 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(dcpl, _tr_str_lit_len("^", 1LL), sv, ones));
#line 5378 "src/taumir/lower.tr"
            long long _trret1653 = dcpl;
            return _trret1653;
        }
#line 5379 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 5380 "src/taumir/lower.tr"
            __auto_type _t6378 = (*sub);
            if (_t6378.tag == HirExpr_EIdent) {
                __auto_type aname = _t6378.data.EIdent.name;
#line 5382 "src/taumir/lower.tr"
                long long ad = LFunc_new_vreg(lf);
#line 5383 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IAddrVar(ad, aname));
#line 5384 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, ad, 0LL);
#line 5385 "src/taumir/lower.tr"
                long long _trret1654 = ad;
                return _trret1654;
            } else if (1) {
                __auto_type _ = _t6378;
#line 5387 "src/taumir/lower.tr"
                return (-1LL);
            }
        }
#line 5388 "src/taumir/lower.tr"
        return (-1LL);
    } else if (_t6376.tag == HirExpr_EBinOp) {
        __auto_type op = _t6376.data.EBinOp.op;
__auto_type l = _t6376.data.EBinOp.left;
__auto_type r = _t6376.data.EBinOp.right;
#line 5394 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
#line 5395 "src/taumir/lower.tr"
            long long scl = lower_expr(m, lf, l);
#line 5396 "src/taumir/lower.tr"
            if ((scl < 0LL)) {
#line 5396 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 5397 "src/taumir/lower.tr"
            long long scla = _norm_bool(lf, scl);
#line 5398 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 5399 "src/taumir/lower.tr"
            TrStr scvar = ({ TrStr _tr_cr = (_lir_itoa(LFunc_fresh_id(lf))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__sc", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 5400 "src/taumir/lower.tr"
            LFunc_add_var(lf, scvar);
#line 5401 "src/taumir/lower.tr"
            LFunc_set_var_type(lf, scvar, 4LL);
#line 5402 "src/taumir/lower.tr"
            long long sc_rhs = LFunc_new_block(lf);
#line 5403 "src/taumir/lower.tr"
            long long sc_sc = LFunc_new_block(lf);
#line 5404 "src/taumir/lower.tr"
            long long sc_end = LFunc_new_block(lf);
#line 5405 "src/taumir/lower.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) {
#line 5406 "src/taumir/lower.tr"
                LFunc_set_term(lf, LTerm_ctor_TCondBr(scla, sc_rhs, sc_sc));
            } else {
#line 5408 "src/taumir/lower.tr"
                LFunc_set_term(lf, LTerm_ctor_TCondBr(scla, sc_sc, sc_rhs));
            }
#line 5410 "src/taumir/lower.tr"
            LFunc_set_cur(lf, sc_rhs);
#line 5411 "src/taumir/lower.tr"
            long long scr = lower_expr(m, lf, r);
#line 5412 "src/taumir/lower.tr"
            if ((scr < 0LL)) {
#line 5412 "src/taumir/lower.tr"
                _tr_str_release(scvar);
                return (-1LL);
            }
#line 5413 "src/taumir/lower.tr"
            long long scrb = _norm_bool(lf, scr);
#line 5414 "src/taumir/lower.tr"
            _flush_fresh_strs(m, lf);
#line 5415 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(scvar, scrb));
#line 5416 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TBr(sc_end));
#line 5418 "src/taumir/lower.tr"
            LFunc_set_cur(lf, sc_sc);
#line 5419 "src/taumir/lower.tr"
            long long sccv = LFunc_new_vreg(lf);
#line 5420 "src/taumir/lower.tr"
            long long scconst = 0LL;
#line 5421 "src/taumir/lower.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) {
#line 5421 "src/taumir/lower.tr"
                scconst = 1LL;
            }
#line 5422 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(sccv, scconst));
#line 5423 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IStoreVar(scvar, sccv));
#line 5424 "src/taumir/lower.tr"
            LFunc_set_term(lf, LTerm_ctor_TBr(sc_end));
#line 5426 "src/taumir/lower.tr"
            LFunc_set_cur(lf, sc_end);
#line 5427 "src/taumir/lower.tr"
            long long scd = LFunc_new_vreg(lf);
#line 5428 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(scd, scvar));
#line 5429 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, scd, 4LL);
#line 5430 "src/taumir/lower.tr"
            long long _trret1655 = scd;
            _tr_str_release(scvar);
            return _trret1655;
        }
#line 5433 "src/taumir/lower.tr"
        TrStr ddn = _dunder_for_op(op);
#line 5434 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((ddn), (_tr_str_lit_len("", 0LL))))) {
#line 5435 "src/taumir/lower.tr"
            TrStr lcls_d = _recv_class(m, lf, l);
#line 5436 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((lcls_d), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, lcls_d))) {
#line 5437 "src/taumir/lower.tr"
                TrStr ddm = LModule_resolve_method(m, lcls_d, ddn);
#line 5438 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((ddm), (_tr_str_lit_len("", 0LL))))) {
#line 5439 "src/taumir/lower.tr"
                    long long da = lower_expr(m, lf, l);
#line 5440 "src/taumir/lower.tr"
                    if ((da < 0LL)) {
#line 5440 "src/taumir/lower.tr"
                        _tr_str_release(ddn);
                        _tr_str_release(lcls_d);
                        _tr_str_release(ddm);
                        return (-1LL);
                    }
#line 5441 "src/taumir/lower.tr"
                    List_ptr* dargs = (void*)List_ptr_new();
#line 5442 "src/taumir/lower.tr"
                    List_ptr_append(dargs, r);
#line 5443 "src/taumir/lower.tr"
                    long long _trret1656 = _lower_obj_call(m, lf, ddm, da, dargs);
                    _tr_str_release(ddn);
                    _tr_str_release(lcls_d);
                    _tr_str_release(ddm);
                    return _trret1656;
                }
            }
        }
#line 5444 "src/taumir/lower.tr"
        long long a = lower_expr(m, lf, l);
#line 5445 "src/taumir/lower.tr"
        if ((a < 0LL)) {
#line 5445 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5446 "src/taumir/lower.tr"
        long long b = lower_expr(m, lf, r);
#line 5447 "src/taumir/lower.tr"
        if ((b < 0LL)) {
#line 5447 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5448 "src/taumir/lower.tr"
        long long at = LFunc_vreg_type(lf, a);
#line 5449 "src/taumir/lower.tr"
        long long bt = LFunc_vreg_type(lf, b);
#line 5451 "src/taumir/lower.tr"
        if (((at == 5LL) || (bt == 5LL))) {
#line 5452 "src/taumir/lower.tr"
            if (((at != 5LL) && (at != 0LL))) {
#line 5452 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5453 "src/taumir/lower.tr"
            if (((bt != 5LL) && (bt != 0LL))) {
#line 5453 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5454 "src/taumir/lower.tr"
            long long fa = a;
#line 5455 "src/taumir/lower.tr"
            long long fb = b;
#line 5456 "src/taumir/lower.tr"
            if ((at == 0LL)) {
#line 5456 "src/taumir/lower.tr"
                fa = _promote_f(lf, a);
            }
#line 5457 "src/taumir/lower.tr"
            if ((bt == 0LL)) {
#line 5457 "src/taumir/lower.tr"
                fb = _promote_f(lf, b);
            }
#line 5458 "src/taumir/lower.tr"
            if ((((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("/", 1LL))))) {
#line 5459 "src/taumir/lower.tr"
                long long fdd = LFunc_new_vreg(lf);
#line 5460 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBinOp(fdd, op, fa, fb));
#line 5461 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fdd, 5LL);
#line 5462 "src/taumir/lower.tr"
                long long _trret1657 = fdd;
                _tr_str_release(ddn);
                return _trret1657;
            }
#line 5463 "src/taumir/lower.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL)))) {
#line 5464 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_pow", 10LL));
#line 5465 "src/taumir/lower.tr"
                long long fpw = LFunc_new_vreg(lf);
#line 5466 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFCall2F(fpw, _tr_str_lit_len("_tr_rt_pow", 10LL), fa, fb));
#line 5467 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fpw, 5LL);
#line 5468 "src/taumir/lower.tr"
                long long _trret1658 = fpw;
                _tr_str_release(ddn);
                return _trret1658;
            }
#line 5469 "src/taumir/lower.tr"
            if (_is_cmp_op(op)) {
#line 5470 "src/taumir/lower.tr"
                long long fcd = LFunc_new_vreg(lf);
#line 5471 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBinOp(fcd, op, fa, fb));
#line 5472 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fcd, 4LL);
#line 5473 "src/taumir/lower.tr"
                long long _trret1659 = fcd;
                _tr_str_release(ddn);
                return _trret1659;
            }
#line 5474 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5476 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && (bt == 10LL))) {
#line 5477 "src/taumir/lower.tr"
            TrStr ccls = _recv_class(m, lf, r);
#line 5478 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((ccls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, ccls))) {
#line 5479 "src/taumir/lower.tr"
                TrStr ccm = LModule_resolve_method(m, ccls, _tr_str_lit_len("__contains__", 12LL));
#line 5480 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((ccm), (_tr_str_lit_len("", 0LL))))) {
#line 5481 "src/taumir/lower.tr"
                    List_ptr* cca = (void*)List_ptr_new();
#line 5482 "src/taumir/lower.tr"
                    List_ptr_append(cca, l);
#line 5483 "src/taumir/lower.tr"
                    long long _trret1660 = _lower_obj_call(m, lf, ccm, b, cca);
                    _tr_str_release(ddn);
                    _tr_str_release(ccls);
                    _tr_str_release(ccm);
                    return _trret1660;
                }
            }
#line 5484 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            _tr_str_release(ccls);
            return (-1LL);
        }
#line 5486 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && _is_dict_tag(bt))) {
#line 5487 "src/taumir/lower.tr"
            if ((_dict_key_is_str(bt) && (at != 1LL))) {
#line 5487 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5488 "src/taumir/lower.tr"
            if (((!_dict_key_is_str(bt)) && (at != 0LL))) {
#line 5488 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5489 "src/taumir/lower.tr"
            TrStr dhsym = _dict_sym(bt, _tr_str_lit_len("has", 3LL));
#line 5490 "src/taumir/lower.tr"
            LModule_add_extern(m, dhsym);
#line 5491 "src/taumir/lower.tr"
            List_i64* dha = (void*)List_i64_new();
#line 5492 "src/taumir/lower.tr"
            List_i64_append(dha, b);
#line 5493 "src/taumir/lower.tr"
            List_i64_append(dha, a);
#line 5494 "src/taumir/lower.tr"
            long long dhd = LFunc_new_vreg(lf);
#line 5495 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(dhd, dhsym, dha));
#line 5496 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, dhd, 4LL);
#line 5497 "src/taumir/lower.tr"
            long long _trret1661 = dhd;
            _tr_str_release(ddn);
            _tr_str_release(dhsym);
            return _trret1661;
        }
#line 5499 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && _is_set_tag(bt))) {
#line 5500 "src/taumir/lower.tr"
            if (((bt == 16LL) && (at != 1LL))) {
#line 5500 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5501 "src/taumir/lower.tr"
            if (((bt == 13LL) && (at != 0LL))) {
#line 5501 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5502 "src/taumir/lower.tr"
            TrStr shas = _set_sym(bt, _tr_str_lit_len("has", 3LL));
#line 5503 "src/taumir/lower.tr"
            LModule_add_extern(m, shas);
#line 5504 "src/taumir/lower.tr"
            List_i64* sha = (void*)List_i64_new();
#line 5505 "src/taumir/lower.tr"
            List_i64_append(sha, b);
#line 5506 "src/taumir/lower.tr"
            List_i64_append(sha, a);
#line 5507 "src/taumir/lower.tr"
            long long shd = LFunc_new_vreg(lf);
#line 5508 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(shd, shas, sha));
#line 5509 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, shd, 4LL);
#line 5510 "src/taumir/lower.tr"
            long long _trret1662 = shd;
            _tr_str_release(ddn);
            _tr_str_release(shas);
            return _trret1662;
        }
#line 5512 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))) && (bt == 1LL))) {
#line 5513 "src/taumir/lower.tr"
            TrStr ssym = _tr_str_lit_len("_tr_rt_str_contains", 19LL);
#line 5514 "src/taumir/lower.tr"
            if (((at == 0LL) || (at == 4LL))) {
#line 5515 "src/taumir/lower.tr"
                TrStr _strtmp_t6379 = _tr_str_lit_len("_tr_rt_str_contains_char", 24LL);
                _tr_str_release(ssym);
                ssym = _strtmp_t6379;
            } else if ((at != 1LL)) {
#line 5516 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                _tr_str_release(ssym);
                return (-1LL);
            }
#line 5517 "src/taumir/lower.tr"
            LModule_add_extern(m, ssym);
#line 5518 "src/taumir/lower.tr"
            List_i64* ssa2 = (void*)List_i64_new();
#line 5519 "src/taumir/lower.tr"
            List_i64_append(ssa2, b);
#line 5520 "src/taumir/lower.tr"
            List_i64_append(ssa2, a);
#line 5521 "src/taumir/lower.tr"
            long long ssd = LFunc_new_vreg(lf);
#line 5522 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ssd, ssym, ssa2));
#line 5523 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ssd, 4LL);
#line 5524 "src/taumir/lower.tr"
            long long _trret1663 = ssd;
            _tr_str_release(ddn);
            _tr_str_release(ssym);
            return _trret1663;
        }
#line 5526 "src/taumir/lower.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("in", 2LL)))) {
#line 5527 "src/taumir/lower.tr"
            if ((!_is_list_tag(bt))) {
#line 5527 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5528 "src/taumir/lower.tr"
            if ((bt == 14LL)) {
#line 5528 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5529 "src/taumir/lower.tr"
            long long want_e = _list_elem_tag(bt);
#line 5530 "src/taumir/lower.tr"
            if ((at != want_e)) {
#line 5530 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5531 "src/taumir/lower.tr"
            TrStr csym = _tr_str_lit_len("_tr_rt_list_contains_i64", 24LL);
#line 5532 "src/taumir/lower.tr"
            if ((want_e == 1LL)) {
#line 5532 "src/taumir/lower.tr"
                TrStr _strtmp_t6380 = _tr_str_lit_len("_tr_rt_list_contains_str", 24LL);
                _tr_str_release(csym);
                csym = _strtmp_t6380;
            }
#line 5533 "src/taumir/lower.tr"
            LModule_add_extern(m, csym);
#line 5534 "src/taumir/lower.tr"
            List_i64* cca = (void*)List_i64_new();
#line 5535 "src/taumir/lower.tr"
            List_i64_append(cca, b);
#line 5536 "src/taumir/lower.tr"
            List_i64_append(cca, a);
#line 5537 "src/taumir/lower.tr"
            long long cd = LFunc_new_vreg(lf);
#line 5538 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(cd, csym, cca));
#line 5539 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, cd, 4LL);
#line 5540 "src/taumir/lower.tr"
            long long _trret1664 = cd;
            _tr_str_release(ddn);
            _tr_str_release(csym);
            return _trret1664;
        }
#line 5542 "src/taumir/lower.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL))) && ((at == 1LL) || (bt == 1LL))) && ((at == 0LL) || (bt == 0LL)))) {
#line 5543 "src/taumir/lower.tr"
            long long sreg = a;
#line 5544 "src/taumir/lower.tr"
            long long nreg = b;
#line 5545 "src/taumir/lower.tr"
            if ((at == 0LL)) {
#line 5546 "src/taumir/lower.tr"
                sreg = b;
#line 5547 "src/taumir/lower.tr"
                nreg = a;
            }
#line 5548 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_repeat", 17LL));
#line 5549 "src/taumir/lower.tr"
            List_i64* ra = (void*)List_i64_new();
#line 5550 "src/taumir/lower.tr"
            List_i64_append(ra, sreg);
#line 5551 "src/taumir/lower.tr"
            List_i64_append(ra, nreg);
#line 5552 "src/taumir/lower.tr"
            long long rd = LFunc_new_vreg(lf);
#line 5553 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit_len("_tr_rt_str_repeat", 17LL), ra));
#line 5554 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, rd, 1LL);
#line 5555 "src/taumir/lower.tr"
            _fresh_mark(lf, rd);
#line 5556 "src/taumir/lower.tr"
            long long _trret1665 = rd;
            _tr_str_release(ddn);
            return _trret1665;
        }
#line 5557 "src/taumir/lower.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && (at == 1LL)) && (bt == 1LL))) {
#line 5558 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
#line 5559 "src/taumir/lower.tr"
            List_i64* ca = (void*)List_i64_new();
#line 5560 "src/taumir/lower.tr"
            List_i64_append(ca, a);
#line 5561 "src/taumir/lower.tr"
            List_i64_append(ca, b);
#line 5562 "src/taumir/lower.tr"
            long long dc = LFunc_new_vreg(lf);
#line 5563 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(dc, _tr_str_lit_len("_tr_rt_str_concat", 17LL), ca));
#line 5564 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, dc, 1LL);
#line 5565 "src/taumir/lower.tr"
            _fresh_mark(lf, dc);
#line 5566 "src/taumir/lower.tr"
            long long _trret1666 = dc;
            _tr_str_release(ddn);
            return _trret1666;
        }
#line 5568 "src/taumir/lower.tr"
        if (((_is_cmp_op(op) && (at == 1LL)) && (bt == 1LL))) {
#line 5569 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_cmp", 14LL));
#line 5570 "src/taumir/lower.tr"
            List_i64* sca = (void*)List_i64_new();
#line 5571 "src/taumir/lower.tr"
            List_i64_append(sca, a);
#line 5572 "src/taumir/lower.tr"
            List_i64_append(sca, b);
#line 5573 "src/taumir/lower.tr"
            long long cmpv = LFunc_new_vreg(lf);
#line 5574 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit_len("_tr_rt_str_cmp", 14LL), sca));
#line 5575 "src/taumir/lower.tr"
            long long zc = LFunc_new_vreg(lf);
#line 5576 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
#line 5577 "src/taumir/lower.tr"
            long long rc = LFunc_new_vreg(lf);
#line 5578 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(rc, op, cmpv, zc));
#line 5579 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, rc, 4LL);
#line 5580 "src/taumir/lower.tr"
            long long _trret1667 = rc;
            _tr_str_release(ddn);
            return _trret1667;
        }
#line 5581 "src/taumir/lower.tr"
        if (((at == 1LL) || (bt == 1LL))) {
#line 5581 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5582 "src/taumir/lower.tr"
        if ((_is_list_tag(at) || _is_list_tag(bt))) {
#line 5582 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5583 "src/taumir/lower.tr"
        if (((at == 11LL) || (bt == 11LL))) {
#line 5586 "src/taumir/lower.tr"
            if (((at != 11LL) || (bt != 11LL))) {
#line 5586 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5587 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
#line 5587 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5588 "src/taumir/lower.tr"
            long long eatag = _emit_field_get(m, lf, a, 0LL, 0LL);
#line 5589 "src/taumir/lower.tr"
            long long ebtag = _emit_field_get(m, lf, b, 0LL, 0LL);
#line 5590 "src/taumir/lower.tr"
            long long ecd = LFunc_new_vreg(lf);
#line 5591 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(ecd, op, eatag, ebtag));
#line 5592 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ecd, 4LL);
#line 5593 "src/taumir/lower.tr"
            long long _trret1668 = ecd;
            _tr_str_release(ddn);
            return _trret1668;
        }
#line 5594 "src/taumir/lower.tr"
        if (((at == 10LL) || (bt == 10LL))) {
#line 5597 "src/taumir/lower.tr"
            if (((at != 10LL) || (bt != 10LL))) {
#line 5597 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
#line 5598 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
#line 5598 "src/taumir/lower.tr"
                _tr_str_release(ddn);
                return (-1LL);
            }
        }
#line 5602 "src/taumir/lower.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((op), (_tr_str_lit_len("or", 2LL))))) {
#line 5603 "src/taumir/lower.tr"
            long long na = _norm_bool(lf, a);
#line 5604 "src/taumir/lower.tr"
            long long nb = _norm_bool(lf, b);
#line 5605 "src/taumir/lower.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) {
#line 5606 "src/taumir/lower.tr"
                long long dand = LFunc_new_vreg(lf);
#line 5607 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBinOp(dand, _tr_str_lit_len("*", 1LL), na, nb));
#line 5608 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, dand, 4LL);
#line 5609 "src/taumir/lower.tr"
                long long _trret1669 = dand;
                _tr_str_release(ddn);
                return _trret1669;
            }
#line 5610 "src/taumir/lower.tr"
            long long ssum = LFunc_new_vreg(lf);
#line 5611 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(ssum, _tr_str_lit_len("+", 1LL), na, nb));
#line 5612 "src/taumir/lower.tr"
            long long oro = _norm_bool(lf, ssum);
#line 5613 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, oro, 4LL);
#line 5614 "src/taumir/lower.tr"
            long long _trret1670 = oro;
            _tr_str_release(ddn);
            return _trret1670;
        }
#line 5615 "src/taumir/lower.tr"
        if (((_tr_str_eqv((op), (_tr_str_lit_len("**", 2LL))) && ((at == 0LL) || (at == 4LL))) && ((bt == 0LL) || (bt == 4LL)))) {
#line 5616 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_int_pow", 14LL));
#line 5617 "src/taumir/lower.tr"
            List_i64* ipa = (void*)List_i64_new();
#line 5618 "src/taumir/lower.tr"
            List_i64_append(ipa, a);
#line 5619 "src/taumir/lower.tr"
            List_i64_append(ipa, b);
#line 5620 "src/taumir/lower.tr"
            long long ipd = LFunc_new_vreg(lf);
#line 5621 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ipd, _tr_str_lit_len("_tr_rt_int_pow", 14LL), ipa));
#line 5622 "src/taumir/lower.tr"
            long long _trret1671 = ipd;
            _tr_str_release(ddn);
            return _trret1671;
        }
#line 5623 "src/taumir/lower.tr"
        if ((!_int_op(op))) {
#line 5623 "src/taumir/lower.tr"
            _tr_str_release(ddn);
            return (-1LL);
        }
#line 5624 "src/taumir/lower.tr"
        long long d3 = LFunc_new_vreg(lf);
#line 5625 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IBinOp(d3, op, a, b));
#line 5626 "src/taumir/lower.tr"
        if (_is_cmp_op(op)) {
#line 5626 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, d3, 4LL);
        }
#line 5627 "src/taumir/lower.tr"
        long long _trret1672 = d3;
        _tr_str_release(ddn);
        return _trret1672;
    } else if (_t6376.tag == HirExpr_ECall) {
        __auto_type callee = _t6376.data.ECall.callee;
__auto_type args = _t6376.data.ECall.args;
#line 5631 "src/taumir/lower.tr"
        TrStr alloc_nm = _tr_str_lit_len("", 0LL);
#line 5632 "src/taumir/lower.tr"
        __auto_type _t6381 = (*callee);
        if (_t6381.tag == HirExpr_EIdent) {
            __auto_type anm = _t6381.data.EIdent.name;
            TrStr _strtmp_t6382 = _tr_str_retain(anm);
            _tr_str_release(alloc_nm);
            alloc_nm = _strtmp_t6382;
        } else if (_t6381.tag == HirExpr_EIndex) {
            __auto_type abase = _t6381.data.EIndex.obj;
#line 5635 "src/taumir/lower.tr"
            __auto_type _t6383 = (*abase);
            if (_t6383.tag == HirExpr_EIdent) {
                __auto_type anm2 = _t6383.data.EIdent.name;
                TrStr _strtmp_t6384 = _tr_str_retain(anm2);
                _tr_str_release(alloc_nm);
                alloc_nm = _strtmp_t6384;
            } else if (1) {
                __auto_type _ = _t6383;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t6381;
            /* pass */
        }
#line 5639 "src/taumir/lower.tr"
        if (((_tr_str_eqv((alloc_nm), (_tr_str_lit_len("alloc", 5LL))) || _tr_str_eqv((alloc_nm), (_tr_str_lit_len("core_alloc_alloc", 16LL)))) && (args->len == 1LL))) {
#line 5641 "src/taumir/lower.tr"
            AstType* aptr_ty = hir_expr_type(e);
#line 5642 "src/taumir/lower.tr"
            long long astride = _ptr_stride(m, aptr_ty);
#line 5643 "src/taumir/lower.tr"
            if ((astride == 0LL)) {
#line 5643 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
#line 5644 "src/taumir/lower.tr"
            long long acnt = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5645 "src/taumir/lower.tr"
            if (((acnt < 0LL) || (LFunc_vreg_type(lf, acnt) != 0LL))) {
#line 5645 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
#line 5646 "src/taumir/lower.tr"
            long long astr_c = LFunc_new_vreg(lf);
#line 5647 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(astr_c, astride));
#line 5648 "src/taumir/lower.tr"
            long long anb = LFunc_new_vreg(lf);
#line 5649 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IBinOp(anb, _tr_str_lit_len("*", 1LL), acnt, astr_c));
#line 5650 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL));
#line 5651 "src/taumir/lower.tr"
            List_i64* aargs = (void*)List_i64_new();
#line 5652 "src/taumir/lower.tr"
            List_i64_append(aargs, anb);
#line 5653 "src/taumir/lower.tr"
            long long ad = LFunc_new_vreg(lf);
#line 5654 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ad, _tr_str_lit_len("_tr_rt_raw_alloc", 16LL), aargs));
#line 5655 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ad, 0LL);
#line 5656 "src/taumir/lower.tr"
            long long _trret1673 = ad;
            _tr_str_release(alloc_nm);
            return _trret1673;
        }
#line 5657 "src/taumir/lower.tr"
        if (((_tr_str_eqv((alloc_nm), (_tr_str_lit_len("dealloc", 7LL))) || _tr_str_eqv((alloc_nm), (_tr_str_lit_len("core_alloc_dealloc", 18LL)))) && (args->len == 1LL))) {
#line 5658 "src/taumir/lower.tr"
            long long dpv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5659 "src/taumir/lower.tr"
            if ((dpv < 0LL)) {
#line 5659 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                return (-1LL);
            }
#line 5660 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_raw_free", 15LL));
#line 5661 "src/taumir/lower.tr"
            List_i64* dfa = (void*)List_i64_new();
#line 5662 "src/taumir/lower.tr"
            List_i64_append(dfa, dpv);
#line 5663 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_raw_free", 15LL), dfa));
#line 5664 "src/taumir/lower.tr"
            long long _trret1674 = dpv;
            _tr_str_release(alloc_nm);
            return _trret1674;
        }
#line 5667 "src/taumir/lower.tr"
        __auto_type _t6385 = (*callee);
        if (_t6385.tag == HirExpr_EPropAccess) {
            __auto_type pfo = _t6385.data.EPropAccess.obj;
__auto_type pfp = _t6385.data.EPropAccess.prop;
#line 5669 "src/taumir/lower.tr"
            TrStr pfcls = _recv_class(m, lf, pfo);
#line 5670 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((pfcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, pfcls))) {
#line 5671 "src/taumir/lower.tr"
                long long pfoff = LModule_field_offset(m, pfcls, pfp);
#line 5672 "src/taumir/lower.tr"
                if (((pfoff >= 0LL) && (LModule_field_tag(m, pfcls, pfp) == 0LL))) {
#line 5673 "src/taumir/lower.tr"
                    long long pfobj = lower_expr(m, lf, pfo);
#line 5674 "src/taumir/lower.tr"
                    if ((pfobj < 0LL)) {
#line 5674 "src/taumir/lower.tr"
                        _tr_str_release(pfcls);
                        _tr_str_release(alloc_nm);
                        return (-1LL);
                    }
#line 5675 "src/taumir/lower.tr"
                    long long pfrt = _tag_of(m, hir_expr_type(e));
#line 5676 "src/taumir/lower.tr"
                    if ((pfrt < 0LL)) {
#line 5676 "src/taumir/lower.tr"
                        _tr_str_release(pfcls);
                        _tr_str_release(alloc_nm);
                        return (-1LL);
                    }
#line 5677 "src/taumir/lower.tr"
                    List_i64* pfargs = (void*)List_i64_new();
#line 5678 "src/taumir/lower.tr"
                    long long pfai = 0LL;
#line 5679 "src/taumir/lower.tr"
                    while ((pfai < args->len)) {
#line 5680 "src/taumir/lower.tr"
                        long long pfav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, pfai)));
#line 5681 "src/taumir/lower.tr"
                        if ((pfav < 0LL)) {
#line 5681 "src/taumir/lower.tr"
                            _tr_str_release(pfcls);
                            List_i64_free(pfargs);
                            _tr_str_release(alloc_nm);
                            return (-1LL);
                        }
#line 5682 "src/taumir/lower.tr"
                        List_i64_append(pfargs, pfav);
#line 5683 "src/taumir/lower.tr"
                        pfai = (pfai + 1LL);
                    }
#line 5684 "src/taumir/lower.tr"
                    long long pffn = _emit_field_get(m, lf, pfobj, pfoff, 0LL);
#line 5685 "src/taumir/lower.tr"
                    long long pfd = LFunc_new_vreg(lf);
#line 5686 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICallInd(pfd, pffn, pfargs));
#line 5687 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, pfd, pfrt);
#line 5688 "src/taumir/lower.tr"
                    if ((pfrt == 1LL)) {
#line 5688 "src/taumir/lower.tr"
                        _fresh_mark(lf, pfd);
                    }
#line 5689 "src/taumir/lower.tr"
                    long long _trret1675 = pfd;
                    _tr_str_release(pfcls);
                    _tr_str_release(alloc_nm);
                    return _trret1675;
                }
            }
            _tr_str_release(pfcls);
        } else if (1) {
            __auto_type _ = _t6385;
#line 5691 "src/taumir/lower.tr"
            /* pass */
        }
#line 5692 "src/taumir/lower.tr"
        TrStr fn = _ident_name(callee);
#line 5693 "src/taumir/lower.tr"
        if (_tr_str_eqv((fn), (_tr_str_lit_len("", 0LL)))) {
#line 5693 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 5696 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("Pointer", 7LL))) && (args->len == 1LL))) {
#line 5697 "src/taumir/lower.tr"
            long long ptv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5698 "src/taumir/lower.tr"
            if ((ptv < 0LL)) {
#line 5698 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5699 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, ptv) != 0LL)) {
#line 5699 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5700 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ptv, 0LL);
#line 5701 "src/taumir/lower.tr"
            long long _trret1676 = ptv;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1676;
        }
#line 5704 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("await_all", 9LL))) && (args->len > 0LL))) {
#line 5705 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_tg_begin", 12LL));
#line 5706 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_tg_begin", 12LL), (void*)List_i64_new()));
#line 5707 "src/taumir/lower.tr"
            lf->in_taskgroup = (lf->in_taskgroup + 1LL);
#line 5708 "src/taumir/lower.tr"
            long long awi = 0LL;
#line 5709 "src/taumir/lower.tr"
            while ((awi < args->len)) {
#line 5710 "src/taumir/lower.tr"
                if ((!_lower_spawn(m, lf, ((HirExpr*)List_ptr_get(args, awi))))) {
#line 5711 "src/taumir/lower.tr"
                    lf->in_taskgroup = (lf->in_taskgroup - 1LL);
#line 5712 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
#line 5713 "src/taumir/lower.tr"
                awi = (awi + 1LL);
            }
#line 5714 "src/taumir/lower.tr"
            lf->in_taskgroup = (lf->in_taskgroup - 1LL);
#line 5715 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_taskgroup_wait", 18LL));
#line 5716 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_taskgroup_wait", 18LL), (void*)List_i64_new()));
#line 5717 "src/taumir/lower.tr"
            long long awz = LFunc_new_vreg(lf);
#line 5718 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(awz, 0LL));
#line 5719 "src/taumir/lower.tr"
            long long _trret1677 = awz;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1677;
        }
#line 5723 "src/taumir/lower.tr"
        AstType* ecty = hir_expr_type(e);
#line 5724 "src/taumir/lower.tr"
        if (LModule_is_enum(m, ecty->name)) {
#line 5725 "src/taumir/lower.tr"
            TrStr evn = _norm_variant(ecty->name, fn);
#line 5726 "src/taumir/lower.tr"
            if ((LModule_enum_variant_index(m, ecty->name, evn) >= 0LL)) {
#line 5727 "src/taumir/lower.tr"
                long long _trret1678 = _lower_enum_ctor(m, lf, ecty->name, evn, args);
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(evn);
                return _trret1678;
            }
        }
#line 5730 "src/taumir/lower.tr"
        if (((LFunc_var_index(lf, fn) >= 0LL) && (LFunc_var_type(lf, fn) == 12LL))) {
#line 5731 "src/taumir/lower.tr"
            long long kret = LFunc_var_xret_of(lf, fn);
#line 5732 "src/taumir/lower.tr"
            if ((kret < 0LL)) {
#line 5732 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5733 "src/taumir/lower.tr"
            if (((args->len + 1LL) > 6LL)) {
#line 5733 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5734 "src/taumir/lower.tr"
            long long kblk = LFunc_new_vreg(lf);
#line 5735 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(kblk, fn));
#line 5736 "src/taumir/lower.tr"
            List_i64* kargs = (void*)List_i64_new();
#line 5737 "src/taumir/lower.tr"
            List_i64_append(kargs, kblk);
#line 5738 "src/taumir/lower.tr"
            long long ki = 0LL;
#line 5739 "src/taumir/lower.tr"
            while ((ki < args->len)) {
#line 5740 "src/taumir/lower.tr"
                long long kav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ki)));
#line 5741 "src/taumir/lower.tr"
                if ((kav < 0LL)) {
#line 5741 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(kargs);
                    return (-1LL);
                }
#line 5742 "src/taumir/lower.tr"
                List_i64_append(kargs, kav);
#line 5743 "src/taumir/lower.tr"
                ki = (ki + 1LL);
            }
#line 5744 "src/taumir/lower.tr"
            long long kfp = _emit_field_get(m, lf, kblk, 0LL, 0LL);
#line 5745 "src/taumir/lower.tr"
            long long kd = LFunc_new_vreg(lf);
#line 5746 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICallInd(kd, kfp, kargs));
#line 5747 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, kd, kret);
#line 5748 "src/taumir/lower.tr"
            if ((kret == 1LL)) {
#line 5748 "src/taumir/lower.tr"
                _fresh_mark(lf, kd);
            }
#line 5749 "src/taumir/lower.tr"
            long long _trret1679 = kd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1679;
        }
#line 5752 "src/taumir/lower.tr"
        if ((((LFunc_var_index(lf, fn) >= 0LL) && (LFunc_var_type(lf, fn) == 0LL)) && (!LModule_is_user_fn(m, fn)))) {
#line 5753 "src/taumir/lower.tr"
            long long frt = _tag_of(m, hir_expr_type(e));
#line 5754 "src/taumir/lower.tr"
            if ((frt < 0LL)) {
#line 5754 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5755 "src/taumir/lower.tr"
            List_i64* fargs = (void*)List_i64_new();
#line 5756 "src/taumir/lower.tr"
            long long fai = 0LL;
#line 5757 "src/taumir/lower.tr"
            while ((fai < args->len)) {
#line 5758 "src/taumir/lower.tr"
                long long fav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, fai)));
#line 5759 "src/taumir/lower.tr"
                if ((fav < 0LL)) {
#line 5759 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(fargs);
                    return (-1LL);
                }
#line 5760 "src/taumir/lower.tr"
                List_i64_append(fargs, fav);
#line 5761 "src/taumir/lower.tr"
                fai = (fai + 1LL);
            }
#line 5762 "src/taumir/lower.tr"
            long long ffp = LFunc_new_vreg(lf);
#line 5763 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoadVar(ffp, fn));
#line 5764 "src/taumir/lower.tr"
            long long ffd = LFunc_new_vreg(lf);
#line 5765 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICallInd(ffd, ffp, fargs));
#line 5766 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ffd, frt);
#line 5767 "src/taumir/lower.tr"
            if ((frt == 1LL)) {
#line 5767 "src/taumir/lower.tr"
                _fresh_mark(lf, ffd);
            }
#line 5768 "src/taumir/lower.tr"
            long long _trret1680 = ffd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1680;
        }
#line 5771 "src/taumir/lower.tr"
        if ((LFunc_var_index(lf, fn) >= 0LL)) {
#line 5772 "src/taumir/lower.tr"
            TrStr callcls = _recv_class(m, lf, callee);
#line 5773 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((callcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, callcls))) {
#line 5774 "src/taumir/lower.tr"
                TrStr callm = LModule_resolve_method(m, callcls, _tr_str_lit_len("__call__", 8LL));
#line 5775 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((callm), (_tr_str_lit_len("", 0LL))))) {
#line 5776 "src/taumir/lower.tr"
                    long long callself = lower_expr(m, lf, callee);
#line 5777 "src/taumir/lower.tr"
                    if ((callself < 0LL)) {
#line 5777 "src/taumir/lower.tr"
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(callcls);
                        _tr_str_release(callm);
                        return (-1LL);
                    }
#line 5778 "src/taumir/lower.tr"
                    long long _trret1681 = _lower_obj_call(m, lf, callm, callself, args);
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(callcls);
                    _tr_str_release(callm);
                    return _trret1681;
                }
            }
        }
#line 5781 "src/taumir/lower.tr"
        if (((args->len == 0LL) && (_prog_generic_class_index(m, fn) >= 0LL))) {
#line 5786 "src/taumir/lower.tr"
            AstType* gct0 = hir_expr_type(e);
#line 5787 "src/taumir/lower.tr"
            AstType* gct = ({ TrStr _at_t6386 = (_own(fn)); __auto_type _wr = (AstType_init(_at_t6386)); _tr_str_release(_at_t6386); _wr; });
#line 5788 "src/taumir/lower.tr"
            if ((gct0->args->len > 0LL)) {
#line 5789 "src/taumir/lower.tr"
                long long gaa = 0LL;
#line 5790 "src/taumir/lower.tr"
                while ((gaa < gct0->args->len)) {
#line 5791 "src/taumir/lower.tr"
                    ({ __auto_type _ord_t6387 = (box_asttype_lir(_subst_ty(m, (*((AstType**)List_ptr_get(gct0->args, gaa)))))); List_ptr_append(gct->args, _ord_t6387); });
#line 5792 "src/taumir/lower.tr"
                    gaa = (gaa + 1LL);
                }
            } else {
#line 5794 "src/taumir/lower.tr"
                HirClass* gcc2 = ({ __auto_type _ord_t6388 = (_prog_generic_class_index(m, fn)); __auto_type _wr = (((HirClass*)List_ptr_get(m->hir_prog->classes, _ord_t6388))); _wr; });
#line 5795 "src/taumir/lower.tr"
                if (((gcc2->generics->len == 0LL) || (m->subst_names->len == 0LL))) {
#line 5795 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
#line 5796 "src/taumir/lower.tr"
                long long gpi = 0LL;
#line 5797 "src/taumir/lower.tr"
                while ((gpi < gcc2->generics->len)) {
#line 5798 "src/taumir/lower.tr"
                    ({ TrStr _at_t6389 = (List_TrStr_get(gcc2->generics, gpi)); TrStr _at_t6390 = (_own(_at_t6389)); __auto_type _ord_t6391 = (box_asttype_lir(_subst_ty(m, AstType_init(_at_t6390)))); List_ptr_append(gct->args, _ord_t6391); _tr_str_release(_at_t6389); _tr_str_release(_at_t6390); });
#line 5799 "src/taumir/lower.tr"
                    gpi = (gpi + 1LL);
                }
            }
#line 5800 "src/taumir/lower.tr"
            if ((gct->args->len == 0LL)) {
#line 5800 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5801 "src/taumir/lower.tr"
            TrStr gcm = _ensure_generic_class(m, gct);
#line 5802 "src/taumir/lower.tr"
            if (_tr_str_eqv((gcm), (_tr_str_lit_len("", 0LL)))) {
#line 5803 "src/taumir/lower.tr"
                if (_tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 5803 "src/taumir/lower.tr"
                    m->fail_note = ({ __auto_type _ord_t6392 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_own(fn)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("generic ctor ", 13LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6392); TrStr _tr_cr = (_own((*((AstType**)List_ptr_get(gct->args, 0LL)))->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("] not instantiable", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
                }
#line 5804 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(gcm);
                return (-1LL);
            }
#line 5805 "src/taumir/lower.tr"
            long long gsz = LFunc_new_vreg(lf);
#line 5806 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(gsz, LModule_class_size(m, gcm)));
#line 5807 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 5808 "src/taumir/lower.tr"
            List_i64* goa = (void*)List_i64_new();
#line 5809 "src/taumir/lower.tr"
            List_i64_append(goa, gsz);
#line 5810 "src/taumir/lower.tr"
            long long god = LFunc_new_vreg(lf);
#line 5811 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(god, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), goa));
#line 5812 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, god, 10LL);
#line 5813 "src/taumir/lower.tr"
            _attach_class_drop(m, lf, god, gcm);
#line 5814 "src/taumir/lower.tr"
            _fresh_mark_obj(lf, god);
#line 5815 "src/taumir/lower.tr"
            long long _trret1682 = god;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(gcm);
            return _trret1682;
        }
#line 5817 "src/taumir/lower.tr"
        if ((LModule_is_class(m, fn) && (args->len == 0LL))) {
#line 5818 "src/taumir/lower.tr"
            long long szc = LFunc_new_vreg(lf);
#line 5819 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(szc, LModule_class_size(m, fn)));
#line 5820 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 5821 "src/taumir/lower.tr"
            List_i64* oaa = (void*)List_i64_new();
#line 5822 "src/taumir/lower.tr"
            List_i64_append(oaa, szc);
#line 5823 "src/taumir/lower.tr"
            long long od = LFunc_new_vreg(lf);
#line 5824 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(od, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), oaa));
#line 5825 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, od, 10LL);
#line 5826 "src/taumir/lower.tr"
            _attach_class_drop(m, lf, od, fn);
#line 5827 "src/taumir/lower.tr"
            _fresh_mark_obj(lf, od);
#line 5828 "src/taumir/lower.tr"
            long long _trret1683 = od;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1683;
        }
#line 5829 "src/taumir/lower.tr"
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))) && (args->len == 1LL)) && ({ TrStr _cmpa_t6393 = (_recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)))); TrStr _cmpb_t6394 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6395 = !_tr_str_eqv(_cmpa_t6393, _cmpb_t6394); _tr_str_release(_cmpa_t6393); _cmpr_t6395; }))) {
#line 5831 "src/taumir/lower.tr"
            TrStr lncls = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5832 "src/taumir/lower.tr"
            if (LModule_is_class(m, lncls)) {
#line 5833 "src/taumir/lower.tr"
                TrStr lnm = LModule_resolve_method(m, lncls, _tr_str_lit_len("__len__", 7LL));
#line 5834 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((lnm), (_tr_str_lit_len("", 0LL))))) {
#line 5835 "src/taumir/lower.tr"
                    long long lnself = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5836 "src/taumir/lower.tr"
                    if ((lnself < 0LL)) {
#line 5836 "src/taumir/lower.tr"
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(lncls);
                        _tr_str_release(lnm);
                        return (-1LL);
                    }
#line 5837 "src/taumir/lower.tr"
                    long long _trret1684 = _lower_obj_call(m, lf, lnm, lnself, (void*)List_ptr_new());
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(lncls);
                    _tr_str_release(lnm);
                    return _trret1684;
                }
            }
        }
#line 5838 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("len", 3LL))) && (args->len == 1LL))) {
#line 5839 "src/taumir/lower.tr"
            long long xv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5840 "src/taumir/lower.tr"
            if ((xv < 0LL)) {
#line 5840 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5841 "src/taumir/lower.tr"
            long long xt = LFunc_vreg_type(lf, xv);
#line 5842 "src/taumir/lower.tr"
            if (_is_list_tag(xt)) {
#line 5843 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 5844 "src/taumir/lower.tr"
                List_i64* lla = (void*)List_i64_new();
#line 5845 "src/taumir/lower.tr"
                List_i64_append(lla, xv);
#line 5846 "src/taumir/lower.tr"
                long long lld = LFunc_new_vreg(lf);
#line 5847 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(lld, _tr_str_lit_len("_tr_rt_list_len", 15LL), lla));
#line 5848 "src/taumir/lower.tr"
                long long _trret1685 = lld;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1685;
            }
#line 5849 "src/taumir/lower.tr"
            if ((xt == 1LL)) {
#line 5850 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
#line 5851 "src/taumir/lower.tr"
                List_i64* sla = (void*)List_i64_new();
#line 5852 "src/taumir/lower.tr"
                List_i64_append(sla, xv);
#line 5853 "src/taumir/lower.tr"
                long long sld = LFunc_new_vreg(lf);
#line 5854 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_strlen", 13LL), sla));
#line 5855 "src/taumir/lower.tr"
                long long _trret1686 = sld;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1686;
            }
#line 5856 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 5857 "src/taumir/lower.tr"
        if ((((_tr_str_eqv((fn), (_tr_str_lit_len("hex", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("oct", 3LL)))) || _tr_str_eqv((fn), (_tr_str_lit_len("bin", 3LL)))) && (args->len == 1LL))) {
#line 5858 "src/taumir/lower.tr"
            long long hxv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5859 "src/taumir/lower.tr"
            if (((hxv < 0LL) || (LFunc_vreg_type(lf, hxv) != 0LL))) {
#line 5859 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5860 "src/taumir/lower.tr"
            TrStr hxsym = _tr_str_lit_len("_tr_rt_hex_str", 14LL);
#line 5861 "src/taumir/lower.tr"
            if (_tr_str_eqv((fn), (_tr_str_lit_len("oct", 3LL)))) {
#line 5861 "src/taumir/lower.tr"
                TrStr _strtmp_t6396 = _tr_str_lit_len("_tr_rt_oct_str", 14LL);
                _tr_str_release(hxsym);
                hxsym = _strtmp_t6396;
            } else if (_tr_str_eqv((fn), (_tr_str_lit_len("bin", 3LL)))) {
#line 5862 "src/taumir/lower.tr"
                TrStr _strtmp_t6397 = _tr_str_lit_len("_tr_rt_bin_str", 14LL);
                _tr_str_release(hxsym);
                hxsym = _strtmp_t6397;
            }
#line 5863 "src/taumir/lower.tr"
            LModule_add_extern(m, hxsym);
#line 5864 "src/taumir/lower.tr"
            List_i64* hxa = (void*)List_i64_new();
#line 5865 "src/taumir/lower.tr"
            List_i64_append(hxa, hxv);
#line 5866 "src/taumir/lower.tr"
            long long hxd = LFunc_new_vreg(lf);
#line 5867 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(hxd, hxsym, hxa));
#line 5868 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, hxd, 1LL);
#line 5869 "src/taumir/lower.tr"
            _fresh_mark(lf, hxd);
#line 5870 "src/taumir/lower.tr"
            long long _trret1687 = hxd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(hxsym);
            return _trret1687;
        }
#line 5871 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("round", 5LL))) && (args->len == 1LL))) {
#line 5872 "src/taumir/lower.tr"
            long long rv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5873 "src/taumir/lower.tr"
            if ((rv0 < 0LL)) {
#line 5873 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5874 "src/taumir/lower.tr"
            long long rvt = LFunc_vreg_type(lf, rv0);
#line 5875 "src/taumir/lower.tr"
            if ((rvt == 0LL)) {
#line 5875 "src/taumir/lower.tr"
                rv0 = _promote_f(lf, rv0);
            } else if ((rvt != 5LL)) {
#line 5876 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5877 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_round", 12LL));
#line 5878 "src/taumir/lower.tr"
            long long rrd = LFunc_new_vreg(lf);
#line 5879 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IFCallF(rrd, _tr_str_lit_len("_tr_rt_round", 12LL), rv0));
#line 5880 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, rrd, 5LL);
#line 5881 "src/taumir/lower.tr"
            long long _trret1688 = rrd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1688;
        }
#line 5882 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("abs", 3LL))) && (args->len == 1LL))) {
#line 5883 "src/taumir/lower.tr"
            long long xv2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5884 "src/taumir/lower.tr"
            if ((xv2 < 0LL)) {
#line 5884 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5885 "src/taumir/lower.tr"
            long long xvt = LFunc_vreg_type(lf, xv2);
#line 5886 "src/taumir/lower.tr"
            if ((xvt == 5LL)) {
#line 5887 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_fabs", 11LL));
#line 5888 "src/taumir/lower.tr"
                long long fabd = LFunc_new_vreg(lf);
#line 5889 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFCallF(fabd, _tr_str_lit_len("_tr_rt_fabs", 11LL), xv2));
#line 5890 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fabd, 5LL);
#line 5891 "src/taumir/lower.tr"
                long long _trret1689 = fabd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1689;
            }
#line 5892 "src/taumir/lower.tr"
            if ((xvt != 0LL)) {
#line 5892 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5893 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_abs_i64", 14LL));
#line 5894 "src/taumir/lower.tr"
            List_i64* aba = (void*)List_i64_new();
#line 5895 "src/taumir/lower.tr"
            List_i64_append(aba, xv2);
#line 5896 "src/taumir/lower.tr"
            long long abd = LFunc_new_vreg(lf);
#line 5897 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(abd, _tr_str_lit_len("_tr_rt_abs_i64", 14LL), aba));
#line 5898 "src/taumir/lower.tr"
            long long _trret1690 = abd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1690;
        }
#line 5899 "src/taumir/lower.tr"
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) && (args->len == 2LL))) {
#line 5900 "src/taumir/lower.tr"
            long long mm1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5901 "src/taumir/lower.tr"
            if ((mm1 < 0LL)) {
#line 5901 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5902 "src/taumir/lower.tr"
            long long mm2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 1LL)));
#line 5903 "src/taumir/lower.tr"
            if ((mm2 < 0LL)) {
#line 5903 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5904 "src/taumir/lower.tr"
            long long mt1 = LFunc_vreg_type(lf, mm1);
#line 5905 "src/taumir/lower.tr"
            long long mt2 = LFunc_vreg_type(lf, mm2);
#line 5906 "src/taumir/lower.tr"
            if (((mt1 == 5LL) || (mt2 == 5LL))) {
#line 5907 "src/taumir/lower.tr"
                if ((mt1 == 0LL)) {
#line 5907 "src/taumir/lower.tr"
                    mm1 = _promote_f(lf, mm1);
                } else if ((mt1 != 5LL)) {
#line 5908 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
#line 5909 "src/taumir/lower.tr"
                if ((mt2 == 0LL)) {
#line 5909 "src/taumir/lower.tr"
                    mm2 = _promote_f(lf, mm2);
                } else if ((mt2 != 5LL)) {
#line 5910 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    return (-1LL);
                }
#line 5911 "src/taumir/lower.tr"
                TrStr fmsym = _tr_str_lit_len("_tr_rt_min_f64", 14LL);
#line 5912 "src/taumir/lower.tr"
                if (_tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) {
#line 5912 "src/taumir/lower.tr"
                    TrStr _strtmp_t6398 = _tr_str_lit_len("_tr_rt_max_f64", 14LL);
                    _tr_str_release(fmsym);
                    fmsym = _strtmp_t6398;
                }
#line 5913 "src/taumir/lower.tr"
                LModule_add_extern(m, fmsym);
#line 5914 "src/taumir/lower.tr"
                long long fmd = LFunc_new_vreg(lf);
#line 5915 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFCall2F(fmd, fmsym, mm1, mm2));
#line 5916 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fmd, 5LL);
#line 5917 "src/taumir/lower.tr"
                long long _trret1691 = fmd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(fmsym);
                return _trret1691;
            }
#line 5918 "src/taumir/lower.tr"
            if (((mt1 != 0LL) || (mt2 != 0LL))) {
#line 5918 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5919 "src/taumir/lower.tr"
            TrStr msym = _tr_str_lit_len("_tr_rt_min_i64", 14LL);
#line 5920 "src/taumir/lower.tr"
            if (_tr_str_eqv((fn), (_tr_str_lit_len("max", 3LL)))) {
#line 5920 "src/taumir/lower.tr"
                TrStr _strtmp_t6399 = _tr_str_lit_len("_tr_rt_max_i64", 14LL);
                _tr_str_release(msym);
                msym = _strtmp_t6399;
            }
#line 5921 "src/taumir/lower.tr"
            LModule_add_extern(m, msym);
#line 5922 "src/taumir/lower.tr"
            List_i64* mma = (void*)List_i64_new();
#line 5923 "src/taumir/lower.tr"
            List_i64_append(mma, mm1);
#line 5924 "src/taumir/lower.tr"
            List_i64_append(mma, mm2);
#line 5925 "src/taumir/lower.tr"
            long long mmd = LFunc_new_vreg(lf);
#line 5926 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(mmd, msym, mma));
#line 5927 "src/taumir/lower.tr"
            long long _trret1692 = mmd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(msym);
            return _trret1692;
        }
#line 5928 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("repr", 4LL))) && (args->len == 1LL))) {
#line 5929 "src/taumir/lower.tr"
            TrStr rprc = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5930 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((rprc), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, rprc))) {
#line 5931 "src/taumir/lower.tr"
                long long rprr = _obj_repr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), (-1LL));
#line 5932 "src/taumir/lower.tr"
                if ((rprr >= 0LL)) {
#line 5932 "src/taumir/lower.tr"
                    long long _trret1693 = rprr;
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(rprc);
                    return _trret1693;
                }
#line 5933 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return (-1LL);
            }
#line 5934 "src/taumir/lower.tr"
            long long rpv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5935 "src/taumir/lower.tr"
            if ((rpv < 0LL)) {
#line 5935 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return (-1LL);
            }
#line 5936 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, rpv) == 1LL)) {
#line 5936 "src/taumir/lower.tr"
                long long _trret1694 = rpv;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(rprc);
                return _trret1694;
            }
#line 5937 "src/taumir/lower.tr"
            long long _trret1695 = _reg_to_str(m, lf, rpv);
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(rprc);
            return _trret1695;
        }
#line 5938 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("str", 3LL))) && (args->len == 1LL))) {
#line 5939 "src/taumir/lower.tr"
            TrStr sobjc = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5940 "src/taumir/lower.tr"
            if ((((!_tr_str_eqv((sobjc), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, sobjc)) && ({ TrStr _cmpa_t6400 = (LModule_resolve_method(m, sobjc, _tr_str_lit_len("__str__", 7LL))); TrStr _cmpb_t6401 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6402 = !_tr_str_eqv(_cmpa_t6400, _cmpb_t6401); _tr_str_release(_cmpa_t6400); _cmpr_t6402; }))) {
#line 5941 "src/taumir/lower.tr"
                long long sobjr = _obj_to_str(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)), (-1LL));
#line 5942 "src/taumir/lower.tr"
                if ((sobjr >= 0LL)) {
#line 5942 "src/taumir/lower.tr"
                    long long _trret1696 = sobjr;
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    _tr_str_release(sobjc);
                    return _trret1696;
                }
#line 5943 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return (-1LL);
            }
#line 5944 "src/taumir/lower.tr"
            long long cv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5945 "src/taumir/lower.tr"
            if ((cv0 < 0LL)) {
#line 5945 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return (-1LL);
            }
#line 5946 "src/taumir/lower.tr"
            long long cvt = LFunc_vreg_type(lf, cv0);
#line 5947 "src/taumir/lower.tr"
            if ((cvt == 1LL)) {
#line 5947 "src/taumir/lower.tr"
                long long _trret1697 = cv0;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return _trret1697;
            }
#line 5948 "src/taumir/lower.tr"
            if ((cvt == 5LL)) {
#line 5949 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL));
#line 5950 "src/taumir/lower.tr"
                long long fsd = LFunc_new_vreg(lf);
#line 5951 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFCall1(fsd, _tr_str_lit_len("_tr_rt_f64_to_str", 17LL), cv0));
#line 5952 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fsd, 1LL);
#line 5953 "src/taumir/lower.tr"
                _fresh_mark(lf, fsd);
#line 5954 "src/taumir/lower.tr"
                long long _trret1698 = fsd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                return _trret1698;
            }
#line 5955 "src/taumir/lower.tr"
            TrStr ssym = _tr_str_lit_len("_tr_rt_i64_to_str", 17LL);
#line 5956 "src/taumir/lower.tr"
            if ((cvt == 4LL)) {
#line 5956 "src/taumir/lower.tr"
                TrStr _strtmp_t6403 = _tr_str_lit_len("_tr_rt_bool_to_str", 18LL);
                _tr_str_release(ssym);
                ssym = _strtmp_t6403;
            } else if ((cvt != 0LL)) {
#line 5957 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                _tr_str_release(sobjc);
                _tr_str_release(ssym);
                return (-1LL);
            }
#line 5958 "src/taumir/lower.tr"
            LModule_add_extern(m, ssym);
#line 5959 "src/taumir/lower.tr"
            List_i64* s2a = (void*)List_i64_new();
#line 5960 "src/taumir/lower.tr"
            List_i64_append(s2a, cv0);
#line 5961 "src/taumir/lower.tr"
            long long s2d = LFunc_new_vreg(lf);
#line 5962 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(s2d, ssym, s2a));
#line 5963 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, s2d, 1LL);
#line 5964 "src/taumir/lower.tr"
            _fresh_mark(lf, s2d);
#line 5965 "src/taumir/lower.tr"
            long long _trret1699 = s2d;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(sobjc);
            _tr_str_release(ssym);
            return _trret1699;
        }
#line 5966 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("float", 5LL))) && (args->len == 1LL))) {
#line 5967 "src/taumir/lower.tr"
            long long fv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5968 "src/taumir/lower.tr"
            if ((fv0 < 0LL)) {
#line 5968 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5969 "src/taumir/lower.tr"
            long long fvt = LFunc_vreg_type(lf, fv0);
#line 5970 "src/taumir/lower.tr"
            if ((fvt == 5LL)) {
#line 5970 "src/taumir/lower.tr"
                long long _trret1700 = fv0;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1700;
            }
#line 5971 "src/taumir/lower.tr"
            if ((fvt == 0LL)) {
#line 5971 "src/taumir/lower.tr"
                long long _trret1701 = _promote_f(lf, fv0);
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1701;
            }
#line 5972 "src/taumir/lower.tr"
            if ((fvt == 1LL)) {
#line 5973 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL));
#line 5974 "src/taumir/lower.tr"
                List_i64* fsa = (void*)List_i64_new();
#line 5975 "src/taumir/lower.tr"
                List_i64_append(fsa, fv0);
#line 5976 "src/taumir/lower.tr"
                long long fsr = LFunc_new_vreg(lf);
#line 5977 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(fsr, _tr_str_lit_len("_tr_rt_str_to_f64", 17LL), fsa));
#line 5978 "src/taumir/lower.tr"
                long long fsf = LFunc_new_vreg(lf);
#line 5979 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(fsf, fsr));
#line 5980 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fsf, 5LL);
#line 5981 "src/taumir/lower.tr"
                long long _trret1702 = fsf;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1702;
            }
#line 5982 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 5983 "src/taumir/lower.tr"
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("_tr_fn_int", 10LL)))) && (args->len == 1LL))) {
#line 5984 "src/taumir/lower.tr"
            long long iv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 5985 "src/taumir/lower.tr"
            if ((iv0 < 0LL)) {
#line 5985 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5986 "src/taumir/lower.tr"
            long long ivt = LFunc_vreg_type(lf, iv0);
#line 5987 "src/taumir/lower.tr"
            if ((ivt == 0LL)) {
#line 5987 "src/taumir/lower.tr"
                long long _trret1703 = iv0;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1703;
            }
#line 5988 "src/taumir/lower.tr"
            if ((ivt == 4LL)) {
#line 5989 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, iv0, 0LL);
#line 5990 "src/taumir/lower.tr"
                long long _trret1704 = iv0;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1704;
            }
#line 5991 "src/taumir/lower.tr"
            if ((ivt == 5LL)) {
#line 5992 "src/taumir/lower.tr"
                long long itd = LFunc_new_vreg(lf);
#line 5993 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFToI(itd, iv0));
#line 5994 "src/taumir/lower.tr"
                long long _trret1705 = itd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1705;
            }
#line 5995 "src/taumir/lower.tr"
            if ((ivt != 1LL)) {
#line 5995 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 5996 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL));
#line 5997 "src/taumir/lower.tr"
            List_i64* i2a = (void*)List_i64_new();
#line 5998 "src/taumir/lower.tr"
            List_i64_append(i2a, iv0);
#line 5999 "src/taumir/lower.tr"
            long long i2d = LFunc_new_vreg(lf);
#line 6000 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(i2d, _tr_str_lit_len("_tr_rt_str_to_i64", 17LL), i2a));
#line 6001 "src/taumir/lower.tr"
            long long _trret1706 = i2d;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1706;
        }
#line 6002 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("sum", 3LL))) && (args->len == 1LL))) {
#line 6003 "src/taumir/lower.tr"
            long long suv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 6004 "src/taumir/lower.tr"
            if ((suv < 0LL)) {
#line 6004 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6005 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, suv) != 2LL)) {
#line 6005 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6006 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL));
#line 6007 "src/taumir/lower.tr"
            List_i64* sua = (void*)List_i64_new();
#line 6008 "src/taumir/lower.tr"
            List_i64_append(sua, suv);
#line 6009 "src/taumir/lower.tr"
            long long sud = LFunc_new_vreg(lf);
#line 6010 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(sud, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL), sua));
#line 6011 "src/taumir/lower.tr"
            long long _trret1707 = sud;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1707;
        }
#line 6012 "src/taumir/lower.tr"
        if (((_tr_str_eqv((fn), (_tr_str_lit_len("any", 3LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("all", 3LL)))) && (args->len == 1LL))) {
#line 6013 "src/taumir/lower.tr"
            long long anv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 6014 "src/taumir/lower.tr"
            if ((anv < 0LL)) {
#line 6014 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6015 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, anv) != 2LL)) {
#line 6015 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6016 "src/taumir/lower.tr"
            TrStr ansym = _tr_str_lit_len("_tr_rt_list_any_i64", 19LL);
#line 6017 "src/taumir/lower.tr"
            if (_tr_str_eqv((fn), (_tr_str_lit_len("all", 3LL)))) {
#line 6017 "src/taumir/lower.tr"
                TrStr _strtmp_t6404 = _tr_str_lit_len("_tr_rt_list_all_i64", 19LL);
                _tr_str_release(ansym);
                ansym = _strtmp_t6404;
            }
#line 6018 "src/taumir/lower.tr"
            LModule_add_extern(m, ansym);
#line 6019 "src/taumir/lower.tr"
            List_i64* ana = (void*)List_i64_new();
#line 6020 "src/taumir/lower.tr"
            List_i64_append(ana, anv);
#line 6021 "src/taumir/lower.tr"
            long long and2 = LFunc_new_vreg(lf);
#line 6022 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(and2, ansym, ana));
#line 6023 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, and2, 4LL);
#line 6024 "src/taumir/lower.tr"
            long long _trret1708 = and2;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(ansym);
            return _trret1708;
        }
#line 6025 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("chr", 3LL))) && (args->len == 1LL))) {
#line 6026 "src/taumir/lower.tr"
            long long chv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 6027 "src/taumir/lower.tr"
            if ((chv < 0LL)) {
#line 6027 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6028 "src/taumir/lower.tr"
            long long cht = LFunc_vreg_type(lf, chv);
#line 6029 "src/taumir/lower.tr"
            if (((cht != 0LL) && (cht != 4LL))) {
#line 6029 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6030 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_chr", 10LL));
#line 6031 "src/taumir/lower.tr"
            List_i64* cha = (void*)List_i64_new();
#line 6032 "src/taumir/lower.tr"
            List_i64_append(cha, chv);
#line 6033 "src/taumir/lower.tr"
            long long chd = LFunc_new_vreg(lf);
#line 6034 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(chd, _tr_str_lit_len("_tr_rt_chr", 10LL), cha));
#line 6035 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, chd, 1LL);
#line 6036 "src/taumir/lower.tr"
            _fresh_mark(lf, chd);
#line 6037 "src/taumir/lower.tr"
            long long _trret1709 = chd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1709;
        }
#line 6038 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("ord", 3LL))) && (args->len == 1LL))) {
#line 6039 "src/taumir/lower.tr"
            long long ordv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 6040 "src/taumir/lower.tr"
            if ((ordv < 0LL)) {
#line 6040 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6041 "src/taumir/lower.tr"
            long long ordt = LFunc_vreg_type(lf, ordv);
#line 6042 "src/taumir/lower.tr"
            if (((ordt == 0LL) || (ordt == 4LL))) {
#line 6042 "src/taumir/lower.tr"
                long long _trret1710 = ordv;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1710;
            }
#line 6043 "src/taumir/lower.tr"
            if ((ordt != 1LL)) {
#line 6043 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6044 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_ord", 14LL));
#line 6045 "src/taumir/lower.tr"
            List_i64* orda = (void*)List_i64_new();
#line 6046 "src/taumir/lower.tr"
            List_i64_append(orda, ordv);
#line 6047 "src/taumir/lower.tr"
            long long ordd = LFunc_new_vreg(lf);
#line 6048 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ordd, _tr_str_lit_len("_tr_rt_str_ord", 14LL), orda));
#line 6049 "src/taumir/lower.tr"
            long long _trret1711 = ordd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1711;
        }
#line 6050 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("bool", 4LL))) && (args->len == 1LL))) {
#line 6051 "src/taumir/lower.tr"
            long long bv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
#line 6052 "src/taumir/lower.tr"
            if ((bv0 < 0LL)) {
#line 6052 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6053 "src/taumir/lower.tr"
            if (_is_list_tag(LFunc_vreg_type(lf, bv0))) {
#line 6053 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6054 "src/taumir/lower.tr"
            long long bnorm = _norm_bool(lf, bv0);
#line 6055 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, bnorm, 4LL);
#line 6056 "src/taumir/lower.tr"
            long long _trret1712 = bnorm;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1712;
        }
#line 6065 "src/taumir/lower.tr"
        if ((_tr_str_eqv((fn), (_tr_str_lit_len("_tr_str_lit", 11LL))) || _tr_str_eqv((fn), (_tr_str_lit_len("_tr_strx_join_trstr", 19LL))))) {
#line 6065 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 6068 "src/taumir/lower.tr"
        if (_tr_str_eqv((fn), (_tr_str_lit_len("_tr_str_wrap", 12LL)))) {
#line 6068 "src/taumir/lower.tr"
            TrStr _strtmp_t6405 = _tr_str_lit_len("_tr_rt_str_new", 14LL);
            _tr_str_release(fn);
            fn = _strtmp_t6405;
        }
#line 6071 "src/taumir/lower.tr"
        if (_tr_str_eqv((fn), (_tr_str_lit_len("_tr_getenv", 10LL)))) {
#line 6071 "src/taumir/lower.tr"
            TrStr _strtmp_t6406 = _tr_str_lit_len("_tr_rt_getenv", 13LL);
            _tr_str_release(fn);
            fn = _strtmp_t6406;
        }
#line 6072 "src/taumir/lower.tr"
        if (LModule_is_extern_fn(m, fn)) {
#line 6074 "src/taumir/lower.tr"
            if (({ __auto_type _ord_t6407 = (args->len); __auto_type _wr = ((_ord_t6407 > _arg_limit(m))); _wr; })) {
#line 6074 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6075 "src/taumir/lower.tr"
            List_i64* exargs = (void*)List_i64_new();
#line 6076 "src/taumir/lower.tr"
            long long exi = 0LL;
#line 6077 "src/taumir/lower.tr"
            while ((exi < args->len)) {
#line 6078 "src/taumir/lower.tr"
                long long exv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, exi)));
#line 6079 "src/taumir/lower.tr"
                if ((exv < 0LL)) {
#line 6079 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(exargs);
                    return (-1LL);
                }
#line 6080 "src/taumir/lower.tr"
                long long ext = LFunc_vreg_type(lf, exv);
#line 6081 "src/taumir/lower.tr"
                if (((((ext != 0LL) && (ext != 1LL)) && (ext != 4LL)) && (ext != 5LL))) {
#line 6081 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(exargs);
                    return (-1LL);
                }
#line 6082 "src/taumir/lower.tr"
                List_i64_append(exargs, exv);
#line 6083 "src/taumir/lower.tr"
                exi = (exi + 1LL);
            }
#line 6084 "src/taumir/lower.tr"
            LModule_add_extern(m, fn);
#line 6085 "src/taumir/lower.tr"
            long long extret = LModule_extern_ret_tag(m, fn);
#line 6086 "src/taumir/lower.tr"
            long long exd = LFunc_new_vreg(lf);
#line 6087 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(exd, fn, exargs));
#line 6088 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, exd, extret);
#line 6089 "src/taumir/lower.tr"
            if ((extret == 1LL)) {
#line 6095 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 6096 "src/taumir/lower.tr"
                List_i64* exwr = (void*)List_i64_new();
#line 6097 "src/taumir/lower.tr"
                List_i64_append(exwr, exd);
#line 6098 "src/taumir/lower.tr"
                long long exwd = LFunc_new_vreg(lf);
#line 6099 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(exwd, _tr_str_lit_len("_tr_rt_str_new", 14LL), exwr));
#line 6100 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, exwd, 1LL);
#line 6101 "src/taumir/lower.tr"
                long long _trret1713 = exwd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1713;
            }
#line 6102 "src/taumir/lower.tr"
            long long _trret1714 = exd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1714;
        }
#line 6107 "src/taumir/lower.tr"
        if ((_is_runtime_fn(fn) && (!LModule_is_user_fn(m, fn)))) {
#line 6108 "src/taumir/lower.tr"
            if (({ __auto_type _ord_t6408 = (args->len); __auto_type _wr = ((_ord_t6408 > _arg_limit(m))); _wr; })) {
#line 6108 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6109 "src/taumir/lower.tr"
            List_i64* rtargs = (void*)List_i64_new();
#line 6110 "src/taumir/lower.tr"
            long long rti = 0LL;
#line 6111 "src/taumir/lower.tr"
            while ((rti < args->len)) {
#line 6112 "src/taumir/lower.tr"
                long long rtv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, rti)));
#line 6113 "src/taumir/lower.tr"
                if ((rtv < 0LL)) {
#line 6113 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(rtargs);
                    return (-1LL);
                }
#line 6114 "src/taumir/lower.tr"
                if ((LFunc_vreg_type(lf, rtv) == 5LL)) {
#line 6114 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(rtargs);
                    return (-1LL);
                }
#line 6115 "src/taumir/lower.tr"
                List_i64_append(rtargs, rtv);
#line 6116 "src/taumir/lower.tr"
                rti = (rti + 1LL);
            }
#line 6117 "src/taumir/lower.tr"
            LModule_add_extern(m, fn);
#line 6118 "src/taumir/lower.tr"
            long long rtret = _tag_of(m, hir_expr_type(e));
#line 6119 "src/taumir/lower.tr"
            if ((rtret < 0LL)) {
#line 6119 "src/taumir/lower.tr"
                rtret = 0LL;
            }
#line 6120 "src/taumir/lower.tr"
            long long rtd = LFunc_new_vreg(lf);
#line 6121 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(rtd, fn, rtargs));
#line 6122 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, rtd, rtret);
#line 6123 "src/taumir/lower.tr"
            if (((rtret == 1LL) && (!_is_rc_str_runtime_fn(fn)))) {
#line 6130 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 6131 "src/taumir/lower.tr"
                List_i64* rtwr = (void*)List_i64_new();
#line 6132 "src/taumir/lower.tr"
                List_i64_append(rtwr, rtd);
#line 6133 "src/taumir/lower.tr"
                long long rtwd = LFunc_new_vreg(lf);
#line 6134 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(rtwd, _tr_str_lit_len("_tr_rt_str_new", 14LL), rtwr));
#line 6135 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, rtwd, 1LL);
#line 6136 "src/taumir/lower.tr"
                long long _trret1715 = rtwd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1715;
            }
#line 6137 "src/taumir/lower.tr"
            long long _trret1716 = rtd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return _trret1716;
        }
#line 6140 "src/taumir/lower.tr"
        if ((!LModule_is_user_fn(m, fn))) {
#line 6141 "src/taumir/lower.tr"
            TrStr mbase = _mono_base(fn);
#line 6142 "src/taumir/lower.tr"
            if ((!_tr_str_eqv((mbase), (_tr_str_lit_len("", 0LL))))) {
#line 6143 "src/taumir/lower.tr"
                long long mgfi = _find_generic_fn(m, mbase);
#line 6144 "src/taumir/lower.tr"
                if ((mgfi >= 0LL)) {
#line 6145 "src/taumir/lower.tr"
                    AstType* mct = ({ TrStr _at_t6409 = (_mono_concrete(fn)); __auto_type _wr = (AstType_init(_at_t6409)); _tr_str_release(_at_t6409); _wr; });
#line 6146 "src/taumir/lower.tr"
                    if ((!_lir_lower_mono_fn(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, mgfi)), fn, mct))) {
#line 6147 "src/taumir/lower.tr"
                        _tr_str_release(alloc_nm);
                        _tr_str_release(fn);
                        _tr_str_release(mbase);
                        return (-1LL);
                    }
                }
            }
        }
#line 6148 "src/taumir/lower.tr"
        if ((!LModule_is_user_fn(m, fn))) {
#line 6149 "src/taumir/lower.tr"
            long long gfi = _find_generic_fn(m, fn);
#line 6150 "src/taumir/lower.tr"
            if ((gfi < 0LL)) {
#line 6150 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6151 "src/taumir/lower.tr"
            if (({ __auto_type _ord_t6410 = (args->len); __auto_type _wr = ((_ord_t6410 > _arg_limit(m))); _wr; })) {
#line 6151 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return (-1LL);
            }
#line 6152 "src/taumir/lower.tr"
            List_i64* gvregs = (void*)List_i64_new();
#line 6153 "src/taumir/lower.tr"
            List_i64* gtags = (void*)List_i64_new();
#line 6154 "src/taumir/lower.tr"
            List_TrStr* gcls = (void*)List_TrStr_new();
#line 6155 "src/taumir/lower.tr"
            TrStr gmangled = _tr_strx_concatv((fn), (_tr_str_lit_len("__m", 3LL)));
#line 6156 "src/taumir/lower.tr"
            long long gi = 0LL;
#line 6157 "src/taumir/lower.tr"
            while ((gi < args->len)) {
#line 6158 "src/taumir/lower.tr"
                long long gav = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, gi)));
#line 6159 "src/taumir/lower.tr"
                if ((gav < 0LL)) {
#line 6159 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(gvregs);
                    List_i64_free(gtags);
                    List_TrStr_free(gcls);
                    _tr_str_release(gmangled);
                    return (-1LL);
                }
#line 6160 "src/taumir/lower.tr"
                long long gat = LFunc_vreg_type(lf, gav);
#line 6161 "src/taumir/lower.tr"
                List_i64_append(gvregs, gav);
#line 6162 "src/taumir/lower.tr"
                List_i64_append(gtags, gat);
#line 6163 "src/taumir/lower.tr"
                TrStr gcn = _tr_str_lit_len("", 0LL);
#line 6164 "src/taumir/lower.tr"
                if (((gat == 10LL) || (gat == 11LL))) {
#line 6165 "src/taumir/lower.tr"
                    TrStr _strtmp_t6411 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(args, gi)));
                    _tr_str_release(gcn);
                    gcn = _strtmp_t6411;
#line 6170 "src/taumir/lower.tr"
                    if (_tr_str_eqv((gcn), (_tr_str_lit_len("", 0LL)))) {
#line 6170 "src/taumir/lower.tr"
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
#line 6171 "src/taumir/lower.tr"
                List_TrStr_append(gcls, gcn);
#line 6172 "src/taumir/lower.tr"
                if ((gi > 0LL)) {
#line 6172 "src/taumir/lower.tr"
                    ({ TrStr _ap_t6412 = (_tr_str_lit_len("_", 1LL)); _tr_str_append(&gmangled, _ap_t6412); });
                }
#line 6173 "src/taumir/lower.tr"
                ({ TrStr _ap_t6413 = (_tr_str_wrap(_tr_int_to_str((long long)(gat)))); _tr_str_append(&gmangled, _ap_t6413); _tr_str_release(_ap_t6413); });
#line 6174 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((gcn), (_tr_str_lit_len("", 0LL))))) {
#line 6174 "src/taumir/lower.tr"
                    ({ TrStr _ap_t6414 = (gcn); _tr_str_append(&gmangled, _ap_t6414); });
                }
#line 6175 "src/taumir/lower.tr"
                gi = (gi + 1LL);
                _tr_str_release(gcn);
            }
#line 6176 "src/taumir/lower.tr"
            if ((!LModule_is_user_fn(m, gmangled))) {
#line 6177 "src/taumir/lower.tr"
                if ((!_lir_lower_generic(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, gfi)), gtags, gcls, gmangled))) {
#line 6178 "src/taumir/lower.tr"
                    _tr_str_release(alloc_nm);
                    _tr_str_release(fn);
                    List_i64_free(gvregs);
                    List_i64_free(gtags);
                    List_TrStr_free(gcls);
                    _tr_str_release(gmangled);
                    return (-1LL);
                }
            }
#line 6179 "src/taumir/lower.tr"
            long long grtag = LModule_fn_ret_tag(m, gmangled);
#line 6180 "src/taumir/lower.tr"
            long long gd = LFunc_new_vreg(lf);
#line 6181 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(gd, gmangled, gvregs));
#line 6182 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, gd, grtag);
#line 6183 "src/taumir/lower.tr"
            if ((grtag == 1LL)) {
#line 6183 "src/taumir/lower.tr"
                _fresh_mark(lf, gd);
            }
#line 6184 "src/taumir/lower.tr"
            long long _trret1717 = gd;
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            _tr_str_release(gmangled);
            return _trret1717;
        }
#line 6185 "src/taumir/lower.tr"
        if (({ __auto_type _ord_t6415 = (args->len); __auto_type _wr = ((_ord_t6415 > _arg_limit(m))); _wr; })) {
#line 6185 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 6186 "src/taumir/lower.tr"
        long long rtag = LModule_fn_ret_tag(m, fn);
#line 6187 "src/taumir/lower.tr"
        if ((rtag < 0LL)) {
#line 6187 "src/taumir/lower.tr"
            _tr_str_release(alloc_nm);
            _tr_str_release(fn);
            return (-1LL);
        }
#line 6188 "src/taumir/lower.tr"
        List_i64* argvregs = (void*)List_i64_new();
#line 6189 "src/taumir/lower.tr"
        long long ai = 0LL;
#line 6190 "src/taumir/lower.tr"
        while ((ai < args->len)) {
#line 6191 "src/taumir/lower.tr"
            long long avr = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
#line 6192 "src/taumir/lower.tr"
            if ((avr < 0LL)) {
#line 6192 "src/taumir/lower.tr"
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                List_i64_free(argvregs);
                return (-1LL);
            }
#line 6193 "src/taumir/lower.tr"
            List_i64_append(argvregs, avr);
#line 6194 "src/taumir/lower.tr"
            ai = (ai + 1LL);
        }
#line 6195 "src/taumir/lower.tr"
        long long d4 = LFunc_new_vreg(lf);
#line 6196 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(d4, fn, argvregs));
#line 6197 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, d4, rtag);
#line 6198 "src/taumir/lower.tr"
        if ((rtag == 1LL)) {
#line 6203 "src/taumir/lower.tr"
            if ((_is_runtime_fn(fn) && (!_is_rc_str_runtime_fn(fn)))) {
#line 6204 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 6205 "src/taumir/lower.tr"
                List_i64* d4wr = (void*)List_i64_new();
#line 6206 "src/taumir/lower.tr"
                List_i64_append(d4wr, d4);
#line 6207 "src/taumir/lower.tr"
                long long d4wd = LFunc_new_vreg(lf);
#line 6208 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(d4wd, _tr_str_lit_len("_tr_rt_str_new", 14LL), d4wr));
#line 6209 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, d4wd, 1LL);
#line 6210 "src/taumir/lower.tr"
                _fresh_mark(lf, d4wd);
#line 6211 "src/taumir/lower.tr"
                long long _trret1718 = d4wd;
                _tr_str_release(alloc_nm);
                _tr_str_release(fn);
                return _trret1718;
            }
#line 6212 "src/taumir/lower.tr"
            _fresh_mark(lf, d4);
        }
#line 6213 "src/taumir/lower.tr"
        long long _trret1719 = d4;
        _tr_str_release(alloc_nm);
        _tr_str_release(fn);
        return _trret1719;
    } else if (_t6376.tag == HirExpr_ETuple) {
        __auto_type titems = _t6376.data.ETuple.items;
#line 6217 "src/taumir/lower.tr"
        if ((titems->len == 0LL)) {
#line 6219 "src/taumir/lower.tr"
            long long zsz = LFunc_new_vreg(lf);
#line 6220 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(zsz, 8LL));
#line 6221 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 6222 "src/taumir/lower.tr"
            List_i64* zba = (void*)List_i64_new();
#line 6223 "src/taumir/lower.tr"
            List_i64_append(zba, zsz);
#line 6224 "src/taumir/lower.tr"
            long long zblk = LFunc_new_vreg(lf);
#line 6225 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(zblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), zba));
#line 6226 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, zblk, 15LL);
#line 6227 "src/taumir/lower.tr"
            _fresh_mark_obj(lf, zblk);
#line 6228 "src/taumir/lower.tr"
            return zblk;
        }
#line 6229 "src/taumir/lower.tr"
        List_i64* tvals = (void*)List_i64_new();
#line 6230 "src/taumir/lower.tr"
        long long tj = 0LL;
#line 6231 "src/taumir/lower.tr"
        while ((tj < titems->len)) {
#line 6232 "src/taumir/lower.tr"
            long long tev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(titems, tj)));
#line 6233 "src/taumir/lower.tr"
            if ((tev < 0LL)) {
#line 6233 "src/taumir/lower.tr"
                List_i64_free(tvals);
                return (-1LL);
            }
#line 6234 "src/taumir/lower.tr"
            long long tevt = LFunc_vreg_type(lf, tev);
#line 6235 "src/taumir/lower.tr"
            if (((((_is_list_tag(tevt) || _is_dict_tag(tevt)) || _is_set_tag(tevt)) || (tevt == 12LL)) || (tevt == 15LL))) {
#line 6235 "src/taumir/lower.tr"
                List_i64_free(tvals);
                return (-1LL);
            }
#line 6236 "src/taumir/lower.tr"
            if ((tevt == 5LL)) {
#line 6237 "src/taumir/lower.tr"
                long long tfb = LFunc_new_vreg(lf);
#line 6238 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(tfb, tev));
#line 6239 "src/taumir/lower.tr"
                tev = tfb;
            } else if ((tevt == 1LL)) {
#line 6241 "src/taumir/lower.tr"
                _secure_str(m, lf, tev);
            } else if (((tevt == 10LL) || (tevt == 11LL))) {
#line 6243 "src/taumir/lower.tr"
                _secure_obj(m, lf, tev);
            }
#line 6244 "src/taumir/lower.tr"
            List_i64_append(tvals, tev);
#line 6245 "src/taumir/lower.tr"
            tj = (tj + 1LL);
        }
#line 6246 "src/taumir/lower.tr"
        long long tsz = LFunc_new_vreg(lf);
#line 6247 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(tsz, (titems->len * 8LL)));
#line 6248 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 6249 "src/taumir/lower.tr"
        List_i64* tba = (void*)List_i64_new();
#line 6250 "src/taumir/lower.tr"
        List_i64_append(tba, tsz);
#line 6251 "src/taumir/lower.tr"
        long long tblk = LFunc_new_vreg(lf);
#line 6252 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(tblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), tba));
#line 6253 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, tblk, 15LL);
#line 6254 "src/taumir/lower.tr"
        long long tk = 0LL;
#line 6255 "src/taumir/lower.tr"
        while ((tk < tvals->len)) {
#line 6256 "src/taumir/lower.tr"
            _emit_field_set(m, lf, tblk, (tk * 8LL), List_i64_get(tvals, tk));
#line 6257 "src/taumir/lower.tr"
            tk = (tk + 1LL);
        }
#line 6258 "src/taumir/lower.tr"
        long long _trret1720 = tblk;
        List_i64_free(tvals);
        return _trret1720;
    } else if (_t6376.tag == HirExpr_ESet) {
        __auto_type sitems = _t6376.data.ESet.items;
#line 6262 "src/taumir/lower.tr"
        if ((sitems->len == 0LL)) {
#line 6262 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6263 "src/taumir/lower.tr"
        long long sev0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(sitems, 0LL)));
#line 6264 "src/taumir/lower.tr"
        if ((sev0 < 0LL)) {
#line 6264 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6265 "src/taumir/lower.tr"
        long long setag = 13LL;
#line 6266 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, sev0) == 1LL)) {
#line 6266 "src/taumir/lower.tr"
            setag = 16LL;
        } else if ((LFunc_vreg_type(lf, sev0) != 0LL)) {
#line 6267 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6268 "src/taumir/lower.tr"
        TrStr snew = _set_sym(setag, _tr_str_lit_len("new", 3LL));
#line 6269 "src/taumir/lower.tr"
        LModule_add_extern(m, snew);
#line 6270 "src/taumir/lower.tr"
        long long shv = LFunc_new_vreg(lf);
#line 6271 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(shv, snew, (void*)List_i64_new()));
#line 6272 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, shv, setag);
#line 6273 "src/taumir/lower.tr"
        TrStr sadd = _set_sym(setag, _tr_str_lit_len("set", 3LL));
#line 6274 "src/taumir/lower.tr"
        LModule_add_extern(m, sadd);
#line 6275 "src/taumir/lower.tr"
        long long sone = LFunc_new_vreg(lf);
#line 6276 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(sone, 1LL));
#line 6277 "src/taumir/lower.tr"
        List_i64* sfirst = (void*)List_i64_new();
#line 6278 "src/taumir/lower.tr"
        List_i64_append(sfirst, shv);
#line 6279 "src/taumir/lower.tr"
        List_i64_append(sfirst, sev0);
#line 6280 "src/taumir/lower.tr"
        List_i64_append(sfirst, sone);
#line 6283 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sadd, sfirst));
#line 6284 "src/taumir/lower.tr"
        long long sli = 1LL;
#line 6285 "src/taumir/lower.tr"
        while ((sli < sitems->len)) {
#line 6286 "src/taumir/lower.tr"
            long long sev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(sitems, sli)));
#line 6287 "src/taumir/lower.tr"
            if ((sev < 0LL)) {
#line 6287 "src/taumir/lower.tr"
                _tr_str_release(snew);
                _tr_str_release(sadd);
                return (-1LL);
            }
#line 6288 "src/taumir/lower.tr"
            if (({ __auto_type _ord_t6416 = (LFunc_vreg_type(lf, sev)); __auto_type _wr = ((_ord_t6416 != LFunc_vreg_type(lf, sev0))); _wr; })) {
#line 6288 "src/taumir/lower.tr"
                _tr_str_release(snew);
                _tr_str_release(sadd);
                return (-1LL);
            }
#line 6289 "src/taumir/lower.tr"
            List_i64* sargs = (void*)List_i64_new();
#line 6290 "src/taumir/lower.tr"
            List_i64_append(sargs, shv);
#line 6291 "src/taumir/lower.tr"
            List_i64_append(sargs, sev);
#line 6292 "src/taumir/lower.tr"
            List_i64_append(sargs, sone);
#line 6293 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), sadd, sargs));
#line 6294 "src/taumir/lower.tr"
            sli = (sli + 1LL);
        }
#line 6295 "src/taumir/lower.tr"
        long long _trret1721 = shv;
        _tr_str_release(snew);
        _tr_str_release(sadd);
        return _trret1721;
    } else if (_t6376.tag == HirExpr_EList) {
        __auto_type items = _t6376.data.EList.items;
__auto_type lty = _t6376.data.EList.ty;
#line 6301 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
#line 6302 "src/taumir/lower.tr"
        long long hv = LFunc_new_vreg(lf);
#line 6303 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
#line 6304 "src/taumir/lower.tr"
        long long elem_t = 0LL;
#line 6305 "src/taumir/lower.tr"
        long long list_tag = 2LL;
#line 6306 "src/taumir/lower.tr"
        if ((items->len == 0LL)) {
#line 6307 "src/taumir/lower.tr"
            long long ann_tag = _tag_of(m, lty);
#line 6308 "src/taumir/lower.tr"
            if ((((((ann_tag == 2LL) || (ann_tag == 3LL)) || (ann_tag == 14LL)) || (ann_tag == 19LL)) || (ann_tag == 22LL))) {
#line 6309 "src/taumir/lower.tr"
                list_tag = ann_tag;
#line 6310 "src/taumir/lower.tr"
                elem_t = _list_elem_tag(ann_tag);
            } else if ((_tr_str_eqv((lty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((lty->name), (_tr_str_lit_len("Vec", 3LL))))) {
#line 6312 "src/taumir/lower.tr"
                if (((lty->args->len > 0LL) && _tr_str_eqv(((*((AstType**)List_ptr_get(lty->args, 0LL)))->name), (_tr_str_lit_len("bool", 4LL))))) {
#line 6313 "src/taumir/lower.tr"
                    list_tag = 22LL;
#line 6314 "src/taumir/lower.tr"
                    elem_t = 4LL;
                }
            }
        }
#line 6315 "src/taumir/lower.tr"
        long long li = 0LL;
#line 6316 "src/taumir/lower.tr"
        while ((li < items->len)) {
#line 6317 "src/taumir/lower.tr"
            long long ev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(items, li)));
#line 6318 "src/taumir/lower.tr"
            if ((ev < 0LL)) {
#line 6318 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 6319 "src/taumir/lower.tr"
            long long evt = LFunc_vreg_type(lf, ev);
#line 6320 "src/taumir/lower.tr"
            if ((((((evt != 0LL) && (evt != 1LL)) && (evt != 4LL)) && (evt != 5LL)) && (evt != 10LL))) {
#line 6320 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 6321 "src/taumir/lower.tr"
            long long eff_t = evt;
#line 6322 "src/taumir/lower.tr"
            if ((li == 0LL)) {
#line 6323 "src/taumir/lower.tr"
                elem_t = eff_t;
#line 6324 "src/taumir/lower.tr"
                list_tag = _list_tag_for_elem(elem_t);
            } else if ((eff_t != elem_t)) {
#line 6326 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 6327 "src/taumir/lower.tr"
            if ((evt == 1LL)) {
#line 6327 "src/taumir/lower.tr"
                _secure_str(m, lf, ev);
            }
#line 6328 "src/taumir/lower.tr"
            if ((((evt == 10LL) || (evt == 11LL)) || (evt == 15LL))) {
#line 6328 "src/taumir/lower.tr"
                _secure_obj(m, lf, ev);
            }
#line 6329 "src/taumir/lower.tr"
            if ((evt == 5LL)) {
#line 6330 "src/taumir/lower.tr"
                long long evb = LFunc_new_vreg(lf);
#line 6331 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(evb, ev));
#line 6332 "src/taumir/lower.tr"
                ev = evb;
            }
#line 6333 "src/taumir/lower.tr"
            TrStr push_sym = _tr_str_lit_len("_tr_rt_list_push_i64", 20LL);
#line 6334 "src/taumir/lower.tr"
            if ((list_tag == 22LL)) {
#line 6334 "src/taumir/lower.tr"
                TrStr _strtmp_t6417 = _tr_str_lit_len("_tr_rt_blist_push", 17LL);
                _tr_str_release(push_sym);
                push_sym = _strtmp_t6417;
            }
#line 6335 "src/taumir/lower.tr"
            LModule_add_extern(m, push_sym);
#line 6336 "src/taumir/lower.tr"
            List_i64* pa = (void*)List_i64_new();
#line 6337 "src/taumir/lower.tr"
            List_i64_append(pa, hv);
#line 6338 "src/taumir/lower.tr"
            List_i64_append(pa, ev);
#line 6339 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), push_sym, pa));
#line 6340 "src/taumir/lower.tr"
            li = (li + 1LL);
            _tr_str_release(push_sym);
        }
#line 6341 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, hv, list_tag);
#line 6342 "src/taumir/lower.tr"
        long long _trret1722 = hv;
        return _trret1722;
    } else if (_t6376.tag == HirExpr_EDict) {
        __auto_type keys = _t6376.data.EDict.keys;
__auto_type vals = _t6376.data.EDict.vals;
__auto_type dty = _t6376.data.EDict.ty;
#line 6344 "src/taumir/lower.tr"
        long long dtag = _ast_type_tag(dty);
#line 6345 "src/taumir/lower.tr"
        if ((keys->len > 0LL)) {
#line 6346 "src/taumir/lower.tr"
            long long k0 = _ast_type_tag(hir_expr_type(((HirExpr*)List_ptr_get(keys, 0LL))));
#line 6347 "src/taumir/lower.tr"
            long long v0 = _ast_type_tag(hir_expr_type(((HirExpr*)List_ptr_get(vals, 0LL))));
#line 6348 "src/taumir/lower.tr"
            if (((k0 == 1LL) && (v0 == 0LL))) {
#line 6348 "src/taumir/lower.tr"
                dtag = 6LL;
            } else if (((k0 == 0LL) && (v0 == 0LL))) {
#line 6349 "src/taumir/lower.tr"
                dtag = 7LL;
            } else if (((k0 == 1LL) && (v0 == 1LL))) {
#line 6350 "src/taumir/lower.tr"
                dtag = 8LL;
            } else if (((k0 == 0LL) && (v0 == 1LL))) {
#line 6351 "src/taumir/lower.tr"
                dtag = 9LL;
            } else if (((k0 == 1LL) && (v0 == 5LL))) {
#line 6352 "src/taumir/lower.tr"
                dtag = 17LL;
            } else if (((k0 == 0LL) && (v0 == 5LL))) {
#line 6353 "src/taumir/lower.tr"
                dtag = 18LL;
            } else {
#line 6354 "src/taumir/lower.tr"
                return (-1LL);
            }
        } else if (((_tr_str_eqv((dty->name), (_tr_str_lit_len("Dict", 4LL))) || _tr_str_eqv((dty->name), (_tr_str_lit_len("Map", 3LL)))) && (dty->args->len == 0LL))) {
#line 6357 "src/taumir/lower.tr"
            dtag = 8LL;
        }
#line 6358 "src/taumir/lower.tr"
        if ((!_is_dict_tag(dtag))) {
#line 6359 "src/taumir/lower.tr"
            TrStr kn = _tr_str_lit_len("?", 1LL);
#line 6359 "src/taumir/lower.tr"
            TrStr vn = _tr_str_lit_len("?", 1LL);
#line 6360 "src/taumir/lower.tr"
            if ((dty->args->len > 0LL)) {
#line 6360 "src/taumir/lower.tr"
                TrStr _strtmp_t6418 = _tr_str_retain((*((AstType**)List_ptr_get(dty->args, 0LL)))->name);
                _tr_str_release(kn);
                kn = _strtmp_t6418;
            }
#line 6361 "src/taumir/lower.tr"
            if ((dty->args->len > 1LL)) {
#line 6361 "src/taumir/lower.tr"
                TrStr _strtmp_t6419 = _tr_str_retain((*((AstType**)List_ptr_get(dty->args, 1LL)))->name);
                _tr_str_release(vn);
                vn = _strtmp_t6419;
            }
#line 6362 "src/taumir/lower.tr"
            m->fail_note = ({ __auto_type _ord_t6420 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(dtag)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("bad dict tag dtag=", 18LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ty=", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6420); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" k=", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" v=", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 6363 "src/taumir/lower.tr"
            _tr_str_release(kn);
            _tr_str_release(vn);
            return (-1LL);
        }
#line 6364 "src/taumir/lower.tr"
        TrStr dnew = _dict_new_sym(dtag);
#line 6365 "src/taumir/lower.tr"
        LModule_add_extern(m, dnew);
#line 6366 "src/taumir/lower.tr"
        long long dhv = LFunc_new_vreg(lf);
#line 6367 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(dhv, dnew, (void*)List_i64_new()));
#line 6368 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, dhv, dtag);
#line 6369 "src/taumir/lower.tr"
        bool kstr = _dict_key_is_str(dtag);
#line 6370 "src/taumir/lower.tr"
        long long vtag = _dict_val_tag(dtag);
#line 6371 "src/taumir/lower.tr"
        long long di = 0LL;
#line 6372 "src/taumir/lower.tr"
        while ((di < keys->len)) {
#line 6373 "src/taumir/lower.tr"
            long long kv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(keys, di)));
#line 6374 "src/taumir/lower.tr"
            if ((kv < 0LL)) {
#line 6374 "src/taumir/lower.tr"
                _tr_str_release(dnew);
                return (-1LL);
            }
#line 6375 "src/taumir/lower.tr"
            if ((kstr && (LFunc_vreg_type(lf, kv) != 1LL))) {
#line 6375 "src/taumir/lower.tr"
                _tr_str_release(dnew);
                return (-1LL);
            }
#line 6376 "src/taumir/lower.tr"
            if (((!kstr) && (LFunc_vreg_type(lf, kv) != 0LL))) {
#line 6376 "src/taumir/lower.tr"
                _tr_str_release(dnew);
                return (-1LL);
            }
#line 6377 "src/taumir/lower.tr"
            long long vv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(vals, di)));
#line 6378 "src/taumir/lower.tr"
            if ((vv < 0LL)) {
#line 6378 "src/taumir/lower.tr"
                _tr_str_release(dnew);
                return (-1LL);
            }
#line 6379 "src/taumir/lower.tr"
            if ((LFunc_vreg_type(lf, vv) != vtag)) {
#line 6379 "src/taumir/lower.tr"
                _tr_str_release(dnew);
                return (-1LL);
            }
#line 6380 "src/taumir/lower.tr"
            if ((vtag == 1LL)) {
#line 6380 "src/taumir/lower.tr"
                _secure_str(m, lf, vv);
            }
#line 6381 "src/taumir/lower.tr"
            if ((vtag == 5LL)) {
#line 6382 "src/taumir/lower.tr"
                long long dvb = LFunc_new_vreg(lf);
#line 6383 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(dvb, vv));
#line 6384 "src/taumir/lower.tr"
                vv = dvb;
            }
#line 6385 "src/taumir/lower.tr"
            TrStr dset = _dict_sym(dtag, _tr_str_lit_len("set", 3LL));
#line 6386 "src/taumir/lower.tr"
            LModule_add_extern(m, dset);
#line 6387 "src/taumir/lower.tr"
            List_i64* dsa = (void*)List_i64_new();
#line 6388 "src/taumir/lower.tr"
            List_i64_append(dsa, dhv);
#line 6389 "src/taumir/lower.tr"
            List_i64_append(dsa, kv);
#line 6390 "src/taumir/lower.tr"
            List_i64_append(dsa, vv);
#line 6391 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), dset, dsa));
#line 6392 "src/taumir/lower.tr"
            di = (di + 1LL);
            _tr_str_release(dset);
        }
#line 6393 "src/taumir/lower.tr"
        long long _trret1723 = dhv;
        _tr_str_release(dnew);
        return _trret1723;
    } else if (_t6376.tag == HirExpr_EFString) {
        __auto_type parts = _t6376.data.EFString.parts;
#line 6397 "src/taumir/lower.tr"
        long long acc = _heap_lit(m, lf, _tr_str_lit_len("", 0LL));
#line 6398 "src/taumir/lower.tr"
        long long fi = 0LL;
#line 6399 "src/taumir/lower.tr"
        while ((fi < parts->len)) {
#line 6400 "src/taumir/lower.tr"
            HirFStringPart* fp = ((HirFStringPart*)List_ptr_get(parts, fi));
#line 6401 "src/taumir/lower.tr"
            long long pr = (-1LL);
#line 6402 "src/taumir/lower.tr"
            if (fp->is_expr) {
#line 6403 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((fp->fmt_spec), (_tr_str_lit_len("", 0LL))))) {
#line 6406 "src/taumir/lower.tr"
                    long long spev = lower_expr(m, lf, fp->expr);
#line 6407 "src/taumir/lower.tr"
                    if ((spev < 0LL)) {
#line 6407 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6408 "src/taumir/lower.tr"
                    long long spidx = ({ __auto_type _ord_t6421 = (fp->fmt_spec); __auto_type _ord_t6422 = (_tr_str_lenv((fp->fmt_spec))); __auto_type _wr = (LModule_add_string(m, _ord_t6421, _ord_t6422)); _wr; });
#line 6409 "src/taumir/lower.tr"
                    long long specv = LFunc_new_vreg(lf);
#line 6410 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IStr(specv, spidx));
#line 6411 "src/taumir/lower.tr"
                    long long spvt = LFunc_vreg_type(lf, spev);
#line 6412 "src/taumir/lower.tr"
                    if ((spvt == 5LL)) {
#line 6413 "src/taumir/lower.tr"
                        long long spbits = LFunc_new_vreg(lf);
#line 6414 "src/taumir/lower.tr"
                        LFunc_emit(lf, LInst_ctor_IFBits(spbits, spev));
#line 6415 "src/taumir/lower.tr"
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_f64", 19LL), spbits, specv, 1LL);
                    } else if ((spvt == 1LL)) {
#line 6417 "src/taumir/lower.tr"
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_str", 19LL), spev, specv, 1LL);
                    } else if (((spvt == 0LL) || (spvt == 4LL))) {
#line 6419 "src/taumir/lower.tr"
                        pr = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_fmt_spec_i64", 19LL), spev, specv, 1LL);
                    } else {
#line 6421 "src/taumir/lower.tr"
                        return (-1LL);
                    }
                } else {
#line 6424 "src/taumir/lower.tr"
                    TrStr fcls = _recv_class(m, lf, fp->expr);
#line 6425 "src/taumir/lower.tr"
                    if ((((!_tr_str_eqv((fcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, fcls)) && ({ TrStr _cmpa_t6423 = (LModule_resolve_method(m, fcls, _tr_str_lit_len("__str__", 7LL))); TrStr _cmpb_t6424 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6425 = !_tr_str_eqv(_cmpa_t6423, _cmpb_t6424); _tr_str_release(_cmpa_t6423); _cmpr_t6425; }))) {
#line 6426 "src/taumir/lower.tr"
                        pr = _obj_to_str(m, lf, fp->expr, (-1LL));
#line 6427 "src/taumir/lower.tr"
                        if ((pr < 0LL)) {
#line 6427 "src/taumir/lower.tr"
                            _tr_str_release(fcls);
                            return (-1LL);
                        }
                    } else {
#line 6429 "src/taumir/lower.tr"
                        long long fev = lower_expr(m, lf, fp->expr);
#line 6430 "src/taumir/lower.tr"
                        if ((fev < 0LL)) {
#line 6430 "src/taumir/lower.tr"
                            _tr_str_release(fcls);
                            return (-1LL);
                        }
#line 6431 "src/taumir/lower.tr"
                        if ((_tr_str_eqv((hir_expr_type(fp->expr)->name), (_tr_str_lit_len("char", 4LL))) && (LFunc_vreg_type(lf, fev) == 0LL))) {
#line 6432 "src/taumir/lower.tr"
                            pr = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_char_to_str", 18LL), fev, 1LL);
                        } else {
#line 6434 "src/taumir/lower.tr"
                            pr = _reg_to_str(m, lf, fev);
                        }
#line 6435 "src/taumir/lower.tr"
                        if ((pr < 0LL)) {
#line 6435 "src/taumir/lower.tr"
                            _tr_str_release(fcls);
                            return (-1LL);
                        }
                    }
                }
            } else {
#line 6437 "src/taumir/lower.tr"
                pr = _heap_lit_len(m, lf, fp->text, fp->text_len);
            }
#line 6438 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_concat", 17LL));
#line 6439 "src/taumir/lower.tr"
            List_i64* fca = (void*)List_i64_new();
#line 6440 "src/taumir/lower.tr"
            List_i64_append(fca, acc);
#line 6441 "src/taumir/lower.tr"
            List_i64_append(fca, pr);
#line 6442 "src/taumir/lower.tr"
            long long fdc = LFunc_new_vreg(lf);
#line 6443 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(fdc, _tr_str_lit_len("_tr_rt_str_concat", 17LL), fca));
#line 6444 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, fdc, 1LL);
#line 6445 "src/taumir/lower.tr"
            _fresh_mark(lf, fdc);
#line 6446 "src/taumir/lower.tr"
            acc = fdc;
#line 6447 "src/taumir/lower.tr"
            fi = (fi + 1LL);
        }
#line 6448 "src/taumir/lower.tr"
        long long _trret1724 = acc;
        return _trret1724;
    } else if (_t6376.tag == HirExpr_EClosure) {
        __auto_type cparams = _t6376.data.EClosure.params;
__auto_type cret = _t6376.data.EClosure.ret_ty;
__auto_type cbody = _t6376.data.EClosure.body;
__auto_type cis_async = _t6376.data.EClosure.is_async;
__auto_type ccaps = _t6376.data.EClosure.captures;
#line 6454 "src/taumir/lower.tr"
        if (cis_async) {
#line 6454 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6455 "src/taumir/lower.tr"
        long long crtag = _tag_of(m, cret);
#line 6456 "src/taumir/lower.tr"
        if ((crtag < 0LL)) {
#line 6456 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6457 "src/taumir/lower.tr"
        if (((cparams->len + 1LL) > 6LL)) {
#line 6457 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6458 "src/taumir/lower.tr"
        TrStr cname = ({ __auto_type _ord_t6426 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(m->funcs->len)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_tr_clo_", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t6426); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lf->name)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 6459 "src/taumir/lower.tr"
        LFunc* clf = ({ TrStr _at_t6427 = (_own(cname)); __auto_type _wr = (LFunc_init(_at_t6427)); _tr_str_release(_at_t6427); _wr; });
#line 6460 "src/taumir/lower.tr"
        List_TrStr_append(clf->params, _tr_str_lit_len("__env", 5LL));
#line 6461 "src/taumir/lower.tr"
        LFunc_add_var(clf, _tr_str_lit_len("__env", 5LL));
#line 6462 "src/taumir/lower.tr"
        long long cpj = 0LL;
#line 6463 "src/taumir/lower.tr"
        while ((cpj < cparams->len)) {
#line 6464 "src/taumir/lower.tr"
            HirParam* cp = ((HirParam*)List_ptr_get(cparams, cpj));
#line 6465 "src/taumir/lower.tr"
            long long cptag = _tag_of(m, cp->ty);
#line 6466 "src/taumir/lower.tr"
            if ((cptag < 0LL)) {
#line 6466 "src/taumir/lower.tr"
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
#line 6467 "src/taumir/lower.tr"
            List_TrStr_append(clf->params, cp->name);
#line 6468 "src/taumir/lower.tr"
            LFunc_add_var(clf, cp->name);
#line 6469 "src/taumir/lower.tr"
            LFunc_set_var_type(clf, cp->name, cptag);
#line 6470 "src/taumir/lower.tr"
            if ((((cptag == 10LL) || (cptag == 11LL)) && (!_is_null_str(cp->ty->name)))) {
#line 6471 "src/taumir/lower.tr"
                ({ __auto_type _ord_t6428 = (cp->name); TrStr _ord_t6429 = (_own(cp->ty->name)); LFunc_set_var_cls(clf, _ord_t6428, _ord_t6429); _tr_str_release(_ord_t6429); });
            }
#line 6472 "src/taumir/lower.tr"
            cpj = (cpj + 1LL);
        }
#line 6474 "src/taumir/lower.tr"
        long long ccj = 0LL;
#line 6475 "src/taumir/lower.tr"
        while ((ccj < ccaps->len)) {
#line 6476 "src/taumir/lower.tr"
            HirParam* cc = ((HirParam*)List_ptr_get(ccaps, ccj));
#line 6477 "src/taumir/lower.tr"
            if ((LFunc_var_index(lf, cc->name) < 0LL)) {
#line 6477 "src/taumir/lower.tr"
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
#line 6478 "src/taumir/lower.tr"
            long long cctag = LFunc_var_type(lf, cc->name);
#line 6479 "src/taumir/lower.tr"
            if (((((cctag != 0LL) && (cctag != 1LL)) && (cctag != 4LL)) && (cctag != 5LL))) {
#line 6479 "src/taumir/lower.tr"
                _tr_str_release(cname);
                _tr_obj_release(clf, _trdrop_LFunc);
                return (-1LL);
            }
#line 6480 "src/taumir/lower.tr"
            ({ TrStr _ord_t6430 = (_own(cc->name)); List_TrStr_append(clf->captures, _ord_t6430); _tr_str_release(_ord_t6430); });
#line 6481 "src/taumir/lower.tr"
            List_i64_append(clf->cap_tags, cctag);
#line 6482 "src/taumir/lower.tr"
            ccj = (ccj + 1LL);
        }
#line 6483 "src/taumir/lower.tr"
        ({ TrStr _ord_t6431 = (_own(cname)); List_TrStr_append(m->fn_names, _ord_t6431); _tr_str_release(_ord_t6431); });
#line 6484 "src/taumir/lower.tr"
        List_i64_append(m->fn_ret, crtag);
#line 6485 "src/taumir/lower.tr"
        LFunc_set_cur(clf, LFunc_new_block(clf));
#line 6486 "src/taumir/lower.tr"
        if ((!lower_block(m, clf, cbody))) {
#line 6486 "src/taumir/lower.tr"
            _tr_str_release(cname);
            _tr_obj_release(clf, _trdrop_LFunc);
            return (-1LL);
        }
#line 6487 "src/taumir/lower.tr"
        LFunc_set_term(clf, LTerm_ctor_TRetInt(0LL));
#line 6488 "src/taumir/lower.tr"
        List_ptr_append(m->funcs, _tr_obj_retain(clf));
#line 6490 "src/taumir/lower.tr"
        long long cblk_sz = LFunc_new_vreg(lf);
#line 6491 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_IConst(cblk_sz, ((1LL + ccaps->len) * 8LL)));
#line 6492 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL));
#line 6493 "src/taumir/lower.tr"
        List_i64* cba = (void*)List_i64_new();
#line 6494 "src/taumir/lower.tr"
        List_i64_append(cba, cblk_sz);
#line 6495 "src/taumir/lower.tr"
        long long cblk = LFunc_new_vreg(lf);
#line 6496 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(cblk, _tr_str_lit_len("_tr_rt_obj_alloc", 16LL), cba));
#line 6497 "src/taumir/lower.tr"
        LFunc_set_vreg_type(lf, cblk, 12LL);
#line 6498 "src/taumir/lower.tr"
        long long cfa = LFunc_new_vreg(lf);
#line 6499 "src/taumir/lower.tr"
        ({ TrStr _at_t6432 = (_own(cname)); LFunc_emit(lf, LInst_ctor_IFuncAddr(cfa, _at_t6432)); _tr_str_release(_at_t6432); });
#line 6500 "src/taumir/lower.tr"
        _emit_field_set(m, lf, cblk, 0LL, cfa);
#line 6501 "src/taumir/lower.tr"
        long long cbi = 0LL;
#line 6502 "src/taumir/lower.tr"
        while ((cbi < ccaps->len)) {
#line 6503 "src/taumir/lower.tr"
            long long cav = LFunc_new_vreg(lf);
#line 6504 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IAddrVar(cav, ((HirParam*)List_ptr_get(ccaps, cbi))->name));
#line 6505 "src/taumir/lower.tr"
            _emit_field_set(m, lf, cblk, ((1LL + cbi) * 8LL), cav);
#line 6506 "src/taumir/lower.tr"
            cbi = (cbi + 1LL);
        }
#line 6507 "src/taumir/lower.tr"
        LFunc_set_vreg_xret(lf, cblk, crtag);
#line 6508 "src/taumir/lower.tr"
        long long _trret1725 = cblk;
        _tr_str_release(cname);
        _tr_obj_release(clf, _trdrop_LFunc);
        return _trret1725;
    } else if (_t6376.tag == HirExpr_EIndex) {
        __auto_type obj = _t6376.data.EIndex.obj;
__auto_type idx = _t6376.data.EIndex._tr_v_index;
#line 6511 "src/taumir/lower.tr"
        TrStr gicls = _recv_class(m, lf, obj);
#line 6512 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((gicls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, gicls))) {
#line 6513 "src/taumir/lower.tr"
            TrStr gim = LModule_resolve_method(m, gicls, _tr_str_lit_len("__getitem__", 11LL));
#line 6514 "src/taumir/lower.tr"
            if ((!_tr_str_eqv((gim), (_tr_str_lit_len("", 0LL))))) {
#line 6515 "src/taumir/lower.tr"
                long long giself = lower_expr(m, lf, obj);
#line 6516 "src/taumir/lower.tr"
                if ((giself < 0LL)) {
#line 6516 "src/taumir/lower.tr"
                    _tr_str_release(gicls);
                    _tr_str_release(gim);
                    return (-1LL);
                }
#line 6517 "src/taumir/lower.tr"
                List_ptr* giargs = (void*)List_ptr_new();
#line 6518 "src/taumir/lower.tr"
                List_ptr_append(giargs, idx);
#line 6519 "src/taumir/lower.tr"
                long long _trret1726 = _lower_obj_call(m, lf, gim, giself, giargs);
                _tr_str_release(gicls);
                _tr_str_release(gim);
                return _trret1726;
            }
        }
#line 6522 "src/taumir/lower.tr"
        TrStr arn = _ident_name(obj);
#line 6523 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((arn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, arn) > 0LL))) {
#line 6524 "src/taumir/lower.tr"
            long long ariv = lower_expr(m, lf, idx);
#line 6525 "src/taumir/lower.tr"
            if (((ariv < 0LL) || (LFunc_vreg_type(lf, ariv) != 0LL))) {
#line 6525 "src/taumir/lower.tr"
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
#line 6526 "src/taumir/lower.tr"
            long long araddr = _array_elem_addr(m, lf, arn, ariv);
#line 6527 "src/taumir/lower.tr"
            long long ard = LFunc_new_vreg(lf);
#line 6528 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ILoad(ard, araddr, 0LL, 2LL));
#line 6529 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, ard, _fixed_arr_elem_tag(m, lf, arn));
#line 6530 "src/taumir/lower.tr"
            long long _trret1727 = ard;
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return _trret1727;
        }
#line 6531 "src/taumir/lower.tr"
        long long ov = lower_expr(m, lf, obj);
#line 6532 "src/taumir/lower.tr"
        if ((ov < 0LL)) {
#line 6532 "src/taumir/lower.tr"
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return (-1LL);
        }
#line 6533 "src/taumir/lower.tr"
        long long ovt = LFunc_vreg_type(lf, ov);
#line 6534 "src/taumir/lower.tr"
        if (_is_dict_tag(ovt)) {
#line 6535 "src/taumir/lower.tr"
            long long dkv = lower_expr(m, lf, idx);
#line 6536 "src/taumir/lower.tr"
            if ((dkv < 0LL)) {
#line 6536 "src/taumir/lower.tr"
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
#line 6537 "src/taumir/lower.tr"
            if ((_dict_key_is_str(ovt) && (LFunc_vreg_type(lf, dkv) != 1LL))) {
#line 6537 "src/taumir/lower.tr"
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
#line 6538 "src/taumir/lower.tr"
            if (((!_dict_key_is_str(ovt)) && (LFunc_vreg_type(lf, dkv) != 0LL))) {
#line 6538 "src/taumir/lower.tr"
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
#line 6539 "src/taumir/lower.tr"
            TrStr dget = _dict_sym(ovt, _tr_str_lit_len("get", 3LL));
#line 6540 "src/taumir/lower.tr"
            LModule_add_extern(m, dget);
#line 6541 "src/taumir/lower.tr"
            List_i64* dga = (void*)List_i64_new();
#line 6542 "src/taumir/lower.tr"
            List_i64_append(dga, ov);
#line 6543 "src/taumir/lower.tr"
            List_i64_append(dga, dkv);
#line 6544 "src/taumir/lower.tr"
            long long dgd = LFunc_new_vreg(lf);
#line 6545 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(dgd, dget, dga));
#line 6546 "src/taumir/lower.tr"
            if ((_dict_val_tag(ovt) == 5LL)) {
#line 6547 "src/taumir/lower.tr"
                long long dgf = LFunc_new_vreg(lf);
#line 6548 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(dgf, dgd));
#line 6549 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, dgf, 5LL);
#line 6550 "src/taumir/lower.tr"
                long long _trret1728 = dgf;
                _tr_str_release(gicls);
                _tr_str_release(arn);
                _tr_str_release(dget);
                return _trret1728;
            }
#line 6551 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, dgd, _dict_val_tag(ovt));
#line 6552 "src/taumir/lower.tr"
            long long _trret1729 = dgd;
            _tr_str_release(gicls);
            _tr_str_release(arn);
            _tr_str_release(dget);
            return _trret1729;
        }
#line 6553 "src/taumir/lower.tr"
        if ((ovt == 1LL)) {
#line 6554 "src/taumir/lower.tr"
            long long siv = lower_expr(m, lf, idx);
#line 6555 "src/taumir/lower.tr"
            if ((siv < 0LL)) {
#line 6555 "src/taumir/lower.tr"
                _tr_str_release(gicls);
                _tr_str_release(arn);
                return (-1LL);
            }
#line 6556 "src/taumir/lower.tr"
            long long _trret1730 = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_char_at", 18LL), ov, siv, 0LL);
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return _trret1730;
        }
#line 6557 "src/taumir/lower.tr"
        if ((!_is_list_tag(ovt))) {
#line 6557 "src/taumir/lower.tr"
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return (-1LL);
        }
#line 6558 "src/taumir/lower.tr"
        long long iv = lower_expr(m, lf, idx);
#line 6559 "src/taumir/lower.tr"
        if ((iv < 0LL)) {
#line 6559 "src/taumir/lower.tr"
            _tr_str_release(gicls);
            _tr_str_release(arn);
            return (-1LL);
        }
#line 6560 "src/taumir/lower.tr"
        long long _trret1731 = _list_get_elem(m, lf, ovt, ov, iv);
        _tr_str_release(gicls);
        _tr_str_release(arn);
        return _trret1731;
    } else if (_t6376.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t6376.data.EPropAccess.obj;
__auto_type prop = _t6376.data.EPropAccess.prop;
#line 6563 "src/taumir/lower.tr"
        __auto_type _t6433 = (*obj);
        if (_t6433.tag == HirExpr_EIdent) {
            __auto_type uvn = _t6433.data.EIdent.name;
#line 6565 "src/taumir/lower.tr"
            if ((LModule_is_enum(m, uvn) && ({ TrStr _at_t6434 = (_norm_variant(uvn, prop)); __auto_type _wr = ((LModule_enum_variant_index(m, uvn, _at_t6434) >= 0LL)); _tr_str_release(_at_t6434); _wr; }))) {
#line 6566 "src/taumir/lower.tr"
                long long _trret1732 = ({ TrStr _at_t6435 = (_norm_variant(uvn, prop)); __auto_type _wr = (_lower_enum_ctor(m, lf, uvn, _at_t6435, (void*)List_ptr_new())); _tr_str_release(_at_t6435); _wr; });
                return _trret1732;
            }
        } else if (1) {
            __auto_type _ = _t6433;
#line 6568 "src/taumir/lower.tr"
            /* pass */
        }
#line 6571 "src/taumir/lower.tr"
        if (_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL)))) {
#line 6572 "src/taumir/lower.tr"
            TrStr aln = _ident_name(obj);
#line 6573 "src/taumir/lower.tr"
            if (((!_tr_str_eqv((aln), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, aln) > 0LL))) {
#line 6574 "src/taumir/lower.tr"
                long long ald = LFunc_new_vreg(lf);
#line 6575 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IConst(ald, _fixed_arr_len(m, lf, aln)));
#line 6576 "src/taumir/lower.tr"
                long long _trret1733 = ald;
                _tr_str_release(aln);
                return _trret1733;
            }
        }
#line 6581 "src/taumir/lower.tr"
        TrStr pcls = _recv_class(m, lf, obj);
#line 6582 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((pcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_class(m, pcls))) {
#line 6583 "src/taumir/lower.tr"
            long long foff = LModule_field_offset(m, pcls, prop);
#line 6584 "src/taumir/lower.tr"
            if ((foff < 0LL)) {
#line 6584 "src/taumir/lower.tr"
                _tr_str_release(pcls);
                return (-1LL);
            }
#line 6585 "src/taumir/lower.tr"
            long long ftg = LModule_field_tag(m, pcls, prop);
#line 6586 "src/taumir/lower.tr"
            if ((ftg < 0LL)) {
#line 6586 "src/taumir/lower.tr"
                _tr_str_release(pcls);
                return (-1LL);
            }
#line 6587 "src/taumir/lower.tr"
            long long fobj = lower_expr(m, lf, obj);
#line 6588 "src/taumir/lower.tr"
            if ((fobj < 0LL)) {
#line 6588 "src/taumir/lower.tr"
                _tr_str_release(pcls);
                return (-1LL);
            }
#line 6589 "src/taumir/lower.tr"
            if ((ftg == 5LL)) {
#line 6590 "src/taumir/lower.tr"
                long long fraw = _emit_field_get(m, lf, fobj, foff, 0LL);
#line 6591 "src/taumir/lower.tr"
                long long ffv = LFunc_new_vreg(lf);
#line 6592 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(ffv, fraw));
#line 6593 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, ffv, 5LL);
#line 6594 "src/taumir/lower.tr"
                long long _trret1734 = ffv;
                _tr_str_release(pcls);
                return _trret1734;
            }
#line 6595 "src/taumir/lower.tr"
            long long _trret1735 = _emit_field_get(m, lf, fobj, foff, ftg);
            _tr_str_release(pcls);
            return _trret1735;
        }
#line 6596 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((pcls), (_tr_str_lit_len("", 0LL)))) && LModule_is_enum(m, pcls))) {
#line 6597 "src/taumir/lower.tr"
            long long ep = _lower_enum_prop(m, lf, obj, pcls, prop);
#line 6598 "src/taumir/lower.tr"
            if ((ep >= 0LL)) {
#line 6598 "src/taumir/lower.tr"
                long long _trret1736 = ep;
                _tr_str_release(pcls);
                return _trret1736;
            }
        }
#line 6599 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((prop), (_tr_str_lit_len("len", 3LL))))) {
#line 6599 "src/taumir/lower.tr"
            _tr_str_release(pcls);
            return (-1LL);
        }
#line 6600 "src/taumir/lower.tr"
        long long ovl = lower_expr(m, lf, obj);
#line 6601 "src/taumir/lower.tr"
        if ((ovl < 0LL)) {
#line 6601 "src/taumir/lower.tr"
            _tr_str_release(pcls);
            return (-1LL);
        }
#line 6602 "src/taumir/lower.tr"
        if (_is_set_tag(LFunc_vreg_type(lf, ovl))) {
#line 6603 "src/taumir/lower.tr"
            TrStr pslsym = _set_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit_len("len", 3LL));
#line 6604 "src/taumir/lower.tr"
            LModule_add_extern(m, pslsym);
#line 6605 "src/taumir/lower.tr"
            List_i64* psla = (void*)List_i64_new();
#line 6606 "src/taumir/lower.tr"
            List_i64_append(psla, ovl);
#line 6607 "src/taumir/lower.tr"
            long long psld = LFunc_new_vreg(lf);
#line 6608 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(psld, pslsym, psla));
#line 6609 "src/taumir/lower.tr"
            long long _trret1737 = psld;
            _tr_str_release(pcls);
            _tr_str_release(pslsym);
            return _trret1737;
        }
#line 6610 "src/taumir/lower.tr"
        if (_is_dict_tag(LFunc_vreg_type(lf, ovl))) {
#line 6611 "src/taumir/lower.tr"
            TrStr dlsym = _dict_sym(LFunc_vreg_type(lf, ovl), _tr_str_lit_len("len", 3LL));
#line 6612 "src/taumir/lower.tr"
            LModule_add_extern(m, dlsym);
#line 6613 "src/taumir/lower.tr"
            List_i64* dla = (void*)List_i64_new();
#line 6614 "src/taumir/lower.tr"
            List_i64_append(dla, ovl);
#line 6615 "src/taumir/lower.tr"
            long long dld = LFunc_new_vreg(lf);
#line 6616 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(dld, dlsym, dla));
#line 6617 "src/taumir/lower.tr"
            long long _trret1738 = dld;
            _tr_str_release(pcls);
            _tr_str_release(dlsym);
            return _trret1738;
        }
#line 6618 "src/taumir/lower.tr"
        if ((LFunc_vreg_type(lf, ovl) == 1LL)) {
#line 6619 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_strlen", 13LL));
#line 6620 "src/taumir/lower.tr"
            List_i64* sla = (void*)List_i64_new();
#line 6621 "src/taumir/lower.tr"
            List_i64_append(sla, ovl);
#line 6622 "src/taumir/lower.tr"
            long long sld = LFunc_new_vreg(lf);
#line 6623 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit_len("_tr_rt_strlen", 13LL), sla));
#line 6624 "src/taumir/lower.tr"
            long long _trret1739 = sld;
            _tr_str_release(pcls);
            return _trret1739;
        }
#line 6625 "src/taumir/lower.tr"
        if ((!_is_list_tag(LFunc_vreg_type(lf, ovl)))) {
#line 6625 "src/taumir/lower.tr"
            _tr_str_release(pcls);
            return (-1LL);
        }
#line 6626 "src/taumir/lower.tr"
        LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_len", 15LL));
#line 6627 "src/taumir/lower.tr"
        List_i64* la = (void*)List_i64_new();
#line 6628 "src/taumir/lower.tr"
        List_i64_append(la, ovl);
#line 6629 "src/taumir/lower.tr"
        long long ld = LFunc_new_vreg(lf);
#line 6630 "src/taumir/lower.tr"
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit_len("_tr_rt_list_len", 15LL), la));
#line 6631 "src/taumir/lower.tr"
        long long _trret1740 = ld;
        _tr_str_release(pcls);
        return _trret1740;
    } else if (_t6376.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t6376.data.EMethodCall.obj;
__auto_type method = _t6376.data.EMethodCall.method;
__auto_type margs = _t6376.data.EMethodCall.args;
#line 6635 "src/taumir/lower.tr"
        __auto_type _t6436 = (*obj);
        if (_t6436.tag == HirExpr_EIdent) {
            __auto_type styn = _t6436.data.EIdent.name;
#line 6637 "src/taumir/lower.tr"
            if (_tr_str_eqv((styn), (_tr_str_lit_len("Coro", 4LL)))) {
#line 6639 "src/taumir/lower.tr"
                if (_tr_str_eqv((method), (_tr_str_lit_len("yield_now", 9LL)))) {
#line 6640 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_yield_h", 14LL));
#line 6641 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_yield_h", 14LL), (void*)List_i64_new()));
#line 6642 "src/taumir/lower.tr"
                    long long cyd = LFunc_new_vreg(lf);
#line 6643 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IConst(cyd, 0LL));
#line 6644 "src/taumir/lower.tr"
                    long long _trret1741 = cyd;
                    return _trret1741;
                }
#line 6645 "src/taumir/lower.tr"
                if (((_tr_str_eqv((method), (_tr_str_lit_len("sleep_ms", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL)))) && (margs->len >= 1LL))) {
#line 6646 "src/taumir/lower.tr"
                    long long msv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6647 "src/taumir/lower.tr"
                    if ((msv < 0LL)) {
#line 6647 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6648 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_sleep_h", 14LL));
#line 6649 "src/taumir/lower.tr"
                    List_i64* sma = (void*)List_i64_new();
#line 6650 "src/taumir/lower.tr"
                    List_i64_append(sma, msv);
#line 6651 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_sleep_h", 14LL), sma));
#line 6652 "src/taumir/lower.tr"
                    long long csd = LFunc_new_vreg(lf);
#line 6653 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IConst(csd, 0LL));
#line 6654 "src/taumir/lower.tr"
                    long long _trret1742 = csd;
                    return _trret1742;
                }
            }
#line 6655 "src/taumir/lower.tr"
            if (_tr_str_eqv((styn), (_tr_str_lit_len("Thread", 6LL)))) {
#line 6657 "src/taumir/lower.tr"
                if (((_tr_str_eqv((method), (_tr_str_lit_len("current_id", 10LL))) || _tr_str_eqv((method), (_tr_str_lit_len("id", 2LL)))) && (margs->len == 0LL))) {
#line 6658 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_thread_current_id", 21LL));
#line 6659 "src/taumir/lower.tr"
                    long long tcd = LFunc_new_vreg(lf);
#line 6660 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(tcd, _tr_str_lit_len("_tr_thread_current_id", 21LL), (void*)List_i64_new()));
#line 6661 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, tcd, 0LL);
#line 6662 "src/taumir/lower.tr"
                    long long _trret1743 = tcd;
                    return _trret1743;
                }
#line 6666 "src/taumir/lower.tr"
                if ((_tr_str_eqv((method), (_tr_str_lit_len("sleep", 5LL))) && (margs->len >= 1LL))) {
#line 6667 "src/taumir/lower.tr"
                    long long tmsv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6668 "src/taumir/lower.tr"
                    if ((tmsv < 0LL)) {
#line 6668 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6669 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_co_sleep_h", 14LL));
#line 6670 "src/taumir/lower.tr"
                    List_i64* tsa = (void*)List_i64_new();
#line 6671 "src/taumir/lower.tr"
                    List_i64_append(tsa, tmsv);
#line 6672 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_co_sleep_h", 14LL), tsa));
#line 6673 "src/taumir/lower.tr"
                    long long tsd = LFunc_new_vreg(lf);
#line 6674 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IConst(tsd, 0LL));
#line 6675 "src/taumir/lower.tr"
                    long long _trret1744 = tsd;
                    return _trret1744;
                }
#line 6679 "src/taumir/lower.tr"
                if ((_tr_str_eqv((method), (_tr_str_lit_len("spawn", 5LL))) && (margs->len >= 1LL))) {
#line 6680 "src/taumir/lower.tr"
                    TrStr tsfn = _ident_name(((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6681 "src/taumir/lower.tr"
                    if (((_is_null_str(tsfn) || _tr_str_eqv((tsfn), (_tr_str_lit_len("", 0LL)))) || (!LModule_is_user_fn(m, tsfn)))) {
#line 6681 "src/taumir/lower.tr"
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
#line 6682 "src/taumir/lower.tr"
                    if ((!_ensure_await_wrapper(m, tsfn))) {
#line 6682 "src/taumir/lower.tr"
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
#line 6683 "src/taumir/lower.tr"
                    long long tsbuf = _pack_spawn_args(m, lf, margs, 1LL);
#line 6684 "src/taumir/lower.tr"
                    if ((tsbuf < 0LL)) {
#line 6684 "src/taumir/lower.tr"
                        _tr_str_release(tsfn);
                        return (-1LL);
                    }
#line 6685 "src/taumir/lower.tr"
                    long long tsfa = LFunc_new_vreg(lf);
#line 6686 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, tsfa, 12LL);
#line 6687 "src/taumir/lower.tr"
                    ({ TrStr _at_t6437 = (_tr_strx_concatv((_tr_str_lit_len("_awaitwrap_", 11LL)), (tsfn))); LFunc_emit(lf, LInst_ctor_IFuncAddr(tsfa, _at_t6437)); _tr_str_release(_at_t6437); });
#line 6688 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_threadobj_spawn", 19LL));
#line 6689 "src/taumir/lower.tr"
                    List_i64* toa = (void*)List_i64_new();
#line 6690 "src/taumir/lower.tr"
                    List_i64_append(toa, tsfa);
#line 6691 "src/taumir/lower.tr"
                    List_i64_append(toa, tsbuf);
#line 6692 "src/taumir/lower.tr"
                    long long tod = LFunc_new_vreg(lf);
#line 6693 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(tod, _tr_str_lit_len("_tr_threadobj_spawn", 19LL), toa));
#line 6694 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, tod, 0LL);
#line 6695 "src/taumir/lower.tr"
                    long long _trret1745 = tod;
                    _tr_str_release(tsfn);
                    return _trret1745;
                }
            }
#line 6696 "src/taumir/lower.tr"
            if (((_tr_str_eqv((styn), (_tr_str_lit_len("Str", 3LL))) || _tr_str_eqv((styn), (_tr_str_lit_len("str", 3LL)))) || _tr_str_eqv((styn), (_tr_str_lit_len("String", 6LL))))) {
#line 6697 "src/taumir/lower.tr"
                if ((_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))) && (margs->len == 2LL))) {
#line 6698 "src/taumir/lower.tr"
                    long long sjl = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6699 "src/taumir/lower.tr"
                    if (((sjl < 0LL) || (LFunc_vreg_type(lf, sjl) != 3LL))) {
#line 6699 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6700 "src/taumir/lower.tr"
                    long long sjs = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 6701 "src/taumir/lower.tr"
                    if (((sjs < 0LL) || (LFunc_vreg_type(lf, sjs) != 1LL))) {
#line 6701 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6702 "src/taumir/lower.tr"
                    long long _trret1746 = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_list_join", 16LL), sjl, sjs, 1LL);
                    return _trret1746;
                }
#line 6703 "src/taumir/lower.tr"
                if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 1LL))) {
#line 6704 "src/taumir/lower.tr"
                    long long sll = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6705 "src/taumir/lower.tr"
                    if (((sll < 0LL) || (LFunc_vreg_type(lf, sll) != 1LL))) {
#line 6705 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6706 "src/taumir/lower.tr"
                    long long _trret1747 = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_strlen", 13LL), sll, 0LL);
                    return _trret1747;
                }
#line 6709 "src/taumir/lower.tr"
                if ((_tr_str_eqv((method), (_tr_str_lit_len("slice", 5LL))) && (margs->len == 3LL))) {
#line 6710 "src/taumir/lower.tr"
                    long long scv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6711 "src/taumir/lower.tr"
                    if (((scv < 0LL) || (LFunc_vreg_type(lf, scv) != 1LL))) {
#line 6711 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6712 "src/taumir/lower.tr"
                    long long sca = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 6713 "src/taumir/lower.tr"
                    if (((sca < 0LL) || (LFunc_vreg_type(lf, sca) != 0LL))) {
#line 6713 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6714 "src/taumir/lower.tr"
                    long long scb = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 2LL)));
#line 6715 "src/taumir/lower.tr"
                    if (((scb < 0LL) || (LFunc_vreg_type(lf, scb) != 0LL))) {
#line 6715 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6716 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
#line 6717 "src/taumir/lower.tr"
                    List_i64* scargs = (void*)List_i64_new();
#line 6718 "src/taumir/lower.tr"
                    List_i64_append(scargs, scv);
#line 6718 "src/taumir/lower.tr"
                    List_i64_append(scargs, sca);
#line 6718 "src/taumir/lower.tr"
                    List_i64_append(scargs, scb);
#line 6719 "src/taumir/lower.tr"
                    long long scd = LFunc_new_vreg(lf);
#line 6720 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(scd, _tr_str_lit_len("_tr_rt_str_slice", 16LL), scargs));
#line 6721 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, scd, 1LL);
#line 6722 "src/taumir/lower.tr"
                    _fresh_mark(lf, scd);
#line 6723 "src/taumir/lower.tr"
                    long long _trret1748 = scd;
                    return _trret1748;
                }
#line 6725 "src/taumir/lower.tr"
                if (((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) || _tr_str_eqv((method), (_tr_str_lit_len("find", 4LL)))) && (margs->len == 2LL))) {
#line 6726 "src/taumir/lower.tr"
                    long long sfv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6727 "src/taumir/lower.tr"
                    if (((sfv < 0LL) || (LFunc_vreg_type(lf, sfv) != 1LL))) {
#line 6727 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6728 "src/taumir/lower.tr"
                    long long sfb = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 6729 "src/taumir/lower.tr"
                    if (((sfb < 0LL) || (LFunc_vreg_type(lf, sfb) != 1LL))) {
#line 6729 "src/taumir/lower.tr"
                        return (-1LL);
                    }
#line 6730 "src/taumir/lower.tr"
                    long long _trret1749 = _str_call1(m, lf, _tr_str_lit_len("_tr_rt_str_find", 15LL), sfv, sfb, 0LL);
                    return _trret1749;
                }
            }
        } else if (1) {
            __auto_type _ = _t6436;
#line 6732 "src/taumir/lower.tr"
            /* pass */
        }
#line 6734 "src/taumir/lower.tr"
        long long boxr = _lower_box_method(m, lf, obj, method, margs);
#line 6735 "src/taumir/lower.tr"
        if ((boxr != (-2LL))) {
#line 6735 "src/taumir/lower.tr"
            long long _trret1750 = boxr;
            return _trret1750;
        }
#line 6739 "src/taumir/lower.tr"
        if ((_tr_str_eqv((hir_expr_type(obj)->name), (_tr_str_lit_len("Chan", 4LL))) && ((((((_tr_str_eqv((method), (_tr_str_lit_len("send", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("close", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))))) {
#line 6740 "src/taumir/lower.tr"
            long long cho = lower_expr(m, lf, obj);
#line 6741 "src/taumir/lower.tr"
            if ((cho < 0LL)) {
#line 6741 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 6742 "src/taumir/lower.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("send", 4LL))) && (margs->len == 1LL))) {
#line 6743 "src/taumir/lower.tr"
                long long chv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6744 "src/taumir/lower.tr"
                if (((chv < 0LL) || (LFunc_vreg_type(lf, chv) != 0LL))) {
#line 6744 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6745 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_send", 13LL));
#line 6746 "src/taumir/lower.tr"
                List_i64* csa = (void*)List_i64_new();
#line 6747 "src/taumir/lower.tr"
                List_i64_append(csa, cho);
#line 6748 "src/taumir/lower.tr"
                List_i64_append(csa, chv);
#line 6749 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_chan_send", 13LL), csa));
#line 6750 "src/taumir/lower.tr"
                long long _trret1751 = cho;
                return _trret1751;
            }
#line 6751 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("recv", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) && (margs->len == 0LL))) {
#line 6752 "src/taumir/lower.tr"
                TrStr crs = _tr_str_lit_len("_tr_chan_recv", 13LL);
#line 6753 "src/taumir/lower.tr"
                if (_tr_str_eqv((method), (_tr_str_lit_len("try_recv", 8LL)))) {
#line 6753 "src/taumir/lower.tr"
                    TrStr _strtmp_t6438 = _tr_str_lit_len("_tr_chan_try_recv_val", 21LL);
                    _tr_str_release(crs);
                    crs = _strtmp_t6438;
                }
#line 6754 "src/taumir/lower.tr"
                LModule_add_extern(m, crs);
#line 6755 "src/taumir/lower.tr"
                List_i64* cra = (void*)List_i64_new();
#line 6756 "src/taumir/lower.tr"
                List_i64_append(cra, cho);
#line 6757 "src/taumir/lower.tr"
                long long crd = LFunc_new_vreg(lf);
#line 6758 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(crd, crs, cra));
#line 6759 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, crd, 0LL);
#line 6760 "src/taumir/lower.tr"
                long long _trret1752 = crd;
                _tr_str_release(crs);
                return _trret1752;
            }
#line 6761 "src/taumir/lower.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("try_send", 8LL))) && (margs->len == 1LL))) {
#line 6762 "src/taumir/lower.tr"
                long long ctv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6763 "src/taumir/lower.tr"
                if (((ctv < 0LL) || (LFunc_vreg_type(lf, ctv) != 0LL))) {
#line 6763 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6764 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_try_send", 17LL));
#line 6765 "src/taumir/lower.tr"
                List_i64* cta = (void*)List_i64_new();
#line 6766 "src/taumir/lower.tr"
                List_i64_append(cta, cho);
#line 6767 "src/taumir/lower.tr"
                List_i64_append(cta, ctv);
#line 6768 "src/taumir/lower.tr"
                long long ctd = LFunc_new_vreg(lf);
#line 6769 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(ctd, _tr_str_lit_len("_tr_chan_try_send", 17LL), cta));
#line 6770 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, ctd, 4LL);
#line 6771 "src/taumir/lower.tr"
                long long _trret1753 = ctd;
                return _trret1753;
            }
#line 6772 "src/taumir/lower.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("close", 5LL))) && (margs->len == 0LL))) {
#line 6773 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_close", 14LL));
#line 6774 "src/taumir/lower.tr"
                List_i64* cca = (void*)List_i64_new();
#line 6775 "src/taumir/lower.tr"
                List_i64_append(cca, cho);
#line 6776 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_chan_close", 14LL), cca));
#line 6777 "src/taumir/lower.tr"
                long long _trret1754 = cho;
                return _trret1754;
            }
#line 6778 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) && (margs->len == 0LL))) {
#line 6779 "src/taumir/lower.tr"
                TrStr cqs = _tr_str_lit_len("_tr_chan_len", 12LL);
#line 6780 "src/taumir/lower.tr"
                long long cqtag = 0LL;
#line 6781 "src/taumir/lower.tr"
                if (_tr_str_eqv((method), (_tr_str_lit_len("is_closed", 9LL)))) {
#line 6782 "src/taumir/lower.tr"
                    TrStr _strtmp_t6439 = _tr_str_lit_len("_tr_chan_is_closed", 18LL);
                    _tr_str_release(cqs);
                    cqs = _strtmp_t6439;
#line 6783 "src/taumir/lower.tr"
                    cqtag = 4LL;
                }
#line 6784 "src/taumir/lower.tr"
                LModule_add_extern(m, cqs);
#line 6785 "src/taumir/lower.tr"
                List_i64* cqa = (void*)List_i64_new();
#line 6786 "src/taumir/lower.tr"
                List_i64_append(cqa, cho);
#line 6787 "src/taumir/lower.tr"
                long long cqd = LFunc_new_vreg(lf);
#line 6788 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(cqd, cqs, cqa));
#line 6789 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, cqd, cqtag);
#line 6790 "src/taumir/lower.tr"
                long long _trret1755 = cqd;
                _tr_str_release(cqs);
                return _trret1755;
            }
#line 6791 "src/taumir/lower.tr"
            return (-1LL);
        }
#line 6793 "src/taumir/lower.tr"
        if (_tr_str_eqv((hir_expr_type(obj)->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 6794 "src/taumir/lower.tr"
            AstType* pty = _ptr_chain_ty(m, obj);
#line 6795 "src/taumir/lower.tr"
            long long pstride = _ptr_stride(m, pty);
#line 6796 "src/taumir/lower.tr"
            if ((pstride == 0LL)) {
#line 6796 "src/taumir/lower.tr"
                return (-1LL);
            }
#line 6797 "src/taumir/lower.tr"
            bool pbyte = (pstride == 1LL);
#line 6798 "src/taumir/lower.tr"
            bool pword = (pstride == 4LL);
#line 6799 "src/taumir/lower.tr"
            long long petag = 0LL;
#line 6800 "src/taumir/lower.tr"
            TrStr pvcls = _tr_str_lit_len("", 0LL);
#line 6801 "src/taumir/lower.tr"
            if ((pty->args->len > 0LL)) {
#line 6802 "src/taumir/lower.tr"
                TrStr petn = _tr_str_retain(_subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL))))->name);
#line 6803 "src/taumir/lower.tr"
                if ((_tr_str_eqv((petn), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((petn), (_tr_str_lit_len("f64", 3LL))))) {
#line 6803 "src/taumir/lower.tr"
                    petag = 5LL;
                } else if ((_tr_str_eqv((petn), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((petn), (_tr_str_lit_len("String", 6LL))))) {
#line 6804 "src/taumir/lower.tr"
                    petag = 1LL;
                } else if (_tr_str_eqv((petn), (_tr_str_lit_len("bool", 4LL)))) {
#line 6805 "src/taumir/lower.tr"
                    petag = 4LL;
                } else if (LModule_is_value_class(m, petn)) {
#line 6806 "src/taumir/lower.tr"
                    petag = 10LL;
#line 6806 "src/taumir/lower.tr"
                    TrStr _strtmp_t6440 = _own(petn);
                    _tr_str_release(pvcls);
                    pvcls = _strtmp_t6440;
                } else if (LModule_is_class(m, petn)) {
#line 6807 "src/taumir/lower.tr"
                    petag = 10LL;
                } else if (LModule_is_enum(m, petn)) {
#line 6808 "src/taumir/lower.tr"
                    petag = 11LL;
                }
#line 6809 "src/taumir/lower.tr"
                if ((_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))) && (_value_struct_generic_size(m, (*((AstType**)List_ptr_get(pty->args, 0LL)))) > 0LL))) {
#line 6810 "src/taumir/lower.tr"
                    petag = 10LL;
#line 6811 "src/taumir/lower.tr"
                    TrStr _pmg = _ensure_generic_class(m, _subst_ty(m, (*((AstType**)List_ptr_get(pty->args, 0LL)))));
#line 6812 "src/taumir/lower.tr"
                    if ((!_tr_str_eqv((_pmg), (_tr_str_lit_len("", 0LL))))) {
#line 6812 "src/taumir/lower.tr"
                        TrStr _strtmp_t6441 = _tr_str_retain(_pmg);
                        _tr_str_release(pvcls);
                        pvcls = _strtmp_t6441;
                    } else {
#line 6813 "src/taumir/lower.tr"
                        TrStr _strtmp_t6442 = _own(petn);
                        _tr_str_release(pvcls);
                        pvcls = _strtmp_t6442;
                    }
                }
            }
#line 6814 "src/taumir/lower.tr"
            long long pbase = lower_expr(m, lf, obj);
#line 6815 "src/taumir/lower.tr"
            if ((pbase < 0LL)) {
#line 6815 "src/taumir/lower.tr"
                _tr_str_release(pvcls);
                return (-1LL);
            }
#line 6816 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) && (margs->len == 1LL))) {
#line 6818 "src/taumir/lower.tr"
                long long poff = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6819 "src/taumir/lower.tr"
                if (((poff < 0LL) || (LFunc_vreg_type(lf, poff) != 0LL))) {
#line 6819 "src/taumir/lower.tr"
                    _tr_str_release(pvcls);
                    return (-1LL);
                }
#line 6820 "src/taumir/lower.tr"
                long long pstr = LFunc_new_vreg(lf);
#line 6821 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IConst(pstr, pstride));
#line 6822 "src/taumir/lower.tr"
                long long pscaled = LFunc_new_vreg(lf);
#line 6823 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBinOp(pscaled, _tr_str_lit_len("*", 1LL), poff, pstr));
#line 6824 "src/taumir/lower.tr"
                long long padr = LFunc_new_vreg(lf);
#line 6825 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBinOp(padr, _tr_str_lit_len("+", 1LL), pbase, pscaled));
#line 6826 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, padr, 0LL);
#line 6827 "src/taumir/lower.tr"
                long long _trret1756 = padr;
                _tr_str_release(pvcls);
                return _trret1756;
            }
#line 6828 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("deref", 5LL)))) && (margs->len == 0LL))) {
#line 6829 "src/taumir/lower.tr"
                if (pbyte) {
#line 6830 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_load_u8", 14LL));
#line 6831 "src/taumir/lower.tr"
                    List_i64* lba = (void*)List_i64_new();
#line 6832 "src/taumir/lower.tr"
                    List_i64_append(lba, pbase);
#line 6833 "src/taumir/lower.tr"
                    long long lbd = LFunc_new_vreg(lf);
#line 6834 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(lbd, _tr_str_lit_len("_tr_rt_load_u8", 14LL), lba));
#line 6835 "src/taumir/lower.tr"
                    long long _trret1757 = lbd;
                    _tr_str_release(pvcls);
                    return _trret1757;
                }
#line 6836 "src/taumir/lower.tr"
                if (pword) {
#line 6837 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_load_i32", 15LL));
#line 6838 "src/taumir/lower.tr"
                    List_i64* lwa = (void*)List_i64_new();
#line 6839 "src/taumir/lower.tr"
                    List_i64_append(lwa, pbase);
#line 6840 "src/taumir/lower.tr"
                    long long lwd = LFunc_new_vreg(lf);
#line 6841 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(lwd, _tr_str_lit_len("_tr_rt_load_i32", 15LL), lwa));
#line 6842 "src/taumir/lower.tr"
                    long long _trret1758 = lwd;
                    _tr_str_release(pvcls);
                    return _trret1758;
                }
#line 6846 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))))) {
#line 6847 "src/taumir/lower.tr"
                    long long rcp = _copy_value_struct(m, lf, pbase, pvcls);
#line 6848 "src/taumir/lower.tr"
                    if ((rcp < 0LL)) {
#line 6848 "src/taumir/lower.tr"
                        _tr_str_release(pvcls);
                        return (-1LL);
                    }
#line 6849 "src/taumir/lower.tr"
                    long long _trret1759 = rcp;
                    _tr_str_release(pvcls);
                    return _trret1759;
                }
#line 6851 "src/taumir/lower.tr"
                if ((petag == 5LL)) {
#line 6852 "src/taumir/lower.tr"
                    long long prraw = _emit_field_get(m, lf, pbase, 0LL, 0LL);
#line 6853 "src/taumir/lower.tr"
                    long long prf = LFunc_new_vreg(lf);
#line 6854 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IBitsF(prf, prraw));
#line 6855 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, prf, 5LL);
#line 6856 "src/taumir/lower.tr"
                    long long _trret1760 = prf;
                    _tr_str_release(pvcls);
                    return _trret1760;
                }
#line 6857 "src/taumir/lower.tr"
                long long _trret1761 = _emit_field_get(m, lf, pbase, 0LL, petag);
                _tr_str_release(pvcls);
                return _trret1761;
            }
#line 6858 "src/taumir/lower.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL))) && (margs->len == 1LL))) {
#line 6860 "src/taumir/lower.tr"
                long long pwv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6861 "src/taumir/lower.tr"
                if ((pwv < 0LL)) {
#line 6861 "src/taumir/lower.tr"
                    _tr_str_release(pvcls);
                    return (-1LL);
                }
#line 6863 "src/taumir/lower.tr"
                if ((!_tr_str_eqv((pvcls), (_tr_str_lit_len("", 0LL))))) {
#line 6864 "src/taumir/lower.tr"
                    long long wsz = LFunc_new_vreg(lf);
#line 6865 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IConst(wsz, pstride));
#line 6866 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_c_memcpy", 12LL));
#line 6867 "src/taumir/lower.tr"
                    List_i64* wca = (void*)List_i64_new();
#line 6868 "src/taumir/lower.tr"
                    List_i64_append(wca, pbase);
#line 6869 "src/taumir/lower.tr"
                    List_i64_append(wca, pwv);
#line 6870 "src/taumir/lower.tr"
                    List_i64_append(wca, wsz);
#line 6871 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_c_memcpy", 12LL), wca));
#line 6872 "src/taumir/lower.tr"
                    long long _trret1762 = pbase;
                    _tr_str_release(pvcls);
                    return _trret1762;
                }
#line 6873 "src/taumir/lower.tr"
                if (pbyte) {
#line 6874 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_store_u8", 15LL));
#line 6875 "src/taumir/lower.tr"
                    List_i64* sba = (void*)List_i64_new();
#line 6876 "src/taumir/lower.tr"
                    List_i64_append(sba, pbase);
#line 6877 "src/taumir/lower.tr"
                    List_i64_append(sba, pwv);
#line 6878 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_store_u8", 15LL), sba));
#line 6879 "src/taumir/lower.tr"
                    long long _trret1763 = pbase;
                    _tr_str_release(pvcls);
                    return _trret1763;
                }
#line 6880 "src/taumir/lower.tr"
                if (pword) {
#line 6881 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_store_i32", 16LL));
#line 6882 "src/taumir/lower.tr"
                    List_i64* swa = (void*)List_i64_new();
#line 6883 "src/taumir/lower.tr"
                    List_i64_append(swa, pbase);
#line 6884 "src/taumir/lower.tr"
                    List_i64_append(swa, pwv);
#line 6885 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_store_i32", 16LL), swa));
#line 6886 "src/taumir/lower.tr"
                    long long _trret1764 = pbase;
                    _tr_str_release(pvcls);
                    return _trret1764;
                }
#line 6887 "src/taumir/lower.tr"
                if ((petag == 5LL)) {
#line 6888 "src/taumir/lower.tr"
                    long long pwvt = LFunc_vreg_type(lf, pwv);
#line 6889 "src/taumir/lower.tr"
                    if ((pwvt == 0LL)) {
#line 6889 "src/taumir/lower.tr"
                        pwv = _promote_f(lf, pwv);
                    } else if ((pwvt != 5LL)) {
#line 6890 "src/taumir/lower.tr"
                        _tr_str_release(pvcls);
                        return (-1LL);
                    }
#line 6891 "src/taumir/lower.tr"
                    long long pwb = LFunc_new_vreg(lf);
#line 6892 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_IFBits(pwb, pwv));
#line 6893 "src/taumir/lower.tr"
                    pwv = pwb;
                } else if ((petag == 1LL)) {
#line 6894 "src/taumir/lower.tr"
                    _secure_str(m, lf, pwv);
                } else if (((petag == 10LL) || (petag == 11LL))) {
#line 6895 "src/taumir/lower.tr"
                    _secure_obj(m, lf, pwv);
                }
#line 6896 "src/taumir/lower.tr"
                _emit_field_set(m, lf, pbase, 0LL, pwv);
#line 6897 "src/taumir/lower.tr"
                long long _trret1765 = pbase;
                _tr_str_release(pvcls);
                return _trret1765;
            }
#line 6898 "src/taumir/lower.tr"
            _tr_str_release(pvcls);
            return (-1LL);
        }
#line 6901 "src/taumir/lower.tr"
        __auto_type _t6443 = (*obj);
        if (_t6443.tag == HirExpr_EIdent) {
            __auto_type inm = _t6443.data.EIdent.name;
__auto_type ity_g = _t6443.data.EIdent.ty;
#line 6911 "src/taumir/lower.tr"
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Map", 3LL))) || _tr_str_eqv((inm), (_tr_str_lit_len("Dict", 4LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))))) {
#line 6912 "src/taumir/lower.tr"
                long long mtag = _tag_of(m, ity_g);
#line 6913 "src/taumir/lower.tr"
                if ((!_is_dict_tag(mtag))) {
#line 6913 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6914 "src/taumir/lower.tr"
                TrStr mnewsym = _dict_new_sym(mtag);
#line 6915 "src/taumir/lower.tr"
                LModule_add_extern(m, mnewsym);
#line 6916 "src/taumir/lower.tr"
                long long mnd = LFunc_new_vreg(lf);
#line 6917 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(mnd, mnewsym, (void*)List_i64_new()));
#line 6918 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, mnd, mtag);
#line 6919 "src/taumir/lower.tr"
                long long _trret1766 = mnd;
                _tr_str_release(mnewsym);
                return _trret1766;
            }
#line 6920 "src/taumir/lower.tr"
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Set", 3LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 0LL))) {
#line 6921 "src/taumir/lower.tr"
                long long stag = (-1LL);
#line 6922 "src/taumir/lower.tr"
                if ((ity_g->args->len > 0LL)) {
#line 6923 "src/taumir/lower.tr"
                    TrStr se = _tr_str_retain((*((AstType**)List_ptr_get(ity_g->args, 0LL)))->name);
#line 6924 "src/taumir/lower.tr"
                    if ((_tr_str_eqv((se), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((se), (_tr_str_lit_len("String", 6LL))))) {
#line 6924 "src/taumir/lower.tr"
                        stag = 16LL;
                    } else if (_is_int_typename(se)) {
#line 6925 "src/taumir/lower.tr"
                        stag = 13LL;
                    }
                }
#line 6926 "src/taumir/lower.tr"
                if ((stag < 0LL)) {
#line 6926 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6927 "src/taumir/lower.tr"
                TrStr newsym = _set_sym(stag, _tr_str_lit_len("new", 3LL));
#line 6928 "src/taumir/lower.tr"
                LModule_add_extern(m, newsym);
#line 6929 "src/taumir/lower.tr"
                long long snd = LFunc_new_vreg(lf);
#line 6930 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(snd, newsym, (void*)List_i64_new()));
#line 6931 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, snd, stag);
#line 6932 "src/taumir/lower.tr"
                long long _trret1767 = snd;
                _tr_str_release(newsym);
                return _trret1767;
            }
#line 6934 "src/taumir/lower.tr"
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Vec", 3LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (ity_g->args->len > 0LL))) {
#line 6935 "src/taumir/lower.tr"
                TrStr ven = _tr_str_retain((*((AstType**)List_ptr_get(ity_g->args, 0LL)))->name);
#line 6936 "src/taumir/lower.tr"
                if ((_tr_str_eqv((ven), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((ven), (_tr_str_lit_len("tuple", 5LL))))) {
#line 6937 "src/taumir/lower.tr"
                    LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
#line 6938 "src/taumir/lower.tr"
                    long long vnd = LFunc_new_vreg(lf);
#line 6939 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(vnd, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
#line 6940 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, vnd, 21LL);
#line 6941 "src/taumir/lower.tr"
                    long long _trret1768 = vnd;
                    _tr_str_release(ven);
                    return _trret1768;
                }
            }
#line 6945 "src/taumir/lower.tr"
            if ((_tr_str_eqv((inm), (_tr_str_lit_len("List", 4LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL)))))) {
#line 6946 "src/taumir/lower.tr"
                long long ltag2 = (-1LL);
#line 6947 "src/taumir/lower.tr"
                if ((ity_g->args->len > 0LL)) {
#line 6948 "src/taumir/lower.tr"
                    long long letag = _tag_of(m, _subst_ty(m, (*((AstType**)List_ptr_get(ity_g->args, 0LL)))));
#line 6949 "src/taumir/lower.tr"
                    ltag2 = _list_tag_for_elem(letag);
                }
#line 6950 "src/taumir/lower.tr"
                if (((ltag2 < 0LL) || (!_is_list_tag(ltag2)))) {
#line 6950 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6951 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_new", 15LL));
#line 6952 "src/taumir/lower.tr"
                long long lnd = LFunc_new_vreg(lf);
#line 6953 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(lnd, _tr_str_lit_len("_tr_rt_list_new", 15LL), (void*)List_i64_new()));
#line 6954 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, lnd, ltag2);
#line 6955 "src/taumir/lower.tr"
                long long _trret1769 = lnd;
                return _trret1769;
            }
#line 6958 "src/taumir/lower.tr"
            if (((_tr_str_eqv((inm), (_tr_str_lit_len("Chan", 4LL))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 1LL))) {
#line 6959 "src/taumir/lower.tr"
                long long ccap = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6960 "src/taumir/lower.tr"
                if (((ccap < 0LL) || (LFunc_vreg_type(lf, ccap) != 0LL))) {
#line 6960 "src/taumir/lower.tr"
                    return (-1LL);
                }
#line 6961 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_chan_new", 12LL));
#line 6962 "src/taumir/lower.tr"
                List_i64* cna = (void*)List_i64_new();
#line 6963 "src/taumir/lower.tr"
                List_i64_append(cna, ccap);
#line 6964 "src/taumir/lower.tr"
                long long cnd = LFunc_new_vreg(lf);
#line 6965 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(cnd, _tr_str_lit_len("_tr_chan_new", 12LL), cna));
#line 6966 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, cnd, 0LL);
#line 6967 "src/taumir/lower.tr"
                long long _trret1770 = cnd;
                return _trret1770;
            }
#line 6970 "src/taumir/lower.tr"
            if (((((((_tr_str_eqv((inm), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((inm), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((inm), (_tr_str_lit_len("ThreadLocal", 11LL)))) && (_tr_str_eqv((method), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("new", 3LL))))) && (margs->len == 1LL))) {
#line 6971 "src/taumir/lower.tr"
                TrStr boxrt = _tr_str_lit_len("_tr_mutexbox_new", 16LL);
#line 6972 "src/taumir/lower.tr"
                if (_tr_str_eqv((inm), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 6972 "src/taumir/lower.tr"
                    TrStr _strtmp_t6444 = _tr_str_lit_len("_tr_rwlbox_new", 14LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t6444;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("Atomic", 6LL)))) {
#line 6973 "src/taumir/lower.tr"
                    TrStr _strtmp_t6445 = _tr_str_lit_len("_tr_atomic_new", 14LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t6445;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("ThreadPool", 10LL)))) {
#line 6974 "src/taumir/lower.tr"
                    TrStr _strtmp_t6446 = _tr_str_lit_len("_tr_threadpool_new", 18LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t6446;
                } else if (_tr_str_eqv((inm), (_tr_str_lit_len("ThreadLocal", 11LL)))) {
#line 6975 "src/taumir/lower.tr"
                    TrStr _strtmp_t6447 = _tr_str_lit_len("_tr_tls_new", 11LL);
                    _tr_str_release(boxrt);
                    boxrt = _strtmp_t6447;
                }
#line 6976 "src/taumir/lower.tr"
                long long bxv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 6977 "src/taumir/lower.tr"
                if (((bxv < 0LL) || (LFunc_vreg_type(lf, bxv) != 0LL))) {
#line 6977 "src/taumir/lower.tr"
                    _tr_str_release(boxrt);
                    return (-1LL);
                }
#line 6978 "src/taumir/lower.tr"
                LModule_add_extern(m, boxrt);
#line 6979 "src/taumir/lower.tr"
                List_i64* bxa = (void*)List_i64_new();
#line 6980 "src/taumir/lower.tr"
                List_i64_append(bxa, bxv);
#line 6981 "src/taumir/lower.tr"
                long long bxd = LFunc_new_vreg(lf);
#line 6982 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(bxd, boxrt, bxa));
#line 6983 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, bxd, 0LL);
#line 6984 "src/taumir/lower.tr"
                long long _trret1771 = bxd;
                _tr_str_release(boxrt);
                return _trret1771;
            }
#line 6985 "src/taumir/lower.tr"
            if (((_prog_generic_class_index(m, inm) >= 0LL) && (ity_g->args->len > 0LL))) {
#line 6986 "src/taumir/lower.tr"
                TrStr gsm = _ensure_generic_class(m, ity_g);
#line 6987 "src/taumir/lower.tr"
                if (_tr_str_eqv((gsm), (_tr_str_lit_len("", 0LL)))) {
#line 6987 "src/taumir/lower.tr"
                    _tr_str_release(gsm);
                    return (-1LL);
                }
#line 6988 "src/taumir/lower.tr"
                long long _trret1772 = ({ TrStr _at_t6448 = (({ TrStr _tr_cl = (_tr_strx_concatv((gsm), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t6448, (-1LL), margs)); _tr_str_release(_at_t6448); _wr; });
                _tr_str_release(gsm);
                return _trret1772;
            }
#line 6989 "src/taumir/lower.tr"
            if (LModule_is_class(m, inm)) {
#line 6990 "src/taumir/lower.tr"
                TrStr smang = LModule_resolve_method_ov(m, inm, method, margs->len);
#line 6991 "src/taumir/lower.tr"
                if (_tr_str_eqv((smang), (_tr_str_lit_len("", 0LL)))) {
#line 6991 "src/taumir/lower.tr"
                    _tr_str_release(smang);
                    return (-1LL);
                }
#line 6992 "src/taumir/lower.tr"
                long long _trret1773 = _lower_obj_call(m, lf, smang, (-1LL), margs);
                _tr_str_release(smang);
                return _trret1773;
            }
#line 6993 "src/taumir/lower.tr"
            if (LModule_is_enum(m, inm)) {
#line 6994 "src/taumir/lower.tr"
                TrStr nvm = _norm_variant(inm, method);
#line 6995 "src/taumir/lower.tr"
                if ((LModule_enum_variant_index(m, inm, nvm) >= 0LL)) {
#line 6996 "src/taumir/lower.tr"
                    long long _trret1774 = _lower_enum_ctor(m, lf, inm, nvm, margs);
                    _tr_str_release(nvm);
                    return _trret1774;
                }
#line 6997 "src/taumir/lower.tr"
                long long _trret1775 = ({ TrStr _at_t6449 = (({ TrStr _tr_cl = (_tr_strx_concatv((inm), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_lower_obj_call(m, lf, _at_t6449, (-1LL), margs)); _tr_str_release(_at_t6449); _wr; });
                _tr_str_release(nvm);
                return _trret1775;
            }
        } else if (1) {
            __auto_type _ = _t6443;
#line 6999 "src/taumir/lower.tr"
            /* pass */
        }
#line 7002 "src/taumir/lower.tr"
        TrStr recv_cls = _recv_class(m, lf, obj);
#line 7005 "src/taumir/lower.tr"
        TrStr amn = _ident_name(obj);
#line 7006 "src/taumir/lower.tr"
        if (((!_tr_str_eqv((amn), (_tr_str_lit_len("", 0LL)))) && (_fixed_arr_len(m, lf, amn) > 0LL))) {
#line 7007 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) && (margs->len == 1LL))) {
#line 7008 "src/taumir/lower.tr"
                long long amiv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7009 "src/taumir/lower.tr"
                if (((amiv < 0LL) || (LFunc_vreg_type(lf, amiv) != 0LL))) {
#line 7009 "src/taumir/lower.tr"
                    _tr_str_release(recv_cls);
                    _tr_str_release(amn);
                    return (-1LL);
                }
#line 7010 "src/taumir/lower.tr"
                long long amaddr = _array_elem_addr(m, lf, amn, amiv);
#line 7011 "src/taumir/lower.tr"
                long long amd = LFunc_new_vreg(lf);
#line 7012 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ILoad(amd, amaddr, 0LL, 2LL));
#line 7013 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, amd, _fixed_arr_elem_tag(m, lf, amn));
#line 7014 "src/taumir/lower.tr"
                long long _trret1776 = amd;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _trret1776;
            }
#line 7015 "src/taumir/lower.tr"
            if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
#line 7016 "src/taumir/lower.tr"
                long long amld = LFunc_new_vreg(lf);
#line 7017 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IConst(amld, _fixed_arr_len(m, lf, amn)));
#line 7018 "src/taumir/lower.tr"
                long long _trret1777 = amld;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _trret1777;
            }
#line 7019 "src/taumir/lower.tr"
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
#line 7025 "src/taumir/lower.tr"
        if (((_tr_str_eqv((recv_cls), (_tr_str_lit_len("StrView", 7LL))) && _tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL)))) && (margs->len == 0LL))) {
#line 7026 "src/taumir/lower.tr"
            long long svo = lower_expr(m, lf, obj);
#line 7027 "src/taumir/lower.tr"
            if ((svo < 0LL)) {
#line 7027 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7028 "src/taumir/lower.tr"
            long long svdata = _emit_field_get(m, lf, svo, 0LL, 0LL);
#line 7029 "src/taumir/lower.tr"
            long long svlen = _emit_field_get(m, lf, svo, 8LL, 0LL);
#line 7030 "src/taumir/lower.tr"
            long long svz = LFunc_new_vreg(lf);
#line 7031 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(svz, 0LL));
#line 7032 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_slice", 16LL));
#line 7033 "src/taumir/lower.tr"
            List_i64* sva = (void*)List_i64_new();
#line 7034 "src/taumir/lower.tr"
            List_i64_append(sva, svdata);
#line 7034 "src/taumir/lower.tr"
            List_i64_append(sva, svz);
#line 7034 "src/taumir/lower.tr"
            List_i64_append(sva, svlen);
#line 7035 "src/taumir/lower.tr"
            long long svd = LFunc_new_vreg(lf);
#line 7036 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(svd, _tr_str_lit_len("_tr_rt_str_slice", 16LL), sva));
#line 7037 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, svd, 1LL);
#line 7038 "src/taumir/lower.tr"
            _fresh_mark(lf, svd);
#line 7039 "src/taumir/lower.tr"
            long long _trret1778 = svd;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1778;
        }
#line 7043 "src/taumir/lower.tr"
        if (((_tr_str_eqv((recv_cls), (_tr_str_lit_len("StringObj", 9LL))) && _tr_str_eqv((method), (_tr_str_lit_len("as_str", 6LL)))) && (margs->len == 0LL))) {
#line 7044 "src/taumir/lower.tr"
            long long soo = lower_expr(m, lf, obj);
#line 7045 "src/taumir/lower.tr"
            if ((soo < 0LL)) {
#line 7045 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7046 "src/taumir/lower.tr"
            long long sodata = _emit_field_get(m, lf, soo, 0LL, 0LL);
#line 7047 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_str_new", 14LL));
#line 7048 "src/taumir/lower.tr"
            List_i64* soa = (void*)List_i64_new();
#line 7049 "src/taumir/lower.tr"
            List_i64_append(soa, sodata);
#line 7050 "src/taumir/lower.tr"
            long long sod = LFunc_new_vreg(lf);
#line 7051 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(sod, _tr_str_lit_len("_tr_rt_str_new", 14LL), soa));
#line 7052 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, sod, 1LL);
#line 7053 "src/taumir/lower.tr"
            _fresh_mark(lf, sod);
#line 7054 "src/taumir/lower.tr"
            long long _trret1779 = sod;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1779;
        }
#line 7055 "src/taumir/lower.tr"
        if ((!_tr_str_eqv((recv_cls), (_tr_str_lit_len("", 0LL))))) {
#line 7058 "src/taumir/lower.tr"
            TrStr dmethod = _tr_str_retain(method);
#line 7059 "src/taumir/lower.tr"
            if (LModule_is_class(m, recv_cls)) {
#line 7060 "src/taumir/lower.tr"
                if (((_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL)))) && ({ TrStr _cmpa_t6450 = (LModule_resolve_method(m, recv_cls, _tr_str_lit_len("__getitem__", 11LL))); TrStr _cmpb_t6451 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t6452 = !_tr_str_eqv(_cmpa_t6450, _cmpb_t6451); _tr_str_release(_cmpa_t6450); _cmpr_t6452; }))) {
#line 7061 "src/taumir/lower.tr"
                    TrStr _strtmp_t6453 = _tr_str_lit_len("__getitem__", 11LL);
                    _tr_str_release(dmethod);
                    dmethod = _strtmp_t6453;
                }
            }
#line 7062 "src/taumir/lower.tr"
            TrStr mangled = ({ TrStr _tr_cl = (_tr_strx_concatv((recv_cls), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (dmethod)); _tr_str_release(_tr_cl); _tr_cres; });
#line 7063 "src/taumir/lower.tr"
            if (LModule_is_class(m, recv_cls)) {
#line 7064 "src/taumir/lower.tr"
                TrStr _strtmp_t6454 = LModule_resolve_method_ov(m, recv_cls, dmethod, margs->len);
                _tr_str_release(mangled);
                mangled = _strtmp_t6454;
#line 7065 "src/taumir/lower.tr"
                if (_tr_str_eqv((mangled), (_tr_str_lit_len("", 0LL)))) {
#line 7068 "src/taumir/lower.tr"
                    long long gmix = _find_generic_method(m, recv_cls, method);
#line 7069 "src/taumir/lower.tr"
                    if ((gmix < 0LL)) {
#line 7069 "src/taumir/lower.tr"
                        _tr_str_release(recv_cls);
                        _tr_str_release(amn);
                        _tr_str_release(dmethod);
                        _tr_str_release(mangled);
                        return (-1LL);
                    }
#line 7070 "src/taumir/lower.tr"
                    long long gm_self = lower_expr(m, lf, obj);
#line 7071 "src/taumir/lower.tr"
                    if ((gm_self < 0LL)) {
#line 7071 "src/taumir/lower.tr"
                        _tr_str_release(recv_cls);
                        _tr_str_release(amn);
                        _tr_str_release(dmethod);
                        _tr_str_release(mangled);
                        return (-1LL);
                    }
#line 7072 "src/taumir/lower.tr"
                    List_i64* gm_tags = (void*)List_i64_new();
#line 7073 "src/taumir/lower.tr"
                    List_TrStr* gm_cls = (void*)List_TrStr_new();
#line 7074 "src/taumir/lower.tr"
                    List_i64* gm_regs = (void*)List_i64_new();
#line 7075 "src/taumir/lower.tr"
                    List_i64_append(gm_tags, LFunc_vreg_type(lf, gm_self));
#line 7076 "src/taumir/lower.tr"
                    ({ TrStr _at_t6455 = (_own(recv_cls)); List_TrStr_append(gm_cls, _at_t6455); _tr_str_release(_at_t6455); });
#line 7077 "src/taumir/lower.tr"
                    List_i64_append(gm_regs, gm_self);
#line 7078 "src/taumir/lower.tr"
                    TrStr gm_name = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((recv_cls), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("__m", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 7079 "src/taumir/lower.tr"
                    long long gmj = 0LL;
#line 7080 "src/taumir/lower.tr"
                    while ((gmj < margs->len)) {
#line 7081 "src/taumir/lower.tr"
                        long long gmv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, gmj)));
#line 7082 "src/taumir/lower.tr"
                        if ((gmv < 0LL)) {
#line 7082 "src/taumir/lower.tr"
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
#line 7083 "src/taumir/lower.tr"
                        long long gmt = LFunc_vreg_type(lf, gmv);
#line 7084 "src/taumir/lower.tr"
                        TrStr gmc = _tr_str_lit_len("", 0LL);
#line 7085 "src/taumir/lower.tr"
                        if (((gmt == 10LL) || (gmt == 11LL))) {
#line 7086 "src/taumir/lower.tr"
                            TrStr _strtmp_t6456 = _recv_class(m, lf, ((HirExpr*)List_ptr_get(margs, gmj)));
                            _tr_str_release(gmc);
                            gmc = _strtmp_t6456;
#line 7087 "src/taumir/lower.tr"
                            if (_tr_str_eqv((gmc), (_tr_str_lit_len("", 0LL)))) {
#line 7087 "src/taumir/lower.tr"
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
#line 7088 "src/taumir/lower.tr"
                        List_i64_append(gm_regs, gmv);
#line 7089 "src/taumir/lower.tr"
                        List_i64_append(gm_tags, gmt);
#line 7090 "src/taumir/lower.tr"
                        List_TrStr_append(gm_cls, gmc);
#line 7091 "src/taumir/lower.tr"
                        if ((gmj > 0LL)) {
#line 7091 "src/taumir/lower.tr"
                            ({ TrStr _ap_t6457 = (_tr_str_lit_len("_", 1LL)); _tr_str_append(&gm_name, _ap_t6457); });
                        }
#line 7092 "src/taumir/lower.tr"
                        ({ TrStr _ap_t6458 = (_tr_str_wrap(_tr_int_to_str((long long)(gmt)))); _tr_str_append(&gm_name, _ap_t6458); _tr_str_release(_ap_t6458); });
#line 7093 "src/taumir/lower.tr"
                        if ((!_tr_str_eqv((gmc), (_tr_str_lit_len("", 0LL))))) {
#line 7093 "src/taumir/lower.tr"
                            ({ TrStr _ap_t6459 = (gmc); _tr_str_append(&gm_name, _ap_t6459); });
                        }
#line 7094 "src/taumir/lower.tr"
                        gmj = (gmj + 1LL);
                        _tr_str_release(gmc);
                    }
#line 7095 "src/taumir/lower.tr"
                    if ((gm_regs->len > 6LL)) {
#line 7095 "src/taumir/lower.tr"
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
#line 7096 "src/taumir/lower.tr"
                    if ((!LModule_is_user_fn(m, gm_name))) {
#line 7097 "src/taumir/lower.tr"
                        if ((!_lir_lower_generic(m, ((HirFunction*)List_ptr_get(m->hir_prog->functions, gmix)), gm_tags, gm_cls, gm_name))) {
#line 7098 "src/taumir/lower.tr"
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
#line 7099 "src/taumir/lower.tr"
                    long long gm_ret = LModule_fn_ret_tag(m, gm_name);
#line 7100 "src/taumir/lower.tr"
                    long long gm_d = LFunc_new_vreg(lf);
#line 7101 "src/taumir/lower.tr"
                    LFunc_emit(lf, LInst_ctor_ICall(gm_d, gm_name, gm_regs));
#line 7102 "src/taumir/lower.tr"
                    LFunc_set_vreg_type(lf, gm_d, gm_ret);
#line 7103 "src/taumir/lower.tr"
                    if ((gm_ret == 1LL)) {
#line 7103 "src/taumir/lower.tr"
                        _fresh_mark(lf, gm_d);
                    }
#line 7104 "src/taumir/lower.tr"
                    long long _trret1780 = gm_d;
                    _tr_str_release(recv_cls);
                    _tr_str_release(amn);
                    _tr_str_release(dmethod);
                    _tr_str_release(mangled);
                    _tr_str_release(gm_name);
                    return _trret1780;
                }
            }
#line 7105 "src/taumir/lower.tr"
            long long rself = lower_expr(m, lf, obj);
#line 7106 "src/taumir/lower.tr"
            if ((rself < 0LL)) {
#line 7106 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                _tr_str_release(dmethod);
                _tr_str_release(mangled);
                return (-1LL);
            }
#line 7107 "src/taumir/lower.tr"
            long long _trret1781 = _lower_obj_call(m, lf, mangled, rself, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(dmethod);
            _tr_str_release(mangled);
            return _trret1781;
        }
#line 7108 "src/taumir/lower.tr"
        long long ovm = lower_expr(m, lf, obj);
#line 7109 "src/taumir/lower.tr"
        if ((ovm < 0LL)) {
#line 7109 "src/taumir/lower.tr"
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
#line 7110 "src/taumir/lower.tr"
        long long ovmt = LFunc_vreg_type(lf, ovm);
#line 7111 "src/taumir/lower.tr"
        if ((ovmt == 1LL)) {
#line 7112 "src/taumir/lower.tr"
            long long _trret1782 = _lower_str_method(m, lf, ovm, method, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1782;
        }
#line 7113 "src/taumir/lower.tr"
        if ((ovmt == 5LL)) {
#line 7114 "src/taumir/lower.tr"
            long long _trret1783 = _lower_float_method(m, lf, ovm, method, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1783;
        }
#line 7115 "src/taumir/lower.tr"
        if ((ovmt == 0LL)) {
#line 7116 "src/taumir/lower.tr"
            long long _trret1784 = _lower_int_method(m, lf, ovm, method, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1784;
        }
#line 7117 "src/taumir/lower.tr"
        if (_is_dict_tag(ovmt)) {
#line 7118 "src/taumir/lower.tr"
            long long _trret1785 = _lower_dict_method(m, lf, ovm, ovmt, method, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1785;
        }
#line 7119 "src/taumir/lower.tr"
        if (_is_set_tag(ovmt)) {
#line 7120 "src/taumir/lower.tr"
            long long _trret1786 = _lower_set_method(m, lf, ovm, ovmt, method, margs);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1786;
        }
#line 7121 "src/taumir/lower.tr"
        if ((((ovmt == 15LL) && (_tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))))) && (margs->len == 1LL))) {
#line 7123 "src/taumir/lower.tr"
            long long tgt_idx = (-1LL);
#line 7124 "src/taumir/lower.tr"
            __auto_type _t6460 = (*((HirExpr*)List_ptr_get(margs, 0LL)));
            if (_t6460.tag == HirExpr_ELitInt) {
                __auto_type tgt_v = _t6460.data.ELitInt.val;
                tgt_idx = tgt_v;
            } else if (1) {
                __auto_type _ = _t6460;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7127 "src/taumir/lower.tr"
            AstType* tup_hty = hir_expr_type(obj);
#line 7128 "src/taumir/lower.tr"
            if (((tgt_idx < 0LL) || (tgt_idx >= tup_hty->args->len))) {
#line 7128 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7129 "src/taumir/lower.tr"
            long long tgt_tag = _tag_of(m, (*((AstType**)List_ptr_get(tup_hty->args, tgt_idx))));
#line 7130 "src/taumir/lower.tr"
            if (((((tgt_tag < 0LL) || _is_list_tag(tgt_tag)) || _is_dict_tag(tgt_tag)) || _is_set_tag(tgt_tag))) {
#line 7130 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7131 "src/taumir/lower.tr"
            if ((tgt_tag == 5LL)) {
#line 7132 "src/taumir/lower.tr"
                long long tgr = _emit_field_get(m, lf, ovm, (tgt_idx * 8LL), 0LL);
#line 7133 "src/taumir/lower.tr"
                long long tgf = LFunc_new_vreg(lf);
#line 7134 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(tgf, tgr));
#line 7135 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, tgf, 5LL);
#line 7136 "src/taumir/lower.tr"
                long long _trret1787 = tgf;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _trret1787;
            }
#line 7137 "src/taumir/lower.tr"
            long long _trret1788 = _emit_field_get(m, lf, ovm, (tgt_idx * 8LL), tgt_tag);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1788;
        }
#line 7138 "src/taumir/lower.tr"
        if ((!_is_list_tag(ovmt))) {
#line 7138 "src/taumir/lower.tr"
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
#line 7139 "src/taumir/lower.tr"
        long long want_elem = _list_elem_tag(ovmt);
#line 7140 "src/taumir/lower.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("length", 6LL)))) && (margs->len == 0LL))) {
#line 7141 "src/taumir/lower.tr"
            long long _trret1789 = _str_call0(m, lf, _tr_str_lit_len("_tr_rt_list_len", 15LL), ovm, 0LL);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1789;
        }
#line 7142 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL))))) {
#line 7143 "src/taumir/lower.tr"
            if ((margs->len != 1LL)) {
#line 7143 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7144 "src/taumir/lower.tr"
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7145 "src/taumir/lower.tr"
            if ((av < 0LL)) {
#line 7145 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7146 "src/taumir/lower.tr"
            long long avt = LFunc_vreg_type(lf, av);
#line 7148 "src/taumir/lower.tr"
            if (((want_elem == 0LL) && (avt == 4LL))) {
#line 7149 "src/taumir/lower.tr"
                avt = 0LL;
            }
#line 7150 "src/taumir/lower.tr"
            if ((avt != want_elem)) {
#line 7150 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7151 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7151 "src/taumir/lower.tr"
                _secure_str(m, lf, av);
            }
#line 7152 "src/taumir/lower.tr"
            if ((((want_elem == 10LL) || (want_elem == 11LL)) || (want_elem == 15LL))) {
#line 7152 "src/taumir/lower.tr"
                _secure_obj(m, lf, av);
            }
#line 7153 "src/taumir/lower.tr"
            if ((want_elem == 5LL)) {
#line 7154 "src/taumir/lower.tr"
                long long avfb = LFunc_new_vreg(lf);
#line 7155 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(avfb, av));
#line 7156 "src/taumir/lower.tr"
                av = avfb;
            }
#line 7157 "src/taumir/lower.tr"
            TrStr apush = _tr_str_lit_len("_tr_rt_list_push_i64", 20LL);
#line 7158 "src/taumir/lower.tr"
            if ((ovmt == 22LL)) {
#line 7158 "src/taumir/lower.tr"
                TrStr _strtmp_t6461 = _tr_str_lit_len("_tr_rt_blist_push", 17LL);
                _tr_str_release(apush);
                apush = _strtmp_t6461;
            }
#line 7159 "src/taumir/lower.tr"
            LModule_add_extern(m, apush);
#line 7160 "src/taumir/lower.tr"
            List_i64* ppa = (void*)List_i64_new();
#line 7161 "src/taumir/lower.tr"
            List_i64_append(ppa, ovm);
#line 7162 "src/taumir/lower.tr"
            List_i64_append(ppa, av);
#line 7163 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), apush, ppa));
#line 7164 "src/taumir/lower.tr"
            long long _trret1790 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(apush);
            return _trret1790;
        }
#line 7165 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("get_index", 9LL))))) {
#line 7166 "src/taumir/lower.tr"
            if ((margs->len != 1LL)) {
#line 7166 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7167 "src/taumir/lower.tr"
            long long giv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7168 "src/taumir/lower.tr"
            if ((giv < 0LL)) {
#line 7168 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7169 "src/taumir/lower.tr"
            long long _trret1791 = _list_get_elem(m, lf, ovmt, ovm, giv);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1791;
        }
#line 7170 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))) && (margs->len == 2LL))) {
#line 7171 "src/taumir/lower.tr"
            long long sti = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7172 "src/taumir/lower.tr"
            if (((sti < 0LL) || (LFunc_vreg_type(lf, sti) != 0LL))) {
#line 7172 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7173 "src/taumir/lower.tr"
            long long stv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 7174 "src/taumir/lower.tr"
            if ((stv < 0LL)) {
#line 7174 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7175 "src/taumir/lower.tr"
            long long stvt = LFunc_vreg_type(lf, stv);
#line 7176 "src/taumir/lower.tr"
            if (((want_elem == 0LL) && (stvt == 4LL))) {
#line 7176 "src/taumir/lower.tr"
                stvt = 0LL;
            }
#line 7177 "src/taumir/lower.tr"
            if ((stvt != want_elem)) {
#line 7177 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7178 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7178 "src/taumir/lower.tr"
                _secure_str(m, lf, stv);
            }
#line 7179 "src/taumir/lower.tr"
            if ((((want_elem == 10LL) || (want_elem == 11LL)) || (want_elem == 15LL))) {
#line 7179 "src/taumir/lower.tr"
                _secure_obj(m, lf, stv);
            }
#line 7180 "src/taumir/lower.tr"
            if ((want_elem == 5LL)) {
#line 7181 "src/taumir/lower.tr"
                long long stfb = LFunc_new_vreg(lf);
#line 7182 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IFBits(stfb, stv));
#line 7183 "src/taumir/lower.tr"
                stv = stfb;
            }
#line 7184 "src/taumir/lower.tr"
            long long st_addr = _inline_list_addr(m, lf, ovm, sti, _list_stride(ovmt));
#line 7185 "src/taumir/lower.tr"
            if ((ovmt == 22LL)) {
#line 7186 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IStoreB(st_addr, 0LL, stv));
            } else {
#line 7188 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IStore(st_addr, 0LL, stv, 2LL));
            }
#line 7189 "src/taumir/lower.tr"
            long long _trret1792 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1792;
        }
#line 7190 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL))) && (margs->len == 0LL))) {
#line 7191 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_pop_i64", 19LL));
#line 7192 "src/taumir/lower.tr"
            List_i64* poa = (void*)List_i64_new();
#line 7193 "src/taumir/lower.tr"
            List_i64_append(poa, ovm);
#line 7194 "src/taumir/lower.tr"
            long long pod = LFunc_new_vreg(lf);
#line 7195 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(pod, _tr_str_lit_len("_tr_rt_list_pop_i64", 19LL), poa));
#line 7196 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, pod, want_elem);
#line 7197 "src/taumir/lower.tr"
            long long _trret1793 = pod;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1793;
        }
#line 7198 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("index_of", 8LL))) && (margs->len == 1LL))) {
#line 7199 "src/taumir/lower.tr"
            long long ixa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7200 "src/taumir/lower.tr"
            if ((ixa < 0LL)) {
#line 7200 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7201 "src/taumir/lower.tr"
            long long ixat = LFunc_vreg_type(lf, ixa);
#line 7202 "src/taumir/lower.tr"
            if (((want_elem == 0LL) && (ixat == 4LL))) {
#line 7202 "src/taumir/lower.tr"
                ixat = 0LL;
            }
#line 7203 "src/taumir/lower.tr"
            if ((ixat != want_elem)) {
#line 7203 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7204 "src/taumir/lower.tr"
            TrStr ixsym = _tr_str_lit_len("_tr_rt_list_index_i64", 21LL);
#line 7205 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7205 "src/taumir/lower.tr"
                TrStr _strtmp_t6462 = _tr_str_lit_len("_tr_rt_list_index_str", 21LL);
                _tr_str_release(ixsym);
                ixsym = _strtmp_t6462;
            }
#line 7206 "src/taumir/lower.tr"
            LModule_add_extern(m, ixsym);
#line 7207 "src/taumir/lower.tr"
            List_i64* ixargs = (void*)List_i64_new();
#line 7208 "src/taumir/lower.tr"
            List_i64_append(ixargs, ovm);
#line 7209 "src/taumir/lower.tr"
            List_i64_append(ixargs, ixa);
#line 7210 "src/taumir/lower.tr"
            long long ixd = LFunc_new_vreg(lf);
#line 7211 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ixd, ixsym, ixargs));
#line 7212 "src/taumir/lower.tr"
            long long _trret1794 = ixd;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(ixsym);
            return _trret1794;
        }
#line 7213 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("contains", 8LL))) && (margs->len == 1LL))) {
#line 7214 "src/taumir/lower.tr"
            long long cxa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7215 "src/taumir/lower.tr"
            if ((cxa < 0LL)) {
#line 7215 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7216 "src/taumir/lower.tr"
            long long cxat = LFunc_vreg_type(lf, cxa);
#line 7217 "src/taumir/lower.tr"
            if (((want_elem == 0LL) && (cxat == 4LL))) {
#line 7217 "src/taumir/lower.tr"
                cxat = 0LL;
            }
#line 7218 "src/taumir/lower.tr"
            if ((cxat != want_elem)) {
#line 7218 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7219 "src/taumir/lower.tr"
            TrStr cxsym = _tr_str_lit_len("_tr_rt_list_contains_i64", 24LL);
#line 7220 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7220 "src/taumir/lower.tr"
                TrStr _strtmp_t6463 = _tr_str_lit_len("_tr_rt_list_contains_str", 24LL);
                _tr_str_release(cxsym);
                cxsym = _strtmp_t6463;
            }
#line 7221 "src/taumir/lower.tr"
            LModule_add_extern(m, cxsym);
#line 7222 "src/taumir/lower.tr"
            List_i64* cxargs = (void*)List_i64_new();
#line 7223 "src/taumir/lower.tr"
            List_i64_append(cxargs, ovm);
#line 7224 "src/taumir/lower.tr"
            List_i64_append(cxargs, cxa);
#line 7225 "src/taumir/lower.tr"
            long long cxd = LFunc_new_vreg(lf);
#line 7226 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(cxd, cxsym, cxargs));
#line 7227 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, cxd, 4LL);
#line 7228 "src/taumir/lower.tr"
            long long _trret1795 = cxd;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(cxsym);
            return _trret1795;
        }
#line 7229 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("count", 5LL))) && (margs->len == 1LL))) {
#line 7230 "src/taumir/lower.tr"
            long long cta = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7231 "src/taumir/lower.tr"
            if ((cta < 0LL)) {
#line 7231 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7232 "src/taumir/lower.tr"
            long long ctat = LFunc_vreg_type(lf, cta);
#line 7233 "src/taumir/lower.tr"
            if (((want_elem == 0LL) && (ctat == 4LL))) {
#line 7233 "src/taumir/lower.tr"
                ctat = 0LL;
            }
#line 7234 "src/taumir/lower.tr"
            if ((ctat != want_elem)) {
#line 7234 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7235 "src/taumir/lower.tr"
            TrStr ctsym = _tr_str_lit_len("_tr_rt_list_count_i64", 21LL);
#line 7236 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7236 "src/taumir/lower.tr"
                TrStr _strtmp_t6464 = _tr_str_lit_len("_tr_rt_list_count_str", 21LL);
                _tr_str_release(ctsym);
                ctsym = _strtmp_t6464;
            }
#line 7237 "src/taumir/lower.tr"
            LModule_add_extern(m, ctsym);
#line 7238 "src/taumir/lower.tr"
            List_i64* ctargs = (void*)List_i64_new();
#line 7239 "src/taumir/lower.tr"
            List_i64_append(ctargs, ovm);
#line 7240 "src/taumir/lower.tr"
            List_i64_append(ctargs, cta);
#line 7241 "src/taumir/lower.tr"
            long long ctd = LFunc_new_vreg(lf);
#line 7242 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(ctd, ctsym, ctargs));
#line 7243 "src/taumir/lower.tr"
            long long _trret1796 = ctd;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(ctsym);
            return _trret1796;
        }
#line 7244 "src/taumir/lower.tr"
        if (((((_tr_str_eqv((method), (_tr_str_lit_len("min", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("min_val", 7LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL)))) && (margs->len == 0LL))) {
#line 7245 "src/taumir/lower.tr"
            if ((want_elem != 0LL)) {
#line 7245 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7246 "src/taumir/lower.tr"
            TrStr mmsym = _tr_str_lit_len("_tr_rt_list_min_i64", 19LL);
#line 7247 "src/taumir/lower.tr"
            if ((_tr_str_eqv((method), (_tr_str_lit_len("max", 3LL))) || _tr_str_eqv((method), (_tr_str_lit_len("max_val", 7LL))))) {
#line 7247 "src/taumir/lower.tr"
                TrStr _strtmp_t6465 = _tr_str_lit_len("_tr_rt_list_max_i64", 19LL);
                _tr_str_release(mmsym);
                mmsym = _strtmp_t6465;
            }
#line 7248 "src/taumir/lower.tr"
            long long _trret1797 = _list_call1(m, lf, mmsym, ovm, 0LL);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(mmsym);
            return _trret1797;
        }
#line 7249 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("sum", 3LL))) && (margs->len == 0LL))) {
#line 7250 "src/taumir/lower.tr"
            if ((want_elem == 0LL)) {
#line 7251 "src/taumir/lower.tr"
                long long _trret1798 = _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_sum_i64", 19LL), ovm, 0LL);
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _trret1798;
            }
#line 7252 "src/taumir/lower.tr"
            if ((want_elem == 5LL)) {
#line 7253 "src/taumir/lower.tr"
                LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_sum_f64", 19LL));
#line 7254 "src/taumir/lower.tr"
                long long fsum = LFunc_new_vreg(lf);
#line 7255 "src/taumir/lower.tr"
                List_i64* fsa = (void*)List_i64_new();
#line 7256 "src/taumir/lower.tr"
                List_i64_append(fsa, ovm);
#line 7257 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_ICall(fsum, _tr_str_lit_len("_tr_rt_list_sum_f64", 19LL), fsa));
#line 7258 "src/taumir/lower.tr"
                long long fsr = LFunc_new_vreg(lf);
#line 7259 "src/taumir/lower.tr"
                LFunc_emit(lf, LInst_ctor_IBitsF(fsr, fsum));
#line 7260 "src/taumir/lower.tr"
                LFunc_set_vreg_type(lf, fsr, 5LL);
#line 7261 "src/taumir/lower.tr"
                long long _trret1799 = fsr;
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return _trret1799;
            }
#line 7262 "src/taumir/lower.tr"
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return (-1LL);
        }
#line 7263 "src/taumir/lower.tr"
        if (((_tr_str_eqv((method), (_tr_str_lit_len("clone", 5LL))) || _tr_str_eqv((method), (_tr_str_lit_len("copy", 4LL)))) && (margs->len == 0LL))) {
#line 7264 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_clone", 17LL));
#line 7265 "src/taumir/lower.tr"
            List_i64* cla = (void*)List_i64_new();
#line 7266 "src/taumir/lower.tr"
            List_i64_append(cla, ovm);
#line 7267 "src/taumir/lower.tr"
            long long cld = LFunc_new_vreg(lf);
#line 7268 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(cld, _tr_str_lit_len("_tr_rt_list_clone", 17LL), cla));
#line 7269 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, cld, ovmt);
#line 7270 "src/taumir/lower.tr"
            long long _trret1800 = cld;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1800;
        }
#line 7271 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL))) && (margs->len == 1LL))) {
#line 7272 "src/taumir/lower.tr"
            long long rmi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7273 "src/taumir/lower.tr"
            if (((rmi < 0LL) || (LFunc_vreg_type(lf, rmi) != 0LL))) {
#line 7273 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7274 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_remove", 18LL));
#line 7275 "src/taumir/lower.tr"
            List_i64* rma = (void*)List_i64_new();
#line 7276 "src/taumir/lower.tr"
            List_i64_append(rma, ovm);
#line 7277 "src/taumir/lower.tr"
            List_i64_append(rma, rmi);
#line 7278 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_remove", 18LL), rma));
#line 7279 "src/taumir/lower.tr"
            long long _trret1801 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1801;
        }
#line 7280 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("swap", 4LL))) && (margs->len == 2LL))) {
#line 7281 "src/taumir/lower.tr"
            long long swi = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7282 "src/taumir/lower.tr"
            if (((swi < 0LL) || (LFunc_vreg_type(lf, swi) != 0LL))) {
#line 7282 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7283 "src/taumir/lower.tr"
            long long swj = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
#line 7284 "src/taumir/lower.tr"
            if (((swj < 0LL) || (LFunc_vreg_type(lf, swj) != 0LL))) {
#line 7284 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7285 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_swap", 16LL));
#line 7286 "src/taumir/lower.tr"
            List_i64* swa = (void*)List_i64_new();
#line 7287 "src/taumir/lower.tr"
            List_i64_append(swa, ovm);
#line 7288 "src/taumir/lower.tr"
            List_i64_append(swa, swi);
#line 7289 "src/taumir/lower.tr"
            List_i64_append(swa, swj);
#line 7290 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_swap", 16LL), swa));
#line 7291 "src/taumir/lower.tr"
            long long _trret1802 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1802;
        }
#line 7292 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("join", 4LL))) && (margs->len == 1LL))) {
#line 7293 "src/taumir/lower.tr"
            if ((ovmt != 3LL)) {
#line 7293 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7294 "src/taumir/lower.tr"
            long long jsep = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7295 "src/taumir/lower.tr"
            if (((jsep < 0LL) || (LFunc_vreg_type(lf, jsep) != 1LL))) {
#line 7295 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7296 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_join", 16LL));
#line 7297 "src/taumir/lower.tr"
            List_i64* jargs = (void*)List_i64_new();
#line 7298 "src/taumir/lower.tr"
            List_i64_append(jargs, ovm);
#line 7299 "src/taumir/lower.tr"
            List_i64_append(jargs, jsep);
#line 7300 "src/taumir/lower.tr"
            long long jd = LFunc_new_vreg(lf);
#line 7301 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall(jd, _tr_str_lit_len("_tr_rt_list_join", 16LL), jargs));
#line 7302 "src/taumir/lower.tr"
            LFunc_set_vreg_type(lf, jd, 1LL);
#line 7303 "src/taumir/lower.tr"
            _fresh_mark(lf, jd);
#line 7304 "src/taumir/lower.tr"
            long long _trret1803 = jd;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1803;
        }
#line 7305 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("is_empty", 8LL))) && (margs->len == 0LL))) {
#line 7306 "src/taumir/lower.tr"
            long long _trret1804 = _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_is_empty", 20LL), ovm, 4LL);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1804;
        }
#line 7307 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("first", 5LL))) && (margs->len == 0LL))) {
#line 7308 "src/taumir/lower.tr"
            long long _trret1805 = _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_first_i64", 21LL), ovm, want_elem);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1805;
        }
#line 7309 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("last", 4LL))) && (margs->len == 0LL))) {
#line 7310 "src/taumir/lower.tr"
            long long _trret1806 = _list_call1(m, lf, _tr_str_lit_len("_tr_rt_list_last_i64", 20LL), ovm, want_elem);
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1806;
        }
#line 7311 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("reverse", 7LL))) && (margs->len == 0LL))) {
#line 7312 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_reverse", 19LL));
#line 7313 "src/taumir/lower.tr"
            List_i64* rva = (void*)List_i64_new();
#line 7314 "src/taumir/lower.tr"
            List_i64_append(rva, ovm);
#line 7315 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_reverse", 19LL), rva));
#line 7316 "src/taumir/lower.tr"
            long long _trret1807 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1807;
        }
#line 7317 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL))) && (margs->len == 1LL))) {
#line 7318 "src/taumir/lower.tr"
            long long exo = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
#line 7319 "src/taumir/lower.tr"
            if (((exo < 0LL) || (LFunc_vreg_type(lf, exo) != ovmt))) {
#line 7319 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7320 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_extend", 18LL));
#line 7321 "src/taumir/lower.tr"
            List_i64* exa = (void*)List_i64_new();
#line 7322 "src/taumir/lower.tr"
            List_i64_append(exa, ovm);
#line 7323 "src/taumir/lower.tr"
            List_i64_append(exa, exo);
#line 7324 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_extend", 18LL), exa));
#line 7325 "src/taumir/lower.tr"
            long long _trret1808 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1808;
        }
#line 7326 "src/taumir/lower.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL))) && (margs->len == 0LL))) {
#line 7327 "src/taumir/lower.tr"
            LModule_add_extern(m, _tr_str_lit_len("_tr_rt_list_clear", 17LL));
#line 7328 "src/taumir/lower.tr"
            List_i64* cla = (void*)List_i64_new();
#line 7329 "src/taumir/lower.tr"
            List_i64_append(cla, ovm);
#line 7330 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit_len("_tr_rt_list_clear", 17LL), cla));
#line 7331 "src/taumir/lower.tr"
            long long _trret1809 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            return _trret1809;
        }
#line 7332 "src/taumir/lower.tr"
        if ((((_tr_str_eqv((method), (_tr_str_lit_len("sort", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_asc", 8LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) && (margs->len == 0LL))) {
#line 7333 "src/taumir/lower.tr"
            if (((want_elem != 0LL) && (want_elem != 1LL))) {
#line 7333 "src/taumir/lower.tr"
                _tr_str_release(recv_cls);
                _tr_str_release(amn);
                return (-1LL);
            }
#line 7334 "src/taumir/lower.tr"
            long long dirv = LFunc_new_vreg(lf);
#line 7335 "src/taumir/lower.tr"
            long long dir = 1LL;
#line 7336 "src/taumir/lower.tr"
            if (_tr_str_eqv((method), (_tr_str_lit_len("sort_desc", 9LL)))) {
#line 7336 "src/taumir/lower.tr"
                dir = (0LL - 1LL);
            }
#line 7337 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_IConst(dirv, dir));
#line 7338 "src/taumir/lower.tr"
            TrStr sortsym = _tr_str_lit_len("_tr_rt_list_sort", 16LL);
#line 7339 "src/taumir/lower.tr"
            if ((want_elem == 1LL)) {
#line 7339 "src/taumir/lower.tr"
                TrStr _strtmp_t6466 = _tr_str_lit_len("_tr_rt_list_sort_str", 20LL);
                _tr_str_release(sortsym);
                sortsym = _strtmp_t6466;
            }
#line 7340 "src/taumir/lower.tr"
            LModule_add_extern(m, sortsym);
#line 7341 "src/taumir/lower.tr"
            List_i64* soa = (void*)List_i64_new();
#line 7342 "src/taumir/lower.tr"
            List_i64_append(soa, ovm);
#line 7343 "src/taumir/lower.tr"
            List_i64_append(soa, dirv);
#line 7344 "src/taumir/lower.tr"
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), sortsym, soa));
#line 7345 "src/taumir/lower.tr"
            long long _trret1810 = ovm;
            _tr_str_release(recv_cls);
            _tr_str_release(amn);
            _tr_str_release(sortsym);
            return _trret1810;
        }
#line 7346 "src/taumir/lower.tr"
        _tr_str_release(recv_cls);
        _tr_str_release(amn);
        return (-1LL);
    } else if (1) {
        __auto_type _ = _t6376;
#line 7348 "src/taumir/lower.tr"
        return (-1LL);
    }
}

__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e) {
#line 7351 "src/taumir/lower.tr"
    long long r = _lower_expr_impl(m, lf, e);
#line 7352 "src/taumir/lower.tr"
    if (((r < 0LL) && _tr_str_eqv((m->fail_note), (_tr_str_lit_len("", 0LL))))) {
#line 7353 "src/taumir/lower.tr"
        m->fail_note = ({ TrStr _tr_cr = (_expr_kind(e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("unsupported expression: ", 24LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    }
#line 7354 "src/taumir/lower.tr"
    return r;
}

