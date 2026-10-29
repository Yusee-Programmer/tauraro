#include "tauraro_types.h"

TrStr Parser__peek_cmp_op(Parser* self);

__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines) {
#line 126 "src/parser.tr"
    Parser* p = ((Parser*)_tr_obj_alloc(sizeof(Parser)));
#line 127 "src/parser.tr"
    p->tokens = tokens;
#line 128 "src/parser.tr"
    p->lines = lines;
#line 129 "src/parser.tr"
    p->cols = (void*)List_i64_new();
#line 130 "src/parser.tr"
    p->src_text = _tr_str_lit_len("", 0LL);
#line 131 "src/parser.tr"
    p->pos = 0LL;
#line 132 "src/parser.tr"
    p->error_count = 0LL;
#line 133 "src/parser.tr"
    p->aug_ctr = 0LL;
#line 134 "src/parser.tr"
    p->current_file = _tr_str_lit_len("", 0LL);
#line 135 "src/parser.tr"
    p->import_aliases = _tr_dict_new(8LL);
#line 136 "src/parser.tr"
    return p;
}

__attribute__((hot)) Token Parser_peek(Parser* self) {
#line 139 "src/parser.tr"
    if ((self->pos < self->tokens->len)) {
#line 140 "src/parser.tr"
        return List_Token_get(self->tokens, self->pos);
    }
#line 141 "src/parser.tr"
    return Token_make_Eof();
}

