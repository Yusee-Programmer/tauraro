#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Formatter* Formatter_init(List_i64* c_lines, List_TrStr* c_texts, List_bool* c_trailing) {
#line 32 "src/fmt.tr"
    Formatter* f = ((Formatter*)_tr_obj_alloc(sizeof(Formatter)));
#line 33 "src/fmt.tr"
    f->out = StringBuilder_init(4096LL);
#line 34 "src/fmt.tr"
    f->indent = 0LL;
#line 35 "src/fmt.tr"
    f->c_lines = c_lines;
#line 36 "src/fmt.tr"
    f->c_texts = c_texts;
#line 37 "src/fmt.tr"
    f->c_trailing = c_trailing;
#line 38 "src/fmt.tr"
    f->ci = 0LL;
#line 39 "src/fmt.tr"
    f->unsupported = false;
#line 40 "src/fmt.tr"
    f->in_fstring = false;
#line 41 "src/fmt.tr"
    return f;
}

__attribute__((hot)) TrStr Formatter_pad(Formatter* self) {
#line 45 "src/fmt.tr"
    StringBuilder* sb = StringBuilder_init(((self->indent * 4LL) + 1LL));
#line 46 "src/fmt.tr"
    long long i = 0LL;
#line 47 "src/fmt.tr"
    while ((i < (self->indent * 4LL))) {
#line 48 "src/fmt.tr"
        StringBuilder_append_char(sb, 32LL);
#line 49 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 50 "src/fmt.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void Formatter_line(Formatter* self, TrStr text) {
#line 53 "src/fmt.tr"
    ({ TrStr _ord_t2815 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2815); _tr_str_release(_ord_t2815); });
#line 54 "src/fmt.tr"
    StringBuilder_append(self->out, text);
#line 55 "src/fmt.tr"
    StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
}

__attribute__((hot)) void Formatter_flush_standalone_before(Formatter* self, long long line_no) {
#line 61 "src/fmt.tr"
    while ((self->ci < self->c_lines->len)) {
#line 62 "src/fmt.tr"
        if ((List_i64_get(self->c_lines, self->ci) >= line_no)) {
#line 62 "src/fmt.tr"
            break;
        }
#line 63 "src/fmt.tr"
        if (List_bool_get(self->c_trailing, self->ci)) {
#line 67 "src/fmt.tr"
            ({ TrStr _ord_t2816 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2816); _tr_str_release(_ord_t2816); });
#line 68 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("#", 1LL));
#line 69 "src/fmt.tr"
            ({ TrStr _ord_t2817 = (List_TrStr_get(self->c_texts, self->ci)); StringBuilder_append(self->out, _ord_t2817); _tr_str_release(_ord_t2817); });
#line 70 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
        } else {
#line 72 "src/fmt.tr"
            TrStr txt = List_TrStr_get(self->c_texts, self->ci);
#line 73 "src/fmt.tr"
            if ((_tr_str_lenv((txt)) == 0LL)) {
#line 74 "src/fmt.tr"
                StringBuilder_append(self->out, _tr_str_lit_len("#\n", 2LL));
            } else {
#line 76 "src/fmt.tr"
                ({ TrStr _ord_t2818 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2818); _tr_str_release(_ord_t2818); });
#line 77 "src/fmt.tr"
                StringBuilder_append(self->out, _tr_str_lit_len("#", 1LL));
#line 78 "src/fmt.tr"
                StringBuilder_append(self->out, txt);
#line 79 "src/fmt.tr"
                StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
            }
        }
#line 80 "src/fmt.tr"
        self->ci = (self->ci + 1LL);
    }
}

__attribute__((hot)) TrStr Formatter_trailing_for(Formatter* self, long long line_no) {
#line 85 "src/fmt.tr"
    if ((self->ci < self->c_lines->len)) {
#line 86 "src/fmt.tr"
        if ((List_bool_get(self->c_trailing, self->ci) && (List_i64_get(self->c_lines, self->ci) == line_no))) {
#line 87 "src/fmt.tr"
            TrStr txt = List_TrStr_get(self->c_texts, self->ci);
#line 88 "src/fmt.tr"
            self->ci = (self->ci + 1LL);
#line 89 "src/fmt.tr"
            TrStr _trret537 = _tr_strx_concatv((_tr_str_lit_len("  #", 3LL)), (txt));
            _tr_str_release(txt);
            return _trret537;
        }
    }
#line 90 "src/fmt.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void Formatter_flush_remaining(Formatter* self) {
#line 93 "src/fmt.tr"
    while ((self->ci < self->c_lines->len)) {
#line 94 "src/fmt.tr"
        TrStr txt = List_TrStr_get(self->c_texts, self->ci);
#line 95 "src/fmt.tr"
        if ((_tr_str_lenv((txt)) == 0LL)) {
#line 96 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("#\n", 2LL));
        } else {
#line 98 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("#", 1LL));
#line 99 "src/fmt.tr"
            StringBuilder_append(self->out, txt);
#line 100 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
        }
#line 101 "src/fmt.tr"
        self->ci = (self->ci + 1LL);
        _tr_str_release(txt);
    }
}

__attribute__((hot)) TrStr Formatter_escape_str(Formatter* self, TrStr s) {
#line 105 "src/fmt.tr"
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((s)) + 2LL));
#line 106 "src/fmt.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 107 "src/fmt.tr"
    long long i = 0LL;
#line 108 "src/fmt.tr"
    long long n = _tr_str_lenv((s));
#line 109 "src/fmt.tr"
    while ((i < n)) {
#line 110 "src/fmt.tr"
        long long c = 0LL;
#line 111 "src/fmt.tr"
        /* unsafe block */
#line 111 "src/fmt.tr"
        c = ((long long)((*(p + i))));
#line 112 "src/fmt.tr"
        if ((c == 92LL)) {
#line 112 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\\\", 2LL));
        } else if ((c == 34LL)) {
#line 113 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\\"", 2LL));
        } else if ((c == 10LL)) {
#line 114 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\n", 2LL));
        } else if ((c == 9LL)) {
#line 115 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\t", 2LL));
        } else if ((c == 13LL)) {
#line 116 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\r", 2LL));
        } else {
#line 117 "src/fmt.tr"
            StringBuilder_append_char(sb, c);
        }
#line 118 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 119 "src/fmt.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_escape_char(Formatter* self, long long v) {
#line 124 "src/fmt.tr"
    if ((v == 0LL)) {
#line 124 "src/fmt.tr"
        return _tr_str_lit_len("\\0", 2LL);
    }
#line 125 "src/fmt.tr"
    if ((v == 92LL)) {
#line 125 "src/fmt.tr"
        return _tr_str_lit_len("\\\\", 2LL);
    }
#line 126 "src/fmt.tr"
    if ((v == 39LL)) {
#line 126 "src/fmt.tr"
        return _tr_str_lit_len("\\'", 2LL);
    }
#line 127 "src/fmt.tr"
    if ((v == 10LL)) {
#line 127 "src/fmt.tr"
        return _tr_str_lit_len("\\n", 2LL);
    }
#line 128 "src/fmt.tr"
    if ((v == 9LL)) {
#line 128 "src/fmt.tr"
        return _tr_str_lit_len("\\t", 2LL);
    }
#line 129 "src/fmt.tr"
    if ((v == 13LL)) {
#line 129 "src/fmt.tr"
        return _tr_str_lit_len("\\r", 2LL);
    }
#line 130 "src/fmt.tr"
    StringBuilder* sb = StringBuilder_init(2LL);
#line 131 "src/fmt.tr"
    StringBuilder_append_char(sb, v);
#line 132 "src/fmt.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_escape_str_sq(Formatter* self, TrStr s) {
#line 136 "src/fmt.tr"
    StringBuilder* sb = StringBuilder_init((_tr_str_lenv((s)) + 2LL));
#line 137 "src/fmt.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 138 "src/fmt.tr"
    long long i = 0LL;
#line 139 "src/fmt.tr"
    long long n = _tr_str_lenv((s));
#line 140 "src/fmt.tr"
    while ((i < n)) {
#line 141 "src/fmt.tr"
        long long c = 0LL;
#line 142 "src/fmt.tr"
        /* unsafe block */
#line 142 "src/fmt.tr"
        c = ((long long)((*(p + i))));
#line 143 "src/fmt.tr"
        if ((c == 92LL)) {
#line 143 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\\\", 2LL));
        } else if ((c == 39LL)) {
#line 144 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\'", 2LL));
        } else if ((c == 10LL)) {
#line 145 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\n", 2LL));
        } else if ((c == 9LL)) {
#line 146 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\t", 2LL));
        } else if ((c == 13LL)) {
#line 147 "src/fmt.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\r", 2LL));
        } else {
#line 148 "src/fmt.tr"
            StringBuilder_append_char(sb, c);
        }
#line 149 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 150 "src/fmt.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_type_str(Formatter* self, AstType** t) {
#line 154 "src/fmt.tr"
    if ((((unsigned long long)(t)) == ((unsigned long long)(0LL)))) {
#line 154 "src/fmt.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 155 "src/fmt.tr"
    AstType* ty = (*t);
#line 157 "src/fmt.tr"
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("ref", 3LL))) && (ty->args->len == 1LL))) {
#line 158 "src/fmt.tr"
        TrStr _trret538 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("ref ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret538;
    }
#line 159 "src/fmt.tr"
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("mut_ref", 7LL))) && (ty->args->len == 1LL))) {
#line 160 "src/fmt.tr"
        TrStr _trret539 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("mut ref ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret539;
    }
#line 161 "src/fmt.tr"
    if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Tuple", 5LL)))) {
#line 162 "src/fmt.tr"
        TrStr ts = _tr_str_lit_len("(", 1LL);
#line 163 "src/fmt.tr"
        long long ti = 0LL;
#line 164 "src/fmt.tr"
        while ((ti < ty->args->len)) {
#line 165 "src/fmt.tr"
            if ((ti > 0LL)) {
#line 165 "src/fmt.tr"
                ({ TrStr _ap_t2819 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ts, _ap_t2819); });
            }
#line 166 "src/fmt.tr"
            ({ TrStr _ap_t2820 = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, ti)))); _tr_str_append(&ts, _ap_t2820); _tr_str_release(_ap_t2820); });
#line 167 "src/fmt.tr"
            ti = (ti + 1LL);
        }
#line 168 "src/fmt.tr"
        TrStr _trret540 = _tr_strx_concatv((ts), (_tr_str_lit_len(")", 1LL)));
        _tr_str_release(ts);
        return _trret540;
    }
