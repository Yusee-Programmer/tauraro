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
    /* pass */
    CTok* t = ((CTok*)_tr_obj_alloc(sizeof(CTok)));
    /* pass */
    t->kind = kind;
    /* pass */
    t->text = _tr_str_retain(text);
    /* pass */
    return t;
}

__attribute__((malloc,returns_nonnull,hot)) Bindgen* Bindgen_init(List_ptr* toks) {
    /* pass */
    Bindgen* b = ((Bindgen*)_tr_obj_alloc(sizeof(Bindgen)));
    /* pass */
    b->toks = toks;
    /* pass */
    b->pos = 0LL;
    /* pass */
    b->funcs = StringBuilder_init(1024LL);
    /* pass */
    b->structs = StringBuilder_init(1024LL);
    /* pass */
    b->types = StringBuilder_init(512LL);
    /* pass */
    b->consts = StringBuilder_init(512LL);
    /* pass */
    b->n_funcs = 0LL;
    /* pass */
    b->n_structs = 0LL;
    /* pass */
    b->seen = _tr_dict_new(64LL);
    /* pass */
    b->defined = (void*)List_TrStr_new();
    /* pass */
    b->skip_syms = _runtime_symbols();
    /* pass */
    b->n_skipped = 0LL;
    /* pass */
    return b;
}

__attribute__((hot)) bool Bindgen_fresh(Bindgen* self, TrStr name) {
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("", 0LL))) || _tr_dict_contains(self->seen, _tr_strz(name)))) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_dict_set(self->seen, _tr_strz(name), true);
    /* pass */
    List_TrStr_append(self->defined, name);
    /* pass */
    return true;
}

__attribute__((hot)) TrStr Bindgen_ct(Bindgen* self) {
    /* pass */
    return _tr_str_retain(((CTok*)List_ptr_get(self->toks, self->pos))->text);
}

__attribute__((hot)) long long Bindgen_ck(Bindgen* self) {
    /* pass */
    return ((CTok*)List_ptr_get(self->toks, self->pos))->kind;
}

__attribute__((hot)) void Bindgen_adv(Bindgen* self) {
    /* pass */
    if ((self->pos < (self->toks->len - 1LL))) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
}

__attribute__((hot)) bool Bindgen_is_punct(Bindgen* self, TrStr p) {
    /* pass */
    return ((Bindgen_ck(self) == 4LL) && _tr_str_eqv((Bindgen_ct(self)), (p)));
}

__attribute__((hot)) long long Bindgen_nk(Bindgen* self) {
    /* pass */
    if (((self->pos + 1LL) < self->toks->len)) {
        /* pass */
        return ((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->kind;
    }
    /* pass */
    return 5LL;
}

__attribute__((hot)) TrStr Bindgen_nt(Bindgen* self) {
    /* pass */
    if (((self->pos + 1LL) < self->toks->len)) {
        /* pass */
        return _tr_str_retain(((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->text);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void Bindgen_skip_balanced_parens(Bindgen* self) {
    /* pass */
    if ((!Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                return;
            }
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_struct_body(Bindgen* self) {
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
        /* pass */
        long long depth = 0LL;
        /* pass */
        while ((Bindgen_ck(self) != 5LL)) {
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
                /* pass */
                depth = (depth + 1LL);
            } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
                /* pass */
                depth = (depth - 1LL);
                /* pass */
                Bindgen_adv(self);
                /* pass */
                if ((depth == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                continue;
            }
            /* pass */
            Bindgen_adv(self);
        }
    }
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_to_semi(Bindgen* self) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
            /* pass */
            depth = (depth - 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL)) && (depth <= 0LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_braces(Bindgen* self) {
    /* pass */
    if ((!Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                return;
            }
            /* pass */
            continue;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_attributes(Bindgen* self) {
    /* pass */
    while (((Bindgen_ck(self) == 0LL) && (_tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("__attribute__", 13LL))) || _tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("__declspec", 10LL)))))) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        Bindgen_skip_balanced_parens(self);
    }
}

__attribute__((hot)) bool Bindgen_func_def_follows(Bindgen* self) {
    /* pass */
    long long save = self->pos;
    /* pass */
    Bindgen_skip_balanced_parens(self);
    /* pass */
    bool isdef = Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL));
    /* pass */
    self->pos = save;
    /* pass */
    return isdef;
}

__attribute__((hot)) List_TrStr* Bindgen_read_type_words(Bindgen* self) {
    /* pass */
    List_TrStr* words = (void*)List_TrStr_new();
    /* pass */
    long long stars = 0LL;
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr w = Bindgen_ct(self);
            /* pass */
            if (_is_ignored_word(w)) {
                /* pass */
                Bindgen_adv(self);
            } else if ((_tr_str_eqv((w), (_tr_str_lit_len("__declspec", 10LL))) || _tr_str_eqv((w), (_tr_str_lit_len("__attribute__", 13LL))))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                Bindgen_skip_balanced_parens(self);
            } else if (((_tr_str_eqv((w), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((w), (_tr_str_lit_len("union", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("enum", 4LL))))) {
                /* pass */
                Bindgen_adv(self);
            } else if (({ TrStr _at_t814 = (Bindgen_nt(self)); __auto_type _wr = (((((words->len >= 1LL) || (stars > 0LL)) && _is_decl_term(Bindgen_nk(self), _at_t814)) && (!_is_prim_type_word(w)))); _tr_str_release(_at_t814); _wr; })) {
                /* pass */
                going = false;
            } else {
                /* pass */
                List_TrStr_append(words, w);
                /* pass */
                Bindgen_adv(self);
            }
        } else if (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
            /* pass */
            stars = (stars + 1LL);
            /* pass */
            Bindgen_adv(self);
        } else {
            /* pass */
            going = false;
        }
    }
    /* pass */
    List_TrStr* res = (void*)List_TrStr_new();
    /* pass */
    ({ TrStr _at_t815 = (_join_words(words)); List_TrStr_append(res, _at_t815); _tr_str_release(_at_t815); });
    /* pass */
    ({ TrStr _at_t816 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); List_TrStr_append(res, _at_t816); _tr_str_release(_at_t816); });
    /* pass */
    return res;
}

__attribute__((hot)) void Bindgen_emit_func(Bindgen* self, TrStr ret_words, long long ret_stars, TrStr name) {
    /* pass */
    if (_tr_dict_contains(self->skip_syms, _tr_strz(name))) {
        /* pass */
        self->n_skipped = (self->n_skipped + 1LL);
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        return;
    }
    /* pass */
    ({ TrStr _sbt_t817 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->funcs, _sbt_t817); _tr_str_release(_sbt_t817); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    bool first = true;
    /* pass */
    long long argn = 0LL;
    /* pass */
    bool variadic = false;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long ploop_before = self->pos;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len(".", 1LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            variadic = true;
            /* pass */
            continue;
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            continue;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr pbase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long pstars = ({ TrStr _at_t818 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t818)); _tr_str_release(_at_t818); _wr; });
        /* pass */
        TrStr pname = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) && _tr_str_eqv((Bindgen_nt(self)), (_tr_str_lit_len("*", 1LL))))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t819 = Bindgen_ct(self);
                _tr_str_release(pname);
                pname = _strtmp_t819;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                TrStr _strtmp_t820 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("arg", 3LL)), _cr); _tr_str_release(_cr); _cres; });
                _tr_str_release(pname);
                pname = _strtmp_t820;
            }
            /* pass */
            if (_is_tr_keyword(pname)) {
                /* pass */
                TrStr _strtmp_t821 = _tr_strx_concatv((pname), (_tr_str_lit_len("_", 1LL)));
                _tr_str_release(pname);
                pname = _strtmp_t821;
            }
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append(self->funcs, _tr_str_lit_len(", ", 2LL));
            }
            /* pass */
            first = false;
            /* pass */
            ({ TrStr _sbt_t822 = (_tr_strx_concatv((pname), (_tr_str_lit_len(": Pointer[void]", 15LL)))); StringBuilder_append(self->funcs, _sbt_t822); _tr_str_release(_sbt_t822); });
            /* pass */
            argn = (argn + 1LL);
            /* pass */
            if ((self->pos == ploop_before)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t823 = Bindgen_ct(self);
            _tr_str_release(pname);
            pname = _strtmp_t823;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            pstars = (pstars + 1LL);
        }
        /* pass */
        if (((_tr_str_eqv((pbase), (_tr_str_lit_len("void", 4LL))) && (pstars == 0LL)) && _tr_str_eqv((pname), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t824 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("arg", 3LL)), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(pname);
            pname = _strtmp_t824;
        }
        /* pass */
        if (_is_tr_keyword(pname)) {
            /* pass */
            TrStr _strtmp_t825 = _tr_strx_concatv((pname), (_tr_str_lit_len("_", 1LL)));
            _tr_str_release(pname);
            pname = _strtmp_t825;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            StringBuilder_append(self->funcs, _tr_str_lit_len(", ", 2LL));
        }
        /* pass */
        first = false;
        /* pass */
        ({ TrStr _sbt_t826 = (({ TrStr _cl = (_tr_strx_concatv((pname), (_tr_str_lit_len(": ", 2LL)))); TrStr _cr = (map_type(pbase, pstars)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->funcs, _sbt_t826); _tr_str_release(_sbt_t826); });
        /* pass */
        argn = (argn + 1LL);
        /* pass */
        if ((self->pos == ploop_before)) {
            /* pass */
            Bindgen_adv(self);
        }
        List_TrStr_free(tw);
        _tr_str_release(pbase);
        _tr_str_release(pname);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (variadic) {
        /* pass */
        StringBuilder_append(self->funcs, _tr_str_lit_len(", args...", 9LL));
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit_len(")", 1LL));
    /* pass */
    TrStr rt = map_type(ret_words, ret_stars);
    /* pass */
    if ((!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL))))) {
        /* pass */
        ({ TrStr _sbt_t827 = (_tr_strx_concatv((_tr_str_lit_len(" -> ", 4LL)), (rt))); StringBuilder_append(self->funcs, _sbt_t827); _tr_str_release(_sbt_t827); });
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit_len("\n", 1LL));
    /* pass */
    self->n_funcs = (self->n_funcs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    _tr_str_release(rt);
}

__attribute__((hot)) void Bindgen_emit_struct(Bindgen* self, TrStr name, bool is_union) {
    /* pass */
    if ((!Bindgen_fresh(self, name))) {
        /* pass */
        Bindgen_skip_struct_body(self);
        /* pass */
        return;
    }
    /* pass */
    if (is_union) {
        /* pass */
        StringBuilder_append(self->structs, _tr_str_lit_len("@union\n", 7LL));
    }
    /* pass */
    ({ TrStr _sbt_t828 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t828); _tr_str_release(_sbt_t828); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long nfields = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long floop_before = self->pos;
        /* pass */
        if ((_tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("union", 5LL))))) {
            /* pass */
            long long nsave = self->pos;
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                List_TrStr* nnames = (void*)List_TrStr_new();
                /* pass */
                List_TrStr* ntypes = (void*)List_TrStr_new();
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    long long nb = self->pos;
                    /* pass */
                    if ((_tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((Bindgen_ct(self)), (_tr_str_lit_len("union", 5LL))))) {
                        /* pass */
                        Bindgen_adv(self);
                        /* pass */
                        if ((Bindgen_ck(self) == 0LL)) {
                            /* pass */
                            Bindgen_adv(self);
                        }
                        /* pass */
                        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
                            /* pass */
                            Bindgen_skip_struct_body(self);
                            /* pass */
                            if ((self->pos == nb)) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            continue;
                        }
                        /* pass */
                        self->pos = nb;
                    }
                    /* pass */
                    List_TrStr* ntw = Bindgen_read_type_words(self);
                    /* pass */
                    TrStr nbase = List_TrStr_get(ntw, 0LL);
                    /* pass */
                    long long nstars = ({ TrStr _at_t829 = (List_TrStr_get(ntw, 1LL)); __auto_type _wr = (_to_int(_at_t829)); _tr_str_release(_at_t829); _wr; });
                    /* pass */
                    bool nmore = true;
                    /* pass */
                    while (nmore) {
                        /* pass */
                        if ((Bindgen_ck(self) != 0LL)) {
                            /* pass */
                            break;
                        }
                        /* pass */
                        TrStr nnm = Bindgen_ct(self);
                        /* pass */
                        Bindgen_adv(self);
                        /* pass */
                        TrStr narr = _tr_str_lit_len("", 0LL);
                        /* pass */
                        bool nisarr = false;
                        /* pass */
                        while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
                            /* pass */
                            Bindgen_adv(self);
                            /* pass */
                            if ((Bindgen_ck(self) == 1LL)) {
                                /* pass */
                                TrStr _strtmp_t830 = Bindgen_ct(self);
                                _tr_str_release(narr);
                                narr = _strtmp_t830;
                            }
                            /* pass */
                            while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            nisarr = true;
                        }
                        /* pass */
                        List_TrStr_append(nnames, nnm);
                        /* pass */
                        if ((nisarr && (!_tr_str_eqv((narr), (_tr_str_lit_len("", 0LL)))))) {
                            /* pass */
                            ({ TrStr _at_t831 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (map_type(nbase, nstars)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (narr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(ntypes, _at_t831); _tr_str_release(_at_t831); });
                        } else {
                            /* pass */
                            ({ TrStr _at_t832 = (map_type(nbase, nstars)); List_TrStr_append(ntypes, _at_t832); _tr_str_release(_at_t832); });
                        }
                        /* pass */
                        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
                            /* pass */
                            Bindgen_adv(self);
                        } else {
                            /* pass */
                            nmore = false;
                        }
                        _tr_str_release(nnm);
                        _tr_str_release(narr);
                    }
                    /* pass */
                    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    /* pass */
                    if ((self->pos == nb)) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    List_TrStr_free(ntw);
                    _tr_str_release(nbase);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                TrStr pfx = _tr_str_lit_len("", 0LL);
                /* pass */
                if ((Bindgen_ck(self) == 0LL)) {
                    /* pass */
                    TrStr _strtmp_t833 = Bindgen_ct(self);
                    _tr_str_release(pfx);
                    pfx = _strtmp_t833;
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                long long nk = 0LL;
                /* pass */
                while ((nk < nnames->len)) {
                    /* pass */
                    TrStr nfn = List_TrStr_get(nnames, nk);
                    /* pass */
                    if ((!_tr_str_eqv((pfx), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t834 = ({ TrStr _cl = (_tr_strx_concatv((pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (nfn)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(nfn);
                        nfn = _strtmp_t834;
                    }
                    /* pass */
                    ({ TrStr _sbt_t835 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (nfn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(ntypes, nk)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t835); _tr_str_release(_sbt_t835); });
                    /* pass */
                    nfields = (nfields + 1LL);
                    /* pass */
                    nk = (nk + 1LL);
                    _tr_str_release(nfn);
                }
                /* pass */
                if ((self->pos == floop_before)) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                continue;
            }
            /* pass */
            self->pos = nsave;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr fbase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long fstars = ({ TrStr _at_t836 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t836)); _tr_str_release(_at_t836); _wr; });
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) && _tr_str_eqv((Bindgen_nt(self)), (_tr_str_lit_len("*", 1LL))))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            TrStr fpname = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t837 = Bindgen_ct(self);
                _tr_str_release(fpname);
                fpname = _strtmp_t837;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            while (((!Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            if ((!_tr_str_eqv((fpname), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                ({ TrStr _sbt_t838 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fpname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": Pointer[void]\n", 16LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t838); _tr_str_release(_sbt_t838); });
                /* pass */
                nfields = (nfields + 1LL);
            }
            /* pass */
            Bindgen_skip_attributes(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if ((self->pos == floop_before)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            continue;
        }
        /* pass */
        bool more = true;
        /* pass */
        bool first_decl = true;
        /* pass */
        while (more) {
            /* pass */
            long long dstars = fstars;
            /* pass */
            if ((!first_decl)) {
                /* pass */
                dstars = 0LL;
                /* pass */
                while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
                    /* pass */
                    dstars = (dstars + 1LL);
                    /* pass */
                    Bindgen_adv(self);
                }
            }
            /* pass */
            first_decl = false;
            /* pass */
            if ((Bindgen_ck(self) != 0LL)) {
                /* pass */
                break;
            }
            /* pass */
            TrStr fname = Bindgen_ct(self);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            bool is_arr = false;
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                TrStr arrn = _tr_str_lit_len("", 0LL);
                /* pass */
                if ((Bindgen_ck(self) == 1LL)) {
                    /* pass */
                    TrStr _strtmp_t839 = Bindgen_ct(self);
                    _tr_str_release(arrn);
                    arrn = _strtmp_t839;
                }
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if ((!_tr_str_eqv((arrn), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    ({ TrStr _sbt_t840 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": [", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, dstars)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("; ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t840); _tr_str_release(_sbt_t840); });
                    /* pass */
                    nfields = (nfields + 1LL);
                    /* pass */
                    is_arr = true;
                }
                _tr_str_release(arrn);
            }
            /* pass */
            if ((!is_arr)) {
                /* pass */
                ({ TrStr _sbt_t841 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    pub ", 8LL)), (fname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, dstars)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t841); _tr_str_release(_sbt_t841); });
                /* pass */
                nfields = (nfields + 1LL);
            }
            /* pass */
            Bindgen_skip_attributes(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            } else {
                /* pass */
                more = false;
            }
            _tr_str_release(fname);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((self->pos == floop_before)) {
            /* pass */
            Bindgen_adv(self);
        }
        List_TrStr_free(tw);
        _tr_str_release(fbase);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if ((nfields == 0LL)) {
        /* pass */
        StringBuilder_append(self->structs, _tr_str_lit_len("    pass\n", 9LL));
    }
    /* pass */
    StringBuilder_append(self->structs, _tr_str_lit_len("\n", 1LL));
    /* pass */
    self->n_structs = (self->n_structs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_enum_expr(Bindgen* self) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("[", 1LL)))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("]", 1LL)))) {
            /* pass */
            depth = (depth - 1LL);
        } else if (((depth <= 0LL) && (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))))) {
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_emit_enum(Bindgen* self, TrStr name) {
    /* pass */
    if (((!_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, name))) {
        /* pass */
        ({ TrStr _sbt_t842 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = c_int\n", 9LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t842); _tr_str_release(_sbt_t842); });
    }
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long next_val = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_skip_attributes(self);
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
            /* pass */
            break;
        }
        /* pass */
        if ((Bindgen_ck(self) != 0LL)) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            continue;
        }
        /* pass */
        TrStr ename = Bindgen_ct(self);
        /* pass */
        Bindgen_adv(self);
        /* pass */
        Bindgen_skip_attributes(self);
        /* pass */
        long long ev = next_val;
        /* pass */
        bool simple = true;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("=", 1LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 1LL)) {
                /* pass */
                ev = ({ TrStr _at_t843 = (Bindgen_ct(self)); __auto_type _wr = (_to_int(_at_t843)); _tr_str_release(_at_t843); _wr; });
                /* pass */
                Bindgen_adv(self);
                /* pass */
                if ((!(Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL)) || Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))))) {
                    /* pass */
                    simple = false;
                    /* pass */
                    Bindgen_skip_enum_expr(self);
                }
            } else {
                /* pass */
                simple = false;
                /* pass */
                Bindgen_skip_enum_expr(self);
            }
        }
        /* pass */
        if ((simple && Bindgen_fresh(self, ename))) {
            /* pass */
            ({ TrStr _sbt_t844 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ename))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": c_int = ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ev)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->consts, _sbt_t844); _tr_str_release(_sbt_t844); });
            /* pass */
            next_val = (ev + 1LL);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len(",", 1LL))) {
            /* pass */
            Bindgen_adv(self);
        }
        _tr_str_release(ename);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len("}", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_parse_decl(Bindgen* self) {
    /* pass */
    if ((Bindgen_ck(self) == 5LL)) {
        /* pass */
        return;
    }
    /* pass */
    TrStr lead = Bindgen_ct(self);
    /* pass */
    if (_tr_str_eqv((lead), (_tr_str_lit_len("typedef", 7LL)))) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr agg = Bindgen_ct(self);
        /* pass */
        if ((_tr_str_eqv((agg), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((agg), (_tr_str_lit_len("union", 5LL))))) {
            /* pass */
            bool is_u = _tr_str_eqv((agg), (_tr_str_lit_len("union", 5LL)));
            /* pass */
            Bindgen_adv(self);
            /* pass */
            TrStr tag = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t845 = Bindgen_ct(self);
                _tr_str_release(tag);
                tag = _strtmp_t845;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
                /* pass */
                TrStr tname = _scan_typedef_name(self->toks, self->pos);
                /* pass */
                TrStr cname = _tr_str_retain(tname);
                /* pass */
                if (_tr_str_eqv((cname), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    TrStr _strtmp_t846 = _tr_str_retain(tag);
                    _tr_str_release(cname);
                    cname = _strtmp_t846;
                }
                /* pass */
                Bindgen_emit_struct(self, cname, is_u);
                /* pass */
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(tag);
                _tr_str_release(tname);
                _tr_str_release(cname);
                return;
            }
            /* pass */
            long long ostars = 0LL;
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
                /* pass */
                ostars = (ostars + 1LL);
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            TrStr oname = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t847 = Bindgen_ct(self);
                _tr_str_release(oname);
                oname = _strtmp_t847;
            }
            /* pass */
            if ((((ostars > 0LL) && (!_tr_str_eqv((oname), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((tag), (_tr_str_lit_len("", 0LL)))))) {
                /* pass */
                if (Bindgen_fresh(self, tag)) {
                    /* pass */
                    ({ TrStr _sbt_t848 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (tag))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass    # opaque handle\n\n", 31LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t848); _tr_str_release(_sbt_t848); });
                    /* pass */
                    self->n_structs = (self->n_structs + 1LL);
                }
                /* pass */
                if (Bindgen_fresh(self, oname)) {
                    /* pass */
                    ({ TrStr _sbt_t849 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (oname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = Pointer[", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t849); _tr_str_release(_sbt_t849); });
                }
            } else if (((!_tr_str_eqv((oname), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, oname))) {
                /* pass */
                ({ TrStr _sbt_t850 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (oname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass    # opaque handle\n\n", 31LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t850); _tr_str_release(_sbt_t850); });
                /* pass */
                self->n_structs = (self->n_structs + 1LL);
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(tag);
            _tr_str_release(oname);
            return;
        }
        /* pass */
        if (_tr_str_eqv((agg), (_tr_str_lit_len("enum", 4LL)))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
                /* pass */
                TrStr ename = _scan_typedef_name(self->toks, self->pos);
                /* pass */
                Bindgen_emit_enum(self, ename);
                /* pass */
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(ename);
                return;
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            return;
        }
        /* pass */
        if (_typedef_is_fnptr(self->toks, self->pos)) {
            /* pass */
            TrStr fpn = _scan_fnptr_name(self->toks, self->pos);
            /* pass */
            if (((!_tr_str_eqv((fpn), (_tr_str_lit_len("", 0LL)))) && Bindgen_fresh(self, fpn))) {
                /* pass */
                ({ TrStr _sbt_t851 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (fpn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = Pointer[void]    # C function pointer\n", 41LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t851); _tr_str_release(_sbt_t851); });
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(fpn);
            return;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr abase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long astars = ({ TrStr _at_t852 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t852)); _tr_str_release(_at_t852); _wr; });
        /* pass */
        TrStr aname = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t853 = Bindgen_ct(self);
            _tr_str_release(aname);
            aname = _strtmp_t853;
        }
        /* pass */
        if (((((!_tr_str_eqv((aname), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((abase), (_tr_str_lit_len("", 0LL))))) && (!_tr_str_eqv((aname), (abase)))) && Bindgen_fresh(self, aname))) {
            /* pass */
            ({ TrStr _sbt_t854 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (aname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(abase, astars)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t854); _tr_str_release(_sbt_t854); });
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(agg);
        List_TrStr_free(tw);
        _tr_str_release(abase);
        _tr_str_release(aname);
        return;
    }
    /* pass */
    if ((_tr_str_eqv((lead), (_tr_str_lit_len("struct", 6LL))) || _tr_str_eqv((lead), (_tr_str_lit_len("union", 5LL))))) {
        /* pass */
        bool is_u2 = _tr_str_eqv((lead), (_tr_str_lit_len("union", 5LL)));
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr tag2 = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t855 = Bindgen_ct(self);
            _tr_str_release(tag2);
            tag2 = _strtmp_t855;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL)) && (!_tr_str_eqv((tag2), (_tr_str_lit_len("", 0LL)))))) {
            /* pass */
            Bindgen_emit_struct(self, tag2, is_u2);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(tag2);
            return;
        }
        /* pass */
        if (((Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL)) && (!_tr_str_eqv((tag2), (_tr_str_lit_len("", 0LL))))) && Bindgen_fresh(self, tag2))) {
            /* pass */
            ({ TrStr _sbt_t856 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (tag2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass    # opaque (forward-declared)\n\n", 43LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t856); _tr_str_release(_sbt_t856); });
            /* pass */
            self->n_structs = (self->n_structs + 1LL);
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(tag2);
        return;
    }
    /* pass */
    if (_tr_str_eqv((lead), (_tr_str_lit_len("enum", 4LL)))) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr etag = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t857 = Bindgen_ct(self);
            _tr_str_release(etag);
            etag = _strtmp_t857;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit_len("{", 1LL))) {
            /* pass */
            Bindgen_emit_enum(self, etag);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(etag);
            return;
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(etag);
        return;
    }
    /* pass */
    List_TrStr* rw = Bindgen_read_type_words(self);
    /* pass */
    TrStr rbase = List_TrStr_get(rw, 0LL);
    /* pass */
    long long rstars = ({ TrStr _at_t858 = (List_TrStr_get(rw, 1LL)); __auto_type _wr = (_to_int(_at_t858)); _tr_str_release(_at_t858); _wr; });
    /* pass */
    TrStr fname = _tr_str_lit_len("", 0LL);
    /* pass */
    if ((Bindgen_ck(self) == 0LL)) {
        /* pass */
        TrStr _strtmp_t859 = Bindgen_ct(self);
        _tr_str_release(fname);
        fname = _strtmp_t859;
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (((!_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL)))) && Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
        /* pass */
        if (Bindgen_func_def_follows(self)) {
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            Bindgen_skip_braces(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            _tr_str_release(lead);
            List_TrStr_free(rw);
            _tr_str_release(rbase);
            _tr_str_release(fname);
            return;
        }
        /* pass */
        Bindgen_emit_func(self, rbase, rstars, fname);
        /* pass */
        _tr_str_release(lead);
        List_TrStr_free(rw);
        _tr_str_release(rbase);
        _tr_str_release(fname);
        return;
    }
    /* pass */
    if ((_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL))) && Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL)))) {
        /* pass */
        long long saved = self->pos;
        /* pass */
        Bindgen_adv(self);
        /* pass */
        long long pdstars = 0LL;
        /* pass */
        bool scan = true;
        /* pass */
        while (scan) {
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr dw = Bindgen_ct(self);
                /* pass */
                if ((_tr_str_eqv((dw), (_tr_str_lit_len("__attribute__", 13LL))) || _tr_str_eqv((dw), (_tr_str_lit_len("__declspec", 10LL))))) {
                    /* pass */
                    Bindgen_adv(self);
                    /* pass */
                    Bindgen_skip_balanced_parens(self);
                } else if (_is_ignored_word(dw)) {
                    /* pass */
                    Bindgen_adv(self);
                } else {
                    /* pass */
                    scan = false;
                }
            } else if (Bindgen_is_punct(self, _tr_str_lit_len("*", 1LL))) {
                /* pass */
                pdstars = (pdstars + 1LL);
                /* pass */
                Bindgen_adv(self);
            } else {
                /* pass */
                scan = false;
            }
        }
        /* pass */
        TrStr pdname = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t860 = Bindgen_ct(self);
            _tr_str_release(pdname);
            pdname = _strtmp_t860;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((((!_tr_str_eqv((pdname), (_tr_str_lit_len("", 0LL)))) && (pdstars == 0LL)) && Bindgen_is_punct(self, _tr_str_lit_len(")", 1LL)))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit_len("(", 1LL))) {
                /* pass */
                if (Bindgen_func_def_follows(self)) {
                    /* pass */
                    Bindgen_skip_balanced_parens(self);
                    /* pass */
                    Bindgen_skip_braces(self);
                    /* pass */
                    if (Bindgen_is_punct(self, _tr_str_lit_len(";", 1LL))) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    /* pass */
                    _tr_str_release(lead);
                    List_TrStr_free(rw);
                    _tr_str_release(rbase);
                    _tr_str_release(fname);
                    _tr_str_release(pdname);
                    return;
                }
                /* pass */
                Bindgen_emit_func(self, rbase, rstars, pdname);
                /* pass */
                _tr_str_release(lead);
                List_TrStr_free(rw);
                _tr_str_release(rbase);
                _tr_str_release(fname);
                _tr_str_release(pdname);
                return;
            }
        }
        /* pass */
        self->pos = saved;
    }
    /* pass */
    Bindgen_skip_to_semi(self);
    _tr_str_release(lead);
    List_TrStr_free(rw);
    _tr_str_release(rbase);
    _tr_str_release(fname);
}

__attribute__((hot)) void Bindgen_run(Bindgen* self) {
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        long long before = self->pos;
        /* pass */
        Bindgen_parse_decl(self);
        /* pass */
        if ((self->pos == before)) {
            /* pass */
            Bindgen_adv(self);
        }
    }
}

__attribute__((hot)) bool _is_alpha(long long c) {
    /* pass */
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _is_digit(long long c) {
    /* pass */
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool _is_alnum(long long c) {
    /* pass */
    return (_is_alpha(c) || _is_digit(c));
}

__attribute__((hot)) bool _is_space(long long c) {
    /* pass */
    return ((((c == 32LL) || (c == 9LL)) || (c == 10LL)) || (c == 13LL));
}

__attribute__((hot)) List_ptr* tokenize_c(TrStr src) {
    /* pass */
    List_ptr* toks = (void*)List_ptr_new();
    /* pass */
    long long n = _tr_str_lenv((src));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(src)[i];
        /* pass */
        if (_is_space(c)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 47LL))) {
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 42LL))) {
            /* pass */
            i = (i + 2LL);
            /* pass */
            while ((((i + 1LL) < n) && (!((_tr_strz(src)[i] == 42LL) && (_tr_strz(src)[(i + 1LL)] == 47LL))))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 2LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 35LL)) {
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_is_alpha(c)) {
            /* pass */
            long long start = i;
            /* pass */
            while (((i < n) && _is_alnum(_tr_strz(src)[i]))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            ({ TrStr _at_t861 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(0LL, _at_t861)); _tr_str_release(_at_t861); });
            /* pass */
            continue;
        }
        /* pass */
        if (_is_digit(c)) {
            /* pass */
            long long start = i;
            /* pass */
            while (((i < n) && ((_is_alnum(_tr_strz(src)[i]) || (_tr_strz(src)[i] == 46LL)) || (_tr_strz(src)[i] == 120LL)))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            ({ TrStr _at_t862 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(1LL, _at_t862)); _tr_str_release(_at_t862); });
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 34LL)) {
            /* pass */
            long long start = i;
            /* pass */
            i = (i + 1LL);
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 34LL))) {
                /* pass */
                if ((_tr_strz(src)[i] == 92LL)) {
                    /* pass */
                    i = (i + 1LL);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t863 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(2LL, _at_t863)); _tr_str_release(_at_t863); });
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 39LL)) {
            /* pass */
            long long start = i;
            /* pass */
            i = (i + 1LL);
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 39LL))) {
                /* pass */
                if ((_tr_strz(src)[i] == 92LL)) {
                    /* pass */
                    i = (i + 1LL);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t864 = (_tr_str_slicev((src), start, i)); List_ptr_append(toks, CTok_init(3LL, _at_t864)); _tr_str_release(_at_t864); });
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _at_t865 = (_tr_str_slicev((src), i, (i + 1LL))); List_ptr_append(toks, CTok_init(4LL, _at_t865)); _tr_str_release(_at_t865); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_ptr_append(toks, CTok_init(5LL, _tr_str_lit_len("", 0LL)));
    /* pass */
    return toks;
}