__attribute__((hot)) Token Parser_advance(Parser* self) {
#line 144 "src/parser.tr"
    Token tok = Parser_peek(self);
#line 145 "src/parser.tr"
    if ((self->pos < self->tokens->len)) {
#line 146 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 147 "src/parser.tr"
    return tok;
}

__attribute__((hot)) void Parser_skip_newlines(Parser* self) {
#line 150 "src/parser.tr"
    bool going = true;
#line 151 "src/parser.tr"
    while (going) {
#line 152 "src/parser.tr"
        going = false;
#line 153 "src/parser.tr"
        __auto_type _t33 = Parser_peek(self);
        if (_t33.tag == Token_Newline) {
#line 155 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 156 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t33;
#line 159 "src/parser.tr"
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self) {
#line 161 "src/parser.tr"
    bool going = true;
#line 162 "src/parser.tr"
    while (going) {
#line 163 "src/parser.tr"
        going = false;
#line 164 "src/parser.tr"
        __auto_type _t34 = Parser_peek(self);
        if ((_t34.tag == Token_Newline || _t34.tag == Token_Indent || _t34.tag == Token_Dedent)) {
#line 166 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 167 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t34;
#line 170 "src/parser.tr"
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_expect_newline(Parser* self) {
#line 172 "src/parser.tr"
    __auto_type _t35 = Parser_peek(self);
    if ((_t35.tag == Token_Newline || _t35.tag == Token_Semicolon)) {
#line 174 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if ((_t35.tag == Token_Eof || _t35.tag == Token_Dedent || _t35.tag == Token_Indent || _t35.tag == Token_RBrace || _t35.tag == Token_RParen || _t35.tag == Token_RBracket || _t35.tag == Token_Comma)) {
#line 176 "src/parser.tr"
        /* pass */
    } else if ((_t35.tag == Token_KwElse || _t35.tag == Token_KwElif || _t35.tag == Token_KwExcept || _t35.tag == Token_KwFinally || _t35.tag == Token_KwCase)) {
#line 178 "src/parser.tr"
        /* pass */
    } else if (_t35.tag == Token_Error) {
        __auto_type _ = _t35.data.Error.msg;
#line 180 "src/parser.tr"
        /* pass */
    } else if (1) {
        __auto_type _ = _t35;
#line 184 "src/parser.tr"
        if (((self->pos > 0LL) && (self->pos <= self->tokens->len))) {
#line 185 "src/parser.tr"
            __auto_type _t36 = List_Token_get(self->tokens, (self->pos - 1LL));
            if ((_t36.tag == Token_Newline || _t36.tag == Token_Dedent || _t36.tag == Token_Semicolon)) {
                return;
            } else if (1) {
                __auto_type _ = _t36;
                /* pass */
            }
        }
#line 191 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("unexpected token after the end of the statement", 47LL), _tr_str_lit_len("each statement goes on its own line (or separate them with ';') - check for a missing operator, comma or ')' before this token.", 127LL));
#line 192 "src/parser.tr"
        while (true) {
#line 193 "src/parser.tr"
            __auto_type _t37 = Parser_peek(self);
            if ((_t37.tag == Token_Newline || _t37.tag == Token_Eof || _t37.tag == Token_Dedent || _t37.tag == Token_Semicolon)) {
#line 195 "src/parser.tr"
                break;
            } else if (1) {
                __auto_type _ = _t37;
                self->pos = (self->pos + 1LL);
            }
        }
    }
}

__attribute__((hot)) void Parser_close_params(Parser* self) {
#line 201 "src/parser.tr"
    if (({ __auto_type _ord_t38 = (Parser_peek(self)); __auto_type _wr = ((_ord_t38.tag == Token_make_RParen().tag)); _wr; })) {
#line 202 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else {
#line 204 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected ')' to close the parameter list", 40LL), _tr_str_lit_len("check for a missing ')' or a stray character in the parameters (a parameter is written 'name: Type').", 101LL));
    }
}

__attribute__((hot)) void Parser_emit_diag_prev(Parser* self, TrStr msg, TrStr hint) {
#line 209 "src/parser.tr"
    if ((self->pos > 0LL)) {
#line 210 "src/parser.tr"
        self->pos = (self->pos - 1LL);
#line 211 "src/parser.tr"
        Parser_emit_diag(self, msg, hint);
#line 212 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else {
#line 214 "src/parser.tr"
        Parser_emit_diag(self, msg, hint);
    }
}

__attribute__((hot)) bool Parser_at_end(Parser* self) {
#line 217 "src/parser.tr"
    __auto_type _t39 = Parser_peek(self);
    if (_t39.tag == Token_Eof) {
#line 219 "src/parser.tr"
        return true;
    } else if (1) {
        __auto_type _ = _t39;
#line 221 "src/parser.tr"
        /* pass */
    }
#line 222 "src/parser.tr"
    return false;
}

__attribute__((hot)) long long Parser_cur_line(Parser* self) {
#line 225 "src/parser.tr"
    if ((self->pos < self->lines->len)) {
#line 226 "src/parser.tr"
        return List_i64_get(self->lines, self->pos);
    }
#line 227 "src/parser.tr"
    if ((self->lines->len > 0LL)) {
#line 228 "src/parser.tr"
        return List_i64_get(self->lines, (self->lines->len - 1LL));
    }
#line 229 "src/parser.tr"
    return 0LL;
}

__attribute__((hot)) long long Parser_cur_col(Parser* self) {
#line 234 "src/parser.tr"
    if ((self->pos < self->cols->len)) {
#line 235 "src/parser.tr"
        return List_i64_get(self->cols, self->pos);
    }
#line 236 "src/parser.tr"
    if ((self->cols->len > 0LL)) {
#line 237 "src/parser.tr"
        return List_i64_get(self->cols, (self->cols->len - 1LL));
    }
#line 238 "src/parser.tr"
    return 0LL;
}

__attribute__((hot)) void Parser_emit_diag(Parser* self, TrStr msg, TrStr hint) {
#line 249 "src/parser.tr"
    long long ln = Parser_cur_line(self);
#line 250 "src/parser.tr"
    long long col = Parser_cur_col(self);
#line 251 "src/parser.tr"
    ({ __auto_type _ord_t40 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t40); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
#line 252 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 253 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 254 "src/parser.tr"
        TrStr _strtmp_t41 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t41;
    }
#line 255 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 256 "src/parser.tr"
    if ((col > 0LL)) {
#line 257 "src/parser.tr"
        ({ TrStr _ap_t42 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t43 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t42); _tr_str_append(&head, _ap_t43); _tr_str_release(_ap_t43); });
    }
#line 258 "src/parser.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 260 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 261 "src/parser.tr"
        TrStr srcline = _nth_source_line(self->src_text, ln);
#line 262 "src/parser.tr"
        long long full_col = col;
#line 263 "src/parser.tr"
        long long col1 = _clip_col(srcline, full_col);
#line 264 "src/parser.tr"
        TrStr _strtmp_t44 = _clip_line(srcline, full_col);
        _tr_str_release(srcline);
        srcline = _strtmp_t44;
#line 265 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 266 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 267 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 268 "src/parser.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 269 "src/parser.tr"
            if ((col1 > 0LL)) {
#line 272 "src/parser.tr"
                long long width = 1LL;
#line 273 "src/parser.tr"
                if ((((self->pos + 1LL) < self->cols->len) && ((self->pos + 1LL) < self->lines->len))) {
#line 274 "src/parser.tr"
                    if ((List_i64_get(self->lines, (self->pos + 1LL)) == ln)) {
#line 275 "src/parser.tr"
                        long long d = (List_i64_get(self->cols, (self->pos + 1LL)) - col);
#line 276 "src/parser.tr"
                        if (((d > 0LL) && (d < 200LL))) {
#line 277 "src/parser.tr"
                            width = d;
                        }
                    }
                }
#line 278 "src/parser.tr"
                ({ __auto_type _ord_t45 = (c_dim(gbar)); __auto_type _ord_t47 = (({ TrStr _tr_cl = (_ord_t45); TrStr _tr_cr = (_spaces((col1 - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _at_t46 = (repeat_char(_tr_str_lit_len("^", 1LL), width)); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t47); TrStr _tr_cr = (c_red(_at_t46)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); _tr_str_release(_at_t46); });
            }
            _tr_str_release(gnum);
        }
    }
#line 279 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 280 "src/parser.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
    }
#line 281 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) TrStr Parser_consume_ident(Parser* self) {
#line 284 "src/parser.tr"
    __auto_type _t48 = Parser_peek(self);
    if (_t48.tag == Token_Ident) {
        __auto_type name = _t48.data.Ident.name;
#line 286 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 287 "src/parser.tr"
        TrStr _trret15 = _tr_str_retain(name);
        return _trret15;
    } else if (_t48.tag == Token_KwInt) {
#line 289 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 290 "src/parser.tr"
        return _tr_str_lit_len("int", 3LL);
    } else if (_t48.tag == Token_KwFloat) {
#line 292 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 293 "src/parser.tr"
        return _tr_str_lit_len("float", 5LL);
    } else if (_t48.tag == Token_KwBool) {
#line 295 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 296 "src/parser.tr"
        return _tr_str_lit_len("bool", 4LL);
    } else if ((_t48.tag == Token_KwStr || _t48.tag == Token_KwString)) {
#line 298 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 299 "src/parser.tr"
        return _tr_str_lit_len("str", 3LL);
    } else if (_t48.tag == Token_KwChar) {
#line 301 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 302 "src/parser.tr"
        return _tr_str_lit_len("char", 4LL);
    } else if (_t48.tag == Token_KwVoid) {
#line 304 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 305 "src/parser.tr"
        return _tr_str_lit_len("void", 4LL);
    } else if (_t48.tag == Token_KwExtend) {
#line 308 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 309 "src/parser.tr"
        return _tr_str_lit_len("extend", 6LL);
    } else if (_t48.tag == Token_KwNone) {
#line 312 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 313 "src/parser.tr"
        return _tr_str_lit_len("None", 4LL);
    } else if (_t48.tag == Token_KwLambda) {
#line 316 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 317 "src/parser.tr"
        return _tr_str_lit_len("lambda", 6LL);
    } else if (_t48.tag == Token_KwSpawn) {
#line 320 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 321 "src/parser.tr"
        return _tr_str_lit_len("spawn", 5LL);
    } else if (_t48.tag == Token_KwAsync) {
#line 324 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 325 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (1) {
        __auto_type _ = _t48;
#line 327 "src/parser.tr"
        /* pass */
    }
#line 328 "src/parser.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Parser_consume_method_ident(Parser* self) {
#line 341 "src/parser.tr"
    TrStr base = Parser_consume_ident(self);
#line 342 "src/parser.tr"
    if ((!_tr_str_eqv((base), (_tr_str_lit_len("", 0LL))))) {
#line 342 "src/parser.tr"
        TrStr _trret16 = _tr_str_retain(base);
        _tr_str_release(base);
        return _trret16;
    }
#line 358 "src/parser.tr"
    __auto_type _t49 = Parser_peek(self);
    if (_t49.tag == Token_KwDef) {
#line 360 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 361 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("def", 3LL);
    } else if (_t49.tag == Token_KwClass) {
#line 363 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 364 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("class", 5LL);
    } else if (_t49.tag == Token_KwEnum) {
#line 366 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 367 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("enum", 4LL);
    } else if (_t49.tag == Token_KwInterface) {
#line 369 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 370 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("interface", 9LL);
    } else if (_t49.tag == Token_KwStruct) {
#line 372 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 373 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("struct", 6LL);
    } else if (_t49.tag == Token_KwIf) {
#line 375 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 376 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("if", 2LL);
    } else if (_t49.tag == Token_KwElif) {
#line 378 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 379 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("elif", 4LL);
    } else if (_t49.tag == Token_KwElse) {
#line 381 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 382 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("else", 4LL);
    } else if (_t49.tag == Token_KwFor) {
#line 384 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 385 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("for", 3LL);
    } else if (_t49.tag == Token_KwWhile) {
#line 387 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 388 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("while", 5LL);
    } else if (_t49.tag == Token_KwLoop) {
#line 390 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 391 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("loop", 4LL);
    } else if (_t49.tag == Token_KwReturn) {
#line 393 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 394 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("return", 6LL);
    } else if (_t49.tag == Token_KwBreak) {
#line 396 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 397 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("break", 5LL);
    } else if (_t49.tag == Token_KwContinue) {
#line 399 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 400 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("continue", 8LL);
    } else if (_t49.tag == Token_KwPass) {
#line 402 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 403 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pass", 4LL);
    } else if (_t49.tag == Token_KwMatch) {
#line 405 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 406 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("match", 5LL);
    } else if (_t49.tag == Token_KwCase) {
#line 408 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 409 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("case", 4LL);
    } else if (_t49.tag == Token_KwTry) {
#line 411 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 412 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("try", 3LL);
    } else if (_t49.tag == Token_KwExcept) {
#line 414 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 415 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("except", 6LL);
    } else if (_t49.tag == Token_KwFinally) {
#line 417 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 418 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("finally", 7LL);
    } else if (_t49.tag == Token_KwRaise) {
#line 420 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 421 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("raise", 5LL);
    } else if (_t49.tag == Token_KwAssert) {
#line 423 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 424 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("assert", 6LL);
    } else if (_t49.tag == Token_KwWith) {
#line 426 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 427 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("with", 4LL);
    } else if (_t49.tag == Token_KwDefer) {
#line 429 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 430 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("defer", 5LL);
    } else if (_t49.tag == Token_KwAsm) {
#line 432 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 433 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("asm", 3LL);
    } else if (_t49.tag == Token_KwTaskGroup) {
#line 435 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 436 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("taskgroup", 9LL);
    } else if (_t49.tag == Token_KwImplements) {
#line 438 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 439 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("implements", 10LL);
    } else if (_t49.tag == Token_KwImport) {
#line 441 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 442 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("import", 6LL);
    } else if (_t49.tag == Token_KwFrom) {
#line 444 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 445 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("from", 4LL);
    } else if (_t49.tag == Token_KwAs) {
#line 447 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 448 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("as", 2LL);
    } else if (_t49.tag == Token_KwIn) {
#line 450 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 451 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("in", 2LL);
    } else if (_t49.tag == Token_KwMut) {
#line 453 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 454 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("mut", 3LL);
    } else if (_t49.tag == Token_KwShared) {
#line 456 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 457 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("shared", 6LL);
    } else if (_t49.tag == Token_KwExtern) {
#line 459 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 460 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extern", 6LL);
    } else if (_t49.tag == Token_KwUnsafe) {
#line 462 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 463 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("unsafe", 6LL);
    } else if (_t49.tag == Token_KwThrows) {
#line 465 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 466 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("throws", 6LL);
    } else if (_t49.tag == Token_KwExtends) {
#line 468 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 469 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extends", 7LL);
    } else if (_t49.tag == Token_KwAwait) {
#line 471 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 472 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("await", 5LL);
    } else if (_t49.tag == Token_KwYield) {
#line 474 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 475 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("yield", 5LL);
    } else if (_t49.tag == Token_KwPub) {
#line 477 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 478 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pub", 3LL);
    } else if (_t49.tag == Token_KwStatic) {
#line 480 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 481 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("static", 6LL);
    } else if (_t49.tag == Token_KwConst) {
#line 483 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 484 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("const", 5LL);
    } else if (_t49.tag == Token_KwActor) {
#line 486 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 487 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("actor", 5LL);
    } else if (_t49.tag == Token_KwSuper) {
#line 489 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 490 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("super", 5LL);
    } else if (_t49.tag == Token_KwExport) {
#line 492 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 493 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("export", 6LL);
    } else if (_t49.tag == Token_KwDecorator) {
#line 495 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 496 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("decorator", 9LL);
    } else if (_t49.tag == Token_KwMacro) {
#line 498 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 499 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("macro", 5LL);
    } else if (_t49.tag == Token_KwDo) {
#line 501 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 502 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("do", 2LL);
    } else if (_t49.tag == Token_KwSizeOf) {
#line 504 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 505 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("sizeof", 6LL);
    } else if (_t49.tag == Token_KwIs) {
#line 507 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 508 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("is", 2LL);
    } else if (_t49.tag == Token_KwTrue) {
#line 510 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 511 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("true", 4LL);
    } else if (_t49.tag == Token_KwFalse) {
#line 513 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 514 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("false", 5LL);
    } else if (_t49.tag == Token_KwAnd) {
#line 516 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 517 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("and", 3LL);
    } else if (_t49.tag == Token_KwOr) {
#line 519 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 520 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("or", 2LL);
    } else if (_t49.tag == Token_KwNot) {
#line 522 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 523 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("not", 3LL);
    } else if (1) {
        __auto_type _ = _t49;
#line 525 "src/parser.tr"
        /* pass */
    }
#line 526 "src/parser.tr"
    _tr_str_release(base);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Parser_consume_module_ident(Parser* self) {
#line 531 "src/parser.tr"
    __auto_type _t50 = Parser_peek(self);
    if (_t50.tag == Token_KwAsync) {
#line 533 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 534 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (_t50.tag == Token_KwMatch) {
#line 536 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 537 "src/parser.tr"
        return _tr_str_lit_len("match", 5LL);
    } else if (_t50.tag == Token_KwFor) {
#line 539 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 540 "src/parser.tr"
        return _tr_str_lit_len("for", 3LL);
    } else if (_t50.tag == Token_KwIn) {
#line 542 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 543 "src/parser.tr"
        return _tr_str_lit_len("in", 2LL);
    } else if (_t50.tag == Token_KwIs) {
#line 545 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 546 "src/parser.tr"
        return _tr_str_lit_len("is", 2LL);
    } else if (1) {
        __auto_type _ = _t50;
#line 548 "src/parser.tr"
        return Parser_consume_ident(self);
    }
#line 549 "src/parser.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) AstType* Parser_parse_type(Parser* self) {
#line 554 "src/parser.tr"
    TrStr name = _tr_str_lit_len("", 0LL);
#line 555 "src/parser.tr"
    __auto_type _t51 = Parser_peek(self);
    if (_t51.tag == Token_KwNone) {
#line 557 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 558 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t51.tag == Token_LParen) {
#line 561 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 562 "src/parser.tr"
        AstType* tup_t = AstType_init(_tr_str_lit_len("Tuple", 5LL));
#line 563 "src/parser.tr"
        while ((({ __auto_type _ord_t52 = (Parser_peek(self)); __auto_type _wr = ((_ord_t52.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t53 = (Parser_peek(self)); __auto_type _wr = ((_ord_t53.tag != Token_make_Eof().tag)); _wr; }))) {
#line 564 "src/parser.tr"
            AstType* elem_t = Parser_parse_type(self);
#line 565 "src/parser.tr"
            ({ __auto_type _ord_t54 = (box_asttype(elem_t)); List_ptr_append(tup_t->args, _ord_t54); });
#line 566 "src/parser.tr"
            if (({ __auto_type _ord_t55 = (Parser_peek(self)); __auto_type _wr = ((_ord_t55.tag == Token_make_Comma().tag)); _wr; })) {
#line 566 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 567 "src/parser.tr"
        if (({ __auto_type _ord_t56 = (Parser_peek(self)); __auto_type _wr = ((_ord_t56.tag == Token_make_RParen().tag)); _wr; })) {
#line 567 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 568 "src/parser.tr"
        _tr_str_release(name);
        return tup_t;
    } else if (_t51.tag == Token_LBracket) {
#line 571 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 572 "src/parser.tr"
        AstType* arr_t = AstType_init(_tr_str_lit_len("Array", 5LL));
#line 573 "src/parser.tr"
        AstType* arr_elem = Parser_parse_type(self);
#line 574 "src/parser.tr"
        ({ __auto_type _ord_t57 = (box_asttype(arr_elem)); List_ptr_append(arr_t->args, _ord_t57); });
#line 575 "src/parser.tr"
        if (({ __auto_type _ord_t58 = (Parser_peek(self)); __auto_type _wr = ((_ord_t58.tag == Token_make_Semicolon().tag)); _wr; })) {
#line 576 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 577 "src/parser.tr"
            __auto_type _t59 = Parser_peek(self);
            if (_t59.tag == Token_IntLit) {
                __auto_type arr_sz = _t59.data.IntLit.val;
#line 579 "src/parser.tr"
                arr_t->array_size = arr_sz;
#line 580 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t59;
                /* pass */
            }
        }
#line 582 "src/parser.tr"
        if (({ __auto_type _ord_t60 = (Parser_peek(self)); __auto_type _wr = ((_ord_t60.tag == Token_make_RBracket().tag)); _wr; })) {
#line 582 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 583 "src/parser.tr"
        _tr_str_release(name);
        return arr_t;
    } else if (_t51.tag == Token_KwDef) {
#line 588 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 589 "src/parser.tr"
        AstType* dfn_t = AstType_init(_tr_str_lit_len("def", 3LL));
#line 590 "src/parser.tr"
        if (({ __auto_type _ord_t61 = (Parser_peek(self)); __auto_type _wr = ((_ord_t61.tag == Token_make_LParen().tag)); _wr; })) {
#line 591 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 592 "src/parser.tr"
            while ((({ __auto_type _ord_t62 = (Parser_peek(self)); __auto_type _wr = ((_ord_t62.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t63 = (Parser_peek(self)); __auto_type _wr = ((_ord_t63.tag != Token_make_Eof().tag)); _wr; }))) {
#line 593 "src/parser.tr"
                AstType* dpt = Parser_parse_type(self);
#line 594 "src/parser.tr"
                ({ __auto_type _ord_t64 = (box_asttype(dpt)); List_ptr_append(dfn_t->args, _ord_t64); });
#line 595 "src/parser.tr"
                if (({ __auto_type _ord_t65 = (Parser_peek(self)); __auto_type _wr = ((_ord_t65.tag == Token_make_Comma().tag)); _wr; })) {
#line 595 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
            }
#line 596 "src/parser.tr"
            if (({ __auto_type _ord_t66 = (Parser_peek(self)); __auto_type _wr = ((_ord_t66.tag == Token_make_RParen().tag)); _wr; })) {
#line 596 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 599 "src/parser.tr"
        AstType** dthrows = (AstType**)(0LL);
#line 600 "src/parser.tr"
        if (({ __auto_type _ord_t67 = (Parser_peek(self)); __auto_type _wr = ((_ord_t67.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 601 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 602 "src/parser.tr"
            dthrows = box_asttype(Parser_parse_type(self));
        }
#line 603 "src/parser.tr"
        AstType* dret = AstType_init(_tr_str_lit_len("void", 4LL));
#line 604 "src/parser.tr"
        if (({ __auto_type _ord_t68 = (Parser_peek(self)); __auto_type _wr = ((_ord_t68.tag == Token_make_Arrow().tag)); _wr; })) {
#line 605 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 606 "src/parser.tr"
            dret = Parser_parse_type(self);
        }
#line 607 "src/parser.tr"
        if ((((unsigned long long)(dthrows)) != ((unsigned long long)(0LL)))) {
#line 608 "src/parser.tr"
            dret = throws_result_type(dret, dthrows);
        }
#line 609 "src/parser.tr"
        ({ __auto_type _ord_t69 = (box_asttype(dret)); List_ptr_append(dfn_t->args, _ord_t69); });
#line 610 "src/parser.tr"
        _tr_str_release(name);
        return dfn_t;
    } else if (_t51.tag == Token_KwMut) {
#line 614 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 615 "src/parser.tr"
        TrStr mr_name = Parser_consume_ident(self);
#line 616 "src/parser.tr"
        if (_tr_str_eqv((mr_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 617 "src/parser.tr"
            AstType* mr_inner = Parser_parse_type(self);
#line 618 "src/parser.tr"
            mr_inner->is_borrow = true;
#line 619 "src/parser.tr"
            mr_inner->is_mut_borrow = true;
#line 620 "src/parser.tr"
            _tr_str_release(mr_name);
            _tr_str_release(name);
            return mr_inner;
        }
#line 622 "src/parser.tr"
        _tr_str_release(mr_name);
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (1) {
        __auto_type _ = _t51;
#line 624 "src/parser.tr"
        TrStr _strtmp_t70 = Parser_consume_ident(self);
        _tr_str_release(name);
        name = _strtmp_t70;
    }
#line 626 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 627 "src/parser.tr"
        Parser_emit_diag_prev(self, _tr_str_lit_len("expected a type here", 20LL), _tr_str_lit_len("write a type name such as 'int', 'str', 'List[int]' or a class name.", 68LL));
#line 629 "src/parser.tr"
        __auto_type _t71 = Parser_peek(self);
        if ((_t71.tag == Token_Eof || _t71.tag == Token_RParen || _t71.tag == Token_RBracket)) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t71;
            self->pos = (self->pos + 1LL);
        }
#line 632 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
#line 638 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL)))) {
#line 639 "src/parser.tr"
        AstType* ref_inner = Parser_parse_type(self);
#line 640 "src/parser.tr"
        ref_inner->is_borrow = true;
#line 641 "src/parser.tr"
        _tr_str_release(name);
        return ref_inner;
    }
#line 644 "src/parser.tr"
    __auto_type _t72 = Parser_peek(self);
    if (_t72.tag == Token_LBracket) {
#line 646 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 647 "src/parser.tr"
        AstType* t = AstType_init(name);
#line 648 "src/parser.tr"
        bool going = true;
#line 649 "src/parser.tr"
        while (going) {
#line 650 "src/parser.tr"
            __auto_type _t73 = Parser_peek(self);
            if ((_t73.tag == Token_RBracket || _t73.tag == Token_Eof)) {
#line 652 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t73;
#line 654 "src/parser.tr"
                /* pass */
            }
#line 655 "src/parser.tr"
            if (going) {
#line 656 "src/parser.tr"
                __auto_type arg_t = Parser_parse_type(self);
#line 657 "src/parser.tr"
                ({ __auto_type _ord_t74 = (box_asttype(arg_t)); List_ptr_append(t->args, _ord_t74); });
#line 658 "src/parser.tr"
                __auto_type _t75 = Parser_peek(self);
                if (_t75.tag == Token_Comma) {
#line 660 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (_t75.tag == Token_Semicolon) {
#line 665 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 666 "src/parser.tr"
                    __auto_type _t76 = Parser_peek(self);
                    if (_t76.tag == Token_IntLit) {
                        __auto_type gsz = _t76.data.IntLit.val;
#line 668 "src/parser.tr"
                        t->array_size = gsz;
#line 669 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t76;
                        /* pass */
                    }
#line 671 "src/parser.tr"
                    going = false;
                } else if (1) {
                    __auto_type _ = _t75;
#line 673 "src/parser.tr"
                    going = false;
                }
            }
        }
#line 674 "src/parser.tr"
        __auto_type _t77 = Parser_peek(self);
        if (_t77.tag == Token_RBracket) {
#line 676 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t77;
#line 678 "src/parser.tr"
            ({ TrStr _at_t78 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the type '", 26LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[...]'", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t78, _tr_str_lit_len("add the matching ']' after the type arguments.", 46LL)); _tr_str_release(_at_t78); });
        }
#line 679 "src/parser.tr"
        _tr_str_release(name);
        return t;
    } else if (1) {
        __auto_type _ = _t72;
#line 681 "src/parser.tr"
        /* pass */
    }
#line 682 "src/parser.tr"
    return AstType_init(name);
}

__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self) {
#line 687 "src/parser.tr"
    List_ptr* pl = (void*)List_ptr_new();
#line 688 "src/parser.tr"
    bool going = true;
#line 689 "src/parser.tr"
    while (going) {
#line 690 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 691 "src/parser.tr"
        __auto_type _t79 = Parser_peek(self);
        if ((_t79.tag == Token_RParen || _t79.tag == Token_Eof)) {
#line 693 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t79;
#line 695 "src/parser.tr"
            /* pass */
        }
#line 696 "src/parser.tr"
        if (going) {
#line 697 "src/parser.tr"
            TrStr item_name = Parser_consume_ident(self);
#line 710 "src/parser.tr"
            if (((_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))) && ({ __auto_type _ord_t80 = (Parser_peek(self)); __auto_type _wr = ((_ord_t80.tag != Token_make_RParen().tag)); _wr; })) && ({ __auto_type _ord_t81 = (Parser_peek(self)); __auto_type _wr = ((_ord_t81.tag != Token_make_Eof().tag)); _wr; }))) {
#line 711 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a parameter name, found a reserved keyword", 51LL), _tr_str_lit_len("reserved words cannot be used as a parameter name (they introduce a new binding, same as a 'mut'/'let' variable) — rename this parameter.", 139LL));
#line 712 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 713 "src/parser.tr"
            if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 714 "src/parser.tr"
                AstType** ty_ptr = (AstType**)(0LL);
#line 715 "src/parser.tr"
                __auto_type _t82 = Parser_peek(self);
                if (_t82.tag == Token_Colon) {
#line 717 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 718 "src/parser.tr"
                    __auto_type ty = Parser_parse_type(self);
#line 721 "src/parser.tr"
                    if (({ __auto_type _ord_t83 = (Parser_peek(self)); __auto_type _wr = ((_ord_t83.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 722 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 723 "src/parser.tr"
                        ty->from_param = Parser_consume_ident(self);
#line 724 "src/parser.tr"
                        List_TrStr_append(ty->from_regions, ty->from_param);
                    }
#line 725 "src/parser.tr"
                    ty_ptr = box_asttype(ty);
                } else if (1) {
                    __auto_type _ = _t82;
#line 731 "src/parser.tr"
                    if (_tr_str_eqv((item_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 732 "src/parser.tr"
                        AstType* bt = Parser_parse_type(self);
#line 733 "src/parser.tr"
                        bt->is_borrow = true;
#line 734 "src/parser.tr"
                        if (({ __auto_type _ord_t84 = (Parser_peek(self)); __auto_type _wr = ((_ord_t84.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 735 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
#line 736 "src/parser.tr"
                            bt->from_param = Parser_consume_ident(self);
#line 737 "src/parser.tr"
                            List_TrStr_append(bt->from_regions, bt->from_param);
                        }
#line 738 "src/parser.tr"
                        ty_ptr = box_asttype(bt);
#line 741 "src/parser.tr"
                        TrStr _strtmp_t85 = _tr_str_retain(bt->name);
                        _tr_str_release(item_name);
                        item_name = _strtmp_t85;
                    }
                }
#line 742 "src/parser.tr"
                Param* p = Param_init(item_name, ty_ptr);
#line 743 "src/parser.tr"
                if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 744 "src/parser.tr"
                    if ((*ty_ptr)->is_borrow) {
#line 745 "src/parser.tr"
                        p->is_ref = true;
                    }
#line 746 "src/parser.tr"
                    if ((*ty_ptr)->is_mut_borrow) {
#line 747 "src/parser.tr"
                        p->is_mut_ref = true;
                    }
                }
#line 750 "src/parser.tr"
                if (({ __auto_type _ord_t86 = (Parser_peek(self)); __auto_type _wr = ((_ord_t86.tag == Token_make_DotDotDot().tag)); _wr; })) {
#line 751 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 752 "src/parser.tr"
                    p->is_variadic = true;
                }
#line 753 "src/parser.tr"
                List_ptr_append(pl, _tr_obj_retain(p));
#line 754 "src/parser.tr"
                __auto_type _t87 = Parser_peek(self);
                if (_t87.tag == Token_Comma) {
#line 756 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 757 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                } else if (1) {
                    __auto_type _ = _t87;
#line 759 "src/parser.tr"
                    going = false;
                }
            } else {
#line 761 "src/parser.tr"
                going = false;
            }
        }
    }
#line 762 "src/parser.tr"
    return pl;
}

__attribute__((hot)) Block* Parser_parse_block(Parser* self) {
#line 767 "src/parser.tr"
    Block* b = Block_init();
#line 768 "src/parser.tr"
    bool is_inline = false;
#line 769 "src/parser.tr"
    __auto_type _t88 = Parser_peek(self);
    if (_t88.tag == Token_Indent) {
#line 771 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (_t88.tag == Token_Newline) {
#line 773 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 774 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
    } else if (1) {
        __auto_type _ = _t88;
#line 776 "src/parser.tr"
        is_inline = true;
    }
#line 778 "src/parser.tr"
    if (is_inline) {
#line 779 "src/parser.tr"
        Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 780 "src/parser.tr"
        Block_push(b, Parser_parse_stmt(self));
#line 781 "src/parser.tr"
        bool semi_going = true;
#line 782 "src/parser.tr"
        while (semi_going) {
#line 783 "src/parser.tr"
            semi_going = false;
#line 784 "src/parser.tr"
            __auto_type _t89 = Parser_peek(self);
            if (_t89.tag == Token_Semicolon) {
#line 787 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 788 "src/parser.tr"
                __auto_type _t90 = Parser_peek(self);
                if ((_t90.tag == Token_Newline || _t90.tag == Token_Eof || _t90.tag == Token_Dedent)) {
#line 790 "src/parser.tr"
                    /* pass */
                } else if (1) {
                    __auto_type _ = _t90;
#line 792 "src/parser.tr"
                    Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 793 "src/parser.tr"
                    Block_push(b, Parser_parse_stmt(self));
#line 794 "src/parser.tr"
                    semi_going = true;
                }
            } else if (1) {
                __auto_type _ = _t89;
#line 798 "src/parser.tr"
                if ((self->pos > 0LL)) {
#line 799 "src/parser.tr"
                    __auto_type _t91 = List_Token_get(self->tokens, (self->pos - 1LL));
                    if (_t91.tag == Token_Semicolon) {
#line 801 "src/parser.tr"
                        __auto_type _t92 = Parser_peek(self);
                        if ((_t92.tag == Token_Newline || _t92.tag == Token_Eof || _t92.tag == Token_Dedent)) {
#line 803 "src/parser.tr"
                            /* pass */
                        } else if (1) {
                            __auto_type _ = _t92;
#line 805 "src/parser.tr"
                            Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 806 "src/parser.tr"
                            Block_push(b, Parser_parse_stmt(self));
#line 807 "src/parser.tr"
                            semi_going = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t91;
#line 809 "src/parser.tr"
                        /* pass */
                    }
                }
            }
        }
#line 810 "src/parser.tr"
        return b;
    }
#line 812 "src/parser.tr"
    bool going = true;
#line 813 "src/parser.tr"
    while (going) {
#line 814 "src/parser.tr"
        __auto_type _t93 = Parser_peek(self);
        if ((_t93.tag == Token_Dedent || _t93.tag == Token_Eof)) {
#line 816 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t93;
#line 818 "src/parser.tr"
            /* pass */
        }
#line 819 "src/parser.tr"
        if (going) {
#line 820 "src/parser.tr"
            Parser_skip_newlines(self);
#line 821 "src/parser.tr"
            __auto_type _t94 = Parser_peek(self);
            if ((_t94.tag == Token_Dedent || _t94.tag == Token_Eof)) {
#line 823 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t94;
#line 825 "src/parser.tr"
                /* pass */
            }
#line 826 "src/parser.tr"
            if (going) {
#line 827 "src/parser.tr"
                long long _sline = Parser_cur_line(self);
#line 828 "src/parser.tr"
                __auto_type s = Parser_parse_stmt(self);
#line 829 "src/parser.tr"
                Block_push(b, box_stmt(Stmt_ctor_SLine(_sline)));
#line 830 "src/parser.tr"
                Block_push(b, s);
            }
        }
    }
#line 831 "src/parser.tr"
    __auto_type _t95 = Parser_peek(self);
    if (_t95.tag == Token_Dedent) {
#line 833 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t95;
#line 835 "src/parser.tr"
        /* pass */
    }
#line 836 "src/parser.tr"
    return b;
}

__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self) {
#line 841 "src/parser.tr"
    __auto_type _t96 = Parser_peek(self);
    if (_t96.tag == Token_KwReturn) {
#line 843 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 844 "src/parser.tr"
        __auto_type _t97 = Parser_peek(self);
        if ((_t97.tag == Token_Newline || _t97.tag == Token_Dedent || _t97.tag == Token_Eof)) {
#line 846 "src/parser.tr"
            return box_stmt(Stmt_ctor_SReturn((Expr*)(0LL)));
        } else if (1) {
            __auto_type _ = _t97;
#line 848 "src/parser.tr"
            /* pass */
        }
#line 849 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 850 "src/parser.tr"
        Parser_expect_newline(self);
#line 851 "src/parser.tr"
        return box_stmt(Stmt_ctor_SReturn(e));
    } else if (_t96.tag == Token_KwPass) {
#line 853 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 854 "src/parser.tr"
        Parser_expect_newline(self);
#line 855 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t96.tag == Token_KwBreak) {
#line 857 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 860 "src/parser.tr"
        Expr* brk_val = (Expr*)(0LL);
#line 861 "src/parser.tr"
        __auto_type _t98 = Parser_peek(self);
        if (_t98.tag == Token_Newline) {
            /* pass */
        } else if (_t98.tag == Token_Dedent) {
            /* pass */
        } else if (_t98.tag == Token_Eof) {
            /* pass */
        } else if (_t98.tag == Token_Semicolon) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t98;
#line 867 "src/parser.tr"
            brk_val = Parser_parse_expr(self);
        }
#line 868 "src/parser.tr"
        Parser_expect_newline(self);
#line 869 "src/parser.tr"
        return box_stmt(Stmt_ctor_SBreak(brk_val));
    } else if (_t96.tag == Token_KwContinue) {
#line 871 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 872 "src/parser.tr"
        Parser_expect_newline(self);
#line 873 "src/parser.tr"
        return box_stmt(Stmt_make_SContinue());
    } else if (_t96.tag == Token_KwRaise) {
#line 875 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 876 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 877 "src/parser.tr"
        Parser_expect_newline(self);
#line 878 "src/parser.tr"
        return box_stmt(Stmt_ctor_SRaise(e));
    } else if (_t96.tag == Token_KwAsm) {
#line 880 "src/parser.tr"
        return Parser_parse_asm_stmt(self);
    } else if (_t96.tag == Token_KwSpawn) {
#line 882 "src/parser.tr"
        return Parser_parse_spawn_stmt(self);
    } else if (_t96.tag == Token_KwTaskGroup) {
#line 884 "src/parser.tr"
        return Parser_parse_taskgroup_stmt(self);
    } else if (_t96.tag == Token_KwGpu) {
#line 886 "src/parser.tr"
        return Parser_parse_gpu_stmt(self);
    } else if (_t96.tag == Token_KwIf) {
#line 888 "src/parser.tr"
        return Parser_parse_if_stmt(self);
    } else if (_t96.tag == Token_KwWhile) {
#line 890 "src/parser.tr"
        return Parser_parse_while_stmt(self);
    } else if (_t96.tag == Token_KwLoop) {
#line 892 "src/parser.tr"
        return Parser_parse_loop_stmt(self);
    } else if (_t96.tag == Token_KwFor) {
#line 894 "src/parser.tr"
        return Parser_parse_for_stmt(self);
    } else if (_t96.tag == Token_KwMatch) {
#line 896 "src/parser.tr"
        return Parser_parse_match_stmt(self);
    } else if (_t96.tag == Token_KwMut) {
#line 898 "src/parser.tr"
        return Parser_parse_let_stmt(self, true);
    } else if (_t96.tag == Token_KwShared) {
#line 900 "src/parser.tr"
        return Parser_parse_shared_let_stmt(self);
    } else if (_t96.tag == Token_KwConst) {
#line 902 "src/parser.tr"
        return Parser_parse_const_let_stmt(self);
    } else if (_t96.tag == Token_KwUnsafe) {
#line 904 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 905 "src/parser.tr"
        __auto_type _t99 = Parser_peek(self);
        if (_t99.tag == Token_Colon) {
#line 907 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t99;
#line 909 "src/parser.tr"
            /* pass */
        }
#line 910 "src/parser.tr"
        __auto_type body = Parser_parse_block(self);
#line 911 "src/parser.tr"
        Stmt* _trret17 = box_stmt(Stmt_ctor_SUnsafe(body));
        return _trret17;
    } else if (_t96.tag == Token_KwDefer) {
#line 914 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 915 "src/parser.tr"
        Stmt* deferred = Parser_parse_stmt(self);
#line 916 "src/parser.tr"
        return box_stmt(Stmt_ctor_SDefer(deferred));
    } else if (_t96.tag == Token_KwWith) {
#line 918 "src/parser.tr"
        return Parser_parse_with_stmt(self);
    } else if (_t96.tag == Token_KwTry) {
#line 920 "src/parser.tr"
        return Parser_parse_try_stmt(self);
    } else if (_t96.tag == Token_KwAssert) {
#line 922 "src/parser.tr"
        return Parser_parse_assert_stmt(self);
    } else if (_t96.tag == Token_KwFrom) {
#line 925 "src/parser.tr"
        Parser_parse_from_import(self);
#line 926 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t96.tag == Token_KwImport) {
#line 928 "src/parser.tr"
        Parser_parse_import(self);
#line 929 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t96.tag == Token_KwClass) {
#line 935 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t96.tag == Token_KwEnum) {
#line 937 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t96.tag == Token_KwInterface) {
#line 939 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t96.tag == Token_KwExtend) {
#line 941 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t96.tag == Token_KwDef) {
#line 943 "src/parser.tr"
        long long _ldline = Parser_cur_line(self);
#line 944 "src/parser.tr"
        FunctionDef* _ldf = Parser_parse_function_def(self, false);
#line 945 "src/parser.tr"
        _ldf->line = _ldline;
#line 946 "src/parser.tr"
        Stmt* _trret18 = box_stmt(Stmt_ctor_SLocalDecl(box_decl(Decl_ctor_DFunction(_ldf))));
        _tr_obj_release(_ldf, _trdrop_FunctionDef);
        return _trret18;
    } else if (1) {
        __auto_type _ = _t96;
#line 948 "src/parser.tr"
        /* pass */
    }
#line 949 "src/parser.tr"
    __auto_type _t100 = Parser_peek(self);
    if (_t100.tag == Token_Ident) {
        __auto_type cs_nm = _t100.data.Ident.name;
#line 951 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("type", 4LL)))) {
#line 955 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 956 "src/parser.tr"
            TrStr _ta_name = Parser_consume_ident(self);
#line 957 "src/parser.tr"
            if (({ __auto_type _ord_t101 = (Parser_peek(self)); __auto_type _wr = ((_ord_t101.tag == Token_make_Eq().tag)); _wr; })) {
#line 958 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 959 "src/parser.tr"
                AstType* _ta_ty = Parser_parse_type(self);
#line 960 "src/parser.tr"
                Parser_expect_newline(self);
#line 961 "src/parser.tr"
                Stmt* _trret19 = box_stmt(Stmt_make_SPass());
                _tr_str_release(_ta_name);
                return _trret19;
            }
        }
#line 962 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("chan_select", 11LL)))) {
#line 963 "src/parser.tr"
            Stmt* _trret20 = Parser_parse_chan_select_stmt(self);
            return _trret20;
        }
#line 964 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("var", 3LL)))) {
#line 971 "src/parser.tr"
            long long _var_save = self->pos;
#line 972 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 973 "src/parser.tr"
            __auto_type _t102 = Parser_peek(self);
            if (_t102.tag == Token_Ident) {
                __auto_type _ = _t102.data.Ident.name;
#line 975 "src/parser.tr"
                Stmt* _trret21 = Parser_parse_assign_or_expr_stmt(self);
                return _trret21;
            } else if (1) {
                __auto_type _ = _t102;
#line 977 "src/parser.tr"
                self->pos = _var_save;
            }
        }
    } else if (1) {
        __auto_type _ = _t100;
        /* pass */
    }
#line 979 "src/parser.tr"
    return Parser_parse_assign_or_expr_stmt(self);
}

__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self) {
#line 982 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 983 "src/parser.tr"
    __auto_type _t103 = Parser_peek(self);
    if (_t103.tag == Token_Colon) {
#line 985 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t103;
#line 987 "src/parser.tr"
        /* pass */
    }
#line 988 "src/parser.tr"
    __auto_type try_body = Parser_parse_block(self);
#line 989 "src/parser.tr"
    List_ptr* catches = (void*)List_ptr_new();
#line 990 "src/parser.tr"
    while (({ __auto_type _ord_t104 = (Parser_peek(self)); __auto_type _wr = ((_ord_t104.tag == Token_make_KwExcept().tag)); _wr; })) {
#line 991 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 992 "src/parser.tr"
        AstType** err_type = (AstType**)(0LL);
#line 993 "src/parser.tr"
        TrStr err_name = _tr_str_lit_len("", 0LL);
#line 994 "src/parser.tr"
        if (({ __auto_type _ord_t105 = (Parser_peek(self)); __auto_type _wr = ((_ord_t105.tag != Token_make_Colon().tag)); _wr; })) {
#line 996 "src/parser.tr"
            bool is_bare_var = false;
#line 997 "src/parser.tr"
            __auto_type _t106 = Parser_peek(self);
            if (_t106.tag == Token_Ident) {
                __auto_type _ = _t106.data.Ident.name;
#line 999 "src/parser.tr"
                long long save_pos = self->pos;
#line 1000 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1001 "src/parser.tr"
                if (({ __auto_type _ord_t107 = (Parser_peek(self)); __auto_type _wr = ((_ord_t107.tag == Token_make_Colon().tag)); _wr; })) {
#line 1002 "src/parser.tr"
                    is_bare_var = true;
                }
#line 1003 "src/parser.tr"
                self->pos = save_pos;
            } else if (1) {
                __auto_type _ = _t106;
#line 1005 "src/parser.tr"
                /* pass */
            }
#line 1006 "src/parser.tr"
            if (is_bare_var) {
#line 1007 "src/parser.tr"
                TrStr _strtmp_t108 = Parser_consume_ident(self);
                _tr_str_release(err_name);
                err_name = _strtmp_t108;
            } else {
#line 1009 "src/parser.tr"
                err_type = box_asttype(Parser_parse_type(self));
#line 1010 "src/parser.tr"
                if (({ __auto_type _ord_t109 = (Parser_peek(self)); __auto_type _wr = ((_ord_t109.tag == Token_make_KwAs().tag)); _wr; })) {
#line 1011 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1012 "src/parser.tr"
                    TrStr _strtmp_t110 = Parser_consume_ident(self);
                    _tr_str_release(err_name);
                    err_name = _strtmp_t110;
                }
            }
        }
#line 1013 "src/parser.tr"
        __auto_type _t111 = Parser_peek(self);
        if (_t111.tag == Token_Colon) {
#line 1015 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t111;
#line 1017 "src/parser.tr"
            /* pass */
        }
#line 1018 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 1019 "src/parser.tr"
        CatchClause* c = CatchClause_init(err_name, (Block**)(0LL));
#line 1020 "src/parser.tr"
        /* unsafe block */
#line 1021 "src/parser.tr"
        Block** body_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1022 "src/parser.tr"
        (*body_ptr = body);
#line 1023 "src/parser.tr"
        c->body = body_ptr;
#line 1024 "src/parser.tr"
        c->err_type = err_type;
#line 1025 "src/parser.tr"
        /* unsafe block */
#line 1026 "src/parser.tr"
        CatchClause** c_ptr = ((CatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(CatchClause*)));
#line 1027 "src/parser.tr"
        (*c_ptr = c);
#line 1028 "src/parser.tr"
        List_ptr_append(catches, c_ptr);
        _tr_str_release(err_name);
    }
#line 1030 "src/parser.tr"
    Block* finally_b = Block_init();
#line 1031 "src/parser.tr"
    if (({ __auto_type _ord_t112 = (Parser_peek(self)); __auto_type _wr = ((_ord_t112.tag == Token_make_KwFinally().tag)); _wr; })) {
#line 1032 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1033 "src/parser.tr"
        __auto_type _t113 = Parser_peek(self);
        if (_t113.tag == Token_Colon) {
#line 1035 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t113;
#line 1037 "src/parser.tr"
            /* pass */
        }
#line 1038 "src/parser.tr"
        finally_b = Parser_parse_block(self);
    } else if ((catches->len == 0LL)) {
#line 1040 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("'try' block has no 'except' or 'finally' clause", 47LL), _tr_str_lit_len("add 'except:' (or 'except Type as e:') after the try block, or a 'finally:' block.", 82LL));
    }
#line 1041 "src/parser.tr"
    Stmt* _trret22 = box_stmt(Stmt_ctor_STry(try_body, catches, finally_b));
    return _trret22;
}

__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self) {
#line 1044 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1045 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1046 "src/parser.tr"
    Expr* msg = (Expr*)(0LL);
#line 1047 "src/parser.tr"
    if (({ __auto_type _ord_t114 = (Parser_peek(self)); __auto_type _wr = ((_ord_t114.tag == Token_make_Comma().tag)); _wr; })) {
#line 1048 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1049 "src/parser.tr"
        msg = Parser_parse_expr(self);
    }
#line 1050 "src/parser.tr"
    Parser_expect_newline(self);
#line 1051 "src/parser.tr"
    return box_stmt(Stmt_ctor_SAssert(cond, msg));
}

__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self) {
#line 1054 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1055 "src/parser.tr"
    List_ptr* items = (void*)List_ptr_new();
#line 1056 "src/parser.tr"
    List_TrStr* aliases = (void*)List_TrStr_new();
#line 1057 "src/parser.tr"
    bool going = true;
#line 1058 "src/parser.tr"
    while (going) {
#line 1059 "src/parser.tr"
        Expr* item_e = Parser_parse_expr(self);
#line 1060 "src/parser.tr"
        TrStr alias = _tr_str_lit_len("", 0LL);
#line 1062 "src/parser.tr"
        if ((((unsigned long long)(item_e)) != ((unsigned long long)(0LL)))) {
#line 1063 "src/parser.tr"
            __auto_type _t115 = (*item_e);
            if (_t115.tag == Expr_ECast) {
                __auto_type cast_inner = _t115.data.ECast.expr;
__auto_type cast_ty_ptr = _t115.data.ECast.ty;
#line 1065 "src/parser.tr"
                if ((((unsigned long long)(cast_ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 1066 "src/parser.tr"
                    TrStr _strtmp_t116 = _tr_str_retain((*cast_ty_ptr)->name);
                    _tr_str_release(alias);
                    alias = _strtmp_t116;
                }
#line 1067 "src/parser.tr"
                item_e = cast_inner;
            } else if (1) {
                __auto_type _ = _t115;
                /* pass */
            }
        }
#line 1069 "src/parser.tr"
        List_ptr_append(items, item_e);
#line 1070 "src/parser.tr"
        List_TrStr_append(aliases, alias);
#line 1071 "src/parser.tr"
        if (({ __auto_type _ord_t117 = (Parser_peek(self)); __auto_type _wr = ((_ord_t117.tag == Token_make_Comma().tag)); _wr; })) {
#line 1072 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 1074 "src/parser.tr"
            going = false;
        }
        _tr_str_release(alias);
    }
#line 1075 "src/parser.tr"
    __auto_type _t118 = Parser_peek(self);
    if (_t118.tag == Token_Colon) {
#line 1077 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t118;
#line 1079 "src/parser.tr"
        /* pass */
    }
#line 1080 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1081 "src/parser.tr"
    Stmt* _trret23 = box_stmt(Stmt_ctor_SWith(items, aliases, body));
    return _trret23;
}

__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self) {
#line 1084 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1085 "src/parser.tr"
    __auto_type _t119 = Parser_peek(self);
    if (_t119.tag == Token_LParen) {
#line 1087 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t119;
#line 1089 "src/parser.tr"
        /* pass */
    }
#line 1090 "src/parser.tr"
    List_TrStr* parts = (void*)List_TrStr_new();
#line 1092 "src/parser.tr"
    bool reading = true;
#line 1093 "src/parser.tr"
    while (reading) {
#line 1094 "src/parser.tr"
        __auto_type _t120 = Parser_peek(self);
        if (_t120.tag == Token_StrLit) {
            __auto_type s = _t120.data.StrLit.val;
#line 1096 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1097 "src/parser.tr"
            List_TrStr_append(parts, s);
#line 1098 "src/parser.tr"
            __auto_type _t121 = Parser_peek(self);
            if (_t121.tag == Token_Comma) {
#line 1100 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t121;
                reading = false;
            }
        } else if (1) {
            __auto_type _ = _t120;
            reading = false;
        }
    }
#line 1103 "src/parser.tr"
    __auto_type _t122 = Parser_peek(self);
    if (_t122.tag == Token_RParen) {
#line 1105 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t122;
#line 1107 "src/parser.tr"
        /* pass */
    }
#line 1108 "src/parser.tr"
    Parser_expect_newline(self);
#line 1109 "src/parser.tr"
    TrStr code = _tr_str_lit_len("", 0LL);
#line 1110 "src/parser.tr"
    TrStr outs = _tr_str_lit_len("", 0LL);
#line 1111 "src/parser.tr"
    TrStr ins = _tr_str_lit_len("", 0LL);
#line 1112 "src/parser.tr"
    TrStr clob = _tr_str_lit_len("", 0LL);
#line 1113 "src/parser.tr"
    if ((parts->len > 0LL)) {
#line 1113 "src/parser.tr"
        TrStr _strtmp_t123 = List_TrStr_get(parts, 0LL);
        _tr_str_release(code);
        code = _strtmp_t123;
    }
#line 1114 "src/parser.tr"
    if ((parts->len > 1LL)) {
#line 1114 "src/parser.tr"
        TrStr _strtmp_t124 = List_TrStr_get(parts, 1LL);
        _tr_str_release(outs);
        outs = _strtmp_t124;
    }
#line 1115 "src/parser.tr"
    if ((parts->len > 2LL)) {
#line 1115 "src/parser.tr"
        TrStr _strtmp_t125 = List_TrStr_get(parts, 2LL);
        _tr_str_release(ins);
        ins = _strtmp_t125;
    }
#line 1116 "src/parser.tr"
    if ((parts->len > 3LL)) {
#line 1116 "src/parser.tr"
        TrStr _strtmp_t126 = List_TrStr_get(parts, 3LL);
        _tr_str_release(clob);
        clob = _strtmp_t126;
    }
#line 1117 "src/parser.tr"
    Stmt* _trret24 = box_stmt(Stmt_ctor_SAsm(code, outs, ins, clob));
    List_TrStr_free(parts);
    _tr_str_release(code);
    _tr_str_release(outs);
    _tr_str_release(ins);
    _tr_str_release(clob);
    return _trret24;
}

__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self) {
#line 1120 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1121 "src/parser.tr"
    __auto_type e = Parser_parse_expr(self);
#line 1122 "src/parser.tr"
    Parser_expect_newline(self);
#line 1123 "src/parser.tr"
    return box_stmt(Stmt_ctor_SSpawn(e));
}

__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self) {
#line 1126 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1127 "src/parser.tr"
    __auto_type _t127 = Parser_peek(self);
    if (_t127.tag == Token_Colon) {
#line 1129 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t127;
#line 1131 "src/parser.tr"
        /* pass */
    }
#line 1132 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1133 "src/parser.tr"
    Stmt* _trret25 = box_stmt(Stmt_ctor_STaskGroup(body));
    return _trret25;
}

__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self) {
#line 1141 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1142 "src/parser.tr"
    if (({ __auto_type _ord_t128 = (Parser_peek(self)); __auto_type _wr = ((_ord_t128.tag == Token_make_Colon().tag)); _wr; })) {
#line 1142 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1143 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1144 "src/parser.tr"
    if (({ __auto_type _ord_t129 = (Parser_peek(self)); __auto_type _wr = ((_ord_t129.tag == Token_make_Indent().tag)); _wr; })) {
#line 1144 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1145 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1146 "src/parser.tr"
    List_ptr* cases = (void*)List_ptr_new();
#line 1147 "src/parser.tr"
    bool going = true;
#line 1148 "src/parser.tr"
    while (going) {
#line 1149 "src/parser.tr"
        __auto_type _t130 = Parser_peek(self);
        if (_t130.tag == Token_KwCase) {
#line 1151 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1153 "src/parser.tr"
            long long arm_kind = (-1LL);
#line 1154 "src/parser.tr"
            TrStr var_nm = _tr_str_lit_len("", 0LL);
#line 1155 "src/parser.tr"
            Expr* chan_e = (Expr*)(0LL);
#line 1156 "src/parser.tr"
            Expr* val_e = (Expr*)(0LL);
#line 1157 "src/parser.tr"
            Expr* timeout_e = (Expr*)(0LL);
#line 1159 "src/parser.tr"
            __auto_type _t131 = Parser_peek(self);
            if (_t131.tag == Token_Ident) {
                __auto_type arm_nm = _t131.data.Ident.name;
#line 1161 "src/parser.tr"
                if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("default", 7LL)))) {
#line 1162 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1163 "src/parser.tr"
                    if (({ __auto_type _ord_t132 = (Parser_peek(self)); __auto_type _wr = ((_ord_t132.tag == Token_make_Colon().tag)); _wr; })) {
#line 1163 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1164 "src/parser.tr"
                    arm_kind = 3LL;
                } else if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("timeout", 7LL)))) {
#line 1167 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1168 "src/parser.tr"
                    if (({ __auto_type _ord_t133 = (Parser_peek(self)); __auto_type _wr = ((_ord_t133.tag == Token_make_LParen().tag)); _wr; })) {
#line 1168 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1169 "src/parser.tr"
                    timeout_e = Parser_parse_expr(self);
#line 1170 "src/parser.tr"
                    if (({ __auto_type _ord_t134 = (Parser_peek(self)); __auto_type _wr = ((_ord_t134.tag == Token_make_RParen().tag)); _wr; })) {
#line 1170 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1171 "src/parser.tr"
                    if (({ __auto_type _ord_t135 = (Parser_peek(self)); __auto_type _wr = ((_ord_t135.tag == Token_make_Colon().tag)); _wr; })) {
#line 1171 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1172 "src/parser.tr"
                    arm_kind = 2LL;
                } else {
#line 1176 "src/parser.tr"
                    long long saved = self->pos;
#line 1177 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1178 "src/parser.tr"
                    __auto_type _t136 = Parser_peek(self);
                    if (_t136.tag == Token_Eq) {
#line 1181 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1182 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1183 "src/parser.tr"
                        if (({ __auto_type _ord_t137 = (Parser_peek(self)); __auto_type _wr = ((_ord_t137.tag == Token_make_Colon().tag)); _wr; })) {
#line 1183 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1184 "src/parser.tr"
                        TrStr _strtmp_t138 = _tr_str_retain(arm_nm);
                        _tr_str_release(var_nm);
                        var_nm = _strtmp_t138;
#line 1185 "src/parser.tr"
                        arm_kind = 0LL;
                    } else if (1) {
                        __auto_type _ = _t136;
#line 1188 "src/parser.tr"
                        self->pos = saved;
#line 1189 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1190 "src/parser.tr"
                        if (({ __auto_type _ord_t139 = (Parser_peek(self)); __auto_type _wr = ((_ord_t139.tag == Token_make_Colon().tag)); _wr; })) {
#line 1190 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1191 "src/parser.tr"
                        arm_kind = 1LL;
                    }
                }
            } else if (1) {
                __auto_type _ = _t131;
#line 1194 "src/parser.tr"
                chan_e = Parser_parse_expr(self);
#line 1195 "src/parser.tr"
                if (({ __auto_type _ord_t140 = (Parser_peek(self)); __auto_type _wr = ((_ord_t140.tag == Token_make_Colon().tag)); _wr; })) {
#line 1195 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1196 "src/parser.tr"
                arm_kind = 1LL;
            }
#line 1197 "src/parser.tr"
            Block* arm_body = Parser_parse_block(self);
#line 1198 "src/parser.tr"
            /* unsafe block */
#line 1199 "src/parser.tr"
            ChanSelectArm** arm_p = ((ChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(ChanSelectArm*)));
#line 1200 "src/parser.tr"
            if ((arm_kind == 0LL)) {
#line 1201 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_recv(chan_e, var_nm, arm_body));
            } else if ((arm_kind == 1LL)) {
#line 1203 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_send(chan_e, (Expr*)(0LL), arm_body));
            } else if ((arm_kind == 2LL)) {
#line 1205 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_timeout(timeout_e, arm_body));
            } else {
#line 1207 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_default(arm_body));
            }
#line 1208 "src/parser.tr"
            List_ptr_append(cases, arm_p);
            _tr_str_release(var_nm);
        } else if ((_t130.tag == Token_Dedent || _t130.tag == Token_Eof)) {
#line 1210 "src/parser.tr"
            going = false;
        } else if (_t130.tag == Token_Newline) {
#line 1212 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t130;
#line 1214 "src/parser.tr"
            going = false;
        }
    }
#line 1215 "src/parser.tr"
    if (({ __auto_type _ord_t141 = (Parser_peek(self)); __auto_type _wr = ((_ord_t141.tag == Token_make_Dedent().tag)); _wr; })) {
#line 1215 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1216 "src/parser.tr"
    return box_stmt(Stmt_ctor_SChanSelect(cases));
}

__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self) {
#line 1219 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1220 "src/parser.tr"
    __auto_type _t142 = Parser_peek(self);
    if (_t142.tag == Token_Colon) {
#line 1222 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t142;
#line 1224 "src/parser.tr"
        /* pass */
    }
#line 1225 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1226 "src/parser.tr"
    Stmt* _trret26 = box_stmt(Stmt_ctor_SGpuBlock(body));
    return _trret26;
}

__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut) {
#line 1229 "src/parser.tr"
    if (is_mut) {
#line 1230 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1232 "src/parser.tr"
    if (({ __auto_type _ord_t143 = (Parser_peek(self)); __auto_type _wr = ((_ord_t143.tag == Token_make_LParen().tag)); _wr; })) {
#line 1233 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1234 "src/parser.tr"
        List_TrStr* pnames = (void*)List_TrStr_new();
#line 1235 "src/parser.tr"
        ({ TrStr _at_t144 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t144); _tr_str_release(_at_t144); });
#line 1236 "src/parser.tr"
        while (({ __auto_type _ord_t145 = (Parser_peek(self)); __auto_type _wr = ((_ord_t145.tag == Token_make_Comma().tag)); _wr; })) {
#line 1237 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1238 "src/parser.tr"
            if (({ __auto_type _ord_t146 = (Parser_peek(self)); __auto_type _wr = ((_ord_t146.tag == Token_make_RParen().tag)); _wr; })) {
#line 1238 "src/parser.tr"
                break;
            }
#line 1239 "src/parser.tr"
            ({ TrStr _at_t147 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t147); _tr_str_release(_at_t147); });
        }
#line 1240 "src/parser.tr"
        if (({ __auto_type _ord_t148 = (Parser_peek(self)); __auto_type _wr = ((_ord_t148.tag == Token_make_RParen().tag)); _wr; })) {
#line 1240 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1241 "src/parser.tr"
        if (({ __auto_type _ord_t149 = (Parser_peek(self)); __auto_type _wr = ((_ord_t149.tag == Token_make_Eq().tag)); _wr; })) {
#line 1241 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1242 "src/parser.tr"
        Expr* pval = Parser_parse_expr(self);
#line 1243 "src/parser.tr"
        Parser_expect_newline(self);
#line 1244 "src/parser.tr"
        return box_stmt(Stmt_ctor_SMultiLet(pnames, is_mut, pval));
    }
#line 1245 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1247 "src/parser.tr"
    if (({ __auto_type _ord_t150 = (Parser_peek(self)); __auto_type _wr = ((_ord_t150.tag == Token_make_Comma().tag)); _wr; })) {
#line 1248 "src/parser.tr"
        List_TrStr* names = (void*)List_TrStr_new();
#line 1249 "src/parser.tr"
        List_TrStr_append(names, name);
#line 1250 "src/parser.tr"
        while (({ __auto_type _ord_t151 = (Parser_peek(self)); __auto_type _wr = ((_ord_t151.tag == Token_make_Comma().tag)); _wr; })) {
#line 1251 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1252 "src/parser.tr"
            ({ TrStr _at_t152 = (Parser_consume_ident(self)); List_TrStr_append(names, _at_t152); _tr_str_release(_at_t152); });
        }
#line 1253 "src/parser.tr"
        __auto_type _t153 = Parser_peek(self);
        if (_t153.tag == Token_Eq) {
#line 1255 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t153;
#line 1257 "src/parser.tr"
            /* pass */
        }
#line 1258 "src/parser.tr"
        Expr* val_ptr = Parser_parse_expr(self);
#line 1259 "src/parser.tr"
        Parser_expect_newline(self);
#line 1260 "src/parser.tr"
        Stmt* _trret27 = box_stmt(Stmt_ctor_SMultiLet(names, is_mut, val_ptr));
        _tr_str_release(name);
        return _trret27;
    }
#line 1261 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1262 "src/parser.tr"
    __auto_type _t154 = Parser_peek(self);
    if (_t154.tag == Token_Colon) {
#line 1264 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1265 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1266 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t154;
#line 1268 "src/parser.tr"
        /* pass */
    }
#line 1269 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1270 "src/parser.tr"
    __auto_type _t155 = Parser_peek(self);
    if (_t155.tag == Token_Eq) {
#line 1272 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1273 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t155;
#line 1275 "src/parser.tr"
        /* pass */
    }
#line 1276 "src/parser.tr"
    if (((((unsigned long long)(ty_ptr)) == ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))))) {
#line 1277 "src/parser.tr"
        ({ TrStr _at_t156 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("variable '", 10LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has neither a type nor an initial value", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t157 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("write 'mut ", 11LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Type' or 'mut ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = value'.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_prev(self, _at_t156, _at_t157); _tr_str_release(_at_t156); _tr_str_release(_at_t157); });
    }
#line 1278 "src/parser.tr"
    Parser_expect_newline(self);
#line 1279 "src/parser.tr"
    Stmt* _trret28 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), is_mut, false, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret28;
}

__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self) {
#line 1282 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1283 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1284 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1285 "src/parser.tr"
    __auto_type _t158 = Parser_peek(self);
    if (_t158.tag == Token_Colon) {
#line 1287 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1288 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1289 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t158;
#line 1291 "src/parser.tr"
        /* pass */
    }
#line 1292 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1293 "src/parser.tr"
    __auto_type _t159 = Parser_peek(self);
    if (_t159.tag == Token_Eq) {
#line 1295 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1296 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t159;
#line 1298 "src/parser.tr"
        /* pass */
    }
#line 1299 "src/parser.tr"
    Parser_expect_newline(self);
#line 1300 "src/parser.tr"
    Stmt* _trret29 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Shared(), true, false, true, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret29;
}

__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self) {
#line 1303 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1304 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1305 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1306 "src/parser.tr"
    __auto_type _t160 = Parser_peek(self);
    if (_t160.tag == Token_Colon) {
#line 1308 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1309 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1310 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t160;
#line 1312 "src/parser.tr"
        /* pass */
    }
#line 1313 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1314 "src/parser.tr"
    __auto_type _t161 = Parser_peek(self);
    if (_t161.tag == Token_Eq) {
#line 1316 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1317 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t161;
#line 1319 "src/parser.tr"
        /* pass */
    }
#line 1320 "src/parser.tr"
    Parser_expect_newline(self);
#line 1321 "src/parser.tr"
    Stmt* _trret30 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, true, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret30;
}

__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self) {
#line 1324 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1325 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1326 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1327 "src/parser.tr"
    __auto_type _t162 = Parser_peek(self);
    if (_t162.tag == Token_Colon) {
#line 1329 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t162;
#line 1331 "src/parser.tr"
        /* pass */
    }
#line 1332 "src/parser.tr"
    __auto_type then_b = Parser_parse_block(self);
#line 1333 "src/parser.tr"
    List_ptr* elifs = (void*)List_ptr_new();
#line 1334 "src/parser.tr"
    Block* else_b = Block_init();
#line 1335 "src/parser.tr"
    bool going = true;
#line 1336 "src/parser.tr"
    while (going) {
#line 1337 "src/parser.tr"
        going = false;
#line 1338 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1339 "src/parser.tr"
        __auto_type _t163 = Parser_peek(self);
        if (_t163.tag == Token_KwElif) {
#line 1341 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1342 "src/parser.tr"
            __auto_type ec = Parser_parse_expr(self);
#line 1343 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1344 "src/parser.tr"
            __auto_type _t164 = Parser_peek(self);
            if (_t164.tag == Token_Colon) {
#line 1346 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t164;
#line 1348 "src/parser.tr"
                /* pass */
            }
#line 1349 "src/parser.tr"
            __auto_type eb = Parser_parse_block(self);
#line 1350 "src/parser.tr"
            /* unsafe block */
#line 1351 "src/parser.tr"
            Block** eb_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1352 "src/parser.tr"
            (*eb_ptr = eb);
#line 1353 "src/parser.tr"
            List_ptr_append(elifs, ElifClause_init(ec, eb_ptr));
#line 1354 "src/parser.tr"
            going = true;
        } else if (_t163.tag == Token_KwElse) {
#line 1356 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1357 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1358 "src/parser.tr"
            __auto_type _t165 = Parser_peek(self);
            if (_t165.tag == Token_Colon) {
#line 1360 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t165;
#line 1362 "src/parser.tr"
                /* pass */
            }
#line 1363 "src/parser.tr"
            else_b = Parser_parse_block(self);
        } else if (1) {
            __auto_type _ = _t163;
#line 1365 "src/parser.tr"
            /* pass */
        }
    }
#line 1366 "src/parser.tr"
    Stmt* _trret31 = box_stmt(Stmt_ctor_SIf(cond, then_b, elifs, else_b));
    return _trret31;
}

__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self) {
#line 1369 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1370 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1371 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1372 "src/parser.tr"
    __auto_type _t166 = Parser_peek(self);
    if (_t166.tag == Token_Colon) {
#line 1374 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t166;
#line 1376 "src/parser.tr"
        /* pass */
    }
#line 1377 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1378 "src/parser.tr"
    Stmt* _trret32 = box_stmt(Stmt_ctor_SWhile(cond, body, (void*)List_ptr_new()));
    return _trret32;
}

__attribute__((hot)) Stmt* Parser_parse_loop_stmt(Parser* self) {
#line 1382 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1383 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1384 "src/parser.tr"
    if (({ __auto_type _ord_t167 = (Parser_peek(self)); __auto_type _wr = ((_ord_t167.tag == Token_make_Colon().tag)); _wr; })) {
#line 1384 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1385 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1386 "src/parser.tr"
    Stmt* _trret33 = ({ __auto_type _ord_t168 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t169 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SWhile(_ord_t168, body, _ord_t169))); _wr; });
    return _trret33;
}

__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self) {
#line 1389 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1392 "src/parser.tr"
    bool for_is_ref = false;
#line 1393 "src/parser.tr"
    if (({ __auto_type _ord_t170 = (Parser_peek(self)); __auto_type _wr = ((_ord_t170.tag == Token_make_KwMut().tag)); _wr; })) {
#line 1394 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1395 "src/parser.tr"
        TrStr _mr = Parser_consume_ident(self);
#line 1396 "src/parser.tr"
        for_is_ref = true;
    }
#line 1397 "src/parser.tr"
    TrStr var = Parser_consume_ident(self);
#line 1398 "src/parser.tr"
    if (((!for_is_ref) && _tr_str_eqv((var), (_tr_str_lit_len("ref", 3LL))))) {
#line 1399 "src/parser.tr"
        __auto_type _t171 = Parser_peek(self);
        if (_t171.tag == Token_KwIn) {
            /* pass */
        } else if (_t171.tag == Token_Comma) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t171;
#line 1403 "src/parser.tr"
            for_is_ref = true;
#line 1404 "src/parser.tr"
            TrStr _strtmp_t172 = Parser_consume_ident(self);
            _tr_str_release(var);
            var = _strtmp_t172;
        }
    }
#line 1406 "src/parser.tr"
    if (({ __auto_type _ord_t173 = (Parser_peek(self)); __auto_type _wr = ((_ord_t173.tag == Token_make_Comma().tag)); _wr; })) {
#line 1407 "src/parser.tr"
        List_TrStr* unpack_vars = (void*)List_TrStr_new();
#line 1408 "src/parser.tr"
        List_TrStr_append(unpack_vars, var);
#line 1409 "src/parser.tr"
        while (({ __auto_type _ord_t174 = (Parser_peek(self)); __auto_type _wr = ((_ord_t174.tag == Token_make_Comma().tag)); _wr; })) {
#line 1410 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1411 "src/parser.tr"
            ({ TrStr _at_t175 = (Parser_consume_ident(self)); List_TrStr_append(unpack_vars, _at_t175); _tr_str_release(_at_t175); });
        }
#line 1412 "src/parser.tr"
        if (({ __auto_type _ord_t176 = (Parser_peek(self)); __auto_type _wr = ((_ord_t176.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1412 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1413 "src/parser.tr"
        Expr* fu_iter = Parser_parse_expr(self);
#line 1414 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1415 "src/parser.tr"
        if (({ __auto_type _ord_t177 = (Parser_peek(self)); __auto_type _wr = ((_ord_t177.tag == Token_make_Colon().tag)); _wr; })) {
#line 1415 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1416 "src/parser.tr"
        Block* fu_body = Parser_parse_block(self);
#line 1417 "src/parser.tr"
        Stmt* _trret34 = box_stmt(Stmt_ctor_SForUnpack(unpack_vars, fu_iter, fu_body));
        _tr_str_release(var);
        return _trret34;
    }
#line 1418 "src/parser.tr"
    __auto_type _t178 = Parser_peek(self);
    if (_t178.tag == Token_KwIn) {
#line 1420 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t178;
#line 1422 "src/parser.tr"
        /* pass */
    }
#line 1423 "src/parser.tr"
    __auto_type iter_e = Parser_parse_expr(self);
#line 1424 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1425 "src/parser.tr"
    __auto_type _t179 = Parser_peek(self);
    if (_t179.tag == Token_Colon) {
#line 1427 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t179;
#line 1429 "src/parser.tr"
        /* pass */
    }
#line 1430 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1431 "src/parser.tr"
    Stmt* _trret35 = box_stmt(Stmt_ctor_SFor(var, iter_e, body, (void*)List_ptr_new(), for_is_ref));
    _tr_str_release(var);
    return _trret35;
}

__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self) {
#line 1434 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1435 "src/parser.tr"
    __auto_type subj = Parser_parse_expr(self);
#line 1436 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1437 "src/parser.tr"
    __auto_type _t180 = Parser_peek(self);
    if (_t180.tag == Token_Colon) {
#line 1439 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t180;
#line 1441 "src/parser.tr"
        /* pass */
    }
#line 1442 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1443 "src/parser.tr"
    return box_stmt(Stmt_ctor_SMatch(subj, arms));
}

__attribute__((hot)) List_ptr* Parser_parse_match_arms(Parser* self) {
#line 1448 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1449 "src/parser.tr"
    __auto_type _t181 = Parser_peek(self);
    if (_t181.tag == Token_Indent) {
#line 1451 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t181;
#line 1453 "src/parser.tr"
        /* pass */
    }
#line 1454 "src/parser.tr"
    List_ptr* arms = (void*)List_ptr_new();
#line 1455 "src/parser.tr"
    bool going = true;
#line 1456 "src/parser.tr"
    while (going) {
#line 1457 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1458 "src/parser.tr"
        __auto_type _t182 = Parser_peek(self);
        if ((_t182.tag == Token_Dedent || _t182.tag == Token_Eof || _t182.tag == Token_RParen || _t182.tag == Token_RBracket || _t182.tag == Token_RBrace)) {
#line 1463 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t182;
#line 1465 "src/parser.tr"
            /* pass */
        }
#line 1466 "src/parser.tr"
        if (going) {
#line 1467 "src/parser.tr"
            Pattern pat = Parser_parse_pattern(self);
#line 1468 "src/parser.tr"
            if (({ __auto_type _ord_t183 = (Parser_peek(self)); __auto_type _wr = ((_ord_t183.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1469 "src/parser.tr"
                List_Pattern* or_pats = (void*)List_Pattern_new();
#line 1470 "src/parser.tr"
                List_Pattern_append(or_pats, pat);
#line 1471 "src/parser.tr"
                while (({ __auto_type _ord_t184 = (Parser_peek(self)); __auto_type _wr = ((_ord_t184.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1472 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1473 "src/parser.tr"
                    List_Pattern_append(or_pats, Parser_parse_pattern(self));
                }
#line 1474 "src/parser.tr"
                pat = Pattern_ctor_POr(or_pats);
            }
#line 1476 "src/parser.tr"
            Expr* guard_expr = (Expr*)(0LL);
#line 1477 "src/parser.tr"
            if (({ __auto_type _ord_t185 = (Parser_peek(self)); __auto_type _wr = ((_ord_t185.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1478 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1479 "src/parser.tr"
                guard_expr = Parser_parse_expr(self);
            }
#line 1480 "src/parser.tr"
            __auto_type _t186 = Parser_peek(self);
            if (_t186.tag == Token_Colon) {
#line 1482 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t186;
#line 1484 "src/parser.tr"
                /* pass */
            }
#line 1485 "src/parser.tr"
            Block* arm_block = Block_init();
#line 1486 "src/parser.tr"
            bool is_block_arm = false;
#line 1487 "src/parser.tr"
            __auto_type _t187 = Parser_peek(self);
            if ((_t187.tag == Token_Newline || _t187.tag == Token_Indent)) {
#line 1489 "src/parser.tr"
                arm_block = Parser_parse_block(self);
#line 1490 "src/parser.tr"
                is_block_arm = true;
            } else if (1) {
                __auto_type _ = _t187;
#line 1492 "src/parser.tr"
                /* pass */
            }
#line 1493 "src/parser.tr"
            if ((!is_block_arm)) {
#line 1494 "src/parser.tr"
                Block_push(arm_block, Parser_parse_stmt(self));
            }
#line 1495 "src/parser.tr"
            Block** ab_ptr = (Block**)(0LL);
#line 1496 "src/parser.tr"
            /* unsafe block */
#line 1497 "src/parser.tr"
            ab_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1498 "src/parser.tr"
            (*ab_ptr = arm_block);
#line 1499 "src/parser.tr"
            MatchArm* new_arm = MatchArm_init(pat, ab_ptr);
#line 1500 "src/parser.tr"
            new_arm->guard = guard_expr;
#line 1501 "src/parser.tr"
            List_ptr_append(arms, _tr_obj_retain(new_arm));
        }
    }
#line 1502 "src/parser.tr"
    __auto_type _t188 = Parser_peek(self);
    if (_t188.tag == Token_Dedent) {
#line 1504 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t188;
#line 1506 "src/parser.tr"
        /* pass */
    }
#line 1507 "src/parser.tr"
    return arms;
}

__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self) {
#line 1510 "src/parser.tr"
    __auto_type _t189 = Parser_peek(self);
    if (_t189.tag == Token_KwCase) {
#line 1512 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t189;
#line 1514 "src/parser.tr"
        /* pass */
    }
#line 1515 "src/parser.tr"
    __auto_type _t190 = Parser_peek(self);
    if (_t190.tag == Token_Ident) {
        __auto_type type_name = _t190.data.Ident.name;
#line 1517 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1518 "src/parser.tr"
        __auto_type _t191 = Parser_peek(self);
        if (_t191.tag == Token_Dot) {
#line 1520 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1521 "src/parser.tr"
            TrStr variant = Parser_consume_ident(self);
#line 1522 "src/parser.tr"
            __auto_type _t192 = Parser_peek(self);
            if (_t192.tag == Token_LParen) {
#line 1524 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1525 "src/parser.tr"
                List_TrStr* fields = (void*)List_TrStr_new();
#line 1526 "src/parser.tr"
                while ((({ __auto_type _ord_t193 = (Parser_peek(self)); __auto_type _wr = ((_ord_t193.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t194 = (Parser_peek(self)); __auto_type _wr = ((_ord_t194.tag != Token_make_Eof().tag)); _wr; }))) {
#line 1527 "src/parser.tr"
                    TrStr fname = Parser_consume_ident(self);
#line 1528 "src/parser.tr"
                    if (_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL)))) {
#line 1529 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 1531 "src/parser.tr"
                        List_TrStr_append(fields, fname);
                    }
#line 1532 "src/parser.tr"
                    if (({ __auto_type _ord_t195 = (Parser_peek(self)); __auto_type _wr = ((_ord_t195.tag == Token_make_Comma().tag)); _wr; })) {
#line 1533 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                    _tr_str_release(fname);
                }
#line 1534 "src/parser.tr"
                if (({ __auto_type _ord_t196 = (Parser_peek(self)); __auto_type _wr = ((_ord_t196.tag == Token_make_RParen().tag)); _wr; })) {
#line 1535 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1536 "src/parser.tr"
                if ((fields->len == 1LL)) {
#line 1537 "src/parser.tr"
                    return ({ TrStr _at_t197 = (List_TrStr_get(fields, 0LL)); __auto_type _wr = (Pattern_ctor_PVariantBind(type_name, variant, _at_t197)); _tr_str_release(_at_t197); _wr; });
                }
#line 1538 "src/parser.tr"
                return Pattern_ctor_PVariantBindMany(type_name, variant, fields);
            } else if (1) {
                __auto_type _ = _t192;
#line 1540 "src/parser.tr"
                /* pass */
            }
#line 1541 "src/parser.tr"
            return Pattern_ctor_PVariant(type_name, variant);
        } else if (1) {
            __auto_type _ = _t191;
#line 1543 "src/parser.tr"
            /* pass */
        }
#line 1544 "src/parser.tr"
        return Pattern_ctor_PBind(type_name);
    } else if (_t190.tag == Token_IntLit) {
        __auto_type v = _t190.data.IntLit.val;
#line 1546 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1547 "src/parser.tr"
        return Pattern_ctor_PLitInt(v);
    } else if (_t190.tag == Token_KwTrue) {
#line 1549 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1550 "src/parser.tr"
        return Pattern_ctor_PLitBool(true);
    } else if (_t190.tag == Token_KwFalse) {
#line 1552 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1553 "src/parser.tr"
        return Pattern_ctor_PLitBool(false);
    } else if (_t190.tag == Token_StrLit) {
        __auto_type s = _t190.data.StrLit.val;
__auto_type blen = _t190.data.StrLit.blen;
#line 1555 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1556 "src/parser.tr"
        return Pattern_ctor_PLitStr(s, blen);
    } else if (1) {
        __auto_type _ = _t190;
#line 1558 "src/parser.tr"
        /* pass */
    }
#line 1559 "src/parser.tr"
    return Pattern_make_PWild();
}

__attribute__((hot)) bool Parser__ast_simple(Parser* self, Expr* e) {
#line 1563 "src/parser.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1563 "src/parser.tr"
        return true;
    }
#line 1564 "src/parser.tr"
    __auto_type _t198 = (*e);
    if (_t198.tag == Expr_EIdent) {
        __auto_type _ = _t198.data.EIdent.name;
        return true;
    } else if (_t198.tag == Expr_ELitInt) {
        __auto_type _ = _t198.data.ELitInt.val;
        return true;
    } else if (_t198.tag == Expr_ELitStr) {
        return true;
    } else if (_t198.tag == Expr_ELitBool) {
        __auto_type _ = _t198.data.ELitBool.val;
        return true;
    } else if (_t198.tag == Expr_EPropAccess) {
        __auto_type o = _t198.data.EPropAccess.obj;
        return Parser__ast_simple(self, o);
    } else if (_t198.tag == Expr_EIndex) {
        __auto_type o = _t198.data.EIndex.obj;
__auto_type i = _t198.data.EIndex._tr_v_index;
        return (Parser__ast_simple(self, o) && Parser__ast_simple(self, i));
    } else if (_t198.tag == Expr_EBinOp) {
        __auto_type a = _t198.data.EBinOp.left;
__auto_type b = _t198.data.EBinOp.right;
        return (Parser__ast_simple(self, a) && Parser__ast_simple(self, b));
    } else if (_t198.tag == Expr_EUnaryOp) {
        __auto_type a = _t198.data.EUnaryOp.expr;
        return Parser__ast_simple(self, a);
    } else if (1) {
        __auto_type _ = _t198;
        return false;
    }
}

__attribute__((hot)) Stmt* Parser_mk_aug(Parser* self, Expr* lhs, TrStr op, Expr* rhs) {
#line 1580 "src/parser.tr"
    Stmt* plain = box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(op, lhs, rhs))));
#line 1581 "src/parser.tr"
    List_ptr* stmts = (void*)List_ptr_new();
#line 1582 "src/parser.tr"
    __auto_type _t199 = (*lhs);
    if (_t199.tag == Expr_EIndex) {
        __auto_type o = _t199.data.EIndex.obj;
__auto_type i = _t199.data.EIndex._tr_v_index;
#line 1584 "src/parser.tr"
        if ((Parser__ast_simple(self, o) && Parser__ast_simple(self, i))) {
#line 1584 "src/parser.tr"
            return plain;
        }
#line 1585 "src/parser.tr"
        Expr* o2 = o;
#line 1586 "src/parser.tr"
        Expr* i2 = i;
#line 1587 "src/parser.tr"
        if ((!Parser__ast_simple(self, o))) {
#line 1588 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1589 "src/parser.tr"
            TrStr on = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1590 "src/parser.tr"
            ({ __auto_type _ord_t200 = (Ownership_make_Own()); __auto_type _ord_t201 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(on, _ord_t200, true, false, false, _ord_t201, o))); });
#line 1591 "src/parser.tr"
            o2 = box_expr(Expr_ctor_EIdent(on));
            _tr_str_release(on);
        }
#line 1592 "src/parser.tr"
        if ((!Parser__ast_simple(self, i))) {
#line 1593 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1594 "src/parser.tr"
            TrStr inm = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1595 "src/parser.tr"
            ({ __auto_type _ord_t202 = (Ownership_make_Own()); __auto_type _ord_t203 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(inm, _ord_t202, true, false, false, _ord_t203, i))); });
#line 1596 "src/parser.tr"
            i2 = box_expr(Expr_ctor_EIdent(inm));
            _tr_str_release(inm);
        }
#line 1597 "src/parser.tr"
        Expr* t_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1598 "src/parser.tr"
        Expr* r_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1599 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node, box_expr(Expr_ctor_EBinOp(op, r_node, rhs)))));
    } else if (_t199.tag == Expr_EPropAccess) {
        __auto_type o = _t199.data.EPropAccess.obj;
__auto_type f = _t199.data.EPropAccess.prop;
#line 1601 "src/parser.tr"
        if (Parser__ast_simple(self, o)) {
#line 1601 "src/parser.tr"
            Stmt* _trret36 = plain;
            return _trret36;
        }
#line 1602 "src/parser.tr"
        self->aug_ctr = (self->aug_ctr + 1LL);
#line 1603 "src/parser.tr"
        TrStr pn = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1604 "src/parser.tr"
        ({ __auto_type _ord_t204 = (Ownership_make_Own()); __auto_type _ord_t205 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(pn, _ord_t204, true, false, false, _ord_t205, o))); });
#line 1605 "src/parser.tr"
        Expr* o3 = box_expr(Expr_ctor_EIdent(pn));
#line 1606 "src/parser.tr"
        Expr* t_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1607 "src/parser.tr"
        Expr* r_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1608 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node2, box_expr(Expr_ctor_EBinOp(op, r_node2, rhs)))));
        _tr_str_release(pn);
    } else if (1) {
        __auto_type _ = _t199;
        return plain;
    }
#line 1610 "src/parser.tr"
    Block* blk = Block_init();
#line 1611 "src/parser.tr"
    long long si = 0LL;
#line 1612 "src/parser.tr"
    while ((si < stmts->len)) {
#line 1613 "src/parser.tr"
        ({ __auto_type _ord_t206 = (((Stmt*)List_ptr_get(stmts, si))); List_ptr_append(blk->stmts, _ord_t206); });
#line 1614 "src/parser.tr"
        si = (si + 1LL);
    }
#line 1615 "src/parser.tr"
    Stmt* _trret37 = ({ __auto_type _ord_t207 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t208 = ((void*)List_ptr_new()); __auto_type _ord_t209 = (Block_init()); __auto_type _wr = (box_stmt(Stmt_ctor_SIf(_ord_t207, blk, _ord_t208, _ord_t209))); _wr; });
    return _trret37;
}

__attribute__((hot)) List_ptr* Parser_parse_comp_generators(Parser* self) {
#line 1619 "src/parser.tr"
    List_ptr* generators = (void*)List_ptr_new();
#line 1620 "src/parser.tr"
    while (({ __auto_type _ord_t210 = (Parser_peek(self)); __auto_type _wr = ((_ord_t210.tag == Token_make_KwFor().tag)); _wr; })) {
#line 1621 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1622 "src/parser.tr"
        TrStr target = Parser_consume_ident(self);
#line 1623 "src/parser.tr"
        __auto_type _t211 = Parser_peek(self);
        if (_t211.tag == Token_KwIn) {
#line 1625 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t211;
#line 1627 "src/parser.tr"
            /* pass */
        }
#line 1628 "src/parser.tr"
        Expr* iter = Parser_parse_or_expr(self);
#line 1629 "src/parser.tr"
        List_ptr* ifs = (void*)List_ptr_new();
#line 1630 "src/parser.tr"
        while (({ __auto_type _ord_t212 = (Parser_peek(self)); __auto_type _wr = ((_ord_t212.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1631 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1632 "src/parser.tr"
            List_ptr_append(ifs, Parser_parse_or_expr(self));
        }
#line 1633 "src/parser.tr"
        Comprehension* c = Comprehension_init(target, iter);
#line 1634 "src/parser.tr"
        c->ifs = ifs;
#line 1635 "src/parser.tr"
        /* unsafe block */
#line 1636 "src/parser.tr"
        Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 1637 "src/parser.tr"
        (*c_ptr = c);
#line 1638 "src/parser.tr"
        List_ptr_append(generators, c_ptr);
        _tr_str_release(target);
    }
#line 1639 "src/parser.tr"
    return generators;
}

__attribute__((hot)) Expr* Parser_desugar_collection_comp(Parser* self, Expr* key, Expr* val, List_ptr* gens) {
#line 1644 "src/parser.tr"
    TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->pos)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__comp", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1645 "src/parser.tr"
    Stmt* body_stmt = box_stmt(Stmt_make_SPass());
#line 1646 "src/parser.tr"
    Expr* empty = box_expr(Expr_ctor_ESet((void*)List_ptr_new()));
#line 1647 "src/parser.tr"
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 1648 "src/parser.tr"
        empty = ({ __auto_type _ord_t213 = ((void*)List_ptr_new()); __auto_type _ord_t214 = ((void*)List_ptr_new()); __auto_type _wr = (box_expr(Expr_ctor_EDict(_ord_t213, _ord_t214))); _wr; });
#line 1649 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SAssign(box_expr(Expr_ctor_EIndex(box_expr(Expr_ctor_EIdent(tmp)), key)), val));
    } else {
#line 1651 "src/parser.tr"
        List_ptr* margs = (void*)List_ptr_new();
#line 1652 "src/parser.tr"
        List_ptr_append(margs, key);
#line 1653 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EMethodCall(box_expr(Expr_ctor_EIdent(tmp)), _tr_str_lit_len("add", 3LL), margs))));
    }
#line 1654 "src/parser.tr"
    long long gi = (gens->len - 1LL);
#line 1655 "src/parser.tr"
    while ((gi >= 0LL)) {
#line 1656 "src/parser.tr"
        Comprehension* g = (*((Comprehension**)List_ptr_get(gens, gi)));
#line 1657 "src/parser.tr"
        Block* inner = Block_init();
#line 1658 "src/parser.tr"
        Block_push(inner, body_stmt);
#line 1659 "src/parser.tr"
        Stmt* cur = box_stmt(Stmt_make_SPass());
#line 1660 "src/parser.tr"
        long long ci = (g->ifs->len - 1LL);
#line 1661 "src/parser.tr"
        Block* guarded = inner;
#line 1662 "src/parser.tr"
        while ((ci >= 0LL)) {
#line 1663 "src/parser.tr"
            Block* outer = Block_init();
#line 1664 "src/parser.tr"
            ({ __auto_type _ord_t215 = (((Expr*)List_ptr_get(g->ifs, ci))); __auto_type _ord_t216 = ((void*)List_ptr_new()); __auto_type _ord_t217 = (Block_init()); Block_push(outer, box_stmt(Stmt_ctor_SIf(_ord_t215, guarded, _ord_t216, _ord_t217))); });
#line 1665 "src/parser.tr"
            guarded = outer;
#line 1666 "src/parser.tr"
            ci = (ci - 1LL);
        }
#line 1667 "src/parser.tr"
        cur = ({ __auto_type _ord_t218 = (g->target); __auto_type _ord_t219 = (g->iter); __auto_type _ord_t220 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SFor(_ord_t218, _ord_t219, guarded, _ord_t220, false))); _wr; });
#line 1668 "src/parser.tr"
        body_stmt = cur;
#line 1669 "src/parser.tr"
        gi = (gi - 1LL);
    }
#line 1670 "src/parser.tr"
    Block* blk = Block_init();
#line 1671 "src/parser.tr"
    ({ __auto_type _ord_t221 = (Ownership_make_Own()); __auto_type _ord_t222 = ((AstType**)(0LL)); Block_push(blk, box_stmt(Stmt_ctor_SLet(tmp, _ord_t221, true, false, false, _ord_t222, empty))); });
#line 1672 "src/parser.tr"
    Block_push(blk, body_stmt);
#line 1673 "src/parser.tr"
    Block_push(blk, box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EIdent(tmp)))));
#line 1674 "src/parser.tr"
    Expr* _trret38 = box_expr(Expr_ctor_EDo(blk));
    _tr_str_release(tmp);
    return _trret38;
}

__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self) {
#line 1677 "src/parser.tr"
    __auto_type lhs = Parser_parse_expr(self);
#line 1679 "src/parser.tr"
    if (({ __auto_type _ord_t223 = (Parser_peek(self)); __auto_type _wr = ((_ord_t223.tag == Token_make_Comma().tag)); _wr; })) {
#line 1680 "src/parser.tr"
        __auto_type _t224 = (*lhs);
        if (_t224.tag == Expr_EIdent) {
            __auto_type mn0 = _t224.data.EIdent.name;
#line 1682 "src/parser.tr"
            List_TrStr* mnames = (void*)List_TrStr_new();
#line 1683 "src/parser.tr"
            List_TrStr_append(mnames, mn0);
#line 1684 "src/parser.tr"
            while (({ __auto_type _ord_t225 = (Parser_peek(self)); __auto_type _wr = ((_ord_t225.tag == Token_make_Comma().tag)); _wr; })) {
#line 1685 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1686 "src/parser.tr"
                ({ TrStr _at_t226 = (Parser_consume_ident(self)); List_TrStr_append(mnames, _at_t226); _tr_str_release(_at_t226); });
            }
#line 1687 "src/parser.tr"
            if (({ __auto_type _ord_t227 = (Parser_peek(self)); __auto_type _wr = ((_ord_t227.tag == Token_make_Eq().tag)); _wr; })) {
#line 1688 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1689 "src/parser.tr"
                Expr* mrhs = Parser_parse_expr(self);
#line 1690 "src/parser.tr"
                if (({ __auto_type _ord_t228 = (Parser_peek(self)); __auto_type _wr = ((_ord_t228.tag == Token_make_Comma().tag)); _wr; })) {
#line 1691 "src/parser.tr"
                    List_ptr* mitems = (void*)List_ptr_new();
#line 1692 "src/parser.tr"
                    List_ptr_append(mitems, mrhs);
#line 1693 "src/parser.tr"
                    while (({ __auto_type _ord_t229 = (Parser_peek(self)); __auto_type _wr = ((_ord_t229.tag == Token_make_Comma().tag)); _wr; })) {
#line 1694 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1695 "src/parser.tr"
                        List_ptr_append(mitems, Parser_parse_expr(self));
                    }
#line 1696 "src/parser.tr"
                    mrhs = box_expr(Expr_ctor_ETuple(mitems));
                }
#line 1697 "src/parser.tr"
                Parser_expect_newline(self);
#line 1698 "src/parser.tr"
                Stmt* _trret39 = box_stmt(Stmt_ctor_SMultiLet(mnames, false, mrhs));
                return _trret39;
            }
        } else if (1) {
            __auto_type _ = _t224;
            /* pass */
        }
    }
#line 1700 "src/parser.tr"
    __auto_type _t230 = Parser_peek(self);
    if (_t230.tag == Token_Colon) {
#line 1703 "src/parser.tr"
        __auto_type _t231 = (*lhs);
        if (_t231.tag == Expr_EIdent) {
            __auto_type name = _t231.data.EIdent.name;
#line 1705 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1706 "src/parser.tr"
            AstType* ty = Parser_parse_type(self);
#line 1707 "src/parser.tr"
            Expr* val = (Expr*)(0LL);
#line 1708 "src/parser.tr"
            if (({ __auto_type _ord_t232 = (Parser_peek(self)); __auto_type _wr = ((_ord_t232.tag == Token_make_Eq().tag)); _wr; })) {
#line 1709 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1710 "src/parser.tr"
                val = Parser_parse_expr(self);
            }
#line 1711 "src/parser.tr"
            Parser_expect_newline(self);
#line 1712 "src/parser.tr"
            Stmt* _trret40 = ({ __auto_type _ord_t233 = (Ownership_make_Own()); __auto_type _ord_t234 = (box_asttype(ty)); __auto_type _wr = (box_stmt(Stmt_ctor_SLet(name, _ord_t233, false, false, false, _ord_t234, val))); _wr; });
            return _trret40;
        } else if (1) {
            __auto_type _ = _t231;
            /* pass */
        }
    } else if (_t230.tag == Token_Eq) {
#line 1715 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1716 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1717 "src/parser.tr"
        Parser_expect_newline(self);
#line 1718 "src/parser.tr"
        return box_stmt(Stmt_ctor_SAssign(lhs, rhs));
    } else if (_t230.tag == Token_PlusEq) {
#line 1720 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1721 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1722 "src/parser.tr"
        Parser_expect_newline(self);
#line 1723 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("+", 1LL), rhs);
    } else if (_t230.tag == Token_MinusEq) {
#line 1725 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1726 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1727 "src/parser.tr"
        Parser_expect_newline(self);
#line 1728 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("-", 1LL), rhs);
    } else if (_t230.tag == Token_StarEq) {
#line 1730 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1731 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1732 "src/parser.tr"
        Parser_expect_newline(self);
#line 1733 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("*", 1LL), rhs);
    } else if (_t230.tag == Token_SlashEq) {
#line 1735 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1736 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1737 "src/parser.tr"
        Parser_expect_newline(self);
#line 1738 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("/", 1LL), rhs);
    } else if (_t230.tag == Token_PercentEq) {
#line 1740 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1741 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1742 "src/parser.tr"
        Parser_expect_newline(self);
#line 1743 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("%", 1LL), rhs);
    } else if (_t230.tag == Token_FloorDivEq) {
#line 1745 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1746 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1747 "src/parser.tr"
        Parser_expect_newline(self);
#line 1748 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("//", 2LL), rhs);
    } else if (_t230.tag == Token_StarStarEq) {
#line 1750 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1751 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1752 "src/parser.tr"
        Parser_expect_newline(self);
#line 1753 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("**", 2LL), rhs);
    } else if (_t230.tag == Token_AmpEq) {
#line 1755 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1756 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1757 "src/parser.tr"
        Parser_expect_newline(self);
#line 1758 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("&", 1LL), rhs);
    } else if (_t230.tag == Token_PipeEq) {
#line 1760 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1761 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1762 "src/parser.tr"
        Parser_expect_newline(self);
#line 1763 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("|", 1LL), rhs);
    } else if (_t230.tag == Token_CaretEq) {
#line 1765 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1766 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1767 "src/parser.tr"
        Parser_expect_newline(self);
#line 1768 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("^", 1LL), rhs);
    } else if (_t230.tag == Token_LtLtEq) {
#line 1770 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1771 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1772 "src/parser.tr"
        Parser_expect_newline(self);
#line 1773 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("<<", 2LL), rhs);
    } else if (_t230.tag == Token_GtGtEq) {
#line 1775 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1776 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1777 "src/parser.tr"
        Parser_expect_newline(self);
#line 1778 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len(">>", 2LL), rhs);
    } else if (1) {
        __auto_type _ = _t230;
#line 1780 "src/parser.tr"
        /* pass */
    }
#line 1781 "src/parser.tr"
    Parser_expect_newline(self);
#line 1782 "src/parser.tr"
    return box_stmt(Stmt_ctor_SExpr(lhs));
}

__attribute__((hot)) Expr* Parser_parse_expr(Parser* self) {
#line 1788 "src/parser.tr"
    if (({ __auto_type _ord_t235 = (Parser_peek(self)); __auto_type _wr = ((_ord_t235.tag == Token_make_KwDo().tag)); _wr; })) {
#line 1789 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1790 "src/parser.tr"
        if (({ __auto_type _ord_t236 = (Parser_peek(self)); __auto_type _wr = ((_ord_t236.tag == Token_make_Colon().tag)); _wr; })) {
#line 1790 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1791 "src/parser.tr"
        Block* do_body = Parser_parse_block(self);
#line 1792 "src/parser.tr"
        Expr* _trret41 = box_expr(Expr_ctor_EDo(do_body));
        return _trret41;
    }
#line 1799 "src/parser.tr"
    if (({ __auto_type _ord_t237 = (Parser_peek(self)); __auto_type _wr = ((_ord_t237.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1800 "src/parser.tr"
        return Parser_parse_if_expr(self);
    }
#line 1804 "src/parser.tr"
    if (({ __auto_type _ord_t238 = (Parser_peek(self)); __auto_type _wr = ((_ord_t238.tag == Token_make_KwMatch().tag)); _wr; })) {
#line 1805 "src/parser.tr"
        return Parser_parse_match_expr(self);
    }
#line 1809 "src/parser.tr"
    if (({ __auto_type _ord_t239 = (Parser_peek(self)); __auto_type _wr = ((_ord_t239.tag == Token_make_KwLoop().tag)); _wr; })) {
#line 1810 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1811 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1812 "src/parser.tr"
        if (({ __auto_type _ord_t240 = (Parser_peek(self)); __auto_type _wr = ((_ord_t240.tag == Token_make_Colon().tag)); _wr; })) {
#line 1812 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1813 "src/parser.tr"
        Block* loop_body = Parser_parse_block(self);
#line 1814 "src/parser.tr"
        Expr* _trret42 = box_expr(Expr_ctor_ELoop(loop_body));
        return _trret42;
    }
#line 1815 "src/parser.tr"
    if (({ __auto_type _ord_t241 = (Parser_peek(self)); __auto_type _wr = ((_ord_t241.tag == Token_make_KwWhile().tag)); _wr; })) {
#line 1816 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1817 "src/parser.tr"
        Expr* we_cond = Parser_parse_expr(self);
#line 1818 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1819 "src/parser.tr"
        if (({ __auto_type _ord_t242 = (Parser_peek(self)); __auto_type _wr = ((_ord_t242.tag == Token_make_Colon().tag)); _wr; })) {
#line 1819 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1820 "src/parser.tr"
        Block* we_body = Parser_parse_block(self);
#line 1821 "src/parser.tr"
        Block* we_else = Block_init();
#line 1822 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1823 "src/parser.tr"
        if (({ __auto_type _ord_t243 = (Parser_peek(self)); __auto_type _wr = ((_ord_t243.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1824 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1825 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1826 "src/parser.tr"
            if (({ __auto_type _ord_t244 = (Parser_peek(self)); __auto_type _wr = ((_ord_t244.tag == Token_make_Colon().tag)); _wr; })) {
#line 1826 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 1827 "src/parser.tr"
            we_else = Parser_parse_block(self);
        }
#line 1828 "src/parser.tr"
        Expr* _trret43 = box_expr(Expr_ctor_EWhileExpr(we_cond, we_body, we_else));
        return _trret43;
    }
#line 1829 "src/parser.tr"
    return Parser_parse_ternary(self);
}

__attribute__((hot)) Expr* Parser_parse_match_expr(Parser* self) {
#line 1832 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1833 "src/parser.tr"
    Expr* subj = Parser_parse_expr(self);
#line 1834 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1835 "src/parser.tr"
    if (({ __auto_type _ord_t245 = (Parser_peek(self)); __auto_type _wr = ((_ord_t245.tag == Token_make_Colon().tag)); _wr; })) {
#line 1835 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1836 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1837 "src/parser.tr"
    return box_expr(Expr_ctor_EMatch(subj, arms));
}

__attribute__((hot)) Expr* Parser_parse_if_expr(Parser* self) {
#line 1844 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1845 "src/parser.tr"
    Expr* cond = Parser_parse_expr(self);
#line 1846 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1847 "src/parser.tr"
    if (({ __auto_type _ord_t246 = (Parser_peek(self)); __auto_type _wr = ((_ord_t246.tag == Token_make_Colon().tag)); _wr; })) {
#line 1847 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1848 "src/parser.tr"
    Block* then_b = Parser_parse_block(self);
#line 1849 "src/parser.tr"
    Expr* then_e = box_expr(Expr_ctor_EDo(then_b));
#line 1850 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1851 "src/parser.tr"
    Expr* else_e = box_expr(Expr_ctor_EDo(Block_init()));
#line 1852 "src/parser.tr"
    __auto_type _t247 = Parser_peek(self);
    if (_t247.tag == Token_KwElif) {
#line 1854 "src/parser.tr"
        else_e = Parser_parse_if_expr(self);
    } else if (_t247.tag == Token_KwElse) {
#line 1856 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1857 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1858 "src/parser.tr"
        if (({ __auto_type _ord_t248 = (Parser_peek(self)); __auto_type _wr = ((_ord_t248.tag == Token_make_Colon().tag)); _wr; })) {
#line 1858 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1859 "src/parser.tr"
        Block* else_b = Parser_parse_block(self);
#line 1860 "src/parser.tr"
        else_e = box_expr(Expr_ctor_EDo(else_b));
    } else if (1) {
        __auto_type _ = _t247;
        /* pass */
    }
#line 1862 "src/parser.tr"
    Expr* _trret44 = box_expr(Expr_ctor_EIfElse(cond, then_e, else_e));
    return _trret44;
}

__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self) {
#line 1865 "src/parser.tr"
    Expr* left = Parser_parse_or_expr(self);
#line 1866 "src/parser.tr"
    if (({ __auto_type _ord_t249 = (Parser_peek(self)); __auto_type _wr = ((_ord_t249.tag == Token_make_Newline().tag)); _wr; })) {
#line 1867 "src/parser.tr"
        return left;
    }
#line 1870 "src/parser.tr"
    __auto_type _t250 = (*left);
    if (_t250.tag == Expr_EClosure) {
#line 1872 "src/parser.tr"
        return left;
    } else if (1) {
        __auto_type _ = _t250;
        /* pass */
    }
#line 1874 "src/parser.tr"
    if (({ __auto_type _ord_t251 = (Parser_peek(self)); __auto_type _wr = ((_ord_t251.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1875 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1876 "src/parser.tr"
        Expr* cond = Parser_parse_or_expr(self);
#line 1877 "src/parser.tr"
        if (({ __auto_type _ord_t252 = (Parser_peek(self)); __auto_type _wr = ((_ord_t252.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1878 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1879 "src/parser.tr"
        Expr* else_e = Parser_parse_ternary(self);
#line 1880 "src/parser.tr"
        return box_expr(Expr_ctor_EIfElse(cond, left, else_e));
    }
#line 1881 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self) {
#line 1884 "src/parser.tr"
    Expr* left = Parser_parse_and_expr(self);
#line 1885 "src/parser.tr"
    while (({ __auto_type _ord_t253 = (Parser_peek(self)); __auto_type _wr = ((_ord_t253.tag == Token_make_KwOr().tag)); _wr; })) {
#line 1886 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1887 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1888 "src/parser.tr"
        __auto_type right = Parser_parse_and_expr(self);
#line 1889 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("or", 2LL), left, right));
    }
#line 1890 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self) {
#line 1893 "src/parser.tr"
    Expr* left = Parser_parse_not_expr(self);
#line 1894 "src/parser.tr"
    while (({ __auto_type _ord_t254 = (Parser_peek(self)); __auto_type _wr = ((_ord_t254.tag == Token_make_KwAnd().tag)); _wr; })) {
#line 1895 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1896 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1897 "src/parser.tr"
        __auto_type right = Parser_parse_not_expr(self);
#line 1898 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), left, right));
    }
#line 1899 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self) {
#line 1902 "src/parser.tr"
    __auto_type _t255 = Parser_peek(self);
    if (_t255.tag == Token_KwNot) {
#line 1904 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1905 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), Parser_parse_not_expr(self)));
    } else if (1) {
        __auto_type _ = _t255;
#line 1907 "src/parser.tr"
        /* pass */
    }
#line 1908 "src/parser.tr"
    return Parser_parse_comparison(self);
}

__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self) {
#line 1911 "src/parser.tr"
    Expr* left = Parser_parse_bitor_expr(self);
#line 1913 "src/parser.tr"
    if (({ __auto_type _ord_t256 = (Parser_peek(self)); __auto_type _wr = ((_ord_t256.tag == Token_make_KwNot().tag)); _wr; })) {
#line 1914 "src/parser.tr"
        long long saved_pos = self->pos;
#line 1915 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1916 "src/parser.tr"
        if (({ __auto_type _ord_t257 = (Parser_peek(self)); __auto_type _wr = ((_ord_t257.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1917 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1918 "src/parser.tr"
            return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("in", 2LL), left, Parser_parse_bitor_expr(self)))));
        }
#line 1919 "src/parser.tr"
        self->pos = saved_pos;
    }
#line 1922 "src/parser.tr"
    List_ptr* operands = (void*)List_ptr_new();
#line 1923 "src/parser.tr"
    List_ptr_append(operands, left);
#line 1924 "src/parser.tr"
    List_TrStr* ops = (void*)List_TrStr_new();
#line 1925 "src/parser.tr"
    while (true) {
#line 1926 "src/parser.tr"
        TrStr op = Parser__peek_cmp_op(self);
#line 1927 "src/parser.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("", 0LL)))) {
#line 1927 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
#line 1928 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1929 "src/parser.tr"
        List_TrStr_append(ops, op);
#line 1930 "src/parser.tr"
        List_ptr_append(operands, Parser_parse_bitor_expr(self));
#line 1931 "src/parser.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("is", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))))) {
#line 1931 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
        _tr_str_release(op);
    }
#line 1932 "src/parser.tr"
    if ((ops->len == 0LL)) {
#line 1932 "src/parser.tr"
        Expr* _trret45 = left;
        List_TrStr_free(ops);
        return _trret45;
    }
#line 1933 "src/parser.tr"
    Expr* result = ({ TrStr _ord_t258 = (List_TrStr_get(ops, 0LL)); __auto_type _ord_t259 = (((Expr*)List_ptr_get(operands, 0LL))); __auto_type _ord_t260 = (((Expr*)List_ptr_get(operands, 1LL))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t258, _ord_t259, _ord_t260))); _tr_str_release(_ord_t258); _wr; });
#line 1934 "src/parser.tr"
    long long ci = 1LL;
#line 1935 "src/parser.tr"
    while ((ci < ops->len)) {
#line 1936 "src/parser.tr"
        Expr* cmp = ({ TrStr _ord_t261 = (List_TrStr_get(ops, ci)); __auto_type _ord_t262 = (((Expr*)List_ptr_get(operands, ci))); __auto_type _ord_t263 = (((Expr*)List_ptr_get(operands, (ci + 1LL)))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t261, _ord_t262, _ord_t263))); _tr_str_release(_ord_t261); _wr; });
#line 1937 "src/parser.tr"
        result = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), result, cmp));
#line 1938 "src/parser.tr"
        ci = (ci + 1LL);
    }
#line 1939 "src/parser.tr"
    Expr* _trret46 = result;
    List_TrStr_free(ops);
    return _trret46;
}

__attribute__((hot)) TrStr Parser__peek_cmp_op(Parser* self) {
#line 1942 "src/parser.tr"
    __auto_type _t264 = Parser_peek(self);
    if (_t264.tag == Token_EqEq) {
        return _tr_str_lit_len("==", 2LL);
    } else if (_t264.tag == Token_NotEq) {
        return _tr_str_lit_len("!=", 2LL);
    } else if (_t264.tag == Token_Lt) {
        return _tr_str_lit_len("<", 1LL);
    } else if (_t264.tag == Token_Gt) {
        return _tr_str_lit_len(">", 1LL);
    } else if (_t264.tag == Token_LtEq) {
        return _tr_str_lit_len("<=", 2LL);
    } else if (_t264.tag == Token_GtEq) {
        return _tr_str_lit_len(">=", 2LL);
    } else if (_t264.tag == Token_KwIs) {
        return _tr_str_lit_len("is", 2LL);
    } else if (_t264.tag == Token_KwIn) {
        return _tr_str_lit_len("in", 2LL);
    } else if (1) {
        __auto_type _ = _t264;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self) {
#line 1954 "src/parser.tr"
    Expr* left = Parser_parse_bitxor_expr(self);
#line 1955 "src/parser.tr"
    while (({ __auto_type _ord_t265 = (Parser_peek(self)); __auto_type _wr = ((_ord_t265.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1956 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1957 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("|", 1LL), left, Parser_parse_bitxor_expr(self)));
    }
#line 1958 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self) {
#line 1961 "src/parser.tr"
    Expr* left = Parser_parse_bitand_expr(self);
#line 1962 "src/parser.tr"
    while (({ __auto_type _ord_t266 = (Parser_peek(self)); __auto_type _wr = ((_ord_t266.tag == Token_make_Caret().tag)); _wr; })) {
#line 1963 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1964 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("^", 1LL), left, Parser_parse_bitand_expr(self)));
    }
#line 1965 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self) {
#line 1968 "src/parser.tr"
    Expr* left = Parser_parse_shift_expr(self);
#line 1969 "src/parser.tr"
    while (({ __auto_type _ord_t267 = (Parser_peek(self)); __auto_type _wr = ((_ord_t267.tag == Token_make_Amp().tag)); _wr; })) {
#line 1970 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1971 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("&", 1LL), left, Parser_parse_shift_expr(self)));
    }
#line 1972 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self) {
#line 1975 "src/parser.tr"
    Expr* left = Parser_parse_additive(self);
#line 1976 "src/parser.tr"
    while (true) {
#line 1977 "src/parser.tr"
        __auto_type _t268 = Parser_peek(self);
        if (_t268.tag == Token_LtLt) {
#line 1979 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1980 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("<<", 2LL), left, Parser_parse_additive(self)));
        } else if (_t268.tag == Token_GtGt) {
#line 1982 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1983 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len(">>", 2LL), left, Parser_parse_additive(self)));
        } else if (1) {
            __auto_type _ = _t268;
            break;
        }
    }
#line 1985 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_additive(Parser* self) {
#line 1988 "src/parser.tr"
    Expr* left = Parser_parse_multiplicative(self);
#line 1989 "src/parser.tr"
    while (true) {
#line 1990 "src/parser.tr"
        __auto_type _t269 = Parser_peek(self);
        if (_t269.tag == Token_Plus) {
#line 1992 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1993 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 1994 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("+", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (_t269.tag == Token_Minus) {
#line 1996 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1997 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 1998 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("-", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (1) {
            __auto_type _ = _t269;
            break;
        }
    }
#line 2000 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self) {
#line 2003 "src/parser.tr"
    Expr* left = Parser_parse_unary(self);
#line 2004 "src/parser.tr"
    while (true) {
#line 2005 "src/parser.tr"
        __auto_type _t270 = Parser_peek(self);
        if (_t270.tag == Token_Star) {
#line 2007 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2008 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("*", 1LL), left, Parser_parse_unary(self)));
        } else if (_t270.tag == Token_Slash) {
#line 2010 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2011 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("/", 1LL), left, Parser_parse_unary(self)));
        } else if (_t270.tag == Token_Percent) {
#line 2013 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2014 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("%", 1LL), left, Parser_parse_unary(self)));
        } else if (_t270.tag == Token_FloorDiv) {
#line 2016 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2017 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("//", 2LL), left, Parser_parse_unary(self)));
        } else if (1) {
            __auto_type _ = _t270;
            break;
        }
    }
#line 2019 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_power(Parser* self) {
#line 2022 "src/parser.tr"
    Expr* base = Parser_parse_postfix(self);
#line 2023 "src/parser.tr"
    if (({ __auto_type _ord_t271 = (Parser_peek(self)); __auto_type _wr = ((_ord_t271.tag == Token_make_StarStar().tag)); _wr; })) {
#line 2024 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2026 "src/parser.tr"
        Expr* exp = Parser_parse_unary(self);
#line 2027 "src/parser.tr"
        return box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("**", 2LL), base, exp));
    }
#line 2028 "src/parser.tr"
    return base;
}

__attribute__((hot)) Expr* Parser_parse_unary(Parser* self) {
#line 2031 "src/parser.tr"
    __auto_type _t272 = Parser_peek(self);
    if (_t272.tag == Token_Minus) {
#line 2033 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2034 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("-", 1LL), Parser_parse_unary(self)));
    } else if (_t272.tag == Token_Tilde) {
#line 2036 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2037 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("~", 1LL), Parser_parse_unary(self)));
    } else if (_t272.tag == Token_Amp) {
#line 2039 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2040 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("&", 1LL), Parser_parse_unary(self)));
    } else if (_t272.tag == Token_Star) {
#line 2042 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2043 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("*", 1LL), Parser_parse_unary(self)));
    } else if (1) {
        __auto_type _ = _t272;
#line 2045 "src/parser.tr"
        /* pass */
    }
#line 2046 "src/parser.tr"
    return Parser_parse_power(self);
}

__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self) {
#line 2049 "src/parser.tr"
    Expr* e = Parser_parse_primary(self);
#line 2050 "src/parser.tr"
    while (true) {
#line 2051 "src/parser.tr"
        __auto_type _t273 = Parser_peek(self);
        if (_t273.tag == Token_Dot) {
#line 2053 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2054 "src/parser.tr"
            TrStr attr = Parser_consume_method_ident(self);
#line 2055 "src/parser.tr"
            __auto_type _t274 = Parser_peek(self);
            if (_t274.tag == Token_LParen) {
#line 2057 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2058 "src/parser.tr"
                e = box_expr(Expr_ctor_EMethodCall(e, attr, Parser_parse_arg_list(self)));
            } else if (1) {
                __auto_type _ = _t274;
#line 2060 "src/parser.tr"
                e = box_expr(Expr_ctor_EPropAccess(e, attr));
            }
            _tr_str_release(attr);
        } else if (_t273.tag == Token_LParen) {
#line 2062 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2067 "src/parser.tr"
            __auto_type _t275 = (*e);
            if (_t275.tag == Expr_EIdent) {
                __auto_type _pf_nm = _t275.data.EIdent.name;
#line 2069 "src/parser.tr"
                if (_tr_dict_contains(self->import_aliases, _tr_strz(_pf_nm))) {
#line 2070 "src/parser.tr"
                    e = ({ TrStr _at_t276 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->import_aliases, _tr_strz(_pf_nm))))); __auto_type _wr = (box_expr(Expr_ctor_EIdent(_at_t276))); _tr_str_release(_at_t276); _wr; });
                }
            } else if (1) {
                __auto_type _ = _t275;
#line 2072 "src/parser.tr"
                /* pass */
            }
#line 2073 "src/parser.tr"
            e = box_expr(Expr_ctor_ECall(e, Parser_parse_arg_list(self)));
        } else if (_t273.tag == Token_LBracket) {
#line 2075 "src/parser.tr"
            long long ix_oln = Parser_cur_line(self);
#line 2076 "src/parser.tr"
            long long ix_ocol = Parser_cur_col(self);
#line 2077 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2082 "src/parser.tr"
            Expr* idx = ((Expr*)(0LL));
#line 2083 "src/parser.tr"
            if (({ __auto_type _ord_t277 = (Parser_peek(self)); __auto_type _wr = ((_ord_t277.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2084 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self))));
            } else {
#line 2086 "src/parser.tr"
                idx = Parser_parse_expr(self);
            }
#line 2096 "src/parser.tr"
            List_ptr* _ti_extra = (void*)List_ptr_new();
#line 2097 "src/parser.tr"
            while (({ __auto_type _ord_t278 = (Parser_peek(self)); __auto_type _wr = ((_ord_t278.tag == Token_make_Comma().tag)); _wr; })) {
#line 2098 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2099 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
#line 2100 "src/parser.tr"
                if (({ __auto_type _ord_t279 = (Parser_peek(self)); __auto_type _wr = ((_ord_t279.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2101 "src/parser.tr"
                    List_ptr_append(_ti_extra, box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self)))));
                } else {
#line 2103 "src/parser.tr"
                    List_ptr_append(_ti_extra, Parser_parse_expr(self));
                }
            }
#line 2104 "src/parser.tr"
            Parser_expect_rbracket(self, ix_oln, ix_ocol, _tr_str_lit_len("this index", 10LL));
#line 2105 "src/parser.tr"
            if ((_ti_extra->len > 0LL)) {
#line 2106 "src/parser.tr"
                List_ptr* _ti_all = (void*)List_ptr_new();
#line 2107 "src/parser.tr"
                List_ptr_append(_ti_all, idx);
#line 2108 "src/parser.tr"
                long long _tj = 0LL;
#line 2109 "src/parser.tr"
                while ((_tj < _ti_extra->len)) {
#line 2110 "src/parser.tr"
                    List_ptr_append(_ti_all, ((Expr*)List_ptr_get(_ti_extra, _tj)));
#line 2111 "src/parser.tr"
                    _tj = (_tj + 1LL);
                }
#line 2112 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETuple(_ti_all));
            }
#line 2113 "src/parser.tr"
            e = box_expr(Expr_ctor_EIndex(e, idx));
        } else if (_t273.tag == Token_Question) {
#line 2115 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2116 "src/parser.tr"
            e = box_expr(Expr_ctor_ETryExpr(e));
        } else if (_t273.tag == Token_KwAs) {
#line 2118 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2119 "src/parser.tr"
            e = box_expr(Expr_ctor_ECast(e, box_asttype(Parser_parse_type(self))));
        } else if (1) {
            __auto_type _ = _t273;
            break;
        }
    }
#line 2121 "src/parser.tr"
    return e;
}

__attribute__((hot)) void Parser_emit_diag_at(Parser* self, long long ln, long long col, TrStr msg, TrStr hint) {
#line 2127 "src/parser.tr"
    ({ __auto_type _ord_t280 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t280); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
#line 2128 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 2129 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 2130 "src/parser.tr"
        TrStr _strtmp_t281 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t281;
    }
#line 2131 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 2132 "src/parser.tr"
    if ((col > 0LL)) {
#line 2133 "src/parser.tr"
        ({ TrStr _ap_t282 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t283 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t282); _tr_str_append(&head, _ap_t283); _tr_str_release(_ap_t283); });
    }
#line 2134 "src/parser.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 2135 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 2136 "src/parser.tr"
        TrStr srcline = _nth_source_line(self->src_text, ln);
#line 2137 "src/parser.tr"
        long long full_col2 = col;
#line 2138 "src/parser.tr"
        long long col2 = _clip_col(srcline, full_col2);
#line 2139 "src/parser.tr"
        TrStr _strtmp_t284 = _clip_line(srcline, full_col2);
        _tr_str_release(srcline);
        srcline = _strtmp_t284;
#line 2140 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 2141 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2142 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2143 "src/parser.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 2144 "src/parser.tr"
            if ((col > 0LL)) {
#line 2145 "src/parser.tr"
                ({ __auto_type _ord_t285 = (c_dim(gbar)); __auto_type _ord_t286 = (({ TrStr _tr_cl = (_ord_t285); TrStr _tr_cr = (_spaces((col2 - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t286); TrStr _tr_cr = (c_red(_tr_str_lit_len("^", 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); });
            }
            _tr_str_release(gnum);
        }
    }
#line 2146 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 2147 "src/parser.tr"
        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
    }
#line 2148 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) void Parser_expect_rparen(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2155 "src/parser.tr"
    __auto_type _t287 = Parser_peek(self);
    if (_t287.tag == Token_RParen) {
#line 2157 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t287;
#line 2159 "src/parser.tr"
        ({ TrStr _at_t288 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '(' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t288, _tr_str_lit_len("add the matching ')'.", 21LL)); _tr_str_release(_at_t288); });
    }
}

__attribute__((hot)) void Parser_expect_rbracket(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2161 "src/parser.tr"
    __auto_type _t289 = Parser_peek(self);
    if (_t289.tag == Token_RBracket) {
#line 2163 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t289;
#line 2165 "src/parser.tr"
        ({ TrStr _at_t290 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t290, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t290); });
    }
}

__attribute__((hot)) void Parser_expect_rbrace(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2167 "src/parser.tr"
    __auto_type _t291 = Parser_peek(self);
    if (_t291.tag == Token_RBrace) {
#line 2169 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t291;
#line 2171 "src/parser.tr"
        ({ TrStr _at_t292 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '{' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t292, _tr_str_lit_len("add the matching '}'.", 21LL)); _tr_str_release(_at_t292); });
    }
}

__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self) {
#line 2176 "src/parser.tr"
    long long oln = 0LL;
#line 2177 "src/parser.tr"
    long long ocol = 0LL;
#line 2178 "src/parser.tr"
    if ((self->pos > 0LL)) {
#line 2179 "src/parser.tr"
        oln = List_i64_get(self->lines, (self->pos - 1LL));
#line 2180 "src/parser.tr"
        ocol = List_i64_get(self->cols, (self->pos - 1LL));
    }
#line 2181 "src/parser.tr"
    List_ptr* el = (void*)List_ptr_new();
#line 2182 "src/parser.tr"
    bool going = true;
#line 2183 "src/parser.tr"
    while (going) {
#line 2184 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 2185 "src/parser.tr"
        __auto_type _t293 = Parser_peek(self);
        if ((_t293.tag == Token_RParen || _t293.tag == Token_Eof)) {
#line 2187 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t293;
#line 2189 "src/parser.tr"
            /* pass */
        }
#line 2190 "src/parser.tr"
        if (going) {
#line 2191 "src/parser.tr"
            List_ptr_append(el, Parser_parse_expr(self));
#line 2192 "src/parser.tr"
            __auto_type _t294 = Parser_peek(self);
            if (_t294.tag == Token_Comma) {
#line 2194 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2195 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            } else if (1) {
                __auto_type _ = _t294;
#line 2197 "src/parser.tr"
                going = false;
            }
        }
    }
#line 2198 "src/parser.tr"
    Parser_expect_rparen(self, oln, ocol, _tr_str_lit_len("this call's argument list", 25LL));
#line 2199 "src/parser.tr"
    return el;
}

__attribute__((hot)) Expr* Parser_parse_primary(Parser* self) {
#line 2202 "src/parser.tr"
    __auto_type _t295 = Parser_peek(self);
    if (_t295.tag == Token_IntLit) {
        __auto_type v = _t295.data.IntLit.val;
#line 2204 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2205 "src/parser.tr"
        return box_expr(Expr_ctor_ELitInt(v));
    } else if (_t295.tag == Token_FloatLit) {
        __auto_type v = _t295.data.FloatLit.val;
#line 2207 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2208 "src/parser.tr"
        return box_expr(Expr_ctor_ELitFloat(v));
    } else if (_t295.tag == Token_StrLit) {
        __auto_type s = _t295.data.StrLit.val;
__auto_type blen = _t295.data.StrLit.blen;
#line 2210 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2211 "src/parser.tr"
        Expr* _trret47 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret47;
    } else if (_t295.tag == Token_TripleStrLit) {
        __auto_type s = _t295.data.TripleStrLit.val;
__auto_type blen = _t295.data.TripleStrLit.blen;
#line 2213 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2214 "src/parser.tr"
        Expr* _trret48 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret48;
    } else if (_t295.tag == Token_RawStrLit) {
        __auto_type s = _t295.data.RawStrLit.val;
__auto_type blen = _t295.data.RawStrLit.blen;
#line 2216 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2217 "src/parser.tr"
        Expr* _trret49 = box_expr(Expr_ctor_ERawStr(s, blen));
        return _trret49;
    } else if (_t295.tag == Token_ByteStrLit) {
        __auto_type s = _t295.data.ByteStrLit.val;
__auto_type blen = _t295.data.ByteStrLit.blen;
#line 2219 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2220 "src/parser.tr"
        Expr* _trret50 = box_expr(Expr_ctor_ELitBytes(s, blen));
        return _trret50;
    } else if (_t295.tag == Token_FStrLit) {
        __auto_type s = _t295.data.FStrLit.val;
__auto_type blen = _t295.data.FStrLit.blen;
#line 2222 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2223 "src/parser.tr"
        Expr* _trret51 = Parser_parse_fstring(self, s, blen);
        return _trret51;
    } else if (_t295.tag == Token_CharLit) {
        __auto_type v = _t295.data.CharLit.val;
#line 2225 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2226 "src/parser.tr"
        return box_expr(Expr_ctor_ELitChar(v));
    } else if (_t295.tag == Token_BoolLit) {
        __auto_type v = _t295.data.BoolLit.val;
#line 2228 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2229 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(v));
    } else if (_t295.tag == Token_KwTrue) {
#line 2231 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2232 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(true));
    } else if (_t295.tag == Token_KwFalse) {
#line 2234 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2235 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(false));
    } else if (_t295.tag == Token_KwNone) {
#line 2237 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2238 "src/parser.tr"
        return box_expr(Expr_make_ELitNone());
    } else if (_t295.tag == Token_KwInt) {
#line 2240 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2241 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("int", 3LL)));
    } else if (_t295.tag == Token_KwFloat) {
#line 2243 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2244 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("float", 5LL)));
    } else if (_t295.tag == Token_KwBool) {
#line 2246 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2247 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("bool", 4LL)));
    } else if ((_t295.tag == Token_KwStr || _t295.tag == Token_KwString)) {
#line 2249 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2250 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("str", 3LL)));
    } else if (_t295.tag == Token_KwChar) {
#line 2252 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2253 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("char", 4LL)));
    } else if (_t295.tag == Token_KwVoid) {
#line 2255 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2256 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("void", 4LL)));
    } else if (_t295.tag == Token_KwSuper) {
#line 2258 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2259 "src/parser.tr"
        TrStr super_base = _tr_str_lit_len("", 0LL);
#line 2260 "src/parser.tr"
        if (({ __auto_type _ord_t296 = (Parser_peek(self)); __auto_type _wr = ((_ord_t296.tag == Token_make_LParen().tag)); _wr; })) {
#line 2261 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2262 "src/parser.tr"
            TrStr _strtmp_t297 = Parser_consume_ident(self);
            _tr_str_release(super_base);
            super_base = _strtmp_t297;
#line 2263 "src/parser.tr"
            if (({ __auto_type _ord_t298 = (Parser_peek(self)); __auto_type _wr = ((_ord_t298.tag == Token_make_RParen().tag)); _wr; })) {
#line 2264 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 2265 "src/parser.tr"
            if (({ __auto_type _ord_t299 = (Parser_peek(self)); __auto_type _wr = ((_ord_t299.tag == Token_make_Dot().tag)); _wr; })) {
#line 2266 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2267 "src/parser.tr"
                TrStr super_method = Parser_consume_ident(self);
#line 2268 "src/parser.tr"
                if (({ __auto_type _ord_t300 = (Parser_peek(self)); __auto_type _wr = ((_ord_t300.tag == Token_make_LParen().tag)); _wr; })) {
#line 2269 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2270 "src/parser.tr"
                    List_ptr* super_args = Parser_parse_arg_list(self);
#line 2271 "src/parser.tr"
                    Expr* _trret52 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method, super_args));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret52;
                } else {
#line 2273 "src/parser.tr"
                    Expr* _trret53 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret53;
                }
            }
        }
#line 2274 "src/parser.tr"
        if (({ __auto_type _ord_t301 = (Parser_peek(self)); __auto_type _wr = ((_ord_t301.tag == Token_make_Dot().tag)); _wr; })) {
#line 2275 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2276 "src/parser.tr"
            TrStr super_next = Parser_consume_ident(self);
#line 2277 "src/parser.tr"
            if (({ __auto_type _ord_t302 = (Parser_peek(self)); __auto_type _wr = ((_ord_t302.tag == Token_make_Dot().tag)); _wr; })) {
#line 2278 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2279 "src/parser.tr"
                TrStr _strtmp_t303 = _tr_str_retain(super_next);
                _tr_str_release(super_base);
                super_base = _strtmp_t303;
#line 2280 "src/parser.tr"
                TrStr super_method2 = Parser_consume_ident(self);
#line 2281 "src/parser.tr"
                if (({ __auto_type _ord_t304 = (Parser_peek(self)); __auto_type _wr = ((_ord_t304.tag == Token_make_LParen().tag)); _wr; })) {
#line 2282 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2283 "src/parser.tr"
                    List_ptr* super_args2 = Parser_parse_arg_list(self);
#line 2284 "src/parser.tr"
                    Expr* _trret54 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method2, super_args2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret54;
                } else {
#line 2286 "src/parser.tr"
                    Expr* _trret55 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret55;
                }
            } else {
#line 2288 "src/parser.tr"
                if (({ __auto_type _ord_t305 = (Parser_peek(self)); __auto_type _wr = ((_ord_t305.tag == Token_make_LParen().tag)); _wr; })) {
#line 2289 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2290 "src/parser.tr"
                    List_ptr* super_args3 = Parser_parse_arg_list(self);
#line 2291 "src/parser.tr"
                    Expr* _trret56 = box_expr(Expr_ctor_ESuperMethodCall(_tr_str_lit_len("", 0LL), super_next, super_args3));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret56;
                } else {
#line 2293 "src/parser.tr"
                    Expr* _trret57 = box_expr(Expr_ctor_ESuperPropAccess(_tr_str_lit_len("", 0LL), super_next));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret57;
                }
            }
        }
#line 2294 "src/parser.tr"
        Expr* _trret58 = box_expr(Expr_ctor_EIdent(_tr_str_lit_len("super", 5LL)));
        _tr_str_release(super_base);
        return _trret58;
    } else if (_t295.tag == Token_Ident) {
        __auto_type name = _t295.data.Ident.name;
#line 2296 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2299 "src/parser.tr"
        if (({ __auto_type _ord_t306 = (Parser_peek(self)); __auto_type _wr = ((_ord_t306.tag == Token_make_Bang().tag)); _wr; })) {
#line 2300 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2301 "src/parser.tr"
            if (({ __auto_type _ord_t307 = (Parser_peek(self)); __auto_type _wr = ((_ord_t307.tag == Token_make_LParen().tag)); _wr; })) {
#line 2302 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2303 "src/parser.tr"
                List_ptr* mac_args = Parser_parse_arg_list(self);
#line 2304 "src/parser.tr"
                Expr* _trret59 = box_expr(Expr_ctor_EMacroCall(name, mac_args));
                return _trret59;
            }
        }
#line 2305 "src/parser.tr"
        Expr* _trret60 = box_expr(Expr_ctor_EIdent(name));
        return _trret60;
    } else if (_t295.tag == Token_KwSizeOf) {
#line 2307 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2308 "src/parser.tr"
        __auto_type _t308 = Parser_peek(self);
        if (_t308.tag == Token_LParen) {
#line 2310 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t308;
#line 2312 "src/parser.tr"
            /* pass */
        }
#line 2313 "src/parser.tr"
        AstType** ty = box_asttype(Parser_parse_type(self));
#line 2314 "src/parser.tr"
        __auto_type _t309 = Parser_peek(self);
        if (_t309.tag == Token_RParen) {
#line 2316 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t309;
#line 2318 "src/parser.tr"
            /* pass */
        }
#line 2319 "src/parser.tr"
        return box_expr(Expr_ctor_ESizeOf(ty));
    } else if (_t295.tag == Token_LParen) {
#line 2321 "src/parser.tr"
        long long p_oln = Parser_cur_line(self);
#line 2322 "src/parser.tr"
        long long p_ocol = Parser_cur_col(self);
#line 2323 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2324 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2325 "src/parser.tr"
        if (({ __auto_type _ord_t310 = (Parser_peek(self)); __auto_type _wr = ((_ord_t310.tag == Token_make_RParen().tag)); _wr; })) {
#line 2326 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2327 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple((void*)List_ptr_new()));
        }
#line 2328 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 2329 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2330 "src/parser.tr"
        if (({ __auto_type _ord_t311 = (Parser_peek(self)); __auto_type _wr = ((_ord_t311.tag == Token_make_Comma().tag)); _wr; })) {
#line 2331 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2332 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2333 "src/parser.tr"
            List_ptr* items = (void*)List_ptr_new();
#line 2334 "src/parser.tr"
            List_ptr_append(items, e);
#line 2335 "src/parser.tr"
            while ((({ __auto_type _ord_t312 = (Parser_peek(self)); __auto_type _wr = ((_ord_t312.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t313 = (Parser_peek(self)); __auto_type _wr = ((_ord_t313.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2336 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2337 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2338 "src/parser.tr"
                if (({ __auto_type _ord_t314 = (Parser_peek(self)); __auto_type _wr = ((_ord_t314.tag == Token_make_Comma().tag)); _wr; })) {
#line 2339 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2340 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2342 "src/parser.tr"
                    break;
                }
            }
#line 2343 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this tuple", 10LL));
#line 2344 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple(items));
        } else {
#line 2346 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this parenthesized expression", 29LL));
#line 2347 "src/parser.tr"
            return e;
        }
    } else if (_t295.tag == Token_LBracket) {
#line 2349 "src/parser.tr"
        long long lb_oln = Parser_cur_line(self);
#line 2350 "src/parser.tr"
        long long lb_ocol = Parser_cur_col(self);
#line 2351 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2352 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2353 "src/parser.tr"
        if (({ __auto_type _ord_t315 = (Parser_peek(self)); __auto_type _wr = ((_ord_t315.tag == Token_make_RBracket().tag)); _wr; })) {
#line 2354 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2355 "src/parser.tr"
            return box_expr(Expr_ctor_EList((void*)List_ptr_new()));
        }
#line 2356 "src/parser.tr"
        Expr* first = Parser_parse_or_expr(self);
#line 2357 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2358 "src/parser.tr"
        if (({ __auto_type _ord_t316 = (Parser_peek(self)); __auto_type _wr = ((_ord_t316.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2359 "src/parser.tr"
            List_ptr* generators = (void*)List_ptr_new();
#line 2360 "src/parser.tr"
            while (({ __auto_type _ord_t317 = (Parser_peek(self)); __auto_type _wr = ((_ord_t317.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2361 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2362 "src/parser.tr"
                TrStr target = Parser_consume_ident(self);
#line 2363 "src/parser.tr"
                __auto_type _t318 = Parser_peek(self);
                if (_t318.tag == Token_KwIn) {
#line 2365 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t318;
#line 2367 "src/parser.tr"
                    /* pass */
                }
#line 2368 "src/parser.tr"
                Expr* iter = Parser_parse_or_expr(self);
#line 2369 "src/parser.tr"
                List_ptr* ifs = (void*)List_ptr_new();
#line 2370 "src/parser.tr"
                while (({ __auto_type _ord_t319 = (Parser_peek(self)); __auto_type _wr = ((_ord_t319.tag == Token_make_KwIf().tag)); _wr; })) {
#line 2371 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2372 "src/parser.tr"
                    List_ptr_append(ifs, Parser_parse_or_expr(self));
                }
#line 2373 "src/parser.tr"
                Comprehension* c = Comprehension_init(target, iter);
#line 2374 "src/parser.tr"
                c->ifs = ifs;
#line 2375 "src/parser.tr"
                /* unsafe block */
#line 2376 "src/parser.tr"
                Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 2377 "src/parser.tr"
                (*c_ptr = c);
#line 2378 "src/parser.tr"
                List_ptr_append(generators, c_ptr);
                _tr_str_release(target);
            }
#line 2379 "src/parser.tr"
            Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list comprehension", 23LL));
#line 2380 "src/parser.tr"
            return box_expr(Expr_ctor_EListComp(first, generators));
        }
#line 2381 "src/parser.tr"
        List_ptr* items = (void*)List_ptr_new();
#line 2382 "src/parser.tr"
        List_ptr_append(items, first);
#line 2383 "src/parser.tr"
        if (({ __auto_type _ord_t320 = (Parser_peek(self)); __auto_type _wr = ((_ord_t320.tag == Token_make_Comma().tag)); _wr; })) {
#line 2384 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2385 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2386 "src/parser.tr"
            while ((({ __auto_type _ord_t321 = (Parser_peek(self)); __auto_type _wr = ((_ord_t321.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t322 = (Parser_peek(self)); __auto_type _wr = ((_ord_t322.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2387 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2388 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2389 "src/parser.tr"
                if (({ __auto_type _ord_t323 = (Parser_peek(self)); __auto_type _wr = ((_ord_t323.tag == Token_make_Comma().tag)); _wr; })) {
#line 2390 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2391 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2393 "src/parser.tr"
                    break;
                }
            }
        }
#line 2394 "src/parser.tr"
        Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list", 9LL));
#line 2395 "src/parser.tr"
        return box_expr(Expr_ctor_EList(items));
    } else if (_t295.tag == Token_LBrace) {
#line 2397 "src/parser.tr"
        long long br_oln = Parser_cur_line(self);
#line 2398 "src/parser.tr"
        long long br_ocol = Parser_cur_col(self);
#line 2399 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2400 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2401 "src/parser.tr"
        List_ptr* keys = (void*)List_ptr_new();
#line 2402 "src/parser.tr"
        List_ptr* vals = (void*)List_ptr_new();
#line 2403 "src/parser.tr"
        List_ptr* set_items = (void*)List_ptr_new();
#line 2404 "src/parser.tr"
        bool is_set = false;
#line 2405 "src/parser.tr"
        if (({ __auto_type _ord_t324 = (Parser_peek(self)); __auto_type _wr = ((_ord_t324.tag != Token_make_RBrace().tag)); _wr; })) {
#line 2406 "src/parser.tr"
            Expr* first = Parser_parse_expr(self);
#line 2407 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2408 "src/parser.tr"
            if (({ __auto_type _ord_t325 = (Parser_peek(self)); __auto_type _wr = ((_ord_t325.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2409 "src/parser.tr"
                List_ptr* sgens = Parser_parse_comp_generators(self);
#line 2410 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2411 "src/parser.tr"
                Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this set comprehension", 22LL));
#line 2412 "src/parser.tr"
                return Parser_desugar_collection_comp(self, first, (Expr*)(0LL), sgens);
            }
#line 2413 "src/parser.tr"
            if (({ __auto_type _ord_t326 = (Parser_peek(self)); __auto_type _wr = ((_ord_t326.tag == Token_make_Colon().tag)); _wr; })) {
#line 2414 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2415 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2416 "src/parser.tr"
                Expr* value = Parser_parse_expr(self);
#line 2417 "src/parser.tr"
                if (({ __auto_type _ord_t327 = (Parser_peek(self)); __auto_type _wr = ((_ord_t327.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2418 "src/parser.tr"
                    List_ptr* dgens = Parser_parse_comp_generators(self);
#line 2419 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2420 "src/parser.tr"
                    Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict comprehension", 23LL));
#line 2421 "src/parser.tr"
                    return Parser_desugar_collection_comp(self, first, value, dgens);
                }
#line 2422 "src/parser.tr"
                List_ptr_append(keys, first);
#line 2423 "src/parser.tr"
                List_ptr_append(vals, value);
#line 2424 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2425 "src/parser.tr"
                if (({ __auto_type _ord_t328 = (Parser_peek(self)); __auto_type _wr = ((_ord_t328.tag == Token_make_Comma().tag)); _wr; })) {
#line 2426 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2427 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2428 "src/parser.tr"
                while ((({ __auto_type _ord_t329 = (Parser_peek(self)); __auto_type _wr = ((_ord_t329.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t330 = (Parser_peek(self)); __auto_type _wr = ((_ord_t330.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2429 "src/parser.tr"
                    Expr* key = Parser_parse_expr(self);
#line 2430 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2431 "src/parser.tr"
                    __auto_type _t331 = Parser_peek(self);
                    if (_t331.tag == Token_Colon) {
#line 2433 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t331;
#line 2435 "src/parser.tr"
                        /* pass */
                    }
#line 2436 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2437 "src/parser.tr"
                    Expr* v = Parser_parse_expr(self);
#line 2438 "src/parser.tr"
                    List_ptr_append(keys, key);
#line 2439 "src/parser.tr"
                    List_ptr_append(vals, v);
#line 2440 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2441 "src/parser.tr"
                    if (({ __auto_type _ord_t332 = (Parser_peek(self)); __auto_type _wr = ((_ord_t332.tag == Token_make_Comma().tag)); _wr; })) {
#line 2442 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2444 "src/parser.tr"
                        break;
                    }
#line 2445 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            } else {
#line 2447 "src/parser.tr"
                is_set = true;
#line 2448 "src/parser.tr"
                List_ptr_append(set_items, first);
#line 2449 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2450 "src/parser.tr"
                if (({ __auto_type _ord_t333 = (Parser_peek(self)); __auto_type _wr = ((_ord_t333.tag == Token_make_Comma().tag)); _wr; })) {
#line 2451 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2452 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2453 "src/parser.tr"
                while ((({ __auto_type _ord_t334 = (Parser_peek(self)); __auto_type _wr = ((_ord_t334.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t335 = (Parser_peek(self)); __auto_type _wr = ((_ord_t335.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2454 "src/parser.tr"
                    List_ptr_append(set_items, Parser_parse_expr(self));
#line 2455 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2456 "src/parser.tr"
                    if (({ __auto_type _ord_t336 = (Parser_peek(self)); __auto_type _wr = ((_ord_t336.tag == Token_make_Comma().tag)); _wr; })) {
#line 2457 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2459 "src/parser.tr"
                        break;
                    }
#line 2460 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            }
        }
#line 2461 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2462 "src/parser.tr"
        Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict/set literal", 21LL));
#line 2463 "src/parser.tr"
        if (is_set) {
#line 2464 "src/parser.tr"
            return box_expr(Expr_ctor_ESet(set_items));
        } else {
#line 2466 "src/parser.tr"
            return box_expr(Expr_ctor_EDict(keys, vals));
        }
    } else if (_t295.tag == Token_KwTry) {
#line 2468 "src/parser.tr"
        Stmt* st = Parser_parse_try_stmt(self);
#line 2469 "src/parser.tr"
        __auto_type _t337 = (*st);
        if (_t337.tag == Stmt_STry) {
            __auto_type try_body = _t337.data.STry.try_body;
__auto_type catches = _t337.data.STry.catches;
__auto_type finally_b = _t337.data.STry.finally_b;
#line 2471 "src/parser.tr"
            Expr* _trret61 = box_expr(Expr_ctor_ETry(try_body, catches, finally_b));
            return _trret61;
        } else if (1) {
            __auto_type _ = _t337;
#line 2473 "src/parser.tr"
            /* pass */
        }
    } else if (_t295.tag == Token_KwAwait) {
#line 2475 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2476 "src/parser.tr"
        return box_expr(Expr_ctor_EAwait(Parser_parse_expr(self)));
    } else if (_t295.tag == Token_KwYield) {
#line 2478 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2479 "src/parser.tr"
        return box_expr(Expr_ctor_EYield(Parser_parse_expr(self)));
    } else if ((_t295.tag == Token_KwDef || _t295.tag == Token_KwAsync)) {
#line 2481 "src/parser.tr"
        bool is_async = false;
#line 2482 "src/parser.tr"
        if (({ __auto_type _ord_t338 = (Parser_peek(self)); __auto_type _wr = ((_ord_t338.tag == Token_make_KwAsync().tag)); _wr; })) {
#line 2483 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2484 "src/parser.tr"
            is_async = true;
        }
#line 2485 "src/parser.tr"
        __auto_type _t339 = Parser_peek(self);
        if (_t339.tag == Token_KwDef) {
#line 2487 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t339;
#line 2489 "src/parser.tr"
            /* pass */
        }
#line 2490 "src/parser.tr"
        __auto_type _t340 = Parser_peek(self);
        if (_t340.tag == Token_LParen) {
#line 2492 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t340;
#line 2494 "src/parser.tr"
            /* pass */
        }
#line 2495 "src/parser.tr"
        List_ptr* params = Parser_parse_param_list(self);
#line 2496 "src/parser.tr"
        Parser_close_params(self);
#line 2497 "src/parser.tr"
        AstType** return_type = (AstType**)(0LL);
#line 2498 "src/parser.tr"
        AstType** clo_throws = (AstType**)(0LL);
#line 2499 "src/parser.tr"
        if (({ __auto_type _ord_t341 = (Parser_peek(self)); __auto_type _wr = ((_ord_t341.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 2500 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2501 "src/parser.tr"
            clo_throws = box_asttype(Parser_parse_type(self));
        }
#line 2502 "src/parser.tr"
        if (({ __auto_type _ord_t342 = (Parser_peek(self)); __auto_type _wr = ((_ord_t342.tag == Token_make_Arrow().tag)); _wr; })) {
#line 2503 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2504 "src/parser.tr"
            return_type = box_asttype(Parser_parse_type(self));
        }
#line 2505 "src/parser.tr"
        if ((((unsigned long long)(clo_throws)) != ((unsigned long long)(0LL)))) {
#line 2506 "src/parser.tr"
            AstType* clo_rt = AstType_init(_tr_str_lit_len("void", 4LL));
#line 2507 "src/parser.tr"
            if ((((unsigned long long)(return_type)) != ((unsigned long long)(0LL)))) {
#line 2507 "src/parser.tr"
                clo_rt = (*return_type);
            }
#line 2508 "src/parser.tr"
            return_type = box_asttype(throws_result_type(clo_rt, clo_throws));
        }
#line 2509 "src/parser.tr"
        __auto_type _t343 = Parser_peek(self);
        if (_t343.tag == Token_Colon) {
#line 2511 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t343;
#line 2513 "src/parser.tr"
            /* pass */
        }
#line 2514 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 2515 "src/parser.tr"
        Expr* _trret62 = box_expr(Expr_ctor_EClosure(params, return_type, body, is_async));
        return _trret62;
    } else if (_t295.tag == Token_KwLambda) {
#line 2519 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2520 "src/parser.tr"
        List_ptr* lparams = (void*)List_ptr_new();
#line 2521 "src/parser.tr"
        while (((({ __auto_type _ord_t344 = (Parser_peek(self)); __auto_type _wr = ((_ord_t344.tag != Token_make_Colon().tag)); _wr; }) && ({ __auto_type _ord_t345 = (Parser_peek(self)); __auto_type _wr = ((_ord_t345.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t346 = (Parser_peek(self)); __auto_type _wr = ((_ord_t346.tag != Token_make_Newline().tag)); _wr; }))) {
#line 2522 "src/parser.tr"
            TrStr lpn = Parser_consume_ident(self);
#line 2523 "src/parser.tr"
            if (_tr_str_eqv((lpn), (_tr_str_lit_len("", 0LL)))) {
#line 2523 "src/parser.tr"
                _tr_str_release(lpn);
                break;
            }
#line 2524 "src/parser.tr"
            List_ptr_append(lparams, Param_init(lpn, (AstType**)(0LL)));
#line 2525 "src/parser.tr"
            if (({ __auto_type _ord_t347 = (Parser_peek(self)); __auto_type _wr = ((_ord_t347.tag == Token_make_Comma().tag)); _wr; })) {
#line 2526 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(lpn);
        }
#line 2527 "src/parser.tr"
        __auto_type _t348 = Parser_peek(self);
        if (_t348.tag == Token_Colon) {
#line 2529 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t348;
#line 2531 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected ':' after the parameters of a 'lambda'", 47LL), _tr_str_lit_len("write `lambda x, y: expr`", 25LL));
        }
#line 2532 "src/parser.tr"
        Expr* lbody_e = Parser_parse_expr(self);
#line 2533 "src/parser.tr"
        Block* lbody = Block_init();
#line 2534 "src/parser.tr"
        Block_push(lbody, box_stmt(Stmt_ctor_SReturn(lbody_e)));
#line 2535 "src/parser.tr"
        Expr* _trret63 = box_expr(Expr_ctor_EClosure(lparams, (AstType**)(0LL), lbody, false));
        return _trret63;
    } else if (_t295.tag == Token_FStrLit) {
        __auto_type raw = _t295.data.FStrLit.val;
__auto_type raw_blen = _t295.data.FStrLit.blen;
#line 2537 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2538 "src/parser.tr"
        Expr* _trret64 = Parser_parse_fstring(self, raw, raw_blen);
        return _trret64;
    } else if (1) {
        __auto_type _ = _t295;
#line 2540 "src/parser.tr"
        /* pass */
    }
#line 2541 "src/parser.tr"
    if ((!Parser_at_end(self))) {
#line 2542 "src/parser.tr"
        TrStr tok_desc = _tr_str_lit_len("token", 5LL);
#line 2543 "src/parser.tr"
        TrStr hint = _tr_str_lit_len("check for a typo, a missing ':' or unbalanced parentheses/brackets near this point.", 83LL);
#line 2544 "src/parser.tr"
        __auto_type _t349 = Parser_peek(self);
        if (_t349.tag == Token_Newline) {
#line 2546 "src/parser.tr"
            TrStr _strtmp_t350 = _tr_str_lit_len("end of line", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t350;
#line 2547 "src/parser.tr"
            TrStr _strtmp_t351 = _tr_str_lit_len("an expression was expected before the end of this line - check for a missing value or trailing operator.", 104LL);
            _tr_str_release(hint);
            hint = _strtmp_t351;
        } else if (_t349.tag == Token_Indent) {
#line 2549 "src/parser.tr"
            TrStr _strtmp_t352 = _tr_str_lit_len("indentation", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t352;
#line 2550 "src/parser.tr"
            TrStr _strtmp_t353 = _tr_str_lit_len("check that this line's indentation matches the surrounding block.", 65LL);
            _tr_str_release(hint);
            hint = _strtmp_t353;
        } else if (_t349.tag == Token_Dedent) {
#line 2552 "src/parser.tr"
            TrStr _strtmp_t354 = _tr_str_lit_len("dedent", 6LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t354;
#line 2553 "src/parser.tr"
            TrStr _strtmp_t355 = _tr_str_lit_len("check that this block is properly indented and closed.", 54LL);
            _tr_str_release(hint);
            hint = _strtmp_t355;
        } else if (_t349.tag == Token_Ident) {
            __auto_type n = _t349.data.Ident.name;
#line 2555 "src/parser.tr"
            TrStr _strtmp_t356 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("identifier '", 12LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t356;
#line 2556 "src/parser.tr"
            TrStr _strtmp_t357 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("an operator, ':' or end of statement was expected before '", 58LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(hint);
            hint = _strtmp_t357;
        } else if (_t349.tag == Token_KwMut) {
            TrStr _strtmp_t358 = _tr_str_lit_len("keyword 'mut'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t358;
        } else if (_t349.tag == Token_KwConst) {
            TrStr _strtmp_t359 = _tr_str_lit_len("keyword 'const'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t359;
        } else if (_t349.tag == Token_KwPub) {
            TrStr _strtmp_t360 = _tr_str_lit_len("keyword 'pub'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t360;
        } else if (_t349.tag == Token_KwReturn) {
            TrStr _strtmp_t361 = _tr_str_lit_len("keyword 'return'", 16LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t361;
        } else if (_t349.tag == Token_KwIf) {
            TrStr _strtmp_t362 = _tr_str_lit_len("keyword 'if'", 12LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t362;
        } else if (_t349.tag == Token_KwWhile) {
            TrStr _strtmp_t363 = _tr_str_lit_len("keyword 'while'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t363;
        } else if (_t349.tag == Token_Comma) {
#line 2564 "src/parser.tr"
            TrStr _strtmp_t364 = _tr_str_lit_len("','", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t364;
#line 2565 "src/parser.tr"
            TrStr _strtmp_t365 = _tr_str_lit_len("remove the extra ',' or add the missing item before it.", 55LL);
            _tr_str_release(hint);
            hint = _strtmp_t365;
        } else if (_t349.tag == Token_Colon) {
#line 2567 "src/parser.tr"
            TrStr _strtmp_t366 = _tr_str_lit_len("':'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t366;
#line 2568 "src/parser.tr"
            TrStr _strtmp_t367 = _tr_str_lit_len("remove the extra ':' or check the statement before it is complete.", 66LL);
            _tr_str_release(hint);
            hint = _strtmp_t367;
        } else if (_t349.tag == Token_RParen) {
#line 2570 "src/parser.tr"
            TrStr _strtmp_t368 = _tr_str_lit_len("')'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t368;
#line 2571 "src/parser.tr"
            TrStr _strtmp_t369 = _tr_str_lit_len("check for an extra ')' or a missing matching '('.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t369;
        } else if (_t349.tag == Token_RBracket) {
#line 2573 "src/parser.tr"
            TrStr _strtmp_t370 = _tr_str_lit_len("']'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t370;
#line 2574 "src/parser.tr"
            TrStr _strtmp_t371 = _tr_str_lit_len("check for an extra ']' or a missing matching '['.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t371;
        } else if (_t349.tag == Token_RBrace) {
#line 2576 "src/parser.tr"
            TrStr _strtmp_t372 = _tr_str_lit_len("'}'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t372;
#line 2577 "src/parser.tr"
            TrStr _strtmp_t373 = _tr_str_lit_len("check for an extra '}' or a missing matching '{'.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t373;
        } else if (_t349.tag == Token_Error) {
            __auto_type em = _t349.data.Error.msg;
#line 2581 "src/parser.tr"
            Parser_emit_diag(self, em, _tr_str_lit_len("", 0LL));
#line 2582 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2583 "src/parser.tr"
            Expr* _trret65 = box_expr(Expr_make_ELitNone());
            return _trret65;
        } else if (1) {
            __auto_type _ = _t349;
            /* pass */
        }
#line 2585 "src/parser.tr"
        if (_tr_str_eqv((tok_desc), (_tr_str_lit_len("token", 5LL)))) {
#line 2588 "src/parser.tr"
            TrStr kw = _tr_str_lit_len("", 0LL);
#line 2589 "src/parser.tr"
            long long kln = Parser_cur_line(self);
#line 2590 "src/parser.tr"
            long long kcol = Parser_cur_col(self);
#line 2591 "src/parser.tr"
            if ((((_tr_str_lenv((self->src_text)) > 0LL) && (kln > 0LL)) && (kcol > 0LL))) {
#line 2592 "src/parser.tr"
                TrStr ksrc = _nth_source_line(self->src_text, kln);
#line 2593 "src/parser.tr"
                long long kend = (kcol - 1LL);
#line 2594 "src/parser.tr"
                while (((kend < _tr_str_lenv((ksrc))) && (((_tr_str_char_at_codev((ksrc), kend) >= 97LL) && (_tr_str_char_at_codev((ksrc), kend) <= 122LL)) || (_tr_str_char_at_codev((ksrc), kend) == 95LL)))) {
#line 2595 "src/parser.tr"
                    kend = (kend + 1LL);
                }
#line 2596 "src/parser.tr"
                if ((kend > (kcol - 1LL))) {
#line 2596 "src/parser.tr"
                    TrStr _strtmp_t374 = _tr_str_slicev((ksrc), (kcol - 1LL), kend);
                    _tr_str_release(kw);
                    kw = _strtmp_t374;
                }
            }
#line 2597 "src/parser.tr"
            bool kw_ok = ((_tr_str_lenv((kw)) > 1LL) && (_tr_str_lenv((kw)) < 16LL));
#line 2598 "src/parser.tr"
            long long ki = 0LL;
#line 2599 "src/parser.tr"
            while ((kw_ok && (ki < _tr_str_lenv((kw))))) {
#line 2600 "src/parser.tr"
                long long kc = _tr_str_char_at_codev((kw), ki);
#line 2601 "src/parser.tr"
                if ((!(((kc >= 97LL) && (kc <= 122LL)) || (kc == 95LL)))) {
#line 2601 "src/parser.tr"
                    kw_ok = false;
                }
#line 2602 "src/parser.tr"
                ki = (ki + 1LL);
            }
#line 2603 "src/parser.tr"
            if (kw_ok) {
#line 2604 "src/parser.tr"
                TrStr _strtmp_t375 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("keyword '", 9LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(tok_desc);
                tok_desc = _strtmp_t375;
#line 2605 "src/parser.tr"
                TrStr _strtmp_t376 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a reserved word and cannot be used as a name or value here - rename it (for example '", 90LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kw)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_').", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(hint);
                hint = _strtmp_t376;
            }
        }
#line 2606 "src/parser.tr"
        ({ TrStr _at_t377 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unexpected ", 11LL)), (tok_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in expression", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t377, hint); _tr_str_release(_at_t377); });
#line 2607 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 2608 "src/parser.tr"
    return box_expr(Expr_make_ELitNone());
}

__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, TrStr raw, long long raw_blen) {
#line 2630 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 2631 "src/parser.tr"
    char* p = ((char*)(_tr_strz(raw)));
#line 2632 "src/parser.tr"
    long long i = 0LL;
#line 2633 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 2635 "src/parser.tr"
    while ((i < raw_blen)) {
#line 2636 "src/parser.tr"
        long long c = ((long long)((*(p + i))));
#line 2637 "src/parser.tr"
        long long c_next = 0LL;
#line 2638 "src/parser.tr"
        if (((i + 1LL) < raw_blen)) {
#line 2638 "src/parser.tr"
            c_next = ((long long)((*(p + (i + 1LL)))));
        }
#line 2640 "src/parser.tr"
        if ((((c == 123LL) && (c_next == 123LL)) || ((c == 125LL) && (c_next == 125LL)))) {
#line 2642 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2643 "src/parser.tr"
            i = (i + 2LL);
        } else if ((c == 123LL)) {
#line 2646 "src/parser.tr"
            if ((sb->buf->len > 0LL)) {
#line 2647 "src/parser.tr"
                ({ TrStr _ord_t378 = (StringBuilder_to_owned(sb)); __auto_type _ord_t379 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t378, _ord_t379)); _tr_str_release(_ord_t378); });
#line 2648 "src/parser.tr"
                sb = StringBuilder_init(64LL);
            }
#line 2650 "src/parser.tr"
            i = (i + 1LL);
#line 2651 "src/parser.tr"
            StringBuilder* expr_sb = StringBuilder_init(64LL);
#line 2652 "src/parser.tr"
            long long depth = 1LL;
#line 2653 "src/parser.tr"
            while (((depth > 0LL) && (i < raw_blen))) {
#line 2654 "src/parser.tr"
                long long ec = ((long long)((*(p + i))));
#line 2655 "src/parser.tr"
                if (((ec == 34LL) || (ec == 39LL))) {
#line 2658 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
#line 2659 "src/parser.tr"
                    i = (i + 1LL);
#line 2660 "src/parser.tr"
                    while (((i < raw_blen) && (((long long)((*(p + i)))) != ec))) {
#line 2661 "src/parser.tr"
                        long long sc = ((long long)((*(p + i))));
#line 2662 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, sc);
#line 2663 "src/parser.tr"
                        i = (i + 1LL);
#line 2664 "src/parser.tr"
                        if (((sc == 92LL) && (i < raw_blen))) {
#line 2665 "src/parser.tr"
                            StringBuilder_append_char(expr_sb, ((long long)((*(p + i)))));
#line 2666 "src/parser.tr"
                            i = (i + 1LL);
                        }
                    }
#line 2667 "src/parser.tr"
                    if ((i < raw_blen)) {
#line 2668 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, ec);
#line 2669 "src/parser.tr"
                        i = (i + 1LL);
                    }
#line 2670 "src/parser.tr"
                    continue;
                }
#line 2671 "src/parser.tr"
                if ((ec == 123LL)) {
#line 2671 "src/parser.tr"
                    depth = (depth + 1LL);
                }
#line 2672 "src/parser.tr"
                if ((ec == 125LL)) {
#line 2672 "src/parser.tr"
                    depth = (depth - 1LL);
                }
#line 2673 "src/parser.tr"
                if ((depth > 0LL)) {
#line 2673 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
                }
#line 2674 "src/parser.tr"
                i = (i + 1LL);
            }
#line 2678 "src/parser.tr"
            TrStr expr_str = StringObj_as_str(StringBuilder_to_string(expr_sb));
#line 2679 "src/parser.tr"
            TrStr fmt_spec = _tr_str_lit_len("", 0LL);
#line 2680 "src/parser.tr"
            long long colon_pos = _find_fmt_colon(expr_str);
#line 2681 "src/parser.tr"
            if ((colon_pos >= 0LL)) {
#line 2682 "src/parser.tr"
                TrStr _fs = _tr_str_slicev((expr_str), (colon_pos + 1LL), _tr_str_lenv((expr_str)));
#line 2683 "src/parser.tr"
                TrStr _strtmp_t380 = _tr_str_stripv((_fs));
                _tr_str_release(fmt_spec);
                fmt_spec = _strtmp_t380;
#line 2684 "src/parser.tr"
                TrStr _es = _tr_str_slicev((expr_str), 0LL, colon_pos);
#line 2685 "src/parser.tr"
                TrStr _strtmp_t381 = _tr_str_stripv((_es));
                _tr_str_release(expr_str);
                expr_str = _strtmp_t381;
                _tr_str_release(_fs);
                _tr_str_release(_es);
            }
#line 2686 "src/parser.tr"
            Lexer* lexer = Lexer_init(expr_str);
#line 2687 "src/parser.tr"
            List_Token* _fstr_tokens = Lexer_tokenize(lexer);
#line 2688 "src/parser.tr"
            if ((_fstr_tokens->len > 0LL)) {
#line 2689 "src/parser.tr"
                Parser* parser = Parser_init(_fstr_tokens, lexer->token_lines);
#line 2690 "src/parser.tr"
                Expr* e = Parser_parse_expr(parser);
#line 2691 "src/parser.tr"
                if ((_tr_str_lenv((fmt_spec)) > 0LL)) {
#line 2692 "src/parser.tr"
                    FStringPart* fpart = FStringPart_init_expr_fmt(e, fmt_spec);
#line 2694 "src/parser.tr"
                    long long sk = 0LL;
#line 2695 "src/parser.tr"
                    while ((sk < _tr_str_lenv((fmt_spec)))) {
#line 2696 "src/parser.tr"
                        if ((_tr_str_char_at_codev((fmt_spec), sk) == 123LL)) {
#line 2697 "src/parser.tr"
                            long long se = (sk + 1LL);
#line 2698 "src/parser.tr"
                            while (((se < _tr_str_lenv((fmt_spec))) && (_tr_str_char_at_codev((fmt_spec), se) != 125LL))) {
#line 2698 "src/parser.tr"
                                se = (se + 1LL);
                            }
#line 2699 "src/parser.tr"
                            if ((se >= _tr_str_lenv((fmt_spec)))) {
#line 2699 "src/parser.tr"
                                break;
                            }
#line 2700 "src/parser.tr"
                            TrStr sub_raw = _tr_str_slicev((fmt_spec), (sk + 1LL), se);
#line 2701 "src/parser.tr"
                            TrStr sub_src = _tr_str_stripv((sub_raw));
#line 2702 "src/parser.tr"
                            Lexer* slex = Lexer_init(sub_src);
#line 2703 "src/parser.tr"
                            List_Token* stoks = Lexer_tokenize(slex);
#line 2704 "src/parser.tr"
                            if ((stoks->len > 0LL)) {
#line 2705 "src/parser.tr"
                                Parser* sparser = Parser_init(stoks, slex->token_lines);
#line 2706 "src/parser.tr"
                                ({ __auto_type _ord_t382 = (Parser_parse_expr(sparser)); List_ptr_append(fpart->spec_args, _ord_t382); });
                                _tr_obj_release(sparser, _trdrop_Parser);
                            }
#line 2707 "src/parser.tr"
                            sk = (se + 1LL);
                            _tr_str_release(sub_raw);
                            _tr_str_release(sub_src);
                            _tr_obj_release(slex, _trdrop_Lexer);
                        } else {
#line 2709 "src/parser.tr"
                            sk = (sk + 1LL);
                        }
                    }
#line 2710 "src/parser.tr"
                    List_ptr_append(fl, _tr_obj_retain(fpart));
                } else {
#line 2712 "src/parser.tr"
                    List_ptr_append(fl, FStringPart_init_expr(e));
                }
                _tr_obj_release(parser, _trdrop_Parser);
            }
            _tr_str_release(expr_str);
            StringBuilder__tr_fn_free(expr_sb);
            _tr_obj_release(lexer, _trdrop_Lexer);
        } else if (((c == 92LL) && ((i + 1LL) < raw_blen))) {
#line 2714 "src/parser.tr"
            long long esc = ((long long)((*(p + (i + 1LL)))));
#line 2715 "src/parser.tr"
            if ((esc == 110LL)) {
#line 2715 "src/parser.tr"
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
#line 2716 "src/parser.tr"
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
#line 2717 "src/parser.tr"
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
#line 2718 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
#line 2719 "src/parser.tr"
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
#line 2720 "src/parser.tr"
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
#line 2721 "src/parser.tr"
                StringBuilder_append_char(sb, 0LL);
            } else if (((((esc == 120LL) || (esc == 117LL)) || (esc == 85LL)) && _fstr_hex_ok(p, (i + 2LL), raw_blen, esc))) {
#line 2724 "src/parser.tr"
                long long fnd = 2LL;
#line 2725 "src/parser.tr"
                if ((esc == 117LL)) {
#line 2725 "src/parser.tr"
                    fnd = 4LL;
                }
#line 2726 "src/parser.tr"
                if ((esc == 85LL)) {
#line 2726 "src/parser.tr"
                    fnd = 8LL;
                }
#line 2727 "src/parser.tr"
                long long fhv = 0LL;
#line 2728 "src/parser.tr"
                long long fk = 0LL;
#line 2729 "src/parser.tr"
                while ((fk < fnd)) {
#line 2730 "src/parser.tr"
                    fhv = ((fhv * 16LL) + lex_hex_val(((long long)((*(p + ((i + 2LL) + fk)))))));
#line 2731 "src/parser.tr"
                    fk = (fk + 1LL);
                }
#line 2732 "src/parser.tr"
                if ((esc == 120LL)) {
#line 2732 "src/parser.tr"
                    StringBuilder_append_char(sb, fhv);
                } else {
#line 2733 "src/parser.tr"
                    lex_append_utf8(sb, fhv);
                }
#line 2734 "src/parser.tr"
                i = (i + fnd);
            } else {
#line 2738 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
#line 2739 "src/parser.tr"
                StringBuilder_append_char(sb, esc);
            }
#line 2740 "src/parser.tr"
            i = (i + 2LL);
        } else {
#line 2742 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2743 "src/parser.tr"
            i = (i + 1LL);
        }
    }
#line 2745 "src/parser.tr"
    if ((sb->buf->len > 0LL)) {
#line 2746 "src/parser.tr"
        ({ TrStr _ord_t383 = (StringBuilder_to_owned(sb)); __auto_type _ord_t384 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t383, _ord_t384)); _tr_str_release(_ord_t383); });
    }
#line 2748 "src/parser.tr"
    StringBuilder__tr_fn_free(sb);
    return box_expr(Expr_ctor_EFString(fl));
}

__attribute__((hot)) long long Parser_nesting_too_deep(Parser* self) {
#line 2754 "src/parser.tr"
    long long depth = 0LL;
#line 2755 "src/parser.tr"
    long long indent = 0LL;
#line 2756 "src/parser.tr"
    long long run = 0LL;
#line 2757 "src/parser.tr"
    long long lam = 0LL;
#line 2758 "src/parser.tr"
    bool prev_colon = false;
#line 2759 "src/parser.tr"
    long long i = 0LL;
#line 2760 "src/parser.tr"
    while ((i < self->tokens->len)) {
#line 2761 "src/parser.tr"
        bool is_prefix = false;
#line 2762 "src/parser.tr"
        __auto_type _t385 = List_Token_get(self->tokens, i);
        if (_t385.tag == Token_KwLambda) {
#line 2764 "src/parser.tr"
            if (prev_colon) {
#line 2764 "src/parser.tr"
                lam = (lam + 1LL);
            } else {
#line 2765 "src/parser.tr"
                lam = 0LL;
            }
        } else if (_t385.tag == Token_Colon) {
            /* pass */
        } else if ((_t385.tag == Token_Newline || _t385.tag == Token_Comma || _t385.tag == Token_RParen || _t385.tag == Token_RBracket || _t385.tag == Token_RBrace)) {
            lam = 0LL;
        } else if (1) {
            __auto_type _ = _t385;
            /* pass */
        }
#line 2769 "src/parser.tr"
        prev_colon = ({ __auto_type _ord_t386 = (List_Token_get(self->tokens, i)); __auto_type _wr = ((_ord_t386.tag == Token_make_Colon().tag)); _wr; });
#line 2770 "src/parser.tr"
        __auto_type _t387 = List_Token_get(self->tokens, i);
        if ((_t387.tag == Token_LParen || _t387.tag == Token_LBracket || _t387.tag == Token_LBrace)) {
#line 2772 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((_t387.tag == Token_RParen || _t387.tag == Token_RBracket || _t387.tag == Token_RBrace)) {
#line 2774 "src/parser.tr"
            if ((depth > 0LL)) {
#line 2774 "src/parser.tr"
                depth = (depth - 1LL);
            }
        } else if (_t387.tag == Token_Indent) {
#line 2776 "src/parser.tr"
            indent = (indent + 1LL);
        } else if (_t387.tag == Token_Dedent) {
#line 2778 "src/parser.tr"
            if ((indent > 0LL)) {
#line 2778 "src/parser.tr"
                indent = (indent - 1LL);
            }
        } else if ((_t387.tag == Token_Minus || _t387.tag == Token_Tilde || _t387.tag == Token_KwNot || _t387.tag == Token_KwAwait || _t387.tag == Token_Bang)) {
#line 2780 "src/parser.tr"
            is_prefix = true;
        } else if (1) {
            __auto_type _ = _t387;
#line 2782 "src/parser.tr"
            /* pass */
        }
#line 2783 "src/parser.tr"
        if (is_prefix) {
#line 2783 "src/parser.tr"
            run = (run + 1LL);
        } else {
#line 2784 "src/parser.tr"
            run = 0LL;
        }
#line 2785 "src/parser.tr"
        if (((((depth > 120LL) || (indent > 100LL)) || (run > 120LL)) || (lam > 100LL))) {
#line 2785 "src/parser.tr"
            return i;
        }
#line 2786 "src/parser.tr"
        i = (i + 1LL);
    }
#line 2787 "src/parser.tr"
    return (-1LL);
}

__attribute__((hot)) Program* Parser_parse_program(Parser* self) {
#line 2790 "src/parser.tr"
    Program* prog = Program_init();
#line 2791 "src/parser.tr"
    long long deep_at = Parser_nesting_too_deep(self);
#line 2792 "src/parser.tr"
    if ((deep_at >= 0LL)) {
#line 2793 "src/parser.tr"
        self->pos = deep_at;
#line 2794 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("this construct is nested too deeply", 35LL), _tr_str_lit_len("the compiler supports at most 120 nested brackets, 100 nested blocks or 120 chained prefix operators - split the expression or move the inner part into a helper function.", 170LL));
#line 2795 "src/parser.tr"
        self->pos = self->tokens->len;
#line 2796 "src/parser.tr"
        return prog;
    }
#line 2797 "src/parser.tr"
    Parser_skip_newlines_and_indent(self);
#line 2798 "src/parser.tr"
    while ((!Parser_at_end(self))) {
#line 2803 "src/parser.tr"
        bool sk = true;
#line 2804 "src/parser.tr"
        while (sk) {
#line 2805 "src/parser.tr"
            sk = false;
#line 2806 "src/parser.tr"
            __auto_type _t388 = Parser_peek(self);
            if ((_t388.tag == Token_Newline || _t388.tag == Token_Dedent)) {
#line 2808 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2809 "src/parser.tr"
                sk = true;
            } else if (_t388.tag == Token_Indent) {
#line 2811 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("unexpected indentation", 22LL), _tr_str_lit_len("this line is indented but no block was opened above it (a statement ending in ':' and its own line) — remove the extra indentation.", 133LL));
#line 2812 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2813 "src/parser.tr"
                sk = true;
            } else if (1) {
                __auto_type _ = _t388;
#line 2815 "src/parser.tr"
                /* pass */
            }
        }
#line 2816 "src/parser.tr"
        if ((!Parser_at_end(self))) {
#line 2817 "src/parser.tr"
            Program_push(prog, Parser_parse_decl(self));
        }
    }
#line 2818 "src/parser.tr"
    return prog;
}

__attribute__((hot)) Decl* Parser_parse_decl(Parser* self) {
#line 2821 "src/parser.tr"
    List_ptr* decorators = (void*)List_ptr_new();
#line 2822 "src/parser.tr"
    while (({ __auto_type _ord_t389 = (Parser_peek(self)); __auto_type _wr = ((_ord_t389.tag == Token_make_At().tag)); _wr; })) {
#line 2823 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2824 "src/parser.tr"
        TrStr name = Parser_consume_ident(self);
#line 2825 "src/parser.tr"
        Decorator* d = Decorator_init(name);
#line 2826 "src/parser.tr"
        if (({ __auto_type _ord_t390 = (Parser_peek(self)); __auto_type _wr = ((_ord_t390.tag == Token_make_LParen().tag)); _wr; })) {
#line 2827 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2828 "src/parser.tr"
            d->args = Parser_parse_arg_list(self);
        }
#line 2829 "src/parser.tr"
        List_ptr_append(decorators, _tr_obj_retain(d));
#line 2830 "src/parser.tr"
        Parser_expect_newline(self);
        _tr_str_release(name);
        _tr_obj_release(d, _trdrop_Decorator);
    }
#line 2832 "src/parser.tr"
    bool is_public = false;
#line 2833 "src/parser.tr"
    bool is_export = false;
#line 2836 "src/parser.tr"
    bool _scan_mods = true;
#line 2837 "src/parser.tr"
    while (_scan_mods) {
#line 2838 "src/parser.tr"
        _scan_mods = false;
#line 2839 "src/parser.tr"
        __auto_type _t391 = Parser_peek(self);
        if (_t391.tag == Token_KwPub) {
#line 2841 "src/parser.tr"
            is_public = true;
#line 2842 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2843 "src/parser.tr"
            _scan_mods = true;
        } else if (_t391.tag == Token_KwExport) {
#line 2845 "src/parser.tr"
            is_export = true;
#line 2846 "src/parser.tr"
            is_public = true;
#line 2847 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2848 "src/parser.tr"
            _scan_mods = true;
        } else if (1) {
            __auto_type _ = _t391;
#line 2850 "src/parser.tr"
            /* pass */
        }
    }
#line 2851 "src/parser.tr"
    __auto_type _t392 = Parser_peek(self);
    if (_t392.tag == Token_KwFrom) {
#line 2853 "src/parser.tr"
        Decl* _trret66 = Parser_parse_from_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret66;
    } else if (_t392.tag == Token_KwImport) {
#line 2855 "src/parser.tr"
        Decl* _trret67 = Parser_parse_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret67;
    } else if (_t392.tag == Token_Ident) {
        __auto_type type_kw = _t392.data.Ident.name;
#line 2858 "src/parser.tr"
        if (_tr_str_eqv((type_kw), (_tr_str_lit_len("type", 4LL)))) {
#line 2859 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2860 "src/parser.tr"
            TrStr alias_name = Parser_consume_ident(self);
#line 2861 "src/parser.tr"
            if (({ __auto_type _ord_t393 = (Parser_peek(self)); __auto_type _wr = ((_ord_t393.tag == Token_make_Eq().tag)); _wr; })) {
#line 2862 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2863 "src/parser.tr"
                AstType* target_ty = Parser_parse_type(self);
#line 2864 "src/parser.tr"
                Parser_expect_newline(self);
#line 2865 "src/parser.tr"
                Decl* _trret68 = box_decl(Decl_ctor_DTypeAlias(alias_name, box_asttype(target_ty)));
                _tr_str_release(alias_name);
                List_ptr_free_obj(decorators, _trdrop_Decorator);
                return _trret68;
            }
        }
    } else if (_t392.tag == Token_KwDef) {
#line 2867 "src/parser.tr"
        long long _fline = Parser_cur_line(self);
#line 2868 "src/parser.tr"
        FunctionDef* f = Parser_parse_function_def(self, false);
#line 2869 "src/parser.tr"
        f->line = _fline;
#line 2870 "src/parser.tr"
        f->is_public = is_public;
#line 2871 "src/parser.tr"
        f->is_export = is_export;
#line 2872 "src/parser.tr"
        f->decorators = decorators;
#line 2873 "src/parser.tr"
        Decl* _trret69 = box_decl(Decl_ctor_DFunction(f));
        _tr_obj_release(f, _trdrop_FunctionDef);
        return _trret69;
    } else if (_t392.tag == Token_KwClass) {
#line 2875 "src/parser.tr"
        long long _cline = Parser_cur_line(self);
#line 2876 "src/parser.tr"
        Decl* c_ptr = Parser_parse_class_decl(self);
#line 2877 "src/parser.tr"
        __auto_type _t394 = (*c_ptr);
        if (_t394.tag == Decl_DClass) {
            __auto_type c = _t394.data.DClass.cls;
#line 2879 "src/parser.tr"
            c->line = _cline;
#line 2880 "src/parser.tr"
            c->is_public = is_public;
#line 2887 "src/parser.tr"
            bool _is_vt = false;
#line 2888 "src/parser.tr"
            long long _dvi = 0LL;
#line 2889 "src/parser.tr"
            while ((_dvi < decorators->len)) {
#line 2890 "src/parser.tr"
                TrStr _dvn = _tr_str_retain(((Decorator*)List_ptr_get(decorators, _dvi))->name);
#line 2891 "src/parser.tr"
                if ((((_tr_str_eqv((_dvn), (_tr_str_lit_len("value_type", 10LL))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("packed", 6LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("aligned", 7LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("union", 5LL))))) {
#line 2891 "src/parser.tr"
                    _is_vt = true;
                }
#line 2892 "src/parser.tr"
                _dvi = (_dvi + 1LL);
                _tr_str_release(_dvn);
            }
#line 2893 "src/parser.tr"
            c->is_class = (!_is_vt);
#line 2894 "src/parser.tr"
            c->decorators = decorators;
#line 2895 "src/parser.tr"
            /* unsafe block */
#line 2895 "src/parser.tr"
            (*c_ptr = Decl_ctor_DClass(c));
        } else if (1) {
            __auto_type _ = _t394;
#line 2897 "src/parser.tr"
            /* pass */
        }
#line 2898 "src/parser.tr"
        return c_ptr;
    } else if (_t392.tag == Token_KwEnum) {
#line 2900 "src/parser.tr"
        long long _eline = Parser_cur_line(self);
#line 2901 "src/parser.tr"
        Decl* e_ptr = Parser_parse_enum_decl(self);
#line 2902 "src/parser.tr"
        __auto_type _t395 = (*e_ptr);
        if (_t395.tag == Decl_DEnum) {
            __auto_type e = _t395.data.DEnum.enm;
#line 2904 "src/parser.tr"
            e->line = _eline;
#line 2905 "src/parser.tr"
            e->is_public = is_public;
#line 2906 "src/parser.tr"
            e->decorators = decorators;
#line 2907 "src/parser.tr"
            /* unsafe block */
#line 2907 "src/parser.tr"
            (*e_ptr = Decl_ctor_DEnum(e));
        } else if (1) {
            __auto_type _ = _t395;
#line 2909 "src/parser.tr"
            /* pass */
        }
#line 2910 "src/parser.tr"
        return e_ptr;
    } else if (_t392.tag == Token_KwInterface) {
#line 2912 "src/parser.tr"
        long long _iline = Parser_cur_line(self);
#line 2913 "src/parser.tr"
        Decl* i_ptr = Parser_parse_interface_decl(self);
#line 2914 "src/parser.tr"
        __auto_type _t396 = (*i_ptr);
        if (_t396.tag == Decl_DInterface) {
            __auto_type i = _t396.data.DInterface.iface;
#line 2916 "src/parser.tr"
            i->line = _iline;
#line 2917 "src/parser.tr"
            i->is_public = is_public;
#line 2918 "src/parser.tr"
            i->decorators = decorators;
#line 2919 "src/parser.tr"
            /* unsafe block */
#line 2919 "src/parser.tr"
            (*i_ptr = Decl_ctor_DInterface(i));
        } else if (1) {
            __auto_type _ = _t396;
#line 2921 "src/parser.tr"
            /* pass */
        }
#line 2922 "src/parser.tr"
        return i_ptr;
    } else if (_t392.tag == Token_KwExtend) {
#line 2924 "src/parser.tr"
        return Parser_parse_extend_decl(self);
    } else if (_t392.tag == Token_KwDecorator) {
#line 2926 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2927 "src/parser.tr"
        long long _dl = Parser_cur_line(self);
#line 2928 "src/parser.tr"
        FunctionDef* _df = Parser_parse_function_def(self, false);
#line 2929 "src/parser.tr"
        _df->line = _dl;
#line 2930 "src/parser.tr"
        _df->is_public = is_public;
#line 2931 "src/parser.tr"
        Decl* _trret70 = box_decl(Decl_ctor_DDecoratorDef(_df));
        _tr_obj_release(_df, _trdrop_FunctionDef);
        return _trret70;
    } else if (_t392.tag == Token_KwMacro) {
#line 2936 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2937 "src/parser.tr"
        long long _ml = Parser_cur_line(self);
#line 2938 "src/parser.tr"
        FunctionDef* _mf = Parser_parse_function_def(self, false);
#line 2939 "src/parser.tr"
        _mf->line = _ml;
#line 2940 "src/parser.tr"
        _mf->is_public = is_public;
#line 2941 "src/parser.tr"
        _mf->is_macro = true;
#line 2942 "src/parser.tr"
        Decl* _trret71 = box_decl(Decl_ctor_DDecoratorDef(_mf));
        _tr_obj_release(_mf, _trdrop_FunctionDef);
        return _trret71;
    } else if (_t392.tag == Token_KwExtern) {
#line 2944 "src/parser.tr"
        return Parser_parse_extern_decl(self);
    } else if (_t392.tag == Token_KwAsync) {
#line 2946 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2947 "src/parser.tr"
        __auto_type _t397 = Parser_peek(self);
        if (_t397.tag == Token_KwDef) {
#line 2949 "src/parser.tr"
            FunctionDef* f = Parser_parse_function_def(self, false);
#line 2950 "src/parser.tr"
            f->is_async = true;
#line 2951 "src/parser.tr"
            f->is_public = is_public;
#line 2952 "src/parser.tr"
            f->is_export = is_export;
#line 2953 "src/parser.tr"
            f->decorators = decorators;
#line 2954 "src/parser.tr"
            Decl* _trret72 = box_decl(Decl_ctor_DFunction(f));
            _tr_obj_release(f, _trdrop_FunctionDef);
            return _trret72;
        } else if (1) {
            __auto_type _ = _t397;
#line 2956 "src/parser.tr"
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t392;
#line 2958 "src/parser.tr"
        /* pass */
    }
#line 2959 "src/parser.tr"
    return box_decl(Decl_ctor_DTopLevelStmt(Parser_parse_stmt(self)));
}

__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self) {
#line 2972 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 2973 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 2974 "src/parser.tr"
    while (({ __auto_type _ord_t398 = (Parser_peek(self)); __auto_type _wr = ((_ord_t398.tag == Token_make_Dot().tag)); _wr; })) {
#line 2975 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2976 "src/parser.tr"
        ({ TrStr _ap_t399 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t400 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t399); _tr_str_append(&path, _ap_t400); _tr_str_release(_ap_t400); });
    }
#line 2977 "src/parser.tr"
    __auto_type _t401 = Parser_peek(self);
    if (_t401.tag == Token_KwImport) {
#line 2979 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t401;
#line 2981 "src/parser.tr"
        /* pass */
    }
#line 2982 "src/parser.tr"
    List_ptr* il = (void*)List_ptr_new();
#line 2983 "src/parser.tr"
    bool multi = false;
#line 2984 "src/parser.tr"
    __auto_type _t402 = Parser_peek(self);
    if ((_t402.tag == Token_LParen || _t402.tag == Token_LBracket)) {
#line 2986 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2987 "src/parser.tr"
        multi = true;
    } else if (1) {
        __auto_type _ = _t402;
#line 2989 "src/parser.tr"
        /* pass */
    }
#line 2990 "src/parser.tr"
    bool going = true;
#line 2991 "src/parser.tr"
    while (going) {
#line 2994 "src/parser.tr"
        if (multi) {
#line 2995 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        } else {
#line 2997 "src/parser.tr"
            Parser_skip_newlines(self);
        }
#line 2998 "src/parser.tr"
        __auto_type _t403 = Parser_peek(self);
        if ((_t403.tag == Token_RParen || _t403.tag == Token_RBracket || _t403.tag == Token_Eof)) {
#line 3000 "src/parser.tr"
            going = false;
        } else if ((_t403.tag == Token_Newline || _t403.tag == Token_Dedent)) {
#line 3002 "src/parser.tr"
            if ((!multi)) {
#line 3003 "src/parser.tr"
                going = false;
            }
        } else if (1) {
            __auto_type _ = _t403;
#line 3005 "src/parser.tr"
            /* pass */
        }
#line 3006 "src/parser.tr"
        if (going) {
#line 3010 "src/parser.tr"
            if (({ __auto_type _ord_t404 = (Parser_peek(self)); __auto_type _wr = ((_ord_t404.tag == Token_make_Star().tag)); _wr; })) {
#line 3011 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3012 "src/parser.tr"
                List_ptr_append(il, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 3013 "src/parser.tr"
                if (({ __auto_type _ord_t405 = (Parser_peek(self)); __auto_type _wr = ((_ord_t405.tag == Token_make_Comma().tag)); _wr; })) {
#line 3014 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if ((!multi)) {
#line 3016 "src/parser.tr"
                    going = false;
                }
            } else {
#line 3018 "src/parser.tr"
                TrStr item_name = Parser_consume_ident(self);
#line 3019 "src/parser.tr"
                if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 3020 "src/parser.tr"
                    ImportItem* item = ImportItem_init(item_name);
#line 3022 "src/parser.tr"
                    if (({ __auto_type _ord_t406 = (Parser_peek(self)); __auto_type _wr = ((_ord_t406.tag == Token_make_KwAs().tag)); _wr; })) {
#line 3023 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 3024 "src/parser.tr"
                        item->alias = Parser_consume_ident(self);
#line 3025 "src/parser.tr"
                        if ((!_tr_str_eqv((item->alias), (_tr_str_lit_len("", 0LL))))) {
#line 3028 "src/parser.tr"
                            _tr_dict_set(self->import_aliases, _tr_strz(item->alias), _tr_str_box(_tr_str_retain(item->name)));
                        }
                    }
#line 3029 "src/parser.tr"
                    List_ptr_append(il, _tr_obj_retain(item));
#line 3030 "src/parser.tr"
                    if (({ __auto_type _ord_t407 = (Parser_peek(self)); __auto_type _wr = ((_ord_t407.tag == Token_make_Comma().tag)); _wr; })) {
#line 3031 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if ((!multi)) {
#line 3033 "src/parser.tr"
                        going = false;
                    }
                } else {
#line 3035 "src/parser.tr"
                    going = false;
                }
            }
        }
    }
#line 3037 "src/parser.tr"
    if (multi) {
#line 3038 "src/parser.tr"
        __auto_type _t408 = Parser_peek(self);
        if ((_t408.tag == Token_RParen || _t408.tag == Token_RBracket)) {
#line 3040 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t408;
#line 3042 "src/parser.tr"
            /* pass */
        }
    }
#line 3043 "src/parser.tr"
    Parser_expect_newline(self);
#line 3044 "src/parser.tr"
    Decl* _trret73 = box_decl(Decl_ctor_DFromImport(path, il));
    _tr_str_release(path);
    return _trret73;
}

__attribute__((hot)) Decl* Parser_parse_import(Parser* self) {
#line 3047 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3048 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 3049 "src/parser.tr"
    while (({ __auto_type _ord_t409 = (Parser_peek(self)); __auto_type _wr = ((_ord_t409.tag == Token_make_Dot().tag)); _wr; })) {
#line 3050 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3053 "src/parser.tr"
        if (({ __auto_type _ord_t410 = (Parser_peek(self)); __auto_type _wr = ((_ord_t410.tag == Token_make_Star().tag)); _wr; })) {
#line 3054 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3055 "src/parser.tr"
            Parser_expect_newline(self);
#line 3056 "src/parser.tr"
            List_ptr* ils = (void*)List_ptr_new();
#line 3057 "src/parser.tr"
            List_ptr_append(ils, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 3058 "src/parser.tr"
            Decl* _trret74 = box_decl(Decl_ctor_DFromImport(path, ils));
            _tr_str_release(path);
            return _trret74;
        }
#line 3059 "src/parser.tr"
        ({ TrStr _ap_t411 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t412 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t411); _tr_str_append(&path, _ap_t412); _tr_str_release(_ap_t412); });
    }
#line 3060 "src/parser.tr"
    TrStr alias = _tr_str_lit_len("", 0LL);
#line 3061 "src/parser.tr"
    if (({ __auto_type _ord_t413 = (Parser_peek(self)); __auto_type _wr = ((_ord_t413.tag == Token_make_KwAs().tag)); _wr; })) {
#line 3062 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3063 "src/parser.tr"
        TrStr _strtmp_t414 = Parser_consume_ident(self);
        _tr_str_release(alias);
        alias = _strtmp_t414;
    }
#line 3064 "src/parser.tr"
    Parser_expect_newline(self);
#line 3065 "src/parser.tr"
    Decl* _trret75 = box_decl(Decl_ctor_DImport(path, alias));
    _tr_str_release(path);
    _tr_str_release(alias);
    return _trret75;
}

__attribute__((hot)) void Parser_parse_generic_bound(Parser* self, TrStr gname, List_ptr* constraints) {
#line 3071 "src/parser.tr"
    if (({ __auto_type _ord_t415 = (Parser_peek(self)); __auto_type _wr = ((_ord_t415.tag == Token_make_Colon().tag)); _wr; })) {
#line 3072 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3073 "src/parser.tr"
        GenericConstraint* gc = GenericConstraint_init(gname);
#line 3074 "src/parser.tr"
        ({ __auto_type _ord_t416 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t416); });
#line 3075 "src/parser.tr"
        while (({ __auto_type _ord_t417 = (Parser_peek(self)); __auto_type _wr = ((_ord_t417.tag == Token_make_Plus().tag)); _wr; })) {
#line 3076 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3077 "src/parser.tr"
            ({ __auto_type _ord_t418 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t418); });
        }
#line 3078 "src/parser.tr"
        List_ptr_append(constraints, _tr_obj_retain(gc));
    }
}

__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method) {
#line 3081 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3082 "src/parser.tr"
    if ((!is_method)) {
#line 3083 "src/parser.tr"
        __auto_type _t419 = Parser_peek(self);
        if (_t419.tag == Token_Ident) {
            __auto_type _ = _t419.data.Ident.name;
            /* pass */
        } else if (1) {
            __auto_type _ = _t419;
#line 3086 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected a function name after 'def'", 36LL), _tr_str_lit_len("write 'def name(params) -> Type:' - the name must be an identifier, not a keyword or symbol.", 92LL));
        }
    }
#line 3087 "src/parser.tr"
    TrStr name = ((is_method) ? (({
#line 3087 "src/parser.tr"
__auto_type _dores = (Parser_consume_method_ident(self));
_dores;
})) : (({
#line 3087 "src/parser.tr"
__auto_type _dores = (Parser_consume_ident(self));
_dores;
})));
#line 3088 "src/parser.tr"
    FunctionDef* f = FunctionDef_init(name);
#line 3089 "src/parser.tr"
    if (({ __auto_type _ord_t420 = (Parser_peek(self)); __auto_type _wr = ((_ord_t420.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3090 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3091 "src/parser.tr"
        while ((((({ __auto_type _ord_t421 = (Parser_peek(self)); __auto_type _wr = ((_ord_t421.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t422 = (Parser_peek(self)); __auto_type _wr = ((_ord_t422.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t423 = (Parser_peek(self)); __auto_type _wr = ((_ord_t423.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t424 = (Parser_peek(self)); __auto_type _wr = ((_ord_t424.tag != Token_make_LParen().tag)); _wr; }))) {
#line 3092 "src/parser.tr"
            TrStr _gpn = Parser_consume_ident(self);
#line 3093 "src/parser.tr"
            if (_tr_str_eqv((_gpn), (_tr_str_lit_len("", 0LL)))) {
#line 3094 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'def f[T](...)'.", 34LL));
#line 3095 "src/parser.tr"
                _tr_str_release(_gpn);
                break;
            }
#line 3096 "src/parser.tr"
            List_TrStr_append(f->generics, _gpn);
#line 3097 "src/parser.tr"
            Parser_parse_generic_bound(self, _gpn, f->constraints);
#line 3098 "src/parser.tr"
            if (({ __auto_type _ord_t425 = (Parser_peek(self)); __auto_type _wr = ((_ord_t425.tag == Token_make_Comma().tag)); _wr; })) {
#line 3099 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_gpn);
        }
#line 3100 "src/parser.tr"
        if (({ __auto_type _ord_t426 = (Parser_peek(self)); __auto_type _wr = ((_ord_t426.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3101 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3103 "src/parser.tr"
            ({ TrStr _at_t427 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of '", 47LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t427, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t427); });
        }
    }
#line 3104 "src/parser.tr"
    if (({ __auto_type _ord_t428 = (Parser_peek(self)); __auto_type _wr = ((_ord_t428.tag == Token_make_LParen().tag)); _wr; })) {
#line 3105 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3106 "src/parser.tr"
        f->params = Parser_parse_param_list(self);
#line 3107 "src/parser.tr"
        Parser_close_params(self);
    }
#line 3108 "src/parser.tr"
    __auto_type _t429 = Parser_peek(self);
    if (_t429.tag == Token_KwThrows) {
#line 3110 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3111 "src/parser.tr"
        f->throws_ty = box_asttype(Parser_parse_type(self));
    } else if (1) {
        __auto_type _ = _t429;
#line 3113 "src/parser.tr"
        /* pass */
    }
#line 3114 "src/parser.tr"
    if (({ __auto_type _ord_t430 = (Parser_peek(self)); __auto_type _wr = ((_ord_t430.tag == Token_make_Arrow().tag)); _wr; })) {
#line 3115 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3116 "src/parser.tr"
        AstType* ret_t = Parser_parse_type(self);
#line 3117 "src/parser.tr"
        if (({ __auto_type _ord_t431 = (Parser_peek(self)); __auto_type _wr = ((_ord_t431.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3121 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3122 "src/parser.tr"
            ret_t->from_param = Parser_consume_ident(self);
#line 3123 "src/parser.tr"
            List_TrStr_append(ret_t->from_regions, ret_t->from_param);
#line 3124 "src/parser.tr"
            while (({ __auto_type _ord_t432 = (Parser_peek(self)); __auto_type _wr = ((_ord_t432.tag == Token_make_Comma().tag)); _wr; })) {
#line 3125 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3126 "src/parser.tr"
                TrStr rgn2 = Parser_consume_ident(self);
#line 3127 "src/parser.tr"
                ret_t->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((ret_t->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3128 "src/parser.tr"
                List_TrStr_append(ret_t->from_regions, rgn2);
                _tr_str_release(rgn2);
            }
        }
#line 3129 "src/parser.tr"
        f->ret_ty = box_asttype(ret_t);
    }
#line 3137 "src/parser.tr"
    bool w_is = false;
#line 3138 "src/parser.tr"
    __auto_type _t433 = Parser_peek(self);
    if (_t433.tag == Token_Ident) {
        __auto_type wkw0 = _t433.data.Ident.name;
#line 3140 "src/parser.tr"
        if (_tr_str_eqv((wkw0), (_tr_str_lit_len("where", 5LL)))) {
#line 3140 "src/parser.tr"
            w_is = true;
        }
    } else if (1) {
        __auto_type _ = _t433;
        /* pass */
    }
#line 3142 "src/parser.tr"
    if (((!w_is) && ({ __auto_type _ord_t434 = (Parser_peek(self)); __auto_type _wr = ((_ord_t434.tag == Token_make_Newline().tag)); _wr; }))) {
#line 3146 "src/parser.tr"
        long long scan = self->pos;
#line 3147 "src/parser.tr"
        bool scanning = true;
#line 3148 "src/parser.tr"
        while (scanning) {
#line 3149 "src/parser.tr"
            __auto_type _t435 = List_Token_get(self->tokens, scan);
            if ((_t435.tag == Token_Newline || _t435.tag == Token_Indent)) {
#line 3151 "src/parser.tr"
                scan = (scan + 1LL);
            } else if (1) {
                __auto_type _ = _t435;
#line 3153 "src/parser.tr"
                scanning = false;
            }
        }
#line 3154 "src/parser.tr"
        __auto_type _t436 = List_Token_get(self->tokens, scan);
        if (_t436.tag == Token_Ident) {
            __auto_type wkw1 = _t436.data.Ident.name;
#line 3156 "src/parser.tr"
            if (_tr_str_eqv((wkw1), (_tr_str_lit_len("where", 5LL)))) {
#line 3157 "src/parser.tr"
                w_is = true;
#line 3158 "src/parser.tr"
                self->pos = scan;
            }
        } else if (1) {
            __auto_type _ = _t436;
            /* pass */
        }
    }
#line 3160 "src/parser.tr"
    if (w_is) {
#line 3161 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3162 "src/parser.tr"
        bool w_paren = false;
#line 3163 "src/parser.tr"
        if (({ __auto_type _ord_t437 = (Parser_peek(self)); __auto_type _wr = ((_ord_t437.tag == Token_make_LParen().tag)); _wr; })) {
#line 3164 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3165 "src/parser.tr"
            w_paren = true;
#line 3166 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        }
#line 3167 "src/parser.tr"
        bool w_more = true;
#line 3168 "src/parser.tr"
        while (w_more) {
#line 3169 "src/parser.tr"
            if (w_paren) {
#line 3169 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            }
#line 3170 "src/parser.tr"
            if ((w_paren && ({ __auto_type _ord_t438 = (Parser_peek(self)); __auto_type _wr = ((_ord_t438.tag == Token_make_RParen().tag)); _wr; }))) {
#line 3171 "src/parser.tr"
                w_more = false;
            } else {
#line 3173 "src/parser.tr"
                TrStr w_a = Parser_consume_ident(self);
#line 3174 "src/parser.tr"
                TrStr w_kw = Parser_consume_ident(self);
#line 3175 "src/parser.tr"
                TrStr w_b = Parser_consume_ident(self);
#line 3176 "src/parser.tr"
                List_TrStr_append(f->outlives_a, w_a);
#line 3177 "src/parser.tr"
                List_TrStr_append(f->outlives_b, w_b);
#line 3178 "src/parser.tr"
                if (w_paren) {
#line 3178 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                }
#line 3179 "src/parser.tr"
                if ((w_paren && ({ __auto_type _ord_t439 = (Parser_peek(self)); __auto_type _wr = ((_ord_t439.tag == Token_make_Comma().tag)); _wr; }))) {
#line 3180 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else {
#line 3182 "src/parser.tr"
                    w_more = false;
                }
            }
        }
#line 3183 "src/parser.tr"
        if (w_paren) {
#line 3184 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 3185 "src/parser.tr"
            if (({ __auto_type _ord_t440 = (Parser_peek(self)); __auto_type _wr = ((_ord_t440.tag == Token_make_RParen().tag)); _wr; })) {
#line 3186 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3187 "src/parser.tr"
    if (({ __auto_type _ord_t441 = (Parser_peek(self)); __auto_type _wr = ((_ord_t441.tag == Token_make_Colon().tag)); _wr; })) {
#line 3188 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3189 "src/parser.tr"
        f->body = Parser_parse_block(self);
    } else {
#line 3191 "src/parser.tr"
        if (((({ __auto_type _ord_t442 = (Parser_peek(self)); __auto_type _wr = ((_ord_t442.tag == Token_make_Newline().tag)); _wr; }) && ((self->pos + 1LL) < self->tokens->len)) && ({ __auto_type _ord_t443 = (List_Token_get(self->tokens, (self->pos + 1LL))); __auto_type _wr = ((_ord_t443.tag == Token_make_Indent().tag)); _wr; }))) {
#line 3192 "src/parser.tr"
            ({ TrStr _at_t444 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("add ':' after the parameter list / return type, e.g. `def ", 58LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) -> int:`.", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_prev(self, _tr_str_lit_len("missing ':' at the end of this function header", 46LL), _at_t444); _tr_str_release(_at_t444); });
        }
#line 3193 "src/parser.tr"
        Parser_expect_newline(self);
#line 3194 "src/parser.tr"
        f->body = Block_init();
    }
#line 3195 "src/parser.tr"
    _tr_str_release(name);
    return f;
}

__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self) {
#line 3198 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3199 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3200 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 3200 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected a class name after 'class'", 35LL), _tr_str_lit_len("write 'class Name:'.", 20LL));
    }
#line 3201 "src/parser.tr"
    ClassDef* c = ClassDef_init(name);
#line 3202 "src/parser.tr"
    if (({ __auto_type _ord_t445 = (Parser_peek(self)); __auto_type _wr = ((_ord_t445.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3203 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3204 "src/parser.tr"
        while ((((({ __auto_type _ord_t446 = (Parser_peek(self)); __auto_type _wr = ((_ord_t446.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t447 = (Parser_peek(self)); __auto_type _wr = ((_ord_t447.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t448 = (Parser_peek(self)); __auto_type _wr = ((_ord_t448.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t449 = (Parser_peek(self)); __auto_type _wr = ((_ord_t449.tag != Token_make_Colon().tag)); _wr; }))) {
#line 3205 "src/parser.tr"
            TrStr _cgpn = Parser_consume_ident(self);
#line 3206 "src/parser.tr"
            if (_tr_str_eqv((_cgpn), (_tr_str_lit_len("", 0LL)))) {
#line 3207 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'class Box[T]:'.", 34LL));
#line 3208 "src/parser.tr"
                _tr_str_release(_cgpn);
                break;
            }
#line 3209 "src/parser.tr"
            List_TrStr_append(c->generics, _cgpn);
#line 3210 "src/parser.tr"
            Parser_parse_generic_bound(self, _cgpn, c->constraints);
#line 3211 "src/parser.tr"
            if (({ __auto_type _ord_t450 = (Parser_peek(self)); __auto_type _wr = ((_ord_t450.tag == Token_make_Comma().tag)); _wr; })) {
#line 3212 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_cgpn);
        }
#line 3213 "src/parser.tr"
        if (({ __auto_type _ord_t451 = (Parser_peek(self)); __auto_type _wr = ((_ord_t451.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3214 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3216 "src/parser.tr"
            ({ TrStr _at_t452 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of class '", 53LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t452, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t452); });
        }
    }
#line 3220 "src/parser.tr"
    if (({ __auto_type _ord_t453 = (Parser_peek(self)); __auto_type _wr = ((_ord_t453.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3221 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3222 "src/parser.tr"
        ({ TrStr _ord_t454 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t454); _tr_str_release(_ord_t454); });
#line 3223 "src/parser.tr"
        while (({ __auto_type _ord_t455 = (Parser_peek(self)); __auto_type _wr = ((_ord_t455.tag == Token_make_Comma().tag)); _wr; })) {
#line 3224 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3225 "src/parser.tr"
            ({ TrStr _ord_t456 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t456); _tr_str_release(_ord_t456); });
        }
    }
#line 3227 "src/parser.tr"
    if (({ __auto_type _ord_t457 = (Parser_peek(self)); __auto_type _wr = ((_ord_t457.tag == Token_make_LParen().tag)); _wr; })) {
#line 3228 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3229 "src/parser.tr"
        ({ TrStr _ord_t458 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t458); _tr_str_release(_ord_t458); });
#line 3230 "src/parser.tr"
        while (({ __auto_type _ord_t459 = (Parser_peek(self)); __auto_type _wr = ((_ord_t459.tag == Token_make_Comma().tag)); _wr; })) {
#line 3231 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3232 "src/parser.tr"
            ({ TrStr _ord_t460 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t460); _tr_str_release(_ord_t460); });
        }
#line 3233 "src/parser.tr"
        if (({ __auto_type _ord_t461 = (Parser_peek(self)); __auto_type _wr = ((_ord_t461.tag == Token_make_RParen().tag)); _wr; })) {
#line 3234 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3235 "src/parser.tr"
    if (({ __auto_type _ord_t462 = (Parser_peek(self)); __auto_type _wr = ((_ord_t462.tag == Token_make_KwExtends().tag)); _wr; })) {
#line 3236 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3237 "src/parser.tr"
        ({ TrStr _ord_t463 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t463); _tr_str_release(_ord_t463); });
#line 3238 "src/parser.tr"
        while (({ __auto_type _ord_t464 = (Parser_peek(self)); __auto_type _wr = ((_ord_t464.tag == Token_make_Comma().tag)); _wr; })) {
#line 3239 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3240 "src/parser.tr"
            ({ TrStr _ord_t465 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t465); _tr_str_release(_ord_t465); });
        }
    }
#line 3241 "src/parser.tr"
    if (({ __auto_type _ord_t466 = (Parser_peek(self)); __auto_type _wr = ((_ord_t466.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3242 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3243 "src/parser.tr"
        while (true) {
#line 3244 "src/parser.tr"
            TrStr _im_name = Parser_consume_ident(self);
#line 3245 "src/parser.tr"
            List_TrStr_append(c->iface_names, _im_name);
#line 3246 "src/parser.tr"
            AstType* _im_ty = AstType_init(_im_name);
#line 3247 "src/parser.tr"
            if (({ __auto_type _ord_t467 = (Parser_peek(self)); __auto_type _wr = ((_ord_t467.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3248 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3249 "src/parser.tr"
                while ((({ __auto_type _ord_t468 = (Parser_peek(self)); __auto_type _wr = ((_ord_t468.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t469 = (Parser_peek(self)); __auto_type _wr = ((_ord_t469.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3250 "src/parser.tr"
                    ({ __auto_type _ord_t470 = (box_asttype(Parser_parse_type(self))); List_ptr_append(_im_ty->args, _ord_t470); });
#line 3251 "src/parser.tr"
                    if (({ __auto_type _ord_t471 = (Parser_peek(self)); __auto_type _wr = ((_ord_t471.tag == Token_make_Comma().tag)); _wr; })) {
#line 3251 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                }
#line 3252 "src/parser.tr"
                if (({ __auto_type _ord_t472 = (Parser_peek(self)); __auto_type _wr = ((_ord_t472.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3252 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
            }
#line 3253 "src/parser.tr"
            ({ __auto_type _ord_t473 = (box_asttype(_im_ty)); List_ptr_append(c->iface_targs, _ord_t473); });
#line 3254 "src/parser.tr"
            if (({ __auto_type _ord_t474 = (Parser_peek(self)); __auto_type _wr = ((_ord_t474.tag == Token_make_Comma().tag)); _wr; })) {
#line 3255 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3256 "src/parser.tr"
                _tr_str_release(_im_name);
                break;
            }
            _tr_str_release(_im_name);
        }
    }
#line 3261 "src/parser.tr"
    bool inline_body = false;
#line 3262 "src/parser.tr"
    if (({ __auto_type _ord_t475 = (Parser_peek(self)); __auto_type _wr = ((_ord_t475.tag == Token_make_Colon().tag)); _wr; })) {
#line 3263 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3264 "src/parser.tr"
        __auto_type _t476 = Parser_peek(self);
        if ((_t476.tag == Token_Newline || _t476.tag == Token_Eof)) {
#line 3266 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t476;
#line 3268 "src/parser.tr"
            inline_body = true;
        }
    } else {
#line 3270 "src/parser.tr"
        ({ TrStr _at_t477 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("expected ':' after the header of class '", 40LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t477, _tr_str_lit_len("a class header is 'class Name[T]:' (optionally followed by '(Base)'), with the members indented on the following lines.", 119LL)); _tr_str_release(_at_t477); });
#line 3271 "src/parser.tr"
        while (true) {
#line 3272 "src/parser.tr"
            __auto_type _t478 = Parser_peek(self);
            if ((_t478.tag == Token_Newline || _t478.tag == Token_Eof || _t478.tag == Token_Colon)) {
                break;
            } else if (1) {
                __auto_type _ = _t478;
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3275 "src/parser.tr"
    if (inline_body) {
#line 3276 "src/parser.tr"
        bool inl = true;
#line 3277 "src/parser.tr"
        while (inl) {
#line 3278 "src/parser.tr"
            bool in_pub = false;
#line 3279 "src/parser.tr"
            __auto_type _t479 = Parser_peek(self);
            if (_t479.tag == Token_KwPub) {
#line 3281 "src/parser.tr"
                in_pub = true;
#line 3282 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t479;
#line 3284 "src/parser.tr"
                /* pass */
            }
#line 3285 "src/parser.tr"
            __auto_type _t480 = Parser_peek(self);
            if (_t480.tag == Token_KwPass) {
#line 3287 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t480.tag == Token_KwDef) {
#line 3289 "src/parser.tr"
                long long _iml = Parser_cur_line(self);
#line 3290 "src/parser.tr"
                FunctionDef* im = Parser_parse_function_def(self, true);
#line 3291 "src/parser.tr"
                im->line = _iml;
#line 3292 "src/parser.tr"
                im->is_public = in_pub;
#line 3293 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(im));
                _tr_obj_release(im, _trdrop_FunctionDef);
            } else if (_t480.tag == Token_Ident) {
                __auto_type ifn = _t480.data.Ident.name;
#line 3295 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3296 "src/parser.tr"
                AstType** ity = (AstType**)(0LL);
#line 3297 "src/parser.tr"
                if (({ __auto_type _ord_t481 = (Parser_peek(self)); __auto_type _wr = ((_ord_t481.tag == Token_make_Colon().tag)); _wr; })) {
#line 3298 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3299 "src/parser.tr"
                    ity = box_asttype(Parser_parse_type(self));
                }
#line 3300 "src/parser.tr"
                ({ FieldDef* _ord_t482 = (FieldDef_init(ifn, ity)); List_ptr_append(c->fields, _tr_obj_retain(_ord_t482)); _tr_obj_release(_ord_t482, _trdrop_FieldDef); });
            } else if (1) {
                __auto_type _ = _t480;
#line 3302 "src/parser.tr"
                inl = false;
            }
#line 3303 "src/parser.tr"
            __auto_type _t483 = Parser_peek(self);
            if (_t483.tag == Token_Semicolon) {
#line 3305 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t483;
#line 3307 "src/parser.tr"
                inl = false;
            }
        }
#line 3308 "src/parser.tr"
        if (({ __auto_type _ord_t484 = (Parser_peek(self)); __auto_type _wr = ((_ord_t484.tag == Token_make_Newline().tag)); _wr; })) {
#line 3309 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3310 "src/parser.tr"
        Decl* _trret76 = box_decl(Decl_ctor_DClass(c));
        _tr_str_release(name);
        _tr_obj_release(c, _trdrop_ClassDef);
        return _trret76;
    }
#line 3311 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3312 "src/parser.tr"
    __auto_type _t485 = Parser_peek(self);
    if (_t485.tag == Token_Indent) {
#line 3314 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t485;
#line 3316 "src/parser.tr"
        /* pass */
    }
#line 3317 "src/parser.tr"
    bool going = true;
#line 3318 "src/parser.tr"
    while (going) {
#line 3319 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3322 "src/parser.tr"
        if (_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL)))) {
#line 3323 "src/parser.tr"
            __auto_type _t486 = Parser_peek(self);
            if (_t486.tag == Token_TripleStrLit) {
                __auto_type _ds = _t486.data.TripleStrLit.val;
#line 3325 "src/parser.tr"
                c->docstring = _tr_str_retain(_ds);
#line 3326 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3327 "src/parser.tr"
                Parser_skip_newlines(self);
            } else if (1) {
                __auto_type _ = _t486;
#line 3329 "src/parser.tr"
                /* pass */
            }
        }
#line 3330 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3331 "src/parser.tr"
        while (({ __auto_type _ord_t487 = (Parser_peek(self)); __auto_type _wr = ((_ord_t487.tag == Token_make_At().tag)); _wr; })) {
#line 3332 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3333 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3334 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3335 "src/parser.tr"
            if (({ __auto_type _ord_t488 = (Parser_peek(self)); __auto_type _wr = ((_ord_t488.tag == Token_make_LParen().tag)); _wr; })) {
#line 3336 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3337 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3338 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3339 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3341 "src/parser.tr"
        bool is_p = false;
#line 3342 "src/parser.tr"
        __auto_type _t489 = Parser_peek(self);
        if (_t489.tag == Token_KwPub) {
#line 3344 "src/parser.tr"
            is_p = true;
#line 3345 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t489;
#line 3347 "src/parser.tr"
            /* pass */
        }
#line 3348 "src/parser.tr"
        __auto_type _t490 = Parser_peek(self);
        if ((_t490.tag == Token_Dedent || _t490.tag == Token_Eof)) {
#line 3350 "src/parser.tr"
            going = false;
        } else if (_t490.tag == Token_KwDef) {
#line 3352 "src/parser.tr"
            long long _mline = Parser_cur_line(self);
#line 3353 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3354 "src/parser.tr"
            m->line = _mline;
#line 3355 "src/parser.tr"
            m->is_public = is_p;
#line 3356 "src/parser.tr"
            m->decorators = decorators;
#line 3357 "src/parser.tr"
            List_ptr_append(c->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t490.tag == Token_KwAsync) {
#line 3359 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3360 "src/parser.tr"
            __auto_type _t491 = Parser_peek(self);
            if (_t491.tag == Token_KwDef) {
#line 3362 "src/parser.tr"
                long long _mline = Parser_cur_line(self);
#line 3363 "src/parser.tr"
                FunctionDef* m = Parser_parse_function_def(self, true);
#line 3364 "src/parser.tr"
                m->line = _mline;
#line 3365 "src/parser.tr"
                m->is_async = true;
#line 3366 "src/parser.tr"
                m->is_public = is_p;
#line 3367 "src/parser.tr"
                m->decorators = decorators;
#line 3368 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(m));
                _tr_obj_release(m, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t491;
#line 3370 "src/parser.tr"
                /* pass */
            }
        } else if (_t490.tag == Token_Ident) {
            __auto_type fname = _t490.data.Ident.name;
#line 3372 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3373 "src/parser.tr"
            AstType** ty_ptr = (AstType**)(0LL);
#line 3374 "src/parser.tr"
            if (({ __auto_type _ord_t492 = (Parser_peek(self)); __auto_type _wr = ((_ord_t492.tag == Token_make_Colon().tag)); _wr; })) {
#line 3375 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3376 "src/parser.tr"
                AstType* fty = Parser_parse_type(self);
#line 3381 "src/parser.tr"
                if (({ __auto_type _ord_t493 = (Parser_peek(self)); __auto_type _wr = ((_ord_t493.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3382 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3383 "src/parser.tr"
                    fty->from_param = Parser_consume_ident(self);
#line 3384 "src/parser.tr"
                    List_TrStr_append(fty->from_regions, fty->from_param);
#line 3385 "src/parser.tr"
                    while (({ __auto_type _ord_t494 = (Parser_peek(self)); __auto_type _wr = ((_ord_t494.tag == Token_make_Comma().tag)); _wr; })) {
#line 3386 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 3387 "src/parser.tr"
                        TrStr frgn2 = Parser_consume_ident(self);
#line 3388 "src/parser.tr"
                        fty->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((fty->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (frgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3389 "src/parser.tr"
                        List_TrStr_append(fty->from_regions, frgn2);
                        _tr_str_release(frgn2);
                    }
                }
#line 3390 "src/parser.tr"
                ty_ptr = box_asttype(fty);
            }
#line 3391 "src/parser.tr"
            FieldDef* fld = FieldDef_init(fname, ty_ptr);
#line 3392 "src/parser.tr"
            List_ptr_append(c->fields, _tr_obj_retain(fld));
#line 3393 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(fld, _trdrop_FieldDef);
        } else if (_t490.tag == Token_KwPass) {
#line 3395 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3396 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t490;
#line 3398 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3399 "src/parser.tr"
    if (({ __auto_type _ord_t495 = (Parser_peek(self)); __auto_type _wr = ((_ord_t495.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3400 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3401 "src/parser.tr"
    Decl* _trret77 = box_decl(Decl_ctor_DClass(c));
    _tr_str_release(name);
    _tr_obj_release(c, _trdrop_ClassDef);
    return _trret77;
}

__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self) {
#line 3404 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3405 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3406 "src/parser.tr"
    EnumDef* e = EnumDef_init(name);
#line 3410 "src/parser.tr"
    if (({ __auto_type _ord_t496 = (Parser_peek(self)); __auto_type _wr = ((_ord_t496.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3411 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3412 "src/parser.tr"
        ({ TrStr _ord_t497 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t497); _tr_str_release(_ord_t497); });
#line 3413 "src/parser.tr"
        while (({ __auto_type _ord_t498 = (Parser_peek(self)); __auto_type _wr = ((_ord_t498.tag == Token_make_Comma().tag)); _wr; })) {
#line 3414 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3415 "src/parser.tr"
            ({ TrStr _ord_t499 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t499); _tr_str_release(_ord_t499); });
        }
    }
#line 3416 "src/parser.tr"
    if (({ __auto_type _ord_t500 = (Parser_peek(self)); __auto_type _wr = ((_ord_t500.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3417 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3418 "src/parser.tr"
        while (true) {
#line 3419 "src/parser.tr"
            ({ TrStr _ord_t501 = (Parser_consume_ident(self)); List_TrStr_append(e->iface_names, _ord_t501); _tr_str_release(_ord_t501); });
#line 3420 "src/parser.tr"
            if (({ __auto_type _ord_t502 = (Parser_peek(self)); __auto_type _wr = ((_ord_t502.tag == Token_make_Comma().tag)); _wr; })) {
#line 3421 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3422 "src/parser.tr"
                break;
            }
        }
    }
#line 3426 "src/parser.tr"
    bool e_inline = false;
#line 3427 "src/parser.tr"
    if (({ __auto_type _ord_t503 = (Parser_peek(self)); __auto_type _wr = ((_ord_t503.tag == Token_make_Colon().tag)); _wr; })) {
#line 3428 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3429 "src/parser.tr"
        __auto_type _t504 = Parser_peek(self);
        if ((_t504.tag == Token_Newline || _t504.tag == Token_Eof)) {
#line 3431 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t504;
#line 3433 "src/parser.tr"
            e_inline = true;
        }
    }
#line 3434 "src/parser.tr"
    if (e_inline) {
#line 3435 "src/parser.tr"
        bool einl = true;
#line 3436 "src/parser.tr"
        while (einl) {
#line 3437 "src/parser.tr"
            __auto_type _t505 = Parser_peek(self);
            if (_t505.tag == Token_KwPass) {
#line 3439 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t505.tag == Token_Ident) {
                __auto_type evn = _t505.data.Ident.name;
#line 3441 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3442 "src/parser.tr"
                VariantDef* evd = VariantDef_init(evn);
#line 3443 "src/parser.tr"
                if (({ __auto_type _ord_t506 = (Parser_peek(self)); __auto_type _wr = ((_ord_t506.tag == Token_make_LParen().tag)); _wr; })) {
#line 3444 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3445 "src/parser.tr"
                    evd->fields = Parser_parse_param_list(self);
#line 3446 "src/parser.tr"
                    Parser_close_params(self);
                }
#line 3447 "src/parser.tr"
                List_ptr_append(e->variants, _tr_obj_retain(evd));
                _tr_obj_release(evd, _trdrop_VariantDef);
            } else if (1) {
                __auto_type _ = _t505;
#line 3449 "src/parser.tr"
                einl = false;
            }
#line 3450 "src/parser.tr"
            __auto_type _t507 = Parser_peek(self);
            if ((_t507.tag == Token_Comma || _t507.tag == Token_Semicolon)) {
#line 3452 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t507;
#line 3454 "src/parser.tr"
                einl = false;
            }
        }
#line 3455 "src/parser.tr"
        if (({ __auto_type _ord_t508 = (Parser_peek(self)); __auto_type _wr = ((_ord_t508.tag == Token_make_Newline().tag)); _wr; })) {
#line 3456 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3457 "src/parser.tr"
        Decl* _trret78 = box_decl(Decl_ctor_DEnum(e));
        _tr_str_release(name);
        _tr_obj_release(e, _trdrop_EnumDef);
        return _trret78;
    }
#line 3458 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3459 "src/parser.tr"
    __auto_type _t509 = Parser_peek(self);
    if (_t509.tag == Token_Indent) {
#line 3461 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t509;
#line 3463 "src/parser.tr"
        /* pass */
    }
#line 3464 "src/parser.tr"
    bool going = true;
#line 3465 "src/parser.tr"
    while (going) {
#line 3466 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3467 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3468 "src/parser.tr"
        while (({ __auto_type _ord_t510 = (Parser_peek(self)); __auto_type _wr = ((_ord_t510.tag == Token_make_At().tag)); _wr; })) {
#line 3469 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3470 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3471 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3472 "src/parser.tr"
            if (({ __auto_type _ord_t511 = (Parser_peek(self)); __auto_type _wr = ((_ord_t511.tag == Token_make_LParen().tag)); _wr; })) {
#line 3473 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3474 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3475 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3476 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3478 "src/parser.tr"
        bool is_p = false;
#line 3479 "src/parser.tr"
        __auto_type _t512 = Parser_peek(self);
        if (_t512.tag == Token_KwPub) {
#line 3481 "src/parser.tr"
            is_p = true;
#line 3482 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t512;
#line 3484 "src/parser.tr"
            /* pass */
        }
#line 3485 "src/parser.tr"
        __auto_type _t513 = Parser_peek(self);
        if ((_t513.tag == Token_Dedent || _t513.tag == Token_Eof)) {
#line 3487 "src/parser.tr"
            going = false;
        } else if (_t513.tag == Token_Ident) {
            __auto_type vname = _t513.data.Ident.name;
#line 3489 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3490 "src/parser.tr"
            VariantDef* vd = VariantDef_init(vname);
#line 3491 "src/parser.tr"
            if (({ __auto_type _ord_t514 = (Parser_peek(self)); __auto_type _wr = ((_ord_t514.tag == Token_make_LParen().tag)); _wr; })) {
#line 3492 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3493 "src/parser.tr"
                vd->fields = Parser_parse_param_list(self);
#line 3494 "src/parser.tr"
                Parser_close_params(self);
            }
#line 3495 "src/parser.tr"
            List_ptr_append(e->variants, _tr_obj_retain(vd));
#line 3496 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(vd, _trdrop_VariantDef);
        } else if (_t513.tag == Token_KwDef) {
#line 3498 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3499 "src/parser.tr"
            m->is_public = is_p;
#line 3500 "src/parser.tr"
            m->decorators = decorators;
#line 3501 "src/parser.tr"
            List_ptr_append(e->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t513.tag == Token_KwPass) {
#line 3503 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3504 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t513;
#line 3506 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3507 "src/parser.tr"
    if (({ __auto_type _ord_t515 = (Parser_peek(self)); __auto_type _wr = ((_ord_t515.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3508 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3509 "src/parser.tr"
    Decl* _trret79 = box_decl(Decl_ctor_DEnum(e));
    _tr_str_release(name);
    _tr_obj_release(e, _trdrop_EnumDef);
    return _trret79;
}

__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self) {
#line 3512 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3513 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3514 "src/parser.tr"
    InterfaceDef* i = InterfaceDef_init(name);
#line 3516 "src/parser.tr"
    if (({ __auto_type _ord_t516 = (Parser_peek(self)); __auto_type _wr = ((_ord_t516.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3517 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3518 "src/parser.tr"
        while ((({ __auto_type _ord_t517 = (Parser_peek(self)); __auto_type _wr = ((_ord_t517.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t518 = (Parser_peek(self)); __auto_type _wr = ((_ord_t518.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3519 "src/parser.tr"
            TrStr gname = Parser_consume_ident(self);
#line 3520 "src/parser.tr"
            if ((!_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL))))) {
#line 3520 "src/parser.tr"
                List_TrStr_append(i->generics, gname);
            }
#line 3521 "src/parser.tr"
            if (({ __auto_type _ord_t519 = (Parser_peek(self)); __auto_type _wr = ((_ord_t519.tag == Token_make_Comma().tag)); _wr; })) {
#line 3521 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(gname);
        }
#line 3522 "src/parser.tr"
        if (({ __auto_type _ord_t520 = (Parser_peek(self)); __auto_type _wr = ((_ord_t520.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3522 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3525 "src/parser.tr"
    if (({ __auto_type _ord_t521 = (Parser_peek(self)); __auto_type _wr = ((_ord_t521.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3526 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3527 "src/parser.tr"
        ({ TrStr _ord_t522 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t522); _tr_str_release(_ord_t522); });
#line 3528 "src/parser.tr"
        while (({ __auto_type _ord_t523 = (Parser_peek(self)); __auto_type _wr = ((_ord_t523.tag == Token_make_Comma().tag)); _wr; })) {
#line 3529 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3530 "src/parser.tr"
            ({ TrStr _ord_t524 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t524); _tr_str_release(_ord_t524); });
        }
    }
#line 3533 "src/parser.tr"
    bool i_inline = false;
#line 3534 "src/parser.tr"
    if (({ __auto_type _ord_t525 = (Parser_peek(self)); __auto_type _wr = ((_ord_t525.tag == Token_make_Colon().tag)); _wr; })) {
#line 3535 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3536 "src/parser.tr"
        __auto_type _t526 = Parser_peek(self);
        if ((_t526.tag == Token_Newline || _t526.tag == Token_Eof)) {
#line 3538 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t526;
#line 3540 "src/parser.tr"
            i_inline = true;
        }
    }
#line 3541 "src/parser.tr"
    if (i_inline) {
#line 3542 "src/parser.tr"
        bool iinl = true;
#line 3543 "src/parser.tr"
        while (iinl) {
#line 3544 "src/parser.tr"
            bool i_pub = false;
#line 3545 "src/parser.tr"
            __auto_type _t527 = Parser_peek(self);
            if (_t527.tag == Token_KwPub) {
#line 3547 "src/parser.tr"
                i_pub = true;
#line 3548 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t527;
#line 3550 "src/parser.tr"
                /* pass */
            }
#line 3551 "src/parser.tr"
            __auto_type _t528 = Parser_peek(self);
            if (_t528.tag == Token_KwPass) {
#line 3553 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t528.tag == Token_KwDef) {
#line 3555 "src/parser.tr"
                FunctionDef* im2 = Parser_parse_function_def(self, true);
#line 3556 "src/parser.tr"
                im2->is_public = i_pub;
#line 3557 "src/parser.tr"
                List_ptr_append(i->methods, _tr_obj_retain(im2));
                _tr_obj_release(im2, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t528;
#line 3559 "src/parser.tr"
                iinl = false;
            }
#line 3560 "src/parser.tr"
            __auto_type _t529 = Parser_peek(self);
            if (_t529.tag == Token_Semicolon) {
#line 3562 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t529;
#line 3564 "src/parser.tr"
                iinl = false;
            }
        }
#line 3565 "src/parser.tr"
        if (({ __auto_type _ord_t530 = (Parser_peek(self)); __auto_type _wr = ((_ord_t530.tag == Token_make_Newline().tag)); _wr; })) {
#line 3566 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3567 "src/parser.tr"
        Decl* _trret80 = box_decl(Decl_ctor_DInterface(i));
        _tr_str_release(name);
        _tr_obj_release(i, _trdrop_InterfaceDef);
        return _trret80;
    }
#line 3568 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3569 "src/parser.tr"
    __auto_type _t531 = Parser_peek(self);
    if (_t531.tag == Token_Indent) {
#line 3571 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t531;
#line 3573 "src/parser.tr"
        /* pass */
    }
#line 3574 "src/parser.tr"
    bool going = true;
#line 3575 "src/parser.tr"
    while (going) {
#line 3576 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3577 "src/parser.tr"
        bool is_p = false;
#line 3578 "src/parser.tr"
        __auto_type _t532 = Parser_peek(self);
        if (_t532.tag == Token_KwPub) {
#line 3580 "src/parser.tr"
            is_p = true;
#line 3581 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t532;
#line 3583 "src/parser.tr"
            /* pass */
        }
#line 3584 "src/parser.tr"
        __auto_type _t533 = Parser_peek(self);
        if ((_t533.tag == Token_Dedent || _t533.tag == Token_Eof)) {
#line 3586 "src/parser.tr"
            going = false;
        } else if (_t533.tag == Token_KwDef) {
#line 3588 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3589 "src/parser.tr"
            m->is_public = is_p;
#line 3590 "src/parser.tr"
            List_ptr_append(i->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t533.tag == Token_KwPass) {
#line 3592 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3593 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t533;
#line 3595 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3596 "src/parser.tr"
    if (({ __auto_type _ord_t534 = (Parser_peek(self)); __auto_type _wr = ((_ord_t534.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3597 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3598 "src/parser.tr"
    Decl* _trret81 = box_decl(Decl_ctor_DInterface(i));
    _tr_str_release(name);
    _tr_obj_release(i, _trdrop_InterfaceDef);
    return _trret81;
}

__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self) {
#line 3601 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3602 "src/parser.tr"
    TrStr target = Parser_consume_ident(self);
#line 3607 "src/parser.tr"
    if (({ __auto_type _ord_t535 = (Parser_peek(self)); __auto_type _wr = ((_ord_t535.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3608 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3609 "src/parser.tr"
        TrStr _erg = Parser_consume_ident(self);
#line 3610 "src/parser.tr"
        while (({ __auto_type _ord_t536 = (Parser_peek(self)); __auto_type _wr = ((_ord_t536.tag == Token_make_Comma().tag)); _wr; })) {
#line 3611 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3612 "src/parser.tr"
            TrStr _strtmp_t537 = Parser_consume_ident(self);
            _tr_str_release(_erg);
            _erg = _strtmp_t537;
        }
    }
#line 3613 "src/parser.tr"
    if (({ __auto_type _ord_t538 = (Parser_peek(self)); __auto_type _wr = ((_ord_t538.tag == Token_make_Colon().tag)); _wr; })) {
#line 3614 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3615 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3616 "src/parser.tr"
    __auto_type _t539 = Parser_peek(self);
    if (_t539.tag == Token_Indent) {
#line 3618 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t539;
#line 3620 "src/parser.tr"
        /* pass */
    }
#line 3621 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3622 "src/parser.tr"
    bool going = true;
#line 3623 "src/parser.tr"
    while (going) {
#line 3624 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3625 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3626 "src/parser.tr"
        while (({ __auto_type _ord_t540 = (Parser_peek(self)); __auto_type _wr = ((_ord_t540.tag == Token_make_At().tag)); _wr; })) {
#line 3627 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3628 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3629 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3630 "src/parser.tr"
            if (({ __auto_type _ord_t541 = (Parser_peek(self)); __auto_type _wr = ((_ord_t541.tag == Token_make_LParen().tag)); _wr; })) {
#line 3631 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3632 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3633 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3634 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3636 "src/parser.tr"
        bool is_p = false;
#line 3637 "src/parser.tr"
        __auto_type _t542 = Parser_peek(self);
        if (_t542.tag == Token_KwPub) {
#line 3639 "src/parser.tr"
            is_p = true;
#line 3640 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t542;
#line 3642 "src/parser.tr"
            /* pass */
        }
#line 3643 "src/parser.tr"
        __auto_type _t543 = Parser_peek(self);
        if ((_t543.tag == Token_Dedent || _t543.tag == Token_Eof)) {
#line 3645 "src/parser.tr"
            going = false;
        } else if (_t543.tag == Token_KwDef) {
#line 3647 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3648 "src/parser.tr"
            m->is_public = is_p;
#line 3649 "src/parser.tr"
            m->decorators = decorators;
#line 3650 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t543.tag == Token_KwAsync) {
#line 3663 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3664 "src/parser.tr"
            __auto_type _t544 = Parser_peek(self);
            if (_t544.tag == Token_KwDef) {
#line 3666 "src/parser.tr"
                FunctionDef* am = Parser_parse_function_def(self, true);
#line 3667 "src/parser.tr"
                am->is_async = true;
#line 3668 "src/parser.tr"
                am->is_public = is_p;
#line 3669 "src/parser.tr"
                am->decorators = decorators;
#line 3670 "src/parser.tr"
                List_ptr_append(fl, _tr_obj_retain(am));
                _tr_obj_release(am, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t544;
#line 3672 "src/parser.tr"
                /* pass */
            }
        } else if (_t543.tag == Token_KwPass) {
#line 3674 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3675 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t543;
#line 3677 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3678 "src/parser.tr"
    if (({ __auto_type _ord_t545 = (Parser_peek(self)); __auto_type _wr = ((_ord_t545.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3679 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3680 "src/parser.tr"
    Decl d = Decl_ctor_DExtend(target, fl);
#line 3681 "src/parser.tr"
    Decl* _trret82 = box_decl(d);
    _tr_str_release(target);
    return _trret82;
}

__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self) {
#line 3684 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3685 "src/parser.tr"
    TrStr abi = _tr_str_lit_len("C", 1LL);
#line 3686 "src/parser.tr"
    __auto_type _t546 = Parser_peek(self);
    if (_t546.tag == Token_StrLit) {
        __auto_type s = _t546.data.StrLit.val;
#line 3688 "src/parser.tr"
        TrStr _strtmp_t547 = _tr_str_retain(s);
        _tr_str_release(abi);
        abi = _strtmp_t547;
#line 3689 "src/parser.tr"
        if ((((((!_tr_str_eqv((abi), (_tr_str_lit_len("C", 1LL)))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("C++", 3LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("CPP", 3LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("c", 1LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("cpp", 3LL)))))) {
#line 3690 "src/parser.tr"
            ({ TrStr _at_t548 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unknown extern ABI \"", 20LL)), (abi))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t548, _tr_str_lit_len("only extern \"C\" (and \"C++\"/\"CPP\", which also bind through C) are supported - expose the function with a C ABI.", 110LL)); _tr_str_release(_at_t548); });
        }
#line 3691 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t546;
#line 3693 "src/parser.tr"
        /* pass */
    }
#line 3694 "src/parser.tr"
    if (({ __auto_type _ord_t549 = (Parser_peek(self)); __auto_type _wr = ((_ord_t549.tag == Token_make_Colon().tag)); _wr; })) {
#line 3695 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3696 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3697 "src/parser.tr"
    __auto_type _t550 = Parser_peek(self);
    if (_t550.tag == Token_Indent) {
#line 3699 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t550;
#line 3701 "src/parser.tr"
        /* pass */
    }
#line 3702 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3703 "src/parser.tr"
    bool going = true;
#line 3704 "src/parser.tr"
    while (going) {
#line 3705 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3706 "src/parser.tr"
        bool is_p = false;
#line 3707 "src/parser.tr"
        __auto_type _t551 = Parser_peek(self);
        if (_t551.tag == Token_KwPub) {
#line 3709 "src/parser.tr"
            is_p = true;
#line 3710 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t551;
#line 3712 "src/parser.tr"
            /* pass */
        }
#line 3713 "src/parser.tr"
        __auto_type _t552 = Parser_peek(self);
        if ((_t552.tag == Token_Dedent || _t552.tag == Token_Eof)) {
#line 3715 "src/parser.tr"
            going = false;
        } else if (_t552.tag == Token_KwDef) {
#line 3717 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, false);
#line 3718 "src/parser.tr"
            m->is_public = is_p;
#line 3722 "src/parser.tr"
            if ((m->params->len > 0LL)) {
#line 3723 "src/parser.tr"
                Param* _ext_last = ((Param*)List_ptr_get(m->params, (m->params->len - 1LL)));
#line 3724 "src/parser.tr"
                if (_ext_last->is_variadic) {
#line 3725 "src/parser.tr"
                    ((Param*)List_ptr_pop(m->params));
#line 3726 "src/parser.tr"
                    m->is_variadic = true;
                }
            }
#line 3727 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (1) {
            __auto_type _ = _t552;
#line 3729 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3730 "src/parser.tr"
    if (({ __auto_type _ord_t553 = (Parser_peek(self)); __auto_type _wr = ((_ord_t553.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3731 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3732 "src/parser.tr"
    Decl* _trret83 = box_decl(Decl_ctor_DExtern(abi, fl));
    _tr_str_release(abi);
    return _trret83;
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
    ({ __auto_type _ord_t554 = (box_asttype(ret)); List_ptr_append(rt->args, _ord_t554); });
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

__attribute__((hot)) long long _clip_start(TrStr line, long long col) {
#line 71 "src/parser.tr"
    if ((_tr_str_lenv((line)) <= 160LL)) {
#line 71 "src/parser.tr"
        return 0LL;
    }
#line 72 "src/parser.tr"
    if ((col > 100LL)) {
#line 72 "src/parser.tr"
        return (col - 61LL);
    }
#line 73 "src/parser.tr"
    return 0LL;
}

__attribute__((hot)) TrStr _clip_line(TrStr line, long long col) {
#line 76 "src/parser.tr"
    if ((_tr_str_lenv((line)) <= 160LL)) {
#line 76 "src/parser.tr"
        return _tr_str_retain(line);
    }
#line 77 "src/parser.tr"
    long long st = _clip_start(line, col);
#line 78 "src/parser.tr"
    long long en = (st + 150LL);
#line 79 "src/parser.tr"
    if ((en > _tr_str_lenv((line)))) {
#line 79 "src/parser.tr"
        en = _tr_str_lenv((line));
    }
#line 80 "src/parser.tr"
    TrStr out = _tr_str_slicev((line), st, en);
#line 81 "src/parser.tr"
    if ((st > 0LL)) {
#line 81 "src/parser.tr"
        TrStr _strtmp_t555 = _tr_strx_concatv((_tr_str_lit_len("...", 3LL)), (out));
        _tr_str_release(out);
        out = _strtmp_t555;
    }
#line 82 "src/parser.tr"
    if ((en < _tr_str_lenv((line)))) {
#line 82 "src/parser.tr"
        ({ TrStr _ap_t556 = (_tr_str_lit_len("...", 3LL)); _tr_str_append(&out, _ap_t556); });
    }
#line 83 "src/parser.tr"
    TrStr _trret14 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret14;
}

__attribute__((hot)) long long _clip_col(TrStr line, long long col) {
#line 86 "src/parser.tr"
    long long st = _clip_start(line, col);
#line 87 "src/parser.tr"
    if ((st > 0LL)) {
#line 87 "src/parser.tr"
        return ((col - st) + 3LL);
    }
#line 88 "src/parser.tr"
    return col;
}

__attribute__((hot)) TrStr _spaces(long long n) {
#line 92 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init((n + 1LL));
#line 93 "src/parser.tr"
    long long i = 0LL;
#line 94 "src/parser.tr"
    while ((i < n)) {
#line 95 "src/parser.tr"
        StringBuilder_append_char(sb, 32LL);
#line 96 "src/parser.tr"
        i = (i + 1LL);
    }
#line 97 "src/parser.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) bool _fstr_hex_ok(char* p, long long at, long long n, long long esc) {
#line 3739 "src/parser.tr"
    long long nd = 2LL;
#line 3740 "src/parser.tr"
    if ((esc == 117LL)) {
#line 3740 "src/parser.tr"
        nd = 4LL;
    }
#line 3741 "src/parser.tr"
    if ((esc == 85LL)) {
#line 3741 "src/parser.tr"
        nd = 8LL;
    }
#line 3742 "src/parser.tr"
    if (((at + nd) > n)) {
#line 3742 "src/parser.tr"
        return false;
    }
#line 3743 "src/parser.tr"
    long long v = 0LL;
#line 3744 "src/parser.tr"
    long long k = 0LL;
#line 3745 "src/parser.tr"
    while ((k < nd)) {
#line 3746 "src/parser.tr"
        long long h = lex_hex_val(((long long)((*(p + (at + k))))));
#line 3747 "src/parser.tr"
        if ((h < 0LL)) {
#line 3747 "src/parser.tr"
            return false;
        }
#line 3748 "src/parser.tr"
        v = ((v * 16LL) + h);
#line 3749 "src/parser.tr"
        k = (k + 1LL);
    }
#line 3750 "src/parser.tr"
    if (((esc != 120LL) && (v > 1114111LL))) {
#line 3750 "src/parser.tr"
        return false;
    }
#line 3751 "src/parser.tr"
    return true;
}

__attribute__((hot)) long long _find_fmt_colon(TrStr s) {
#line 3754 "src/parser.tr"
    long long i = 0LL;
#line 3755 "src/parser.tr"
    long long n = _tr_str_lenv((s));
#line 3756 "src/parser.tr"
    long long depth = 0LL;
#line 3757 "src/parser.tr"
    while ((i < n)) {
#line 3758 "src/parser.tr"
        long long c = _tr_str_char_at_codev((s), i);
#line 3759 "src/parser.tr"
        if (((c == 34LL) || (c == 39LL))) {
#line 3761 "src/parser.tr"
            long long qc = c;
#line 3762 "src/parser.tr"
            i = (i + 1LL);
#line 3763 "src/parser.tr"
            while (((i < n) && (_tr_str_char_at_codev((s), i) != qc))) {
#line 3764 "src/parser.tr"
                if ((_tr_str_char_at_codev((s), i) == 92LL)) {
#line 3764 "src/parser.tr"
                    i = (i + 1LL);
                }
#line 3765 "src/parser.tr"
                i = (i + 1LL);
            }
        } else if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
#line 3766 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
#line 3767 "src/parser.tr"
            depth = (depth - 1LL);
        } else if (((c == 58LL) && (depth == 0LL))) {
#line 3768 "src/parser.tr"
            return i;
        }
#line 3769 "src/parser.tr"
        i = (i + 1LL);
    }
#line 3770 "src/parser.tr"
    return (-1LL);
}

