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
void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow);
TrStr _cxxwalk_src();
void _rm_files(TrStr files);
TrStr _local_exe(TrStr stem);
TrStr _detect_libclang(TrStr cc);
CppType* _cpp_parse_type(TrStr spelling);
TrStr _last_seg(TrStr s);
TrStr _cpp_ctype(CppType* t);
TrStr _cpp_tr_type(CppType* t);
List_TrStr* _cpp_ret(CppType* rt, TrStr call);
TrStr _ns_pop(TrStr path);
TrStr _ns_us(TrStr path);
TrStr _rstrip_cr(TrStr s);
void _cpp_generate(TrStr ir, TrStr header, TrStr out);
TrStr _cpp_detect_include_dirs(TrStr cc);
TrStr _cpp_std_flag(TrStr extra);
long long _cpp_fatal_count(TrStr diag);
void _cpp_print_diag(TrStr diag);
bool _cpp_ir_is_empty(TrStr ir);
void _cpp_cleanup();

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
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) == 0) || _tr_dict_contains(self->seen, _tr_strz(name)))) {
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
    return ((Bindgen_ck(self) == 4LL) && (strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(p)) == 0));
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
    return _tr_str_lit("");
}

__attribute__((hot)) void Bindgen_skip_balanced_parens(Bindgen* self) {
    /* pass */
    if ((!Bindgen_is_punct(self, _tr_str_lit("(")))) {
        /* pass */
        return;
    }
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("("))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
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
    if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
        /* pass */
        long long depth = 0LL;
        /* pass */
        while ((Bindgen_ck(self) != 5LL)) {
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                depth = (depth + 1LL);
            } else if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
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
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
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
        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
            /* pass */
            depth = (depth - 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit(";")) && (depth <= 0LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
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
            } else if (((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__declspec"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__attribute__"))) == 0))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                Bindgen_skip_balanced_parens(self);
            } else if ((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("union"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("enum"))) == 0))) {
                /* pass */
                Bindgen_adv(self);
            } else if (({ TrStr _at_t776 = (Bindgen_nt(self)); __auto_type _wr = (((((words->len >= 1LL) || (stars > 0LL)) && _is_decl_term(Bindgen_nk(self), _at_t776)) && (!_is_prim_type_word(w)))); _tr_str_release(_at_t776); _wr; })) {
                /* pass */
                going = false;
            } else {
                /* pass */
                List_TrStr_append(words, w);
                /* pass */
                Bindgen_adv(self);
            }
        } else if (Bindgen_is_punct(self, _tr_str_lit("*"))) {
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
    ({ TrStr _at_t777 = (_join_words(words)); List_TrStr_append(res, _at_t777); _tr_str_release(_at_t777); });
    /* pass */
    ({ TrStr _wt_t778 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); TrStr _at_t779 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); List_TrStr_append(res, _at_t779); _tr_str_release(_wt_t778); _tr_str_release(_at_t779); });
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
    ({ TrStr _sbt_t780 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->funcs, _sbt_t780); _tr_str_release(_sbt_t780); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    bool first = true;
    /* pass */
    long long argn = 0LL;
    /* pass */
    bool variadic = false;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(")"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long ploop_before = self->pos;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("."))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            variadic = true;
            /* pass */
            continue;
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(","))) {
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
        long long pstars = ({ TrStr _at_t781 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t781)); _tr_str_release(_at_t781); _wr; });
        /* pass */
        TrStr pname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("(")) && (strcmp(_tr_strz(Bindgen_nt(self)), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t782 = Bindgen_ct(self);
                _tr_str_release(pname);
                pname = _strtmp_t782;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t783 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(pname);
                pname = _strtmp_t783;
            }
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append(self->funcs, _tr_str_lit(", "));
            }
            /* pass */
            first = false;
            /* pass */
            ({ TrStr _sbt_t784 = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": Pointer[void]")))); StringBuilder_append(self->funcs, _sbt_t784); _tr_str_release(_sbt_t784); });
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
            TrStr _strtmp_t785 = Bindgen_ct(self);
            _tr_str_release(pname);
            pname = _strtmp_t785;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        while (Bindgen_is_punct(self, _tr_str_lit("["))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (((!Bindgen_is_punct(self, _tr_str_lit("]"))) && (Bindgen_ck(self) != 5LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("]"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            pstars = (pstars + 1LL);
        }
        /* pass */
        if ((((strcmp(_tr_strz(pbase), _tr_strz(_tr_str_lit("void"))) == 0) && (pstars == 0LL)) && (strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t786 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(pname);
            pname = _strtmp_t786;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            StringBuilder_append(self->funcs, _tr_str_lit(", "));
        }
        /* pass */
        first = false;
        /* pass */
        ({ TrStr _sbt_t787 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (map_type(pbase, pstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->funcs, _sbt_t787); _tr_str_release(_sbt_t787); });
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
    if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (variadic) {
        /* pass */
        StringBuilder_append(self->funcs, _tr_str_lit(", args..."));
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit(")"));
    /* pass */
    TrStr rt = map_type(ret_words, ret_stars);
    /* pass */
    if ((strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("void"))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t788 = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -> ")), _tr_strz(rt))); StringBuilder_append(self->funcs, _sbt_t788); _tr_str_release(_sbt_t788); });
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit("\n"));
    /* pass */
    self->n_funcs = (self->n_funcs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
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
        StringBuilder_append(self->structs, _tr_str_lit("@union\n"));
    }
    /* pass */
    ({ TrStr _sbt_t789 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@value_type\nclass ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t789); _tr_str_release(_sbt_t789); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long nfields = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit("}"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long floop_before = self->pos;
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr fbase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long fstars = ({ TrStr _at_t790 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t790)); _tr_str_release(_at_t790); _wr; });
        /* pass */
        bool more = true;
        /* pass */
        while (more) {
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
            while (Bindgen_is_punct(self, _tr_str_lit("["))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                TrStr arrn = _tr_str_lit("");
                /* pass */
                if ((Bindgen_ck(self) == 1LL)) {
                    /* pass */
                    TrStr _strtmp_t791 = Bindgen_ct(self);
                    _tr_str_release(arrn);
                    arrn = _strtmp_t791;
                }
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit("]"))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit("]"))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if ((strcmp(_tr_strz(arrn), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ({ TrStr _sbt_t792 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, fstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t792); _tr_str_release(_sbt_t792); });
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
                ({ TrStr _sbt_t793 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, fstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t793); _tr_str_release(_sbt_t793); });
                /* pass */
                nfields = (nfields + 1LL);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(","))) {
                /* pass */
                Bindgen_adv(self);
            } else {
                /* pass */
                more = false;
            }
            _tr_str_release(fname);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
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
    if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if ((nfields == 0LL)) {
        /* pass */
        StringBuilder_append(self->structs, _tr_str_lit("    pass\n"));
    }
    /* pass */
    StringBuilder_append(self->structs, _tr_str_lit("\n"));
    /* pass */
    self->n_structs = (self->n_structs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
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
        if ((Bindgen_is_punct(self, _tr_str_lit("(")) || Bindgen_is_punct(self, _tr_str_lit("[")))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit(")")) || Bindgen_is_punct(self, _tr_str_lit("]")))) {
            /* pass */
            depth = (depth - 1LL);
        } else if (((depth <= 0LL) && (Bindgen_is_punct(self, _tr_str_lit(",")) || Bindgen_is_punct(self, _tr_str_lit("}"))))) {
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_emit_enum(Bindgen* self, TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, name))) {
        /* pass */
        ({ TrStr _sbt_t794 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = c_int\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t794); _tr_str_release(_sbt_t794); });
    }
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long next_val = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit("}"))) && (Bindgen_ck(self) != 5LL))) {
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
        long long ev = next_val;
        /* pass */
        bool simple = true;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("="))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 1LL)) {
                /* pass */
                ev = ({ TrStr _at_t795 = (Bindgen_ct(self)); __auto_type _wr = (_to_int(_at_t795)); _tr_str_release(_at_t795); _wr; });
                /* pass */
                Bindgen_adv(self);
                /* pass */
                if ((!(Bindgen_is_punct(self, _tr_str_lit(",")) || Bindgen_is_punct(self, _tr_str_lit("}"))))) {
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
            ({ TrStr _sbt_t796 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ename))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ev)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->consts, _sbt_t796); _tr_str_release(_sbt_t796); });
            /* pass */
            next_val = (ev + 1LL);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(","))) {
            /* pass */
            Bindgen_adv(self);
        }
        _tr_str_release(ename);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
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
    if ((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("typedef"))) == 0)) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr agg = Bindgen_ct(self);
        /* pass */
        if (((strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("union"))) == 0))) {
            /* pass */
            bool is_u = (strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("union"))) == 0);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            TrStr tag = _tr_str_lit("");
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t797 = Bindgen_ct(self);
                _tr_str_release(tag);
                tag = _strtmp_t797;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                TrStr tname = _scan_typedef_name(self->toks, self->pos);
                /* pass */
                TrStr cname = _tr_str_retain(tname);
                /* pass */
                if ((strcmp(_tr_strz(cname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t798 = _tr_str_retain(tag);
                    _tr_str_release(cname);
                    cname = _strtmp_t798;
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
            while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                ostars = (ostars + 1LL);
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            TrStr oname = _tr_str_lit("");
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t799 = Bindgen_ct(self);
                _tr_str_release(oname);
                oname = _strtmp_t799;
            }
            /* pass */
            if ((((ostars > 0LL) && (strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(tag), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                if (Bindgen_fresh(self, tag)) {
                    /* pass */
                    ({ TrStr _sbt_t800 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t800); _tr_str_release(_sbt_t800); });
                    /* pass */
                    self->n_structs = (self->n_structs + 1LL);
                }
                /* pass */
                if (Bindgen_fresh(self, oname)) {
                    /* pass */
                    ({ TrStr _sbt_t801 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t801); _tr_str_release(_sbt_t801); });
                }
            } else if (((strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, oname))) {
                /* pass */
                ({ TrStr _sbt_t802 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t802); _tr_str_release(_sbt_t802); });
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
        if ((strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("enum"))) == 0)) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
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
            if (((strcmp(_tr_strz(fpn), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, fpn))) {
                /* pass */
                ({ TrStr _sbt_t803 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(fpn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer[void]    # C function pointer\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t803); _tr_str_release(_sbt_t803); });
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
        long long astars = ({ TrStr _at_t804 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t804)); _tr_str_release(_at_t804); _wr; });
        /* pass */
        TrStr aname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t805 = Bindgen_ct(self);
            _tr_str_release(aname);
            aname = _strtmp_t805;
        }
        /* pass */
        if (((((strcmp(_tr_strz(aname), _tr_strz(_tr_str_lit(""))) != 0) && (strcmp(_tr_strz(abase), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(aname), _tr_strz(abase)) != 0)) && Bindgen_fresh(self, aname))) {
            /* pass */
            ({ TrStr _sbt_t806 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(aname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(abase, astars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t806); _tr_str_release(_sbt_t806); });
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
    if (((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("union"))) == 0))) {
        /* pass */
        bool is_u2 = (strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("union"))) == 0);
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr tag2 = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t807 = Bindgen_ct(self);
            _tr_str_release(tag2);
            tag2 = _strtmp_t807;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("{")) && (strcmp(_tr_strz(tag2), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            Bindgen_emit_struct(self, tag2, is_u2);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(tag2);
            return;
        }
        /* pass */
        if (((Bindgen_is_punct(self, _tr_str_lit(";")) && (strcmp(_tr_strz(tag2), _tr_strz(_tr_str_lit(""))) != 0)) && Bindgen_fresh(self, tag2))) {
            /* pass */
            ({ TrStr _sbt_t808 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque (forward-declared)\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t808); _tr_str_release(_sbt_t808); });
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
    if ((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("enum"))) == 0)) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr etag = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t809 = Bindgen_ct(self);
            _tr_str_release(etag);
            etag = _strtmp_t809;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
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
    long long rstars = ({ TrStr _at_t810 = (List_TrStr_get(rw, 1LL)); __auto_type _wr = (_to_int(_at_t810)); _tr_str_release(_at_t810); _wr; });
    /* pass */
    TrStr fname = _tr_str_lit("");
    /* pass */
    if ((Bindgen_ck(self) == 0LL)) {
        /* pass */
        TrStr _strtmp_t811 = Bindgen_ct(self);
        _tr_str_release(fname);
        fname = _strtmp_t811;
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_is_punct(self, _tr_str_lit("(")))) {
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
    long long n = _tr_strlen(_tr_strz(src));
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
            ({ TrStr _at_t812 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(0LL, _at_t812)); _tr_str_release(_at_t812); });
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
            ({ TrStr _at_t813 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(1LL, _at_t813)); _tr_str_release(_at_t813); });
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
            ({ TrStr _at_t814 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(2LL, _at_t814)); _tr_str_release(_at_t814); });
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
            ({ TrStr _at_t815 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(3LL, _at_t815)); _tr_str_release(_at_t815); });
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _at_t816 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), i, (i + 1LL)))); List_ptr_append(toks, CTok_init(4LL, _at_t816)); _tr_str_release(_at_t816); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_ptr_append(toks, CTok_init(5LL, _tr_str_lit("")));
    /* pass */
    return toks;
}