#line 169 "src/fmt.tr"
    if (_tr_str_eqv((ty->name), (_tr_str_lit_len("def", 3LL)))) {
#line 171 "src/fmt.tr"
        TrStr ds = _tr_str_lit_len("def(", 4LL);
#line 172 "src/fmt.tr"
        long long di = 0LL;
#line 173 "src/fmt.tr"
        while ((di < (ty->args->len - 1LL))) {
#line 174 "src/fmt.tr"
            if ((di > 0LL)) {
#line 174 "src/fmt.tr"
                ({ TrStr _ap_t2821 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ds, _ap_t2821); });
            }
#line 175 "src/fmt.tr"
            ({ TrStr _ap_t2822 = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, di)))); _tr_str_append(&ds, _ap_t2822); _tr_str_release(_ap_t2822); });
#line 176 "src/fmt.tr"
            di = (di + 1LL);
        }
#line 177 "src/fmt.tr"
        ({ TrStr _ap_t2823 = (_tr_str_lit_len(")", 1LL)); _tr_str_append(&ds, _ap_t2823); });
#line 178 "src/fmt.tr"
        if ((ty->args->len > 0LL)) {
#line 179 "src/fmt.tr"
            TrStr rt = Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, (ty->args->len - 1LL))));
#line 180 "src/fmt.tr"
            if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 180 "src/fmt.tr"
                ({ TrStr _ap_t2824 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t2825 = (rt); _tr_str_append(&ds, _ap_t2824); _tr_str_append(&ds, _ap_t2825); });
            }
        }
#line 181 "src/fmt.tr"
        TrStr _trret541 = _tr_str_retain(ds);
        _tr_str_release(ds);
        return _trret541;
    }
#line 182 "src/fmt.tr"
    TrStr s = _tr_str_retain(ty->name);
#line 183 "src/fmt.tr"
    if ((ty->args->len > 0LL)) {
#line 184 "src/fmt.tr"
        TrStr _strtmp_t2827 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2827;
#line 185 "src/fmt.tr"
        long long i = 0LL;
#line 186 "src/fmt.tr"
        while ((i < ty->args->len)) {
#line 187 "src/fmt.tr"
            if ((i > 0LL)) {
#line 187 "src/fmt.tr"
                TrStr _strtmp_t2829 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2829;
            }
#line 188 "src/fmt.tr"
            TrStr _strtmp_t2831 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2831;
#line 189 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 190 "src/fmt.tr"
        TrStr _strtmp_t2833 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2833;
    }
#line 191 "src/fmt.tr"
    if ((_tr_str_lenv((ty->from_param)) > 0LL)) {
#line 192 "src/fmt.tr"
        TrStr _strtmp_t2836 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" from ", 6LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->from_param)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2836;
    }
#line 193 "src/fmt.tr"
    TrStr _trret542 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret542;
}

__attribute__((hot)) bool Formatter_needs_parens(Formatter* self, Expr* e) {
#line 199 "src/fmt.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 199 "src/fmt.tr"
        return false;
    }
#line 200 "src/fmt.tr"
    __auto_type _t2837 = (*e);
    if (_t2837.tag == Expr_EBinOp) {
        return true;
    } else if (_t2837.tag == Expr_EUnaryOp) {
        return true;
    } else if (_t2837.tag == Expr_EIfElse) {
        return true;
    } else if (_t2837.tag == Expr_ETuple) {
        __auto_type _ = _t2837.data.ETuple.items;
        return true;
    } else if (_t2837.tag == Expr_ERange) {
        return true;
    } else if (_t2837.tag == Expr_ECast) {
        return true;
    } else if (1) {
        __auto_type _ = _t2837;
        return false;
    }
}

__attribute__((hot)) TrStr Formatter_operand_str(Formatter* self, Expr* e) {
#line 210 "src/fmt.tr"
    if (Formatter_needs_parens(self, e)) {
#line 211 "src/fmt.tr"
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    }
#line 212 "src/fmt.tr"
    return Formatter_expr_str(self, e);
}

