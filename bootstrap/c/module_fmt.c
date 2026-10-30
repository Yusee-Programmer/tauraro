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
    ({ TrStr _ord_t2536 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2536); _tr_str_release(_ord_t2536); });
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
            ({ TrStr _ord_t2537 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2537); _tr_str_release(_ord_t2537); });
#line 68 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("#", 1LL));
#line 69 "src/fmt.tr"
            ({ TrStr _ord_t2538 = (List_TrStr_get(self->c_texts, self->ci)); StringBuilder_append(self->out, _ord_t2538); _tr_str_release(_ord_t2538); });
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
                ({ TrStr _ord_t2539 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2539); _tr_str_release(_ord_t2539); });
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
            TrStr _trret508 = _tr_strx_concatv((_tr_str_lit_len("  #", 3LL)), (txt));
            _tr_str_release(txt);
            return _trret508;
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
        TrStr _trret509 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("ref ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret509;
    }
#line 159 "src/fmt.tr"
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("mut_ref", 7LL))) && (ty->args->len == 1LL))) {
#line 160 "src/fmt.tr"
        TrStr _trret510 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("mut ref ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret510;
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
                ({ TrStr _ap_t2540 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ts, _ap_t2540); });
            }
#line 166 "src/fmt.tr"
            ({ TrStr _ap_t2541 = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, ti)))); _tr_str_append(&ts, _ap_t2541); _tr_str_release(_ap_t2541); });
#line 167 "src/fmt.tr"
            ti = (ti + 1LL);
        }
#line 168 "src/fmt.tr"
        TrStr _trret511 = _tr_strx_concatv((ts), (_tr_str_lit_len(")", 1LL)));
        _tr_str_release(ts);
        return _trret511;
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
                ({ TrStr _ap_t2542 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ds, _ap_t2542); });
            }
#line 175 "src/fmt.tr"
            ({ TrStr _ap_t2543 = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, di)))); _tr_str_append(&ds, _ap_t2543); _tr_str_release(_ap_t2543); });
#line 176 "src/fmt.tr"
            di = (di + 1LL);
        }
#line 177 "src/fmt.tr"
        ({ TrStr _ap_t2544 = (_tr_str_lit_len(")", 1LL)); _tr_str_append(&ds, _ap_t2544); });
#line 178 "src/fmt.tr"
        if ((ty->args->len > 0LL)) {
#line 179 "src/fmt.tr"
            TrStr rt = Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, (ty->args->len - 1LL))));
#line 180 "src/fmt.tr"
            if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 180 "src/fmt.tr"
                ({ TrStr _ap_t2545 = (_tr_str_lit_len(" -> ", 4LL)); TrStr _ap_t2546 = (rt); _tr_str_append(&ds, _ap_t2545); _tr_str_append(&ds, _ap_t2546); });
            }
        }
#line 181 "src/fmt.tr"
        TrStr _trret512 = _tr_str_retain(ds);
        _tr_str_release(ds);
        return _trret512;
    }
#line 182 "src/fmt.tr"
    TrStr s = _tr_str_retain(ty->name);
