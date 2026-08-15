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
            } else if (({ TrStr _at_t765 = (Bindgen_nt(self)); __auto_type _wr = (((((words->len >= 1LL) || (stars > 0LL)) && _is_decl_term(Bindgen_nk(self), _at_t765)) && (!_is_prim_type_word(w)))); _tr_str_release(_at_t765); _wr; })) {
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
    ({ TrStr _at_t766 = (_join_words(words)); List_TrStr_append(res, _at_t766); _tr_str_release(_at_t766); });
    /* pass */
    ({ TrStr _wt_t767 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); TrStr _at_t768 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); List_TrStr_append(res, _at_t768); _tr_str_release(_wt_t767); _tr_str_release(_at_t768); });
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
    ({ TrStr _sbt_t769 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->funcs, _sbt_t769); _tr_str_release(_sbt_t769); });
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
        long long pstars = ({ TrStr _at_t770 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t770)); _tr_str_release(_at_t770); _wr; });
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
                TrStr _strtmp_t771 = Bindgen_ct(self);
                _tr_str_release(pname);
                pname = _strtmp_t771;
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
                TrStr _strtmp_t772 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(pname);
                pname = _strtmp_t772;
            }
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append(self->funcs, _tr_str_lit(", "));
            }
            /* pass */
            first = false;
            /* pass */
            ({ TrStr _sbt_t773 = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": Pointer[void]")))); StringBuilder_append(self->funcs, _sbt_t773); _tr_str_release(_sbt_t773); });
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
            TrStr _strtmp_t774 = Bindgen_ct(self);
            _tr_str_release(pname);
            pname = _strtmp_t774;
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
            TrStr _strtmp_t775 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(pname);
            pname = _strtmp_t775;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            StringBuilder_append(self->funcs, _tr_str_lit(", "));
        }
        /* pass */
        first = false;
        /* pass */
        ({ TrStr _sbt_t776 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (map_type(pbase, pstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->funcs, _sbt_t776); _tr_str_release(_sbt_t776); });
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
        ({ TrStr _sbt_t777 = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -> ")), _tr_strz(rt))); StringBuilder_append(self->funcs, _sbt_t777); _tr_str_release(_sbt_t777); });
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
    ({ TrStr _sbt_t778 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@value_type\nclass ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t778); _tr_str_release(_sbt_t778); });
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
        long long fstars = ({ TrStr _at_t779 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t779)); _tr_str_release(_at_t779); _wr; });
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
                    TrStr _strtmp_t780 = Bindgen_ct(self);
                    _tr_str_release(arrn);
                    arrn = _strtmp_t780;
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
                    ({ TrStr _sbt_t781 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, fstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t781); _tr_str_release(_sbt_t781); });
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
                ({ TrStr _sbt_t782 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, fstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t782); _tr_str_release(_sbt_t782); });
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
        ({ TrStr _sbt_t783 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = c_int\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t783); _tr_str_release(_sbt_t783); });
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
                ev = ({ TrStr _at_t784 = (Bindgen_ct(self)); __auto_type _wr = (_to_int(_at_t784)); _tr_str_release(_at_t784); _wr; });
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
            ({ TrStr _sbt_t785 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ename))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ev)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->consts, _sbt_t785); _tr_str_release(_sbt_t785); });
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
                TrStr _strtmp_t786 = Bindgen_ct(self);
                _tr_str_release(tag);
                tag = _strtmp_t786;
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
                    TrStr _strtmp_t787 = _tr_str_retain(tag);
                    _tr_str_release(cname);
                    cname = _strtmp_t787;
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
                TrStr _strtmp_t788 = Bindgen_ct(self);
                _tr_str_release(oname);
                oname = _strtmp_t788;
            }
            /* pass */
            if ((((ostars > 0LL) && (strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(tag), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                if (Bindgen_fresh(self, tag)) {
                    /* pass */
                    ({ TrStr _sbt_t789 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t789); _tr_str_release(_sbt_t789); });
                    /* pass */
                    self->n_structs = (self->n_structs + 1LL);
                }
                /* pass */
                if (Bindgen_fresh(self, oname)) {
                    /* pass */
                    ({ TrStr _sbt_t790 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t790); _tr_str_release(_sbt_t790); });
                }
            } else if (((strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, oname))) {
                /* pass */
                ({ TrStr _sbt_t791 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t791); _tr_str_release(_sbt_t791); });
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
                ({ TrStr _sbt_t792 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(fpn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer[void]    # C function pointer\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t792); _tr_str_release(_sbt_t792); });
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
        long long astars = ({ TrStr _at_t793 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t793)); _tr_str_release(_at_t793); _wr; });
        /* pass */
        TrStr aname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t794 = Bindgen_ct(self);
            _tr_str_release(aname);
            aname = _strtmp_t794;
        }
        /* pass */
        if (((((strcmp(_tr_strz(aname), _tr_strz(_tr_str_lit(""))) != 0) && (strcmp(_tr_strz(abase), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(aname), _tr_strz(abase)) != 0)) && Bindgen_fresh(self, aname))) {
            /* pass */
            ({ TrStr _sbt_t795 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(aname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(abase, astars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t795); _tr_str_release(_sbt_t795); });
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
            TrStr _strtmp_t796 = Bindgen_ct(self);
            _tr_str_release(tag2);
            tag2 = _strtmp_t796;
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
            ({ TrStr _sbt_t797 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque (forward-declared)\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t797); _tr_str_release(_sbt_t797); });
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
            TrStr _strtmp_t798 = Bindgen_ct(self);
            _tr_str_release(etag);
            etag = _strtmp_t798;
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
    long long rstars = ({ TrStr _at_t799 = (List_TrStr_get(rw, 1LL)); __auto_type _wr = (_to_int(_at_t799)); _tr_str_release(_at_t799); _wr; });
    /* pass */
    TrStr fname = _tr_str_lit("");
    /* pass */
    if ((Bindgen_ck(self) == 0LL)) {
        /* pass */
        TrStr _strtmp_t800 = Bindgen_ct(self);
        _tr_str_release(fname);
        fname = _strtmp_t800;
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
            ({ TrStr _at_t801 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(0LL, _at_t801)); _tr_str_release(_at_t801); });
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
            ({ TrStr _at_t802 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(1LL, _at_t802)); _tr_str_release(_at_t802); });
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
            ({ TrStr _at_t803 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(2LL, _at_t803)); _tr_str_release(_at_t803); });
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
            ({ TrStr _at_t804 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(3LL, _at_t804)); _tr_str_release(_at_t804); });
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _at_t805 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), i, (i + 1LL)))); List_ptr_append(toks, CTok_init(4LL, _at_t805)); _tr_str_release(_at_t805); });
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
        TrStr _strtmp_t806 = _tr_str_lit("char");
        _tr_str_release(inner);
        inner = _strtmp_t806;
    }
    /* pass */
    if ((strcmp(_tr_strz(base), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        TrStr _strtmp_t807 = _tr_str_lit("void");
        _tr_str_release(inner);
        inner = _strtmp_t807;
    }
    /* pass */
    TrStr ty = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(inner))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long d = 1LL;
    /* pass */
    while ((d < stars)) {
        /* pass */
        TrStr _strtmp_t808 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t808;
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
    TrStr _strtmp_t809 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("malloc calloc realloc free aligned_alloc abort exit _Exit atexit quick_exit system getenv setenv unsetenv abs labs llabs atoi atol atoll atof strtol strtoul strtoll strtoull strtod strtof rand srand qsort bsearch div ldiv lldiv ")));
    _tr_str_release(names);
    names = _strtmp_t809;
    /* pass */
    TrStr _strtmp_t810 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("memcpy memmove memset memcmp memchr strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strlen strnlen strdup strndup strtok strspn strcspn strpbrk strerror strcoll strxfrm strcasecmp strncasecmp ")));
    _tr_str_release(names);
    names = _strtmp_t810;
    /* pass */
    TrStr _strtmp_t811 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("sqrt sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow cbrt hypot floor ceil round trunc fabs fmod ldexp frexp modf fmin fmax copysign nextafter nan isnan isinf signbit ")));
    _tr_str_release(names);
    names = _strtmp_t811;
    /* pass */
    TrStr _strtmp_t812 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("read write open close lseek unlink stat fstat mkdir rmdir access dup dup2 pipe fork execve waitpid kill getpid ")));
    _tr_str_release(names);
    names = _strtmp_t812;
    /* pass */
    TrStr _strtmp_t813 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("CreateWindow CloseWindow CreateWindowExA CreateWindowExW GetMessage DispatchMessage MessageBox MessageBoxA MessageBoxW ")));
    _tr_str_release(names);
    names = _strtmp_t813;
    /* pass */
    TrStr _strtmp_t814 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("Rectangle Ellipse Polygon Polyline Arc Chord Pie RoundRect LineTo MoveToEx FillRect FrameRect InvertRect DrawIcon DrawText DrawTextEx TextOut GetObject LoadImage CreateFont PlaySound Polygon PolyBezier ")));
    _tr_str_release(names);
    names = _strtmp_t814;
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
            ({ TrStr _dkt_t815 = (List_TrStr_get(parts, i)); _tr_dict_set(m, _tr_strz(_dkt_t815), true); _tr_str_release(_dkt_t815); });
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
            TrStr _strtmp_t816 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" ")));
            _tr_str_release(s);
            s = _strtmp_t816;
        }
        /* pass */
        TrStr _strtmp_t817 = ({ TrStr _cr = (List_TrStr_get(words, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t817;
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
        if (({ TrStr _wt_t818 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + ol)))); __auto_type _wr = ((((i + ol) <= n) && (strcmp(_wt_t818.data, _tr_strz(old)) == 0))); _tr_str_release(_wt_t818); _wr; })) {
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
            ({ TrStr _sbt_t819 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + 1LL)))); StringBuilder_append(sb, _sbt_t819); _tr_str_release(_sbt_t819); });
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
    __auto_type q1 = ({ char* _t820 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t820 ? (long long)(_t820 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t821 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t821 ? (long long)(_t821 - (_tr_strz(rest))) : -1LL; });
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
    __auto_type q1 = ({ char* _t822 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t822 ? (long long)(_t822 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t823 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t823 ? (long long)(_t823 - (_tr_strz(rest))) : -1LL; });
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return false;
    }
    /* pass */
    return ({ TrStr _wt_t824 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); TrStr _wt_t825 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); __auto_type _wr = ((({ char* _t826 = strstr(_wt_t825.data, _tr_strz(_tr_str_lit(" 3"))); _t826 ? (long long)(_t826 - (_wt_t825.data)) : -1LL; }) >= 0LL)); _tr_str_release(_wt_t824); _tr_str_release(_wt_t825); _wr; });
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
    ({ TrStr _aet_t827 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E -dM \"_bindgen_empty.h\" > \"_bindgen_base.i\" 2>_bindgen_err.txt")))); _tr_system(_aet_t827.data); _tr_str_release(_aet_t827); });
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
        TrStr nm = ({ TrStr _at_t828 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t828)); _tr_str_release(_at_t828); _wr; });
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
        TrStr ln = ({ TrStr _at_t829 = (List_TrStr_get(lines, i)); __auto_type _wr = (_lstrip(_at_t829)); _tr_str_release(_at_t829); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t830 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t830)); _tr_str_release(_at_t830); _wr; });
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
        __auto_type sp = ({ char* _t831 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit(" "))); _t831 ? (long long)(_t831 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if ((sp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, sp));
        /* pass */
        if ((({ char* _t832 = strstr(_tr_strz(nm), _tr_strz(_tr_str_lit("("))); _t832 ? (long long)(_t832 - (_tr_strz(nm))) : -1LL; }) >= 0LL)) {
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
            ({ TrStr _sbt_t833 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t833); _tr_str_release(_sbt_t833); });
        } else if ((((c0 >= 48LL) && (c0 <= 57LL)) || (c0 == 45LL))) {
            /* pass */
            long long iv = _to_int(val);
            /* pass */
            ({ TrStr _sbt_t834 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(iv)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t834); _tr_str_release(_sbt_t834); });
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
    ({ TrStr _aet_t835 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_decls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t835.data); _tr_str_release(_aet_t835); });
    /* pass */
    ({ TrStr _aet_t836 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E -dM \"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_defs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t836.data); _tr_str_release(_aet_t836); });
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
    TrStr src = ({ TrStr _at_t837 = (read_file(tmp_decls)); __auto_type _wr = (_filter_to_target(_at_t837, header)); _tr_str_release(_at_t837); _wr; });
    /* pass */
    List_ptr* toks = tokenize_c(src);
    /* pass */
    Bindgen* bg = Bindgen_init(toks);
    /* pass */
    Bindgen_run(bg);
    /* pass */
    TrMap* baseline = _load_baseline(cc);
    /* pass */
    ({ TrStr _at_t838 = (read_file(tmp_defs)); emit_defines(bg, _at_t838, baseline, _target_define_names(header)); _tr_str_release(_at_t838); });
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t839 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# Auto-generated FFI bindings for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen).\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t839); _tr_str_release(_sbt_t839); });
    /* pass */
    TrStr ts = StringObj_as_str(StringBuilder_to_string(bg->types));
    /* pass */
    if ((strcmp(_tr_strz(ts), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t840 = (_tr_strx_concat(_tr_strz(ts), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t840); _tr_str_release(_sbt_t840); });
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
        ({ TrStr _sbt_t841 = (_tr_strx_concat(_tr_strz(cs), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t841); _tr_str_release(_sbt_t841); });
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
            TrStr _strtmp_t843 = ({ TrStr _at_t842 = (_tr_strx_concat(_tr_strz(dn), _tr_strz(_tr_str_lit("_")))); __auto_type _wr = (_rename_word(result, dn, _at_t842)); _tr_str_release(_at_t842); _wr; });
            _tr_str_release(result);
            result = _strtmp_t843;
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
        TrStr _strtmp_t844 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(msg), _tr_strz(_tr_str_lit(" (")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_skipped)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" runtime/libc symbols skipped)"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(msg);
        msg = _strtmp_t844;
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