__attribute__((hot)) TrStr Formatter_args_str(Formatter* self, List_ptr* args) {
#line 215 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 216 "src/fmt.tr"
    long long i = 0LL;
#line 217 "src/fmt.tr"
    while ((i < args->len)) {
#line 218 "src/fmt.tr"
        if ((i > 0LL)) {
#line 218 "src/fmt.tr"
            TrStr _strtmp_t2839 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2839;
        }
#line 219 "src/fmt.tr"
        TrStr _strtmp_t2841 = ({ TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(args, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2841;
#line 220 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 221 "src/fmt.tr"
    TrStr _trret543 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret543;
}

__attribute__((hot)) TrStr Formatter_expr_str(Formatter* self, Expr* e) {
#line 224 "src/fmt.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 224 "src/fmt.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 225 "src/fmt.tr"
    __auto_type _t2842 = (*e);
    if (_t2842.tag == Expr_ELitInt) {
        __auto_type v = _t2842.data.ELitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t2842.tag == Expr_ELitFloat) {
        __auto_type v = _t2842.data.ELitFloat.val;
        return _tr_str_wrap(_tr_float_to_str((double)(v)));
    } else if (_t2842.tag == Expr_ELitStr) {
        __auto_type v = _t2842.data.ELitStr.val;
#line 232 "src/fmt.tr"
        if (self->in_fstring) {
#line 233 "src/fmt.tr"
            TrStr _trret544 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str_sq(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            return _trret544;
        }
#line 234 "src/fmt.tr"
        TrStr _trret545 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("\"", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret545;
    } else if (_t2842.tag == Expr_ELitBool) {
        __auto_type b = _t2842.data.ELitBool.val;
#line 236 "src/fmt.tr"
        if (b) {
#line 236 "src/fmt.tr"
            return _tr_str_lit_len("True", 4LL);
        }
#line 237 "src/fmt.tr"
        return _tr_str_lit_len("False", 5LL);
    } else if (_t2842.tag == Expr_ELitChar) {
        __auto_type v = _t2842.data.ELitChar.val;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_char(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_ELitBytes) {
        __auto_type v = _t2842.data.ELitBytes.val;
        TrStr _trret546 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("b\"", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret546;
    } else if (_t2842.tag == Expr_ERawStr) {
        __auto_type v = _t2842.data.ERawStr.val;
        TrStr _trret547 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("r\"", 2LL)), (v))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret547;
    } else if (_t2842.tag == Expr_ELitNone) {
        return _tr_str_lit_len("None", 4LL);
    } else if (_t2842.tag == Expr_EIdent) {
        __auto_type nm = _t2842.data.EIdent.name;
        TrStr _trret548 = _tr_str_retain(nm);
        return _trret548;
    } else if (_t2842.tag == Expr_EBinOp) {
        __auto_type op = _t2842.data.EBinOp.op;
__auto_type l = _t2842.data.EBinOp.left;
__auto_type r = _t2842.data.EBinOp.right;
#line 244 "src/fmt.tr"
        TrStr _trret549 = ({ __auto_type _ord_t2843 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, l)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (op)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2843); TrStr _tr_cr = (Formatter_operand_str(self, r)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        return _trret549;
    } else if (_t2842.tag == Expr_EUnaryOp) {
        __auto_type op = _t2842.data.EUnaryOp.op;
__auto_type x = _t2842.data.EUnaryOp.expr;
#line 246 "src/fmt.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL)))) {
#line 246 "src/fmt.tr"
            TrStr _trret550 = ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("not ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            return _trret550;
        }
#line 247 "src/fmt.tr"
        TrStr _trret551 = ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((op), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret551;
    } else if (_t2842.tag == Expr_ECall) {
        __auto_type callee = _t2842.data.ECall.callee;
__auto_type args = _t2842.data.ECall.args;
#line 249 "src/fmt.tr"
        return ({ __auto_type _ord_t2844 = (({ TrStr _tr_cl = (Formatter_expr_str(self, callee)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2844); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
    } else if (_t2842.tag == Expr_EMethodCall) {
        __auto_type obj = _t2842.data.EMethodCall.obj;
__auto_type m = _t2842.data.EMethodCall.method;
__auto_type args = _t2842.data.EMethodCall.args;
#line 251 "src/fmt.tr"
        TrStr _trret552 = ({ __auto_type _ord_t2845 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2845); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        return _trret552;
    } else if (_t2842.tag == Expr_EPropAccess) {
        __auto_type obj = _t2842.data.EPropAccess.obj;
__auto_type p = _t2842.data.EPropAccess.prop;
#line 253 "src/fmt.tr"
        TrStr _trret553 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret553;
    } else if (_t2842.tag == Expr_EIndex) {
        __auto_type obj = _t2842.data.EIndex.obj;
__auto_type idx = _t2842.data.EIndex._tr_v_index;
#line 255 "src/fmt.tr"
        return ({ __auto_type _ord_t2846 = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2846); TrStr _tr_cr = (Formatter_expr_str(self, idx)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
    } else if (_t2842.tag == Expr_ECast) {
        __auto_type x = _t2842.data.ECast.expr;
__auto_type ty = _t2842.data.ECast.ty;
#line 257 "src/fmt.tr"
        return ({ __auto_type _ord_t2847 = (({ TrStr _tr_cl = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" as ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2847); TrStr _tr_cr = (Formatter_type_str(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t2842.tag == Expr_EFString) {
        __auto_type parts = _t2842.data.EFString.parts;
        return Formatter_fstring_str(self, parts);
    } else if (_t2842.tag == Expr_ETryExpr) {
        __auto_type x = _t2842.data.ETryExpr.expr;
        return ({ TrStr _tr_cl = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("?", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_EClosure) {
        __auto_type params = _t2842.data.EClosure.params;
__auto_type ret_ty = _t2842.data.EClosure.ret_ty;
__auto_type body = _t2842.data.EClosure.body;
__auto_type is_async = _t2842.data.EClosure.is_async;
#line 261 "src/fmt.tr"
        TrStr _trret554 = Formatter_closure_str(self, params, ret_ty, body, is_async);
        return _trret554;
    } else if (_t2842.tag == Expr_ESuperMethodCall) {
        __auto_type base = _t2842.data.ESuperMethodCall.base_class;
__auto_type m = _t2842.data.ESuperMethodCall.method;
__auto_type args = _t2842.data.ESuperMethodCall.args;
#line 263 "src/fmt.tr"
        TrStr _trret555 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret555;
    } else if (_t2842.tag == Expr_ESuperPropAccess) {
        __auto_type base = _t2842.data.ESuperPropAccess.base_class;
__auto_type p = _t2842.data.ESuperPropAccess.prop;
#line 265 "src/fmt.tr"
        TrStr _trret556 = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), (p));
        return _trret556;
    } else if (_t2842.tag == Expr_EList) {
        __auto_type items = _t2842.data.EList.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_ESet) {
        __auto_type items = _t2842.data.ESet.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("{", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("}", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_EDict) {
        __auto_type keys = _t2842.data.EDict.keys;
__auto_type vals = _t2842.data.EDict.vals;
        return Formatter_dict_str(self, keys, vals);
    } else if (_t2842.tag == Expr_ETuple) {
        __auto_type items = _t2842.data.ETuple.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_EListComp) {
        __auto_type elem = _t2842.data.EListComp.element;
__auto_type gens = _t2842.data.EListComp.generators;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_comp_str(self, elem, gens)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_EGeneratorExpr) {
        __auto_type elem = _t2842.data.EGeneratorExpr.element;
__auto_type gens = _t2842.data.EGeneratorExpr.generators;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_comp_str(self, elem, gens)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_ESlice) {
        __auto_type a = _t2842.data.ESlice.start;
__auto_type b = _t2842.data.ESlice.stop;
__auto_type c = _t2842.data.ESlice.step;
        return Formatter_slice_str(self, a, b, c);
    } else if (_t2842.tag == Expr_EAwait) {
        __auto_type x = _t2842.data.EAwait.expr;
        return ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("await ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t2842.tag == Expr_EYield) {
        __auto_type x = _t2842.data.EYield.expr;
#line 275 "src/fmt.tr"
        if ((((unsigned long long)(x)) == ((unsigned long long)(0LL)))) {
#line 275 "src/fmt.tr"
            return _tr_str_lit_len("yield", 5LL);
        }
#line 276 "src/fmt.tr"
        return ({ TrStr _tr_cr = (Formatter_expr_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("yield ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t2842.tag == Expr_ERange) {
        __auto_type a = _t2842.data.ERange.start;
__auto_type b = _t2842.data.ERange.end;
__auto_type inclusive = _t2842.data.ERange.inclusive;
#line 278 "src/fmt.tr"
        TrStr sep = _tr_str_lit_len("..", 2LL);
#line 279 "src/fmt.tr"
        if (inclusive) {
#line 279 "src/fmt.tr"
            TrStr _strtmp_t2848 = _tr_str_lit_len("..=", 3LL);
            _tr_str_release(sep);
            sep = _strtmp_t2848;
        }
#line 280 "src/fmt.tr"
        TrStr _trret557 = ({ __auto_type _ord_t2849 = (({ TrStr _tr_cl = (Formatter_operand_str(self, a)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sep)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2849); TrStr _tr_cr = (Formatter_operand_str(self, b)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(sep);
        return _trret557;
    } else if (_t2842.tag == Expr_ESizeOf) {
        __auto_type ty = _t2842.data.ESizeOf.ty;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_type_str(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("sizeof(", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2842.tag == Expr_EIfElse) {
        __auto_type c = _t2842.data.EIfElse.cond;
__auto_type t = _t2842.data.EIfElse.then_expr;
__auto_type f = _t2842.data.EIfElse.else_expr;
#line 283 "src/fmt.tr"
        return ({ __auto_type _ord_t2850 = (({ TrStr _tr_cl = (Formatter_operand_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" if ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2851 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2850); TrStr _tr_cr = (Formatter_operand_str(self, c)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" else ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2851); TrStr _tr_cr = (Formatter_operand_str(self, f)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t2842.tag == Expr_ETypeArg) {
        __auto_type ty = _t2842.data.ETypeArg.ty;
        return Formatter_type_str(self, ty);
    } else if (1) {
        __auto_type _ = _t2842;
#line 286 "src/fmt.tr"
        self->unsupported = true;
#line 287 "src/fmt.tr"
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) TrStr Formatter_fstring_str(Formatter* self, List_ptr* parts) {
#line 290 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("f\"", 2LL);
#line 291 "src/fmt.tr"
    long long i = 0LL;
#line 292 "src/fmt.tr"
    while ((i < parts->len)) {
#line 293 "src/fmt.tr"
        FStringPart* part = ((FStringPart*)List_ptr_get(parts, i));
#line 294 "src/fmt.tr"
        if (part->is_expr) {
#line 295 "src/fmt.tr"
            bool saved = self->in_fstring;
#line 296 "src/fmt.tr"
            self->in_fstring = true;
#line 297 "src/fmt.tr"
            TrStr es = Formatter_expr_str(self, part->expr);
#line 298 "src/fmt.tr"
            self->in_fstring = saved;
#line 299 "src/fmt.tr"
            TrStr _strtmp_t2854 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("{", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (es)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2854;
#line 300 "src/fmt.tr"
            if ((_tr_str_lenv((part->fmt_spec)) > 0LL)) {
#line 301 "src/fmt.tr"
                TrStr _strtmp_t2857 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (part->fmt_spec)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t2857;
            }
#line 302 "src/fmt.tr"
            TrStr _strtmp_t2859 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t2859;
            _tr_str_release(es);
        } else {
#line 304 "src/fmt.tr"
            TrStr _strtmp_t2863 = ({ TrStr _apt_t2862 = _tr_str_retain(part->text); __auto_type _wr = (({ TrStr _tr_cr = (Formatter_escape_str(self, _apt_t2862)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); _tr_str_release(_apt_t2862); _wr; });
            _tr_str_release(s);
            s = _strtmp_t2863;
        }
#line 305 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 306 "src/fmt.tr"
    TrStr _trret558 = _tr_strx_concatv((s), (_tr_str_lit_len("\"", 1LL)));
    _tr_str_release(s);
    return _trret558;
}

__attribute__((hot)) TrStr Formatter_dict_str(Formatter* self, List_ptr* keys, List_ptr* vals) {
#line 309 "src/fmt.tr"
    if ((keys->len == 0LL)) {
#line 309 "src/fmt.tr"
        return _tr_str_lit_len("{}", 2LL);
    }
#line 310 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("{", 1LL);
#line 311 "src/fmt.tr"
    long long i = 0LL;
#line 312 "src/fmt.tr"
    while ((i < keys->len)) {
#line 313 "src/fmt.tr"
        if ((i > 0LL)) {
#line 313 "src/fmt.tr"
            TrStr _strtmp_t2865 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2865;
        }
#line 314 "src/fmt.tr"
        TrStr _strtmp_t2870 = ({ __auto_type _ord_t2869 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(keys, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2869); TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(vals, i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(s);
        s = _strtmp_t2870;
#line 315 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 316 "src/fmt.tr"
    TrStr _trret559 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
    _tr_str_release(s);
    return _trret559;
}

__attribute__((hot)) TrStr Formatter_comp_str(Formatter* self, Expr* elem, List_ptr* gens) {
#line 319 "src/fmt.tr"
    TrStr s = Formatter_expr_str(self, elem);
#line 320 "src/fmt.tr"
    long long i = 0LL;
#line 321 "src/fmt.tr"
    while ((i < gens->len)) {
#line 322 "src/fmt.tr"
        Comprehension* g = (*((Comprehension**)List_ptr_get(gens, i)));
#line 323 "src/fmt.tr"
        TrStr _strtmp_t2876 = ({ __auto_type _ord_t2875 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" for ", 5LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2875); TrStr _tr_cr = (Formatter_expr_str(self, g->iter)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(s);
        s = _strtmp_t2876;
#line 324 "src/fmt.tr"
        long long j = 0LL;
#line 325 "src/fmt.tr"
        while ((j < g->ifs->len)) {
#line 326 "src/fmt.tr"
            TrStr _strtmp_t2879 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" if ", 4LL)))); TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(g->ifs, j)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2879;
#line 327 "src/fmt.tr"
            j = (j + 1LL);
        }
#line 328 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 329 "src/fmt.tr"
    TrStr _trret560 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret560;
}

__attribute__((hot)) TrStr Formatter_slice_str(Formatter* self, Expr* a, Expr* b, Expr* c) {
#line 332 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 333 "src/fmt.tr"
    if ((((unsigned long long)(a)) != ((unsigned long long)(0LL)))) {
#line 333 "src/fmt.tr"
        TrStr _strtmp_t2881 = ({ TrStr _tr_cr = (Formatter_expr_str(self, a)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2881;
    }
#line 334 "src/fmt.tr"
    TrStr _strtmp_t2883 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t2883;
#line 335 "src/fmt.tr"
    if ((((unsigned long long)(b)) != ((unsigned long long)(0LL)))) {
#line 335 "src/fmt.tr"
        TrStr _strtmp_t2885 = ({ TrStr _tr_cr = (Formatter_expr_str(self, b)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2885;
    }
#line 336 "src/fmt.tr"
    if ((((unsigned long long)(c)) != ((unsigned long long)(0LL)))) {
#line 336 "src/fmt.tr"
        TrStr _strtmp_t2888 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2888;
    }
#line 337 "src/fmt.tr"
    TrStr _trret561 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret561;
}

__attribute__((hot)) TrStr Formatter_closure_str(Formatter* self, List_ptr* params, AstType** ret_ty, Block* body, bool is_async) {
#line 340 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 341 "src/fmt.tr"
    if (is_async) {
#line 341 "src/fmt.tr"
        TrStr _strtmp_t2889 = _tr_str_lit_len("async ", 6LL);
        _tr_str_release(s);
        s = _strtmp_t2889;
    }
#line 342 "src/fmt.tr"
    TrStr _strtmp_t2893 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("def (", 5LL)))); TrStr _tr_cr = (Formatter_params_str(self, params)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2893;
#line 343 "src/fmt.tr"
    if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
#line 344 "src/fmt.tr"
        TrStr rt = Formatter_type_str(self, ret_ty);
#line 345 "src/fmt.tr"
        if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 345 "src/fmt.tr"
            TrStr _strtmp_t2896 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2896;
        }
    }
#line 346 "src/fmt.tr"
    TrStr _strtmp_t2898 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t2898;
#line 351 "src/fmt.tr"
    Stmt* real = (Stmt*)(0LL);
#line 352 "src/fmt.tr"
    long long _tr_v_count = 0LL;
#line 353 "src/fmt.tr"
    long long i = 0LL;
#line 354 "src/fmt.tr"
    while ((i < body->stmts->len)) {
#line 355 "src/fmt.tr"
        __auto_type _t2899 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t2899.tag == Stmt_SLine) {
            __auto_type _ = _t2899.data.SLine.n;
            /* pass */
        } else if (1) {
            __auto_type _ = _t2899;
#line 358 "src/fmt.tr"
            _tr_v_count = (_tr_v_count + 1LL);
#line 359 "src/fmt.tr"
            real = ((Stmt*)List_ptr_get(body->stmts, i));
        }
#line 360 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 361 "src/fmt.tr"
    if ((_tr_v_count == 1LL)) {
#line 362 "src/fmt.tr"
        __auto_type _t2900 = (*real);
        if (_t2900.tag == Stmt_SReturn) {
            __auto_type v = _t2900.data.SReturn.val;
#line 364 "src/fmt.tr"
            if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 364 "src/fmt.tr"
                TrStr _trret562 = _tr_strx_concatv((s), (_tr_str_lit_len(" return", 7LL)));
                _tr_str_release(s);
                return _trret562;
            }
#line 365 "src/fmt.tr"
            TrStr _trret563 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" return ", 8LL)))); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            return _trret563;
        } else if (_t2900.tag == Stmt_SExpr) {
            __auto_type e = _t2900.data.SExpr.expr;
            TrStr _trret564 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            return _trret564;
        } else if (1) {
            __auto_type _ = _t2900;
#line 368 "src/fmt.tr"
            self->unsupported = true;
#line 369 "src/fmt.tr"
            TrStr _trret565 = _tr_str_retain(s);
            _tr_str_release(s);
            return _trret565;
        }
    }
#line 370 "src/fmt.tr"
    self->unsupported = true;
#line 371 "src/fmt.tr"
    TrStr _trret566 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret566;
}

__attribute__((hot)) TrStr Formatter_params_str(Formatter* self, List_ptr* params) {
#line 375 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 376 "src/fmt.tr"
    long long i = 0LL;
#line 377 "src/fmt.tr"
    while ((i < params->len)) {
#line 378 "src/fmt.tr"
        if ((i > 0LL)) {
#line 378 "src/fmt.tr"
            TrStr _strtmp_t2902 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2902;
        }
#line 379 "src/fmt.tr"
        Param* p = ((Param*)List_ptr_get(params, i));
#line 383 "src/fmt.tr"
        ({ TrStr _ap_t2903 = (p->name); _tr_str_append(&s, _ap_t2903); });
#line 384 "src/fmt.tr"
        TrStr tn = Formatter_type_str(self, p->ty);
#line 385 "src/fmt.tr"
        if (((_tr_str_lenv((tn)) > 0LL) && (!_tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL)))))) {
#line 386 "src/fmt.tr"
            TrStr _strtmp_t2906 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tn)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2906;
        }
#line 387 "src/fmt.tr"
        if (p->is_variadic) {
#line 387 "src/fmt.tr"
            TrStr _strtmp_t2908 = _tr_strx_concatv((s), (_tr_str_lit_len("...", 3LL)));
            _tr_str_release(s);
            s = _strtmp_t2908;
        }
#line 388 "src/fmt.tr"
        i = (i + 1LL);
        _tr_str_release(tn);
    }
#line 389 "src/fmt.tr"
    TrStr _trret567 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret567;
}

__attribute__((hot)) void Formatter_emit_decorators(Formatter* self, List_ptr* decs) {
#line 392 "src/fmt.tr"
    long long i = 0LL;
#line 393 "src/fmt.tr"
    while ((i < decs->len)) {
#line 394 "src/fmt.tr"
        Decorator* d = ((Decorator*)List_ptr_get(decs, i));
#line 395 "src/fmt.tr"
        if ((d->args->len > 0LL)) {
#line 396 "src/fmt.tr"
            ({ __auto_type _ord_t2909 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (d->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2910 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2909); TrStr _tr_cr = (Formatter_args_str(self, d->args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2910); _tr_str_release(_at_t2910); });
        } else {
#line 398 "src/fmt.tr"
            ({ TrStr _at_t2911 = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (d->name))); Formatter_line(self, _at_t2911); _tr_str_release(_at_t2911); });
        }
#line 399 "src/fmt.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Formatter_emit_block(Formatter* self, Block* b) {
#line 404 "src/fmt.tr"
    long long cur_line = 0LL;
#line 405 "src/fmt.tr"
    long long i = 0LL;
#line 406 "src/fmt.tr"
    while ((i < b->stmts->len)) {
#line 407 "src/fmt.tr"
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
#line 408 "src/fmt.tr"
        __auto_type _t2912 = (*sp);
        if (_t2912.tag == Stmt_SLine) {
            __auto_type n = _t2912.data.SLine.n;
#line 410 "src/fmt.tr"
            Formatter_flush_standalone_before(self, n);
#line 411 "src/fmt.tr"
            cur_line = n;
        } else if (1) {
            __auto_type _ = _t2912;
#line 413 "src/fmt.tr"
            Formatter_emit_stmt(self, sp, cur_line);
        }
#line 414 "src/fmt.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Formatter_emit_simple(Formatter* self, TrStr text, long long src_line) {
#line 418 "src/fmt.tr"
    ({ TrStr _ord_t2913 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2913); _tr_str_release(_ord_t2913); });
#line 419 "src/fmt.tr"
    StringBuilder_append(self->out, text);
#line 420 "src/fmt.tr"
    ({ TrStr _ord_t2914 = (Formatter_trailing_for(self, src_line)); StringBuilder_append(self->out, _ord_t2914); _tr_str_release(_ord_t2914); });
#line 421 "src/fmt.tr"
    StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
}

__attribute__((hot)) void Formatter_emit_stmt(Formatter* self, Stmt* sp, long long src_line) {
#line 424 "src/fmt.tr"
    __auto_type _t2915 = (*sp);
    if (_t2915.tag == Stmt_SExpr) {
        __auto_type e = _t2915.data.SExpr.expr;
        ({ TrStr _at_t2916 = (Formatter_expr_str(self, e)); Formatter_emit_simple(self, _at_t2916, src_line); _tr_str_release(_at_t2916); });
    } else if (_t2915.tag == Stmt_SLet) {
        __auto_type name = _t2915.data.SLet.name;
__auto_type own = _t2915.data.SLet.ownership;
__auto_type is_mut = _t2915.data.SLet.is_mut;
__auto_type is_const = _t2915.data.SLet.is_const;
__auto_type is_shared = _t2915.data.SLet.is_shared;
__auto_type ty = _t2915.data.SLet.ty;
__auto_type val = _t2915.data.SLet.val;
#line 427 "src/fmt.tr"
        ({ TrStr _at_t2917 = (Formatter_let_str(self, name, own, is_mut, is_const, is_shared, ty, val)); Formatter_emit_simple(self, _at_t2917, src_line); _tr_str_release(_at_t2917); });
    } else if (_t2915.tag == Stmt_SMultiLet) {
        __auto_type names = _t2915.data.SMultiLet.names;
__auto_type is_mut = _t2915.data.SMultiLet.is_mut;
__auto_type val = _t2915.data.SMultiLet.val;
#line 429 "src/fmt.tr"
        TrStr pfx = _tr_str_lit_len("", 0LL);
#line 430 "src/fmt.tr"
        if (is_mut) {
#line 430 "src/fmt.tr"
            TrStr _strtmp_t2918 = _tr_str_lit_len("mut ", 4LL);
            _tr_str_release(pfx);
            pfx = _strtmp_t2918;
        }
#line 431 "src/fmt.tr"
        TrStr ns = _tr_str_lit_len("", 0LL);
#line 432 "src/fmt.tr"
        long long k = 0LL;
#line 433 "src/fmt.tr"
        while ((k < names->len)) {
#line 434 "src/fmt.tr"
            if ((k > 0LL)) {
#line 434 "src/fmt.tr"
                ({ TrStr _ap_t2919 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ns, _ap_t2919); });
            }
#line 435 "src/fmt.tr"
            ({ TrStr _ap_t2920 = (List_TrStr_get(names, k)); _tr_str_append(&ns, _ap_t2920); _tr_str_release(_ap_t2920); });
#line 436 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 437 "src/fmt.tr"
        ({ TrStr _at_t2921 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((pfx), (ns))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, val)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2921, src_line); _tr_str_release(_at_t2921); });
        _tr_str_release(pfx);
        _tr_str_release(ns);
    } else if (_t2915.tag == Stmt_SAssign) {
        __auto_type t = _t2915.data.SAssign.target;
__auto_type v = _t2915.data.SAssign.val;
#line 439 "src/fmt.tr"
        ({ __auto_type _ord_t2922 = (({ TrStr _tr_cl = (Formatter_expr_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2923 = (({ TrStr _tr_cl = (_ord_t2922); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2923, src_line); _tr_str_release(_at_t2923); });
    } else if (_t2915.tag == Stmt_SReturn) {
        __auto_type v = _t2915.data.SReturn.val;
#line 441 "src/fmt.tr"
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 441 "src/fmt.tr"
            Formatter_emit_simple(self, _tr_str_lit_len("return", 6LL), src_line);
        } else {
#line 442 "src/fmt.tr"
            ({ TrStr _at_t2924 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2924, src_line); _tr_str_release(_at_t2924); });
        }
    } else if (_t2915.tag == Stmt_SBreak) {
        __auto_type bv = _t2915.data.SBreak.val;
#line 444 "src/fmt.tr"
        if ((((unsigned long long)(bv)) == ((unsigned long long)(0LL)))) {
#line 444 "src/fmt.tr"
            Formatter_emit_simple(self, _tr_str_lit_len("break", 5LL), src_line);
        } else {
#line 445 "src/fmt.tr"
            ({ TrStr _at_t2925 = (({ TrStr _tr_cr = (Formatter_expr_str(self, bv)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("break ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2925, src_line); _tr_str_release(_at_t2925); });
        }
    } else if (_t2915.tag == Stmt_SContinue) {
        Formatter_emit_simple(self, _tr_str_lit_len("continue", 8LL), src_line);
    } else if (_t2915.tag == Stmt_SPass) {
        Formatter_emit_simple(self, _tr_str_lit_len("pass", 4LL), src_line);
    } else if (_t2915.tag == Stmt_SRaise) {
        __auto_type v = _t2915.data.SRaise.val;
        ({ TrStr _at_t2926 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("raise ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2926, src_line); _tr_str_release(_at_t2926); });
    } else if (_t2915.tag == Stmt_SAssert) {
        __auto_type c = _t2915.data.SAssert.cond;
__auto_type m = _t2915.data.SAssert.msg;
#line 450 "src/fmt.tr"
        if ((((unsigned long long)(m)) == ((unsigned long long)(0LL)))) {
#line 450 "src/fmt.tr"
            ({ TrStr _at_t2927 = (({ TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("assert ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2927, src_line); _tr_str_release(_at_t2927); });
        } else {
#line 451 "src/fmt.tr"
            ({ __auto_type _ord_t2928 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("assert ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2929 = (({ TrStr _tr_cl = (_ord_t2928); TrStr _tr_cr = (Formatter_expr_str(self, m)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2929, src_line); _tr_str_release(_at_t2929); });
        }
    } else if (_t2915.tag == Stmt_SUnsafe) {
        __auto_type body = _t2915.data.SUnsafe.body;
#line 453 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("unsafe:", 7LL), src_line);
#line 454 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 455 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 456 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_SIf) {
        __auto_type cond = _t2915.data.SIf.cond;
__auto_type then_b = _t2915.data.SIf.then_b;
__auto_type elifs = _t2915.data.SIf.elifs;
__auto_type else_b = _t2915.data.SIf.else_b;
#line 458 "src/fmt.tr"
        ({ TrStr _at_t2930 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("if ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2930, src_line); _tr_str_release(_at_t2930); });
#line 459 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 460 "src/fmt.tr"
        Formatter_emit_block(self, then_b);
#line 461 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
#line 462 "src/fmt.tr"
        long long ei = 0LL;
#line 463 "src/fmt.tr"
        while ((ei < elifs->len)) {
#line 464 "src/fmt.tr"
            ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, ei));
#line 465 "src/fmt.tr"
            ({ TrStr _at_t2931 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, ec->cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("elif ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2931); _tr_str_release(_at_t2931); });
#line 466 "src/fmt.tr"
            self->indent = (self->indent + 1LL);
#line 467 "src/fmt.tr"
            Formatter_emit_block(self, (*ec->body));
#line 468 "src/fmt.tr"
            self->indent = (self->indent - 1LL);
#line 469 "src/fmt.tr"
            ei = (ei + 1LL);
        }
#line 470 "src/fmt.tr"
        if ((else_b->stmts->len > 0LL)) {
#line 471 "src/fmt.tr"
            Formatter_line(self, _tr_str_lit_len("else:", 5LL));
#line 472 "src/fmt.tr"
            self->indent = (self->indent + 1LL);
#line 473 "src/fmt.tr"
            Formatter_emit_block(self, else_b);
#line 474 "src/fmt.tr"
            self->indent = (self->indent - 1LL);
        }
    } else if (_t2915.tag == Stmt_SWhile) {
        __auto_type cond = _t2915.data.SWhile.cond;
__auto_type body = _t2915.data.SWhile.body;
__auto_type decs = _t2915.data.SWhile.decorators;
#line 476 "src/fmt.tr"
        ({ TrStr _at_t2932 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("while ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2932, src_line); _tr_str_release(_at_t2932); });
#line 477 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 478 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 479 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_SFor) {
        __auto_type var = _t2915.data.SFor.var;
__auto_type it = _t2915.data.SFor.iter;
__auto_type body = _t2915.data.SFor.body;
__auto_type decs = _t2915.data.SFor.decorators;
#line 481 "src/fmt.tr"
        ({ TrStr _at_t2933 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("for ", 4LL)), (var))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, it)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2933, src_line); _tr_str_release(_at_t2933); });
#line 482 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 483 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 484 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_SForUnpack) {
        __auto_type vars = _t2915.data.SForUnpack.vars;
__auto_type it = _t2915.data.SForUnpack.iter;
__auto_type body = _t2915.data.SForUnpack.body;
#line 486 "src/fmt.tr"
        TrStr vs = _tr_str_lit_len("", 0LL);
#line 487 "src/fmt.tr"
        long long k = 0LL;
#line 488 "src/fmt.tr"
        while ((k < vars->len)) {
#line 489 "src/fmt.tr"
            if ((k > 0LL)) {
#line 489 "src/fmt.tr"
                ({ TrStr _ap_t2934 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&vs, _ap_t2934); });
            }
#line 490 "src/fmt.tr"
            ({ TrStr _ap_t2935 = (List_TrStr_get(vars, k)); _tr_str_append(&vs, _ap_t2935); _tr_str_release(_ap_t2935); });
#line 491 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 492 "src/fmt.tr"
        ({ TrStr _at_t2936 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("for ", 4LL)), (vs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, it)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2936, src_line); _tr_str_release(_at_t2936); });
#line 493 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 494 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 495 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
        _tr_str_release(vs);
    } else if (_t2915.tag == Stmt_SMatch) {
        __auto_type e = _t2915.data.SMatch.expr;
__auto_type arms = _t2915.data.SMatch.arms;
#line 497 "src/fmt.tr"
        ({ TrStr _at_t2937 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("match ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2937, src_line); _tr_str_release(_at_t2937); });
#line 498 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 499 "src/fmt.tr"
        long long ai = 0LL;
#line 500 "src/fmt.tr"
        while ((ai < arms->len)) {
#line 501 "src/fmt.tr"
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, ai));
#line 502 "src/fmt.tr"
            TrStr head = ({ TrStr _tr_cr = (Formatter_pattern_str(self, arm->pat)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("case ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 503 "src/fmt.tr"
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
#line 504 "src/fmt.tr"
                ({ TrStr _ap_t2938 = (_tr_str_lit_len(" if ", 4LL)); TrStr _ap_t2939 = (Formatter_expr_str(self, arm->guard)); _tr_str_append(&head, _ap_t2938); _tr_str_append(&head, _ap_t2939); _tr_str_release(_ap_t2939); });
            }
#line 505 "src/fmt.tr"
            ({ TrStr _at_t2940 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2940); _tr_str_release(_at_t2940); });
#line 506 "src/fmt.tr"
            self->indent = (self->indent + 1LL);
#line 507 "src/fmt.tr"
            Formatter_emit_block(self, (*arm->body));
#line 508 "src/fmt.tr"
            self->indent = (self->indent - 1LL);
#line 509 "src/fmt.tr"
            ai = (ai + 1LL);
            _tr_str_release(head);
        }
#line 510 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_STry) {
        __auto_type try_b = _t2915.data.STry.try_body;
__auto_type catches = _t2915.data.STry.catches;
__auto_type finally_b = _t2915.data.STry.finally_b;
#line 512 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("try:", 4LL), src_line);
#line 513 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 514 "src/fmt.tr"
        Formatter_emit_block(self, try_b);
#line 515 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
#line 516 "src/fmt.tr"
        long long ci2 = 0LL;
#line 517 "src/fmt.tr"
        while ((ci2 < catches->len)) {
#line 518 "src/fmt.tr"
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, ci2)));
#line 519 "src/fmt.tr"
            TrStr head = _tr_str_lit_len("except", 6LL);
#line 520 "src/fmt.tr"
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
#line 521 "src/fmt.tr"
                TrStr et = Formatter_type_str(self, cc->err_type);
#line 522 "src/fmt.tr"
                if (((_tr_str_lenv((et)) > 0LL) && (!_tr_str_eqv((et), (_tr_str_lit_len("void", 4LL)))))) {
#line 523 "src/fmt.tr"
                    ({ TrStr _ap_t2941 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t2942 = (et); _tr_str_append(&head, _ap_t2941); _tr_str_append(&head, _ap_t2942); });
                }
            }
#line 524 "src/fmt.tr"
            if ((_tr_str_lenv((cc->err_name)) > 0LL)) {
#line 525 "src/fmt.tr"
                ({ TrStr _ap_t2943 = (_tr_str_lit_len(" as ", 4LL)); TrStr _ap_t2944 = (cc->err_name); _tr_str_append(&head, _ap_t2943); _tr_str_append(&head, _ap_t2944); });
            }
#line 526 "src/fmt.tr"
            ({ TrStr _at_t2945 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2945); _tr_str_release(_at_t2945); });
#line 527 "src/fmt.tr"
            self->indent = (self->indent + 1LL);
#line 528 "src/fmt.tr"
            Formatter_emit_block(self, (*cc->body));
#line 529 "src/fmt.tr"
            self->indent = (self->indent - 1LL);
#line 530 "src/fmt.tr"
            ci2 = (ci2 + 1LL);
            _tr_str_release(head);
        }
#line 531 "src/fmt.tr"
        if ((finally_b->stmts->len > 0LL)) {
#line 532 "src/fmt.tr"
            Formatter_line(self, _tr_str_lit_len("finally:", 8LL));
#line 533 "src/fmt.tr"
            self->indent = (self->indent + 1LL);
#line 534 "src/fmt.tr"
            Formatter_emit_block(self, finally_b);
#line 535 "src/fmt.tr"
            self->indent = (self->indent - 1LL);
        }
    } else if (_t2915.tag == Stmt_SWith) {
        __auto_type items = _t2915.data.SWith.items;
__auto_type aliases = _t2915.data.SWith.aliases;
__auto_type body = _t2915.data.SWith.body;
#line 537 "src/fmt.tr"
        TrStr hs = _tr_str_lit_len("with ", 5LL);
#line 538 "src/fmt.tr"
        long long k = 0LL;
#line 539 "src/fmt.tr"
        while ((k < items->len)) {
#line 540 "src/fmt.tr"
            if ((k > 0LL)) {
#line 540 "src/fmt.tr"
                ({ TrStr _ap_t2946 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&hs, _ap_t2946); });
            }
#line 541 "src/fmt.tr"
            ({ TrStr _ap_t2947 = (Formatter_expr_str(self, ((Expr*)List_ptr_get(items, k)))); _tr_str_append(&hs, _ap_t2947); _tr_str_release(_ap_t2947); });
#line 542 "src/fmt.tr"
            if (((k < aliases->len) && (_tr_str_lenv((List_TrStr_get(aliases, k))) > 0LL))) {
#line 543 "src/fmt.tr"
                ({ TrStr _ap_t2948 = (_tr_str_lit_len(" as ", 4LL)); TrStr _ap_t2949 = (List_TrStr_get(aliases, k)); _tr_str_append(&hs, _ap_t2948); _tr_str_append(&hs, _ap_t2949); _tr_str_release(_ap_t2949); });
            }
#line 544 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 545 "src/fmt.tr"
        ({ TrStr _at_t2950 = (_tr_strx_concatv((hs), (_tr_str_lit_len(":", 1LL)))); Formatter_emit_simple(self, _at_t2950, src_line); _tr_str_release(_at_t2950); });
#line 546 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 547 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 548 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
        _tr_str_release(hs);
    } else if (_t2915.tag == Stmt_SSpawn) {
        __auto_type e = _t2915.data.SSpawn.expr;
        ({ TrStr _at_t2951 = (({ TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("spawn ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2951, src_line); _tr_str_release(_at_t2951); });
    } else if (_t2915.tag == Stmt_STaskGroup) {
        __auto_type body = _t2915.data.STaskGroup.body;
#line 551 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("task_group:", 11LL), src_line);
#line 552 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 553 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 554 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_SDefer) {
        __auto_type inner = _t2915.data.SDefer.stmt;
#line 556 "src/fmt.tr"
        ({ TrStr _ord_t2952 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2952); _tr_str_release(_ord_t2952); });
#line 557 "src/fmt.tr"
        StringBuilder_append(self->out, _tr_str_lit_len("defer ", 6LL));
#line 559 "src/fmt.tr"
        Formatter_emit_inline_stmt(self, inner);
#line 560 "src/fmt.tr"
        StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
    } else if (_t2915.tag == Stmt_SGpuBlock) {
        __auto_type body = _t2915.data.SGpuBlock.body;
#line 562 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("gpu:", 4LL), src_line);
#line 563 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 564 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 565 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2915.tag == Stmt_SLocalDecl) {
        __auto_type d = _t2915.data.SLocalDecl.decl;
        Formatter_emit_decl(self, d);
    } else if (1) {
        __auto_type _ = _t2915;
#line 570 "src/fmt.tr"
        self->unsupported = true;
    }
}

__attribute__((hot)) void Formatter_emit_inline_stmt(Formatter* self, Stmt* sp) {
#line 575 "src/fmt.tr"
    __auto_type _t2953 = (*sp);
    if (_t2953.tag == Stmt_SExpr) {
        __auto_type e = _t2953.data.SExpr.expr;
        ({ TrStr _ord_t2954 = (Formatter_expr_str(self, e)); StringBuilder_append(self->out, _ord_t2954); _tr_str_release(_ord_t2954); });
    } else if (_t2953.tag == Stmt_SReturn) {
        __auto_type v = _t2953.data.SReturn.val;
#line 578 "src/fmt.tr"
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 578 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("return", 6LL));
        } else {
#line 579 "src/fmt.tr"
            ({ TrStr _ord_t2955 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(self->out, _ord_t2955); _tr_str_release(_ord_t2955); });
        }
    } else if (_t2953.tag == Stmt_SAssign) {
        __auto_type t = _t2953.data.SAssign.target;
__auto_type v = _t2953.data.SAssign.val;
        ({ __auto_type _ord_t2956 = (({ TrStr _tr_cl = (Formatter_expr_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t2957 = (({ TrStr _tr_cl = (_ord_t2956); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(self->out, _ord_t2957); _tr_str_release(_ord_t2957); });
    } else if (1) {
        __auto_type _ = _t2953;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_let_str(Formatter* self, TrStr name, Ownership own, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) {
#line 586 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 587 "src/fmt.tr"
    if (is_const) {
#line 587 "src/fmt.tr"
        TrStr _strtmp_t2958 = _tr_str_lit_len("const ", 6LL);
        _tr_str_release(s);
        s = _strtmp_t2958;
    } else if (is_shared) {
#line 588 "src/fmt.tr"
        TrStr _strtmp_t2959 = _tr_str_lit_len("shared ", 7LL);
        _tr_str_release(s);
        s = _strtmp_t2959;
    } else if (is_mut) {
#line 589 "src/fmt.tr"
        TrStr _strtmp_t2960 = _tr_str_lit_len("mut ", 4LL);
        _tr_str_release(s);
        s = _strtmp_t2960;
    }
#line 590 "src/fmt.tr"
    ({ TrStr _ap_t2961 = (name); _tr_str_append(&s, _ap_t2961); });
#line 591 "src/fmt.tr"
    TrStr tn = Formatter_type_str(self, ty);
#line 592 "src/fmt.tr"
    if (((_tr_str_lenv((tn)) > 0LL) && (!_tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL)))))) {
#line 593 "src/fmt.tr"
        TrStr _strtmp_t2964 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tn)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2964;
    }
#line 594 "src/fmt.tr"
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 595 "src/fmt.tr"
        TrStr _strtmp_t2967 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" = ", 3LL)))); TrStr _tr_cr = (Formatter_expr_str(self, val)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2967;
    }
#line 596 "src/fmt.tr"
    TrStr _trret568 = _tr_str_retain(s);
    _tr_str_release(s);
    _tr_str_release(tn);
    return _trret568;
}

__attribute__((hot)) TrStr Formatter_pattern_str(Formatter* self, Pattern p) {
#line 599 "src/fmt.tr"
    __auto_type _t2968 = p;
    if (_t2968.tag == Pattern_PWild) {
        return _tr_str_lit_len("_", 1LL);
    } else if (_t2968.tag == Pattern_PBind) {
        __auto_type n = _t2968.data.PBind.name;
        TrStr _trret569 = _tr_str_retain(n);
        return _trret569;
    } else if (_t2968.tag == Pattern_PLitInt) {
        __auto_type v = _t2968.data.PLitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t2968.tag == Pattern_PLitStr) {
        __auto_type v = _t2968.data.PLitStr.val;
        TrStr _trret570 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("\"", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret570;
    } else if (_t2968.tag == Pattern_PLitBool) {
        __auto_type b = _t2968.data.PLitBool.val;
#line 605 "src/fmt.tr"
        if (b) {
#line 605 "src/fmt.tr"
            return _tr_str_lit_len("True", 4LL);
        }
#line 606 "src/fmt.tr"
        return _tr_str_lit_len("False", 5LL);
    } else if (_t2968.tag == Pattern_PVariant) {
        __auto_type tn = _t2968.data.PVariant.type_name;
__auto_type vn = _t2968.data.PVariant.variant;
        TrStr _trret571 = ({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret571;
    } else if (_t2968.tag == Pattern_PVariantBind) {
        __auto_type tn = _t2968.data.PVariantBind.type_name;
__auto_type vn = _t2968.data.PVariantBind.variant;
__auto_type f = _t2968.data.PVariantBind.field;
        TrStr _trret572 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret572;
    } else if (_t2968.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t2968.data.PVariantBindMany.type_name;
__auto_type vn = _t2968.data.PVariantBindMany.variant;
__auto_type fs = _t2968.data.PVariantBindMany.fields;
#line 610 "src/fmt.tr"
        TrStr s = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 611 "src/fmt.tr"
        long long i = 0LL;
#line 612 "src/fmt.tr"
        while ((i < fs->len)) {
#line 613 "src/fmt.tr"
            if ((i > 0LL)) {
#line 613 "src/fmt.tr"
                TrStr _strtmp_t2970 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2970;
            }
#line 614 "src/fmt.tr"
            TrStr _strtmp_t2972 = ({ TrStr _tr_cr = (List_TrStr_get(fs, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2972;
#line 615 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 616 "src/fmt.tr"
        TrStr _trret573 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
        _tr_str_release(s);
        return _trret573;
    } else if (_t2968.tag == Pattern_PTuple) {
        __auto_type a = _t2968.data.PTuple.first;
__auto_type b = _t2968.data.PTuple.second;
        TrStr _trret574 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (a))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (b)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret574;
    } else if (_t2968.tag == Pattern_POr) {
        __auto_type pats = _t2968.data.POr.patterns;
#line 619 "src/fmt.tr"
        TrStr s = _tr_str_lit_len("", 0LL);
#line 620 "src/fmt.tr"
        long long i = 0LL;
#line 621 "src/fmt.tr"
        while ((i < pats->len)) {
#line 622 "src/fmt.tr"
            if ((i > 0LL)) {
#line 622 "src/fmt.tr"
                TrStr _strtmp_t2974 = _tr_strx_concatv((s), (_tr_str_lit_len(" | ", 3LL)));
                _tr_str_release(s);
                s = _strtmp_t2974;
            }
#line 623 "src/fmt.tr"
            TrStr _strtmp_t2976 = ({ TrStr _tr_cr = (Formatter_pattern_str(self, List_Pattern_get(pats, i))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2976;
#line 624 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 625 "src/fmt.tr"
        TrStr _trret575 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret575;
    } else if (1) {
        __auto_type _ = _t2968;
        return _tr_str_lit_len("_", 1LL);
    }
}

__attribute__((hot)) TrStr Formatter_fn_header(Formatter* self, FunctionDef* f) {
#line 630 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 631 "src/fmt.tr"
    if (f->is_public) {
#line 631 "src/fmt.tr"
        TrStr _strtmp_t2977 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(s);
        s = _strtmp_t2977;
    }
#line 632 "src/fmt.tr"
    if (f->is_async) {
#line 632 "src/fmt.tr"
        TrStr _strtmp_t2979 = _tr_strx_concatv((s), (_tr_str_lit_len("async ", 6LL)));
        _tr_str_release(s);
        s = _strtmp_t2979;
    }
#line 633 "src/fmt.tr"
    TrStr _strtmp_t2982 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("def ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2982;
#line 634 "src/fmt.tr"
    if ((f->generics->len > 0LL)) {
#line 635 "src/fmt.tr"
        TrStr _strtmp_t2984 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2984;
#line 636 "src/fmt.tr"
        long long gi = 0LL;
#line 637 "src/fmt.tr"
        while ((gi < f->generics->len)) {
#line 638 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 638 "src/fmt.tr"
                TrStr _strtmp_t2986 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2986;
            }
#line 639 "src/fmt.tr"
            TrStr _strtmp_t2988 = ({ TrStr _tr_cr = (List_TrStr_get(f->generics, gi)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2988;
#line 640 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 641 "src/fmt.tr"
        TrStr _strtmp_t2990 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2990;
    }
#line 642 "src/fmt.tr"
    TrStr _strtmp_t2994 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("(", 1LL)))); TrStr _tr_cr = (Formatter_params_str(self, f->params)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2994;
#line 645 "src/fmt.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 646 "src/fmt.tr"
        TrStr tt = Formatter_type_str(self, f->throws_ty);
#line 647 "src/fmt.tr"
        if ((_tr_str_lenv((tt)) > 0LL)) {
#line 647 "src/fmt.tr"
            TrStr _strtmp_t2997 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" throws ", 8LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2997;
        }
    }
#line 648 "src/fmt.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 649 "src/fmt.tr"
        TrStr rt = Formatter_type_str(self, f->ret_ty);
#line 650 "src/fmt.tr"
        if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 650 "src/fmt.tr"
            TrStr _strtmp_t3000 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t3000;
        }
    }
#line 651 "src/fmt.tr"
    TrStr _trret576 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    return _trret576;
}

__attribute__((hot)) void Formatter_emit_function(Formatter* self, FunctionDef* f) {
#line 654 "src/fmt.tr"
    Formatter_flush_standalone_before(self, f->line);
#line 655 "src/fmt.tr"
    Formatter_emit_decorators(self, f->decorators);
#line 656 "src/fmt.tr"
    ({ TrStr _ord_t3001 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t3001); _tr_str_release(_ord_t3001); });
#line 657 "src/fmt.tr"
    ({ TrStr _ord_t3002 = (Formatter_fn_header(self, f)); StringBuilder_append(self->out, _ord_t3002); _tr_str_release(_ord_t3002); });
#line 658 "src/fmt.tr"
    ({ TrStr _ord_t3003 = (Formatter_trailing_for(self, f->line)); StringBuilder_append(self->out, _ord_t3003); _tr_str_release(_ord_t3003); });
#line 659 "src/fmt.tr"
    StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
#line 660 "src/fmt.tr"
    self->indent = (self->indent + 1LL);
#line 661 "src/fmt.tr"
    if ((f->body->stmts->len == 0LL)) {
#line 662 "src/fmt.tr"
        Formatter_line(self, _tr_str_lit_len("pass", 4LL));
    } else {
#line 664 "src/fmt.tr"
        Formatter_emit_block(self, f->body);
    }
#line 665 "src/fmt.tr"
    self->indent = (self->indent - 1LL);
}

__attribute__((hot)) void Formatter_emit_class(Formatter* self, ClassDef* c, TrStr kw) {
#line 668 "src/fmt.tr"
    Formatter_flush_standalone_before(self, c->line);
#line 669 "src/fmt.tr"
    Formatter_emit_decorators(self, c->decorators);
#line 670 "src/fmt.tr"
    TrStr head = _tr_str_lit_len("", 0LL);
#line 671 "src/fmt.tr"
    if (c->is_public) {
#line 671 "src/fmt.tr"
        TrStr _strtmp_t3004 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t3004;
    }
#line 672 "src/fmt.tr"
    ({ TrStr _ap_t3005 = (kw); TrStr _ap_t3006 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t3007 = (c->name); _tr_str_append(&head, _ap_t3005); _tr_str_append(&head, _ap_t3006); _tr_str_append(&head, _ap_t3007); });
#line 673 "src/fmt.tr"
    if ((c->generics->len > 0LL)) {
#line 674 "src/fmt.tr"
        ({ TrStr _ap_t3008 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t3008); });
#line 675 "src/fmt.tr"
        long long gi = 0LL;
#line 676 "src/fmt.tr"
        while ((gi < c->generics->len)) {
#line 677 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 677 "src/fmt.tr"
                ({ TrStr _ap_t3009 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t3009); });
            }
#line 678 "src/fmt.tr"
            ({ TrStr _ap_t3010 = (List_TrStr_get(c->generics, gi)); _tr_str_append(&head, _ap_t3010); _tr_str_release(_ap_t3010); });
#line 679 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 680 "src/fmt.tr"
        ({ TrStr _ap_t3011 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t3011); });
    }
#line 681 "src/fmt.tr"
    if ((c->base_classes->len > 0LL)) {
#line 682 "src/fmt.tr"
        ({ TrStr _ap_t3012 = (_tr_str_lit_len(" extends ", 9LL)); TrStr _ap_t3013 = (List_TrStr_get(c->base_classes, 0LL)); _tr_str_append(&head, _ap_t3012); _tr_str_append(&head, _ap_t3013); _tr_str_release(_ap_t3013); });
#line 683 "src/fmt.tr"
        long long bi = 1LL;
#line 684 "src/fmt.tr"
        while ((bi < c->base_classes->len)) {
#line 685 "src/fmt.tr"
            ({ TrStr _ap_t3014 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t3015 = (List_TrStr_get(c->base_classes, bi)); _tr_str_append(&head, _ap_t3014); _tr_str_append(&head, _ap_t3015); _tr_str_release(_ap_t3015); });
#line 686 "src/fmt.tr"
            bi = (bi + 1LL);
        }
    }
#line 687 "src/fmt.tr"
    if ((c->iface_names->len > 0LL)) {
#line 688 "src/fmt.tr"
        ({ TrStr _ap_t3016 = (_tr_str_lit_len(" implements ", 12LL)); _tr_str_append(&head, _ap_t3016); });
#line 689 "src/fmt.tr"
        long long ii = 0LL;
#line 690 "src/fmt.tr"
        while ((ii < c->iface_names->len)) {
#line 691 "src/fmt.tr"
            if ((ii > 0LL)) {
#line 691 "src/fmt.tr"
                ({ TrStr _ap_t3017 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t3017); });
            }
#line 692 "src/fmt.tr"
            ({ TrStr _ap_t3018 = (List_TrStr_get(c->iface_names, ii)); _tr_str_append(&head, _ap_t3018); _tr_str_release(_ap_t3018); });
#line 693 "src/fmt.tr"
            ii = (ii + 1LL);
        }
    }
#line 694 "src/fmt.tr"
    ({ TrStr _at_t3019 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t3019); _tr_str_release(_at_t3019); });
#line 695 "src/fmt.tr"
    self->indent = (self->indent + 1LL);
#line 696 "src/fmt.tr"
    bool had_body = false;
#line 697 "src/fmt.tr"
    long long fi = 0LL;
#line 698 "src/fmt.tr"
    while ((fi < c->fields->len)) {
#line 699 "src/fmt.tr"
        FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
#line 700 "src/fmt.tr"
        TrStr fs = ({ __auto_type _ord_t3020 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("pub ", 4LL)), (fld->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t3020); TrStr _tr_cr = (Formatter_type_str(self, fld->ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 701 "src/fmt.tr"
        if ((((unsigned long long)(fld->default_val)) != ((unsigned long long)(0LL)))) {
#line 702 "src/fmt.tr"
            ({ TrStr _ap_t3021 = (_tr_str_lit_len(" = ", 3LL)); TrStr _ap_t3022 = (Formatter_expr_str(self, fld->default_val)); _tr_str_append(&fs, _ap_t3021); _tr_str_append(&fs, _ap_t3022); _tr_str_release(_ap_t3022); });
        }
#line 703 "src/fmt.tr"
        Formatter_line(self, fs);
#line 704 "src/fmt.tr"
        had_body = true;
#line 705 "src/fmt.tr"
        fi = (fi + 1LL);
        _tr_str_release(fs);
    }
#line 706 "src/fmt.tr"
    long long mi = 0LL;
#line 707 "src/fmt.tr"
    while ((mi < c->methods->len)) {
#line 708 "src/fmt.tr"
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(c->methods, mi)));
#line 709 "src/fmt.tr"
        had_body = true;
#line 710 "src/fmt.tr"
        mi = (mi + 1LL);
    }
#line 711 "src/fmt.tr"
    if ((!had_body)) {
#line 712 "src/fmt.tr"
        Formatter_line(self, _tr_str_lit_len("pass", 4LL));
    }
#line 713 "src/fmt.tr"
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_enum(Formatter* self, EnumDef* e) {
#line 716 "src/fmt.tr"
    Formatter_flush_standalone_before(self, e->line);
#line 717 "src/fmt.tr"
    Formatter_emit_decorators(self, e->decorators);
#line 718 "src/fmt.tr"
    TrStr head = _tr_str_lit_len("", 0LL);
#line 719 "src/fmt.tr"
    if (e->is_public) {
#line 719 "src/fmt.tr"
        TrStr _strtmp_t3023 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t3023;
    }
#line 720 "src/fmt.tr"
    ({ TrStr _ap_t3024 = (_tr_str_lit_len("enum ", 5LL)); TrStr _ap_t3025 = (e->name); _tr_str_append(&head, _ap_t3024); _tr_str_append(&head, _ap_t3025); });
#line 721 "src/fmt.tr"
    if ((e->generics->len > 0LL)) {
#line 722 "src/fmt.tr"
        ({ TrStr _ap_t3026 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t3026); });
#line 723 "src/fmt.tr"
        long long gi = 0LL;
#line 724 "src/fmt.tr"
        while ((gi < e->generics->len)) {
#line 725 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 725 "src/fmt.tr"
                ({ TrStr _ap_t3027 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t3027); });
            }
#line 726 "src/fmt.tr"
            ({ TrStr _ap_t3028 = (List_TrStr_get(e->generics, gi)); _tr_str_append(&head, _ap_t3028); _tr_str_release(_ap_t3028); });
#line 727 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 728 "src/fmt.tr"
        ({ TrStr _ap_t3029 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t3029); });
    }
#line 729 "src/fmt.tr"
    ({ TrStr _at_t3030 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t3030); _tr_str_release(_at_t3030); });
#line 730 "src/fmt.tr"
    self->indent = (self->indent + 1LL);
#line 731 "src/fmt.tr"
    long long vi = 0LL;
#line 732 "src/fmt.tr"
    while ((vi < e->variants->len)) {
#line 733 "src/fmt.tr"
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
#line 734 "src/fmt.tr"
        if ((v->fields->len > 0LL)) {
#line 735 "src/fmt.tr"
            ({ __auto_type _ord_t3031 = (_tr_strx_concatv((v->name), (_tr_str_lit_len("(", 1LL)))); TrStr _at_t3032 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3031); TrStr _tr_cr = (Formatter_params_str(self, v->fields)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t3032); _tr_str_release(_at_t3032); });
        } else {
#line 737 "src/fmt.tr"
            ({ TrStr _apt_t3033 = _tr_str_retain(v->name); Formatter_line(self, _apt_t3033); _tr_str_release(_apt_t3033); });
        }
#line 738 "src/fmt.tr"
        vi = (vi + 1LL);
    }
#line 739 "src/fmt.tr"
    long long mi = 0LL;
#line 740 "src/fmt.tr"
    while ((mi < e->methods->len)) {
#line 741 "src/fmt.tr"
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(e->methods, mi)));
#line 742 "src/fmt.tr"
        mi = (mi + 1LL);
    }
#line 743 "src/fmt.tr"
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_interface(Formatter* self, InterfaceDef* it) {
#line 746 "src/fmt.tr"
    Formatter_flush_standalone_before(self, it->line);
#line 747 "src/fmt.tr"
    TrStr head = _tr_str_lit_len("", 0LL);
#line 748 "src/fmt.tr"
    if (it->is_public) {
#line 748 "src/fmt.tr"
        TrStr _strtmp_t3034 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t3034;
    }
#line 749 "src/fmt.tr"
    ({ TrStr _ap_t3035 = (_tr_str_lit_len("interface ", 10LL)); TrStr _ap_t3036 = (it->name); _tr_str_append(&head, _ap_t3035); _tr_str_append(&head, _ap_t3036); });
#line 750 "src/fmt.tr"
    if ((it->generics->len > 0LL)) {
#line 751 "src/fmt.tr"
        ({ TrStr _ap_t3037 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t3037); });
#line 752 "src/fmt.tr"
        long long gi = 0LL;
#line 753 "src/fmt.tr"
        while ((gi < it->generics->len)) {
#line 754 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 754 "src/fmt.tr"
                ({ TrStr _ap_t3038 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t3038); });
            }
#line 755 "src/fmt.tr"
            ({ TrStr _ap_t3039 = (List_TrStr_get(it->generics, gi)); _tr_str_append(&head, _ap_t3039); _tr_str_release(_ap_t3039); });
#line 756 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 757 "src/fmt.tr"
        ({ TrStr _ap_t3040 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t3040); });
    }
#line 758 "src/fmt.tr"
    ({ TrStr _at_t3041 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t3041); _tr_str_release(_at_t3041); });
#line 759 "src/fmt.tr"
    self->indent = (self->indent + 1LL);
#line 760 "src/fmt.tr"
    long long mi = 0LL;
#line 761 "src/fmt.tr"
    while ((mi < it->methods->len)) {
#line 762 "src/fmt.tr"
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(it->methods, mi)));
#line 763 "src/fmt.tr"
        mi = (mi + 1LL);
    }
#line 764 "src/fmt.tr"
    if ((it->methods->len == 0LL)) {
#line 764 "src/fmt.tr"
        Formatter_line(self, _tr_str_lit_len("pass", 4LL));
    }
#line 765 "src/fmt.tr"
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_decl(Formatter* self, Decl* dp) {
#line 768 "src/fmt.tr"
    __auto_type _t3042 = (*dp);
    if (_t3042.tag == Decl_DFunction) {
        __auto_type f = _t3042.data.DFunction.func;
        Formatter_emit_function(self, f);
    } else if (_t3042.tag == Decl_DDecoratorDef) {
        __auto_type f = _t3042.data.DDecoratorDef.func;
#line 771 "src/fmt.tr"
        Formatter_flush_standalone_before(self, f->line);
#line 774 "src/fmt.tr"
        ({ TrStr _at_t3043 = (({ TrStr _tr_cr = (Formatter_fn_header(self, f)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("decorator ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_line(self, _at_t3043); _tr_str_release(_at_t3043); });
#line 775 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 776 "src/fmt.tr"
        if ((f->body->stmts->len == 0LL)) {
#line 776 "src/fmt.tr"
            Formatter_line(self, _tr_str_lit_len("pass", 4LL));
        } else {
#line 777 "src/fmt.tr"
            Formatter_emit_block(self, f->body);
        }
#line 778 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t3042.tag == Decl_DClass) {
        __auto_type c = _t3042.data.DClass.cls;
        Formatter_emit_class(self, c, _tr_str_lit_len("class", 5LL));
    } else if (_t3042.tag == Decl_DActor) {
        __auto_type c = _t3042.data.DActor.cls;
        Formatter_emit_class(self, c, _tr_str_lit_len("actor", 5LL));
    } else if (_t3042.tag == Decl_DEnum) {
        __auto_type e = _t3042.data.DEnum.enm;
        Formatter_emit_enum(self, e);
    } else if (_t3042.tag == Decl_DInterface) {
        __auto_type it = _t3042.data.DInterface.iface;
        Formatter_emit_interface(self, it);
    } else if (_t3042.tag == Decl_DExtend) {
        __auto_type target = _t3042.data.DExtend.target;
__auto_type methods = _t3042.data.DExtend.methods;
#line 784 "src/fmt.tr"
        ({ TrStr _at_t3044 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("extend ", 7LL)), (target))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t3044); _tr_str_release(_at_t3044); });
#line 785 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 786 "src/fmt.tr"
        long long mi = 0LL;
#line 787 "src/fmt.tr"
        while ((mi < methods->len)) {
#line 788 "src/fmt.tr"
            Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(methods, mi)));
#line 789 "src/fmt.tr"
            mi = (mi + 1LL);
        }
#line 790 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t3042.tag == Decl_DImport) {
        __auto_type path = _t3042.data.DImport.path;
__auto_type alias = _t3042.data.DImport.alias;
#line 792 "src/fmt.tr"
        if ((_tr_str_lenv((alias)) > 0LL)) {
#line 792 "src/fmt.tr"
            ({ TrStr _at_t3045 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("import ", 7LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" as ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (alias)); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t3045); _tr_str_release(_at_t3045); });
        } else {
#line 793 "src/fmt.tr"
            ({ TrStr _at_t3046 = (_tr_strx_concatv((_tr_str_lit_len("import ", 7LL)), (path))); Formatter_line(self, _at_t3046); _tr_str_release(_at_t3046); });
        }
    } else if (_t3042.tag == Decl_DFromImport) {
        __auto_type path = _t3042.data.DFromImport.path;
__auto_type items = _t3042.data.DFromImport.items;
#line 795 "src/fmt.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("from ", 5LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" import ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 796 "src/fmt.tr"
        long long i = 0LL;
#line 797 "src/fmt.tr"
        while ((i < items->len)) {
#line 798 "src/fmt.tr"
            if ((i > 0LL)) {
#line 798 "src/fmt.tr"
                TrStr _strtmp_t3048 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t3048;
            }
#line 799 "src/fmt.tr"
            ImportItem* it = ((ImportItem*)List_ptr_get(items, i));
#line 800 "src/fmt.tr"
            ({ TrStr _ap_t3049 = (it->name); _tr_str_append(&s, _ap_t3049); });
#line 801 "src/fmt.tr"
            if ((_tr_str_lenv((it->alias)) > 0LL)) {
#line 801 "src/fmt.tr"
                TrStr _strtmp_t3052 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" as ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (it->alias)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t3052;
            }
#line 802 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 803 "src/fmt.tr"
        Formatter_line(self, s);
        _tr_str_release(s);
    } else if (_t3042.tag == Decl_DExtern) {
        __auto_type abi = _t3042.data.DExtern.abi;
__auto_type functions = _t3042.data.DExtern.functions;
#line 805 "src/fmt.tr"
        ({ TrStr _at_t3053 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("extern \"", 8LL)), (abi))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\":", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t3053); _tr_str_release(_at_t3053); });
#line 806 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 807 "src/fmt.tr"
        long long fi = 0LL;
#line 808 "src/fmt.tr"
        while ((fi < functions->len)) {
#line 809 "src/fmt.tr"
            ({ TrStr _at_t3054 = (Formatter_fn_header_no_colon(self, ((FunctionDef*)List_ptr_get(functions, fi)))); Formatter_line(self, _at_t3054); _tr_str_release(_at_t3054); });
#line 810 "src/fmt.tr"
            fi = (fi + 1LL);
        }
#line 811 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t3042.tag == Decl_DTypeAlias) {
        __auto_type name = _t3042.data.DTypeAlias.name;
__auto_type target = _t3042.data.DTypeAlias.target;
#line 813 "src/fmt.tr"
        ({ TrStr _at_t3055 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_type_str(self, target)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_line(self, _at_t3055); _tr_str_release(_at_t3055); });
    } else if (_t3042.tag == Decl_DTopLevelStmt) {
        __auto_type s = _t3042.data.DTopLevelStmt.stmt;
#line 815 "src/fmt.tr"
        Formatter_emit_stmt(self, s, 0LL);
    } else if (1) {
        __auto_type _ = _t3042;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_fn_header_no_colon(Formatter* self, FunctionDef* f) {
#line 820 "src/fmt.tr"
    TrStr h = Formatter_fn_header(self, f);
#line 821 "src/fmt.tr"
    if (((_tr_str_lenv((h)) > 0LL) && ({ __auto_type _ord_t3056 = ((_tr_str_lenv((h)) - 1LL)); __auto_type _ord_t3057 = (_tr_str_lenv((h))); __auto_type _wr = (({ TrStr _cmpa_t3058 = (_tr_str_slicev((h), _ord_t3056, _ord_t3057)); TrStr _cmpb_t3059 = (_tr_str_lit_len(":", 1LL)); _Bool _cmpr_t3060 = _tr_str_eqv(_cmpa_t3058, _cmpb_t3059); _tr_str_release(_cmpa_t3058); _cmpr_t3060; })); _wr; }))) {
#line 822 "src/fmt.tr"
        TrStr _trret577 = _tr_str_slicev((h), 0LL, (_tr_str_lenv((h)) - 1LL));
        _tr_str_release(h);
        return _trret577;
    }
#line 823 "src/fmt.tr"
    TrStr _trret578 = _tr_str_retain(h);
    _tr_str_release(h);
    return _trret578;
}

__attribute__((hot)) bool Formatter_is_block_decl(Formatter* self, Decl* dp) {
#line 828 "src/fmt.tr"
    __auto_type _t3061 = (*dp);
    if (_t3061.tag == Decl_DFunction) {
        __auto_type _ = _t3061.data.DFunction.func;
        return true;
    } else if (_t3061.tag == Decl_DDecoratorDef) {
        __auto_type _ = _t3061.data.DDecoratorDef.func;
        return true;
    } else if (_t3061.tag == Decl_DClass) {
        __auto_type _ = _t3061.data.DClass.cls;
        return true;
    } else if (_t3061.tag == Decl_DActor) {
        __auto_type _ = _t3061.data.DActor.cls;
        return true;
    } else if (_t3061.tag == Decl_DEnum) {
        __auto_type _ = _t3061.data.DEnum.enm;
        return true;
    } else if (_t3061.tag == Decl_DInterface) {
        __auto_type _ = _t3061.data.DInterface.iface;
        return true;
    } else if (_t3061.tag == Decl_DExtend) {
        return true;
    } else if (_t3061.tag == Decl_DExtern) {
        return true;
    } else if (1) {
        __auto_type _ = _t3061;
        return false;
    }
}

__attribute__((hot)) TrStr Formatter_format_program(Formatter* self, Program* prog) {
#line 840 "src/fmt.tr"
    long long i = 0LL;
#line 841 "src/fmt.tr"
    while ((i < prog->decls->len)) {
#line 842 "src/fmt.tr"
        Decl* dp = ((Decl*)List_ptr_get(prog->decls, i));
#line 845 "src/fmt.tr"
        if ((((i > 0LL) && Formatter_is_block_decl(self, dp)) && (self->out->buf->len > 0LL))) {
#line 846 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
        }
#line 847 "src/fmt.tr"
        Formatter_emit_decl(self, dp);
#line 848 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 849 "src/fmt.tr"
    Formatter_flush_remaining(self);
#line 850 "src/fmt.tr"
    return StringObj_as_str(StringBuilder_to_string(self->out));
}

