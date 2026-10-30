#include "tauraro_types.h"

bool _is_alpha(long long c);
bool _is_digit(long long c);
bool _is_alnum(long long c);
bool _is_space(long long c);
TrStr map_base(TrStr words);
TrStr map_type(TrStr words, long long stars);
TrMap* _runtime_symbols();
bool _is_prim_type_word(TrStr w);
bool _is_ignored_word(TrStr w);
bool _is_decl_term(long long kind, TrStr text);
TrStr _join_words(List_TrStr* words);
long long _to_int(TrStr s);
long long _skip_gnu_attrs(List_ptr* toks, long long pos);
TrStr _scan_typedef_name(List_ptr* toks, long long pos);
bool _typedef_is_fnptr(List_ptr* toks, long long pos);
TrStr _scan_fnptr_name(List_ptr* toks, long long pos);
bool _is_ident_byte(long long c);
TrStr _rename_word(TrStr text, TrStr old, TrStr new_);
TrStr _basename(TrStr p);
TrStr _marker_file(TrStr line);
bool _marker_is_system(TrStr line);
TrStr _filter_to_target(TrStr raw, TrStr target);
TrStr _macro_name(TrStr rest);
TrMap* _load_baseline(TrStr cc);
TrMap* _target_define_names(TrStr header);
TrStr _lstrip(TrStr s);
bool _is_builtin_ty_name(TrStr n);
bool _is_libc_ty_name(TrStr n);
bool _is_system_record_ty(TrStr n);
TrStr _ident_at(TrStr text, long long start);
TrStr _opaque_fallbacks(TrStr body, TrMap* defined);
bool _is_single_string_literal(TrStr val);
bool _is_single_int_literal(TrStr val);
void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow);
TrStr _cxx_of(TrStr cc);
TrStr _macro_args(long long n, long long mode, long long atype, long long tp);
TrStr _ent_name(TrStr e);
long long _ent_arity(TrStr e);
long long _ent_typepos(TrStr e);
bool _is_ident_ch(long long c);
bool _param_ptr_type(TrStr body, TrStr pname);
bool _balanced_delims(TrStr s);
List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline);
bool _has_cc_error(TrStr errtxt);
TrMap* _macro_bad_names(TrStr errtxt);
TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp);
List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake);
TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrMap* _macro_remaining(List_TrStr* macros, TrMap* done);
void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros);
void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra);
TrStr _cxxwalk_src();
void _rm_files(TrStr files);
TrStr _local_exe(TrStr stem);
TrStr _detect_libclang(TrStr cc);
CppType* _cpp_parse_type(TrStr spelling);
TrStr _last_seg(TrStr s);
TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member);
TrStr _cpp_ident(TrStr s);
bool _is_clean_ident(TrStr s);
bool _is_tr_keyword(TrStr n);
TrStr _uniq_sym(TrStr base_sym, TrMap* used);
TrStr _cpp_tr_pname(TrStr pname);
TrStr _cpp_ctype(CppType* t);
TrStr _cpp_tr_type(CppType* t);
List_TrStr* _cpp_ret(CppType* rt, TrStr call);
TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque);
TrStr _cpp_qual(TrStr base, TrMap* class_qual);
List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque);
TrStr _ns_pop(TrStr path);
TrStr _ns_us(TrStr path);
TrStr _rstrip_cr(TrStr s);
TrStr _c_to_cpp(TrStr cn);
List_TrStr* _desc4(TrStr desc);
List_TrStr* _parse_tclass(TrStr rest);
TrStr _stars(long long n);
TrStr _ptr_wrap(TrStr inner, long long n);
TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names);
TrStr _shim_body(TrStr ret_ctype, TrStr body);
TrStr _fnptr_cast(TrStr fnty);
void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs);
TrStr _cpp_detect_include_dirs(TrStr cc);
TrStr _cpp_std_flag(TrStr extra);
long long _cpp_fatal_count(TrStr diag);
void _cpp_print_diag(TrStr diag);
bool _cpp_ir_is_empty(TrStr ir);
void _cpp_cleanup();
bool _is_expr_proxy_spec(TrStr s);
List_TrStr* _collect_specs(TrStr ir);

__attribute__((malloc,returns_nonnull,hot)) CTok* CTok_init(long long kind, TrStr text) {
#line 23 "src/bindgen.tr"
    CTok* t = ((CTok*)_tr_obj_alloc(sizeof(CTok)));
#line 24 "src/bindgen.tr"
    t->kind = kind;
#line 25 "src/bindgen.tr"
    t->text = _tr_str_retain(text);
#line 26 "src/bindgen.tr"
    return t;
}

__attribute__((malloc,returns_nonnull,hot)) Bindgen* Bindgen_init(List_ptr* toks) {
#line 307 "src/bindgen.tr"
    Bindgen* b = ((Bindgen*)_tr_obj_alloc(sizeof(Bindgen)));
#line 308 "src/bindgen.tr"
    b->toks = toks;
#line 309 "src/bindgen.tr"
    b->pos = 0LL;
#line 310 "src/bindgen.tr"
    b->funcs = StringBuilder_init(1024LL);
#line 311 "src/bindgen.tr"
    b->structs = StringBuilder_init(1024LL);
#line 312 "src/bindgen.tr"
    b->types = StringBuilder_init(512LL);
#line 313 "src/bindgen.tr"
    b->consts = StringBuilder_init(512LL);
#line 314 "src/bindgen.tr"
    b->n_funcs = 0LL;
#line 315 "src/bindgen.tr"
    b->n_structs = 0LL;
#line 316 "src/bindgen.tr"
    b->seen = _tr_dict_new(64LL);
#line 317 "src/bindgen.tr"
    b->defined = (void*)List_TrStr_new();
#line 318 "src/bindgen.tr"
    b->skip_syms = _runtime_symbols();
#line 319 "src/bindgen.tr"
    b->n_skipped = 0LL;
#line 320 "src/bindgen.tr"
    return b;
}

__attribute__((hot)) bool Bindgen_fresh(Bindgen* self, TrStr name) {
#line 325 "src/bindgen.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("", 0LL))) || _tr_dict_contains(self->seen, _tr_strz(name)))) {
#line 325 "src/bindgen.tr"
        return false;
    }
#line 326 "src/bindgen.tr"
    _tr_dict_set(self->seen, _tr_strz(name), true);
#line 327 "src/bindgen.tr"
    List_TrStr_append(self->defined, name);
#line 328 "src/bindgen.tr"
    return true;
}

__attribute__((hot)) TrStr Bindgen_ct(Bindgen* self) {
#line 331 "src/bindgen.tr"
    return _tr_str_retain(((CTok*)List_ptr_get(self->toks, self->pos))->text);
}

__attribute__((hot)) long long Bindgen_ck(Bindgen* self) {
#line 333 "src/bindgen.tr"
    return ((CTok*)List_ptr_get(self->toks, self->pos))->kind;
}

__attribute__((hot)) void Bindgen_adv(Bindgen* self) {
#line 335 "src/bindgen.tr"
    if ((self->pos < (self->toks->len - 1LL))) {
#line 335 "src/bindgen.tr"
        self->pos = (self->pos + 1LL);
    }
}

__attribute__((hot)) bool Bindgen_is_punct(Bindgen* self, TrStr p) {
#line 337 "src/bindgen.tr"
    return ((Bindgen_ck(self) == 4LL) && ({ TrStr _cmpa_t3001 = (Bindgen_ct(self)); TrStr _cmpb_t3002 = (p); _Bool _cmpr_t3003 = _tr_str_eqv(_cmpa_t3001, _cmpb_t3002); _tr_str_release(_cmpa_t3001); _cmpr_t3003; }));
}

__attribute__((hot)) long long Bindgen_nk(Bindgen* self) {
#line 339 "src/bindgen.tr"
    if (((self->pos + 1LL) < self->toks->len)) {
#line 339 "src/bindgen.tr"
        return ((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->kind;
    }
#line 340 "src/bindgen.tr"
    return 5LL;
}

__attribute__((hot)) TrStr Bindgen_nt(Bindgen* self) {
#line 342 "src/bindgen.tr"
    if (((self->pos + 1LL) < self->toks->len)) {
#line 342 "src/bindgen.tr"
        return _tr_str_retain(((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->text);
    }
#line 343 "src/bindgen.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void Bindgen_skip_balanced_parens(Bindgen* self) {
#line 346 "src/bindgen.tr"
    if ((!Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
#line 346 "src/bindgen.tr"
        return;
    }
#line 347 "src/bindgen.tr"
    long long depth = 0LL;
#line 348 "src/bindgen.tr"
    while ((Bindgen_ck(self) != 5LL)) {
#line 349 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL))) {
#line 349 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
#line 351 "src/bindgen.tr"
            depth = (depth - 1LL);
#line 352 "src/bindgen.tr"
            if ((depth == 0LL)) {
#line 353 "src/bindgen.tr"
                Bindgen_adv(self);
#line 354 "src/bindgen.tr"
                return;
            }
        }
#line 355 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_struct_body(Bindgen* self) {
#line 359 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 360 "src/bindgen.tr"
        long long depth = 0LL;
#line 361 "src/bindgen.tr"
        while ((Bindgen_ck(self) != 5LL)) {
#line 362 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 362 "src/bindgen.tr"
                depth = (depth + 1LL);
            } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 364 "src/bindgen.tr"
                depth = (depth - 1LL);
#line 365 "src/bindgen.tr"
                Bindgen_adv(self);
#line 366 "src/bindgen.tr"
                if ((depth == 0LL)) {
#line 366 "src/bindgen.tr"
                    break;
                }
#line 367 "src/bindgen.tr"
                continue;
            }
#line 368 "src/bindgen.tr"
            Bindgen_adv(self);
        }
    }
#line 369 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 369 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 370 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 370 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_to_semi(Bindgen* self) {
#line 373 "src/bindgen.tr"
    long long depth = 0LL;
#line 374 "src/bindgen.tr"
    while ((Bindgen_ck(self) != 5LL)) {
#line 375 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 375 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 376 "src/bindgen.tr"
            depth = (depth - 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL)) && (depth <= 0LL))) {
#line 378 "src/bindgen.tr"
            Bindgen_adv(self);
#line 379 "src/bindgen.tr"
            return;
        }
#line 380 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_braces(Bindgen* self) {
#line 385 "src/bindgen.tr"
    if ((!Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL)))) {
#line 385 "src/bindgen.tr"
        return;
    }
#line 386 "src/bindgen.tr"
    long long depth = 0LL;
#line 387 "src/bindgen.tr"
    while ((Bindgen_ck(self) != 5LL)) {
#line 388 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 388 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 390 "src/bindgen.tr"
            depth = (depth - 1LL);
#line 391 "src/bindgen.tr"
            Bindgen_adv(self);
#line 392 "src/bindgen.tr"
            if ((depth == 0LL)) {
#line 392 "src/bindgen.tr"
                return;
            }
#line 393 "src/bindgen.tr"
            continue;
        }
#line 394 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_attributes(Bindgen* self) {
#line 398 "src/bindgen.tr"
    while (((Bindgen_ck(self) == 0LL) && (({ TrStr _cmpa_t3004 = (Bindgen_ct(self)); TrStr _cmpb_t3005 = (_tr_str_lit_len("__attribute__", 13LL)); _Bool _cmpr_t3006 = _tr_str_eqv(_cmpa_t3004, _cmpb_t3005); _tr_str_release(_cmpa_t3004); _cmpr_t3006; }) || ({ TrStr _cmpa_t3007 = (Bindgen_ct(self)); TrStr _cmpb_t3008 = (_tr_str_lit_len("__declspec", 10LL)); _Bool _cmpr_t3009 = _tr_str_eqv(_cmpa_t3007, _cmpb_t3008); _tr_str_release(_cmpa_t3007); _cmpr_t3009; })))) {
#line 399 "src/bindgen.tr"
        Bindgen_adv(self);
#line 400 "src/bindgen.tr"
        Bindgen_skip_balanced_parens(self);
    }
}

__attribute__((hot)) bool Bindgen_func_def_follows(Bindgen* self) {
#line 406 "src/bindgen.tr"
    long long save = self->pos;
#line 407 "src/bindgen.tr"
    Bindgen_skip_balanced_parens(self);
#line 408 "src/bindgen.tr"
    bool isdef = Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL));
#line 409 "src/bindgen.tr"
    self->pos = save;
#line 410 "src/bindgen.tr"
    return isdef;
}

__attribute__((hot)) List_TrStr* Bindgen_read_type_words(Bindgen* self) {
#line 418 "src/bindgen.tr"
    List_TrStr* words = (void*)List_TrStr_new();
#line 419 "src/bindgen.tr"
    long long stars = 0LL;
#line 420 "src/bindgen.tr"
    bool going = true;
#line 421 "src/bindgen.tr"
    while (going) {
#line 422 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 423 "src/bindgen.tr"
            TrStr w = Bindgen_ct(self);
#line 424 "src/bindgen.tr"
            if (_is_ignored_word(w)) {
#line 425 "src/bindgen.tr"
                Bindgen_adv(self);
            } else if ((_tr_str_eqv((w), (_tr_str_lit_len("__declspec", 10LL))) || _tr_str_eqv((w), (_tr_str_lit_len("__attribute__", 13LL))))) {
#line 427 "src/bindgen.tr"
                Bindgen_adv(self);
#line 428 "src/bindgen.tr"
                Bindgen_skip_balanced_parens(self);
            } else if (((_tr_str_eqv((w), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((w), (_tr_str_lit_len("union", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("enum", 4LL))))) {
#line 430 "src/bindgen.tr"
                Bindgen_adv(self);
            } else if (((((words->len >= 1LL) || (stars > 0LL)) && ({ __auto_type _ord_t3011 = (Bindgen_nk(self)); TrStr _at_t3010 = (Bindgen_nt(self)); __auto_type _wr = (_is_decl_term(_ord_t3011, _at_t3010)); _tr_str_release(_at_t3010); _wr; })) && (!_is_prim_type_word(w)))) {
#line 432 "src/bindgen.tr"
                going = false;
            } else {
#line 434 "src/bindgen.tr"
                List_TrStr_append(words, w);
#line 435 "src/bindgen.tr"
                Bindgen_adv(self);
            }
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 437 "src/bindgen.tr"
            stars = (stars + 1LL);
#line 438 "src/bindgen.tr"
            Bindgen_adv(self);
        } else {
#line 440 "src/bindgen.tr"
            going = false;
        }
    }
#line 441 "src/bindgen.tr"
    List_TrStr* res = (void*)List_TrStr_new();
#line 442 "src/bindgen.tr"
    ({ TrStr _at_t3012 = (_join_words(words)); List_TrStr_append(res, _at_t3012); _tr_str_release(_at_t3012); });
#line 443 "src/bindgen.tr"
    ({ TrStr _at_t3013 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); List_TrStr_append(res, _at_t3013); _tr_str_release(_at_t3013); });
#line 444 "src/bindgen.tr"
    return res;
}

__attribute__((hot)) void Bindgen_emit_func(Bindgen* self, TrStr ret_words, long long ret_stars, TrStr name) {
#line 448 "src/bindgen.tr"
    if (_tr_dict_contains(self->skip_syms, _tr_strz(name))) {
#line 449 "src/bindgen.tr"
        self->n_skipped = (self->n_skipped + 1LL);
#line 450 "src/bindgen.tr"
        Bindgen_skip_to_semi(self);
#line 451 "src/bindgen.tr"
        return;
    }
#line 452 "src/bindgen.tr"
    ({ TrStr _sbt_t3014 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->funcs, _sbt_t3014); _tr_str_release(_sbt_t3014); });
#line 453 "src/bindgen.tr"
    Bindgen_adv(self);
#line 454 "src/bindgen.tr"
    bool first = true;
#line 455 "src/bindgen.tr"
    long long argn = 0LL;
#line 456 "src/bindgen.tr"
    bool variadic = false;
#line 457 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 458 "src/bindgen.tr"
        long long ploop_before = self->pos;
#line 459 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len(".", 1LL))) {
#line 460 "src/bindgen.tr"
            Bindgen_adv(self);
#line 461 "src/bindgen.tr"
            variadic = true;
#line 462 "src/bindgen.tr"
            continue;
        }
#line 463 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
#line 464 "src/bindgen.tr"
            Bindgen_adv(self);
#line 465 "src/bindgen.tr"
            continue;
        }
#line 466 "src/bindgen.tr"
        List_TrStr* tw = Bindgen_read_type_words(self);
#line 467 "src/bindgen.tr"
        TrStr pbase = List_TrStr_get(tw, 0LL);
#line 468 "src/bindgen.tr"
        long long pstars = ({ TrStr _at_t3015 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t3015)); _tr_str_release(_at_t3015); _wr; });
#line 469 "src/bindgen.tr"
        TrStr pname = _tr_str_lit_len("", 0LL);
#line 473 "src/bindgen.tr"
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) && ({ TrStr _cmpa_t3016 = (Bindgen_nt(self)); TrStr _cmpb_t3017 = (_tr_str_lit_len("*", 1LL)); _Bool _cmpr_t3018 = _tr_str_eqv(_cmpa_t3016, _cmpb_t3017); _tr_str_release(_cmpa_t3016); _cmpr_t3018; }))) {
#line 474 "src/bindgen.tr"
            Bindgen_adv(self);
#line 475 "src/bindgen.tr"
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 475 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 476 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 477 "src/bindgen.tr"
                TrStr _strtmp_t3019 = Bindgen_ct(self);
                _tr_str_release(pname);
                pname = _strtmp_t3019;
#line 478 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 479 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
#line 479 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 480 "src/bindgen.tr"
            Bindgen_skip_balanced_parens(self);
#line 481 "src/bindgen.tr"
            if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
#line 481 "src/bindgen.tr"
                TrStr _strtmp_t3020 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("arg", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(pname);
                pname = _strtmp_t3020;
            }
#line 482 "src/bindgen.tr"
            if (_is_tr_keyword(pname)) {
#line 482 "src/bindgen.tr"
                ({ TrStr _ap_t3021 = (_tr_str_lit_len("_", 1LL)); _tr_str_append(&pname, _ap_t3021); });
            }
#line 483 "src/bindgen.tr"
            if ((!first)) {
#line 483 "src/bindgen.tr"
                StringBuilder_append(self->funcs, _tr_str_lit_len(", ", 2LL));
            }
#line 484 "src/bindgen.tr"
            first = false;
#line 485 "src/bindgen.tr"
            ({ TrStr _sbt_t3022 = (_tr_strx_concatv((pname), (_tr_str_lit_len(": Pointer[void]", 15LL)))); StringBuilder_append(self->funcs, _sbt_t3022); _tr_str_release(_sbt_t3022); });
#line 486 "src/bindgen.tr"
            argn = (argn + 1LL);
#line 487 "src/bindgen.tr"
            if ((self->pos == ploop_before)) {
#line 487 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 488 "src/bindgen.tr"
            _tr_str_release(pbase);
            _tr_str_release(pname);
            continue;
        }
#line 489 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 490 "src/bindgen.tr"
            TrStr _strtmp_t3023 = Bindgen_ct(self);
            _tr_str_release(pname);
            pname = _strtmp_t3023;
#line 491 "src/bindgen.tr"
            Bindgen_adv(self);
        }
#line 492 "src/bindgen.tr"
        while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
#line 493 "src/bindgen.tr"
            Bindgen_adv(self);
#line 494 "src/bindgen.tr"
            while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 494 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 495 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
#line 495 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 496 "src/bindgen.tr"
            pstars = (pstars + 1LL);
        }
#line 497 "src/bindgen.tr"
        if (((_tr_str_eqv((pbase), (_tr_str_lit_len("void", 4LL))) && (pstars == 0LL)) && _tr_str_eqv((pname), (_tr_str_lit_len("", 0LL))))) {
#line 498 "src/bindgen.tr"
            _tr_str_release(pbase);
            _tr_str_release(pname);
            continue;
        }
#line 499 "src/bindgen.tr"
        if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
#line 500 "src/bindgen.tr"
            TrStr _strtmp_t3024 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("arg", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(pname);
            pname = _strtmp_t3024;
        }
#line 501 "src/bindgen.tr"
        if (_is_tr_keyword(pname)) {
#line 501 "src/bindgen.tr"
            ({ TrStr _ap_t3025 = (_tr_str_lit_len("_", 1LL)); _tr_str_append(&pname, _ap_t3025); });
        }
#line 502 "src/bindgen.tr"
        if ((!first)) {
#line 502 "src/bindgen.tr"
            StringBuilder_append(self->funcs, _tr_str_lit_len(", ", 2LL));
        }
#line 503 "src/bindgen.tr"
        first = false;
#line 504 "src/bindgen.tr"
        ({ TrStr _ord_t3026 = (({ TrStr _tr_cl = (_tr_strx_concatv((pname), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cr = (map_type(pbase, pstars)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(self->funcs, _ord_t3026); _tr_str_release(_ord_t3026); });
#line 505 "src/bindgen.tr"
        argn = (argn + 1LL);
#line 506 "src/bindgen.tr"
        if ((self->pos == ploop_before)) {
#line 506 "src/bindgen.tr"
            Bindgen_adv(self);
        }
        _tr_str_release(pbase);
        _tr_str_release(pname);
    }
#line 507 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
#line 507 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 508 "src/bindgen.tr"
    if (variadic) {
#line 508 "src/bindgen.tr"
        StringBuilder_append(self->funcs, _tr_str_lit_len(", args...", 9LL));
    }
#line 509 "src/bindgen.tr"
    StringBuilder_append(self->funcs, _tr_str_lit_len(")", 1LL));
#line 510 "src/bindgen.tr"
    TrStr rt = map_type(ret_words, ret_stars);
#line 511 "src/bindgen.tr"
    if ((!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL))))) {
#line 511 "src/bindgen.tr"
        ({ TrStr _sbt_t3027 = (_tr_strx_concatv((_tr_str_lit_len(" -> ", 4LL)), (rt))); StringBuilder_append(self->funcs, _sbt_t3027); _tr_str_release(_sbt_t3027); });
    }
#line 512 "src/bindgen.tr"
    StringBuilder_append(self->funcs, _tr_str_lit_len("\n", 1LL));
#line 513 "src/bindgen.tr"
    self->n_funcs = (self->n_funcs + 1LL);
#line 514 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 514 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 515 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 515 "src/bindgen.tr"
        Bindgen_adv(self);
    }
    _tr_str_release(rt);
}

__attribute__((hot)) void Bindgen_emit_struct(Bindgen* self, TrStr name, bool is_union) {
#line 518 "src/bindgen.tr"
    if ((!Bindgen_fresh(self, name))) {
#line 519 "src/bindgen.tr"
        Bindgen_skip_struct_body(self);
#line 520 "src/bindgen.tr"
        return;
    }
#line 521 "src/bindgen.tr"
    if (is_union) {
#line 521 "src/bindgen.tr"
        StringBuilder_append(self->structs, _tr_str_lit_len("@union\n", 7LL));
    }
#line 522 "src/bindgen.tr"
    ({ TrStr _sbt_t3028 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _sbt_t3028); _tr_str_release(_sbt_t3028); });
#line 523 "src/bindgen.tr"
    Bindgen_adv(self);
#line 524 "src/bindgen.tr"
    long long nfields = 0LL;
#line 525 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 526 "src/bindgen.tr"
        long long floop_before = self->pos;
#line 531 "src/bindgen.tr"
        if ((({ TrStr _cmpa_t3029 = (Bindgen_ct(self)); TrStr _cmpb_t3030 = (_tr_str_lit_len("struct", 6LL)); _Bool _cmpr_t3031 = _tr_str_eqv(_cmpa_t3029, _cmpb_t3030); _tr_str_release(_cmpa_t3029); _cmpr_t3031; }) || ({ TrStr _cmpa_t3032 = (Bindgen_ct(self)); TrStr _cmpb_t3033 = (_tr_str_lit_len("union", 5LL)); _Bool _cmpr_t3034 = _tr_str_eqv(_cmpa_t3032, _cmpb_t3033); _tr_str_release(_cmpa_t3032); _cmpr_t3034; }))) {
#line 532 "src/bindgen.tr"
            long long nsave = self->pos;
#line 533 "src/bindgen.tr"
            Bindgen_adv(self);
#line 534 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 534 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 535 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 536 "src/bindgen.tr"
                Bindgen_adv(self);
#line 537 "src/bindgen.tr"
                List_TrStr* nnames = (void*)List_TrStr_new();
#line 538 "src/bindgen.tr"
                List_TrStr* ntypes = (void*)List_TrStr_new();
#line 539 "src/bindgen.tr"
                while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 540 "src/bindgen.tr"
                    long long nb = self->pos;
#line 557 "src/bindgen.tr"
                    if ((({ TrStr _cmpa_t3035 = (Bindgen_ct(self)); TrStr _cmpb_t3036 = (_tr_str_lit_len("struct", 6LL)); _Bool _cmpr_t3037 = _tr_str_eqv(_cmpa_t3035, _cmpb_t3036); _tr_str_release(_cmpa_t3035); _cmpr_t3037; }) || ({ TrStr _cmpa_t3038 = (Bindgen_ct(self)); TrStr _cmpb_t3039 = (_tr_str_lit_len("union", 5LL)); _Bool _cmpr_t3040 = _tr_str_eqv(_cmpa_t3038, _cmpb_t3039); _tr_str_release(_cmpa_t3038); _cmpr_t3040; }))) {
#line 558 "src/bindgen.tr"
                        Bindgen_adv(self);
#line 559 "src/bindgen.tr"
                        if ((Bindgen_ck(self) == 0LL)) {
#line 559 "src/bindgen.tr"
                            Bindgen_adv(self);
                        }
#line 560 "src/bindgen.tr"
                        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 561 "src/bindgen.tr"
                            Bindgen_skip_struct_body(self);
#line 562 "src/bindgen.tr"
                            if ((self->pos == nb)) {
#line 562 "src/bindgen.tr"
                                Bindgen_adv(self);
                            }
#line 563 "src/bindgen.tr"
                            continue;
                        }
#line 564 "src/bindgen.tr"
                        self->pos = nb;
                    }
#line 565 "src/bindgen.tr"
                    List_TrStr* ntw = Bindgen_read_type_words(self);
#line 566 "src/bindgen.tr"
                    TrStr nbase = List_TrStr_get(ntw, 0LL);
#line 567 "src/bindgen.tr"
                    long long nstars = ({ TrStr _at_t3041 = (List_TrStr_get(ntw, 1LL)); __auto_type _wr = (_to_int(_at_t3041)); _tr_str_release(_at_t3041); _wr; });
#line 568 "src/bindgen.tr"
                    bool nmore = true;
#line 569 "src/bindgen.tr"
                    while (nmore) {
#line 570 "src/bindgen.tr"
                        if ((Bindgen_ck(self) != 0LL)) {
#line 570 "src/bindgen.tr"
                            break;
                        }
#line 571 "src/bindgen.tr"
                        TrStr nnm = Bindgen_ct(self);
#line 572 "src/bindgen.tr"
                        Bindgen_adv(self);
#line 573 "src/bindgen.tr"
                        TrStr narr = _tr_str_lit_len("", 0LL);
#line 574 "src/bindgen.tr"
                        bool nisarr = false;
#line 575 "src/bindgen.tr"
                        while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
#line 576 "src/bindgen.tr"
                            Bindgen_adv(self);
#line 577 "src/bindgen.tr"
                            if ((Bindgen_ck(self) == 1LL)) {
#line 577 "src/bindgen.tr"
                                TrStr _strtmp_t3042 = Bindgen_ct(self);
                                _tr_str_release(narr);
                                narr = _strtmp_t3042;
                            }
#line 578 "src/bindgen.tr"
                            while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 578 "src/bindgen.tr"
                                Bindgen_adv(self);
                            }
#line 579 "src/bindgen.tr"
                            if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
#line 579 "src/bindgen.tr"
                                Bindgen_adv(self);
                            }
#line 580 "src/bindgen.tr"
                            nisarr = true;
                        }
#line 581 "src/bindgen.tr"
                        List_TrStr_append(nnames, nnm);
#line 582 "src/bindgen.tr"
                        if ((nisarr && (!_tr_str_eqv((narr), (_tr_str_lit_len("", 0LL)))))) {
#line 582 "src/bindgen.tr"
                            ({ TrStr _at_t3043 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (map_type(nbase, nstars)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (narr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(ntypes, _at_t3043); _tr_str_release(_at_t3043); });
                        } else {
#line 583 "src/bindgen.tr"
                            ({ TrStr _at_t3044 = (map_type(nbase, nstars)); List_TrStr_append(ntypes, _at_t3044); _tr_str_release(_at_t3044); });
                        }
#line 584 "src/bindgen.tr"
                        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
#line 584 "src/bindgen.tr"
                            Bindgen_adv(self);
                        } else {
#line 585 "src/bindgen.tr"
                            nmore = false;
                        }
                        _tr_str_release(nnm);
                        _tr_str_release(narr);
                    }
#line 586 "src/bindgen.tr"
                    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 586 "src/bindgen.tr"
                        Bindgen_adv(self);
                    }
#line 587 "src/bindgen.tr"
                    if ((self->pos == nb)) {
#line 587 "src/bindgen.tr"
                        Bindgen_adv(self);
                    }
                    _tr_str_release(nbase);
                }
#line 588 "src/bindgen.tr"
                if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 588 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 589 "src/bindgen.tr"
                TrStr pfx = _tr_str_lit_len("", 0LL);
#line 590 "src/bindgen.tr"
                if ((Bindgen_ck(self) == 0LL)) {
#line 591 "src/bindgen.tr"
                    TrStr _strtmp_t3045 = Bindgen_ct(self);
                    _tr_str_release(pfx);
                    pfx = _strtmp_t3045;
#line 592 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 593 "src/bindgen.tr"
                while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 593 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 594 "src/bindgen.tr"
                if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 594 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 595 "src/bindgen.tr"
                long long nk = 0LL;
#line 596 "src/bindgen.tr"
                while ((nk < nnames->len)) {
#line 597 "src/bindgen.tr"
                    TrStr nfn = List_TrStr_get(nnames, nk);
#line 598 "src/bindgen.tr"
                    if ((!_tr_str_eqv((pfx), (_tr_str_lit_len("", 0LL))))) {
#line 598 "src/bindgen.tr"
                        TrStr _strtmp_t3046 = ({ TrStr _tr_cl = (_tr_strx_concatv((pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nfn)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(nfn);
                        nfn = _strtmp_t3046;
                    }
#line 599 "src/bindgen.tr"
                    ({ TrStr _ord_t3047 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (nfn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (List_TrStr_get(ntypes, nk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _ord_t3047); _tr_str_release(_ord_t3047); });
#line 600 "src/bindgen.tr"
                    nfields = (nfields + 1LL);
#line 601 "src/bindgen.tr"
                    nk = (nk + 1LL);
                    _tr_str_release(nfn);
                }
#line 602 "src/bindgen.tr"
                if ((self->pos == floop_before)) {
#line 602 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 603 "src/bindgen.tr"
                _tr_str_release(pfx);
                continue;
            }
#line 604 "src/bindgen.tr"
            self->pos = nsave;
        }
#line 605 "src/bindgen.tr"
        List_TrStr* tw = Bindgen_read_type_words(self);
#line 606 "src/bindgen.tr"
        TrStr fbase = List_TrStr_get(tw, 0LL);
#line 607 "src/bindgen.tr"
        long long fstars = ({ TrStr _at_t3048 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t3048)); _tr_str_release(_at_t3048); _wr; });
#line 612 "src/bindgen.tr"
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) && ({ TrStr _cmpa_t3049 = (Bindgen_nt(self)); TrStr _cmpb_t3050 = (_tr_str_lit_len("*", 1LL)); _Bool _cmpr_t3051 = _tr_str_eqv(_cmpa_t3049, _cmpb_t3050); _tr_str_release(_cmpa_t3049); _cmpr_t3051; }))) {
#line 613 "src/bindgen.tr"
            Bindgen_adv(self);
#line 614 "src/bindgen.tr"
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 614 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 615 "src/bindgen.tr"
            TrStr fpname = _tr_str_lit_len("", 0LL);
#line 616 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 617 "src/bindgen.tr"
                TrStr _strtmp_t3052 = Bindgen_ct(self);
                _tr_str_release(fpname);
                fpname = _strtmp_t3052;
#line 618 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 619 "src/bindgen.tr"
            while (((!Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 619 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 620 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
#line 620 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 621 "src/bindgen.tr"
            Bindgen_skip_balanced_parens(self);
#line 622 "src/bindgen.tr"
            if ((!_tr_str_eqv((fpname), (_tr_str_lit_len("", 0LL))))) {
#line 623 "src/bindgen.tr"
                ({ TrStr _sbt_t3053 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fpname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Pointer[void]\n", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _sbt_t3053); _tr_str_release(_sbt_t3053); });
#line 624 "src/bindgen.tr"
                nfields = (nfields + 1LL);
            }
#line 625 "src/bindgen.tr"
            Bindgen_skip_attributes(self);
#line 626 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 626 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 627 "src/bindgen.tr"
            if ((self->pos == floop_before)) {
#line 627 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 628 "src/bindgen.tr"
            _tr_str_release(fbase);
            _tr_str_release(fpname);
            continue;
        }
#line 629 "src/bindgen.tr"
        bool more = true;
#line 630 "src/bindgen.tr"
        bool first_decl = true;
#line 631 "src/bindgen.tr"
        while (more) {
#line 636 "src/bindgen.tr"
            long long dstars = fstars;
#line 637 "src/bindgen.tr"
            if ((!first_decl)) {
#line 638 "src/bindgen.tr"
                dstars = 0LL;
#line 639 "src/bindgen.tr"
                while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 640 "src/bindgen.tr"
                    dstars = (dstars + 1LL);
#line 641 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
            }
#line 642 "src/bindgen.tr"
            first_decl = false;
#line 643 "src/bindgen.tr"
            if ((Bindgen_ck(self) != 0LL)) {
#line 643 "src/bindgen.tr"
                break;
            }
#line 644 "src/bindgen.tr"
            TrStr fname = Bindgen_ct(self);
#line 645 "src/bindgen.tr"
            Bindgen_adv(self);
#line 646 "src/bindgen.tr"
            bool is_arr = false;
#line 647 "src/bindgen.tr"
            while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
#line 648 "src/bindgen.tr"
                Bindgen_adv(self);
#line 649 "src/bindgen.tr"
                TrStr arrn = _tr_str_lit_len("", 0LL);
#line 650 "src/bindgen.tr"
                if ((Bindgen_ck(self) == 1LL)) {
#line 650 "src/bindgen.tr"
                    TrStr _strtmp_t3054 = Bindgen_ct(self);
                    _tr_str_release(arrn);
                    arrn = _strtmp_t3054;
                }
#line 651 "src/bindgen.tr"
                while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 651 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 652 "src/bindgen.tr"
                if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
#line 652 "src/bindgen.tr"
                    Bindgen_adv(self);
                }
#line 653 "src/bindgen.tr"
                if ((!_tr_str_eqv((arrn), (_tr_str_lit_len("", 0LL))))) {
#line 654 "src/bindgen.tr"
                    ({ TrStr _ord_t3055 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (map_type(fbase, dstars)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("; ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arrn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _ord_t3055); _tr_str_release(_ord_t3055); });
#line 655 "src/bindgen.tr"
                    nfields = (nfields + 1LL);
#line 656 "src/bindgen.tr"
                    is_arr = true;
                }
                _tr_str_release(arrn);
            }
#line 657 "src/bindgen.tr"
            if ((!is_arr)) {
#line 658 "src/bindgen.tr"
                ({ TrStr _ord_t3056 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (map_type(fbase, dstars)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _ord_t3056); _tr_str_release(_ord_t3056); });
#line 659 "src/bindgen.tr"
                nfields = (nfields + 1LL);
            }
#line 660 "src/bindgen.tr"
            Bindgen_skip_attributes(self);
#line 661 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
#line 662 "src/bindgen.tr"
                Bindgen_adv(self);
            } else {
#line 664 "src/bindgen.tr"
                more = false;
            }
            _tr_str_release(fname);
        }
#line 665 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 665 "src/bindgen.tr"
            Bindgen_adv(self);
        }
#line 666 "src/bindgen.tr"
        if ((self->pos == floop_before)) {
#line 666 "src/bindgen.tr"
            Bindgen_adv(self);
        }
        _tr_str_release(fbase);
    }
#line 667 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 667 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 668 "src/bindgen.tr"
    if ((nfields == 0LL)) {
#line 668 "src/bindgen.tr"
        StringBuilder_append(self->structs, _tr_str_lit_len("    pass\n", 9LL));
    }
#line 669 "src/bindgen.tr"
    StringBuilder_append(self->structs, _tr_str_lit_len("\n", 1LL));
#line 670 "src/bindgen.tr"
    self->n_structs = (self->n_structs + 1LL);
#line 671 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 671 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 672 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 672 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_enum_expr(Bindgen* self) {
#line 676 "src/bindgen.tr"
    long long depth = 0LL;
#line 677 "src/bindgen.tr"
    while ((Bindgen_ck(self) != 5LL)) {
#line 678 "src/bindgen.tr"
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL)))) {
#line 678 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL)))) {
#line 679 "src/bindgen.tr"
            depth = (depth - 1LL);
        } else if (((depth <= 0LL) && (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))))) {
#line 680 "src/bindgen.tr"
            return;
        }
#line 681 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_emit_enum(Bindgen* self, TrStr name) {
#line 684 "src/bindgen.tr"
    if (((!_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, name))) {
#line 684 "src/bindgen.tr"
        ({ TrStr _sbt_t3057 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = c_int\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->types, _sbt_t3057); _tr_str_release(_sbt_t3057); });
    }
#line 685 "src/bindgen.tr"
    Bindgen_adv(self);
#line 686 "src/bindgen.tr"
    long long next_val = 0LL;
#line 687 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 688 "src/bindgen.tr"
        Bindgen_skip_attributes(self);
#line 689 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 689 "src/bindgen.tr"
            break;
        }
#line 690 "src/bindgen.tr"
        if ((Bindgen_ck(self) != 0LL)) {
#line 691 "src/bindgen.tr"
            Bindgen_adv(self);
#line 692 "src/bindgen.tr"
            continue;
        }
#line 693 "src/bindgen.tr"
        TrStr ename = Bindgen_ct(self);
#line 694 "src/bindgen.tr"
        Bindgen_adv(self);
#line 695 "src/bindgen.tr"
        Bindgen_skip_attributes(self);
#line 696 "src/bindgen.tr"
        long long ev = next_val;
#line 697 "src/bindgen.tr"
        bool simple = true;
#line 698 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("=", 1LL))) {
#line 699 "src/bindgen.tr"
            Bindgen_adv(self);
#line 702 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 1LL)) {
#line 703 "src/bindgen.tr"
                ev = ({ TrStr _at_t3058 = (Bindgen_ct(self)); __auto_type _wr = (_to_int(_at_t3058)); _tr_str_release(_at_t3058); _wr; });
#line 704 "src/bindgen.tr"
                Bindgen_adv(self);
#line 705 "src/bindgen.tr"
                if ((!(Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))))) {
#line 706 "src/bindgen.tr"
                    simple = false;
#line 707 "src/bindgen.tr"
                    Bindgen_skip_enum_expr(self);
                }
            } else {
#line 709 "src/bindgen.tr"
                simple = false;
#line 710 "src/bindgen.tr"
                Bindgen_skip_enum_expr(self);
            }
        }
#line 711 "src/bindgen.tr"
        if ((simple && Bindgen_fresh(self, ename))) {
#line 712 "src/bindgen.tr"
            ({ TrStr _ord_t3059 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ename))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": c_int = ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ev)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->consts, _ord_t3059); _tr_str_release(_ord_t3059); });
#line 713 "src/bindgen.tr"
            next_val = (ev + 1LL);
        }
#line 714 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
#line 714 "src/bindgen.tr"
            Bindgen_adv(self);
        }
        _tr_str_release(ename);
    }
#line 715 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
#line 715 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 716 "src/bindgen.tr"
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
#line 716 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 717 "src/bindgen.tr"
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 717 "src/bindgen.tr"
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_parse_decl(Bindgen* self) {
#line 720 "src/bindgen.tr"
    if ((Bindgen_ck(self) == 5LL)) {
#line 720 "src/bindgen.tr"
        return;
    }
#line 721 "src/bindgen.tr"
    TrStr lead = Bindgen_ct(self);
#line 722 "src/bindgen.tr"
    if (_tr_str_eqv((lead), (_tr_str_lit_len("typedef", 7LL)))) {
#line 723 "src/bindgen.tr"
        Bindgen_adv(self);
#line 724 "src/bindgen.tr"
        TrStr agg = Bindgen_ct(self);
#line 725 "src/bindgen.tr"
        if ((_tr_str_eqv((agg), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((agg), (_tr_str_lit_len("union", 5LL))))) {
#line 726 "src/bindgen.tr"
            bool is_u = _tr_str_eqv((agg), (_tr_str_lit_len("union", 5LL)));
#line 727 "src/bindgen.tr"
            Bindgen_adv(self);
#line 728 "src/bindgen.tr"
            TrStr tag = _tr_str_lit_len("", 0LL);
#line 729 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 730 "src/bindgen.tr"
                TrStr _strtmp_t3060 = Bindgen_ct(self);
                _tr_str_release(tag);
                tag = _strtmp_t3060;
#line 731 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 732 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 733 "src/bindgen.tr"
                TrStr tname = _scan_typedef_name(self->toks, self->pos);
#line 734 "src/bindgen.tr"
                TrStr cname = _tr_str_retain(tname);
#line 735 "src/bindgen.tr"
                if (_tr_str_eqv((cname), (_tr_str_lit_len("", 0LL)))) {
#line 735 "src/bindgen.tr"
                    TrStr _strtmp_t3061 = _tr_str_retain(tag);
                    _tr_str_release(cname);
                    cname = _strtmp_t3061;
                }
#line 736 "src/bindgen.tr"
                Bindgen_emit_struct(self, cname, is_u);
#line 737 "src/bindgen.tr"
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(tag);
                _tr_str_release(tname);
                _tr_str_release(cname);
                return;
            }
#line 739 "src/bindgen.tr"
            long long ostars = 0LL;
#line 740 "src/bindgen.tr"
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 741 "src/bindgen.tr"
                ostars = (ostars + 1LL);
#line 742 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 743 "src/bindgen.tr"
            TrStr oname = _tr_str_lit_len("", 0LL);
#line 744 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 745 "src/bindgen.tr"
                TrStr _strtmp_t3062 = Bindgen_ct(self);
                _tr_str_release(oname);
                oname = _strtmp_t3062;
            }
#line 746 "src/bindgen.tr"
            if ((((ostars > 0LL) && (!_tr_str_eqv((oname), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((tag), (_tr_str_lit_len("", 0LL)))))) {
#line 748 "src/bindgen.tr"
                if (Bindgen_fresh(self, tag)) {
#line 749 "src/bindgen.tr"
                    ({ TrStr _sbt_t3063 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (tag))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass    # opaque handle\n\n", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _sbt_t3063); _tr_str_release(_sbt_t3063); });
#line 750 "src/bindgen.tr"
                    self->n_structs = (self->n_structs + 1LL);
                }
#line 751 "src/bindgen.tr"
                if (Bindgen_fresh(self, oname)) {
#line 752 "src/bindgen.tr"
                    ({ TrStr _sbt_t3064 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (oname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = Pointer[", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tag)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->types, _sbt_t3064); _tr_str_release(_sbt_t3064); });
                }
            } else if (((!_tr_str_eqv((oname), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, oname))) {
#line 754 "src/bindgen.tr"
                ({ TrStr _sbt_t3065 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (oname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass    # opaque handle\n\n", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _sbt_t3065); _tr_str_release(_sbt_t3065); });
#line 755 "src/bindgen.tr"
                self->n_structs = (self->n_structs + 1LL);
            }
#line 756 "src/bindgen.tr"
            Bindgen_skip_to_semi(self);
#line 757 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(tag);
            _tr_str_release(oname);
            return;
        }
#line 758 "src/bindgen.tr"
        if (_tr_str_eqv((agg), (_tr_str_lit_len("enum", 4LL)))) {
#line 759 "src/bindgen.tr"
            Bindgen_adv(self);
#line 760 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 760 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 761 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 762 "src/bindgen.tr"
                TrStr ename = _scan_typedef_name(self->toks, self->pos);
#line 763 "src/bindgen.tr"
                Bindgen_emit_enum(self, ename);
#line 764 "src/bindgen.tr"
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(ename);
                return;
            }
#line 765 "src/bindgen.tr"
            Bindgen_skip_to_semi(self);
#line 766 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(agg);
            return;
        }
#line 767 "src/bindgen.tr"
        if (_typedef_is_fnptr(self->toks, self->pos)) {
#line 768 "src/bindgen.tr"
            TrStr fpn = _scan_fnptr_name(self->toks, self->pos);
#line 769 "src/bindgen.tr"
            if (((!_tr_str_eqv((fpn), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, fpn))) {
#line 769 "src/bindgen.tr"
                ({ TrStr _sbt_t3066 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (fpn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = Pointer[void]    # C function pointer\n", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->types, _sbt_t3066); _tr_str_release(_sbt_t3066); });
            }
#line 770 "src/bindgen.tr"
            Bindgen_skip_to_semi(self);
#line 771 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(fpn);
            return;
        }
#line 772 "src/bindgen.tr"
        List_TrStr* tw = Bindgen_read_type_words(self);
#line 773 "src/bindgen.tr"
        TrStr abase = List_TrStr_get(tw, 0LL);
#line 774 "src/bindgen.tr"
        long long astars = ({ TrStr _at_t3067 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t3067)); _tr_str_release(_at_t3067); _wr; });
#line 775 "src/bindgen.tr"
        TrStr aname = _tr_str_lit_len("", 0LL);
#line 776 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 776 "src/bindgen.tr"
            TrStr _strtmp_t3068 = Bindgen_ct(self);
            _tr_str_release(aname);
            aname = _strtmp_t3068;
        }
#line 777 "src/bindgen.tr"
        if (((((!_tr_str_eqv((aname), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((abase), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((aname), (abase)))) && Bindgen_fresh(self, aname))) {
#line 778 "src/bindgen.tr"
            ({ TrStr _ord_t3069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (aname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (map_type(abase, astars)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->types, _ord_t3069); _tr_str_release(_ord_t3069); });
        }
#line 779 "src/bindgen.tr"
        Bindgen_skip_to_semi(self);
#line 780 "src/bindgen.tr"
        _tr_str_release(lead);
        _tr_str_release(agg);
        _tr_str_release(abase);
        _tr_str_release(aname);
        return;
    }
#line 781 "src/bindgen.tr"
    if ((_tr_str_eqv((lead), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((lead), (_tr_str_lit_len("union", 5LL))))) {
#line 782 "src/bindgen.tr"
        bool is_u2 = _tr_str_eqv((lead), (_tr_str_lit_len("union", 5LL)));
#line 783 "src/bindgen.tr"
        Bindgen_adv(self);
#line 784 "src/bindgen.tr"
        TrStr tag2 = _tr_str_lit_len("", 0LL);
#line 785 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 786 "src/bindgen.tr"
            TrStr _strtmp_t3070 = Bindgen_ct(self);
            _tr_str_release(tag2);
            tag2 = _strtmp_t3070;
#line 787 "src/bindgen.tr"
            Bindgen_adv(self);
        }
#line 788 "src/bindgen.tr"
        if ((Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL)) && (!_tr_str_eqv((tag2), (_tr_str_lit_len("", 0LL)))))) {
#line 789 "src/bindgen.tr"
            Bindgen_emit_struct(self, tag2, is_u2);
#line 790 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(tag2);
            return;
        }
#line 794 "src/bindgen.tr"
        if (((Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL)) && (!_tr_str_eqv((tag2), (_tr_str_lit_len("", 0LL))))) && Bindgen_fresh(self, tag2))) {
#line 795 "src/bindgen.tr"
            ({ TrStr _sbt_t3071 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (tag2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass    # opaque (forward-declared)\n\n", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(self->structs, _sbt_t3071); _tr_str_release(_sbt_t3071); });
#line 796 "src/bindgen.tr"
            self->n_structs = (self->n_structs + 1LL);
        }
#line 797 "src/bindgen.tr"
        Bindgen_skip_to_semi(self);
#line 798 "src/bindgen.tr"
        _tr_str_release(lead);
        _tr_str_release(tag2);
        return;
    }
#line 799 "src/bindgen.tr"
    if (_tr_str_eqv((lead), (_tr_str_lit_len("enum", 4LL)))) {
#line 800 "src/bindgen.tr"
        Bindgen_adv(self);
#line 801 "src/bindgen.tr"
        TrStr etag = _tr_str_lit_len("", 0LL);
#line 802 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 803 "src/bindgen.tr"
            TrStr _strtmp_t3072 = Bindgen_ct(self);
            _tr_str_release(etag);
            etag = _strtmp_t3072;
#line 804 "src/bindgen.tr"
            Bindgen_adv(self);
        }
#line 805 "src/bindgen.tr"
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
#line 806 "src/bindgen.tr"
            Bindgen_emit_enum(self, etag);
#line 807 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(etag);
            return;
        }
#line 808 "src/bindgen.tr"
        Bindgen_skip_to_semi(self);
#line 809 "src/bindgen.tr"
        _tr_str_release(lead);
        _tr_str_release(etag);
        return;
    }
#line 810 "src/bindgen.tr"
    List_TrStr* rw = Bindgen_read_type_words(self);
#line 811 "src/bindgen.tr"
    TrStr rbase = List_TrStr_get(rw, 0LL);
#line 812 "src/bindgen.tr"
    long long rstars = ({ TrStr _at_t3073 = (List_TrStr_get(rw, 1LL)); __auto_type _wr = (_to_int(_at_t3073)); _tr_str_release(_at_t3073); _wr; });
#line 813 "src/bindgen.tr"
    TrStr fname = _tr_str_lit_len("", 0LL);
#line 814 "src/bindgen.tr"
    if ((Bindgen_ck(self) == 0LL)) {
#line 815 "src/bindgen.tr"
        TrStr _strtmp_t3074 = Bindgen_ct(self);
        _tr_str_release(fname);
        fname = _strtmp_t3074;
#line 816 "src/bindgen.tr"
        Bindgen_adv(self);
    }
#line 817 "src/bindgen.tr"
    if (((!_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL)))) && Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
#line 818 "src/bindgen.tr"
        if (Bindgen_func_def_follows(self)) {
#line 819 "src/bindgen.tr"
            Bindgen_skip_balanced_parens(self);
#line 820 "src/bindgen.tr"
            Bindgen_skip_braces(self);
#line 821 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 821 "src/bindgen.tr"
                Bindgen_adv(self);
            }
#line 822 "src/bindgen.tr"
            _tr_str_release(lead);
            _tr_str_release(rbase);
            _tr_str_release(fname);
            return;
        }
#line 823 "src/bindgen.tr"
        Bindgen_emit_func(self, rbase, rstars, fname);
#line 824 "src/bindgen.tr"
        _tr_str_release(lead);
        _tr_str_release(rbase);
        _tr_str_release(fname);
        return;
    }
#line 830 "src/bindgen.tr"
    if ((_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL))) && Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
#line 831 "src/bindgen.tr"
        long long saved = self->pos;
#line 832 "src/bindgen.tr"
        Bindgen_adv(self);
#line 833 "src/bindgen.tr"
        long long pdstars = 0LL;
#line 834 "src/bindgen.tr"
        bool scan = true;
#line 835 "src/bindgen.tr"
        while (scan) {
#line 836 "src/bindgen.tr"
            if ((Bindgen_ck(self) == 0LL)) {
#line 837 "src/bindgen.tr"
                TrStr dw = Bindgen_ct(self);
#line 838 "src/bindgen.tr"
                if ((_tr_str_eqv((dw), (_tr_str_lit_len("__attribute__", 13LL))) || _tr_str_eqv((dw), (_tr_str_lit_len("__declspec", 10LL))))) {
#line 839 "src/bindgen.tr"
                    Bindgen_adv(self);
#line 840 "src/bindgen.tr"
                    Bindgen_skip_balanced_parens(self);
                } else if (_is_ignored_word(dw)) {
#line 842 "src/bindgen.tr"
                    Bindgen_adv(self);
                } else {
#line 844 "src/bindgen.tr"
                    scan = false;
                }
            } else if (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
#line 846 "src/bindgen.tr"
                pdstars = (pdstars + 1LL);
#line 847 "src/bindgen.tr"
                Bindgen_adv(self);
            } else {
#line 849 "src/bindgen.tr"
                scan = false;
            }
        }
#line 850 "src/bindgen.tr"
        TrStr pdname = _tr_str_lit_len("", 0LL);
#line 851 "src/bindgen.tr"
        if ((Bindgen_ck(self) == 0LL)) {
#line 852 "src/bindgen.tr"
            TrStr _strtmp_t3075 = Bindgen_ct(self);
            _tr_str_release(pdname);
            pdname = _strtmp_t3075;
#line 853 "src/bindgen.tr"
            Bindgen_adv(self);
        }
#line 854 "src/bindgen.tr"
        if ((((!_tr_str_eqv((pdname), (_tr_str_lit_len("", 0LL)))) && (pdstars == 0LL)) && Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL)))) {
#line 855 "src/bindgen.tr"
            Bindgen_adv(self);
#line 856 "src/bindgen.tr"
            if (Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL))) {
#line 857 "src/bindgen.tr"
                if (Bindgen_func_def_follows(self)) {
#line 858 "src/bindgen.tr"
                    Bindgen_skip_balanced_parens(self);
#line 859 "src/bindgen.tr"
                    Bindgen_skip_braces(self);
#line 860 "src/bindgen.tr"
                    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
#line 860 "src/bindgen.tr"
                        Bindgen_adv(self);
                    }
#line 861 "src/bindgen.tr"
                    _tr_str_release(lead);
                    _tr_str_release(rbase);
                    _tr_str_release(fname);
                    _tr_str_release(pdname);
                    return;
                }
#line 862 "src/bindgen.tr"
                Bindgen_emit_func(self, rbase, rstars, pdname);
#line 863 "src/bindgen.tr"
                _tr_str_release(lead);
                _tr_str_release(rbase);
                _tr_str_release(fname);
                _tr_str_release(pdname);
                return;
            }
        }
#line 864 "src/bindgen.tr"
        self->pos = saved;
    }
#line 865 "src/bindgen.tr"
    Bindgen_skip_to_semi(self);
    _tr_str_release(lead);
    _tr_str_release(rbase);
    _tr_str_release(fname);
}

__attribute__((hot)) void Bindgen_run(Bindgen* self) {
#line 868 "src/bindgen.tr"
    while ((Bindgen_ck(self) != 5LL)) {
#line 869 "src/bindgen.tr"
        long long before = self->pos;
#line 870 "src/bindgen.tr"
        Bindgen_parse_decl(self);
#line 871 "src/bindgen.tr"
        if ((self->pos == before)) {
#line 871 "src/bindgen.tr"
            Bindgen_adv(self);
        }
    }
}

__attribute__((hot)) bool _is_alpha(long long c) {
#line 29 "src/bindgen.tr"
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _is_digit(long long c) {
#line 31 "src/bindgen.tr"
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool _is_alnum(long long c) {
#line 33 "src/bindgen.tr"
    return (_is_alpha(c) || _is_digit(c));
}

__attribute__((hot)) bool _is_space(long long c) {
#line 35 "src/bindgen.tr"
    return ((((c == 32LL) || (c == 9LL)) || (c == 10LL)) || (c == 13LL));
}

__attribute__((hot)) List_ptr* tokenize_c(TrStr src) {
#line 38 "src/bindgen.tr"
    List_ptr* toks = (void*)List_ptr_new();
#line 39 "src/bindgen.tr"
    long long n = _tr_str_lenv((src));
#line 40 "src/bindgen.tr"
    long long i = 0LL;
#line 41 "src/bindgen.tr"
    while ((i < n)) {
#line 42 "src/bindgen.tr"
        char c = _tr_strz(src)[i];
#line 43 "src/bindgen.tr"
        if (_is_space(c)) {
#line 44 "src/bindgen.tr"
            i = (i + 1LL);
#line 45 "src/bindgen.tr"
            continue;
        }
#line 46 "src/bindgen.tr"
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 47LL))) {
#line 47 "src/bindgen.tr"
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
#line 47 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 48 "src/bindgen.tr"
            continue;
        }
#line 49 "src/bindgen.tr"
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 42LL))) {
#line 50 "src/bindgen.tr"
            i = (i + 2LL);
#line 51 "src/bindgen.tr"
            while ((((i + 1LL) < n) && (!((_tr_strz(src)[i] == 42LL) && (_tr_strz(src)[(i + 1LL)] == 47LL))))) {
#line 51 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 52 "src/bindgen.tr"
            i = (i + 2LL);
#line 53 "src/bindgen.tr"
            continue;
        }
#line 54 "src/bindgen.tr"
        if ((c == 35LL)) {
#line 55 "src/bindgen.tr"
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
#line 55 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 56 "src/bindgen.tr"
            continue;
        }
#line 57 "src/bindgen.tr"
        if (_is_alpha(c)) {
#line 58 "src/bindgen.tr"
            long long start = i;
#line 59 "src/bindgen.tr"
            while (((i < n) && _is_alnum(_tr_strz(src)[i]))) {
#line 59 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 60 "src/bindgen.tr"
            ({ TrStr _at_t3076 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(0LL, _at_t3076)); _tr_str_release(_at_t3076); });
#line 61 "src/bindgen.tr"
            continue;
        }
#line 62 "src/bindgen.tr"
        if (_is_digit(c)) {
#line 63 "src/bindgen.tr"
            long long start = i;
#line 64 "src/bindgen.tr"
            while (((i < n) && ((_is_alnum(_tr_strz(src)[i]) || (_tr_strz(src)[i] == 46LL)) || (_tr_strz(src)[i] == 120LL)))) {
#line 64 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 65 "src/bindgen.tr"
            ({ TrStr _at_t3077 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(1LL, _at_t3077)); _tr_str_release(_at_t3077); });
#line 66 "src/bindgen.tr"
            continue;
        }
#line 67 "src/bindgen.tr"
        if ((c == 34LL)) {
#line 68 "src/bindgen.tr"
            long long start = i;
#line 69 "src/bindgen.tr"
            i = (i + 1LL);
#line 70 "src/bindgen.tr"
            while (((i < n) && (_tr_strz(src)[i] != 34LL))) {
#line 71 "src/bindgen.tr"
                if ((_tr_strz(src)[i] == 92LL)) {
#line 71 "src/bindgen.tr"
                    i = (i + 1LL);
                }
#line 72 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 73 "src/bindgen.tr"
            i = (i + 1LL);
#line 74 "src/bindgen.tr"
            ({ TrStr _at_t3078 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(2LL, _at_t3078)); _tr_str_release(_at_t3078); });
#line 75 "src/bindgen.tr"
            continue;
        }
#line 76 "src/bindgen.tr"
        if ((c == 39LL)) {
#line 77 "src/bindgen.tr"
            long long start = i;
#line 78 "src/bindgen.tr"
            i = (i + 1LL);
#line 79 "src/bindgen.tr"
            while (((i < n) && (_tr_strz(src)[i] != 39LL))) {
#line 80 "src/bindgen.tr"
                if ((_tr_strz(src)[i] == 92LL)) {
#line 80 "src/bindgen.tr"
                    i = (i + 1LL);
                }
#line 81 "src/bindgen.tr"
                i = (i + 1LL);
            }
#line 82 "src/bindgen.tr"
            i = (i + 1LL);
#line 83 "src/bindgen.tr"
            ({ TrStr _at_t3079 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(3LL, _at_t3079)); _tr_str_release(_at_t3079); });
#line 84 "src/bindgen.tr"
            continue;
        }
#line 85 "src/bindgen.tr"
        ({ TrStr _at_t3080 = (_tr_str_slicev((src), i, (i + 1LL))); List_ptr_append(toks, CTok_init(4LL, _at_t3080)); _tr_str_release(_at_t3080); });
#line 86 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 87 "src/bindgen.tr"
    List_ptr_append(toks, CTok_init(5LL, _tr_str_lit_len("", 0LL)));
#line 88 "src/bindgen.tr"
    return toks;
}

__attribute__((hot)) TrStr map_base(TrStr words) {
#line 92 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("void", 4LL)))) {
#line 92 "src/bindgen.tr"
        return _tr_str_lit_len("void", 4LL);
    }
#line 93 "src/bindgen.tr"
    if (((_tr_str_eqv((words), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((words), (_tr_str_lit_len("signed", 6LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed int", 10LL))))) {
#line 93 "src/bindgen.tr"
        return _tr_str_lit_len("c_int", 5LL);
    }
#line 94 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned int", 12LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned", 8LL))))) {
#line 94 "src/bindgen.tr"
        return _tr_str_lit_len("c_uint", 6LL);
    }
#line 95 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("char", 4LL)))) {
#line 95 "src/bindgen.tr"
        return _tr_str_lit_len("c_char", 6LL);
    }
#line 96 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("unsigned char", 13LL)))) {
#line 96 "src/bindgen.tr"
        return _tr_str_lit_len("c_uchar", 7LL);
    }
#line 97 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("signed char", 11LL)))) {
#line 97 "src/bindgen.tr"
        return _tr_str_lit_len("c_schar", 7LL);
    }
#line 98 "src/bindgen.tr"
    if (((_tr_str_eqv((words), (_tr_str_lit_len("short", 5LL))) || _tr_str_eqv((words), (_tr_str_lit_len("short int", 9LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed short", 12LL))))) {
#line 98 "src/bindgen.tr"
        return _tr_str_lit_len("c_short", 7LL);
    }
#line 99 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned short", 14LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned short int", 18LL))))) {
#line 99 "src/bindgen.tr"
        return _tr_str_lit_len("c_ushort", 8LL);
    }
#line 100 "src/bindgen.tr"
    if (((_tr_str_eqv((words), (_tr_str_lit_len("long", 4LL))) || _tr_str_eqv((words), (_tr_str_lit_len("long int", 8LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed long", 11LL))))) {
#line 100 "src/bindgen.tr"
        return _tr_str_lit_len("c_long", 6LL);
    }
#line 101 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned long", 13LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned long int", 17LL))))) {
#line 101 "src/bindgen.tr"
        return _tr_str_lit_len("c_ulong", 7LL);
    }
#line 102 "src/bindgen.tr"
    if (((_tr_str_eqv((words), (_tr_str_lit_len("long long", 9LL))) || _tr_str_eqv((words), (_tr_str_lit_len("long long int", 13LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed long long", 16LL))))) {
#line 102 "src/bindgen.tr"
        return _tr_str_lit_len("c_longlong", 10LL);
    }
#line 103 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned long long", 18LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned long long int", 22LL))))) {
#line 103 "src/bindgen.tr"
        return _tr_str_lit_len("c_ulonglong", 11LL);
    }
#line 104 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("va_list", 7LL))) || _tr_str_eqv((words), (_tr_str_lit_len("__builtin_va_list", 17LL))))) {
#line 104 "src/bindgen.tr"
        return _tr_str_lit_len("Pointer[void]", 13LL);
    }
#line 105 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("float", 5LL)))) {
#line 105 "src/bindgen.tr"
        return _tr_str_lit_len("c_float", 7LL);
    }
#line 106 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("double", 6LL)))) {
#line 106 "src/bindgen.tr"
        return _tr_str_lit_len("c_double", 8LL);
    }
#line 107 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("long double", 11LL)))) {
#line 107 "src/bindgen.tr"
        return _tr_str_lit_len("c_ldouble", 9LL);
    }
#line 108 "src/bindgen.tr"
    if ((_tr_str_eqv((words), (_tr_str_lit_len("_Bool", 5LL))) || _tr_str_eqv((words), (_tr_str_lit_len("bool", 4LL))))) {
#line 108 "src/bindgen.tr"
        return _tr_str_lit_len("bool", 4LL);
    }
#line 109 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("size_t", 6LL)))) {
#line 109 "src/bindgen.tr"
        return _tr_str_lit_len("c_size_t", 8LL);
    }
#line 110 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("ssize_t", 7LL)))) {
#line 110 "src/bindgen.tr"
        return _tr_str_lit_len("c_ssize_t", 9LL);
    }
#line 111 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("int8_t", 6LL)))) {
#line 111 "src/bindgen.tr"
        return _tr_str_lit_len("c_int8_t", 8LL);
    }
#line 112 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("int16_t", 7LL)))) {
#line 112 "src/bindgen.tr"
        return _tr_str_lit_len("c_int16_t", 9LL);
    }
#line 113 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("int32_t", 7LL)))) {
#line 113 "src/bindgen.tr"
        return _tr_str_lit_len("c_int32_t", 9LL);
    }
#line 114 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("int64_t", 7LL)))) {
#line 114 "src/bindgen.tr"
        return _tr_str_lit_len("c_int64_t", 9LL);
    }
#line 115 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint8_t", 7LL)))) {
#line 115 "src/bindgen.tr"
        return _tr_str_lit_len("c_uint8_t", 9LL);
    }
#line 116 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint16_t", 8LL)))) {
#line 116 "src/bindgen.tr"
        return _tr_str_lit_len("c_uint16_t", 10LL);
    }
#line 117 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint32_t", 8LL)))) {
#line 117 "src/bindgen.tr"
        return _tr_str_lit_len("c_uint32_t", 10LL);
    }
#line 118 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint64_t", 8LL)))) {
#line 118 "src/bindgen.tr"
        return _tr_str_lit_len("c_uint64_t", 10LL);
    }
#line 119 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("intptr_t", 8LL)))) {
#line 119 "src/bindgen.tr"
        return _tr_str_lit_len("c_intptr_t", 10LL);
    }
#line 120 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("uintptr_t", 9LL)))) {
#line 120 "src/bindgen.tr"
        return _tr_str_lit_len("c_uintptr_t", 11LL);
    }
#line 121 "src/bindgen.tr"
    if (_tr_str_eqv((words), (_tr_str_lit_len("ptrdiff_t", 9LL)))) {
#line 121 "src/bindgen.tr"
        return _tr_str_lit_len("c_ptrdiff_t", 11LL);
    }
#line 122 "src/bindgen.tr"
    return _tr_str_retain(words);
}

__attribute__((hot)) TrStr map_type(TrStr words, long long stars) {
#line 125 "src/bindgen.tr"
    TrStr base = map_base(words);
#line 126 "src/bindgen.tr"
    if ((stars <= 0LL)) {
#line 126 "src/bindgen.tr"
        TrStr _trret611 = _tr_str_retain(base);
        _tr_str_release(base);
        return _trret611;
    }
#line 127 "src/bindgen.tr"
    TrStr inner = _tr_str_retain(base);
#line 128 "src/bindgen.tr"
    if (_tr_str_eqv((base), (_tr_str_lit_len("c_char", 6LL)))) {
#line 128 "src/bindgen.tr"
        TrStr _strtmp_t3081 = _tr_str_lit_len("char", 4LL);
        _tr_str_release(inner);
        inner = _strtmp_t3081;
    }
#line 129 "src/bindgen.tr"
    if (_tr_str_eqv((base), (_tr_str_lit_len("void", 4LL)))) {
#line 129 "src/bindgen.tr"
        TrStr _strtmp_t3082 = _tr_str_lit_len("void", 4LL);
        _tr_str_release(inner);
        inner = _strtmp_t3082;
    }
#line 130 "src/bindgen.tr"
    TrStr ty = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (inner))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 131 "src/bindgen.tr"
    long long d = 1LL;
#line 132 "src/bindgen.tr"
    while ((d < stars)) {
#line 133 "src/bindgen.tr"
        TrStr _strtmp_t3083 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(ty);
        ty = _strtmp_t3083;
#line 134 "src/bindgen.tr"
        d = (d + 1LL);
    }
#line 135 "src/bindgen.tr"
    TrStr _trret612 = _tr_str_retain(ty);
    _tr_str_release(base);
    _tr_str_release(inner);
    _tr_str_release(ty);
    return _trret612;
}

__attribute__((hot)) TrMap* _runtime_symbols() {
#line 145 "src/bindgen.tr"
    TrStr names = _tr_str_lit_len("printf fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf scanf fscanf sscanf fopen fclose fread fwrite fseek ftell fgets fputs fgetc fputc getc putc getchar putchar puts perror fflush freopen setvbuf setbuf rewind remove rename tmpfile ungetc feof ferror clearerr ", 275LL);
#line 146 "src/bindgen.tr"
    ({ TrStr _ap_t3084 = (_tr_str_lit_len("malloc calloc realloc free aligned_alloc abort exit _Exit atexit quick_exit system getenv setenv unsetenv abs labs llabs atoi atol atoll atof strtol strtoul strtoll strtoull strtod strtof rand srand qsort bsearch div ldiv lldiv ", 228LL)); _tr_str_append(&names, _ap_t3084); });
#line 147 "src/bindgen.tr"
    ({ TrStr _ap_t3085 = (_tr_str_lit_len("memcpy memmove memset memcmp memchr strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strlen strnlen strdup strndup strtok strspn strcspn strpbrk strerror strcoll strxfrm strcasecmp strncasecmp ", 211LL)); _tr_str_append(&names, _ap_t3085); });
#line 148 "src/bindgen.tr"
    ({ TrStr _ap_t3086 = (_tr_str_lit_len("sqrt sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow cbrt hypot floor ceil round trunc fabs fmod ldexp frexp modf fmin fmax copysign nextafter nan isnan isinf signbit ", 195LL)); _tr_str_append(&names, _ap_t3086); });
#line 149 "src/bindgen.tr"
    ({ TrStr _ap_t3087 = (_tr_str_lit_len("read write open close lseek unlink stat fstat mkdir rmdir access dup dup2 pipe fork execve waitpid kill getpid ", 111LL)); _tr_str_append(&names, _ap_t3087); });
#line 150 "src/bindgen.tr"
    ({ TrStr _ap_t3088 = (_tr_str_lit_len("CreateWindow CloseWindow CreateWindowExA CreateWindowExW GetMessage DispatchMessage MessageBox MessageBoxA MessageBoxW ", 119LL)); _tr_str_append(&names, _ap_t3088); });
#line 153 "src/bindgen.tr"
    ({ TrStr _ap_t3089 = (_tr_str_lit_len("Rectangle Ellipse Polygon Polyline Arc Chord Pie RoundRect LineTo MoveToEx FillRect FrameRect InvertRect DrawIcon DrawText DrawTextEx TextOut GetObject LoadImage CreateFont PlaySound Polygon PolyBezier ", 202LL)); _tr_str_append(&names, _ap_t3089); });
#line 154 "src/bindgen.tr"
    TrMap* m = _tr_dict_new(512LL);
#line 155 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((names), (_tr_str_lit_len(" ", 1LL)));
#line 156 "src/bindgen.tr"
    long long i = 0LL;
#line 157 "src/bindgen.tr"
    while ((i < parts->len)) {
#line 158 "src/bindgen.tr"
        if (({ TrStr _cmpa_t3090 = (List_TrStr_get(parts, i)); TrStr _cmpb_t3091 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3092 = !_tr_str_eqv(_cmpa_t3090, _cmpb_t3091); _tr_str_release(_cmpa_t3090); _cmpr_t3092; })) {
#line 158 "src/bindgen.tr"
            ({ TrStr _dkt_t3093 = (List_TrStr_get(parts, i)); _tr_dict_set(m, _tr_strz(_dkt_t3093), true); _tr_str_release(_dkt_t3093); });
        }
#line 159 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 160 "src/bindgen.tr"
    _tr_str_release(names);
    List_TrStr_free(parts);
    return m;
}

__attribute__((hot)) bool _is_prim_type_word(TrStr w) {
#line 165 "src/bindgen.tr"
    return ((((((((((_tr_str_eqv((w), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((w), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("short", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("long", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("double", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("signed", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("unsigned", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("_Bool", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("bool", 4LL))));
}

__attribute__((hot)) bool _is_ignored_word(TrStr w) {
#line 170 "src/bindgen.tr"
    return ((((((((((((((((((((_tr_str_eqv((w), (_tr_str_lit_len("const", 5LL))) || _tr_str_eqv((w), (_tr_str_lit_len("volatile", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("extern", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("static", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("inline", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("register", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("auto", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__inline", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__inline__", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__forceinline", 13LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("restrict", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__restrict", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__restrict__", 12LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__cdecl", 7LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__stdcall", 9LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__fastcall", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__thiscall", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("WINAPI", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("APIENTRY", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__extension__", 13LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("_Noreturn", 9LL))));
}

__attribute__((hot)) bool _is_decl_term(long long kind, TrStr text) {
#line 174 "src/bindgen.tr"
    if ((kind == 5LL)) {
#line 174 "src/bindgen.tr"
        return true;
    }
#line 175 "src/bindgen.tr"
    if ((kind != 4LL)) {
#line 175 "src/bindgen.tr"
        return false;
    }
#line 176 "src/bindgen.tr"
    return ((((((_tr_str_eqv((text), (_tr_str_lit_len(",", 1LL))) || _tr_str_eqv((text), (_tr_str_lit_len(";", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len(")", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("[", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("=", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("(", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("}", 1LL))));
}

__attribute__((hot)) TrStr _join_words(List_TrStr* words) {
#line 179 "src/bindgen.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 180 "src/bindgen.tr"
    long long i = 0LL;
#line 181 "src/bindgen.tr"
    while ((i < words->len)) {
#line 182 "src/bindgen.tr"
        if ((i > 0LL)) {
#line 182 "src/bindgen.tr"
            TrStr _strtmp_t3095 = _tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t3095;
        }
#line 183 "src/bindgen.tr"
        TrStr _strtmp_t3097 = ({ TrStr _tr_cr = (List_TrStr_get(words, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t3097;
#line 184 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 185 "src/bindgen.tr"
    TrStr _trret613 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret613;
}

__attribute__((hot)) long long _to_int(TrStr s) {
#line 188 "src/bindgen.tr"
    long long r = 0LL;
#line 189 "src/bindgen.tr"
    long long i = 0LL;
#line 190 "src/bindgen.tr"
    bool neg = false;
#line 191 "src/bindgen.tr"
    if (((_tr_str_lenv((s)) > 0LL) && (_tr_strz(s)[0LL] == 45LL))) {
#line 192 "src/bindgen.tr"
        neg = true;
#line 193 "src/bindgen.tr"
        i = 1LL;
    }
#line 197 "src/bindgen.tr"
    if (((((i + 1LL) < _tr_str_lenv((s))) && (_tr_strz(s)[i] == 48LL)) && ((_tr_strz(s)[(i + 1LL)] == 120LL) || (_tr_strz(s)[(i + 1LL)] == 88LL)))) {
#line 198 "src/bindgen.tr"
        i = (i + 2LL);
#line 199 "src/bindgen.tr"
        long long hd = 0LL;
#line 200 "src/bindgen.tr"
        while (((i < _tr_str_lenv((s))) && (hd < 15LL))) {
#line 201 "src/bindgen.tr"
            char hc = _tr_strz(s)[i];
#line 202 "src/bindgen.tr"
            if (((hc >= 48LL) && (hc <= 57LL))) {
#line 202 "src/bindgen.tr"
                r = ((r * 16LL) + (hc - 48LL));
            } else if (((hc >= 97LL) && (hc <= 102LL))) {
#line 203 "src/bindgen.tr"
                r = ((r * 16LL) + (hc - 87LL));
            } else if (((hc >= 65LL) && (hc <= 70LL))) {
#line 204 "src/bindgen.tr"
                r = ((r * 16LL) + (hc - 55LL));
            } else {
#line 205 "src/bindgen.tr"
                break;
            }
#line 206 "src/bindgen.tr"
            i = (i + 1LL);
#line 207 "src/bindgen.tr"
            hd = (hd + 1LL);
        }
#line 208 "src/bindgen.tr"
        if (neg) {
#line 208 "src/bindgen.tr"
            return (0LL - r);
        }
#line 209 "src/bindgen.tr"
        return r;
    }
#line 210 "src/bindgen.tr"
    long long dd = 0LL;
#line 211 "src/bindgen.tr"
    while (((i < _tr_str_lenv((s))) && (dd < 18LL))) {
#line 212 "src/bindgen.tr"
        char c = _tr_strz(s)[i];
#line 213 "src/bindgen.tr"
        if (((c >= 48LL) && (c <= 57LL))) {
#line 214 "src/bindgen.tr"
            r = ((r * 10LL) + (c - 48LL));
        } else {
#line 216 "src/bindgen.tr"
            break;
        }
#line 217 "src/bindgen.tr"
        i = (i + 1LL);
#line 218 "src/bindgen.tr"
        dd = (dd + 1LL);
    }
#line 219 "src/bindgen.tr"
    if (neg) {
#line 219 "src/bindgen.tr"
        return (0LL - r);
    }
#line 220 "src/bindgen.tr"
    return r;
}

__attribute__((hot)) long long _skip_gnu_attrs(List_ptr* toks, long long pos) {
#line 233 "src/bindgen.tr"
    long long i = pos;
#line 234 "src/bindgen.tr"
    while ((i < toks->len)) {
#line 235 "src/bindgen.tr"
        CTok* t = ((CTok*)List_ptr_get(toks, i));
#line 236 "src/bindgen.tr"
        if ((t->kind != 0LL)) {
#line 236 "src/bindgen.tr"
            break;
        }
#line 237 "src/bindgen.tr"
        if (((!_tr_str_eqv((t->text), (_tr_str_lit_len("__attribute__", 13LL)))) && (!_tr_str_eqv((t->text), (_tr_str_lit_len("__declspec", 10LL)))))) {
#line 237 "src/bindgen.tr"
            break;
        }
#line 238 "src/bindgen.tr"
        i = (i + 1LL);
#line 239 "src/bindgen.tr"
        if ((i >= toks->len)) {
#line 239 "src/bindgen.tr"
            break;
        }
#line 240 "src/bindgen.tr"
        if ((!((((CTok*)List_ptr_get(toks, i))->kind == 4LL) && _tr_str_eqv((((CTok*)List_ptr_get(toks, i))->text), (_tr_str_lit_len("(", 1LL)))))) {
#line 240 "src/bindgen.tr"
            break;
        }
#line 241 "src/bindgen.tr"
        long long depth = 0LL;
#line 242 "src/bindgen.tr"
        while ((i < toks->len)) {
#line 243 "src/bindgen.tr"
            CTok* pt = ((CTok*)List_ptr_get(toks, i));
#line 244 "src/bindgen.tr"
            if (((pt->kind == 4LL) && _tr_str_eqv((pt->text), (_tr_str_lit_len("(", 1LL))))) {
#line 244 "src/bindgen.tr"
                depth = (depth + 1LL);
            } else if (((pt->kind == 4LL) && _tr_str_eqv((pt->text), (_tr_str_lit_len(")", 1LL))))) {
#line 246 "src/bindgen.tr"
                depth = (depth - 1LL);
#line 247 "src/bindgen.tr"
                if ((depth == 0LL)) {
#line 248 "src/bindgen.tr"
                    i = (i + 1LL);
#line 249 "src/bindgen.tr"
                    break;
                }
            }
#line 250 "src/bindgen.tr"
            i = (i + 1LL);
        }
    }
#line 251 "src/bindgen.tr"
    return i;
}

__attribute__((hot)) TrStr _scan_typedef_name(List_ptr* toks, long long pos) {
#line 254 "src/bindgen.tr"
    long long i = pos;
#line 255 "src/bindgen.tr"
    long long depth = 0LL;
#line 256 "src/bindgen.tr"
    while ((i < toks->len)) {
#line 257 "src/bindgen.tr"
        CTok* t = ((CTok*)List_ptr_get(toks, i));
#line 258 "src/bindgen.tr"
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("{", 1LL))))) {
#line 258 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("}", 1LL))))) {
#line 260 "src/bindgen.tr"
            depth = (depth - 1LL);
#line 261 "src/bindgen.tr"
            if ((depth == 0LL)) {
#line 262 "src/bindgen.tr"
                long long ni = _skip_gnu_attrs(toks, (i + 1LL));
#line 263 "src/bindgen.tr"
                if (((ni < toks->len) && (((CTok*)List_ptr_get(toks, ni))->kind == 0LL))) {
#line 264 "src/bindgen.tr"
                    return _tr_str_retain(((CTok*)List_ptr_get(toks, ni))->text);
                }
#line 265 "src/bindgen.tr"
                return _tr_str_lit_len("", 0LL);
            }
        }
#line 266 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 267 "src/bindgen.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _typedef_is_fnptr(List_ptr* toks, long long pos) {
#line 270 "src/bindgen.tr"
    long long i = pos;
#line 271 "src/bindgen.tr"
    while (((i + 1LL) < toks->len)) {
#line 272 "src/bindgen.tr"
        CTok* t = ((CTok*)List_ptr_get(toks, i));
#line 273 "src/bindgen.tr"
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len(";", 1LL))))) {
#line 273 "src/bindgen.tr"
            return false;
        }
#line 274 "src/bindgen.tr"
        if (((((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("(", 1LL)))) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && _tr_str_eqv((((CTok*)List_ptr_get(toks, (i + 1LL)))->text), (_tr_str_lit_len("*", 1LL))))) {
#line 275 "src/bindgen.tr"
            return true;
        }
#line 276 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 277 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) TrStr _scan_fnptr_name(List_ptr* toks, long long pos) {
#line 280 "src/bindgen.tr"
    long long i = pos;
#line 281 "src/bindgen.tr"
    while (((i + 2LL) < toks->len)) {
#line 282 "src/bindgen.tr"
        CTok* t = ((CTok*)List_ptr_get(toks, i));
#line 283 "src/bindgen.tr"
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len(";", 1LL))))) {
#line 283 "src/bindgen.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 284 "src/bindgen.tr"
        if (((((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("(", 1LL)))) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && _tr_str_eqv((((CTok*)List_ptr_get(toks, (i + 1LL)))->text), (_tr_str_lit_len("*", 1LL))))) {
#line 285 "src/bindgen.tr"
            if ((((CTok*)List_ptr_get(toks, (i + 2LL)))->kind == 0LL)) {
#line 285 "src/bindgen.tr"
                return _tr_str_retain(((CTok*)List_ptr_get(toks, (i + 2LL)))->text);
            }
#line 286 "src/bindgen.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 287 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 288 "src/bindgen.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _is_ident_byte(long long c) {
#line 876 "src/bindgen.tr"
    return (((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL));
}

__attribute__((hot)) TrStr _rename_word(TrStr text, TrStr old, TrStr new_) {
#line 882 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((text)) + 16LL));
#line 883 "src/bindgen.tr"
    long long n = _tr_str_lenv((text));
#line 884 "src/bindgen.tr"
    long long ol = _tr_str_lenv((old));
#line 885 "src/bindgen.tr"
    long long i = 0LL;
#line 886 "src/bindgen.tr"
    while ((i < n)) {
#line 887 "src/bindgen.tr"
        bool matched = false;
#line 888 "src/bindgen.tr"
        if ((((i + ol) <= n) && ({ TrStr _cmpa_t3098 = (_tr_str_slicev((text), i, (i + ol))); TrStr _cmpb_t3099 = (old); _Bool _cmpr_t3100 = _tr_str_eqv(_cmpa_t3098, _cmpb_t3099); _tr_str_release(_cmpa_t3098); _cmpr_t3100; }))) {
#line 889 "src/bindgen.tr"
            bool before_ok = ((i == 0LL) || (!_is_ident_byte(_tr_strz(text)[(i - 1LL)])));
#line 890 "src/bindgen.tr"
            bool after_ok = (((i + ol) >= n) || (!_is_ident_byte(_tr_strz(text)[(i + ol)])));
#line 891 "src/bindgen.tr"
            if ((before_ok && after_ok)) {
#line 892 "src/bindgen.tr"
                StringBuilder_append(sb, new_);
#line 893 "src/bindgen.tr"
                i = (i + ol);
#line 894 "src/bindgen.tr"
                matched = true;
            }
        }
#line 895 "src/bindgen.tr"
        if ((!matched)) {
#line 896 "src/bindgen.tr"
            ({ TrStr _sbt_t3101 = (_tr_str_slicev((text), i, (i + 1LL))); StringBuilder_append(sb, _sbt_t3101); _tr_str_release(_sbt_t3101); });
#line 897 "src/bindgen.tr"
            i = (i + 1LL);
        }
    }
#line 898 "src/bindgen.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _basename(TrStr p) {
#line 902 "src/bindgen.tr"
    long long last = 0LL;
#line 903 "src/bindgen.tr"
    long long i = 0LL;
#line 904 "src/bindgen.tr"
    while ((i < _tr_str_lenv((p)))) {
#line 905 "src/bindgen.tr"
        char c = _tr_strz(p)[i];
#line 906 "src/bindgen.tr"
        if (((c == 47LL) || (c == 92LL))) {
#line 906 "src/bindgen.tr"
            last = (i + 1LL);
        }
#line 907 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 908 "src/bindgen.tr"
    return _tr_str_slicev((p), last, _tr_str_lenv((p)));
}

__attribute__((hot)) TrStr _marker_file(TrStr line) {
#line 912 "src/bindgen.tr"
    if ((((_tr_str_lenv((line)) < 3LL) || (_tr_strz(line)[0LL] != 35LL)) || (_tr_strz(line)[1LL] != 32LL))) {
#line 912 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 913 "src/bindgen.tr"
    long long q1 = _tr_str_index_ofv((line), (_tr_str_lit_len("\"", 1LL)));
#line 914 "src/bindgen.tr"
    if ((q1 < 0LL)) {
#line 914 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 915 "src/bindgen.tr"
    TrStr rest = _tr_str_slicev((line), (q1 + 1LL), _tr_str_lenv((line)));
#line 916 "src/bindgen.tr"
    long long q2 = _tr_str_index_ofv((rest), (_tr_str_lit_len("\"", 1LL)));
#line 917 "src/bindgen.tr"
    if ((q2 < 0LL)) {
#line 917 "src/bindgen.tr"
        _tr_str_release(rest);
        return _tr_str_lit_len("", 0LL);
    }
#line 918 "src/bindgen.tr"
    TrStr _trret614 = _tr_str_slicev((rest), 0LL, q2);
    _tr_str_release(rest);
    return _trret614;
}

__attribute__((hot)) bool _marker_is_system(TrStr line) {
#line 924 "src/bindgen.tr"
    long long q1 = _tr_str_index_ofv((line), (_tr_str_lit_len("\"", 1LL)));
#line 925 "src/bindgen.tr"
    if ((q1 < 0LL)) {
#line 925 "src/bindgen.tr"
        return false;
    }
#line 926 "src/bindgen.tr"
    TrStr rest = _tr_str_slicev((line), (q1 + 1LL), _tr_str_lenv((line)));
#line 927 "src/bindgen.tr"
    long long q2 = _tr_str_index_ofv((rest), (_tr_str_lit_len("\"", 1LL)));
#line 928 "src/bindgen.tr"
    if ((q2 < 0LL)) {
#line 928 "src/bindgen.tr"
        _tr_str_release(rest);
        return false;
    }
#line 931 "src/bindgen.tr"
    bool _trret615 = (_tr_str_index_ofv((_tr_str_slicev((rest), (q2 + 1LL), _tr_str_lenv((rest)))), (_tr_str_lit_len(" 3", 2LL))) >= 0LL);
    _tr_str_release(rest);
    return _trret615;
}

__attribute__((hot)) TrStr _filter_to_target(TrStr raw, TrStr target) {
#line 937 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init(_tr_str_lenv((raw)));
#line 938 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((raw), (_tr_str_lit_len("\n", 1LL)));
#line 939 "src/bindgen.tr"
    bool cur_ok = false;
#line 940 "src/bindgen.tr"
    long long i = 0LL;
#line 941 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 942 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 943 "src/bindgen.tr"
        i = (i + 1LL);
#line 944 "src/bindgen.tr"
        if (({ TrStr _cmpa_t3102 = (_marker_file(ln)); TrStr _cmpb_t3103 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3104 = !_tr_str_eqv(_cmpa_t3102, _cmpb_t3103); _tr_str_release(_cmpa_t3102); _cmpr_t3104; })) {
#line 945 "src/bindgen.tr"
            cur_ok = (!_marker_is_system(ln));
#line 946 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 947 "src/bindgen.tr"
        if (cur_ok) {
#line 948 "src/bindgen.tr"
            StringBuilder_append(sb, ln);
#line 949 "src/bindgen.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
        }
        _tr_str_release(ln);
    }
#line 950 "src/bindgen.tr"
    TrStr _trret616 = StringObj_as_str(StringBuilder_to_string(sb));
    List_TrStr_free(lines);
    StringBuilder__tr_fn_free(sb);
    return _trret616;
}

__attribute__((hot)) TrStr _macro_name(TrStr rest) {
#line 954 "src/bindgen.tr"
    long long i = 0LL;
#line 955 "src/bindgen.tr"
    while ((i < _tr_str_lenv((rest)))) {
#line 956 "src/bindgen.tr"
        char c = _tr_strz(rest)[i];
#line 957 "src/bindgen.tr"
        if (((c == 32LL) || (c == 40LL))) {
#line 957 "src/bindgen.tr"
            break;
        }
#line 958 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 959 "src/bindgen.tr"
    return _tr_str_slicev((rest), 0LL, i);
}

__attribute__((hot)) TrMap* _load_baseline(TrStr cc) {
#line 964 "src/bindgen.tr"
    TrMap* m = _tr_dict_new(512LL);
#line 965 "src/bindgen.tr"
    write_file(_tr_str_lit_len("_bindgen_empty.h", 16LL), _tr_str_lit_len("", 0LL));
#line 966 "src/bindgen.tr"
    ({ TrStr _aet_t3105 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" -E -dM \"_bindgen_empty.h\" > \"_bindgen_base.i\" 2>_bindgen_err.txt", 65LL)))); _tr_system(_aet_t3105.data); _tr_str_release(_aet_t3105); });
#line 967 "src/bindgen.tr"
    if ((!file_exists(_tr_str_lit_len("_bindgen_base.i", 15LL)))) {
#line 967 "src/bindgen.tr"
        return m;
    }
#line 968 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((read_file(_tr_str_lit_len("_bindgen_base.i", 15LL))), (_tr_str_lit_len("\n", 1LL)));
#line 969 "src/bindgen.tr"
    long long i = 0LL;
#line 970 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 971 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 972 "src/bindgen.tr"
        i = (i + 1LL);
#line 973 "src/bindgen.tr"
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
#line 973 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 974 "src/bindgen.tr"
        TrStr nm = ({ TrStr _at_t3106 = (_tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)))); __auto_type _wr = (_macro_name(_at_t3106)); _tr_str_release(_at_t3106); _wr; });
#line 975 "src/bindgen.tr"
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
#line 975 "src/bindgen.tr"
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
#line 976 "src/bindgen.tr"
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrMap* _target_define_names(TrStr header) {
#line 981 "src/bindgen.tr"
    TrMap* m = _tr_dict_new(128LL);
#line 982 "src/bindgen.tr"
    if ((!file_exists(header))) {
#line 982 "src/bindgen.tr"
        return m;
    }
#line 983 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((read_file(header)), (_tr_str_lit_len("\n", 1LL)));
#line 984 "src/bindgen.tr"
    long long i = 0LL;
#line 985 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 986 "src/bindgen.tr"
        TrStr ln = ({ TrStr _at_t3107 = (List_TrStr_get(lines, i)); __auto_type _wr = (_lstrip(_at_t3107)); _tr_str_release(_at_t3107); _wr; });
#line 987 "src/bindgen.tr"
        i = (i + 1LL);
#line 988 "src/bindgen.tr"
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
#line 988 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 989 "src/bindgen.tr"
        TrStr nm = ({ TrStr _at_t3108 = (_tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)))); __auto_type _wr = (_macro_name(_at_t3108)); _tr_str_release(_at_t3108); _wr; });
#line 990 "src/bindgen.tr"
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
#line 990 "src/bindgen.tr"
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
#line 991 "src/bindgen.tr"
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _lstrip(TrStr s) {
#line 994 "src/bindgen.tr"
    long long i = 0LL;
#line 995 "src/bindgen.tr"
    while (((i < _tr_str_lenv((s))) && ((_tr_strz(s)[i] == 32LL) || (_tr_strz(s)[i] == 9LL)))) {
#line 995 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 996 "src/bindgen.tr"
    return _tr_str_slicev((s), i, _tr_str_lenv((s)));
}

__attribute__((hot)) bool _is_builtin_ty_name(TrStr n) {
#line 1007 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))))) {
#line 1007 "src/bindgen.tr"
        return true;
    }
#line 1008 "src/bindgen.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 1008 "src/bindgen.tr"
        return true;
    }
#line 1009 "src/bindgen.tr"
    return _tr_str_starts_withv((n), (_tr_str_lit_len("c_", 2LL)));
}

__attribute__((hot)) bool _is_libc_ty_name(TrStr n) {
#line 1018 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("FILE", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("fpos_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("va_list", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("jmp_buf", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sigjmp_buf", 10LL))))) {
#line 1018 "src/bindgen.tr"
        return true;
    }
#line 1019 "src/bindgen.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("time_t", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("clock_t", 7LL))))) {
#line 1019 "src/bindgen.tr"
        return true;
    }
#line 1020 "src/bindgen.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("wchar_t", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("wint_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mbstate_t", 9LL))))) {
#line 1020 "src/bindgen.tr"
        return true;
    }
#line 1021 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("off_t", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("off64_t", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mode_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pid_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("uid_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("gid_t", 5LL))))) {
#line 1021 "src/bindgen.tr"
        return true;
    }
#line 1022 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("dev_t", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ino_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("nlink_t", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("blkcnt_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("blksize_t", 9LL))))) {
#line 1022 "src/bindgen.tr"
        return true;
    }
#line 1023 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("fd_set", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("sigset_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("DIR", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pthread_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sig_atomic_t", 12LL))))) {
#line 1023 "src/bindgen.tr"
        return true;
    }
#line 1024 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) bool _is_system_record_ty(TrStr n) {
#line 1031 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("_GUID", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("GUID", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IID", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("CLSID", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FMTID", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("UUID", 4LL))))) {
#line 1031 "src/bindgen.tr"
        return true;
    }
#line 1032 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("IUnknown", 8LL))) || _tr_str_eqv((n), (_tr_str_lit_len("IDispatch", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IErrorInfo", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ITypeInfo", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ITypeLib", 8LL))))) {
#line 1032 "src/bindgen.tr"
        return true;
    }
#line 1033 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("IStream", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ISequentialStream", 17LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IStorage", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IRecordInfo", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IServiceProvider", 16LL))))) {
#line 1033 "src/bindgen.tr"
        return true;
    }
#line 1034 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("VARIANT", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("VARIANTARG", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("DECIMAL", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SAFEARRAY", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SAFEARRAYBOUND", 14LL))))) {
#line 1034 "src/bindgen.tr"
        return true;
    }
#line 1035 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("DISPPARAMS", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("EXCEPINFO", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("CY", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("BSTRBLOB", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("BLOB", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FLAGGED_WORD_BLOB", 17LL))))) {
#line 1035 "src/bindgen.tr"
        return true;
    }
#line 1036 "src/bindgen.tr"
    if (((((((_tr_str_eqv((n), (_tr_str_lit_len("POINT", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("POINTL", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SIZE", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SIZEL", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RECT", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RECTL", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FILETIME", 8LL))))) {
#line 1036 "src/bindgen.tr"
        return true;
    }
#line 1037 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("LARGE_INTEGER", 13LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ULARGE_INTEGER", 14LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("LUID", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("MSG", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("POINTS", 6LL))))) {
#line 1037 "src/bindgen.tr"
        return true;
    }
#line 1038 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) TrStr _ident_at(TrStr text, long long start) {
#line 1042 "src/bindgen.tr"
    long long i = start;
#line 1043 "src/bindgen.tr"
    while (((i < _tr_str_lenv((text))) && (((_tr_strz(text)[i] == 32LL) || (_tr_strz(text)[i] == 9LL)) || (_tr_strz(text)[i] == 91LL)))) {
#line 1043 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1044 "src/bindgen.tr"
    if ((i >= _tr_str_lenv((text)))) {
#line 1044 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 1045 "src/bindgen.tr"
    char c = _tr_strz(text)[i];
#line 1046 "src/bindgen.tr"
    if ((!((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL)))) {
#line 1046 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 1047 "src/bindgen.tr"
    long long b = i;
#line 1048 "src/bindgen.tr"
    while ((i < _tr_str_lenv((text)))) {
#line 1049 "src/bindgen.tr"
        c = _tr_strz(text)[i];
#line 1050 "src/bindgen.tr"
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
#line 1050 "src/bindgen.tr"
            i = (i + 1LL);
        } else {
#line 1051 "src/bindgen.tr"
            break;
        }
    }
#line 1052 "src/bindgen.tr"
    return _tr_str_slicev((text), b, i);
}

__attribute__((hot)) TrStr _opaque_fallbacks(TrStr body, TrMap* defined) {
#line 1057 "src/bindgen.tr"
    TrMap* want = _tr_dict_new(64LL);
#line 1058 "src/bindgen.tr"
    List_TrStr* order = (void*)List_TrStr_new();
#line 1059 "src/bindgen.tr"
    long long i = 0LL;
#line 1060 "src/bindgen.tr"
    long long n = _tr_str_lenv((body));
#line 1061 "src/bindgen.tr"
    while ((i < n)) {
#line 1062 "src/bindgen.tr"
        char c = _tr_strz(body)[i];
#line 1063 "src/bindgen.tr"
        TrStr nm = _tr_str_lit_len("", 0LL);
#line 1064 "src/bindgen.tr"
        if ((c == 58LL)) {
#line 1065 "src/bindgen.tr"
            TrStr _strtmp_t3109 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t3109;
#line 1065 "src/bindgen.tr"
            i = (i + 1LL);
        } else if ((((c == 45LL) && ((i + 1LL) < n)) && (_tr_strz(body)[(i + 1LL)] == 62LL))) {
#line 1067 "src/bindgen.tr"
            TrStr _strtmp_t3110 = _ident_at(body, (i + 2LL));
            _tr_str_release(nm);
            nm = _strtmp_t3110;
#line 1067 "src/bindgen.tr"
            i = (i + 2LL);
        } else if ((c == 91LL)) {
#line 1069 "src/bindgen.tr"
            TrStr _strtmp_t3111 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t3111;
#line 1069 "src/bindgen.tr"
            i = (i + 1LL);
        } else {
#line 1071 "src/bindgen.tr"
            i = (i + 1LL);
        }
#line 1072 "src/bindgen.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL)))) {
#line 1072 "src/bindgen.tr"
            _tr_str_release(nm);
            continue;
        }
#line 1073 "src/bindgen.tr"
        if (_is_builtin_ty_name(nm)) {
#line 1073 "src/bindgen.tr"
            _tr_str_release(nm);
            continue;
        }
#line 1074 "src/bindgen.tr"
        if (_is_libc_ty_name(nm)) {
#line 1074 "src/bindgen.tr"
            _tr_str_release(nm);
            continue;
        }
#line 1075 "src/bindgen.tr"
        if (_tr_dict_contains(defined, _tr_strz(nm))) {
#line 1075 "src/bindgen.tr"
            _tr_str_release(nm);
            continue;
        }
#line 1076 "src/bindgen.tr"
        if (_tr_dict_contains(want, _tr_strz(nm))) {
#line 1076 "src/bindgen.tr"
            _tr_str_release(nm);
            continue;
        }
#line 1077 "src/bindgen.tr"
        _tr_dict_set(want, _tr_strz(nm), true);
#line 1078 "src/bindgen.tr"
        List_TrStr_append(order, nm);
        _tr_str_release(nm);
    }
#line 1079 "src/bindgen.tr"
    if ((order->len == 0LL)) {
#line 1079 "src/bindgen.tr"
        Dict_free(want);
        List_TrStr_free(order);
        return _tr_str_lit_len("", 0LL);
    }
#line 1080 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init(256LL);
#line 1081 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("# Opaque fallbacks: types referenced by this header but defined in a system or\n", 79LL));
#line 1082 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("# sub-header (bound as opaque handles; pass by Pointer). Verify by-value layout.\n", 81LL));
#line 1083 "src/bindgen.tr"
    long long k = 0LL;
#line 1084 "src/bindgen.tr"
    while ((k < order->len)) {
#line 1085 "src/bindgen.tr"
        ({ TrStr _sbt_t3112 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(order, k)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = Pointer[void]\n", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3112); _tr_str_release(_sbt_t3112); });
#line 1086 "src/bindgen.tr"
        k = (k + 1LL);
    }
#line 1087 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
#line 1088 "src/bindgen.tr"
    TrStr _trret617 = StringObj_as_str(StringBuilder_to_string(sb));
    Dict_free(want);
    List_TrStr_free(order);
    StringBuilder__tr_fn_free(sb);
    return _trret617;
}

__attribute__((hot)) bool _is_single_string_literal(TrStr val) {
#line 1092 "src/bindgen.tr"
    if (((_tr_str_lenv((val)) < 2LL) || (_tr_strz(val)[0LL] != 34LL))) {
#line 1092 "src/bindgen.tr"
        return false;
    }
#line 1093 "src/bindgen.tr"
    long long i = 1LL;
#line 1094 "src/bindgen.tr"
    bool esc = false;
#line 1095 "src/bindgen.tr"
    while ((i < _tr_str_lenv((val)))) {
#line 1096 "src/bindgen.tr"
        char c = _tr_strz(val)[i];
#line 1097 "src/bindgen.tr"
        if (esc) {
#line 1098 "src/bindgen.tr"
            esc = false;
        } else if ((c == 92LL)) {
#line 1100 "src/bindgen.tr"
            esc = true;
        } else if ((c == 34LL)) {
#line 1103 "src/bindgen.tr"
            long long j = (i + 1LL);
#line 1104 "src/bindgen.tr"
            while ((j < _tr_str_lenv((val)))) {
#line 1105 "src/bindgen.tr"
                char t = _tr_strz(val)[j];
#line 1106 "src/bindgen.tr"
                if (((t != 32LL) && (t != 9LL))) {
#line 1106 "src/bindgen.tr"
                    return false;
                }
#line 1107 "src/bindgen.tr"
                j = (j + 1LL);
            }
#line 1108 "src/bindgen.tr"
            return true;
        }
#line 1109 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1110 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) bool _is_single_int_literal(TrStr val) {
#line 1114 "src/bindgen.tr"
    long long n = _tr_str_lenv((val));
#line 1116 "src/bindgen.tr"
    while (((n > 0LL) && ((_tr_strz(val)[(n - 1LL)] == 32LL) || (_tr_strz(val)[(n - 1LL)] == 9LL)))) {
#line 1116 "src/bindgen.tr"
        n = (n - 1LL);
    }
#line 1117 "src/bindgen.tr"
    if ((n == 0LL)) {
#line 1117 "src/bindgen.tr"
        return false;
    }
#line 1118 "src/bindgen.tr"
    long long i = 0LL;
#line 1119 "src/bindgen.tr"
    if (((_tr_strz(val)[0LL] == 45LL) || (_tr_strz(val)[0LL] == 43LL))) {
#line 1119 "src/bindgen.tr"
        i = 1LL;
    }
#line 1120 "src/bindgen.tr"
    if ((i >= n)) {
#line 1120 "src/bindgen.tr"
        return false;
    }
#line 1121 "src/bindgen.tr"
    bool hex = false;
#line 1122 "src/bindgen.tr"
    if (((((i + 1LL) < n) && (_tr_strz(val)[i] == 48LL)) && ((_tr_strz(val)[(i + 1LL)] == 120LL) || (_tr_strz(val)[(i + 1LL)] == 88LL)))) {
#line 1123 "src/bindgen.tr"
        hex = true;
#line 1124 "src/bindgen.tr"
        i = (i + 2LL);
#line 1125 "src/bindgen.tr"
        if ((i >= n)) {
#line 1125 "src/bindgen.tr"
            return false;
        }
    }
#line 1126 "src/bindgen.tr"
    while ((i < n)) {
#line 1127 "src/bindgen.tr"
        char c = _tr_strz(val)[i];
#line 1128 "src/bindgen.tr"
        bool ok = false;
#line 1129 "src/bindgen.tr"
        if (((c >= 48LL) && (c <= 57LL))) {
#line 1129 "src/bindgen.tr"
            ok = true;
        } else if ((hex && (((c >= 97LL) && (c <= 102LL)) || ((c >= 65LL) && (c <= 70LL))))) {
#line 1130 "src/bindgen.tr"
            ok = true;
        }
#line 1131 "src/bindgen.tr"
        if ((!ok)) {
#line 1131 "src/bindgen.tr"
            return false;
        }
#line 1132 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1133 "src/bindgen.tr"
    return true;
}

__attribute__((hot)) void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow) {
#line 1136 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((defs), (_tr_str_lit_len("\n", 1LL)));
#line 1137 "src/bindgen.tr"
    long long i = 0LL;
#line 1138 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 1139 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 1140 "src/bindgen.tr"
        i = (i + 1LL);
#line 1141 "src/bindgen.tr"
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
#line 1141 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1142 "src/bindgen.tr"
        TrStr rest = _tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)));
#line 1143 "src/bindgen.tr"
        long long sp = _tr_str_index_ofv((rest), (_tr_str_lit_len(" ", 1LL)));
#line 1144 "src/bindgen.tr"
        if ((sp < 0LL)) {
#line 1144 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            continue;
        }
#line 1145 "src/bindgen.tr"
        TrStr nm = _tr_str_slicev((rest), 0LL, sp);
#line 1146 "src/bindgen.tr"
        if ((_tr_str_index_ofv((nm), (_tr_str_lit_len("(", 1LL))) >= 0LL)) {
#line 1146 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1147 "src/bindgen.tr"
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
#line 1147 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1148 "src/bindgen.tr"
        if ((!_tr_dict_contains(allow, _tr_strz(nm)))) {
#line 1148 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1149 "src/bindgen.tr"
        TrStr val = ({ TrStr _at_t3113 = (_tr_str_slicev((rest), (sp + 1LL), _tr_str_lenv((rest)))); __auto_type _wr = (_lstrip(_at_t3113)); _tr_str_release(_at_t3113); _wr; });
#line 1150 "src/bindgen.tr"
        if ((_tr_str_lenv((val)) == 0LL)) {
#line 1150 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            _tr_str_release(val);
            continue;
        }
#line 1151 "src/bindgen.tr"
        char c0 = _tr_strz(val)[0LL];
#line 1152 "src/bindgen.tr"
        if ((c0 == 34LL)) {
#line 1156 "src/bindgen.tr"
            if (_is_single_string_literal(val)) {
#line 1157 "src/bindgen.tr"
                ({ TrStr _sbt_t3114 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(bg->consts, _sbt_t3114); _tr_str_release(_sbt_t3114); });
            }
        } else if ((((c0 >= 48LL) && (c0 <= 57LL)) || (c0 == 45LL))) {
#line 1160 "src/bindgen.tr"
            if (_is_single_int_literal(val)) {
#line 1161 "src/bindgen.tr"
                long long iv = _to_int(val);
#line 1162 "src/bindgen.tr"
                ({ TrStr _ord_t3115 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": c_int = ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(iv)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(bg->consts, _ord_t3115); _tr_str_release(_ord_t3115); });
            }
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(val);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) TrStr _cxx_of(TrStr cc) {
#line 1174 "src/bindgen.tr"
    if (_tr_str_eqv((cc), (_tr_str_lit_len("gcc", 3LL)))) {
#line 1174 "src/bindgen.tr"
        return _tr_str_lit_len("g++", 3LL);
    }
#line 1175 "src/bindgen.tr"
    if (_tr_str_eqv((cc), (_tr_str_lit_len("clang", 5LL)))) {
#line 1175 "src/bindgen.tr"
        return _tr_str_lit_len("clang++", 7LL);
    }
#line 1176 "src/bindgen.tr"
    if ((_tr_str_index_ofv((cc), (_tr_str_lit_len("++", 2LL))) >= 0LL)) {
#line 1176 "src/bindgen.tr"
        return _tr_str_retain(cc);
    }
#line 1177 "src/bindgen.tr"
    return _tr_str_lit_len("c++", 3LL);
}

__attribute__((hot)) TrStr _macro_args(long long n, long long mode, long long atype, long long tp) {
#line 1184 "src/bindgen.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 1185 "src/bindgen.tr"
    bool first = true;
#line 1186 "src/bindgen.tr"
    long long i = 0LL;
#line 1187 "src/bindgen.tr"
    while ((i < n)) {
#line 1188 "src/bindgen.tr"
        if ((i == tp)) {
#line 1191 "src/bindgen.tr"
            if ((mode == 1LL)) {
#line 1192 "src/bindgen.tr"
                if ((!first)) {
#line 1192 "src/bindgen.tr"
                    TrStr _strtmp_t3117 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(s);
                    s = _strtmp_t3117;
                }
#line 1193 "src/bindgen.tr"
                TrStr _strtmp_t3119 = _tr_strx_concatv((s), (_tr_str_lit_len("char", 4LL)));
                _tr_str_release(s);
                s = _strtmp_t3119;
#line 1194 "src/bindgen.tr"
                first = false;
            }
#line 1195 "src/bindgen.tr"
            i = (i + 1LL);
#line 1196 "src/bindgen.tr"
            continue;
        }
#line 1197 "src/bindgen.tr"
        if ((!first)) {
#line 1197 "src/bindgen.tr"
            TrStr _strtmp_t3121 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t3121;
        }
#line 1198 "src/bindgen.tr"
        first = false;
#line 1199 "src/bindgen.tr"
        if ((mode == 0LL)) {
#line 1200 "src/bindgen.tr"
            if ((atype == 0LL)) {
#line 1200 "src/bindgen.tr"
                TrStr _strtmp_t3124 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("void* a", 7LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t3124;
            } else {
#line 1201 "src/bindgen.tr"
                TrStr _strtmp_t3127 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("intptr_t a", 10LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t3127;
            }
        } else if ((mode == 1LL)) {
#line 1203 "src/bindgen.tr"
            TrStr _strtmp_t3130 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t3130;
        } else {
#line 1205 "src/bindgen.tr"
            if ((atype == 0LL)) {
#line 1205 "src/bindgen.tr"
                TrStr _strtmp_t3134 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Pointer[void]", 15LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t3134;
            } else {
#line 1206 "src/bindgen.tr"
                TrStr _strtmp_t3138 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": c_intptr_t", 12LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t3138;
            }
        }
#line 1207 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1208 "src/bindgen.tr"
    TrStr _trret618 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret618;
}

__attribute__((hot)) TrStr _ent_name(TrStr e) {
#line 1213 "src/bindgen.tr"
    return _tr_str_slicev((e), 0LL, _tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))));
}

__attribute__((hot)) long long _ent_arity(TrStr e) {
#line 1215 "src/bindgen.tr"
    TrStr r = ({ __auto_type _ord_t3139 = ((_tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))) + 1LL)); __auto_type _ord_t3140 = (_tr_str_lenv((e))); __auto_type _wr = (_tr_str_slicev((e), _ord_t3139, _ord_t3140)); _wr; });
#line 1216 "src/bindgen.tr"
    long long b = _tr_str_index_ofv((r), (_tr_str_lit_len("|", 1LL)));
#line 1217 "src/bindgen.tr"
    if ((b < 0LL)) {
#line 1217 "src/bindgen.tr"
        long long _trret619 = _to_int(r);
        _tr_str_release(r);
        return _trret619;
    }
#line 1218 "src/bindgen.tr"
    long long _trret620 = ({ TrStr _at_t3141 = (_tr_str_slicev((r), 0LL, b)); __auto_type _wr = (_to_int(_at_t3141)); _tr_str_release(_at_t3141); _wr; });
    _tr_str_release(r);
    return _trret620;
}

__attribute__((hot)) long long _ent_typepos(TrStr e) {
#line 1220 "src/bindgen.tr"
    TrStr r = ({ __auto_type _ord_t3142 = ((_tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))) + 1LL)); __auto_type _ord_t3143 = (_tr_str_lenv((e))); __auto_type _wr = (_tr_str_slicev((e), _ord_t3142, _ord_t3143)); _wr; });
#line 1221 "src/bindgen.tr"
    long long b = _tr_str_index_ofv((r), (_tr_str_lit_len("|", 1LL)));
#line 1222 "src/bindgen.tr"
    if ((b < 0LL)) {
#line 1222 "src/bindgen.tr"
        _tr_str_release(r);
        return (-1LL);
    }
#line 1223 "src/bindgen.tr"
    long long _trret621 = ({ TrStr _at_t3144 = (_tr_str_slicev((r), (b + 1LL), _tr_str_lenv((r)))); __auto_type _wr = (_to_int(_at_t3144)); _tr_str_release(_at_t3144); _wr; });
    _tr_str_release(r);
    return _trret621;
}

__attribute__((hot)) bool _is_ident_ch(long long c) {
#line 1227 "src/bindgen.tr"
    return (((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _param_ptr_type(TrStr body, TrStr pname) {
#line 1232 "src/bindgen.tr"
    long long plen = _tr_str_lenv((pname));
#line 1233 "src/bindgen.tr"
    if ((plen == 0LL)) {
#line 1233 "src/bindgen.tr"
        return false;
    }
#line 1234 "src/bindgen.tr"
    long long i = 0LL;
#line 1235 "src/bindgen.tr"
    while (((i + plen) <= _tr_str_lenv((body)))) {
#line 1236 "src/bindgen.tr"
        if (({ TrStr _cmpa_t3145 = (_tr_str_slicev((body), i, (i + plen))); TrStr _cmpb_t3146 = (pname); _Bool _cmpr_t3147 = _tr_str_eqv(_cmpa_t3145, _cmpb_t3146); _tr_str_release(_cmpa_t3145); _cmpr_t3147; })) {
#line 1237 "src/bindgen.tr"
            bool okb = true;
#line 1238 "src/bindgen.tr"
            if (((i > 0LL) && _is_ident_ch(_tr_strz(body)[(i - 1LL)]))) {
#line 1238 "src/bindgen.tr"
                okb = false;
            }
#line 1239 "src/bindgen.tr"
            long long j = (i + plen);
#line 1240 "src/bindgen.tr"
            bool oka = true;
#line 1241 "src/bindgen.tr"
            if (((j < _tr_str_lenv((body))) && _is_ident_ch(_tr_strz(body)[j]))) {
#line 1241 "src/bindgen.tr"
                oka = false;
            }
#line 1242 "src/bindgen.tr"
            if ((okb && oka)) {
#line 1243 "src/bindgen.tr"
                while (((j < _tr_str_lenv((body))) && ((_tr_strz(body)[j] == 32LL) || (_tr_strz(body)[j] == 9LL)))) {
#line 1243 "src/bindgen.tr"
                    j = (j + 1LL);
                }
#line 1244 "src/bindgen.tr"
                if (((j < _tr_str_lenv((body))) && (_tr_strz(body)[j] == 42LL))) {
#line 1244 "src/bindgen.tr"
                    return true;
                }
            }
        }
#line 1245 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1246 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) bool _balanced_delims(TrStr s) {
#line 1252 "src/bindgen.tr"
    long long depth = 0LL;
#line 1253 "src/bindgen.tr"
    long long i = 0LL;
#line 1254 "src/bindgen.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 1255 "src/bindgen.tr"
        char c = _tr_strz(s)[i];
#line 1256 "src/bindgen.tr"
        if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
#line 1256 "src/bindgen.tr"
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
#line 1258 "src/bindgen.tr"
            depth = (depth - 1LL);
#line 1259 "src/bindgen.tr"
            if ((depth < 0LL)) {
#line 1259 "src/bindgen.tr"
                return false;
            }
        }
#line 1260 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 1261 "src/bindgen.tr"
    return (depth == 0LL);
}

__attribute__((hot)) List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline) {
#line 1265 "src/bindgen.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1266 "src/bindgen.tr"
    TrMap* seen = _tr_dict_new(128LL);
#line 1267 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((defs), (_tr_str_lit_len("\n", 1LL)));
#line 1268 "src/bindgen.tr"
    long long i = 0LL;
#line 1269 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 1270 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 1271 "src/bindgen.tr"
        i = (i + 1LL);
#line 1272 "src/bindgen.tr"
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
#line 1272 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1273 "src/bindgen.tr"
        TrStr rest = _tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)));
#line 1274 "src/bindgen.tr"
        long long par = _tr_str_index_ofv((rest), (_tr_str_lit_len("(", 1LL)));
#line 1275 "src/bindgen.tr"
        if ((par <= 0LL)) {
#line 1275 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            continue;
        }
#line 1276 "src/bindgen.tr"
        long long sp = _tr_str_index_ofv((rest), (_tr_str_lit_len(" ", 1LL)));
#line 1277 "src/bindgen.tr"
        if (((sp >= 0LL) && (sp < par))) {
#line 1277 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            continue;
        }
#line 1278 "src/bindgen.tr"
        TrStr nm = _tr_str_slicev((rest), 0LL, par);
#line 1279 "src/bindgen.tr"
        if (_tr_str_starts_withv((nm), (_tr_str_lit_len("__", 2LL)))) {
#line 1279 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1280 "src/bindgen.tr"
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
#line 1280 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1281 "src/bindgen.tr"
        if (_tr_dict_contains(seen, _tr_strz(nm))) {
#line 1281 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1282 "src/bindgen.tr"
        long long _tr_v_close = _tr_str_index_ofv((rest), (_tr_str_lit_len(")", 1LL)));
#line 1283 "src/bindgen.tr"
        if (((_tr_v_close < 0LL) || (_tr_v_close < par))) {
#line 1283 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            continue;
        }
#line 1284 "src/bindgen.tr"
        TrStr plist = _tr_str_slicev((rest), (par + 1LL), _tr_v_close);
#line 1285 "src/bindgen.tr"
        if ((_tr_str_index_ofv((plist), (_tr_str_lit_len("...", 3LL))) >= 0LL)) {
#line 1285 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            _tr_str_release(plist);
            continue;
        }
#line 1286 "src/bindgen.tr"
        TrStr body = ({ TrStr _at_t3148 = (_tr_str_slicev((rest), (_tr_v_close + 1LL), _tr_str_lenv((rest)))); __auto_type _wr = (_lstrip(_at_t3148)); _tr_str_release(_at_t3148); _wr; });
#line 1287 "src/bindgen.tr"
        if (_tr_str_eqv((body), (_tr_str_lit_len("", 0LL)))) {
#line 1287 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            _tr_str_release(plist);
            _tr_str_release(body);
            continue;
        }
#line 1288 "src/bindgen.tr"
        if ((!_balanced_delims(body))) {
#line 1288 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            _tr_str_release(plist);
            _tr_str_release(body);
            continue;
        }
#line 1290 "src/bindgen.tr"
        long long arity = 0LL;
#line 1291 "src/bindgen.tr"
        TrStr pt = _tr_str_stripv((plist));
#line 1292 "src/bindgen.tr"
        if ((!_tr_str_eqv((pt), (_tr_str_lit_len("", 0LL))))) {
#line 1293 "src/bindgen.tr"
            arity = 1LL;
#line 1294 "src/bindgen.tr"
            long long ci = 0LL;
#line 1295 "src/bindgen.tr"
            while ((ci < _tr_str_lenv((pt)))) {
#line 1296 "src/bindgen.tr"
                if ((_tr_strz(pt)[ci] == 44LL)) {
#line 1296 "src/bindgen.tr"
                    arity = (arity + 1LL);
                }
#line 1297 "src/bindgen.tr"
                ci = (ci + 1LL);
            }
        }
#line 1298 "src/bindgen.tr"
        if ((arity > 8LL)) {
#line 1298 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(rest);
            _tr_str_release(nm);
            _tr_str_release(plist);
            _tr_str_release(body);
            _tr_str_release(pt);
            continue;
        }
#line 1302 "src/bindgen.tr"
        long long typepos = (-1LL);
#line 1303 "src/bindgen.tr"
        if (((((arity > 0LL) && (_tr_str_index_ofv((body), (_tr_str_lit_len("sizeof", 6LL))) < 0LL)) && (_tr_str_index_ofv((body), (_tr_str_lit_len("offsetof", 8LL))) < 0LL)) && (_tr_str_index_ofv((body), (_tr_str_lit_len("##", 2LL))) < 0LL))) {
#line 1304 "src/bindgen.tr"
            List_TrStr* parts = _tr_str_splitv((plist), (_tr_str_lit_len(",", 1LL)));
#line 1305 "src/bindgen.tr"
            long long pi = 0LL;
#line 1306 "src/bindgen.tr"
            long long nfound = 0LL;
#line 1307 "src/bindgen.tr"
            while ((pi < parts->len)) {
#line 1308 "src/bindgen.tr"
                TrStr pnm = _tr_str_stripv((List_TrStr_get(parts, pi)));
#line 1309 "src/bindgen.tr"
                if (_param_ptr_type(body, pnm)) {
#line 1310 "src/bindgen.tr"
                    typepos = pi;
#line 1311 "src/bindgen.tr"
                    nfound = (nfound + 1LL);
                }
#line 1312 "src/bindgen.tr"
                pi = (pi + 1LL);
                _tr_str_release(pnm);
            }
#line 1313 "src/bindgen.tr"
            if ((nfound != 1LL)) {
#line 1313 "src/bindgen.tr"
                typepos = (-1LL);
            }
        }
#line 1314 "src/bindgen.tr"
        _tr_dict_set(seen, _tr_strz(nm), true);
#line 1315 "src/bindgen.tr"
        ({ __auto_type _ord_t3149 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((nm), (_tr_str_lit_len("|", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(arity)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3150 = (({ TrStr _tr_cl = (_ord_t3149); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(typepos)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(out, _at_t3150); _tr_str_release(_at_t3150); });
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(plist);
        _tr_str_release(body);
        _tr_str_release(pt);
    }
#line 1316 "src/bindgen.tr"
    Dict_free(seen);
    List_TrStr_free(lines);
    return out;
}

__attribute__((hot)) bool _has_cc_error(TrStr errtxt) {
#line 1320 "src/bindgen.tr"
    return ((_tr_str_index_ofv((errtxt), (_tr_str_lit_len(": error:", 8LL))) >= 0LL) || (_tr_str_index_ofv((errtxt), (_tr_str_lit_len(": error :", 9LL))) >= 0LL));
}

__attribute__((hot)) TrMap* _macro_bad_names(TrStr errtxt) {
#line 1327 "src/bindgen.tr"
    TrMap* m = _tr_dict_new(64LL);
#line 1328 "src/bindgen.tr"
    TrStr key = _tr_str_lit_len("in expansion of macro ", 22LL);
#line 1329 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((errtxt), (_tr_str_lit_len("\n", 1LL)));
#line 1330 "src/bindgen.tr"
    long long i = 0LL;
#line 1331 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 1332 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 1333 "src/bindgen.tr"
        i = (i + 1LL);
#line 1334 "src/bindgen.tr"
        long long fp = _tr_str_index_ofv((ln), (key));
#line 1335 "src/bindgen.tr"
        if ((fp < 0LL)) {
#line 1335 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1336 "src/bindgen.tr"
        TrStr after = ({ __auto_type _ord_t3151 = ((fp + _tr_str_lenv((key)))); __auto_type _ord_t3152 = (_tr_str_lenv((ln))); __auto_type _wr = (_tr_str_slicev((ln), _ord_t3151, _ord_t3152)); _wr; });
#line 1337 "src/bindgen.tr"
        long long q1 = _tr_str_index_ofv((after), (_tr_str_lit_len("'", 1LL)));
#line 1338 "src/bindgen.tr"
        if ((q1 < 0LL)) {
#line 1338 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(after);
            continue;
        }
#line 1339 "src/bindgen.tr"
        TrStr rest2 = _tr_str_slicev((after), (q1 + 1LL), _tr_str_lenv((after)));
#line 1340 "src/bindgen.tr"
        long long q2 = _tr_str_index_ofv((rest2), (_tr_str_lit_len("'", 1LL)));
#line 1341 "src/bindgen.tr"
        if ((q2 < 0LL)) {
#line 1341 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(after);
            _tr_str_release(rest2);
            continue;
        }
#line 1342 "src/bindgen.tr"
        TrStr nm = _tr_str_slicev((rest2), 0LL, q2);
#line 1343 "src/bindgen.tr"
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
#line 1343 "src/bindgen.tr"
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(after);
        _tr_str_release(rest2);
        _tr_str_release(nm);
    }
#line 1344 "src/bindgen.tr"
    _tr_str_release(key);
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp) {
#line 1352 "src/bindgen.tr"
    if ((form == 0LL)) {
#line 1353 "src/bindgen.tr"
        return ({ __auto_type _ord_t3153 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void* ", 6LL)), (sym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("){ return (void*)(intptr_t)(", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3153); TrStr _tr_cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")); }\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
    }
#line 1357 "src/bindgen.tr"
    return ({ __auto_type _ord_t3154 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), (sym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("){ ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3154); TrStr _tr_cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake) {
#line 1362 "src/bindgen.tr"
    StringBuilder* probe = StringBuilder_init(8192LL);
#line 1363 "src/bindgen.tr"
    ({ TrStr _sbt_t3155 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(probe, _sbt_t3155); _tr_str_release(_sbt_t3155); });
#line 1364 "src/bindgen.tr"
    StringBuilder_append(probe, _tr_str_lit_len("#include <stdint.h>\n", 20LL));
#line 1365 "src/bindgen.tr"
    List_TrStr* order = (void*)List_TrStr_new();
#line 1366 "src/bindgen.tr"
    long long k = 0LL;
#line 1367 "src/bindgen.tr"
    while ((k < macros->len)) {
#line 1368 "src/bindgen.tr"
        TrStr ent = List_TrStr_get(macros, k);
#line 1369 "src/bindgen.tr"
        k = (k + 1LL);
#line 1370 "src/bindgen.tr"
        TrStr nm = _ent_name(ent);
#line 1371 "src/bindgen.tr"
        if ((!_tr_dict_contains(cand, _tr_strz(nm)))) {
#line 1371 "src/bindgen.tr"
            _tr_str_release(ent);
            _tr_str_release(nm);
            continue;
        }
#line 1372 "src/bindgen.tr"
        long long tp = (0LL - 1LL);
#line 1373 "src/bindgen.tr"
        if (bake) {
#line 1374 "src/bindgen.tr"
            tp = _ent_typepos(ent);
#line 1375 "src/bindgen.tr"
            if ((tp < 0LL)) {
#line 1375 "src/bindgen.tr"
                _tr_str_release(ent);
                _tr_str_release(nm);
                continue;
            }
        }
#line 1376 "src/bindgen.tr"
        ({ TrStr _at_t3156 = (_tr_strx_concatv((_tr_str_lit_len("mP_", 3LL)), (nm))); TrStr _sbt_t3157 = (_macro_shim_line(_at_t3156, nm, _ent_arity(ent), form, atype, tp)); StringBuilder_append(probe, _sbt_t3157); _tr_str_release(_at_t3156); _tr_str_release(_sbt_t3157); });
#line 1377 "src/bindgen.tr"
        List_TrStr_append(order, nm);
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
#line 1378 "src/bindgen.tr"
    ({ TrStr _at_t3158 = (StringObj_as_str(StringBuilder_to_string(probe))); write_file(_tr_str_lit_len("_macroprobe.c", 13LL), _at_t3158); _tr_str_release(_at_t3158); });
#line 1379 "src/bindgen.tr"
    StringBuilder__tr_fn_free(probe);
    return order;
}

__attribute__((hot)) TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
#line 1388 "src/bindgen.tr"
    List_TrStr* order = _macro_probe_write(header, macros, cand, form, atype, bake);
#line 1389 "src/bindgen.tr"
    ({ TrStr _aet_t3159 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cxx), (_tr_str_lit_len(" -std=c++17 -fsyntax-only -fmax-errors=0 -fpermissive -w ", 57LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (extra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" _macroprobe.c > _macroprobe.err 2>&1", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3159.data); _tr_str_release(_aet_t3159); });
#line 1390 "src/bindgen.tr"
    TrStr errtxt = _tr_str_lit_len("", 0LL);
#line 1391 "src/bindgen.tr"
    if (file_exists(_tr_str_lit_len("_macroprobe.err", 15LL))) {
#line 1391 "src/bindgen.tr"
        TrStr _strtmp_t3160 = read_file(_tr_str_lit_len("_macroprobe.err", 15LL));
        _tr_str_release(errtxt);
        errtxt = _strtmp_t3160;
    }
#line 1392 "src/bindgen.tr"
    TrMap* bad = _macro_bad_names(errtxt);
#line 1393 "src/bindgen.tr"
    TrStr key = _tr_str_lit_len("_macroprobe.c:", 14LL);
#line 1394 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((errtxt), (_tr_str_lit_len("\n", 1LL)));
#line 1395 "src/bindgen.tr"
    long long i = 0LL;
#line 1396 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 1397 "src/bindgen.tr"
        TrStr ln = List_TrStr_get(lines, i);
#line 1398 "src/bindgen.tr"
        i = (i + 1LL);
#line 1399 "src/bindgen.tr"
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len(": error:", 8LL))) < 0LL)) {
#line 1399 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1400 "src/bindgen.tr"
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("is not allowed here", 19LL))) >= 0LL)) {
#line 1400 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1401 "src/bindgen.tr"
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("expected unqualified-id", 23LL))) >= 0LL)) {
#line 1401 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1402 "src/bindgen.tr"
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("expected declaration", 20LL))) >= 0LL)) {
#line 1402 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1403 "src/bindgen.tr"
        long long fp = _tr_str_index_ofv((ln), (key));
#line 1404 "src/bindgen.tr"
        if ((fp < 0LL)) {
#line 1404 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 1405 "src/bindgen.tr"
        TrStr after = ({ __auto_type _ord_t3161 = ((fp + _tr_str_lenv((key)))); __auto_type _ord_t3162 = (_tr_str_lenv((ln))); __auto_type _wr = (_tr_str_slicev((ln), _ord_t3161, _ord_t3162)); _wr; });
#line 1406 "src/bindgen.tr"
        long long colon = _tr_str_index_ofv((after), (_tr_str_lit_len(":", 1LL)));
#line 1407 "src/bindgen.tr"
        if ((colon < 0LL)) {
#line 1407 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(after);
            continue;
        }
#line 1408 "src/bindgen.tr"
        long long idx = ({ TrStr _at_t3163 = (_tr_str_slicev((after), 0LL, colon)); __auto_type _wr = ((_to_int(_at_t3163) - 3LL)); _tr_str_release(_at_t3163); _wr; });
#line 1409 "src/bindgen.tr"
        if (((idx >= 0LL) && (idx < order->len))) {
#line 1409 "src/bindgen.tr"
            ({ TrStr _dkt_t3164 = (List_TrStr_get(order, idx)); _tr_dict_set(bad, _tr_strz(_dkt_t3164), true); _tr_str_release(_dkt_t3164); });
        }
        _tr_str_release(ln);
        _tr_str_release(after);
    }
#line 1410 "src/bindgen.tr"
    List_TrStr_free(order);
    _tr_str_release(errtxt);
    _tr_str_release(key);
    List_TrStr_free(lines);
    return bad;
}

__attribute__((hot)) TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
#line 1414 "src/bindgen.tr"
    TrMap* cur = cand;
#line 1415 "src/bindgen.tr"
    long long iter = 0LL;
#line 1416 "src/bindgen.tr"
    while ((iter < 12LL)) {
#line 1417 "src/bindgen.tr"
        iter = (iter + 1LL);
#line 1418 "src/bindgen.tr"
        TrMap* bad = _macro_form_bad(header, macros, cur, form, atype, bake, cxx, extra);
#line 1419 "src/bindgen.tr"
        if ((bad->len == 0LL)) {
#line 1419 "src/bindgen.tr"
            break;
        }
#line 1420 "src/bindgen.tr"
        TrMap* next = _tr_dict_new(64LL);
#line 1421 "src/bindgen.tr"
        long long k = 0LL;
#line 1422 "src/bindgen.tr"
        while ((k < macros->len)) {
#line 1423 "src/bindgen.tr"
            TrStr ent = List_TrStr_get(macros, k);
#line 1424 "src/bindgen.tr"
            k = (k + 1LL);
#line 1425 "src/bindgen.tr"
            TrStr nm = _ent_name(ent);
#line 1426 "src/bindgen.tr"
            if ((_tr_dict_contains(cur, _tr_strz(nm)) && (!_tr_dict_contains(bad, _tr_strz(nm))))) {
#line 1426 "src/bindgen.tr"
                _tr_dict_set(next, _tr_strz(nm), true);
            }
            _tr_str_release(ent);
            _tr_str_release(nm);
        }
#line 1427 "src/bindgen.tr"
        if ((next->len == cur->len)) {
#line 1427 "src/bindgen.tr"
            break;
        }
#line 1428 "src/bindgen.tr"
        cur = next;
        Dict_free(bad);
    }
#line 1429 "src/bindgen.tr"
    return cur;
}

__attribute__((hot)) TrMap* _macro_remaining(List_TrStr* macros, TrMap* done) {
#line 1433 "src/bindgen.tr"
    TrMap* m = _tr_dict_new(64LL);
#line 1434 "src/bindgen.tr"
    long long k = 0LL;
#line 1435 "src/bindgen.tr"
    while ((k < macros->len)) {
#line 1436 "src/bindgen.tr"
        TrStr ent = List_TrStr_get(macros, k);
#line 1437 "src/bindgen.tr"
        k = (k + 1LL);
#line 1438 "src/bindgen.tr"
        TrStr nm = _tr_str_slicev((ent), 0LL, _tr_str_index_ofv((ent), (_tr_str_lit_len("|", 1LL))));
#line 1439 "src/bindgen.tr"
        if ((!_tr_dict_contains(done, _tr_strz(nm)))) {
#line 1439 "src/bindgen.tr"
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
#line 1440 "src/bindgen.tr"
    return m;
}

__attribute__((hot)) void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros) {
#line 1444 "src/bindgen.tr"
    long long k = 0LL;
#line 1445 "src/bindgen.tr"
    while ((k < macros->len)) {
#line 1446 "src/bindgen.tr"
        TrStr ent = List_TrStr_get(macros, k);
#line 1447 "src/bindgen.tr"
        k = (k + 1LL);
#line 1448 "src/bindgen.tr"
        TrStr nm = _tr_str_slicev((ent), 0LL, _tr_str_index_ofv((ent), (_tr_str_lit_len("|", 1LL))));
#line 1449 "src/bindgen.tr"
        if (_tr_dict_contains(ok, _tr_strz(nm))) {
#line 1449 "src/bindgen.tr"
            _tr_dict_set(done, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
#line 1454 "src/bindgen.tr"
    TrMap* cand = _macro_remaining(macros, done);
#line 1455 "src/bindgen.tr"
    if (bake) {
#line 1458 "src/bindgen.tr"
        TrMap* c2 = _tr_dict_new(64LL);
#line 1459 "src/bindgen.tr"
        long long ci = 0LL;
#line 1460 "src/bindgen.tr"
        while ((ci < macros->len)) {
#line 1461 "src/bindgen.tr"
            TrStr e2 = List_TrStr_get(macros, ci);
#line 1462 "src/bindgen.tr"
            ci = (ci + 1LL);
#line 1463 "src/bindgen.tr"
            TrStr n2 = _ent_name(e2);
#line 1464 "src/bindgen.tr"
            if ((_tr_dict_contains(cand, _tr_strz(n2)) && (_ent_typepos(e2) >= 0LL))) {
#line 1464 "src/bindgen.tr"
                _tr_dict_set(c2, _tr_strz(n2), true);
            }
            _tr_str_release(e2);
            _tr_str_release(n2);
        }
#line 1465 "src/bindgen.tr"
        cand = c2;
    }
#line 1466 "src/bindgen.tr"
    TrMap* ok = _macro_verify_form(header, macros, cand, form, atype, bake, cxx, extra);
#line 1467 "src/bindgen.tr"
    TrStr bk = _tr_str_lit_len("0", 1LL);
#line 1468 "src/bindgen.tr"
    if (bake) {
#line 1468 "src/bindgen.tr"
        TrStr _strtmp_t3165 = _tr_str_lit_len("1", 1LL);
        _tr_str_release(bk);
        bk = _strtmp_t3165;
    }
#line 1469 "src/bindgen.tr"
    long long k = 0LL;
#line 1470 "src/bindgen.tr"
    while ((k < macros->len)) {
#line 1471 "src/bindgen.tr"
        TrStr ent = List_TrStr_get(macros, k);
#line 1472 "src/bindgen.tr"
        k = (k + 1LL);
#line 1473 "src/bindgen.tr"
        TrStr nm = _ent_name(ent);
#line 1474 "src/bindgen.tr"
        if ((_tr_dict_contains(ok, _tr_strz(nm)) && (!_tr_dict_contains(done, _tr_strz(nm))))) {
#line 1475 "src/bindgen.tr"
            _tr_dict_set(done, _tr_strz(nm), true);
#line 1476 "src/bindgen.tr"
            ({ __auto_type _ord_t3166 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(form)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _dvt_t3167 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3166); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(atype)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bk)); _tr_str_release(_tr_cl); _tr_cres; })); _tr_dict_set(plan, _tr_strz(nm), _tr_str_box(_tr_str_retain(_dvt_t3167))); _tr_str_release(_dvt_t3167); });
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    Dict_free(ok);
    _tr_str_release(bk);
}

__attribute__((hot)) TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra) {
#line 1489 "src/bindgen.tr"
    if ((macros->len == 0LL)) {
#line 1489 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 1490 "src/bindgen.tr"
    long long n_total = macros->len;
#line 1491 "src/bindgen.tr"
    TrMap* done = _tr_dict_new(64LL);
#line 1492 "src/bindgen.tr"
    TrMap* plan = _tr_dict_new(64LL);
#line 1493 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 0LL, 0LL, false, cxx, extra);
#line 1494 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 1LL, 0LL, false, cxx, extra);
#line 1495 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 0LL, 1LL, false, cxx, extra);
#line 1496 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 1LL, 1LL, false, cxx, extra);
#line 1497 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 0LL, 0LL, true, cxx, extra);
#line 1498 "src/bindgen.tr"
    _macro_pass(header, macros, done, plan, 1LL, 0LL, true, cxx, extra);
#line 1499 "src/bindgen.tr"
    _tr_system(_tr_strz(_tr_str_lit_len("rm -f _macroprobe.c _macroprobe.err 2>/dev/null", 47LL)));
#line 1501 "src/bindgen.tr"
    StringBuilder* shim = StringBuilder_init(8192LL);
#line 1502 "src/bindgen.tr"
    ({ TrStr _sbt_t3168 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("// Auto-generated macro shims for ", 34LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (tauraro-bindgen --macros).\n", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3168); _tr_str_release(_sbt_t3168); });
#line 1503 "src/bindgen.tr"
    ({ TrStr _sbt_t3169 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3169); _tr_str_release(_sbt_t3169); });
#line 1504 "src/bindgen.tr"
    StringBuilder_append(shim, _tr_str_lit_len("#include <stdint.h>\n", 20LL));
#line 1505 "src/bindgen.tr"
    StringBuilder_append(shim, _tr_str_lit_len("extern \"C\" {\n", 13LL));
#line 1506 "src/bindgen.tr"
    StringBuilder* binds = StringBuilder_init(4096LL);
#line 1507 "src/bindgen.tr"
    long long kept = 0LL;
#line 1508 "src/bindgen.tr"
    long long n_ip = 0LL;
#line 1509 "src/bindgen.tr"
    long long n_te = 0LL;
#line 1510 "src/bindgen.tr"
    long long k2 = 0LL;
#line 1511 "src/bindgen.tr"
    while ((k2 < macros->len)) {
#line 1512 "src/bindgen.tr"
        TrStr ent = List_TrStr_get(macros, k2);
#line 1513 "src/bindgen.tr"
        k2 = (k2 + 1LL);
#line 1514 "src/bindgen.tr"
        TrStr nm = _ent_name(ent);
#line 1515 "src/bindgen.tr"
        if ((!_tr_dict_contains(plan, _tr_strz(nm)))) {
#line 1515 "src/bindgen.tr"
            _tr_str_release(ent);
            _tr_str_release(nm);
            continue;
        }
#line 1516 "src/bindgen.tr"
        long long ar = _ent_arity(ent);
#line 1517 "src/bindgen.tr"
        List_TrStr* spec = _tr_str_splitv((_tr_str_retain(_tr_str_unbox(_tr_dict_get(plan, _tr_strz(nm))))), (_tr_str_lit_len("|", 1LL)));
#line 1518 "src/bindgen.tr"
        long long form = ({ TrStr _at_t3170 = (List_TrStr_get(spec, 0LL)); __auto_type _wr = (_to_int(_at_t3170)); _tr_str_release(_at_t3170); _wr; });
#line 1519 "src/bindgen.tr"
        long long atype = ({ TrStr _at_t3171 = (List_TrStr_get(spec, 1LL)); __auto_type _wr = (_to_int(_at_t3171)); _tr_str_release(_at_t3171); _wr; });
#line 1520 "src/bindgen.tr"
        long long tp = (0LL - 1LL);
#line 1521 "src/bindgen.tr"
        if (({ TrStr _cmpa_t3172 = (List_TrStr_get(spec, 2LL)); TrStr _cmpb_t3173 = (_tr_str_lit_len("1", 1LL)); _Bool _cmpr_t3174 = _tr_str_eqv(_cmpa_t3172, _cmpb_t3173); _tr_str_release(_cmpa_t3172); _cmpr_t3174; })) {
#line 1521 "src/bindgen.tr"
            tp = _ent_typepos(ent);
        }
#line 1522 "src/bindgen.tr"
        ({ TrStr _at_t3175 = (_tr_strx_concatv((_tr_str_lit_len("m_", 2LL)), (nm))); TrStr _sbt_t3176 = (_macro_shim_line(_at_t3175, nm, ar, form, atype, tp)); StringBuilder_append(shim, _sbt_t3176); _tr_str_release(_at_t3175); _tr_str_release(_sbt_t3176); });
#line 1523 "src/bindgen.tr"
        TrStr bl = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def m_", 10LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_macro_args(ar, 2LL, atype, tp)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 1524 "src/bindgen.tr"
        if ((form == 0LL)) {
#line 1524 "src/bindgen.tr"
            ({ TrStr _ap_t3177 = (_tr_str_lit_len(" -> Pointer[void]", 17LL)); _tr_str_append(&bl, _ap_t3177); });
        }
#line 1525 "src/bindgen.tr"
        ({ TrStr _sbt_t3178 = (_tr_strx_concatv((bl), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(binds, _sbt_t3178); _tr_str_release(_sbt_t3178); });
#line 1526 "src/bindgen.tr"
        kept = (kept + 1LL);
#line 1527 "src/bindgen.tr"
        if ((atype == 1LL)) {
#line 1527 "src/bindgen.tr"
            n_ip = (n_ip + 1LL);
        }
#line 1528 "src/bindgen.tr"
        if ((tp >= 0LL)) {
#line 1528 "src/bindgen.tr"
            n_te = (n_te + 1LL);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
        List_TrStr_free(spec);
        _tr_str_release(bl);
    }
#line 1529 "src/bindgen.tr"
    StringBuilder_append(shim, _tr_str_lit_len("}\n", 2LL));
#line 1530 "src/bindgen.tr"
    if ((kept == 0LL)) {
#line 1530 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 1531 "src/bindgen.tr"
    ({ TrStr _at_t3179 = (_tr_strx_concatv((out), (_tr_str_lit_len("_macros.c", 9LL)))); TrStr _at_t3180 = (StringObj_as_str(StringBuilder_to_string(shim))); write_file(_at_t3179, _at_t3180); _tr_str_release(_at_t3179); _tr_str_release(_at_t3180); });
#line 1532 "src/bindgen.tr"
    ({ __auto_type _ord_t3181 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(kept)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: macro shims: ", 22LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3182 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3181); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_total)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" function-like macros bound (", 29LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3183 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3182); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_ip)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" intptr, ", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3183); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_te)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" type-erased) -> ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (out)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_macros.c (m_<NAME>)", 20LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); }); });
#line 1533 "src/bindgen.tr"
    StringBuilder__tr_fn_free(shim);
    return StringObj_as_str(StringBuilder_to_string(binds));
}

__attribute__((hot)) void run_bindgen(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs, bool want_macros) {
#line 1536 "src/bindgen.tr"
    TrStr tmp_decls = _tr_str_lit_len("_bindgen_decls.i", 16LL);
#line 1537 "src/bindgen.tr"
    TrStr tmp_defs = _tr_str_lit_len("_bindgen_defs.i", 15LL);
#line 1540 "src/bindgen.tr"
    ({ TrStr _aet_t3184 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cc), (extra))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" -E \"", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (header)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" > \"", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp_decls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" 2>_bindgen_err.txt", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3184.data); _tr_str_release(_aet_t3184); });
#line 1541 "src/bindgen.tr"
    ({ TrStr _aet_t3185 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cc), (extra))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" -E -dM \"", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (header)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" > \"", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp_defs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" 2>>_bindgen_err.txt", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3185.data); _tr_str_release(_aet_t3185); });
#line 1542 "src/bindgen.tr"
    if ((!file_exists(tmp_decls))) {
#line 1543 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: preprocessing failed (see _bindgen_err.txt)", 52LL))); printf("\n"); });
#line 1544 "src/bindgen.tr"
        _tr_str_release(tmp_decls);
        _tr_str_release(tmp_defs);
        return;
    }
#line 1545 "src/bindgen.tr"
    TrStr src = ({ TrStr _at_t3186 = (read_file(tmp_decls)); __auto_type _wr = (_filter_to_target(_at_t3186, header)); _tr_str_release(_at_t3186); _wr; });
#line 1546 "src/bindgen.tr"
    List_ptr* toks = tokenize_c(src);
#line 1547 "src/bindgen.tr"
    Bindgen* bg = Bindgen_init(toks);
#line 1548 "src/bindgen.tr"
    Bindgen_run(bg);
#line 1549 "src/bindgen.tr"
    TrMap* baseline = _load_baseline(cc);
#line 1550 "src/bindgen.tr"
    ({ TrStr _at_t3187 = (read_file(tmp_defs)); emit_defines(bg, _at_t3187, baseline, _target_define_names(header)); _tr_str_release(_at_t3187); });
#line 1551 "src/bindgen.tr"
    TrStr macro_binds = _tr_str_lit_len("", 0LL);
#line 1552 "src/bindgen.tr"
    TrStr macro_shim = _tr_str_lit_len("", 0LL);
#line 1553 "src/bindgen.tr"
    if (want_macros) {
#line 1554 "src/bindgen.tr"
        TrStr _strtmp_t3191 = ({ TrStr _at_t3188 = (read_file(tmp_defs)); __auto_type _ord_t3190 = (_collect_fn_macros(_at_t3188, baseline)); TrStr _at_t3189 = (_cxx_of(cc)); __auto_type _wr = (_gen_macro_shims(header, out, _ord_t3190, _at_t3189, extra)); _tr_str_release(_at_t3188); _tr_str_release(_at_t3189); _wr; });
        _tr_str_release(macro_binds);
        macro_binds = _strtmp_t3191;
#line 1555 "src/bindgen.tr"
        if ((!_tr_str_eqv((macro_binds), (_tr_str_lit_len("", 0LL))))) {
#line 1555 "src/bindgen.tr"
            TrStr _strtmp_t3192 = ({ TrStr _tr_cl = (_basename(out)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_macros.c", 9LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(macro_shim);
            macro_shim = _strtmp_t3192;
        }
    }
#line 1556 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init(4096LL);
#line 1557 "src/bindgen.tr"
    ({ TrStr _sbt_t3193 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# Auto-generated FFI bindings for ", 34LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (tauraro-bindgen).\n", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3193); _tr_str_release(_sbt_t3193); });
#line 1558 "src/bindgen.tr"
    if ((!_tr_str_eqv((macro_shim), (_tr_str_lit_len("", 0LL))))) {
#line 1559 "src/bindgen.tr"
        ({ TrStr _sbt_t3194 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-shim: ", 20LL)), (macro_shim))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3194); _tr_str_release(_sbt_t3194); });
#line 1561 "src/bindgen.tr"
        ({ TrStr _sbt_t3195 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-cflags: -fpermissive ", 35LL)), (extra))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3195); _tr_str_release(_sbt_t3195); });
    }
#line 1564 "src/bindgen.tr"
    if ((!_tr_str_eqv((pkglibs), (_tr_str_lit_len("", 0LL))))) {
#line 1564 "src/bindgen.tr"
        ({ TrStr _sbt_t3196 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-linkflags:", 24LL)), (pkglibs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3196); _tr_str_release(_sbt_t3196); });
    }
#line 1565 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
#line 1566 "src/bindgen.tr"
    TrStr ts = StringObj_as_str(StringBuilder_to_string(bg->types));
#line 1567 "src/bindgen.tr"
    TrStr ss = StringObj_as_str(StringBuilder_to_string(bg->structs));
#line 1568 "src/bindgen.tr"
    TrStr cs = StringObj_as_str(StringBuilder_to_string(bg->consts));
#line 1569 "src/bindgen.tr"
    TrStr fs = StringObj_as_str(StringBuilder_to_string(bg->funcs));
#line 1570 "src/bindgen.tr"
    if ((!_tr_str_eqv((macro_binds), (_tr_str_lit_len("", 0LL))))) {
#line 1570 "src/bindgen.tr"
        ({ TrStr _ap_t3197 = (macro_binds); _tr_str_append(&fs, _ap_t3197); });
    }
#line 1573 "src/bindgen.tr"
    TrStr fb = ({ TrStr _at_t3198 = (({ TrStr _tr_cl = (_tr_strx_concatv((ts), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fs)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_opaque_fallbacks(_at_t3198, bg->seen)); _tr_str_release(_at_t3198); _wr; });
#line 1574 "src/bindgen.tr"
    if ((!_tr_str_eqv((fb), (_tr_str_lit_len("", 0LL))))) {
#line 1574 "src/bindgen.tr"
        StringBuilder_append(sb, fb);
    }
#line 1575 "src/bindgen.tr"
    if ((!_tr_str_eqv((ts), (_tr_str_lit_len("", 0LL))))) {
#line 1575 "src/bindgen.tr"
        ({ TrStr _sbt_t3199 = (_tr_strx_concatv((ts), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t3199); _tr_str_release(_sbt_t3199); });
    }
#line 1576 "src/bindgen.tr"
    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 1576 "src/bindgen.tr"
        StringBuilder_append(sb, ss);
    }
#line 1577 "src/bindgen.tr"
    if ((!_tr_str_eqv((cs), (_tr_str_lit_len("", 0LL))))) {
#line 1577 "src/bindgen.tr"
        ({ TrStr _sbt_t3200 = (_tr_strx_concatv((cs), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t3200); _tr_str_release(_sbt_t3200); });
    }
#line 1578 "src/bindgen.tr"
    if ((!_tr_str_eqv((fs), (_tr_str_lit_len("", 0LL))))) {
#line 1579 "src/bindgen.tr"
        StringBuilder_append(sb, _tr_str_lit_len("extern \"C\":\n", 12LL));
#line 1580 "src/bindgen.tr"
        StringBuilder_append(sb, fs);
    }
#line 1581 "src/bindgen.tr"
    TrStr result = StringObj_as_str(StringBuilder_to_string(sb));
#line 1586 "src/bindgen.tr"
    long long ri = 0LL;
#line 1587 "src/bindgen.tr"
    long long n_renamed = 0LL;
#line 1588 "src/bindgen.tr"
    while ((ri < bg->defined->len)) {
#line 1589 "src/bindgen.tr"
        TrStr dn = List_TrStr_get(bg->defined, ri);
#line 1590 "src/bindgen.tr"
        ri = (ri + 1LL);
#line 1591 "src/bindgen.tr"
        if (_tr_dict_contains(bg->skip_syms, _tr_strz(dn))) {
#line 1592 "src/bindgen.tr"
            TrStr _strtmp_t3202 = ({ TrStr _at_t3201 = (_tr_strx_concatv((dn), (_tr_str_lit_len("_", 1LL)))); __auto_type _wr = (_rename_word(result, dn, _at_t3201)); _tr_str_release(_at_t3201); _wr; });
            _tr_str_release(result);
            result = _strtmp_t3202;
#line 1593 "src/bindgen.tr"
            n_renamed = (n_renamed + 1LL);
        }
        _tr_str_release(dn);
    }
#line 1594 "src/bindgen.tr"
    write_file(out, result);
#line 1595 "src/bindgen.tr"
    _rm_files(_tr_str_lit_len("_bindgen_decls.i _bindgen_defs.i _bindgen_base.i _bindgen_empty.h _bindgen_err.txt", 82LL));
#line 1596 "src/bindgen.tr"
    TrStr msg = ({ __auto_type _ord_t3203 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("bindgen: wrote ", 15LL)), (out))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" — ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_structs)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" types, ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3203); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_funcs)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" functions", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 1597 "src/bindgen.tr"
    if ((bg->n_skipped > 0LL)) {
#line 1597 "src/bindgen.tr"
        ({ TrStr _ap_t3204 = (_tr_str_lit_len(" (", 2LL)); TrStr _ap_t3205 = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_skipped)))); TrStr _ap_t3206 = (_tr_str_lit_len(" runtime/libc symbols skipped)", 30LL)); _tr_str_append(&msg, _ap_t3204); _tr_str_append(&msg, _ap_t3205); _tr_str_append(&msg, _ap_t3206); _tr_str_release(_ap_t3205); });
    }
#line 1598 "src/bindgen.tr"
    ({ printf("%s", _tr_strz(msg)); printf("\n"); });
    _tr_str_release(tmp_decls);
    _tr_str_release(tmp_defs);
    _tr_str_release(src);
    List_ptr_free_obj(toks, _trdrop_CTok);
    _tr_obj_release(bg, _trdrop_Bindgen);
    _tr_str_release(macro_binds);
    _tr_str_release(macro_shim);
    _tr_str_release(ts);
    _tr_str_release(ss);
    _tr_str_release(cs);
    _tr_str_release(fs);
    _tr_str_release(fb);
    _tr_str_release(result);
    _tr_str_release(msg);
    StringBuilder__tr_fn_free(sb);
}

__attribute__((hot)) TrStr _cxxwalk_src() {
#line 1616 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init(8192LL);
#line 1617 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#include <clang-c/Index.h>\n", 27LL));
#line 1618 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#include <stdio.h>\n", 19LL));
#line 1619 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#include <string.h>\n", 20LL));
#line 1620 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("#include <stdlib.h>\n", 20LL));
#line 1621 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static const char* S(CXString s){ const char* p=clang_getCString(s); return p?p:\"\"; }\n", 86LL));
#line 1622 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static const char* scalar(enum CXTypeKind k){\n", 46LL));
#line 1623 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  switch(k){ case CXType_Bool:return \"bool\";\n", 45LL));
#line 1624 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Char_S:case CXType_Char_U:return \"c_char\"; case CXType_SChar:return \"c_schar\"; case CXType_UChar:return \"c_uchar\";\n", 131LL));
#line 1625 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Char16:return \"c_char16\"; case CXType_Char32:return \"c_char32\"; case CXType_WChar:return \"c_wchar\";\n", 116LL));
#line 1626 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Short:return \"c_short\"; case CXType_UShort:return \"c_ushort\"; case CXType_Int:return \"c_int\"; case CXType_UInt:return \"c_uint\";\n", 144LL));
#line 1627 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Long:return \"c_long\"; case CXType_ULong:return \"c_ulong\"; case CXType_LongLong:return \"c_longlong\"; case CXType_ULongLong:return \"c_ulonglong\";\n", 160LL));
#line 1628 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Float:return \"c_float\"; case CXType_Double:return \"c_double\"; case CXType_LongDouble:return \"c_ldouble\"; default:return \"\"; }\n", 142LL));
#line 1629 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1630 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static char g_params[8][64]; static CXType g_args[8]; static int g_np=0;\n", 73LL));
#line 1631 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static char g_cbase[256]={0};   /* container template base (e.g. \"std::map\") for nested-type detection */\n", 106LL));
#line 1632 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_inst=0;            /* 1 while walking template instantiations (--inst mode) */\n", 92LL));
#line 1633 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_hasbegin=0, g_hasend=0;   /* container exposes begin()/end() -> emit ITER helper */\n", 97LL));
#line 1634 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* Collect NON-system external records used by value, so we can lay them out as @value_type\n", 92LL));
#line 1635 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   (a library value struct in a filtered sub-header would otherwise be an opaque handle). System\n", 97LL));
#line 1636 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   records (windows.h) are left on the bare path — zero risk to the working COM handling. */\n", 95LL));
#line 1637 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static CXCursor g_ext[128]; static char g_extn[128][160]; static int g_next=0;\n", 79LL));
#line 1638 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_ext(CXType rec){\n", 37LL));
#line 1639 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_inst) return;\n", 21LL));
#line 1640 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor decl=clang_getTypeDeclaration(rec);\n", 47LL));
#line 1641 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXSourceLocation loc=clang_getCursorLocation(decl);\n", 54LL));
#line 1642 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Location_isFromMainFile(loc)) return;   /* main-file records already emitted via CLASS */\n", 101LL));
#line 1643 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Location_isInSystemHeader(loc)) return; /* system record -> bare path (unchanged) */\n", 96LL));
#line 1644 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Type_getSizeOf(rec)<=0) return;\n", 43LL));
#line 1645 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXString sp=clang_getTypeSpelling(rec); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 134LL));
#line 1646 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(strstr(r,\"<\")||strstr(r,\"::\")){ clang_disposeString(sp); return; } /* skip templates / nested (kept simple) */\n", 116LL));
#line 1647 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<g_next;i++) if(strcmp(g_extn[i],r)==0){ clang_disposeString(sp); return; }\n", 91LL));
#line 1648 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_next<128){ strncpy(g_extn[g_next],r,159); g_extn[g_next][159]=0; g_ext[g_next]=decl; g_next++; }\n", 104LL));
#line 1649 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeString(sp);\n", 27LL));
#line 1650 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1651 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void field_tautype(CXType t){\n", 37LL));
#line 1652 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType c=clang_getCanonicalType(t);\n", 38LL));
#line 1653 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_ConstantArray){ CXType e=clang_getArrayElementType(c); long long n=clang_getArraySize(c);\n", 110LL));
#line 1654 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"[\"); field_tautype(e); printf(\"; %lld]\", n); return; }\n", 67LL));
#line 1655 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Pointer){ printf(\"Pointer[void]\"); return; }\n", 65LL));
#line 1656 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  const char* sc=scalar(c.kind); if(sc[0]){ printf(\"%s\", sc); return; }\n", 72LL));
#line 1657 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Enum){ printf(\"c_int\"); return; }\n", 54LL));
#line 1658 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Record){ CXString sp=clang_getTypeSpelling(c); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6;\n", 122LL));
#line 1659 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    const char* seg=strrchr(r,':'); printf(\"%s\", seg?seg+1:r); clang_disposeString(sp); return; }\n", 98LL));
#line 1660 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  printf(\"c_int\");\n", 19LL));
#line 1661 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1662 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult sfields(CXCursor c, CXCursor p, CXClientData d){\n", 80LL));
#line 1663 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)!=CX_CXXPrivate && clang_getCXXAccessSpecifier(c)!=CX_CXXProtected){\n", 150LL));
#line 1664 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"SFIELD \"); field_tautype(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 112LL));
#line 1665 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue;\n", 32LL));
#line 1666 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1667 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_extstructs(void){\n", 35LL));
#line 1668 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<g_next;i++){ printf(\"XSTRUCT %s\\n\", g_extn[i]); clang_visitChildren(g_ext[i], sfields, 0); printf(\"EXSTRUCT\\n\"); }\n", 131LL));
#line 1669 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1670 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* whole-identifier-token search: does `needle` appear in `hay` bounded by non-ident chars? */\n", 95LL));
#line 1671 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int has_token(const char* hay, const char* needle){\n", 59LL));
#line 1672 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  int nl=strlen(needle); if(nl==0) return 0; const char* p=hay;\n", 64LL));
#line 1673 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  while((p=strstr(p,needle))){ char b=(p==hay)?0:p[-1]; char a=p[nl];\n", 70LL));
#line 1674 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    int lb=!((b>='A'&&b<='Z')||(b>='a'&&b<='z')||(b>='0'&&b<='9')||b=='_');\n", 76LL));
#line 1675 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    int la=!((a>='A'&&a<='Z')||(a>='a'&&a<='z')||(a>='0'&&a<='9')||a=='_');\n", 76LL));
#line 1676 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(lb&&la) return 1; p+=nl; }\n", 34LL));
#line 1677 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return 0;\n", 12LL));
#line 1678 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1679 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_noncopy;\n", 22LL));
#line 1680 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult copyctorck(CXCursor c, CXCursor p, CXClientData d){\n", 83LL));
#line 1681 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_Constructor && clang_CXXConstructor_isCopyConstructor(c)){\n", 97LL));
#line 1682 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    enum CXAvailabilityKind av=clang_getCursorAvailability(c);\n", 63LL));
#line 1683 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    enum CX_CXXAccessSpecifier acc=clang_getCXXAccessSpecifier(c);\n", 67LL));
#line 1684 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(av==CXAvailability_NotAvailable || acc==CX_CXXPrivate || acc==CX_CXXProtected) g_noncopy=1; }\n", 101LL));
#line 1685 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1686 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int is_noncopyable(CXType t){\n", 37LL));
#line 1687 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor dc=clang_getTypeDeclaration(t); CXCursor def=clang_getCursorDefinition(dc); if(clang_Cursor_isNull(def)) def=dc;\n", 124LL));
#line 1688 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  g_noncopy=0; clang_visitChildren(def, copyctorck, 0);\n", 56LL));
#line 1690 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(!g_noncopy){ CXCursor tm=clang_getSpecializedCursorTemplate(dc); if(!clang_Cursor_isNull(tm)){ CXCursor td=clang_getCursorDefinition(tm); if(clang_Cursor_isNull(td)) td=tm; clang_visitChildren(td, copyctorck, 0); } }\n", 222LL));
#line 1691 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return g_noncopy; }\n", 22LL));
#line 1692 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void classify(int depth,int ref,CXType cur){\n", 52LL));
#line 1695 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  { CXString _ps=clang_getTypeSpelling(cur); int _pk=(strstr(clang_getCString(_ps),\"...\")!=0); clang_disposeString(_ps); if(_pk){ printf(\"%d~%d~d~\", depth, ref); return; } }\n", 174LL));
#line 1696 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXTypeKind k=cur.kind; const char* sc=scalar(k);\n", 56LL));
#line 1697 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXType_Void) printf(\"%d~%d~v~\", depth, ref);\n", 53LL));
#line 1698 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(sc[0]) printf(\"%d~%d~p~%s\", depth, ref, sc);\n", 55LL));
#line 1699 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_Enum){ CXType u=clang_getEnumDeclIntegerType(clang_getTypeDeclaration(cur)); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\";\n", 150LL));
#line 1700 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~e~%s#%s\", depth, ref, S(sp), us); clang_disposeString(sp); }\n", 119LL));
#line 1701 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_Record){\n", 29LL));
#line 1705 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(depth==0 && clang_Type_getSizeOf(cur)==CXTypeLayoutError_Incomplete){ CXCursor _td=clang_getTypeDeclaration(cur); CXCursor _tm=clang_getSpecializedCursorTemplate(_td); if(clang_Cursor_isNull(_tm) || clang_Cursor_isNull(clang_getCursorDefinition(_tm))){ printf(\"%d~%d~d~\", depth, ref); return; } }\n", 304LL));
#line 1706 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(depth==0 && is_noncopyable(cur)){ printf(\"%d~%d~d~\", depth, ref); return; }\n", 83LL));
#line 1707 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(cur); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 136LL));
#line 1708 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(strncmp(r,\"std::function<\",14)==0 && clang_Type_getNumTemplateArguments(cur)>=1){ CXType _ft=clang_Type_getTemplateArgumentAsType(cur,0); CXString _fs=clang_getTypeSpelling(_ft); printf(\"%d~%d~f~%s\", depth, ref, S(_fs)); clang_disposeString(_fs); }\n", 256LL));
#line 1709 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    else if(strncmp(r,\"std::basic_string\",17)==0 && (strstr(r,\"basic_string<char,\")||strstr(r,\"basic_string<char>\")||strstr(r,\"basic_string_view<char,\")||strstr(r,\"basic_string_view<char>\"))){ if(ref==1){ printf(\"%d~%d~d~\", depth, ref); } else { printf(\"%d~%d~s~string\", depth, ref); } }\n", 288LL));
#line 1710 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    else printf(\"%d~%d~r~%s\", depth, ref, r); clang_disposeString(sp); }\n", 73LL));
#line 1711 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_FunctionProto){ CXString sp=clang_getTypeSpelling(clang_getCanonicalType(cur)); printf(\"%d~%d~f~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n", 168LL));
#line 1712 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else { CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~u~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n", 117LL));
#line 1713 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1714 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void classify_arg(int depth,int ref,CXType t){\n", 54LL));
#line 1715 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType c=clang_getCanonicalType(t);\n", 38LL));
#line 1716 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_LValueReference||c.kind==CXType_RValueReference){\n", 70LL));
#line 1717 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXType pt=clang_getPointeeType(c); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; c=clang_getCanonicalType(pt); }\n", 144LL));
#line 1718 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  classify(depth,ref,c); }\n", 27LL));
#line 1719 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void tds(CXType t){\n", 27LL));
#line 1720 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  // ref: 0 = not-a-reference (value or pointer); 1 = mutable `T&`; 2 = const `const T&`.\n", 90LL));
#line 1721 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  int depth=0,ref=0; CXType cur=t;\n", 35LL));
#line 1722 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  while(1){ CXType cc=clang_getCanonicalType(cur);\n", 51LL));
#line 1723 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(cc.kind==CXType_Pointer){depth++; CXType pt=clang_getPointeeType(cur); if(pt.kind==CXType_Invalid) pt=clang_getPointeeType(cc); cur=pt;}\n", 144LL));
#line 1724 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    else if(cc.kind==CXType_LValueReference||cc.kind==CXType_RValueReference){ CXType pt=clang_getPointeeType(cur); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; cur=pt; }\n", 199LL));
#line 1725 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    else if(cc.kind==CXType_ConstantArray||cc.kind==CXType_IncompleteArray||cc.kind==CXType_VariableArray||cc.kind==CXType_DependentSizedArray){ depth++; cur=clang_getArrayElementType(cc); }\n", 191LL));
#line 1726 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    else break; }\n", 18LL));
#line 1727 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType fcan=clang_getCanonicalType(cur);\n", 43LL));
#line 1728 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(fcan.kind==CXType_MemberPointer){ printf(\"%d~%d~d~\", depth, ref); return; }\n", 81LL));
#line 1729 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_np>0){\n", 14LL));
#line 1730 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // A template-parameter-dependent type has canonical spelling \"type-parameter-0-<i>\" (even\n", 95LL));
#line 1731 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // when written via a typedef like value_type/reference) — substitute the concrete arg i.\n", 96LL));
#line 1732 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(fcan); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 137LL));
#line 1733 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(strncmp(r,\"type-parameter-0-\",17)==0){ int idx=atoi(r+17); if(idx>=0 && idx<g_np){ clang_disposeString(sp); classify_arg(depth,ref,g_args[idx]); return; } }\n", 164LL));
#line 1734 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // fallback: match the parameter name literally (direct `T` uses)\n", 70LL));
#line 1735 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    for(int i=0;i<g_np;i++){ if(strcmp(r,g_params[i])==0){ clang_disposeString(sp); classify_arg(depth,ref,g_args[i]); return; } }\n", 131LL));
#line 1736 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // Sequence-container element accessors: `X::reference`/`::const_reference` = `value_type&` and\n", 100LL));
#line 1737 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // `::pointer`/`::const_pointer` = `value_type*` — libstdc++ routes these through an\n", 91LL));
#line 1738 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // `__alloc_traits<…>` typedef that stays dependent, but for vector/string/deque/list/set the\n", 100LL));
#line 1739 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // element type is the FIRST template arg. Resolve to it so at()/front()/back()/[] bind.\n", 93LL));
#line 1740 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    { const char* seg=strrchr(r,':'); if(g_np>0 && seg && seg>r+1 && seg[-1]==':'){ seg++;\n", 91LL));
#line 1741 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"reference\")==0||strcmp(seg,\"const_reference\")==0){ clang_disposeString(sp); classify(depth+1,1,clang_getCanonicalType(g_args[0])); return; }\n", 164LL));
#line 1742 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"pointer\")==0||strcmp(seg,\"const_pointer\")==0){ clang_disposeString(sp); classify(depth+1,0,clang_getCanonicalType(g_args[0])); return; }\n", 160LL));
#line 1743 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        // container `size_type`/`difference_type` also route through dependent allocator traits.\n", 98LL));
#line 1744 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"size_type\")==0){ printf(\"%d~%d~p~c_size_t\", depth, ref); clang_disposeString(sp); return; }\n", 115LL));
#line 1745 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"difference_type\")==0){ printf(\"%d~%d~p~c_ptrdiff_t\", depth, ref); clang_disposeString(sp); return; } } }\n", 128LL));
#line 1746 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // Any type still carrying an un-substituted template param (self-type copy-ctor\n", 85LL));
#line 1747 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // `vector<_Tp,_Alloc>`, or an internal `__alloc_traits<…>` type — often kind Unexposed,\n", 97LL));
#line 1748 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // not Record) is dependent -> mark 'd' so the generator skips the whole method (its shim\n", 94LL));
#line 1749 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // would be un-nameable). Applies regardless of kind.\n", 58LL));
#line 1750 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    int dep=has_token(r,\"type-parameter\");\n", 43LL));
#line 1751 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    for(int i=0;i<g_np && !dep;i++) dep=has_token(r,g_params[i]);\n", 66LL));
#line 1752 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // a NESTED type of the container written without template args (`std::map::value_compare`,\n", 96LL));
#line 1753 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // an uninstantiated `iterator`) is un-nameable in the shim -> skip the method.\n", 84LL));
#line 1754 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(!dep && g_cbase[0]){ char nb[262]; snprintf(nb,sizeof nb,\"%s::\",g_cbase); if(strstr(r,nb)) dep=1; }\n", 107LL));
#line 1755 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(dep){ printf(\"%d~%d~d~\", depth, ref); clang_disposeString(sp); return; }\n", 80LL));
#line 1756 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_disposeString(sp);\n", 29LL));
#line 1757 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  }\n", 4LL));
#line 1758 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(fcan.kind==CXType_Record && depth==0) collect_ext(fcan);  /* by-value external record -> lay out */\n", 105LL));
#line 1759 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  classify(depth,ref,fcan);\n", 28LL));
#line 1760 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1761 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void params(CXCursor c){ int n=clang_Cursor_getNumArguments(c);\n", 71LL));
#line 1762 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<n;i++){ CXCursor a=clang_Cursor_getArgument(c,i); CXString nm=clang_getCursorSpelling(a); const char* nn=S(nm);\n", 128LL));
#line 1763 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"PARAM \"); tds(clang_getArgType(clang_getCursorType(c),i)); if(nn[0])printf(\"|%s\\n\",nn); else printf(\"|a%d\\n\",i); clang_disposeString(nm); } }\n", 154LL));
#line 1764 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* A parameter with a default argument has a child that is neither a type/namespace/template ref\n", 97LL));
#line 1765 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   (those describe the type) — it's the default-value expression. Count the TRAILING defaults (C++\n", 101LL));
#line 1766 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   requires defaults to be trailing) so the generator can emit shorter overloads. */\n", 85LL));
#line 1767 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_hasdef;\n", 21LL));
#line 1768 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult defck(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
#line 1769 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1770 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k!=CXCursor_TypeRef && k!=CXCursor_NamespaceRef && k!=CXCursor_TemplateRef && k!=CXCursor_ParmDecl) g_hasdef=1;\n", 117LL));
#line 1771 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1772 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int count_defaults(CXCursor c){ int n=clang_Cursor_getNumArguments(c), d=0;\n", 83LL));
#line 1773 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=n-1;i>=0;i--){ g_hasdef=0; clang_visitChildren(clang_Cursor_getArgument(c,i), defck, 0); if(g_hasdef) d++; else break; }\n", 133LL));
#line 1774 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return d; }\n", 14LL));
#line 1775 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult fld(CXCursor c, CXCursor p, CXClientData d){\n", 76LL));
#line 1776 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 195LL));
#line 1777 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1778 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult gvar(CXCursor c, CXCursor p, CXClientData d){\n", 77LL));
#line 1779 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_VarDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 192LL));
#line 1780 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1781 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* Inherited-method support: bind a derived class's PUBLIC base methods too (the shim calls them\n", 97LL));
#line 1782 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   through the derived pointer — public inheritance upcasts). Dedup by name so an override / diamond\n", 103LL));
#line 1783 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   base isn't bound twice; own methods are recorded first so they win. */\n", 74LL));
#line 1784 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static char g_seenm[512][96]; static int g_nseenm=0;\n", 53LL));
#line 1785 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_abstract=0;\n", 25LL));
#line 1786 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static char g_curclass[256]={0};\n", 33LL));
#line 1787 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int seen_method(const char* nm){ for(int i=0;i<g_nseenm;i++) if(strcmp(g_seenm[i],nm)==0) return 1; if(g_nseenm<512){ strncpy(g_seenm[g_nseenm],nm,95); g_seenm[g_nseenm][95]=0; g_nseenm++; } return 0; }\n", 210LL));
#line 1788 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_method(CXCursor c){\n", 37LL));
#line 1789 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1790 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if((k==CXCursor_CXXMethod||k==CXCursor_Constructor) && clang_getCursorAvailability(c)==CXAvailability_NotAvailable) return;\n", 126LL));
#line 1791 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 77LL));
#line 1792 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c)));\n", 173LL));
#line 1793 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    { CXString mn=clang_getCursorSpelling(c); CXString pn=clang_getCursorSpelling(clang_getCursorSemanticParent(c)); const char* pns=S(pn);\n", 140LL));
#line 1794 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(g_curclass[0] && pns[0] && strcmp(pns,g_curclass)!=0) printf(\"|%s|%s\\n\", S(mn), pns); else printf(\"|%s\\n\", S(mn));\n", 124LL));
#line 1795 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(mn); clang_disposeString(pn); }\n", 58LL));
#line 1796 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    params(c); printf(\"EMETHOD\\n\"); }\n", 38LL));
#line 1797 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !g_abstract){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 171LL));
#line 1798 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n", 52LL));
#line 1799 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
#line 1800 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ownmeth(CXCursor c, CXCursor p, CXClientData d){\n", 80LL));
#line 1801 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1802 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) seen_method(S(clang_getCursorSpelling(c)));\n", 120LL));
#line 1803 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod||k==CXCursor_Constructor||k==CXCursor_Destructor) emit_method(c);\n", 93LL));
#line 1804 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1805 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_inherited(CXCursor basespec);\n", 47LL));
#line 1806 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult inheritm(CXCursor c, CXCursor p, CXClientData d){\n", 81LL));
#line 1807 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1808 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !clang_CXXMethod_isStatic(c)){\n", 109LL));
#line 1809 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    const char* nm=S(clang_getCursorSpelling(c));\n", 50LL));
#line 1810 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(nm[0]!='~' && !seen_method(nm)) emit_method(c); }\n", 57LL));
#line 1811 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n", 107LL));
#line 1812 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1813 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_inherited(CXCursor basespec){\n", 47LL));
#line 1814 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType bt=clang_getCursorType(basespec);\n", 43LL));
#line 1815 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXString bs=clang_getTypeSpelling(bt); const char* br=S(bs); int istmpl=(strstr(br,\"<\")!=NULL); clang_disposeString(bs);\n", 123LL));
#line 1816 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(istmpl) return;                            /* skip template-base inheritance (kept simple) */\n", 99LL));
#line 1817 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n", 128LL));
#line 1818 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(bdef, inheritm, 0); }\n", 44LL));
#line 1819 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult basespec(CXCursor c, CXCursor p, CXClientData d){\n", 81LL));
#line 1820 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n", 123LL));
#line 1821 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1822 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_prot_base(CXCursor bs);\n", 44LL));
#line 1823 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult collectprot(CXCursor c, CXCursor p, CXClientData d){\n", 84LL));
#line 1824 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1825 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod||k==CXCursor_UsingDeclaration){ enum CX_CXXAccessSpecifier a=clang_getCXXAccessSpecifier(c);\n", 120LL));
#line 1826 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(a==CX_CXXProtected||a==CX_CXXPrivate) seen_method(S(clang_getCursorSpelling(c))); }\n", 91LL));
#line 1827 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXBaseSpecifier) collect_prot_base(c);\n", 62LL));
#line 1828 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1829 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_prot_base(CXCursor bs){\n", 44LL));
#line 1830 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType bt=clang_getCursorType(bs);\n", 37LL));
#line 1831 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXString sp=clang_getTypeSpelling(bt); const char* r=S(sp); int istmpl=(strstr(r,\"<\")!=NULL); clang_disposeString(sp);\n", 121LL));
#line 1832 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(istmpl) return;\n", 21LL));
#line 1833 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n", 128LL));
#line 1834 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(bdef, collectprot, 0); }\n", 47LL));
#line 1835 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_ntp; static char g_tpname[64];\n", 44LL));
#line 1836 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ftcount(CXCursor cc, CXCursor pp, CXClientData dd){\n", 83LL));
#line 1837 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter){ g_ntp++; strncpy(g_tpname, S(clang_getCursorSpelling(cc)), 63); g_tpname[63]=0; }\n", 145LL));
#line 1838 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1839 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int is_tparam(CXType t){\n", 32LL));
#line 1840 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(t.kind==CXType_LValueReference||t.kind==CXType_RValueReference) t=clang_getPointeeType(t);\n", 96LL));
#line 1841 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXString s=clang_getTypeSpelling(t); const char* r=S(s); if(strncmp(r,\"const \",6)==0)r+=6;\n", 93LL));
#line 1842 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  int ok=(strcmp(r,g_tpname)==0); clang_disposeString(s); return ok; }\n", 71LL));
#line 1843 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_ftmpl(CXCursor c, const char* nm){\n", 52LL));
#line 1844 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  g_ntp=0; g_tpname[0]=0; clang_visitChildren(c, ftcount, 0);\n", 62LL));
#line 1845 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_ntp!=1 || !g_tpname[0]) return;\n", 39LL));
#line 1846 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXType ft=clang_getCursorType(c);\n", 36LL));
#line 1847 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(!is_tparam(clang_getResultType(ft))) return;\n", 50LL));
#line 1848 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  int np=clang_getNumArgTypes(ft); if(np<0) return;\n", 52LL));
#line 1849 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<np;i++){ if(!is_tparam(clang_getArgType(ft,i))) return; }\n", 74LL));
#line 1850 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  printf(\"FTMPL %d|%s\\n\", np, nm); }\n", 37LL));
#line 1851 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult _tpk_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n", 83LL));
#line 1852 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind kk=clang_getCursorKind(cc);\n", 48LL));
#line 1853 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(kk==CXCursor_TemplateTypeParameter||kk==CXCursor_NonTypeTemplateParameter||kk==CXCursor_TemplateTemplateParameter){ *(int*)dd=1; return CXChildVisit_Break; }\n", 163LL));
#line 1854 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1855 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("/* An uninstantiated primary/partial class template (has template-parameter children) can't be\n", 95LL));
#line 1856 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   bound as a concrete class — its methods would reference the bare template name (`__gmp_expr`\n", 98LL));
#line 1857 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("   without <args>). Only concrete classes and full instantiations are bindable. */\n", 83LL));
#line 1858 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int is_template_pattern(CXCursor c){ int f=0; clang_visitChildren(c,_tpk_cb,&f); return f; }\n", 100LL));
#line 1859 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d);\n", 78LL));
#line 1860 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult nested(CXCursor c, CXCursor p, CXClientData d){\n", 79LL));
#line 1861 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1862 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)) || k==CXCursor_EnumDecl) return visit(c,p,d);\n", 130LL));
#line 1863 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1864 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
#line 1865 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(!clang_Location_isFromMainFile(clang_getCursorLocation(c))) return CXChildVisit_Continue;\n", 95LL));
#line 1866 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c); CXString nmS=clang_getCursorSpelling(c); const char* nm=S(nmS);\n", 110LL));
#line 1867 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_ClassTemplate||k==CXCursor_ClassTemplatePartialSpecialization){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 139LL));
#line 1870 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_LinkageSpec){ clang_visitChildren(c,visit,0); clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 121LL));
#line 1871 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_Namespace){ printf(\"NS %s\\n\",nm); clang_visitChildren(c,visit,0); printf(\"ENS\\n\"); }\n", 102LL));
#line 1872 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)){\n", 89LL));
#line 1873 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(is_template_pattern(c)){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 90LL));
#line 1874 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXType rt=clang_getCursorType(c); if(!clang_Cursor_isNull(clang_getSpecializedCursorTemplate(c))){ CXString tsx=clang_getTypeSpelling(rt); printf(\"TCLASS %s %d\\n\", clang_getCString(tsx), clang_isPODType(rt)); clang_disposeString(tsx); } else { printf(\"CLASS %s %d\\n\",nm, clang_isPODType(rt)); }\n", 299LL));
#line 1875 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    int saved=g_nseenm; g_nseenm=0;             /* per-class method-name dedup set */\n", 86LL));
#line 1876 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    int savedabs=g_abstract; g_abstract=clang_CXXRecord_isAbstract(c);\n", 71LL));
#line 1877 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    char savedcc[256]; strncpy(savedcc,g_curclass,255); savedcc[255]=0; strncpy(g_curclass,nm,255); g_curclass[255]=0;\n", 119LL));
#line 1878 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,fld,0);\n", 34LL));
#line 1879 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,gvar,0);\n", 35LL));
#line 1880 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,ownmeth,0);           /* own methods/ctors/dtor (records names) */\n", 93LL));
#line 1881 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,collectprot,0);       /* exclude names re-declared protected in a base */\n", 100LL));
#line 1882 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,basespec,0);          /* inherited public methods (deduped) */\n", 89LL));
#line 1883 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,nested,0);            /* nested classes + enums */\n", 77LL));
#line 1884 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    g_nseenm=saved; g_abstract=savedabs; strncpy(g_curclass,savedcc,255); g_curclass[255]=0;\n", 93LL));
#line 1885 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"ECLASS\\n\"); }\n", 26LL));
#line 1886 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 82LL));
#line 1887 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EMETHOD\\n\"); }\n", 227LL));
#line 1888 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 156LL));
#line 1889 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n", 52LL));
#line 1890 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_FunctionDecl){ printf(\"FUNC %d \", count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EFUNC\\n\"); }\n", 178LL));
#line 1891 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_VarDecl){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\",nm); }\n", 100LL));
#line 1892 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_FunctionTemplate) emit_ftmpl(c, nm);\n", 59LL));
#line 1893 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_EnumDecl){ CXType u=clang_getEnumDeclIntegerType(c); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\"; printf(\"ENUM %s %s\\n\", nm[0]?nm:\"anon\", us); clang_visitChildren(c,visit,0); printf(\"EENUM\\n\"); }\n", 226LL));
#line 1894 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_EnumConstantDecl) printf(\"EVAL %s %lld\\n\", nm, (long long)clang_getEnumConstantDeclValue(c));\n", 116LL));
#line 1895 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_TypedefDecl || k==CXCursor_TypeAliasDecl){\n", 65LL));
#line 1896 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // `typedef basic_stringstream<char> stringstream;` — a typedef to a template specialization\n", 99LL));
#line 1897 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // that no signature references. Emit a TDSPEC hint so bindgen force-instantiates + binds it\n", 97LL));
#line 1898 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    // (fixes template-only headers like <sstream>/<random>). std::string is handled by its typemap.\n", 101LL));
#line 1899 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXType u=clang_getCanonicalType(clang_getTypedefDeclUnderlyingType(c));\n", 76LL));
#line 1900 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(u.kind==CXType_Record && clang_Type_getNumTemplateArguments(u)>0){\n", 74LL));
#line 1901 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      CXString us=clang_getTypeSpelling(u); const char* r=S(us);\n", 65LL));
#line 1902 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(strncmp(r,\"std::basic_string<char\",22)!=0) printf(\"TDSPEC %s\\n\", r);\n", 78LL));
#line 1903 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(us); } }\n", 35LL));
#line 1904 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 60LL));
#line 1905 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult collectparams(CXCursor cc, CXCursor pp, CXClientData dd){\n", 89LL));
#line 1906 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter && g_np<8){ strncpy(g_params[g_np], S(clang_getCursorSpelling(cc)), 63); g_params[g_np][63]=0; g_np++; }\n", 166LL));
#line 1907 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1908 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult imeth(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
#line 1909 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
#line 1910 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 77LL));
#line 1911 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 101LL));
#line 1912 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 82LL));
#line 1913 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    { CXString ms=clang_getCursorSpelling(c); const char* mn=clang_getCString(ms);\n", 83LL));
#line 1914 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(strcmp(mn,\"begin\")==0) g_hasbegin=1; else if(strcmp(mn,\"end\")==0) g_hasend=1;\n", 87LL));
#line 1915 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(ms); }\n", 33LL));
#line 1916 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); params(c); printf(\"EMETHOD\\n\"); }\n", 255LL));
#line 1917 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 156LL));
#line 1918 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1919 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static int g_ownmc;\n", 20LL));
#line 1920 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ownmc_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n", 84LL));
#line 1921 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind kk=clang_getCursorKind(cc);\n", 48LL));
#line 1922 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(kk==CXCursor_CXXMethod||kk==CXCursor_Constructor) g_ownmc++;\n", 66LL));
#line 1923 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1924 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult inst(CXCursor c, CXCursor p, CXClientData d){\n", 77LL));
#line 1925 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if((clang_getCursorKind(c)==CXCursor_ClassDecl || clang_getCursorKind(c)==CXCursor_StructDecl) && clang_isCursorDefinition(c)){\n", 130LL));
#line 1926 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(is_template_pattern(c)) return CXChildVisit_Continue;   /* partial spec / primary pattern (`__gmp_expr<T,T>`) — not a concrete instantiation */\n", 153LL));
#line 1927 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    CXCursor tmpl=clang_getSpecializedCursorTemplate(c);\n", 57LL));
#line 1928 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(!clang_Cursor_isNull(tmpl)){\n", 36LL));
#line 1929 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // `getSpecializedCursorTemplate` may return a forward DECLARATION of the template (true for\n", 99LL));
#line 1930 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // std::map/set), which has no member cursors; get the DEFINITION so its methods are visible.\n", 100LL));
#line 1931 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      CXCursor def=clang_getCursorDefinition(tmpl); if(clang_Cursor_isNull(def)) def=tmpl;\n", 91LL));
#line 1932 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      CXType ct=clang_getCursorType(c); g_np=0; clang_visitChildren(def, collectparams, 0);\n", 92LL));
#line 1933 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      int na=clang_Type_getNumTemplateArguments(ct); for(int i=0;i<na && i<8;i++) g_args[i]=clang_Type_getTemplateArgumentAsType(ct,i);\n", 136LL));
#line 1934 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      CXString cts=clang_getTypeSpelling(ct); { const char* cs=S(cts); const char* lt=strchr(cs,'<'); int bl=lt?(int)(lt-cs):(int)strlen(cs); if(bl>255)bl=255; memcpy(g_cbase,cs,bl); g_cbase[bl]=0; }\n", 200LL));
#line 1935 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      printf(\"TCLASS %s %d\\n\", S(cts), clang_isPODType(ct)); clang_disposeString(cts);\n", 87LL));
#line 1936 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      g_hasbegin=0; g_hasend=0;\n", 32LL));
#line 1937 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // Explicit specialization (own members>0) OVERRIDES the primary -> bind ITS members\n", 91LL));
#line 1938 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // (concrete, no substitution); implicit instantiation (0) -> primary + arg substitution.\n", 96LL));
#line 1939 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      g_ownmc=0; clang_visitChildren(c, ownmc_cb, 0);\n", 54LL));
#line 1940 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      CXCursor msrc = (g_ownmc>0) ? c : def;\n", 45LL));
#line 1941 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      clang_visitChildren(msrc, imeth, 0);\n", 43LL));
#line 1942 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // Smart pointers: `get()` returns `element_type*` via a metafunction-dependent typedef that\n", 99LL));
#line 1943 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // libclang won't reduce, and it's inherited from a template base — so synthesize it directly.\n", 103LL));
#line 1944 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      // The element type is the first template arg (a pointer to it). Enables extracting the pointee.\n", 103LL));
#line 1945 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>0 && (strcmp(g_cbase,\"std::shared_ptr\")==0 || strcmp(g_cbase,\"std::unique_ptr\")==0 || strcmp(g_cbase,\"std::weak_ptr\")==0)){\n", 136LL));
#line 1946 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"METHOD .c \"); classify(1,0,clang_getCanonicalType(g_args[0])); printf(\"|get\\n\"); printf(\"EMETHOD\\n\"); }\n", 120LL));
#line 1947 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>0 && g_hasbegin && g_hasend &&\n", 43LL));
#line 1948 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("         (strcmp(g_cbase,\"std::list\")==0||strcmp(g_cbase,\"std::forward_list\")==0||\n", 83LL));
#line 1949 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::set\")==0||strcmp(g_cbase,\"std::multiset\")==0||\n", 78LL));
#line 1950 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::unordered_set\")==0||strcmp(g_cbase,\"std::unordered_multiset\")==0)){\n", 99LL));
#line 1951 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITER \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\"); }\n", 90LL));
#line 1952 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>=2 && g_hasbegin && g_hasend &&\n", 44LL));
#line 1953 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("         (strcmp(g_cbase,\"std::map\")==0||strcmp(g_cbase,\"std::multimap\")==0||\n", 78LL));
#line 1954 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::unordered_map\")==0||strcmp(g_cbase,\"std::unordered_multimap\")==0)){\n", 99LL));
#line 1955 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITERMAPKEY \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\");\n", 94LL));
#line 1956 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITERMAPVAL \"); classify(0,0,clang_getCanonicalType(g_args[1])); printf(\"\\n\"); }\n", 96LL));
#line 1957 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("      printf(\"ECLASS\\n\"); g_np=0; g_cbase[0]=0;\n", 48LL));
#line 1958 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    }\n", 6LL));
#line 1959 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  }\n", 4LL));
#line 1960 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
#line 1961 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("int main(int argc,char**argv){\n", 31LL));
#line 1962 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  int inst_mode=0, off=1;\n", 26LL));
#line 1963 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(argc>1 && strcmp(argv[1],\"--inst\")==0){ inst_mode=1; off=2; }\n", 67LL));
#line 1964 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(argc<off+1){ fprintf(stderr,\"usage: cxxwalk [--inst] <header> [clang args...]\\n\"); return 2; }\n", 100LL));
#line 1965 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXIndex idx=clang_createIndex(0,0);\n", 38LL));
#line 1966 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  CXTranslationUnit tu=clang_parseTranslationUnit(idx,argv[off],(const char**)(argv+off+1),argc-off-1,0,0,0);\n", 110LL));
#line 1967 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(!tu){ fprintf(stderr,\"cxxwalk: libclang could not create a translation unit\\n\"); return 1; }\n", 98LL));
#line 1968 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  unsigned nd=clang_getNumDiagnostics(tu), nerr=0, nfatal=0;\n", 61LL));
#line 1969 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  for(unsigned i=0;i<nd;i++){ CXDiagnostic dg=clang_getDiagnostic(tu,i); enum CXDiagnosticSeverity sv=clang_getDiagnosticSeverity(dg);\n", 135LL));
#line 1970 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    if(sv>=CXDiagnostic_Error){ CXString ds=clang_formatDiagnostic(dg,clang_defaultDiagnosticDisplayOptions()); fprintf(stderr,\"DIAG %s\\n\",clang_getCString(ds)); clang_disposeString(ds); nerr++; if(sv==CXDiagnostic_Fatal) nfatal++; }\n", 234LL));
#line 1971 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("    clang_disposeDiagnostic(dg); }\n", 35LL));
#line 1972 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  fprintf(stderr,\"DIAGERR %u %u\\n\", nerr, nfatal);\n", 51LL));
#line 1973 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  g_inst=inst_mode;\n", 20LL));
#line 1974 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(clang_getTranslationUnitCursor(tu), inst_mode?inst:visit, 0);\n", 84LL));
#line 1975 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  if(!inst_mode) emit_extstructs();   /* lay out by-value external (non-system) records */\n", 91LL));
#line 1976 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeTranslationUnit(tu); clang_disposeIndex(idx); return 0; }\n", 73LL));
#line 1977 "src/bindgen.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void _rm_files(TrStr files) {
#line 1983 "src/bindgen.tr"
    if (_tr_is_windows()) {
#line 1984 "src/bindgen.tr"
        ({ TrStr _aet_t3207 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("del /q ", 7LL)), (files))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" >nul 2>&1", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3207.data); _tr_str_release(_aet_t3207); });
    } else {
#line 1986 "src/bindgen.tr"
        ({ TrStr _aet_t3208 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("rm -f ", 6LL)), (files))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 2>/dev/null", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3208.data); _tr_str_release(_aet_t3208); });
    }
}

__attribute__((hot)) TrStr _local_exe(TrStr stem) {
#line 1990 "src/bindgen.tr"
    if (_tr_is_windows()) {
#line 1990 "src/bindgen.tr"
        return ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len(".\\", 2LL)), (stem))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".exe", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
    }
#line 1991 "src/bindgen.tr"
    return ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("./", 2LL)), (stem))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".exe", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
}

__attribute__((hot)) TrStr _detect_libclang(TrStr cc) {
#line 1995 "src/bindgen.tr"
    write_file(_tr_str_lit_len("_cxxprobe.c", 11LL), _tr_str_lit_len("#include <clang-c/Index.h>\nint main(){ clang_createIndex(0,0); return 0; }\n", 75LL));
#line 1996 "src/bindgen.tr"
    long long rc = ({ TrStr _aet_t3209 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" _cxxprobe.c -o _cxxprobe.exe -lclang 2>_cxx_err.txt", 52LL)))); __auto_type _wr = (_tr_system(_aet_t3209.data)); _tr_str_release(_aet_t3209); _wr; });
#line 1997 "src/bindgen.tr"
    _rm_files(_tr_str_lit_len("_cxxprobe.c _cxxprobe.exe _cxx_err.txt", 38LL));
#line 1998 "src/bindgen.tr"
    if ((rc == 0LL)) {
#line 1998 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 1999 "src/bindgen.tr"
    StringBuilder* g = StringBuilder_init(768LL);
#line 2000 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("bindgen: `-h cpp` needs libclang (Clang's C API), which was not found.\n", 71LL));
#line 2001 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("         It is required ONLY for C++ headers; plain C headers never use it.\n\n", 77LL));
#line 2002 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("Install it:\n", 12LL));
#line 2003 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("  Windows       winget install LLVM.LLVM   (or MSYS2: pacman -S mingw-w64-x86_64-clang)\n", 88LL));
#line 2004 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("  Debian/Ubuntu sudo apt install libclang-dev\n", 46LL));
#line 2005 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("  Fedora        sudo dnf install clang-devel\n", 45LL));
#line 2006 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("  Arch          sudo pacman -S clang\n", 37LL));
#line 2007 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("  macOS         brew install llvm   (or Xcode Command Line Tools)\n\n", 67LL));
#line 2008 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("Download        https://github.com/llvm/llvm-project/releases\n", 62LL));
#line 2009 "src/bindgen.tr"
    StringBuilder_append(g, _tr_str_lit_len("Then re-run:    tauraroc bindgen <header.hpp> -h cpp\n", 53LL));
#line 2010 "src/bindgen.tr"
    return StringObj_as_str(StringBuilder_to_string(g));
}

__attribute__((hot)) CppType* _cpp_parse_type(TrStr spelling) {
#line 2021 "src/bindgen.tr"
    CppType* t = ((CppType*)_tr_obj_alloc(sizeof(CppType)));
#line 2022 "src/bindgen.tr"
    t->base = _tr_str_lit_len("", 0LL);
#line 2023 "src/bindgen.tr"
    t->ptr = 0LL;
#line 2024 "src/bindgen.tr"
    t->was_ptr = false;
#line 2025 "src/bindgen.tr"
    t->was_ref = false;
#line 2026 "src/bindgen.tr"
    t->is_prim = false;
#line 2027 "src/bindgen.tr"
    TrStr norm = _tr_str_lit_len("", 0LL);
#line 2028 "src/bindgen.tr"
    long long i = 0LL;
#line 2029 "src/bindgen.tr"
    while ((i < _tr_str_lenv((spelling)))) {
#line 2030 "src/bindgen.tr"
        TrStr ch = _tr_str_slicev((spelling), i, (i + 1LL));
#line 2031 "src/bindgen.tr"
        if (_tr_str_eqv((ch), (_tr_str_lit_len("*", 1LL)))) {
#line 2031 "src/bindgen.tr"
            ({ TrStr _ap_t3210 = (_tr_str_lit_len(" * ", 3LL)); _tr_str_append(&norm, _ap_t3210); });
        } else if (_tr_str_eqv((ch), (_tr_str_lit_len("&", 1LL)))) {
#line 2032 "src/bindgen.tr"
            ({ TrStr _ap_t3211 = (_tr_str_lit_len(" & ", 3LL)); _tr_str_append(&norm, _ap_t3211); });
        } else {
#line 2033 "src/bindgen.tr"
            ({ TrStr _ap_t3212 = (ch); _tr_str_append(&norm, _ap_t3212); });
        }
#line 2034 "src/bindgen.tr"
        i = (i + 1LL);
        _tr_str_release(ch);
    }
#line 2035 "src/bindgen.tr"
    List_TrStr* words = _tr_str_splitv((norm), (_tr_str_lit_len(" ", 1LL)));
#line 2036 "src/bindgen.tr"
    TrStr bw = _tr_str_lit_len("", 0LL);
#line 2037 "src/bindgen.tr"
    long long wi = 0LL;
#line 2038 "src/bindgen.tr"
    while ((wi < words->len)) {
#line 2039 "src/bindgen.tr"
        TrStr w = List_TrStr_get(words, wi);
#line 2040 "src/bindgen.tr"
        wi = (wi + 1LL);
#line 2041 "src/bindgen.tr"
        if (_tr_str_eqv((w), (_tr_str_lit_len("", 0LL)))) {
#line 2041 "src/bindgen.tr"
            _tr_str_release(w);
            continue;
        }
#line 2042 "src/bindgen.tr"
        if (((((_tr_str_eqv((w), (_tr_str_lit_len("const", 5LL))) || _tr_str_eqv((w), (_tr_str_lit_len("volatile", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("struct", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("class", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("enum", 4LL))))) {
#line 2042 "src/bindgen.tr"
            _tr_str_release(w);
            continue;
        }
#line 2043 "src/bindgen.tr"
        if (_tr_str_eqv((w), (_tr_str_lit_len("*", 1LL)))) {
#line 2043 "src/bindgen.tr"
            t->ptr = (t->ptr + 1LL);
#line 2043 "src/bindgen.tr"
            t->was_ptr = true;
#line 2043 "src/bindgen.tr"
            _tr_str_release(w);
            continue;
        }
#line 2044 "src/bindgen.tr"
        if (_tr_str_eqv((w), (_tr_str_lit_len("&", 1LL)))) {
#line 2044 "src/bindgen.tr"
            t->ptr = (t->ptr + 1LL);
#line 2044 "src/bindgen.tr"
            t->was_ref = true;
#line 2044 "src/bindgen.tr"
            _tr_str_release(w);
            continue;
        }
#line 2045 "src/bindgen.tr"
        if (_tr_str_eqv((bw), (_tr_str_lit_len("", 0LL)))) {
#line 2045 "src/bindgen.tr"
            TrStr _strtmp_t3213 = _tr_str_retain(w);
            _tr_str_release(bw);
            bw = _strtmp_t3213;
        } else {
#line 2046 "src/bindgen.tr"
            ({ TrStr _ap_t3214 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3215 = (w); _tr_str_append(&bw, _ap_t3214); _tr_str_append(&bw, _ap_t3215); });
        }
        _tr_str_release(w);
    }
#line 2047 "src/bindgen.tr"
    t->base = _tr_str_retain(bw);
#line 2048 "src/bindgen.tr"
    TrStr mb = map_base(bw);
#line 2049 "src/bindgen.tr"
    if ((((!_tr_str_eqv((mb), (bw))) || _tr_str_eqv((bw), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((bw), (_tr_str_lit_len("bool", 4LL))))) {
#line 2049 "src/bindgen.tr"
        t->is_prim = true;
    }
#line 2050 "src/bindgen.tr"
    _tr_str_release(norm);
    List_TrStr_free(words);
    _tr_str_release(bw);
    _tr_str_release(mb);
    return t;
}

__attribute__((hot)) TrStr _last_seg(TrStr s) {
#line 2054 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((s), (_tr_str_lit_len("::", 2LL)));
#line 2055 "src/bindgen.tr"
    if ((parts->len == 0LL)) {
#line 2055 "src/bindgen.tr"
        TrStr _trret622 = _tr_str_retain(s);
        List_TrStr_free(parts);
        return _trret622;
    }
#line 2056 "src/bindgen.tr"
    return List_TrStr_get(parts, (parts->len - 1LL));
}

__attribute__((hot)) TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member) {
#line 2066 "src/bindgen.tr"
    if ((_tr_str_lenv((mname)) < 9LL)) {
#line 2066 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 2067 "src/bindgen.tr"
    TrStr g = _tr_str_slicev((mname), 8LL, _tr_str_lenv((mname)));
#line 2070 "src/bindgen.tr"
    char c0 = _tr_strz(g)[0LL];
#line 2071 "src/bindgen.tr"
    if (((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 32LL))) {
#line 2071 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("", 0LL);
    }
#line 2072 "src/bindgen.tr"
    bool unary = false;
#line 2073 "src/bindgen.tr"
    if (is_member) {
#line 2073 "src/bindgen.tr"
        unary = (nparams == 0LL);
    } else {
#line 2074 "src/bindgen.tr"
        unary = (nparams <= 1LL);
    }
#line 2075 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("[]", 2LL)))) {
#line 2075 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_index", 8LL);
    }
#line 2076 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("()", 2LL)))) {
#line 2076 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_call", 7LL);
    }
#line 2077 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("->", 2LL)))) {
#line 2077 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_arrow", 8LL);
    }
#line 2078 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("++", 2LL)))) {
#line 2078 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_inc", 6LL);
    }
#line 2079 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("--", 2LL)))) {
#line 2079 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_dec", 6LL);
    }
#line 2080 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("==", 2LL)))) {
#line 2080 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_eq", 5LL);
    }
#line 2081 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("!=", 2LL)))) {
#line 2081 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_ne", 5LL);
    }
#line 2082 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("<=", 2LL)))) {
#line 2082 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_le", 5LL);
    }
#line 2083 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len(">=", 2LL)))) {
#line 2083 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_ge", 5LL);
    }
#line 2084 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("<", 1LL)))) {
#line 2084 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_lt", 5LL);
    }
#line 2085 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len(">", 1LL)))) {
#line 2085 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_gt", 5LL);
    }
#line 2086 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("<<", 2LL)))) {
#line 2086 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_lshift", 9LL);
    }
#line 2087 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len(">>", 2LL)))) {
#line 2087 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_rshift", 9LL);
    }
#line 2088 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("+=", 2LL)))) {
#line 2088 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_iadd", 7LL);
    }
#line 2089 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("-=", 2LL)))) {
#line 2089 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_isub", 7LL);
    }
#line 2090 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("*=", 2LL)))) {
#line 2090 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_imul", 7LL);
    }
#line 2091 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("/=", 2LL)))) {
#line 2091 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_idiv", 7LL);
    }
#line 2092 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("%=", 2LL)))) {
#line 2092 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_imod", 7LL);
    }
#line 2093 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("&=", 2LL)))) {
#line 2093 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_iand", 7LL);
    }
#line 2094 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("|=", 2LL)))) {
#line 2094 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_ior", 6LL);
    }
#line 2095 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("^=", 2LL)))) {
#line 2095 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_ixor", 7LL);
    }
#line 2096 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("*", 1LL)))) {
#line 2097 "src/bindgen.tr"
        if (unary) {
#line 2097 "src/bindgen.tr"
            _tr_str_release(g);
            return _tr_str_lit_len("op_deref", 8LL);
        }
#line 2098 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_mul", 6LL);
    }
#line 2099 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("+", 1LL)))) {
#line 2100 "src/bindgen.tr"
        if (unary) {
#line 2100 "src/bindgen.tr"
            _tr_str_release(g);
            return _tr_str_lit_len("op_pos", 6LL);
        }
#line 2101 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_add", 6LL);
    }
#line 2102 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("-", 1LL)))) {
#line 2103 "src/bindgen.tr"
        if (unary) {
#line 2103 "src/bindgen.tr"
            _tr_str_release(g);
            return _tr_str_lit_len("op_neg", 6LL);
        }
#line 2104 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_sub", 6LL);
    }
#line 2105 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("/", 1LL)))) {
#line 2105 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_div", 6LL);
    }
#line 2106 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("%", 1LL)))) {
#line 2106 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_mod", 6LL);
    }
#line 2107 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("&", 1LL)))) {
#line 2108 "src/bindgen.tr"
        if (unary) {
#line 2108 "src/bindgen.tr"
            _tr_str_release(g);
            return _tr_str_lit_len("", 0LL);
        }
#line 2109 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitand", 9LL);
    }
#line 2110 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("|", 1LL)))) {
#line 2110 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitor", 8LL);
    }
#line 2111 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("^", 1LL)))) {
#line 2111 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_xor", 6LL);
    }
#line 2112 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("~", 1LL)))) {
#line 2112 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitnot", 9LL);
    }
#line 2113 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("!", 1LL)))) {
#line 2113 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_lnot", 7LL);
    }
#line 2114 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("&&", 2LL)))) {
#line 2114 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_land", 7LL);
    }
#line 2115 "src/bindgen.tr"
    if (_tr_str_eqv((g), (_tr_str_lit_len("||", 2LL)))) {
#line 2115 "src/bindgen.tr"
        _tr_str_release(g);
        return _tr_str_lit_len("op_lor", 6LL);
    }
#line 2116 "src/bindgen.tr"
    _tr_str_release(g);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _cpp_ident(TrStr s) {
#line 2122 "src/bindgen.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 2123 "src/bindgen.tr"
    long long i = 0LL;
#line 2124 "src/bindgen.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 2125 "src/bindgen.tr"
        char c = _tr_strz(s)[i];
#line 2126 "src/bindgen.tr"
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
#line 2127 "src/bindgen.tr"
            ({ TrStr _ap_t3216 = (_tr_str_slicev((s), i, (i + 1LL))); _tr_str_append(&out, _ap_t3216); _tr_str_release(_ap_t3216); });
        } else {
#line 2129 "src/bindgen.tr"
            ({ TrStr _ap_t3217 = (_tr_str_lit_len("_", 1LL)); _tr_str_append(&out, _ap_t3217); });
        }
#line 2130 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2131 "src/bindgen.tr"
    if (_tr_str_eqv((out), (_tr_str_lit_len("", 0LL)))) {
#line 2131 "src/bindgen.tr"
        _tr_str_release(out);
        return _tr_str_lit_len("T_", 2LL);
    }
#line 2132 "src/bindgen.tr"
    char c0 = _tr_strz(out)[0LL];
#line 2133 "src/bindgen.tr"
    if (((c0 >= 48LL) && (c0 <= 57LL))) {
#line 2133 "src/bindgen.tr"
        TrStr _trret623 = _tr_strx_concatv((_tr_str_lit_len("T_", 2LL)), (out));
        _tr_str_release(out);
        return _trret623;
    }
#line 2134 "src/bindgen.tr"
    TrStr _trret624 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret624;
}

__attribute__((hot)) bool _is_clean_ident(TrStr s) {
#line 2140 "src/bindgen.tr"
    if ((_tr_str_lenv((s)) == 0LL)) {
#line 2140 "src/bindgen.tr"
        return false;
    }
#line 2141 "src/bindgen.tr"
    char c0 = _tr_strz(s)[0LL];
#line 2142 "src/bindgen.tr"
    if ((!((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 95LL)))) {
#line 2142 "src/bindgen.tr"
        return false;
    }
#line 2143 "src/bindgen.tr"
    long long i = 1LL;
#line 2144 "src/bindgen.tr"
    while ((i < _tr_str_lenv((s)))) {
#line 2145 "src/bindgen.tr"
        char c = _tr_strz(s)[i];
#line 2146 "src/bindgen.tr"
        if ((!(((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL)))) {
#line 2146 "src/bindgen.tr"
            return false;
        }
#line 2147 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2148 "src/bindgen.tr"
    return true;
}

__attribute__((hot)) bool _is_tr_keyword(TrStr n) {
#line 2152 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("is", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("as", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("not", 3LL))))) {
#line 2152 "src/bindgen.tr"
        return true;
    }
#line 2153 "src/bindgen.tr"
    if (((((((_tr_str_eqv((n), (_tr_str_lit_len("if", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("else", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("elif", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("for", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("while", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("match", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("case", 4LL))))) {
#line 2153 "src/bindgen.tr"
        return true;
    }
#line 2154 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("def", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("return", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("class", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("enum", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("type", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("interface", 9LL))))) {
#line 2154 "src/bindgen.tr"
        return true;
    }
#line 2155 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("mut", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("pub", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("from", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("import", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pass", 4LL))))) {
#line 2155 "src/bindgen.tr"
        return true;
    }
#line 2156 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("break", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("continue", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("true", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("false", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("none", 4LL))))) {
#line 2156 "src/bindgen.tr"
        return true;
    }
#line 2157 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("super", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("with", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("try", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("raise", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("async", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("await", 5LL))))) {
#line 2157 "src/bindgen.tr"
        return true;
    }
#line 2158 "src/bindgen.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("var", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("do", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("loop", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("defer", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("actor", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("decorator", 9LL))))) {
#line 2158 "src/bindgen.tr"
        return true;
    }
#line 2159 "src/bindgen.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("extend", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("implements", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("extends", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("throws", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("spawn", 5LL))))) {
#line 2159 "src/bindgen.tr"
        return true;
    }
#line 2160 "src/bindgen.tr"
    return false;
}

__attribute__((hot)) TrStr _uniq_sym(TrStr base_sym, TrMap* used) {
#line 2165 "src/bindgen.tr"
    if (_tr_dict_contains(used, _tr_strz(base_sym))) {
#line 2166 "src/bindgen.tr"
        long long n = (((long long)(uintptr_t)_tr_dict_get(used, _tr_strz(base_sym))) + 1LL);
#line 2167 "src/bindgen.tr"
        _tr_dict_set(used, _tr_strz(base_sym), n);
#line 2168 "src/bindgen.tr"
        return ({ TrStr _tr_cl = (_tr_strx_concatv((base_sym), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
    }
#line 2169 "src/bindgen.tr"
    _tr_dict_set(used, _tr_strz(base_sym), 1LL);
#line 2170 "src/bindgen.tr"
    return _tr_str_retain(base_sym);
}

__attribute__((hot)) TrStr _cpp_tr_pname(TrStr pname) {
#line 2174 "src/bindgen.tr"
    if (((_tr_str_eqv((pname), (_tr_str_lit_len("self", 4LL))) || _tr_str_eqv((pname), (_tr_str_lit_len("obj", 3LL)))) || _is_tr_keyword(pname))) {
#line 2174 "src/bindgen.tr"
        return _tr_strx_concatv((pname), (_tr_str_lit_len("_", 1LL)));
    }
#line 2175 "src/bindgen.tr"
    return _tr_str_retain(pname);
}

__attribute__((hot)) TrStr _cpp_ctype(CppType* t) {
#line 2179 "src/bindgen.tr"
    TrStr s = _tr_str_retain(t->base);
#line 2180 "src/bindgen.tr"
    long long i = 0LL;
#line 2181 "src/bindgen.tr"
    while ((i < t->ptr)) {
#line 2182 "src/bindgen.tr"
        TrStr _strtmp_t3219 = _tr_strx_concatv((s), (_tr_str_lit_len("*", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t3219;
#line 2183 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2184 "src/bindgen.tr"
    TrStr _trret625 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret625;
}

__attribute__((hot)) TrStr _cpp_tr_type(CppType* t) {
#line 2191 "src/bindgen.tr"
    if (t->is_prim) {
#line 2191 "src/bindgen.tr"
        return map_type(t->base, t->ptr);
    }
#line 2192 "src/bindgen.tr"
    long long eff = t->ptr;
#line 2193 "src/bindgen.tr"
    if ((eff < 1LL)) {
#line 2193 "src/bindgen.tr"
        eff = 1LL;
    }
#line 2194 "src/bindgen.tr"
    TrStr ty = _last_seg(t->base);
#line 2195 "src/bindgen.tr"
    long long k = 1LL;
#line 2196 "src/bindgen.tr"
    while ((k < eff)) {
#line 2197 "src/bindgen.tr"
        TrStr _strtmp_t3220 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(ty);
        ty = _strtmp_t3220;
#line 2198 "src/bindgen.tr"
        k = (k + 1LL);
    }
#line 2199 "src/bindgen.tr"
    TrStr _trret626 = _tr_str_retain(ty);
    _tr_str_release(ty);
    return _trret626;
}

__attribute__((hot)) List_TrStr* _cpp_ret(CppType* rt, TrStr call) {
#line 2203 "src/bindgen.tr"
    List_TrStr* r = (void*)List_TrStr_new();
#line 2204 "src/bindgen.tr"
    if ((_tr_str_eqv((rt->base), (_tr_str_lit_len("void", 4LL))) && (rt->ptr == 0LL))) {
#line 2205 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("void", 4LL));
#line 2205 "src/bindgen.tr"
        ({ TrStr _at_t3221 = (_tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)))); List_TrStr_append(r, _at_t3221); _tr_str_release(_at_t3221); });
#line 2205 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("", 0LL));
#line 2206 "src/bindgen.tr"
        return r;
    }
#line 2207 "src/bindgen.tr"
    if (rt->is_prim) {
#line 2208 "src/bindgen.tr"
        ({ TrStr _at_t3222 = (_cpp_ctype(rt)); List_TrStr_append(r, _at_t3222); _tr_str_release(_at_t3222); });
#line 2208 "src/bindgen.tr"
        ({ TrStr _at_t3223 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3223); _tr_str_release(_at_t3223); });
#line 2208 "src/bindgen.tr"
        ({ TrStr _at_t3224 = (_cpp_tr_type(rt)); List_TrStr_append(r, _at_t3224); _tr_str_release(_at_t3224); });
#line 2209 "src/bindgen.tr"
        return r;
    }
#line 2210 "src/bindgen.tr"
    TrStr trret = _cpp_tr_type(rt);
#line 2211 "src/bindgen.tr"
    if (rt->was_ptr) {
#line 2212 "src/bindgen.tr"
        ({ TrStr _at_t3225 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3225); _tr_str_release(_at_t3225); });
#line 2212 "src/bindgen.tr"
        ({ TrStr _at_t3226 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3226); _tr_str_release(_at_t3226); });
#line 2212 "src/bindgen.tr"
        List_TrStr_append(r, trret);
    } else if (rt->was_ref) {
#line 2214 "src/bindgen.tr"
        ({ TrStr _at_t3227 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3227); _tr_str_release(_at_t3227); });
#line 2214 "src/bindgen.tr"
        ({ TrStr _at_t3228 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return &(", 9LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3228); _tr_str_release(_at_t3228); });
#line 2214 "src/bindgen.tr"
        List_TrStr_append(r, trret);
    } else {
#line 2216 "src/bindgen.tr"
        ({ TrStr _at_t3229 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3229); _tr_str_release(_at_t3229); });
#line 2216 "src/bindgen.tr"
        ({ TrStr _at_t3230 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return new ", 11LL)), (rt->base))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3230); _tr_str_release(_at_t3230); });
#line 2216 "src/bindgen.tr"
        List_TrStr_append(r, trret);
    }
#line 2217 "src/bindgen.tr"
    _tr_str_release(trret);
    return r;
}

__attribute__((hot)) TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque) {
#line 2224 "src/bindgen.tr"
    TrStr seg = _last_seg(base);
#line 2225 "src/bindgen.tr"
    TrStr handle = _tr_str_retain(seg);
#line 2226 "src/bindgen.tr"
    if ((_is_libc_ty_name(seg) || _is_system_record_ty(seg))) {
#line 2230 "src/bindgen.tr"
        TrStr _strtmp_t3231 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t3231;
    } else if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
#line 2233 "src/bindgen.tr"
        TrStr _strtmp_t3232 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t3232;
    } else if ((!_tr_dict_contains(class_names, _tr_strz(seg)))) {
#line 2235 "src/bindgen.tr"
        TrStr _strtmp_t3233 = _cpp_ident(base);
        _tr_str_release(handle);
        handle = _strtmp_t3233;
#line 2236 "src/bindgen.tr"
        if (({ TrStr _dkt_t3234 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (handle))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3234)))); _tr_str_release(_dkt_t3234); _wr; })) {
#line 2237 "src/bindgen.tr"
            ({ TrStr _sbt_t3235 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (handle))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(opaque, _sbt_t3235); _tr_str_release(_sbt_t3235); });
#line 2238 "src/bindgen.tr"
            ({ TrStr _dkt_t3236 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (handle))); _tr_dict_set(seen, _tr_strz(_dkt_t3236), true); _tr_str_release(_dkt_t3236); });
        }
    }
#line 2242 "src/bindgen.tr"
    TrStr ty = _tr_str_retain(handle);
#line 2243 "src/bindgen.tr"
    long long k = 1LL;
#line 2244 "src/bindgen.tr"
    if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
#line 2244 "src/bindgen.tr"
        k = 0LL;
    }
#line 2245 "src/bindgen.tr"
    while ((k < nd)) {
#line 2246 "src/bindgen.tr"
        TrStr _strtmp_t3237 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(ty);
        ty = _strtmp_t3237;
#line 2247 "src/bindgen.tr"
        k = (k + 1LL);
    }
#line 2248 "src/bindgen.tr"
    TrStr _trret627 = _tr_str_retain(ty);
    _tr_str_release(seg);
    _tr_str_release(handle);
    _tr_str_release(ty);
    return _trret627;
}

__attribute__((hot)) TrStr _cpp_qual(TrStr base, TrMap* class_qual) {
#line 2254 "src/bindgen.tr"
    TrStr seg = _last_seg(base);
#line 2255 "src/bindgen.tr"
    if (_tr_dict_contains(class_qual, _tr_strz(seg))) {
#line 2255 "src/bindgen.tr"
        TrStr _trret628 = _tr_str_retain(_tr_str_unbox(_tr_dict_get(class_qual, _tr_strz(seg))));
        _tr_str_release(seg);
        return _trret628;
    }
#line 2256 "src/bindgen.tr"
    TrStr _trret629 = _tr_str_retain(base);
    _tr_str_release(seg);
    return _trret629;
}

__attribute__((hot)) List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque) {
#line 2262 "src/bindgen.tr"
    List_TrStr* r = (void*)List_TrStr_new();
#line 2263 "src/bindgen.tr"
    List_TrStr* d4 = _desc4(desc);
#line 2264 "src/bindgen.tr"
    long long rdepth = ({ TrStr _at_t3238 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t3238)); _tr_str_release(_at_t3238); _wr; });
#line 2265 "src/bindgen.tr"
    bool rref = ({ TrStr _cmpa_t3239 = (List_TrStr_get(d4, 1LL)); TrStr _cmpb_t3240 = (_tr_str_lit_len("0", 1LL)); _Bool _cmpr_t3241 = !_tr_str_eqv(_cmpa_t3239, _cmpb_t3240); _tr_str_release(_cmpa_t3239); _cmpr_t3241; });
#line 2266 "src/bindgen.tr"
    TrStr cat = List_TrStr_get(d4, 2LL);
#line 2267 "src/bindgen.tr"
    TrStr detail = List_TrStr_get(d4, 3LL);
#line 2268 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("v", 1LL)))) {
#line 2269 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("void", 4LL));
#line 2269 "src/bindgen.tr"
        ({ TrStr _at_t3242 = (_tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)))); List_TrStr_append(r, _at_t3242); _tr_str_release(_at_t3242); });
#line 2269 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("", 0LL));
#line 2269 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
#line 2270 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("p", 1LL)))) {
#line 2271 "src/bindgen.tr"
        TrStr cpp = _c_to_cpp(detail);
#line 2272 "src/bindgen.tr"
        if ((rdepth == 0LL)) {
#line 2273 "src/bindgen.tr"
            List_TrStr_append(r, cpp);
#line 2273 "src/bindgen.tr"
            ({ TrStr _at_t3243 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3243); _tr_str_release(_at_t3243); });
#line 2273 "src/bindgen.tr"
            List_TrStr_append(r, detail);
#line 2273 "src/bindgen.tr"
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(cpp);
            return r;
        }
#line 2276 "src/bindgen.tr"
        TrStr cty = ({ TrStr _tr_cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (_stars(rdepth)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 2277 "src/bindgen.tr"
        if (rref) {
#line 2277 "src/bindgen.tr"
            List_TrStr_append(r, cty);
#line 2277 "src/bindgen.tr"
            ({ TrStr _at_t3244 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")(&(", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("));", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3244); _tr_str_release(_at_t3244); });
#line 2277 "src/bindgen.tr"
            ({ TrStr _at_t3245 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t3245); _tr_str_release(_at_t3245); });
        } else {
#line 2278 "src/bindgen.tr"
            List_TrStr_append(r, cty);
#line 2278 "src/bindgen.tr"
            ({ TrStr _at_t3246 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")(", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3246); _tr_str_release(_at_t3246); });
#line 2278 "src/bindgen.tr"
            ({ TrStr _at_t3247 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t3247); _tr_str_release(_at_t3247); });
        }
#line 2279 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(cpp);
        _tr_str_release(cty);
        return r;
    }
#line 2280 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("e", 1LL)))) {
#line 2281 "src/bindgen.tr"
        TrStr espell = List_TrStr_get(_tr_str_splitv((detail), (_tr_str_lit_len("#", 1LL))), 0LL);
#line 2282 "src/bindgen.tr"
        TrStr ealias = _last_seg(espell);
#line 2283 "src/bindgen.tr"
        if ((rdepth == 0LL)) {
#line 2284 "src/bindgen.tr"
            List_TrStr_append(r, espell);
#line 2284 "src/bindgen.tr"
            ({ TrStr _at_t3248 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3248); _tr_str_release(_at_t3248); });
#line 2284 "src/bindgen.tr"
            List_TrStr_append(r, ealias);
#line 2284 "src/bindgen.tr"
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(espell);
            _tr_str_release(ealias);
            return r;
        }
#line 2285 "src/bindgen.tr"
        if (rref) {
#line 2285 "src/bindgen.tr"
            ({ TrStr _at_t3249 = (_tr_strx_concatv((espell), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3249); _tr_str_release(_at_t3249); });
#line 2285 "src/bindgen.tr"
            ({ TrStr _at_t3250 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return &(", 9LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3250); _tr_str_release(_at_t3250); });
#line 2285 "src/bindgen.tr"
            ({ TrStr _at_t3251 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t3251); _tr_str_release(_at_t3251); });
        } else {
#line 2286 "src/bindgen.tr"
            ({ TrStr _at_t3252 = (_tr_strx_concatv((espell), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3252); _tr_str_release(_at_t3252); });
#line 2286 "src/bindgen.tr"
            ({ TrStr _at_t3253 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3253); _tr_str_release(_at_t3253); });
#line 2286 "src/bindgen.tr"
            ({ TrStr _at_t3254 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t3254); _tr_str_release(_at_t3254); });
        }
#line 2287 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(espell);
        _tr_str_release(ealias);
        return r;
    }
#line 2288 "src/bindgen.tr"
    if ((_tr_str_eqv((cat), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((detail), (_tr_str_lit_len("string", 6LL))))) {
#line 2290 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("char*", 5LL));
#line 2290 "src/bindgen.tr"
        ({ TrStr _at_t3255 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return _tr_cpp_strdup(", 22LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3255); _tr_str_release(_at_t3255); });
#line 2290 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("Pointer[char]", 13LL));
#line 2290 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
#line 2291 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("f", 1LL)))) {
#line 2292 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("void*", 5LL));
#line 2292 "src/bindgen.tr"
        ({ TrStr _at_t3256 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (void*)(", 15LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3256); _tr_str_release(_at_t3256); });
#line 2292 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("Pointer[void]", 13LL));
#line 2292 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
#line 2293 "src/bindgen.tr"
    TrStr seg = _last_seg(detail);
#line 2294 "src/bindgen.tr"
    if (((_tr_str_eqv((cat), (_tr_str_lit_len("r", 1LL))) && _tr_dict_contains(value_structs, _tr_strz(seg))) && (rdepth == 0LL))) {
#line 2295 "src/bindgen.tr"
        ({ TrStr _at_t3257 = (_cpp_qual(detail, class_qual)); List_TrStr_append(r, _at_t3257); _tr_str_release(_at_t3257); });
#line 2295 "src/bindgen.tr"
        ({ TrStr _at_t3258 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3258); _tr_str_release(_at_t3258); });
#line 2295 "src/bindgen.tr"
        List_TrStr_append(r, seg);
#line 2295 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(seg);
        return r;
    }
#line 2296 "src/bindgen.tr"
    long long nd = rdepth;
#line 2297 "src/bindgen.tr"
    if ((nd < 1LL)) {
#line 2297 "src/bindgen.tr"
        nd = 1LL;
    }
#line 2298 "src/bindgen.tr"
    TrStr handle = _cpp_opaque_handle(detail, nd, class_names, value_structs, seen, opaque);
#line 2299 "src/bindgen.tr"
    TrStr cb = _cpp_qual(detail, class_qual);
#line 2301 "src/bindgen.tr"
    if ((rdepth == 0LL)) {
#line 2302 "src/bindgen.tr"
        ({ TrStr _at_t3259 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3259); _tr_str_release(_at_t3259); });
#line 2302 "src/bindgen.tr"
        ({ TrStr _at_t3260 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return new ", 11LL)), (cb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3260); _tr_str_release(_at_t3260); });
#line 2302 "src/bindgen.tr"
        List_TrStr_append(r, handle);
    } else if (rref) {
#line 2304 "src/bindgen.tr"
        ({ TrStr _at_t3261 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3261); _tr_str_release(_at_t3261); });
#line 2304 "src/bindgen.tr"
        ({ TrStr _at_t3262 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)(&(", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("));", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3262); _tr_str_release(_at_t3262); });
#line 2304 "src/bindgen.tr"
        List_TrStr_append(r, handle);
    } else {
#line 2306 "src/bindgen.tr"
        ({ TrStr _at_t3263 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t3263); _tr_str_release(_at_t3263); });
#line 2306 "src/bindgen.tr"
        ({ TrStr _at_t3264 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("*)(", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (call)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(r, _at_t3264); _tr_str_release(_at_t3264); });
#line 2306 "src/bindgen.tr"
        List_TrStr_append(r, handle);
    }
#line 2307 "src/bindgen.tr"
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    _tr_str_release(seg);
    _tr_str_release(handle);
    _tr_str_release(cb);
    return r;
}

__attribute__((hot)) TrStr _ns_pop(TrStr path) {
#line 2311 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((path), (_tr_str_lit_len("::", 2LL)));
#line 2312 "src/bindgen.tr"
    if ((parts->len <= 1LL)) {
#line 2312 "src/bindgen.tr"
        List_TrStr_free(parts);
        return _tr_str_lit_len("", 0LL);
    }
#line 2313 "src/bindgen.tr"
    TrStr r = _tr_str_lit_len("", 0LL);
#line 2314 "src/bindgen.tr"
    long long i = 0LL;
#line 2315 "src/bindgen.tr"
    while ((i < (parts->len - 1LL))) {
#line 2316 "src/bindgen.tr"
        if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 2316 "src/bindgen.tr"
            TrStr _strtmp_t3265 = List_TrStr_get(parts, i);
            _tr_str_release(r);
            r = _strtmp_t3265;
        } else {
#line 2317 "src/bindgen.tr"
            TrStr _strtmp_t3268 = ({ TrStr _tr_cl = (_tr_strx_concatv((r), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cr = (List_TrStr_get(parts, i)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(r);
            r = _strtmp_t3268;
        }
#line 2318 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2319 "src/bindgen.tr"
    TrStr _trret630 = _tr_str_retain(r);
    List_TrStr_free(parts);
    _tr_str_release(r);
    return _trret630;
}

__attribute__((hot)) TrStr _ns_us(TrStr path) {
#line 2323 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((path), (_tr_str_lit_len("::", 2LL)));
#line 2324 "src/bindgen.tr"
    TrStr r = _tr_str_lit_len("", 0LL);
#line 2325 "src/bindgen.tr"
    long long i = 0LL;
#line 2326 "src/bindgen.tr"
    while ((i < parts->len)) {
#line 2327 "src/bindgen.tr"
        if (({ TrStr _cmpa_t3269 = (List_TrStr_get(parts, i)); TrStr _cmpb_t3270 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3271 = !_tr_str_eqv(_cmpa_t3269, _cmpb_t3270); _tr_str_release(_cmpa_t3269); _cmpr_t3271; })) {
#line 2328 "src/bindgen.tr"
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 2328 "src/bindgen.tr"
                TrStr _strtmp_t3272 = List_TrStr_get(parts, i);
                _tr_str_release(r);
                r = _strtmp_t3272;
            } else {
#line 2329 "src/bindgen.tr"
                TrStr _strtmp_t3275 = ({ TrStr _tr_cl = (_tr_strx_concatv((r), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cr = (List_TrStr_get(parts, i)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
                _tr_str_release(r);
                r = _strtmp_t3275;
            }
        }
#line 2330 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2331 "src/bindgen.tr"
    TrStr _trret631 = _tr_str_retain(r);
    List_TrStr_free(parts);
    _tr_str_release(r);
    return _trret631;
}

__attribute__((hot)) TrStr _rstrip_cr(TrStr s) {
#line 2334 "src/bindgen.tr"
    long long n = _tr_str_lenv((s));
#line 2335 "src/bindgen.tr"
    while (((n > 0LL) && ({ TrStr _cmpa_t3276 = (_tr_str_slicev((s), (n - 1LL), n)); TrStr _cmpb_t3277 = (_tr_str_lit_len("\r", 1LL)); _Bool _cmpr_t3278 = _tr_str_eqv(_cmpa_t3276, _cmpb_t3277); _tr_str_release(_cmpa_t3276); _cmpr_t3278; }))) {
#line 2335 "src/bindgen.tr"
        n = (n - 1LL);
    }
#line 2336 "src/bindgen.tr"
    if ((n == _tr_str_lenv((s)))) {
#line 2336 "src/bindgen.tr"
        return _tr_str_retain(s);
    }
#line 2337 "src/bindgen.tr"
    return _tr_str_slicev((s), 0LL, n);
}

__attribute__((hot)) TrStr _c_to_cpp(TrStr cn) {
#line 2346 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("void", 4LL)))) {
#line 2346 "src/bindgen.tr"
        return _tr_str_lit_len("void", 4LL);
    }
#line 2347 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("bool", 4LL)))) {
#line 2347 "src/bindgen.tr"
        return _tr_str_lit_len("bool", 4LL);
    }
#line 2348 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int", 5LL)))) {
#line 2348 "src/bindgen.tr"
        return _tr_str_lit_len("int", 3LL);
    }
#line 2349 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint", 6LL)))) {
#line 2349 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned int", 12LL);
    }
#line 2350 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char", 6LL)))) {
#line 2350 "src/bindgen.tr"
        return _tr_str_lit_len("char", 4LL);
    }
#line 2351 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_schar", 7LL)))) {
#line 2351 "src/bindgen.tr"
        return _tr_str_lit_len("signed char", 11LL);
    }
#line 2352 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uchar", 7LL)))) {
#line 2352 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned char", 13LL);
    }
#line 2353 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_wchar", 7LL)))) {
#line 2353 "src/bindgen.tr"
        return _tr_str_lit_len("wchar_t", 7LL);
    }
#line 2354 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char16", 8LL)))) {
#line 2354 "src/bindgen.tr"
        return _tr_str_lit_len("char16_t", 8LL);
    }
#line 2355 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char32", 8LL)))) {
#line 2355 "src/bindgen.tr"
        return _tr_str_lit_len("char32_t", 8LL);
    }
#line 2356 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_short", 7LL)))) {
#line 2356 "src/bindgen.tr"
        return _tr_str_lit_len("short", 5LL);
    }
#line 2357 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ushort", 8LL)))) {
#line 2357 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned short", 14LL);
    }
#line 2358 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_long", 6LL)))) {
#line 2358 "src/bindgen.tr"
        return _tr_str_lit_len("long", 4LL);
    }
#line 2359 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ulong", 7LL)))) {
#line 2359 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned long", 13LL);
    }
#line 2360 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_longlong", 10LL)))) {
#line 2360 "src/bindgen.tr"
        return _tr_str_lit_len("long long", 9LL);
    }
#line 2361 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ulonglong", 11LL)))) {
#line 2361 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned long long", 18LL);
    }
#line 2362 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_float", 7LL)))) {
#line 2362 "src/bindgen.tr"
        return _tr_str_lit_len("float", 5LL);
    }
#line 2363 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_double", 8LL)))) {
#line 2363 "src/bindgen.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 2364 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ldouble", 9LL)))) {
#line 2364 "src/bindgen.tr"
        return _tr_str_lit_len("long double", 11LL);
    }
#line 2365 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_size_t", 8LL)))) {
#line 2365 "src/bindgen.tr"
        return _tr_str_lit_len("size_t", 6LL);
    }
#line 2366 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ssize_t", 9LL)))) {
#line 2366 "src/bindgen.tr"
        return _tr_str_lit_len("ptrdiff_t", 9LL);
    }
#line 2367 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ptrdiff_t", 11LL)))) {
#line 2367 "src/bindgen.tr"
        return _tr_str_lit_len("ptrdiff_t", 9LL);
    }
#line 2368 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_intptr_t", 10LL)))) {
#line 2368 "src/bindgen.tr"
        return _tr_str_lit_len("intptr_t", 8LL);
    }
#line 2369 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uintptr_t", 11LL)))) {
#line 2369 "src/bindgen.tr"
        return _tr_str_lit_len("uintptr_t", 9LL);
    }
#line 2370 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int8_t", 8LL)))) {
#line 2370 "src/bindgen.tr"
        return _tr_str_lit_len("signed char", 11LL);
    }
#line 2371 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint8_t", 9LL)))) {
#line 2371 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned char", 13LL);
    }
#line 2372 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int16_t", 9LL)))) {
#line 2372 "src/bindgen.tr"
        return _tr_str_lit_len("short", 5LL);
    }
#line 2373 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint16_t", 10LL)))) {
#line 2373 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned short", 14LL);
    }
#line 2374 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int32_t", 9LL)))) {
#line 2374 "src/bindgen.tr"
        return _tr_str_lit_len("int", 3LL);
    }
#line 2375 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint32_t", 10LL)))) {
#line 2375 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned int", 12LL);
    }
#line 2376 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int64_t", 9LL)))) {
#line 2376 "src/bindgen.tr"
        return _tr_str_lit_len("long long", 9LL);
    }
#line 2377 "src/bindgen.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint64_t", 10LL)))) {
#line 2377 "src/bindgen.tr"
        return _tr_str_lit_len("unsigned long long", 18LL);
    }
#line 2378 "src/bindgen.tr"
    return _tr_str_retain(cn);
}

__attribute__((hot)) List_TrStr* _desc4(TrStr desc) {
#line 2382 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((desc), (_tr_str_lit_len("~", 1LL)));
#line 2383 "src/bindgen.tr"
    List_TrStr* r = (void*)List_TrStr_new();
#line 2384 "src/bindgen.tr"
    if ((parts->len < 3LL)) {
#line 2385 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
#line 2385 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
#line 2385 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("u", 1LL));
#line 2385 "src/bindgen.tr"
        List_TrStr_append(r, desc);
#line 2386 "src/bindgen.tr"
        List_TrStr_free(parts);
        return r;
    }
#line 2387 "src/bindgen.tr"
    ({ TrStr _at_t3279 = (List_TrStr_get(parts, 0LL)); List_TrStr_append(r, _at_t3279); _tr_str_release(_at_t3279); });
#line 2388 "src/bindgen.tr"
    ({ TrStr _at_t3280 = (List_TrStr_get(parts, 1LL)); List_TrStr_append(r, _at_t3280); _tr_str_release(_at_t3280); });
#line 2389 "src/bindgen.tr"
    ({ TrStr _at_t3281 = (List_TrStr_get(parts, 2LL)); List_TrStr_append(r, _at_t3281); _tr_str_release(_at_t3281); });
#line 2390 "src/bindgen.tr"
    TrStr det = _tr_str_lit_len("", 0LL);
#line 2391 "src/bindgen.tr"
    if ((parts->len > 3LL)) {
#line 2391 "src/bindgen.tr"
        TrStr _strtmp_t3282 = List_TrStr_get(parts, 3LL);
        _tr_str_release(det);
        det = _strtmp_t3282;
    }
#line 2392 "src/bindgen.tr"
    long long k = 4LL;
#line 2393 "src/bindgen.tr"
    while ((k < parts->len)) {
#line 2394 "src/bindgen.tr"
        ({ TrStr _ap_t3283 = (_tr_str_lit_len("~", 1LL)); TrStr _ap_t3284 = (List_TrStr_get(parts, k)); _tr_str_append(&det, _ap_t3283); _tr_str_append(&det, _ap_t3284); _tr_str_release(_ap_t3284); });
#line 2395 "src/bindgen.tr"
        k = (k + 1LL);
    }
#line 2396 "src/bindgen.tr"
    List_TrStr_append(r, det);
#line 2397 "src/bindgen.tr"
    List_TrStr_free(parts);
    _tr_str_release(det);
    return r;
}

__attribute__((hot)) List_TrStr* _parse_tclass(TrStr rest) {
#line 2402 "src/bindgen.tr"
    List_TrStr* parts = _tr_str_splitv((rest), (_tr_str_lit_len(" ", 1LL)));
#line 2403 "src/bindgen.tr"
    List_TrStr* r = (void*)List_TrStr_new();
#line 2404 "src/bindgen.tr"
    if ((parts->len < 2LL)) {
#line 2405 "src/bindgen.tr"
        List_TrStr_append(r, rest);
#line 2405 "src/bindgen.tr"
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
#line 2405 "src/bindgen.tr"
        List_TrStr_free(parts);
        return r;
    }
#line 2406 "src/bindgen.tr"
    TrStr pod = List_TrStr_get(parts, (parts->len - 1LL));
#line 2407 "src/bindgen.tr"
    TrStr sp = _tr_str_lit_len("", 0LL);
#line 2408 "src/bindgen.tr"
    long long i = 0LL;
#line 2409 "src/bindgen.tr"
    while ((i < (parts->len - 1LL))) {
#line 2410 "src/bindgen.tr"
        if (_tr_str_eqv((sp), (_tr_str_lit_len("", 0LL)))) {
#line 2410 "src/bindgen.tr"
            TrStr _strtmp_t3285 = List_TrStr_get(parts, i);
            _tr_str_release(sp);
            sp = _strtmp_t3285;
        } else {
#line 2411 "src/bindgen.tr"
            ({ TrStr _ap_t3286 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3287 = (List_TrStr_get(parts, i)); _tr_str_append(&sp, _ap_t3286); _tr_str_append(&sp, _ap_t3287); _tr_str_release(_ap_t3287); });
        }
#line 2412 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2413 "src/bindgen.tr"
    List_TrStr_append(r, sp);
#line 2413 "src/bindgen.tr"
    List_TrStr_append(r, pod);
#line 2414 "src/bindgen.tr"
    List_TrStr_free(parts);
    _tr_str_release(pod);
    _tr_str_release(sp);
    return r;
}

__attribute__((hot)) TrStr _stars(long long n) {
#line 2418 "src/bindgen.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 2419 "src/bindgen.tr"
    long long i = 0LL;
#line 2420 "src/bindgen.tr"
    while ((i < n)) {
#line 2421 "src/bindgen.tr"
        TrStr _strtmp_t3289 = _tr_strx_concatv((s), (_tr_str_lit_len("*", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t3289;
#line 2422 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2423 "src/bindgen.tr"
    TrStr _trret632 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret632;
}

__attribute__((hot)) TrStr _ptr_wrap(TrStr inner, long long n) {
#line 2427 "src/bindgen.tr"
    TrStr t = _tr_str_retain(inner);
#line 2428 "src/bindgen.tr"
    long long i = 0LL;
#line 2429 "src/bindgen.tr"
    while ((i < n)) {
#line 2430 "src/bindgen.tr"
        TrStr _strtmp_t3290 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (t))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(t);
        t = _strtmp_t3290;
#line 2431 "src/bindgen.tr"
        i = (i + 1LL);
    }
#line 2432 "src/bindgen.tr"
    TrStr _trret633 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret633;
}

__attribute__((hot)) TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names) {
#line 2438 "src/bindgen.tr"
    List_TrStr* d4 = _desc4(desc);
#line 2439 "src/bindgen.tr"
    long long depth = ({ TrStr _at_t3291 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t3291)); _tr_str_release(_at_t3291); _wr; });
#line 2440 "src/bindgen.tr"
    TrStr cat = List_TrStr_get(d4, 2LL);
#line 2441 "src/bindgen.tr"
    TrStr detail = List_TrStr_get(d4, 3LL);
#line 2442 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("p", 1LL)))) {
#line 2443 "src/bindgen.tr"
        if ((depth == 0LL)) {
#line 2443 "src/bindgen.tr"
            TrStr _trret634 = _tr_str_retain(detail);
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            return _trret634;
        }
#line 2444 "src/bindgen.tr"
        TrStr _trret635 = _ptr_wrap(detail, depth);
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _trret635;
    }
#line 2445 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("e", 1LL)))) {
#line 2446 "src/bindgen.tr"
        TrStr ealias = ({ TrStr _at_t3292 = (List_TrStr_get(_tr_str_splitv((detail), (_tr_str_lit_len("#", 1LL))), 0LL)); __auto_type _wr = (_last_seg(_at_t3292)); _tr_str_release(_at_t3292); _wr; });
#line 2447 "src/bindgen.tr"
        if ((depth == 0LL)) {
#line 2447 "src/bindgen.tr"
            TrStr _trret636 = _tr_str_retain(ealias);
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(ealias);
            return _trret636;
        }
#line 2448 "src/bindgen.tr"
        TrStr _trret637 = _ptr_wrap(ealias, depth);
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(ealias);
        return _trret637;
    }
#line 2449 "src/bindgen.tr"
    if (_tr_str_eqv((cat), (_tr_str_lit_len("r", 1LL)))) {
#line 2450 "src/bindgen.tr"
        TrStr seg = _last_seg(detail);
#line 2451 "src/bindgen.tr"
        if ((depth == 0LL)) {
#line 2452 "src/bindgen.tr"
            if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
#line 2452 "src/bindgen.tr"
                TrStr _trret638 = _tr_str_retain(seg);
                List_TrStr_free(d4);
                _tr_str_release(cat);
                _tr_str_release(detail);
                _tr_str_release(seg);
                return _trret638;
            }
#line 2453 "src/bindgen.tr"
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(seg);
            return _tr_str_lit_len("Pointer[void]", 13LL);
        }
#line 2454 "src/bindgen.tr"
        TrStr _trret639 = _ptr_wrap(seg, depth);
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(seg);
        return _trret639;
    }
#line 2455 "src/bindgen.tr"
    if ((depth == 0LL)) {
#line 2455 "src/bindgen.tr"
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _tr_str_lit_len("Pointer[void]", 13LL);
    }
#line 2456 "src/bindgen.tr"
    TrStr _trret640 = _ptr_wrap(_tr_str_lit_len("void", 4LL), depth);
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    return _trret640;
}

__attribute__((hot)) TrStr _shim_body(TrStr ret_ctype, TrStr body) {
#line 2463 "src/bindgen.tr"
    TrStr zero = _tr_str_lit_len("return {};", 10LL);
#line 2464 "src/bindgen.tr"
    if (_tr_str_eqv((ret_ctype), (_tr_str_lit_len("void", 4LL)))) {
#line 2464 "src/bindgen.tr"
        TrStr _strtmp_t3293 = _tr_str_lit_len("return;", 7LL);
        _tr_str_release(zero);
        zero = _strtmp_t3293;
    }
#line 2465 "src/bindgen.tr"
    TrStr _trret641 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("try { ", 6LL)), (body))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" } catch (const std::exception& e) { _tr_cpp_set_error(e.what()); ", 66LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (zero)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" } catch (...) { _tr_cpp_set_error(\"C++ exception\"); ", 53LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (zero)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(zero);
    return _trret641;
}

__attribute__((hot)) TrStr _fnptr_cast(TrStr fnty) {
#line 2471 "src/bindgen.tr"
    long long p = _tr_str_index_ofv((fnty), (_tr_str_lit_len("(", 1LL)));
#line 2472 "src/bindgen.tr"
    if ((p < 0LL)) {
#line 2472 "src/bindgen.tr"
        return _tr_str_retain(fnty);
    }
#line 2473 "src/bindgen.tr"
    return ({ __auto_type _ord_t3294 = (({ TrStr _tr_cl = (_tr_str_slicev((fnty), 0LL, p)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(*)", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t3294); TrStr _tr_cr = (_tr_str_slicev((fnty), p, _tr_str_lenv((fnty)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
}

__attribute__((hot)) void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs) {
#line 2478 "src/bindgen.tr"
    StringBuilder* opaque = StringBuilder_init(512LL);
#line 2479 "src/bindgen.tr"
    StringBuilder* consts = StringBuilder_init(512LL);
#line 2480 "src/bindgen.tr"
    StringBuilder* decls = StringBuilder_init(2048LL);
#line 2481 "src/bindgen.tr"
    StringBuilder* shim = StringBuilder_init(2048LL);
#line 2482 "src/bindgen.tr"
    StringBuilder* usings = StringBuilder_init(256LL);
#line 2483 "src/bindgen.tr"
    TrMap* seen = _tr_dict_new(64LL);
#line 2485 "src/bindgen.tr"
    TrStr ns_path = _tr_str_lit_len("", 0LL);
#line 2486 "src/bindgen.tr"
    long long n_classes = 0LL;
#line 2487 "src/bindgen.tr"
    long long n_fns = 0LL;
#line 2488 "src/bindgen.tr"
    TrMap* sym_used = _tr_dict_new(64LL);
#line 2492 "src/bindgen.tr"
    List_TrStr* cstk_name = (void*)List_TrStr_new();
#line 2493 "src/bindgen.tr"
    List_TrStr* cstk_qual = (void*)List_TrStr_new();
#line 2494 "src/bindgen.tr"
    List_TrStr* cstk_pfx = (void*)List_TrStr_new();
#line 2497 "src/bindgen.tr"
    TrStr fld_buf = _tr_str_lit_len("", 0LL);
#line 2498 "src/bindgen.tr"
    List_TrStr* fld_stack = (void*)List_TrStr_new();
#line 2503 "src/bindgen.tr"
    TrMap* enum_names = _tr_dict_new(32LL);
#line 2504 "src/bindgen.tr"
    TrMap* class_names = _tr_dict_new(32LL);
#line 2505 "src/bindgen.tr"
    TrMap* value_structs = _tr_dict_new(32LL);
#line 2506 "src/bindgen.tr"
    TrMap* class_qual = _tr_dict_new(32LL);
#line 2507 "src/bindgen.tr"
    List_TrStr* ps_name = (void*)List_TrStr_new();
#line 2508 "src/bindgen.tr"
    List_TrStr* ps_pod = (void*)List_TrStr_new();
#line 2509 "src/bindgen.tr"
    List_TrStr* ps_nf = (void*)List_TrStr_new();
#line 2510 "src/bindgen.tr"
    List_TrStr* plines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
#line 2511 "src/bindgen.tr"
    long long pli = 0LL;
#line 2512 "src/bindgen.tr"
    while ((pli < plines->len)) {
#line 2513 "src/bindgen.tr"
        TrStr pl = ({ TrStr _at_t3295 = (List_TrStr_get(plines, pli)); __auto_type _wr = (_rstrip_cr(_at_t3295)); _tr_str_release(_at_t3295); _wr; });
#line 2514 "src/bindgen.tr"
        pli = (pli + 1LL);
#line 2515 "src/bindgen.tr"
        if (_tr_str_starts_withv((pl), (_tr_str_lit_len("ENUM ", 5LL)))) {
#line 2516 "src/bindgen.tr"
            TrStr erest = _tr_str_slicev((pl), 5LL, _tr_str_lenv((pl)));
#line 2517 "src/bindgen.tr"
            TrStr ename0 = List_TrStr_get(_tr_str_splitv((erest), (_tr_str_lit_len(" ", 1LL))), 0LL);
#line 2518 "src/bindgen.tr"
            if ((!_tr_str_eqv((ename0), (_tr_str_lit_len("anon", 4LL))))) {
#line 2518 "src/bindgen.tr"
                _tr_dict_set(enum_names, _tr_strz(ename0), true);
            }
            _tr_str_release(erest);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("CLASS ", 6LL)))) {
#line 2520 "src/bindgen.tr"
            TrStr crest = _tr_str_slicev((pl), 6LL, _tr_str_lenv((pl)));
#line 2521 "src/bindgen.tr"
            List_TrStr* cparts = _tr_str_splitv((crest), (_tr_str_lit_len(" ", 1LL)));
#line 2522 "src/bindgen.tr"
            TrStr cnm = List_TrStr_get(cparts, 0LL);
#line 2523 "src/bindgen.tr"
            TrStr cpod = _tr_str_lit_len("0", 1LL);
#line 2524 "src/bindgen.tr"
            if ((cparts->len > 1LL)) {
#line 2524 "src/bindgen.tr"
                TrStr _strtmp_t3296 = List_TrStr_get(cparts, 1LL);
                _tr_str_release(cpod);
                cpod = _strtmp_t3296;
            }
#line 2525 "src/bindgen.tr"
            _tr_dict_set(class_names, _tr_strz(cnm), true);
#line 2526 "src/bindgen.tr"
            List_TrStr_append(ps_name, cnm);
#line 2526 "src/bindgen.tr"
            List_TrStr_append(ps_pod, cpod);
#line 2526 "src/bindgen.tr"
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            _tr_str_release(crest);
            _tr_str_release(cnm);
            _tr_str_release(cpod);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("TCLASS ", 7LL)))) {
#line 2528 "src/bindgen.tr"
            List_TrStr* tp = ({ TrStr _at_t3297 = (_tr_str_slicev((pl), 7LL, _tr_str_lenv((pl)))); __auto_type _wr = (_parse_tclass(_at_t3297)); _tr_str_release(_at_t3297); _wr; });
#line 2529 "src/bindgen.tr"
            TrStr tnm = ({ TrStr _at_t3298 = (List_TrStr_get(tp, 0LL)); __auto_type _wr = (_cpp_ident(_at_t3298)); _tr_str_release(_at_t3298); _wr; });
#line 2530 "src/bindgen.tr"
            _tr_dict_set(class_names, _tr_strz(tnm), true);
#line 2531 "src/bindgen.tr"
            List_TrStr_append(ps_name, tnm);
#line 2531 "src/bindgen.tr"
            ({ TrStr _at_t3299 = (List_TrStr_get(tp, 1LL)); List_TrStr_append(ps_pod, _at_t3299); _tr_str_release(_at_t3299); });
#line 2531 "src/bindgen.tr"
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            List_TrStr_free(tp);
            _tr_str_release(tnm);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("XSTRUCT ", 8LL)))) {
#line 2534 "src/bindgen.tr"
            TrStr xn0 = _tr_str_slicev((pl), 8LL, _tr_str_lenv((pl)));
#line 2535 "src/bindgen.tr"
            _tr_dict_set(class_names, _tr_strz(xn0), true);
#line 2536 "src/bindgen.tr"
            List_TrStr_append(ps_name, xn0);
#line 2536 "src/bindgen.tr"
            List_TrStr_append(ps_pod, _tr_str_lit_len("1", 1LL));
#line 2536 "src/bindgen.tr"
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            _tr_str_release(xn0);
        } else if ((_tr_str_starts_withv((pl), (_tr_str_lit_len("FIELD ", 6LL))) || _tr_str_starts_withv((pl), (_tr_str_lit_len("SFIELD ", 7LL))))) {
#line 2538 "src/bindgen.tr"
            if ((ps_nf->len > 0LL)) {
#line 2539 "src/bindgen.tr"
                List_TrStr_pop(ps_nf);
#line 2540 "src/bindgen.tr"
                List_TrStr_append(ps_nf, _tr_str_lit_len("1", 1LL));
            }
        } else if ((_tr_str_eqv((pl), (_tr_str_lit_len("ECLASS", 6LL))) || _tr_str_eqv((pl), (_tr_str_lit_len("EXSTRUCT", 8LL))))) {
#line 2542 "src/bindgen.tr"
            if ((ps_name->len > 0LL)) {
#line 2543 "src/bindgen.tr"
                TrStr nm2 = List_TrStr_pop(ps_name);
#line 2544 "src/bindgen.tr"
                TrStr pod2 = List_TrStr_pop(ps_pod);
#line 2545 "src/bindgen.tr"
                TrStr nf2 = List_TrStr_pop(ps_nf);
#line 2546 "src/bindgen.tr"
                if ((_tr_str_eqv((pod2), (_tr_str_lit_len("1", 1LL))) && _tr_str_eqv((nf2), (_tr_str_lit_len("1", 1LL))))) {
#line 2546 "src/bindgen.tr"
                    _tr_dict_set(value_structs, _tr_strz(nm2), true);
                }
                _tr_str_release(pod2);
                _tr_str_release(nf2);
            }
        }
        _tr_str_release(pl);
    }
#line 2548 "src/bindgen.tr"
    TrStr mode = _tr_str_lit_len("", 0LL);
#line 2549 "src/bindgen.tr"
    TrStr m_flags = _tr_str_lit_len("..", 2LL);
#line 2550 "src/bindgen.tr"
    TrStr m_ret = _tr_str_lit_len("", 0LL);
#line 2551 "src/bindgen.tr"
    TrStr m_name = _tr_str_lit_len("", 0LL);
#line 2552 "src/bindgen.tr"
    TrStr m_qual = _tr_str_lit_len("", 0LL);
#line 2553 "src/bindgen.tr"
    long long m_ndef = 0LL;
#line 2554 "src/bindgen.tr"
    List_TrStr* params = (void*)List_TrStr_new();
#line 2556 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
#line 2557 "src/bindgen.tr"
    long long li = 0LL;
#line 2558 "src/bindgen.tr"
    while ((li < lines->len)) {
#line 2559 "src/bindgen.tr"
        TrStr line = ({ TrStr _at_t3300 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t3300)); _tr_str_release(_at_t3300); _wr; });
#line 2560 "src/bindgen.tr"
        li = (li + 1LL);
#line 2561 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("", 0LL)))) {
#line 2561 "src/bindgen.tr"
            _tr_str_release(line);
            continue;
        }
#line 2562 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("TDSPEC ", 7LL)))) {
#line 2562 "src/bindgen.tr"
            _tr_str_release(line);
            continue;
        }
#line 2565 "src/bindgen.tr"
        TrStr cur_class = _tr_str_lit_len("", 0LL);
#line 2566 "src/bindgen.tr"
        TrStr cur_class_qual = _tr_str_lit_len("", 0LL);
#line 2567 "src/bindgen.tr"
        TrStr class_pfx = _tr_str_lit_len("", 0LL);
#line 2568 "src/bindgen.tr"
        if ((cstk_name->len > 0LL)) {
#line 2569 "src/bindgen.tr"
            TrStr _strtmp_t3301 = List_TrStr_get(cstk_name, (cstk_name->len - 1LL));
            _tr_str_release(cur_class);
            cur_class = _strtmp_t3301;
#line 2570 "src/bindgen.tr"
            TrStr _strtmp_t3302 = List_TrStr_get(cstk_qual, (cstk_qual->len - 1LL));
            _tr_str_release(cur_class_qual);
            cur_class_qual = _strtmp_t3302;
#line 2571 "src/bindgen.tr"
            TrStr _strtmp_t3303 = List_TrStr_get(cstk_pfx, (cstk_pfx->len - 1LL));
            _tr_str_release(class_pfx);
            class_pfx = _strtmp_t3303;
        }
#line 2573 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("CTOR", 4LL)))) {
#line 2574 "src/bindgen.tr"
            m_ndef = 0LL;
#line 2575 "src/bindgen.tr"
            if ((_tr_str_lenv((line)) > 5LL)) {
#line 2575 "src/bindgen.tr"
                m_ndef = ({ TrStr _at_t3304 = (_tr_str_slicev((line), 5LL, _tr_str_lenv((line)))); __auto_type _wr = (_to_int(_at_t3304)); _tr_str_release(_at_t3304); _wr; });
            }
#line 2576 "src/bindgen.tr"
            TrStr _strtmp_t3305 = _tr_str_lit_len("ctor", 4LL);
            _tr_str_release(mode);
            mode = _strtmp_t3305;
#line 2576 "src/bindgen.tr"
            params = (void*)List_TrStr_new();
#line 2576 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2577 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("DTOR", 4LL)))) {
#line 2578 "src/bindgen.tr"
            if ((!_is_clean_ident(cur_class))) {
#line 2578 "src/bindgen.tr"
                _tr_str_release(line);
                _tr_str_release(cur_class);
                _tr_str_release(cur_class_qual);
                _tr_str_release(class_pfx);
                continue;
            }
#line 2579 "src/bindgen.tr"
            TrStr ds = ({ TrStr _at_t3306 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_delete", 7LL)))); __auto_type _wr = (_uniq_sym(_at_t3306, sym_used)); _tr_str_release(_at_t3306); _wr; });
#line 2580 "src/bindgen.tr"
            ({ TrStr _sbt_t3307 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), (ds))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class_qual)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("* self) { ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_shim_body(_tr_str_lit_len("void", 4LL), _tr_str_lit_len("delete self;", 12LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3307); _tr_str_release(_sbt_t3307); });
#line 2581 "src/bindgen.tr"
            ({ TrStr _sbt_t3308 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (ds))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(decls, _sbt_t3308); _tr_str_release(_sbt_t3308); });
#line 2582 "src/bindgen.tr"
            n_fns = (n_fns + 1LL);
#line 2583 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(ds);
            continue;
        }
#line 2584 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("ECLASS", 6LL)))) {
#line 2587 "src/bindgen.tr"
            if ((_is_clean_ident(cur_class) && ({ TrStr _dkt_t3309 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3309)))); _tr_str_release(_dkt_t3309); _wr; }))) {
#line 2588 "src/bindgen.tr"
                if ((_tr_dict_contains(value_structs, _tr_strz(cur_class)) && (!_tr_str_eqv((fld_buf), (_tr_str_lit_len("", 0LL)))))) {
#line 2589 "src/bindgen.tr"
                    ({ TrStr _sbt_t3310 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (cur_class))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fld_buf)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(opaque, _sbt_t3310); _tr_str_release(_sbt_t3310); });
                } else {
#line 2591 "src/bindgen.tr"
                    ({ TrStr _sbt_t3311 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (cur_class))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(opaque, _sbt_t3311); _tr_str_release(_sbt_t3311); });
                }
#line 2592 "src/bindgen.tr"
                ({ TrStr _dkt_t3312 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t3312), true); _tr_str_release(_dkt_t3312); });
#line 2593 "src/bindgen.tr"
                n_classes = (n_classes + 1LL);
            }
#line 2594 "src/bindgen.tr"
            if ((cstk_name->len > 0LL)) {
#line 2595 "src/bindgen.tr"
                List_TrStr_pop(cstk_name);
#line 2596 "src/bindgen.tr"
                List_TrStr_pop(cstk_qual);
#line 2597 "src/bindgen.tr"
                List_TrStr_pop(cstk_pfx);
            }
#line 2598 "src/bindgen.tr"
            if ((fld_stack->len > 0LL)) {
#line 2598 "src/bindgen.tr"
                TrStr _strtmp_t3313 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t3313;
            } else {
#line 2599 "src/bindgen.tr"
                TrStr _strtmp_t3314 = _tr_str_lit_len("", 0LL);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t3314;
            }
#line 2600 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2601 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FIELD ", 6LL)))) {
#line 2608 "src/bindgen.tr"
            TrStr frest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
#line 2609 "src/bindgen.tr"
            List_TrStr* fbar = _tr_str_splitv((frest), (_tr_str_lit_len("|", 1LL)));
#line 2610 "src/bindgen.tr"
            TrStr fdesc = List_TrStr_get(fbar, 0LL);
#line 2611 "src/bindgen.tr"
            TrStr fnm = _tr_str_lit_len("", 0LL);
#line 2612 "src/bindgen.tr"
            if ((fbar->len > 1LL)) {
#line 2612 "src/bindgen.tr"
                TrStr _strtmp_t3315 = List_TrStr_get(fbar, 1LL);
                _tr_str_release(fnm);
                fnm = _strtmp_t3315;
            }
#line 2613 "src/bindgen.tr"
            if (_is_clean_ident(fnm)) {
#line 2614 "src/bindgen.tr"
                if (_tr_dict_contains(value_structs, _tr_strz(cur_class))) {
#line 2615 "src/bindgen.tr"
                    TrStr fmap = _cpp_field_type(fdesc, value_structs, enum_names);
#line 2616 "src/bindgen.tr"
                    ({ TrStr _ap_t3316 = (_tr_str_lit_len("    pub ", 8LL)); TrStr _ap_t3317 = (_cpp_tr_pname(fnm)); TrStr _ap_t3318 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t3319 = (fmap); TrStr _ap_t3320 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&fld_buf, _ap_t3316); _tr_str_append(&fld_buf, _ap_t3317); _tr_str_append(&fld_buf, _ap_t3318); _tr_str_append(&fld_buf, _ap_t3319); _tr_str_append(&fld_buf, _ap_t3320); _tr_str_release(_ap_t3317); });
                    _tr_str_release(fmap);
                } else if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((fdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((fdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
#line 2620 "src/bindgen.tr"
                    List_TrStr* fri = ({ TrStr _at_t3321 = (_tr_strx_concatv((_tr_str_lit_len("self->", 6LL)), (fnm))); __auto_type _wr = (_cpp_ret_ex(fdesc, _at_t3321, value_structs, class_names, class_qual, seen, opaque)); _tr_str_release(_at_t3321); _wr; });
#line 2621 "src/bindgen.tr"
                    TrStr fsym = ({ TrStr _at_t3322 = (({ TrStr _tr_cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fnm)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_uniq_sym(_at_t3322, sym_used)); _tr_str_release(_at_t3322); _wr; });
#line 2622 "src/bindgen.tr"
                    ({ __auto_type _ord_t3325 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(fri, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fsym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class_qual)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("* self) { ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3323 = (List_TrStr_get(fri, 0LL)); TrStr _at_t3324 = (List_TrStr_get(fri, 1LL)); TrStr _sbt_t3326 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3325); TrStr _tr_cr = (_shim_body(_at_t3323, _at_t3324)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3326); _tr_str_release(_at_t3323); _tr_str_release(_at_t3324); _tr_str_release(_sbt_t3326); });
#line 2623 "src/bindgen.tr"
                    TrStr fd = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (fsym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2624 "src/bindgen.tr"
                    if (({ TrStr _cmpa_t3327 = (List_TrStr_get(fri, 2LL)); TrStr _cmpb_t3328 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3329 = !_tr_str_eqv(_cmpa_t3327, _cmpb_t3328); _tr_str_release(_cmpa_t3327); _cmpr_t3329; })) {
#line 2624 "src/bindgen.tr"
                        ({ TrStr _ap_t3330 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t3331 = (List_TrStr_get(fri, 2LL)); _tr_str_append(&fd, _ap_t3330); _tr_str_append(&fd, _ap_t3331); _tr_str_release(_ap_t3331); });
                    }
#line 2625 "src/bindgen.tr"
                    ({ TrStr _sbt_t3332 = (_tr_strx_concatv((fd), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t3332); _tr_str_release(_sbt_t3332); });
#line 2626 "src/bindgen.tr"
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(fsym);
                    _tr_str_release(fd);
                }
            }
#line 2627 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(frest);
            _tr_str_release(fdesc);
            _tr_str_release(fnm);
            continue;
        }
#line 2628 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("ITER ", 5LL)))) {
#line 2634 "src/bindgen.tr"
            TrStr idesc = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
#line 2635 "src/bindgen.tr"
            if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((idesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((idesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
#line 2636 "src/bindgen.tr"
                List_TrStr* iri = _cpp_ret_ex(idesc, _tr_str_lit_len("(*it)", 5LL), value_structs, class_names, class_qual, seen, opaque);
#line 2637 "src/bindgen.tr"
                TrStr isym = ({ TrStr _at_t3333 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_nth", 4LL)))); __auto_type _wr = (_uniq_sym(_at_t3333, sym_used)); _tr_str_release(_at_t3333); _wr; });
#line 2638 "src/bindgen.tr"
                ({ __auto_type _ord_t3336 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(iri, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (isym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class_qual)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("* self, long i) { ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3334 = (List_TrStr_get(iri, 0LL)); TrStr _at_t3335 = (({ TrStr _tr_cr = (List_TrStr_get(iri, 1LL)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("auto it = self->begin(); std::advance(it, (long)i); ", 52LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _sbt_t3337 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3336); TrStr _tr_cr = (_shim_body(_at_t3334, _at_t3335)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3337); _tr_str_release(_at_t3334); _tr_str_release(_at_t3335); _tr_str_release(_sbt_t3337); });
#line 2639 "src/bindgen.tr"
                TrStr id = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (isym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i: c_long)", 12LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2640 "src/bindgen.tr"
                if (({ TrStr _cmpa_t3338 = (List_TrStr_get(iri, 2LL)); TrStr _cmpb_t3339 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3340 = !_tr_str_eqv(_cmpa_t3338, _cmpb_t3339); _tr_str_release(_cmpa_t3338); _cmpr_t3340; })) {
#line 2640 "src/bindgen.tr"
                    ({ TrStr _ap_t3341 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t3342 = (List_TrStr_get(iri, 2LL)); _tr_str_append(&id, _ap_t3341); _tr_str_append(&id, _ap_t3342); _tr_str_release(_ap_t3342); });
                }
#line 2641 "src/bindgen.tr"
                ({ TrStr _sbt_t3343 = (_tr_strx_concatv((id), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t3343); _tr_str_release(_sbt_t3343); });
#line 2642 "src/bindgen.tr"
                n_fns = (n_fns + 1LL);
                _tr_str_release(isym);
                _tr_str_release(id);
            }
#line 2643 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(idesc);
            continue;
        }
#line 2644 "src/bindgen.tr"
        if ((_tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPKEY ", 11LL))) || _tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPVAL ", 11LL))))) {
#line 2648 "src/bindgen.tr"
            bool is_key = _tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPKEY ", 11LL)));
#line 2649 "src/bindgen.tr"
            TrStr mdesc = _tr_str_slicev((line), 11LL, _tr_str_lenv((line)));
#line 2650 "src/bindgen.tr"
            if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((mdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((mdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
#line 2651 "src/bindgen.tr"
                TrStr mfield = _tr_str_lit_len("(it->second)", 12LL);
#line 2652 "src/bindgen.tr"
                TrStr mname = _tr_str_lit_len("_val_nth", 8LL);
#line 2653 "src/bindgen.tr"
                if (is_key) {
#line 2654 "src/bindgen.tr"
                    TrStr _strtmp_t3344 = _tr_str_lit_len("(it->first)", 11LL);
                    _tr_str_release(mfield);
                    mfield = _strtmp_t3344;
#line 2655 "src/bindgen.tr"
                    TrStr _strtmp_t3345 = _tr_str_lit_len("_key_nth", 8LL);
                    _tr_str_release(mname);
                    mname = _strtmp_t3345;
                }
#line 2656 "src/bindgen.tr"
                List_TrStr* mri = _cpp_ret_ex(mdesc, mfield, value_structs, class_names, class_qual, seen, opaque);
#line 2657 "src/bindgen.tr"
                TrStr msym = ({ TrStr _at_t3346 = (_tr_strx_concatv((class_pfx), (mname))); __auto_type _wr = (_uniq_sym(_at_t3346, sym_used)); _tr_str_release(_at_t3346); _wr; });
#line 2658 "src/bindgen.tr"
                ({ __auto_type _ord_t3349 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(mri, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class_qual)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("* self, long i) { ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3347 = (List_TrStr_get(mri, 0LL)); TrStr _at_t3348 = (({ TrStr _tr_cr = (List_TrStr_get(mri, 1LL)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("auto it = self->begin(); std::advance(it, (long)i); ", 52LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _sbt_t3350 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3349); TrStr _tr_cr = (_shim_body(_at_t3347, _at_t3348)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3350); _tr_str_release(_at_t3347); _tr_str_release(_at_t3348); _tr_str_release(_sbt_t3350); });
#line 2659 "src/bindgen.tr"
                TrStr md = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (msym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i: c_long)", 12LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2660 "src/bindgen.tr"
                if (({ TrStr _cmpa_t3351 = (List_TrStr_get(mri, 2LL)); TrStr _cmpb_t3352 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3353 = !_tr_str_eqv(_cmpa_t3351, _cmpb_t3352); _tr_str_release(_cmpa_t3351); _cmpr_t3353; })) {
#line 2660 "src/bindgen.tr"
                    ({ TrStr _ap_t3354 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t3355 = (List_TrStr_get(mri, 2LL)); _tr_str_append(&md, _ap_t3354); _tr_str_append(&md, _ap_t3355); _tr_str_release(_ap_t3355); });
                }
#line 2661 "src/bindgen.tr"
                ({ TrStr _sbt_t3356 = (_tr_strx_concatv((md), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t3356); _tr_str_release(_sbt_t3356); });
#line 2662 "src/bindgen.tr"
                n_fns = (n_fns + 1LL);
                _tr_str_release(mfield);
                _tr_str_release(mname);
                _tr_str_release(msym);
                _tr_str_release(md);
            }
#line 2663 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(mdesc);
            continue;
        }
#line 2664 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("GVAR ", 5LL)))) {
#line 2668 "src/bindgen.tr"
            TrStr grest = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
#line 2669 "src/bindgen.tr"
            List_TrStr* gbar = _tr_str_splitv((grest), (_tr_str_lit_len("|", 1LL)));
#line 2670 "src/bindgen.tr"
            TrStr gvdesc = List_TrStr_get(gbar, 0LL);
#line 2671 "src/bindgen.tr"
            TrStr gvnm = _tr_str_lit_len("", 0LL);
#line 2672 "src/bindgen.tr"
            if ((gbar->len > 1LL)) {
#line 2672 "src/bindgen.tr"
                TrStr _strtmp_t3357 = List_TrStr_get(gbar, 1LL);
                _tr_str_release(gvnm);
                gvnm = _strtmp_t3357;
            }
#line 2673 "src/bindgen.tr"
            TrStr gcat = List_TrStr_get(_desc4(gvdesc), 2LL);
#line 2676 "src/bindgen.tr"
            if ((((((_is_clean_ident(gvnm) && (_tr_str_index_ofv((gvdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((gvdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL)) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("u", 1LL))))) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("f", 1LL))))) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("v", 1LL)))))) {
#line 2677 "src/bindgen.tr"
                TrStr gqual = _tr_str_retain(gvnm);
#line 2680 "src/bindgen.tr"
                TrStr gsym0 = _tr_strx_concatv((_tr_str_lit_len("g_", 2LL)), (gvnm));
#line 2681 "src/bindgen.tr"
                if ((!_tr_str_eqv((cur_class), (_tr_str_lit_len("", 0LL))))) {
#line 2682 "src/bindgen.tr"
                    TrStr _strtmp_t3358 = ({ TrStr _tr_cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gvnm)); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t3358;
#line 2683 "src/bindgen.tr"
                    TrStr _strtmp_t3359 = ({ TrStr _tr_cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gvnm)); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t3359;
                } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
#line 2685 "src/bindgen.tr"
                    TrStr _strtmp_t3360 = ({ TrStr _tr_cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gvnm)); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t3360;
#line 2686 "src/bindgen.tr"
                    TrStr _strtmp_t3361 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ns_us(ns_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gvnm)); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t3361;
                }
#line 2687 "src/bindgen.tr"
                List_TrStr* gvri = _cpp_ret_ex(gvdesc, gqual, value_structs, class_names, class_qual, seen, opaque);
#line 2688 "src/bindgen.tr"
                TrStr gvs = _uniq_sym(gsym0, sym_used);
#line 2689 "src/bindgen.tr"
                ({ __auto_type _ord_t3364 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(gvri, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gvs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("() { ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3362 = (List_TrStr_get(gvri, 0LL)); TrStr _at_t3363 = (List_TrStr_get(gvri, 1LL)); TrStr _sbt_t3365 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3364); TrStr _tr_cr = (_shim_body(_at_t3362, _at_t3363)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3365); _tr_str_release(_at_t3362); _tr_str_release(_at_t3363); _tr_str_release(_sbt_t3365); });
#line 2690 "src/bindgen.tr"
                TrStr gvd = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (gvs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2691 "src/bindgen.tr"
                if (({ TrStr _cmpa_t3366 = (List_TrStr_get(gvri, 2LL)); TrStr _cmpb_t3367 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3368 = !_tr_str_eqv(_cmpa_t3366, _cmpb_t3367); _tr_str_release(_cmpa_t3366); _cmpr_t3368; })) {
#line 2691 "src/bindgen.tr"
                    ({ TrStr _ap_t3369 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t3370 = (List_TrStr_get(gvri, 2LL)); _tr_str_append(&gvd, _ap_t3369); _tr_str_append(&gvd, _ap_t3370); _tr_str_release(_ap_t3370); });
                }
#line 2692 "src/bindgen.tr"
                ({ TrStr _sbt_t3371 = (_tr_strx_concatv((gvd), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t3371); _tr_str_release(_sbt_t3371); });
#line 2693 "src/bindgen.tr"
                n_fns = (n_fns + 1LL);
                _tr_str_release(gqual);
                _tr_str_release(gsym0);
                _tr_str_release(gvs);
                _tr_str_release(gvd);
            }
#line 2694 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(grest);
            _tr_str_release(gvdesc);
            _tr_str_release(gvnm);
            _tr_str_release(gcat);
            continue;
        }
#line 2695 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FTMPL ", 6LL)))) {
#line 2699 "src/bindgen.tr"
            TrStr ftrest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
#line 2700 "src/bindgen.tr"
            List_TrStr* ftbar = _tr_str_splitv((ftrest), (_tr_str_lit_len("|", 1LL)));
#line 2701 "src/bindgen.tr"
            long long ftnp = ({ TrStr _at_t3372 = (List_TrStr_get(ftbar, 0LL)); __auto_type _wr = (_to_int(_at_t3372)); _tr_str_release(_at_t3372); _wr; });
#line 2702 "src/bindgen.tr"
            TrStr ftnm = _tr_str_lit_len("", 0LL);
#line 2703 "src/bindgen.tr"
            if ((ftbar->len > 1LL)) {
#line 2703 "src/bindgen.tr"
                TrStr _strtmp_t3373 = List_TrStr_get(ftbar, 1LL);
                _tr_str_release(ftnm);
                ftnm = _strtmp_t3373;
            }
#line 2704 "src/bindgen.tr"
            if ((_is_clean_ident(ftnm) && (ftnp >= 0LL))) {
#line 2705 "src/bindgen.tr"
                List_TrStr* ftys = (void*)List_TrStr_new();
#line 2706 "src/bindgen.tr"
                List_TrStr_append(ftys, _tr_str_lit_len("int", 3LL));
#line 2707 "src/bindgen.tr"
                List_TrStr_append(ftys, _tr_str_lit_len("double", 6LL));
#line 2708 "src/bindgen.tr"
                List_TrStr* fctys = (void*)List_TrStr_new();
#line 2709 "src/bindgen.tr"
                List_TrStr_append(fctys, _tr_str_lit_len("c_int", 5LL));
#line 2710 "src/bindgen.tr"
                List_TrStr_append(fctys, _tr_str_lit_len("c_double", 8LL));
#line 2711 "src/bindgen.tr"
                long long fti = 0LL;
#line 2712 "src/bindgen.tr"
                while ((fti < ftys->len)) {
#line 2713 "src/bindgen.tr"
                    TrStr fcpp = List_TrStr_get(ftys, fti);
#line 2714 "src/bindgen.tr"
                    TrStr fcty = List_TrStr_get(fctys, fti);
#line 2715 "src/bindgen.tr"
                    fti = (fti + 1LL);
#line 2716 "src/bindgen.tr"
                    TrStr fshimp = _tr_str_lit_len("", 0LL);
#line 2717 "src/bindgen.tr"
                    TrStr ftrp = _tr_str_lit_len("", 0LL);
#line 2718 "src/bindgen.tr"
                    TrStr fcallargs = _tr_str_lit_len("", 0LL);
#line 2719 "src/bindgen.tr"
                    long long fpi = 0LL;
#line 2720 "src/bindgen.tr"
                    while ((fpi < ftnp)) {
#line 2721 "src/bindgen.tr"
                        TrStr fan = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(fpi)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 2722 "src/bindgen.tr"
                        fpi = (fpi + 1LL);
#line 2723 "src/bindgen.tr"
                        if ((!_tr_str_eqv((fshimp), (_tr_str_lit_len("", 0LL))))) {
#line 2723 "src/bindgen.tr"
                            ({ TrStr _ap_t3374 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&fshimp, _ap_t3374); });
                        }
#line 2724 "src/bindgen.tr"
                        ({ TrStr _ap_t3375 = (fcpp); TrStr _ap_t3376 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3377 = (fan); _tr_str_append(&fshimp, _ap_t3375); _tr_str_append(&fshimp, _ap_t3376); _tr_str_append(&fshimp, _ap_t3377); });
#line 2725 "src/bindgen.tr"
                        if ((!_tr_str_eqv((ftrp), (_tr_str_lit_len("", 0LL))))) {
#line 2725 "src/bindgen.tr"
                            ({ TrStr _ap_t3378 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ftrp, _ap_t3378); });
                        }
#line 2726 "src/bindgen.tr"
                        ({ TrStr _ap_t3379 = (fan); TrStr _ap_t3380 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t3381 = (fcty); _tr_str_append(&ftrp, _ap_t3379); _tr_str_append(&ftrp, _ap_t3380); _tr_str_append(&ftrp, _ap_t3381); });
#line 2727 "src/bindgen.tr"
                        if ((!_tr_str_eqv((fcallargs), (_tr_str_lit_len("", 0LL))))) {
#line 2727 "src/bindgen.tr"
                            ({ TrStr _ap_t3382 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&fcallargs, _ap_t3382); });
                        }
#line 2728 "src/bindgen.tr"
                        ({ TrStr _ap_t3383 = (fan); _tr_str_append(&fcallargs, _ap_t3383); });
                        _tr_str_release(fan);
                    }
#line 2729 "src/bindgen.tr"
                    TrStr fqual = _tr_str_retain(ftnm);
#line 2730 "src/bindgen.tr"
                    TrStr ftsym0 = ({ TrStr _tr_cl = (_tr_strx_concatv((ftnm), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fcpp)); _tr_str_release(_tr_cl); _tr_cres; });
#line 2731 "src/bindgen.tr"
                    if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
#line 2732 "src/bindgen.tr"
                        TrStr _strtmp_t3384 = ({ TrStr _tr_cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ftnm)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(fqual);
                        fqual = _strtmp_t3384;
#line 2733 "src/bindgen.tr"
                        TrStr _strtmp_t3385 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ns_us(ns_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ftnm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fcpp)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(ftsym0);
                        ftsym0 = _strtmp_t3385;
                    }
#line 2734 "src/bindgen.tr"
                    TrStr ftsym = _uniq_sym(ftsym0, sym_used);
#line 2735 "src/bindgen.tr"
                    TrStr fcall = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((fqual), (_tr_str_lit_len("<", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fcpp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(">(", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fcallargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2736 "src/bindgen.tr"
                    ({ TrStr _at_t3386 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (fcall))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t3387 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((fcpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ftsym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fshimp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_shim_body(fcpp, _at_t3386)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3387); _tr_str_release(_at_t3386); _tr_str_release(_sbt_t3387); });
#line 2737 "src/bindgen.tr"
                    ({ TrStr _sbt_t3388 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (ftsym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ftrp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") -> ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fcty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(decls, _sbt_t3388); _tr_str_release(_sbt_t3388); });
#line 2738 "src/bindgen.tr"
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(fcpp);
                    _tr_str_release(fcty);
                    _tr_str_release(fshimp);
                    _tr_str_release(ftrp);
                    _tr_str_release(fcallargs);
                    _tr_str_release(fqual);
                    _tr_str_release(ftsym0);
                    _tr_str_release(ftsym);
                    _tr_str_release(fcall);
                }
            }
#line 2739 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(ftrest);
            _tr_str_release(ftnm);
            continue;
        }
#line 2740 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("XSTRUCT ", 8LL)))) {
#line 2743 "src/bindgen.tr"
            TrStr xn = _tr_str_slicev((line), 8LL, _tr_str_lenv((line)));
#line 2744 "src/bindgen.tr"
            List_TrStr_append(cstk_name, xn);
#line 2744 "src/bindgen.tr"
            List_TrStr_append(cstk_qual, xn);
#line 2744 "src/bindgen.tr"
            List_TrStr_append(cstk_pfx, xn);
#line 2745 "src/bindgen.tr"
            _tr_dict_set(class_qual, _tr_strz(xn), _tr_str_box(_tr_str_retain(xn)));
#line 2746 "src/bindgen.tr"
            List_TrStr_append(fld_stack, fld_buf);
#line 2747 "src/bindgen.tr"
            TrStr _strtmp_t3389 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t3389;
#line 2748 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(xn);
            continue;
        }
#line 2749 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("SFIELD ", 7LL)))) {
#line 2751 "src/bindgen.tr"
            TrStr srest = _tr_str_slicev((line), 7LL, _tr_str_lenv((line)));
#line 2752 "src/bindgen.tr"
            List_TrStr* sbar = _tr_str_splitv((srest), (_tr_str_lit_len("|", 1LL)));
#line 2753 "src/bindgen.tr"
            TrStr sty = List_TrStr_get(sbar, 0LL);
#line 2754 "src/bindgen.tr"
            TrStr snm = _tr_str_lit_len("", 0LL);
#line 2755 "src/bindgen.tr"
            if ((sbar->len > 1LL)) {
#line 2755 "src/bindgen.tr"
                TrStr _strtmp_t3390 = List_TrStr_get(sbar, 1LL);
                _tr_str_release(snm);
                snm = _strtmp_t3390;
            }
#line 2756 "src/bindgen.tr"
            if (_is_clean_ident(snm)) {
#line 2757 "src/bindgen.tr"
                ({ TrStr _ap_t3391 = (_tr_str_lit_len("    pub ", 8LL)); TrStr _ap_t3392 = (_cpp_tr_pname(snm)); TrStr _ap_t3393 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t3394 = (sty); TrStr _ap_t3395 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&fld_buf, _ap_t3391); _tr_str_append(&fld_buf, _ap_t3392); _tr_str_append(&fld_buf, _ap_t3393); _tr_str_append(&fld_buf, _ap_t3394); _tr_str_append(&fld_buf, _ap_t3395); _tr_str_release(_ap_t3392); });
            }
#line 2758 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(srest);
            _tr_str_release(sty);
            _tr_str_release(snm);
            continue;
        }
#line 2759 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("EXSTRUCT", 8LL)))) {
#line 2760 "src/bindgen.tr"
            if ((_is_clean_ident(cur_class) && ({ TrStr _dkt_t3396 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3396)))); _tr_str_release(_dkt_t3396); _wr; }))) {
#line 2761 "src/bindgen.tr"
                if ((!_tr_str_eqv((fld_buf), (_tr_str_lit_len("", 0LL))))) {
#line 2761 "src/bindgen.tr"
                    ({ TrStr _sbt_t3397 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (cur_class))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fld_buf)); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(opaque, _sbt_t3397); _tr_str_release(_sbt_t3397); });
                } else {
#line 2762 "src/bindgen.tr"
                    ({ TrStr _sbt_t3398 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (cur_class))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(opaque, _sbt_t3398); _tr_str_release(_sbt_t3398); });
                }
#line 2763 "src/bindgen.tr"
                ({ TrStr _dkt_t3399 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t3399), true); _tr_str_release(_dkt_t3399); });
#line 2764 "src/bindgen.tr"
                n_classes = (n_classes + 1LL);
            }
#line 2765 "src/bindgen.tr"
            if ((cstk_name->len > 0LL)) {
#line 2766 "src/bindgen.tr"
                List_TrStr_pop(cstk_name);
#line 2767 "src/bindgen.tr"
                List_TrStr_pop(cstk_qual);
#line 2768 "src/bindgen.tr"
                List_TrStr_pop(cstk_pfx);
            }
#line 2769 "src/bindgen.tr"
            if ((fld_stack->len > 0LL)) {
#line 2769 "src/bindgen.tr"
                TrStr _strtmp_t3400 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t3400;
            } else {
#line 2770 "src/bindgen.tr"
                TrStr _strtmp_t3401 = _tr_str_lit_len("", 0LL);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t3401;
            }
#line 2771 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2772 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("ENS", 3LL)))) {
#line 2773 "src/bindgen.tr"
            TrStr _strtmp_t3402 = _ns_pop(ns_path);
            _tr_str_release(ns_path);
            ns_path = _strtmp_t3402;
#line 2773 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2774 "src/bindgen.tr"
        if (_tr_str_eqv((line), (_tr_str_lit_len("EENUM", 5LL)))) {
#line 2775 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2777 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("NS ", 3LL)))) {
#line 2778 "src/bindgen.tr"
            TrStr nn = _tr_str_slicev((line), 3LL, _tr_str_lenv((line)));
#line 2779 "src/bindgen.tr"
            if (_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL)))) {
#line 2779 "src/bindgen.tr"
                TrStr _strtmp_t3403 = _tr_str_retain(nn);
                _tr_str_release(ns_path);
                ns_path = _strtmp_t3403;
            } else {
#line 2780 "src/bindgen.tr"
                ({ TrStr _ap_t3404 = (_tr_str_lit_len("::", 2LL)); TrStr _ap_t3405 = (nn); _tr_str_append(&ns_path, _ap_t3404); _tr_str_append(&ns_path, _ap_t3405); });
            }
#line 2781 "src/bindgen.tr"
            if (({ TrStr _dkt_t3406 = (_tr_strx_concatv((_tr_str_lit_len("using:", 6LL)), (ns_path))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3406)))); _tr_str_release(_dkt_t3406); _wr; })) {
#line 2782 "src/bindgen.tr"
                ({ TrStr _sbt_t3407 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("using namespace ", 16LL)), (ns_path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(usings, _sbt_t3407); _tr_str_release(_sbt_t3407); });
#line 2783 "src/bindgen.tr"
                ({ TrStr _dkt_t3408 = (_tr_strx_concatv((_tr_str_lit_len("using:", 6LL)), (ns_path))); _tr_dict_set(seen, _tr_strz(_dkt_t3408), true); _tr_str_release(_dkt_t3408); });
            }
#line 2784 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(nn);
            continue;
        }
#line 2785 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("TCLASS ", 7LL)))) {
#line 2788 "src/bindgen.tr"
            List_TrStr* tp2 = ({ TrStr _at_t3409 = (_tr_str_slicev((line), 7LL, _tr_str_lenv((line)))); __auto_type _wr = (_parse_tclass(_at_t3409)); _tr_str_release(_at_t3409); _wr; });
#line 2789 "src/bindgen.tr"
            TrStr tspell = List_TrStr_get(tp2, 0LL);
#line 2790 "src/bindgen.tr"
            TrStr tname = _cpp_ident(tspell);
#line 2791 "src/bindgen.tr"
            List_TrStr_append(cstk_name, tname);
#line 2792 "src/bindgen.tr"
            List_TrStr_append(cstk_qual, tspell);
#line 2793 "src/bindgen.tr"
            List_TrStr_append(cstk_pfx, tname);
#line 2794 "src/bindgen.tr"
            _tr_dict_set(class_qual, _tr_strz(tname), _tr_str_box(_tr_str_retain(tspell)));
#line 2795 "src/bindgen.tr"
            List_TrStr_append(fld_stack, fld_buf);
#line 2796 "src/bindgen.tr"
            TrStr _strtmp_t3410 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t3410;
#line 2797 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(tspell);
            _tr_str_release(tname);
            continue;
        }
#line 2798 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("CLASS ", 6LL)))) {
#line 2799 "src/bindgen.tr"
            TrStr crest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
#line 2800 "src/bindgen.tr"
            TrStr ncls = List_TrStr_get(_tr_str_splitv((crest), (_tr_str_lit_len(" ", 1LL))), 0LL);
#line 2802 "src/bindgen.tr"
            TrStr nqual = _tr_str_retain(ncls);
#line 2803 "src/bindgen.tr"
            TrStr npfx = _tr_str_retain(ncls);
#line 2804 "src/bindgen.tr"
            if ((!_tr_str_eqv((cur_class_qual), (_tr_str_lit_len("", 0LL))))) {
#line 2804 "src/bindgen.tr"
                TrStr _strtmp_t3411 = ({ TrStr _tr_cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ncls)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t3411;
            } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
#line 2805 "src/bindgen.tr"
                TrStr _strtmp_t3412 = ({ TrStr _tr_cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ncls)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t3412;
            }
#line 2806 "src/bindgen.tr"
            if ((!_tr_str_eqv((class_pfx), (_tr_str_lit_len("", 0LL))))) {
#line 2806 "src/bindgen.tr"
                TrStr _strtmp_t3413 = ({ TrStr _tr_cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ncls)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t3413;
            } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
#line 2807 "src/bindgen.tr"
                TrStr _strtmp_t3414 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ns_us(ns_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ncls)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t3414;
            }
#line 2808 "src/bindgen.tr"
            List_TrStr_append(cstk_name, ncls);
#line 2809 "src/bindgen.tr"
            List_TrStr_append(cstk_qual, nqual);
#line 2810 "src/bindgen.tr"
            List_TrStr_append(cstk_pfx, npfx);
#line 2811 "src/bindgen.tr"
            _tr_dict_set(class_qual, _tr_strz(ncls), _tr_str_box(_tr_str_retain(nqual)));
#line 2813 "src/bindgen.tr"
            List_TrStr_append(fld_stack, fld_buf);
#line 2814 "src/bindgen.tr"
            TrStr _strtmp_t3415 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t3415;
#line 2815 "src/bindgen.tr"
            _tr_str_release(crest);
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(ncls);
            _tr_str_release(nqual);
            _tr_str_release(npfx);
            continue;
        }
#line 2816 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("ENUM ", 5LL)))) {
#line 2818 "src/bindgen.tr"
            TrStr erest = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
#line 2819 "src/bindgen.tr"
            List_TrStr* eps = _tr_str_splitv((erest), (_tr_str_lit_len(" ", 1LL)));
#line 2820 "src/bindgen.tr"
            TrStr en = List_TrStr_get(eps, 0LL);
#line 2821 "src/bindgen.tr"
            TrStr eunder = _tr_str_lit_len("c_int", 5LL);
#line 2822 "src/bindgen.tr"
            if ((eps->len > 1LL)) {
#line 2822 "src/bindgen.tr"
                TrStr _strtmp_t3416 = List_TrStr_get(eps, 1LL);
                _tr_str_release(eunder);
                eunder = _strtmp_t3416;
            }
#line 2823 "src/bindgen.tr"
            if ((_is_clean_ident(en) && ({ TrStr _dkt_t3417 = (_tr_strx_concatv((_tr_str_lit_len("enum:", 5LL)), (en))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3417)))); _tr_str_release(_dkt_t3417); _wr; }))) {
#line 2824 "src/bindgen.tr"
                ({ TrStr _sbt_t3418 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (en))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (eunder)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(consts, _sbt_t3418); _tr_str_release(_sbt_t3418); });
#line 2825 "src/bindgen.tr"
                ({ TrStr _dkt_t3419 = (_tr_strx_concatv((_tr_str_lit_len("enum:", 5LL)), (en))); _tr_dict_set(seen, _tr_strz(_dkt_t3419), true); _tr_str_release(_dkt_t3419); });
            }
#line 2826 "src/bindgen.tr"
            _tr_str_release(erest);
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(en);
            _tr_str_release(eunder);
            continue;
        }
#line 2827 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("EVAL ", 5LL)))) {
#line 2828 "src/bindgen.tr"
            TrStr er = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
#line 2829 "src/bindgen.tr"
            List_TrStr* ep = _tr_str_splitv((er), (_tr_str_lit_len(" ", 1LL)));
#line 2830 "src/bindgen.tr"
            TrStr ename = List_TrStr_get(ep, 0LL);
#line 2831 "src/bindgen.tr"
            TrStr eval_ = _tr_str_lit_len("0", 1LL);
#line 2832 "src/bindgen.tr"
            if ((ep->len > 1LL)) {
#line 2832 "src/bindgen.tr"
                TrStr _strtmp_t3420 = List_TrStr_get(ep, 1LL);
                _tr_str_release(eval_);
                eval_ = _strtmp_t3420;
            }
#line 2833 "src/bindgen.tr"
            if (({ TrStr _dkt_t3421 = (_tr_strx_concatv((_tr_str_lit_len("eval:", 5LL)), (ename))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t3421)))); _tr_str_release(_dkt_t3421); _wr; })) {
#line 2834 "src/bindgen.tr"
                ({ TrStr _sbt_t3422 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ename))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (eval_)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(consts, _sbt_t3422); _tr_str_release(_sbt_t3422); });
#line 2835 "src/bindgen.tr"
                ({ TrStr _dkt_t3423 = (_tr_strx_concatv((_tr_str_lit_len("eval:", 5LL)), (ename))); _tr_dict_set(seen, _tr_strz(_dkt_t3423), true); _tr_str_release(_dkt_t3423); });
            }
#line 2836 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(er);
            _tr_str_release(ename);
            _tr_str_release(eval_);
            continue;
        }
#line 2837 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("METHOD ", 7LL)))) {
#line 2838 "src/bindgen.tr"
            TrStr r = _tr_str_slicev((line), 7LL, _tr_str_lenv((line)));
#line 2839 "src/bindgen.tr"
            TrStr _strtmp_t3424 = _tr_str_slicev((r), 0LL, 2LL);
            _tr_str_release(m_flags);
            m_flags = _strtmp_t3424;
#line 2840 "src/bindgen.tr"
            TrStr r2 = _tr_str_slicev((r), 3LL, _tr_str_lenv((r)));
#line 2841 "src/bindgen.tr"
            long long sp1 = _tr_str_index_ofv((r2), (_tr_str_lit_len(" ", 1LL)));
#line 2842 "src/bindgen.tr"
            m_ndef = ({ TrStr _at_t3425 = (_tr_str_slicev((r2), 0LL, sp1)); __auto_type _wr = (_to_int(_at_t3425)); _tr_str_release(_at_t3425); _wr; });
#line 2843 "src/bindgen.tr"
            TrStr r3 = _tr_str_slicev((r2), (sp1 + 1LL), _tr_str_lenv((r2)));
#line 2844 "src/bindgen.tr"
            List_TrStr* rn = _tr_str_splitv((r3), (_tr_str_lit_len("|", 1LL)));
#line 2845 "src/bindgen.tr"
            TrStr _strtmp_t3426 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t3426;
#line 2846 "src/bindgen.tr"
            TrStr _strtmp_t3427 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t3427;
#line 2847 "src/bindgen.tr"
            TrStr _strtmp_t3428 = _tr_str_lit_len("", 0LL);
            _tr_str_release(m_qual);
            m_qual = _strtmp_t3428;
#line 2848 "src/bindgen.tr"
            if ((rn->len > 2LL)) {
#line 2848 "src/bindgen.tr"
                TrStr _strtmp_t3429 = List_TrStr_get(rn, 2LL);
                _tr_str_release(m_qual);
                m_qual = _strtmp_t3429;
            }
#line 2849 "src/bindgen.tr"
            TrStr _strtmp_t3430 = _tr_str_lit_len("method", 6LL);
            _tr_str_release(mode);
            mode = _strtmp_t3430;
#line 2849 "src/bindgen.tr"
            params = (void*)List_TrStr_new();
#line 2849 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(r);
            _tr_str_release(r2);
            _tr_str_release(r3);
            continue;
        }
#line 2850 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FUNC ", 5LL)))) {
#line 2851 "src/bindgen.tr"
            TrStr r = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
#line 2852 "src/bindgen.tr"
            long long sp1 = _tr_str_index_ofv((r), (_tr_str_lit_len(" ", 1LL)));
#line 2853 "src/bindgen.tr"
            m_ndef = ({ TrStr _at_t3431 = (_tr_str_slicev((r), 0LL, sp1)); __auto_type _wr = (_to_int(_at_t3431)); _tr_str_release(_at_t3431); _wr; });
#line 2854 "src/bindgen.tr"
            TrStr r3 = _tr_str_slicev((r), (sp1 + 1LL), _tr_str_lenv((r)));
#line 2855 "src/bindgen.tr"
            List_TrStr* rn = _tr_str_splitv((r3), (_tr_str_lit_len("|", 1LL)));
#line 2856 "src/bindgen.tr"
            TrStr _strtmp_t3432 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t3432;
#line 2857 "src/bindgen.tr"
            TrStr _strtmp_t3433 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t3433;
#line 2858 "src/bindgen.tr"
            TrStr _strtmp_t3434 = _tr_str_lit_len("", 0LL);
            _tr_str_release(m_qual);
            m_qual = _strtmp_t3434;
#line 2859 "src/bindgen.tr"
            TrStr _strtmp_t3435 = _tr_str_lit_len("..", 2LL);
            _tr_str_release(m_flags);
            m_flags = _strtmp_t3435;
#line 2860 "src/bindgen.tr"
            TrStr _strtmp_t3436 = _tr_str_lit_len("func", 4LL);
            _tr_str_release(mode);
            mode = _strtmp_t3436;
#line 2860 "src/bindgen.tr"
            params = (void*)List_TrStr_new();
#line 2860 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(r);
            _tr_str_release(r3);
            continue;
        }
#line 2861 "src/bindgen.tr"
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("PARAM ", 6LL)))) {
#line 2862 "src/bindgen.tr"
            ({ TrStr _at_t3437 = (_tr_str_slicev((line), 6LL, _tr_str_lenv((line)))); List_TrStr_append(params, _at_t3437); _tr_str_release(_at_t3437); });
#line 2863 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            continue;
        }
#line 2865 "src/bindgen.tr"
        if (((_tr_str_eqv((line), (_tr_str_lit_len("EMETHOD", 7LL))) || _tr_str_eqv((line), (_tr_str_lit_len("ECTOR", 5LL)))) || _tr_str_eqv((line), (_tr_str_lit_len("EFUNC", 5LL))))) {
#line 2866 "src/bindgen.tr"
            bool is_static = ({ TrStr _cmpa_t3438 = (_tr_str_slicev((m_flags), 0LL, 1LL)); TrStr _cmpb_t3439 = (_tr_str_lit_len("s", 1LL)); _Bool _cmpr_t3440 = _tr_str_eqv(_cmpa_t3438, _cmpb_t3439); _tr_str_release(_cmpa_t3438); _cmpr_t3440; });
#line 2867 "src/bindgen.tr"
            bool is_const = ({ TrStr _cmpa_t3441 = (_tr_str_slicev((m_flags), 1LL, 2LL)); TrStr _cmpb_t3442 = (_tr_str_lit_len("c", 1LL)); _Bool _cmpr_t3443 = _tr_str_eqv(_cmpa_t3441, _cmpb_t3442); _tr_str_release(_cmpa_t3441); _cmpr_t3443; });
#line 2872 "src/bindgen.tr"
            TrStr m_sym = _tr_str_retain(m_name);
#line 2873 "src/bindgen.tr"
            if ((((!_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) && (_tr_str_lenv((m_name)) >= 8LL)) && ({ TrStr _cmpa_t3444 = (_tr_str_slicev((m_name), 0LL, 8LL)); TrStr _cmpb_t3445 = (_tr_str_lit_len("operator", 8LL)); _Bool _cmpr_t3446 = _tr_str_eqv(_cmpa_t3444, _cmpb_t3445); _tr_str_release(_cmpa_t3444); _cmpr_t3446; }))) {
#line 2874 "src/bindgen.tr"
                TrStr _strtmp_t3447 = _cpp_op_name(m_name, params->len, _tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL))));
                _tr_str_release(m_sym);
                m_sym = _strtmp_t3447;
#line 2875 "src/bindgen.tr"
                if (_tr_str_eqv((m_sym), (_tr_str_lit_len("", 0LL)))) {
#line 2876 "src/bindgen.tr"
                    TrStr _strtmp_t3448 = _tr_str_lit_len("", 0LL);
                    _tr_str_release(mode);
                    mode = _strtmp_t3448;
#line 2876 "src/bindgen.tr"
                    _tr_str_release(line);
                    _tr_str_release(cur_class);
                    _tr_str_release(cur_class_qual);
                    _tr_str_release(class_pfx);
                    _tr_str_release(m_sym);
                    continue;
                }
            }
#line 2878 "src/bindgen.tr"
            if (((_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL))) || _tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) && (!_is_clean_ident(cur_class)))) {
#line 2879 "src/bindgen.tr"
                TrStr _strtmp_t3449 = _tr_str_lit_len("", 0LL);
                _tr_str_release(mode);
                mode = _strtmp_t3449;
#line 2879 "src/bindgen.tr"
                _tr_str_release(line);
                _tr_str_release(cur_class);
                _tr_str_release(cur_class_qual);
                _tr_str_release(class_pfx);
                _tr_str_release(m_sym);
                continue;
            }
#line 2884 "src/bindgen.tr"
            bool has_dep = ((_tr_str_index_ofv((m_ret), (_tr_str_lit_len("type-parameter", 14LL))) >= 0LL) || (_tr_str_index_ofv((m_ret), (_tr_str_lit_len("~d~", 3LL))) >= 0LL));
#line 2885 "src/bindgen.tr"
            long long pj = 0LL;
#line 2886 "src/bindgen.tr"
            while ((pj < params->len)) {
#line 2887 "src/bindgen.tr"
                TrStr pvd = List_TrStr_get(params, pj);
#line 2888 "src/bindgen.tr"
                if (((_tr_str_index_ofv((pvd), (_tr_str_lit_len("type-parameter", 14LL))) >= 0LL) || (_tr_str_index_ofv((pvd), (_tr_str_lit_len("~d~", 3LL))) >= 0LL))) {
#line 2888 "src/bindgen.tr"
                    has_dep = true;
                }
#line 2892 "src/bindgen.tr"
                if (((_tr_str_eqv((m_name), (_tr_str_lit_len("operator()", 10LL))) && _tr_str_starts_withv((pvd), (_tr_str_lit_len("0~", 2LL)))) && (_tr_str_index_ofv((pvd), (_tr_str_lit_len("~u~", 3LL))) >= 0LL))) {
#line 2892 "src/bindgen.tr"
                    has_dep = true;
                }
#line 2893 "src/bindgen.tr"
                pj = (pj + 1LL);
                _tr_str_release(pvd);
            }
#line 2894 "src/bindgen.tr"
            if (has_dep) {
#line 2895 "src/bindgen.tr"
                TrStr _strtmp_t3450 = _tr_str_lit_len("", 0LL);
                _tr_str_release(mode);
                mode = _strtmp_t3450;
#line 2895 "src/bindgen.tr"
                _tr_str_release(line);
                _tr_str_release(cur_class);
                _tr_str_release(cur_class_qual);
                _tr_str_release(class_pfx);
                _tr_str_release(m_sym);
                continue;
            }
#line 2898 "src/bindgen.tr"
            List_TrStr* full_params = (void*)List_TrStr_new();
#line 2899 "src/bindgen.tr"
            long long fpi = 0LL;
#line 2900 "src/bindgen.tr"
            while ((fpi < params->len)) {
#line 2901 "src/bindgen.tr"
                ({ TrStr _at_t3451 = (List_TrStr_get(params, fpi)); List_TrStr_append(full_params, _at_t3451); _tr_str_release(_at_t3451); });
#line 2902 "src/bindgen.tr"
                fpi = (fpi + 1LL);
            }
#line 2903 "src/bindgen.tr"
            long long min_arity = (full_params->len - m_ndef);
#line 2904 "src/bindgen.tr"
            if ((min_arity < 0LL)) {
#line 2904 "src/bindgen.tr"
                min_arity = 0LL;
            }
#line 2905 "src/bindgen.tr"
            long long arity = full_params->len;
#line 2906 "src/bindgen.tr"
            while ((arity >= min_arity)) {
#line 2907 "src/bindgen.tr"
                List_TrStr* tparams = (void*)List_TrStr_new();
#line 2908 "src/bindgen.tr"
                long long tti = 0LL;
#line 2909 "src/bindgen.tr"
                while ((tti < arity)) {
#line 2910 "src/bindgen.tr"
                    ({ TrStr _at_t3452 = (List_TrStr_get(full_params, tti)); List_TrStr_append(tparams, _at_t3452); _tr_str_release(_at_t3452); });
#line 2911 "src/bindgen.tr"
                    tti = (tti + 1LL);
                }
#line 2912 "src/bindgen.tr"
                params = tparams;
#line 2913 "src/bindgen.tr"
                TrStr shimp = _tr_str_lit_len("", 0LL);
#line 2914 "src/bindgen.tr"
                TrStr trp = _tr_str_lit_len("", 0LL);
#line 2915 "src/bindgen.tr"
                TrStr fargs = _tr_str_lit_len("", 0LL);
#line 2916 "src/bindgen.tr"
                if ((_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL))) && (!is_static))) {
#line 2917 "src/bindgen.tr"
                    if (is_const) {
#line 2917 "src/bindgen.tr"
                        TrStr _strtmp_t3453 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (cur_class_qual))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("* self", 6LL))); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(shimp);
                        shimp = _strtmp_t3453;
                    } else {
#line 2918 "src/bindgen.tr"
                        TrStr _strtmp_t3454 = _tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("* self", 6LL)));
                        _tr_str_release(shimp);
                        shimp = _strtmp_t3454;
                    }
#line 2919 "src/bindgen.tr"
                    TrStr _strtmp_t3455 = _tr_strx_concatv((_tr_str_lit_len("obj: ", 5LL)), (cur_class));
                    _tr_str_release(trp);
                    trp = _strtmp_t3455;
                }
#line 2920 "src/bindgen.tr"
                long long pi = 0LL;
#line 2921 "src/bindgen.tr"
                while ((pi < params->len)) {
#line 2922 "src/bindgen.tr"
                    TrStr pv = List_TrStr_get(params, pi);
#line 2923 "src/bindgen.tr"
                    pi = (pi + 1LL);
#line 2924 "src/bindgen.tr"
                    List_TrStr* bar = _tr_str_splitv((pv), (_tr_str_lit_len("|", 1LL)));
#line 2925 "src/bindgen.tr"
                    TrStr ptype = List_TrStr_get(bar, 0LL);
#line 2926 "src/bindgen.tr"
                    TrStr pname = _tr_str_lit_len("", 0LL);
#line 2927 "src/bindgen.tr"
                    if ((bar->len > 1LL)) {
#line 2927 "src/bindgen.tr"
                        TrStr _strtmp_t3456 = List_TrStr_get(bar, 1LL);
                        _tr_str_release(pname);
                        pname = _strtmp_t3456;
                    }
#line 2928 "src/bindgen.tr"
                    if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
#line 2928 "src/bindgen.tr"
                        TrStr _strtmp_t3457 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
                        _tr_str_release(pname);
                        pname = _strtmp_t3457;
                    }
#line 2929 "src/bindgen.tr"
                    TrStr spname = _tr_str_retain(pname);
#line 2930 "src/bindgen.tr"
                    if (_tr_str_eqv((spname), (_tr_str_lit_len("self", 4LL)))) {
#line 2930 "src/bindgen.tr"
                        TrStr _strtmp_t3458 = _tr_str_lit_len("self_", 5LL);
                        _tr_str_release(spname);
                        spname = _strtmp_t3458;
                    }
#line 2931 "src/bindgen.tr"
                    TrStr tpname = _cpp_tr_pname(pname);
#line 2933 "src/bindgen.tr"
                    List_TrStr* d4 = _desc4(ptype);
#line 2934 "src/bindgen.tr"
                    long long pdepth = ({ TrStr _at_t3459 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t3459)); _tr_str_release(_at_t3459); _wr; });
#line 2935 "src/bindgen.tr"
                    TrStr prefk = List_TrStr_get(d4, 1LL);
#line 2936 "src/bindgen.tr"
                    bool pref = (!_tr_str_eqv((prefk), (_tr_str_lit_len("0", 1LL))));
#line 2937 "src/bindgen.tr"
                    TrStr pcat = List_TrStr_get(d4, 2LL);
#line 2938 "src/bindgen.tr"
                    TrStr pdet = List_TrStr_get(d4, 3LL);
#line 2939 "src/bindgen.tr"
                    TrStr sc = _tr_str_lit_len("", 0LL);
#line 2940 "src/bindgen.tr"
                    TrStr fw = _tr_str_lit_len("", 0LL);
#line 2941 "src/bindgen.tr"
                    TrStr trt = _tr_str_lit_len("", 0LL);
#line 2942 "src/bindgen.tr"
                    if (_tr_str_eqv((pcat), (_tr_str_lit_len("p", 1LL)))) {
#line 2943 "src/bindgen.tr"
                        TrStr cpp = _c_to_cpp(pdet);
#line 2944 "src/bindgen.tr"
                        if ((pdepth == 0LL)) {
#line 2945 "src/bindgen.tr"
                            TrStr _strtmp_t3460 = ({ TrStr _tr_cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3460;
#line 2945 "src/bindgen.tr"
                            TrStr _strtmp_t3461 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3461;
#line 2945 "src/bindgen.tr"
                            TrStr _strtmp_t3462 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t3462;
                        } else if ((_tr_str_eqv((prefk), (_tr_str_lit_len("2", 1LL))) && (pdepth == 1LL))) {
#line 2949 "src/bindgen.tr"
                            TrStr _strtmp_t3463 = ({ TrStr _tr_cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3463;
#line 2949 "src/bindgen.tr"
                            TrStr _strtmp_t3464 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3464;
#line 2949 "src/bindgen.tr"
                            TrStr _strtmp_t3465 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t3465;
                        } else {
#line 2951 "src/bindgen.tr"
                            TrStr _strtmp_t3466 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (_stars(pdepth)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3466;
#line 2952 "src/bindgen.tr"
                            if (pref) {
#line 2952 "src/bindgen.tr"
                                TrStr _strtmp_t3467 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t3467;
                            } else {
#line 2953 "src/bindgen.tr"
                                TrStr _strtmp_t3468 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t3468;
                            }
#line 2954 "src/bindgen.tr"
                            TrStr _strtmp_t3469 = _ptr_wrap(pdet, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t3469;
                        }
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("e", 1LL)))) {
#line 2956 "src/bindgen.tr"
                        TrStr espell = List_TrStr_get(_tr_str_splitv((pdet), (_tr_str_lit_len("#", 1LL))), 0LL);
#line 2957 "src/bindgen.tr"
                        TrStr ealias = _last_seg(espell);
#line 2958 "src/bindgen.tr"
                        if ((pdepth == 0LL)) {
#line 2959 "src/bindgen.tr"
                            TrStr _strtmp_t3470 = ({ TrStr _tr_cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3470;
#line 2959 "src/bindgen.tr"
                            TrStr _strtmp_t3471 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3471;
#line 2959 "src/bindgen.tr"
                            TrStr _strtmp_t3472 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t3472;
                        } else if ((_tr_str_eqv((prefk), (_tr_str_lit_len("2", 1LL))) && (pdepth == 1LL))) {
#line 2961 "src/bindgen.tr"
                            TrStr _strtmp_t3473 = ({ TrStr _tr_cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3473;
#line 2961 "src/bindgen.tr"
                            TrStr _strtmp_t3474 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3474;
#line 2961 "src/bindgen.tr"
                            TrStr _strtmp_t3475 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t3475;
                        } else {
#line 2963 "src/bindgen.tr"
                            TrStr _strtmp_t3476 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (_stars(pdepth)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3476;
#line 2964 "src/bindgen.tr"
                            if (pref) {
#line 2964 "src/bindgen.tr"
                                TrStr _strtmp_t3477 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t3477;
                            } else {
#line 2965 "src/bindgen.tr"
                                TrStr _strtmp_t3478 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t3478;
                            }
#line 2966 "src/bindgen.tr"
                            TrStr _strtmp_t3479 = _ptr_wrap(ealias, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t3479;
                        }
                    } else if ((_tr_str_eqv((pcat), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((pdet), (_tr_str_lit_len("string", 6LL))))) {
#line 2969 "src/bindgen.tr"
                        TrStr _strtmp_t3480 = _tr_strx_concatv((_tr_str_lit_len("const char* ", 12LL)), (spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t3480;
#line 2970 "src/bindgen.tr"
                        TrStr _strtmp_t3481 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("std::string(", 12LL)), (spname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t3481;
#line 2971 "src/bindgen.tr"
                        TrStr _strtmp_t3482 = _tr_str_lit_len("Pointer[char]", 13LL);
                        _tr_str_release(trt);
                        trt = _strtmp_t3482;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("f", 1LL)))) {
#line 2976 "src/bindgen.tr"
                        TrStr _strtmp_t3483 = _tr_strx_concatv((_tr_str_lit_len("void* ", 6LL)), (spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t3483;
#line 2977 "src/bindgen.tr"
                        TrStr _strtmp_t3484 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_fnptr_cast(pdet)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t3484;
#line 2978 "src/bindgen.tr"
                        TrStr _strtmp_t3485 = _tr_str_lit_len("Pointer[void]", 13LL);
                        _tr_str_release(trt);
                        trt = _strtmp_t3485;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("v", 1LL)))) {
#line 2983 "src/bindgen.tr"
                        long long nd = pdepth;
#line 2984 "src/bindgen.tr"
                        if ((nd < 1LL)) {
#line 2984 "src/bindgen.tr"
                            nd = 1LL;
                        }
#line 2985 "src/bindgen.tr"
                        TrStr _strtmp_t3486 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_stars(nd)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t3486;
#line 2986 "src/bindgen.tr"
                        if (pref) {
#line 2986 "src/bindgen.tr"
                            TrStr _strtmp_t3487 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t3487;
                        } else {
#line 2987 "src/bindgen.tr"
                            TrStr _strtmp_t3488 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3488;
                        }
#line 2988 "src/bindgen.tr"
                        TrStr _strtmp_t3489 = _ptr_wrap(_tr_str_lit_len("void", 4LL), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t3489;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("r", 1LL)))) {
#line 2990 "src/bindgen.tr"
                        TrStr seg = _last_seg(pdet);
#line 2991 "src/bindgen.tr"
                        if ((_tr_dict_contains(value_structs, _tr_strz(seg)) && (pdepth == 0LL))) {
#line 2992 "src/bindgen.tr"
                            TrStr _strtmp_t3490 = ({ TrStr _tr_cl = (_tr_strx_concatv((pdet), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3490;
#line 2992 "src/bindgen.tr"
                            TrStr _strtmp_t3491 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3491;
#line 2992 "src/bindgen.tr"
                            TrStr _strtmp_t3492 = _tr_str_retain(seg);
                            _tr_str_release(trt);
                            trt = _strtmp_t3492;
                        } else {
#line 2994 "src/bindgen.tr"
                            long long nd = pdepth;
#line 2995 "src/bindgen.tr"
                            if ((nd < 1LL)) {
#line 2995 "src/bindgen.tr"
                                nd = 1LL;
                            }
#line 2996 "src/bindgen.tr"
                            TrStr _strtmp_t3494 = ({ __auto_type _ord_t3493 = (({ TrStr _tr_cl = (_cpp_qual(pdet, class_qual)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3493); TrStr _tr_cr = (_stars(nd)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
                            _tr_str_release(sc);
                            sc = _strtmp_t3494;
#line 2997 "src/bindgen.tr"
                            if ((pdepth == 0LL)) {
#line 2997 "src/bindgen.tr"
                                TrStr _strtmp_t3495 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t3495;
                            } else if (pref) {
#line 2998 "src/bindgen.tr"
                                TrStr _strtmp_t3496 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t3496;
                            } else {
#line 2999 "src/bindgen.tr"
                                TrStr _strtmp_t3497 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t3497;
                            }
#line 3000 "src/bindgen.tr"
                            TrStr _strtmp_t3498 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                            _tr_str_release(trt);
                            trt = _strtmp_t3498;
                        }
                    } else if (_tr_str_eqv((pdet), (_tr_str_lit_len("", 0LL)))) {
#line 3004 "src/bindgen.tr"
                        long long nd = pdepth;
#line 3005 "src/bindgen.tr"
                        if ((nd < 1LL)) {
#line 3005 "src/bindgen.tr"
                            nd = 1LL;
                        }
#line 3006 "src/bindgen.tr"
                        TrStr _strtmp_t3499 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_stars(nd)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t3499;
#line 3007 "src/bindgen.tr"
                        TrStr _strtmp_t3500 = _tr_str_retain(spname);
                        _tr_str_release(fw);
                        fw = _strtmp_t3500;
#line 3008 "src/bindgen.tr"
                        TrStr _strtmp_t3501 = _ptr_wrap(_tr_str_lit_len("void", 4LL), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t3501;
                    } else {
#line 3011 "src/bindgen.tr"
                        long long nd = pdepth;
#line 3012 "src/bindgen.tr"
                        if ((nd < 1LL)) {
#line 3012 "src/bindgen.tr"
                            nd = 1LL;
                        }
#line 3013 "src/bindgen.tr"
                        TrStr _strtmp_t3502 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((pdet), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (_stars(nd)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (spname)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t3502;
#line 3014 "src/bindgen.tr"
                        if ((pdepth == 0LL)) {
#line 3014 "src/bindgen.tr"
                            TrStr _strtmp_t3503 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t3503;
                        } else if (pref) {
#line 3015 "src/bindgen.tr"
                            TrStr _strtmp_t3504 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t3504;
                        } else {
#line 3016 "src/bindgen.tr"
                            TrStr _strtmp_t3505 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t3505;
                        }
#line 3017 "src/bindgen.tr"
                        TrStr _strtmp_t3506 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                        _tr_str_release(trt);
                        trt = _strtmp_t3506;
                    }
#line 3018 "src/bindgen.tr"
                    if (_tr_str_eqv((shimp), (_tr_str_lit_len("", 0LL)))) {
#line 3018 "src/bindgen.tr"
                        TrStr _strtmp_t3507 = _tr_str_retain(sc);
                        _tr_str_release(shimp);
                        shimp = _strtmp_t3507;
                    } else {
#line 3019 "src/bindgen.tr"
                        ({ TrStr _ap_t3508 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t3509 = (sc); _tr_str_append(&shimp, _ap_t3508); _tr_str_append(&shimp, _ap_t3509); });
                    }
#line 3020 "src/bindgen.tr"
                    if (_tr_str_eqv((fargs), (_tr_str_lit_len("", 0LL)))) {
#line 3020 "src/bindgen.tr"
                        TrStr _strtmp_t3510 = _tr_str_retain(fw);
                        _tr_str_release(fargs);
                        fargs = _strtmp_t3510;
                    } else {
#line 3021 "src/bindgen.tr"
                        ({ TrStr _ap_t3511 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t3512 = (fw); _tr_str_append(&fargs, _ap_t3511); _tr_str_append(&fargs, _ap_t3512); });
                    }
#line 3022 "src/bindgen.tr"
                    if (_tr_str_eqv((trp), (_tr_str_lit_len("", 0LL)))) {
#line 3022 "src/bindgen.tr"
                        TrStr _strtmp_t3513 = ({ TrStr _tr_cl = (_tr_strx_concatv((tpname), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (trt)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(trp);
                        trp = _strtmp_t3513;
                    } else {
#line 3023 "src/bindgen.tr"
                        ({ TrStr _ap_t3514 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t3515 = (tpname); TrStr _ap_t3516 = (_tr_str_lit_len(": ", 2LL)); TrStr _ap_t3517 = (trt); _tr_str_append(&trp, _ap_t3514); _tr_str_append(&trp, _ap_t3515); _tr_str_append(&trp, _ap_t3516); _tr_str_append(&trp, _ap_t3517); });
                    }
                    _tr_str_release(pv);
                    List_TrStr_free(bar);
                    _tr_str_release(ptype);
                    _tr_str_release(pname);
                    _tr_str_release(spname);
                    _tr_str_release(tpname);
                    List_TrStr_free(d4);
                    _tr_str_release(prefk);
                    _tr_str_release(pcat);
                    _tr_str_release(pdet);
                    _tr_str_release(sc);
                    _tr_str_release(fw);
                    _tr_str_release(trt);
                }
#line 3025 "src/bindgen.tr"
                TrStr call = _tr_str_lit_len("", 0LL);
#line 3026 "src/bindgen.tr"
                if (_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) {
#line 3027 "src/bindgen.tr"
                    TrStr _strtmp_t3518 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("new ", 4LL)), (cur_class_qual))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(call);
                    call = _strtmp_t3518;
                } else if (_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) {
#line 3032 "src/bindgen.tr"
                    TrStr mcall = _tr_str_retain(m_name);
#line 3033 "src/bindgen.tr"
                    if ((!_tr_str_eqv((m_qual), (_tr_str_lit_len("", 0LL))))) {
#line 3033 "src/bindgen.tr"
                        TrStr _strtmp_t3519 = ({ TrStr _tr_cl = (_tr_strx_concatv((m_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m_name)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(mcall);
                        mcall = _strtmp_t3519;
                    }
#line 3034 "src/bindgen.tr"
                    if (is_static) {
#line 3034 "src/bindgen.tr"
                        TrStr _strtmp_t3520 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(call);
                        call = _strtmp_t3520;
                    } else {
#line 3035 "src/bindgen.tr"
                        TrStr _strtmp_t3521 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("self->", 6LL)), (mcall))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(call);
                        call = _strtmp_t3521;
                    }
                } else {
#line 3037 "src/bindgen.tr"
                    TrStr pre = _tr_str_lit_len("", 0LL);
#line 3038 "src/bindgen.tr"
                    if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
#line 3038 "src/bindgen.tr"
                        TrStr _strtmp_t3522 = _tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)));
                        _tr_str_release(pre);
                        pre = _strtmp_t3522;
                    }
#line 3039 "src/bindgen.tr"
                    TrStr _strtmp_t3523 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((pre), (m_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fargs)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(call);
                    call = _strtmp_t3523;
                    _tr_str_release(pre);
                }
#line 3041 "src/bindgen.tr"
                if (_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) {
#line 3042 "src/bindgen.tr"
                    TrStr sym = ({ TrStr _at_t3524 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_new", 4LL)))); __auto_type _wr = (_uniq_sym(_at_t3524, sym_used)); _tr_str_release(_at_t3524); _wr; });
#line 3043 "src/bindgen.tr"
                    ({ TrStr _at_t3525 = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("*", 1LL)))); TrStr _at_t3526 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t3527 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("* ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (shimp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_shim_body(_at_t3525, _at_t3526)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3527); _tr_str_release(_at_t3525); _tr_str_release(_at_t3526); _tr_str_release(_sbt_t3527); });
#line 3044 "src/bindgen.tr"
                    ({ TrStr _sbt_t3528 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (sym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (trp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") -> ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cur_class)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(decls, _sbt_t3528); _tr_str_release(_sbt_t3528); });
#line 3045 "src/bindgen.tr"
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym);
                } else {
#line 3047 "src/bindgen.tr"
                    List_TrStr* ri = _cpp_ret_ex(m_ret, call, value_structs, class_names, class_qual, seen, opaque);
#line 3048 "src/bindgen.tr"
                    TrStr sym0 = _tr_str_lit_len("", 0LL);
#line 3049 "src/bindgen.tr"
                    if (_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) {
#line 3049 "src/bindgen.tr"
                        TrStr _strtmp_t3529 = ({ TrStr _tr_cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m_sym)); _tr_str_release(_tr_cl); _tr_cres; });
                        _tr_str_release(sym0);
                        sym0 = _strtmp_t3529;
                    } else {
#line 3055 "src/bindgen.tr"
                        if (_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL)))) {
#line 3055 "src/bindgen.tr"
                            TrStr _strtmp_t3530 = _tr_strx_concatv((_tr_str_lit_len("g_", 2LL)), (m_sym));
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t3530;
                        } else {
#line 3056 "src/bindgen.tr"
                            TrStr _strtmp_t3531 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ns_us(ns_path)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m_sym)); _tr_str_release(_tr_cl); _tr_cres; });
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t3531;
                        }
                    }
#line 3057 "src/bindgen.tr"
                    TrStr sym = _uniq_sym(sym0, sym_used);
#line 3058 "src/bindgen.tr"
                    ({ __auto_type _ord_t3534 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (List_TrStr_get(ri, 0LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sym)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (shimp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3532 = (List_TrStr_get(ri, 0LL)); TrStr _at_t3533 = (List_TrStr_get(ri, 1LL)); TrStr _sbt_t3535 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3534); TrStr _tr_cr = (_shim_body(_at_t3532, _at_t3533)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(shim, _sbt_t3535); _tr_str_release(_at_t3532); _tr_str_release(_at_t3533); _tr_str_release(_sbt_t3535); });
#line 3059 "src/bindgen.tr"
                    TrStr d = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (sym))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (trp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 3060 "src/bindgen.tr"
                    if (({ TrStr _cmpa_t3536 = (List_TrStr_get(ri, 2LL)); TrStr _cmpb_t3537 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3538 = !_tr_str_eqv(_cmpa_t3536, _cmpb_t3537); _tr_str_release(_cmpa_t3536); _cmpr_t3538; })) {
#line 3060 "src/bindgen.tr"
                        ({ TrStr _ap_t3539 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t3540 = (List_TrStr_get(ri, 2LL)); _tr_str_append(&d, _ap_t3539); _tr_str_append(&d, _ap_t3540); _tr_str_release(_ap_t3540); });
                    }
#line 3061 "src/bindgen.tr"
                    ({ TrStr _sbt_t3541 = (_tr_strx_concatv((d), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t3541); _tr_str_release(_sbt_t3541); });
#line 3062 "src/bindgen.tr"
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym0);
                    _tr_str_release(sym);
                    _tr_str_release(d);
                }
#line 3063 "src/bindgen.tr"
                arity = (arity - 1LL);
                _tr_str_release(shimp);
                _tr_str_release(trp);
                _tr_str_release(fargs);
                _tr_str_release(call);
            }
#line 3064 "src/bindgen.tr"
            TrStr _strtmp_t3542 = _tr_str_lit_len("", 0LL);
            _tr_str_release(mode);
            mode = _strtmp_t3542;
#line 3065 "src/bindgen.tr"
            _tr_str_release(line);
            _tr_str_release(cur_class);
            _tr_str_release(cur_class_qual);
            _tr_str_release(class_pfx);
            _tr_str_release(m_sym);
            continue;
        }
        _tr_str_release(line);
        _tr_str_release(cur_class);
        _tr_str_release(cur_class_qual);
        _tr_str_release(class_pfx);
    }
#line 3068 "src/bindgen.tr"
    TrStr base = _tr_str_retain(out);
#line 3069 "src/bindgen.tr"
    if (((_tr_str_lenv((base)) > 3LL) && ({ __auto_type _ord_t3543 = ((_tr_str_lenv((base)) - 3LL)); __auto_type _ord_t3544 = (_tr_str_lenv((base))); __auto_type _wr = (({ TrStr _cmpa_t3545 = (_tr_str_slicev((base), _ord_t3543, _ord_t3544)); TrStr _cmpb_t3546 = (_tr_str_lit_len(".tr", 3LL)); _Bool _cmpr_t3547 = _tr_str_eqv(_cmpa_t3545, _cmpb_t3546); _tr_str_release(_cmpa_t3545); _cmpr_t3547; })); _wr; }))) {
#line 3070 "src/bindgen.tr"
        TrStr _strtmp_t3548 = _tr_str_slicev((base), 0LL, (_tr_str_lenv((base)) - 3LL));
        _tr_str_release(base);
        base = _strtmp_t3548;
    }
#line 3071 "src/bindgen.tr"
    TrStr shim_name = _tr_strx_concatv((base), (_tr_str_lit_len("_shim.cpp", 9LL)));
#line 3073 "src/bindgen.tr"
    StringBuilder* sb = StringBuilder_init(4096LL);
#line 3074 "src/bindgen.tr"
    ({ TrStr _sbt_t3549 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# Auto-generated C++ FFI bindings for ", 38LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (tauraro-bindgen -h cpp).\n", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3549); _tr_str_release(_sbt_t3549); });
#line 3078 "src/bindgen.tr"
    ({ TrStr _sbt_t3550 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_basename(shim_name)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-shim: ", 20LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3550); _tr_str_release(_sbt_t3550); });
#line 3079 "src/bindgen.tr"
    StringBuilder_append(sb, _tr_str_lit_len("# tauraro-cpp-lib: stdc++\n", 26LL));
#line 3080 "src/bindgen.tr"
    if ((!_tr_str_eqv((pkglibs), (_tr_str_lit_len("", 0LL))))) {
#line 3080 "src/bindgen.tr"
        ({ TrStr _sbt_t3551 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-linkflags:", 24LL)), (pkglibs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3551); _tr_str_release(_sbt_t3551); });
    }
#line 3081 "src/bindgen.tr"
    if ((!_tr_str_eqv((shim_cflags), (_tr_str_lit_len("", 0LL))))) {
#line 3081 "src/bindgen.tr"
        ({ TrStr _sbt_t3552 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-cflags: ", 22LL)), (shim_cflags))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3552); _tr_str_release(_sbt_t3552); });
    }
#line 3082 "src/bindgen.tr"
    ({ TrStr _sbt_t3553 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("# (opt out with --no-auto-cpp, then: c++ -c ", 44LL)), (shim_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ; tauraroc app.tr --link ", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (base)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_shim.o -lstdc++)\n\n", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sb, _sbt_t3553); _tr_str_release(_sbt_t3553); });
#line 3083 "src/bindgen.tr"
    TrStr os_ = StringObj_as_str(StringBuilder_to_string(opaque));
#line 3084 "src/bindgen.tr"
    TrStr cs = StringObj_as_str(StringBuilder_to_string(consts));
#line 3085 "src/bindgen.tr"
    TrStr ds2 = StringObj_as_str(StringBuilder_to_string(decls));
#line 3090 "src/bindgen.tr"
    TrMap* defined_names = _tr_dict_new(64LL);
#line 3091 "src/bindgen.tr"
    List_TrStr* dlns = _tr_str_splitv((({ TrStr _tr_cl = (_tr_strx_concatv((os_), (_tr_str_lit_len("\n", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cs)); _tr_str_release(_tr_cl); _tr_cres; })), (_tr_str_lit_len("\n", 1LL)));
#line 3092 "src/bindgen.tr"
    long long dii = 0LL;
#line 3093 "src/bindgen.tr"
    while ((dii < dlns->len)) {
#line 3094 "src/bindgen.tr"
        TrStr dl = List_TrStr_get(dlns, dii);
#line 3095 "src/bindgen.tr"
        dii = (dii + 1LL);
#line 3096 "src/bindgen.tr"
        if (_tr_str_starts_withv((dl), (_tr_str_lit_len("class ", 6LL)))) {
#line 3097 "src/bindgen.tr"
            TrStr dn = _ident_at(dl, 6LL);
#line 3098 "src/bindgen.tr"
            if ((!_tr_str_eqv((dn), (_tr_str_lit_len("", 0LL))))) {
#line 3098 "src/bindgen.tr"
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        } else if (_tr_str_starts_withv((dl), (_tr_str_lit_len("type ", 5LL)))) {
#line 3100 "src/bindgen.tr"
            TrStr dn = _ident_at(dl, 5LL);
#line 3101 "src/bindgen.tr"
            if ((!_tr_str_eqv((dn), (_tr_str_lit_len("", 0LL))))) {
#line 3101 "src/bindgen.tr"
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        }
        _tr_str_release(dl);
    }
#line 3102 "src/bindgen.tr"
    TrStr fb = ({ TrStr _at_t3554 = (_tr_strx_concatv((os_), (ds2))); __auto_type _wr = (_opaque_fallbacks(_at_t3554, defined_names)); _tr_str_release(_at_t3554); _wr; });
#line 3103 "src/bindgen.tr"
    if ((!_tr_str_eqv((fb), (_tr_str_lit_len("", 0LL))))) {
#line 3103 "src/bindgen.tr"
        StringBuilder_append(sb, fb);
    }
#line 3104 "src/bindgen.tr"
    if ((!_tr_str_eqv((os_), (_tr_str_lit_len("", 0LL))))) {
#line 3104 "src/bindgen.tr"
        ({ TrStr _sbt_t3555 = (_tr_strx_concatv((os_), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t3555); _tr_str_release(_sbt_t3555); });
    }
#line 3105 "src/bindgen.tr"
    if ((!_tr_str_eqv((cs), (_tr_str_lit_len("", 0LL))))) {
#line 3105 "src/bindgen.tr"
        ({ TrStr _sbt_t3556 = (_tr_strx_concatv((cs), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t3556); _tr_str_release(_sbt_t3556); });
    }
#line 3106 "src/bindgen.tr"
    if ((!_tr_str_eqv((ds2), (_tr_str_lit_len("", 0LL))))) {
#line 3107 "src/bindgen.tr"
        StringBuilder_append(sb, _tr_str_lit_len("extern \"C\":\n", 12LL));
#line 3111 "src/bindgen.tr"
        StringBuilder_append(sb, _tr_str_lit_len("    def tauraro_cpp_last_error() -> Pointer[char]\n", 50LL));
#line 3112 "src/bindgen.tr"
        StringBuilder_append(sb, _tr_str_lit_len("    def tauraro_cpp_clear_error()\n", 34LL));
#line 3113 "src/bindgen.tr"
        StringBuilder_append(sb, ds2);
    }
#line 3114 "src/bindgen.tr"
    ({ TrStr _at_t3557 = (StringObj_as_str(StringBuilder_to_string(sb))); write_file(out, _at_t3557); _tr_str_release(_at_t3557); });
#line 3116 "src/bindgen.tr"
    StringBuilder* sh = StringBuilder_init(4096LL);
#line 3117 "src/bindgen.tr"
    ({ TrStr _sbt_t3558 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("// Auto-generated C++ -> C shim for ", 36LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" (tauraro-bindgen -h cpp).\n", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sh, _sbt_t3558); _tr_str_release(_sbt_t3558); });
#line 3118 "src/bindgen.tr"
    ({ TrStr _sbt_t3559 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("// Compile:  c++ -c ", 20LL)), (shim_name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sh, _sbt_t3559); _tr_str_release(_sbt_t3559); });
#line 3119 "src/bindgen.tr"
    ({ TrStr _sbt_t3560 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(sh, _sbt_t3560); _tr_str_release(_sbt_t3560); });
#line 3120 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("#include <string>\n#include <string_view>\n#include <cstring>\n#include <cstdlib>\n#include <iterator>\n", 99LL));
#line 3121 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("// std::string return -> heap char* copy (caller owns it; free with the runtime free).\n", 87LL));
#line 3122 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("static char* _tr_cpp_strdup(std::string_view s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.data(), s.size()); p[s.size()]=0; } return p; }\n", 151LL));
#line 3123 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("// Exception safety: a C++ exception must never cross `extern \"C\"` (it would std::terminate).\n", 94LL));
#line 3124 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("// Every wrapper catches, records the message here, and returns a zero value; the caller\n", 89LL));
#line 3125 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("// can read/clear it via tauraro_cpp_last_error()/tauraro_cpp_clear_error().\n", 77LL));
#line 3126 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("static thread_local std::string _tr_cpp_err;\n", 45LL));
#line 3127 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("static void _tr_cpp_set_error(const char* m){ _tr_cpp_err = m ? m : \"C++ exception\"; }\n", 87LL));
#line 3128 "src/bindgen.tr"
    ({ TrStr _sbt_t3561 = (StringObj_as_str(StringBuilder_to_string(usings))); StringBuilder_append(sh, _sbt_t3561); _tr_str_release(_sbt_t3561); });
#line 3129 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("extern \"C\" {\n", 13LL));
#line 3130 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("const char* tauraro_cpp_last_error(void){ return _tr_cpp_strdup(_tr_cpp_err); }\n", 80LL));
#line 3131 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("void tauraro_cpp_clear_error(void){ _tr_cpp_err.clear(); }\n", 59LL));
#line 3132 "src/bindgen.tr"
    ({ TrStr _sbt_t3562 = (StringObj_as_str(StringBuilder_to_string(shim))); StringBuilder_append(sh, _sbt_t3562); _tr_str_release(_sbt_t3562); });
#line 3133 "src/bindgen.tr"
    StringBuilder_append(sh, _tr_str_lit_len("}\n", 2LL));
#line 3134 "src/bindgen.tr"
    ({ TrStr _at_t3563 = (StringObj_as_str(StringBuilder_to_string(sh))); write_file(shim_name, _at_t3563); _tr_str_release(_at_t3563); });
#line 3136 "src/bindgen.tr"
    ({ __auto_type _ord_t3564 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("bindgen: wrote ", 15LL)), (out))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" + ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (shim_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" — ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_classes)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" classes, ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3564); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_fns)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" wrappers", 9LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); }); });
#line 3137 "src/bindgen.tr"
    ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: next:  import this module and build — tauraroc auto-compiles + links the shim (zero-cost).", 101LL))); printf("\n"); });
    _tr_str_release(ns_path);
    List_TrStr_free(cstk_name);
    List_TrStr_free(cstk_qual);
    List_TrStr_free(cstk_pfx);
    _tr_str_release(fld_buf);
    List_TrStr_free(fld_stack);
    List_TrStr_free(ps_name);
    List_TrStr_free(ps_pod);
    List_TrStr_free(ps_nf);
    List_TrStr_free(plines);
    _tr_str_release(mode);
    _tr_str_release(m_flags);
    _tr_str_release(m_ret);
    _tr_str_release(m_name);
    _tr_str_release(m_qual);
    List_TrStr_free(params);
    List_TrStr_free(lines);
    _tr_str_release(base);
    _tr_str_release(shim_name);
    _tr_str_release(os_);
    _tr_str_release(cs);
    _tr_str_release(ds2);
    List_TrStr_free(dlns);
    _tr_str_release(fb);
    StringBuilder__tr_fn_free(opaque);
    StringBuilder__tr_fn_free(consts);
    StringBuilder__tr_fn_free(decls);
    StringBuilder__tr_fn_free(shim);
    StringBuilder__tr_fn_free(usings);
    StringBuilder__tr_fn_free(sb);
    StringBuilder__tr_fn_free(sh);
}

__attribute__((hot)) TrStr _cpp_detect_include_dirs(TrStr cc) {
#line 3146 "src/bindgen.tr"
    write_file(_tr_str_lit_len("_cxx_empty.cpp", 14LL), _tr_str_lit_len("\n", 1LL));
#line 3147 "src/bindgen.tr"
    ({ TrStr _aet_t3565 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" -x c++ -E -v _cxx_empty.cpp > _cxx_pp.txt 2> _cxx_inc.txt", 58LL)))); _tr_system(_aet_t3565.data); _tr_str_release(_aet_t3565); });
#line 3148 "src/bindgen.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 3149 "src/bindgen.tr"
    if (file_exists(_tr_str_lit_len("_cxx_inc.txt", 12LL))) {
#line 3150 "src/bindgen.tr"
        TrStr txt = read_file(_tr_str_lit_len("_cxx_inc.txt", 12LL));
#line 3151 "src/bindgen.tr"
        List_TrStr* lines = _tr_str_splitv((txt), (_tr_str_lit_len("\n", 1LL)));
#line 3152 "src/bindgen.tr"
        bool collecting = false;
#line 3153 "src/bindgen.tr"
        long long li = 0LL;
#line 3154 "src/bindgen.tr"
        while ((li < lines->len)) {
#line 3155 "src/bindgen.tr"
            TrStr ln = ({ TrStr _at_t3566 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t3566)); _tr_str_release(_at_t3566); _wr; });
#line 3156 "src/bindgen.tr"
            li = (li + 1LL);
#line 3157 "src/bindgen.tr"
            if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("search starts here", 18LL))) >= 0LL)) {
#line 3158 "src/bindgen.tr"
                collecting = true;
#line 3159 "src/bindgen.tr"
                _tr_str_release(ln);
                continue;
            }
#line 3160 "src/bindgen.tr"
            if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("End of search list", 18LL))) >= 0LL)) {
#line 3161 "src/bindgen.tr"
                collecting = false;
#line 3162 "src/bindgen.tr"
                _tr_str_release(ln);
                continue;
            }
#line 3163 "src/bindgen.tr"
            if (collecting) {
#line 3164 "src/bindgen.tr"
                TrStr d = _lstrip(ln);
#line 3166 "src/bindgen.tr"
                long long fpos = _tr_str_index_ofv((d), (_tr_str_lit_len(" (framework directory)", 22LL)));
#line 3167 "src/bindgen.tr"
                if ((fpos >= 0LL)) {
#line 3167 "src/bindgen.tr"
                    TrStr _strtmp_t3567 = _tr_str_slicev((d), 0LL, fpos);
                    _tr_str_release(d);
                    d = _strtmp_t3567;
                }
#line 3168 "src/bindgen.tr"
                if ((!_tr_str_eqv((d), (_tr_str_lit_len("", 0LL))))) {
#line 3169 "src/bindgen.tr"
                    ({ TrStr _ap_t3568 = (_tr_str_lit_len(" -I\"", 4LL)); TrStr _ap_t3569 = (d); TrStr _ap_t3570 = (_tr_str_lit_len("\"", 1LL)); _tr_str_append(&out, _ap_t3568); _tr_str_append(&out, _ap_t3569); _tr_str_append(&out, _ap_t3570); });
                }
            }
            _tr_str_release(ln);
        }
        _tr_str_release(txt);
    }
#line 3170 "src/bindgen.tr"
    _rm_files(_tr_str_lit_len("_cxx_empty.cpp _cxx_pp.txt _cxx_inc.txt", 39LL));
#line 3171 "src/bindgen.tr"
    TrStr _trret642 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret642;
}

__attribute__((hot)) TrStr _cpp_std_flag(TrStr extra) {
#line 3175 "src/bindgen.tr"
    if ((_tr_str_index_ofv((extra), (_tr_str_lit_len("-std=", 5LL))) >= 0LL)) {
#line 3175 "src/bindgen.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 3176 "src/bindgen.tr"
    return _tr_str_lit_len("-std=c++17 ", 11LL);
}

__attribute__((hot)) long long _cpp_fatal_count(TrStr diag) {
#line 3180 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((diag), (_tr_str_lit_len("\n", 1LL)));
#line 3181 "src/bindgen.tr"
    long long li = 0LL;
#line 3182 "src/bindgen.tr"
    long long f = 0LL;
#line 3183 "src/bindgen.tr"
    while ((li < lines->len)) {
#line 3184 "src/bindgen.tr"
        TrStr ln = ({ TrStr _at_t3571 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t3571)); _tr_str_release(_at_t3571); _wr; });
#line 3185 "src/bindgen.tr"
        li = (li + 1LL);
#line 3186 "src/bindgen.tr"
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("DIAGERR ", 8LL))) == 0LL)) {
#line 3187 "src/bindgen.tr"
            List_TrStr* parts = _tr_str_splitv((ln), (_tr_str_lit_len(" ", 1LL)));
#line 3188 "src/bindgen.tr"
            if ((parts->len >= 3LL)) {
#line 3188 "src/bindgen.tr"
                f = ({ TrStr _at_t3572 = (List_TrStr_get(parts, 2LL)); __auto_type _wr = (_to_int(_at_t3572)); _tr_str_release(_at_t3572); _wr; });
            }
        }
        _tr_str_release(ln);
    }
#line 3189 "src/bindgen.tr"
    long long _trret643 = f;
    List_TrStr_free(lines);
    return _trret643;
}

__attribute__((hot)) void _cpp_print_diag(TrStr diag) {
#line 3193 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((diag), (_tr_str_lit_len("\n", 1LL)));
#line 3194 "src/bindgen.tr"
    long long li = 0LL;
#line 3195 "src/bindgen.tr"
    long long shown = 0LL;
#line 3196 "src/bindgen.tr"
    while ((li < lines->len)) {
#line 3197 "src/bindgen.tr"
        TrStr ln = ({ TrStr _at_t3573 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t3573)); _tr_str_release(_at_t3573); _wr; });
#line 3198 "src/bindgen.tr"
        li = (li + 1LL);
#line 3199 "src/bindgen.tr"
        if (((_tr_str_index_ofv((ln), (_tr_str_lit_len("DIAG ", 5LL))) == 0LL) && (shown < 12LL))) {
#line 3200 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cr = (_tr_str_slicev((ln), 5LL, _tr_str_lenv((ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("    ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); });
#line 3201 "src/bindgen.tr"
            shown = (shown + 1LL);
        }
        _tr_str_release(ln);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) bool _cpp_ir_is_empty(TrStr ir) {
#line 3205 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
#line 3206 "src/bindgen.tr"
    long long li = 0LL;
#line 3207 "src/bindgen.tr"
    while ((li < lines->len)) {
#line 3208 "src/bindgen.tr"
        if (({ TrStr _at_t3574 = (List_TrStr_get(lines, li)); __auto_type _wr = (({ TrStr _cmpa_t3575 = (_rstrip_cr(_at_t3574)); TrStr _cmpb_t3576 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3577 = !_tr_str_eqv(_cmpa_t3575, _cmpb_t3576); _tr_str_release(_cmpa_t3575); _cmpr_t3577; })); _tr_str_release(_at_t3574); _wr; })) {
#line 3208 "src/bindgen.tr"
            List_TrStr_free(lines);
            return false;
        }
#line 3209 "src/bindgen.tr"
        li = (li + 1LL);
    }
#line 3210 "src/bindgen.tr"
    List_TrStr_free(lines);
    return true;
}

__attribute__((hot)) void _cpp_cleanup() {
#line 3213 "src/bindgen.tr"
    _rm_files(_tr_str_lit_len("_cxxwalk.c _cxxwalk.exe _cxx.ir _cxx_err.txt _cxx_diag.txt _cxx_force.cpp _cxx_inst.ir", 86LL));
}

__attribute__((hot)) bool _is_expr_proxy_spec(TrStr s) {
#line 3228 "src/bindgen.tr"
    long long lt = _tr_str_index_ofv((s), (_tr_str_lit_len("<", 1LL)));
#line 3229 "src/bindgen.tr"
    if ((lt < 0LL)) {
#line 3229 "src/bindgen.tr"
        return false;
    }
#line 3230 "src/bindgen.tr"
    TrStr inner = _tr_str_slicev((s), (lt + 1LL), _tr_str_lenv((s)));
#line 3231 "src/bindgen.tr"
    if ((_tr_str_index_ofv((inner), (_tr_str_lit_len("<", 1LL))) < 0LL)) {
#line 3231 "src/bindgen.tr"
        _tr_str_release(inner);
        return false;
    }
#line 3232 "src/bindgen.tr"
    if (_tr_str_starts_withv((s), (_tr_str_lit_len("std::", 5LL)))) {
#line 3232 "src/bindgen.tr"
        _tr_str_release(inner);
        return false;
    }
#line 3233 "src/bindgen.tr"
    _tr_str_release(inner);
    return true;
}

__attribute__((hot)) List_TrStr* _collect_specs(TrStr ir) {
#line 3238 "src/bindgen.tr"
    List_TrStr* specs = (void*)List_TrStr_new();
#line 3239 "src/bindgen.tr"
    TrMap* seen = _tr_dict_new(16LL);
#line 3240 "src/bindgen.tr"
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
#line 3241 "src/bindgen.tr"
    long long i = 0LL;
#line 3242 "src/bindgen.tr"
    while ((i < lines->len)) {
#line 3243 "src/bindgen.tr"
        TrStr ln = ({ TrStr _at_t3578 = (List_TrStr_get(lines, i)); __auto_type _wr = (_rstrip_cr(_at_t3578)); _tr_str_release(_at_t3578); _wr; });
#line 3244 "src/bindgen.tr"
        i = (i + 1LL);
#line 3247 "src/bindgen.tr"
        if (_tr_str_starts_withv((ln), (_tr_str_lit_len("TDSPEC ", 7LL)))) {
#line 3248 "src/bindgen.tr"
            TrStr td = _tr_str_slicev((ln), 7LL, _tr_str_lenv((ln)));
#line 3249 "src/bindgen.tr"
            if ((((_tr_str_index_ofv((td), (_tr_str_lit_len("<", 1LL))) >= 0LL) && (!_is_expr_proxy_spec(td))) && (!_tr_dict_contains(seen, _tr_strz(td))))) {
#line 3250 "src/bindgen.tr"
                _tr_dict_set(seen, _tr_strz(td), true);
#line 3251 "src/bindgen.tr"
                List_TrStr_append(specs, td);
            }
#line 3252 "src/bindgen.tr"
            _tr_str_release(ln);
            _tr_str_release(td);
            continue;
        }
#line 3253 "src/bindgen.tr"
        long long p = _tr_str_index_ofv((ln), (_tr_str_lit_len("~r~", 3LL)));
#line 3254 "src/bindgen.tr"
        if ((p < 0LL)) {
#line 3254 "src/bindgen.tr"
            _tr_str_release(ln);
            continue;
        }
#line 3255 "src/bindgen.tr"
        TrStr rest = _tr_str_slicev((ln), (p + 3LL), _tr_str_lenv((ln)));
#line 3256 "src/bindgen.tr"
        TrStr detail = _tr_str_retain(rest);
#line 3257 "src/bindgen.tr"
        long long bar = _tr_str_index_ofv((rest), (_tr_str_lit_len("|", 1LL)));
#line 3258 "src/bindgen.tr"
        if ((bar >= 0LL)) {
#line 3258 "src/bindgen.tr"
            TrStr _strtmp_t3579 = _tr_str_slicev((rest), 0LL, bar);
            _tr_str_release(detail);
            detail = _strtmp_t3579;
        }
#line 3259 "src/bindgen.tr"
        if ((((_tr_str_index_ofv((detail), (_tr_str_lit_len("<", 1LL))) >= 0LL) && (!_is_expr_proxy_spec(detail))) && (!_tr_dict_contains(seen, _tr_strz(detail))))) {
#line 3260 "src/bindgen.tr"
            _tr_dict_set(seen, _tr_strz(detail), true);
#line 3261 "src/bindgen.tr"
            List_TrStr_append(specs, detail);
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(detail);
    }
#line 3262 "src/bindgen.tr"
    Dict_free(seen);
    List_TrStr_free(lines);
    return specs;
}

__attribute__((hot)) void run_bindgen_cpp(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs) {
#line 3265 "src/bindgen.tr"
    TrStr guide = _detect_libclang(cc);
#line 3266 "src/bindgen.tr"
    if ((!_tr_str_eqv((guide), (_tr_str_lit_len("", 0LL))))) {
#line 3267 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(guide)); printf("\n"); });
#line 3268 "src/bindgen.tr"
        _tr_str_release(guide);
        return;
    }
#line 3269 "src/bindgen.tr"
    ({ TrStr _at_t3580 = (_cxxwalk_src()); write_file(_tr_str_lit_len("_cxxwalk.c", 10LL), _at_t3580); _tr_str_release(_at_t3580); });
#line 3270 "src/bindgen.tr"
    long long rc = ({ TrStr _aet_t3581 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" _cxxwalk.c -o _cxxwalk.exe -lclang 2>_cxx_err.txt", 50LL)))); __auto_type _wr = (_tr_system(_aet_t3581.data)); _tr_str_release(_aet_t3581); _wr; });
#line 3271 "src/bindgen.tr"
    if (((rc != 0LL) || (!file_exists(_tr_str_lit_len("_cxxwalk.exe", 12LL))))) {
#line 3272 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: could not compile the libclang walker (see _cxx_err.txt)", 65LL))); printf("\n"); });
#line 3273 "src/bindgen.tr"
        _tr_str_release(guide);
        return;
    }
#line 3275 "src/bindgen.tr"
    TrStr clang_args = ({ __auto_type _ord_t3582 = (({ TrStr _tr_cr = (_cpp_std_flag(extra)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("-x c++ ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t3582); TrStr _tr_cr = (_cpp_detect_include_dirs(cc)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 3276 "src/bindgen.tr"
    if ((!_tr_str_eqv((extra), (_tr_str_lit_len("", 0LL))))) {
#line 3276 "src/bindgen.tr"
        ({ TrStr _ap_t3583 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3584 = (extra); _tr_str_append(&clang_args, _ap_t3583); _tr_str_append(&clang_args, _ap_t3584); });
    }
#line 3277 "src/bindgen.tr"
    ({ TrStr _aet_t3585 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_local_exe(_tr_str_lit_len("_cxxwalk", 8LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" \"", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (header)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (clang_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" > _cxx.ir 2> _cxx_diag.txt", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3585.data); _tr_str_release(_aet_t3585); });
#line 3278 "src/bindgen.tr"
    TrStr diag = _tr_str_lit_len("", 0LL);
#line 3279 "src/bindgen.tr"
    if (file_exists(_tr_str_lit_len("_cxx_diag.txt", 13LL))) {
#line 3279 "src/bindgen.tr"
        TrStr _strtmp_t3586 = read_file(_tr_str_lit_len("_cxx_diag.txt", 13LL));
        _tr_str_release(diag);
        diag = _strtmp_t3586;
    }
#line 3280 "src/bindgen.tr"
    long long nfatal = _cpp_fatal_count(diag);
#line 3281 "src/bindgen.tr"
    if (((!file_exists(_tr_str_lit_len("_cxx.ir", 7LL))) || ({ TrStr _at_t3587 = (read_file(_tr_str_lit_len("_cxx.ir", 7LL))); __auto_type _wr = (_cpp_ir_is_empty(_at_t3587)); _tr_str_release(_at_t3587); _wr; }))) {
#line 3282 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(_tr_strx_concatv((_tr_str_lit_len("bindgen: no bindable declarations found in ", 43LL)), (header)))); printf("\n"); });
#line 3283 "src/bindgen.tr"
        if ((_tr_str_index_ofv((diag), (_tr_str_lit_len("DIAG ", 5LL))) >= 0LL)) {
#line 3284 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: libclang could not fully parse the header:", 51LL))); printf("\n"); });
#line 3285 "src/bindgen.tr"
            _cpp_print_diag(diag);
#line 3286 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(_tr_str_lit_len("", 0LL))); printf("\n"); });
#line 3287 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  Most 'file not found' errors mean a missing include dir. Re-run with the", 74LL))); printf("\n"); });
#line 3288 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  library's include path(s) (and any required defines), e.g.:", 61LL))); printf("\n"); });
#line 3289 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("    tauraroc bindgen ", 21LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" -h cpp -I<include-dir> [-I<more>] [-D<MACRO>]", 46LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
        } else {
#line 3291 "src/bindgen.tr"
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  (the header parsed cleanly but declares no public classes/functions to bind.)", 79LL))); printf("\n"); });
        }
#line 3292 "src/bindgen.tr"
        _cpp_cleanup();
#line 3293 "src/bindgen.tr"
        _tr_str_release(guide);
        _tr_str_release(clang_args);
        _tr_str_release(diag);
        return;
    }
#line 3294 "src/bindgen.tr"
    TrStr ir = read_file(_tr_str_lit_len("_cxx.ir", 7LL));
#line 3298 "src/bindgen.tr"
    List_TrStr* specs = _collect_specs(ir);
#line 3299 "src/bindgen.tr"
    long long n_specs = 0LL;
#line 3300 "src/bindgen.tr"
    if ((specs->len > 0LL)) {
#line 3301 "src/bindgen.tr"
        StringBuilder* fsb = StringBuilder_init(512LL);
#line 3302 "src/bindgen.tr"
        ({ TrStr _sbt_t3588 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(fsb, _sbt_t3588); _tr_str_release(_sbt_t3588); });
#line 3303 "src/bindgen.tr"
        long long si = 0LL;
#line 3304 "src/bindgen.tr"
        while ((si < specs->len)) {
#line 3305 "src/bindgen.tr"
            ({ TrStr _sbt_t3589 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(specs, si)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("template class ", 15LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(fsb, _sbt_t3589); _tr_str_release(_sbt_t3589); });
#line 3306 "src/bindgen.tr"
            si = (si + 1LL);
        }
#line 3307 "src/bindgen.tr"
        ({ TrStr _at_t3590 = (StringObj_as_str(StringBuilder_to_string(fsb))); write_file(_tr_str_lit_len("_cxx_force.cpp", 14LL), _at_t3590); _tr_str_release(_at_t3590); });
#line 3308 "src/bindgen.tr"
        ({ TrStr _aet_t3591 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_local_exe(_tr_str_lit_len("_cxxwalk", 8LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" --inst _cxx_force.cpp ", 23LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (clang_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" > _cxx_inst.ir 2>> _cxx_diag.txt", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t3591.data); _tr_str_release(_aet_t3591); });
#line 3309 "src/bindgen.tr"
        if ((file_exists(_tr_str_lit_len("_cxx_inst.ir", 12LL)) && ({ TrStr _at_t3592 = (read_file(_tr_str_lit_len("_cxx_inst.ir", 12LL))); __auto_type _wr = ((!_cpp_ir_is_empty(_at_t3592))); _tr_str_release(_at_t3592); _wr; }))) {
#line 3310 "src/bindgen.tr"
            TrStr _strtmp_t3595 = ({ TrStr _tr_cl = (_tr_strx_concatv((ir), (_tr_str_lit_len("\n", 1LL)))); TrStr _tr_cr = (read_file(_tr_str_lit_len("_cxx_inst.ir", 12LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(ir);
            ir = _strtmp_t3595;
#line 3311 "src/bindgen.tr"
            n_specs = specs->len;
        }
        StringBuilder__tr_fn_free(fsb);
    }
#line 3315 "src/bindgen.tr"
    TrStr shim_cflags = _cpp_std_flag(extra);
#line 3316 "src/bindgen.tr"
    if ((!_tr_str_eqv((extra), (_tr_str_lit_len("", 0LL))))) {
#line 3316 "src/bindgen.tr"
        ({ TrStr _ap_t3596 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3597 = (extra); _tr_str_append(&shim_cflags, _ap_t3596); _tr_str_append(&shim_cflags, _ap_t3597); });
    }
#line 3317 "src/bindgen.tr"
    _cpp_generate(ir, header, out, shim_cflags, pkglibs);
#line 3318 "src/bindgen.tr"
    if ((n_specs > 0LL)) {
#line 3319 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_specs)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: instantiated ", 22LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" template specialization(s)", 27LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
    }
#line 3320 "src/bindgen.tr"
    if ((nfatal > 0LL)) {
#line 3321 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nfatal)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: WARNING — libclang reported ", 39LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" fatal error(s); bindings may be INCOMPLETE:", 44LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 3322 "src/bindgen.tr"
        _cpp_print_diag(diag);
#line 3323 "src/bindgen.tr"
        ({ printf("%s", _tr_strz(_tr_str_lit_len("  Add the missing include dir(s) with -I<dir> for a complete binding.", 69LL))); printf("\n"); });
    }
#line 3324 "src/bindgen.tr"
    _cpp_cleanup();
    _tr_str_release(guide);
    _tr_str_release(clang_args);
    _tr_str_release(diag);
    _tr_str_release(ir);
    List_TrStr_free(specs);
    _tr_str_release(shim_cflags);
}