__attribute__((hot)) TrStr map_base(TrStr words) {
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    if (((_tr_str_eqv((words), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((words), (_tr_str_lit_len("signed", 6LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed int", 10LL))))) {
        /* pass */
        return _tr_str_lit_len("c_int", 5LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned int", 12LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned", 8LL))))) {
        /* pass */
        return _tr_str_lit_len("c_uint", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("char", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("c_char", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("unsigned char", 13LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uchar", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("signed char", 11LL)))) {
        /* pass */
        return _tr_str_lit_len("c_schar", 7LL);
    }
    /* pass */
    if (((_tr_str_eqv((words), (_tr_str_lit_len("short", 5LL))) || _tr_str_eqv((words), (_tr_str_lit_len("short int", 9LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed short", 12LL))))) {
        /* pass */
        return _tr_str_lit_len("c_short", 7LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned short", 14LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned short int", 18LL))))) {
        /* pass */
        return _tr_str_lit_len("c_ushort", 8LL);
    }
    /* pass */
    if (((_tr_str_eqv((words), (_tr_str_lit_len("long", 4LL))) || _tr_str_eqv((words), (_tr_str_lit_len("long int", 8LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed long", 11LL))))) {
        /* pass */
        return _tr_str_lit_len("c_long", 6LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned long", 13LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned long int", 17LL))))) {
        /* pass */
        return _tr_str_lit_len("c_ulong", 7LL);
    }
    /* pass */
    if (((_tr_str_eqv((words), (_tr_str_lit_len("long long", 9LL))) || _tr_str_eqv((words), (_tr_str_lit_len("long long int", 13LL)))) || _tr_str_eqv((words), (_tr_str_lit_len("signed long long", 16LL))))) {
        /* pass */
        return _tr_str_lit_len("c_longlong", 10LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("unsigned long long", 18LL))) || _tr_str_eqv((words), (_tr_str_lit_len("unsigned long long int", 22LL))))) {
        /* pass */
        return _tr_str_lit_len("c_ulonglong", 11LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("va_list", 7LL))) || _tr_str_eqv((words), (_tr_str_lit_len("__builtin_va_list", 17LL))))) {
        /* pass */
        return _tr_str_lit_len("Pointer[void]", 13LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("float", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("c_float", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("double", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("c_double", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("long double", 11LL)))) {
        /* pass */
        return _tr_str_lit_len("c_ldouble", 9LL);
    }
    /* pass */
    if ((_tr_str_eqv((words), (_tr_str_lit_len("_Bool", 5LL))) || _tr_str_eqv((words), (_tr_str_lit_len("bool", 4LL))))) {
        /* pass */
        return _tr_str_lit_len("bool", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("size_t", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("c_size_t", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("ssize_t", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("c_ssize_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("int8_t", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("c_int8_t", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("int16_t", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("c_int16_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("int32_t", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("c_int32_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("int64_t", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("c_int64_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint8_t", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uint8_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint16_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uint16_t", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint32_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uint32_t", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("uint64_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uint64_t", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("intptr_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("c_intptr_t", 10LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("uintptr_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("c_uintptr_t", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((words), (_tr_str_lit_len("ptrdiff_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("c_ptrdiff_t", 11LL);
    }
    /* pass */
    return _tr_str_retain(words);
}

__attribute__((hot)) TrStr map_type(TrStr words, long long stars) {
    /* pass */
    TrStr base = map_base(words);
    /* pass */
    if ((stars <= 0LL)) {
        /* pass */
        return base;
    }
    /* pass */
    TrStr inner = _tr_str_retain(base);
    /* pass */
    if (_tr_str_eqv((base), (_tr_str_lit_len("c_char", 6LL)))) {
        /* pass */
        TrStr _strtmp_t866 = _tr_str_lit_len("char", 4LL);
        _tr_str_release(inner);
        inner = _strtmp_t866;
    }
    /* pass */
    if (_tr_str_eqv((base), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        TrStr _strtmp_t867 = _tr_str_lit_len("void", 4LL);
        _tr_str_release(inner);
        inner = _strtmp_t867;
    }
    /* pass */
    TrStr ty = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (inner))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long d = 1LL;
    /* pass */
    while ((d < stars)) {
        /* pass */
        TrStr _strtmp_t868 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t868;
        /* pass */
        d = (d + 1LL);
    }
    /* pass */
    _tr_str_release(base);
    _tr_str_release(inner);
    return ty;
}

__attribute__((hot)) TrMap* _runtime_symbols() {
    /* pass */
    TrStr names = _tr_str_lit_len("printf fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf scanf fscanf sscanf fopen fclose fread fwrite fseek ftell fgets fputs fgetc fputc getc putc getchar putchar puts perror fflush freopen setvbuf setbuf rewind remove rename tmpfile ungetc feof ferror clearerr ", 275LL);
    /* pass */
    TrStr _strtmp_t869 = _tr_strx_concatv((names), (_tr_str_lit_len("malloc calloc realloc free aligned_alloc abort exit _Exit atexit quick_exit system getenv setenv unsetenv abs labs llabs atoi atol atoll atof strtol strtoul strtoll strtoull strtod strtof rand srand qsort bsearch div ldiv lldiv ", 228LL)));
    _tr_str_release(names);
    names = _strtmp_t869;
    /* pass */
    TrStr _strtmp_t870 = _tr_strx_concatv((names), (_tr_str_lit_len("memcpy memmove memset memcmp memchr strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strlen strnlen strdup strndup strtok strspn strcspn strpbrk strerror strcoll strxfrm strcasecmp strncasecmp ", 211LL)));
    _tr_str_release(names);
    names = _strtmp_t870;
    /* pass */
    TrStr _strtmp_t871 = _tr_strx_concatv((names), (_tr_str_lit_len("sqrt sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow cbrt hypot floor ceil round trunc fabs fmod ldexp frexp modf fmin fmax copysign nextafter nan isnan isinf signbit ", 195LL)));
    _tr_str_release(names);
    names = _strtmp_t871;
    /* pass */
    TrStr _strtmp_t872 = _tr_strx_concatv((names), (_tr_str_lit_len("read write open close lseek unlink stat fstat mkdir rmdir access dup dup2 pipe fork execve waitpid kill getpid ", 111LL)));
    _tr_str_release(names);
    names = _strtmp_t872;
    /* pass */
    TrStr _strtmp_t873 = _tr_strx_concatv((names), (_tr_str_lit_len("CreateWindow CloseWindow CreateWindowExA CreateWindowExW GetMessage DispatchMessage MessageBox MessageBoxA MessageBoxW ", 119LL)));
    _tr_str_release(names);
    names = _strtmp_t873;
    /* pass */
    TrStr _strtmp_t874 = _tr_strx_concatv((names), (_tr_str_lit_len("Rectangle Ellipse Polygon Polyline Arc Chord Pie RoundRect LineTo MoveToEx FillRect FrameRect InvertRect DrawIcon DrawText DrawTextEx TextOut GetObject LoadImage CreateFont PlaySound Polygon PolyBezier ", 202LL)));
    _tr_str_release(names);
    names = _strtmp_t874;
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    List_TrStr* parts = _tr_str_splitv((names), (_tr_str_lit_len(" ", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((!_tr_str_eqv((List_TrStr_get(parts, i)), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            ({ TrStr _dkt_t875 = (List_TrStr_get(parts, i)); _tr_dict_set(m, _tr_strz(_dkt_t875), true); _tr_str_release(_dkt_t875); });
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    _tr_str_release(names);
    List_TrStr_free(parts);
    return m;
}

__attribute__((hot)) bool _is_prim_type_word(TrStr w) {
    /* pass */
    return ((((((((((_tr_str_eqv((w), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((w), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("short", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("long", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("double", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("signed", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("unsigned", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("_Bool", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("bool", 4LL))));
}

__attribute__((hot)) bool _is_ignored_word(TrStr w) {
    /* pass */
    return ((((((((((((((((((((_tr_str_eqv((w), (_tr_str_lit_len("const", 5LL))) || _tr_str_eqv((w), (_tr_str_lit_len("volatile", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("extern", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("static", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("inline", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("register", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("auto", 4LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__inline", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__inline__", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__forceinline", 13LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("restrict", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__restrict", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__restrict__", 12LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__cdecl", 7LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__stdcall", 9LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__fastcall", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__thiscall", 10LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("WINAPI", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("APIENTRY", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("__extension__", 13LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("_Noreturn", 9LL))));
}

__attribute__((hot)) bool _is_decl_term(long long kind, TrStr text) {
    /* pass */
    if ((kind == 5LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((kind != 4LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return ((((((_tr_str_eqv((text), (_tr_str_lit_len(",", 1LL))) || _tr_str_eqv((text), (_tr_str_lit_len(";", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len(")", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("[", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("=", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("(", 1LL)))) || _tr_str_eqv((text), (_tr_str_lit_len("}", 1LL))));
}

__attribute__((hot)) TrStr _join_words(List_TrStr* words) {
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < words->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t876 = _tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t876;
        }
        /* pass */
        TrStr _strtmp_t877 = ({ TrStr _cr = (List_TrStr_get(words, i)); TrStr _cres = _tr_strx_concatv((s), _cr); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t877;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) long long _to_int(TrStr s) {
    /* pass */
    long long r = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    bool neg = false;
    /* pass */
    if (((_tr_str_lenv((s)) > 0LL) && (_tr_strz(s)[0LL] == 45LL))) {
        /* pass */
        neg = true;
        /* pass */
        i = 1LL;
    }
    /* pass */
    if (((((i + 1LL) < _tr_str_lenv((s))) && (_tr_strz(s)[i] == 48LL)) && ((_tr_strz(s)[(i + 1LL)] == 120LL) || (_tr_strz(s)[(i + 1LL)] == 88LL)))) {
        /* pass */
        i = (i + 2LL);
        /* pass */
        long long hd = 0LL;
        /* pass */
        while (((i < _tr_str_lenv((s))) && (hd < 15LL))) {
            /* pass */
            char hc = _tr_strz(s)[i];
            /* pass */
            if (((hc >= 48LL) && (hc <= 57LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 48LL));
            } else if (((hc >= 97LL) && (hc <= 102LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 87LL));
            } else if (((hc >= 65LL) && (hc <= 70LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 55LL));
            } else {
                /* pass */
                break;
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            hd = (hd + 1LL);
        }
        /* pass */
        if (neg) {
            /* pass */
            return (0LL - r);
        }
        /* pass */
        return r;
    }
    /* pass */
    long long dd = 0LL;
    /* pass */
    while (((i < _tr_str_lenv((s))) && (dd < 18LL))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if (((c >= 48LL) && (c <= 57LL))) {
            /* pass */
            r = ((r * 10LL) + (c - 48LL));
        } else {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        dd = (dd + 1LL);
    }
    /* pass */
    if (neg) {
        /* pass */
        return (0LL - r);
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long _skip_gnu_attrs(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while ((i < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if ((t->kind != 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if (((!_tr_str_eqv((t->text), (_tr_str_lit_len("__attribute__", 13LL)))) && (!_tr_str_eqv((t->text), (_tr_str_lit_len("__declspec", 10LL)))))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((i >= toks->len)) {
            /* pass */
            break;
        }
        /* pass */
        if ((!((((CTok*)List_ptr_get(toks, i))->kind == 4LL) && _tr_str_eqv((((CTok*)List_ptr_get(toks, i))->text), (_tr_str_lit_len("(", 1LL)))))) {
            /* pass */
            break;
        }
        /* pass */
        long long depth = 0LL;
        /* pass */
        while ((i < toks->len)) {
            /* pass */
            CTok* pt = ((CTok*)List_ptr_get(toks, i));
            /* pass */
            if (((pt->kind == 4LL) && _tr_str_eqv((pt->text), (_tr_str_lit_len("(", 1LL))))) {
                /* pass */
                depth = (depth + 1LL);
            } else if (((pt->kind == 4LL) && _tr_str_eqv((pt->text), (_tr_str_lit_len(")", 1LL))))) {
                /* pass */
                depth = (depth - 1LL);
                /* pass */
                if ((depth == 0LL)) {
                    /* pass */
                    i = (i + 1LL);
                    /* pass */
                    break;
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    return i;
}

__attribute__((hot)) TrStr _scan_typedef_name(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((i < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("{", 1LL))))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("}", 1LL))))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                long long ni = _skip_gnu_attrs(toks, (i + 1LL));
                /* pass */
                if (((ni < toks->len) && (((CTok*)List_ptr_get(toks, ni))->kind == 0LL))) {
                    /* pass */
                    return _tr_str_retain(((CTok*)List_ptr_get(toks, ni))->text);
                }
                /* pass */
                return _tr_str_lit_len("", 0LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _typedef_is_fnptr(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while (((i + 1LL) < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len(";", 1LL))))) {
            /* pass */
            return false;
        }
        /* pass */
        if (((((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("(", 1LL)))) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && _tr_str_eqv((((CTok*)List_ptr_get(toks, (i + 1LL)))->text), (_tr_str_lit_len("*", 1LL))))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _scan_fnptr_name(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while (((i + 2LL) < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len(";", 1LL))))) {
            /* pass */
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        if (((((t->kind == 4LL) && _tr_str_eqv((t->text), (_tr_str_lit_len("(", 1LL)))) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && _tr_str_eqv((((CTok*)List_ptr_get(toks, (i + 1LL)))->text), (_tr_str_lit_len("*", 1LL))))) {
            /* pass */
            if ((((CTok*)List_ptr_get(toks, (i + 2LL)))->kind == 0LL)) {
                /* pass */
                return _tr_str_retain(((CTok*)List_ptr_get(toks, (i + 2LL)))->text);
            }
            /* pass */
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _is_ident_byte(long long c) {
    /* pass */
    return (((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL));
}

__attribute__((hot)) TrStr _rename_word(TrStr text, TrStr old, TrStr new_) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((text)) + 16LL));
    /* pass */
    long long n = _tr_str_lenv((text));
    /* pass */
    long long ol = _tr_str_lenv((old));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        bool matched = false;
        /* pass */
        if ((((i + ol) <= n) && _tr_str_eqv((_tr_str_slicev((text), i, (i + ol))), (old)))) {
            /* pass */
            bool before_ok = ((i == 0LL) || (!_is_ident_byte(_tr_strz(text)[(i - 1LL)])));
            /* pass */
            bool after_ok = (((i + ol) >= n) || (!_is_ident_byte(_tr_strz(text)[(i + ol)])));
            /* pass */
            if ((before_ok && after_ok)) {
                /* pass */
                StringBuilder_append(sb, new_);
                /* pass */
                i = (i + ol);
                /* pass */
                matched = true;
            }
        }
        /* pass */
        if ((!matched)) {
            /* pass */
            ({ TrStr _sbt_t878 = (_tr_str_slicev((text), i, (i + 1LL))); StringBuilder_append(sb, _sbt_t878); _tr_str_release(_sbt_t878); });
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _basename(TrStr p) {
    /* pass */
    long long last = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_str_lenv((p)))) {
        /* pass */
        char c = _tr_strz(p)[i];
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            last = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_slicev((p), last, _tr_str_lenv((p)));
}

__attribute__((hot)) TrStr _marker_file(TrStr line) {
    /* pass */
    if ((((_tr_str_lenv((line)) < 3LL) || (_tr_strz(line)[0LL] != 35LL)) || (_tr_strz(line)[1LL] != 32LL))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    __auto_type q1 = _tr_str_index_ofv((line), (_tr_str_lit_len("\"", 1LL)));
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    TrStr rest = _tr_str_slicev((line), (q1 + 1LL), _tr_str_lenv((line)));
    /* pass */
    __auto_type q2 = _tr_str_index_ofv((rest), (_tr_str_lit_len("\"", 1LL)));
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_str_slicev((rest), 0LL, q2);
}

__attribute__((hot)) bool _marker_is_system(TrStr line) {
    /* pass */
    __auto_type q1 = _tr_str_index_ofv((line), (_tr_str_lit_len("\"", 1LL)));
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rest = _tr_str_slicev((line), (q1 + 1LL), _tr_str_lenv((line)));
    /* pass */
    __auto_type q2 = _tr_str_index_ofv((rest), (_tr_str_lit_len("\"", 1LL)));
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return false;
    }
    /* pass */
    return (_tr_str_index_ofv((_tr_str_slicev((rest), (q2 + 1LL), _tr_str_lenv((rest)))), (_tr_str_lit_len(" 3", 2LL))) >= 0LL);
}

__attribute__((hot)) TrStr _filter_to_target(TrStr raw, TrStr target) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(_tr_str_lenv((raw)));
    /* pass */
    List_TrStr* lines = _tr_str_splitv((raw), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    bool cur_ok = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_eqv((_marker_file(ln)), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            cur_ok = (!_marker_is_system(ln));
            /* pass */
            continue;
        }
        /* pass */
        if (cur_ok) {
            /* pass */
            StringBuilder_append(sb, ln);
            /* pass */
            StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
        }
        _tr_str_release(ln);
    }
    /* pass */
    List_TrStr_free(lines);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _macro_name(TrStr rest) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_str_lenv((rest)))) {
        /* pass */
        char c = _tr_strz(rest)[i];
        /* pass */
        if (((c == 32LL) || (c == 40LL))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_slicev((rest), 0LL, i);
}

__attribute__((hot)) TrMap* _load_baseline(TrStr cc) {
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    write_file(_tr_str_lit_len("_bindgen_empty.h", 16LL), _tr_str_lit_len("", 0LL));
    /* pass */
    ({ TrStr _aet_t879 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" -E -dM \"_bindgen_empty.h\" > \"_bindgen_base.i\" 2>_bindgen_err.txt", 65LL)))); _tr_system(_aet_t879.data); _tr_str_release(_aet_t879); });
    /* pass */
    if ((!file_exists(_tr_str_lit_len("_bindgen_base.i", 15LL)))) {
        /* pass */
        return m;
    }
    /* pass */
    List_TrStr* lines = _tr_str_splitv((read_file(_tr_str_lit_len("_bindgen_base.i", 15LL))), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t880 = (_tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)))); __auto_type _wr = (_macro_name(_at_t880)); _tr_str_release(_at_t880); _wr; });
        /* pass */
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
    /* pass */
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrMap* _target_define_names(TrStr header) {
    /* pass */
    TrMap* m = _tr_dict_new(128LL);
    /* pass */
    if ((!file_exists(header))) {
        /* pass */
        return m;
    }
    /* pass */
    List_TrStr* lines = _tr_str_splitv((read_file(header)), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t881 = (List_TrStr_get(lines, i)); __auto_type _wr = (_lstrip(_at_t881)); _tr_str_release(_at_t881); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t882 = (_tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)))); __auto_type _wr = (_macro_name(_at_t882)); _tr_str_release(_at_t882); _wr; });
        /* pass */
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
    /* pass */
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _lstrip(TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i < _tr_str_lenv((s))) && ((_tr_strz(s)[i] == 32LL) || (_tr_strz(s)[i] == 9LL)))) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_slicev((s), i, _tr_str_lenv((s)));
}

__attribute__((hot)) bool _is_builtin_ty_name(TrStr n) {
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("int", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    return _tr_str_starts_withv((n), (_tr_str_lit_len("c_", 2LL)));
}

__attribute__((hot)) bool _is_libc_ty_name(TrStr n) {
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("FILE", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("fpos_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("va_list", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("jmp_buf", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sigjmp_buf", 10LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("time_t", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("clock_t", 7LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("wchar_t", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("wint_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mbstate_t", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("off_t", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("off64_t", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mode_t", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pid_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("uid_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("gid_t", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("dev_t", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ino_t", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("nlink_t", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("blkcnt_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("blksize_t", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("fd_set", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("sigset_t", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("DIR", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pthread_t", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("sig_atomic_t", 12LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_system_record_ty(TrStr n) {
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("_GUID", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("GUID", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IID", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("CLSID", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FMTID", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("UUID", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("IUnknown", 8LL))) || _tr_str_eqv((n), (_tr_str_lit_len("IDispatch", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IErrorInfo", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ITypeInfo", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ITypeLib", 8LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("IStream", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ISequentialStream", 17LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IStorage", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IRecordInfo", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("IServiceProvider", 16LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("VARIANT", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("VARIANTARG", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("DECIMAL", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SAFEARRAY", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SAFEARRAYBOUND", 14LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("DISPPARAMS", 10LL))) || _tr_str_eqv((n), (_tr_str_lit_len("EXCEPINFO", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("CY", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("BSTRBLOB", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("BLOB", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FLAGGED_WORD_BLOB", 17LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((_tr_str_eqv((n), (_tr_str_lit_len("POINT", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("POINTL", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SIZE", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("SIZEL", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RECT", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RECTL", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("FILETIME", 8LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("LARGE_INTEGER", 13LL))) || _tr_str_eqv((n), (_tr_str_lit_len("ULARGE_INTEGER", 14LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("LUID", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("MSG", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("POINTS", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _ident_at(TrStr text, long long start) {
    /* pass */
    long long i = start;
    /* pass */
    while (((i < _tr_str_lenv((text))) && (((_tr_strz(text)[i] == 32LL) || (_tr_strz(text)[i] == 9LL)) || (_tr_strz(text)[i] == 91LL)))) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((i >= _tr_str_lenv((text)))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    char c = _tr_strz(text)[i];
    /* pass */
    if ((!((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL)))) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    long long b = i;
    /* pass */
    while ((i < _tr_str_lenv((text)))) {
        /* pass */
        c = _tr_strz(text)[i];
        /* pass */
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
            /* pass */
            i = (i + 1LL);
        } else {
            /* pass */
            break;
        }
    }
    /* pass */
    return _tr_str_slicev((text), b, i);
}

__attribute__((hot)) TrStr _opaque_fallbacks(TrStr body, TrMap* defined) {
    /* pass */
    TrMap* want = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* order = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_str_lenv((body));
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(body)[i];
        /* pass */
        TrStr nm = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((c == 58LL)) {
            /* pass */
            TrStr _strtmp_t883 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t883;
            /* pass */
            i = (i + 1LL);
        } else if ((((c == 45LL) && ((i + 1LL) < n)) && (_tr_strz(body)[(i + 1LL)] == 62LL))) {
            /* pass */
            TrStr _strtmp_t884 = _ident_at(body, (i + 2LL));
            _tr_str_release(nm);
            nm = _strtmp_t884;
            /* pass */
            i = (i + 2LL);
        } else if ((c == 91LL)) {
            /* pass */
            TrStr _strtmp_t885 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t885;
            /* pass */
            i = (i + 1LL);
        } else {
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_is_builtin_ty_name(nm)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_is_libc_ty_name(nm)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(defined, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(want, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        _tr_dict_set(want, _tr_strz(nm), true);
        /* pass */
        List_TrStr_append(order, nm);
        _tr_str_release(nm);
    }
    /* pass */
    if ((order->len == 0LL)) {
        /* pass */
        Dict_free(want);
        List_TrStr_free(order);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(256LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("# Opaque fallbacks: types referenced by this header but defined in a system or\n", 79LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("# sub-header (bound as opaque handles; pass by Pointer). Verify by-value layout.\n", 81LL));
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < order->len)) {
        /* pass */
        ({ TrStr _sbt_t886 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(order, k)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = Pointer[void]\n", 17LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t886); _tr_str_release(_sbt_t886); });
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
    /* pass */
    Dict_free(want);
    List_TrStr_free(order);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) bool _is_single_string_literal(TrStr val) {
    /* pass */
    if (((_tr_str_lenv((val)) < 2LL) || (_tr_strz(val)[0LL] != 34LL))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 1LL;
    /* pass */
    bool esc = false;
    /* pass */
    while ((i < _tr_str_lenv((val)))) {
        /* pass */
        char c = _tr_strz(val)[i];
        /* pass */
        if (esc) {
            /* pass */
            esc = false;
        } else if ((c == 92LL)) {
            /* pass */
            esc = true;
        } else if ((c == 34LL)) {
            /* pass */
            long long j = (i + 1LL);
            /* pass */
            while ((j < _tr_str_lenv((val)))) {
                /* pass */
                char t = _tr_strz(val)[j];
                /* pass */
                if (((t != 32LL) && (t != 9LL))) {
                    /* pass */
                    return false;
                }
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_single_int_literal(TrStr val) {
    /* pass */
    long long n = _tr_str_lenv((val));
    /* pass */
    while (((n > 0LL) && ((_tr_strz(val)[(n - 1LL)] == 32LL) || (_tr_strz(val)[(n - 1LL)] == 9LL)))) {
        /* pass */
        n = (n - 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    if (((_tr_strz(val)[0LL] == 45LL) || (_tr_strz(val)[0LL] == 43LL))) {
        /* pass */
        i = 1LL;
    }
    /* pass */
    if ((i >= n)) {
        /* pass */
        return false;
    }
    /* pass */
    bool hex = false;
    /* pass */
    if (((((i + 1LL) < n) && (_tr_strz(val)[i] == 48LL)) && ((_tr_strz(val)[(i + 1LL)] == 120LL) || (_tr_strz(val)[(i + 1LL)] == 88LL)))) {
        /* pass */
        hex = true;
        /* pass */
        i = (i + 2LL);
        /* pass */
        if ((i >= n)) {
            /* pass */
            return false;
        }
    }
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(val)[i];
        /* pass */
        bool ok = false;
        /* pass */
        if (((c >= 48LL) && (c <= 57LL))) {
            /* pass */
            ok = true;
        } else if ((hex && (((c >= 97LL) && (c <= 102LL)) || ((c >= 65LL) && (c <= 70LL))))) {
            /* pass */
            ok = true;
        }
        /* pass */
        if ((!ok)) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow) {
    /* pass */
    List_TrStr* lines = _tr_str_splitv((defs), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)));
        /* pass */
        __auto_type sp = _tr_str_index_ofv((rest), (_tr_str_lit_len(" ", 1LL)));
        /* pass */
        if ((sp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_slicev((rest), 0LL, sp);
        /* pass */
        if ((_tr_str_index_ofv((nm), (_tr_str_lit_len("(", 1LL))) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!_tr_dict_contains(allow, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr val = ({ TrStr _at_t887 = (_tr_str_slicev((rest), (sp + 1LL), _tr_str_lenv((rest)))); __auto_type _wr = (_lstrip(_at_t887)); _tr_str_release(_at_t887); _wr; });
        /* pass */
        if ((_tr_str_lenv((val)) == 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        char c0 = _tr_strz(val)[0LL];
        /* pass */
        if ((c0 == 34LL)) {
            /* pass */
            if (_is_single_string_literal(val)) {
                /* pass */
                ({ TrStr _sbt_t888 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t888); _tr_str_release(_sbt_t888); });
            }
        } else if ((((c0 >= 48LL) && (c0 <= 57LL)) || (c0 == 45LL))) {
            /* pass */
            if (_is_single_int_literal(val)) {
                /* pass */
                long long iv = _to_int(val);
                /* pass */
                ({ TrStr _sbt_t889 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": c_int = ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(iv)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t889); _tr_str_release(_sbt_t889); });
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
    /* pass */
    if (_tr_str_eqv((cc), (_tr_str_lit_len("gcc", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("g++", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((cc), (_tr_str_lit_len("clang", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("clang++", 7LL);
    }
    /* pass */
    if ((_tr_str_index_ofv((cc), (_tr_str_lit_len("++", 2LL))) >= 0LL)) {
        /* pass */
        return _tr_str_retain(cc);
    }
    /* pass */
    return _tr_str_lit_len("c++", 3LL);
}

__attribute__((hot)) TrStr _macro_args(long long n, long long mode, long long atype, long long tp) {
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    bool first = true;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        if ((i == tp)) {
            /* pass */
            if ((mode == 1LL)) {
                /* pass */
                if ((!first)) {
                    /* pass */
                    TrStr _strtmp_t890 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(s);
                    s = _strtmp_t890;
                }
                /* pass */
                TrStr _strtmp_t891 = _tr_strx_concatv((s), (_tr_str_lit_len("char", 4LL)));
                _tr_str_release(s);
                s = _strtmp_t891;
                /* pass */
                first = false;
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            TrStr _strtmp_t892 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t892;
        }
        /* pass */
        first = false;
        /* pass */
        if ((mode == 0LL)) {
            /* pass */
            if ((atype == 0LL)) {
                /* pass */
                TrStr _strtmp_t893 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("void* a", 7LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t893;
            } else {
                /* pass */
                TrStr _strtmp_t894 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("intptr_t a", 10LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t894;
            }
        } else if ((mode == 1LL)) {
            /* pass */
            TrStr _strtmp_t895 = ({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t895;
        } else {
            /* pass */
            if ((atype == 0LL)) {
                /* pass */
                TrStr _strtmp_t896 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": Pointer[void]", 15LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t896;
            } else {
                /* pass */
                TrStr _strtmp_t897 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((s), (_tr_str_lit_len("a", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": c_intptr_t", 12LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t897;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _ent_name(TrStr e) {
    /* pass */
    return _tr_str_slicev((e), 0LL, _tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))));
}

__attribute__((hot)) long long _ent_arity(TrStr e) {
    /* pass */
    TrStr r = _tr_str_slicev((e), (_tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))) + 1LL), _tr_str_lenv((e)));
    /* pass */
    __auto_type b = _tr_str_index_ofv((r), (_tr_str_lit_len("|", 1LL)));
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        return _to_int(r);
    }
    /* pass */
    return ({ TrStr _at_t898 = (_tr_str_slicev((r), 0LL, b)); __auto_type _wr = (_to_int(_at_t898)); _tr_str_release(_at_t898); _wr; });
}

__attribute__((hot)) long long _ent_typepos(TrStr e) {
    /* pass */
    TrStr r = _tr_str_slicev((e), (_tr_str_index_ofv((e), (_tr_str_lit_len("|", 1LL))) + 1LL), _tr_str_lenv((e)));
    /* pass */
    __auto_type b = _tr_str_index_ofv((r), (_tr_str_lit_len("|", 1LL)));
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        _tr_str_release(r);
        return (-1LL);
    }
    /* pass */
    return ({ TrStr _at_t899 = (_tr_str_slicev((r), (b + 1LL), _tr_str_lenv((r)))); __auto_type _wr = (_to_int(_at_t899)); _tr_str_release(_at_t899); _wr; });
}

__attribute__((hot)) bool _is_ident_ch(long long c) {
    /* pass */
    return (((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _param_ptr_type(TrStr body, TrStr pname) {
    /* pass */
    long long plen = _tr_str_lenv((pname));
    /* pass */
    if ((plen == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i + plen) <= _tr_str_lenv((body)))) {
        /* pass */
        if (_tr_str_eqv((_tr_str_slicev((body), i, (i + plen))), (pname))) {
            /* pass */
            bool okb = true;
            /* pass */
            if (((i > 0LL) && _is_ident_ch(_tr_strz(body)[(i - 1LL)]))) {
                /* pass */
                okb = false;
            }
            /* pass */
            long long j = (i + plen);
            /* pass */
            bool oka = true;
            /* pass */
            if (((j < _tr_str_lenv((body))) && _is_ident_ch(_tr_strz(body)[j]))) {
                /* pass */
                oka = false;
            }
            /* pass */
            if ((okb && oka)) {
                /* pass */
                while (((j < _tr_str_lenv((body))) && ((_tr_strz(body)[j] == 32LL) || (_tr_strz(body)[j] == 9LL)))) {
                    /* pass */
                    j = (j + 1LL);
                }
                /* pass */
                if (((j < _tr_str_lenv((body))) && (_tr_strz(body)[j] == 42LL))) {
                    /* pass */
                    return true;
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _balanced_delims(TrStr s) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_str_lenv((s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth < 0LL)) {
                /* pass */
                return false;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (depth == 0LL);
}

__attribute__((hot)) List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    TrMap* seen = _tr_dict_new(128LL);
    /* pass */
    List_TrStr* lines = _tr_str_splitv((defs), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_withv((ln), (_tr_str_lit_len("#define ", 8LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_slicev((ln), 8LL, _tr_str_lenv((ln)));
        /* pass */
        __auto_type par = _tr_str_index_ofv((rest), (_tr_str_lit_len("(", 1LL)));
        /* pass */
        if ((par <= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type sp = _tr_str_index_ofv((rest), (_tr_str_lit_len(" ", 1LL)));
        /* pass */
        if (((sp >= 0LL) && (sp < par))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_slicev((rest), 0LL, par);
        /* pass */
        if (_tr_str_starts_withv((nm), (_tr_str_lit_len("__", 2LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type _tr_v_close = _tr_str_index_ofv((rest), (_tr_str_lit_len(")", 1LL)));
        /* pass */
        if (((_tr_v_close < 0LL) || (_tr_v_close < par))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr plist = _tr_str_slicev((rest), (par + 1LL), _tr_v_close);
        /* pass */
        if ((_tr_str_index_ofv((plist), (_tr_str_lit_len("...", 3LL))) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr body = ({ TrStr _at_t900 = (_tr_str_slicev((rest), (_tr_v_close + 1LL), _tr_str_lenv((rest)))); __auto_type _wr = (_lstrip(_at_t900)); _tr_str_release(_at_t900); _wr; });
        /* pass */
        if (_tr_str_eqv((body), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!_balanced_delims(body))) {
            /* pass */
            continue;
        }
        /* pass */
        long long arity = 0LL;
        /* pass */
        TrStr pt = _tr_str_stripv((plist));
        /* pass */
        if ((!_tr_str_eqv((pt), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            arity = 1LL;
            /* pass */
            long long ci = 0LL;
            /* pass */
            while ((ci < _tr_str_lenv((pt)))) {
                /* pass */
                if ((_tr_strz(pt)[ci] == 44LL)) {
                    /* pass */
                    arity = (arity + 1LL);
                }
                /* pass */
                ci = (ci + 1LL);
            }
        }
        /* pass */
        if ((arity > 8LL)) {
            /* pass */
            continue;
        }
        /* pass */
        long long typepos = (-1LL);
        /* pass */
        if (((((arity > 0LL) && (_tr_str_index_ofv((body), (_tr_str_lit_len("sizeof", 6LL))) < 0LL)) && (_tr_str_index_ofv((body), (_tr_str_lit_len("offsetof", 8LL))) < 0LL)) && (_tr_str_index_ofv((body), (_tr_str_lit_len("##", 2LL))) < 0LL))) {
            /* pass */
            List_TrStr* parts = _tr_str_splitv((plist), (_tr_str_lit_len(",", 1LL)));
            /* pass */
            long long pi = 0LL;
            /* pass */
            long long nfound = 0LL;
            /* pass */
            while ((pi < parts->len)) {
                /* pass */
                TrStr pnm = _tr_str_stripv((List_TrStr_get(parts, pi)));
                /* pass */
                if (_param_ptr_type(body, pnm)) {
                    /* pass */
                    typepos = pi;
                    /* pass */
                    nfound = (nfound + 1LL);
                }
                /* pass */
                pi = (pi + 1LL);
                _tr_str_release(pnm);
            }
            /* pass */
            if ((nfound != 1LL)) {
                /* pass */
                typepos = (-1LL);
            }
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(nm), true);
        /* pass */
        ({ TrStr _at_t901 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((nm), (_tr_str_lit_len("|", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(arity)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(typepos)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(out, _at_t901); _tr_str_release(_at_t901); });
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(plist);
        _tr_str_release(body);
        _tr_str_release(pt);
    }
    /* pass */
    Dict_free(seen);
    List_TrStr_free(lines);
    return out;
}

__attribute__((hot)) bool _has_cc_error(TrStr errtxt) {
    /* pass */
    return ((_tr_str_index_ofv((errtxt), (_tr_str_lit_len(": error:", 8LL))) >= 0LL) || (_tr_str_index_ofv((errtxt), (_tr_str_lit_len(": error :", 9LL))) >= 0LL));
}

__attribute__((hot)) TrMap* _macro_bad_names(TrStr errtxt) {
    /* pass */
    TrMap* m = _tr_dict_new(64LL);
    /* pass */
    TrStr key = _tr_str_lit_len("in expansion of macro ", 22LL);
    /* pass */
    List_TrStr* lines = _tr_str_splitv((errtxt), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        __auto_type fp = _tr_str_index_ofv((ln), (key));
        /* pass */
        if ((fp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr after = _tr_str_slicev((ln), (fp + _tr_str_lenv((key))), _tr_str_lenv((ln)));
        /* pass */
        __auto_type q1 = _tr_str_index_ofv((after), (_tr_str_lit_len("'", 1LL)));
        /* pass */
        if ((q1 < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest2 = _tr_str_slicev((after), (q1 + 1LL), _tr_str_lenv((after)));
        /* pass */
        __auto_type q2 = _tr_str_index_ofv((rest2), (_tr_str_lit_len("'", 1LL)));
        /* pass */
        if ((q2 < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_slicev((rest2), 0LL, q2);
        /* pass */
        if ((!_tr_str_eqv((nm), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(after);
        _tr_str_release(rest2);
        _tr_str_release(nm);
    }
    /* pass */
    _tr_str_release(key);
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp) {
    /* pass */
    if ((form == 0LL)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void* ", 6LL)), (sym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("){ return (void*)(intptr_t)(", 28LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")); }\n", 6LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), (sym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("){ ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("); }\n", 5LL))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake) {
    /* pass */
    StringBuilder* probe = StringBuilder_init(8192LL);
    /* pass */
    ({ TrStr _sbt_t902 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(probe, _sbt_t902); _tr_str_release(_sbt_t902); });
    /* pass */
    StringBuilder_append(probe, _tr_str_lit_len("#include <stdint.h>\n", 20LL));
    /* pass */
    List_TrStr* order = (void*)List_TrStr_new();
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((!_tr_dict_contains(cand, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        long long tp = (0LL - 1LL);
        /* pass */
        if (bake) {
            /* pass */
            tp = _ent_typepos(ent);
            /* pass */
            if ((tp < 0LL)) {
                /* pass */
                continue;
            }
        }
        /* pass */
        ({ TrStr _at_t903 = (_tr_strx_concatv((_tr_str_lit_len("mP_", 3LL)), (nm))); TrStr _sbt_t904 = (_macro_shim_line(_at_t903, nm, _ent_arity(ent), form, atype, tp)); StringBuilder_append(probe, _sbt_t904); _tr_str_release(_at_t903); _tr_str_release(_sbt_t904); });
        /* pass */
        List_TrStr_append(order, nm);
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    /* pass */
    ({ TrStr _at_t905 = (StringObj_as_str(StringBuilder_to_string(probe))); write_file(_tr_str_lit_len("_macroprobe.c", 13LL), _at_t905); _tr_str_release(_at_t905); });
    /* pass */
    StringBuilder__tr_fn_free(probe);
    return order;
}

__attribute__((hot)) TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    List_TrStr* order = _macro_probe_write(header, macros, cand, form, atype, bake);
    /* pass */
    ({ TrStr _aet_t906 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cxx), (_tr_str_lit_len(" -std=c++17 -fsyntax-only -fmax-errors=0 -fpermissive -w ", 57LL)))); TrStr _cres = _tr_strx_concatv(_cl, (extra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" _macroprobe.c > _macroprobe.err 2>&1", 37LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t906.data); _tr_str_release(_aet_t906); });
    /* pass */
    TrStr errtxt = _tr_str_lit_len("", 0LL);
    /* pass */
    if (file_exists(_tr_str_lit_len("_macroprobe.err", 15LL))) {
        /* pass */
        TrStr _strtmp_t907 = read_file(_tr_str_lit_len("_macroprobe.err", 15LL));
        _tr_str_release(errtxt);
        errtxt = _strtmp_t907;
    }
    /* pass */
    TrMap* bad = _macro_bad_names(errtxt);
    /* pass */
    TrStr key = _tr_str_lit_len("_macroprobe.c:", 14LL);
    /* pass */
    List_TrStr* lines = _tr_str_splitv((errtxt), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len(": error:", 8LL))) < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("is not allowed here", 19LL))) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("expected unqualified-id", 23LL))) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("expected declaration", 20LL))) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type fp = _tr_str_index_ofv((ln), (key));
        /* pass */
        if ((fp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr after = _tr_str_slicev((ln), (fp + _tr_str_lenv((key))), _tr_str_lenv((ln)));
        /* pass */
        __auto_type colon = _tr_str_index_ofv((after), (_tr_str_lit_len(":", 1LL)));
        /* pass */
        if ((colon < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        long long idx = ({ TrStr _at_t908 = (_tr_str_slicev((after), 0LL, colon)); __auto_type _wr = ((_to_int(_at_t908) - 3LL)); _tr_str_release(_at_t908); _wr; });
        /* pass */
        if (((idx >= 0LL) && (idx < order->len))) {
            /* pass */
            ({ TrStr _dkt_t909 = (List_TrStr_get(order, idx)); _tr_dict_set(bad, _tr_strz(_dkt_t909), true); _tr_str_release(_dkt_t909); });
        }
        _tr_str_release(ln);
        _tr_str_release(after);
    }
    /* pass */
    List_TrStr_free(order);
    _tr_str_release(errtxt);
    _tr_str_release(key);
    List_TrStr_free(lines);
    return bad;
}

__attribute__((hot)) TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    TrMap* cur = cand;
    /* pass */
    long long iter = 0LL;
    /* pass */
    while ((iter < 12LL)) {
        /* pass */
        iter = (iter + 1LL);
        /* pass */
        TrMap* bad = _macro_form_bad(header, macros, cur, form, atype, bake, cxx, extra);
        /* pass */
        if ((bad->len == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        TrMap* next = _tr_dict_new(64LL);
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < macros->len)) {
            /* pass */
            TrStr ent = List_TrStr_get(macros, k);
            /* pass */
            k = (k + 1LL);
            /* pass */
            TrStr nm = _ent_name(ent);
            /* pass */
            if ((_tr_dict_contains(cur, _tr_strz(nm)) && (!_tr_dict_contains(bad, _tr_strz(nm))))) {
                /* pass */
                _tr_dict_set(next, _tr_strz(nm), true);
            }
            _tr_str_release(ent);
            _tr_str_release(nm);
        }
        /* pass */
        if ((next->len == cur->len)) {
            /* pass */
            break;
        }
        /* pass */
        cur = next;
        Dict_free(bad);
    }
    /* pass */
    return cur;
}

__attribute__((hot)) TrMap* _macro_remaining(List_TrStr* macros, TrMap* done) {
    /* pass */
    TrMap* m = _tr_dict_new(64LL);
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _tr_str_slicev((ent), 0LL, _tr_str_index_ofv((ent), (_tr_str_lit_len("|", 1LL))));
        /* pass */
        if ((!_tr_dict_contains(done, _tr_strz(nm)))) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    /* pass */
    return m;
}

__attribute__((hot)) void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros) {
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _tr_str_slicev((ent), 0LL, _tr_str_index_ofv((ent), (_tr_str_lit_len("|", 1LL))));
        /* pass */
        if (_tr_dict_contains(ok, _tr_strz(nm))) {
            /* pass */
            _tr_dict_set(done, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    TrMap* cand = _macro_remaining(macros, done);
    /* pass */
    if (bake) {
        /* pass */
        TrMap* c2 = _tr_dict_new(64LL);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < macros->len)) {
            /* pass */
            TrStr e2 = List_TrStr_get(macros, ci);
            /* pass */
            ci = (ci + 1LL);
            /* pass */
            TrStr n2 = _ent_name(e2);
            /* pass */
            if ((_tr_dict_contains(cand, _tr_strz(n2)) && (_ent_typepos(e2) >= 0LL))) {
                /* pass */
                _tr_dict_set(c2, _tr_strz(n2), true);
            }
            _tr_str_release(e2);
            _tr_str_release(n2);
        }
        /* pass */
        cand = c2;
    }
    /* pass */
    TrMap* ok = _macro_verify_form(header, macros, cand, form, atype, bake, cxx, extra);
    /* pass */
    TrStr bk = _tr_str_lit_len("0", 1LL);
    /* pass */
    if (bake) {
        /* pass */
        TrStr _strtmp_t910 = _tr_str_lit_len("1", 1LL);
        _tr_str_release(bk);
        bk = _strtmp_t910;
    }
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((_tr_dict_contains(ok, _tr_strz(nm)) && (!_tr_dict_contains(done, _tr_strz(nm))))) {
            /* pass */
            _tr_dict_set(done, _tr_strz(nm), true);
            /* pass */
            ({ TrStr _dvt_t911 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_str_wrap(_tr_int_to_str((long long)(form)))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(atype)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bk)); _tr_str_release(_cl); _cres; })); _tr_dict_set(plan, _tr_strz(nm), _tr_str_box(_tr_str_retain(_dvt_t911))); _tr_str_release(_dvt_t911); });
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    Dict_free(ok);
    _tr_str_release(bk);
}

__attribute__((hot)) TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra) {
    /* pass */
    if ((macros->len == 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    long long n_total = macros->len;
    /* pass */
    TrMap* done = _tr_dict_new(64LL);
    /* pass */
    TrMap* plan = _tr_dict_new(64LL);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 0LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 0LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 1LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 1LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 0LL, true, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 0LL, true, cxx, extra);
    /* pass */
    _tr_system(_tr_strz(_tr_str_lit_len("rm -f _macroprobe.c _macroprobe.err 2>/dev/null", 47LL)));
    /* pass */
    StringBuilder* shim = StringBuilder_init(8192LL);
    /* pass */
    ({ TrStr _sbt_t912 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("// Auto-generated macro shims for ", 34LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" (tauraro-bindgen --macros).\n", 29LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t912); _tr_str_release(_sbt_t912); });
    /* pass */
    ({ TrStr _sbt_t913 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t913); _tr_str_release(_sbt_t913); });
    /* pass */
    StringBuilder_append(shim, _tr_str_lit_len("#include <stdint.h>\n", 20LL));
    /* pass */
    StringBuilder_append(shim, _tr_str_lit_len("extern \"C\" {\n", 13LL));
    /* pass */
    StringBuilder* binds = StringBuilder_init(4096LL);
    /* pass */
    long long kept = 0LL;
    /* pass */
    long long n_ip = 0LL;
    /* pass */
    long long n_te = 0LL;
    /* pass */
    long long k2 = 0LL;
    /* pass */
    while ((k2 < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k2);
        /* pass */
        k2 = (k2 + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((!_tr_dict_contains(plan, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        long long ar = _ent_arity(ent);
        /* pass */
        List_TrStr* spec = _tr_str_splitv((_tr_str_retain(_tr_str_unbox(_tr_dict_get(plan, _tr_strz(nm))))), (_tr_str_lit_len("|", 1LL)));
        /* pass */
        long long form = ({ TrStr _at_t914 = (List_TrStr_get(spec, 0LL)); __auto_type _wr = (_to_int(_at_t914)); _tr_str_release(_at_t914); _wr; });
        /* pass */
        long long atype = ({ TrStr _at_t915 = (List_TrStr_get(spec, 1LL)); __auto_type _wr = (_to_int(_at_t915)); _tr_str_release(_at_t915); _wr; });
        /* pass */
        long long tp = (0LL - 1LL);
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(spec, 2LL)), (_tr_str_lit_len("1", 1LL)))) {
            /* pass */
            tp = _ent_typepos(ent);
        }
        /* pass */
        ({ TrStr _at_t916 = (_tr_strx_concatv((_tr_str_lit_len("m_", 2LL)), (nm))); TrStr _sbt_t917 = (_macro_shim_line(_at_t916, nm, ar, form, atype, tp)); StringBuilder_append(shim, _sbt_t917); _tr_str_release(_at_t916); _tr_str_release(_sbt_t917); });
        /* pass */
        TrStr bl = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def m_", 10LL)), (nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 2LL, atype, tp)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((form == 0LL)) {
            /* pass */
            TrStr _strtmp_t918 = _tr_strx_concatv((bl), (_tr_str_lit_len(" -> Pointer[void]", 17LL)));
            _tr_str_release(bl);
            bl = _strtmp_t918;
        }
        /* pass */
        ({ TrStr _sbt_t919 = (_tr_strx_concatv((bl), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(binds, _sbt_t919); _tr_str_release(_sbt_t919); });
        /* pass */
        kept = (kept + 1LL);
        /* pass */
        if ((atype == 1LL)) {
            /* pass */
            n_ip = (n_ip + 1LL);
        }
        /* pass */
        if ((tp >= 0LL)) {
            /* pass */
            n_te = (n_te + 1LL);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
        List_TrStr_free(spec);
        _tr_str_release(bl);
    }
    /* pass */
    StringBuilder_append(shim, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    if ((kept == 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    ({ TrStr _at_t920 = (_tr_strx_concatv((out), (_tr_str_lit_len("_macros.c", 9LL)))); TrStr _at_t921 = (StringObj_as_str(StringBuilder_to_string(shim))); write_file(_at_t920, _at_t921); _tr_str_release(_at_t920); _tr_str_release(_at_t921); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(kept)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: macro shims: ", 22LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" of ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_total)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" function-like macros bound (", 29LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_ip)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" intptr, ", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_te)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" type-erased) -> ", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (out)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_macros.c (m_<NAME>)", 20LL))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    StringBuilder__tr_fn_free(shim);
    return StringObj_as_str(StringBuilder_to_string(binds));
}

__attribute__((hot)) void run_bindgen(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs, bool want_macros) {
    /* pass */
    TrStr tmp_decls = _tr_str_lit_len("_bindgen_decls.i", 16LL);
    /* pass */
    TrStr tmp_defs = _tr_str_lit_len("_bindgen_defs.i", 15LL);
    /* pass */
    ({ TrStr _aet_t922 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cc), (extra))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" -E \"", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\" > \"", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp_decls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\" 2>_bindgen_err.txt", 20LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t922.data); _tr_str_release(_aet_t922); });
    /* pass */
    ({ TrStr _aet_t923 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cc), (extra))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" -E -dM \"", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\" > \"", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tmp_defs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\" 2>>_bindgen_err.txt", 21LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t923.data); _tr_str_release(_aet_t923); });
    /* pass */
    if ((!file_exists(tmp_decls))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: preprocessing failed (see _bindgen_err.txt)", 52LL))); printf("\n"); });
        /* pass */
        _tr_str_release(tmp_decls);
        _tr_str_release(tmp_defs);
        return;
    }
    /* pass */
    TrStr src = ({ TrStr _at_t924 = (read_file(tmp_decls)); __auto_type _wr = (_filter_to_target(_at_t924, header)); _tr_str_release(_at_t924); _wr; });
    /* pass */
    List_ptr* toks = tokenize_c(src);
    /* pass */
    Bindgen* bg = Bindgen_init(toks);
    /* pass */
    Bindgen_run(bg);
    /* pass */
    TrMap* baseline = _load_baseline(cc);
    /* pass */
    ({ TrStr _at_t925 = (read_file(tmp_defs)); emit_defines(bg, _at_t925, baseline, _target_define_names(header)); _tr_str_release(_at_t925); });
    /* pass */
    TrStr macro_binds = _tr_str_lit_len("", 0LL);
    /* pass */
    TrStr macro_shim = _tr_str_lit_len("", 0LL);
    /* pass */
    if (want_macros) {
        /* pass */
        TrStr _strtmp_t928 = ({ TrStr _at_t926 = (read_file(tmp_defs)); TrStr _at_t927 = (_cxx_of(cc)); __auto_type _wr = (_gen_macro_shims(header, out, _collect_fn_macros(_at_t926, baseline), _at_t927, extra)); _tr_str_release(_at_t926); _tr_str_release(_at_t927); _wr; });
        _tr_str_release(macro_binds);
        macro_binds = _strtmp_t928;
        /* pass */
        if ((!_tr_str_eqv((macro_binds), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr _strtmp_t929 = ({ TrStr _cl = (_basename(out)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_macros.c", 9LL))); _tr_str_release(_cl); _cres; });
            _tr_str_release(macro_shim);
            macro_shim = _strtmp_t929;
        }
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t930 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# Auto-generated FFI bindings for ", 34LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" (tauraro-bindgen).\n", 20LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t930); _tr_str_release(_sbt_t930); });
    /* pass */
    if ((!_tr_str_eqv((macro_shim), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t931 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-shim: ", 20LL)), (macro_shim))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t931); _tr_str_release(_sbt_t931); });
        /* pass */
        ({ TrStr _sbt_t932 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-cflags: -fpermissive ", 35LL)), (extra))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t932); _tr_str_release(_sbt_t932); });
    }
    /* pass */
    if ((!_tr_str_eqv((pkglibs), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t933 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-linkflags:", 24LL)), (pkglibs))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t933); _tr_str_release(_sbt_t933); });
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("\n", 1LL));
    /* pass */
    TrStr ts = StringObj_as_str(StringBuilder_to_string(bg->types));
    /* pass */
    TrStr ss = StringObj_as_str(StringBuilder_to_string(bg->structs));
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(bg->consts));
    /* pass */
    TrStr fs = StringObj_as_str(StringBuilder_to_string(bg->funcs));
    /* pass */
    if ((!_tr_str_eqv((macro_binds), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        TrStr _strtmp_t934 = _tr_strx_concatv((fs), (macro_binds));
        _tr_str_release(fs);
        fs = _strtmp_t934;
    }
    /* pass */
    TrStr fb = ({ TrStr _at_t935 = (({ TrStr _cl = (_tr_strx_concatv((ts), (ss))); TrStr _cres = _tr_strx_concatv(_cl, (fs)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_opaque_fallbacks(_at_t935, bg->seen)); _tr_str_release(_at_t935); _wr; });
    /* pass */
    if ((!_tr_str_eqv((fb), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        StringBuilder_append(sb, fb);
    }
    /* pass */
    if ((!_tr_str_eqv((ts), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t936 = (_tr_strx_concatv((ts), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t936); _tr_str_release(_sbt_t936); });
    }
    /* pass */
    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        StringBuilder_append(sb, ss);
    }
    /* pass */
    if ((!_tr_str_eqv((cs), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t937 = (_tr_strx_concatv((cs), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t937); _tr_str_release(_sbt_t937); });
    }
    /* pass */
    if ((!_tr_str_eqv((fs), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("extern \"C\":\n", 12LL));
        /* pass */
        StringBuilder_append(sb, fs);
    }
    /* pass */
    TrStr result = StringObj_as_str(StringBuilder_to_string(sb));
    /* pass */
    long long ri = 0LL;
    /* pass */
    long long n_renamed = 0LL;
    /* pass */
    while ((ri < bg->defined->len)) {
        /* pass */
        TrStr dn = List_TrStr_get(bg->defined, ri);
        /* pass */
        ri = (ri + 1LL);
        /* pass */
        if (_tr_dict_contains(bg->skip_syms, _tr_strz(dn))) {
            /* pass */
            TrStr _strtmp_t939 = ({ TrStr _at_t938 = (_tr_strx_concatv((dn), (_tr_str_lit_len("_", 1LL)))); __auto_type _wr = (_rename_word(result, dn, _at_t938)); _tr_str_release(_at_t938); _wr; });
            _tr_str_release(result);
            result = _strtmp_t939;
            /* pass */
            n_renamed = (n_renamed + 1LL);
        }
        _tr_str_release(dn);
    }
    /* pass */
    write_file(out, result);
    /* pass */
    _rm_files(_tr_str_lit_len("_bindgen_decls.i _bindgen_defs.i _bindgen_base.i _bindgen_empty.h _bindgen_err.txt", 82LL));
    /* pass */
    TrStr msg = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("bindgen: wrote ", 15LL)), (out))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" — ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_structs)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" types, ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_funcs)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" functions", 10LL))); _tr_str_release(_cl); _cres; });
    /* pass */
    if ((bg->n_skipped > 0LL)) {
        /* pass */
        TrStr _strtmp_t940 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((msg), (_tr_str_lit_len(" (", 2LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_skipped)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" runtime/libc symbols skipped)", 30LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(msg);
        msg = _strtmp_t940;
    }
    /* pass */
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
    /* pass */
    StringBuilder* sb = StringBuilder_init(8192LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#include <clang-c/Index.h>\n", 27LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#include <stdio.h>\n", 19LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#include <string.h>\n", 20LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("#include <stdlib.h>\n", 20LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static const char* S(CXString s){ const char* p=clang_getCString(s); return p?p:\"\"; }\n", 86LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static const char* scalar(enum CXTypeKind k){\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  switch(k){ case CXType_Bool:return \"bool\";\n", 45LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Char_S:case CXType_Char_U:return \"c_char\"; case CXType_SChar:return \"c_schar\"; case CXType_UChar:return \"c_uchar\";\n", 131LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Char16:return \"c_char16\"; case CXType_Char32:return \"c_char32\"; case CXType_WChar:return \"c_wchar\";\n", 116LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Short:return \"c_short\"; case CXType_UShort:return \"c_ushort\"; case CXType_Int:return \"c_int\"; case CXType_UInt:return \"c_uint\";\n", 144LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Long:return \"c_long\"; case CXType_ULong:return \"c_ulong\"; case CXType_LongLong:return \"c_longlong\"; case CXType_ULongLong:return \"c_ulonglong\";\n", 160LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    case CXType_Float:return \"c_float\"; case CXType_Double:return \"c_double\"; case CXType_LongDouble:return \"c_ldouble\"; default:return \"\"; }\n", 142LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static char g_params[8][64]; static CXType g_args[8]; static int g_np=0;\n", 73LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static char g_cbase[256]={0};   /* container template base (e.g. \"std::map\") for nested-type detection */\n", 106LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_inst=0;            /* 1 while walking template instantiations (--inst mode) */\n", 92LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_hasbegin=0, g_hasend=0;   /* container exposes begin()/end() -> emit ITER helper */\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("/* Collect NON-system external records used by value, so we can lay them out as @value_type\n", 92LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   (a library value struct in a filtered sub-header would otherwise be an opaque handle). System\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   records (windows.h) are left on the bare path — zero risk to the working COM handling. */\n", 95LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static CXCursor g_ext[128]; static char g_extn[128][160]; static int g_next=0;\n", 79LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_ext(CXType rec){\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_inst) return;\n", 21LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor decl=clang_getTypeDeclaration(rec);\n", 47LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXSourceLocation loc=clang_getCursorLocation(decl);\n", 54LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Location_isFromMainFile(loc)) return;   /* main-file records already emitted via CLASS */\n", 101LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Location_isInSystemHeader(loc)) return; /* system record -> bare path (unchanged) */\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_Type_getSizeOf(rec)<=0) return;\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXString sp=clang_getTypeSpelling(rec); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 134LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(strstr(r,\"<\")||strstr(r,\"::\")){ clang_disposeString(sp); return; } /* skip templates / nested (kept simple) */\n", 116LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<g_next;i++) if(strcmp(g_extn[i],r)==0){ clang_disposeString(sp); return; }\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_next<128){ strncpy(g_extn[g_next],r,159); g_extn[g_next][159]=0; g_ext[g_next]=decl; g_next++; }\n", 104LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeString(sp);\n", 27LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void field_tautype(CXType t){\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType c=clang_getCanonicalType(t);\n", 38LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_ConstantArray){ CXType e=clang_getArrayElementType(c); long long n=clang_getArraySize(c);\n", 110LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"[\"); field_tautype(e); printf(\"; %lld]\", n); return; }\n", 67LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Pointer){ printf(\"Pointer[void]\"); return; }\n", 65LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  const char* sc=scalar(c.kind); if(sc[0]){ printf(\"%s\", sc); return; }\n", 72LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Enum){ printf(\"c_int\"); return; }\n", 54LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_Record){ CXString sp=clang_getTypeSpelling(c); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6;\n", 122LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    const char* seg=strrchr(r,':'); printf(\"%s\", seg?seg+1:r); clang_disposeString(sp); return; }\n", 98LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  printf(\"c_int\");\n", 19LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult sfields(CXCursor c, CXCursor p, CXClientData d){\n", 80LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)!=CX_CXXPrivate && clang_getCXXAccessSpecifier(c)!=CX_CXXProtected){\n", 150LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"SFIELD \"); field_tautype(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 112LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue;\n", 32LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_extstructs(void){\n", 35LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<g_next;i++){ printf(\"XSTRUCT %s\\n\", g_extn[i]); clang_visitChildren(g_ext[i], sfields, 0); printf(\"EXSTRUCT\\n\"); }\n", 131LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("/* whole-identifier-token search: does `needle` appear in `hay` bounded by non-ident chars? */\n", 95LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int has_token(const char* hay, const char* needle){\n", 59LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  int nl=strlen(needle); if(nl==0) return 0; const char* p=hay;\n", 64LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  while((p=strstr(p,needle))){ char b=(p==hay)?0:p[-1]; char a=p[nl];\n", 70LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    int lb=!((b>='A'&&b<='Z')||(b>='a'&&b<='z')||(b>='0'&&b<='9')||b=='_');\n", 76LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    int la=!((a>='A'&&a<='Z')||(a>='a'&&a<='z')||(a>='0'&&a<='9')||a=='_');\n", 76LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(lb&&la) return 1; p+=nl; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return 0;\n", 12LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_noncopy;\n", 22LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult copyctorck(CXCursor c, CXCursor p, CXClientData d){\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_Constructor && clang_CXXConstructor_isCopyConstructor(c)){\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    enum CXAvailabilityKind av=clang_getCursorAvailability(c);\n", 63LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    enum CX_CXXAccessSpecifier acc=clang_getCXXAccessSpecifier(c);\n", 67LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(av==CXAvailability_NotAvailable || acc==CX_CXXPrivate || acc==CX_CXXProtected) g_noncopy=1; }\n", 101LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int is_noncopyable(CXType t){\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor dc=clang_getTypeDeclaration(t); CXCursor def=clang_getCursorDefinition(dc); if(clang_Cursor_isNull(def)) def=dc;\n", 124LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  g_noncopy=0; clang_visitChildren(def, copyctorck, 0);\n", 56LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(!g_noncopy){ CXCursor tm=clang_getSpecializedCursorTemplate(dc); if(!clang_Cursor_isNull(tm)){ CXCursor td=clang_getCursorDefinition(tm); if(clang_Cursor_isNull(td)) td=tm; clang_visitChildren(td, copyctorck, 0); } }\n", 222LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return g_noncopy; }\n", 22LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void classify(int depth,int ref,CXType cur){\n", 52LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  { CXString _ps=clang_getTypeSpelling(cur); int _pk=(strstr(clang_getCString(_ps),\"...\")!=0); clang_disposeString(_ps); if(_pk){ printf(\"%d~%d~d~\", depth, ref); return; } }\n", 174LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXTypeKind k=cur.kind; const char* sc=scalar(k);\n", 56LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXType_Void) printf(\"%d~%d~v~\", depth, ref);\n", 53LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(sc[0]) printf(\"%d~%d~p~%s\", depth, ref, sc);\n", 55LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_Enum){ CXType u=clang_getEnumDeclIntegerType(clang_getTypeDeclaration(cur)); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\";\n", 150LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~e~%s#%s\", depth, ref, S(sp), us); clang_disposeString(sp); }\n", 119LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_Record){\n", 29LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(depth==0 && clang_Type_getSizeOf(cur)==CXTypeLayoutError_Incomplete){ CXCursor _td=clang_getTypeDeclaration(cur); CXCursor _tm=clang_getSpecializedCursorTemplate(_td); if(clang_Cursor_isNull(_tm) || clang_Cursor_isNull(clang_getCursorDefinition(_tm))){ printf(\"%d~%d~d~\", depth, ref); return; } }\n", 304LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(depth==0 && is_noncopyable(cur)){ printf(\"%d~%d~d~\", depth, ref); return; }\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(cur); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 136LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(strncmp(r,\"std::function<\",14)==0 && clang_Type_getNumTemplateArguments(cur)>=1){ CXType _ft=clang_Type_getTemplateArgumentAsType(cur,0); CXString _fs=clang_getTypeSpelling(_ft); printf(\"%d~%d~f~%s\", depth, ref, S(_fs)); clang_disposeString(_fs); }\n", 256LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    else if(strncmp(r,\"std::basic_string\",17)==0 && (strstr(r,\"basic_string<char,\")||strstr(r,\"basic_string<char>\")||strstr(r,\"basic_string_view<char,\")||strstr(r,\"basic_string_view<char>\"))){ if(ref==1){ printf(\"%d~%d~d~\", depth, ref); } else { printf(\"%d~%d~s~string\", depth, ref); } }\n", 288LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    else printf(\"%d~%d~r~%s\", depth, ref, r); clang_disposeString(sp); }\n", 73LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXType_FunctionProto){ CXString sp=clang_getTypeSpelling(clang_getCanonicalType(cur)); printf(\"%d~%d~f~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n", 168LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else { CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~u~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n", 117LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void classify_arg(int depth,int ref,CXType t){\n", 54LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType c=clang_getCanonicalType(t);\n", 38LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(c.kind==CXType_LValueReference||c.kind==CXType_RValueReference){\n", 70LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXType pt=clang_getPointeeType(c); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; c=clang_getCanonicalType(pt); }\n", 144LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  classify(depth,ref,c); }\n", 27LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void tds(CXType t){\n", 27LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  // ref: 0 = not-a-reference (value or pointer); 1 = mutable `T&`; 2 = const `const T&`.\n", 90LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  int depth=0,ref=0; CXType cur=t;\n", 35LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  while(1){ CXType cc=clang_getCanonicalType(cur);\n", 51LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(cc.kind==CXType_Pointer){depth++; CXType pt=clang_getPointeeType(cur); if(pt.kind==CXType_Invalid) pt=clang_getPointeeType(cc); cur=pt;}\n", 144LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    else if(cc.kind==CXType_LValueReference||cc.kind==CXType_RValueReference){ CXType pt=clang_getPointeeType(cur); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; cur=pt; }\n", 199LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    else if(cc.kind==CXType_ConstantArray||cc.kind==CXType_IncompleteArray||cc.kind==CXType_VariableArray||cc.kind==CXType_DependentSizedArray){ depth++; cur=clang_getArrayElementType(cc); }\n", 191LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    else break; }\n", 18LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType fcan=clang_getCanonicalType(cur);\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(fcan.kind==CXType_MemberPointer){ printf(\"%d~%d~d~\", depth, ref); return; }\n", 81LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_np>0){\n", 14LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // A template-parameter-dependent type has canonical spelling \"type-parameter-0-<i>\" (even\n", 95LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // when written via a typedef like value_type/reference) — substitute the concrete arg i.\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXString sp=clang_getTypeSpelling(fcan); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n", 137LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(strncmp(r,\"type-parameter-0-\",17)==0){ int idx=atoi(r+17); if(idx>=0 && idx<g_np){ clang_disposeString(sp); classify_arg(depth,ref,g_args[idx]); return; } }\n", 164LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // fallback: match the parameter name literally (direct `T` uses)\n", 70LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    for(int i=0;i<g_np;i++){ if(strcmp(r,g_params[i])==0){ clang_disposeString(sp); classify_arg(depth,ref,g_args[i]); return; } }\n", 131LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // Sequence-container element accessors: `X::reference`/`::const_reference` = `value_type&` and\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // `::pointer`/`::const_pointer` = `value_type*` — libstdc++ routes these through an\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // `__alloc_traits<…>` typedef that stays dependent, but for vector/string/deque/list/set the\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // element type is the FIRST template arg. Resolve to it so at()/front()/back()/[] bind.\n", 93LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    { const char* seg=strrchr(r,':'); if(g_np>0 && seg && seg>r+1 && seg[-1]==':'){ seg++;\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"reference\")==0||strcmp(seg,\"const_reference\")==0){ clang_disposeString(sp); classify(depth+1,1,clang_getCanonicalType(g_args[0])); return; }\n", 164LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"pointer\")==0||strcmp(seg,\"const_pointer\")==0){ clang_disposeString(sp); classify(depth+1,0,clang_getCanonicalType(g_args[0])); return; }\n", 160LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        // container `size_type`/`difference_type` also route through dependent allocator traits.\n", 98LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"size_type\")==0){ printf(\"%d~%d~p~c_size_t\", depth, ref); clang_disposeString(sp); return; }\n", 115LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        if(strcmp(seg,\"difference_type\")==0){ printf(\"%d~%d~p~c_ptrdiff_t\", depth, ref); clang_disposeString(sp); return; } } }\n", 128LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // Any type still carrying an un-substituted template param (self-type copy-ctor\n", 85LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // `vector<_Tp,_Alloc>`, or an internal `__alloc_traits<…>` type — often kind Unexposed,\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // not Record) is dependent -> mark 'd' so the generator skips the whole method (its shim\n", 94LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // would be un-nameable). Applies regardless of kind.\n", 58LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    int dep=has_token(r,\"type-parameter\");\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    for(int i=0;i<g_np && !dep;i++) dep=has_token(r,g_params[i]);\n", 66LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // a NESTED type of the container written without template args (`std::map::value_compare`,\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // an uninstantiated `iterator`) is un-nameable in the shim -> skip the method.\n", 84LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(!dep && g_cbase[0]){ char nb[262]; snprintf(nb,sizeof nb,\"%s::\",g_cbase); if(strstr(r,nb)) dep=1; }\n", 107LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(dep){ printf(\"%d~%d~d~\", depth, ref); clang_disposeString(sp); return; }\n", 80LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_disposeString(sp);\n", 29LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  }\n", 4LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(fcan.kind==CXType_Record && depth==0) collect_ext(fcan);  /* by-value external record -> lay out */\n", 105LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  classify(depth,ref,fcan);\n", 28LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void params(CXCursor c){ int n=clang_Cursor_getNumArguments(c);\n", 71LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<n;i++){ CXCursor a=clang_Cursor_getArgument(c,i); CXString nm=clang_getCursorSpelling(a); const char* nn=S(nm);\n", 128LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"PARAM \"); tds(clang_getArgType(clang_getCursorType(c),i)); if(nn[0])printf(\"|%s\\n\",nn); else printf(\"|a%d\\n\",i); clang_disposeString(nm); } }\n", 154LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("/* A parameter with a default argument has a child that is neither a type/namespace/template ref\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   (those describe the type) — it's the default-value expression. Count the TRAILING defaults (C++\n", 101LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   requires defaults to be trailing) so the generator can emit shorter overloads. */\n", 85LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_hasdef;\n", 21LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult defck(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k!=CXCursor_TypeRef && k!=CXCursor_NamespaceRef && k!=CXCursor_TemplateRef && k!=CXCursor_ParmDecl) g_hasdef=1;\n", 117LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int count_defaults(CXCursor c){ int n=clang_Cursor_getNumArguments(c), d=0;\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=n-1;i>=0;i--){ g_hasdef=0; clang_visitChildren(clang_Cursor_getArgument(c,i), defck, 0); if(g_hasdef) d++; else break; }\n", 133LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return d; }\n", 14LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult fld(CXCursor c, CXCursor p, CXClientData d){\n", 76LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 195LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult gvar(CXCursor c, CXCursor p, CXClientData d){\n", 77LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_VarDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 192LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("/* Inherited-method support: bind a derived class's PUBLIC base methods too (the shim calls them\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   through the derived pointer — public inheritance upcasts). Dedup by name so an override / diamond\n", 103LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   base isn't bound twice; own methods are recorded first so they win. */\n", 74LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static char g_seenm[512][96]; static int g_nseenm=0;\n", 53LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_abstract=0;\n", 25LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static char g_curclass[256]={0};\n", 33LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int seen_method(const char* nm){ for(int i=0;i<g_nseenm;i++) if(strcmp(g_seenm[i],nm)==0) return 1; if(g_nseenm<512){ strncpy(g_seenm[g_nseenm],nm,95); g_seenm[g_nseenm][95]=0; g_nseenm++; } return 0; }\n", 210LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_method(CXCursor c){\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if((k==CXCursor_CXXMethod||k==CXCursor_Constructor) && clang_getCursorAvailability(c)==CXAvailability_NotAvailable) return;\n", 126LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 77LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c)));\n", 173LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    { CXString mn=clang_getCursorSpelling(c); CXString pn=clang_getCursorSpelling(clang_getCursorSemanticParent(c)); const char* pns=S(pn);\n", 140LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(g_curclass[0] && pns[0] && strcmp(pns,g_curclass)!=0) printf(\"|%s|%s\\n\", S(mn), pns); else printf(\"|%s\\n\", S(mn));\n", 124LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(mn); clang_disposeString(pn); }\n", 58LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    params(c); printf(\"EMETHOD\\n\"); }\n", 38LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !g_abstract){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 171LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n", 52LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ownmeth(CXCursor c, CXCursor p, CXClientData d){\n", 80LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) seen_method(S(clang_getCursorSpelling(c)));\n", 120LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod||k==CXCursor_Constructor||k==CXCursor_Destructor) emit_method(c);\n", 93LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_inherited(CXCursor basespec);\n", 47LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult inheritm(CXCursor c, CXCursor p, CXClientData d){\n", 81LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !clang_CXXMethod_isStatic(c)){\n", 109LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    const char* nm=S(clang_getCursorSpelling(c));\n", 50LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(nm[0]!='~' && !seen_method(nm)) emit_method(c); }\n", 57LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n", 107LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_inherited(CXCursor basespec){\n", 47LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType bt=clang_getCursorType(basespec);\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXString bs=clang_getTypeSpelling(bt); const char* br=S(bs); int istmpl=(strstr(br,\"<\")!=NULL); clang_disposeString(bs);\n", 123LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(istmpl) return;                            /* skip template-base inheritance (kept simple) */\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n", 128LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(bdef, inheritm, 0); }\n", 44LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult basespec(CXCursor c, CXCursor p, CXClientData d){\n", 81LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(c)==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n", 123LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_prot_base(CXCursor bs);\n", 44LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult collectprot(CXCursor c, CXCursor p, CXClientData d){\n", 84LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_CXXMethod||k==CXCursor_UsingDeclaration){ enum CX_CXXAccessSpecifier a=clang_getCXXAccessSpecifier(c);\n", 120LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(a==CX_CXXProtected||a==CX_CXXPrivate) seen_method(S(clang_getCursorSpelling(c))); }\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXBaseSpecifier) collect_prot_base(c);\n", 62LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void collect_prot_base(CXCursor bs){\n", 44LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType bt=clang_getCursorType(bs);\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXString sp=clang_getTypeSpelling(bt); const char* r=S(sp); int istmpl=(strstr(r,\"<\")!=NULL); clang_disposeString(sp);\n", 121LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(istmpl) return;\n", 21LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n", 128LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(bdef, collectprot, 0); }\n", 47LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_ntp; static char g_tpname[64];\n", 44LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ftcount(CXCursor cc, CXCursor pp, CXClientData dd){\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter){ g_ntp++; strncpy(g_tpname, S(clang_getCursorSpelling(cc)), 63); g_tpname[63]=0; }\n", 145LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int is_tparam(CXType t){\n", 32LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(t.kind==CXType_LValueReference||t.kind==CXType_RValueReference) t=clang_getPointeeType(t);\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXString s=clang_getTypeSpelling(t); const char* r=S(s); if(strncmp(r,\"const \",6)==0)r+=6;\n", 93LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  int ok=(strcmp(r,g_tpname)==0); clang_disposeString(s); return ok; }\n", 71LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static void emit_ftmpl(CXCursor c, const char* nm){\n", 52LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  g_ntp=0; g_tpname[0]=0; clang_visitChildren(c, ftcount, 0);\n", 62LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(g_ntp!=1 || !g_tpname[0]) return;\n", 39LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXType ft=clang_getCursorType(c);\n", 36LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(!is_tparam(clang_getResultType(ft))) return;\n", 50LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  int np=clang_getNumArgTypes(ft); if(np<0) return;\n", 52LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(int i=0;i<np;i++){ if(!is_tparam(clang_getArgType(ft,i))) return; }\n", 74LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  printf(\"FTMPL %d|%s\\n\", np, nm); }\n", 37LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult _tpk_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind kk=clang_getCursorKind(cc);\n", 48LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(kk==CXCursor_TemplateTypeParameter||kk==CXCursor_NonTypeTemplateParameter||kk==CXCursor_TemplateTemplateParameter){ *(int*)dd=1; return CXChildVisit_Break; }\n", 163LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("/* An uninstantiated primary/partial class template (has template-parameter children) can't be\n", 95LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   bound as a concrete class — its methods would reference the bare template name (`__gmp_expr`\n", 98LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("   without <args>). Only concrete classes and full instantiations are bindable. */\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int is_template_pattern(CXCursor c){ int f=0; clang_visitChildren(c,_tpk_cb,&f); return f; }\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d);\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult nested(CXCursor c, CXCursor p, CXClientData d){\n", 79LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)) || k==CXCursor_EnumDecl) return visit(c,p,d);\n", 130LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(!clang_Location_isFromMainFile(clang_getCursorLocation(c))) return CXChildVisit_Continue;\n", 95LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c); CXString nmS=clang_getCursorSpelling(c); const char* nm=S(nmS);\n", 110LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_ClassTemplate||k==CXCursor_ClassTemplatePartialSpecialization){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 139LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_LinkageSpec){ clang_visitChildren(c,visit,0); clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 121LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_Namespace){ printf(\"NS %s\\n\",nm); clang_visitChildren(c,visit,0); printf(\"ENS\\n\"); }\n", 102LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)){\n", 89LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(is_template_pattern(c)){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 90LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXType rt=clang_getCursorType(c); if(!clang_Cursor_isNull(clang_getSpecializedCursorTemplate(c))){ CXString tsx=clang_getTypeSpelling(rt); printf(\"TCLASS %s %d\\n\", clang_getCString(tsx), clang_isPODType(rt)); clang_disposeString(tsx); } else { printf(\"CLASS %s %d\\n\",nm, clang_isPODType(rt)); }\n", 299LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    int saved=g_nseenm; g_nseenm=0;             /* per-class method-name dedup set */\n", 86LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    int savedabs=g_abstract; g_abstract=clang_CXXRecord_isAbstract(c);\n", 71LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    char savedcc[256]; strncpy(savedcc,g_curclass,255); savedcc[255]=0; strncpy(g_curclass,nm,255); g_curclass[255]=0;\n", 119LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,fld,0);\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,gvar,0);\n", 35LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,ownmeth,0);           /* own methods/ctors/dtor (records names) */\n", 93LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,collectprot,0);       /* exclude names re-declared protected in a base */\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,basespec,0);          /* inherited public methods (deduped) */\n", 89LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_visitChildren(c,nested,0);            /* nested classes + enums */\n", 77LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    g_nseenm=saved; g_abstract=savedabs; strncpy(g_curclass,savedcc,255); g_curclass[255]=0;\n", 93LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"ECLASS\\n\"); }\n", 26LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 82LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EMETHOD\\n\"); }\n", 227LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 156LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n", 52LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_FunctionDecl){ printf(\"FUNC %d \", count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EFUNC\\n\"); }\n", 178LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_VarDecl){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\",nm); }\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_FunctionTemplate) emit_ftmpl(c, nm);\n", 59LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_EnumDecl){ CXType u=clang_getEnumDeclIntegerType(c); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\"; printf(\"ENUM %s %s\\n\", nm[0]?nm:\"anon\", us); clang_visitChildren(c,visit,0); printf(\"EENUM\\n\"); }\n", 226LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_EnumConstantDecl) printf(\"EVAL %s %lld\\n\", nm, (long long)clang_getEnumConstantDeclValue(c));\n", 116LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_TypedefDecl || k==CXCursor_TypeAliasDecl){\n", 65LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // `typedef basic_stringstream<char> stringstream;` — a typedef to a template specialization\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // that no signature references. Emit a TDSPEC hint so bindgen force-instantiates + binds it\n", 97LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    // (fixes template-only headers like <sstream>/<random>). std::string is handled by its typemap.\n", 101LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXType u=clang_getCanonicalType(clang_getTypedefDeclUnderlyingType(c));\n", 76LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(u.kind==CXType_Record && clang_Type_getNumTemplateArguments(u)>0){\n", 74LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      CXString us=clang_getTypeSpelling(u); const char* r=S(us);\n", 65LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(strncmp(r,\"std::basic_string<char\",22)!=0) printf(\"TDSPEC %s\\n\", r);\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(us); } }\n", 35LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeString(nmS); return CXChildVisit_Continue; }\n", 60LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult collectparams(CXCursor cc, CXCursor pp, CXClientData dd){\n", 89LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter && g_np<8){ strncpy(g_params[g_np], S(clang_getCursorSpelling(cc)), 63); g_params[g_np][63]=0; g_np++; }\n", 166LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult imeth(CXCursor c, CXCursor p, CXClientData d){\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind k=clang_getCursorKind(c);\n", 46LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(k==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 77LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n", 101LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n", 82LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    { CXString ms=clang_getCursorSpelling(c); const char* mn=clang_getCString(ms);\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(strcmp(mn,\"begin\")==0) g_hasbegin=1; else if(strcmp(mn,\"end\")==0) g_hasend=1;\n", 87LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      clang_disposeString(ms); }\n", 33LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); params(c); printf(\"EMETHOD\\n\"); }\n", 255LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n", 156LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static int g_ownmc;\n", 20LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult ownmc_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n", 84LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  enum CXCursorKind kk=clang_getCursorKind(cc);\n", 48LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(kk==CXCursor_CXXMethod||kk==CXCursor_Constructor) g_ownmc++;\n", 66LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("static enum CXChildVisitResult inst(CXCursor c, CXCursor p, CXClientData d){\n", 77LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if((clang_getCursorKind(c)==CXCursor_ClassDecl || clang_getCursorKind(c)==CXCursor_StructDecl) && clang_isCursorDefinition(c)){\n", 130LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(is_template_pattern(c)) return CXChildVisit_Continue;   /* partial spec / primary pattern (`__gmp_expr<T,T>`) — not a concrete instantiation */\n", 153LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    CXCursor tmpl=clang_getSpecializedCursorTemplate(c);\n", 57LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(!clang_Cursor_isNull(tmpl)){\n", 36LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // `getSpecializedCursorTemplate` may return a forward DECLARATION of the template (true for\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // std::map/set), which has no member cursors; get the DEFINITION so its methods are visible.\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      CXCursor def=clang_getCursorDefinition(tmpl); if(clang_Cursor_isNull(def)) def=tmpl;\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      CXType ct=clang_getCursorType(c); g_np=0; clang_visitChildren(def, collectparams, 0);\n", 92LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      int na=clang_Type_getNumTemplateArguments(ct); for(int i=0;i<na && i<8;i++) g_args[i]=clang_Type_getTemplateArgumentAsType(ct,i);\n", 136LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      CXString cts=clang_getTypeSpelling(ct); { const char* cs=S(cts); const char* lt=strchr(cs,'<'); int bl=lt?(int)(lt-cs):(int)strlen(cs); if(bl>255)bl=255; memcpy(g_cbase,cs,bl); g_cbase[bl]=0; }\n", 200LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      printf(\"TCLASS %s %d\\n\", S(cts), clang_isPODType(ct)); clang_disposeString(cts);\n", 87LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      g_hasbegin=0; g_hasend=0;\n", 32LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // Explicit specialization (own members>0) OVERRIDES the primary -> bind ITS members\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // (concrete, no substitution); implicit instantiation (0) -> primary + arg substitution.\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      g_ownmc=0; clang_visitChildren(c, ownmc_cb, 0);\n", 54LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      CXCursor msrc = (g_ownmc>0) ? c : def;\n", 45LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      clang_visitChildren(msrc, imeth, 0);\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // Smart pointers: `get()` returns `element_type*` via a metafunction-dependent typedef that\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // libclang won't reduce, and it's inherited from a template base — so synthesize it directly.\n", 103LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      // The element type is the first template arg (a pointer to it). Enables extracting the pointee.\n", 103LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>0 && (strcmp(g_cbase,\"std::shared_ptr\")==0 || strcmp(g_cbase,\"std::unique_ptr\")==0 || strcmp(g_cbase,\"std::weak_ptr\")==0)){\n", 136LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"METHOD .c \"); classify(1,0,clang_getCanonicalType(g_args[0])); printf(\"|get\\n\"); printf(\"EMETHOD\\n\"); }\n", 120LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>0 && g_hasbegin && g_hasend &&\n", 43LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("         (strcmp(g_cbase,\"std::list\")==0||strcmp(g_cbase,\"std::forward_list\")==0||\n", 83LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::set\")==0||strcmp(g_cbase,\"std::multiset\")==0||\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::unordered_set\")==0||strcmp(g_cbase,\"std::unordered_multiset\")==0)){\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITER \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\"); }\n", 90LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      if(na>=2 && g_hasbegin && g_hasend &&\n", 44LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("         (strcmp(g_cbase,\"std::map\")==0||strcmp(g_cbase,\"std::multimap\")==0||\n", 78LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("          strcmp(g_cbase,\"std::unordered_map\")==0||strcmp(g_cbase,\"std::unordered_multimap\")==0)){\n", 99LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITERMAPKEY \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\");\n", 94LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("        printf(\"ITERMAPVAL \"); classify(0,0,clang_getCanonicalType(g_args[1])); printf(\"\\n\"); }\n", 96LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("      printf(\"ECLASS\\n\"); g_np=0; g_cbase[0]=0;\n", 48LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    }\n", 6LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  }\n", 4LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  return CXChildVisit_Continue; }\n", 34LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("int main(int argc,char**argv){\n", 31LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  int inst_mode=0, off=1;\n", 26LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(argc>1 && strcmp(argv[1],\"--inst\")==0){ inst_mode=1; off=2; }\n", 67LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(argc<off+1){ fprintf(stderr,\"usage: cxxwalk [--inst] <header> [clang args...]\\n\"); return 2; }\n", 100LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXIndex idx=clang_createIndex(0,0);\n", 38LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  CXTranslationUnit tu=clang_parseTranslationUnit(idx,argv[off],(const char**)(argv+off+1),argc-off-1,0,0,0);\n", 110LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(!tu){ fprintf(stderr,\"cxxwalk: libclang could not create a translation unit\\n\"); return 1; }\n", 98LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  unsigned nd=clang_getNumDiagnostics(tu), nerr=0, nfatal=0;\n", 61LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  for(unsigned i=0;i<nd;i++){ CXDiagnostic dg=clang_getDiagnostic(tu,i); enum CXDiagnosticSeverity sv=clang_getDiagnosticSeverity(dg);\n", 135LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    if(sv>=CXDiagnostic_Error){ CXString ds=clang_formatDiagnostic(dg,clang_defaultDiagnosticDisplayOptions()); fprintf(stderr,\"DIAG %s\\n\",clang_getCString(ds)); clang_disposeString(ds); nerr++; if(sv==CXDiagnostic_Fatal) nfatal++; }\n", 234LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("    clang_disposeDiagnostic(dg); }\n", 35LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  fprintf(stderr,\"DIAGERR %u %u\\n\", nerr, nfatal);\n", 51LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  g_inst=inst_mode;\n", 20LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_visitChildren(clang_getTranslationUnitCursor(tu), inst_mode?inst:visit, 0);\n", 84LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  if(!inst_mode) emit_extstructs();   /* lay out by-value external (non-system) records */\n", 91LL));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("  clang_disposeTranslationUnit(tu); clang_disposeIndex(idx); return 0; }\n", 73LL));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void _rm_files(TrStr files) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _aet_t941 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("del /q ", 7LL)), (files))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" >nul 2>&1", 10LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t941.data); _tr_str_release(_aet_t941); });
    } else {
        /* pass */
        ({ TrStr _aet_t942 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("rm -f ", 6LL)), (files))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" 2>/dev/null", 12LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t942.data); _tr_str_release(_aet_t942); });
    }
}

__attribute__((hot)) TrStr _local_exe(TrStr stem) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len(".\\", 2LL)), (stem))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".exe", 4LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("./", 2LL)), (stem))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(".exe", 4LL))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr _detect_libclang(TrStr cc) {
    /* pass */
    write_file(_tr_str_lit_len("_cxxprobe.c", 11LL), _tr_str_lit_len("#include <clang-c/Index.h>\nint main(){ clang_createIndex(0,0); return 0; }\n", 75LL));
    /* pass */
    long long rc = ({ TrStr _aet_t943 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" _cxxprobe.c -o _cxxprobe.exe -lclang 2>_cxx_err.txt", 52LL)))); __auto_type _wr = (_tr_system(_aet_t943.data)); _tr_str_release(_aet_t943); _wr; });
    /* pass */
    _rm_files(_tr_str_lit_len("_cxxprobe.c _cxxprobe.exe _cxx_err.txt", 38LL));
    /* pass */
    if ((rc == 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    StringBuilder* g = StringBuilder_init(768LL);
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("bindgen: `-h cpp` needs libclang (Clang's C API), which was not found.\n", 71LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("         It is required ONLY for C++ headers; plain C headers never use it.\n\n", 77LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("Install it:\n", 12LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("  Windows       winget install LLVM.LLVM   (or MSYS2: pacman -S mingw-w64-x86_64-clang)\n", 88LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("  Debian/Ubuntu sudo apt install libclang-dev\n", 46LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("  Fedora        sudo dnf install clang-devel\n", 45LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("  Arch          sudo pacman -S clang\n", 37LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("  macOS         brew install llvm   (or Xcode Command Line Tools)\n\n", 67LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("Download        https://github.com/llvm/llvm-project/releases\n", 62LL));
    /* pass */
    StringBuilder_append(g, _tr_str_lit_len("Then re-run:    tauraroc bindgen <header.hpp> -h cpp\n", 53LL));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(g));
}

__attribute__((hot)) CppType* _cpp_parse_type(TrStr spelling) {
    /* pass */
    CppType* t = ((CppType*)_tr_obj_alloc(sizeof(CppType)));
    /* pass */
    t->base = _tr_str_lit_len("", 0LL);
    /* pass */
    t->ptr = 0LL;
    /* pass */
    t->was_ptr = false;
    /* pass */
    t->was_ref = false;
    /* pass */
    t->is_prim = false;
    /* pass */
    TrStr norm = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_str_lenv((spelling)))) {
        /* pass */
        TrStr ch = _tr_str_slicev((spelling), i, (i + 1LL));
        /* pass */
        if (_tr_str_eqv((ch), (_tr_str_lit_len("*", 1LL)))) {
            /* pass */
            TrStr _strtmp_t944 = _tr_strx_concatv((norm), (_tr_str_lit_len(" * ", 3LL)));
            _tr_str_release(norm);
            norm = _strtmp_t944;
        } else if (_tr_str_eqv((ch), (_tr_str_lit_len("&", 1LL)))) {
            /* pass */
            TrStr _strtmp_t945 = _tr_strx_concatv((norm), (_tr_str_lit_len(" & ", 3LL)));
            _tr_str_release(norm);
            norm = _strtmp_t945;
        } else {
            /* pass */
            TrStr _strtmp_t946 = _tr_strx_concatv((norm), (ch));
            _tr_str_release(norm);
            norm = _strtmp_t946;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(ch);
    }
    /* pass */
    List_TrStr* words = _tr_str_splitv((norm), (_tr_str_lit_len(" ", 1LL)));
    /* pass */
    TrStr bw = _tr_str_lit_len("", 0LL);
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < words->len)) {
        /* pass */
        TrStr w = List_TrStr_get(words, wi);
        /* pass */
        wi = (wi + 1LL);
        /* pass */
        if (_tr_str_eqv((w), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (((((_tr_str_eqv((w), (_tr_str_lit_len("const", 5LL))) || _tr_str_eqv((w), (_tr_str_lit_len("volatile", 8LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("struct", 6LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("class", 5LL)))) || _tr_str_eqv((w), (_tr_str_lit_len("enum", 4LL))))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((w), (_tr_str_lit_len("*", 1LL)))) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ptr = true;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((w), (_tr_str_lit_len("&", 1LL)))) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ref = true;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((bw), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t947 = _tr_str_retain(w);
            _tr_str_release(bw);
            bw = _strtmp_t947;
        } else {
            /* pass */
            TrStr _strtmp_t948 = ({ TrStr _cl = (_tr_strx_concatv((bw), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (w)); _tr_str_release(_cl); _cres; });
            _tr_str_release(bw);
            bw = _strtmp_t948;
        }
        _tr_str_release(w);
    }
    /* pass */
    t->base = _tr_str_retain(bw);
    /* pass */
    TrStr mb = map_base(bw);
    /* pass */
    if ((((!_tr_str_eqv((mb), (bw))) || _tr_str_eqv((bw), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((bw), (_tr_str_lit_len("bool", 4LL))))) {
        /* pass */
        t->is_prim = true;
    }
    /* pass */
    _tr_str_release(norm);
    List_TrStr_free(words);
    _tr_str_release(bw);
    _tr_str_release(mb);
    return t;
}

__attribute__((hot)) TrStr _last_seg(TrStr s) {
    /* pass */
    List_TrStr* parts = _tr_str_splitv((s), (_tr_str_lit_len("::", 2LL)));
    /* pass */
    if ((parts->len == 0LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_retain(s);
    }
    /* pass */
    return List_TrStr_get(parts, (parts->len - 1LL));
}

__attribute__((hot)) TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member) {
    /* pass */
    if ((_tr_str_lenv((mname)) < 9LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    TrStr g = _tr_str_slicev((mname), 8LL, _tr_str_lenv((mname)));
    /* pass */
    char c0 = _tr_strz(g)[0LL];
    /* pass */
    if (((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 32LL))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    bool unary = false;
    /* pass */
    if (is_member) {
        /* pass */
        unary = (nparams == 0LL);
    } else {
        /* pass */
        unary = (nparams <= 1LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("[]", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_index", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("()", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_call", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("->", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_arrow", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("++", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_inc", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("--", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_dec", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("==", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_eq", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("!=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_ne", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("<=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_le", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len(">=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_ge", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("<", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_lt", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len(">", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_gt", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("<<", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_lshift", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len(">>", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_rshift", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("+=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_iadd", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("-=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_isub", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("*=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_imul", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("/=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_idiv", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("%=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_imod", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("&=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_iand", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("|=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_ior", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("^=", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_ixor", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("*", 1LL)))) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit_len("op_deref", 8LL);
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_mul", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("+", 1LL)))) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit_len("op_pos", 6LL);
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_add", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("-", 1LL)))) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit_len("op_neg", 6LL);
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_sub", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("/", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_div", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("%", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_mod", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("&", 1LL)))) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit_len("", 0LL);
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitand", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("|", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitor", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("^", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_xor", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("~", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_bitnot", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("!", 1LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_lnot", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("&&", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_land", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((g), (_tr_str_lit_len("||", 2LL)))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit_len("op_lor", 6LL);
    }
    /* pass */
    _tr_str_release(g);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _cpp_ident(TrStr s) {
    /* pass */
    TrStr out = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_str_lenv((s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
            /* pass */
            TrStr _strtmp_t949 = ({ TrStr _cr = (_tr_str_slicev((s), i, (i + 1LL))); TrStr _cres = _tr_strx_concatv((out), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(out);
            out = _strtmp_t949;
        } else {
            /* pass */
            TrStr _strtmp_t950 = _tr_strx_concatv((out), (_tr_str_lit_len("_", 1LL)));
            _tr_str_release(out);
            out = _strtmp_t950;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (_tr_str_eqv((out), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        _tr_str_release(out);
        return _tr_str_lit_len("T_", 2LL);
    }
    /* pass */
    char c0 = _tr_strz(out)[0LL];
    /* pass */
    if (((c0 >= 48LL) && (c0 <= 57LL))) {
        /* pass */
        return _tr_strx_concatv((_tr_str_lit_len("T_", 2LL)), (out));
    }
    /* pass */
    return out;
}

__attribute__((hot)) bool _is_clean_ident(TrStr s) {
    /* pass */
    if ((_tr_str_lenv((s)) == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    char c0 = _tr_strz(s)[0LL];
    /* pass */
    if ((!((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 95LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 1LL;
    /* pass */
    while ((i < _tr_str_lenv((s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((!(((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool _is_tr_keyword(TrStr n) {
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("in", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("is", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("as", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("and", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("not", 3LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((_tr_str_eqv((n), (_tr_str_lit_len("if", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("else", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("elif", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("for", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("while", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("match", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("case", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("def", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("return", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("class", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("enum", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("type", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("interface", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("mut", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("pub", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("from", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("import", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("pass", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("break", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("continue", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("true", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("false", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("none", 4LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("super", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("with", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("try", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("raise", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("async", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("await", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("var", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("do", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("loop", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("defer", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("actor", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("decorator", 9LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("extend", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("implements", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("extends", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("throws", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("spawn", 5LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _uniq_sym(TrStr base_sym, TrMap* used) {
    /* pass */
    if (_tr_dict_contains(used, _tr_strz(base_sym))) {
        /* pass */
        long long n = (((long long)(uintptr_t)_tr_dict_get(used, _tr_strz(base_sym))) + 1LL);
        /* pass */
        _tr_dict_set(used, _tr_strz(base_sym), n);
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concatv((base_sym), (_tr_str_lit_len("_", 1LL)))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    _tr_dict_set(used, _tr_strz(base_sym), 1LL);
    /* pass */
    return _tr_str_retain(base_sym);
}

__attribute__((hot)) TrStr _cpp_tr_pname(TrStr pname) {
    /* pass */
    if (((_tr_str_eqv((pname), (_tr_str_lit_len("self", 4LL))) || _tr_str_eqv((pname), (_tr_str_lit_len("obj", 3LL)))) || _is_tr_keyword(pname))) {
        /* pass */
        return _tr_strx_concatv((pname), (_tr_str_lit_len("_", 1LL)));
    }
    /* pass */
    return _tr_str_retain(pname);
}

__attribute__((hot)) TrStr _cpp_ctype(CppType* t) {
    /* pass */
    TrStr s = _tr_str_retain(t->base);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < t->ptr)) {
        /* pass */
        TrStr _strtmp_t951 = _tr_strx_concatv((s), (_tr_str_lit_len("*", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t951;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _cpp_tr_type(CppType* t) {
    /* pass */
    if (t->is_prim) {
        /* pass */
        return map_type(t->base, t->ptr);
    }
    /* pass */
    long long eff = t->ptr;
    /* pass */
    if ((eff < 1LL)) {
        /* pass */
        eff = 1LL;
    }
    /* pass */
    TrStr ty = _last_seg(t->base);
    /* pass */
    long long k = 1LL;
    /* pass */
    while ((k < eff)) {
        /* pass */
        TrStr _strtmp_t952 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t952;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    return ty;
}

__attribute__((hot)) List_TrStr* _cpp_ret(CppType* rt, TrStr call) {
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if ((_tr_str_eqv((rt->base), (_tr_str_lit_len("void", 4LL))) && (rt->ptr == 0LL))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("void", 4LL));
        /* pass */
        ({ TrStr _at_t953 = (_tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)))); List_TrStr_append(r, _at_t953); _tr_str_release(_at_t953); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("", 0LL));
        /* pass */
        return r;
    }
    /* pass */
    if (rt->is_prim) {
        /* pass */
        ({ TrStr _at_t954 = (_cpp_ctype(rt)); List_TrStr_append(r, _at_t954); _tr_str_release(_at_t954); });
        /* pass */
        ({ TrStr _at_t955 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t955); _tr_str_release(_at_t955); });
        /* pass */
        ({ TrStr _at_t956 = (_cpp_tr_type(rt)); List_TrStr_append(r, _at_t956); _tr_str_release(_at_t956); });
        /* pass */
        return r;
    }
    /* pass */
    TrStr trret = _cpp_tr_type(rt);
    /* pass */
    if (rt->was_ptr) {
        /* pass */
        ({ TrStr _at_t957 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t957); _tr_str_release(_at_t957); });
        /* pass */
        ({ TrStr _at_t958 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t958); _tr_str_release(_at_t958); });
        /* pass */
        List_TrStr_append(r, trret);
    } else if (rt->was_ref) {
        /* pass */
        ({ TrStr _at_t959 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t959); _tr_str_release(_at_t959); });
        /* pass */
        ({ TrStr _at_t960 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return &(", 9LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t960); _tr_str_release(_at_t960); });
        /* pass */
        List_TrStr_append(r, trret);
    } else {
        /* pass */
        ({ TrStr _at_t961 = (_tr_strx_concatv((rt->base), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t961); _tr_str_release(_at_t961); });
        /* pass */
        ({ TrStr _at_t962 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return new ", 11LL)), (rt->base))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t962); _tr_str_release(_at_t962); });
        /* pass */
        List_TrStr_append(r, trret);
    }
    /* pass */
    _tr_str_release(trret);
    return r;
}

__attribute__((hot)) TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque) {
    /* pass */
    TrStr seg = _last_seg(base);
    /* pass */
    TrStr handle = _tr_str_retain(seg);
    /* pass */
    if ((_is_libc_ty_name(seg) || _is_system_record_ty(seg))) {
        /* pass */
        TrStr _strtmp_t963 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t963;
    } else if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
        /* pass */
        TrStr _strtmp_t964 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t964;
    } else if ((!_tr_dict_contains(class_names, _tr_strz(seg)))) {
        /* pass */
        TrStr _strtmp_t965 = _cpp_ident(base);
        _tr_str_release(handle);
        handle = _strtmp_t965;
        /* pass */
        if (({ TrStr _dkt_t966 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (handle))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t966)))); _tr_str_release(_dkt_t966); _wr; })) {
            /* pass */
            ({ TrStr _sbt_t967 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (handle))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t967); _tr_str_release(_sbt_t967); });
            /* pass */
            ({ TrStr _dkt_t968 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (handle))); _tr_dict_set(seen, _tr_strz(_dkt_t968), true); _tr_str_release(_dkt_t968); });
        }
    }
    /* pass */
    TrStr ty = _tr_str_retain(handle);
    /* pass */
    long long k = 1LL;
    /* pass */
    if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
        /* pass */
        k = 0LL;
    }
    /* pass */
    while ((k < nd)) {
        /* pass */
        TrStr _strtmp_t969 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (ty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t969;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    _tr_str_release(seg);
    _tr_str_release(handle);
    return ty;
}

__attribute__((hot)) TrStr _cpp_qual(TrStr base, TrMap* class_qual) {
    /* pass */
    TrStr seg = _last_seg(base);
    /* pass */
    if (_tr_dict_contains(class_qual, _tr_strz(seg))) {
        /* pass */
        return _tr_str_retain(_tr_str_unbox(_tr_dict_get(class_qual, _tr_strz(seg))));
    }
    /* pass */
    _tr_str_release(seg);
    return _tr_str_retain(base);
}

__attribute__((hot)) List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque) {
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* d4 = _desc4(desc);
    /* pass */
    long long rdepth = ({ TrStr _at_t970 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t970)); _tr_str_release(_at_t970); _wr; });
    /* pass */
    bool rref = (!_tr_str_eqv((List_TrStr_get(d4, 1LL)), (_tr_str_lit_len("0", 1LL))));
    /* pass */
    TrStr cat = List_TrStr_get(d4, 2LL);
    /* pass */
    TrStr detail = List_TrStr_get(d4, 3LL);
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("v", 1LL)))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("void", 4LL));
        /* pass */
        ({ TrStr _at_t971 = (_tr_strx_concatv((call), (_tr_str_lit_len(";", 1LL)))); List_TrStr_append(r, _at_t971); _tr_str_release(_at_t971); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("", 0LL));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("p", 1LL)))) {
        /* pass */
        TrStr cpp = _c_to_cpp(detail);
        /* pass */
        if ((rdepth == 0LL)) {
            /* pass */
            List_TrStr_append(r, cpp);
            /* pass */
            ({ TrStr _at_t972 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t972); _tr_str_release(_at_t972); });
            /* pass */
            List_TrStr_append(r, detail);
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(cpp);
            return r;
        }
        /* pass */
        TrStr cty = ({ TrStr _cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cr = (_stars(rdepth)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        /* pass */
        if (rref) {
            /* pass */
            List_TrStr_append(r, cty);
            /* pass */
            ({ TrStr _at_t973 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")(&(", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("));", 3LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t973); _tr_str_release(_at_t973); });
            /* pass */
            ({ TrStr _at_t974 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t974); _tr_str_release(_at_t974); });
        } else {
            /* pass */
            List_TrStr_append(r, cty);
            /* pass */
            ({ TrStr _at_t975 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")(", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t975); _tr_str_release(_at_t975); });
            /* pass */
            ({ TrStr _at_t976 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t976); _tr_str_release(_at_t976); });
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(cpp);
        _tr_str_release(cty);
        return r;
    }
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("e", 1LL)))) {
        /* pass */
        TrStr espell = List_TrStr_get(_tr_str_splitv((detail), (_tr_str_lit_len("#", 1LL))), 0LL);
        /* pass */
        TrStr ealias = _last_seg(espell);
        /* pass */
        if ((rdepth == 0LL)) {
            /* pass */
            List_TrStr_append(r, espell);
            /* pass */
            ({ TrStr _at_t977 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t977); _tr_str_release(_at_t977); });
            /* pass */
            List_TrStr_append(r, ealias);
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(espell);
            _tr_str_release(ealias);
            return r;
        }
        /* pass */
        if (rref) {
            /* pass */
            ({ TrStr _at_t978 = (_tr_strx_concatv((espell), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t978); _tr_str_release(_at_t978); });
            /* pass */
            ({ TrStr _at_t979 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return &(", 9LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t979); _tr_str_release(_at_t979); });
            /* pass */
            ({ TrStr _at_t980 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t980); _tr_str_release(_at_t980); });
        } else {
            /* pass */
            ({ TrStr _at_t981 = (_tr_strx_concatv((espell), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t981); _tr_str_release(_at_t981); });
            /* pass */
            ({ TrStr _at_t982 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t982); _tr_str_release(_at_t982); });
            /* pass */
            ({ TrStr _at_t983 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t983); _tr_str_release(_at_t983); });
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(espell);
        _tr_str_release(ealias);
        return r;
    }
    /* pass */
    if ((_tr_str_eqv((cat), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((detail), (_tr_str_lit_len("string", 6LL))))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("char*", 5LL));
        /* pass */
        ({ TrStr _at_t984 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return _tr_cpp_strdup(", 22LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t984); _tr_str_release(_at_t984); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("Pointer[char]", 13LL));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("f", 1LL)))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("void*", 5LL));
        /* pass */
        ({ TrStr _at_t985 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (void*)(", 15LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t985); _tr_str_release(_at_t985); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("Pointer[void]", 13LL));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    TrStr seg = _last_seg(detail);
    /* pass */
    if (((_tr_str_eqv((cat), (_tr_str_lit_len("r", 1LL))) && _tr_dict_contains(value_structs, _tr_strz(seg))) && (rdepth == 0LL))) {
        /* pass */
        ({ TrStr _at_t986 = (_cpp_qual(detail, class_qual)); List_TrStr_append(r, _at_t986); _tr_str_release(_at_t986); });
        /* pass */
        ({ TrStr _at_t987 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t987); _tr_str_release(_at_t987); });
        /* pass */
        List_TrStr_append(r, seg);
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(seg);
        return r;
    }
    /* pass */
    long long nd = rdepth;
    /* pass */
    if ((nd < 1LL)) {
        /* pass */
        nd = 1LL;
    }
    /* pass */
    TrStr handle = _cpp_opaque_handle(detail, nd, class_names, value_structs, seen, opaque);
    /* pass */
    TrStr cb = _cpp_qual(detail, class_qual);
    /* pass */
    if ((rdepth == 0LL)) {
        /* pass */
        ({ TrStr _at_t988 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t988); _tr_str_release(_at_t988); });
        /* pass */
        ({ TrStr _at_t989 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return new ", 11LL)), (cb))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t989); _tr_str_release(_at_t989); });
        /* pass */
        List_TrStr_append(r, handle);
    } else if (rref) {
        /* pass */
        ({ TrStr _at_t990 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t990); _tr_str_release(_at_t990); });
        /* pass */
        ({ TrStr _at_t991 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cb))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)(&(", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("));", 3LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t991); _tr_str_release(_at_t991); });
        /* pass */
        List_TrStr_append(r, handle);
    } else {
        /* pass */
        ({ TrStr _at_t992 = (_tr_strx_concatv((cb), (_tr_str_lit_len("*", 1LL)))); List_TrStr_append(r, _at_t992); _tr_str_release(_at_t992); });
        /* pass */
        ({ TrStr _at_t993 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return (", 8LL)), (cb))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("*)(", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(");", 2LL))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t993); _tr_str_release(_at_t993); });
        /* pass */
        List_TrStr_append(r, handle);
    }
    /* pass */
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    _tr_str_release(seg);
    _tr_str_release(handle);
    _tr_str_release(cb);
    return r;
}

__attribute__((hot)) TrStr _ns_pop(TrStr path) {
    /* pass */
    List_TrStr* parts = _tr_str_splitv((path), (_tr_str_lit_len("::", 2LL)));
    /* pass */
    if ((parts->len <= 1LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    TrStr r = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (parts->len - 1LL))) {
        /* pass */
        if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t994 = List_TrStr_get(parts, i);
            _tr_str_release(r);
            r = _strtmp_t994;
        } else {
            /* pass */
            TrStr _strtmp_t995 = ({ TrStr _cl = (_tr_strx_concatv((r), (_tr_str_lit_len("::", 2LL)))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(r);
            r = _strtmp_t995;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_free(parts);
    return r;
}

__attribute__((hot)) TrStr _ns_us(TrStr path) {
    /* pass */
    List_TrStr* parts = _tr_str_splitv((path), (_tr_str_lit_len("::", 2LL)));
    /* pass */
    TrStr r = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((!_tr_str_eqv((List_TrStr_get(parts, i)), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                TrStr _strtmp_t996 = List_TrStr_get(parts, i);
                _tr_str_release(r);
                r = _strtmp_t996;
            } else {
                /* pass */
                TrStr _strtmp_t997 = ({ TrStr _cl = (_tr_strx_concatv((r), (_tr_str_lit_len("_", 1LL)))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(r);
                r = _strtmp_t997;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_free(parts);
    return r;
}

__attribute__((hot)) TrStr _rstrip_cr(TrStr s) {
    /* pass */
    long long n = _tr_str_lenv((s));
    /* pass */
    while (((n > 0LL) && _tr_str_eqv((_tr_str_slicev((s), (n - 1LL), n)), (_tr_str_lit_len("\r", 1LL))))) {
        /* pass */
        n = (n - 1LL);
    }
    /* pass */
    if ((n == _tr_str_lenv((s)))) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    return _tr_str_slicev((s), 0LL, n);
}

__attribute__((hot)) TrStr _c_to_cpp(TrStr cn) {
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("bool", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int", 5LL)))) {
        /* pass */
        return _tr_str_lit_len("int", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned int", 12LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("char", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_schar", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("signed char", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uchar", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned char", 13LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_wchar", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("wchar_t", 7LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char16", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("char16_t", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_char32", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("char32_t", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_short", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("short", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ushort", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned short", 14LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_long", 6LL)))) {
        /* pass */
        return _tr_str_lit_len("long", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ulong", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned long", 13LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_longlong", 10LL)))) {
        /* pass */
        return _tr_str_lit_len("long long", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ulonglong", 11LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned long long", 18LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_float", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("float", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_double", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("double", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ldouble", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("long double", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_size_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("size_t", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ssize_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("ptrdiff_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_ptrdiff_t", 11LL)))) {
        /* pass */
        return _tr_str_lit_len("ptrdiff_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_intptr_t", 10LL)))) {
        /* pass */
        return _tr_str_lit_len("intptr_t", 8LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uintptr_t", 11LL)))) {
        /* pass */
        return _tr_str_lit_len("uintptr_t", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int8_t", 8LL)))) {
        /* pass */
        return _tr_str_lit_len("signed char", 11LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint8_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned char", 13LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int16_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("short", 5LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint16_t", 10LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned short", 14LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int32_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("int", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint32_t", 10LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned int", 12LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_int64_t", 9LL)))) {
        /* pass */
        return _tr_str_lit_len("long long", 9LL);
    }
    /* pass */
    if (_tr_str_eqv((cn), (_tr_str_lit_len("c_uint64_t", 10LL)))) {
        /* pass */
        return _tr_str_lit_len("unsigned long long", 18LL);
    }
    /* pass */
    return _tr_str_retain(cn);
}

__attribute__((hot)) List_TrStr* _desc4(TrStr desc) {
    /* pass */
    List_TrStr* parts = _tr_str_splitv((desc), (_tr_str_lit_len("~", 1LL)));
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if ((parts->len < 3LL)) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("u", 1LL));
        /* pass */
        List_TrStr_append(r, desc);
        /* pass */
        List_TrStr_free(parts);
        return r;
    }
    /* pass */
    ({ TrStr _at_t998 = (List_TrStr_get(parts, 0LL)); List_TrStr_append(r, _at_t998); _tr_str_release(_at_t998); });
    /* pass */
    ({ TrStr _at_t999 = (List_TrStr_get(parts, 1LL)); List_TrStr_append(r, _at_t999); _tr_str_release(_at_t999); });
    /* pass */
    ({ TrStr _at_t1000 = (List_TrStr_get(parts, 2LL)); List_TrStr_append(r, _at_t1000); _tr_str_release(_at_t1000); });
    /* pass */
    TrStr det = _tr_str_lit_len("", 0LL);
    /* pass */
    if ((parts->len > 3LL)) {
        /* pass */
        TrStr _strtmp_t1001 = List_TrStr_get(parts, 3LL);
        _tr_str_release(det);
        det = _strtmp_t1001;
    }
    /* pass */
    long long k = 4LL;
    /* pass */
    while ((k < parts->len)) {
        /* pass */
        TrStr _strtmp_t1002 = ({ TrStr _cl = (_tr_strx_concatv((det), (_tr_str_lit_len("~", 1LL)))); TrStr _cr = (List_TrStr_get(parts, k)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(det);
        det = _strtmp_t1002;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    List_TrStr_append(r, det);
    /* pass */
    List_TrStr_free(parts);
    _tr_str_release(det);
    return r;
}

__attribute__((hot)) List_TrStr* _parse_tclass(TrStr rest) {
    /* pass */
    List_TrStr* parts = _tr_str_splitv((rest), (_tr_str_lit_len(" ", 1LL)));
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if ((parts->len < 2LL)) {
        /* pass */
        List_TrStr_append(r, rest);
        /* pass */
        List_TrStr_append(r, _tr_str_lit_len("0", 1LL));
        /* pass */
        List_TrStr_free(parts);
        return r;
    }
    /* pass */
    TrStr pod = List_TrStr_get(parts, (parts->len - 1LL));
    /* pass */
    TrStr sp = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (parts->len - 1LL))) {
        /* pass */
        if (_tr_str_eqv((sp), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t1003 = List_TrStr_get(parts, i);
            _tr_str_release(sp);
            sp = _strtmp_t1003;
        } else {
            /* pass */
            TrStr _strtmp_t1004 = ({ TrStr _cl = (_tr_strx_concatv((sp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(sp);
            sp = _strtmp_t1004;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(r, sp);
    /* pass */
    List_TrStr_append(r, pod);
    /* pass */
    List_TrStr_free(parts);
    _tr_str_release(pod);
    _tr_str_release(sp);
    return r;
}

__attribute__((hot)) TrStr _stars(long long n) {
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t1005 = _tr_strx_concatv((s), (_tr_str_lit_len("*", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t1005;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _ptr_wrap(TrStr inner, long long n) {
    /* pass */
    TrStr t = _tr_str_retain(inner);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t1006 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("Pointer[", 8LL)), (t))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_cl); _cres; });
        _tr_str_release(t);
        t = _strtmp_t1006;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return t;
}

__attribute__((hot)) TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names) {
    /* pass */
    List_TrStr* d4 = _desc4(desc);
    /* pass */
    long long depth = ({ TrStr _at_t1007 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t1007)); _tr_str_release(_at_t1007); _wr; });
    /* pass */
    TrStr cat = List_TrStr_get(d4, 2LL);
    /* pass */
    TrStr detail = List_TrStr_get(d4, 3LL);
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("p", 1LL)))) {
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            return detail;
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        return _ptr_wrap(detail, depth);
    }
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("e", 1LL)))) {
        /* pass */
        TrStr ealias = ({ TrStr _at_t1008 = (List_TrStr_get(_tr_str_splitv((detail), (_tr_str_lit_len("#", 1LL))), 0LL)); __auto_type _wr = (_last_seg(_at_t1008)); _tr_str_release(_at_t1008); _wr; });
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            return ealias;
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _ptr_wrap(ealias, depth);
    }
    /* pass */
    if (_tr_str_eqv((cat), (_tr_str_lit_len("r", 1LL)))) {
        /* pass */
        TrStr seg = _last_seg(detail);
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
                /* pass */
                List_TrStr_free(d4);
                _tr_str_release(cat);
                _tr_str_release(detail);
                return seg;
            }
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(seg);
            return _tr_str_lit_len("Pointer[void]", 13LL);
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _ptr_wrap(seg, depth);
    }
    /* pass */
    if ((depth == 0LL)) {
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _tr_str_lit_len("Pointer[void]", 13LL);
    }
    /* pass */
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    return _ptr_wrap(_tr_str_lit_len("void", 4LL), depth);
}

__attribute__((hot)) TrStr _shim_body(TrStr ret_ctype, TrStr body) {
    /* pass */
    TrStr zero = _tr_str_lit_len("return {};", 10LL);
    /* pass */
    if (_tr_str_eqv((ret_ctype), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        TrStr _strtmp_t1009 = _tr_str_lit_len("return;", 7LL);
        _tr_str_release(zero);
        zero = _strtmp_t1009;
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("try { ", 6LL)), (body))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" } catch (const std::exception& e) { _tr_cpp_set_error(e.what()); ", 66LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (zero)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" } catch (...) { _tr_cpp_set_error(\"C++ exception\"); ", 53LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (zero)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }", 2LL))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr _fnptr_cast(TrStr fnty) {
    /* pass */
    __auto_type p = _tr_str_index_ofv((fnty), (_tr_str_lit_len("(", 1LL)));
    /* pass */
    if ((p < 0LL)) {
        /* pass */
        return _tr_str_retain(fnty);
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (_tr_str_slicev((fnty), 0LL, p)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(*)", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_slicev((fnty), p, _tr_str_lenv((fnty)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs) {
    /* pass */
    StringBuilder* opaque = StringBuilder_init(512LL);
    /* pass */
    StringBuilder* consts = StringBuilder_init(512LL);
    /* pass */
    StringBuilder* decls = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder* shim = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder* usings = StringBuilder_init(256LL);
    /* pass */
    TrMap* seen = _tr_dict_new(64LL);
    /* pass */
    TrStr ns_path = _tr_str_lit_len("", 0LL);
    /* pass */
    long long n_classes = 0LL;
    /* pass */
    long long n_fns = 0LL;
    /* pass */
    TrMap* sym_used = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* cstk_name = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* cstk_qual = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* cstk_pfx = (void*)List_TrStr_new();
    /* pass */
    TrStr fld_buf = _tr_str_lit_len("", 0LL);
    /* pass */
    List_TrStr* fld_stack = (void*)List_TrStr_new();
    /* pass */
    TrMap* enum_names = _tr_dict_new(32LL);
    /* pass */
    TrMap* class_names = _tr_dict_new(32LL);
    /* pass */
    TrMap* value_structs = _tr_dict_new(32LL);
    /* pass */
    TrMap* class_qual = _tr_dict_new(32LL);
    /* pass */
    List_TrStr* ps_name = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* ps_pod = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* ps_nf = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* plines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long pli = 0LL;
    /* pass */
    while ((pli < plines->len)) {
        /* pass */
        TrStr pl = ({ TrStr _at_t1010 = (List_TrStr_get(plines, pli)); __auto_type _wr = (_rstrip_cr(_at_t1010)); _tr_str_release(_at_t1010); _wr; });
        /* pass */
        pli = (pli + 1LL);
        /* pass */
        if (_tr_str_starts_withv((pl), (_tr_str_lit_len("ENUM ", 5LL)))) {
            /* pass */
            TrStr erest = _tr_str_slicev((pl), 5LL, _tr_str_lenv((pl)));
            /* pass */
            TrStr ename0 = List_TrStr_get(_tr_str_splitv((erest), (_tr_str_lit_len(" ", 1LL))), 0LL);
            /* pass */
            if ((!_tr_str_eqv((ename0), (_tr_str_lit_len("anon", 4LL))))) {
                /* pass */
                _tr_dict_set(enum_names, _tr_strz(ename0), true);
            }
            _tr_str_release(erest);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("CLASS ", 6LL)))) {
            /* pass */
            TrStr crest = _tr_str_slicev((pl), 6LL, _tr_str_lenv((pl)));
            /* pass */
            List_TrStr* cparts = _tr_str_splitv((crest), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            TrStr cnm = List_TrStr_get(cparts, 0LL);
            /* pass */
            TrStr cpod = _tr_str_lit_len("0", 1LL);
            /* pass */
            if ((cparts->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1011 = List_TrStr_get(cparts, 1LL);
                _tr_str_release(cpod);
                cpod = _strtmp_t1011;
            }
            /* pass */
            _tr_dict_set(class_names, _tr_strz(cnm), true);
            /* pass */
            List_TrStr_append(ps_name, cnm);
            /* pass */
            List_TrStr_append(ps_pod, cpod);
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            _tr_str_release(crest);
            _tr_str_release(cnm);
            _tr_str_release(cpod);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("TCLASS ", 7LL)))) {
            /* pass */
            List_TrStr* tp = ({ TrStr _at_t1012 = (_tr_str_slicev((pl), 7LL, _tr_str_lenv((pl)))); __auto_type _wr = (_parse_tclass(_at_t1012)); _tr_str_release(_at_t1012); _wr; });
            /* pass */
            TrStr tnm = ({ TrStr _at_t1013 = (List_TrStr_get(tp, 0LL)); __auto_type _wr = (_cpp_ident(_at_t1013)); _tr_str_release(_at_t1013); _wr; });
            /* pass */
            _tr_dict_set(class_names, _tr_strz(tnm), true);
            /* pass */
            List_TrStr_append(ps_name, tnm);
            /* pass */
            ({ TrStr _at_t1014 = (List_TrStr_get(tp, 1LL)); List_TrStr_append(ps_pod, _at_t1014); _tr_str_release(_at_t1014); });
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            List_TrStr_free(tp);
            _tr_str_release(tnm);
        } else if (_tr_str_starts_withv((pl), (_tr_str_lit_len("XSTRUCT ", 8LL)))) {
            /* pass */
            TrStr xn0 = _tr_str_slicev((pl), 8LL, _tr_str_lenv((pl)));
            /* pass */
            _tr_dict_set(class_names, _tr_strz(xn0), true);
            /* pass */
            List_TrStr_append(ps_name, xn0);
            /* pass */
            List_TrStr_append(ps_pod, _tr_str_lit_len("1", 1LL));
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit_len("0", 1LL));
            _tr_str_release(xn0);
        } else if ((_tr_str_starts_withv((pl), (_tr_str_lit_len("FIELD ", 6LL))) || _tr_str_starts_withv((pl), (_tr_str_lit_len("SFIELD ", 7LL))))) {
            /* pass */
            if ((ps_nf->len > 0LL)) {
                /* pass */
                List_TrStr_pop(ps_nf);
                /* pass */
                List_TrStr_append(ps_nf, _tr_str_lit_len("1", 1LL));
            }
        } else if ((_tr_str_eqv((pl), (_tr_str_lit_len("ECLASS", 6LL))) || _tr_str_eqv((pl), (_tr_str_lit_len("EXSTRUCT", 8LL))))) {
            /* pass */
            if ((ps_name->len > 0LL)) {
                /* pass */
                TrStr nm2 = List_TrStr_pop(ps_name);
                /* pass */
                TrStr pod2 = List_TrStr_pop(ps_pod);
                /* pass */
                TrStr nf2 = List_TrStr_pop(ps_nf);
                /* pass */
                if ((_tr_str_eqv((pod2), (_tr_str_lit_len("1", 1LL))) && _tr_str_eqv((nf2), (_tr_str_lit_len("1", 1LL))))) {
                    /* pass */
                    _tr_dict_set(value_structs, _tr_strz(nm2), true);
                }
                _tr_str_release(pod2);
                _tr_str_release(nf2);
            }
        }
        _tr_str_release(pl);
    }
    /* pass */
    TrStr mode = _tr_str_lit_len("", 0LL);
    /* pass */
    TrStr m_flags = _tr_str_lit_len("..", 2LL);
    /* pass */
    TrStr m_ret = _tr_str_lit_len("", 0LL);
    /* pass */
    TrStr m_name = _tr_str_lit_len("", 0LL);
    /* pass */
    TrStr m_qual = _tr_str_lit_len("", 0LL);
    /* pass */
    long long m_ndef = 0LL;
    /* pass */
    List_TrStr* params = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr line = ({ TrStr _at_t1015 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1015)); _tr_str_release(_at_t1015); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("TDSPEC ", 7LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr cur_class = _tr_str_lit_len("", 0LL);
        /* pass */
        TrStr cur_class_qual = _tr_str_lit_len("", 0LL);
        /* pass */
        TrStr class_pfx = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((cstk_name->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t1016 = List_TrStr_get(cstk_name, (cstk_name->len - 1LL));
            _tr_str_release(cur_class);
            cur_class = _strtmp_t1016;
            /* pass */
            TrStr _strtmp_t1017 = List_TrStr_get(cstk_qual, (cstk_qual->len - 1LL));
            _tr_str_release(cur_class_qual);
            cur_class_qual = _strtmp_t1017;
            /* pass */
            TrStr _strtmp_t1018 = List_TrStr_get(cstk_pfx, (cstk_pfx->len - 1LL));
            _tr_str_release(class_pfx);
            class_pfx = _strtmp_t1018;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("CTOR", 4LL)))) {
            /* pass */
            m_ndef = 0LL;
            /* pass */
            if ((_tr_str_lenv((line)) > 5LL)) {
                /* pass */
                m_ndef = ({ TrStr _at_t1019 = (_tr_str_slicev((line), 5LL, _tr_str_lenv((line)))); __auto_type _wr = (_to_int(_at_t1019)); _tr_str_release(_at_t1019); _wr; });
            }
            /* pass */
            TrStr _strtmp_t1020 = _tr_str_lit_len("ctor", 4LL);
            _tr_str_release(mode);
            mode = _strtmp_t1020;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("DTOR", 4LL)))) {
            /* pass */
            if ((!_is_clean_ident(cur_class))) {
                /* pass */
                continue;
            }
            /* pass */
            TrStr ds = ({ TrStr _at_t1021 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_delete", 7LL)))); __auto_type _wr = (_uniq_sym(_at_t1021, sym_used)); _tr_str_release(_at_t1021); _wr; });
            /* pass */
            ({ TrStr _sbt_t1022 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), (ds))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("* self) { ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_tr_str_lit_len("void", 4LL), _tr_str_lit_len("delete self;", 12LL))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1022); _tr_str_release(_sbt_t1022); });
            /* pass */
            ({ TrStr _sbt_t1023 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (ds))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1023); _tr_str_release(_sbt_t1023); });
            /* pass */
            n_fns = (n_fns + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("ECLASS", 6LL)))) {
            /* pass */
            if (({ TrStr _dkt_t1024 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); __auto_type _wr = ((_is_clean_ident(cur_class) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1024))))); _tr_str_release(_dkt_t1024); _wr; })) {
                /* pass */
                if ((_tr_dict_contains(value_structs, _tr_strz(cur_class)) && (!_tr_str_eqv((fld_buf), (_tr_str_lit_len("", 0LL)))))) {
                    /* pass */
                    ({ TrStr _sbt_t1025 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (cur_class))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fld_buf)); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1025); _tr_str_release(_sbt_t1025); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t1026 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (cur_class))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1026); _tr_str_release(_sbt_t1026); });
                }
                /* pass */
                ({ TrStr _dkt_t1027 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t1027), true); _tr_str_release(_dkt_t1027); });
                /* pass */
                n_classes = (n_classes + 1LL);
            }
            /* pass */
            if ((cstk_name->len > 0LL)) {
                /* pass */
                List_TrStr_pop(cstk_name);
                /* pass */
                List_TrStr_pop(cstk_qual);
                /* pass */
                List_TrStr_pop(cstk_pfx);
            }
            /* pass */
            if ((fld_stack->len > 0LL)) {
                /* pass */
                TrStr _strtmp_t1028 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1028;
            } else {
                /* pass */
                TrStr _strtmp_t1029 = _tr_str_lit_len("", 0LL);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1029;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FIELD ", 6LL)))) {
            /* pass */
            TrStr frest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* fbar = _tr_str_splitv((frest), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            TrStr fdesc = List_TrStr_get(fbar, 0LL);
            /* pass */
            TrStr fnm = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((fbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1030 = List_TrStr_get(fbar, 1LL);
                _tr_str_release(fnm);
                fnm = _strtmp_t1030;
            }
            /* pass */
            if (_is_clean_ident(fnm)) {
                /* pass */
                if (_tr_dict_contains(value_structs, _tr_strz(cur_class))) {
                    /* pass */
                    TrStr fmap = _cpp_field_type(fdesc, value_structs, enum_names);
                    /* pass */
                    TrStr _strtmp_t1031 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fld_buf), (_tr_str_lit_len("    pub ", 8LL)))); TrStr _cr = (_cpp_tr_pname(fnm)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fmap)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(fld_buf);
                    fld_buf = _strtmp_t1031;
                    _tr_str_release(fmap);
                } else if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((fdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((fdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
                    /* pass */
                    List_TrStr* fri = ({ TrStr _at_t1032 = (_tr_strx_concatv((_tr_str_lit_len("self->", 6LL)), (fnm))); __auto_type _wr = (_cpp_ret_ex(fdesc, _at_t1032, value_structs, class_names, class_qual, seen, opaque)); _tr_str_release(_at_t1032); _wr; });
                    /* pass */
                    TrStr fsym = ({ TrStr _at_t1033 = (({ TrStr _cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fnm)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_uniq_sym(_at_t1033, sym_used)); _tr_str_release(_at_t1033); _wr; });
                    /* pass */
                    ({ TrStr _at_t1034 = (List_TrStr_get(fri, 0LL)); TrStr _at_t1035 = (List_TrStr_get(fri, 1LL)); TrStr _sbt_t1036 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(fri, 0LL)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fsym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("* self) { ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1034, _at_t1035)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1036); _tr_str_release(_at_t1034); _tr_str_release(_at_t1035); _tr_str_release(_sbt_t1036); });
                    /* pass */
                    TrStr fd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (fsym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((!_tr_str_eqv((List_TrStr_get(fri, 2LL)), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t1037 = ({ TrStr _cl = (_tr_strx_concatv((fd), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (List_TrStr_get(fri, 2LL)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        _tr_str_release(fd);
                        fd = _strtmp_t1037;
                    }
                    /* pass */
                    ({ TrStr _sbt_t1038 = (_tr_strx_concatv((fd), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t1038); _tr_str_release(_sbt_t1038); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(fsym);
                    _tr_str_release(fd);
                }
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("ITER ", 5LL)))) {
            /* pass */
            TrStr idesc = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
            /* pass */
            if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((idesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((idesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
                /* pass */
                List_TrStr* iri = _cpp_ret_ex(idesc, _tr_str_lit_len("(*it)", 5LL), value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr isym = ({ TrStr _at_t1039 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_nth", 4LL)))); __auto_type _wr = (_uniq_sym(_at_t1039, sym_used)); _tr_str_release(_at_t1039); _wr; });
                /* pass */
                ({ TrStr _at_t1040 = (List_TrStr_get(iri, 0LL)); TrStr _at_t1041 = (({ TrStr _cr = (List_TrStr_get(iri, 1LL)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("auto it = self->begin(); std::advance(it, (long)i); ", 52LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _sbt_t1042 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(iri, 0LL)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (isym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("* self, long i) { ", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1040, _at_t1041)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1042); _tr_str_release(_at_t1040); _tr_str_release(_at_t1041); _tr_str_release(_sbt_t1042); });
                /* pass */
                TrStr id = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (isym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i: c_long)", 12LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((!_tr_str_eqv((List_TrStr_get(iri, 2LL)), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1043 = ({ TrStr _cl = (_tr_strx_concatv((id), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (List_TrStr_get(iri, 2LL)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(id);
                    id = _strtmp_t1043;
                }
                /* pass */
                ({ TrStr _sbt_t1044 = (_tr_strx_concatv((id), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t1044); _tr_str_release(_sbt_t1044); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(isym);
                _tr_str_release(id);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((_tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPKEY ", 11LL))) || _tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPVAL ", 11LL))))) {
            /* pass */
            bool is_key = _tr_str_starts_withv((line), (_tr_str_lit_len("ITERMAPKEY ", 11LL)));
            /* pass */
            TrStr mdesc = _tr_str_slicev((line), 11LL, _tr_str_lenv((line)));
            /* pass */
            if (((_is_clean_ident(cur_class) && (_tr_str_index_ofv((mdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((mdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL))) {
                /* pass */
                TrStr mfield = _tr_str_lit_len("(it->second)", 12LL);
                /* pass */
                TrStr mname = _tr_str_lit_len("_val_nth", 8LL);
                /* pass */
                if (is_key) {
                    /* pass */
                    TrStr _strtmp_t1045 = _tr_str_lit_len("(it->first)", 11LL);
                    _tr_str_release(mfield);
                    mfield = _strtmp_t1045;
                    /* pass */
                    TrStr _strtmp_t1046 = _tr_str_lit_len("_key_nth", 8LL);
                    _tr_str_release(mname);
                    mname = _strtmp_t1046;
                }
                /* pass */
                List_TrStr* mri = _cpp_ret_ex(mdesc, mfield, value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr msym = ({ TrStr _at_t1047 = (_tr_strx_concatv((class_pfx), (mname))); __auto_type _wr = (_uniq_sym(_at_t1047, sym_used)); _tr_str_release(_at_t1047); _wr; });
                /* pass */
                ({ TrStr _at_t1048 = (List_TrStr_get(mri, 0LL)); TrStr _at_t1049 = (({ TrStr _cr = (List_TrStr_get(mri, 1LL)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("auto it = self->begin(); std::advance(it, (long)i); ", 52LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _sbt_t1050 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(mri, 0LL)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (msym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("* self, long i) { ", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1048, _at_t1049)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1050); _tr_str_release(_at_t1048); _tr_str_release(_at_t1049); _tr_str_release(_sbt_t1050); });
                /* pass */
                TrStr md = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (msym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(obj: ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i: c_long)", 12LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((!_tr_str_eqv((List_TrStr_get(mri, 2LL)), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1051 = ({ TrStr _cl = (_tr_strx_concatv((md), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (List_TrStr_get(mri, 2LL)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(md);
                    md = _strtmp_t1051;
                }
                /* pass */
                ({ TrStr _sbt_t1052 = (_tr_strx_concatv((md), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t1052); _tr_str_release(_sbt_t1052); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(mfield);
                _tr_str_release(mname);
                _tr_str_release(msym);
                _tr_str_release(md);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("GVAR ", 5LL)))) {
            /* pass */
            TrStr grest = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* gbar = _tr_str_splitv((grest), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            TrStr gvdesc = List_TrStr_get(gbar, 0LL);
            /* pass */
            TrStr gvnm = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((gbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1053 = List_TrStr_get(gbar, 1LL);
                _tr_str_release(gvnm);
                gvnm = _strtmp_t1053;
            }
            /* pass */
            TrStr gcat = List_TrStr_get(_desc4(gvdesc), 2LL);
            /* pass */
            if ((((((_is_clean_ident(gvnm) && (_tr_str_index_ofv((gvdesc), (_tr_str_lit_len("type-parameter", 14LL))) < 0LL)) && (_tr_str_index_ofv((gvdesc), (_tr_str_lit_len("~d~", 3LL))) < 0LL)) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("u", 1LL))))) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("f", 1LL))))) && (!_tr_str_eqv((gcat), (_tr_str_lit_len("v", 1LL)))))) {
                /* pass */
                TrStr gqual = _tr_str_retain(gvnm);
                /* pass */
                TrStr gsym0 = _tr_strx_concatv((_tr_str_lit_len("g_", 2LL)), (gvnm));
                /* pass */
                if ((!_tr_str_eqv((cur_class), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1054 = ({ TrStr _cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t1054;
                    /* pass */
                    TrStr _strtmp_t1055 = ({ TrStr _cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t1055;
                } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1056 = ({ TrStr _cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t1056;
                    /* pass */
                    TrStr _strtmp_t1057 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t1057;
                }
                /* pass */
                List_TrStr* gvri = _cpp_ret_ex(gvdesc, gqual, value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr gvs = _uniq_sym(gsym0, sym_used);
                /* pass */
                ({ TrStr _at_t1058 = (List_TrStr_get(gvri, 0LL)); TrStr _at_t1059 = (List_TrStr_get(gvri, 1LL)); TrStr _sbt_t1060 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(gvri, 0LL)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gvs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("() { ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1058, _at_t1059)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1060); _tr_str_release(_at_t1058); _tr_str_release(_at_t1059); _tr_str_release(_sbt_t1060); });
                /* pass */
                TrStr gvd = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (gvs))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()", 2LL))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((!_tr_str_eqv((List_TrStr_get(gvri, 2LL)), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1061 = ({ TrStr _cl = (_tr_strx_concatv((gvd), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (List_TrStr_get(gvri, 2LL)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(gvd);
                    gvd = _strtmp_t1061;
                }
                /* pass */
                ({ TrStr _sbt_t1062 = (_tr_strx_concatv((gvd), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t1062); _tr_str_release(_sbt_t1062); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(gqual);
                _tr_str_release(gsym0);
                _tr_str_release(gvs);
                _tr_str_release(gvd);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FTMPL ", 6LL)))) {
            /* pass */
            TrStr ftrest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* ftbar = _tr_str_splitv((ftrest), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            long long ftnp = ({ TrStr _at_t1063 = (List_TrStr_get(ftbar, 0LL)); __auto_type _wr = (_to_int(_at_t1063)); _tr_str_release(_at_t1063); _wr; });
            /* pass */
            TrStr ftnm = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((ftbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1064 = List_TrStr_get(ftbar, 1LL);
                _tr_str_release(ftnm);
                ftnm = _strtmp_t1064;
            }
            /* pass */
            if ((_is_clean_ident(ftnm) && (ftnp >= 0LL))) {
                /* pass */
                List_TrStr* ftys = (void*)List_TrStr_new();
                /* pass */
                List_TrStr_append(ftys, _tr_str_lit_len("int", 3LL));
                /* pass */
                List_TrStr_append(ftys, _tr_str_lit_len("double", 6LL));
                /* pass */
                List_TrStr* fctys = (void*)List_TrStr_new();
                /* pass */
                List_TrStr_append(fctys, _tr_str_lit_len("c_int", 5LL));
                /* pass */
                List_TrStr_append(fctys, _tr_str_lit_len("c_double", 8LL));
                /* pass */
                long long fti = 0LL;
                /* pass */
                while ((fti < ftys->len)) {
                    /* pass */
                    TrStr fcpp = List_TrStr_get(ftys, fti);
                    /* pass */
                    TrStr fcty = List_TrStr_get(fctys, fti);
                    /* pass */
                    fti = (fti + 1LL);
                    /* pass */
                    TrStr fshimp = _tr_str_lit_len("", 0LL);
                    /* pass */
                    TrStr ftrp = _tr_str_lit_len("", 0LL);
                    /* pass */
                    TrStr fcallargs = _tr_str_lit_len("", 0LL);
                    /* pass */
                    long long fpi = 0LL;
                    /* pass */
                    while ((fpi < ftnp)) {
                        /* pass */
                        TrStr fan = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(fpi)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
                        /* pass */
                        fpi = (fpi + 1LL);
                        /* pass */
                        if ((!_tr_str_eqv((fshimp), (_tr_str_lit_len("", 0LL))))) {
                            /* pass */
                            TrStr _strtmp_t1065 = _tr_strx_concatv((fshimp), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(fshimp);
                            fshimp = _strtmp_t1065;
                        }
                        /* pass */
                        TrStr _strtmp_t1066 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fshimp), (fcpp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fan)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fshimp);
                        fshimp = _strtmp_t1066;
                        /* pass */
                        if ((!_tr_str_eqv((ftrp), (_tr_str_lit_len("", 0LL))))) {
                            /* pass */
                            TrStr _strtmp_t1067 = _tr_strx_concatv((ftrp), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(ftrp);
                            ftrp = _strtmp_t1067;
                        }
                        /* pass */
                        TrStr _strtmp_t1068 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((ftrp), (fan))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fcty)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(ftrp);
                        ftrp = _strtmp_t1068;
                        /* pass */
                        if ((!_tr_str_eqv((fcallargs), (_tr_str_lit_len("", 0LL))))) {
                            /* pass */
                            TrStr _strtmp_t1069 = _tr_strx_concatv((fcallargs), (_tr_str_lit_len(", ", 2LL)));
                            _tr_str_release(fcallargs);
                            fcallargs = _strtmp_t1069;
                        }
                        /* pass */
                        TrStr _strtmp_t1070 = _tr_strx_concatv((fcallargs), (fan));
                        _tr_str_release(fcallargs);
                        fcallargs = _strtmp_t1070;
                        _tr_str_release(fan);
                    }
                    /* pass */
                    TrStr fqual = _tr_str_retain(ftnm);
                    /* pass */
                    TrStr ftsym0 = ({ TrStr _cl = (_tr_strx_concatv((ftnm), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fcpp)); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t1071 = ({ TrStr _cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ftnm)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fqual);
                        fqual = _strtmp_t1071;
                        /* pass */
                        TrStr _strtmp_t1072 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ftnm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fcpp)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(ftsym0);
                        ftsym0 = _strtmp_t1072;
                    }
                    /* pass */
                    TrStr ftsym = _uniq_sym(ftsym0, sym_used);
                    /* pass */
                    TrStr fcall = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fqual), (_tr_str_lit_len("<", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fcpp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(">(", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fcallargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    ({ TrStr _at_t1073 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (fcall))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t1074 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fcpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ftsym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fshimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(fcpp, _at_t1073)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1074); _tr_str_release(_at_t1073); _tr_str_release(_sbt_t1074); });
                    /* pass */
                    ({ TrStr _sbt_t1075 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (ftsym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ftrp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") -> ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fcty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1075); _tr_str_release(_sbt_t1075); });
                    /* pass */
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
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("XSTRUCT ", 8LL)))) {
            /* pass */
            TrStr xn = _tr_str_slicev((line), 8LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr_append(cstk_name, xn);
            /* pass */
            List_TrStr_append(cstk_qual, xn);
            /* pass */
            List_TrStr_append(cstk_pfx, xn);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(xn), _tr_str_box(_tr_str_retain(xn)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1076 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1076;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("SFIELD ", 7LL)))) {
            /* pass */
            TrStr srest = _tr_str_slicev((line), 7LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* sbar = _tr_str_splitv((srest), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            TrStr sty = List_TrStr_get(sbar, 0LL);
            /* pass */
            TrStr snm = _tr_str_lit_len("", 0LL);
            /* pass */
            if ((sbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1077 = List_TrStr_get(sbar, 1LL);
                _tr_str_release(snm);
                snm = _strtmp_t1077;
            }
            /* pass */
            if (_is_clean_ident(snm)) {
                /* pass */
                TrStr _strtmp_t1078 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((fld_buf), (_tr_str_lit_len("    pub ", 8LL)))); TrStr _cr = (_cpp_tr_pname(snm)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; });
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1078;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("EXSTRUCT", 8LL)))) {
            /* pass */
            if (({ TrStr _dkt_t1079 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); __auto_type _wr = ((_is_clean_ident(cur_class) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1079))))); _tr_str_release(_dkt_t1079); _wr; })) {
                /* pass */
                if ((!_tr_str_eqv((fld_buf), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    ({ TrStr _sbt_t1080 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("@value_type\nclass ", 18LL)), (cur_class))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fld_buf)); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1080); _tr_str_release(_sbt_t1080); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t1081 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("class ", 6LL)), (cur_class))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n    pass\n", 11LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1081); _tr_str_release(_sbt_t1081); });
                }
                /* pass */
                ({ TrStr _dkt_t1082 = (_tr_strx_concatv((_tr_str_lit_len("class:", 6LL)), (cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t1082), true); _tr_str_release(_dkt_t1082); });
                /* pass */
                n_classes = (n_classes + 1LL);
            }
            /* pass */
            if ((cstk_name->len > 0LL)) {
                /* pass */
                List_TrStr_pop(cstk_name);
                /* pass */
                List_TrStr_pop(cstk_qual);
                /* pass */
                List_TrStr_pop(cstk_pfx);
            }
            /* pass */
            if ((fld_stack->len > 0LL)) {
                /* pass */
                TrStr _strtmp_t1083 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1083;
            } else {
                /* pass */
                TrStr _strtmp_t1084 = _tr_str_lit_len("", 0LL);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1084;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("ENS", 3LL)))) {
            /* pass */
            TrStr _strtmp_t1085 = _ns_pop(ns_path);
            _tr_str_release(ns_path);
            ns_path = _strtmp_t1085;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_eqv((line), (_tr_str_lit_len("EENUM", 5LL)))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("NS ", 3LL)))) {
            /* pass */
            TrStr nn = _tr_str_slicev((line), 3LL, _tr_str_lenv((line)));
            /* pass */
            if (_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL)))) {
                /* pass */
                TrStr _strtmp_t1086 = _tr_str_retain(nn);
                _tr_str_release(ns_path);
                ns_path = _strtmp_t1086;
            } else {
                /* pass */
                TrStr _strtmp_t1087 = ({ TrStr _cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (nn)); _tr_str_release(_cl); _cres; });
                _tr_str_release(ns_path);
                ns_path = _strtmp_t1087;
            }
            /* pass */
            if (({ TrStr _dkt_t1088 = (_tr_strx_concatv((_tr_str_lit_len("using:", 6LL)), (ns_path))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t1088)))); _tr_str_release(_dkt_t1088); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1089 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("using namespace ", 16LL)), (ns_path))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(usings, _sbt_t1089); _tr_str_release(_sbt_t1089); });
                /* pass */
                ({ TrStr _dkt_t1090 = (_tr_strx_concatv((_tr_str_lit_len("using:", 6LL)), (ns_path))); _tr_dict_set(seen, _tr_strz(_dkt_t1090), true); _tr_str_release(_dkt_t1090); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("TCLASS ", 7LL)))) {
            /* pass */
            List_TrStr* tp2 = ({ TrStr _at_t1091 = (_tr_str_slicev((line), 7LL, _tr_str_lenv((line)))); __auto_type _wr = (_parse_tclass(_at_t1091)); _tr_str_release(_at_t1091); _wr; });
            /* pass */
            TrStr tspell = List_TrStr_get(tp2, 0LL);
            /* pass */
            TrStr tname = _cpp_ident(tspell);
            /* pass */
            List_TrStr_append(cstk_name, tname);
            /* pass */
            List_TrStr_append(cstk_qual, tspell);
            /* pass */
            List_TrStr_append(cstk_pfx, tname);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(tname), _tr_str_box(_tr_str_retain(tspell)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1092 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1092;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("CLASS ", 6LL)))) {
            /* pass */
            TrStr crest = _tr_str_slicev((line), 6LL, _tr_str_lenv((line)));
            /* pass */
            TrStr ncls = List_TrStr_get(_tr_str_splitv((crest), (_tr_str_lit_len(" ", 1LL))), 0LL);
            /* pass */
            TrStr nqual = _tr_str_retain(ncls);
            /* pass */
            TrStr npfx = _tr_str_retain(ncls);
            /* pass */
            if ((!_tr_str_eqv((cur_class_qual), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr _strtmp_t1093 = ({ TrStr _cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t1093;
            } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr _strtmp_t1094 = ({ TrStr _cl = (_tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t1094;
            }
            /* pass */
            if ((!_tr_str_eqv((class_pfx), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr _strtmp_t1095 = ({ TrStr _cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t1095;
            } else if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr _strtmp_t1096 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t1096;
            }
            /* pass */
            List_TrStr_append(cstk_name, ncls);
            /* pass */
            List_TrStr_append(cstk_qual, nqual);
            /* pass */
            List_TrStr_append(cstk_pfx, npfx);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(ncls), _tr_str_box(_tr_str_retain(nqual)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1097 = _tr_str_lit_len("", 0LL);
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1097;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("ENUM ", 5LL)))) {
            /* pass */
            TrStr erest = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* eps = _tr_str_splitv((erest), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            TrStr en = List_TrStr_get(eps, 0LL);
            /* pass */
            TrStr eunder = _tr_str_lit_len("c_int", 5LL);
            /* pass */
            if ((eps->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1098 = List_TrStr_get(eps, 1LL);
                _tr_str_release(eunder);
                eunder = _strtmp_t1098;
            }
            /* pass */
            if (({ TrStr _dkt_t1099 = (_tr_strx_concatv((_tr_str_lit_len("enum:", 5LL)), (en))); __auto_type _wr = ((_is_clean_ident(en) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1099))))); _tr_str_release(_dkt_t1099); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1100 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (en))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (eunder)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t1100); _tr_str_release(_sbt_t1100); });
                /* pass */
                ({ TrStr _dkt_t1101 = (_tr_strx_concatv((_tr_str_lit_len("enum:", 5LL)), (en))); _tr_dict_set(seen, _tr_strz(_dkt_t1101), true); _tr_str_release(_dkt_t1101); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("EVAL ", 5LL)))) {
            /* pass */
            TrStr er = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
            /* pass */
            List_TrStr* ep = _tr_str_splitv((er), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            TrStr ename = List_TrStr_get(ep, 0LL);
            /* pass */
            TrStr eval_ = _tr_str_lit_len("0", 1LL);
            /* pass */
            if ((ep->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1102 = List_TrStr_get(ep, 1LL);
                _tr_str_release(eval_);
                eval_ = _strtmp_t1102;
            }
            /* pass */
            if (({ TrStr _dkt_t1103 = (_tr_strx_concatv((_tr_str_lit_len("eval:", 5LL)), (ename))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t1103)))); _tr_str_release(_dkt_t1103); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1104 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (ename))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (eval_)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t1104); _tr_str_release(_sbt_t1104); });
                /* pass */
                ({ TrStr _dkt_t1105 = (_tr_strx_concatv((_tr_str_lit_len("eval:", 5LL)), (ename))); _tr_dict_set(seen, _tr_strz(_dkt_t1105), true); _tr_str_release(_dkt_t1105); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("METHOD ", 7LL)))) {
            /* pass */
            TrStr r = _tr_str_slicev((line), 7LL, _tr_str_lenv((line)));
            /* pass */
            TrStr _strtmp_t1106 = _tr_str_slicev((r), 0LL, 2LL);
            _tr_str_release(m_flags);
            m_flags = _strtmp_t1106;
            /* pass */
            TrStr r2 = _tr_str_slicev((r), 3LL, _tr_str_lenv((r)));
            /* pass */
            __auto_type sp1 = _tr_str_index_ofv((r2), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            m_ndef = ({ TrStr _at_t1107 = (_tr_str_slicev((r2), 0LL, sp1)); __auto_type _wr = (_to_int(_at_t1107)); _tr_str_release(_at_t1107); _wr; });
            /* pass */
            TrStr r3 = _tr_str_slicev((r2), (sp1 + 1LL), _tr_str_lenv((r2)));
            /* pass */
            List_TrStr* rn = _tr_str_splitv((r3), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            TrStr _strtmp_t1108 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t1108;
            /* pass */
            TrStr _strtmp_t1109 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t1109;
            /* pass */
            TrStr _strtmp_t1110 = _tr_str_lit_len("", 0LL);
            _tr_str_release(m_qual);
            m_qual = _strtmp_t1110;
            /* pass */
            if ((rn->len > 2LL)) {
                /* pass */
                TrStr _strtmp_t1111 = List_TrStr_get(rn, 2LL);
                _tr_str_release(m_qual);
                m_qual = _strtmp_t1111;
            }
            /* pass */
            TrStr _strtmp_t1112 = _tr_str_lit_len("method", 6LL);
            _tr_str_release(mode);
            mode = _strtmp_t1112;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("FUNC ", 5LL)))) {
            /* pass */
            TrStr r = _tr_str_slicev((line), 5LL, _tr_str_lenv((line)));
            /* pass */
            __auto_type sp1 = _tr_str_index_ofv((r), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            m_ndef = ({ TrStr _at_t1113 = (_tr_str_slicev((r), 0LL, sp1)); __auto_type _wr = (_to_int(_at_t1113)); _tr_str_release(_at_t1113); _wr; });
            /* pass */
            TrStr r3 = _tr_str_slicev((r), (sp1 + 1LL), _tr_str_lenv((r)));
            /* pass */
            List_TrStr* rn = _tr_str_splitv((r3), (_tr_str_lit_len("|", 1LL)));
            /* pass */
            TrStr _strtmp_t1114 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t1114;
            /* pass */
            TrStr _strtmp_t1115 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t1115;
            /* pass */
            TrStr _strtmp_t1116 = _tr_str_lit_len("", 0LL);
            _tr_str_release(m_qual);
            m_qual = _strtmp_t1116;
            /* pass */
            TrStr _strtmp_t1117 = _tr_str_lit_len("..", 2LL);
            _tr_str_release(m_flags);
            m_flags = _strtmp_t1117;
            /* pass */
            TrStr _strtmp_t1118 = _tr_str_lit_len("func", 4LL);
            _tr_str_release(mode);
            mode = _strtmp_t1118;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_withv((line), (_tr_str_lit_len("PARAM ", 6LL)))) {
            /* pass */
            ({ TrStr _at_t1119 = (_tr_str_slicev((line), 6LL, _tr_str_lenv((line)))); List_TrStr_append(params, _at_t1119); _tr_str_release(_at_t1119); });
            /* pass */
            continue;
        }
        /* pass */
        if (((_tr_str_eqv((line), (_tr_str_lit_len("EMETHOD", 7LL))) || _tr_str_eqv((line), (_tr_str_lit_len("ECTOR", 5LL)))) || _tr_str_eqv((line), (_tr_str_lit_len("EFUNC", 5LL))))) {
            /* pass */
            bool is_static = _tr_str_eqv((_tr_str_slicev((m_flags), 0LL, 1LL)), (_tr_str_lit_len("s", 1LL)));
            /* pass */
            bool is_const = _tr_str_eqv((_tr_str_slicev((m_flags), 1LL, 2LL)), (_tr_str_lit_len("c", 1LL)));
            /* pass */
            TrStr m_sym = _tr_str_retain(m_name);
            /* pass */
            if ((((!_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) && (_tr_str_lenv((m_name)) >= 8LL)) && _tr_str_eqv((_tr_str_slicev((m_name), 0LL, 8LL)), (_tr_str_lit_len("operator", 8LL))))) {
                /* pass */
                TrStr _strtmp_t1120 = _cpp_op_name(m_name, params->len, _tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL))));
                _tr_str_release(m_sym);
                m_sym = _strtmp_t1120;
                /* pass */
                if (_tr_str_eqv((m_sym), (_tr_str_lit_len("", 0LL)))) {
                    /* pass */
                    TrStr _strtmp_t1121 = _tr_str_lit_len("", 0LL);
                    _tr_str_release(mode);
                    mode = _strtmp_t1121;
                    /* pass */
                    continue;
                }
            }
            /* pass */
            if (((_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL))) || _tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) && (!_is_clean_ident(cur_class)))) {
                /* pass */
                TrStr _strtmp_t1122 = _tr_str_lit_len("", 0LL);
                _tr_str_release(mode);
                mode = _strtmp_t1122;
                /* pass */
                continue;
            }
            /* pass */
            bool has_dep = ((_tr_str_index_ofv((m_ret), (_tr_str_lit_len("type-parameter", 14LL))) >= 0LL) || (_tr_str_index_ofv((m_ret), (_tr_str_lit_len("~d~", 3LL))) >= 0LL));
            /* pass */
            long long pj = 0LL;
            /* pass */
            while ((pj < params->len)) {
                /* pass */
                TrStr pvd = List_TrStr_get(params, pj);
                /* pass */
                if (((_tr_str_index_ofv((pvd), (_tr_str_lit_len("type-parameter", 14LL))) >= 0LL) || (_tr_str_index_ofv((pvd), (_tr_str_lit_len("~d~", 3LL))) >= 0LL))) {
                    /* pass */
                    has_dep = true;
                }
                /* pass */
                if (((_tr_str_eqv((m_name), (_tr_str_lit_len("operator()", 10LL))) && _tr_str_starts_withv((pvd), (_tr_str_lit_len("0~", 2LL)))) && (_tr_str_index_ofv((pvd), (_tr_str_lit_len("~u~", 3LL))) >= 0LL))) {
                    /* pass */
                    has_dep = true;
                }
                /* pass */
                pj = (pj + 1LL);
                _tr_str_release(pvd);
            }
            /* pass */
            if (has_dep) {
                /* pass */
                TrStr _strtmp_t1123 = _tr_str_lit_len("", 0LL);
                _tr_str_release(mode);
                mode = _strtmp_t1123;
                /* pass */
                continue;
            }
            /* pass */
            List_TrStr* full_params = (void*)List_TrStr_new();
            /* pass */
            long long fpi = 0LL;
            /* pass */
            while ((fpi < params->len)) {
                /* pass */
                ({ TrStr _at_t1124 = (List_TrStr_get(params, fpi)); List_TrStr_append(full_params, _at_t1124); _tr_str_release(_at_t1124); });
                /* pass */
                fpi = (fpi + 1LL);
            }
            /* pass */
            long long min_arity = (full_params->len - m_ndef);
            /* pass */
            if ((min_arity < 0LL)) {
                /* pass */
                min_arity = 0LL;
            }
            /* pass */
            long long arity = full_params->len;
            /* pass */
            while ((arity >= min_arity)) {
                /* pass */
                List_TrStr* tparams = (void*)List_TrStr_new();
                /* pass */
                long long tti = 0LL;
                /* pass */
                while ((tti < arity)) {
                    /* pass */
                    ({ TrStr _at_t1125 = (List_TrStr_get(full_params, tti)); List_TrStr_append(tparams, _at_t1125); _tr_str_release(_at_t1125); });
                    /* pass */
                    tti = (tti + 1LL);
                }
                /* pass */
                params = tparams;
                /* pass */
                TrStr shimp = _tr_str_lit_len("", 0LL);
                /* pass */
                TrStr trp = _tr_str_lit_len("", 0LL);
                /* pass */
                TrStr fargs = _tr_str_lit_len("", 0LL);
                /* pass */
                if ((_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL))) && (!is_static))) {
                    /* pass */
                    if (is_const) {
                        /* pass */
                        TrStr _strtmp_t1126 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("const ", 6LL)), (cur_class_qual))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("* self", 6LL))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1126;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1127 = _tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("* self", 6LL)));
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1127;
                    }
                    /* pass */
                    TrStr _strtmp_t1128 = _tr_strx_concatv((_tr_str_lit_len("obj: ", 5LL)), (cur_class));
                    _tr_str_release(trp);
                    trp = _strtmp_t1128;
                }
                /* pass */
                long long pi = 0LL;
                /* pass */
                while ((pi < params->len)) {
                    /* pass */
                    TrStr pv = List_TrStr_get(params, pi);
                    /* pass */
                    pi = (pi + 1LL);
                    /* pass */
                    List_TrStr* bar = _tr_str_splitv((pv), (_tr_str_lit_len("|", 1LL)));
                    /* pass */
                    TrStr ptype = List_TrStr_get(bar, 0LL);
                    /* pass */
                    TrStr pname = _tr_str_lit_len("", 0LL);
                    /* pass */
                    if ((bar->len > 1LL)) {
                        /* pass */
                        TrStr _strtmp_t1129 = List_TrStr_get(bar, 1LL);
                        _tr_str_release(pname);
                        pname = _strtmp_t1129;
                    }
                    /* pass */
                    if (_tr_str_eqv((pname), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        TrStr _strtmp_t1130 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("a", 1LL)), _cr); _tr_str_release(_cr); _cres; });
                        _tr_str_release(pname);
                        pname = _strtmp_t1130;
                    }
                    /* pass */
                    TrStr spname = _tr_str_retain(pname);
                    /* pass */
                    if (_tr_str_eqv((spname), (_tr_str_lit_len("self", 4LL)))) {
                        /* pass */
                        TrStr _strtmp_t1131 = _tr_str_lit_len("self_", 5LL);
                        _tr_str_release(spname);
                        spname = _strtmp_t1131;
                    }
                    /* pass */
                    TrStr tpname = _cpp_tr_pname(pname);
                    /* pass */
                    List_TrStr* d4 = _desc4(ptype);
                    /* pass */
                    long long pdepth = ({ TrStr _at_t1132 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t1132)); _tr_str_release(_at_t1132); _wr; });
                    /* pass */
                    TrStr prefk = List_TrStr_get(d4, 1LL);
                    /* pass */
                    bool pref = (!_tr_str_eqv((prefk), (_tr_str_lit_len("0", 1LL))));
                    /* pass */
                    TrStr pcat = List_TrStr_get(d4, 2LL);
                    /* pass */
                    TrStr pdet = List_TrStr_get(d4, 3LL);
                    /* pass */
                    TrStr sc = _tr_str_lit_len("", 0LL);
                    /* pass */
                    TrStr fw = _tr_str_lit_len("", 0LL);
                    /* pass */
                    TrStr trt = _tr_str_lit_len("", 0LL);
                    /* pass */
                    if (_tr_str_eqv((pcat), (_tr_str_lit_len("p", 1LL)))) {
                        /* pass */
                        TrStr cpp = _c_to_cpp(pdet);
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1133 = ({ TrStr _cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1133;
                            /* pass */
                            TrStr _strtmp_t1134 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1134;
                            /* pass */
                            TrStr _strtmp_t1135 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t1135;
                        } else if ((_tr_str_eqv((prefk), (_tr_str_lit_len("2", 1LL))) && (pdepth == 1LL))) {
                            /* pass */
                            TrStr _strtmp_t1136 = ({ TrStr _cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1136;
                            /* pass */
                            TrStr _strtmp_t1137 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1137;
                            /* pass */
                            TrStr _strtmp_t1138 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t1138;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1139 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cpp), (_tr_str_lit_len(" ", 1LL)))); TrStr _cr = (_stars(pdepth)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1139;
                            /* pass */
                            if (pref) {
                                /* pass */
                                TrStr _strtmp_t1140 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1140;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1141 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1141;
                            }
                            /* pass */
                            TrStr _strtmp_t1142 = _ptr_wrap(pdet, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t1142;
                        }
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("e", 1LL)))) {
                        /* pass */
                        TrStr espell = List_TrStr_get(_tr_str_splitv((pdet), (_tr_str_lit_len("#", 1LL))), 0LL);
                        /* pass */
                        TrStr ealias = _last_seg(espell);
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1143 = ({ TrStr _cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1143;
                            /* pass */
                            TrStr _strtmp_t1144 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1144;
                            /* pass */
                            TrStr _strtmp_t1145 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t1145;
                        } else if ((_tr_str_eqv((prefk), (_tr_str_lit_len("2", 1LL))) && (pdepth == 1LL))) {
                            /* pass */
                            TrStr _strtmp_t1146 = ({ TrStr _cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1146;
                            /* pass */
                            TrStr _strtmp_t1147 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1147;
                            /* pass */
                            TrStr _strtmp_t1148 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t1148;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1149 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((espell), (_tr_str_lit_len(" ", 1LL)))); TrStr _cr = (_stars(pdepth)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1149;
                            /* pass */
                            if (pref) {
                                /* pass */
                                TrStr _strtmp_t1150 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1150;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1151 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1151;
                            }
                            /* pass */
                            TrStr _strtmp_t1152 = _ptr_wrap(ealias, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t1152;
                        }
                    } else if ((_tr_str_eqv((pcat), (_tr_str_lit_len("s", 1LL))) && _tr_str_eqv((pdet), (_tr_str_lit_len("string", 6LL))))) {
                        /* pass */
                        TrStr _strtmp_t1153 = _tr_strx_concatv((_tr_str_lit_len("const char* ", 12LL)), (spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t1153;
                        /* pass */
                        TrStr _strtmp_t1154 = ({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("std::string(", 12LL)), (spname))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t1154;
                        /* pass */
                        TrStr _strtmp_t1155 = _tr_str_lit_len("Pointer[char]", 13LL);
                        _tr_str_release(trt);
                        trt = _strtmp_t1155;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("f", 1LL)))) {
                        /* pass */
                        TrStr _strtmp_t1156 = _tr_strx_concatv((_tr_str_lit_len("void* ", 6LL)), (spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t1156;
                        /* pass */
                        TrStr _strtmp_t1157 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_fnptr_cast(pdet)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t1157;
                        /* pass */
                        TrStr _strtmp_t1158 = _tr_str_lit_len("Pointer[void]", 13LL);
                        _tr_str_release(trt);
                        trt = _strtmp_t1158;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("v", 1LL)))) {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1159 = ({ TrStr _cl = (({ TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1159;
                        /* pass */
                        if (pref) {
                            /* pass */
                            TrStr _strtmp_t1160 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1160;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1161 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1161;
                        }
                        /* pass */
                        TrStr _strtmp_t1162 = _ptr_wrap(_tr_str_lit_len("void", 4LL), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t1162;
                    } else if (_tr_str_eqv((pcat), (_tr_str_lit_len("r", 1LL)))) {
                        /* pass */
                        TrStr seg = _last_seg(pdet);
                        /* pass */
                        if ((_tr_dict_contains(value_structs, _tr_strz(seg)) && (pdepth == 0LL))) {
                            /* pass */
                            TrStr _strtmp_t1163 = ({ TrStr _cl = (_tr_strx_concatv((pdet), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1163;
                            /* pass */
                            TrStr _strtmp_t1164 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1164;
                            /* pass */
                            TrStr _strtmp_t1165 = _tr_str_retain(seg);
                            _tr_str_release(trt);
                            trt = _strtmp_t1165;
                        } else {
                            /* pass */
                            long long nd = pdepth;
                            /* pass */
                            if ((nd < 1LL)) {
                                /* pass */
                                nd = 1LL;
                            }
                            /* pass */
                            TrStr _strtmp_t1166 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_cpp_qual(pdet, class_qual)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1166;
                            /* pass */
                            if ((pdepth == 0LL)) {
                                /* pass */
                                TrStr _strtmp_t1167 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1167;
                            } else if (pref) {
                                /* pass */
                                TrStr _strtmp_t1168 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1168;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1169 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1169;
                            }
                            /* pass */
                            TrStr _strtmp_t1170 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                            _tr_str_release(trt);
                            trt = _strtmp_t1170;
                        }
                    } else if (_tr_str_eqv((pdet), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1171 = ({ TrStr _cl = (({ TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("void ", 5LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1171;
                        /* pass */
                        TrStr _strtmp_t1172 = _tr_str_retain(spname);
                        _tr_str_release(fw);
                        fw = _strtmp_t1172;
                        /* pass */
                        TrStr _strtmp_t1173 = _ptr_wrap(_tr_str_lit_len("void", 4LL), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t1173;
                    } else {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1174 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((pdet), (_tr_str_lit_len(" ", 1LL)))); TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1174;
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1175 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1175;
                        } else if (pref) {
                            /* pass */
                            TrStr _strtmp_t1176 = _tr_strx_concatv((_tr_str_lit_len("*", 1LL)), (spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1176;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1177 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1177;
                        }
                        /* pass */
                        TrStr _strtmp_t1178 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                        _tr_str_release(trt);
                        trt = _strtmp_t1178;
                    }
                    /* pass */
                    if (_tr_str_eqv((shimp), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        TrStr _strtmp_t1179 = _tr_str_retain(sc);
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1179;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1180 = ({ TrStr _cl = (_tr_strx_concatv((shimp), (_tr_str_lit_len(", ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (sc)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1180;
                    }
                    /* pass */
                    if (_tr_str_eqv((fargs), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        TrStr _strtmp_t1181 = _tr_str_retain(fw);
                        _tr_str_release(fargs);
                        fargs = _strtmp_t1181;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1182 = ({ TrStr _cl = (_tr_strx_concatv((fargs), (_tr_str_lit_len(", ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (fw)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fargs);
                        fargs = _strtmp_t1182;
                    }
                    /* pass */
                    if (_tr_str_eqv((trp), (_tr_str_lit_len("", 0LL)))) {
                        /* pass */
                        TrStr _strtmp_t1183 = ({ TrStr _cl = (_tr_strx_concatv((tpname), (_tr_str_lit_len(": ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (trt)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(trp);
                        trp = _strtmp_t1183;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1184 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((trp), (_tr_str_lit_len(", ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (tpname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (trt)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(trp);
                        trp = _strtmp_t1184;
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
                /* pass */
                TrStr call = _tr_str_lit_len("", 0LL);
                /* pass */
                if (_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) {
                    /* pass */
                    TrStr _strtmp_t1185 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("new ", 4LL)), (cur_class_qual))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t1185;
                } else if (_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) {
                    /* pass */
                    TrStr mcall = _tr_str_retain(m_name);
                    /* pass */
                    if ((!_tr_str_eqv((m_qual), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t1186 = ({ TrStr _cl = (_tr_strx_concatv((m_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m_name)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(mcall);
                        mcall = _strtmp_t1186;
                    }
                    /* pass */
                    if (is_static) {
                        /* pass */
                        TrStr _strtmp_t1187 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("::", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(call);
                        call = _strtmp_t1187;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1188 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("self->", 6LL)), (mcall))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(call);
                        call = _strtmp_t1188;
                    }
                } else {
                    /* pass */
                    TrStr pre = _tr_str_lit_len("", 0LL);
                    /* pass */
                    if ((!_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t1189 = _tr_strx_concatv((ns_path), (_tr_str_lit_len("::", 2LL)));
                        _tr_str_release(pre);
                        pre = _strtmp_t1189;
                    }
                    /* pass */
                    TrStr _strtmp_t1190 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((pre), (m_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t1190;
                    _tr_str_release(pre);
                }
                /* pass */
                if (_tr_str_eqv((mode), (_tr_str_lit_len("ctor", 4LL)))) {
                    /* pass */
                    TrStr sym = ({ TrStr _at_t1191 = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_new", 4LL)))); __auto_type _wr = (_uniq_sym(_at_t1191, sym_used)); _tr_str_release(_at_t1191); _wr; });
                    /* pass */
                    ({ TrStr _at_t1192 = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("*", 1LL)))); TrStr _at_t1193 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), (call))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t1194 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((cur_class_qual), (_tr_str_lit_len("* ", 2LL)))); TrStr _cres = _tr_strx_concatv(_cl, (sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1192, _at_t1193)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1194); _tr_str_release(_at_t1192); _tr_str_release(_at_t1193); _tr_str_release(_sbt_t1194); });
                    /* pass */
                    ({ TrStr _sbt_t1195 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (sym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") -> ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1195); _tr_str_release(_sbt_t1195); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym);
                } else {
                    /* pass */
                    List_TrStr* ri = _cpp_ret_ex(m_ret, call, value_structs, class_names, class_qual, seen, opaque);
                    /* pass */
                    TrStr sym0 = _tr_str_lit_len("", 0LL);
                    /* pass */
                    if (_tr_str_eqv((mode), (_tr_str_lit_len("method", 6LL)))) {
                        /* pass */
                        TrStr _strtmp_t1196 = ({ TrStr _cl = (_tr_strx_concatv((class_pfx), (_tr_str_lit_len("_", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (m_sym)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sym0);
                        sym0 = _strtmp_t1196;
                    } else {
                        /* pass */
                        if (_tr_str_eqv((ns_path), (_tr_str_lit_len("", 0LL)))) {
                            /* pass */
                            TrStr _strtmp_t1197 = _tr_strx_concatv((_tr_str_lit_len("g_", 2LL)), (m_sym));
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t1197;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1198 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m_sym)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t1198;
                        }
                    }
                    /* pass */
                    TrStr sym = _uniq_sym(sym0, sym_used);
                    /* pass */
                    ({ TrStr _at_t1199 = (List_TrStr_get(ri, 0LL)); TrStr _at_t1200 = (List_TrStr_get(ri, 1LL)); TrStr _sbt_t1201 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(ri, 0LL)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(") { ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1199, _at_t1200)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" }\n", 3LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1201); _tr_str_release(_at_t1199); _tr_str_release(_at_t1200); _tr_str_release(_sbt_t1201); });
                    /* pass */
                    TrStr d = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    def ", 8LL)), (sym))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((!_tr_str_eqv((List_TrStr_get(ri, 2LL)), (_tr_str_lit_len("", 0LL))))) {
                        /* pass */
                        TrStr _strtmp_t1202 = ({ TrStr _cl = (_tr_strx_concatv((d), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _cr = (List_TrStr_get(ri, 2LL)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        _tr_str_release(d);
                        d = _strtmp_t1202;
                    }
                    /* pass */
                    ({ TrStr _sbt_t1203 = (_tr_strx_concatv((d), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(decls, _sbt_t1203); _tr_str_release(_sbt_t1203); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym0);
                    _tr_str_release(sym);
                    _tr_str_release(d);
                }
                /* pass */
                arity = (arity - 1LL);
                _tr_str_release(shimp);
                _tr_str_release(trp);
                _tr_str_release(fargs);
                _tr_str_release(call);
            }
            /* pass */
            TrStr _strtmp_t1204 = _tr_str_lit_len("", 0LL);
            _tr_str_release(mode);
            mode = _strtmp_t1204;
            /* pass */
            continue;
        }
        _tr_str_release(line);
        _tr_str_release(cur_class);
        _tr_str_release(cur_class_qual);
        _tr_str_release(class_pfx);
    }
    /* pass */
    TrStr base = _tr_str_retain(out);
    /* pass */
    if (((_tr_str_lenv((base)) > 3LL) && _tr_str_eqv((_tr_str_slicev((base), (_tr_str_lenv((base)) - 3LL), _tr_str_lenv((base)))), (_tr_str_lit_len(".tr", 3LL))))) {
        /* pass */
        TrStr _strtmp_t1205 = _tr_str_slicev((base), 0LL, (_tr_str_lenv((base)) - 3LL));
        _tr_str_release(base);
        base = _strtmp_t1205;
    }
    /* pass */
    TrStr shim_name = _tr_strx_concatv((base), (_tr_str_lit_len("_shim.cpp", 9LL)));
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t1206 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# Auto-generated C++ FFI bindings for ", 38LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" (tauraro-bindgen -h cpp).\n", 27LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1206); _tr_str_release(_sbt_t1206); });
    /* pass */
    ({ TrStr _sbt_t1207 = (({ TrStr _cl = (({ TrStr _cr = (_basename(shim_name)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-shim: ", 20LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1207); _tr_str_release(_sbt_t1207); });
    /* pass */
    StringBuilder_append(sb, _tr_str_lit_len("# tauraro-cpp-lib: stdc++\n", 26LL));
    /* pass */
    if ((!_tr_str_eqv((pkglibs), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t1208 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-linkflags:", 24LL)), (pkglibs))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1208); _tr_str_release(_sbt_t1208); });
    }
    /* pass */
    if ((!_tr_str_eqv((shim_cflags), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t1209 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# tauraro-cpp-cflags: ", 22LL)), (shim_cflags))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1209); _tr_str_release(_sbt_t1209); });
    }
    /* pass */
    ({ TrStr _sbt_t1210 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("# (opt out with --no-auto-cpp, then: c++ -c ", 44LL)), (shim_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ; tauraroc app.tr --link ", 26LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (base)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("_shim.o -lstdc++)\n\n", 19LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1210); _tr_str_release(_sbt_t1210); });
    /* pass */
    TrStr os_ = StringObj_as_str(StringBuilder_to_string(opaque));
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(consts));
    /* pass */
    TrStr ds2 = StringObj_as_str(StringBuilder_to_string(decls));
    /* pass */
    TrMap* defined_names = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* dlns = _tr_str_splitv((({ TrStr _cl = (_tr_strx_concatv((os_), (_tr_str_lit_len("\n", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (cs)); _tr_str_release(_cl); _cres; })), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long dii = 0LL;
    /* pass */
    while ((dii < dlns->len)) {
        /* pass */
        TrStr dl = List_TrStr_get(dlns, dii);
        /* pass */
        dii = (dii + 1LL);
        /* pass */
        if (_tr_str_starts_withv((dl), (_tr_str_lit_len("class ", 6LL)))) {
            /* pass */
            TrStr dn = _ident_at(dl, 6LL);
            /* pass */
            if ((!_tr_str_eqv((dn), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        } else if (_tr_str_starts_withv((dl), (_tr_str_lit_len("type ", 5LL)))) {
            /* pass */
            TrStr dn = _ident_at(dl, 5LL);
            /* pass */
            if ((!_tr_str_eqv((dn), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        }
        _tr_str_release(dl);
    }
    /* pass */
    TrStr fb = ({ TrStr _at_t1211 = (_tr_strx_concatv((os_), (ds2))); __auto_type _wr = (_opaque_fallbacks(_at_t1211, defined_names)); _tr_str_release(_at_t1211); _wr; });
    /* pass */
    if ((!_tr_str_eqv((fb), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        StringBuilder_append(sb, fb);
    }
    /* pass */
    if ((!_tr_str_eqv((os_), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t1212 = (_tr_strx_concatv((os_), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t1212); _tr_str_release(_sbt_t1212); });
    }
    /* pass */
    if ((!_tr_str_eqv((cs), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ TrStr _sbt_t1213 = (_tr_strx_concatv((cs), (_tr_str_lit_len("\n", 1LL)))); StringBuilder_append(sb, _sbt_t1213); _tr_str_release(_sbt_t1213); });
    }
    /* pass */
    if ((!_tr_str_eqv((ds2), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("extern \"C\":\n", 12LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("    def tauraro_cpp_last_error() -> Pointer[char]\n", 50LL));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit_len("    def tauraro_cpp_clear_error()\n", 34LL));
        /* pass */
        StringBuilder_append(sb, ds2);
    }
    /* pass */
    ({ TrStr _at_t1214 = (StringObj_as_str(StringBuilder_to_string(sb))); write_file(out, _at_t1214); _tr_str_release(_at_t1214); });
    /* pass */
    StringBuilder* sh = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t1215 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("// Auto-generated C++ -> C shim for ", 36LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" (tauraro-bindgen -h cpp).\n", 27LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1215); _tr_str_release(_sbt_t1215); });
    /* pass */
    ({ TrStr _sbt_t1216 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("// Compile:  c++ -c ", 20LL)), (shim_name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1216); _tr_str_release(_sbt_t1216); });
    /* pass */
    ({ TrStr _sbt_t1217 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1217); _tr_str_release(_sbt_t1217); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("#include <string>\n#include <string_view>\n#include <cstring>\n#include <cstdlib>\n#include <iterator>\n", 99LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("// std::string return -> heap char* copy (caller owns it; free with the runtime free).\n", 87LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("static char* _tr_cpp_strdup(std::string_view s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.data(), s.size()); p[s.size()]=0; } return p; }\n", 151LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("// Exception safety: a C++ exception must never cross `extern \"C\"` (it would std::terminate).\n", 94LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("// Every wrapper catches, records the message here, and returns a zero value; the caller\n", 89LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("// can read/clear it via tauraro_cpp_last_error()/tauraro_cpp_clear_error().\n", 77LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("static thread_local std::string _tr_cpp_err;\n", 45LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("static void _tr_cpp_set_error(const char* m){ _tr_cpp_err = m ? m : \"C++ exception\"; }\n", 87LL));
    /* pass */
    ({ TrStr _sbt_t1218 = (StringObj_as_str(StringBuilder_to_string(usings))); StringBuilder_append(sh, _sbt_t1218); _tr_str_release(_sbt_t1218); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("extern \"C\" {\n", 13LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("const char* tauraro_cpp_last_error(void){ return _tr_cpp_strdup(_tr_cpp_err); }\n", 80LL));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("void tauraro_cpp_clear_error(void){ _tr_cpp_err.clear(); }\n", 59LL));
    /* pass */
    ({ TrStr _sbt_t1219 = (StringObj_as_str(StringBuilder_to_string(shim))); StringBuilder_append(sh, _sbt_t1219); _tr_str_release(_sbt_t1219); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit_len("}\n", 2LL));
    /* pass */
    ({ TrStr _at_t1220 = (StringObj_as_str(StringBuilder_to_string(sh))); write_file(shim_name, _at_t1220); _tr_str_release(_at_t1220); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("bindgen: wrote ", 15LL)), (out))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" + ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (shim_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" — ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_classes)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" classes, ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_fns)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" wrappers", 9LL))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
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
    /* pass */
    write_file(_tr_str_lit_len("_cxx_empty.cpp", 14LL), _tr_str_lit_len("\n", 1LL));
    /* pass */
    ({ TrStr _aet_t1221 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" -x c++ -E -v _cxx_empty.cpp > _cxx_pp.txt 2> _cxx_inc.txt", 58LL)))); _tr_system(_aet_t1221.data); _tr_str_release(_aet_t1221); });
    /* pass */
    TrStr out = _tr_str_lit_len("", 0LL);
    /* pass */
    if (file_exists(_tr_str_lit_len("_cxx_inc.txt", 12LL))) {
        /* pass */
        TrStr txt = read_file(_tr_str_lit_len("_cxx_inc.txt", 12LL));
        /* pass */
        List_TrStr* lines = _tr_str_splitv((txt), (_tr_str_lit_len("\n", 1LL)));
        /* pass */
        bool collecting = false;
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < lines->len)) {
            /* pass */
            TrStr ln = ({ TrStr _at_t1222 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1222)); _tr_str_release(_at_t1222); _wr; });
            /* pass */
            li = (li + 1LL);
            /* pass */
            if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("search starts here", 18LL))) >= 0LL)) {
                /* pass */
                collecting = true;
                /* pass */
                continue;
            }
            /* pass */
            if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("End of search list", 18LL))) >= 0LL)) {
                /* pass */
                collecting = false;
                /* pass */
                continue;
            }
            /* pass */
            if (collecting) {
                /* pass */
                TrStr d = _lstrip(ln);
                /* pass */
                __auto_type fpos = _tr_str_index_ofv((d), (_tr_str_lit_len(" (framework directory)", 22LL)));
                /* pass */
                if ((fpos >= 0LL)) {
                    /* pass */
                    TrStr _strtmp_t1223 = _tr_str_slicev((d), 0LL, fpos);
                    _tr_str_release(d);
                    d = _strtmp_t1223;
                }
                /* pass */
                if ((!_tr_str_eqv((d), (_tr_str_lit_len("", 0LL))))) {
                    /* pass */
                    TrStr _strtmp_t1224 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((out), (_tr_str_lit_len(" -I\"", 4LL)))); TrStr _cres = _tr_strx_concatv(_cl, (d)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(out);
                    out = _strtmp_t1224;
                }
            }
            _tr_str_release(ln);
        }
        _tr_str_release(txt);
    }
    /* pass */
    _rm_files(_tr_str_lit_len("_cxx_empty.cpp _cxx_pp.txt _cxx_inc.txt", 39LL));
    /* pass */
    return out;
}

__attribute__((hot)) TrStr _cpp_std_flag(TrStr extra) {
    /* pass */
    if ((_tr_str_index_ofv((extra), (_tr_str_lit_len("-std=", 5LL))) >= 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_str_lit_len("-std=c++17 ", 11LL);
}

__attribute__((hot)) long long _cpp_fatal_count(TrStr diag) {
    /* pass */
    List_TrStr* lines = _tr_str_splitv((diag), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long f = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1225 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1225)); _tr_str_release(_at_t1225); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if ((_tr_str_index_ofv((ln), (_tr_str_lit_len("DIAGERR ", 8LL))) == 0LL)) {
            /* pass */
            List_TrStr* parts = _tr_str_splitv((ln), (_tr_str_lit_len(" ", 1LL)));
            /* pass */
            if ((parts->len >= 3LL)) {
                /* pass */
                f = ({ TrStr _at_t1226 = (List_TrStr_get(parts, 2LL)); __auto_type _wr = (_to_int(_at_t1226)); _tr_str_release(_at_t1226); _wr; });
            }
        }
        _tr_str_release(ln);
    }
    /* pass */
    List_TrStr_free(lines);
    return f;
}

__attribute__((hot)) void _cpp_print_diag(TrStr diag) {
    /* pass */
    List_TrStr* lines = _tr_str_splitv((diag), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long shown = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1227 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1227)); _tr_str_release(_at_t1227); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if (((_tr_str_index_ofv((ln), (_tr_str_lit_len("DIAG ", 5LL))) == 0LL) && (shown < 12LL))) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cr = (_tr_str_slicev((ln), 5LL, _tr_str_lenv((ln)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("    ", 4LL)), _cr); _tr_str_release(_cr); _cres; }))); printf("\n"); });
            /* pass */
            shown = (shown + 1LL);
        }
        _tr_str_release(ln);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) bool _cpp_ir_is_empty(TrStr ir) {
    /* pass */
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        if (({ TrStr _at_t1228 = (List_TrStr_get(lines, li)); __auto_type _wr = ((!_tr_str_eqv((_rstrip_cr(_at_t1228)), (_tr_str_lit_len("", 0LL))))); _tr_str_release(_at_t1228); _wr; })) {
            /* pass */
            List_TrStr_free(lines);
            return false;
        }
        /* pass */
        li = (li + 1LL);
    }
    /* pass */
    List_TrStr_free(lines);
    return true;
}

__attribute__((hot)) void _cpp_cleanup() {
    /* pass */
    _rm_files(_tr_str_lit_len("_cxxwalk.c _cxxwalk.exe _cxx.ir _cxx_err.txt _cxx_diag.txt _cxx_force.cpp _cxx_inst.ir", 86LL));
}

__attribute__((hot)) bool _is_expr_proxy_spec(TrStr s) {
    /* pass */
    __auto_type lt = _tr_str_index_ofv((s), (_tr_str_lit_len("<", 1LL)));
    /* pass */
    if ((lt < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr inner = _tr_str_slicev((s), (lt + 1LL), _tr_str_lenv((s)));
    /* pass */
    if ((_tr_str_index_ofv((inner), (_tr_str_lit_len("<", 1LL))) < 0LL)) {
        /* pass */
        _tr_str_release(inner);
        return false;
    }
    /* pass */
    if (_tr_str_starts_withv((s), (_tr_str_lit_len("std::", 5LL)))) {
        /* pass */
        _tr_str_release(inner);
        return false;
    }
    /* pass */
    _tr_str_release(inner);
    return true;
}

__attribute__((hot)) List_TrStr* _collect_specs(TrStr ir) {
    /* pass */
    List_TrStr* specs = (void*)List_TrStr_new();
    /* pass */
    TrMap* seen = _tr_dict_new(16LL);
    /* pass */
    List_TrStr* lines = _tr_str_splitv((ir), (_tr_str_lit_len("\n", 1LL)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1229 = (List_TrStr_get(lines, i)); __auto_type _wr = (_rstrip_cr(_at_t1229)); _tr_str_release(_at_t1229); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if (_tr_str_starts_withv((ln), (_tr_str_lit_len("TDSPEC ", 7LL)))) {
            /* pass */
            TrStr td = _tr_str_slicev((ln), 7LL, _tr_str_lenv((ln)));
            /* pass */
            if ((((_tr_str_index_ofv((td), (_tr_str_lit_len("<", 1LL))) >= 0LL) && (!_is_expr_proxy_spec(td))) && (!_tr_dict_contains(seen, _tr_strz(td))))) {
                /* pass */
                _tr_dict_set(seen, _tr_strz(td), true);
                /* pass */
                List_TrStr_append(specs, td);
            }
            /* pass */
            continue;
        }
        /* pass */
        __auto_type p = _tr_str_index_ofv((ln), (_tr_str_lit_len("~r~", 3LL)));
        /* pass */
        if ((p < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_slicev((ln), (p + 3LL), _tr_str_lenv((ln)));
        /* pass */
        TrStr detail = _tr_str_retain(rest);
        /* pass */
        __auto_type bar = _tr_str_index_ofv((rest), (_tr_str_lit_len("|", 1LL)));
        /* pass */
        if ((bar >= 0LL)) {
            /* pass */
            TrStr _strtmp_t1230 = _tr_str_slicev((rest), 0LL, bar);
            _tr_str_release(detail);
            detail = _strtmp_t1230;
        }
        /* pass */
        if ((((_tr_str_index_ofv((detail), (_tr_str_lit_len("<", 1LL))) >= 0LL) && (!_is_expr_proxy_spec(detail))) && (!_tr_dict_contains(seen, _tr_strz(detail))))) {
            /* pass */
            _tr_dict_set(seen, _tr_strz(detail), true);
            /* pass */
            List_TrStr_append(specs, detail);
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(detail);
    }
    /* pass */
    Dict_free(seen);
    List_TrStr_free(lines);
    return specs;
}

__attribute__((hot)) void run_bindgen_cpp(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs) {
    /* pass */
    TrStr guide = _detect_libclang(cc);
    /* pass */
    if ((!_tr_str_eqv((guide), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        ({ printf("%s", _tr_strz(guide)); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    ({ TrStr _at_t1231 = (_cxxwalk_src()); write_file(_tr_str_lit_len("_cxxwalk.c", 10LL), _at_t1231); _tr_str_release(_at_t1231); });
    /* pass */
    long long rc = ({ TrStr _aet_t1232 = (_tr_strx_concatv((cc), (_tr_str_lit_len(" _cxxwalk.c -o _cxxwalk.exe -lclang 2>_cxx_err.txt", 50LL)))); __auto_type _wr = (_tr_system(_aet_t1232.data)); _tr_str_release(_aet_t1232); _wr; });
    /* pass */
    if (((rc != 0LL) || (!file_exists(_tr_str_lit_len("_cxxwalk.exe", 12LL))))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: could not compile the libclang walker (see _cxx_err.txt)", 65LL))); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    TrStr clang_args = ({ TrStr _cl = (({ TrStr _cr = (_cpp_std_flag(extra)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("-x c++ ", 7LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cr = (_cpp_detect_include_dirs(cc)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if ((!_tr_str_eqv((extra), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        TrStr _strtmp_t1233 = ({ TrStr _cl = (_tr_strx_concatv((clang_args), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (extra)); _tr_str_release(_cl); _cres; });
        _tr_str_release(clang_args);
        clang_args = _strtmp_t1233;
    }
    /* pass */
    ({ TrStr _aet_t1234 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_local_exe(_tr_str_lit_len("_cxxwalk", 8LL))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" \"", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\" ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (clang_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" > _cxx.ir 2> _cxx_diag.txt", 27LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t1234.data); _tr_str_release(_aet_t1234); });
    /* pass */
    TrStr diag = _tr_str_lit_len("", 0LL);
    /* pass */
    if (file_exists(_tr_str_lit_len("_cxx_diag.txt", 13LL))) {
        /* pass */
        TrStr _strtmp_t1235 = read_file(_tr_str_lit_len("_cxx_diag.txt", 13LL));
        _tr_str_release(diag);
        diag = _strtmp_t1235;
    }
    /* pass */
    long long nfatal = _cpp_fatal_count(diag);
    /* pass */
    if (({ TrStr _at_t1236 = (read_file(_tr_str_lit_len("_cxx.ir", 7LL))); __auto_type _wr = (((!file_exists(_tr_str_lit_len("_cxx.ir", 7LL))) || _cpp_ir_is_empty(_at_t1236))); _tr_str_release(_at_t1236); _wr; })) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concatv((_tr_str_lit_len("bindgen: no bindable declarations found in ", 43LL)), (header)))); printf("\n"); });
        /* pass */
        if ((_tr_str_index_ofv((diag), (_tr_str_lit_len("DIAG ", 5LL))) >= 0LL)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit_len("bindgen: libclang could not fully parse the header:", 51LL))); printf("\n"); });
            /* pass */
            _cpp_print_diag(diag);
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit_len("", 0LL))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  Most 'file not found' errors mean a missing include dir. Re-run with the", 74LL))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  library's include path(s) (and any required defines), e.g.:", 61LL))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("    tauraroc bindgen ", 21LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" -h cpp -I<include-dir> [-I<more>] [-D<MACRO>]", 46LL))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        } else {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit_len("  (the header parsed cleanly but declares no public classes/functions to bind.)", 79LL))); printf("\n"); });
        }
        /* pass */
        _cpp_cleanup();
        /* pass */
        _tr_str_release(guide);
        _tr_str_release(clang_args);
        _tr_str_release(diag);
        return;
    }
    /* pass */
    TrStr ir = read_file(_tr_str_lit_len("_cxx.ir", 7LL));
    /* pass */
    List_TrStr* specs = _collect_specs(ir);
    /* pass */
    long long n_specs = 0LL;
    /* pass */
    if ((specs->len > 0LL)) {
        /* pass */
        StringBuilder* fsb = StringBuilder_init(512LL);
        /* pass */
        ({ TrStr _sbt_t1237 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("#include \"", 10LL)), (header))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(fsb, _sbt_t1237); _tr_str_release(_sbt_t1237); });
        /* pass */
        long long si = 0LL;
        /* pass */
        while ((si < specs->len)) {
            /* pass */
            ({ TrStr _sbt_t1238 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(specs, si)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("template class ", 15LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(";\n", 2LL))); _tr_str_release(_cl); _cres; })); StringBuilder_append(fsb, _sbt_t1238); _tr_str_release(_sbt_t1238); });
            /* pass */
            si = (si + 1LL);
        }
        /* pass */
        ({ TrStr _at_t1239 = (StringObj_as_str(StringBuilder_to_string(fsb))); write_file(_tr_str_lit_len("_cxx_force.cpp", 14LL), _at_t1239); _tr_str_release(_at_t1239); });
        /* pass */
        ({ TrStr _aet_t1240 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_local_exe(_tr_str_lit_len("_cxxwalk", 8LL))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" --inst _cxx_force.cpp ", 23LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (clang_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" > _cxx_inst.ir 2>> _cxx_diag.txt", 33LL))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t1240.data); _tr_str_release(_aet_t1240); });
        /* pass */
        if (({ TrStr _at_t1241 = (read_file(_tr_str_lit_len("_cxx_inst.ir", 12LL))); __auto_type _wr = ((file_exists(_tr_str_lit_len("_cxx_inst.ir", 12LL)) && (!_cpp_ir_is_empty(_at_t1241)))); _tr_str_release(_at_t1241); _wr; })) {
            /* pass */
            TrStr _strtmp_t1242 = ({ TrStr _cl = (_tr_strx_concatv((ir), (_tr_str_lit_len("\n", 1LL)))); TrStr _cr = (read_file(_tr_str_lit_len("_cxx_inst.ir", 12LL))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(ir);
            ir = _strtmp_t1242;
            /* pass */
            n_specs = specs->len;
        }
        StringBuilder__tr_fn_free(fsb);
    }
    /* pass */
    TrStr shim_cflags = _cpp_std_flag(extra);
    /* pass */
    if ((!_tr_str_eqv((extra), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        TrStr _strtmp_t1243 = ({ TrStr _cl = (_tr_strx_concatv((shim_cflags), (_tr_str_lit_len(" ", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (extra)); _tr_str_release(_cl); _cres; });
        _tr_str_release(shim_cflags);
        shim_cflags = _strtmp_t1243;
    }
    /* pass */
    _cpp_generate(ir, header, out, shim_cflags, pkglibs);
    /* pass */
    if ((n_specs > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_specs)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: instantiated ", 22LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" template specialization(s)", 27LL))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    if ((nfatal > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(nfatal)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("bindgen: WARNING — libclang reported ", 39LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" fatal error(s); bindings may be INCOMPLETE:", 44LL))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        _cpp_print_diag(diag);
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit_len("  Add the missing include dir(s) with -I<dir> for a complete binding.", 69LL))); printf("\n"); });
    }
    /* pass */
    _cpp_cleanup();
    _tr_str_release(guide);
    _tr_str_release(clang_args);
    _tr_str_release(diag);
    _tr_str_release(ir);
    List_TrStr_free(specs);
    _tr_str_release(shim_cflags);
}