#line 183 "src/fmt.tr"
    if ((ty->args->len > 0LL)) {
#line 184 "src/fmt.tr"
        TrStr _strtmp_t2548 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2548;
#line 185 "src/fmt.tr"
        long long i = 0LL;
#line 186 "src/fmt.tr"
        while ((i < ty->args->len)) {
#line 187 "src/fmt.tr"
            if ((i > 0LL)) {
#line 187 "src/fmt.tr"
                TrStr _strtmp_t2550 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2550;
            }
#line 188 "src/fmt.tr"
            TrStr _strtmp_t2552 = ({ TrStr _tr_cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2552;
#line 189 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 190 "src/fmt.tr"
        TrStr _strtmp_t2554 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2554;
    }
#line 191 "src/fmt.tr"
    if ((_tr_str_lenv((ty->from_param)) > 0LL)) {
#line 192 "src/fmt.tr"
        TrStr _strtmp_t2557 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" from ", 6LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->from_param)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2557;
    }
#line 193 "src/fmt.tr"
    TrStr _trret513 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret513;
}

__attribute__((hot)) bool Formatter_needs_parens(Formatter* self, Expr* e) {
#line 199 "src/fmt.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 199 "src/fmt.tr"
        return false;
    }
#line 200 "src/fmt.tr"
    __auto_type _t2558 = (*e);
    if (_t2558.tag == Expr_EBinOp) {
        return true;
    } else if (_t2558.tag == Expr_EUnaryOp) {
        return true;
    } else if (_t2558.tag == Expr_EIfElse) {
        return true;
    } else if (_t2558.tag == Expr_ETuple) {
        __auto_type _ = _t2558.data.ETuple.items;
        return true;
    } else if (_t2558.tag == Expr_ERange) {
        return true;
    } else if (_t2558.tag == Expr_ECast) {
        return true;
    } else if (1) {
        __auto_type _ = _t2558;
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
            TrStr _strtmp_t2560 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2560;
        }
#line 219 "src/fmt.tr"
        TrStr _strtmp_t2562 = ({ TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(args, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2562;
#line 220 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 221 "src/fmt.tr"
    TrStr _trret514 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret514;
}

__attribute__((hot)) TrStr Formatter_expr_str(Formatter* self, Expr* e) {
#line 224 "src/fmt.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 224 "src/fmt.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 225 "src/fmt.tr"
    __auto_type _t2563 = (*e);
    if (_t2563.tag == Expr_ELitInt) {
        __auto_type v = _t2563.data.ELitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t2563.tag == Expr_ELitFloat) {
        __auto_type v = _t2563.data.ELitFloat.val;
        return _tr_str_wrap(_tr_float_to_str((double)(v)));
    } else if (_t2563.tag == Expr_ELitStr) {
        __auto_type v = _t2563.data.ELitStr.val;
#line 232 "src/fmt.tr"
        if (self->in_fstring) {
#line 233 "src/fmt.tr"
            TrStr _trret515 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str_sq(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            return _trret515;
        }
#line 234 "src/fmt.tr"
        TrStr _trret516 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("\"", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret516;
    } else if (_t2563.tag == Expr_ELitBool) {
        __auto_type b = _t2563.data.ELitBool.val;
#line 236 "src/fmt.tr"
        if (b) {
#line 236 "src/fmt.tr"
            return _tr_str_lit_len("True", 4LL);
        }
#line 237 "src/fmt.tr"
        return _tr_str_lit_len("False", 5LL);
    } else if (_t2563.tag == Expr_ELitChar) {
        __auto_type v = _t2563.data.ELitChar.val;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_char(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("'", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_ELitBytes) {
        __auto_type v = _t2563.data.ELitBytes.val;
        TrStr _trret517 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("b\"", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret517;
    } else if (_t2563.tag == Expr_ERawStr) {
        __auto_type v = _t2563.data.ERawStr.val;
        TrStr _trret518 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("r\"", 2LL)), (v))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret518;
    } else if (_t2563.tag == Expr_ELitNone) {
        return _tr_str_lit_len("None", 4LL);
    } else if (_t2563.tag == Expr_EIdent) {
        __auto_type nm = _t2563.data.EIdent.name;
        TrStr _trret519 = _tr_str_retain(nm);
        return _trret519;
    } else if (_t2563.tag == Expr_EBinOp) {
        __auto_type op = _t2563.data.EBinOp.op;
__auto_type l = _t2563.data.EBinOp.left;
__auto_type r = _t2563.data.EBinOp.right;
#line 244 "src/fmt.tr"
        TrStr _trret520 = ({ __auto_type _ord_t2564 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, l)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (op)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2564); TrStr _tr_cr = (Formatter_operand_str(self, r)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        return _trret520;
    } else if (_t2563.tag == Expr_EUnaryOp) {
        __auto_type op = _t2563.data.EUnaryOp.op;
__auto_type x = _t2563.data.EUnaryOp.expr;
#line 246 "src/fmt.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL)))) {
#line 246 "src/fmt.tr"
            TrStr _trret521 = ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("not ", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            return _trret521;
        }
#line 247 "src/fmt.tr"
        TrStr _trret522 = ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((op), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        return _trret522;
    } else if (_t2563.tag == Expr_ECall) {
        __auto_type callee = _t2563.data.ECall.callee;
__auto_type args = _t2563.data.ECall.args;
#line 249 "src/fmt.tr"
        return ({ __auto_type _ord_t2565 = (({ TrStr _tr_cl = (Formatter_expr_str(self, callee)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2565); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
    } else if (_t2563.tag == Expr_EMethodCall) {
        __auto_type obj = _t2563.data.EMethodCall.obj;
__auto_type m = _t2563.data.EMethodCall.method;
__auto_type args = _t2563.data.EMethodCall.args;
#line 251 "src/fmt.tr"
        TrStr _trret523 = ({ __auto_type _ord_t2566 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2566); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
        return _trret523;
    } else if (_t2563.tag == Expr_EPropAccess) {
        __auto_type obj = _t2563.data.EPropAccess.obj;
__auto_type p = _t2563.data.EPropAccess.prop;
#line 253 "src/fmt.tr"
        TrStr _trret524 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret524;
    } else if (_t2563.tag == Expr_EIndex) {
        __auto_type obj = _t2563.data.EIndex.obj;
__auto_type idx = _t2563.data.EIndex._tr_v_index;
#line 255 "src/fmt.tr"
        return ({ __auto_type _ord_t2567 = (({ TrStr _tr_cl = (Formatter_operand_str(self, obj)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2567); TrStr _tr_cr = (Formatter_expr_str(self, idx)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
    } else if (_t2563.tag == Expr_ECast) {
        __auto_type x = _t2563.data.ECast.expr;
__auto_type ty = _t2563.data.ECast.ty;
#line 257 "src/fmt.tr"
        return ({ __auto_type _ord_t2568 = (({ TrStr _tr_cl = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" as ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2568); TrStr _tr_cr = (Formatter_type_str(self, ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t2563.tag == Expr_EFString) {
        __auto_type parts = _t2563.data.EFString.parts;
        return Formatter_fstring_str(self, parts);
    } else if (_t2563.tag == Expr_ETryExpr) {
        __auto_type x = _t2563.data.ETryExpr.expr;
        return ({ TrStr _tr_cl = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("?", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_EClosure) {
        __auto_type params = _t2563.data.EClosure.params;
__auto_type ret_ty = _t2563.data.EClosure.ret_ty;
__auto_type body = _t2563.data.EClosure.body;
__auto_type is_async = _t2563.data.EClosure.is_async;
#line 261 "src/fmt.tr"
        TrStr _trret525 = Formatter_closure_str(self, params, ret_ty, body, is_async);
        return _trret525;
    } else if (_t2563.tag == Expr_ESuperMethodCall) {
        __auto_type base = _t2563.data.ESuperMethodCall.base_class;
__auto_type m = _t2563.data.ESuperMethodCall.method;
__auto_type args = _t2563.data.ESuperMethodCall.args;
#line 263 "src/fmt.tr"
        TrStr _trret526 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_args_str(self, args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret526;
    } else if (_t2563.tag == Expr_ESuperPropAccess) {
        __auto_type base = _t2563.data.ESuperPropAccess.base_class;
__auto_type p = _t2563.data.ESuperPropAccess.prop;
#line 265 "src/fmt.tr"
        TrStr _trret527 = _tr_strx_concatv((_tr_str_lit_len("super.", 6LL)), (p));
        return _trret527;
    } else if (_t2563.tag == Expr_EList) {
        __auto_type items = _t2563.data.EList.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_ESet) {
        __auto_type items = _t2563.data.ESet.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("{", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("}", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_EDict) {
        __auto_type keys = _t2563.data.EDict.keys;
__auto_type vals = _t2563.data.EDict.vals;
        return Formatter_dict_str(self, keys, vals);
    } else if (_t2563.tag == Expr_ETuple) {
        __auto_type items = _t2563.data.ETuple.items;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_args_str(self, items)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_EListComp) {
        __auto_type elem = _t2563.data.EListComp.element;
__auto_type gens = _t2563.data.EListComp.generators;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_comp_str(self, elem, gens)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_EGeneratorExpr) {
        __auto_type elem = _t2563.data.EGeneratorExpr.element;
__auto_type gens = _t2563.data.EGeneratorExpr.generators;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_comp_str(self, elem, gens)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("(", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_ESlice) {
        __auto_type a = _t2563.data.ESlice.start;
__auto_type b = _t2563.data.ESlice.stop;
__auto_type c = _t2563.data.ESlice.step;
        return Formatter_slice_str(self, a, b, c);
    } else if (_t2563.tag == Expr_EAwait) {
        __auto_type x = _t2563.data.EAwait.expr;
        return ({ TrStr _tr_cr = (Formatter_operand_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("await ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t2563.tag == Expr_EYield) {
        __auto_type x = _t2563.data.EYield.expr;
#line 275 "src/fmt.tr"
        if ((((unsigned long long)(x)) == ((unsigned long long)(0LL)))) {
#line 275 "src/fmt.tr"
            return _tr_str_lit_len("yield", 5LL);
        }
#line 276 "src/fmt.tr"
        return ({ TrStr _tr_cr = (Formatter_expr_str(self, x)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("yield ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t2563.tag == Expr_ERange) {
        __auto_type a = _t2563.data.ERange.start;
__auto_type b = _t2563.data.ERange.end;
__auto_type inclusive = _t2563.data.ERange.inclusive;
#line 278 "src/fmt.tr"
        TrStr sep = _tr_str_lit_len("..", 2LL);
#line 279 "src/fmt.tr"
        if (inclusive) {
#line 279 "src/fmt.tr"
            TrStr _strtmp_t2569 = _tr_str_lit_len("..=", 3LL);
            _tr_str_release(sep);
            sep = _strtmp_t2569;
        }
#line 280 "src/fmt.tr"
        TrStr _trret528 = ({ __auto_type _ord_t2570 = (({ TrStr _tr_cl = (Formatter_operand_str(self, a)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sep)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2570); TrStr _tr_cr = (Formatter_operand_str(self, b)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(sep);
        return _trret528;
    } else if (_t2563.tag == Expr_ESizeOf) {
        __auto_type ty = _t2563.data.ESizeOf.ty;
        return ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_type_str(self, ty)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("sizeof(", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    } else if (_t2563.tag == Expr_EIfElse) {
        __auto_type c = _t2563.data.EIfElse.cond;
__auto_type t = _t2563.data.EIfElse.then_expr;
__auto_type f = _t2563.data.EIfElse.else_expr;
#line 283 "src/fmt.tr"
        return ({ __auto_type _ord_t2571 = (({ TrStr _tr_cl = (Formatter_operand_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" if ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2572 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2571); TrStr _tr_cr = (Formatter_operand_str(self, c)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" else ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2572); TrStr _tr_cr = (Formatter_operand_str(self, f)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t2563.tag == Expr_ETypeArg) {
        __auto_type ty = _t2563.data.ETypeArg.ty;
        return Formatter_type_str(self, ty);
    } else if (1) {
        __auto_type _ = _t2563;
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
            TrStr _strtmp_t2575 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("{", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (es)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2575;
#line 300 "src/fmt.tr"
            if ((_tr_str_lenv((part->fmt_spec)) > 0LL)) {
#line 301 "src/fmt.tr"
                TrStr _strtmp_t2578 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (part->fmt_spec)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t2578;
            }
#line 302 "src/fmt.tr"
            TrStr _strtmp_t2580 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
            _tr_str_release(s);
            s = _strtmp_t2580;
            _tr_str_release(es);
        } else {
#line 304 "src/fmt.tr"
            TrStr _strtmp_t2582 = ({ TrStr _tr_cr = (Formatter_escape_str(self, part->text)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2582;
        }
#line 305 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 306 "src/fmt.tr"
    TrStr _trret529 = _tr_strx_concatv((s), (_tr_str_lit_len("\"", 1LL)));
    _tr_str_release(s);
    return _trret529;
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
            TrStr _strtmp_t2584 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2584;
        }
#line 314 "src/fmt.tr"
        TrStr _strtmp_t2589 = ({ __auto_type _ord_t2588 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(keys, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2588); TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(vals, i)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(s);
        s = _strtmp_t2589;
#line 315 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 316 "src/fmt.tr"
    TrStr _trret530 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
    _tr_str_release(s);
    return _trret530;
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
        TrStr _strtmp_t2595 = ({ __auto_type _ord_t2594 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" for ", 5LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2594); TrStr _tr_cr = (Formatter_expr_str(self, g->iter)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
        _tr_str_release(s);
        s = _strtmp_t2595;
#line 324 "src/fmt.tr"
        long long j = 0LL;
#line 325 "src/fmt.tr"
        while ((j < g->ifs->len)) {
#line 326 "src/fmt.tr"
            TrStr _strtmp_t2598 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" if ", 4LL)))); TrStr _tr_cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(g->ifs, j)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2598;
#line 327 "src/fmt.tr"
            j = (j + 1LL);
        }
#line 328 "src/fmt.tr"
        i = (i + 1LL);
    }
#line 329 "src/fmt.tr"
    TrStr _trret531 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret531;
}

__attribute__((hot)) TrStr Formatter_slice_str(Formatter* self, Expr* a, Expr* b, Expr* c) {
#line 332 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 333 "src/fmt.tr"
    if ((((unsigned long long)(a)) != ((unsigned long long)(0LL)))) {
#line 333 "src/fmt.tr"
        TrStr _strtmp_t2600 = ({ TrStr _tr_cr = (Formatter_expr_str(self, a)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2600;
    }
#line 334 "src/fmt.tr"
    TrStr _strtmp_t2602 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t2602;
#line 335 "src/fmt.tr"
    if ((((unsigned long long)(b)) != ((unsigned long long)(0LL)))) {
#line 335 "src/fmt.tr"
        TrStr _strtmp_t2604 = ({ TrStr _tr_cr = (Formatter_expr_str(self, b)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2604;
    }
#line 336 "src/fmt.tr"
    if ((((unsigned long long)(c)) != ((unsigned long long)(0LL)))) {
#line 336 "src/fmt.tr"
        TrStr _strtmp_t2607 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2607;
    }
#line 337 "src/fmt.tr"
    TrStr _trret532 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret532;
}

__attribute__((hot)) TrStr Formatter_closure_str(Formatter* self, List_ptr* params, AstType** ret_ty, Block* body, bool is_async) {
#line 340 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 341 "src/fmt.tr"
    if (is_async) {
#line 341 "src/fmt.tr"
        TrStr _strtmp_t2608 = _tr_str_lit_len("async ", 6LL);
        _tr_str_release(s);
        s = _strtmp_t2608;
    }
#line 342 "src/fmt.tr"
    TrStr _strtmp_t2612 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("def (", 5LL)))); TrStr _tr_cr = (Formatter_params_str(self, params)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2612;
#line 343 "src/fmt.tr"
    if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
#line 344 "src/fmt.tr"
        TrStr rt = Formatter_type_str(self, ret_ty);
#line 345 "src/fmt.tr"
        if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 345 "src/fmt.tr"
            TrStr _strtmp_t2615 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2615;
        }
    }
#line 346 "src/fmt.tr"
    TrStr _strtmp_t2617 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    s = _strtmp_t2617;
#line 351 "src/fmt.tr"
    Stmt* real = (Stmt*)(0LL);
#line 352 "src/fmt.tr"
    long long _tr_v_count = 0LL;
#line 353 "src/fmt.tr"
    long long i = 0LL;
#line 354 "src/fmt.tr"
    while ((i < body->stmts->len)) {
#line 355 "src/fmt.tr"
        __auto_type _t2618 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t2618.tag == Stmt_SLine) {
            __auto_type _ = _t2618.data.SLine.n;
            /* pass */
        } else if (1) {
            __auto_type _ = _t2618;
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
        __auto_type _t2619 = (*real);
        if (_t2619.tag == Stmt_SReturn) {
            __auto_type v = _t2619.data.SReturn.val;
#line 364 "src/fmt.tr"
            if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 364 "src/fmt.tr"
                TrStr _trret533 = _tr_strx_concatv((s), (_tr_str_lit_len(" return", 7LL)));
                _tr_str_release(s);
                return _trret533;
            }
#line 365 "src/fmt.tr"
            TrStr _trret534 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" return ", 8LL)))); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            return _trret534;
        } else if (_t2619.tag == Stmt_SExpr) {
            __auto_type e = _t2619.data.SExpr.expr;
            TrStr _trret535 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)))); TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            return _trret535;
        } else if (1) {
            __auto_type _ = _t2619;
#line 368 "src/fmt.tr"
            self->unsupported = true;
#line 369 "src/fmt.tr"
            TrStr _trret536 = _tr_str_retain(s);
            _tr_str_release(s);
            return _trret536;
        }
    }
#line 370 "src/fmt.tr"
    self->unsupported = true;
#line 371 "src/fmt.tr"
    TrStr _trret537 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret537;
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
            TrStr _strtmp_t2621 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t2621;
        }
#line 379 "src/fmt.tr"
        Param* p = ((Param*)List_ptr_get(params, i));
#line 383 "src/fmt.tr"
        ({ TrStr _ap_t2622 = (p->name); _tr_str_append(&s, _ap_t2622); });
#line 384 "src/fmt.tr"
        TrStr tn = Formatter_type_str(self, p->ty);
#line 385 "src/fmt.tr"
        if (((_tr_str_lenv((tn)) > 0LL) && (!_tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL)))))) {
#line 386 "src/fmt.tr"
            TrStr _strtmp_t2625 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tn)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2625;
        }
#line 387 "src/fmt.tr"
        if (p->is_variadic) {
#line 387 "src/fmt.tr"
            TrStr _strtmp_t2627 = _tr_strx_concatv((s), (_tr_str_lit_len("...", 3LL)));
            _tr_str_release(s);
            s = _strtmp_t2627;
        }
#line 388 "src/fmt.tr"
        i = (i + 1LL);
        _tr_str_release(tn);
    }
#line 389 "src/fmt.tr"
    TrStr _trret538 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret538;
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
            ({ __auto_type _ord_t2628 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (d->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2629 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2628); TrStr _tr_cr = (Formatter_args_str(self, d->args)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2629); _tr_str_release(_at_t2629); });
        } else {
#line 398 "src/fmt.tr"
            ({ TrStr _at_t2630 = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (d->name))); Formatter_line(self, _at_t2630); _tr_str_release(_at_t2630); });
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
        __auto_type _t2631 = (*sp);
        if (_t2631.tag == Stmt_SLine) {
            __auto_type n = _t2631.data.SLine.n;
#line 410 "src/fmt.tr"
            Formatter_flush_standalone_before(self, n);
#line 411 "src/fmt.tr"
            cur_line = n;
        } else if (1) {
            __auto_type _ = _t2631;
#line 413 "src/fmt.tr"
            Formatter_emit_stmt(self, sp, cur_line);
        }
#line 414 "src/fmt.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Formatter_emit_simple(Formatter* self, TrStr text, long long src_line) {
#line 418 "src/fmt.tr"
    ({ TrStr _ord_t2632 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2632); _tr_str_release(_ord_t2632); });
#line 419 "src/fmt.tr"
    StringBuilder_append(self->out, text);
#line 420 "src/fmt.tr"
    ({ TrStr _ord_t2633 = (Formatter_trailing_for(self, src_line)); StringBuilder_append(self->out, _ord_t2633); _tr_str_release(_ord_t2633); });
#line 421 "src/fmt.tr"
    StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
}

__attribute__((hot)) void Formatter_emit_stmt(Formatter* self, Stmt* sp, long long src_line) {
#line 424 "src/fmt.tr"
    __auto_type _t2634 = (*sp);
    if (_t2634.tag == Stmt_SExpr) {
        __auto_type e = _t2634.data.SExpr.expr;
        ({ TrStr _at_t2635 = (Formatter_expr_str(self, e)); Formatter_emit_simple(self, _at_t2635, src_line); _tr_str_release(_at_t2635); });
    } else if (_t2634.tag == Stmt_SLet) {
        __auto_type name = _t2634.data.SLet.name;
__auto_type own = _t2634.data.SLet.ownership;
__auto_type is_mut = _t2634.data.SLet.is_mut;
__auto_type is_const = _t2634.data.SLet.is_const;
__auto_type is_shared = _t2634.data.SLet.is_shared;
__auto_type ty = _t2634.data.SLet.ty;
__auto_type val = _t2634.data.SLet.val;
#line 427 "src/fmt.tr"
        ({ TrStr _at_t2636 = (Formatter_let_str(self, name, own, is_mut, is_const, is_shared, ty, val)); Formatter_emit_simple(self, _at_t2636, src_line); _tr_str_release(_at_t2636); });
    } else if (_t2634.tag == Stmt_SMultiLet) {
        __auto_type names = _t2634.data.SMultiLet.names;
__auto_type is_mut = _t2634.data.SMultiLet.is_mut;
__auto_type val = _t2634.data.SMultiLet.val;
#line 429 "src/fmt.tr"
        TrStr pfx = _tr_str_lit_len("", 0LL);
#line 430 "src/fmt.tr"
        if (is_mut) {
#line 430 "src/fmt.tr"
            TrStr _strtmp_t2637 = _tr_str_lit_len("mut ", 4LL);
            _tr_str_release(pfx);
            pfx = _strtmp_t2637;
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
                ({ TrStr _ap_t2638 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ns, _ap_t2638); });
            }
#line 435 "src/fmt.tr"
            ({ TrStr _ap_t2639 = (List_TrStr_get(names, k)); _tr_str_append(&ns, _ap_t2639); _tr_str_release(_ap_t2639); });
#line 436 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 437 "src/fmt.tr"
        ({ TrStr _at_t2640 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((pfx), (ns))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, val)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2640, src_line); _tr_str_release(_at_t2640); });
        _tr_str_release(pfx);
        _tr_str_release(ns);
    } else if (_t2634.tag == Stmt_SAssign) {
        __auto_type t = _t2634.data.SAssign.target;
__auto_type v = _t2634.data.SAssign.val;
#line 439 "src/fmt.tr"
        ({ __auto_type _ord_t2641 = (({ TrStr _tr_cl = (Formatter_expr_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2642 = (({ TrStr _tr_cl = (_ord_t2641); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2642, src_line); _tr_str_release(_at_t2642); });
    } else if (_t2634.tag == Stmt_SReturn) {
        __auto_type v = _t2634.data.SReturn.val;
#line 441 "src/fmt.tr"
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 441 "src/fmt.tr"
            Formatter_emit_simple(self, _tr_str_lit_len("return", 6LL), src_line);
        } else {
#line 442 "src/fmt.tr"
            ({ TrStr _at_t2643 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2643, src_line); _tr_str_release(_at_t2643); });
        }
    } else if (_t2634.tag == Stmt_SBreak) {
        __auto_type bv = _t2634.data.SBreak.val;
#line 444 "src/fmt.tr"
        if ((((unsigned long long)(bv)) == ((unsigned long long)(0LL)))) {
#line 444 "src/fmt.tr"
            Formatter_emit_simple(self, _tr_str_lit_len("break", 5LL), src_line);
        } else {
#line 445 "src/fmt.tr"
            ({ TrStr _at_t2644 = (({ TrStr _tr_cr = (Formatter_expr_str(self, bv)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("break ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2644, src_line); _tr_str_release(_at_t2644); });
        }
    } else if (_t2634.tag == Stmt_SContinue) {
        Formatter_emit_simple(self, _tr_str_lit_len("continue", 8LL), src_line);
    } else if (_t2634.tag == Stmt_SPass) {
        Formatter_emit_simple(self, _tr_str_lit_len("pass", 4LL), src_line);
    } else if (_t2634.tag == Stmt_SRaise) {
        __auto_type v = _t2634.data.SRaise.val;
        ({ TrStr _at_t2645 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("raise ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2645, src_line); _tr_str_release(_at_t2645); });
    } else if (_t2634.tag == Stmt_SAssert) {
        __auto_type c = _t2634.data.SAssert.cond;
__auto_type m = _t2634.data.SAssert.msg;
#line 450 "src/fmt.tr"
        if ((((unsigned long long)(m)) == ((unsigned long long)(0LL)))) {
#line 450 "src/fmt.tr"
            ({ TrStr _at_t2646 = (({ TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("assert ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2646, src_line); _tr_str_release(_at_t2646); });
        } else {
#line 451 "src/fmt.tr"
            ({ __auto_type _ord_t2647 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, c)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("assert ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2648 = (({ TrStr _tr_cl = (_ord_t2647); TrStr _tr_cr = (Formatter_expr_str(self, m)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2648, src_line); _tr_str_release(_at_t2648); });
        }
    } else if (_t2634.tag == Stmt_SUnsafe) {
        __auto_type body = _t2634.data.SUnsafe.body;
#line 453 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("unsafe:", 7LL), src_line);
#line 454 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 455 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 456 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2634.tag == Stmt_SIf) {
        __auto_type cond = _t2634.data.SIf.cond;
__auto_type then_b = _t2634.data.SIf.then_b;
__auto_type elifs = _t2634.data.SIf.elifs;
__auto_type else_b = _t2634.data.SIf.else_b;
#line 458 "src/fmt.tr"
        ({ TrStr _at_t2649 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("if ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2649, src_line); _tr_str_release(_at_t2649); });
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
            ({ TrStr _at_t2650 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, ec->cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("elif ", 5LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2650); _tr_str_release(_at_t2650); });
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
    } else if (_t2634.tag == Stmt_SWhile) {
        __auto_type cond = _t2634.data.SWhile.cond;
__auto_type body = _t2634.data.SWhile.body;
__auto_type decs = _t2634.data.SWhile.decorators;
#line 476 "src/fmt.tr"
        ({ TrStr _at_t2651 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, cond)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("while ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2651, src_line); _tr_str_release(_at_t2651); });
#line 477 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 478 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 479 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2634.tag == Stmt_SFor) {
        __auto_type var = _t2634.data.SFor.var;
__auto_type it = _t2634.data.SFor.iter;
__auto_type body = _t2634.data.SFor.body;
__auto_type decs = _t2634.data.SFor.decorators;
#line 481 "src/fmt.tr"
        ({ TrStr _at_t2652 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("for ", 4LL)), (var))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, it)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2652, src_line); _tr_str_release(_at_t2652); });
#line 482 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 483 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 484 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2634.tag == Stmt_SForUnpack) {
        __auto_type vars = _t2634.data.SForUnpack.vars;
__auto_type it = _t2634.data.SForUnpack.iter;
__auto_type body = _t2634.data.SForUnpack.body;
#line 486 "src/fmt.tr"
        TrStr vs = _tr_str_lit_len("", 0LL);
#line 487 "src/fmt.tr"
        long long k = 0LL;
#line 488 "src/fmt.tr"
        while ((k < vars->len)) {
#line 489 "src/fmt.tr"
            if ((k > 0LL)) {
#line 489 "src/fmt.tr"
                ({ TrStr _ap_t2653 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&vs, _ap_t2653); });
            }
#line 490 "src/fmt.tr"
            ({ TrStr _ap_t2654 = (List_TrStr_get(vars, k)); _tr_str_append(&vs, _ap_t2654); _tr_str_release(_ap_t2654); });
#line 491 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 492 "src/fmt.tr"
        ({ TrStr _at_t2655 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("for ", 4LL)), (vs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_expr_str(self, it)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2655, src_line); _tr_str_release(_at_t2655); });
#line 493 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 494 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 495 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
        _tr_str_release(vs);
    } else if (_t2634.tag == Stmt_SMatch) {
        __auto_type e = _t2634.data.SMatch.expr;
__auto_type arms = _t2634.data.SMatch.arms;
#line 497 "src/fmt.tr"
        ({ TrStr _at_t2656 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("match ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_emit_simple(self, _at_t2656, src_line); _tr_str_release(_at_t2656); });
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
                ({ TrStr _ap_t2657 = (_tr_str_lit_len(" if ", 4LL)); TrStr _ap_t2658 = (Formatter_expr_str(self, arm->guard)); _tr_str_append(&head, _ap_t2657); _tr_str_append(&head, _ap_t2658); _tr_str_release(_ap_t2658); });
            }
#line 505 "src/fmt.tr"
            ({ TrStr _at_t2659 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2659); _tr_str_release(_at_t2659); });
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
    } else if (_t2634.tag == Stmt_STry) {
        __auto_type try_b = _t2634.data.STry.try_body;
__auto_type catches = _t2634.data.STry.catches;
__auto_type finally_b = _t2634.data.STry.finally_b;
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
                    ({ TrStr _ap_t2660 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t2661 = (et); _tr_str_append(&head, _ap_t2660); _tr_str_append(&head, _ap_t2661); });
                }
            }
#line 524 "src/fmt.tr"
            if ((_tr_str_lenv((cc->err_name)) > 0LL)) {
#line 525 "src/fmt.tr"
                ({ TrStr _ap_t2662 = (_tr_str_lit_len(" as ", 4LL)); TrStr _ap_t2663 = (cc->err_name); _tr_str_append(&head, _ap_t2662); _tr_str_append(&head, _ap_t2663); });
            }
#line 526 "src/fmt.tr"
            ({ TrStr _at_t2664 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2664); _tr_str_release(_at_t2664); });
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
    } else if (_t2634.tag == Stmt_SWith) {
        __auto_type items = _t2634.data.SWith.items;
__auto_type aliases = _t2634.data.SWith.aliases;
__auto_type body = _t2634.data.SWith.body;
#line 537 "src/fmt.tr"
        TrStr hs = _tr_str_lit_len("with ", 5LL);
#line 538 "src/fmt.tr"
        long long k = 0LL;
#line 539 "src/fmt.tr"
        while ((k < items->len)) {
#line 540 "src/fmt.tr"
            if ((k > 0LL)) {
#line 540 "src/fmt.tr"
                ({ TrStr _ap_t2665 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&hs, _ap_t2665); });
            }
#line 541 "src/fmt.tr"
            ({ TrStr _ap_t2666 = (Formatter_expr_str(self, ((Expr*)List_ptr_get(items, k)))); _tr_str_append(&hs, _ap_t2666); _tr_str_release(_ap_t2666); });
#line 542 "src/fmt.tr"
            if (((k < aliases->len) && (_tr_str_lenv((List_TrStr_get(aliases, k))) > 0LL))) {
#line 543 "src/fmt.tr"
                ({ TrStr _ap_t2667 = (_tr_str_lit_len(" as ", 4LL)); TrStr _ap_t2668 = (List_TrStr_get(aliases, k)); _tr_str_append(&hs, _ap_t2667); _tr_str_append(&hs, _ap_t2668); _tr_str_release(_ap_t2668); });
            }
#line 544 "src/fmt.tr"
            k = (k + 1LL);
        }
#line 545 "src/fmt.tr"
        ({ TrStr _at_t2669 = (_tr_strx_concatv((hs), (_tr_str_lit_len(":", 1LL)))); Formatter_emit_simple(self, _at_t2669, src_line); _tr_str_release(_at_t2669); });
#line 546 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 547 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 548 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
        _tr_str_release(hs);
    } else if (_t2634.tag == Stmt_SSpawn) {
        __auto_type e = _t2634.data.SSpawn.expr;
        ({ TrStr _at_t2670 = (({ TrStr _tr_cr = (Formatter_expr_str(self, e)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("spawn ", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_emit_simple(self, _at_t2670, src_line); _tr_str_release(_at_t2670); });
    } else if (_t2634.tag == Stmt_STaskGroup) {
        __auto_type body = _t2634.data.STaskGroup.body;
#line 551 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("task_group:", 11LL), src_line);
#line 552 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 553 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 554 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2634.tag == Stmt_SDefer) {
        __auto_type inner = _t2634.data.SDefer.stmt;
#line 556 "src/fmt.tr"
        ({ TrStr _ord_t2671 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2671); _tr_str_release(_ord_t2671); });
#line 557 "src/fmt.tr"
        StringBuilder_append(self->out, _tr_str_lit_len("defer ", 6LL));
#line 559 "src/fmt.tr"
        Formatter_emit_inline_stmt(self, inner);
#line 560 "src/fmt.tr"
        StringBuilder_append(self->out, _tr_str_lit_len("\n", 1LL));
    } else if (_t2634.tag == Stmt_SGpuBlock) {
        __auto_type body = _t2634.data.SGpuBlock.body;
#line 562 "src/fmt.tr"
        Formatter_emit_simple(self, _tr_str_lit_len("gpu:", 4LL), src_line);
#line 563 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 564 "src/fmt.tr"
        Formatter_emit_block(self, body);
#line 565 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2634.tag == Stmt_SLocalDecl) {
        __auto_type d = _t2634.data.SLocalDecl.decl;
        Formatter_emit_decl(self, d);
    } else if (1) {
        __auto_type _ = _t2634;
#line 570 "src/fmt.tr"
        self->unsupported = true;
    }
}

__attribute__((hot)) void Formatter_emit_inline_stmt(Formatter* self, Stmt* sp) {
#line 575 "src/fmt.tr"
    __auto_type _t2672 = (*sp);
    if (_t2672.tag == Stmt_SExpr) {
        __auto_type e = _t2672.data.SExpr.expr;
        ({ TrStr _ord_t2673 = (Formatter_expr_str(self, e)); StringBuilder_append(self->out, _ord_t2673); _tr_str_release(_ord_t2673); });
    } else if (_t2672.tag == Stmt_SReturn) {
        __auto_type v = _t2672.data.SReturn.val;
#line 578 "src/fmt.tr"
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
#line 578 "src/fmt.tr"
            StringBuilder_append(self->out, _tr_str_lit_len("return", 6LL));
        } else {
#line 579 "src/fmt.tr"
            ({ TrStr _ord_t2674 = (({ TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("return ", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(self->out, _ord_t2674); _tr_str_release(_ord_t2674); });
        }
    } else if (_t2672.tag == Stmt_SAssign) {
        __auto_type t = _t2672.data.SAssign.target;
__auto_type v = _t2672.data.SAssign.val;
        ({ __auto_type _ord_t2675 = (({ TrStr _tr_cl = (Formatter_expr_str(self, t)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t2676 = (({ TrStr _tr_cl = (_ord_t2675); TrStr _tr_cr = (Formatter_expr_str(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(self->out, _ord_t2676); _tr_str_release(_ord_t2676); });
    } else if (1) {
        __auto_type _ = _t2672;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_let_str(Formatter* self, TrStr name, Ownership own, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) {
#line 586 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 587 "src/fmt.tr"
    if (is_const) {
#line 587 "src/fmt.tr"
        TrStr _strtmp_t2677 = _tr_str_lit_len("const ", 6LL);
        _tr_str_release(s);
        s = _strtmp_t2677;
    } else if (is_shared) {
#line 588 "src/fmt.tr"
        TrStr _strtmp_t2678 = _tr_str_lit_len("shared ", 7LL);
        _tr_str_release(s);
        s = _strtmp_t2678;
    } else if (is_mut) {
#line 589 "src/fmt.tr"
        TrStr _strtmp_t2679 = _tr_str_lit_len("mut ", 4LL);
        _tr_str_release(s);
        s = _strtmp_t2679;
    }
#line 590 "src/fmt.tr"
    ({ TrStr _ap_t2680 = (name); _tr_str_append(&s, _ap_t2680); });
#line 591 "src/fmt.tr"
    TrStr tn = Formatter_type_str(self, ty);
#line 592 "src/fmt.tr"
    if (((_tr_str_lenv((tn)) > 0LL) && (!_tr_str_eqv((tn), (_tr_str_lit_len("void", 4LL)))))) {
#line 593 "src/fmt.tr"
        TrStr _strtmp_t2683 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(": ", 2LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tn)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2683;
    }
#line 594 "src/fmt.tr"
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 595 "src/fmt.tr"
        TrStr _strtmp_t2686 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" = ", 3LL)))); TrStr _tr_cr = (Formatter_expr_str(self, val)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t2686;
    }
#line 596 "src/fmt.tr"
    TrStr _trret539 = _tr_str_retain(s);
    _tr_str_release(s);
    _tr_str_release(tn);
    return _trret539;
}

__attribute__((hot)) TrStr Formatter_pattern_str(Formatter* self, Pattern p) {
#line 599 "src/fmt.tr"
    __auto_type _t2687 = p;
    if (_t2687.tag == Pattern_PWild) {
        return _tr_str_lit_len("_", 1LL);
    } else if (_t2687.tag == Pattern_PBind) {
        __auto_type n = _t2687.data.PBind.name;
        TrStr _trret540 = _tr_str_retain(n);
        return _trret540;
    } else if (_t2687.tag == Pattern_PLitInt) {
        __auto_type v = _t2687.data.PLitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t2687.tag == Pattern_PLitStr) {
        __auto_type v = _t2687.data.PLitStr.val;
        TrStr _trret541 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (Formatter_escape_str(self, v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("\"", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret541;
    } else if (_t2687.tag == Pattern_PLitBool) {
        __auto_type b = _t2687.data.PLitBool.val;
#line 605 "src/fmt.tr"
        if (b) {
#line 605 "src/fmt.tr"
            return _tr_str_lit_len("True", 4LL);
        }
#line 606 "src/fmt.tr"
        return _tr_str_lit_len("False", 5LL);
    } else if (_t2687.tag == Pattern_PVariant) {
        __auto_type tn = _t2687.data.PVariant.type_name;
__auto_type vn = _t2687.data.PVariant.variant;
        TrStr _trret542 = ({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret542;
    } else if (_t2687.tag == Pattern_PVariantBind) {
        __auto_type tn = _t2687.data.PVariantBind.type_name;
__auto_type vn = _t2687.data.PVariantBind.variant;
__auto_type f = _t2687.data.PVariantBind.field;
        TrStr _trret543 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret543;
    } else if (_t2687.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t2687.data.PVariantBindMany.type_name;
__auto_type vn = _t2687.data.PVariantBindMany.variant;
__auto_type fs = _t2687.data.PVariantBindMany.fields;
#line 610 "src/fmt.tr"
        TrStr s = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 611 "src/fmt.tr"
        long long i = 0LL;
#line 612 "src/fmt.tr"
        while ((i < fs->len)) {
#line 613 "src/fmt.tr"
            if ((i > 0LL)) {
#line 613 "src/fmt.tr"
                TrStr _strtmp_t2689 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2689;
            }
#line 614 "src/fmt.tr"
            TrStr _strtmp_t2691 = ({ TrStr _tr_cr = (List_TrStr_get(fs, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2691;
#line 615 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 616 "src/fmt.tr"
        TrStr _trret544 = _tr_strx_concatv((s), (_tr_str_lit_len(")", 1LL)));
        _tr_str_release(s);
        return _trret544;
    } else if (_t2687.tag == Pattern_PTuple) {
        __auto_type a = _t2687.data.PTuple.first;
__auto_type b = _t2687.data.PTuple.second;
        TrStr _trret545 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("(", 1LL)), (a))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (b)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret545;
    } else if (_t2687.tag == Pattern_POr) {
        __auto_type pats = _t2687.data.POr.patterns;
#line 619 "src/fmt.tr"
        TrStr s = _tr_str_lit_len("", 0LL);
#line 620 "src/fmt.tr"
        long long i = 0LL;
#line 621 "src/fmt.tr"
        while ((i < pats->len)) {
#line 622 "src/fmt.tr"
            if ((i > 0LL)) {
#line 622 "src/fmt.tr"
                TrStr _strtmp_t2693 = _tr_strx_concatv((s), (_tr_str_lit_len(" | ", 3LL)));
                _tr_str_release(s);
                s = _strtmp_t2693;
            }
#line 623 "src/fmt.tr"
            TrStr _strtmp_t2695 = ({ TrStr _tr_cr = (Formatter_pattern_str(self, List_Pattern_get(pats, i))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2695;
#line 624 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 625 "src/fmt.tr"
        TrStr _trret546 = _tr_str_retain(s);
        _tr_str_release(s);
        return _trret546;
    } else if (1) {
        __auto_type _ = _t2687;
        return _tr_str_lit_len("_", 1LL);
    }
}

__attribute__((hot)) TrStr Formatter_fn_header(Formatter* self, FunctionDef* f) {
#line 630 "src/fmt.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 631 "src/fmt.tr"
    if (f->is_public) {
#line 631 "src/fmt.tr"
        TrStr _strtmp_t2696 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(s);
        s = _strtmp_t2696;
    }
#line 632 "src/fmt.tr"
    if (f->is_async) {
#line 632 "src/fmt.tr"
        TrStr _strtmp_t2698 = _tr_strx_concatv((s), (_tr_str_lit_len("async ", 6LL)));
        _tr_str_release(s);
        s = _strtmp_t2698;
    }
#line 633 "src/fmt.tr"
    TrStr _strtmp_t2701 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("def ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2701;
#line 634 "src/fmt.tr"
    if ((f->generics->len > 0LL)) {
#line 635 "src/fmt.tr"
        TrStr _strtmp_t2703 = _tr_strx_concatv((s), (_tr_str_lit_len("[", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2703;
#line 636 "src/fmt.tr"
        long long gi = 0LL;
#line 637 "src/fmt.tr"
        while ((gi < f->generics->len)) {
#line 638 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 638 "src/fmt.tr"
                TrStr _strtmp_t2705 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2705;
            }
#line 639 "src/fmt.tr"
            TrStr _strtmp_t2707 = ({ TrStr _tr_cr = (List_TrStr_get(f->generics, gi)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2707;
#line 640 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 641 "src/fmt.tr"
        TrStr _strtmp_t2709 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t2709;
    }
#line 642 "src/fmt.tr"
    TrStr _strtmp_t2713 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len("(", 1LL)))); TrStr _tr_cr = (Formatter_params_str(self, f->params)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(s);
    s = _strtmp_t2713;
#line 645 "src/fmt.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 646 "src/fmt.tr"
        TrStr tt = Formatter_type_str(self, f->throws_ty);
#line 647 "src/fmt.tr"
        if ((_tr_str_lenv((tt)) > 0LL)) {
#line 647 "src/fmt.tr"
            TrStr _strtmp_t2716 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" throws ", 8LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2716;
        }
    }
#line 648 "src/fmt.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 649 "src/fmt.tr"
        TrStr rt = Formatter_type_str(self, f->ret_ty);
#line 650 "src/fmt.tr"
        if (((_tr_str_lenv((rt)) > 0LL) && (!_tr_str_eqv((rt), (_tr_str_lit_len("void", 4LL)))))) {
#line 650 "src/fmt.tr"
            TrStr _strtmp_t2719 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" -> ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rt)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t2719;
        }
    }
#line 651 "src/fmt.tr"
    TrStr _trret547 = _tr_strx_concatv((s), (_tr_str_lit_len(":", 1LL)));
    _tr_str_release(s);
    return _trret547;
}

__attribute__((hot)) void Formatter_emit_function(Formatter* self, FunctionDef* f) {
#line 654 "src/fmt.tr"
    Formatter_flush_standalone_before(self, f->line);
#line 655 "src/fmt.tr"
    Formatter_emit_decorators(self, f->decorators);
#line 656 "src/fmt.tr"
    ({ TrStr _ord_t2720 = (Formatter_pad(self)); StringBuilder_append(self->out, _ord_t2720); _tr_str_release(_ord_t2720); });
#line 657 "src/fmt.tr"
    ({ TrStr _ord_t2721 = (Formatter_fn_header(self, f)); StringBuilder_append(self->out, _ord_t2721); _tr_str_release(_ord_t2721); });
#line 658 "src/fmt.tr"
    ({ TrStr _ord_t2722 = (Formatter_trailing_for(self, f->line)); StringBuilder_append(self->out, _ord_t2722); _tr_str_release(_ord_t2722); });
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
        TrStr _strtmp_t2723 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t2723;
    }
#line 672 "src/fmt.tr"
    ({ TrStr _ap_t2724 = (kw); TrStr _ap_t2725 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t2726 = (c->name); _tr_str_append(&head, _ap_t2724); _tr_str_append(&head, _ap_t2725); _tr_str_append(&head, _ap_t2726); });
#line 673 "src/fmt.tr"
    if ((c->generics->len > 0LL)) {
#line 674 "src/fmt.tr"
        ({ TrStr _ap_t2727 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t2727); });
#line 675 "src/fmt.tr"
        long long gi = 0LL;
#line 676 "src/fmt.tr"
        while ((gi < c->generics->len)) {
#line 677 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 677 "src/fmt.tr"
                ({ TrStr _ap_t2728 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t2728); });
            }
#line 678 "src/fmt.tr"
            ({ TrStr _ap_t2729 = (List_TrStr_get(c->generics, gi)); _tr_str_append(&head, _ap_t2729); _tr_str_release(_ap_t2729); });
#line 679 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 680 "src/fmt.tr"
        ({ TrStr _ap_t2730 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t2730); });
    }
#line 681 "src/fmt.tr"
    if ((c->base_classes->len > 0LL)) {
#line 682 "src/fmt.tr"
        ({ TrStr _ap_t2731 = (_tr_str_lit_len(" extends ", 9LL)); TrStr _ap_t2732 = (List_TrStr_get(c->base_classes, 0LL)); _tr_str_append(&head, _ap_t2731); _tr_str_append(&head, _ap_t2732); _tr_str_release(_ap_t2732); });
#line 683 "src/fmt.tr"
        long long bi = 1LL;
#line 684 "src/fmt.tr"
        while ((bi < c->base_classes->len)) {
#line 685 "src/fmt.tr"
            ({ TrStr _ap_t2733 = (_tr_str_lit_len(", ", 2LL)); TrStr _ap_t2734 = (List_TrStr_get(c->base_classes, bi)); _tr_str_append(&head, _ap_t2733); _tr_str_append(&head, _ap_t2734); _tr_str_release(_ap_t2734); });
#line 686 "src/fmt.tr"
            bi = (bi + 1LL);
        }
    }
#line 687 "src/fmt.tr"
    if ((c->iface_names->len > 0LL)) {
#line 688 "src/fmt.tr"
        ({ TrStr _ap_t2735 = (_tr_str_lit_len(" implements ", 12LL)); _tr_str_append(&head, _ap_t2735); });
#line 689 "src/fmt.tr"
        long long ii = 0LL;
#line 690 "src/fmt.tr"
        while ((ii < c->iface_names->len)) {
#line 691 "src/fmt.tr"
            if ((ii > 0LL)) {
#line 691 "src/fmt.tr"
                ({ TrStr _ap_t2736 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t2736); });
            }
#line 692 "src/fmt.tr"
            ({ TrStr _ap_t2737 = (List_TrStr_get(c->iface_names, ii)); _tr_str_append(&head, _ap_t2737); _tr_str_release(_ap_t2737); });
#line 693 "src/fmt.tr"
            ii = (ii + 1LL);
        }
    }
#line 694 "src/fmt.tr"
    ({ TrStr _at_t2738 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2738); _tr_str_release(_at_t2738); });
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
        TrStr fs = ({ __auto_type _ord_t2739 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("pub ", 4LL)), (fld->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t2739); TrStr _tr_cr = (Formatter_type_str(self, fld->ty)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
#line 701 "src/fmt.tr"
        if ((((unsigned long long)(fld->default_val)) != ((unsigned long long)(0LL)))) {
#line 702 "src/fmt.tr"
            ({ TrStr _ap_t2740 = (_tr_str_lit_len(" = ", 3LL)); TrStr _ap_t2741 = (Formatter_expr_str(self, fld->default_val)); _tr_str_append(&fs, _ap_t2740); _tr_str_append(&fs, _ap_t2741); _tr_str_release(_ap_t2741); });
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
        TrStr _strtmp_t2742 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t2742;
    }
#line 720 "src/fmt.tr"
    ({ TrStr _ap_t2743 = (_tr_str_lit_len("enum ", 5LL)); TrStr _ap_t2744 = (e->name); _tr_str_append(&head, _ap_t2743); _tr_str_append(&head, _ap_t2744); });
#line 721 "src/fmt.tr"
    if ((e->generics->len > 0LL)) {
#line 722 "src/fmt.tr"
        ({ TrStr _ap_t2745 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t2745); });
#line 723 "src/fmt.tr"
        long long gi = 0LL;
#line 724 "src/fmt.tr"
        while ((gi < e->generics->len)) {
#line 725 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 725 "src/fmt.tr"
                ({ TrStr _ap_t2746 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t2746); });
            }
#line 726 "src/fmt.tr"
            ({ TrStr _ap_t2747 = (List_TrStr_get(e->generics, gi)); _tr_str_append(&head, _ap_t2747); _tr_str_release(_ap_t2747); });
#line 727 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 728 "src/fmt.tr"
        ({ TrStr _ap_t2748 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t2748); });
    }
#line 729 "src/fmt.tr"
    ({ TrStr _at_t2749 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2749); _tr_str_release(_at_t2749); });
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
            ({ __auto_type _ord_t2750 = (_tr_strx_concatv((v->name), (_tr_str_lit_len("(", 1LL)))); TrStr _at_t2751 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2750); TrStr _tr_cr = (Formatter_params_str(self, v->fields)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2751); _tr_str_release(_at_t2751); });
        } else {
#line 737 "src/fmt.tr"
            Formatter_line(self, v->name);
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
        TrStr _strtmp_t2752 = _tr_str_lit_len("pub ", 4LL);
        _tr_str_release(head);
        head = _strtmp_t2752;
    }
#line 749 "src/fmt.tr"
    ({ TrStr _ap_t2753 = (_tr_str_lit_len("interface ", 10LL)); TrStr _ap_t2754 = (it->name); _tr_str_append(&head, _ap_t2753); _tr_str_append(&head, _ap_t2754); });
#line 750 "src/fmt.tr"
    if ((it->generics->len > 0LL)) {
#line 751 "src/fmt.tr"
        ({ TrStr _ap_t2755 = (_tr_str_lit_len("[", 1LL)); _tr_str_append(&head, _ap_t2755); });
#line 752 "src/fmt.tr"
        long long gi = 0LL;
#line 753 "src/fmt.tr"
        while ((gi < it->generics->len)) {
#line 754 "src/fmt.tr"
            if ((gi > 0LL)) {
#line 754 "src/fmt.tr"
                ({ TrStr _ap_t2756 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&head, _ap_t2756); });
            }
#line 755 "src/fmt.tr"
            ({ TrStr _ap_t2757 = (List_TrStr_get(it->generics, gi)); _tr_str_append(&head, _ap_t2757); _tr_str_release(_ap_t2757); });
#line 756 "src/fmt.tr"
            gi = (gi + 1LL);
        }
#line 757 "src/fmt.tr"
        ({ TrStr _ap_t2758 = (_tr_str_lit_len("]", 1LL)); _tr_str_append(&head, _ap_t2758); });
    }
#line 758 "src/fmt.tr"
    ({ TrStr _at_t2759 = (_tr_strx_concatv((head), (_tr_str_lit_len(":", 1LL)))); Formatter_line(self, _at_t2759); _tr_str_release(_at_t2759); });
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
    __auto_type _t2760 = (*dp);
    if (_t2760.tag == Decl_DFunction) {
        __auto_type f = _t2760.data.DFunction.func;
        Formatter_emit_function(self, f);
    } else if (_t2760.tag == Decl_DDecoratorDef) {
        __auto_type f = _t2760.data.DDecoratorDef.func;
#line 771 "src/fmt.tr"
        Formatter_flush_standalone_before(self, f->line);
#line 774 "src/fmt.tr"
        ({ TrStr _at_t2761 = (({ TrStr _tr_cr = (Formatter_fn_header(self, f)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("decorator ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_line(self, _at_t2761); _tr_str_release(_at_t2761); });
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
    } else if (_t2760.tag == Decl_DClass) {
        __auto_type c = _t2760.data.DClass.cls;
        Formatter_emit_class(self, c, _tr_str_lit_len("class", 5LL));
    } else if (_t2760.tag == Decl_DActor) {
        __auto_type c = _t2760.data.DActor.cls;
        Formatter_emit_class(self, c, _tr_str_lit_len("actor", 5LL));
    } else if (_t2760.tag == Decl_DEnum) {
        __auto_type e = _t2760.data.DEnum.enm;
        Formatter_emit_enum(self, e);
    } else if (_t2760.tag == Decl_DInterface) {
        __auto_type it = _t2760.data.DInterface.iface;
        Formatter_emit_interface(self, it);
    } else if (_t2760.tag == Decl_DExtend) {
        __auto_type target = _t2760.data.DExtend.target;
__auto_type methods = _t2760.data.DExtend.methods;
#line 784 "src/fmt.tr"
        ({ TrStr _at_t2762 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("extend ", 7LL)), (target))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2762); _tr_str_release(_at_t2762); });
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
    } else if (_t2760.tag == Decl_DImport) {
        __auto_type path = _t2760.data.DImport.path;
__auto_type alias = _t2760.data.DImport.alias;
#line 792 "src/fmt.tr"
        if ((_tr_str_lenv((alias)) > 0LL)) {
#line 792 "src/fmt.tr"
            ({ TrStr _at_t2763 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("import ", 7LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" as ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (alias)); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2763); _tr_str_release(_at_t2763); });
        } else {
#line 793 "src/fmt.tr"
            ({ TrStr _at_t2764 = (_tr_strx_concatv((_tr_str_lit_len("import ", 7LL)), (path))); Formatter_line(self, _at_t2764); _tr_str_release(_at_t2764); });
        }
    } else if (_t2760.tag == Decl_DFromImport) {
        __auto_type path = _t2760.data.DFromImport.path;
__auto_type items = _t2760.data.DFromImport.items;
#line 795 "src/fmt.tr"
        TrStr s = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("from ", 5LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" import ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 796 "src/fmt.tr"
        long long i = 0LL;
#line 797 "src/fmt.tr"
        while ((i < items->len)) {
#line 798 "src/fmt.tr"
            if ((i > 0LL)) {
#line 798 "src/fmt.tr"
                TrStr _strtmp_t2766 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t2766;
            }
#line 799 "src/fmt.tr"
            ImportItem* it = ((ImportItem*)List_ptr_get(items, i));
#line 800 "src/fmt.tr"
            ({ TrStr _ap_t2767 = (it->name); _tr_str_append(&s, _ap_t2767); });
#line 801 "src/fmt.tr"
            if ((_tr_str_lenv((it->alias)) > 0LL)) {
#line 801 "src/fmt.tr"
                TrStr _strtmp_t2770 = ({ TrStr _tr_cl = (_tr_strx_concatv((s), (_tr_str_lit_len(" as ", 4LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (it->alias)); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(s);
                s = _strtmp_t2770;
            }
#line 802 "src/fmt.tr"
            i = (i + 1LL);
        }
#line 803 "src/fmt.tr"
        Formatter_line(self, s);
        _tr_str_release(s);
    } else if (_t2760.tag == Decl_DExtern) {
        __auto_type abi = _t2760.data.DExtern.abi;
__auto_type functions = _t2760.data.DExtern.functions;
#line 805 "src/fmt.tr"
        ({ TrStr _at_t2771 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("extern \"", 8LL)), (abi))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\":", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); Formatter_line(self, _at_t2771); _tr_str_release(_at_t2771); });
#line 806 "src/fmt.tr"
        self->indent = (self->indent + 1LL);
#line 807 "src/fmt.tr"
        long long fi = 0LL;
#line 808 "src/fmt.tr"
        while ((fi < functions->len)) {
#line 809 "src/fmt.tr"
            ({ TrStr _at_t2772 = (Formatter_fn_header_no_colon(self, ((FunctionDef*)List_ptr_get(functions, fi)))); Formatter_line(self, _at_t2772); _tr_str_release(_at_t2772); });
#line 810 "src/fmt.tr"
            fi = (fi + 1LL);
        }
#line 811 "src/fmt.tr"
        self->indent = (self->indent - 1LL);
    } else if (_t2760.tag == Decl_DTypeAlias) {
        __auto_type name = _t2760.data.DTypeAlias.name;
__auto_type target = _t2760.data.DTypeAlias.target;
#line 813 "src/fmt.tr"
        ({ TrStr _at_t2773 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("type ", 5LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (Formatter_type_str(self, target)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); Formatter_line(self, _at_t2773); _tr_str_release(_at_t2773); });
    } else if (_t2760.tag == Decl_DTopLevelStmt) {
        __auto_type s = _t2760.data.DTopLevelStmt.stmt;
#line 815 "src/fmt.tr"
        Formatter_emit_stmt(self, s, 0LL);
    } else if (1) {
        __auto_type _ = _t2760;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_fn_header_no_colon(Formatter* self, FunctionDef* f) {
#line 820 "src/fmt.tr"
    TrStr h = Formatter_fn_header(self, f);
#line 821 "src/fmt.tr"
    if (((_tr_str_lenv((h)) > 0LL) && ({ __auto_type _ord_t2774 = ((_tr_str_lenv((h)) - 1LL)); __auto_type _ord_t2775 = (_tr_str_lenv((h))); __auto_type _wr = (({ TrStr _cmpa_t2776 = (_tr_str_slicev((h), _ord_t2774, _ord_t2775)); TrStr _cmpb_t2777 = (_tr_str_lit_len(":", 1LL)); _Bool _cmpr_t2778 = _tr_str_eqv(_cmpa_t2776, _cmpb_t2777); _tr_str_release(_cmpa_t2776); _cmpr_t2778; })); _wr; }))) {
#line 822 "src/fmt.tr"
        TrStr _trret548 = _tr_str_slicev((h), 0LL, (_tr_str_lenv((h)) - 1LL));
        _tr_str_release(h);
        return _trret548;
    }
#line 823 "src/fmt.tr"
    TrStr _trret549 = _tr_str_retain(h);
    _tr_str_release(h);
    return _trret549;
}

__attribute__((hot)) bool Formatter_is_block_decl(Formatter* self, Decl* dp) {
#line 828 "src/fmt.tr"
    __auto_type _t2779 = (*dp);
    if (_t2779.tag == Decl_DFunction) {
        __auto_type _ = _t2779.data.DFunction.func;
        return true;
    } else if (_t2779.tag == Decl_DDecoratorDef) {
        __auto_type _ = _t2779.data.DDecoratorDef.func;
        return true;
    } else if (_t2779.tag == Decl_DClass) {
        __auto_type _ = _t2779.data.DClass.cls;
        return true;
    } else if (_t2779.tag == Decl_DActor) {
        __auto_type _ = _t2779.data.DActor.cls;
        return true;
    } else if (_t2779.tag == Decl_DEnum) {
        __auto_type _ = _t2779.data.DEnum.enm;
        return true;
    } else if (_t2779.tag == Decl_DInterface) {
        __auto_type _ = _t2779.data.DInterface.iface;
        return true;
    } else if (_t2779.tag == Decl_DExtend) {
        return true;
    } else if (_t2779.tag == Decl_DExtern) {
        return true;
    } else if (1) {
        __auto_type _ = _t2779;
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