__attribute__((hot)) TrStr map_base(TrStr words) {
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_int");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned int"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_uint");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_char");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uchar");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_schar");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("short"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("short int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed short"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_short");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned short"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned short int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ushort");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed long"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_long");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ulong");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long long int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed long long"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_longlong");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long long int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ulonglong");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("va_list"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("__builtin_va_list"))) == 0))) {
        /* pass */
        return _tr_str_lit("Pointer[void]");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("float"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_float");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("double"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_double");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long double"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ldouble");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("_Bool"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("bool"))) == 0))) {
        /* pass */
        return _tr_str_lit("bool");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("size_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_size_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("ssize_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ssize_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int8_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int16_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int32_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int64_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint8_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint16_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint32_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint64_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("intptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_intptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uintptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uintptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("ptrdiff_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ptrdiff_t");
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
    if ((strcmp(_tr_strz(base), _tr_strz(_tr_str_lit("c_char"))) == 0)) {
        /* pass */
        TrStr _strtmp_t817 = _tr_str_lit("char");
        _tr_str_release(inner);
        inner = _strtmp_t817;
    }
    /* pass */
    if ((strcmp(_tr_strz(base), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        TrStr _strtmp_t818 = _tr_str_lit("void");
        _tr_str_release(inner);
        inner = _strtmp_t818;
    }
    /* pass */
    TrStr ty = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(inner))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long d = 1LL;
    /* pass */
    while ((d < stars)) {
        /* pass */
        TrStr _strtmp_t819 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t819;
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
    TrStr names = _tr_str_lit("printf fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf scanf fscanf sscanf fopen fclose fread fwrite fseek ftell fgets fputs fgetc fputc getc putc getchar putchar puts perror fflush freopen setvbuf setbuf rewind remove rename tmpfile ungetc feof ferror clearerr ");
    /* pass */
    TrStr _strtmp_t820 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("malloc calloc realloc free aligned_alloc abort exit _Exit atexit quick_exit system getenv setenv unsetenv abs labs llabs atoi atol atoll atof strtol strtoul strtoll strtoull strtod strtof rand srand qsort bsearch div ldiv lldiv ")));
    _tr_str_release(names);
    names = _strtmp_t820;
    /* pass */
    TrStr _strtmp_t821 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("memcpy memmove memset memcmp memchr strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strlen strnlen strdup strndup strtok strspn strcspn strpbrk strerror strcoll strxfrm strcasecmp strncasecmp ")));
    _tr_str_release(names);
    names = _strtmp_t821;
    /* pass */
    TrStr _strtmp_t822 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("sqrt sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow cbrt hypot floor ceil round trunc fabs fmod ldexp frexp modf fmin fmax copysign nextafter nan isnan isinf signbit ")));
    _tr_str_release(names);
    names = _strtmp_t822;
    /* pass */
    TrStr _strtmp_t823 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("read write open close lseek unlink stat fstat mkdir rmdir access dup dup2 pipe fork execve waitpid kill getpid ")));
    _tr_str_release(names);
    names = _strtmp_t823;
    /* pass */
    TrStr _strtmp_t824 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("CreateWindow CloseWindow CreateWindowExA CreateWindowExW GetMessage DispatchMessage MessageBox MessageBoxA MessageBoxW ")));
    _tr_str_release(names);
    names = _strtmp_t824;
    /* pass */
    TrStr _strtmp_t825 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("Rectangle Ellipse Polygon Polyline Arc Chord Pie RoundRect LineTo MoveToEx FillRect FrameRect InvertRect DrawIcon DrawText DrawTextEx TextOut GetObject LoadImage CreateFont PlaySound Polygon PolyBezier ")));
    _tr_str_release(names);
    names = _strtmp_t825;
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(names), _tr_strz(_tr_str_lit(" ")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(parts, i)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            ({ TrStr _dkt_t826 = (List_TrStr_get(parts, i)); _tr_dict_set(m, _tr_strz(_dkt_t826), true); _tr_str_release(_dkt_t826); });
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
    return (((((((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("short"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("int"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("long"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("double"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("signed"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("unsigned"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("_Bool"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("bool"))) == 0));
}

__attribute__((hot)) bool _is_ignored_word(TrStr w) {
    /* pass */
    return (((((((((((((((((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("const"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("volatile"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("extern"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("static"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("inline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("register"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("auto"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__inline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__inline__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__forceinline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("restrict"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__restrict"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__restrict__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__cdecl"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__stdcall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__fastcall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__thiscall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("WINAPI"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("APIENTRY"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__extension__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("_Noreturn"))) == 0));
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
    return (((((((strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(","))) == 0) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(";"))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(")"))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("["))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("="))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("("))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("}"))) == 0));
}

__attribute__((hot)) TrStr _join_words(List_TrStr* words) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < words->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t827 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" ")));
            _tr_str_release(s);
            s = _strtmp_t827;
        }
        /* pass */
        TrStr _strtmp_t828 = ({ TrStr _cr = (List_TrStr_get(words, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t828;
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
    if (((_tr_strlen(_tr_strz(s)) > 0LL) && (_tr_strz(s)[0LL] == 45LL))) {
        /* pass */
        neg = true;
        /* pass */
        i = 1LL;
    }
    /* pass */
    if (((((i + 1LL) < _tr_strlen(_tr_strz(s))) && (_tr_strz(s)[i] == 48LL)) && ((_tr_strz(s)[(i + 1LL)] == 120LL) || (_tr_strz(s)[(i + 1LL)] == 88LL)))) {
        /* pass */
        i = (i + 2LL);
        /* pass */
        long long hd = 0LL;
        /* pass */
        while (((i < _tr_strlen(_tr_strz(s))) && (hd < 15LL))) {
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
    while (((i < _tr_strlen(_tr_strz(s))) && (dd < 18LL))) {
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
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("{"))) == 0))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("}"))) == 0))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                if ((((i + 1LL) < toks->len) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 0LL))) {
                    /* pass */
                    return _tr_str_retain(((CTok*)List_ptr_get(toks, (i + 1LL)))->text);
                }
                /* pass */
                return _tr_str_lit("");
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool _typedef_is_fnptr(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while (((i + 1LL) < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit(";"))) == 0))) {
            /* pass */
            return false;
        }
        /* pass */
        if (((((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("("))) == 0)) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && (strcmp(_tr_strz(((CTok*)List_ptr_get(toks, (i + 1LL)))->text), _tr_strz(_tr_str_lit("*"))) == 0))) {
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
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit(";"))) == 0))) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        if (((((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("("))) == 0)) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && (strcmp(_tr_strz(((CTok*)List_ptr_get(toks, (i + 1LL)))->text), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            if ((((CTok*)List_ptr_get(toks, (i + 2LL)))->kind == 0LL)) {
                /* pass */
                return _tr_str_retain(((CTok*)List_ptr_get(toks, (i + 2LL)))->text);
            }
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool _is_ident_byte(long long c) {
    /* pass */
    return (((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL));
}

__attribute__((hot)) TrStr _rename_word(TrStr text, TrStr old, TrStr new_) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((_tr_strlen(_tr_strz(text)) + 16LL));
    /* pass */
    long long n = _tr_strlen(_tr_strz(text));
    /* pass */
    long long ol = _tr_strlen(_tr_strz(old));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        bool matched = false;
        /* pass */
        if (({ TrStr _wt_t829 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + ol)))); __auto_type _wr = ((((i + ol) <= n) && (strcmp(_wt_t829.data, _tr_strz(old)) == 0))); _tr_str_release(_wt_t829); _wr; })) {
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
            ({ TrStr _sbt_t830 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + 1LL)))); StringBuilder_append(sb, _sbt_t830); _tr_str_release(_sbt_t830); });
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
    while ((i < _tr_strlen(_tr_strz(p)))) {
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
    return _tr_str_wrap(_tr_str_slice(_tr_strz(p), last, _tr_strlen(_tr_strz(p))));
}

__attribute__((hot)) TrStr _marker_file(TrStr line) {
    /* pass */
    if ((((_tr_strlen(_tr_strz(line)) < 3LL) || (_tr_strz(line)[0LL] != 35LL)) || (_tr_strz(line)[1LL] != 32LL))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type q1 = ({ char* _t831 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t831 ? (long long)(_t831 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t832 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t832 ? (long long)(_t832 - (_tr_strz(rest))) : -1LL; });
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, q2));
}

__attribute__((hot)) bool _marker_is_system(TrStr line) {
    /* pass */
    __auto_type q1 = ({ char* _t833 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t833 ? (long long)(_t833 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t834 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t834 ? (long long)(_t834 - (_tr_strz(rest))) : -1LL; });
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return false;
    }
    /* pass */
    return ({ TrStr _wt_t835 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); TrStr _wt_t836 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); __auto_type _wr = ((({ char* _t837 = strstr(_wt_t836.data, _tr_strz(_tr_str_lit(" 3"))); _t837 ? (long long)(_t837 - (_wt_t836.data)) : -1LL; }) >= 0LL)); _tr_str_release(_wt_t835); _tr_str_release(_wt_t836); _wr; });
}

__attribute__((hot)) TrStr _filter_to_target(TrStr raw, TrStr target) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(_tr_strlen(_tr_strz(raw)));
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(raw), _tr_strz(_tr_str_lit("\n")));
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
        if ((strcmp(_tr_strz(_marker_file(ln)), _tr_strz(_tr_str_lit(""))) != 0)) {
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
            StringBuilder_append(sb, _tr_str_lit("\n"));
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
    while ((i < _tr_strlen(_tr_strz(rest)))) {
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
    return _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, i));
}

__attribute__((hot)) TrMap* _load_baseline(TrStr cc) {
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    write_file(_tr_str_lit("_bindgen_empty.h"), _tr_str_lit(""));
    /* pass */
    ({ TrStr _aet_t838 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E -dM \"_bindgen_empty.h\" > \"_bindgen_base.i\" 2>_bindgen_err.txt")))); _tr_system(_aet_t838.data); _tr_str_release(_aet_t838); });
    /* pass */
    if ((!file_exists(_tr_str_lit("_bindgen_base.i")))) {
        /* pass */
        return m;
    }
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(read_file(_tr_str_lit("_bindgen_base.i"))), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t839 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t839)); _tr_str_release(_at_t839); _wr; });
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) != 0)) {
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
    List_TrStr* lines = _tr_str_split(_tr_strz(read_file(header)), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t840 = (List_TrStr_get(lines, i)); __auto_type _wr = (_lstrip(_at_t840)); _tr_str_release(_at_t840); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t841 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t841)); _tr_str_release(_at_t841); _wr; });
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) != 0)) {
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
    while (((i < _tr_strlen(_tr_strz(s))) && ((_tr_strz(s)[i] == 32LL) || (_tr_strz(s)[i] == 9LL)))) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), i, _tr_strlen(_tr_strz(s))));
}

