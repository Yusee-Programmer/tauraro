#include "tauraro_types.h"

TrStr Parser__peek_cmp_op(Parser* self);

__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines) {
#line 104 "src/parser.tr"
    Parser* p = ((Parser*)_tr_obj_alloc(sizeof(Parser)));
#line 105 "src/parser.tr"
    p->tokens = tokens;
#line 106 "src/parser.tr"
    p->lines = lines;
#line 107 "src/parser.tr"
    p->cols = (void*)List_i64_new();
#line 108 "src/parser.tr"
    p->src_text = _tr_str_lit_len("", 0LL);
#line 109 "src/parser.tr"
    p->pos = 0LL;
#line 110 "src/parser.tr"
    p->error_count = 0LL;
#line 111 "src/parser.tr"
    p->aug_ctr = 0LL;
#line 112 "src/parser.tr"
    p->current_file = _tr_str_lit_len("", 0LL);
#line 113 "src/parser.tr"
    p->import_aliases = _tr_dict_new(8LL);
#line 114 "src/parser.tr"
    return p;
}

__attribute__((hot)) Token Parser_peek(Parser* self) {
#line 117 "src/parser.tr"
    if ((self->pos < self->tokens->len)) {
#line 118 "src/parser.tr"
        return List_Token_get(self->tokens, self->pos);
    }
#line 119 "src/parser.tr"
    return Token_make_Eof();
}

__attribute__((hot)) Token Parser_advance(Parser* self) {
#line 122 "src/parser.tr"
    Token tok = Parser_peek(self);
#line 123 "src/parser.tr"
    if ((self->pos < self->tokens->len)) {
#line 124 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 125 "src/parser.tr"
    return tok;
}

__attribute__((hot)) void Parser_skip_newlines(Parser* self) {
#line 128 "src/parser.tr"
    bool going = true;
#line 129 "src/parser.tr"
    while (going) {
#line 130 "src/parser.tr"
        going = false;
#line 131 "src/parser.tr"
        __auto_type _t33 = Parser_peek(self);
        if (_t33.tag == Token_Newline) {
#line 133 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 134 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t33;
#line 137 "src/parser.tr"
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self) {
#line 139 "src/parser.tr"
    bool going = true;
#line 140 "src/parser.tr"
    while (going) {
#line 141 "src/parser.tr"
        going = false;
#line 142 "src/parser.tr"
        __auto_type _t34 = Parser_peek(self);
        if ((_t34.tag == Token_Newline || _t34.tag == Token_Indent || _t34.tag == Token_Dedent)) {
#line 144 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 145 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t34;
#line 148 "src/parser.tr"
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_expect_newline(Parser* self) {
#line 150 "src/parser.tr"
    __auto_type _t35 = Parser_peek(self);
    if ((_t35.tag == Token_Newline || _t35.tag == Token_Semicolon)) {
#line 152 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if ((_t35.tag == Token_Eof || _t35.tag == Token_Dedent || _t35.tag == Token_Indent || _t35.tag == Token_RBrace || _t35.tag == Token_RParen || _t35.tag == Token_RBracket || _t35.tag == Token_Comma)) {
#line 154 "src/parser.tr"
        /* pass */
    } else if ((_t35.tag == Token_KwElse || _t35.tag == Token_KwElif || _t35.tag == Token_KwExcept || _t35.tag == Token_KwFinally || _t35.tag == Token_KwCase)) {
#line 156 "src/parser.tr"
        /* pass */
    } else if (_t35.tag == Token_Error) {
        __auto_type _ = _t35.data.Error.msg;
#line 158 "src/parser.tr"
        /* pass */
    } else if (1) {
        __auto_type _ = _t35;
#line 162 "src/parser.tr"
        if (((self->pos > 0LL) && (self->pos <= self->tokens->len))) {
#line 163 "src/parser.tr"
            __auto_type _t36 = List_Token_get(self->tokens, (self->pos - 1LL));
            if ((_t36.tag == Token_Newline || _t36.tag == Token_Dedent || _t36.tag == Token_Semicolon)) {
                return;
            } else if (1) {
                __auto_type _ = _t36;
                /* pass */
            }
        }
#line 169 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("unexpected token after the end of the statement", 47LL), _tr_str_lit_len("each statement goes on its own line (or separate them with ';') - check for a missing operator, comma or ')' before this token.", 127LL));
#line 170 "src/parser.tr"
        while (true) {
#line 171 "src/parser.tr"
            __auto_type _t37 = Parser_peek(self);
            if ((_t37.tag == Token_Newline || _t37.tag == Token_Eof || _t37.tag == Token_Dedent || _t37.tag == Token_Semicolon)) {
#line 173 "src/parser.tr"
                break;
            } else if (1) {
                __auto_type _ = _t37;
                self->pos = (self->pos + 1LL);
            }
        }
    }
}

__attribute__((hot)) void Parser_close_params(Parser* self) {
#line 179 "src/parser.tr"
    if (({ __auto_type _ord_t38 = (Parser_peek(self)); __auto_type _wr = ((_ord_t38.tag == Token_make_RParen().tag)); _wr; })) {
#line 180 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else {
#line 182 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected ')' to close the parameter list", 40LL), _tr_str_lit_len("check for a missing ')' or a stray character in the parameters (a parameter is written 'name: Type').", 101LL));
    }
}

__attribute__((hot)) void Parser_emit_diag_prev(Parser* self, TrStr msg, TrStr hint) {
#line 187 "src/parser.tr"
    if ((self->pos > 0LL)) {
#line 188 "src/parser.tr"
        self->pos = (self->pos - 1LL);
#line 189 "src/parser.tr"
        Parser_emit_diag(self, msg, hint);
#line 190 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else {
#line 192 "src/parser.tr"
        Parser_emit_diag(self, msg, hint);
    }
}

__attribute__((hot)) bool Parser_at_end(Parser* self) {
#line 195 "src/parser.tr"
    __auto_type _t39 = Parser_peek(self);
    if (_t39.tag == Token_Eof) {
#line 197 "src/parser.tr"
        return true;
    } else if (1) {
        __auto_type _ = _t39;
#line 199 "src/parser.tr"
        /* pass */
    }
#line 200 "src/parser.tr"
    return false;
}

__attribute__((hot)) long long Parser_cur_line(Parser* self) {
#line 203 "src/parser.tr"
    if ((self->pos < self->lines->len)) {
#line 204 "src/parser.tr"
        return List_i64_get(self->lines, self->pos);
    }
#line 205 "src/parser.tr"
    if ((self->lines->len > 0LL)) {
#line 206 "src/parser.tr"
        return List_i64_get(self->lines, (self->lines->len - 1LL));
    }
#line 207 "src/parser.tr"
    return 0LL;
}

__attribute__((hot)) long long Parser_cur_col(Parser* self) {
#line 212 "src/parser.tr"
    if ((self->pos < self->cols->len)) {
#line 213 "src/parser.tr"
        return List_i64_get(self->cols, self->pos);
    }
#line 214 "src/parser.tr"
    if ((self->cols->len > 0LL)) {
#line 215 "src/parser.tr"
        return List_i64_get(self->cols, (self->cols->len - 1LL));
    }
#line 216 "src/parser.tr"
    return 0LL;
}

__attribute__((hot)) void Parser_emit_diag(Parser* self, TrStr msg, TrStr hint) {
#line 227 "src/parser.tr"
    long long ln = Parser_cur_line(self);
#line 228 "src/parser.tr"
    long long col = Parser_cur_col(self);
#line 229 "src/parser.tr"
    ({ __auto_type _ord_t40 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t40); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
#line 230 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 231 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 232 "src/parser.tr"
        TrStr _strtmp_t41 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t41;
    }
#line 233 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 234 "src/parser.tr"
    if ((col > 0LL)) {
#line 235 "src/parser.tr"
        ({ TrStr _ap_t42 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t43 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t42); _tr_str_append(&head, _ap_t43); _tr_str_release(_ap_t43); });
    }
#line 236 "src/parser.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 238 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 239 "src/parser.tr"
        TrStr srcline = _nth_source_line(self->src_text, ln);
#line 240 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 241 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 242 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 243 "src/parser.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 244 "src/parser.tr"
            if ((col > 0LL)) {
#line 247 "src/parser.tr"
                long long width = 1LL;
#line 248 "src/parser.tr"
                if ((((self->pos + 1LL) < self->cols->len) && ((self->pos + 1LL) < self->lines->len))) {
#line 249 "src/parser.tr"
                    if ((List_i64_get(self->lines, (self->pos + 1LL)) == ln)) {
#line 250 "src/parser.tr"
                        long long d = (List_i64_get(self->cols, (self->pos + 1LL)) - col);
#line 251 "src/parser.tr"
                        if (((d > 0LL) && (d < 200LL))) {
#line 252 "src/parser.tr"
                            width = d;
                        }
                    }
                }
#line 253 "src/parser.tr"
                ({ __auto_type _ord_t44 = (c_dim(gbar)); __auto_type _ord_t46 = (({ TrStr _tr_cl = (_ord_t44); TrStr _tr_cr = (_spaces((col - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _at_t45 = (repeat_char(_tr_str_lit_len("^", 1LL), width)); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t46); TrStr _tr_cr = (c_red(_at_t45)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); _tr_str_release(_at_t45); });
            }
            _tr_str_release(gnum);
        }
    }
#line 254 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 255 "src/parser.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
    }
#line 256 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) TrStr Parser_consume_ident(Parser* self) {
#line 259 "src/parser.tr"
    __auto_type _t47 = Parser_peek(self);
    if (_t47.tag == Token_Ident) {
        __auto_type name = _t47.data.Ident.name;
#line 261 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 262 "src/parser.tr"
        TrStr _trret14 = _tr_str_retain(name);
        return _trret14;
    } else if (_t47.tag == Token_KwInt) {
#line 264 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 265 "src/parser.tr"
        return _tr_str_lit_len("int", 3LL);
    } else if (_t47.tag == Token_KwFloat) {
#line 267 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 268 "src/parser.tr"
        return _tr_str_lit_len("float", 5LL);
    } else if (_t47.tag == Token_KwBool) {
#line 270 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 271 "src/parser.tr"
        return _tr_str_lit_len("bool", 4LL);
    } else if ((_t47.tag == Token_KwStr || _t47.tag == Token_KwString)) {
#line 273 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 274 "src/parser.tr"
        return _tr_str_lit_len("str", 3LL);
    } else if (_t47.tag == Token_KwChar) {
#line 276 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 277 "src/parser.tr"
        return _tr_str_lit_len("char", 4LL);
    } else if (_t47.tag == Token_KwVoid) {
#line 279 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 280 "src/parser.tr"
        return _tr_str_lit_len("void", 4LL);
    } else if (_t47.tag == Token_KwExtend) {
#line 283 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 284 "src/parser.tr"
        return _tr_str_lit_len("extend", 6LL);
    } else if (_t47.tag == Token_KwNone) {
#line 287 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 288 "src/parser.tr"
        return _tr_str_lit_len("None", 4LL);
    } else if (_t47.tag == Token_KwLambda) {
#line 291 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 292 "src/parser.tr"
        return _tr_str_lit_len("lambda", 6LL);
    } else if (_t47.tag == Token_KwSpawn) {
#line 295 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 296 "src/parser.tr"
        return _tr_str_lit_len("spawn", 5LL);
    } else if (_t47.tag == Token_KwAsync) {
#line 299 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 300 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (1) {
        __auto_type _ = _t47;
#line 302 "src/parser.tr"
        /* pass */
    }
#line 303 "src/parser.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Parser_consume_method_ident(Parser* self) {
#line 316 "src/parser.tr"
    TrStr base = Parser_consume_ident(self);
#line 317 "src/parser.tr"
    if ((!_tr_str_eqv((base), (_tr_str_lit_len("", 0LL))))) {
#line 317 "src/parser.tr"
        TrStr _trret15 = _tr_str_retain(base);
        _tr_str_release(base);
        return _trret15;
    }
#line 333 "src/parser.tr"
    __auto_type _t48 = Parser_peek(self);
    if (_t48.tag == Token_KwDef) {
#line 335 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 336 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("def", 3LL);
    } else if (_t48.tag == Token_KwClass) {
#line 338 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 339 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("class", 5LL);
    } else if (_t48.tag == Token_KwEnum) {
#line 341 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 342 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("enum", 4LL);
    } else if (_t48.tag == Token_KwInterface) {
#line 344 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 345 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("interface", 9LL);
    } else if (_t48.tag == Token_KwStruct) {
#line 347 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 348 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("struct", 6LL);
    } else if (_t48.tag == Token_KwIf) {
#line 350 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 351 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("if", 2LL);
    } else if (_t48.tag == Token_KwElif) {
#line 353 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 354 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("elif", 4LL);
    } else if (_t48.tag == Token_KwElse) {
#line 356 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 357 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("else", 4LL);
    } else if (_t48.tag == Token_KwFor) {
#line 359 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 360 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("for", 3LL);
    } else if (_t48.tag == Token_KwWhile) {
#line 362 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 363 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("while", 5LL);
    } else if (_t48.tag == Token_KwLoop) {
#line 365 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 366 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("loop", 4LL);
    } else if (_t48.tag == Token_KwReturn) {
#line 368 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 369 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("return", 6LL);
    } else if (_t48.tag == Token_KwBreak) {
#line 371 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 372 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("break", 5LL);
    } else if (_t48.tag == Token_KwContinue) {
#line 374 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 375 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("continue", 8LL);
    } else if (_t48.tag == Token_KwPass) {
#line 377 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 378 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pass", 4LL);
    } else if (_t48.tag == Token_KwMatch) {
#line 380 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 381 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("match", 5LL);
    } else if (_t48.tag == Token_KwCase) {
#line 383 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 384 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("case", 4LL);
    } else if (_t48.tag == Token_KwTry) {
#line 386 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 387 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("try", 3LL);
    } else if (_t48.tag == Token_KwExcept) {
#line 389 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 390 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("except", 6LL);
    } else if (_t48.tag == Token_KwFinally) {
#line 392 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 393 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("finally", 7LL);
    } else if (_t48.tag == Token_KwRaise) {
#line 395 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 396 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("raise", 5LL);
    } else if (_t48.tag == Token_KwAssert) {
#line 398 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 399 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("assert", 6LL);
    } else if (_t48.tag == Token_KwWith) {
#line 401 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 402 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("with", 4LL);
    } else if (_t48.tag == Token_KwDefer) {
#line 404 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 405 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("defer", 5LL);
    } else if (_t48.tag == Token_KwAsm) {
#line 407 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 408 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("asm", 3LL);
    } else if (_t48.tag == Token_KwTaskGroup) {
#line 410 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 411 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("taskgroup", 9LL);
    } else if (_t48.tag == Token_KwImplements) {
#line 413 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 414 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("implements", 10LL);
    } else if (_t48.tag == Token_KwImport) {
#line 416 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 417 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("import", 6LL);
    } else if (_t48.tag == Token_KwFrom) {
#line 419 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 420 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("from", 4LL);
    } else if (_t48.tag == Token_KwAs) {
#line 422 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 423 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("as", 2LL);
    } else if (_t48.tag == Token_KwIn) {
#line 425 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 426 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("in", 2LL);
    } else if (_t48.tag == Token_KwMut) {
#line 428 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 429 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("mut", 3LL);
    } else if (_t48.tag == Token_KwShared) {
#line 431 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 432 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("shared", 6LL);
    } else if (_t48.tag == Token_KwExtern) {
#line 434 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 435 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extern", 6LL);
    } else if (_t48.tag == Token_KwUnsafe) {
#line 437 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 438 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("unsafe", 6LL);
    } else if (_t48.tag == Token_KwThrows) {
#line 440 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 441 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("throws", 6LL);
    } else if (_t48.tag == Token_KwExtends) {
#line 443 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 444 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extends", 7LL);
    } else if (_t48.tag == Token_KwAwait) {
#line 446 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 447 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("await", 5LL);
    } else if (_t48.tag == Token_KwYield) {
#line 449 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 450 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("yield", 5LL);
    } else if (_t48.tag == Token_KwPub) {
#line 452 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 453 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pub", 3LL);
    } else if (_t48.tag == Token_KwStatic) {
#line 455 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 456 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("static", 6LL);
    } else if (_t48.tag == Token_KwConst) {
#line 458 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 459 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("const", 5LL);
    } else if (_t48.tag == Token_KwActor) {
#line 461 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 462 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("actor", 5LL);
    } else if (_t48.tag == Token_KwSuper) {
#line 464 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 465 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("super", 5LL);
    } else if (_t48.tag == Token_KwExport) {
#line 467 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 468 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("export", 6LL);
    } else if (_t48.tag == Token_KwDecorator) {
#line 470 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 471 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("decorator", 9LL);
    } else if (_t48.tag == Token_KwMacro) {
#line 473 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 474 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("macro", 5LL);
    } else if (_t48.tag == Token_KwDo) {
#line 476 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 477 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("do", 2LL);
    } else if (_t48.tag == Token_KwSizeOf) {
#line 479 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 480 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("sizeof", 6LL);
    } else if (_t48.tag == Token_KwIs) {
#line 482 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 483 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("is", 2LL);
    } else if (_t48.tag == Token_KwTrue) {
#line 485 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 486 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("true", 4LL);
    } else if (_t48.tag == Token_KwFalse) {
#line 488 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 489 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("false", 5LL);
    } else if (_t48.tag == Token_KwAnd) {
#line 491 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 492 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("and", 3LL);
    } else if (_t48.tag == Token_KwOr) {
#line 494 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 495 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("or", 2LL);
    } else if (_t48.tag == Token_KwNot) {
#line 497 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 498 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("not", 3LL);
    } else if (1) {
        __auto_type _ = _t48;
#line 500 "src/parser.tr"
        /* pass */
    }
#line 501 "src/parser.tr"
    _tr_str_release(base);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Parser_consume_module_ident(Parser* self) {
#line 506 "src/parser.tr"
    __auto_type _t49 = Parser_peek(self);
    if (_t49.tag == Token_KwAsync) {
#line 508 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 509 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (_t49.tag == Token_KwMatch) {
#line 511 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 512 "src/parser.tr"
        return _tr_str_lit_len("match", 5LL);
    } else if (_t49.tag == Token_KwFor) {
#line 514 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 515 "src/parser.tr"
        return _tr_str_lit_len("for", 3LL);
    } else if (_t49.tag == Token_KwIn) {
#line 517 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 518 "src/parser.tr"
        return _tr_str_lit_len("in", 2LL);
    } else if (_t49.tag == Token_KwIs) {
#line 520 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 521 "src/parser.tr"
        return _tr_str_lit_len("is", 2LL);
    } else if (1) {
        __auto_type _ = _t49;
#line 523 "src/parser.tr"
        return Parser_consume_ident(self);
    }
#line 524 "src/parser.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) AstType* Parser_parse_type(Parser* self) {
#line 529 "src/parser.tr"
    TrStr name = _tr_str_lit_len("", 0LL);
#line 530 "src/parser.tr"
    __auto_type _t50 = Parser_peek(self);
    if (_t50.tag == Token_KwNone) {
#line 532 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 533 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t50.tag == Token_LParen) {
#line 536 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 537 "src/parser.tr"
        AstType* tup_t = AstType_init(_tr_str_lit_len("Tuple", 5LL));
#line 538 "src/parser.tr"
        while ((({ __auto_type _ord_t51 = (Parser_peek(self)); __auto_type _wr = ((_ord_t51.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t52 = (Parser_peek(self)); __auto_type _wr = ((_ord_t52.tag != Token_make_Eof().tag)); _wr; }))) {
#line 539 "src/parser.tr"
            AstType* elem_t = Parser_parse_type(self);
#line 540 "src/parser.tr"
            ({ __auto_type _ord_t53 = (box_asttype(elem_t)); List_ptr_append(tup_t->args, _ord_t53); });
#line 541 "src/parser.tr"
            if (({ __auto_type _ord_t54 = (Parser_peek(self)); __auto_type _wr = ((_ord_t54.tag == Token_make_Comma().tag)); _wr; })) {
#line 541 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 542 "src/parser.tr"
        if (({ __auto_type _ord_t55 = (Parser_peek(self)); __auto_type _wr = ((_ord_t55.tag == Token_make_RParen().tag)); _wr; })) {
#line 542 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 543 "src/parser.tr"
        _tr_str_release(name);
        return tup_t;
    } else if (_t50.tag == Token_LBracket) {
#line 546 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 547 "src/parser.tr"
        AstType* arr_t = AstType_init(_tr_str_lit_len("Array", 5LL));
#line 548 "src/parser.tr"
        AstType* arr_elem = Parser_parse_type(self);
#line 549 "src/parser.tr"
        ({ __auto_type _ord_t56 = (box_asttype(arr_elem)); List_ptr_append(arr_t->args, _ord_t56); });
#line 550 "src/parser.tr"
        if (({ __auto_type _ord_t57 = (Parser_peek(self)); __auto_type _wr = ((_ord_t57.tag == Token_make_Semicolon().tag)); _wr; })) {
#line 551 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 552 "src/parser.tr"
            __auto_type _t58 = Parser_peek(self);
            if (_t58.tag == Token_IntLit) {
                __auto_type arr_sz = _t58.data.IntLit.val;
#line 554 "src/parser.tr"
                arr_t->array_size = arr_sz;
#line 555 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t58;
                /* pass */
            }
        }
#line 557 "src/parser.tr"
        if (({ __auto_type _ord_t59 = (Parser_peek(self)); __auto_type _wr = ((_ord_t59.tag == Token_make_RBracket().tag)); _wr; })) {
#line 557 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 558 "src/parser.tr"
        _tr_str_release(name);
        return arr_t;
    } else if (_t50.tag == Token_KwDef) {
#line 563 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 564 "src/parser.tr"
        AstType* dfn_t = AstType_init(_tr_str_lit_len("def", 3LL));
#line 565 "src/parser.tr"
        if (({ __auto_type _ord_t60 = (Parser_peek(self)); __auto_type _wr = ((_ord_t60.tag == Token_make_LParen().tag)); _wr; })) {
#line 566 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 567 "src/parser.tr"
            while ((({ __auto_type _ord_t61 = (Parser_peek(self)); __auto_type _wr = ((_ord_t61.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t62 = (Parser_peek(self)); __auto_type _wr = ((_ord_t62.tag != Token_make_Eof().tag)); _wr; }))) {
#line 568 "src/parser.tr"
                AstType* dpt = Parser_parse_type(self);
#line 569 "src/parser.tr"
                ({ __auto_type _ord_t63 = (box_asttype(dpt)); List_ptr_append(dfn_t->args, _ord_t63); });
#line 570 "src/parser.tr"
                if (({ __auto_type _ord_t64 = (Parser_peek(self)); __auto_type _wr = ((_ord_t64.tag == Token_make_Comma().tag)); _wr; })) {
#line 570 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
            }
#line 571 "src/parser.tr"
            if (({ __auto_type _ord_t65 = (Parser_peek(self)); __auto_type _wr = ((_ord_t65.tag == Token_make_RParen().tag)); _wr; })) {
#line 571 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 574 "src/parser.tr"
        AstType** dthrows = (AstType**)(0LL);
#line 575 "src/parser.tr"
        if (({ __auto_type _ord_t66 = (Parser_peek(self)); __auto_type _wr = ((_ord_t66.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 576 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 577 "src/parser.tr"
            dthrows = box_asttype(Parser_parse_type(self));
        }
#line 578 "src/parser.tr"
        AstType* dret = AstType_init(_tr_str_lit_len("void", 4LL));
#line 579 "src/parser.tr"
        if (({ __auto_type _ord_t67 = (Parser_peek(self)); __auto_type _wr = ((_ord_t67.tag == Token_make_Arrow().tag)); _wr; })) {
#line 580 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 581 "src/parser.tr"
            dret = Parser_parse_type(self);
        }
#line 582 "src/parser.tr"
        if ((((unsigned long long)(dthrows)) != ((unsigned long long)(0LL)))) {
#line 583 "src/parser.tr"
            dret = throws_result_type(dret, dthrows);
        }
#line 584 "src/parser.tr"
        ({ __auto_type _ord_t68 = (box_asttype(dret)); List_ptr_append(dfn_t->args, _ord_t68); });
#line 585 "src/parser.tr"
        _tr_str_release(name);
        return dfn_t;
    } else if (_t50.tag == Token_KwMut) {
#line 589 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 590 "src/parser.tr"
        TrStr mr_name = Parser_consume_ident(self);
#line 591 "src/parser.tr"
        if (_tr_str_eqv((mr_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 592 "src/parser.tr"
            AstType* mr_inner = Parser_parse_type(self);
#line 593 "src/parser.tr"
            mr_inner->is_borrow = true;
#line 594 "src/parser.tr"
            mr_inner->is_mut_borrow = true;
#line 595 "src/parser.tr"
            _tr_str_release(mr_name);
            _tr_str_release(name);
            return mr_inner;
        }
#line 597 "src/parser.tr"
        _tr_str_release(mr_name);
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (1) {
        __auto_type _ = _t50;
#line 599 "src/parser.tr"
        TrStr _strtmp_t69 = Parser_consume_ident(self);
        _tr_str_release(name);
        name = _strtmp_t69;
    }
#line 601 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 602 "src/parser.tr"
        Parser_emit_diag_prev(self, _tr_str_lit_len("expected a type here", 20LL), _tr_str_lit_len("write a type name such as 'int', 'str', 'List[int]' or a class name.", 68LL));
#line 604 "src/parser.tr"
        __auto_type _t70 = Parser_peek(self);
        if ((_t70.tag == Token_Eof || _t70.tag == Token_RParen || _t70.tag == Token_RBracket)) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t70;
            self->pos = (self->pos + 1LL);
        }
#line 607 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
#line 613 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL)))) {
#line 614 "src/parser.tr"
        AstType* ref_inner = Parser_parse_type(self);
#line 615 "src/parser.tr"
        ref_inner->is_borrow = true;
#line 616 "src/parser.tr"
        _tr_str_release(name);
        return ref_inner;
    }
#line 619 "src/parser.tr"
    __auto_type _t71 = Parser_peek(self);
    if (_t71.tag == Token_LBracket) {
#line 621 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 622 "src/parser.tr"
        AstType* t = AstType_init(name);
#line 623 "src/parser.tr"
        bool going = true;
#line 624 "src/parser.tr"
        while (going) {
#line 625 "src/parser.tr"
            __auto_type _t72 = Parser_peek(self);
            if ((_t72.tag == Token_RBracket || _t72.tag == Token_Eof)) {
#line 627 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t72;
#line 629 "src/parser.tr"
                /* pass */
            }
#line 630 "src/parser.tr"
            if (going) {
#line 631 "src/parser.tr"
                __auto_type arg_t = Parser_parse_type(self);
#line 632 "src/parser.tr"
                ({ __auto_type _ord_t73 = (box_asttype(arg_t)); List_ptr_append(t->args, _ord_t73); });
#line 633 "src/parser.tr"
                __auto_type _t74 = Parser_peek(self);
                if (_t74.tag == Token_Comma) {
#line 635 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (_t74.tag == Token_Semicolon) {
#line 640 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 641 "src/parser.tr"
                    __auto_type _t75 = Parser_peek(self);
                    if (_t75.tag == Token_IntLit) {
                        __auto_type gsz = _t75.data.IntLit.val;
#line 643 "src/parser.tr"
                        t->array_size = gsz;
#line 644 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t75;
                        /* pass */
                    }
#line 646 "src/parser.tr"
                    going = false;
                } else if (1) {
                    __auto_type _ = _t74;
#line 648 "src/parser.tr"
                    going = false;
                }
            }
        }
#line 649 "src/parser.tr"
        __auto_type _t76 = Parser_peek(self);
        if (_t76.tag == Token_RBracket) {
#line 651 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t76;
#line 653 "src/parser.tr"
            ({ TrStr _at_t77 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the type '", 26LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[...]'", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t77, _tr_str_lit_len("add the matching ']' after the type arguments.", 46LL)); _tr_str_release(_at_t77); });
        }
#line 654 "src/parser.tr"
        _tr_str_release(name);
        return t;
    } else if (1) {
        __auto_type _ = _t71;
#line 656 "src/parser.tr"
        /* pass */
    }
#line 657 "src/parser.tr"
    return AstType_init(name);
}

__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self) {
#line 662 "src/parser.tr"
    List_ptr* pl = (void*)List_ptr_new();
#line 663 "src/parser.tr"
    bool going = true;
#line 664 "src/parser.tr"
    while (going) {
#line 665 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 666 "src/parser.tr"
        __auto_type _t78 = Parser_peek(self);
        if ((_t78.tag == Token_RParen || _t78.tag == Token_Eof)) {
#line 668 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t78;
#line 670 "src/parser.tr"
            /* pass */
        }
#line 671 "src/parser.tr"
        if (going) {
#line 672 "src/parser.tr"
            TrStr item_name = Parser_consume_ident(self);
#line 685 "src/parser.tr"
            if (((_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))) && ({ __auto_type _ord_t79 = (Parser_peek(self)); __auto_type _wr = ((_ord_t79.tag != Token_make_RParen().tag)); _wr; })) && ({ __auto_type _ord_t80 = (Parser_peek(self)); __auto_type _wr = ((_ord_t80.tag != Token_make_Eof().tag)); _wr; }))) {
#line 686 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a parameter name, found a reserved keyword", 51LL), _tr_str_lit_len("reserved words cannot be used as a parameter name (they introduce a new binding, same as a 'mut'/'let' variable) — rename this parameter.", 139LL));
#line 687 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 688 "src/parser.tr"
            if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 689 "src/parser.tr"
                AstType** ty_ptr = (AstType**)(0LL);
#line 690 "src/parser.tr"
                __auto_type _t81 = Parser_peek(self);
                if (_t81.tag == Token_Colon) {
#line 692 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 693 "src/parser.tr"
                    __auto_type ty = Parser_parse_type(self);
#line 696 "src/parser.tr"
                    if (({ __auto_type _ord_t82 = (Parser_peek(self)); __auto_type _wr = ((_ord_t82.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 697 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 698 "src/parser.tr"
                        ty->from_param = Parser_consume_ident(self);
#line 699 "src/parser.tr"
                        List_TrStr_append(ty->from_regions, ty->from_param);
                    }
#line 700 "src/parser.tr"
                    ty_ptr = box_asttype(ty);
                } else if (1) {
                    __auto_type _ = _t81;
#line 706 "src/parser.tr"
                    if (_tr_str_eqv((item_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 707 "src/parser.tr"
                        AstType* bt = Parser_parse_type(self);
#line 708 "src/parser.tr"
                        bt->is_borrow = true;
#line 709 "src/parser.tr"
                        if (({ __auto_type _ord_t83 = (Parser_peek(self)); __auto_type _wr = ((_ord_t83.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 710 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
#line 711 "src/parser.tr"
                            bt->from_param = Parser_consume_ident(self);
#line 712 "src/parser.tr"
                            List_TrStr_append(bt->from_regions, bt->from_param);
                        }
#line 713 "src/parser.tr"
                        ty_ptr = box_asttype(bt);
#line 716 "src/parser.tr"
                        TrStr _strtmp_t84 = _tr_str_retain(bt->name);
                        _tr_str_release(item_name);
                        item_name = _strtmp_t84;
                    }
                }
#line 717 "src/parser.tr"
                Param* p = Param_init(item_name, ty_ptr);
#line 718 "src/parser.tr"
                if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 719 "src/parser.tr"
                    if ((*ty_ptr)->is_borrow) {
#line 720 "src/parser.tr"
                        p->is_ref = true;
                    }
#line 721 "src/parser.tr"
                    if ((*ty_ptr)->is_mut_borrow) {
#line 722 "src/parser.tr"
                        p->is_mut_ref = true;
                    }
                }
#line 725 "src/parser.tr"
                if (({ __auto_type _ord_t85 = (Parser_peek(self)); __auto_type _wr = ((_ord_t85.tag == Token_make_DotDotDot().tag)); _wr; })) {
#line 726 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 727 "src/parser.tr"
                    p->is_variadic = true;
                }
#line 728 "src/parser.tr"
                List_ptr_append(pl, _tr_obj_retain(p));
#line 729 "src/parser.tr"
                __auto_type _t86 = Parser_peek(self);
                if (_t86.tag == Token_Comma) {
#line 731 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 732 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                } else if (1) {
                    __auto_type _ = _t86;
#line 734 "src/parser.tr"
                    going = false;
                }
            } else {
#line 736 "src/parser.tr"
                going = false;
            }
        }
    }
#line 737 "src/parser.tr"
    return pl;
}

__attribute__((hot)) Block* Parser_parse_block(Parser* self) {
#line 742 "src/parser.tr"
    Block* b = Block_init();
#line 743 "src/parser.tr"
    bool is_inline = false;
#line 744 "src/parser.tr"
    __auto_type _t87 = Parser_peek(self);
    if (_t87.tag == Token_Indent) {
#line 746 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (_t87.tag == Token_Newline) {
#line 748 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 749 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
    } else if (1) {
        __auto_type _ = _t87;
#line 751 "src/parser.tr"
        is_inline = true;
    }
#line 753 "src/parser.tr"
    if (is_inline) {
#line 754 "src/parser.tr"
        Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 755 "src/parser.tr"
        Block_push(b, Parser_parse_stmt(self));
#line 756 "src/parser.tr"
        bool semi_going = true;
#line 757 "src/parser.tr"
        while (semi_going) {
#line 758 "src/parser.tr"
            semi_going = false;
#line 759 "src/parser.tr"
            __auto_type _t88 = Parser_peek(self);
            if (_t88.tag == Token_Semicolon) {
#line 762 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 763 "src/parser.tr"
                __auto_type _t89 = Parser_peek(self);
                if ((_t89.tag == Token_Newline || _t89.tag == Token_Eof || _t89.tag == Token_Dedent)) {
#line 765 "src/parser.tr"
                    /* pass */
                } else if (1) {
                    __auto_type _ = _t89;
#line 767 "src/parser.tr"
                    Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 768 "src/parser.tr"
                    Block_push(b, Parser_parse_stmt(self));
#line 769 "src/parser.tr"
                    semi_going = true;
                }
            } else if (1) {
                __auto_type _ = _t88;
#line 773 "src/parser.tr"
                if ((self->pos > 0LL)) {
#line 774 "src/parser.tr"
                    __auto_type _t90 = List_Token_get(self->tokens, (self->pos - 1LL));
                    if (_t90.tag == Token_Semicolon) {
#line 776 "src/parser.tr"
                        __auto_type _t91 = Parser_peek(self);
                        if ((_t91.tag == Token_Newline || _t91.tag == Token_Eof || _t91.tag == Token_Dedent)) {
#line 778 "src/parser.tr"
                            /* pass */
                        } else if (1) {
                            __auto_type _ = _t91;
#line 780 "src/parser.tr"
                            Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 781 "src/parser.tr"
                            Block_push(b, Parser_parse_stmt(self));
#line 782 "src/parser.tr"
                            semi_going = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t90;
#line 784 "src/parser.tr"
                        /* pass */
                    }
                }
            }
        }
#line 785 "src/parser.tr"
        return b;
    }
#line 787 "src/parser.tr"
    bool going = true;
#line 788 "src/parser.tr"
    while (going) {
#line 789 "src/parser.tr"
        __auto_type _t92 = Parser_peek(self);
        if ((_t92.tag == Token_Dedent || _t92.tag == Token_Eof)) {
#line 791 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t92;
#line 793 "src/parser.tr"
            /* pass */
        }
#line 794 "src/parser.tr"
        if (going) {
#line 795 "src/parser.tr"
            Parser_skip_newlines(self);
#line 796 "src/parser.tr"
            __auto_type _t93 = Parser_peek(self);
            if ((_t93.tag == Token_Dedent || _t93.tag == Token_Eof)) {
#line 798 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t93;
#line 800 "src/parser.tr"
                /* pass */
            }
#line 801 "src/parser.tr"
            if (going) {
#line 802 "src/parser.tr"
                long long _sline = Parser_cur_line(self);
#line 803 "src/parser.tr"
                __auto_type s = Parser_parse_stmt(self);
#line 804 "src/parser.tr"
                Block_push(b, box_stmt(Stmt_ctor_SLine(_sline)));
#line 805 "src/parser.tr"
                Block_push(b, s);
            }
        }
    }
#line 806 "src/parser.tr"
    __auto_type _t94 = Parser_peek(self);
    if (_t94.tag == Token_Dedent) {
#line 808 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t94;
#line 810 "src/parser.tr"
        /* pass */
    }
#line 811 "src/parser.tr"
    return b;
}

__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self) {
#line 816 "src/parser.tr"
    __auto_type _t95 = Parser_peek(self);
    if (_t95.tag == Token_KwReturn) {
#line 818 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 819 "src/parser.tr"
        __auto_type _t96 = Parser_peek(self);
        if ((_t96.tag == Token_Newline || _t96.tag == Token_Dedent || _t96.tag == Token_Eof)) {
#line 821 "src/parser.tr"
            return box_stmt(Stmt_ctor_SReturn((Expr*)(0LL)));
        } else if (1) {
            __auto_type _ = _t96;
#line 823 "src/parser.tr"
            /* pass */
        }
#line 824 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 825 "src/parser.tr"
        Parser_expect_newline(self);
#line 826 "src/parser.tr"
        return box_stmt(Stmt_ctor_SReturn(e));
    } else if (_t95.tag == Token_KwPass) {
#line 828 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 829 "src/parser.tr"
        Parser_expect_newline(self);
#line 830 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t95.tag == Token_KwBreak) {
#line 832 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 835 "src/parser.tr"
        Expr* brk_val = (Expr*)(0LL);
#line 836 "src/parser.tr"
        __auto_type _t97 = Parser_peek(self);
        if (_t97.tag == Token_Newline) {
            /* pass */
        } else if (_t97.tag == Token_Dedent) {
            /* pass */
        } else if (_t97.tag == Token_Eof) {
            /* pass */
        } else if (_t97.tag == Token_Semicolon) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t97;
#line 842 "src/parser.tr"
            brk_val = Parser_parse_expr(self);
        }
#line 843 "src/parser.tr"
        Parser_expect_newline(self);
#line 844 "src/parser.tr"
        return box_stmt(Stmt_ctor_SBreak(brk_val));
    } else if (_t95.tag == Token_KwContinue) {
#line 846 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 847 "src/parser.tr"
        Parser_expect_newline(self);
#line 848 "src/parser.tr"
        return box_stmt(Stmt_make_SContinue());
    } else if (_t95.tag == Token_KwRaise) {
#line 850 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 851 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 852 "src/parser.tr"
        Parser_expect_newline(self);
#line 853 "src/parser.tr"
        return box_stmt(Stmt_ctor_SRaise(e));
    } else if (_t95.tag == Token_KwAsm) {
#line 855 "src/parser.tr"
        return Parser_parse_asm_stmt(self);
    } else if (_t95.tag == Token_KwSpawn) {
#line 857 "src/parser.tr"
        return Parser_parse_spawn_stmt(self);
    } else if (_t95.tag == Token_KwTaskGroup) {
#line 859 "src/parser.tr"
        return Parser_parse_taskgroup_stmt(self);
    } else if (_t95.tag == Token_KwGpu) {
#line 861 "src/parser.tr"
        return Parser_parse_gpu_stmt(self);
    } else if (_t95.tag == Token_KwIf) {
#line 863 "src/parser.tr"
        return Parser_parse_if_stmt(self);
    } else if (_t95.tag == Token_KwWhile) {
#line 865 "src/parser.tr"
        return Parser_parse_while_stmt(self);
    } else if (_t95.tag == Token_KwLoop) {
#line 867 "src/parser.tr"
        return Parser_parse_loop_stmt(self);
    } else if (_t95.tag == Token_KwFor) {
#line 869 "src/parser.tr"
        return Parser_parse_for_stmt(self);
    } else if (_t95.tag == Token_KwMatch) {
#line 871 "src/parser.tr"
        return Parser_parse_match_stmt(self);
    } else if (_t95.tag == Token_KwMut) {
#line 873 "src/parser.tr"
        return Parser_parse_let_stmt(self, true);
    } else if (_t95.tag == Token_KwShared) {
#line 875 "src/parser.tr"
        return Parser_parse_shared_let_stmt(self);
    } else if (_t95.tag == Token_KwConst) {
#line 877 "src/parser.tr"
        return Parser_parse_const_let_stmt(self);
    } else if (_t95.tag == Token_KwUnsafe) {
#line 879 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 880 "src/parser.tr"
        __auto_type _t98 = Parser_peek(self);
        if (_t98.tag == Token_Colon) {
#line 882 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t98;
#line 884 "src/parser.tr"
            /* pass */
        }
#line 885 "src/parser.tr"
        __auto_type body = Parser_parse_block(self);
#line 886 "src/parser.tr"
        Stmt* _trret16 = box_stmt(Stmt_ctor_SUnsafe(body));
        return _trret16;
    } else if (_t95.tag == Token_KwDefer) {
#line 889 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 890 "src/parser.tr"
        Stmt* deferred = Parser_parse_stmt(self);
#line 891 "src/parser.tr"
        return box_stmt(Stmt_ctor_SDefer(deferred));
    } else if (_t95.tag == Token_KwWith) {
#line 893 "src/parser.tr"
        return Parser_parse_with_stmt(self);
    } else if (_t95.tag == Token_KwTry) {
#line 895 "src/parser.tr"
        return Parser_parse_try_stmt(self);
    } else if (_t95.tag == Token_KwAssert) {
#line 897 "src/parser.tr"
        return Parser_parse_assert_stmt(self);
    } else if (_t95.tag == Token_KwFrom) {
#line 900 "src/parser.tr"
        Parser_parse_from_import(self);
#line 901 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t95.tag == Token_KwImport) {
#line 903 "src/parser.tr"
        Parser_parse_import(self);
#line 904 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t95.tag == Token_KwClass) {
#line 910 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t95.tag == Token_KwEnum) {
#line 912 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t95.tag == Token_KwInterface) {
#line 914 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t95.tag == Token_KwExtend) {
#line 916 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t95.tag == Token_KwDef) {
#line 918 "src/parser.tr"
        long long _ldline = Parser_cur_line(self);
#line 919 "src/parser.tr"
        FunctionDef* _ldf = Parser_parse_function_def(self, false);
#line 920 "src/parser.tr"
        _ldf->line = _ldline;
#line 921 "src/parser.tr"
        Stmt* _trret17 = box_stmt(Stmt_ctor_SLocalDecl(box_decl(Decl_ctor_DFunction(_ldf))));
        _tr_obj_release(_ldf, _trdrop_FunctionDef);
        return _trret17;
    } else if (1) {
        __auto_type _ = _t95;
#line 923 "src/parser.tr"
        /* pass */
    }
#line 924 "src/parser.tr"
    __auto_type _t99 = Parser_peek(self);
    if (_t99.tag == Token_Ident) {
        __auto_type cs_nm = _t99.data.Ident.name;
#line 926 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("type", 4LL)))) {
#line 930 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 931 "src/parser.tr"
            TrStr _ta_name = Parser_consume_ident(self);
#line 932 "src/parser.tr"
            if (({ __auto_type _ord_t100 = (Parser_peek(self)); __auto_type _wr = ((_ord_t100.tag == Token_make_Eq().tag)); _wr; })) {
#line 933 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 934 "src/parser.tr"
                AstType* _ta_ty = Parser_parse_type(self);
#line 935 "src/parser.tr"
                Parser_expect_newline(self);
#line 936 "src/parser.tr"
                Stmt* _trret18 = box_stmt(Stmt_make_SPass());
                _tr_str_release(_ta_name);
                return _trret18;
            }
        }
#line 937 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("chan_select", 11LL)))) {
#line 938 "src/parser.tr"
            Stmt* _trret19 = Parser_parse_chan_select_stmt(self);
            return _trret19;
        }
#line 939 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("var", 3LL)))) {
#line 946 "src/parser.tr"
            long long _var_save = self->pos;
#line 947 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 948 "src/parser.tr"
            __auto_type _t101 = Parser_peek(self);
            if (_t101.tag == Token_Ident) {
                __auto_type _ = _t101.data.Ident.name;
#line 950 "src/parser.tr"
                Stmt* _trret20 = Parser_parse_assign_or_expr_stmt(self);
                return _trret20;
            } else if (1) {
                __auto_type _ = _t101;
#line 952 "src/parser.tr"
                self->pos = _var_save;
            }
        }
    } else if (1) {
        __auto_type _ = _t99;
        /* pass */
    }
#line 954 "src/parser.tr"
    return Parser_parse_assign_or_expr_stmt(self);
}

__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self) {
#line 957 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 958 "src/parser.tr"
    __auto_type _t102 = Parser_peek(self);
    if (_t102.tag == Token_Colon) {
#line 960 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t102;
#line 962 "src/parser.tr"
        /* pass */
    }
#line 963 "src/parser.tr"
    __auto_type try_body = Parser_parse_block(self);
#line 964 "src/parser.tr"
    List_ptr* catches = (void*)List_ptr_new();
#line 965 "src/parser.tr"
    while (({ __auto_type _ord_t103 = (Parser_peek(self)); __auto_type _wr = ((_ord_t103.tag == Token_make_KwExcept().tag)); _wr; })) {
#line 966 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 967 "src/parser.tr"
        AstType** err_type = (AstType**)(0LL);
#line 968 "src/parser.tr"
        TrStr err_name = _tr_str_lit_len("", 0LL);
#line 969 "src/parser.tr"
        if (({ __auto_type _ord_t104 = (Parser_peek(self)); __auto_type _wr = ((_ord_t104.tag != Token_make_Colon().tag)); _wr; })) {
#line 971 "src/parser.tr"
            bool is_bare_var = false;
#line 972 "src/parser.tr"
            __auto_type _t105 = Parser_peek(self);
            if (_t105.tag == Token_Ident) {
                __auto_type _ = _t105.data.Ident.name;
#line 974 "src/parser.tr"
                long long save_pos = self->pos;
#line 975 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 976 "src/parser.tr"
                if (({ __auto_type _ord_t106 = (Parser_peek(self)); __auto_type _wr = ((_ord_t106.tag == Token_make_Colon().tag)); _wr; })) {
#line 977 "src/parser.tr"
                    is_bare_var = true;
                }
#line 978 "src/parser.tr"
                self->pos = save_pos;
            } else if (1) {
                __auto_type _ = _t105;
#line 980 "src/parser.tr"
                /* pass */
            }
#line 981 "src/parser.tr"
            if (is_bare_var) {
#line 982 "src/parser.tr"
                TrStr _strtmp_t107 = Parser_consume_ident(self);
                _tr_str_release(err_name);
                err_name = _strtmp_t107;
            } else {
#line 984 "src/parser.tr"
                err_type = box_asttype(Parser_parse_type(self));
#line 985 "src/parser.tr"
                if (({ __auto_type _ord_t108 = (Parser_peek(self)); __auto_type _wr = ((_ord_t108.tag == Token_make_KwAs().tag)); _wr; })) {
#line 986 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 987 "src/parser.tr"
                    TrStr _strtmp_t109 = Parser_consume_ident(self);
                    _tr_str_release(err_name);
                    err_name = _strtmp_t109;
                }
            }
        }
#line 988 "src/parser.tr"
        __auto_type _t110 = Parser_peek(self);
        if (_t110.tag == Token_Colon) {
#line 990 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t110;
#line 992 "src/parser.tr"
            /* pass */
        }
#line 993 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 994 "src/parser.tr"
        CatchClause* c = CatchClause_init(err_name, (Block**)(0LL));
#line 995 "src/parser.tr"
        /* unsafe block */
#line 996 "src/parser.tr"
        Block** body_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 997 "src/parser.tr"
        (*body_ptr = body);
#line 998 "src/parser.tr"
        c->body = body_ptr;
#line 999 "src/parser.tr"
        c->err_type = err_type;
#line 1000 "src/parser.tr"
        /* unsafe block */
#line 1001 "src/parser.tr"
        CatchClause** c_ptr = ((CatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(CatchClause*)));
#line 1002 "src/parser.tr"
        (*c_ptr = c);
#line 1003 "src/parser.tr"
        List_ptr_append(catches, c_ptr);
        _tr_str_release(err_name);
    }
#line 1005 "src/parser.tr"
    Block* finally_b = Block_init();
#line 1006 "src/parser.tr"
    if (({ __auto_type _ord_t111 = (Parser_peek(self)); __auto_type _wr = ((_ord_t111.tag == Token_make_KwFinally().tag)); _wr; })) {
#line 1007 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1008 "src/parser.tr"
        __auto_type _t112 = Parser_peek(self);
        if (_t112.tag == Token_Colon) {
#line 1010 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t112;
#line 1012 "src/parser.tr"
            /* pass */
        }
#line 1013 "src/parser.tr"
        finally_b = Parser_parse_block(self);
    } else if ((catches->len == 0LL)) {
#line 1015 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("'try' block has no 'except' or 'finally' clause", 47LL), _tr_str_lit_len("add 'except:' (or 'except Type as e:') after the try block, or a 'finally:' block.", 82LL));
    }
#line 1016 "src/parser.tr"
    Stmt* _trret21 = box_stmt(Stmt_ctor_STry(try_body, catches, finally_b));
    return _trret21;
}

__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self) {
#line 1019 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1020 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1021 "src/parser.tr"
    Expr* msg = (Expr*)(0LL);
#line 1022 "src/parser.tr"
    if (({ __auto_type _ord_t113 = (Parser_peek(self)); __auto_type _wr = ((_ord_t113.tag == Token_make_Comma().tag)); _wr; })) {
#line 1023 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1024 "src/parser.tr"
        msg = Parser_parse_expr(self);
    }
#line 1025 "src/parser.tr"
    Parser_expect_newline(self);
#line 1026 "src/parser.tr"
    return box_stmt(Stmt_ctor_SAssert(cond, msg));
}

__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self) {
#line 1029 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1030 "src/parser.tr"
    List_ptr* items = (void*)List_ptr_new();
#line 1031 "src/parser.tr"
    List_TrStr* aliases = (void*)List_TrStr_new();
#line 1032 "src/parser.tr"
    bool going = true;
#line 1033 "src/parser.tr"
    while (going) {
#line 1034 "src/parser.tr"
        Expr* item_e = Parser_parse_expr(self);
#line 1035 "src/parser.tr"
        TrStr alias = _tr_str_lit_len("", 0LL);
#line 1037 "src/parser.tr"
        if ((((unsigned long long)(item_e)) != ((unsigned long long)(0LL)))) {
#line 1038 "src/parser.tr"
            __auto_type _t114 = (*item_e);
            if (_t114.tag == Expr_ECast) {
                __auto_type cast_inner = _t114.data.ECast.expr;
__auto_type cast_ty_ptr = _t114.data.ECast.ty;
#line 1040 "src/parser.tr"
                if ((((unsigned long long)(cast_ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 1041 "src/parser.tr"
                    TrStr _strtmp_t115 = _tr_str_retain((*cast_ty_ptr)->name);
                    _tr_str_release(alias);
                    alias = _strtmp_t115;
                }
#line 1042 "src/parser.tr"
                item_e = cast_inner;
            } else if (1) {
                __auto_type _ = _t114;
                /* pass */
            }
        }
#line 1044 "src/parser.tr"
        List_ptr_append(items, item_e);
#line 1045 "src/parser.tr"
        List_TrStr_append(aliases, alias);
#line 1046 "src/parser.tr"
        if (({ __auto_type _ord_t116 = (Parser_peek(self)); __auto_type _wr = ((_ord_t116.tag == Token_make_Comma().tag)); _wr; })) {
#line 1047 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 1049 "src/parser.tr"
            going = false;
        }
        _tr_str_release(alias);
    }
#line 1050 "src/parser.tr"
    __auto_type _t117 = Parser_peek(self);
    if (_t117.tag == Token_Colon) {
#line 1052 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t117;
#line 1054 "src/parser.tr"
        /* pass */
    }
#line 1055 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1056 "src/parser.tr"
    Stmt* _trret22 = box_stmt(Stmt_ctor_SWith(items, aliases, body));
    return _trret22;
}

__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self) {
#line 1059 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1060 "src/parser.tr"
    __auto_type _t118 = Parser_peek(self);
    if (_t118.tag == Token_LParen) {
#line 1062 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t118;
#line 1064 "src/parser.tr"
        /* pass */
    }
#line 1065 "src/parser.tr"
    List_TrStr* parts = (void*)List_TrStr_new();
#line 1067 "src/parser.tr"
    bool reading = true;
#line 1068 "src/parser.tr"
    while (reading) {
#line 1069 "src/parser.tr"
        __auto_type _t119 = Parser_peek(self);
        if (_t119.tag == Token_StrLit) {
            __auto_type s = _t119.data.StrLit.val;
#line 1071 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1072 "src/parser.tr"
            List_TrStr_append(parts, s);
#line 1073 "src/parser.tr"
            __auto_type _t120 = Parser_peek(self);
            if (_t120.tag == Token_Comma) {
#line 1075 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t120;
                reading = false;
            }
        } else if (1) {
            __auto_type _ = _t119;
            reading = false;
        }
    }
#line 1078 "src/parser.tr"
    __auto_type _t121 = Parser_peek(self);
    if (_t121.tag == Token_RParen) {
#line 1080 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t121;
#line 1082 "src/parser.tr"
        /* pass */
    }
#line 1083 "src/parser.tr"
    Parser_expect_newline(self);
#line 1084 "src/parser.tr"
    TrStr code = _tr_str_lit_len("", 0LL);
#line 1085 "src/parser.tr"
    TrStr outs = _tr_str_lit_len("", 0LL);
#line 1086 "src/parser.tr"
    TrStr ins = _tr_str_lit_len("", 0LL);
#line 1087 "src/parser.tr"
    TrStr clob = _tr_str_lit_len("", 0LL);
#line 1088 "src/parser.tr"
    if ((parts->len > 0LL)) {
#line 1088 "src/parser.tr"
        TrStr _strtmp_t122 = List_TrStr_get(parts, 0LL);
        _tr_str_release(code);
        code = _strtmp_t122;
    }
#line 1089 "src/parser.tr"
    if ((parts->len > 1LL)) {
#line 1089 "src/parser.tr"
        TrStr _strtmp_t123 = List_TrStr_get(parts, 1LL);
        _tr_str_release(outs);
        outs = _strtmp_t123;
    }
#line 1090 "src/parser.tr"
    if ((parts->len > 2LL)) {
#line 1090 "src/parser.tr"
        TrStr _strtmp_t124 = List_TrStr_get(parts, 2LL);
        _tr_str_release(ins);
        ins = _strtmp_t124;
    }
#line 1091 "src/parser.tr"
    if ((parts->len > 3LL)) {
#line 1091 "src/parser.tr"
        TrStr _strtmp_t125 = List_TrStr_get(parts, 3LL);
        _tr_str_release(clob);
        clob = _strtmp_t125;
    }
#line 1092 "src/parser.tr"
    Stmt* _trret23 = box_stmt(Stmt_ctor_SAsm(code, outs, ins, clob));
    List_TrStr_free(parts);
    _tr_str_release(code);
    _tr_str_release(outs);
    _tr_str_release(ins);
    _tr_str_release(clob);
    return _trret23;
}

__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self) {
#line 1095 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1096 "src/parser.tr"
    __auto_type e = Parser_parse_expr(self);
#line 1097 "src/parser.tr"
    Parser_expect_newline(self);
#line 1098 "src/parser.tr"
    return box_stmt(Stmt_ctor_SSpawn(e));
}

__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self) {
#line 1101 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1102 "src/parser.tr"
    __auto_type _t126 = Parser_peek(self);
    if (_t126.tag == Token_Colon) {
#line 1104 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t126;
#line 1106 "src/parser.tr"
        /* pass */
    }
#line 1107 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1108 "src/parser.tr"
    Stmt* _trret24 = box_stmt(Stmt_ctor_STaskGroup(body));
    return _trret24;
}

__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self) {
#line 1116 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1117 "src/parser.tr"
    if (({ __auto_type _ord_t127 = (Parser_peek(self)); __auto_type _wr = ((_ord_t127.tag == Token_make_Colon().tag)); _wr; })) {
#line 1117 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1118 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1119 "src/parser.tr"
    if (({ __auto_type _ord_t128 = (Parser_peek(self)); __auto_type _wr = ((_ord_t128.tag == Token_make_Indent().tag)); _wr; })) {
#line 1119 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1120 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1121 "src/parser.tr"
    List_ptr* cases = (void*)List_ptr_new();
#line 1122 "src/parser.tr"
    bool going = true;
#line 1123 "src/parser.tr"
    while (going) {
#line 1124 "src/parser.tr"
        __auto_type _t129 = Parser_peek(self);
        if (_t129.tag == Token_KwCase) {
#line 1126 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1128 "src/parser.tr"
            long long arm_kind = (-1LL);
#line 1129 "src/parser.tr"
            TrStr var_nm = _tr_str_lit_len("", 0LL);
#line 1130 "src/parser.tr"
            Expr* chan_e = (Expr*)(0LL);
#line 1131 "src/parser.tr"
            Expr* val_e = (Expr*)(0LL);
#line 1132 "src/parser.tr"
            Expr* timeout_e = (Expr*)(0LL);
#line 1134 "src/parser.tr"
            __auto_type _t130 = Parser_peek(self);
            if (_t130.tag == Token_Ident) {
                __auto_type arm_nm = _t130.data.Ident.name;
#line 1136 "src/parser.tr"
                if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("default", 7LL)))) {
#line 1137 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1138 "src/parser.tr"
                    if (({ __auto_type _ord_t131 = (Parser_peek(self)); __auto_type _wr = ((_ord_t131.tag == Token_make_Colon().tag)); _wr; })) {
#line 1138 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1139 "src/parser.tr"
                    arm_kind = 3LL;
                } else if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("timeout", 7LL)))) {
#line 1142 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1143 "src/parser.tr"
                    if (({ __auto_type _ord_t132 = (Parser_peek(self)); __auto_type _wr = ((_ord_t132.tag == Token_make_LParen().tag)); _wr; })) {
#line 1143 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1144 "src/parser.tr"
                    timeout_e = Parser_parse_expr(self);
#line 1145 "src/parser.tr"
                    if (({ __auto_type _ord_t133 = (Parser_peek(self)); __auto_type _wr = ((_ord_t133.tag == Token_make_RParen().tag)); _wr; })) {
#line 1145 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1146 "src/parser.tr"
                    if (({ __auto_type _ord_t134 = (Parser_peek(self)); __auto_type _wr = ((_ord_t134.tag == Token_make_Colon().tag)); _wr; })) {
#line 1146 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1147 "src/parser.tr"
                    arm_kind = 2LL;
                } else {
#line 1151 "src/parser.tr"
                    long long saved = self->pos;
#line 1152 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1153 "src/parser.tr"
                    __auto_type _t135 = Parser_peek(self);
                    if (_t135.tag == Token_Eq) {
#line 1156 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1157 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1158 "src/parser.tr"
                        if (({ __auto_type _ord_t136 = (Parser_peek(self)); __auto_type _wr = ((_ord_t136.tag == Token_make_Colon().tag)); _wr; })) {
#line 1158 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1159 "src/parser.tr"
                        TrStr _strtmp_t137 = _tr_str_retain(arm_nm);
                        _tr_str_release(var_nm);
                        var_nm = _strtmp_t137;
#line 1160 "src/parser.tr"
                        arm_kind = 0LL;
                    } else if (1) {
                        __auto_type _ = _t135;
#line 1163 "src/parser.tr"
                        self->pos = saved;
#line 1164 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1165 "src/parser.tr"
                        if (({ __auto_type _ord_t138 = (Parser_peek(self)); __auto_type _wr = ((_ord_t138.tag == Token_make_Colon().tag)); _wr; })) {
#line 1165 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1166 "src/parser.tr"
                        arm_kind = 1LL;
                    }
                }
            } else if (1) {
                __auto_type _ = _t130;
#line 1169 "src/parser.tr"
                chan_e = Parser_parse_expr(self);
#line 1170 "src/parser.tr"
                if (({ __auto_type _ord_t139 = (Parser_peek(self)); __auto_type _wr = ((_ord_t139.tag == Token_make_Colon().tag)); _wr; })) {
#line 1170 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1171 "src/parser.tr"
                arm_kind = 1LL;
            }
#line 1172 "src/parser.tr"
            Block* arm_body = Parser_parse_block(self);
#line 1173 "src/parser.tr"
            /* unsafe block */
#line 1174 "src/parser.tr"
            ChanSelectArm** arm_p = ((ChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(ChanSelectArm*)));
#line 1175 "src/parser.tr"
            if ((arm_kind == 0LL)) {
#line 1176 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_recv(chan_e, var_nm, arm_body));
            } else if ((arm_kind == 1LL)) {
#line 1178 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_send(chan_e, (Expr*)(0LL), arm_body));
            } else if ((arm_kind == 2LL)) {
#line 1180 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_timeout(timeout_e, arm_body));
            } else {
#line 1182 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_default(arm_body));
            }
#line 1183 "src/parser.tr"
            List_ptr_append(cases, arm_p);
            _tr_str_release(var_nm);
        } else if ((_t129.tag == Token_Dedent || _t129.tag == Token_Eof)) {
#line 1185 "src/parser.tr"
            going = false;
        } else if (_t129.tag == Token_Newline) {
#line 1187 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t129;
#line 1189 "src/parser.tr"
            going = false;
        }
    }
#line 1190 "src/parser.tr"
    if (({ __auto_type _ord_t140 = (Parser_peek(self)); __auto_type _wr = ((_ord_t140.tag == Token_make_Dedent().tag)); _wr; })) {
#line 1190 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1191 "src/parser.tr"
    return box_stmt(Stmt_ctor_SChanSelect(cases));
}

__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self) {
#line 1194 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1195 "src/parser.tr"
    __auto_type _t141 = Parser_peek(self);
    if (_t141.tag == Token_Colon) {
#line 1197 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t141;
#line 1199 "src/parser.tr"
        /* pass */
    }
#line 1200 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1201 "src/parser.tr"
    Stmt* _trret25 = box_stmt(Stmt_ctor_SGpuBlock(body));
    return _trret25;
}

__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut) {
#line 1204 "src/parser.tr"
    if (is_mut) {
#line 1205 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1207 "src/parser.tr"
    if (({ __auto_type _ord_t142 = (Parser_peek(self)); __auto_type _wr = ((_ord_t142.tag == Token_make_LParen().tag)); _wr; })) {
#line 1208 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1209 "src/parser.tr"
        List_TrStr* pnames = (void*)List_TrStr_new();
#line 1210 "src/parser.tr"
        ({ TrStr _at_t143 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t143); _tr_str_release(_at_t143); });
#line 1211 "src/parser.tr"
        while (({ __auto_type _ord_t144 = (Parser_peek(self)); __auto_type _wr = ((_ord_t144.tag == Token_make_Comma().tag)); _wr; })) {
#line 1212 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1213 "src/parser.tr"
            if (({ __auto_type _ord_t145 = (Parser_peek(self)); __auto_type _wr = ((_ord_t145.tag == Token_make_RParen().tag)); _wr; })) {
#line 1213 "src/parser.tr"
                break;
            }
#line 1214 "src/parser.tr"
            ({ TrStr _at_t146 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t146); _tr_str_release(_at_t146); });
        }
#line 1215 "src/parser.tr"
        if (({ __auto_type _ord_t147 = (Parser_peek(self)); __auto_type _wr = ((_ord_t147.tag == Token_make_RParen().tag)); _wr; })) {
#line 1215 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1216 "src/parser.tr"
        if (({ __auto_type _ord_t148 = (Parser_peek(self)); __auto_type _wr = ((_ord_t148.tag == Token_make_Eq().tag)); _wr; })) {
#line 1216 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1217 "src/parser.tr"
        Expr* pval = Parser_parse_expr(self);
#line 1218 "src/parser.tr"
        Parser_expect_newline(self);
#line 1219 "src/parser.tr"
        return box_stmt(Stmt_ctor_SMultiLet(pnames, is_mut, pval));
    }
#line 1220 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1222 "src/parser.tr"
    if (({ __auto_type _ord_t149 = (Parser_peek(self)); __auto_type _wr = ((_ord_t149.tag == Token_make_Comma().tag)); _wr; })) {
#line 1223 "src/parser.tr"
        List_TrStr* names = (void*)List_TrStr_new();
#line 1224 "src/parser.tr"
        List_TrStr_append(names, name);
#line 1225 "src/parser.tr"
        while (({ __auto_type _ord_t150 = (Parser_peek(self)); __auto_type _wr = ((_ord_t150.tag == Token_make_Comma().tag)); _wr; })) {
#line 1226 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1227 "src/parser.tr"
            ({ TrStr _at_t151 = (Parser_consume_ident(self)); List_TrStr_append(names, _at_t151); _tr_str_release(_at_t151); });
        }
#line 1228 "src/parser.tr"
        __auto_type _t152 = Parser_peek(self);
        if (_t152.tag == Token_Eq) {
#line 1230 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t152;
#line 1232 "src/parser.tr"
            /* pass */
        }
#line 1233 "src/parser.tr"
        Expr* val_ptr = Parser_parse_expr(self);
#line 1234 "src/parser.tr"
        Parser_expect_newline(self);
#line 1235 "src/parser.tr"
        Stmt* _trret26 = box_stmt(Stmt_ctor_SMultiLet(names, is_mut, val_ptr));
        _tr_str_release(name);
        return _trret26;
    }
#line 1236 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1237 "src/parser.tr"
    __auto_type _t153 = Parser_peek(self);
    if (_t153.tag == Token_Colon) {
#line 1239 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1240 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1241 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t153;
#line 1243 "src/parser.tr"
        /* pass */
    }
#line 1244 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1245 "src/parser.tr"
    __auto_type _t154 = Parser_peek(self);
    if (_t154.tag == Token_Eq) {
#line 1247 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1248 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t154;
#line 1250 "src/parser.tr"
        /* pass */
    }
#line 1251 "src/parser.tr"
    if (((((unsigned long long)(ty_ptr)) == ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))))) {
#line 1252 "src/parser.tr"
        ({ TrStr _at_t155 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("variable '", 10LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has neither a type nor an initial value", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t156 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("write 'mut ", 11LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Type' or 'mut ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = value'.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_prev(self, _at_t155, _at_t156); _tr_str_release(_at_t155); _tr_str_release(_at_t156); });
    }
#line 1253 "src/parser.tr"
    Parser_expect_newline(self);
#line 1254 "src/parser.tr"
    Stmt* _trret27 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), is_mut, false, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret27;
}

__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self) {
#line 1257 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1258 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1259 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1260 "src/parser.tr"
    __auto_type _t157 = Parser_peek(self);
    if (_t157.tag == Token_Colon) {
#line 1262 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1263 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1264 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t157;
#line 1266 "src/parser.tr"
        /* pass */
    }
#line 1267 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1268 "src/parser.tr"
    __auto_type _t158 = Parser_peek(self);
    if (_t158.tag == Token_Eq) {
#line 1270 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1271 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t158;
#line 1273 "src/parser.tr"
        /* pass */
    }
#line 1274 "src/parser.tr"
    Parser_expect_newline(self);
#line 1275 "src/parser.tr"
    Stmt* _trret28 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Shared(), true, false, true, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret28;
}

__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self) {
#line 1278 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1279 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1280 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1281 "src/parser.tr"
    __auto_type _t159 = Parser_peek(self);
    if (_t159.tag == Token_Colon) {
#line 1283 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1284 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1285 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t159;
#line 1287 "src/parser.tr"
        /* pass */
    }
#line 1288 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1289 "src/parser.tr"
    __auto_type _t160 = Parser_peek(self);
    if (_t160.tag == Token_Eq) {
#line 1291 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1292 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t160;
#line 1294 "src/parser.tr"
        /* pass */
    }
#line 1295 "src/parser.tr"
    Parser_expect_newline(self);
#line 1296 "src/parser.tr"
    Stmt* _trret29 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, true, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret29;
}

__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self) {
#line 1299 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1300 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1301 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1302 "src/parser.tr"
    __auto_type _t161 = Parser_peek(self);
    if (_t161.tag == Token_Colon) {
#line 1304 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t161;
#line 1306 "src/parser.tr"
        /* pass */
    }
#line 1307 "src/parser.tr"
    __auto_type then_b = Parser_parse_block(self);
#line 1308 "src/parser.tr"
    List_ptr* elifs = (void*)List_ptr_new();
#line 1309 "src/parser.tr"
    Block* else_b = Block_init();
#line 1310 "src/parser.tr"
    bool going = true;
#line 1311 "src/parser.tr"
    while (going) {
#line 1312 "src/parser.tr"
        going = false;
#line 1313 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1314 "src/parser.tr"
        __auto_type _t162 = Parser_peek(self);
        if (_t162.tag == Token_KwElif) {
#line 1316 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1317 "src/parser.tr"
            __auto_type ec = Parser_parse_expr(self);
#line 1318 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1319 "src/parser.tr"
            __auto_type _t163 = Parser_peek(self);
            if (_t163.tag == Token_Colon) {
#line 1321 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t163;
#line 1323 "src/parser.tr"
                /* pass */
            }
#line 1324 "src/parser.tr"
            __auto_type eb = Parser_parse_block(self);
#line 1325 "src/parser.tr"
            /* unsafe block */
#line 1326 "src/parser.tr"
            Block** eb_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1327 "src/parser.tr"
            (*eb_ptr = eb);
#line 1328 "src/parser.tr"
            List_ptr_append(elifs, ElifClause_init(ec, eb_ptr));
#line 1329 "src/parser.tr"
            going = true;
        } else if (_t162.tag == Token_KwElse) {
#line 1331 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1332 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1333 "src/parser.tr"
            __auto_type _t164 = Parser_peek(self);
            if (_t164.tag == Token_Colon) {
#line 1335 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t164;
#line 1337 "src/parser.tr"
                /* pass */
            }
#line 1338 "src/parser.tr"
            else_b = Parser_parse_block(self);
        } else if (1) {
            __auto_type _ = _t162;
#line 1340 "src/parser.tr"
            /* pass */
        }
    }
#line 1341 "src/parser.tr"
    Stmt* _trret30 = box_stmt(Stmt_ctor_SIf(cond, then_b, elifs, else_b));
    return _trret30;
}

__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self) {
#line 1344 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1345 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1346 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1347 "src/parser.tr"
    __auto_type _t165 = Parser_peek(self);
    if (_t165.tag == Token_Colon) {
#line 1349 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t165;
#line 1351 "src/parser.tr"
        /* pass */
    }
#line 1352 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1353 "src/parser.tr"
    Stmt* _trret31 = box_stmt(Stmt_ctor_SWhile(cond, body, (void*)List_ptr_new()));
    return _trret31;
}

__attribute__((hot)) Stmt* Parser_parse_loop_stmt(Parser* self) {
#line 1357 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1358 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1359 "src/parser.tr"
    if (({ __auto_type _ord_t166 = (Parser_peek(self)); __auto_type _wr = ((_ord_t166.tag == Token_make_Colon().tag)); _wr; })) {
#line 1359 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1360 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1361 "src/parser.tr"
    Stmt* _trret32 = ({ __auto_type _ord_t167 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t168 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SWhile(_ord_t167, body, _ord_t168))); _wr; });
    return _trret32;
}

__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self) {
#line 1364 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1367 "src/parser.tr"
    bool for_is_ref = false;
#line 1368 "src/parser.tr"
    if (({ __auto_type _ord_t169 = (Parser_peek(self)); __auto_type _wr = ((_ord_t169.tag == Token_make_KwMut().tag)); _wr; })) {
#line 1369 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1370 "src/parser.tr"
        TrStr _mr = Parser_consume_ident(self);
#line 1371 "src/parser.tr"
        for_is_ref = true;
    }
#line 1372 "src/parser.tr"
    TrStr var = Parser_consume_ident(self);
#line 1373 "src/parser.tr"
    if (((!for_is_ref) && _tr_str_eqv((var), (_tr_str_lit_len("ref", 3LL))))) {
#line 1374 "src/parser.tr"
        __auto_type _t170 = Parser_peek(self);
        if (_t170.tag == Token_KwIn) {
            /* pass */
        } else if (_t170.tag == Token_Comma) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t170;
#line 1378 "src/parser.tr"
            for_is_ref = true;
#line 1379 "src/parser.tr"
            TrStr _strtmp_t171 = Parser_consume_ident(self);
            _tr_str_release(var);
            var = _strtmp_t171;
        }
    }
#line 1381 "src/parser.tr"
    if (({ __auto_type _ord_t172 = (Parser_peek(self)); __auto_type _wr = ((_ord_t172.tag == Token_make_Comma().tag)); _wr; })) {
#line 1382 "src/parser.tr"
        List_TrStr* unpack_vars = (void*)List_TrStr_new();
#line 1383 "src/parser.tr"
        List_TrStr_append(unpack_vars, var);
#line 1384 "src/parser.tr"
        while (({ __auto_type _ord_t173 = (Parser_peek(self)); __auto_type _wr = ((_ord_t173.tag == Token_make_Comma().tag)); _wr; })) {
#line 1385 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1386 "src/parser.tr"
            ({ TrStr _at_t174 = (Parser_consume_ident(self)); List_TrStr_append(unpack_vars, _at_t174); _tr_str_release(_at_t174); });
        }
#line 1387 "src/parser.tr"
        if (({ __auto_type _ord_t175 = (Parser_peek(self)); __auto_type _wr = ((_ord_t175.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1387 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1388 "src/parser.tr"
        Expr* fu_iter = Parser_parse_expr(self);
#line 1389 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1390 "src/parser.tr"
        if (({ __auto_type _ord_t176 = (Parser_peek(self)); __auto_type _wr = ((_ord_t176.tag == Token_make_Colon().tag)); _wr; })) {
#line 1390 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1391 "src/parser.tr"
        Block* fu_body = Parser_parse_block(self);
#line 1392 "src/parser.tr"
        Stmt* _trret33 = box_stmt(Stmt_ctor_SForUnpack(unpack_vars, fu_iter, fu_body));
        _tr_str_release(var);
        return _trret33;
    }
#line 1393 "src/parser.tr"
    __auto_type _t177 = Parser_peek(self);
    if (_t177.tag == Token_KwIn) {
#line 1395 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t177;
#line 1397 "src/parser.tr"
        /* pass */
    }
#line 1398 "src/parser.tr"
    __auto_type iter_e = Parser_parse_expr(self);
#line 1399 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1400 "src/parser.tr"
    __auto_type _t178 = Parser_peek(self);
    if (_t178.tag == Token_Colon) {
#line 1402 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t178;
#line 1404 "src/parser.tr"
        /* pass */
    }
#line 1405 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1406 "src/parser.tr"
    Stmt* _trret34 = box_stmt(Stmt_ctor_SFor(var, iter_e, body, (void*)List_ptr_new(), for_is_ref));
    _tr_str_release(var);
    return _trret34;
}

__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self) {
#line 1409 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1410 "src/parser.tr"
    __auto_type subj = Parser_parse_expr(self);
#line 1411 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1412 "src/parser.tr"
    __auto_type _t179 = Parser_peek(self);
    if (_t179.tag == Token_Colon) {
#line 1414 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t179;
#line 1416 "src/parser.tr"
        /* pass */
    }
#line 1417 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1418 "src/parser.tr"
    return box_stmt(Stmt_ctor_SMatch(subj, arms));
}

__attribute__((hot)) List_ptr* Parser_parse_match_arms(Parser* self) {
#line 1423 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1424 "src/parser.tr"
    __auto_type _t180 = Parser_peek(self);
    if (_t180.tag == Token_Indent) {
#line 1426 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t180;
#line 1428 "src/parser.tr"
        /* pass */
    }
#line 1429 "src/parser.tr"
    List_ptr* arms = (void*)List_ptr_new();
#line 1430 "src/parser.tr"
    bool going = true;
#line 1431 "src/parser.tr"
    while (going) {
#line 1432 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1433 "src/parser.tr"
        __auto_type _t181 = Parser_peek(self);
        if ((_t181.tag == Token_Dedent || _t181.tag == Token_Eof || _t181.tag == Token_RParen || _t181.tag == Token_RBracket || _t181.tag == Token_RBrace)) {
#line 1438 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t181;
#line 1440 "src/parser.tr"
            /* pass */
        }
#line 1441 "src/parser.tr"
        if (going) {
#line 1442 "src/parser.tr"
            Pattern pat = Parser_parse_pattern(self);
#line 1443 "src/parser.tr"
            if (({ __auto_type _ord_t182 = (Parser_peek(self)); __auto_type _wr = ((_ord_t182.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1444 "src/parser.tr"
                List_Pattern* or_pats = (void*)List_Pattern_new();
#line 1445 "src/parser.tr"
                List_Pattern_append(or_pats, pat);
#line 1446 "src/parser.tr"
                while (({ __auto_type _ord_t183 = (Parser_peek(self)); __auto_type _wr = ((_ord_t183.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1447 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1448 "src/parser.tr"
                    List_Pattern_append(or_pats, Parser_parse_pattern(self));
                }
#line 1449 "src/parser.tr"
                pat = Pattern_ctor_POr(or_pats);
            }
#line 1451 "src/parser.tr"
            Expr* guard_expr = (Expr*)(0LL);
#line 1452 "src/parser.tr"
            if (({ __auto_type _ord_t184 = (Parser_peek(self)); __auto_type _wr = ((_ord_t184.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1453 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1454 "src/parser.tr"
                guard_expr = Parser_parse_expr(self);
            }
#line 1455 "src/parser.tr"
            __auto_type _t185 = Parser_peek(self);
            if (_t185.tag == Token_Colon) {
#line 1457 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t185;
#line 1459 "src/parser.tr"
                /* pass */
            }
#line 1460 "src/parser.tr"
            Block* arm_block = Block_init();
#line 1461 "src/parser.tr"
            bool is_block_arm = false;
#line 1462 "src/parser.tr"
            __auto_type _t186 = Parser_peek(self);
            if ((_t186.tag == Token_Newline || _t186.tag == Token_Indent)) {
#line 1464 "src/parser.tr"
                arm_block = Parser_parse_block(self);
#line 1465 "src/parser.tr"
                is_block_arm = true;
            } else if (1) {
                __auto_type _ = _t186;
#line 1467 "src/parser.tr"
                /* pass */
            }
#line 1468 "src/parser.tr"
            if ((!is_block_arm)) {
#line 1469 "src/parser.tr"
                Block_push(arm_block, Parser_parse_stmt(self));
            }
#line 1470 "src/parser.tr"
            Block** ab_ptr = (Block**)(0LL);
#line 1471 "src/parser.tr"
            /* unsafe block */
#line 1472 "src/parser.tr"
            ab_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1473 "src/parser.tr"
            (*ab_ptr = arm_block);
#line 1474 "src/parser.tr"
            MatchArm* new_arm = MatchArm_init(pat, ab_ptr);
#line 1475 "src/parser.tr"
            new_arm->guard = guard_expr;
#line 1476 "src/parser.tr"
            List_ptr_append(arms, _tr_obj_retain(new_arm));
        }
    }
#line 1477 "src/parser.tr"
    __auto_type _t187 = Parser_peek(self);
    if (_t187.tag == Token_Dedent) {
#line 1479 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t187;
#line 1481 "src/parser.tr"
        /* pass */
    }
#line 1482 "src/parser.tr"
    return arms;
}

__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self) {
#line 1485 "src/parser.tr"
    __auto_type _t188 = Parser_peek(self);
    if (_t188.tag == Token_KwCase) {
#line 1487 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t188;
#line 1489 "src/parser.tr"
        /* pass */
    }
#line 1490 "src/parser.tr"
    __auto_type _t189 = Parser_peek(self);
    if (_t189.tag == Token_Ident) {
        __auto_type type_name = _t189.data.Ident.name;
#line 1492 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1493 "src/parser.tr"
        __auto_type _t190 = Parser_peek(self);
        if (_t190.tag == Token_Dot) {
#line 1495 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1496 "src/parser.tr"
            TrStr variant = Parser_consume_ident(self);
#line 1497 "src/parser.tr"
            __auto_type _t191 = Parser_peek(self);
            if (_t191.tag == Token_LParen) {
#line 1499 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1500 "src/parser.tr"
                List_TrStr* fields = (void*)List_TrStr_new();
#line 1501 "src/parser.tr"
                while ((({ __auto_type _ord_t192 = (Parser_peek(self)); __auto_type _wr = ((_ord_t192.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t193 = (Parser_peek(self)); __auto_type _wr = ((_ord_t193.tag != Token_make_Eof().tag)); _wr; }))) {
#line 1502 "src/parser.tr"
                    TrStr fname = Parser_consume_ident(self);
#line 1503 "src/parser.tr"
                    if (_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL)))) {
#line 1504 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 1506 "src/parser.tr"
                        List_TrStr_append(fields, fname);
                    }
#line 1507 "src/parser.tr"
                    if (({ __auto_type _ord_t194 = (Parser_peek(self)); __auto_type _wr = ((_ord_t194.tag == Token_make_Comma().tag)); _wr; })) {
#line 1508 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                    _tr_str_release(fname);
                }
#line 1509 "src/parser.tr"
                if (({ __auto_type _ord_t195 = (Parser_peek(self)); __auto_type _wr = ((_ord_t195.tag == Token_make_RParen().tag)); _wr; })) {
#line 1510 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1511 "src/parser.tr"
                if ((fields->len == 1LL)) {
#line 1512 "src/parser.tr"
                    return ({ TrStr _at_t196 = (List_TrStr_get(fields, 0LL)); __auto_type _wr = (Pattern_ctor_PVariantBind(type_name, variant, _at_t196)); _tr_str_release(_at_t196); _wr; });
                }
#line 1513 "src/parser.tr"
                return Pattern_ctor_PVariantBindMany(type_name, variant, fields);
            } else if (1) {
                __auto_type _ = _t191;
#line 1515 "src/parser.tr"
                /* pass */
            }
#line 1516 "src/parser.tr"
            return Pattern_ctor_PVariant(type_name, variant);
        } else if (1) {
            __auto_type _ = _t190;
#line 1518 "src/parser.tr"
            /* pass */
        }
#line 1519 "src/parser.tr"
        return Pattern_ctor_PBind(type_name);
    } else if (_t189.tag == Token_IntLit) {
        __auto_type v = _t189.data.IntLit.val;
#line 1521 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1522 "src/parser.tr"
        return Pattern_ctor_PLitInt(v);
    } else if (_t189.tag == Token_KwTrue) {
#line 1524 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1525 "src/parser.tr"
        return Pattern_ctor_PLitBool(true);
    } else if (_t189.tag == Token_KwFalse) {
#line 1527 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1528 "src/parser.tr"
        return Pattern_ctor_PLitBool(false);
    } else if (_t189.tag == Token_StrLit) {
        __auto_type s = _t189.data.StrLit.val;
__auto_type blen = _t189.data.StrLit.blen;
#line 1530 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1531 "src/parser.tr"
        return Pattern_ctor_PLitStr(s, blen);
    } else if (1) {
        __auto_type _ = _t189;
#line 1533 "src/parser.tr"
        /* pass */
    }
#line 1534 "src/parser.tr"
    return Pattern_make_PWild();
}

__attribute__((hot)) bool Parser__ast_simple(Parser* self, Expr* e) {
#line 1538 "src/parser.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1538 "src/parser.tr"
        return true;
    }
#line 1539 "src/parser.tr"
    __auto_type _t197 = (*e);
    if (_t197.tag == Expr_EIdent) {
        __auto_type _ = _t197.data.EIdent.name;
        return true;
    } else if (_t197.tag == Expr_ELitInt) {
        __auto_type _ = _t197.data.ELitInt.val;
        return true;
    } else if (_t197.tag == Expr_ELitStr) {
        return true;
    } else if (_t197.tag == Expr_ELitBool) {
        __auto_type _ = _t197.data.ELitBool.val;
        return true;
    } else if (_t197.tag == Expr_EPropAccess) {
        __auto_type o = _t197.data.EPropAccess.obj;
        return Parser__ast_simple(self, o);
    } else if (_t197.tag == Expr_EIndex) {
        __auto_type o = _t197.data.EIndex.obj;
__auto_type i = _t197.data.EIndex._tr_v_index;
        return (Parser__ast_simple(self, o) && Parser__ast_simple(self, i));
    } else if (_t197.tag == Expr_EBinOp) {
        __auto_type a = _t197.data.EBinOp.left;
__auto_type b = _t197.data.EBinOp.right;
        return (Parser__ast_simple(self, a) && Parser__ast_simple(self, b));
    } else if (_t197.tag == Expr_EUnaryOp) {
        __auto_type a = _t197.data.EUnaryOp.expr;
        return Parser__ast_simple(self, a);
    } else if (1) {
        __auto_type _ = _t197;
        return false;
    }
}

__attribute__((hot)) Stmt* Parser_mk_aug(Parser* self, Expr* lhs, TrStr op, Expr* rhs) {
#line 1555 "src/parser.tr"
    Stmt* plain = box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(op, lhs, rhs))));
#line 1556 "src/parser.tr"
    List_ptr* stmts = (void*)List_ptr_new();
#line 1557 "src/parser.tr"
    __auto_type _t198 = (*lhs);
    if (_t198.tag == Expr_EIndex) {
        __auto_type o = _t198.data.EIndex.obj;
__auto_type i = _t198.data.EIndex._tr_v_index;
#line 1559 "src/parser.tr"
        if ((Parser__ast_simple(self, o) && Parser__ast_simple(self, i))) {
#line 1559 "src/parser.tr"
            return plain;
        }
#line 1560 "src/parser.tr"
        Expr* o2 = o;
#line 1561 "src/parser.tr"
        Expr* i2 = i;
#line 1562 "src/parser.tr"
        if ((!Parser__ast_simple(self, o))) {
#line 1563 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1564 "src/parser.tr"
            TrStr on = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1565 "src/parser.tr"
            ({ __auto_type _ord_t199 = (Ownership_make_Own()); __auto_type _ord_t200 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(on, _ord_t199, true, false, false, _ord_t200, o))); });
#line 1566 "src/parser.tr"
            o2 = box_expr(Expr_ctor_EIdent(on));
            _tr_str_release(on);
        }
#line 1567 "src/parser.tr"
        if ((!Parser__ast_simple(self, i))) {
#line 1568 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1569 "src/parser.tr"
            TrStr inm = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1570 "src/parser.tr"
            ({ __auto_type _ord_t201 = (Ownership_make_Own()); __auto_type _ord_t202 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(inm, _ord_t201, true, false, false, _ord_t202, i))); });
#line 1571 "src/parser.tr"
            i2 = box_expr(Expr_ctor_EIdent(inm));
            _tr_str_release(inm);
        }
#line 1572 "src/parser.tr"
        Expr* t_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1573 "src/parser.tr"
        Expr* r_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1574 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node, box_expr(Expr_ctor_EBinOp(op, r_node, rhs)))));
    } else if (_t198.tag == Expr_EPropAccess) {
        __auto_type o = _t198.data.EPropAccess.obj;
__auto_type f = _t198.data.EPropAccess.prop;
#line 1576 "src/parser.tr"
        if (Parser__ast_simple(self, o)) {
#line 1576 "src/parser.tr"
            Stmt* _trret35 = plain;
            return _trret35;
        }
#line 1577 "src/parser.tr"
        self->aug_ctr = (self->aug_ctr + 1LL);
#line 1578 "src/parser.tr"
        TrStr pn = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1579 "src/parser.tr"
        ({ __auto_type _ord_t203 = (Ownership_make_Own()); __auto_type _ord_t204 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(pn, _ord_t203, true, false, false, _ord_t204, o))); });
#line 1580 "src/parser.tr"
        Expr* o3 = box_expr(Expr_ctor_EIdent(pn));
#line 1581 "src/parser.tr"
        Expr* t_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1582 "src/parser.tr"
        Expr* r_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1583 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node2, box_expr(Expr_ctor_EBinOp(op, r_node2, rhs)))));
        _tr_str_release(pn);
    } else if (1) {
        __auto_type _ = _t198;
        return plain;
    }
#line 1585 "src/parser.tr"
    Block* blk = Block_init();
#line 1586 "src/parser.tr"
    long long si = 0LL;
#line 1587 "src/parser.tr"
    while ((si < stmts->len)) {
#line 1588 "src/parser.tr"
        ({ __auto_type _ord_t205 = (((Stmt*)List_ptr_get(stmts, si))); List_ptr_append(blk->stmts, _ord_t205); });
#line 1589 "src/parser.tr"
        si = (si + 1LL);
    }
#line 1590 "src/parser.tr"
    Stmt* _trret36 = ({ __auto_type _ord_t206 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t207 = ((void*)List_ptr_new()); __auto_type _ord_t208 = (Block_init()); __auto_type _wr = (box_stmt(Stmt_ctor_SIf(_ord_t206, blk, _ord_t207, _ord_t208))); _wr; });
    return _trret36;
}

__attribute__((hot)) List_ptr* Parser_parse_comp_generators(Parser* self) {
#line 1594 "src/parser.tr"
    List_ptr* generators = (void*)List_ptr_new();
#line 1595 "src/parser.tr"
    while (({ __auto_type _ord_t209 = (Parser_peek(self)); __auto_type _wr = ((_ord_t209.tag == Token_make_KwFor().tag)); _wr; })) {
#line 1596 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1597 "src/parser.tr"
        TrStr target = Parser_consume_ident(self);
#line 1598 "src/parser.tr"
        __auto_type _t210 = Parser_peek(self);
        if (_t210.tag == Token_KwIn) {
#line 1600 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t210;
#line 1602 "src/parser.tr"
            /* pass */
        }
#line 1603 "src/parser.tr"
        Expr* iter = Parser_parse_or_expr(self);
#line 1604 "src/parser.tr"
        List_ptr* ifs = (void*)List_ptr_new();
#line 1605 "src/parser.tr"
        while (({ __auto_type _ord_t211 = (Parser_peek(self)); __auto_type _wr = ((_ord_t211.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1606 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1607 "src/parser.tr"
            List_ptr_append(ifs, Parser_parse_or_expr(self));
        }
#line 1608 "src/parser.tr"
        Comprehension* c = Comprehension_init(target, iter);
#line 1609 "src/parser.tr"
        c->ifs = ifs;
#line 1610 "src/parser.tr"
        /* unsafe block */
#line 1611 "src/parser.tr"
        Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 1612 "src/parser.tr"
        (*c_ptr = c);
#line 1613 "src/parser.tr"
        List_ptr_append(generators, c_ptr);
        _tr_str_release(target);
    }
#line 1614 "src/parser.tr"
    return generators;
}

__attribute__((hot)) Expr* Parser_desugar_collection_comp(Parser* self, Expr* key, Expr* val, List_ptr* gens) {
#line 1619 "src/parser.tr"
    TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->pos)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__comp", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1620 "src/parser.tr"
    Stmt* body_stmt = box_stmt(Stmt_make_SPass());
#line 1621 "src/parser.tr"
    Expr* empty = box_expr(Expr_ctor_ESet((void*)List_ptr_new()));
#line 1622 "src/parser.tr"
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 1623 "src/parser.tr"
        empty = ({ __auto_type _ord_t212 = ((void*)List_ptr_new()); __auto_type _ord_t213 = ((void*)List_ptr_new()); __auto_type _wr = (box_expr(Expr_ctor_EDict(_ord_t212, _ord_t213))); _wr; });
#line 1624 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SAssign(box_expr(Expr_ctor_EIndex(box_expr(Expr_ctor_EIdent(tmp)), key)), val));
    } else {
#line 1626 "src/parser.tr"
        List_ptr* margs = (void*)List_ptr_new();
#line 1627 "src/parser.tr"
        List_ptr_append(margs, key);
#line 1628 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EMethodCall(box_expr(Expr_ctor_EIdent(tmp)), _tr_str_lit_len("add", 3LL), margs))));
    }
#line 1629 "src/parser.tr"
    long long gi = (gens->len - 1LL);
#line 1630 "src/parser.tr"
    while ((gi >= 0LL)) {
#line 1631 "src/parser.tr"
        Comprehension* g = (*((Comprehension**)List_ptr_get(gens, gi)));
#line 1632 "src/parser.tr"
        Block* inner = Block_init();
#line 1633 "src/parser.tr"
        Block_push(inner, body_stmt);
#line 1634 "src/parser.tr"
        Stmt* cur = box_stmt(Stmt_make_SPass());
#line 1635 "src/parser.tr"
        long long ci = (g->ifs->len - 1LL);
#line 1636 "src/parser.tr"
        Block* guarded = inner;
#line 1637 "src/parser.tr"
        while ((ci >= 0LL)) {
#line 1638 "src/parser.tr"
            Block* outer = Block_init();
#line 1639 "src/parser.tr"
            ({ __auto_type _ord_t214 = (((Expr*)List_ptr_get(g->ifs, ci))); __auto_type _ord_t215 = ((void*)List_ptr_new()); __auto_type _ord_t216 = (Block_init()); Block_push(outer, box_stmt(Stmt_ctor_SIf(_ord_t214, guarded, _ord_t215, _ord_t216))); });
#line 1640 "src/parser.tr"
            guarded = outer;
#line 1641 "src/parser.tr"
            ci = (ci - 1LL);
        }
#line 1642 "src/parser.tr"
        cur = ({ __auto_type _ord_t217 = (g->target); __auto_type _ord_t218 = (g->iter); __auto_type _ord_t219 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SFor(_ord_t217, _ord_t218, guarded, _ord_t219, false))); _wr; });
#line 1643 "src/parser.tr"
        body_stmt = cur;
#line 1644 "src/parser.tr"
        gi = (gi - 1LL);
    }
#line 1645 "src/parser.tr"
    Block* blk = Block_init();
#line 1646 "src/parser.tr"
    ({ __auto_type _ord_t220 = (Ownership_make_Own()); __auto_type _ord_t221 = ((AstType**)(0LL)); Block_push(blk, box_stmt(Stmt_ctor_SLet(tmp, _ord_t220, true, false, false, _ord_t221, empty))); });
#line 1647 "src/parser.tr"
    Block_push(blk, body_stmt);
#line 1648 "src/parser.tr"
    Block_push(blk, box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EIdent(tmp)))));
#line 1649 "src/parser.tr"
    Expr* _trret37 = box_expr(Expr_ctor_EDo(blk));
    _tr_str_release(tmp);
    return _trret37;
}

__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self) {
#line 1652 "src/parser.tr"
    __auto_type lhs = Parser_parse_expr(self);
#line 1654 "src/parser.tr"
    if (({ __auto_type _ord_t222 = (Parser_peek(self)); __auto_type _wr = ((_ord_t222.tag == Token_make_Comma().tag)); _wr; })) {
#line 1655 "src/parser.tr"
        __auto_type _t223 = (*lhs);
        if (_t223.tag == Expr_EIdent) {
            __auto_type mn0 = _t223.data.EIdent.name;
#line 1657 "src/parser.tr"
            List_TrStr* mnames = (void*)List_TrStr_new();
#line 1658 "src/parser.tr"
            List_TrStr_append(mnames, mn0);
#line 1659 "src/parser.tr"
            while (({ __auto_type _ord_t224 = (Parser_peek(self)); __auto_type _wr = ((_ord_t224.tag == Token_make_Comma().tag)); _wr; })) {
#line 1660 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1661 "src/parser.tr"
                ({ TrStr _at_t225 = (Parser_consume_ident(self)); List_TrStr_append(mnames, _at_t225); _tr_str_release(_at_t225); });
            }
#line 1662 "src/parser.tr"
            if (({ __auto_type _ord_t226 = (Parser_peek(self)); __auto_type _wr = ((_ord_t226.tag == Token_make_Eq().tag)); _wr; })) {
#line 1663 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1664 "src/parser.tr"
                Expr* mrhs = Parser_parse_expr(self);
#line 1665 "src/parser.tr"
                if (({ __auto_type _ord_t227 = (Parser_peek(self)); __auto_type _wr = ((_ord_t227.tag == Token_make_Comma().tag)); _wr; })) {
#line 1666 "src/parser.tr"
                    List_ptr* mitems = (void*)List_ptr_new();
#line 1667 "src/parser.tr"
                    List_ptr_append(mitems, mrhs);
#line 1668 "src/parser.tr"
                    while (({ __auto_type _ord_t228 = (Parser_peek(self)); __auto_type _wr = ((_ord_t228.tag == Token_make_Comma().tag)); _wr; })) {
#line 1669 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1670 "src/parser.tr"
                        List_ptr_append(mitems, Parser_parse_expr(self));
                    }
#line 1671 "src/parser.tr"
                    mrhs = box_expr(Expr_ctor_ETuple(mitems));
                }
#line 1672 "src/parser.tr"
                Parser_expect_newline(self);
#line 1673 "src/parser.tr"
                Stmt* _trret38 = box_stmt(Stmt_ctor_SMultiLet(mnames, false, mrhs));
                return _trret38;
            }
        } else if (1) {
            __auto_type _ = _t223;
            /* pass */
        }
    }
#line 1675 "src/parser.tr"
    __auto_type _t229 = Parser_peek(self);
    if (_t229.tag == Token_Colon) {
#line 1678 "src/parser.tr"
        __auto_type _t230 = (*lhs);
        if (_t230.tag == Expr_EIdent) {
            __auto_type name = _t230.data.EIdent.name;
#line 1680 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1681 "src/parser.tr"
            AstType* ty = Parser_parse_type(self);
#line 1682 "src/parser.tr"
            Expr* val = (Expr*)(0LL);
#line 1683 "src/parser.tr"
            if (({ __auto_type _ord_t231 = (Parser_peek(self)); __auto_type _wr = ((_ord_t231.tag == Token_make_Eq().tag)); _wr; })) {
#line 1684 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1685 "src/parser.tr"
                val = Parser_parse_expr(self);
            }
#line 1686 "src/parser.tr"
            Parser_expect_newline(self);
#line 1687 "src/parser.tr"
            Stmt* _trret39 = ({ __auto_type _ord_t232 = (Ownership_make_Own()); __auto_type _ord_t233 = (box_asttype(ty)); __auto_type _wr = (box_stmt(Stmt_ctor_SLet(name, _ord_t232, false, false, false, _ord_t233, val))); _wr; });
            return _trret39;
        } else if (1) {
            __auto_type _ = _t230;
            /* pass */
        }
    } else if (_t229.tag == Token_Eq) {
#line 1690 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1691 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1692 "src/parser.tr"
        Parser_expect_newline(self);
#line 1693 "src/parser.tr"
        return box_stmt(Stmt_ctor_SAssign(lhs, rhs));
    } else if (_t229.tag == Token_PlusEq) {
#line 1695 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1696 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1697 "src/parser.tr"
        Parser_expect_newline(self);
#line 1698 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("+", 1LL), rhs);
    } else if (_t229.tag == Token_MinusEq) {
#line 1700 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1701 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1702 "src/parser.tr"
        Parser_expect_newline(self);
#line 1703 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("-", 1LL), rhs);
    } else if (_t229.tag == Token_StarEq) {
#line 1705 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1706 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1707 "src/parser.tr"
        Parser_expect_newline(self);
#line 1708 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("*", 1LL), rhs);
    } else if (_t229.tag == Token_SlashEq) {
#line 1710 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1711 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1712 "src/parser.tr"
        Parser_expect_newline(self);
#line 1713 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("/", 1LL), rhs);
    } else if (_t229.tag == Token_PercentEq) {
#line 1715 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1716 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1717 "src/parser.tr"
        Parser_expect_newline(self);
#line 1718 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("%", 1LL), rhs);
    } else if (_t229.tag == Token_FloorDivEq) {
#line 1720 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1721 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1722 "src/parser.tr"
        Parser_expect_newline(self);
#line 1723 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("//", 2LL), rhs);
    } else if (_t229.tag == Token_StarStarEq) {
#line 1725 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1726 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1727 "src/parser.tr"
        Parser_expect_newline(self);
#line 1728 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("**", 2LL), rhs);
    } else if (_t229.tag == Token_AmpEq) {
#line 1730 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1731 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1732 "src/parser.tr"
        Parser_expect_newline(self);
#line 1733 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("&", 1LL), rhs);
    } else if (_t229.tag == Token_PipeEq) {
#line 1735 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1736 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1737 "src/parser.tr"
        Parser_expect_newline(self);
#line 1738 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("|", 1LL), rhs);
    } else if (_t229.tag == Token_CaretEq) {
#line 1740 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1741 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1742 "src/parser.tr"
        Parser_expect_newline(self);
#line 1743 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("^", 1LL), rhs);
    } else if (_t229.tag == Token_LtLtEq) {
#line 1745 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1746 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1747 "src/parser.tr"
        Parser_expect_newline(self);
#line 1748 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("<<", 2LL), rhs);
    } else if (_t229.tag == Token_GtGtEq) {
#line 1750 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1751 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1752 "src/parser.tr"
        Parser_expect_newline(self);
#line 1753 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len(">>", 2LL), rhs);
    } else if (1) {
        __auto_type _ = _t229;
#line 1755 "src/parser.tr"
        /* pass */
    }
#line 1756 "src/parser.tr"
    Parser_expect_newline(self);
#line 1757 "src/parser.tr"
    return box_stmt(Stmt_ctor_SExpr(lhs));
}

__attribute__((hot)) Expr* Parser_parse_expr(Parser* self) {
#line 1763 "src/parser.tr"
    if (({ __auto_type _ord_t234 = (Parser_peek(self)); __auto_type _wr = ((_ord_t234.tag == Token_make_KwDo().tag)); _wr; })) {
#line 1764 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1765 "src/parser.tr"
        if (({ __auto_type _ord_t235 = (Parser_peek(self)); __auto_type _wr = ((_ord_t235.tag == Token_make_Colon().tag)); _wr; })) {
#line 1765 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1766 "src/parser.tr"
        Block* do_body = Parser_parse_block(self);
#line 1767 "src/parser.tr"
        Expr* _trret40 = box_expr(Expr_ctor_EDo(do_body));
        return _trret40;
    }
#line 1774 "src/parser.tr"
    if (({ __auto_type _ord_t236 = (Parser_peek(self)); __auto_type _wr = ((_ord_t236.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1775 "src/parser.tr"
        return Parser_parse_if_expr(self);
    }
#line 1779 "src/parser.tr"
    if (({ __auto_type _ord_t237 = (Parser_peek(self)); __auto_type _wr = ((_ord_t237.tag == Token_make_KwMatch().tag)); _wr; })) {
#line 1780 "src/parser.tr"
        return Parser_parse_match_expr(self);
    }
#line 1784 "src/parser.tr"
    if (({ __auto_type _ord_t238 = (Parser_peek(self)); __auto_type _wr = ((_ord_t238.tag == Token_make_KwLoop().tag)); _wr; })) {
#line 1785 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1786 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1787 "src/parser.tr"
        if (({ __auto_type _ord_t239 = (Parser_peek(self)); __auto_type _wr = ((_ord_t239.tag == Token_make_Colon().tag)); _wr; })) {
#line 1787 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1788 "src/parser.tr"
        Block* loop_body = Parser_parse_block(self);
#line 1789 "src/parser.tr"
        Expr* _trret41 = box_expr(Expr_ctor_ELoop(loop_body));
        return _trret41;
    }
#line 1790 "src/parser.tr"
    if (({ __auto_type _ord_t240 = (Parser_peek(self)); __auto_type _wr = ((_ord_t240.tag == Token_make_KwWhile().tag)); _wr; })) {
#line 1791 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1792 "src/parser.tr"
        Expr* we_cond = Parser_parse_expr(self);
#line 1793 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1794 "src/parser.tr"
        if (({ __auto_type _ord_t241 = (Parser_peek(self)); __auto_type _wr = ((_ord_t241.tag == Token_make_Colon().tag)); _wr; })) {
#line 1794 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1795 "src/parser.tr"
        Block* we_body = Parser_parse_block(self);
#line 1796 "src/parser.tr"
        Block* we_else = Block_init();
#line 1797 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1798 "src/parser.tr"
        if (({ __auto_type _ord_t242 = (Parser_peek(self)); __auto_type _wr = ((_ord_t242.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1799 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1800 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1801 "src/parser.tr"
            if (({ __auto_type _ord_t243 = (Parser_peek(self)); __auto_type _wr = ((_ord_t243.tag == Token_make_Colon().tag)); _wr; })) {
#line 1801 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 1802 "src/parser.tr"
            we_else = Parser_parse_block(self);
        }
#line 1803 "src/parser.tr"
        Expr* _trret42 = box_expr(Expr_ctor_EWhileExpr(we_cond, we_body, we_else));
        return _trret42;
    }
#line 1804 "src/parser.tr"
    return Parser_parse_ternary(self);
}

__attribute__((hot)) Expr* Parser_parse_match_expr(Parser* self) {
#line 1807 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1808 "src/parser.tr"
    Expr* subj = Parser_parse_expr(self);
#line 1809 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1810 "src/parser.tr"
    if (({ __auto_type _ord_t244 = (Parser_peek(self)); __auto_type _wr = ((_ord_t244.tag == Token_make_Colon().tag)); _wr; })) {
#line 1810 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1811 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1812 "src/parser.tr"
    return box_expr(Expr_ctor_EMatch(subj, arms));
}

__attribute__((hot)) Expr* Parser_parse_if_expr(Parser* self) {
#line 1819 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1820 "src/parser.tr"
    Expr* cond = Parser_parse_expr(self);
#line 1821 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1822 "src/parser.tr"
    if (({ __auto_type _ord_t245 = (Parser_peek(self)); __auto_type _wr = ((_ord_t245.tag == Token_make_Colon().tag)); _wr; })) {
#line 1822 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1823 "src/parser.tr"
    Block* then_b = Parser_parse_block(self);
#line 1824 "src/parser.tr"
    Expr* then_e = box_expr(Expr_ctor_EDo(then_b));
#line 1825 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1826 "src/parser.tr"
    Expr* else_e = box_expr(Expr_ctor_EDo(Block_init()));
#line 1827 "src/parser.tr"
    __auto_type _t246 = Parser_peek(self);
    if (_t246.tag == Token_KwElif) {
#line 1829 "src/parser.tr"
        else_e = Parser_parse_if_expr(self);
    } else if (_t246.tag == Token_KwElse) {
#line 1831 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1832 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1833 "src/parser.tr"
        if (({ __auto_type _ord_t247 = (Parser_peek(self)); __auto_type _wr = ((_ord_t247.tag == Token_make_Colon().tag)); _wr; })) {
#line 1833 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1834 "src/parser.tr"
        Block* else_b = Parser_parse_block(self);
#line 1835 "src/parser.tr"
        else_e = box_expr(Expr_ctor_EDo(else_b));
    } else if (1) {
        __auto_type _ = _t246;
        /* pass */
    }
#line 1837 "src/parser.tr"
    Expr* _trret43 = box_expr(Expr_ctor_EIfElse(cond, then_e, else_e));
    return _trret43;
}

__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self) {
#line 1840 "src/parser.tr"
    Expr* left = Parser_parse_or_expr(self);
#line 1841 "src/parser.tr"
    if (({ __auto_type _ord_t248 = (Parser_peek(self)); __auto_type _wr = ((_ord_t248.tag == Token_make_Newline().tag)); _wr; })) {
#line 1842 "src/parser.tr"
        return left;
    }
#line 1845 "src/parser.tr"
    __auto_type _t249 = (*left);
    if (_t249.tag == Expr_EClosure) {
#line 1847 "src/parser.tr"
        return left;
    } else if (1) {
        __auto_type _ = _t249;
        /* pass */
    }
#line 1849 "src/parser.tr"
    if (({ __auto_type _ord_t250 = (Parser_peek(self)); __auto_type _wr = ((_ord_t250.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1850 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1851 "src/parser.tr"
        Expr* cond = Parser_parse_or_expr(self);
#line 1852 "src/parser.tr"
        if (({ __auto_type _ord_t251 = (Parser_peek(self)); __auto_type _wr = ((_ord_t251.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1853 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1854 "src/parser.tr"
        Expr* else_e = Parser_parse_ternary(self);
#line 1855 "src/parser.tr"
        return box_expr(Expr_ctor_EIfElse(cond, left, else_e));
    }
#line 1856 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self) {
#line 1859 "src/parser.tr"
    Expr* left = Parser_parse_and_expr(self);
#line 1860 "src/parser.tr"
    while (({ __auto_type _ord_t252 = (Parser_peek(self)); __auto_type _wr = ((_ord_t252.tag == Token_make_KwOr().tag)); _wr; })) {
#line 1861 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1862 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1863 "src/parser.tr"
        __auto_type right = Parser_parse_and_expr(self);
#line 1864 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("or", 2LL), left, right));
    }
#line 1865 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self) {
#line 1868 "src/parser.tr"
    Expr* left = Parser_parse_not_expr(self);
#line 1869 "src/parser.tr"
    while (({ __auto_type _ord_t253 = (Parser_peek(self)); __auto_type _wr = ((_ord_t253.tag == Token_make_KwAnd().tag)); _wr; })) {
#line 1870 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1871 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1872 "src/parser.tr"
        __auto_type right = Parser_parse_not_expr(self);
#line 1873 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), left, right));
    }
#line 1874 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self) {
#line 1877 "src/parser.tr"
    __auto_type _t254 = Parser_peek(self);
    if (_t254.tag == Token_KwNot) {
#line 1879 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1880 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), Parser_parse_not_expr(self)));
    } else if (1) {
        __auto_type _ = _t254;
#line 1882 "src/parser.tr"
        /* pass */
    }
#line 1883 "src/parser.tr"
    return Parser_parse_comparison(self);
}

__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self) {
#line 1886 "src/parser.tr"
    Expr* left = Parser_parse_bitor_expr(self);
#line 1888 "src/parser.tr"
    if (({ __auto_type _ord_t255 = (Parser_peek(self)); __auto_type _wr = ((_ord_t255.tag == Token_make_KwNot().tag)); _wr; })) {
#line 1889 "src/parser.tr"
        long long saved_pos = self->pos;
#line 1890 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1891 "src/parser.tr"
        if (({ __auto_type _ord_t256 = (Parser_peek(self)); __auto_type _wr = ((_ord_t256.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1892 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1893 "src/parser.tr"
            return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("in", 2LL), left, Parser_parse_bitor_expr(self)))));
        }
#line 1894 "src/parser.tr"
        self->pos = saved_pos;
    }
#line 1897 "src/parser.tr"
    List_ptr* operands = (void*)List_ptr_new();
#line 1898 "src/parser.tr"
    List_ptr_append(operands, left);
#line 1899 "src/parser.tr"
    List_TrStr* ops = (void*)List_TrStr_new();
#line 1900 "src/parser.tr"
    while (true) {
#line 1901 "src/parser.tr"
        TrStr op = Parser__peek_cmp_op(self);
#line 1902 "src/parser.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("", 0LL)))) {
#line 1902 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
#line 1903 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1904 "src/parser.tr"
        List_TrStr_append(ops, op);
#line 1905 "src/parser.tr"
        List_ptr_append(operands, Parser_parse_bitor_expr(self));
#line 1906 "src/parser.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("is", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))))) {
#line 1906 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
        _tr_str_release(op);
    }
#line 1907 "src/parser.tr"
    if ((ops->len == 0LL)) {
#line 1907 "src/parser.tr"
        Expr* _trret44 = left;
        List_TrStr_free(ops);
        return _trret44;
    }
#line 1908 "src/parser.tr"
    Expr* result = ({ TrStr _ord_t257 = (List_TrStr_get(ops, 0LL)); __auto_type _ord_t258 = (((Expr*)List_ptr_get(operands, 0LL))); __auto_type _ord_t259 = (((Expr*)List_ptr_get(operands, 1LL))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t257, _ord_t258, _ord_t259))); _tr_str_release(_ord_t257); _wr; });
#line 1909 "src/parser.tr"
    long long ci = 1LL;
#line 1910 "src/parser.tr"
    while ((ci < ops->len)) {
#line 1911 "src/parser.tr"
        Expr* cmp = ({ TrStr _ord_t260 = (List_TrStr_get(ops, ci)); __auto_type _ord_t261 = (((Expr*)List_ptr_get(operands, ci))); __auto_type _ord_t262 = (((Expr*)List_ptr_get(operands, (ci + 1LL)))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t260, _ord_t261, _ord_t262))); _tr_str_release(_ord_t260); _wr; });
#line 1912 "src/parser.tr"
        result = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), result, cmp));
#line 1913 "src/parser.tr"
        ci = (ci + 1LL);
    }
#line 1914 "src/parser.tr"
    Expr* _trret45 = result;
    List_TrStr_free(ops);
    return _trret45;
}

__attribute__((hot)) TrStr Parser__peek_cmp_op(Parser* self) {
#line 1917 "src/parser.tr"
    __auto_type _t263 = Parser_peek(self);
    if (_t263.tag == Token_EqEq) {
        return _tr_str_lit_len("==", 2LL);
    } else if (_t263.tag == Token_NotEq) {
        return _tr_str_lit_len("!=", 2LL);
    } else if (_t263.tag == Token_Lt) {
        return _tr_str_lit_len("<", 1LL);
    } else if (_t263.tag == Token_Gt) {
        return _tr_str_lit_len(">", 1LL);
    } else if (_t263.tag == Token_LtEq) {
        return _tr_str_lit_len("<=", 2LL);
    } else if (_t263.tag == Token_GtEq) {
        return _tr_str_lit_len(">=", 2LL);
    } else if (_t263.tag == Token_KwIs) {
        return _tr_str_lit_len("is", 2LL);
    } else if (_t263.tag == Token_KwIn) {
        return _tr_str_lit_len("in", 2LL);
    } else if (1) {
        __auto_type _ = _t263;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self) {
#line 1929 "src/parser.tr"
    Expr* left = Parser_parse_bitxor_expr(self);
#line 1930 "src/parser.tr"
    while (({ __auto_type _ord_t264 = (Parser_peek(self)); __auto_type _wr = ((_ord_t264.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1931 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1932 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("|", 1LL), left, Parser_parse_bitxor_expr(self)));
    }
#line 1933 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self) {
#line 1936 "src/parser.tr"
    Expr* left = Parser_parse_bitand_expr(self);
#line 1937 "src/parser.tr"
    while (({ __auto_type _ord_t265 = (Parser_peek(self)); __auto_type _wr = ((_ord_t265.tag == Token_make_Caret().tag)); _wr; })) {
#line 1938 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1939 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("^", 1LL), left, Parser_parse_bitand_expr(self)));
    }
#line 1940 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self) {
#line 1943 "src/parser.tr"
    Expr* left = Parser_parse_shift_expr(self);
#line 1944 "src/parser.tr"
    while (({ __auto_type _ord_t266 = (Parser_peek(self)); __auto_type _wr = ((_ord_t266.tag == Token_make_Amp().tag)); _wr; })) {
#line 1945 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1946 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("&", 1LL), left, Parser_parse_shift_expr(self)));
    }
#line 1947 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self) {
#line 1950 "src/parser.tr"
    Expr* left = Parser_parse_additive(self);
#line 1951 "src/parser.tr"
    while (true) {
#line 1952 "src/parser.tr"
        __auto_type _t267 = Parser_peek(self);
        if (_t267.tag == Token_LtLt) {
#line 1954 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1955 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("<<", 2LL), left, Parser_parse_additive(self)));
        } else if (_t267.tag == Token_GtGt) {
#line 1957 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1958 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len(">>", 2LL), left, Parser_parse_additive(self)));
        } else if (1) {
            __auto_type _ = _t267;
            break;
        }
    }
#line 1960 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_additive(Parser* self) {
#line 1963 "src/parser.tr"
    Expr* left = Parser_parse_multiplicative(self);
#line 1964 "src/parser.tr"
    while (true) {
#line 1965 "src/parser.tr"
        __auto_type _t268 = Parser_peek(self);
        if (_t268.tag == Token_Plus) {
#line 1967 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1968 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 1969 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("+", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (_t268.tag == Token_Minus) {
#line 1971 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1972 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 1973 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("-", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (1) {
            __auto_type _ = _t268;
            break;
        }
    }
#line 1975 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self) {
#line 1978 "src/parser.tr"
    Expr* left = Parser_parse_unary(self);
#line 1979 "src/parser.tr"
    while (true) {
#line 1980 "src/parser.tr"
        __auto_type _t269 = Parser_peek(self);
        if (_t269.tag == Token_Star) {
#line 1982 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1983 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("*", 1LL), left, Parser_parse_unary(self)));
        } else if (_t269.tag == Token_Slash) {
#line 1985 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1986 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("/", 1LL), left, Parser_parse_unary(self)));
        } else if (_t269.tag == Token_Percent) {
#line 1988 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1989 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("%", 1LL), left, Parser_parse_unary(self)));
        } else if (_t269.tag == Token_FloorDiv) {
#line 1991 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1992 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("//", 2LL), left, Parser_parse_unary(self)));
        } else if (1) {
            __auto_type _ = _t269;
            break;
        }
    }
#line 1994 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_power(Parser* self) {
#line 1997 "src/parser.tr"
    Expr* base = Parser_parse_postfix(self);
#line 1998 "src/parser.tr"
    if (({ __auto_type _ord_t270 = (Parser_peek(self)); __auto_type _wr = ((_ord_t270.tag == Token_make_StarStar().tag)); _wr; })) {
#line 1999 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2001 "src/parser.tr"
        Expr* exp = Parser_parse_unary(self);
#line 2002 "src/parser.tr"
        return box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("**", 2LL), base, exp));
    }
#line 2003 "src/parser.tr"
    return base;
}

__attribute__((hot)) Expr* Parser_parse_unary(Parser* self) {
#line 2006 "src/parser.tr"
    __auto_type _t271 = Parser_peek(self);
    if (_t271.tag == Token_Minus) {
#line 2008 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2009 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("-", 1LL), Parser_parse_unary(self)));
    } else if (_t271.tag == Token_Tilde) {
#line 2011 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2012 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("~", 1LL), Parser_parse_unary(self)));
    } else if (_t271.tag == Token_Amp) {
#line 2014 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2015 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("&", 1LL), Parser_parse_unary(self)));
    } else if (_t271.tag == Token_Star) {
#line 2017 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2018 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("*", 1LL), Parser_parse_unary(self)));
    } else if (1) {
        __auto_type _ = _t271;
#line 2020 "src/parser.tr"
        /* pass */
    }
#line 2021 "src/parser.tr"
    return Parser_parse_power(self);
}

__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self) {
#line 2024 "src/parser.tr"
    Expr* e = Parser_parse_primary(self);
#line 2025 "src/parser.tr"
    while (true) {
#line 2026 "src/parser.tr"
        __auto_type _t272 = Parser_peek(self);
        if (_t272.tag == Token_Dot) {
#line 2028 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2029 "src/parser.tr"
            TrStr attr = Parser_consume_method_ident(self);
#line 2030 "src/parser.tr"
            __auto_type _t273 = Parser_peek(self);
            if (_t273.tag == Token_LParen) {
#line 2032 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2033 "src/parser.tr"
                e = box_expr(Expr_ctor_EMethodCall(e, attr, Parser_parse_arg_list(self)));
            } else if (1) {
                __auto_type _ = _t273;
#line 2035 "src/parser.tr"
                e = box_expr(Expr_ctor_EPropAccess(e, attr));
            }
            _tr_str_release(attr);
        } else if (_t272.tag == Token_LParen) {
#line 2037 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2042 "src/parser.tr"
            __auto_type _t274 = (*e);
            if (_t274.tag == Expr_EIdent) {
                __auto_type _pf_nm = _t274.data.EIdent.name;
#line 2044 "src/parser.tr"
                if (_tr_dict_contains(self->import_aliases, _tr_strz(_pf_nm))) {
#line 2045 "src/parser.tr"
                    e = ({ TrStr _at_t275 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->import_aliases, _tr_strz(_pf_nm))))); __auto_type _wr = (box_expr(Expr_ctor_EIdent(_at_t275))); _tr_str_release(_at_t275); _wr; });
                }
            } else if (1) {
                __auto_type _ = _t274;
#line 2047 "src/parser.tr"
                /* pass */
            }
#line 2048 "src/parser.tr"
            e = box_expr(Expr_ctor_ECall(e, Parser_parse_arg_list(self)));
        } else if (_t272.tag == Token_LBracket) {
#line 2050 "src/parser.tr"
            long long ix_oln = Parser_cur_line(self);
#line 2051 "src/parser.tr"
            long long ix_ocol = Parser_cur_col(self);
#line 2052 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2057 "src/parser.tr"
            Expr* idx = ((Expr*)(0LL));
#line 2058 "src/parser.tr"
            if (({ __auto_type _ord_t276 = (Parser_peek(self)); __auto_type _wr = ((_ord_t276.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2059 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self))));
            } else {
#line 2061 "src/parser.tr"
                idx = Parser_parse_expr(self);
            }
#line 2071 "src/parser.tr"
            List_ptr* _ti_extra = (void*)List_ptr_new();
#line 2072 "src/parser.tr"
            while (({ __auto_type _ord_t277 = (Parser_peek(self)); __auto_type _wr = ((_ord_t277.tag == Token_make_Comma().tag)); _wr; })) {
#line 2073 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2074 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
#line 2075 "src/parser.tr"
                if (({ __auto_type _ord_t278 = (Parser_peek(self)); __auto_type _wr = ((_ord_t278.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2076 "src/parser.tr"
                    List_ptr_append(_ti_extra, box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self)))));
                } else {
#line 2078 "src/parser.tr"
                    List_ptr_append(_ti_extra, Parser_parse_expr(self));
                }
            }
#line 2079 "src/parser.tr"
            Parser_expect_rbracket(self, ix_oln, ix_ocol, _tr_str_lit_len("this index", 10LL));
#line 2080 "src/parser.tr"
            if ((_ti_extra->len > 0LL)) {
#line 2081 "src/parser.tr"
                List_ptr* _ti_all = (void*)List_ptr_new();
#line 2082 "src/parser.tr"
                List_ptr_append(_ti_all, idx);
#line 2083 "src/parser.tr"
                long long _tj = 0LL;
#line 2084 "src/parser.tr"
                while ((_tj < _ti_extra->len)) {
#line 2085 "src/parser.tr"
                    List_ptr_append(_ti_all, ((Expr*)List_ptr_get(_ti_extra, _tj)));
#line 2086 "src/parser.tr"
                    _tj = (_tj + 1LL);
                }
#line 2087 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETuple(_ti_all));
            }
#line 2088 "src/parser.tr"
            e = box_expr(Expr_ctor_EIndex(e, idx));
        } else if (_t272.tag == Token_Question) {
#line 2090 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2091 "src/parser.tr"
            e = box_expr(Expr_ctor_ETryExpr(e));
        } else if (_t272.tag == Token_KwAs) {
#line 2093 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2094 "src/parser.tr"
            e = box_expr(Expr_ctor_ECast(e, box_asttype(Parser_parse_type(self))));
        } else if (1) {
            __auto_type _ = _t272;
            break;
        }
    }
#line 2096 "src/parser.tr"
    return e;
}

__attribute__((hot)) void Parser_emit_diag_at(Parser* self, long long ln, long long col, TrStr msg, TrStr hint) {
#line 2102 "src/parser.tr"
    ({ __auto_type _ord_t279 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t279); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
#line 2103 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 2104 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 2105 "src/parser.tr"
        TrStr _strtmp_t280 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t280;
    }
#line 2106 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 2107 "src/parser.tr"
    if ((col > 0LL)) {
#line 2108 "src/parser.tr"
        ({ TrStr _ap_t281 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t282 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t281); _tr_str_append(&head, _ap_t282); _tr_str_release(_ap_t282); });
    }
#line 2109 "src/parser.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 2110 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 2111 "src/parser.tr"
        TrStr srcline = _nth_source_line(self->src_text, ln);
#line 2112 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 2113 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2114 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2115 "src/parser.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 2116 "src/parser.tr"
            if ((col > 0LL)) {
#line 2117 "src/parser.tr"
                ({ __auto_type _ord_t283 = (c_dim(gbar)); __auto_type _ord_t284 = (({ TrStr _tr_cl = (_ord_t283); TrStr _tr_cr = (_spaces((col - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t284); TrStr _tr_cr = (c_red(_tr_str_lit_len("^", 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
            }
            _tr_str_release(gnum);
        }
    }
#line 2118 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 2119 "src/parser.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
    }
#line 2120 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) void Parser_expect_rparen(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2127 "src/parser.tr"
    __auto_type _t285 = Parser_peek(self);
    if (_t285.tag == Token_RParen) {
#line 2129 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t285;
#line 2131 "src/parser.tr"
        ({ TrStr _at_t286 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '(' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t286, _tr_str_lit_len("add the matching ')'.", 21LL)); _tr_str_release(_at_t286); });
    }
}

__attribute__((hot)) void Parser_expect_rbracket(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2133 "src/parser.tr"
    __auto_type _t287 = Parser_peek(self);
    if (_t287.tag == Token_RBracket) {
#line 2135 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t287;
#line 2137 "src/parser.tr"
        ({ TrStr _at_t288 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t288, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t288); });
    }
}

__attribute__((hot)) void Parser_expect_rbrace(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2139 "src/parser.tr"
    __auto_type _t289 = Parser_peek(self);
    if (_t289.tag == Token_RBrace) {
#line 2141 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t289;
#line 2143 "src/parser.tr"
        ({ TrStr _at_t290 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '{' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t290, _tr_str_lit_len("add the matching '}'.", 21LL)); _tr_str_release(_at_t290); });
    }
}

__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self) {
#line 2148 "src/parser.tr"
    long long oln = 0LL;
#line 2149 "src/parser.tr"
    long long ocol = 0LL;
#line 2150 "src/parser.tr"
    if ((self->pos > 0LL)) {
#line 2151 "src/parser.tr"
        oln = List_i64_get(self->lines, (self->pos - 1LL));
#line 2152 "src/parser.tr"
        ocol = List_i64_get(self->cols, (self->pos - 1LL));
    }
#line 2153 "src/parser.tr"
    List_ptr* el = (void*)List_ptr_new();
#line 2154 "src/parser.tr"
    bool going = true;
#line 2155 "src/parser.tr"
    while (going) {
#line 2156 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 2157 "src/parser.tr"
        __auto_type _t291 = Parser_peek(self);
        if ((_t291.tag == Token_RParen || _t291.tag == Token_Eof)) {
#line 2159 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t291;
#line 2161 "src/parser.tr"
            /* pass */
        }
#line 2162 "src/parser.tr"
        if (going) {
#line 2163 "src/parser.tr"
            List_ptr_append(el, Parser_parse_expr(self));
#line 2164 "src/parser.tr"
            __auto_type _t292 = Parser_peek(self);
            if (_t292.tag == Token_Comma) {
#line 2166 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2167 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            } else if (1) {
                __auto_type _ = _t292;
#line 2169 "src/parser.tr"
                going = false;
            }
        }
    }
#line 2170 "src/parser.tr"
    Parser_expect_rparen(self, oln, ocol, _tr_str_lit_len("this call's argument list", 25LL));
#line 2171 "src/parser.tr"
    return el;
}

__attribute__((hot)) Expr* Parser_parse_primary(Parser* self) {
#line 2174 "src/parser.tr"
    __auto_type _t293 = Parser_peek(self);
    if (_t293.tag == Token_IntLit) {
        __auto_type v = _t293.data.IntLit.val;
#line 2176 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2177 "src/parser.tr"
        return box_expr(Expr_ctor_ELitInt(v));
    } else if (_t293.tag == Token_FloatLit) {
        __auto_type v = _t293.data.FloatLit.val;
#line 2179 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2180 "src/parser.tr"
        return box_expr(Expr_ctor_ELitFloat(v));
    } else if (_t293.tag == Token_StrLit) {
        __auto_type s = _t293.data.StrLit.val;
__auto_type blen = _t293.data.StrLit.blen;
#line 2182 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2183 "src/parser.tr"
        Expr* _trret46 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret46;
    } else if (_t293.tag == Token_TripleStrLit) {
        __auto_type s = _t293.data.TripleStrLit.val;
__auto_type blen = _t293.data.TripleStrLit.blen;
#line 2185 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2186 "src/parser.tr"
        Expr* _trret47 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret47;
    } else if (_t293.tag == Token_RawStrLit) {
        __auto_type s = _t293.data.RawStrLit.val;
__auto_type blen = _t293.data.RawStrLit.blen;
#line 2188 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2189 "src/parser.tr"
        Expr* _trret48 = box_expr(Expr_ctor_ERawStr(s, blen));
        return _trret48;
    } else if (_t293.tag == Token_ByteStrLit) {
        __auto_type s = _t293.data.ByteStrLit.val;
__auto_type blen = _t293.data.ByteStrLit.blen;
#line 2191 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2192 "src/parser.tr"
        Expr* _trret49 = box_expr(Expr_ctor_ELitBytes(s, blen));
        return _trret49;
    } else if (_t293.tag == Token_FStrLit) {
        __auto_type s = _t293.data.FStrLit.val;
__auto_type blen = _t293.data.FStrLit.blen;
#line 2194 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2195 "src/parser.tr"
        Expr* _trret50 = Parser_parse_fstring(self, s, blen);
        return _trret50;
    } else if (_t293.tag == Token_CharLit) {
        __auto_type v = _t293.data.CharLit.val;
#line 2197 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2198 "src/parser.tr"
        return box_expr(Expr_ctor_ELitChar(v));
    } else if (_t293.tag == Token_BoolLit) {
        __auto_type v = _t293.data.BoolLit.val;
#line 2200 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2201 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(v));
    } else if (_t293.tag == Token_KwTrue) {
#line 2203 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2204 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(true));
    } else if (_t293.tag == Token_KwFalse) {
#line 2206 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2207 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(false));
    } else if (_t293.tag == Token_KwNone) {
#line 2209 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2210 "src/parser.tr"
        return box_expr(Expr_make_ELitNone());
    } else if (_t293.tag == Token_KwInt) {
#line 2212 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2213 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("int", 3LL)));
    } else if (_t293.tag == Token_KwFloat) {
#line 2215 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2216 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("float", 5LL)));
    } else if (_t293.tag == Token_KwBool) {
#line 2218 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2219 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("bool", 4LL)));
    } else if ((_t293.tag == Token_KwStr || _t293.tag == Token_KwString)) {
#line 2221 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2222 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("str", 3LL)));
    } else if (_t293.tag == Token_KwChar) {
#line 2224 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2225 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("char", 4LL)));
    } else if (_t293.tag == Token_KwVoid) {
#line 2227 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2228 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("void", 4LL)));
    } else if (_t293.tag == Token_KwSuper) {
#line 2230 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2231 "src/parser.tr"
        TrStr super_base = _tr_str_lit_len("", 0LL);
#line 2232 "src/parser.tr"
        if (({ __auto_type _ord_t294 = (Parser_peek(self)); __auto_type _wr = ((_ord_t294.tag == Token_make_LParen().tag)); _wr; })) {
#line 2233 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2234 "src/parser.tr"
            TrStr _strtmp_t295 = Parser_consume_ident(self);
            _tr_str_release(super_base);
            super_base = _strtmp_t295;
#line 2235 "src/parser.tr"
            if (({ __auto_type _ord_t296 = (Parser_peek(self)); __auto_type _wr = ((_ord_t296.tag == Token_make_RParen().tag)); _wr; })) {
#line 2236 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 2237 "src/parser.tr"
            if (({ __auto_type _ord_t297 = (Parser_peek(self)); __auto_type _wr = ((_ord_t297.tag == Token_make_Dot().tag)); _wr; })) {
#line 2238 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2239 "src/parser.tr"
                TrStr super_method = Parser_consume_ident(self);
#line 2240 "src/parser.tr"
                if (({ __auto_type _ord_t298 = (Parser_peek(self)); __auto_type _wr = ((_ord_t298.tag == Token_make_LParen().tag)); _wr; })) {
#line 2241 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2242 "src/parser.tr"
                    List_ptr* super_args = Parser_parse_arg_list(self);
#line 2243 "src/parser.tr"
                    Expr* _trret51 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method, super_args));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret51;
                } else {
#line 2245 "src/parser.tr"
                    Expr* _trret52 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret52;
                }
            }
        }
#line 2246 "src/parser.tr"
        if (({ __auto_type _ord_t299 = (Parser_peek(self)); __auto_type _wr = ((_ord_t299.tag == Token_make_Dot().tag)); _wr; })) {
#line 2247 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2248 "src/parser.tr"
            TrStr super_next = Parser_consume_ident(self);
#line 2249 "src/parser.tr"
            if (({ __auto_type _ord_t300 = (Parser_peek(self)); __auto_type _wr = ((_ord_t300.tag == Token_make_Dot().tag)); _wr; })) {
#line 2250 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2251 "src/parser.tr"
                TrStr _strtmp_t301 = _tr_str_retain(super_next);
                _tr_str_release(super_base);
                super_base = _strtmp_t301;
#line 2252 "src/parser.tr"
                TrStr super_method2 = Parser_consume_ident(self);
#line 2253 "src/parser.tr"
                if (({ __auto_type _ord_t302 = (Parser_peek(self)); __auto_type _wr = ((_ord_t302.tag == Token_make_LParen().tag)); _wr; })) {
#line 2254 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2255 "src/parser.tr"
                    List_ptr* super_args2 = Parser_parse_arg_list(self);
#line 2256 "src/parser.tr"
                    Expr* _trret53 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method2, super_args2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret53;
                } else {
#line 2258 "src/parser.tr"
                    Expr* _trret54 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret54;
                }
            } else {
#line 2260 "src/parser.tr"
                if (({ __auto_type _ord_t303 = (Parser_peek(self)); __auto_type _wr = ((_ord_t303.tag == Token_make_LParen().tag)); _wr; })) {
#line 2261 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2262 "src/parser.tr"
                    List_ptr* super_args3 = Parser_parse_arg_list(self);
#line 2263 "src/parser.tr"
                    Expr* _trret55 = box_expr(Expr_ctor_ESuperMethodCall(_tr_str_lit_len("", 0LL), super_next, super_args3));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret55;
                } else {
#line 2265 "src/parser.tr"
                    Expr* _trret56 = box_expr(Expr_ctor_ESuperPropAccess(_tr_str_lit_len("", 0LL), super_next));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret56;
                }
            }
        }
#line 2266 "src/parser.tr"
        Expr* _trret57 = box_expr(Expr_ctor_EIdent(_tr_str_lit_len("super", 5LL)));
        _tr_str_release(super_base);
        return _trret57;
    } else if (_t293.tag == Token_Ident) {
        __auto_type name = _t293.data.Ident.name;
#line 2268 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2271 "src/parser.tr"
        if (({ __auto_type _ord_t304 = (Parser_peek(self)); __auto_type _wr = ((_ord_t304.tag == Token_make_Bang().tag)); _wr; })) {
#line 2272 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2273 "src/parser.tr"
            if (({ __auto_type _ord_t305 = (Parser_peek(self)); __auto_type _wr = ((_ord_t305.tag == Token_make_LParen().tag)); _wr; })) {
#line 2274 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2275 "src/parser.tr"
                List_ptr* mac_args = Parser_parse_arg_list(self);
#line 2276 "src/parser.tr"
                Expr* _trret58 = box_expr(Expr_ctor_EMacroCall(name, mac_args));
                return _trret58;
            }
        }
#line 2277 "src/parser.tr"
        Expr* _trret59 = box_expr(Expr_ctor_EIdent(name));
        return _trret59;
    } else if (_t293.tag == Token_KwSizeOf) {
#line 2279 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2280 "src/parser.tr"
        __auto_type _t306 = Parser_peek(self);
        if (_t306.tag == Token_LParen) {
#line 2282 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t306;
#line 2284 "src/parser.tr"
            /* pass */
        }
#line 2285 "src/parser.tr"
        AstType** ty = box_asttype(Parser_parse_type(self));
#line 2286 "src/parser.tr"
        __auto_type _t307 = Parser_peek(self);
        if (_t307.tag == Token_RParen) {
#line 2288 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t307;
#line 2290 "src/parser.tr"
            /* pass */
        }
#line 2291 "src/parser.tr"
        return box_expr(Expr_ctor_ESizeOf(ty));
    } else if (_t293.tag == Token_LParen) {
#line 2293 "src/parser.tr"
        long long p_oln = Parser_cur_line(self);
#line 2294 "src/parser.tr"
        long long p_ocol = Parser_cur_col(self);
#line 2295 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2296 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2297 "src/parser.tr"
        if (({ __auto_type _ord_t308 = (Parser_peek(self)); __auto_type _wr = ((_ord_t308.tag == Token_make_RParen().tag)); _wr; })) {
#line 2298 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2299 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple((void*)List_ptr_new()));
        }
#line 2300 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 2301 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2302 "src/parser.tr"
        if (({ __auto_type _ord_t309 = (Parser_peek(self)); __auto_type _wr = ((_ord_t309.tag == Token_make_Comma().tag)); _wr; })) {
#line 2303 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2304 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2305 "src/parser.tr"
            List_ptr* items = (void*)List_ptr_new();
#line 2306 "src/parser.tr"
            List_ptr_append(items, e);
#line 2307 "src/parser.tr"
            while ((({ __auto_type _ord_t310 = (Parser_peek(self)); __auto_type _wr = ((_ord_t310.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t311 = (Parser_peek(self)); __auto_type _wr = ((_ord_t311.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2308 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2309 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2310 "src/parser.tr"
                if (({ __auto_type _ord_t312 = (Parser_peek(self)); __auto_type _wr = ((_ord_t312.tag == Token_make_Comma().tag)); _wr; })) {
#line 2311 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2312 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2314 "src/parser.tr"
                    break;
                }
            }
#line 2315 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this tuple", 10LL));
#line 2316 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple(items));
        } else {
#line 2318 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this parenthesized expression", 29LL));
#line 2319 "src/parser.tr"
            return e;
        }
    } else if (_t293.tag == Token_LBracket) {
#line 2321 "src/parser.tr"
        long long lb_oln = Parser_cur_line(self);
#line 2322 "src/parser.tr"
        long long lb_ocol = Parser_cur_col(self);
#line 2323 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2324 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2325 "src/parser.tr"
        if (({ __auto_type _ord_t313 = (Parser_peek(self)); __auto_type _wr = ((_ord_t313.tag == Token_make_RBracket().tag)); _wr; })) {
#line 2326 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2327 "src/parser.tr"
            return box_expr(Expr_ctor_EList((void*)List_ptr_new()));
        }
#line 2328 "src/parser.tr"
        Expr* first = Parser_parse_or_expr(self);
#line 2329 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2330 "src/parser.tr"
        if (({ __auto_type _ord_t314 = (Parser_peek(self)); __auto_type _wr = ((_ord_t314.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2331 "src/parser.tr"
            List_ptr* generators = (void*)List_ptr_new();
#line 2332 "src/parser.tr"
            while (({ __auto_type _ord_t315 = (Parser_peek(self)); __auto_type _wr = ((_ord_t315.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2333 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2334 "src/parser.tr"
                TrStr target = Parser_consume_ident(self);
#line 2335 "src/parser.tr"
                __auto_type _t316 = Parser_peek(self);
                if (_t316.tag == Token_KwIn) {
#line 2337 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t316;
#line 2339 "src/parser.tr"
                    /* pass */
                }
#line 2340 "src/parser.tr"
                Expr* iter = Parser_parse_or_expr(self);
#line 2341 "src/parser.tr"
                List_ptr* ifs = (void*)List_ptr_new();
#line 2342 "src/parser.tr"
                while (({ __auto_type _ord_t317 = (Parser_peek(self)); __auto_type _wr = ((_ord_t317.tag == Token_make_KwIf().tag)); _wr; })) {
#line 2343 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2344 "src/parser.tr"
                    List_ptr_append(ifs, Parser_parse_or_expr(self));
                }
#line 2345 "src/parser.tr"
                Comprehension* c = Comprehension_init(target, iter);
#line 2346 "src/parser.tr"
                c->ifs = ifs;
#line 2347 "src/parser.tr"
                /* unsafe block */
#line 2348 "src/parser.tr"
                Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 2349 "src/parser.tr"
                (*c_ptr = c);
#line 2350 "src/parser.tr"
                List_ptr_append(generators, c_ptr);
                _tr_str_release(target);
            }
#line 2351 "src/parser.tr"
            Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list comprehension", 23LL));
#line 2352 "src/parser.tr"
            return box_expr(Expr_ctor_EListComp(first, generators));
        }
#line 2353 "src/parser.tr"
        List_ptr* items = (void*)List_ptr_new();
#line 2354 "src/parser.tr"
        List_ptr_append(items, first);
#line 2355 "src/parser.tr"
        if (({ __auto_type _ord_t318 = (Parser_peek(self)); __auto_type _wr = ((_ord_t318.tag == Token_make_Comma().tag)); _wr; })) {
#line 2356 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2357 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2358 "src/parser.tr"
            while ((({ __auto_type _ord_t319 = (Parser_peek(self)); __auto_type _wr = ((_ord_t319.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t320 = (Parser_peek(self)); __auto_type _wr = ((_ord_t320.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2359 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2360 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2361 "src/parser.tr"
                if (({ __auto_type _ord_t321 = (Parser_peek(self)); __auto_type _wr = ((_ord_t321.tag == Token_make_Comma().tag)); _wr; })) {
#line 2362 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2363 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2365 "src/parser.tr"
                    break;
                }
            }
        }
#line 2366 "src/parser.tr"
        Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list", 9LL));
#line 2367 "src/parser.tr"
        return box_expr(Expr_ctor_EList(items));
    } else if (_t293.tag == Token_LBrace) {
#line 2369 "src/parser.tr"
        long long br_oln = Parser_cur_line(self);
#line 2370 "src/parser.tr"
        long long br_ocol = Parser_cur_col(self);
#line 2371 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2372 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2373 "src/parser.tr"
        List_ptr* keys = (void*)List_ptr_new();
#line 2374 "src/parser.tr"
        List_ptr* vals = (void*)List_ptr_new();
#line 2375 "src/parser.tr"
        List_ptr* set_items = (void*)List_ptr_new();
#line 2376 "src/parser.tr"
        bool is_set = false;
#line 2377 "src/parser.tr"
        if (({ __auto_type _ord_t322 = (Parser_peek(self)); __auto_type _wr = ((_ord_t322.tag != Token_make_RBrace().tag)); _wr; })) {
#line 2378 "src/parser.tr"
            Expr* first = Parser_parse_expr(self);
#line 2379 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2380 "src/parser.tr"
            if (({ __auto_type _ord_t323 = (Parser_peek(self)); __auto_type _wr = ((_ord_t323.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2381 "src/parser.tr"
                List_ptr* sgens = Parser_parse_comp_generators(self);
#line 2382 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2383 "src/parser.tr"
                Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this set comprehension", 22LL));
#line 2384 "src/parser.tr"
                return Parser_desugar_collection_comp(self, first, (Expr*)(0LL), sgens);
            }
#line 2385 "src/parser.tr"
            if (({ __auto_type _ord_t324 = (Parser_peek(self)); __auto_type _wr = ((_ord_t324.tag == Token_make_Colon().tag)); _wr; })) {
#line 2386 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2387 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2388 "src/parser.tr"
                Expr* value = Parser_parse_expr(self);
#line 2389 "src/parser.tr"
                if (({ __auto_type _ord_t325 = (Parser_peek(self)); __auto_type _wr = ((_ord_t325.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2390 "src/parser.tr"
                    List_ptr* dgens = Parser_parse_comp_generators(self);
#line 2391 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2392 "src/parser.tr"
                    Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict comprehension", 23LL));
#line 2393 "src/parser.tr"
                    return Parser_desugar_collection_comp(self, first, value, dgens);
                }
#line 2394 "src/parser.tr"
                List_ptr_append(keys, first);
#line 2395 "src/parser.tr"
                List_ptr_append(vals, value);
#line 2396 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2397 "src/parser.tr"
                if (({ __auto_type _ord_t326 = (Parser_peek(self)); __auto_type _wr = ((_ord_t326.tag == Token_make_Comma().tag)); _wr; })) {
#line 2398 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2399 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2400 "src/parser.tr"
                while ((({ __auto_type _ord_t327 = (Parser_peek(self)); __auto_type _wr = ((_ord_t327.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t328 = (Parser_peek(self)); __auto_type _wr = ((_ord_t328.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2401 "src/parser.tr"
                    Expr* key = Parser_parse_expr(self);
#line 2402 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2403 "src/parser.tr"
                    __auto_type _t329 = Parser_peek(self);
                    if (_t329.tag == Token_Colon) {
#line 2405 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t329;
#line 2407 "src/parser.tr"
                        /* pass */
                    }
#line 2408 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2409 "src/parser.tr"
                    Expr* v = Parser_parse_expr(self);
#line 2410 "src/parser.tr"
                    List_ptr_append(keys, key);
#line 2411 "src/parser.tr"
                    List_ptr_append(vals, v);
#line 2412 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2413 "src/parser.tr"
                    if (({ __auto_type _ord_t330 = (Parser_peek(self)); __auto_type _wr = ((_ord_t330.tag == Token_make_Comma().tag)); _wr; })) {
#line 2414 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2416 "src/parser.tr"
                        break;
                    }
#line 2417 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            } else {
#line 2419 "src/parser.tr"
                is_set = true;
#line 2420 "src/parser.tr"
                List_ptr_append(set_items, first);
#line 2421 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2422 "src/parser.tr"
                if (({ __auto_type _ord_t331 = (Parser_peek(self)); __auto_type _wr = ((_ord_t331.tag == Token_make_Comma().tag)); _wr; })) {
#line 2423 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2424 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2425 "src/parser.tr"
                while ((({ __auto_type _ord_t332 = (Parser_peek(self)); __auto_type _wr = ((_ord_t332.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t333 = (Parser_peek(self)); __auto_type _wr = ((_ord_t333.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2426 "src/parser.tr"
                    List_ptr_append(set_items, Parser_parse_expr(self));
#line 2427 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2428 "src/parser.tr"
                    if (({ __auto_type _ord_t334 = (Parser_peek(self)); __auto_type _wr = ((_ord_t334.tag == Token_make_Comma().tag)); _wr; })) {
#line 2429 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2431 "src/parser.tr"
                        break;
                    }
#line 2432 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            }
        }
#line 2433 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2434 "src/parser.tr"
        Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict/set literal", 21LL));
#line 2435 "src/parser.tr"
        if (is_set) {
#line 2436 "src/parser.tr"
            return box_expr(Expr_ctor_ESet(set_items));
        } else {
#line 2438 "src/parser.tr"
            return box_expr(Expr_ctor_EDict(keys, vals));
        }
    } else if (_t293.tag == Token_KwTry) {
#line 2440 "src/parser.tr"
        Stmt* st = Parser_parse_try_stmt(self);
#line 2441 "src/parser.tr"
        __auto_type _t335 = (*st);
        if (_t335.tag == Stmt_STry) {
            __auto_type try_body = _t335.data.STry.try_body;
__auto_type catches = _t335.data.STry.catches;
__auto_type finally_b = _t335.data.STry.finally_b;
#line 2443 "src/parser.tr"
            Expr* _trret60 = box_expr(Expr_ctor_ETry(try_body, catches, finally_b));
            return _trret60;
        } else if (1) {
            __auto_type _ = _t335;
#line 2445 "src/parser.tr"
            /* pass */
        }
    } else if (_t293.tag == Token_KwAwait) {
#line 2447 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2448 "src/parser.tr"
        return box_expr(Expr_ctor_EAwait(Parser_parse_expr(self)));
    } else if (_t293.tag == Token_KwYield) {
#line 2450 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2451 "src/parser.tr"
        return box_expr(Expr_ctor_EYield(Parser_parse_expr(self)));
    } else if ((_t293.tag == Token_KwDef || _t293.tag == Token_KwAsync)) {
#line 2453 "src/parser.tr"
        bool is_async = false;
#line 2454 "src/parser.tr"
        if (({ __auto_type _ord_t336 = (Parser_peek(self)); __auto_type _wr = ((_ord_t336.tag == Token_make_KwAsync().tag)); _wr; })) {
#line 2455 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2456 "src/parser.tr"
            is_async = true;
        }
#line 2457 "src/parser.tr"
        __auto_type _t337 = Parser_peek(self);
        if (_t337.tag == Token_KwDef) {
#line 2459 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t337;
#line 2461 "src/parser.tr"
            /* pass */
        }
#line 2462 "src/parser.tr"
        __auto_type _t338 = Parser_peek(self);
        if (_t338.tag == Token_LParen) {
#line 2464 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t338;
#line 2466 "src/parser.tr"
            /* pass */
        }
#line 2467 "src/parser.tr"
        List_ptr* params = Parser_parse_param_list(self);
#line 2468 "src/parser.tr"
        Parser_close_params(self);
#line 2469 "src/parser.tr"
        AstType** return_type = (AstType**)(0LL);
#line 2470 "src/parser.tr"
        AstType** clo_throws = (AstType**)(0LL);
#line 2471 "src/parser.tr"
        if (({ __auto_type _ord_t339 = (Parser_peek(self)); __auto_type _wr = ((_ord_t339.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 2472 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2473 "src/parser.tr"
            clo_throws = box_asttype(Parser_parse_type(self));
        }
#line 2474 "src/parser.tr"
        if (({ __auto_type _ord_t340 = (Parser_peek(self)); __auto_type _wr = ((_ord_t340.tag == Token_make_Arrow().tag)); _wr; })) {
#line 2475 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2476 "src/parser.tr"
            return_type = box_asttype(Parser_parse_type(self));
        }
#line 2477 "src/parser.tr"
        if ((((unsigned long long)(clo_throws)) != ((unsigned long long)(0LL)))) {
#line 2478 "src/parser.tr"
            AstType* clo_rt = AstType_init(_tr_str_lit_len("void", 4LL));
#line 2479 "src/parser.tr"
            if ((((unsigned long long)(return_type)) != ((unsigned long long)(0LL)))) {
#line 2479 "src/parser.tr"
                clo_rt = (*return_type);
            }
#line 2480 "src/parser.tr"
            return_type = box_asttype(throws_result_type(clo_rt, clo_throws));
        }
#line 2481 "src/parser.tr"
        __auto_type _t341 = Parser_peek(self);
        if (_t341.tag == Token_Colon) {
#line 2483 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t341;
#line 2485 "src/parser.tr"
            /* pass */
        }
#line 2486 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 2487 "src/parser.tr"
        Expr* _trret61 = box_expr(Expr_ctor_EClosure(params, return_type, body, is_async));
        return _trret61;
    } else if (_t293.tag == Token_KwLambda) {
#line 2491 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2492 "src/parser.tr"
        List_ptr* lparams = (void*)List_ptr_new();
#line 2493 "src/parser.tr"
        while (((({ __auto_type _ord_t342 = (Parser_peek(self)); __auto_type _wr = ((_ord_t342.tag != Token_make_Colon().tag)); _wr; }) && ({ __auto_type _ord_t343 = (Parser_peek(self)); __auto_type _wr = ((_ord_t343.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t344 = (Parser_peek(self)); __auto_type _wr = ((_ord_t344.tag != Token_make_Newline().tag)); _wr; }))) {
#line 2494 "src/parser.tr"
            TrStr lpn = Parser_consume_ident(self);
#line 2495 "src/parser.tr"
            if (_tr_str_eqv((lpn), (_tr_str_lit_len("", 0LL)))) {
#line 2495 "src/parser.tr"
                _tr_str_release(lpn);
                break;
            }
#line 2496 "src/parser.tr"
            List_ptr_append(lparams, Param_init(lpn, (AstType**)(0LL)));
#line 2497 "src/parser.tr"
            if (({ __auto_type _ord_t345 = (Parser_peek(self)); __auto_type _wr = ((_ord_t345.tag == Token_make_Comma().tag)); _wr; })) {
#line 2498 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(lpn);
        }
#line 2499 "src/parser.tr"
        __auto_type _t346 = Parser_peek(self);
        if (_t346.tag == Token_Colon) {
#line 2501 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t346;
#line 2503 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected ':' after the parameters of a 'lambda'", 47LL), _tr_str_lit_len("write `lambda x, y: expr`", 25LL));
        }
#line 2504 "src/parser.tr"
        Expr* lbody_e = Parser_parse_expr(self);
#line 2505 "src/parser.tr"
        Block* lbody = Block_init();
#line 2506 "src/parser.tr"
        Block_push(lbody, box_stmt(Stmt_ctor_SReturn(lbody_e)));
#line 2507 "src/parser.tr"
        Expr* _trret62 = box_expr(Expr_ctor_EClosure(lparams, (AstType**)(0LL), lbody, false));
        return _trret62;
    } else if (_t293.tag == Token_FStrLit) {
        __auto_type raw = _t293.data.FStrLit.val;
__auto_type raw_blen = _t293.data.FStrLit.blen;
#line 2509 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2510 "src/parser.tr"
        Expr* _trret63 = Parser_parse_fstring(self, raw, raw_blen);
        return _trret63;
    } else if (1) {
        __auto_type _ = _t293;
#line 2512 "src/parser.tr"
        /* pass */
    }
#line 2513 "src/parser.tr"
    if ((!Parser_at_end(self))) {
#line 2514 "src/parser.tr"
        TrStr tok_desc = _tr_str_lit_len("token", 5LL);
#line 2515 "src/parser.tr"
        TrStr hint = _tr_str_lit_len("check for a typo, a missing ':' or unbalanced parentheses/brackets near this point.", 83LL);
#line 2516 "src/parser.tr"
        __auto_type _t347 = Parser_peek(self);
        if (_t347.tag == Token_Newline) {
#line 2518 "src/parser.tr"
            TrStr _strtmp_t348 = _tr_str_lit_len("end of line", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t348;
#line 2519 "src/parser.tr"
            TrStr _strtmp_t349 = _tr_str_lit_len("an expression was expected before the end of this line - check for a missing value or trailing operator.", 104LL);
            _tr_str_release(hint);
            hint = _strtmp_t349;
        } else if (_t347.tag == Token_Indent) {
#line 2521 "src/parser.tr"
            TrStr _strtmp_t350 = _tr_str_lit_len("indentation", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t350;
#line 2522 "src/parser.tr"
            TrStr _strtmp_t351 = _tr_str_lit_len("check that this line's indentation matches the surrounding block.", 65LL);
            _tr_str_release(hint);
            hint = _strtmp_t351;
        } else if (_t347.tag == Token_Dedent) {
#line 2524 "src/parser.tr"
            TrStr _strtmp_t352 = _tr_str_lit_len("dedent", 6LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t352;
#line 2525 "src/parser.tr"
            TrStr _strtmp_t353 = _tr_str_lit_len("check that this block is properly indented and closed.", 54LL);
            _tr_str_release(hint);
            hint = _strtmp_t353;
        } else if (_t347.tag == Token_Ident) {
            __auto_type n = _t347.data.Ident.name;
#line 2527 "src/parser.tr"
            TrStr _strtmp_t354 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("identifier '", 12LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t354;
#line 2528 "src/parser.tr"
            TrStr _strtmp_t355 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("an operator, ':' or end of statement was expected before '", 58LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(hint);
            hint = _strtmp_t355;
        } else if (_t347.tag == Token_KwMut) {
            TrStr _strtmp_t356 = _tr_str_lit_len("keyword 'mut'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t356;
        } else if (_t347.tag == Token_KwConst) {
            TrStr _strtmp_t357 = _tr_str_lit_len("keyword 'const'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t357;
        } else if (_t347.tag == Token_KwPub) {
            TrStr _strtmp_t358 = _tr_str_lit_len("keyword 'pub'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t358;
        } else if (_t347.tag == Token_KwReturn) {
            TrStr _strtmp_t359 = _tr_str_lit_len("keyword 'return'", 16LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t359;
        } else if (_t347.tag == Token_KwIf) {
            TrStr _strtmp_t360 = _tr_str_lit_len("keyword 'if'", 12LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t360;
        } else if (_t347.tag == Token_KwWhile) {
            TrStr _strtmp_t361 = _tr_str_lit_len("keyword 'while'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t361;
        } else if (_t347.tag == Token_Comma) {
#line 2536 "src/parser.tr"
            TrStr _strtmp_t362 = _tr_str_lit_len("','", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t362;
#line 2537 "src/parser.tr"
            TrStr _strtmp_t363 = _tr_str_lit_len("remove the extra ',' or add the missing item before it.", 55LL);
            _tr_str_release(hint);
            hint = _strtmp_t363;
        } else if (_t347.tag == Token_Colon) {
#line 2539 "src/parser.tr"
            TrStr _strtmp_t364 = _tr_str_lit_len("':'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t364;
#line 2540 "src/parser.tr"
            TrStr _strtmp_t365 = _tr_str_lit_len("remove the extra ':' or check the statement before it is complete.", 66LL);
            _tr_str_release(hint);
            hint = _strtmp_t365;
        } else if (_t347.tag == Token_RParen) {
#line 2542 "src/parser.tr"
            TrStr _strtmp_t366 = _tr_str_lit_len("')'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t366;
#line 2543 "src/parser.tr"
            TrStr _strtmp_t367 = _tr_str_lit_len("check for an extra ')' or a missing matching '('.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t367;
        } else if (_t347.tag == Token_RBracket) {
#line 2545 "src/parser.tr"
            TrStr _strtmp_t368 = _tr_str_lit_len("']'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t368;
#line 2546 "src/parser.tr"
            TrStr _strtmp_t369 = _tr_str_lit_len("check for an extra ']' or a missing matching '['.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t369;
        } else if (_t347.tag == Token_RBrace) {
#line 2548 "src/parser.tr"
            TrStr _strtmp_t370 = _tr_str_lit_len("'}'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t370;
#line 2549 "src/parser.tr"
            TrStr _strtmp_t371 = _tr_str_lit_len("check for an extra '}' or a missing matching '{'.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t371;
        } else if (_t347.tag == Token_Error) {
            __auto_type em = _t347.data.Error.msg;
#line 2553 "src/parser.tr"
            Parser_emit_diag(self, em, _tr_str_lit_len("", 0LL));
#line 2554 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2555 "src/parser.tr"
            Expr* _trret64 = box_expr(Expr_make_ELitNone());
            return _trret64;
        } else if (1) {
            __auto_type _ = _t347;
            /* pass */
        }
#line 2557 "src/parser.tr"
        if (_tr_str_eqv((tok_desc), (_tr_str_lit_len("token", 5LL)))) {
#line 2560 "src/parser.tr"
            TrStr kw = _tr_str_lit_len("", 0LL);
#line 2561 "src/parser.tr"
            long long kln = Parser_cur_line(self);
#line 2562 "src/parser.tr"
            long long kcol = Parser_cur_col(self);
#line 2563 "src/parser.tr"
            if ((((_tr_str_lenv((self->src_text)) > 0LL) && (kln > 0LL)) && (kcol > 0LL))) {
#line 2564 "src/parser.tr"
                TrStr ksrc = _nth_source_line(self->src_text, kln);
#line 2565 "src/parser.tr"
                long long kend = (kcol - 1LL);
#line 2566 "src/parser.tr"
                while (((kend < _tr_str_lenv((ksrc))) && (((_tr_str_char_at_codev((ksrc), kend) >= 97LL) && (_tr_str_char_at_codev((ksrc), kend) <= 122LL)) || (_tr_str_char_at_codev((ksrc), kend) == 95LL)))) {
#line 2567 "src/parser.tr"
                    kend = (kend + 1LL);
                }
#line 2568 "src/parser.tr"
                if ((kend > (kcol - 1LL))) {
#line 2568 "src/parser.tr"
                    TrStr _strtmp_t372 = _tr_str_slicev((ksrc), (kcol - 1LL), kend);
                    _tr_str_release(kw);
                    kw = _strtmp_t372;
                }
            }
#line 2569 "src/parser.tr"
            bool kw_ok = ((_tr_str_lenv((kw)) > 1LL) && (_tr_str_lenv((kw)) < 16LL));
#line 2570 "src/parser.tr"
            long long ki = 0LL;
#line 2571 "src/parser.tr"
            while ((kw_ok && (ki < _tr_str_lenv((kw))))) {
#line 2572 "src/parser.tr"
                long long kc = _tr_str_char_at_codev((kw), ki);
#line 2573 "src/parser.tr"
                if ((!(((kc >= 97LL) && (kc <= 122LL)) || (kc == 95LL)))) {
#line 2573 "src/parser.tr"
                    kw_ok = false;
                }
#line 2574 "src/parser.tr"
                ki = (ki + 1LL);
            }
#line 2575 "src/parser.tr"
            if (kw_ok) {
#line 2576 "src/parser.tr"
                TrStr _strtmp_t373 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("keyword '", 9LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(tok_desc);
                tok_desc = _strtmp_t373;
#line 2577 "src/parser.tr"
                TrStr _strtmp_t374 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a reserved word and cannot be used as a name or value here - rename it (for example '", 90LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kw)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_').", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(hint);
                hint = _strtmp_t374;
            }
        }
#line 2578 "src/parser.tr"
        ({ TrStr _at_t375 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unexpected ", 11LL)), (tok_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in expression", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t375, hint); _tr_str_release(_at_t375); });
#line 2579 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 2580 "src/parser.tr"
    return box_expr(Expr_make_ELitNone());
}

__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, TrStr raw, long long raw_blen) {
#line 2602 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 2603 "src/parser.tr"
    char* p = ((char*)(_tr_strz(raw)));
#line 2604 "src/parser.tr"
    long long i = 0LL;
#line 2605 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 2607 "src/parser.tr"
    while ((i < raw_blen)) {
#line 2608 "src/parser.tr"
        long long c = ((long long)((*(p + i))));
#line 2609 "src/parser.tr"
        long long c_next = 0LL;
#line 2610 "src/parser.tr"
        if (((i + 1LL) < raw_blen)) {
#line 2610 "src/parser.tr"
            c_next = ((long long)((*(p + (i + 1LL)))));
        }
#line 2612 "src/parser.tr"
        if ((((c == 123LL) && (c_next == 123LL)) || ((c == 125LL) && (c_next == 125LL)))) {
#line 2614 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2615 "src/parser.tr"
            i = (i + 2LL);
        } else if ((c == 123LL)) {
#line 2618 "src/parser.tr"
            if ((sb->buf->len > 0LL)) {
#line 2619 "src/parser.tr"
                ({ TrStr _ord_t376 = (StringBuilder_to_owned(sb)); __auto_type _ord_t377 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t376, _ord_t377)); _tr_str_release(_ord_t376); });
#line 2620 "src/parser.tr"
                sb = StringBuilder_init(64LL);
            }
#line 2622 "src/parser.tr"
            i = (i + 1LL);
#line 2623 "src/parser.tr"
            StringBuilder* expr_sb = StringBuilder_init(64LL);
#line 2624 "src/parser.tr"
            long long depth = 1LL;
#line 2625 "src/parser.tr"
            while (((depth > 0LL) && (i < raw_blen))) {
#line 2626 "src/parser.tr"
                long long ec = ((long long)((*(p + i))));
#line 2627 "src/parser.tr"
                if (((ec == 34LL) || (ec == 39LL))) {
#line 2630 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
#line 2631 "src/parser.tr"
                    i = (i + 1LL);
#line 2632 "src/parser.tr"
                    while (((i < raw_blen) && (((long long)((*(p + i)))) != ec))) {
#line 2633 "src/parser.tr"
                        long long sc = ((long long)((*(p + i))));
#line 2634 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, sc);
#line 2635 "src/parser.tr"
                        i = (i + 1LL);
#line 2636 "src/parser.tr"
                        if (((sc == 92LL) && (i < raw_blen))) {
#line 2637 "src/parser.tr"
                            StringBuilder_append_char(expr_sb, ((long long)((*(p + i)))));
#line 2638 "src/parser.tr"
                            i = (i + 1LL);
                        }
                    }
#line 2639 "src/parser.tr"
                    if ((i < raw_blen)) {
#line 2640 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, ec);
#line 2641 "src/parser.tr"
                        i = (i + 1LL);
                    }
#line 2642 "src/parser.tr"
                    continue;
                }
#line 2643 "src/parser.tr"
                if ((ec == 123LL)) {
#line 2643 "src/parser.tr"
                    depth = (depth + 1LL);
                }
#line 2644 "src/parser.tr"
                if ((ec == 125LL)) {
#line 2644 "src/parser.tr"
                    depth = (depth - 1LL);
                }
#line 2645 "src/parser.tr"
                if ((depth > 0LL)) {
#line 2645 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
                }
#line 2646 "src/parser.tr"
                i = (i + 1LL);
            }
#line 2650 "src/parser.tr"
            TrStr expr_str = StringObj_as_str(StringBuilder_to_string(expr_sb));
#line 2651 "src/parser.tr"
            TrStr fmt_spec = _tr_str_lit_len("", 0LL);
#line 2652 "src/parser.tr"
            long long colon_pos = _find_fmt_colon(expr_str);
#line 2653 "src/parser.tr"
            if ((colon_pos >= 0LL)) {
#line 2654 "src/parser.tr"
                TrStr _fs = _tr_str_slicev((expr_str), (colon_pos + 1LL), _tr_str_lenv((expr_str)));
#line 2655 "src/parser.tr"
                TrStr _strtmp_t378 = _tr_str_stripv((_fs));
                _tr_str_release(fmt_spec);
                fmt_spec = _strtmp_t378;
#line 2656 "src/parser.tr"
                TrStr _es = _tr_str_slicev((expr_str), 0LL, colon_pos);
#line 2657 "src/parser.tr"
                TrStr _strtmp_t379 = _tr_str_stripv((_es));
                _tr_str_release(expr_str);
                expr_str = _strtmp_t379;
                _tr_str_release(_fs);
                _tr_str_release(_es);
            }
#line 2658 "src/parser.tr"
            Lexer* lexer = Lexer_init(expr_str);
#line 2659 "src/parser.tr"
            List_Token* _fstr_tokens = Lexer_tokenize(lexer);
#line 2660 "src/parser.tr"
            if ((_fstr_tokens->len > 0LL)) {
#line 2661 "src/parser.tr"
                Parser* parser = Parser_init(_fstr_tokens, lexer->token_lines);
#line 2662 "src/parser.tr"
                Expr* e = Parser_parse_expr(parser);
#line 2663 "src/parser.tr"
                if ((_tr_str_lenv((fmt_spec)) > 0LL)) {
#line 2664 "src/parser.tr"
                    FStringPart* fpart = FStringPart_init_expr_fmt(e, fmt_spec);
#line 2666 "src/parser.tr"
                    long long sk = 0LL;
#line 2667 "src/parser.tr"
                    while ((sk < _tr_str_lenv((fmt_spec)))) {
#line 2668 "src/parser.tr"
                        if ((_tr_str_char_at_codev((fmt_spec), sk) == 123LL)) {
#line 2669 "src/parser.tr"
                            long long se = (sk + 1LL);
#line 2670 "src/parser.tr"
                            while (((se < _tr_str_lenv((fmt_spec))) && (_tr_str_char_at_codev((fmt_spec), se) != 125LL))) {
#line 2670 "src/parser.tr"
                                se = (se + 1LL);
                            }
#line 2671 "src/parser.tr"
                            if ((se >= _tr_str_lenv((fmt_spec)))) {
#line 2671 "src/parser.tr"
                                break;
                            }
#line 2672 "src/parser.tr"
                            TrStr sub_raw = _tr_str_slicev((fmt_spec), (sk + 1LL), se);
#line 2673 "src/parser.tr"
                            TrStr sub_src = _tr_str_stripv((sub_raw));
#line 2674 "src/parser.tr"
                            Lexer* slex = Lexer_init(sub_src);
#line 2675 "src/parser.tr"
                            List_Token* stoks = Lexer_tokenize(slex);
#line 2676 "src/parser.tr"
                            if ((stoks->len > 0LL)) {
#line 2677 "src/parser.tr"
                                Parser* sparser = Parser_init(stoks, slex->token_lines);
#line 2678 "src/parser.tr"
                                ({ __auto_type _ord_t380 = (Parser_parse_expr(sparser)); List_ptr_append(fpart->spec_args, _ord_t380); });
                                _tr_obj_release(sparser, _trdrop_Parser);
                            }
#line 2679 "src/parser.tr"
                            sk = (se + 1LL);
                            _tr_str_release(sub_raw);
                            _tr_str_release(sub_src);
                            _tr_obj_release(slex, _trdrop_Lexer);
                        } else {
#line 2681 "src/parser.tr"
                            sk = (sk + 1LL);
                        }
                    }
#line 2682 "src/parser.tr"
                    List_ptr_append(fl, _tr_obj_retain(fpart));
                } else {
#line 2684 "src/parser.tr"
                    List_ptr_append(fl, FStringPart_init_expr(e));
                }
                _tr_obj_release(parser, _trdrop_Parser);
            }
            _tr_str_release(expr_str);
            StringBuilder__tr_fn_free(expr_sb);
            _tr_obj_release(lexer, _trdrop_Lexer);
        } else if (((c == 92LL) && ((i + 1LL) < raw_blen))) {
#line 2686 "src/parser.tr"
            long long esc = ((long long)((*(p + (i + 1LL)))));
#line 2687 "src/parser.tr"
            if ((esc == 110LL)) {
#line 2687 "src/parser.tr"
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
#line 2688 "src/parser.tr"
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
#line 2689 "src/parser.tr"
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
#line 2690 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
#line 2691 "src/parser.tr"
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
#line 2692 "src/parser.tr"
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
#line 2693 "src/parser.tr"
                StringBuilder_append_char(sb, 0LL);
            } else if (((((esc == 120LL) || (esc == 117LL)) || (esc == 85LL)) && _fstr_hex_ok(p, (i + 2LL), raw_blen, esc))) {
#line 2696 "src/parser.tr"
                long long fnd = 2LL;
#line 2697 "src/parser.tr"
                if ((esc == 117LL)) {
#line 2697 "src/parser.tr"
                    fnd = 4LL;
                }
#line 2698 "src/parser.tr"
                if ((esc == 85LL)) {
#line 2698 "src/parser.tr"
                    fnd = 8LL;
                }
#line 2699 "src/parser.tr"
                long long fhv = 0LL;
#line 2700 "src/parser.tr"
                long long fk = 0LL;
#line 2701 "src/parser.tr"
                while ((fk < fnd)) {
#line 2702 "src/parser.tr"
                    fhv = ((fhv * 16LL) + lex_hex_val(((long long)((*(p + ((i + 2LL) + fk)))))));
#line 2703 "src/parser.tr"
                    fk = (fk + 1LL);
                }
#line 2704 "src/parser.tr"
                if ((esc == 120LL)) {
#line 2704 "src/parser.tr"
                    StringBuilder_append_char(sb, fhv);
                } else {
#line 2705 "src/parser.tr"
                    lex_append_utf8(sb, fhv);
                }
#line 2706 "src/parser.tr"
                i = (i + fnd);
            } else {
#line 2710 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
#line 2711 "src/parser.tr"
                StringBuilder_append_char(sb, esc);
            }
#line 2712 "src/parser.tr"
            i = (i + 2LL);
        } else {
#line 2714 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2715 "src/parser.tr"
            i = (i + 1LL);
        }
    }
#line 2717 "src/parser.tr"
    if ((sb->buf->len > 0LL)) {
#line 2718 "src/parser.tr"
        ({ TrStr _ord_t381 = (StringBuilder_to_owned(sb)); __auto_type _ord_t382 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t381, _ord_t382)); _tr_str_release(_ord_t381); });
    }
#line 2720 "src/parser.tr"
    StringBuilder__tr_fn_free(sb);
    return box_expr(Expr_ctor_EFString(fl));
}

__attribute__((hot)) long long Parser_nesting_too_deep(Parser* self) {
#line 2726 "src/parser.tr"
    long long depth = 0LL;
#line 2727 "src/parser.tr"
    long long indent = 0LL;
#line 2728 "src/parser.tr"
    long long run = 0LL;
#line 2729 "src/parser.tr"
    long long lam = 0LL;
#line 2730 "src/parser.tr"
    bool prev_colon = false;
#line 2731 "src/parser.tr"
    long long i = 0LL;
#line 2732 "src/parser.tr"
    while ((i < self->tokens->len)) {
#line 2733 "src/parser.tr"
        bool is_prefix = false;
#line 2734 "src/parser.tr"
        __auto_type _t383 = List_Token_get(self->tokens, i);
        if (_t383.tag == Token_KwLambda) {
#line 2736 "src/parser.tr"
            if (prev_colon) {
#line 2736 "src/parser.tr"
                lam = (lam + 1LL);
            } else {
#line 2737 "src/parser.tr"
                lam = 0LL;
            }
        } else if (_t383.tag == Token_Colon) {
            /* pass */
        } else if ((_t383.tag == Token_Newline || _t383.tag == Token_Comma || _t383.tag == Token_RParen || _t383.tag == Token_RBracket || _t383.tag == Token_RBrace)) {
            lam = 0LL;
        } else if (1) {
            __auto_type _ = _t383;
            /* pass */
        }
#line 2741 "src/parser.tr"
        prev_colon = ({ __auto_type _ord_t384 = (List_Token_get(self->tokens, i)); __auto_type _wr = ((_ord_t384.tag == Token_make_Colon().tag)); _wr; });
#line 2742 "src/parser.tr"
        __auto_type _t385 = List_Token_get(self->tokens, i);
        if ((_t385.tag == Token_LParen || _t385.tag == Token_LBracket || _t385.tag == Token_LBrace)) {
#line 2744 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((_t385.tag == Token_RParen || _t385.tag == Token_RBracket || _t385.tag == Token_RBrace)) {
#line 2746 "src/parser.tr"
            if ((depth > 0LL)) {
#line 2746 "src/parser.tr"
                depth = (depth - 1LL);
            }
        } else if (_t385.tag == Token_Indent) {
#line 2748 "src/parser.tr"
            indent = (indent + 1LL);
        } else if (_t385.tag == Token_Dedent) {
#line 2750 "src/parser.tr"
            if ((indent > 0LL)) {
#line 2750 "src/parser.tr"
                indent = (indent - 1LL);
            }
        } else if ((_t385.tag == Token_Minus || _t385.tag == Token_Tilde || _t385.tag == Token_KwNot || _t385.tag == Token_KwAwait || _t385.tag == Token_Bang)) {
#line 2752 "src/parser.tr"
            is_prefix = true;
        } else if (1) {
            __auto_type _ = _t385;
#line 2754 "src/parser.tr"
            /* pass */
        }
#line 2755 "src/parser.tr"
        if (is_prefix) {
#line 2755 "src/parser.tr"
            run = (run + 1LL);
        } else {
#line 2756 "src/parser.tr"
            run = 0LL;
        }
#line 2757 "src/parser.tr"
        if (((((depth > 120LL) || (indent > 100LL)) || (run > 120LL)) || (lam > 100LL))) {
#line 2757 "src/parser.tr"
            return i;
        }
#line 2758 "src/parser.tr"
        i = (i + 1LL);
    }
#line 2759 "src/parser.tr"
    return (-1LL);
}

__attribute__((hot)) Program* Parser_parse_program(Parser* self) {
#line 2762 "src/parser.tr"
    Program* prog = Program_init();
#line 2763 "src/parser.tr"
    long long deep_at = Parser_nesting_too_deep(self);
#line 2764 "src/parser.tr"
    if ((deep_at >= 0LL)) {
#line 2765 "src/parser.tr"
        self->pos = deep_at;
#line 2766 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("this construct is nested too deeply", 35LL), _tr_str_lit_len("the compiler supports at most 120 nested brackets, 100 nested blocks or 120 chained prefix operators - split the expression or move the inner part into a helper function.", 170LL));
#line 2767 "src/parser.tr"
        self->pos = self->tokens->len;
#line 2768 "src/parser.tr"
        return prog;
    }
#line 2769 "src/parser.tr"
    Parser_skip_newlines_and_indent(self);
#line 2770 "src/parser.tr"
    while ((!Parser_at_end(self))) {
#line 2775 "src/parser.tr"
        bool sk = true;
#line 2776 "src/parser.tr"
        while (sk) {
#line 2777 "src/parser.tr"
            sk = false;
#line 2778 "src/parser.tr"
            __auto_type _t386 = Parser_peek(self);
            if ((_t386.tag == Token_Newline || _t386.tag == Token_Dedent)) {
#line 2780 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2781 "src/parser.tr"
                sk = true;
            } else if (_t386.tag == Token_Indent) {
#line 2783 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("unexpected indentation", 22LL), _tr_str_lit_len("this line is indented but no block was opened above it (a statement ending in ':' and its own line) — remove the extra indentation.", 133LL));
#line 2784 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2785 "src/parser.tr"
                sk = true;
            } else if (1) {
                __auto_type _ = _t386;
#line 2787 "src/parser.tr"
                /* pass */
            }
        }
#line 2788 "src/parser.tr"
        if ((!Parser_at_end(self))) {
#line 2789 "src/parser.tr"
            Program_push(prog, Parser_parse_decl(self));
        }
    }
#line 2790 "src/parser.tr"
    return prog;
}

__attribute__((hot)) Decl* Parser_parse_decl(Parser* self) {
#line 2793 "src/parser.tr"
    List_ptr* decorators = (void*)List_ptr_new();
#line 2794 "src/parser.tr"
    while (({ __auto_type _ord_t387 = (Parser_peek(self)); __auto_type _wr = ((_ord_t387.tag == Token_make_At().tag)); _wr; })) {
#line 2795 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2796 "src/parser.tr"
        TrStr name = Parser_consume_ident(self);
#line 2797 "src/parser.tr"
        Decorator* d = Decorator_init(name);
#line 2798 "src/parser.tr"
        if (({ __auto_type _ord_t388 = (Parser_peek(self)); __auto_type _wr = ((_ord_t388.tag == Token_make_LParen().tag)); _wr; })) {
#line 2799 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2800 "src/parser.tr"
            d->args = Parser_parse_arg_list(self);
        }
#line 2801 "src/parser.tr"
        List_ptr_append(decorators, _tr_obj_retain(d));
#line 2802 "src/parser.tr"
        Parser_expect_newline(self);
        _tr_str_release(name);
        _tr_obj_release(d, _trdrop_Decorator);
    }
#line 2804 "src/parser.tr"
    bool is_public = false;
#line 2805 "src/parser.tr"
    bool is_export = false;
#line 2808 "src/parser.tr"
    bool _scan_mods = true;
#line 2809 "src/parser.tr"
    while (_scan_mods) {
#line 2810 "src/parser.tr"
        _scan_mods = false;
#line 2811 "src/parser.tr"
        __auto_type _t389 = Parser_peek(self);
        if (_t389.tag == Token_KwPub) {
#line 2813 "src/parser.tr"
            is_public = true;
#line 2814 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2815 "src/parser.tr"
            _scan_mods = true;
        } else if (_t389.tag == Token_KwExport) {
#line 2817 "src/parser.tr"
            is_export = true;
#line 2818 "src/parser.tr"
            is_public = true;
#line 2819 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2820 "src/parser.tr"
            _scan_mods = true;
        } else if (1) {
            __auto_type _ = _t389;
#line 2822 "src/parser.tr"
            /* pass */
        }
    }
#line 2823 "src/parser.tr"
    __auto_type _t390 = Parser_peek(self);
    if (_t390.tag == Token_KwFrom) {
#line 2825 "src/parser.tr"
        Decl* _trret65 = Parser_parse_from_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret65;
    } else if (_t390.tag == Token_KwImport) {
#line 2827 "src/parser.tr"
        Decl* _trret66 = Parser_parse_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret66;
    } else if (_t390.tag == Token_Ident) {
        __auto_type type_kw = _t390.data.Ident.name;
#line 2830 "src/parser.tr"
        if (_tr_str_eqv((type_kw), (_tr_str_lit_len("type", 4LL)))) {
#line 2831 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2832 "src/parser.tr"
            TrStr alias_name = Parser_consume_ident(self);
#line 2833 "src/parser.tr"
            if (({ __auto_type _ord_t391 = (Parser_peek(self)); __auto_type _wr = ((_ord_t391.tag == Token_make_Eq().tag)); _wr; })) {
#line 2834 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2835 "src/parser.tr"
                AstType* target_ty = Parser_parse_type(self);
#line 2836 "src/parser.tr"
                Parser_expect_newline(self);
#line 2837 "src/parser.tr"
                Decl* _trret67 = box_decl(Decl_ctor_DTypeAlias(alias_name, box_asttype(target_ty)));
                _tr_str_release(alias_name);
                List_ptr_free_obj(decorators, _trdrop_Decorator);
                return _trret67;
            }
        }
    } else if (_t390.tag == Token_KwDef) {
#line 2839 "src/parser.tr"
        long long _fline = Parser_cur_line(self);
#line 2840 "src/parser.tr"
        FunctionDef* f = Parser_parse_function_def(self, false);
#line 2841 "src/parser.tr"
        f->line = _fline;
#line 2842 "src/parser.tr"
        f->is_public = is_public;
#line 2843 "src/parser.tr"
        f->is_export = is_export;
#line 2844 "src/parser.tr"
        f->decorators = decorators;
#line 2845 "src/parser.tr"
        Decl* _trret68 = box_decl(Decl_ctor_DFunction(f));
        _tr_obj_release(f, _trdrop_FunctionDef);
        return _trret68;
    } else if (_t390.tag == Token_KwClass) {
#line 2847 "src/parser.tr"
        long long _cline = Parser_cur_line(self);
#line 2848 "src/parser.tr"
        Decl* c_ptr = Parser_parse_class_decl(self);
#line 2849 "src/parser.tr"
        __auto_type _t392 = (*c_ptr);
        if (_t392.tag == Decl_DClass) {
            __auto_type c = _t392.data.DClass.cls;
#line 2851 "src/parser.tr"
            c->line = _cline;
#line 2852 "src/parser.tr"
            c->is_public = is_public;
#line 2859 "src/parser.tr"
            bool _is_vt = false;
#line 2860 "src/parser.tr"
            long long _dvi = 0LL;
#line 2861 "src/parser.tr"
            while ((_dvi < decorators->len)) {
#line 2862 "src/parser.tr"
                TrStr _dvn = _tr_str_retain(((Decorator*)List_ptr_get(decorators, _dvi))->name);
#line 2863 "src/parser.tr"
                if ((((_tr_str_eqv((_dvn), (_tr_str_lit_len("value_type", 10LL))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("packed", 6LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("aligned", 7LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("union", 5LL))))) {
#line 2863 "src/parser.tr"
                    _is_vt = true;
                }
#line 2864 "src/parser.tr"
                _dvi = (_dvi + 1LL);
                _tr_str_release(_dvn);
            }
#line 2865 "src/parser.tr"
            c->is_class = (!_is_vt);
#line 2866 "src/parser.tr"
            c->decorators = decorators;
#line 2867 "src/parser.tr"
            /* unsafe block */
#line 2867 "src/parser.tr"
            (*c_ptr = Decl_ctor_DClass(c));
        } else if (1) {
            __auto_type _ = _t392;
#line 2869 "src/parser.tr"
            /* pass */
        }
#line 2870 "src/parser.tr"
        return c_ptr;
    } else if (_t390.tag == Token_KwEnum) {
#line 2872 "src/parser.tr"
        long long _eline = Parser_cur_line(self);
#line 2873 "src/parser.tr"
        Decl* e_ptr = Parser_parse_enum_decl(self);
#line 2874 "src/parser.tr"
        __auto_type _t393 = (*e_ptr);
        if (_t393.tag == Decl_DEnum) {
            __auto_type e = _t393.data.DEnum.enm;
#line 2876 "src/parser.tr"
            e->line = _eline;
#line 2877 "src/parser.tr"
            e->is_public = is_public;
#line 2878 "src/parser.tr"
            e->decorators = decorators;
#line 2879 "src/parser.tr"
            /* unsafe block */
#line 2879 "src/parser.tr"
            (*e_ptr = Decl_ctor_DEnum(e));
        } else if (1) {
            __auto_type _ = _t393;
#line 2881 "src/parser.tr"
            /* pass */
        }
#line 2882 "src/parser.tr"
        return e_ptr;
    } else if (_t390.tag == Token_KwInterface) {
#line 2884 "src/parser.tr"
        long long _iline = Parser_cur_line(self);
#line 2885 "src/parser.tr"
        Decl* i_ptr = Parser_parse_interface_decl(self);
#line 2886 "src/parser.tr"
        __auto_type _t394 = (*i_ptr);
        if (_t394.tag == Decl_DInterface) {
            __auto_type i = _t394.data.DInterface.iface;
#line 2888 "src/parser.tr"
            i->line = _iline;
#line 2889 "src/parser.tr"
            i->is_public = is_public;
#line 2890 "src/parser.tr"
            i->decorators = decorators;
#line 2891 "src/parser.tr"
            /* unsafe block */
#line 2891 "src/parser.tr"
            (*i_ptr = Decl_ctor_DInterface(i));
        } else if (1) {
            __auto_type _ = _t394;
#line 2893 "src/parser.tr"
            /* pass */
        }
#line 2894 "src/parser.tr"
        return i_ptr;
    } else if (_t390.tag == Token_KwExtend) {
#line 2896 "src/parser.tr"
        return Parser_parse_extend_decl(self);
    } else if (_t390.tag == Token_KwDecorator) {
#line 2898 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2899 "src/parser.tr"
        long long _dl = Parser_cur_line(self);
#line 2900 "src/parser.tr"
        FunctionDef* _df = Parser_parse_function_def(self, false);
#line 2901 "src/parser.tr"
        _df->line = _dl;
#line 2902 "src/parser.tr"
        _df->is_public = is_public;
#line 2903 "src/parser.tr"
        Decl* _trret69 = box_decl(Decl_ctor_DDecoratorDef(_df));
        _tr_obj_release(_df, _trdrop_FunctionDef);
        return _trret69;
    } else if (_t390.tag == Token_KwMacro) {
#line 2908 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2909 "src/parser.tr"
        long long _ml = Parser_cur_line(self);
#line 2910 "src/parser.tr"
        FunctionDef* _mf = Parser_parse_function_def(self, false);
#line 2911 "src/parser.tr"
        _mf->line = _ml;
#line 2912 "src/parser.tr"
        _mf->is_public = is_public;
#line 2913 "src/parser.tr"
        _mf->is_macro = true;
#line 2914 "src/parser.tr"
        Decl* _trret70 = box_decl(Decl_ctor_DDecoratorDef(_mf));
        _tr_obj_release(_mf, _trdrop_FunctionDef);
        return _trret70;
    } else if (_t390.tag == Token_KwExtern) {
#line 2916 "src/parser.tr"
        return Parser_parse_extern_decl(self);
    } else if (_t390.tag == Token_KwAsync) {
#line 2918 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2919 "src/parser.tr"
        __auto_type _t395 = Parser_peek(self);
        if (_t395.tag == Token_KwDef) {
#line 2921 "src/parser.tr"
            FunctionDef* f = Parser_parse_function_def(self, false);
#line 2922 "src/parser.tr"
            f->is_async = true;
#line 2923 "src/parser.tr"
            f->is_public = is_public;
#line 2924 "src/parser.tr"
            f->is_export = is_export;
#line 2925 "src/parser.tr"
            f->decorators = decorators;
#line 2926 "src/parser.tr"
            Decl* _trret71 = box_decl(Decl_ctor_DFunction(f));
            _tr_obj_release(f, _trdrop_FunctionDef);
            return _trret71;
        } else if (1) {
            __auto_type _ = _t395;
#line 2928 "src/parser.tr"
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t390;
#line 2930 "src/parser.tr"
        /* pass */
    }
#line 2931 "src/parser.tr"
    return box_decl(Decl_ctor_DTopLevelStmt(Parser_parse_stmt(self)));
}

__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self) {
#line 2944 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 2945 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 2946 "src/parser.tr"
    while (({ __auto_type _ord_t396 = (Parser_peek(self)); __auto_type _wr = ((_ord_t396.tag == Token_make_Dot().tag)); _wr; })) {
#line 2947 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2948 "src/parser.tr"
        ({ TrStr _ap_t397 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t398 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t397); _tr_str_append(&path, _ap_t398); _tr_str_release(_ap_t398); });
    }
#line 2949 "src/parser.tr"
    __auto_type _t399 = Parser_peek(self);
    if (_t399.tag == Token_KwImport) {
#line 2951 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t399;
#line 2953 "src/parser.tr"
        /* pass */
    }
#line 2954 "src/parser.tr"
    List_ptr* il = (void*)List_ptr_new();
#line 2955 "src/parser.tr"
    bool multi = false;
#line 2956 "src/parser.tr"
    __auto_type _t400 = Parser_peek(self);
    if ((_t400.tag == Token_LParen || _t400.tag == Token_LBracket)) {
#line 2958 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2959 "src/parser.tr"
        multi = true;
    } else if (1) {
        __auto_type _ = _t400;
#line 2961 "src/parser.tr"
        /* pass */
    }
#line 2962 "src/parser.tr"
    bool going = true;
#line 2963 "src/parser.tr"
    while (going) {
#line 2966 "src/parser.tr"
        if (multi) {
#line 2967 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        } else {
#line 2969 "src/parser.tr"
            Parser_skip_newlines(self);
        }
#line 2970 "src/parser.tr"
        __auto_type _t401 = Parser_peek(self);
        if ((_t401.tag == Token_RParen || _t401.tag == Token_RBracket || _t401.tag == Token_Eof)) {
#line 2972 "src/parser.tr"
            going = false;
        } else if ((_t401.tag == Token_Newline || _t401.tag == Token_Dedent)) {
#line 2974 "src/parser.tr"
            if ((!multi)) {
#line 2975 "src/parser.tr"
                going = false;
            }
        } else if (1) {
            __auto_type _ = _t401;
#line 2977 "src/parser.tr"
            /* pass */
        }
#line 2978 "src/parser.tr"
        if (going) {
#line 2982 "src/parser.tr"
            if (({ __auto_type _ord_t402 = (Parser_peek(self)); __auto_type _wr = ((_ord_t402.tag == Token_make_Star().tag)); _wr; })) {
#line 2983 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2984 "src/parser.tr"
                List_ptr_append(il, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 2985 "src/parser.tr"
                if (({ __auto_type _ord_t403 = (Parser_peek(self)); __auto_type _wr = ((_ord_t403.tag == Token_make_Comma().tag)); _wr; })) {
#line 2986 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if ((!multi)) {
#line 2988 "src/parser.tr"
                    going = false;
                }
            } else {
#line 2990 "src/parser.tr"
                TrStr item_name = Parser_consume_ident(self);
#line 2991 "src/parser.tr"
                if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 2992 "src/parser.tr"
                    ImportItem* item = ImportItem_init(item_name);
#line 2994 "src/parser.tr"
                    if (({ __auto_type _ord_t404 = (Parser_peek(self)); __auto_type _wr = ((_ord_t404.tag == Token_make_KwAs().tag)); _wr; })) {
#line 2995 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 2996 "src/parser.tr"
                        item->alias = Parser_consume_ident(self);
#line 2997 "src/parser.tr"
                        if ((!_tr_str_eqv((item->alias), (_tr_str_lit_len("", 0LL))))) {
#line 3000 "src/parser.tr"
                            _tr_dict_set(self->import_aliases, _tr_strz(item->alias), _tr_str_box(_tr_str_retain(item->name)));
                        }
                    }
#line 3001 "src/parser.tr"
                    List_ptr_append(il, _tr_obj_retain(item));
#line 3002 "src/parser.tr"
                    if (({ __auto_type _ord_t405 = (Parser_peek(self)); __auto_type _wr = ((_ord_t405.tag == Token_make_Comma().tag)); _wr; })) {
#line 3003 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if ((!multi)) {
#line 3005 "src/parser.tr"
                        going = false;
                    }
                } else {
#line 3007 "src/parser.tr"
                    going = false;
                }
            }
        }
    }
#line 3009 "src/parser.tr"
    if (multi) {
#line 3010 "src/parser.tr"
        __auto_type _t406 = Parser_peek(self);
        if ((_t406.tag == Token_RParen || _t406.tag == Token_RBracket)) {
#line 3012 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t406;
#line 3014 "src/parser.tr"
            /* pass */
        }
    }
#line 3015 "src/parser.tr"
    Parser_expect_newline(self);
#line 3016 "src/parser.tr"
    Decl* _trret72 = box_decl(Decl_ctor_DFromImport(path, il));
    _tr_str_release(path);
    return _trret72;
}

__attribute__((hot)) Decl* Parser_parse_import(Parser* self) {
#line 3019 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3020 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 3021 "src/parser.tr"
    while (({ __auto_type _ord_t407 = (Parser_peek(self)); __auto_type _wr = ((_ord_t407.tag == Token_make_Dot().tag)); _wr; })) {
#line 3022 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3025 "src/parser.tr"
        if (({ __auto_type _ord_t408 = (Parser_peek(self)); __auto_type _wr = ((_ord_t408.tag == Token_make_Star().tag)); _wr; })) {
#line 3026 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3027 "src/parser.tr"
            Parser_expect_newline(self);
#line 3028 "src/parser.tr"
            List_ptr* ils = (void*)List_ptr_new();
#line 3029 "src/parser.tr"
            List_ptr_append(ils, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 3030 "src/parser.tr"
            Decl* _trret73 = box_decl(Decl_ctor_DFromImport(path, ils));
            _tr_str_release(path);
            return _trret73;
        }
#line 3031 "src/parser.tr"
        ({ TrStr _ap_t409 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t410 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t409); _tr_str_append(&path, _ap_t410); _tr_str_release(_ap_t410); });
    }
#line 3032 "src/parser.tr"
    TrStr alias = _tr_str_lit_len("", 0LL);
#line 3033 "src/parser.tr"
    if (({ __auto_type _ord_t411 = (Parser_peek(self)); __auto_type _wr = ((_ord_t411.tag == Token_make_KwAs().tag)); _wr; })) {
#line 3034 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3035 "src/parser.tr"
        TrStr _strtmp_t412 = Parser_consume_ident(self);
        _tr_str_release(alias);
        alias = _strtmp_t412;
    }
#line 3036 "src/parser.tr"
    Parser_expect_newline(self);
#line 3037 "src/parser.tr"
    Decl* _trret74 = box_decl(Decl_ctor_DImport(path, alias));
    _tr_str_release(path);
    _tr_str_release(alias);
    return _trret74;
}

__attribute__((hot)) void Parser_parse_generic_bound(Parser* self, TrStr gname, List_ptr* constraints) {
#line 3043 "src/parser.tr"
    if (({ __auto_type _ord_t413 = (Parser_peek(self)); __auto_type _wr = ((_ord_t413.tag == Token_make_Colon().tag)); _wr; })) {
#line 3044 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3045 "src/parser.tr"
        GenericConstraint* gc = GenericConstraint_init(gname);
#line 3046 "src/parser.tr"
        ({ __auto_type _ord_t414 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t414); });
#line 3047 "src/parser.tr"
        while (({ __auto_type _ord_t415 = (Parser_peek(self)); __auto_type _wr = ((_ord_t415.tag == Token_make_Plus().tag)); _wr; })) {
#line 3048 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3049 "src/parser.tr"
            ({ __auto_type _ord_t416 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t416); });
        }
#line 3050 "src/parser.tr"
        List_ptr_append(constraints, _tr_obj_retain(gc));
    }
}

__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method) {
#line 3053 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3054 "src/parser.tr"
    if ((!is_method)) {
#line 3055 "src/parser.tr"
        __auto_type _t417 = Parser_peek(self);
        if (_t417.tag == Token_Ident) {
            __auto_type _ = _t417.data.Ident.name;
            /* pass */
        } else if (1) {
            __auto_type _ = _t417;
#line 3058 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected a function name after 'def'", 36LL), _tr_str_lit_len("write 'def name(params) -> Type:' - the name must be an identifier, not a keyword or symbol.", 92LL));
        }
    }
#line 3059 "src/parser.tr"
    TrStr name = ((is_method) ? (({
#line 3059 "src/parser.tr"
__auto_type _dores = (Parser_consume_method_ident(self));
_dores;
})) : (({
#line 3059 "src/parser.tr"
__auto_type _dores = (Parser_consume_ident(self));
_dores;
})));
#line 3060 "src/parser.tr"
    FunctionDef* f = FunctionDef_init(name);
#line 3061 "src/parser.tr"
    if (({ __auto_type _ord_t418 = (Parser_peek(self)); __auto_type _wr = ((_ord_t418.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3062 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3063 "src/parser.tr"
        while ((((({ __auto_type _ord_t419 = (Parser_peek(self)); __auto_type _wr = ((_ord_t419.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t420 = (Parser_peek(self)); __auto_type _wr = ((_ord_t420.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t421 = (Parser_peek(self)); __auto_type _wr = ((_ord_t421.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t422 = (Parser_peek(self)); __auto_type _wr = ((_ord_t422.tag != Token_make_LParen().tag)); _wr; }))) {
#line 3064 "src/parser.tr"
            TrStr _gpn = Parser_consume_ident(self);
#line 3065 "src/parser.tr"
            if (_tr_str_eqv((_gpn), (_tr_str_lit_len("", 0LL)))) {
#line 3066 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'def f[T](...)'.", 34LL));
#line 3067 "src/parser.tr"
                _tr_str_release(_gpn);
                break;
            }
#line 3068 "src/parser.tr"
            List_TrStr_append(f->generics, _gpn);
#line 3069 "src/parser.tr"
            Parser_parse_generic_bound(self, _gpn, f->constraints);
#line 3070 "src/parser.tr"
            if (({ __auto_type _ord_t423 = (Parser_peek(self)); __auto_type _wr = ((_ord_t423.tag == Token_make_Comma().tag)); _wr; })) {
#line 3071 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_gpn);
        }
#line 3072 "src/parser.tr"
        if (({ __auto_type _ord_t424 = (Parser_peek(self)); __auto_type _wr = ((_ord_t424.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3073 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3075 "src/parser.tr"
            ({ TrStr _at_t425 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of '", 47LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t425, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t425); });
        }
    }
#line 3076 "src/parser.tr"
    if (({ __auto_type _ord_t426 = (Parser_peek(self)); __auto_type _wr = ((_ord_t426.tag == Token_make_LParen().tag)); _wr; })) {
#line 3077 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3078 "src/parser.tr"
        f->params = Parser_parse_param_list(self);
#line 3079 "src/parser.tr"
        Parser_close_params(self);
    }
#line 3080 "src/parser.tr"
    __auto_type _t427 = Parser_peek(self);
    if (_t427.tag == Token_KwThrows) {
#line 3082 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3083 "src/parser.tr"
        f->throws_ty = box_asttype(Parser_parse_type(self));
    } else if (1) {
        __auto_type _ = _t427;
#line 3085 "src/parser.tr"
        /* pass */
    }
#line 3086 "src/parser.tr"
    if (({ __auto_type _ord_t428 = (Parser_peek(self)); __auto_type _wr = ((_ord_t428.tag == Token_make_Arrow().tag)); _wr; })) {
#line 3087 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3088 "src/parser.tr"
        AstType* ret_t = Parser_parse_type(self);
#line 3089 "src/parser.tr"
        if (({ __auto_type _ord_t429 = (Parser_peek(self)); __auto_type _wr = ((_ord_t429.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3093 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3094 "src/parser.tr"
            ret_t->from_param = Parser_consume_ident(self);
#line 3095 "src/parser.tr"
            List_TrStr_append(ret_t->from_regions, ret_t->from_param);
#line 3096 "src/parser.tr"
            while (({ __auto_type _ord_t430 = (Parser_peek(self)); __auto_type _wr = ((_ord_t430.tag == Token_make_Comma().tag)); _wr; })) {
#line 3097 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3098 "src/parser.tr"
                TrStr rgn2 = Parser_consume_ident(self);
#line 3099 "src/parser.tr"
                ret_t->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((ret_t->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3100 "src/parser.tr"
                List_TrStr_append(ret_t->from_regions, rgn2);
                _tr_str_release(rgn2);
            }
        }
#line 3101 "src/parser.tr"
        f->ret_ty = box_asttype(ret_t);
    }
#line 3109 "src/parser.tr"
    bool w_is = false;
#line 3110 "src/parser.tr"
    __auto_type _t431 = Parser_peek(self);
    if (_t431.tag == Token_Ident) {
        __auto_type wkw0 = _t431.data.Ident.name;
#line 3112 "src/parser.tr"
        if (_tr_str_eqv((wkw0), (_tr_str_lit_len("where", 5LL)))) {
#line 3112 "src/parser.tr"
            w_is = true;
        }
    } else if (1) {
        __auto_type _ = _t431;
        /* pass */
    }
#line 3114 "src/parser.tr"
    if (((!w_is) && ({ __auto_type _ord_t432 = (Parser_peek(self)); __auto_type _wr = ((_ord_t432.tag == Token_make_Newline().tag)); _wr; }))) {
#line 3118 "src/parser.tr"
        long long scan = self->pos;
#line 3119 "src/parser.tr"
        bool scanning = true;
#line 3120 "src/parser.tr"
        while (scanning) {
#line 3121 "src/parser.tr"
            __auto_type _t433 = List_Token_get(self->tokens, scan);
            if ((_t433.tag == Token_Newline || _t433.tag == Token_Indent)) {
#line 3123 "src/parser.tr"
                scan = (scan + 1LL);
            } else if (1) {
                __auto_type _ = _t433;
#line 3125 "src/parser.tr"
                scanning = false;
            }
        }
#line 3126 "src/parser.tr"
        __auto_type _t434 = List_Token_get(self->tokens, scan);
        if (_t434.tag == Token_Ident) {
            __auto_type wkw1 = _t434.data.Ident.name;
#line 3128 "src/parser.tr"
            if (_tr_str_eqv((wkw1), (_tr_str_lit_len("where", 5LL)))) {
#line 3129 "src/parser.tr"
                w_is = true;
#line 3130 "src/parser.tr"
                self->pos = scan;
            }
        } else if (1) {
            __auto_type _ = _t434;
            /* pass */
        }
    }
#line 3132 "src/parser.tr"
    if (w_is) {
#line 3133 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3134 "src/parser.tr"
        bool w_paren = false;
#line 3135 "src/parser.tr"
        if (({ __auto_type _ord_t435 = (Parser_peek(self)); __auto_type _wr = ((_ord_t435.tag == Token_make_LParen().tag)); _wr; })) {
#line 3136 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3137 "src/parser.tr"
            w_paren = true;
#line 3138 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        }
#line 3139 "src/parser.tr"
        bool w_more = true;
#line 3140 "src/parser.tr"
        while (w_more) {
#line 3141 "src/parser.tr"
            if (w_paren) {
#line 3141 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            }
#line 3142 "src/parser.tr"
            if ((w_paren && ({ __auto_type _ord_t436 = (Parser_peek(self)); __auto_type _wr = ((_ord_t436.tag == Token_make_RParen().tag)); _wr; }))) {
#line 3143 "src/parser.tr"
                w_more = false;
            } else {
#line 3145 "src/parser.tr"
                TrStr w_a = Parser_consume_ident(self);
#line 3146 "src/parser.tr"
                TrStr w_kw = Parser_consume_ident(self);
#line 3147 "src/parser.tr"
                TrStr w_b = Parser_consume_ident(self);
#line 3148 "src/parser.tr"
                List_TrStr_append(f->outlives_a, w_a);
#line 3149 "src/parser.tr"
                List_TrStr_append(f->outlives_b, w_b);
#line 3150 "src/parser.tr"
                if (w_paren) {
#line 3150 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                }
#line 3151 "src/parser.tr"
                if ((w_paren && ({ __auto_type _ord_t437 = (Parser_peek(self)); __auto_type _wr = ((_ord_t437.tag == Token_make_Comma().tag)); _wr; }))) {
#line 3152 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else {
#line 3154 "src/parser.tr"
                    w_more = false;
                }
            }
        }
#line 3155 "src/parser.tr"
        if (w_paren) {
#line 3156 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 3157 "src/parser.tr"
            if (({ __auto_type _ord_t438 = (Parser_peek(self)); __auto_type _wr = ((_ord_t438.tag == Token_make_RParen().tag)); _wr; })) {
#line 3158 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3159 "src/parser.tr"
    if (({ __auto_type _ord_t439 = (Parser_peek(self)); __auto_type _wr = ((_ord_t439.tag == Token_make_Colon().tag)); _wr; })) {
#line 3160 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3161 "src/parser.tr"
        f->body = Parser_parse_block(self);
    } else {
#line 3163 "src/parser.tr"
        Parser_expect_newline(self);
#line 3164 "src/parser.tr"
        f->body = Block_init();
    }
#line 3165 "src/parser.tr"
    _tr_str_release(name);
    return f;
}

__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self) {
#line 3168 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3169 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3170 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 3170 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected a class name after 'class'", 35LL), _tr_str_lit_len("write 'class Name:'.", 20LL));
    }
#line 3171 "src/parser.tr"
    ClassDef* c = ClassDef_init(name);
#line 3172 "src/parser.tr"
    if (({ __auto_type _ord_t440 = (Parser_peek(self)); __auto_type _wr = ((_ord_t440.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3173 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3174 "src/parser.tr"
        while ((((({ __auto_type _ord_t441 = (Parser_peek(self)); __auto_type _wr = ((_ord_t441.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t442 = (Parser_peek(self)); __auto_type _wr = ((_ord_t442.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t443 = (Parser_peek(self)); __auto_type _wr = ((_ord_t443.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t444 = (Parser_peek(self)); __auto_type _wr = ((_ord_t444.tag != Token_make_Colon().tag)); _wr; }))) {
#line 3175 "src/parser.tr"
            TrStr _cgpn = Parser_consume_ident(self);
#line 3176 "src/parser.tr"
            if (_tr_str_eqv((_cgpn), (_tr_str_lit_len("", 0LL)))) {
#line 3177 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'class Box[T]:'.", 34LL));
#line 3178 "src/parser.tr"
                _tr_str_release(_cgpn);
                break;
            }
#line 3179 "src/parser.tr"
            List_TrStr_append(c->generics, _cgpn);
#line 3180 "src/parser.tr"
            Parser_parse_generic_bound(self, _cgpn, c->constraints);
#line 3181 "src/parser.tr"
            if (({ __auto_type _ord_t445 = (Parser_peek(self)); __auto_type _wr = ((_ord_t445.tag == Token_make_Comma().tag)); _wr; })) {
#line 3182 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_cgpn);
        }
#line 3183 "src/parser.tr"
        if (({ __auto_type _ord_t446 = (Parser_peek(self)); __auto_type _wr = ((_ord_t446.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3184 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3186 "src/parser.tr"
            ({ TrStr _at_t447 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of class '", 53LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t447, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t447); });
        }
    }
#line 3190 "src/parser.tr"
    if (({ __auto_type _ord_t448 = (Parser_peek(self)); __auto_type _wr = ((_ord_t448.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3191 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3192 "src/parser.tr"
        ({ TrStr _ord_t449 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t449); _tr_str_release(_ord_t449); });
#line 3193 "src/parser.tr"
        while (({ __auto_type _ord_t450 = (Parser_peek(self)); __auto_type _wr = ((_ord_t450.tag == Token_make_Comma().tag)); _wr; })) {
#line 3194 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3195 "src/parser.tr"
            ({ TrStr _ord_t451 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t451); _tr_str_release(_ord_t451); });
        }
    }
#line 3197 "src/parser.tr"
    if (({ __auto_type _ord_t452 = (Parser_peek(self)); __auto_type _wr = ((_ord_t452.tag == Token_make_LParen().tag)); _wr; })) {
#line 3198 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3199 "src/parser.tr"
        ({ TrStr _ord_t453 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t453); _tr_str_release(_ord_t453); });
#line 3200 "src/parser.tr"
        while (({ __auto_type _ord_t454 = (Parser_peek(self)); __auto_type _wr = ((_ord_t454.tag == Token_make_Comma().tag)); _wr; })) {
#line 3201 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3202 "src/parser.tr"
            ({ TrStr _ord_t455 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t455); _tr_str_release(_ord_t455); });
        }
#line 3203 "src/parser.tr"
        if (({ __auto_type _ord_t456 = (Parser_peek(self)); __auto_type _wr = ((_ord_t456.tag == Token_make_RParen().tag)); _wr; })) {
#line 3204 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3205 "src/parser.tr"
    if (({ __auto_type _ord_t457 = (Parser_peek(self)); __auto_type _wr = ((_ord_t457.tag == Token_make_KwExtends().tag)); _wr; })) {
#line 3206 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3207 "src/parser.tr"
        ({ TrStr _ord_t458 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t458); _tr_str_release(_ord_t458); });
#line 3208 "src/parser.tr"
        while (({ __auto_type _ord_t459 = (Parser_peek(self)); __auto_type _wr = ((_ord_t459.tag == Token_make_Comma().tag)); _wr; })) {
#line 3209 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3210 "src/parser.tr"
            ({ TrStr _ord_t460 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t460); _tr_str_release(_ord_t460); });
        }
    }
#line 3211 "src/parser.tr"
    if (({ __auto_type _ord_t461 = (Parser_peek(self)); __auto_type _wr = ((_ord_t461.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3212 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3213 "src/parser.tr"
        while (true) {
#line 3214 "src/parser.tr"
            TrStr _im_name = Parser_consume_ident(self);
#line 3215 "src/parser.tr"
            List_TrStr_append(c->iface_names, _im_name);
#line 3216 "src/parser.tr"
            AstType* _im_ty = AstType_init(_im_name);
#line 3217 "src/parser.tr"
            if (({ __auto_type _ord_t462 = (Parser_peek(self)); __auto_type _wr = ((_ord_t462.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3218 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3219 "src/parser.tr"
                while ((({ __auto_type _ord_t463 = (Parser_peek(self)); __auto_type _wr = ((_ord_t463.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t464 = (Parser_peek(self)); __auto_type _wr = ((_ord_t464.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3220 "src/parser.tr"
                    ({ __auto_type _ord_t465 = (box_asttype(Parser_parse_type(self))); List_ptr_append(_im_ty->args, _ord_t465); });
#line 3221 "src/parser.tr"
                    if (({ __auto_type _ord_t466 = (Parser_peek(self)); __auto_type _wr = ((_ord_t466.tag == Token_make_Comma().tag)); _wr; })) {
#line 3221 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                }
#line 3222 "src/parser.tr"
                if (({ __auto_type _ord_t467 = (Parser_peek(self)); __auto_type _wr = ((_ord_t467.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3222 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
            }
#line 3223 "src/parser.tr"
            ({ __auto_type _ord_t468 = (box_asttype(_im_ty)); List_ptr_append(c->iface_targs, _ord_t468); });
#line 3224 "src/parser.tr"
            if (({ __auto_type _ord_t469 = (Parser_peek(self)); __auto_type _wr = ((_ord_t469.tag == Token_make_Comma().tag)); _wr; })) {
#line 3225 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3226 "src/parser.tr"
                _tr_str_release(_im_name);
                break;
            }
            _tr_str_release(_im_name);
        }
    }
#line 3231 "src/parser.tr"
    bool inline_body = false;
#line 3232 "src/parser.tr"
    if (({ __auto_type _ord_t470 = (Parser_peek(self)); __auto_type _wr = ((_ord_t470.tag == Token_make_Colon().tag)); _wr; })) {
#line 3233 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3234 "src/parser.tr"
        __auto_type _t471 = Parser_peek(self);
        if ((_t471.tag == Token_Newline || _t471.tag == Token_Eof)) {
#line 3236 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t471;
#line 3238 "src/parser.tr"
            inline_body = true;
        }
    } else {
#line 3240 "src/parser.tr"
        ({ TrStr _at_t472 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("expected ':' after the header of class '", 40LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t472, _tr_str_lit_len("a class header is 'class Name[T]:' (optionally followed by '(Base)'), with the members indented on the following lines.", 119LL)); _tr_str_release(_at_t472); });
#line 3241 "src/parser.tr"
        while (true) {
#line 3242 "src/parser.tr"
            __auto_type _t473 = Parser_peek(self);
            if ((_t473.tag == Token_Newline || _t473.tag == Token_Eof || _t473.tag == Token_Colon)) {
                break;
            } else if (1) {
                __auto_type _ = _t473;
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3245 "src/parser.tr"
    if (inline_body) {
#line 3246 "src/parser.tr"
        bool inl = true;
#line 3247 "src/parser.tr"
        while (inl) {
#line 3248 "src/parser.tr"
            bool in_pub = false;
#line 3249 "src/parser.tr"
            __auto_type _t474 = Parser_peek(self);
            if (_t474.tag == Token_KwPub) {
#line 3251 "src/parser.tr"
                in_pub = true;
#line 3252 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t474;
#line 3254 "src/parser.tr"
                /* pass */
            }
#line 3255 "src/parser.tr"
            __auto_type _t475 = Parser_peek(self);
            if (_t475.tag == Token_KwPass) {
#line 3257 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t475.tag == Token_KwDef) {
#line 3259 "src/parser.tr"
                long long _iml = Parser_cur_line(self);
#line 3260 "src/parser.tr"
                FunctionDef* im = Parser_parse_function_def(self, true);
#line 3261 "src/parser.tr"
                im->line = _iml;
#line 3262 "src/parser.tr"
                im->is_public = in_pub;
#line 3263 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(im));
                _tr_obj_release(im, _trdrop_FunctionDef);
            } else if (_t475.tag == Token_Ident) {
                __auto_type ifn = _t475.data.Ident.name;
#line 3265 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3266 "src/parser.tr"
                AstType** ity = (AstType**)(0LL);
#line 3267 "src/parser.tr"
                if (({ __auto_type _ord_t476 = (Parser_peek(self)); __auto_type _wr = ((_ord_t476.tag == Token_make_Colon().tag)); _wr; })) {
#line 3268 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3269 "src/parser.tr"
                    ity = box_asttype(Parser_parse_type(self));
                }
#line 3270 "src/parser.tr"
                ({ FieldDef* _ord_t477 = (FieldDef_init(ifn, ity)); List_ptr_append(c->fields, _tr_obj_retain(_ord_t477)); _tr_obj_release(_ord_t477, _trdrop_FieldDef); });
            } else if (1) {
                __auto_type _ = _t475;
#line 3272 "src/parser.tr"
                inl = false;
            }
#line 3273 "src/parser.tr"
            __auto_type _t478 = Parser_peek(self);
            if (_t478.tag == Token_Semicolon) {
#line 3275 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t478;
#line 3277 "src/parser.tr"
                inl = false;
            }
        }
#line 3278 "src/parser.tr"
        if (({ __auto_type _ord_t479 = (Parser_peek(self)); __auto_type _wr = ((_ord_t479.tag == Token_make_Newline().tag)); _wr; })) {
#line 3279 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3280 "src/parser.tr"
        Decl* _trret75 = box_decl(Decl_ctor_DClass(c));
        _tr_str_release(name);
        _tr_obj_release(c, _trdrop_ClassDef);
        return _trret75;
    }
#line 3281 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3282 "src/parser.tr"
    __auto_type _t480 = Parser_peek(self);
    if (_t480.tag == Token_Indent) {
#line 3284 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t480;
#line 3286 "src/parser.tr"
        /* pass */
    }
#line 3287 "src/parser.tr"
    bool going = true;
#line 3288 "src/parser.tr"
    while (going) {
#line 3289 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3292 "src/parser.tr"
        if (_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL)))) {
#line 3293 "src/parser.tr"
            __auto_type _t481 = Parser_peek(self);
            if (_t481.tag == Token_TripleStrLit) {
                __auto_type _ds = _t481.data.TripleStrLit.val;
#line 3295 "src/parser.tr"
                c->docstring = _tr_str_retain(_ds);
#line 3296 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3297 "src/parser.tr"
                Parser_skip_newlines(self);
            } else if (1) {
                __auto_type _ = _t481;
#line 3299 "src/parser.tr"
                /* pass */
            }
        }
#line 3300 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3301 "src/parser.tr"
        while (({ __auto_type _ord_t482 = (Parser_peek(self)); __auto_type _wr = ((_ord_t482.tag == Token_make_At().tag)); _wr; })) {
#line 3302 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3303 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3304 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3305 "src/parser.tr"
            if (({ __auto_type _ord_t483 = (Parser_peek(self)); __auto_type _wr = ((_ord_t483.tag == Token_make_LParen().tag)); _wr; })) {
#line 3306 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3307 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3308 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3309 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3311 "src/parser.tr"
        bool is_p = false;
#line 3312 "src/parser.tr"
        __auto_type _t484 = Parser_peek(self);
        if (_t484.tag == Token_KwPub) {
#line 3314 "src/parser.tr"
            is_p = true;
#line 3315 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t484;
#line 3317 "src/parser.tr"
            /* pass */
        }
#line 3318 "src/parser.tr"
        __auto_type _t485 = Parser_peek(self);
        if ((_t485.tag == Token_Dedent || _t485.tag == Token_Eof)) {
#line 3320 "src/parser.tr"
            going = false;
        } else if (_t485.tag == Token_KwDef) {
#line 3322 "src/parser.tr"
            long long _mline = Parser_cur_line(self);
#line 3323 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3324 "src/parser.tr"
            m->line = _mline;
#line 3325 "src/parser.tr"
            m->is_public = is_p;
#line 3326 "src/parser.tr"
            m->decorators = decorators;
#line 3327 "src/parser.tr"
            List_ptr_append(c->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t485.tag == Token_KwAsync) {
#line 3329 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3330 "src/parser.tr"
            __auto_type _t486 = Parser_peek(self);
            if (_t486.tag == Token_KwDef) {
#line 3332 "src/parser.tr"
                long long _mline = Parser_cur_line(self);
#line 3333 "src/parser.tr"
                FunctionDef* m = Parser_parse_function_def(self, true);
#line 3334 "src/parser.tr"
                m->line = _mline;
#line 3335 "src/parser.tr"
                m->is_async = true;
#line 3336 "src/parser.tr"
                m->is_public = is_p;
#line 3337 "src/parser.tr"
                m->decorators = decorators;
#line 3338 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(m));
                _tr_obj_release(m, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t486;
#line 3340 "src/parser.tr"
                /* pass */
            }
        } else if (_t485.tag == Token_Ident) {
            __auto_type fname = _t485.data.Ident.name;
#line 3342 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3343 "src/parser.tr"
            AstType** ty_ptr = (AstType**)(0LL);
#line 3344 "src/parser.tr"
            if (({ __auto_type _ord_t487 = (Parser_peek(self)); __auto_type _wr = ((_ord_t487.tag == Token_make_Colon().tag)); _wr; })) {
#line 3345 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3346 "src/parser.tr"
                AstType* fty = Parser_parse_type(self);
#line 3351 "src/parser.tr"
                if (({ __auto_type _ord_t488 = (Parser_peek(self)); __auto_type _wr = ((_ord_t488.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3352 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3353 "src/parser.tr"
                    fty->from_param = Parser_consume_ident(self);
#line 3354 "src/parser.tr"
                    List_TrStr_append(fty->from_regions, fty->from_param);
#line 3355 "src/parser.tr"
                    while (({ __auto_type _ord_t489 = (Parser_peek(self)); __auto_type _wr = ((_ord_t489.tag == Token_make_Comma().tag)); _wr; })) {
#line 3356 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 3357 "src/parser.tr"
                        TrStr frgn2 = Parser_consume_ident(self);
#line 3358 "src/parser.tr"
                        fty->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((fty->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (frgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3359 "src/parser.tr"
                        List_TrStr_append(fty->from_regions, frgn2);
                        _tr_str_release(frgn2);
                    }
                }
#line 3360 "src/parser.tr"
                ty_ptr = box_asttype(fty);
            }
#line 3361 "src/parser.tr"
            FieldDef* fld = FieldDef_init(fname, ty_ptr);
#line 3362 "src/parser.tr"
            List_ptr_append(c->fields, _tr_obj_retain(fld));
#line 3363 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(fld, _trdrop_FieldDef);
        } else if (_t485.tag == Token_KwPass) {
#line 3365 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3366 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t485;
#line 3368 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3369 "src/parser.tr"
    if (({ __auto_type _ord_t490 = (Parser_peek(self)); __auto_type _wr = ((_ord_t490.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3370 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3371 "src/parser.tr"
    Decl* _trret76 = box_decl(Decl_ctor_DClass(c));
    _tr_str_release(name);
    _tr_obj_release(c, _trdrop_ClassDef);
    return _trret76;
}

__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self) {
#line 3374 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3375 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3376 "src/parser.tr"
    EnumDef* e = EnumDef_init(name);
#line 3380 "src/parser.tr"
    if (({ __auto_type _ord_t491 = (Parser_peek(self)); __auto_type _wr = ((_ord_t491.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3381 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3382 "src/parser.tr"
        ({ TrStr _ord_t492 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t492); _tr_str_release(_ord_t492); });
#line 3383 "src/parser.tr"
        while (({ __auto_type _ord_t493 = (Parser_peek(self)); __auto_type _wr = ((_ord_t493.tag == Token_make_Comma().tag)); _wr; })) {
#line 3384 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3385 "src/parser.tr"
            ({ TrStr _ord_t494 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t494); _tr_str_release(_ord_t494); });
        }
    }
#line 3386 "src/parser.tr"
    if (({ __auto_type _ord_t495 = (Parser_peek(self)); __auto_type _wr = ((_ord_t495.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3387 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3388 "src/parser.tr"
        while (true) {
#line 3389 "src/parser.tr"
            ({ TrStr _ord_t496 = (Parser_consume_ident(self)); List_TrStr_append(e->iface_names, _ord_t496); _tr_str_release(_ord_t496); });
#line 3390 "src/parser.tr"
            if (({ __auto_type _ord_t497 = (Parser_peek(self)); __auto_type _wr = ((_ord_t497.tag == Token_make_Comma().tag)); _wr; })) {
#line 3391 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3392 "src/parser.tr"
                break;
            }
        }
    }
#line 3396 "src/parser.tr"
    bool e_inline = false;
#line 3397 "src/parser.tr"
    if (({ __auto_type _ord_t498 = (Parser_peek(self)); __auto_type _wr = ((_ord_t498.tag == Token_make_Colon().tag)); _wr; })) {
#line 3398 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3399 "src/parser.tr"
        __auto_type _t499 = Parser_peek(self);
        if ((_t499.tag == Token_Newline || _t499.tag == Token_Eof)) {
#line 3401 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t499;
#line 3403 "src/parser.tr"
            e_inline = true;
        }
    }
#line 3404 "src/parser.tr"
    if (e_inline) {
#line 3405 "src/parser.tr"
        bool einl = true;
#line 3406 "src/parser.tr"
        while (einl) {
#line 3407 "src/parser.tr"
            __auto_type _t500 = Parser_peek(self);
            if (_t500.tag == Token_KwPass) {
#line 3409 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t500.tag == Token_Ident) {
                __auto_type evn = _t500.data.Ident.name;
#line 3411 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3412 "src/parser.tr"
                VariantDef* evd = VariantDef_init(evn);
#line 3413 "src/parser.tr"
                if (({ __auto_type _ord_t501 = (Parser_peek(self)); __auto_type _wr = ((_ord_t501.tag == Token_make_LParen().tag)); _wr; })) {
#line 3414 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3415 "src/parser.tr"
                    evd->fields = Parser_parse_param_list(self);
#line 3416 "src/parser.tr"
                    Parser_close_params(self);
                }
#line 3417 "src/parser.tr"
                List_ptr_append(e->variants, _tr_obj_retain(evd));
                _tr_obj_release(evd, _trdrop_VariantDef);
            } else if (1) {
                __auto_type _ = _t500;
#line 3419 "src/parser.tr"
                einl = false;
            }
#line 3420 "src/parser.tr"
            __auto_type _t502 = Parser_peek(self);
            if ((_t502.tag == Token_Comma || _t502.tag == Token_Semicolon)) {
#line 3422 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t502;
#line 3424 "src/parser.tr"
                einl = false;
            }
        }
#line 3425 "src/parser.tr"
        if (({ __auto_type _ord_t503 = (Parser_peek(self)); __auto_type _wr = ((_ord_t503.tag == Token_make_Newline().tag)); _wr; })) {
#line 3426 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3427 "src/parser.tr"
        Decl* _trret77 = box_decl(Decl_ctor_DEnum(e));
        _tr_str_release(name);
        _tr_obj_release(e, _trdrop_EnumDef);
        return _trret77;
    }
#line 3428 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3429 "src/parser.tr"
    __auto_type _t504 = Parser_peek(self);
    if (_t504.tag == Token_Indent) {
#line 3431 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t504;
#line 3433 "src/parser.tr"
        /* pass */
    }
#line 3434 "src/parser.tr"
    bool going = true;
#line 3435 "src/parser.tr"
    while (going) {
#line 3436 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3437 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3438 "src/parser.tr"
        while (({ __auto_type _ord_t505 = (Parser_peek(self)); __auto_type _wr = ((_ord_t505.tag == Token_make_At().tag)); _wr; })) {
#line 3439 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3440 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3441 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3442 "src/parser.tr"
            if (({ __auto_type _ord_t506 = (Parser_peek(self)); __auto_type _wr = ((_ord_t506.tag == Token_make_LParen().tag)); _wr; })) {
#line 3443 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3444 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3445 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3446 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3448 "src/parser.tr"
        bool is_p = false;
#line 3449 "src/parser.tr"
        __auto_type _t507 = Parser_peek(self);
        if (_t507.tag == Token_KwPub) {
#line 3451 "src/parser.tr"
            is_p = true;
#line 3452 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t507;
#line 3454 "src/parser.tr"
            /* pass */
        }
#line 3455 "src/parser.tr"
        __auto_type _t508 = Parser_peek(self);
        if ((_t508.tag == Token_Dedent || _t508.tag == Token_Eof)) {
#line 3457 "src/parser.tr"
            going = false;
        } else if (_t508.tag == Token_Ident) {
            __auto_type vname = _t508.data.Ident.name;
#line 3459 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3460 "src/parser.tr"
            VariantDef* vd = VariantDef_init(vname);
#line 3461 "src/parser.tr"
            if (({ __auto_type _ord_t509 = (Parser_peek(self)); __auto_type _wr = ((_ord_t509.tag == Token_make_LParen().tag)); _wr; })) {
#line 3462 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3463 "src/parser.tr"
                vd->fields = Parser_parse_param_list(self);
#line 3464 "src/parser.tr"
                Parser_close_params(self);
            }
#line 3465 "src/parser.tr"
            List_ptr_append(e->variants, _tr_obj_retain(vd));
#line 3466 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(vd, _trdrop_VariantDef);
        } else if (_t508.tag == Token_KwDef) {
#line 3468 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3469 "src/parser.tr"
            m->is_public = is_p;
#line 3470 "src/parser.tr"
            m->decorators = decorators;
#line 3471 "src/parser.tr"
            List_ptr_append(e->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t508.tag == Token_KwPass) {
#line 3473 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3474 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t508;
#line 3476 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3477 "src/parser.tr"
    if (({ __auto_type _ord_t510 = (Parser_peek(self)); __auto_type _wr = ((_ord_t510.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3478 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3479 "src/parser.tr"
    Decl* _trret78 = box_decl(Decl_ctor_DEnum(e));
    _tr_str_release(name);
    _tr_obj_release(e, _trdrop_EnumDef);
    return _trret78;
}

__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self) {
#line 3482 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3483 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3484 "src/parser.tr"
    InterfaceDef* i = InterfaceDef_init(name);
#line 3486 "src/parser.tr"
    if (({ __auto_type _ord_t511 = (Parser_peek(self)); __auto_type _wr = ((_ord_t511.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3487 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3488 "src/parser.tr"
        while ((({ __auto_type _ord_t512 = (Parser_peek(self)); __auto_type _wr = ((_ord_t512.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t513 = (Parser_peek(self)); __auto_type _wr = ((_ord_t513.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3489 "src/parser.tr"
            TrStr gname = Parser_consume_ident(self);
#line 3490 "src/parser.tr"
            if ((!_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL))))) {
#line 3490 "src/parser.tr"
                List_TrStr_append(i->generics, gname);
            }
#line 3491 "src/parser.tr"
            if (({ __auto_type _ord_t514 = (Parser_peek(self)); __auto_type _wr = ((_ord_t514.tag == Token_make_Comma().tag)); _wr; })) {
#line 3491 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(gname);
        }
#line 3492 "src/parser.tr"
        if (({ __auto_type _ord_t515 = (Parser_peek(self)); __auto_type _wr = ((_ord_t515.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3492 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3495 "src/parser.tr"
    if (({ __auto_type _ord_t516 = (Parser_peek(self)); __auto_type _wr = ((_ord_t516.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3496 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3497 "src/parser.tr"
        ({ TrStr _ord_t517 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t517); _tr_str_release(_ord_t517); });
#line 3498 "src/parser.tr"
        while (({ __auto_type _ord_t518 = (Parser_peek(self)); __auto_type _wr = ((_ord_t518.tag == Token_make_Comma().tag)); _wr; })) {
#line 3499 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3500 "src/parser.tr"
            ({ TrStr _ord_t519 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t519); _tr_str_release(_ord_t519); });
        }
    }
#line 3503 "src/parser.tr"
    bool i_inline = false;
#line 3504 "src/parser.tr"
    if (({ __auto_type _ord_t520 = (Parser_peek(self)); __auto_type _wr = ((_ord_t520.tag == Token_make_Colon().tag)); _wr; })) {
#line 3505 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3506 "src/parser.tr"
        __auto_type _t521 = Parser_peek(self);
        if ((_t521.tag == Token_Newline || _t521.tag == Token_Eof)) {
#line 3508 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t521;
#line 3510 "src/parser.tr"
            i_inline = true;
        }
    }
#line 3511 "src/parser.tr"
    if (i_inline) {
#line 3512 "src/parser.tr"
        bool iinl = true;
#line 3513 "src/parser.tr"
        while (iinl) {
#line 3514 "src/parser.tr"
            bool i_pub = false;
#line 3515 "src/parser.tr"
            __auto_type _t522 = Parser_peek(self);
            if (_t522.tag == Token_KwPub) {
#line 3517 "src/parser.tr"
                i_pub = true;
#line 3518 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t522;
#line 3520 "src/parser.tr"
                /* pass */
            }
#line 3521 "src/parser.tr"
            __auto_type _t523 = Parser_peek(self);
            if (_t523.tag == Token_KwPass) {
#line 3523 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t523.tag == Token_KwDef) {
#line 3525 "src/parser.tr"
                FunctionDef* im2 = Parser_parse_function_def(self, true);
#line 3526 "src/parser.tr"
                im2->is_public = i_pub;
#line 3527 "src/parser.tr"
                List_ptr_append(i->methods, _tr_obj_retain(im2));
                _tr_obj_release(im2, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t523;
#line 3529 "src/parser.tr"
                iinl = false;
            }
#line 3530 "src/parser.tr"
            __auto_type _t524 = Parser_peek(self);
            if (_t524.tag == Token_Semicolon) {
#line 3532 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t524;
#line 3534 "src/parser.tr"
                iinl = false;
            }
        }
#line 3535 "src/parser.tr"
        if (({ __auto_type _ord_t525 = (Parser_peek(self)); __auto_type _wr = ((_ord_t525.tag == Token_make_Newline().tag)); _wr; })) {
#line 3536 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3537 "src/parser.tr"
        Decl* _trret79 = box_decl(Decl_ctor_DInterface(i));
        _tr_str_release(name);
        _tr_obj_release(i, _trdrop_InterfaceDef);
        return _trret79;
    }
#line 3538 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3539 "src/parser.tr"
    __auto_type _t526 = Parser_peek(self);
    if (_t526.tag == Token_Indent) {
#line 3541 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t526;
#line 3543 "src/parser.tr"
        /* pass */
    }
#line 3544 "src/parser.tr"
    bool going = true;
#line 3545 "src/parser.tr"
    while (going) {
#line 3546 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3547 "src/parser.tr"
        bool is_p = false;
#line 3548 "src/parser.tr"
        __auto_type _t527 = Parser_peek(self);
        if (_t527.tag == Token_KwPub) {
#line 3550 "src/parser.tr"
            is_p = true;
#line 3551 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t527;
#line 3553 "src/parser.tr"
            /* pass */
        }
#line 3554 "src/parser.tr"
        __auto_type _t528 = Parser_peek(self);
        if ((_t528.tag == Token_Dedent || _t528.tag == Token_Eof)) {
#line 3556 "src/parser.tr"
            going = false;
        } else if (_t528.tag == Token_KwDef) {
#line 3558 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3559 "src/parser.tr"
            m->is_public = is_p;
#line 3560 "src/parser.tr"
            List_ptr_append(i->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t528.tag == Token_KwPass) {
#line 3562 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3563 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t528;
#line 3565 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3566 "src/parser.tr"
    if (({ __auto_type _ord_t529 = (Parser_peek(self)); __auto_type _wr = ((_ord_t529.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3567 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3568 "src/parser.tr"
    Decl* _trret80 = box_decl(Decl_ctor_DInterface(i));
    _tr_str_release(name);
    _tr_obj_release(i, _trdrop_InterfaceDef);
    return _trret80;
}

__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self) {
#line 3571 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3572 "src/parser.tr"
    TrStr target = Parser_consume_ident(self);
#line 3577 "src/parser.tr"
    if (({ __auto_type _ord_t530 = (Parser_peek(self)); __auto_type _wr = ((_ord_t530.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3578 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3579 "src/parser.tr"
        TrStr _erg = Parser_consume_ident(self);
#line 3580 "src/parser.tr"
        while (({ __auto_type _ord_t531 = (Parser_peek(self)); __auto_type _wr = ((_ord_t531.tag == Token_make_Comma().tag)); _wr; })) {
#line 3581 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3582 "src/parser.tr"
            TrStr _strtmp_t532 = Parser_consume_ident(self);
            _tr_str_release(_erg);
            _erg = _strtmp_t532;
        }
    }
#line 3583 "src/parser.tr"
    if (({ __auto_type _ord_t533 = (Parser_peek(self)); __auto_type _wr = ((_ord_t533.tag == Token_make_Colon().tag)); _wr; })) {
#line 3584 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3585 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3586 "src/parser.tr"
    __auto_type _t534 = Parser_peek(self);
    if (_t534.tag == Token_Indent) {
#line 3588 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t534;
#line 3590 "src/parser.tr"
        /* pass */
    }
#line 3591 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3592 "src/parser.tr"
    bool going = true;
#line 3593 "src/parser.tr"
    while (going) {
#line 3594 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3595 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3596 "src/parser.tr"
        while (({ __auto_type _ord_t535 = (Parser_peek(self)); __auto_type _wr = ((_ord_t535.tag == Token_make_At().tag)); _wr; })) {
#line 3597 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3598 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3599 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3600 "src/parser.tr"
            if (({ __auto_type _ord_t536 = (Parser_peek(self)); __auto_type _wr = ((_ord_t536.tag == Token_make_LParen().tag)); _wr; })) {
#line 3601 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3602 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3603 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3604 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3606 "src/parser.tr"
        bool is_p = false;
#line 3607 "src/parser.tr"
        __auto_type _t537 = Parser_peek(self);
        if (_t537.tag == Token_KwPub) {
#line 3609 "src/parser.tr"
            is_p = true;
#line 3610 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t537;
#line 3612 "src/parser.tr"
            /* pass */
        }
#line 3613 "src/parser.tr"
        __auto_type _t538 = Parser_peek(self);
        if ((_t538.tag == Token_Dedent || _t538.tag == Token_Eof)) {
#line 3615 "src/parser.tr"
            going = false;
        } else if (_t538.tag == Token_KwDef) {
#line 3617 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3618 "src/parser.tr"
            m->is_public = is_p;
#line 3619 "src/parser.tr"
            m->decorators = decorators;
#line 3620 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t538.tag == Token_KwAsync) {
#line 3633 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3634 "src/parser.tr"
            __auto_type _t539 = Parser_peek(self);
            if (_t539.tag == Token_KwDef) {
#line 3636 "src/parser.tr"
                FunctionDef* am = Parser_parse_function_def(self, true);
#line 3637 "src/parser.tr"
                am->is_async = true;
#line 3638 "src/parser.tr"
                am->is_public = is_p;
#line 3639 "src/parser.tr"
                am->decorators = decorators;
#line 3640 "src/parser.tr"
                List_ptr_append(fl, _tr_obj_retain(am));
                _tr_obj_release(am, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t539;
#line 3642 "src/parser.tr"
                /* pass */
            }
        } else if (_t538.tag == Token_KwPass) {
#line 3644 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3645 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t538;
#line 3647 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3648 "src/parser.tr"
    if (({ __auto_type _ord_t540 = (Parser_peek(self)); __auto_type _wr = ((_ord_t540.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3649 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3650 "src/parser.tr"
    Decl d = Decl_ctor_DExtend(target, fl);
#line 3651 "src/parser.tr"
    Decl* _trret81 = box_decl(d);
    _tr_str_release(target);
    return _trret81;
}

__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self) {
#line 3654 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3655 "src/parser.tr"
    TrStr abi = _tr_str_lit_len("C", 1LL);
#line 3656 "src/parser.tr"
    __auto_type _t541 = Parser_peek(self);
    if (_t541.tag == Token_StrLit) {
        __auto_type s = _t541.data.StrLit.val;
#line 3658 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3659 "src/parser.tr"
        TrStr _strtmp_t542 = _tr_str_retain(s);
        _tr_str_release(abi);
        abi = _strtmp_t542;
    } else if (1) {
        __auto_type _ = _t541;
#line 3661 "src/parser.tr"
        /* pass */
    }
#line 3662 "src/parser.tr"
    if (({ __auto_type _ord_t543 = (Parser_peek(self)); __auto_type _wr = ((_ord_t543.tag == Token_make_Colon().tag)); _wr; })) {
#line 3663 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3664 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3665 "src/parser.tr"
    __auto_type _t544 = Parser_peek(self);
    if (_t544.tag == Token_Indent) {
#line 3667 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t544;
#line 3669 "src/parser.tr"
        /* pass */
    }
#line 3670 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3671 "src/parser.tr"
    bool going = true;
#line 3672 "src/parser.tr"
    while (going) {
#line 3673 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3674 "src/parser.tr"
        bool is_p = false;
#line 3675 "src/parser.tr"
        __auto_type _t545 = Parser_peek(self);
        if (_t545.tag == Token_KwPub) {
#line 3677 "src/parser.tr"
            is_p = true;
#line 3678 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t545;
#line 3680 "src/parser.tr"
            /* pass */
        }
#line 3681 "src/parser.tr"
        __auto_type _t546 = Parser_peek(self);
        if ((_t546.tag == Token_Dedent || _t546.tag == Token_Eof)) {
#line 3683 "src/parser.tr"
            going = false;
        } else if (_t546.tag == Token_KwDef) {
#line 3685 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, false);
#line 3686 "src/parser.tr"
            m->is_public = is_p;
#line 3690 "src/parser.tr"
            if ((m->params->len > 0LL)) {
#line 3691 "src/parser.tr"
                Param* _ext_last = ((Param*)List_ptr_get(m->params, (m->params->len - 1LL)));
#line 3692 "src/parser.tr"
                if (_ext_last->is_variadic) {
#line 3693 "src/parser.tr"
                    ((Param*)List_ptr_pop(m->params));
#line 3694 "src/parser.tr"
                    m->is_variadic = true;
                }
            }
#line 3695 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (1) {
            __auto_type _ = _t546;
#line 3697 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3698 "src/parser.tr"
    if (({ __auto_type _ord_t547 = (Parser_peek(self)); __auto_type _wr = ((_ord_t547.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3699 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3700 "src/parser.tr"
    Decl* _trret82 = box_decl(Decl_ctor_DExtern(abi, fl));
    _tr_str_release(abi);
    return _trret82;
}

__attribute__((hot)) Expr* box_expr(Expr e) {
#line 14 "src/parser.tr"
    /* unsafe block */
#line 15 "src/parser.tr"
    Expr* p = ((Expr*)_tr_c_calloc((size_t)(1LL), sizeof(Expr)));
#line 16 "src/parser.tr"
    (*p = e);
#line 17 "src/parser.tr"
    return p;
}

__attribute__((hot)) AstType* throws_result_type(AstType* ret, AstType** err) {
#line 22 "src/parser.tr"
    AstType* rt = AstType_init(_tr_str_lit_len("Result", 6LL));
#line 23 "src/parser.tr"
    ({ __auto_type _ord_t548 = (box_asttype(ret)); List_ptr_append(rt->args, _ord_t548); });
#line 24 "src/parser.tr"
    List_ptr_append(rt->args, err);
#line 25 "src/parser.tr"
    rt->is_throws = true;
#line 26 "src/parser.tr"
    return rt;
}

__attribute__((hot)) Stmt* box_stmt(Stmt s) {
#line 29 "src/parser.tr"
    /* unsafe block */
#line 30 "src/parser.tr"
    Stmt* p = ((Stmt*)_tr_c_calloc((size_t)(1LL), sizeof(Stmt)));
#line 31 "src/parser.tr"
    (*p = s);
#line 32 "src/parser.tr"
    return p;
}

__attribute__((hot)) Decl* box_decl(Decl d) {
#line 35 "src/parser.tr"
    /* unsafe block */
#line 36 "src/parser.tr"
    Decl* p = ((Decl*)_tr_c_calloc((size_t)(1LL), sizeof(Decl)));
#line 37 "src/parser.tr"
    (*p = d);
#line 38 "src/parser.tr"
    return p;
}

__attribute__((hot)) AstType** box_asttype(AstType* t) {
#line 41 "src/parser.tr"
    /* unsafe block */
#line 42 "src/parser.tr"
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
#line 43 "src/parser.tr"
    (*p = t);
#line 44 "src/parser.tr"
    return p;
}

__attribute__((hot)) TrStr _nth_source_line(TrStr src, long long n) {
#line 49 "src/parser.tr"
    if ((n <= 0LL)) {
#line 49 "src/parser.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 50 "src/parser.tr"
    char* p = ((char*)(_tr_strz(src)));
#line 51 "src/parser.tr"
    long long i = 0LL;
#line 52 "src/parser.tr"
    long long line = 1LL;
#line 53 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init(80LL);
#line 54 "src/parser.tr"
    while (true) {
#line 55 "src/parser.tr"
        long long c = 0LL;
#line 56 "src/parser.tr"
        /* unsafe block */
#line 57 "src/parser.tr"
        c = ((long long)((*(p + i))));
#line 58 "src/parser.tr"
        if ((c == 0LL)) {
#line 58 "src/parser.tr"
            break;
        }
#line 59 "src/parser.tr"
        if ((line == n)) {
#line 60 "src/parser.tr"
            if (((c == 10LL) || (c == 13LL))) {
#line 60 "src/parser.tr"
                break;
            }
#line 61 "src/parser.tr"
            StringBuilder_append_char(sb, c);
        } else if ((line > n)) {
#line 63 "src/parser.tr"
            break;
        }
#line 64 "src/parser.tr"
        if ((c == 10LL)) {
#line 64 "src/parser.tr"
            line = (line + 1LL);
        }
#line 65 "src/parser.tr"
        i = (i + 1LL);
    }
#line 66 "src/parser.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _spaces(long long n) {
#line 70 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init((n + 1LL));
#line 71 "src/parser.tr"
    long long i = 0LL;
#line 72 "src/parser.tr"
    while ((i < n)) {
#line 73 "src/parser.tr"
        StringBuilder_append_char(sb, 32LL);
#line 74 "src/parser.tr"
        i = (i + 1LL);
    }
#line 75 "src/parser.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) bool _fstr_hex_ok(char* p, long long at, long long n, long long esc) {
#line 3707 "src/parser.tr"
    long long nd = 2LL;
#line 3708 "src/parser.tr"
    if ((esc == 117LL)) {
#line 3708 "src/parser.tr"
        nd = 4LL;
    }
#line 3709 "src/parser.tr"
    if ((esc == 85LL)) {
#line 3709 "src/parser.tr"
        nd = 8LL;
    }
#line 3710 "src/parser.tr"
    if (((at + nd) > n)) {
#line 3710 "src/parser.tr"
        return false;
    }
#line 3711 "src/parser.tr"
    long long v = 0LL;
#line 3712 "src/parser.tr"
    long long k = 0LL;
#line 3713 "src/parser.tr"
    while ((k < nd)) {
#line 3714 "src/parser.tr"
        long long h = lex_hex_val(((long long)((*(p + (at + k))))));
#line 3715 "src/parser.tr"
        if ((h < 0LL)) {
#line 3715 "src/parser.tr"
            return false;
        }
#line 3716 "src/parser.tr"
        v = ((v * 16LL) + h);
#line 3717 "src/parser.tr"
        k = (k + 1LL);
    }
#line 3718 "src/parser.tr"
    if (((esc != 120LL) && (v > 1114111LL))) {
#line 3718 "src/parser.tr"
        return false;
    }
#line 3719 "src/parser.tr"
    return true;
}

__attribute__((hot)) long long _find_fmt_colon(TrStr s) {
#line 3722 "src/parser.tr"
    long long i = 0LL;
#line 3723 "src/parser.tr"
    long long n = _tr_str_lenv((s));
#line 3724 "src/parser.tr"
    long long depth = 0LL;
#line 3725 "src/parser.tr"
    while ((i < n)) {
#line 3726 "src/parser.tr"
        long long c = _tr_str_char_at_codev((s), i);
#line 3727 "src/parser.tr"
        if (((c == 34LL) || (c == 39LL))) {
#line 3729 "src/parser.tr"
            long long qc = c;
#line 3730 "src/parser.tr"
            i = (i + 1LL);
#line 3731 "src/parser.tr"
            while (((i < n) && (_tr_str_char_at_codev((s), i) != qc))) {
#line 3732 "src/parser.tr"
                if ((_tr_str_char_at_codev((s), i) == 92LL)) {
#line 3732 "src/parser.tr"
                    i = (i + 1LL);
                }
#line 3733 "src/parser.tr"
                i = (i + 1LL);
            }
        } else if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
#line 3734 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
#line 3735 "src/parser.tr"
            depth = (depth - 1LL);
        } else if (((c == 58LL) && (depth == 0LL))) {
#line 3736 "src/parser.tr"
            return i;
        }
#line 3737 "src/parser.tr"
        i = (i + 1LL);
    }
#line 3738 "src/parser.tr"
    return (-1LL);
}