__attribute__((hot)) void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(defs), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))));
        /* pass */
        __auto_type sp = ({ char* _t842 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit(" "))); _t842 ? (long long)(_t842 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if ((sp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, sp));
        /* pass */
        if ((({ char* _t843 = strstr(_tr_strz(nm), _tr_strz(_tr_str_lit("("))); _t843 ? (long long)(_t843 - (_tr_strz(nm))) : -1LL; }) >= 0LL)) {
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
        TrStr val = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), (sp + 1LL), _tr_strlen(_tr_strz(rest))));
        /* pass */
        if ((_tr_strlen(_tr_strz(val)) == 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        char c0 = _tr_strz(val)[0LL];
        /* pass */
        if ((c0 == 34LL)) {
            /* pass */
            ({ TrStr _sbt_t844 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t844); _tr_str_release(_sbt_t844); });
        } else if ((((c0 >= 48LL) && (c0 <= 57LL)) || (c0 == 45LL))) {
            /* pass */
            long long iv = _to_int(val);
            /* pass */
            ({ TrStr _sbt_t845 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(iv)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t845); _tr_str_release(_sbt_t845); });
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(val);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) void run_bindgen(TrStr header, TrStr out, TrStr cc) {
    /* pass */
    TrStr tmp_decls = _tr_str_lit("_bindgen_decls.i");
    /* pass */
    TrStr tmp_defs = _tr_str_lit("_bindgen_defs.i");
    /* pass */
    ({ TrStr _aet_t846 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_decls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t846.data); _tr_str_release(_aet_t846); });
    /* pass */
    ({ TrStr _aet_t847 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E -dM \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_defs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t847.data); _tr_str_release(_aet_t847); });
    /* pass */
    if ((!file_exists(tmp_decls))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: preprocessing failed (see _bindgen_err.txt)"))); printf("\n"); });
        /* pass */
        _tr_str_release(tmp_decls);
        _tr_str_release(tmp_defs);
        return;
    }
    /* pass */
    TrStr src = ({ TrStr _at_t848 = (read_file(tmp_decls)); __auto_type _wr = (_filter_to_target(_at_t848, header)); _tr_str_release(_at_t848); _wr; });
    /* pass */
    List_ptr* toks = tokenize_c(src);
    /* pass */
    Bindgen* bg = Bindgen_init(toks);
    /* pass */
    Bindgen_run(bg);
    /* pass */
    TrMap* baseline = _load_baseline(cc);
    /* pass */
    ({ TrStr _at_t849 = (read_file(tmp_defs)); emit_defines(bg, _at_t849, baseline, _target_define_names(header)); _tr_str_release(_at_t849); });
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t850 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# Auto-generated FFI bindings for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen).\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t850); _tr_str_release(_sbt_t850); });
    /* pass */
    TrStr ts = StringObj_as_str(StringBuilder_to_string(bg->types));
    /* pass */
    if ((strcmp(_tr_strz(ts), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t851 = (_tr_strx_concat(_tr_strz(ts), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t851); _tr_str_release(_sbt_t851); });
    }
    /* pass */
    TrStr ss = StringObj_as_str(StringBuilder_to_string(bg->structs));
    /* pass */
    if ((strcmp(_tr_strz(ss), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, ss);
    }
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(bg->consts));
    /* pass */
    if ((strcmp(_tr_strz(cs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t852 = (_tr_strx_concat(_tr_strz(cs), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t852); _tr_str_release(_sbt_t852); });
    }
    /* pass */
    TrStr fs = StringObj_as_str(StringBuilder_to_string(bg->funcs));
    /* pass */
    if ((strcmp(_tr_strz(fs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("extern \"C\":\n"));
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
            TrStr _strtmp_t854 = ({ TrStr _at_t853 = (_tr_strx_concat(_tr_strz(dn), _tr_strz(_tr_str_lit("_")))); __auto_type _wr = (_rename_word(result, dn, _at_t853)); _tr_str_release(_at_t853); _wr; });
            _tr_str_release(result);
            result = _strtmp_t854;
            /* pass */
            n_renamed = (n_renamed + 1LL);
        }
        _tr_str_release(dn);
    }
    /* pass */
    write_file(out, result);
    /* pass */
    _tr_system(_tr_strz(_tr_str_lit("rm -f _bindgen_decls.i _bindgen_defs.i _bindgen_base.i _bindgen_empty.h _bindgen_err.txt 2>/dev/null")));
    /* pass */
    TrStr msg = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: wrote ")), _tr_strz(out))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" — "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_structs)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" types, "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_funcs)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" functions"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if ((bg->n_skipped > 0LL)) {
        /* pass */
        TrStr _strtmp_t855 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(msg), _tr_strz(_tr_str_lit(" (")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_skipped)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" runtime/libc symbols skipped)"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(msg);
        msg = _strtmp_t855;
    }
    /* pass */
    ({ printf("%s", _tr_strz(msg)); printf("\n"); });
    _tr_str_release(tmp_decls);
    _tr_str_release(tmp_defs);
    _tr_str_release(src);
    List_ptr_free_obj(toks, _trdrop_CTok);
    _tr_obj_release(bg, _trdrop_Bindgen);
    _tr_str_release(ts);
    _tr_str_release(ss);
    _tr_str_release(cs);
    _tr_str_release(fs);
    _tr_str_release(result);
    _tr_str_release(msg);
}

__attribute__((hot)) TrStr _cxxwalk_src() {
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <clang-c/Index.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <stdio.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static const char* S(CXString s){ const char* p=clang_getCString(s); return p?p:\"\"; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void params(CXCursor c){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int n=clang_Cursor_getNumArguments(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    for(int i=0;i<n;i++){ CXCursor a=clang_Cursor_getArgument(c,i);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        CXString ty=clang_getTypeSpelling(clang_getArgType(clang_getCursorType(c),i));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        CXString nm=clang_getCursorSpelling(a);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        const char* nn=S(nm);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(nn[0]) printf(\"PARAM %s|%s\\n\", S(ty), nn);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        else      printf(\"PARAM %s|a%d\\n\", S(ty), i);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        clang_disposeString(ty); clang_disposeString(nm);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult visit(CXCursor c, CXCursor parent, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(!clang_Location_isFromMainFile(clang_getCursorLocation(c))) return CXChildVisit_Continue;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXString nmS=clang_getCursorSpelling(c); const char* nm=S(nmS);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXString rtS; const char* rt;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(k==CXCursor_Namespace){ printf(\"NS %s\\n\",nm); clang_visitChildren(c,visit,0); printf(\"ENS\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"CLASS %s\\n\",nm); clang_visitChildren(c,visit,0); printf(\"ECLASS\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        rtS=clang_getTypeSpelling(clang_getResultType(clang_getCursorType(c))); rt=S(rtS);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"METHOD %c%c %s|%s\\n\", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', rt, nm);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        params(c); printf(\"EMETHOD\\n\"); clang_disposeString(rtS); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR\\n\"); params(c); printf(\"ECTOR\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_Destructor){ printf(\"DTOR\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_FunctionDecl){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        rtS=clang_getTypeSpelling(clang_getResultType(clang_getCursorType(c))); rt=S(rtS);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"FUNC %s|%s\\n\", rt, nm); params(c); printf(\"EFUNC\\n\"); clang_disposeString(rtS); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_EnumDecl){ printf(\"ENUM %s\\n\", nm[0]?nm:\"anon\"); clang_visitChildren(c,visit,0); printf(\"EENUM\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(k==CXCursor_EnumConstantDecl){ printf(\"EVAL %s %lld\\n\", nm, clang_getEnumConstantDeclValue(c)); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_disposeString(nmS);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    return CXChildVisit_Continue;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("int main(int argc,char**argv){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(argc<2){ fprintf(stderr,\"usage: cxxwalk <header> [clang args...]\\n\"); return 2; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXIndex idx=clang_createIndex(0,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    /* clang args are forwarded verbatim from argv[2..] (bindgen builds them:\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("       -x c++ -std=... + detected include dirs + user -I/-D flags). */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXTranslationUnit tu=clang_parseTranslationUnit(idx,argv[1],(const char**)(argv+2),argc-2,0,0,CXTranslationUnit_None);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(!tu){ fprintf(stderr,\"cxxwalk: libclang could not create a translation unit\\n\"); return 1; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    /* Report error/fatal diagnostics on stderr (bindgen surfaces them); count fatals. */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    unsigned nd=clang_getNumDiagnostics(tu), nerr=0, nfatal=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    for(unsigned i=0;i<nd;i++){ CXDiagnostic dg=clang_getDiagnostic(tu,i);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        enum CXDiagnosticSeverity sv=clang_getDiagnosticSeverity(dg);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(sv>=CXDiagnostic_Error){ CXString ds=clang_formatDiagnostic(dg,clang_defaultDiagnosticDisplayOptions());\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("            fprintf(stderr,\"DIAG %s\\n\",clang_getCString(ds)); clang_disposeString(ds);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("            nerr++; if(sv==CXDiagnostic_Fatal) nfatal++; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        clang_disposeDiagnostic(dg); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    fprintf(stderr,\"DIAGERR %u %u\\n\", nerr, nfatal);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(clang_getTranslationUnitCursor(tu),visit,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_disposeTranslationUnit(tu);clang_disposeIndex(idx);return 0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void _rm_files(TrStr files) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _aet_t856 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("del /q ")), _tr_strz(files))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" >nul 2>&1"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t856.data); _tr_str_release(_aet_t856); });
    } else {
        /* pass */
        ({ TrStr _aet_t857 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f ")), _tr_strz(files))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t857.data); _tr_str_release(_aet_t857); });
    }
}

__attribute__((hot)) TrStr _local_exe(TrStr stem) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit(".\\")), _tr_strz(stem))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".exe"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("./")), _tr_strz(stem))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".exe"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr _detect_libclang(TrStr cc) {
    /* pass */
    write_file(_tr_str_lit("_cxxprobe.c"), _tr_str_lit("#include <clang-c/Index.h>\nint main(){ clang_createIndex(0,0); return 0; }\n"));
    /* pass */
    long long rc = ({ TrStr _aet_t858 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" _cxxprobe.c -o _cxxprobe.exe -lclang 2>_cxx_err.txt")))); __auto_type _wr = (_tr_system(_aet_t858.data)); _tr_str_release(_aet_t858); _wr; });
    /* pass */
    _rm_files(_tr_str_lit("_cxxprobe.c _cxxprobe.exe _cxx_err.txt"));
    /* pass */
    if ((rc == 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    StringBuilder* g = StringBuilder_init(768LL);
    /* pass */
    StringBuilder_append(g, _tr_str_lit("bindgen: `-h cpp` needs libclang (Clang's C API), which was not found.\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("         It is required ONLY for C++ headers; plain C headers never use it.\n\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Install it:\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Windows       winget install LLVM.LLVM   (or MSYS2: pacman -S mingw-w64-x86_64-clang)\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Debian/Ubuntu sudo apt install libclang-dev\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Fedora        sudo dnf install clang-devel\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Arch          sudo pacman -S clang\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  macOS         brew install llvm   (or Xcode Command Line Tools)\n\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Download        https://github.com/llvm/llvm-project/releases\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Then re-run:    tauraroc bindgen <header.hpp> -h cpp\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(g));
}

__attribute__((hot)) CppType* _cpp_parse_type(TrStr spelling) {
    /* pass */
    CppType* t = ((CppType*)_tr_obj_alloc(sizeof(CppType)));
    /* pass */
    t->base = _tr_str_lit("");
    /* pass */
    t->ptr = 0LL;
    /* pass */
    t->was_ptr = false;
    /* pass */
    t->was_ref = false;
    /* pass */
    t->is_prim = false;
    /* pass */
    TrStr norm = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(spelling)))) {
        /* pass */
        TrStr ch = _tr_str_wrap(_tr_str_slice(_tr_strz(spelling), i, (i + 1LL)));
        /* pass */
        if ((strcmp(_tr_strz(ch), _tr_strz(_tr_str_lit("*"))) == 0)) {
            /* pass */
            TrStr _strtmp_t859 = _tr_strx_concat(_tr_strz(norm), _tr_strz(_tr_str_lit(" * ")));
            _tr_str_release(norm);
            norm = _strtmp_t859;
        } else if ((strcmp(_tr_strz(ch), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            TrStr _strtmp_t860 = _tr_strx_concat(_tr_strz(norm), _tr_strz(_tr_str_lit(" & ")));
            _tr_str_release(norm);
            norm = _strtmp_t860;
        } else {
            /* pass */
            TrStr _strtmp_t861 = _tr_strx_concat(_tr_strz(norm), _tr_strz(ch));
            _tr_str_release(norm);
            norm = _strtmp_t861;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(ch);
    }
    /* pass */
    List_TrStr* words = _tr_str_split(_tr_strz(norm), _tr_strz(_tr_str_lit(" ")));
    /* pass */
    TrStr bw = _tr_str_lit("");
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < words->len)) {
        /* pass */
        TrStr w = List_TrStr_get(words, wi);
        /* pass */
        wi = (wi + 1LL);
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("const"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("volatile"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("struct"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("class"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("enum"))) == 0))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("*"))) == 0)) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ptr = true;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ref = true;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t862 = _tr_str_retain(w);
            _tr_str_release(bw);
            bw = _strtmp_t862;
        } else {
            /* pass */
            TrStr _strtmp_t863 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bw), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(w)); _tr_str_release(_cl); _cres; });
            _tr_str_release(bw);
            bw = _strtmp_t863;
        }
        _tr_str_release(w);
    }
    /* pass */
    t->base = _tr_str_retain(bw);
    /* pass */
    TrStr mb = map_base(bw);
    /* pass */
    if ((((strcmp(_tr_strz(mb), _tr_strz(bw)) != 0) || (strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit("void"))) == 0)) || (strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit("bool"))) == 0))) {
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
    List_TrStr* parts = _tr_str_split(_tr_strz(s), _tr_strz(_tr_str_lit("::")));
    /* pass */
    if ((parts->len == 0LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_retain(s);
    }
    /* pass */
    return List_TrStr_get(parts, (parts->len - 1LL));
}

__attribute__((hot)) TrStr _cpp_ctype(CppType* t) {
    /* pass */
    TrStr s = t->base;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < t->ptr)) {
        /* pass */
        s = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("*")));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_retain(s);
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
        TrStr _strtmp_t864 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t864;
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
    if (((strcmp(_tr_strz(rt->base), _tr_strz(_tr_str_lit("void"))) == 0) && (rt->ptr == 0LL))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("void"));
        /* pass */
        ({ TrStr _at_t865 = (_tr_strx_concat(_tr_strz(call), _tr_strz(_tr_str_lit(";")))); List_TrStr_append(r, _at_t865); _tr_str_release(_at_t865); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit(""));
        /* pass */
        return r;
    }
    /* pass */
    if (rt->is_prim) {
        /* pass */
        ({ TrStr _at_t866 = (_cpp_ctype(rt)); List_TrStr_append(r, _at_t866); _tr_str_release(_at_t866); });
        /* pass */
        ({ TrStr _at_t867 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t867); _tr_str_release(_at_t867); });
        /* pass */
        ({ TrStr _at_t868 = (_cpp_tr_type(rt)); List_TrStr_append(r, _at_t868); _tr_str_release(_at_t868); });
        /* pass */
        return r;
    }
    /* pass */
    TrStr trret = _cpp_tr_type(rt);
    /* pass */
    if (rt->was_ptr) {
        /* pass */
        ({ TrStr _at_t869 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t869); _tr_str_release(_at_t869); });
        /* pass */
        ({ TrStr _at_t870 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t870); _tr_str_release(_at_t870); });
        /* pass */
        List_TrStr_append(r, trret);
    } else if (rt->was_ref) {
        /* pass */
        ({ TrStr _at_t871 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t871); _tr_str_release(_at_t871); });
        /* pass */
        ({ TrStr _at_t872 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return &(")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t872); _tr_str_release(_at_t872); });
        /* pass */
        List_TrStr_append(r, trret);
    } else {
        /* pass */
        ({ TrStr _at_t873 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t873); _tr_str_release(_at_t873); });
        /* pass */
        ({ TrStr _at_t874 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return new ")), _tr_strz(rt->base))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t874); _tr_str_release(_at_t874); });
        /* pass */
        List_TrStr_append(r, trret);
    }
    /* pass */
    _tr_str_release(trret);
    return r;
}

__attribute__((hot)) TrStr _ns_pop(TrStr path) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(path), _tr_strz(_tr_str_lit("::")));
    /* pass */
    if ((parts->len <= 1LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_lit("");
    }
    /* pass */
    TrStr r = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (parts->len - 1LL))) {
        /* pass */
        if ((strcmp(_tr_strz(r), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t875 = List_TrStr_get(parts, i);
            _tr_str_release(r);
            r = _strtmp_t875;
        } else {
            /* pass */
            TrStr _strtmp_t876 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(r), _tr_strz(_tr_str_lit("::")))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(r);
            r = _strtmp_t876;
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
    List_TrStr* parts = _tr_str_split(_tr_strz(path), _tr_strz(_tr_str_lit("::")));
    /* pass */
    TrStr r = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(parts, i)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            if ((strcmp(_tr_strz(r), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t877 = List_TrStr_get(parts, i);
                _tr_str_release(r);
                r = _strtmp_t877;
            } else {
                /* pass */
                TrStr _strtmp_t878 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(r), _tr_strz(_tr_str_lit("_")))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(r);
                r = _strtmp_t878;
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
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    while (({ TrStr _wt_t879 = (_tr_str_wrap(_tr_str_slice(_tr_strz(s), (n - 1LL), n))); __auto_type _wr = (((n > 0LL) && (strcmp(_wt_t879.data, _tr_strz(_tr_str_lit("\r"))) == 0))); _tr_str_release(_wt_t879); _wr; })) {
        /* pass */
        n = (n - 1LL);
    }
    /* pass */
    if ((n == _tr_strlen(_tr_strz(s)))) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, n));
}

__attribute__((hot)) void _cpp_generate(TrStr ir, TrStr header, TrStr out) {
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
    TrStr ns_path = _tr_str_lit("");
    /* pass */
    TrStr cur_class = _tr_str_lit("");
    /* pass */
    TrStr cur_class_qual = _tr_str_lit("");
    /* pass */
    TrStr class_pfx = _tr_str_lit("");
    /* pass */
    long long n_classes = 0LL;
    /* pass */
    long long n_fns = 0LL;
    /* pass */
    TrStr mode = _tr_str_lit("");
    /* pass */
    TrStr m_flags = _tr_str_lit("..");
    /* pass */
    TrStr m_ret = _tr_str_lit("");
    /* pass */
    TrStr m_name = _tr_str_lit("");
    /* pass */
    List_TrStr* params = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr line = ({ TrStr _at_t880 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t880)); _tr_str_release(_at_t880); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("CTOR"))) == 0)) {
            /* pass */
            TrStr _strtmp_t881 = _tr_str_lit("ctor");
            _tr_str_release(mode);
            mode = _strtmp_t881;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("DTOR"))) == 0)) {
            /* pass */
            TrStr ds = _tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_delete")));
            /* pass */
            ({ TrStr _sbt_t882 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void ")), _tr_strz(ds))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self) { delete self; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t882); _tr_str_release(_sbt_t882); });
            /* pass */
            ({ TrStr _sbt_t883 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(ds))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(obj: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t883); _tr_str_release(_sbt_t883); });
            /* pass */
            n_fns = (n_fns + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ECLASS"))) == 0)) {
            /* pass */
            TrStr _strtmp_t884 = _tr_str_lit("");
            _tr_str_release(cur_class);
            cur_class = _strtmp_t884;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ENS"))) == 0)) {
            /* pass */
            TrStr _strtmp_t885 = _ns_pop(ns_path);
            _tr_str_release(ns_path);
            ns_path = _strtmp_t885;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EENUM"))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("NS ")))) {
            /* pass */
            TrStr nn = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 3LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t886 = _tr_str_retain(nn);
                _tr_str_release(ns_path);
                ns_path = _strtmp_t886;
            } else {
                /* pass */
                TrStr _strtmp_t887 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nn)); _tr_str_release(_cl); _cres; });
                _tr_str_release(ns_path);
                ns_path = _strtmp_t887;
            }
            /* pass */
            if (({ TrStr _dkt_t888 = (_tr_strx_concat(_tr_strz(_tr_str_lit("using:")), _tr_strz(ns_path))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t888)))); _tr_str_release(_dkt_t888); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t889 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("using namespace ")), _tr_strz(ns_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(usings, _sbt_t889); _tr_str_release(_sbt_t889); });
                /* pass */
                ({ TrStr _dkt_t890 = (_tr_strx_concat(_tr_strz(_tr_str_lit("using:")), _tr_strz(ns_path))); _tr_dict_set(seen, _tr_strz(_dkt_t890), true); _tr_str_release(_dkt_t890); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("CLASS ")))) {
            /* pass */
            TrStr _strtmp_t891 = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))));
            _tr_str_release(cur_class);
            cur_class = _strtmp_t891;
            /* pass */
            if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t892 = _tr_str_retain(cur_class);
                _tr_str_release(cur_class_qual);
                cur_class_qual = _strtmp_t892;
            } else {
                /* pass */
                TrStr _strtmp_t893 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; });
                _tr_str_release(cur_class_qual);
                cur_class_qual = _strtmp_t893;
            }
            /* pass */
            if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t894 = _tr_str_retain(cur_class);
                _tr_str_release(class_pfx);
                class_pfx = _strtmp_t894;
            } else {
                /* pass */
                TrStr _strtmp_t895 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; });
                _tr_str_release(class_pfx);
                class_pfx = _strtmp_t895;
            }
            /* pass */
            if (({ TrStr _dkt_t896 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t896)))); _tr_str_release(_dkt_t896); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t897 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(cur_class))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t897); _tr_str_release(_sbt_t897); });
                /* pass */
                ({ TrStr _dkt_t898 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t898), true); _tr_str_release(_dkt_t898); });
                /* pass */
                n_classes = (n_classes + 1LL);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ENUM ")))) {
            /* pass */
            TrStr en = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            if (({ TrStr _dkt_t899 = (_tr_strx_concat(_tr_strz(_tr_str_lit("enum:")), _tr_strz(en))); __auto_type _wr = (((strcmp(_tr_strz(en), _tr_strz(_tr_str_lit("anon"))) != 0) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t899))))); _tr_str_release(_dkt_t899); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t900 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(en))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = c_int\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t900); _tr_str_release(_sbt_t900); });
                /* pass */
                ({ TrStr _dkt_t901 = (_tr_strx_concat(_tr_strz(_tr_str_lit("enum:")), _tr_strz(en))); _tr_dict_set(seen, _tr_strz(_dkt_t901), true); _tr_str_release(_dkt_t901); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("EVAL ")))) {
            /* pass */
            TrStr er = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* ep = _tr_str_split(_tr_strz(er), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            TrStr ename = List_TrStr_get(ep, 0LL);
            /* pass */
            TrStr eval_ = _tr_str_lit("0");
            /* pass */
            if ((ep->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t902 = List_TrStr_get(ep, 1LL);
                _tr_str_release(eval_);
                eval_ = _strtmp_t902;
            }
            /* pass */
            ({ TrStr _sbt_t903 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ename))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(eval_)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t903); _tr_str_release(_sbt_t903); });
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("METHOD ")))) {
            /* pass */
            TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 7LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            TrStr _strtmp_t904 = _tr_str_wrap(_tr_str_slice(_tr_strz(r), 0LL, 2LL));
            _tr_str_release(m_flags);
            m_flags = _strtmp_t904;
            /* pass */
            TrStr r2 = _tr_str_wrap(_tr_str_slice(_tr_strz(r), 3LL, _tr_strlen(_tr_strz(r))));
            /* pass */
            List_TrStr* rn = _tr_str_split(_tr_strz(r2), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr _strtmp_t905 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t905;
            /* pass */
            TrStr _strtmp_t906 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t906;
            /* pass */
            TrStr _strtmp_t907 = _tr_str_lit("method");
            _tr_str_release(mode);
            mode = _strtmp_t907;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("FUNC ")))) {
            /* pass */
            TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* rn = _tr_str_split(_tr_strz(r), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr _strtmp_t908 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t908;
            /* pass */
            TrStr _strtmp_t909 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t909;
            /* pass */
            TrStr _strtmp_t910 = _tr_str_lit("..");
            _tr_str_release(m_flags);
            m_flags = _strtmp_t910;
            /* pass */
            TrStr _strtmp_t911 = _tr_str_lit("func");
            _tr_str_release(mode);
            mode = _strtmp_t911;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("PARAM ")))) {
            /* pass */
            ({ TrStr _wt_t912 = (_tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))))); TrStr _at_t913 = (_tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))))); List_TrStr_append(params, _at_t913); _tr_str_release(_wt_t912); _tr_str_release(_at_t913); });
            /* pass */
            continue;
        }
        /* pass */
        if ((((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EMETHOD"))) == 0) || (strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ECTOR"))) == 0)) || (strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EFUNC"))) == 0))) {
            /* pass */
            bool is_static = ({ TrStr _wt_t914 = (_tr_str_wrap(_tr_str_slice(_tr_strz(m_flags), 0LL, 1LL))); __auto_type _wr = ((strcmp(_wt_t914.data, _tr_strz(_tr_str_lit("s"))) == 0)); _tr_str_release(_wt_t914); _wr; });
            /* pass */
            bool is_const = ({ TrStr _wt_t915 = (_tr_str_wrap(_tr_str_slice(_tr_strz(m_flags), 1LL, 2LL))); __auto_type _wr = ((strcmp(_wt_t915.data, _tr_strz(_tr_str_lit("c"))) == 0)); _tr_str_release(_wt_t915); _wr; });
            /* pass */
            TrStr shimp = _tr_str_lit("");
            /* pass */
            TrStr trp = _tr_str_lit("");
            /* pass */
            TrStr fargs = _tr_str_lit("");
            /* pass */
            if (((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0) && (!is_static))) {
                /* pass */
                if (is_const) {
                    /* pass */
                    TrStr _strtmp_t916 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(cur_class_qual))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(shimp);
                    shimp = _strtmp_t916;
                } else {
                    /* pass */
                    TrStr _strtmp_t917 = _tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("* self")));
                    _tr_str_release(shimp);
                    shimp = _strtmp_t917;
                }
                /* pass */
                TrStr _strtmp_t918 = _tr_strx_concat(_tr_strz(_tr_str_lit("obj: ")), _tr_strz(cur_class));
                _tr_str_release(trp);
                trp = _strtmp_t918;
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
                List_TrStr* bar = _tr_str_split(_tr_strz(pv), _tr_strz(_tr_str_lit("|")));
                /* pass */
                TrStr ptype = List_TrStr_get(bar, 0LL);
                /* pass */
                TrStr pname = _tr_str_lit("");
                /* pass */
                if ((bar->len > 1LL)) {
                    /* pass */
                    TrStr _strtmp_t919 = List_TrStr_get(bar, 1LL);
                    _tr_str_release(pname);
                    pname = _strtmp_t919;
                }
                /* pass */
                if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t920 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
                    _tr_str_release(pname);
                    pname = _strtmp_t920;
                }
                /* pass */
                if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit("self"))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t921 = _tr_str_lit("obj");
                    _tr_str_release(pname);
                    pname = _strtmp_t921;
                }
                /* pass */
                CppType* t = _cpp_parse_type(ptype);
                /* pass */
                TrStr sc = _tr_str_lit("");
                /* pass */
                TrStr fw = _tr_str_lit("");
                /* pass */
                if (t->is_prim) {
                    /* pass */
                    TrStr _strtmp_t922 = ({ TrStr _cl = (({ TrStr _cl = (_cpp_ctype(t)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pname)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(sc);
                    sc = _strtmp_t922;
                    /* pass */
                    TrStr _strtmp_t923 = _tr_str_retain(pname);
                    _tr_str_release(fw);
                    fw = _strtmp_t923;
                } else {
                    /* pass */
                    long long nd = t->ptr;
                    /* pass */
                    if ((nd < 1LL)) {
                        /* pass */
                        nd = 1LL;
                    }
                    /* pass */
                    TrStr stars = _tr_str_lit("");
                    /* pass */
                    long long si = 0LL;
                    /* pass */
                    while ((si < nd)) {
                        /* pass */
                        TrStr _strtmp_t924 = _tr_strx_concat(_tr_strz(stars), _tr_strz(_tr_str_lit("*")));
                        _tr_str_release(stars);
                        stars = _strtmp_t924;
                        /* pass */
                        si = (si + 1LL);
                    }
                    /* pass */
                    TrStr _strtmp_t925 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(t->base), _tr_strz(stars))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pname)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(sc);
                    sc = _strtmp_t925;
                    /* pass */
                    if (t->was_ptr) {
                        /* pass */
                        TrStr _strtmp_t926 = _tr_str_retain(pname);
                        _tr_str_release(fw);
                        fw = _strtmp_t926;
                    } else {
                        /* pass */
                        TrStr _strtmp_t927 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(pname));
                        _tr_str_release(fw);
                        fw = _strtmp_t927;
                    }
                    _tr_str_release(stars);
                }
                /* pass */
                TrStr trt = _cpp_tr_type(t);
                /* pass */
                if ((strcmp(_tr_strz(shimp), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t928 = _tr_str_retain(sc);
                    _tr_str_release(shimp);
                    shimp = _strtmp_t928;
                } else {
                    /* pass */
                    TrStr _strtmp_t929 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(shimp), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sc)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(shimp);
                    shimp = _strtmp_t929;
                }
                /* pass */
                if ((strcmp(_tr_strz(fargs), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t930 = _tr_str_retain(fw);
                    _tr_str_release(fargs);
                    fargs = _strtmp_t930;
                } else {
                    /* pass */
                    TrStr _strtmp_t931 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(fargs), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fw)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(fargs);
                    fargs = _strtmp_t931;
                }
                /* pass */
                if ((strcmp(_tr_strz(trp), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t932 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trt)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(trp);
                    trp = _strtmp_t932;
                } else {
                    /* pass */
                    TrStr _strtmp_t933 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(trp), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trt)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(trp);
                    trp = _strtmp_t933;
                }
                _tr_str_release(pv);
                List_TrStr_free(bar);
                _tr_str_release(ptype);
                _tr_str_release(pname);
                _tr_obj_release(t, _trdrop_CppType);
                _tr_str_release(sc);
                _tr_str_release(fw);
                _tr_str_release(trt);
            }
            /* pass */
            TrStr call = _tr_str_lit("");
            /* pass */
            if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) == 0)) {
                /* pass */
                TrStr _strtmp_t934 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("new ")), _tr_strz(cur_class_qual))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(call);
                call = _strtmp_t934;
            } else if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0)) {
                /* pass */
                if (is_static) {
                    /* pass */
                    TrStr _strtmp_t935 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t935;
                } else {
                    /* pass */
                    TrStr _strtmp_t936 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("self->")), _tr_strz(m_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t936;
                }
            } else {
                /* pass */
                TrStr pre = _tr_str_lit("");
                /* pass */
                if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t937 = _tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")));
                    _tr_str_release(pre);
                    pre = _strtmp_t937;
                }
                /* pass */
                TrStr _strtmp_t938 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pre), _tr_strz(m_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(call);
                call = _strtmp_t938;
                _tr_str_release(pre);
            }
            /* pass */
            if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) == 0)) {
                /* pass */
                TrStr sym = _tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_new")));
                /* pass */
                ({ TrStr _sbt_t939 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("* ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") { return "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t939); _tr_str_release(_sbt_t939); });
                /* pass */
                ({ TrStr _sbt_t940 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t940); _tr_str_release(_sbt_t940); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(sym);
            } else {
                /* pass */
                CppType* rt = _cpp_parse_type(m_ret);
                /* pass */
                List_TrStr* ri = _cpp_ret(rt, call);
                /* pass */
                TrStr sym = _tr_str_lit("");
                /* pass */
                if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t941 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_name)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(sym);
                    sym = _strtmp_t941;
                } else {
                    /* pass */
                    if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t942 = _tr_str_retain(m_name);
                        _tr_str_release(sym);
                        sym = _strtmp_t942;
                    } else {
                        /* pass */
                        TrStr _strtmp_t943 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_name)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sym);
                        sym = _strtmp_t943;
                    }
                }
                /* pass */
                ({ TrStr _sbt_t944 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(ri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(ri, 1LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t944); _tr_str_release(_sbt_t944); });
                /* pass */
                TrStr d = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(ri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t945 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(d), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(ri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(d);
                    d = _strtmp_t945;
                }
                /* pass */
                ({ TrStr _sbt_t946 = (_tr_strx_concat(_tr_strz(d), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t946); _tr_str_release(_sbt_t946); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(sym);
                _tr_str_release(d);
            }
            /* pass */
            TrStr _strtmp_t947 = _tr_str_lit("");
            _tr_str_release(mode);
            mode = _strtmp_t947;
            /* pass */
            continue;
        }
        _tr_str_release(line);
    }
    /* pass */
    TrStr base = _tr_str_retain(out);
    /* pass */
    if (({ TrStr _wt_t948 = (_tr_str_wrap(_tr_str_slice(_tr_strz(base), (_tr_strlen(_tr_strz(base)) - 3LL), _tr_strlen(_tr_strz(base))))); __auto_type _wr = (((_tr_strlen(_tr_strz(base)) > 3LL) && (strcmp(_wt_t948.data, _tr_strz(_tr_str_lit(".tr"))) == 0))); _tr_str_release(_wt_t948); _wr; })) {
        /* pass */
        TrStr _strtmp_t949 = _tr_str_wrap(_tr_str_slice(_tr_strz(base), 0LL, (_tr_strlen(_tr_strz(base)) - 3LL)));
        _tr_str_release(base);
        base = _strtmp_t949;
    }
    /* pass */
    TrStr shim_name = _tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("_shim.cpp")));
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t950 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# Auto-generated C++ FFI bindings for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen -h cpp).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t950); _tr_str_release(_sbt_t950); });
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("# Build the shim once, then link it (and the C++ library) with your program:\n"));
    /* pass */
    ({ TrStr _sbt_t951 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#   c++ -c ")), _tr_strz(shim_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -o "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(base)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_shim.o\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t951); _tr_str_release(_sbt_t951); });
    /* pass */
    ({ TrStr _sbt_t952 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#   tauraroc yourapp.tr --link ")), _tr_strz(base))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_shim.o -lstdc++ -o app\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t952); _tr_str_release(_sbt_t952); });
    /* pass */
    TrStr os_ = StringObj_as_str(StringBuilder_to_string(opaque));
    /* pass */
    if ((strcmp(_tr_strz(os_), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t953 = (_tr_strx_concat(_tr_strz(os_), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t953); _tr_str_release(_sbt_t953); });
    }
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(consts));
    /* pass */
    if ((strcmp(_tr_strz(cs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t954 = (_tr_strx_concat(_tr_strz(cs), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t954); _tr_str_release(_sbt_t954); });
    }
    /* pass */
    TrStr ds2 = StringObj_as_str(StringBuilder_to_string(decls));
    /* pass */
    if ((strcmp(_tr_strz(ds2), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("extern \"C\":\n"));
        /* pass */
        StringBuilder_append(sb, ds2);
    }
    /* pass */
    ({ TrStr _at_t955 = (StringObj_as_str(StringBuilder_to_string(sb))); write_file(out, _at_t955); _tr_str_release(_at_t955); });
    /* pass */
    StringBuilder* sh = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t956 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("// Auto-generated C++ -> C shim for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen -h cpp).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t956); _tr_str_release(_sbt_t956); });
    /* pass */
    ({ TrStr _sbt_t957 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("// Compile:  c++ -c ")), _tr_strz(shim_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t957); _tr_str_release(_sbt_t957); });
    /* pass */
    ({ TrStr _sbt_t958 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#include \"")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t958); _tr_str_release(_sbt_t958); });
    /* pass */
    ({ TrStr _sbt_t959 = (StringObj_as_str(StringBuilder_to_string(usings))); StringBuilder_append(sh, _sbt_t959); _tr_str_release(_sbt_t959); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("extern \"C\" {\n"));
    /* pass */
    ({ TrStr _sbt_t960 = (StringObj_as_str(StringBuilder_to_string(shim))); StringBuilder_append(sh, _sbt_t960); _tr_str_release(_sbt_t960); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("}\n"));
    /* pass */
    ({ TrStr _at_t961 = (StringObj_as_str(StringBuilder_to_string(sh))); write_file(shim_name, _at_t961); _tr_str_release(_at_t961); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: wrote ")), _tr_strz(out))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" + "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" — "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_classes)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" classes, "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_fns)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" wrappers"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: next:  c++ -c ")), _tr_strz(shim_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("   then link with  --link "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(base)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_shim.o -lstdc++"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    Dict_free(seen);
    _tr_str_release(ns_path);
    _tr_str_release(cur_class);
    _tr_str_release(cur_class_qual);
    _tr_str_release(class_pfx);
    _tr_str_release(mode);
    _tr_str_release(m_flags);
    _tr_str_release(m_ret);
    _tr_str_release(m_name);
    List_TrStr_free(params);
    List_TrStr_free(lines);
    _tr_str_release(base);
    _tr_str_release(shim_name);
    _tr_str_release(os_);
    _tr_str_release(cs);
    _tr_str_release(ds2);
}

__attribute__((hot)) TrStr _cpp_detect_include_dirs(TrStr cc) {
    /* pass */
    write_file(_tr_str_lit("_cxx_empty.cpp"), _tr_str_lit("\n"));
    /* pass */
    ({ TrStr _aet_t962 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -x c++ -E -v _cxx_empty.cpp > _cxx_pp.txt 2> _cxx_inc.txt")))); _tr_system(_aet_t962.data); _tr_str_release(_aet_t962); });
    /* pass */
    TrStr out = _tr_str_lit("");
    /* pass */
    if (file_exists(_tr_str_lit("_cxx_inc.txt"))) {
        /* pass */
        TrStr txt = read_file(_tr_str_lit("_cxx_inc.txt"));
        /* pass */
        List_TrStr* lines = _tr_str_split(_tr_strz(txt), _tr_strz(_tr_str_lit("\n")));
        /* pass */
        bool collecting = false;
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < lines->len)) {
            /* pass */
            TrStr ln = ({ TrStr _at_t963 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t963)); _tr_str_release(_at_t963); _wr; });
            /* pass */
            li = (li + 1LL);
            /* pass */
            if ((({ char* _t964 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("search starts here"))); _t964 ? (long long)(_t964 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
                /* pass */
                collecting = true;
                /* pass */
                continue;
            }
            /* pass */
            if ((({ char* _t965 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("End of search list"))); _t965 ? (long long)(_t965 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
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
                __auto_type fpos = ({ char* _t966 = strstr(_tr_strz(d), _tr_strz(_tr_str_lit(" (framework directory)"))); _t966 ? (long long)(_t966 - (_tr_strz(d))) : -1LL; });
                /* pass */
                if ((fpos >= 0LL)) {
                    /* pass */
                    TrStr _strtmp_t967 = _tr_str_wrap(_tr_str_slice(_tr_strz(d), 0LL, fpos));
                    _tr_str_release(d);
                    d = _strtmp_t967;
                }
                /* pass */
                if ((strcmp(_tr_strz(d), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t968 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit(" -I\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(d)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(out);
                    out = _strtmp_t968;
                }
            }
            _tr_str_release(ln);
        }
        _tr_str_release(txt);
    }
    /* pass */
    _rm_files(_tr_str_lit("_cxx_empty.cpp _cxx_pp.txt _cxx_inc.txt"));
    /* pass */
    return out;
}

__attribute__((hot)) TrStr _cpp_std_flag(TrStr extra) {
    /* pass */
    if ((({ char* _t969 = strstr(_tr_strz(extra), _tr_strz(_tr_str_lit("-std="))); _t969 ? (long long)(_t969 - (_tr_strz(extra))) : -1LL; }) >= 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_lit("-std=c++17 ");
}

__attribute__((hot)) long long _cpp_fatal_count(TrStr diag) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(diag), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long f = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t970 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t970)); _tr_str_release(_at_t970); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if ((({ char* _t971 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("DIAGERR "))); _t971 ? (long long)(_t971 - (_tr_strz(ln))) : -1LL; }) == 0LL)) {
            /* pass */
            List_TrStr* parts = _tr_str_split(_tr_strz(ln), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            if ((parts->len >= 3LL)) {
                /* pass */
                f = ({ TrStr _at_t972 = (List_TrStr_get(parts, 2LL)); __auto_type _wr = (_to_int(_at_t972)); _tr_str_release(_at_t972); _wr; });
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
    List_TrStr* lines = _tr_str_split(_tr_strz(diag), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long shown = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t973 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t973)); _tr_str_release(_at_t973); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if (((({ char* _t974 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("DIAG "))); _t974 ? (long long)(_t974 - (_tr_strz(ln))) : -1LL; }) == 0LL) && (shown < 12LL))) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cr = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 5LL, _tr_strlen(_tr_strz(ln))))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("    ")), _cr.data); _tr_str_release(_cr); _cres; }))); printf("\n"); });
            /* pass */
            shown = (shown + 1LL);
        }
        _tr_str_release(ln);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) bool _cpp_ir_is_empty(TrStr ir) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        if (({ TrStr _at_t975 = (List_TrStr_get(lines, li)); __auto_type _wr = ((strcmp(_tr_strz(_rstrip_cr(_at_t975)), _tr_strz(_tr_str_lit(""))) != 0)); _tr_str_release(_at_t975); _wr; })) {
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
    _rm_files(_tr_str_lit("_cxxwalk.c _cxxwalk.exe _cxx.ir _cxx_err.txt _cxx_diag.txt"));
}

__attribute__((hot)) void run_bindgen_cpp(TrStr header, TrStr out, TrStr cc, TrStr extra) {
    /* pass */
    TrStr guide = _detect_libclang(cc);
    /* pass */
    if ((strcmp(_tr_strz(guide), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(guide)); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    ({ TrStr _at_t976 = (_cxxwalk_src()); write_file(_tr_str_lit("_cxxwalk.c"), _at_t976); _tr_str_release(_at_t976); });
    /* pass */
    long long rc = ({ TrStr _aet_t977 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" _cxxwalk.c -o _cxxwalk.exe -lclang 2>_cxx_err.txt")))); __auto_type _wr = (_tr_system(_aet_t977.data)); _tr_str_release(_aet_t977); _wr; });
    /* pass */
    if (((rc != 0LL) || (!file_exists(_tr_str_lit("_cxxwalk.exe"))))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: could not compile the libclang walker (see _cxx_err.txt)"))); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    TrStr clang_args = ({ TrStr _cl = (({ TrStr _cr = (_cpp_std_flag(extra)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-x c++ ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cr = (_cpp_detect_include_dirs(cc)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if ((strcmp(_tr_strz(extra), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t978 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(clang_args), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(extra)); _tr_str_release(_cl); _cres; });
        _tr_str_release(clang_args);
        clang_args = _strtmp_t978;
    }
    /* pass */
    ({ TrStr _aet_t979 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_local_exe(_tr_str_lit("_cxxwalk"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(clang_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" > _cxx.ir 2> _cxx_diag.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t979.data); _tr_str_release(_aet_t979); });
    /* pass */
    TrStr diag = _tr_str_lit("");
    /* pass */
    if (file_exists(_tr_str_lit("_cxx_diag.txt"))) {
        /* pass */
        TrStr _strtmp_t980 = read_file(_tr_str_lit("_cxx_diag.txt"));
        _tr_str_release(diag);
        diag = _strtmp_t980;
    }
    /* pass */
    long long nfatal = _cpp_fatal_count(diag);
    /* pass */
    if (({ TrStr _at_t981 = (read_file(_tr_str_lit("_cxx.ir"))); __auto_type _wr = (((!file_exists(_tr_str_lit("_cxx.ir"))) || _cpp_ir_is_empty(_at_t981))); _tr_str_release(_at_t981); _wr; })) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: no bindable declarations found in ")), _tr_strz(header)))); printf("\n"); });
        /* pass */
        if ((({ char* _t982 = strstr(_tr_strz(diag), _tr_strz(_tr_str_lit("DIAG "))); _t982 ? (long long)(_t982 - (_tr_strz(diag))) : -1LL; }) >= 0LL)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: libclang could not fully parse the header:"))); printf("\n"); });
            /* pass */
            _cpp_print_diag(diag);
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit(""))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  Most 'file not found' errors mean a missing include dir. Re-run with the"))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  library's include path(s) (and any required defines), e.g.:"))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    tauraroc bindgen ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -h cpp -I<include-dir> [-I<more>] [-D<MACRO>]"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        } else {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  (the header parsed cleanly but declares no public classes/functions to bind.)"))); printf("\n"); });
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
    TrStr ir = read_file(_tr_str_lit("_cxx.ir"));
    /* pass */
    _cpp_generate(ir, header, out);
    /* pass */
    if ((nfatal > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(nfatal)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: WARNING — libclang reported ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" fatal error(s); bindings may be INCOMPLETE:"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        _cpp_print_diag(diag);
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("  Add the missing include dir(s) with -I<dir> for a complete binding."))); printf("\n"); });
    }
    /* pass */
    _cpp_cleanup();
    _tr_str_release(guide);
    _tr_str_release(clang_args);
    _tr_str_release(diag);
    _tr_str_release(ir);
}

