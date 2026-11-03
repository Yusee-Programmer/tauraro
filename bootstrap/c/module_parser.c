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
    TrStr _sfo_t51 = p->src_text;
    p->src_text = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t51);
#line 131 "src/parser.tr"
    p->pos = 0LL;
#line 132 "src/parser.tr"
    p->error_count = 0LL;
#line 133 "src/parser.tr"
    p->aug_ctr = 0LL;
#line 134 "src/parser.tr"
    TrStr _sfo_t52 = p->current_file;
    p->current_file = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t52);
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
        __auto_type _t53 = Parser_peek(self);
        if (_t53.tag == Token_Newline) {
#line 155 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 156 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t53;
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
        __auto_type _t54 = Parser_peek(self);
        if ((_t54.tag == Token_Newline || _t54.tag == Token_Indent || _t54.tag == Token_Dedent)) {
#line 166 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 167 "src/parser.tr"
            going = true;
        } else if (1) {
            __auto_type _ = _t54;
#line 170 "src/parser.tr"
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_expect_newline(Parser* self) {
#line 172 "src/parser.tr"
    __auto_type _t55 = Parser_peek(self);
    if ((_t55.tag == Token_Newline || _t55.tag == Token_Semicolon)) {
#line 174 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if ((_t55.tag == Token_Eof || _t55.tag == Token_Dedent || _t55.tag == Token_Indent || _t55.tag == Token_RBrace || _t55.tag == Token_RParen || _t55.tag == Token_RBracket || _t55.tag == Token_Comma)) {
#line 176 "src/parser.tr"
        /* pass */
    } else if ((_t55.tag == Token_KwElse || _t55.tag == Token_KwElif || _t55.tag == Token_KwExcept || _t55.tag == Token_KwFinally || _t55.tag == Token_KwCase)) {
#line 178 "src/parser.tr"
        /* pass */
    } else if (_t55.tag == Token_Error) {
        __auto_type _ = _t55.data.Error.msg;
#line 180 "src/parser.tr"
        /* pass */
    } else if (1) {
        __auto_type _ = _t55;
#line 184 "src/parser.tr"
        if (((self->pos > 0LL) && (self->pos <= self->tokens->len))) {
#line 185 "src/parser.tr"
            __auto_type _t56 = List_Token_get(self->tokens, (self->pos - 1LL));
            if ((_t56.tag == Token_Newline || _t56.tag == Token_Dedent || _t56.tag == Token_Semicolon)) {
                return;
            } else if (1) {
                __auto_type _ = _t56;
                /* pass */
            }
        }
#line 191 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("unexpected token after the end of the statement", 47LL), _tr_str_lit_len("each statement goes on its own line (or separate them with ';') - check for a missing operator, comma or ')' before this token.", 127LL));
#line 192 "src/parser.tr"
        while (true) {
#line 193 "src/parser.tr"
            __auto_type _t57 = Parser_peek(self);
            if ((_t57.tag == Token_Newline || _t57.tag == Token_Eof || _t57.tag == Token_Dedent || _t57.tag == Token_Semicolon)) {
#line 195 "src/parser.tr"
                break;
            } else if (1) {
                __auto_type _ = _t57;
                self->pos = (self->pos + 1LL);
            }
        }
    }
}

__attribute__((hot)) void Parser_close_params(Parser* self) {
#line 201 "src/parser.tr"
    if (({ __auto_type _ord_t58 = (Parser_peek(self)); __auto_type _wr = ((_ord_t58.tag == Token_make_RParen().tag)); _wr; })) {
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
    __auto_type _t59 = Parser_peek(self);
    if (_t59.tag == Token_Eof) {
#line 219 "src/parser.tr"
        return true;
    } else if (1) {
        __auto_type _ = _t59;
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
    ({ __auto_type _ord_t60 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ ({ TrStr _ps = ({ TrStr _tr_cl = (_ord_t60); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); }); });
#line 252 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 253 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 254 "src/parser.tr"
        TrStr _strtmp_t61 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t61;
    }
#line 255 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 256 "src/parser.tr"
    if ((col > 0LL)) {
#line 257 "src/parser.tr"
        ({ TrStr _ap_t62 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t63 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t62); _tr_str_append(&head, _ap_t63); _tr_str_release(_ap_t63); });
    }
#line 258 "src/parser.tr"
    ({ ({ TrStr _ps = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
#line 260 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 261 "src/parser.tr"
        TrStr srcline = ({ TrStr _apt_t64 = _tr_str_retain(self->src_text); __auto_type _wr = (_nth_source_line(_apt_t64, ln)); _tr_str_release(_apt_t64); _wr; });
#line 262 "src/parser.tr"
        long long full_col = col;
#line 263 "src/parser.tr"
        long long col1 = _clip_col(srcline, full_col);
#line 264 "src/parser.tr"
        TrStr _strtmp_t65 = _clip_line(srcline, full_col);
        _tr_str_release(srcline);
        srcline = _strtmp_t65;
#line 265 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 266 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 267 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 268 "src/parser.tr"
            ({ ({ TrStr _ps = ({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
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
                ({ __auto_type _ord_t66 = (c_dim(gbar)); __auto_type _ord_t68 = (({ TrStr _tr_cl = (_ord_t66); TrStr _tr_cr = (_spaces((col1 - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _at_t67 = (repeat_char(_tr_str_lit_len("^", 1LL), width)); ({ ({ TrStr _ps = ({ TrStr _tr_cl = (_ord_t68); TrStr _tr_cr = (c_red(_at_t67)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); }); _tr_str_release(_at_t67); });
            }
            _tr_str_release(gnum);
        }
    }
#line 279 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 280 "src/parser.tr"
        ({ ({ TrStr _ps = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
    }
#line 281 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) TrStr Parser_consume_ident(Parser* self) {
#line 284 "src/parser.tr"
    __auto_type _t69 = Parser_peek(self);
    if (_t69.tag == Token_Ident) {
        __auto_type name = _t69.data.Ident.name;
#line 286 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 287 "src/parser.tr"
        TrStr _trret15 = _tr_str_retain(name);
        return _trret15;
    } else if (_t69.tag == Token_KwInt) {
#line 289 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 290 "src/parser.tr"
        return _tr_str_lit_len("int", 3LL);
    } else if (_t69.tag == Token_KwFloat) {
#line 292 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 293 "src/parser.tr"
        return _tr_str_lit_len("float", 5LL);
    } else if (_t69.tag == Token_KwBool) {
#line 295 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 296 "src/parser.tr"
        return _tr_str_lit_len("bool", 4LL);
    } else if ((_t69.tag == Token_KwStr || _t69.tag == Token_KwString)) {
#line 298 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 299 "src/parser.tr"
        return _tr_str_lit_len("str", 3LL);
    } else if (_t69.tag == Token_KwChar) {
#line 301 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 302 "src/parser.tr"
        return _tr_str_lit_len("char", 4LL);
    } else if (_t69.tag == Token_KwVoid) {
#line 304 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 305 "src/parser.tr"
        return _tr_str_lit_len("void", 4LL);
    } else if (_t69.tag == Token_KwExtend) {
#line 308 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 309 "src/parser.tr"
        return _tr_str_lit_len("extend", 6LL);
    } else if (_t69.tag == Token_KwNone) {
#line 312 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 313 "src/parser.tr"
        return _tr_str_lit_len("None", 4LL);
    } else if (_t69.tag == Token_KwLambda) {
#line 316 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 317 "src/parser.tr"
        return _tr_str_lit_len("lambda", 6LL);
    } else if (_t69.tag == Token_KwSpawn) {
#line 320 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 321 "src/parser.tr"
        return _tr_str_lit_len("spawn", 5LL);
    } else if (_t69.tag == Token_KwAsync) {
#line 324 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 325 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (1) {
        __auto_type _ = _t69;
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
    __auto_type _t70 = Parser_peek(self);
    if (_t70.tag == Token_KwDef) {
#line 360 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 361 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("def", 3LL);
    } else if (_t70.tag == Token_KwClass) {
#line 363 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 364 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("class", 5LL);
    } else if (_t70.tag == Token_KwEnum) {
#line 366 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 367 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("enum", 4LL);
    } else if (_t70.tag == Token_KwInterface) {
#line 369 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 370 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("interface", 9LL);
    } else if (_t70.tag == Token_KwStruct) {
#line 372 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 373 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("struct", 6LL);
    } else if (_t70.tag == Token_KwIf) {
#line 375 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 376 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("if", 2LL);
    } else if (_t70.tag == Token_KwElif) {
#line 378 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 379 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("elif", 4LL);
    } else if (_t70.tag == Token_KwElse) {
#line 381 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 382 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("else", 4LL);
    } else if (_t70.tag == Token_KwFor) {
#line 384 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 385 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("for", 3LL);
    } else if (_t70.tag == Token_KwWhile) {
#line 387 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 388 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("while", 5LL);
    } else if (_t70.tag == Token_KwLoop) {
#line 390 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 391 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("loop", 4LL);
    } else if (_t70.tag == Token_KwReturn) {
#line 393 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 394 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("return", 6LL);
    } else if (_t70.tag == Token_KwBreak) {
#line 396 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 397 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("break", 5LL);
    } else if (_t70.tag == Token_KwContinue) {
#line 399 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 400 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("continue", 8LL);
    } else if (_t70.tag == Token_KwPass) {
#line 402 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 403 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pass", 4LL);
    } else if (_t70.tag == Token_KwMatch) {
#line 405 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 406 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("match", 5LL);
    } else if (_t70.tag == Token_KwCase) {
#line 408 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 409 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("case", 4LL);
    } else if (_t70.tag == Token_KwTry) {
#line 411 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 412 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("try", 3LL);
    } else if (_t70.tag == Token_KwExcept) {
#line 414 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 415 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("except", 6LL);
    } else if (_t70.tag == Token_KwFinally) {
#line 417 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 418 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("finally", 7LL);
    } else if (_t70.tag == Token_KwRaise) {
#line 420 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 421 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("raise", 5LL);
    } else if (_t70.tag == Token_KwAssert) {
#line 423 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 424 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("assert", 6LL);
    } else if (_t70.tag == Token_KwWith) {
#line 426 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 427 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("with", 4LL);
    } else if (_t70.tag == Token_KwDefer) {
#line 429 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 430 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("defer", 5LL);
    } else if (_t70.tag == Token_KwAsm) {
#line 432 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 433 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("asm", 3LL);
    } else if (_t70.tag == Token_KwTaskGroup) {
#line 435 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 436 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("taskgroup", 9LL);
    } else if (_t70.tag == Token_KwImplements) {
#line 438 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 439 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("implements", 10LL);
    } else if (_t70.tag == Token_KwImport) {
#line 441 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 442 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("import", 6LL);
    } else if (_t70.tag == Token_KwFrom) {
#line 444 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 445 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("from", 4LL);
    } else if (_t70.tag == Token_KwAs) {
#line 447 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 448 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("as", 2LL);
    } else if (_t70.tag == Token_KwIn) {
#line 450 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 451 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("in", 2LL);
    } else if (_t70.tag == Token_KwMut) {
#line 453 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 454 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("mut", 3LL);
    } else if (_t70.tag == Token_KwShared) {
#line 456 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 457 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("shared", 6LL);
    } else if (_t70.tag == Token_KwExtern) {
#line 459 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 460 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extern", 6LL);
    } else if (_t70.tag == Token_KwUnsafe) {
#line 462 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 463 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("unsafe", 6LL);
    } else if (_t70.tag == Token_KwThrows) {
#line 465 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 466 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("throws", 6LL);
    } else if (_t70.tag == Token_KwExtends) {
#line 468 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 469 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("extends", 7LL);
    } else if (_t70.tag == Token_KwAwait) {
#line 471 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 472 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("await", 5LL);
    } else if (_t70.tag == Token_KwYield) {
#line 474 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 475 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("yield", 5LL);
    } else if (_t70.tag == Token_KwPub) {
#line 477 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 478 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("pub", 3LL);
    } else if (_t70.tag == Token_KwStatic) {
#line 480 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 481 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("static", 6LL);
    } else if (_t70.tag == Token_KwConst) {
#line 483 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 484 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("const", 5LL);
    } else if (_t70.tag == Token_KwActor) {
#line 486 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 487 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("actor", 5LL);
    } else if (_t70.tag == Token_KwSuper) {
#line 489 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 490 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("super", 5LL);
    } else if (_t70.tag == Token_KwExport) {
#line 492 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 493 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("export", 6LL);
    } else if (_t70.tag == Token_KwDecorator) {
#line 495 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 496 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("decorator", 9LL);
    } else if (_t70.tag == Token_KwMacro) {
#line 498 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 499 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("macro", 5LL);
    } else if (_t70.tag == Token_KwDo) {
#line 501 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 502 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("do", 2LL);
    } else if (_t70.tag == Token_KwSizeOf) {
#line 504 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 505 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("sizeof", 6LL);
    } else if (_t70.tag == Token_KwIs) {
#line 507 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 508 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("is", 2LL);
    } else if (_t70.tag == Token_KwTrue) {
#line 510 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 511 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("true", 4LL);
    } else if (_t70.tag == Token_KwFalse) {
#line 513 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 514 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("false", 5LL);
    } else if (_t70.tag == Token_KwAnd) {
#line 516 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 517 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("and", 3LL);
    } else if (_t70.tag == Token_KwOr) {
#line 519 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 520 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("or", 2LL);
    } else if (_t70.tag == Token_KwNot) {
#line 522 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 523 "src/parser.tr"
        _tr_str_release(base);
        return _tr_str_lit_len("not", 3LL);
    } else if (1) {
        __auto_type _ = _t70;
#line 525 "src/parser.tr"
        /* pass */
    }
#line 526 "src/parser.tr"
    _tr_str_release(base);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr Parser_consume_module_ident(Parser* self) {
#line 531 "src/parser.tr"
    __auto_type _t71 = Parser_peek(self);
    if (_t71.tag == Token_KwAsync) {
#line 533 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 534 "src/parser.tr"
        return _tr_str_lit_len("async", 5LL);
    } else if (_t71.tag == Token_KwMatch) {
#line 536 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 537 "src/parser.tr"
        return _tr_str_lit_len("match", 5LL);
    } else if (_t71.tag == Token_KwFor) {
#line 539 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 540 "src/parser.tr"
        return _tr_str_lit_len("for", 3LL);
    } else if (_t71.tag == Token_KwIn) {
#line 542 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 543 "src/parser.tr"
        return _tr_str_lit_len("in", 2LL);
    } else if (_t71.tag == Token_KwIs) {
#line 545 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 546 "src/parser.tr"
        return _tr_str_lit_len("is", 2LL);
    } else if (1) {
        __auto_type _ = _t71;
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
    __auto_type _t72 = Parser_peek(self);
    if (_t72.tag == Token_KwNone) {
#line 557 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 558 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (_t72.tag == Token_LParen) {
#line 561 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 562 "src/parser.tr"
        AstType* tup_t = AstType_init(_tr_str_lit_len("Tuple", 5LL));
#line 563 "src/parser.tr"
        while ((({ __auto_type _ord_t73 = (Parser_peek(self)); __auto_type _wr = ((_ord_t73.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t74 = (Parser_peek(self)); __auto_type _wr = ((_ord_t74.tag != Token_make_Eof().tag)); _wr; }))) {
#line 564 "src/parser.tr"
            AstType* elem_t = Parser_parse_type(self);
#line 565 "src/parser.tr"
            ({ __auto_type _ord_t75 = (box_asttype(elem_t)); List_ptr_append(tup_t->args, _ord_t75); });
#line 566 "src/parser.tr"
            if (({ __auto_type _ord_t76 = (Parser_peek(self)); __auto_type _wr = ((_ord_t76.tag == Token_make_Comma().tag)); _wr; })) {
#line 566 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 567 "src/parser.tr"
        if (({ __auto_type _ord_t77 = (Parser_peek(self)); __auto_type _wr = ((_ord_t77.tag == Token_make_RParen().tag)); _wr; })) {
#line 567 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 568 "src/parser.tr"
        _tr_str_release(name);
        return tup_t;
    } else if (_t72.tag == Token_LBracket) {
#line 571 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 572 "src/parser.tr"
        AstType* arr_t = AstType_init(_tr_str_lit_len("Array", 5LL));
#line 573 "src/parser.tr"
        AstType* arr_elem = Parser_parse_type(self);
#line 574 "src/parser.tr"
        ({ __auto_type _ord_t78 = (box_asttype(arr_elem)); List_ptr_append(arr_t->args, _ord_t78); });
#line 575 "src/parser.tr"
        if (({ __auto_type _ord_t79 = (Parser_peek(self)); __auto_type _wr = ((_ord_t79.tag == Token_make_Semicolon().tag)); _wr; })) {
#line 576 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 577 "src/parser.tr"
            __auto_type _t80 = Parser_peek(self);
            if (_t80.tag == Token_IntLit) {
                __auto_type arr_sz = _t80.data.IntLit.val;
#line 579 "src/parser.tr"
                arr_t->array_size = arr_sz;
#line 580 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t80;
                /* pass */
            }
        } else if (({ __auto_type _ord_t81 = (Parser_peek(self)); __auto_type _wr = ((_ord_t81.tag == Token_make_RBracket().tag)); _wr; })) {
#line 583 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("an array type needs a length", 28LL), _tr_str_lit_len("write a fixed-size array as '[T; N]' (e.g. '[u8; 16]'), or use List[T] for a growable list.", 91LL));
        }
#line 584 "src/parser.tr"
        if (({ __auto_type _ord_t82 = (Parser_peek(self)); __auto_type _wr = ((_ord_t82.tag == Token_make_RBracket().tag)); _wr; })) {
#line 584 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 585 "src/parser.tr"
        _tr_str_release(name);
        return arr_t;
    } else if (_t72.tag == Token_KwDef) {
#line 590 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 591 "src/parser.tr"
        AstType* dfn_t = AstType_init(_tr_str_lit_len("def", 3LL));
#line 592 "src/parser.tr"
        if (({ __auto_type _ord_t83 = (Parser_peek(self)); __auto_type _wr = ((_ord_t83.tag == Token_make_LParen().tag)); _wr; })) {
#line 593 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 594 "src/parser.tr"
            while ((({ __auto_type _ord_t84 = (Parser_peek(self)); __auto_type _wr = ((_ord_t84.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t85 = (Parser_peek(self)); __auto_type _wr = ((_ord_t85.tag != Token_make_Eof().tag)); _wr; }))) {
#line 595 "src/parser.tr"
                long long dpos0 = self->pos;
#line 596 "src/parser.tr"
                AstType* dpt = Parser_parse_type(self);
#line 597 "src/parser.tr"
                ({ __auto_type _ord_t86 = (box_asttype(dpt)); List_ptr_append(dfn_t->args, _ord_t86); });
#line 598 "src/parser.tr"
                if (({ __auto_type _ord_t87 = (Parser_peek(self)); __auto_type _wr = ((_ord_t87.tag == Token_make_Comma().tag)); _wr; })) {
#line 598 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 599 "src/parser.tr"
                if ((self->pos == dpos0)) {
#line 599 "src/parser.tr"
                    break;
                }
            }
#line 600 "src/parser.tr"
            if (({ __auto_type _ord_t88 = (Parser_peek(self)); __auto_type _wr = ((_ord_t88.tag == Token_make_RParen().tag)); _wr; })) {
#line 600 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
#line 603 "src/parser.tr"
        AstType** dthrows = (AstType**)(0LL);
#line 604 "src/parser.tr"
        if (({ __auto_type _ord_t89 = (Parser_peek(self)); __auto_type _wr = ((_ord_t89.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 605 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 606 "src/parser.tr"
            dthrows = box_asttype(Parser_parse_type(self));
        }
#line 607 "src/parser.tr"
        AstType* dret = AstType_init(_tr_str_lit_len("void", 4LL));
#line 608 "src/parser.tr"
        if (({ __auto_type _ord_t90 = (Parser_peek(self)); __auto_type _wr = ((_ord_t90.tag == Token_make_Arrow().tag)); _wr; })) {
#line 609 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 610 "src/parser.tr"
            dret = Parser_parse_type(self);
        }
#line 611 "src/parser.tr"
        if ((((unsigned long long)(dthrows)) != ((unsigned long long)(0LL)))) {
#line 612 "src/parser.tr"
            dret = throws_result_type(dret, dthrows);
        }
#line 613 "src/parser.tr"
        ({ __auto_type _ord_t91 = (box_asttype(dret)); List_ptr_append(dfn_t->args, _ord_t91); });
#line 614 "src/parser.tr"
        _tr_str_release(name);
        return dfn_t;
    } else if (_t72.tag == Token_KwMut) {
#line 618 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 619 "src/parser.tr"
        TrStr mr_name = Parser_consume_ident(self);
#line 620 "src/parser.tr"
        if (_tr_str_eqv((mr_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 621 "src/parser.tr"
            AstType* mr_inner = Parser_parse_type(self);
#line 622 "src/parser.tr"
            mr_inner->is_borrow = true;
#line 623 "src/parser.tr"
            mr_inner->is_mut_borrow = true;
#line 624 "src/parser.tr"
            _tr_str_release(mr_name);
            _tr_str_release(name);
            return mr_inner;
        }
#line 626 "src/parser.tr"
        Parser_emit_diag_prev(self, _tr_str_lit_len("'mut' in a type must be followed by 'ref'", 41LL), _tr_str_lit_len("write 'mut ref T' for an exclusive borrow, or just 'T' for an owned value (mutability is declared on the variable, not the type).", 129LL));
#line 627 "src/parser.tr"
        _tr_str_release(mr_name);
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    } else if (1) {
        __auto_type _ = _t72;
#line 629 "src/parser.tr"
        TrStr _strtmp_t92 = Parser_consume_ident(self);
        _tr_str_release(name);
        name = _strtmp_t92;
    }
#line 631 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 632 "src/parser.tr"
        Parser_emit_diag_prev(self, _tr_str_lit_len("expected a type here", 20LL), _tr_str_lit_len("write a type name such as 'int', 'str', 'List[int]' or a class name.", 68LL));
#line 634 "src/parser.tr"
        __auto_type _t93 = Parser_peek(self);
        if ((_t93.tag == Token_Eof || _t93.tag == Token_RParen || _t93.tag == Token_RBracket)) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t93;
            self->pos = (self->pos + 1LL);
        }
#line 637 "src/parser.tr"
        _tr_str_release(name);
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
#line 643 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("ref", 3LL)))) {
#line 644 "src/parser.tr"
        AstType* ref_inner = Parser_parse_type(self);
#line 645 "src/parser.tr"
        ref_inner->is_borrow = true;
#line 646 "src/parser.tr"
        _tr_str_release(name);
        return ref_inner;
    }
#line 649 "src/parser.tr"
    __auto_type _t94 = Parser_peek(self);
    if (_t94.tag == Token_LBracket) {
#line 651 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 652 "src/parser.tr"
        AstType* t = AstType_init(name);
#line 653 "src/parser.tr"
        bool going = true;
#line 654 "src/parser.tr"
        while (going) {
#line 655 "src/parser.tr"
            __auto_type _t95 = Parser_peek(self);
            if ((_t95.tag == Token_RBracket || _t95.tag == Token_Eof)) {
#line 657 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t95;
#line 659 "src/parser.tr"
                /* pass */
            }
#line 660 "src/parser.tr"
            if (going) {
#line 661 "src/parser.tr"
                __auto_type arg_t = Parser_parse_type(self);
#line 662 "src/parser.tr"
                ({ __auto_type _ord_t96 = (box_asttype(arg_t)); List_ptr_append(t->args, _ord_t96); });
#line 663 "src/parser.tr"
                __auto_type _t97 = Parser_peek(self);
                if (_t97.tag == Token_Comma) {
#line 665 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (_t97.tag == Token_Semicolon) {
#line 670 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 671 "src/parser.tr"
                    __auto_type _t98 = Parser_peek(self);
                    if (_t98.tag == Token_IntLit) {
                        __auto_type gsz = _t98.data.IntLit.val;
#line 673 "src/parser.tr"
                        t->array_size = gsz;
#line 674 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t98;
                        /* pass */
                    }
#line 676 "src/parser.tr"
                    going = false;
                } else if (1) {
                    __auto_type _ = _t97;
#line 678 "src/parser.tr"
                    going = false;
                }
            }
        }
#line 679 "src/parser.tr"
        __auto_type _t99 = Parser_peek(self);
        if (_t99.tag == Token_RBracket) {
#line 681 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t99;
#line 683 "src/parser.tr"
            ({ TrStr _at_t100 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the type '", 26LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[...]'", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t100, _tr_str_lit_len("add the matching ']' after the type arguments.", 46LL)); _tr_str_release(_at_t100); });
        }
#line 684 "src/parser.tr"
        _tr_str_release(name);
        return t;
    } else if (1) {
        __auto_type _ = _t94;
#line 686 "src/parser.tr"
        /* pass */
    }
#line 687 "src/parser.tr"
    return AstType_init(name);
}

__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self) {
#line 692 "src/parser.tr"
    List_ptr* pl = (void*)List_ptr_new();
#line 693 "src/parser.tr"
    bool going = true;
#line 694 "src/parser.tr"
    while (going) {
#line 695 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 696 "src/parser.tr"
        __auto_type _t101 = Parser_peek(self);
        if ((_t101.tag == Token_RParen || _t101.tag == Token_Eof)) {
#line 698 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t101;
#line 700 "src/parser.tr"
            /* pass */
        }
#line 701 "src/parser.tr"
        if (going) {
#line 702 "src/parser.tr"
            TrStr item_name = Parser_consume_ident(self);
#line 715 "src/parser.tr"
            if (((_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))) && ({ __auto_type _ord_t102 = (Parser_peek(self)); __auto_type _wr = ((_ord_t102.tag != Token_make_RParen().tag)); _wr; })) && ({ __auto_type _ord_t103 = (Parser_peek(self)); __auto_type _wr = ((_ord_t103.tag != Token_make_Eof().tag)); _wr; }))) {
#line 716 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a parameter name, found a reserved keyword", 51LL), _tr_str_lit_len("reserved words cannot be used as a parameter name (they introduce a new binding, same as a 'mut'/'let' variable) — rename this parameter.", 139LL));
#line 717 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 718 "src/parser.tr"
            if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 719 "src/parser.tr"
                AstType** ty_ptr = (AstType**)(0LL);
#line 720 "src/parser.tr"
                __auto_type _t104 = Parser_peek(self);
                if (_t104.tag == Token_Colon) {
#line 722 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 723 "src/parser.tr"
                    __auto_type ty = Parser_parse_type(self);
#line 726 "src/parser.tr"
                    if (({ __auto_type _ord_t105 = (Parser_peek(self)); __auto_type _wr = ((_ord_t105.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 727 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 728 "src/parser.tr"
                        ty->from_param = Parser_consume_ident(self);
#line 729 "src/parser.tr"
                        ({ TrStr _apt_t106 = _tr_str_retain(ty->from_param); List_TrStr_append(ty->from_regions, _apt_t106); _tr_str_release(_apt_t106); });
                    }
#line 730 "src/parser.tr"
                    ty_ptr = box_asttype(ty);
                } else if (1) {
                    __auto_type _ = _t104;
#line 736 "src/parser.tr"
                    if (_tr_str_eqv((item_name), (_tr_str_lit_len("ref", 3LL)))) {
#line 737 "src/parser.tr"
                        AstType* bt = Parser_parse_type(self);
#line 738 "src/parser.tr"
                        bt->is_borrow = true;
#line 739 "src/parser.tr"
                        if (({ __auto_type _ord_t107 = (Parser_peek(self)); __auto_type _wr = ((_ord_t107.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 740 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
#line 741 "src/parser.tr"
                            bt->from_param = Parser_consume_ident(self);
#line 742 "src/parser.tr"
                            ({ TrStr _apt_t108 = _tr_str_retain(bt->from_param); List_TrStr_append(bt->from_regions, _apt_t108); _tr_str_release(_apt_t108); });
                        }
#line 743 "src/parser.tr"
                        ty_ptr = box_asttype(bt);
#line 746 "src/parser.tr"
                        TrStr _strtmp_t109 = _tr_str_retain(bt->name);
                        _tr_str_release(item_name);
                        item_name = _strtmp_t109;
                    }
                }
#line 747 "src/parser.tr"
                Param* p = Param_init(item_name, ty_ptr);
#line 748 "src/parser.tr"
                if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 749 "src/parser.tr"
                    if ((*ty_ptr)->is_borrow) {
#line 750 "src/parser.tr"
                        p->is_ref = true;
                    }
#line 751 "src/parser.tr"
                    if ((*ty_ptr)->is_mut_borrow) {
#line 752 "src/parser.tr"
                        p->is_mut_ref = true;
                    }
                }
#line 755 "src/parser.tr"
                if (({ __auto_type _ord_t110 = (Parser_peek(self)); __auto_type _wr = ((_ord_t110.tag == Token_make_DotDotDot().tag)); _wr; })) {
#line 756 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 757 "src/parser.tr"
                    p->is_variadic = true;
                }
#line 758 "src/parser.tr"
                List_ptr_append(pl, _tr_obj_retain(p));
#line 759 "src/parser.tr"
                __auto_type _t111 = Parser_peek(self);
                if (_t111.tag == Token_Comma) {
#line 761 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 762 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                } else if (1) {
                    __auto_type _ = _t111;
#line 764 "src/parser.tr"
                    going = false;
                }
            } else {
#line 766 "src/parser.tr"
                going = false;
            }
        }
    }
#line 767 "src/parser.tr"
    return pl;
}

__attribute__((hot)) Block* Parser_parse_block(Parser* self) {
#line 772 "src/parser.tr"
    Block* b = Block_init();
#line 773 "src/parser.tr"
    bool is_inline = false;
#line 774 "src/parser.tr"
    __auto_type _t112 = Parser_peek(self);
    if (_t112.tag == Token_Indent) {
#line 776 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (_t112.tag == Token_Newline) {
#line 778 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 779 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
    } else if (1) {
        __auto_type _ = _t112;
#line 781 "src/parser.tr"
        is_inline = true;
    }
#line 783 "src/parser.tr"
    if (is_inline) {
#line 784 "src/parser.tr"
        Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 785 "src/parser.tr"
        Block_push(b, Parser_parse_stmt(self));
#line 786 "src/parser.tr"
        bool semi_going = true;
#line 787 "src/parser.tr"
        while (semi_going) {
#line 788 "src/parser.tr"
            semi_going = false;
#line 789 "src/parser.tr"
            __auto_type _t113 = Parser_peek(self);
            if (_t113.tag == Token_Semicolon) {
#line 792 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 793 "src/parser.tr"
                __auto_type _t114 = Parser_peek(self);
                if ((_t114.tag == Token_Newline || _t114.tag == Token_Eof || _t114.tag == Token_Dedent)) {
#line 795 "src/parser.tr"
                    /* pass */
                } else if (1) {
                    __auto_type _ = _t114;
#line 797 "src/parser.tr"
                    Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 798 "src/parser.tr"
                    Block_push(b, Parser_parse_stmt(self));
#line 799 "src/parser.tr"
                    semi_going = true;
                }
            } else if (1) {
                __auto_type _ = _t113;
#line 803 "src/parser.tr"
                if ((self->pos > 0LL)) {
#line 804 "src/parser.tr"
                    __auto_type _t115 = List_Token_get(self->tokens, (self->pos - 1LL));
                    if (_t115.tag == Token_Semicolon) {
#line 806 "src/parser.tr"
                        __auto_type _t116 = Parser_peek(self);
                        if ((_t116.tag == Token_Newline || _t116.tag == Token_Eof || _t116.tag == Token_Dedent)) {
#line 808 "src/parser.tr"
                            /* pass */
                        } else if (1) {
                            __auto_type _ = _t116;
#line 810 "src/parser.tr"
                            Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
#line 811 "src/parser.tr"
                            Block_push(b, Parser_parse_stmt(self));
#line 812 "src/parser.tr"
                            semi_going = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t115;
#line 814 "src/parser.tr"
                        /* pass */
                    }
                }
            }
        }
#line 815 "src/parser.tr"
        return b;
    }
#line 817 "src/parser.tr"
    bool going = true;
#line 818 "src/parser.tr"
    while (going) {
#line 819 "src/parser.tr"
        __auto_type _t117 = Parser_peek(self);
        if ((_t117.tag == Token_Dedent || _t117.tag == Token_Eof)) {
#line 821 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t117;
#line 823 "src/parser.tr"
            /* pass */
        }
#line 824 "src/parser.tr"
        if (going) {
#line 825 "src/parser.tr"
            Parser_skip_newlines(self);
#line 826 "src/parser.tr"
            __auto_type _t118 = Parser_peek(self);
            if ((_t118.tag == Token_Dedent || _t118.tag == Token_Eof)) {
#line 828 "src/parser.tr"
                going = false;
            } else if (1) {
                __auto_type _ = _t118;
#line 830 "src/parser.tr"
                /* pass */
            }
#line 831 "src/parser.tr"
            if (going) {
#line 832 "src/parser.tr"
                long long _sline = Parser_cur_line(self);
#line 833 "src/parser.tr"
                __auto_type s = Parser_parse_stmt(self);
#line 834 "src/parser.tr"
                Block_push(b, box_stmt(Stmt_ctor_SLine(_sline)));
#line 835 "src/parser.tr"
                Block_push(b, s);
            }
        }
    }
#line 836 "src/parser.tr"
    __auto_type _t119 = Parser_peek(self);
    if (_t119.tag == Token_Dedent) {
#line 838 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t119;
#line 840 "src/parser.tr"
        /* pass */
    }
#line 841 "src/parser.tr"
    return b;
}

__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self) {
#line 846 "src/parser.tr"
    __auto_type _t120 = Parser_peek(self);
    if (_t120.tag == Token_KwReturn) {
#line 848 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 849 "src/parser.tr"
        __auto_type _t121 = Parser_peek(self);
        if ((_t121.tag == Token_Newline || _t121.tag == Token_Dedent || _t121.tag == Token_Eof)) {
#line 851 "src/parser.tr"
            return box_stmt(Stmt_ctor_SReturn((Expr*)(0LL)));
        } else if (1) {
            __auto_type _ = _t121;
#line 853 "src/parser.tr"
            /* pass */
        }
#line 854 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 855 "src/parser.tr"
        Parser_expect_newline(self);
#line 856 "src/parser.tr"
        return box_stmt(Stmt_ctor_SReturn(e));
    } else if (_t120.tag == Token_KwPass) {
#line 858 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 859 "src/parser.tr"
        Parser_expect_newline(self);
#line 860 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t120.tag == Token_KwBreak) {
#line 862 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 865 "src/parser.tr"
        Expr* brk_val = (Expr*)(0LL);
#line 866 "src/parser.tr"
        __auto_type _t122 = Parser_peek(self);
        if (_t122.tag == Token_Newline) {
            /* pass */
        } else if (_t122.tag == Token_Dedent) {
            /* pass */
        } else if (_t122.tag == Token_Eof) {
            /* pass */
        } else if (_t122.tag == Token_Semicolon) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t122;
#line 872 "src/parser.tr"
            brk_val = Parser_parse_expr(self);
        }
#line 873 "src/parser.tr"
        Parser_expect_newline(self);
#line 874 "src/parser.tr"
        return box_stmt(Stmt_ctor_SBreak(brk_val));
    } else if (_t120.tag == Token_KwContinue) {
#line 876 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 877 "src/parser.tr"
        Parser_expect_newline(self);
#line 878 "src/parser.tr"
        return box_stmt(Stmt_make_SContinue());
    } else if (_t120.tag == Token_KwRaise) {
#line 880 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 881 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 882 "src/parser.tr"
        Parser_expect_newline(self);
#line 883 "src/parser.tr"
        return box_stmt(Stmt_ctor_SRaise(e));
    } else if (_t120.tag == Token_KwAsm) {
#line 885 "src/parser.tr"
        return Parser_parse_asm_stmt(self);
    } else if (_t120.tag == Token_KwSpawn) {
#line 887 "src/parser.tr"
        return Parser_parse_spawn_stmt(self);
    } else if (_t120.tag == Token_KwTaskGroup) {
#line 889 "src/parser.tr"
        return Parser_parse_taskgroup_stmt(self);
    } else if (_t120.tag == Token_KwGpu) {
#line 891 "src/parser.tr"
        return Parser_parse_gpu_stmt(self);
    } else if (_t120.tag == Token_KwIf) {
#line 893 "src/parser.tr"
        return Parser_parse_if_stmt(self);
    } else if (_t120.tag == Token_KwWhile) {
#line 895 "src/parser.tr"
        return Parser_parse_while_stmt(self);
    } else if (_t120.tag == Token_KwLoop) {
#line 897 "src/parser.tr"
        return Parser_parse_loop_stmt(self);
    } else if (_t120.tag == Token_KwFor) {
#line 899 "src/parser.tr"
        return Parser_parse_for_stmt(self);
    } else if (_t120.tag == Token_KwMatch) {
#line 901 "src/parser.tr"
        return Parser_parse_match_stmt(self);
    } else if (_t120.tag == Token_KwMut) {
#line 903 "src/parser.tr"
        return Parser_parse_let_stmt(self, true);
    } else if (_t120.tag == Token_KwShared) {
#line 905 "src/parser.tr"
        return Parser_parse_shared_let_stmt(self);
    } else if (_t120.tag == Token_KwConst) {
#line 907 "src/parser.tr"
        return Parser_parse_const_let_stmt(self);
    } else if (_t120.tag == Token_KwUnsafe) {
#line 909 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 910 "src/parser.tr"
        __auto_type _t123 = Parser_peek(self);
        if (_t123.tag == Token_Colon) {
#line 912 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t123;
#line 914 "src/parser.tr"
            /* pass */
        }
#line 915 "src/parser.tr"
        __auto_type body = Parser_parse_block(self);
#line 916 "src/parser.tr"
        Stmt* _trret17 = box_stmt(Stmt_ctor_SUnsafe(body));
        return _trret17;
    } else if (_t120.tag == Token_KwDefer) {
#line 919 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 920 "src/parser.tr"
        Stmt* deferred = Parser_parse_stmt(self);
#line 921 "src/parser.tr"
        return box_stmt(Stmt_ctor_SDefer(deferred));
    } else if (_t120.tag == Token_KwWith) {
#line 923 "src/parser.tr"
        return Parser_parse_with_stmt(self);
    } else if (_t120.tag == Token_KwTry) {
#line 925 "src/parser.tr"
        return Parser_parse_try_stmt(self);
    } else if (_t120.tag == Token_KwAssert) {
#line 927 "src/parser.tr"
        return Parser_parse_assert_stmt(self);
    } else if (_t120.tag == Token_KwFrom) {
#line 930 "src/parser.tr"
        Parser_parse_from_import(self);
#line 931 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t120.tag == Token_KwImport) {
#line 933 "src/parser.tr"
        Parser_parse_import(self);
#line 934 "src/parser.tr"
        return box_stmt(Stmt_make_SPass());
    } else if (_t120.tag == Token_KwClass) {
#line 940 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t120.tag == Token_KwEnum) {
#line 942 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t120.tag == Token_KwInterface) {
#line 944 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t120.tag == Token_KwExtend) {
#line 946 "src/parser.tr"
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t120.tag == Token_KwDef) {
#line 948 "src/parser.tr"
        long long _ldline = Parser_cur_line(self);
#line 949 "src/parser.tr"
        FunctionDef* _ldf = Parser_parse_function_def(self, false);
#line 950 "src/parser.tr"
        _ldf->line = _ldline;
#line 951 "src/parser.tr"
        Stmt* _trret18 = box_stmt(Stmt_ctor_SLocalDecl(box_decl(Decl_ctor_DFunction(_ldf))));
        _tr_obj_release(_ldf, _trdrop_FunctionDef);
        return _trret18;
    } else if (1) {
        __auto_type _ = _t120;
#line 953 "src/parser.tr"
        /* pass */
    }
#line 954 "src/parser.tr"
    __auto_type _t124 = Parser_peek(self);
    if (_t124.tag == Token_Ident) {
        __auto_type cs_nm = _t124.data.Ident.name;
#line 956 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("type", 4LL)))) {
#line 960 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 961 "src/parser.tr"
            TrStr _ta_name = Parser_consume_ident(self);
#line 962 "src/parser.tr"
            if (({ __auto_type _ord_t125 = (Parser_peek(self)); __auto_type _wr = ((_ord_t125.tag == Token_make_Eq().tag)); _wr; })) {
#line 963 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 964 "src/parser.tr"
                AstType* _ta_ty = Parser_parse_type(self);
#line 965 "src/parser.tr"
                Parser_expect_newline(self);
#line 966 "src/parser.tr"
                Stmt* _trret19 = box_stmt(Stmt_make_SPass());
                _tr_str_release(_ta_name);
                return _trret19;
            }
        }
#line 967 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("chan_select", 11LL)))) {
#line 968 "src/parser.tr"
            Stmt* _trret20 = Parser_parse_chan_select_stmt(self);
            return _trret20;
        }
#line 969 "src/parser.tr"
        if (_tr_str_eqv((cs_nm), (_tr_str_lit_len("var", 3LL)))) {
#line 976 "src/parser.tr"
            long long _var_save = self->pos;
#line 977 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 978 "src/parser.tr"
            __auto_type _t126 = Parser_peek(self);
            if (_t126.tag == Token_Ident) {
                __auto_type _ = _t126.data.Ident.name;
#line 980 "src/parser.tr"
                Stmt* _trret21 = Parser_parse_assign_or_expr_stmt(self);
                return _trret21;
            } else if (1) {
                __auto_type _ = _t126;
#line 982 "src/parser.tr"
                self->pos = _var_save;
            }
        }
    } else if (1) {
        __auto_type _ = _t124;
        /* pass */
    }
#line 984 "src/parser.tr"
    return Parser_parse_assign_or_expr_stmt(self);
}

__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self) {
#line 987 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 988 "src/parser.tr"
    __auto_type _t127 = Parser_peek(self);
    if (_t127.tag == Token_Colon) {
#line 990 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t127;
#line 992 "src/parser.tr"
        /* pass */
    }
#line 993 "src/parser.tr"
    __auto_type try_body = Parser_parse_block(self);
#line 994 "src/parser.tr"
    List_ptr* catches = (void*)List_ptr_new();
#line 995 "src/parser.tr"
    while (({ __auto_type _ord_t128 = (Parser_peek(self)); __auto_type _wr = ((_ord_t128.tag == Token_make_KwExcept().tag)); _wr; })) {
#line 996 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 997 "src/parser.tr"
        AstType** err_type = (AstType**)(0LL);
#line 998 "src/parser.tr"
        TrStr err_name = _tr_str_lit_len("", 0LL);
#line 999 "src/parser.tr"
        if (({ __auto_type _ord_t129 = (Parser_peek(self)); __auto_type _wr = ((_ord_t129.tag != Token_make_Colon().tag)); _wr; })) {
#line 1001 "src/parser.tr"
            bool is_bare_var = false;
#line 1002 "src/parser.tr"
            __auto_type _t130 = Parser_peek(self);
            if (_t130.tag == Token_Ident) {
                __auto_type _ = _t130.data.Ident.name;
#line 1004 "src/parser.tr"
                long long save_pos = self->pos;
#line 1005 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1006 "src/parser.tr"
                if (({ __auto_type _ord_t131 = (Parser_peek(self)); __auto_type _wr = ((_ord_t131.tag == Token_make_Colon().tag)); _wr; })) {
#line 1007 "src/parser.tr"
                    is_bare_var = true;
                }
#line 1008 "src/parser.tr"
                self->pos = save_pos;
            } else if (1) {
                __auto_type _ = _t130;
#line 1010 "src/parser.tr"
                /* pass */
            }
#line 1011 "src/parser.tr"
            if (is_bare_var) {
#line 1012 "src/parser.tr"
                TrStr _strtmp_t132 = Parser_consume_ident(self);
                _tr_str_release(err_name);
                err_name = _strtmp_t132;
            } else {
#line 1014 "src/parser.tr"
                err_type = box_asttype(Parser_parse_type(self));
#line 1015 "src/parser.tr"
                if (({ __auto_type _ord_t133 = (Parser_peek(self)); __auto_type _wr = ((_ord_t133.tag == Token_make_KwAs().tag)); _wr; })) {
#line 1016 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1017 "src/parser.tr"
                    TrStr _strtmp_t134 = Parser_consume_ident(self);
                    _tr_str_release(err_name);
                    err_name = _strtmp_t134;
                }
            }
        }
#line 1018 "src/parser.tr"
        __auto_type _t135 = Parser_peek(self);
        if (_t135.tag == Token_Colon) {
#line 1020 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t135;
#line 1022 "src/parser.tr"
            /* pass */
        }
#line 1023 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 1024 "src/parser.tr"
        CatchClause* c = CatchClause_init(err_name, (Block**)(0LL));
#line 1025 "src/parser.tr"
        /* unsafe block */
#line 1026 "src/parser.tr"
        Block** body_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1027 "src/parser.tr"
        (*body_ptr = body);
#line 1028 "src/parser.tr"
        c->body = body_ptr;
#line 1029 "src/parser.tr"
        c->err_type = err_type;
#line 1030 "src/parser.tr"
        /* unsafe block */
#line 1031 "src/parser.tr"
        CatchClause** c_ptr = ((CatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(CatchClause*)));
#line 1032 "src/parser.tr"
        (*c_ptr = c);
#line 1033 "src/parser.tr"
        List_ptr_append(catches, c_ptr);
        _tr_str_release(err_name);
    }
#line 1035 "src/parser.tr"
    Block* finally_b = Block_init();
#line 1036 "src/parser.tr"
    if (({ __auto_type _ord_t136 = (Parser_peek(self)); __auto_type _wr = ((_ord_t136.tag == Token_make_KwFinally().tag)); _wr; })) {
#line 1037 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1038 "src/parser.tr"
        __auto_type _t137 = Parser_peek(self);
        if (_t137.tag == Token_Colon) {
#line 1040 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t137;
#line 1042 "src/parser.tr"
            /* pass */
        }
#line 1043 "src/parser.tr"
        finally_b = Parser_parse_block(self);
    } else if ((catches->len == 0LL)) {
#line 1045 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("'try' block has no 'except' or 'finally' clause", 47LL), _tr_str_lit_len("add 'except:' (or 'except Type as e:') after the try block, or a 'finally:' block.", 82LL));
    }
#line 1046 "src/parser.tr"
    Stmt* _trret22 = box_stmt(Stmt_ctor_STry(try_body, catches, finally_b));
    return _trret22;
}

__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self) {
#line 1049 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1050 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1051 "src/parser.tr"
    Expr* msg = (Expr*)(0LL);
#line 1052 "src/parser.tr"
    if (({ __auto_type _ord_t138 = (Parser_peek(self)); __auto_type _wr = ((_ord_t138.tag == Token_make_Comma().tag)); _wr; })) {
#line 1053 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1054 "src/parser.tr"
        msg = Parser_parse_expr(self);
    }
#line 1055 "src/parser.tr"
    Parser_expect_newline(self);
#line 1056 "src/parser.tr"
    return box_stmt(Stmt_ctor_SAssert(cond, msg));
}

__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self) {
#line 1059 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1060 "src/parser.tr"
    List_ptr* items = (void*)List_ptr_new();
#line 1061 "src/parser.tr"
    List_TrStr* aliases = (void*)List_TrStr_new();
#line 1062 "src/parser.tr"
    bool going = true;
#line 1063 "src/parser.tr"
    while (going) {
#line 1064 "src/parser.tr"
        Expr* item_e = Parser_parse_expr(self);
#line 1065 "src/parser.tr"
        TrStr alias = _tr_str_lit_len("", 0LL);
#line 1067 "src/parser.tr"
        if ((((unsigned long long)(item_e)) != ((unsigned long long)(0LL)))) {
#line 1068 "src/parser.tr"
            __auto_type _t139 = (*item_e);
            if (_t139.tag == Expr_ECast) {
                __auto_type cast_inner = _t139.data.ECast.expr;
__auto_type cast_ty_ptr = _t139.data.ECast.ty;
#line 1070 "src/parser.tr"
                if ((((unsigned long long)(cast_ty_ptr)) != ((unsigned long long)(0LL)))) {
#line 1071 "src/parser.tr"
                    TrStr _strtmp_t140 = _tr_str_retain((*cast_ty_ptr)->name);
                    _tr_str_release(alias);
                    alias = _strtmp_t140;
                }
#line 1072 "src/parser.tr"
                item_e = cast_inner;
            } else if (1) {
                __auto_type _ = _t139;
                /* pass */
            }
        }
#line 1074 "src/parser.tr"
        List_ptr_append(items, item_e);
#line 1075 "src/parser.tr"
        List_TrStr_append(aliases, alias);
#line 1076 "src/parser.tr"
        if (({ __auto_type _ord_t141 = (Parser_peek(self)); __auto_type _wr = ((_ord_t141.tag == Token_make_Comma().tag)); _wr; })) {
#line 1077 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 1079 "src/parser.tr"
            going = false;
        }
        _tr_str_release(alias);
    }
#line 1080 "src/parser.tr"
    __auto_type _t142 = Parser_peek(self);
    if (_t142.tag == Token_Colon) {
#line 1082 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t142;
#line 1084 "src/parser.tr"
        /* pass */
    }
#line 1085 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1086 "src/parser.tr"
    Stmt* _trret23 = box_stmt(Stmt_ctor_SWith(items, aliases, body));
    return _trret23;
}

__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self) {
#line 1089 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1090 "src/parser.tr"
    __auto_type _t143 = Parser_peek(self);
    if (_t143.tag == Token_LParen) {
#line 1092 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t143;
#line 1094 "src/parser.tr"
        /* pass */
    }
#line 1095 "src/parser.tr"
    List_TrStr* parts = (void*)List_TrStr_new();
#line 1097 "src/parser.tr"
    bool reading = true;
#line 1098 "src/parser.tr"
    while (reading) {
#line 1099 "src/parser.tr"
        __auto_type _t144 = Parser_peek(self);
        if (_t144.tag == Token_StrLit) {
            __auto_type s = _t144.data.StrLit.val;
#line 1101 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1102 "src/parser.tr"
            List_TrStr_append(parts, s);
#line 1103 "src/parser.tr"
            __auto_type _t145 = Parser_peek(self);
            if (_t145.tag == Token_Comma) {
#line 1105 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t145;
                reading = false;
            }
        } else if (1) {
            __auto_type _ = _t144;
            reading = false;
        }
    }
#line 1108 "src/parser.tr"
    __auto_type _t146 = Parser_peek(self);
    if (_t146.tag == Token_RParen) {
#line 1110 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t146;
#line 1112 "src/parser.tr"
        /* pass */
    }
#line 1113 "src/parser.tr"
    Parser_expect_newline(self);
#line 1114 "src/parser.tr"
    TrStr code = _tr_str_lit_len("", 0LL);
#line 1115 "src/parser.tr"
    TrStr outs = _tr_str_lit_len("", 0LL);
#line 1116 "src/parser.tr"
    TrStr ins = _tr_str_lit_len("", 0LL);
#line 1117 "src/parser.tr"
    TrStr clob = _tr_str_lit_len("", 0LL);
#line 1118 "src/parser.tr"
    if ((parts->len > 0LL)) {
#line 1118 "src/parser.tr"
        TrStr _strtmp_t147 = List_TrStr_get(parts, 0LL);
        _tr_str_release(code);
        code = _strtmp_t147;
    }
#line 1119 "src/parser.tr"
    if ((parts->len > 1LL)) {
#line 1119 "src/parser.tr"
        TrStr _strtmp_t148 = List_TrStr_get(parts, 1LL);
        _tr_str_release(outs);
        outs = _strtmp_t148;
    }
#line 1120 "src/parser.tr"
    if ((parts->len > 2LL)) {
#line 1120 "src/parser.tr"
        TrStr _strtmp_t149 = List_TrStr_get(parts, 2LL);
        _tr_str_release(ins);
        ins = _strtmp_t149;
    }
#line 1121 "src/parser.tr"
    if ((parts->len > 3LL)) {
#line 1121 "src/parser.tr"
        TrStr _strtmp_t150 = List_TrStr_get(parts, 3LL);
        _tr_str_release(clob);
        clob = _strtmp_t150;
    }
#line 1122 "src/parser.tr"
    Stmt* _trret24 = box_stmt(Stmt_ctor_SAsm(code, outs, ins, clob));
    List_TrStr_free(parts);
    _tr_str_release(code);
    _tr_str_release(outs);
    _tr_str_release(ins);
    _tr_str_release(clob);
    return _trret24;
}

__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self) {
#line 1125 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1126 "src/parser.tr"
    __auto_type e = Parser_parse_expr(self);
#line 1127 "src/parser.tr"
    Parser_expect_newline(self);
#line 1128 "src/parser.tr"
    return box_stmt(Stmt_ctor_SSpawn(e));
}

__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self) {
#line 1131 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1132 "src/parser.tr"
    __auto_type _t151 = Parser_peek(self);
    if (_t151.tag == Token_Colon) {
#line 1134 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t151;
#line 1136 "src/parser.tr"
        /* pass */
    }
#line 1137 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1138 "src/parser.tr"
    Stmt* _trret25 = box_stmt(Stmt_ctor_STaskGroup(body));
    return _trret25;
}

__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self) {
#line 1146 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1147 "src/parser.tr"
    if (({ __auto_type _ord_t152 = (Parser_peek(self)); __auto_type _wr = ((_ord_t152.tag == Token_make_Colon().tag)); _wr; })) {
#line 1147 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1148 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1149 "src/parser.tr"
    if (({ __auto_type _ord_t153 = (Parser_peek(self)); __auto_type _wr = ((_ord_t153.tag == Token_make_Indent().tag)); _wr; })) {
#line 1149 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1150 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1151 "src/parser.tr"
    List_ptr* cases = (void*)List_ptr_new();
#line 1152 "src/parser.tr"
    bool going = true;
#line 1153 "src/parser.tr"
    while (going) {
#line 1154 "src/parser.tr"
        __auto_type _t154 = Parser_peek(self);
        if (_t154.tag == Token_KwCase) {
#line 1156 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1158 "src/parser.tr"
            long long arm_kind = (-1LL);
#line 1159 "src/parser.tr"
            TrStr var_nm = _tr_str_lit_len("", 0LL);
#line 1160 "src/parser.tr"
            Expr* chan_e = (Expr*)(0LL);
#line 1161 "src/parser.tr"
            Expr* val_e = (Expr*)(0LL);
#line 1162 "src/parser.tr"
            Expr* timeout_e = (Expr*)(0LL);
#line 1164 "src/parser.tr"
            __auto_type _t155 = Parser_peek(self);
            if (_t155.tag == Token_Ident) {
                __auto_type arm_nm = _t155.data.Ident.name;
#line 1166 "src/parser.tr"
                if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("default", 7LL)))) {
#line 1167 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1168 "src/parser.tr"
                    if (({ __auto_type _ord_t156 = (Parser_peek(self)); __auto_type _wr = ((_ord_t156.tag == Token_make_Colon().tag)); _wr; })) {
#line 1168 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1169 "src/parser.tr"
                    arm_kind = 3LL;
                } else if (_tr_str_eqv((arm_nm), (_tr_str_lit_len("timeout", 7LL)))) {
#line 1172 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1173 "src/parser.tr"
                    if (({ __auto_type _ord_t157 = (Parser_peek(self)); __auto_type _wr = ((_ord_t157.tag == Token_make_LParen().tag)); _wr; })) {
#line 1173 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1174 "src/parser.tr"
                    timeout_e = Parser_parse_expr(self);
#line 1175 "src/parser.tr"
                    if (({ __auto_type _ord_t158 = (Parser_peek(self)); __auto_type _wr = ((_ord_t158.tag == Token_make_RParen().tag)); _wr; })) {
#line 1175 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1176 "src/parser.tr"
                    if (({ __auto_type _ord_t159 = (Parser_peek(self)); __auto_type _wr = ((_ord_t159.tag == Token_make_Colon().tag)); _wr; })) {
#line 1176 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
#line 1177 "src/parser.tr"
                    arm_kind = 2LL;
                } else {
#line 1181 "src/parser.tr"
                    long long saved = self->pos;
#line 1182 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1183 "src/parser.tr"
                    __auto_type _t160 = Parser_peek(self);
                    if (_t160.tag == Token_Eq) {
#line 1186 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1187 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1188 "src/parser.tr"
                        if (({ __auto_type _ord_t161 = (Parser_peek(self)); __auto_type _wr = ((_ord_t161.tag == Token_make_Colon().tag)); _wr; })) {
#line 1188 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1189 "src/parser.tr"
                        TrStr _strtmp_t162 = _tr_str_retain(arm_nm);
                        _tr_str_release(var_nm);
                        var_nm = _strtmp_t162;
#line 1190 "src/parser.tr"
                        arm_kind = 0LL;
                    } else if (1) {
                        __auto_type _ = _t160;
#line 1193 "src/parser.tr"
                        self->pos = saved;
#line 1194 "src/parser.tr"
                        chan_e = Parser_parse_expr(self);
#line 1195 "src/parser.tr"
                        if (({ __auto_type _ord_t163 = (Parser_peek(self)); __auto_type _wr = ((_ord_t163.tag == Token_make_Colon().tag)); _wr; })) {
#line 1195 "src/parser.tr"
                            self->pos = (self->pos + 1LL);
                        }
#line 1196 "src/parser.tr"
                        arm_kind = 1LL;
                    }
                }
            } else if (1) {
                __auto_type _ = _t155;
#line 1199 "src/parser.tr"
                chan_e = Parser_parse_expr(self);
#line 1200 "src/parser.tr"
                if (({ __auto_type _ord_t164 = (Parser_peek(self)); __auto_type _wr = ((_ord_t164.tag == Token_make_Colon().tag)); _wr; })) {
#line 1200 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1201 "src/parser.tr"
                arm_kind = 1LL;
            }
#line 1202 "src/parser.tr"
            Block* arm_body = Parser_parse_block(self);
#line 1203 "src/parser.tr"
            /* unsafe block */
#line 1204 "src/parser.tr"
            ChanSelectArm** arm_p = ((ChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(ChanSelectArm*)));
#line 1205 "src/parser.tr"
            if ((arm_kind == 0LL)) {
#line 1206 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_recv(chan_e, var_nm, arm_body));
            } else if ((arm_kind == 1LL)) {
#line 1208 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_send(chan_e, (Expr*)(0LL), arm_body));
            } else if ((arm_kind == 2LL)) {
#line 1210 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_timeout(timeout_e, arm_body));
            } else {
#line 1212 "src/parser.tr"
                (*arm_p = ChanSelectArm_init_default(arm_body));
            }
#line 1213 "src/parser.tr"
            List_ptr_append(cases, arm_p);
            _tr_str_release(var_nm);
        } else if ((_t154.tag == Token_Dedent || _t154.tag == Token_Eof)) {
#line 1215 "src/parser.tr"
            going = false;
        } else if (_t154.tag == Token_Newline) {
#line 1217 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t154;
#line 1219 "src/parser.tr"
            going = false;
        }
    }
#line 1220 "src/parser.tr"
    if (({ __auto_type _ord_t165 = (Parser_peek(self)); __auto_type _wr = ((_ord_t165.tag == Token_make_Dedent().tag)); _wr; })) {
#line 1220 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1221 "src/parser.tr"
    return box_stmt(Stmt_ctor_SChanSelect(cases));
}

__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self) {
#line 1224 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1225 "src/parser.tr"
    __auto_type _t166 = Parser_peek(self);
    if (_t166.tag == Token_Colon) {
#line 1227 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t166;
#line 1229 "src/parser.tr"
        /* pass */
    }
#line 1230 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1231 "src/parser.tr"
    Stmt* _trret26 = box_stmt(Stmt_ctor_SGpuBlock(body));
    return _trret26;
}

__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut) {
#line 1234 "src/parser.tr"
    if (is_mut) {
#line 1235 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1237 "src/parser.tr"
    if (({ __auto_type _ord_t167 = (Parser_peek(self)); __auto_type _wr = ((_ord_t167.tag == Token_make_LParen().tag)); _wr; })) {
#line 1238 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1239 "src/parser.tr"
        List_TrStr* pnames = (void*)List_TrStr_new();
#line 1240 "src/parser.tr"
        ({ TrStr _at_t168 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t168); _tr_str_release(_at_t168); });
#line 1241 "src/parser.tr"
        while (({ __auto_type _ord_t169 = (Parser_peek(self)); __auto_type _wr = ((_ord_t169.tag == Token_make_Comma().tag)); _wr; })) {
#line 1242 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1243 "src/parser.tr"
            if (({ __auto_type _ord_t170 = (Parser_peek(self)); __auto_type _wr = ((_ord_t170.tag == Token_make_RParen().tag)); _wr; })) {
#line 1243 "src/parser.tr"
                break;
            }
#line 1244 "src/parser.tr"
            ({ TrStr _at_t171 = (Parser_consume_ident(self)); List_TrStr_append(pnames, _at_t171); _tr_str_release(_at_t171); });
        }
#line 1245 "src/parser.tr"
        if (({ __auto_type _ord_t172 = (Parser_peek(self)); __auto_type _wr = ((_ord_t172.tag == Token_make_RParen().tag)); _wr; })) {
#line 1245 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1246 "src/parser.tr"
        if (({ __auto_type _ord_t173 = (Parser_peek(self)); __auto_type _wr = ((_ord_t173.tag == Token_make_Eq().tag)); _wr; })) {
#line 1246 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1247 "src/parser.tr"
        Expr* pval = Parser_parse_expr(self);
#line 1248 "src/parser.tr"
        Parser_expect_newline(self);
#line 1249 "src/parser.tr"
        return box_stmt(Stmt_ctor_SMultiLet(pnames, is_mut, pval));
    }
#line 1250 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1252 "src/parser.tr"
    if (({ __auto_type _ord_t174 = (Parser_peek(self)); __auto_type _wr = ((_ord_t174.tag == Token_make_Comma().tag)); _wr; })) {
#line 1253 "src/parser.tr"
        List_TrStr* names = (void*)List_TrStr_new();
#line 1254 "src/parser.tr"
        List_TrStr_append(names, name);
#line 1255 "src/parser.tr"
        while (({ __auto_type _ord_t175 = (Parser_peek(self)); __auto_type _wr = ((_ord_t175.tag == Token_make_Comma().tag)); _wr; })) {
#line 1256 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1257 "src/parser.tr"
            ({ TrStr _at_t176 = (Parser_consume_ident(self)); List_TrStr_append(names, _at_t176); _tr_str_release(_at_t176); });
        }
#line 1258 "src/parser.tr"
        __auto_type _t177 = Parser_peek(self);
        if (_t177.tag == Token_Eq) {
#line 1260 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t177;
#line 1262 "src/parser.tr"
            /* pass */
        }
#line 1263 "src/parser.tr"
        Expr* val_ptr = Parser_parse_expr(self);
#line 1264 "src/parser.tr"
        Parser_expect_newline(self);
#line 1265 "src/parser.tr"
        Stmt* _trret27 = box_stmt(Stmt_ctor_SMultiLet(names, is_mut, val_ptr));
        _tr_str_release(name);
        return _trret27;
    }
#line 1266 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1267 "src/parser.tr"
    __auto_type _t178 = Parser_peek(self);
    if (_t178.tag == Token_Colon) {
#line 1269 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1270 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1271 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t178;
#line 1273 "src/parser.tr"
        /* pass */
    }
#line 1274 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1275 "src/parser.tr"
    __auto_type _t179 = Parser_peek(self);
    if (_t179.tag == Token_Eq) {
#line 1277 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1278 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t179;
#line 1280 "src/parser.tr"
        /* pass */
    }
#line 1281 "src/parser.tr"
    if (((((unsigned long long)(ty_ptr)) == ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))))) {
#line 1282 "src/parser.tr"
        ({ TrStr _at_t180 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("variable '", 10LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has neither a type nor an initial value", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t181 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("write 'mut ", 11LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Type' or 'mut ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = value'.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_prev(self, _at_t180, _at_t181); _tr_str_release(_at_t180); _tr_str_release(_at_t181); });
    }
#line 1283 "src/parser.tr"
    Parser_expect_newline(self);
#line 1284 "src/parser.tr"
    Stmt* _trret28 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), is_mut, false, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret28;
}

__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self) {
#line 1287 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1288 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1289 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1290 "src/parser.tr"
    __auto_type _t182 = Parser_peek(self);
    if (_t182.tag == Token_Colon) {
#line 1292 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1293 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1294 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t182;
#line 1296 "src/parser.tr"
        /* pass */
    }
#line 1297 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1298 "src/parser.tr"
    __auto_type _t183 = Parser_peek(self);
    if (_t183.tag == Token_Eq) {
#line 1300 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1301 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t183;
#line 1303 "src/parser.tr"
        /* pass */
    }
#line 1304 "src/parser.tr"
    Parser_expect_newline(self);
#line 1305 "src/parser.tr"
    Stmt* _trret29 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Shared(), true, false, true, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret29;
}

__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self) {
#line 1308 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1309 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 1310 "src/parser.tr"
    AstType** ty_ptr = (AstType**)(0LL);
#line 1311 "src/parser.tr"
    __auto_type _t184 = Parser_peek(self);
    if (_t184.tag == Token_Colon) {
#line 1313 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1314 "src/parser.tr"
        __auto_type ty = Parser_parse_type(self);
#line 1315 "src/parser.tr"
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t184;
#line 1317 "src/parser.tr"
        /* pass */
    }
#line 1318 "src/parser.tr"
    Expr* val_ptr = (Expr*)(0LL);
#line 1319 "src/parser.tr"
    __auto_type _t185 = Parser_peek(self);
    if (_t185.tag == Token_Eq) {
#line 1321 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1322 "src/parser.tr"
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t185;
#line 1324 "src/parser.tr"
        /* pass */
    }
#line 1325 "src/parser.tr"
    Parser_expect_newline(self);
#line 1326 "src/parser.tr"
    Stmt* _trret30 = box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, true, false, ty_ptr, val_ptr));
    _tr_str_release(name);
    return _trret30;
}

__attribute__((hot)) void Parser_require_colon_after_cond(Parser* self, TrStr kw) {
#line 1330 "src/parser.tr"
    long long _cc_ln = Parser_cur_line(self);
#line 1331 "src/parser.tr"
    if (((self->pos > 0LL) && ((self->pos - 1LL) < self->lines->len))) {
#line 1331 "src/parser.tr"
        _cc_ln = List_i64_get(self->lines, (self->pos - 1LL));
    }
#line 1332 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1333 "src/parser.tr"
    __auto_type _t186 = Parser_peek(self);
    if (_t186.tag == Token_Colon) {
#line 1335 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t186;
#line 1337 "src/parser.tr"
        ({ TrStr _at_t187 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("expected ':' after the condition of '", 37LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t188 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("write '", 7LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" condition:' followed by an indented block.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, _cc_ln, 0LL, _at_t187, _at_t188); _tr_str_release(_at_t187); _tr_str_release(_at_t188); });
    }
}

__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self) {
#line 1340 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1341 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1342 "src/parser.tr"
    Parser_require_colon_after_cond(self, _tr_str_lit_len("if", 2LL));
#line 1343 "src/parser.tr"
    __auto_type then_b = Parser_parse_block(self);
#line 1344 "src/parser.tr"
    List_ptr* elifs = (void*)List_ptr_new();
#line 1345 "src/parser.tr"
    Block* else_b = Block_init();
#line 1346 "src/parser.tr"
    bool going = true;
#line 1347 "src/parser.tr"
    while (going) {
#line 1348 "src/parser.tr"
        going = false;
#line 1349 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1350 "src/parser.tr"
        __auto_type _t189 = Parser_peek(self);
        if (_t189.tag == Token_KwElif) {
#line 1352 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1353 "src/parser.tr"
            __auto_type ec = Parser_parse_expr(self);
#line 1354 "src/parser.tr"
            Parser_require_colon_after_cond(self, _tr_str_lit_len("elif", 4LL));
#line 1355 "src/parser.tr"
            __auto_type eb = Parser_parse_block(self);
#line 1356 "src/parser.tr"
            /* unsafe block */
#line 1357 "src/parser.tr"
            Block** eb_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1358 "src/parser.tr"
            (*eb_ptr = eb);
#line 1359 "src/parser.tr"
            List_ptr_append(elifs, ElifClause_init(ec, eb_ptr));
#line 1360 "src/parser.tr"
            going = true;
        } else if (_t189.tag == Token_KwElse) {
#line 1362 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1363 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1364 "src/parser.tr"
            __auto_type _t190 = Parser_peek(self);
            if (_t190.tag == Token_Colon) {
#line 1366 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t190;
#line 1368 "src/parser.tr"
                /* pass */
            }
#line 1369 "src/parser.tr"
            else_b = Parser_parse_block(self);
        } else if (1) {
            __auto_type _ = _t189;
#line 1371 "src/parser.tr"
            /* pass */
        }
    }
#line 1372 "src/parser.tr"
    Stmt* _trret31 = box_stmt(Stmt_ctor_SIf(cond, then_b, elifs, else_b));
    return _trret31;
}

__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self) {
#line 1375 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1376 "src/parser.tr"
    __auto_type cond = Parser_parse_expr(self);
#line 1377 "src/parser.tr"
    Parser_require_colon_after_cond(self, _tr_str_lit_len("while", 5LL));
#line 1378 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1379 "src/parser.tr"
    Stmt* _trret32 = box_stmt(Stmt_ctor_SWhile(cond, body, (void*)List_ptr_new()));
    return _trret32;
}

__attribute__((hot)) Stmt* Parser_parse_loop_stmt(Parser* self) {
#line 1383 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1384 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1385 "src/parser.tr"
    if (({ __auto_type _ord_t191 = (Parser_peek(self)); __auto_type _wr = ((_ord_t191.tag == Token_make_Colon().tag)); _wr; })) {
#line 1385 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1386 "src/parser.tr"
    Block* body = Parser_parse_block(self);
#line 1387 "src/parser.tr"
    Stmt* _trret33 = ({ __auto_type _ord_t192 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t193 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SWhile(_ord_t192, body, _ord_t193))); _wr; });
    return _trret33;
}

__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self) {
#line 1390 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1393 "src/parser.tr"
    bool for_is_ref = false;
#line 1394 "src/parser.tr"
    if (({ __auto_type _ord_t194 = (Parser_peek(self)); __auto_type _wr = ((_ord_t194.tag == Token_make_KwMut().tag)); _wr; })) {
#line 1395 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1396 "src/parser.tr"
        TrStr _mr = Parser_consume_ident(self);
#line 1397 "src/parser.tr"
        for_is_ref = true;
    }
#line 1398 "src/parser.tr"
    TrStr var = Parser_consume_ident(self);
#line 1399 "src/parser.tr"
    if (((!for_is_ref) && _tr_str_eqv((var), (_tr_str_lit_len("ref", 3LL))))) {
#line 1400 "src/parser.tr"
        __auto_type _t195 = Parser_peek(self);
        if (_t195.tag == Token_KwIn) {
            /* pass */
        } else if (_t195.tag == Token_Comma) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t195;
#line 1404 "src/parser.tr"
            for_is_ref = true;
#line 1405 "src/parser.tr"
            TrStr _strtmp_t196 = Parser_consume_ident(self);
            _tr_str_release(var);
            var = _strtmp_t196;
        }
    }
#line 1407 "src/parser.tr"
    if (({ __auto_type _ord_t197 = (Parser_peek(self)); __auto_type _wr = ((_ord_t197.tag == Token_make_Comma().tag)); _wr; })) {
#line 1408 "src/parser.tr"
        List_TrStr* unpack_vars = (void*)List_TrStr_new();
#line 1409 "src/parser.tr"
        List_TrStr_append(unpack_vars, var);
#line 1410 "src/parser.tr"
        while (({ __auto_type _ord_t198 = (Parser_peek(self)); __auto_type _wr = ((_ord_t198.tag == Token_make_Comma().tag)); _wr; })) {
#line 1411 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1412 "src/parser.tr"
            ({ TrStr _at_t199 = (Parser_consume_ident(self)); List_TrStr_append(unpack_vars, _at_t199); _tr_str_release(_at_t199); });
        }
#line 1413 "src/parser.tr"
        if (({ __auto_type _ord_t200 = (Parser_peek(self)); __auto_type _wr = ((_ord_t200.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1413 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1414 "src/parser.tr"
        Expr* fu_iter = Parser_parse_expr(self);
#line 1415 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1416 "src/parser.tr"
        if (({ __auto_type _ord_t201 = (Parser_peek(self)); __auto_type _wr = ((_ord_t201.tag == Token_make_Colon().tag)); _wr; })) {
#line 1416 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1417 "src/parser.tr"
        Block* fu_body = Parser_parse_block(self);
#line 1418 "src/parser.tr"
        Stmt* _trret34 = box_stmt(Stmt_ctor_SForUnpack(unpack_vars, fu_iter, fu_body));
        _tr_str_release(var);
        return _trret34;
    }
#line 1419 "src/parser.tr"
    __auto_type _t202 = Parser_peek(self);
    if (_t202.tag == Token_KwIn) {
#line 1421 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t202;
#line 1423 "src/parser.tr"
        /* pass */
    }
#line 1424 "src/parser.tr"
    __auto_type iter_e = Parser_parse_expr(self);
#line 1425 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1426 "src/parser.tr"
    __auto_type _t203 = Parser_peek(self);
    if (_t203.tag == Token_Colon) {
#line 1428 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t203;
#line 1430 "src/parser.tr"
        /* pass */
    }
#line 1431 "src/parser.tr"
    __auto_type body = Parser_parse_block(self);
#line 1432 "src/parser.tr"
    Stmt* _trret35 = box_stmt(Stmt_ctor_SFor(var, iter_e, body, (void*)List_ptr_new(), for_is_ref));
    _tr_str_release(var);
    return _trret35;
}

__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self) {
#line 1435 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1436 "src/parser.tr"
    __auto_type subj = Parser_parse_expr(self);
#line 1437 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1438 "src/parser.tr"
    __auto_type _t204 = Parser_peek(self);
    if (_t204.tag == Token_Colon) {
#line 1440 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t204;
#line 1442 "src/parser.tr"
        /* pass */
    }
#line 1443 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1444 "src/parser.tr"
    return box_stmt(Stmt_ctor_SMatch(subj, arms));
}

__attribute__((hot)) List_ptr* Parser_parse_match_arms(Parser* self) {
#line 1449 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1450 "src/parser.tr"
    __auto_type _t205 = Parser_peek(self);
    if (_t205.tag == Token_Indent) {
#line 1452 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t205;
#line 1454 "src/parser.tr"
        /* pass */
    }
#line 1455 "src/parser.tr"
    List_ptr* arms = (void*)List_ptr_new();
#line 1456 "src/parser.tr"
    bool going = true;
#line 1457 "src/parser.tr"
    while (going) {
#line 1458 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1459 "src/parser.tr"
        __auto_type _t206 = Parser_peek(self);
        if ((_t206.tag == Token_Dedent || _t206.tag == Token_Eof || _t206.tag == Token_RParen || _t206.tag == Token_RBracket || _t206.tag == Token_RBrace)) {
#line 1464 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t206;
#line 1466 "src/parser.tr"
            /* pass */
        }
#line 1467 "src/parser.tr"
        if (going) {
#line 1468 "src/parser.tr"
            Pattern pat = Parser_parse_pattern(self);
#line 1469 "src/parser.tr"
            if (({ __auto_type _ord_t207 = (Parser_peek(self)); __auto_type _wr = ((_ord_t207.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1470 "src/parser.tr"
                List_Pattern* or_pats = (void*)List_Pattern_new();
#line 1471 "src/parser.tr"
                List_Pattern_append(or_pats, pat);
#line 1472 "src/parser.tr"
                while (({ __auto_type _ord_t208 = (Parser_peek(self)); __auto_type _wr = ((_ord_t208.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1473 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 1474 "src/parser.tr"
                    List_Pattern_append(or_pats, Parser_parse_pattern(self));
                }
#line 1475 "src/parser.tr"
                pat = Pattern_ctor_POr(or_pats);
            }
#line 1477 "src/parser.tr"
            Expr* guard_expr = (Expr*)(0LL);
#line 1478 "src/parser.tr"
            if (({ __auto_type _ord_t209 = (Parser_peek(self)); __auto_type _wr = ((_ord_t209.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1479 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1480 "src/parser.tr"
                guard_expr = Parser_parse_expr(self);
            }
#line 1481 "src/parser.tr"
            __auto_type _t210 = Parser_peek(self);
            if (_t210.tag == Token_Colon) {
#line 1483 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t210;
#line 1485 "src/parser.tr"
                /* pass */
            }
#line 1486 "src/parser.tr"
            Block* arm_block = Block_init();
#line 1487 "src/parser.tr"
            bool is_block_arm = false;
#line 1488 "src/parser.tr"
            __auto_type _t211 = Parser_peek(self);
            if ((_t211.tag == Token_Newline || _t211.tag == Token_Indent)) {
#line 1490 "src/parser.tr"
                arm_block = Parser_parse_block(self);
#line 1491 "src/parser.tr"
                is_block_arm = true;
            } else if (1) {
                __auto_type _ = _t211;
#line 1493 "src/parser.tr"
                /* pass */
            }
#line 1494 "src/parser.tr"
            if ((!is_block_arm)) {
#line 1495 "src/parser.tr"
                Block_push(arm_block, Parser_parse_stmt(self));
            }
#line 1496 "src/parser.tr"
            Block** ab_ptr = (Block**)(0LL);
#line 1497 "src/parser.tr"
            /* unsafe block */
#line 1498 "src/parser.tr"
            ab_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
#line 1499 "src/parser.tr"
            (*ab_ptr = arm_block);
#line 1500 "src/parser.tr"
            MatchArm* new_arm = MatchArm_init(pat, ab_ptr);
#line 1501 "src/parser.tr"
            new_arm->guard = guard_expr;
#line 1502 "src/parser.tr"
            List_ptr_append(arms, _tr_obj_retain(new_arm));
        }
    }
#line 1503 "src/parser.tr"
    __auto_type _t212 = Parser_peek(self);
    if (_t212.tag == Token_Dedent) {
#line 1505 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t212;
#line 1507 "src/parser.tr"
        /* pass */
    }
#line 1508 "src/parser.tr"
    return arms;
}

__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self) {
#line 1511 "src/parser.tr"
    __auto_type _t213 = Parser_peek(self);
    if (_t213.tag == Token_KwCase) {
#line 1513 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t213;
#line 1515 "src/parser.tr"
        /* pass */
    }
#line 1516 "src/parser.tr"
    __auto_type _t214 = Parser_peek(self);
    if (_t214.tag == Token_Ident) {
        __auto_type type_name = _t214.data.Ident.name;
#line 1518 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1519 "src/parser.tr"
        __auto_type _t215 = Parser_peek(self);
        if (_t215.tag == Token_Dot) {
#line 1521 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1522 "src/parser.tr"
            TrStr variant = Parser_consume_ident(self);
#line 1523 "src/parser.tr"
            __auto_type _t216 = Parser_peek(self);
            if (_t216.tag == Token_LParen) {
#line 1525 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1526 "src/parser.tr"
                List_TrStr* fields = (void*)List_TrStr_new();
#line 1527 "src/parser.tr"
                while ((({ __auto_type _ord_t217 = (Parser_peek(self)); __auto_type _wr = ((_ord_t217.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t218 = (Parser_peek(self)); __auto_type _wr = ((_ord_t218.tag != Token_make_Eof().tag)); _wr; }))) {
#line 1528 "src/parser.tr"
                    TrStr fname = Parser_consume_ident(self);
#line 1529 "src/parser.tr"
                    if (_tr_str_eqv((fname), (_tr_str_lit_len("", 0LL)))) {
#line 1530 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 1532 "src/parser.tr"
                        List_TrStr_append(fields, fname);
                    }
#line 1533 "src/parser.tr"
                    if (({ __auto_type _ord_t219 = (Parser_peek(self)); __auto_type _wr = ((_ord_t219.tag == Token_make_Comma().tag)); _wr; })) {
#line 1534 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                    _tr_str_release(fname);
                }
#line 1535 "src/parser.tr"
                if (({ __auto_type _ord_t220 = (Parser_peek(self)); __auto_type _wr = ((_ord_t220.tag == Token_make_RParen().tag)); _wr; })) {
#line 1536 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 1537 "src/parser.tr"
                if ((fields->len == 1LL)) {
#line 1538 "src/parser.tr"
                    return ({ TrStr _at_t221 = (List_TrStr_get(fields, 0LL)); __auto_type _wr = (Pattern_ctor_PVariantBind(type_name, variant, _at_t221)); _tr_str_release(_at_t221); _wr; });
                }
#line 1539 "src/parser.tr"
                return Pattern_ctor_PVariantBindMany(type_name, variant, fields);
            } else if (1) {
                __auto_type _ = _t216;
#line 1541 "src/parser.tr"
                /* pass */
            }
#line 1542 "src/parser.tr"
            return Pattern_ctor_PVariant(type_name, variant);
        } else if (1) {
            __auto_type _ = _t215;
#line 1544 "src/parser.tr"
            /* pass */
        }
#line 1545 "src/parser.tr"
        return Pattern_ctor_PBind(type_name);
    } else if (_t214.tag == Token_IntLit) {
        __auto_type v = _t214.data.IntLit.val;
#line 1547 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1548 "src/parser.tr"
        return Pattern_ctor_PLitInt(v);
    } else if (_t214.tag == Token_KwTrue) {
#line 1550 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1551 "src/parser.tr"
        return Pattern_ctor_PLitBool(true);
    } else if (_t214.tag == Token_KwFalse) {
#line 1553 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1554 "src/parser.tr"
        return Pattern_ctor_PLitBool(false);
    } else if (_t214.tag == Token_StrLit) {
        __auto_type s = _t214.data.StrLit.val;
__auto_type blen = _t214.data.StrLit.blen;
#line 1556 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1557 "src/parser.tr"
        return Pattern_ctor_PLitStr(s, blen);
    } else if (1) {
        __auto_type _ = _t214;
#line 1559 "src/parser.tr"
        /* pass */
    }
#line 1560 "src/parser.tr"
    return Pattern_make_PWild();
}

__attribute__((hot)) bool Parser__ast_simple(Parser* self, Expr* e) {
#line 1564 "src/parser.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1564 "src/parser.tr"
        return true;
    }
#line 1565 "src/parser.tr"
    __auto_type _t222 = (*e);
    if (_t222.tag == Expr_EIdent) {
        __auto_type _ = _t222.data.EIdent.name;
        return true;
    } else if (_t222.tag == Expr_ELitInt) {
        __auto_type _ = _t222.data.ELitInt.val;
        return true;
    } else if (_t222.tag == Expr_ELitStr) {
        return true;
    } else if (_t222.tag == Expr_ELitBool) {
        __auto_type _ = _t222.data.ELitBool.val;
        return true;
    } else if (_t222.tag == Expr_EPropAccess) {
        __auto_type o = _t222.data.EPropAccess.obj;
        return Parser__ast_simple(self, o);
    } else if (_t222.tag == Expr_EIndex) {
        __auto_type o = _t222.data.EIndex.obj;
__auto_type i = _t222.data.EIndex._tr_v_index;
        return (Parser__ast_simple(self, o) && Parser__ast_simple(self, i));
    } else if (_t222.tag == Expr_EBinOp) {
        __auto_type a = _t222.data.EBinOp.left;
__auto_type b = _t222.data.EBinOp.right;
        return (Parser__ast_simple(self, a) && Parser__ast_simple(self, b));
    } else if (_t222.tag == Expr_EUnaryOp) {
        __auto_type a = _t222.data.EUnaryOp.expr;
        return Parser__ast_simple(self, a);
    } else if (1) {
        __auto_type _ = _t222;
        return false;
    }
}

__attribute__((hot)) Stmt* Parser_mk_aug(Parser* self, Expr* lhs, TrStr op, Expr* rhs) {
#line 1581 "src/parser.tr"
    Stmt* plain = box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(op, lhs, rhs))));
#line 1582 "src/parser.tr"
    List_ptr* stmts = (void*)List_ptr_new();
#line 1583 "src/parser.tr"
    __auto_type _t223 = (*lhs);
    if (_t223.tag == Expr_EIndex) {
        __auto_type o = _t223.data.EIndex.obj;
__auto_type i = _t223.data.EIndex._tr_v_index;
#line 1585 "src/parser.tr"
        if ((Parser__ast_simple(self, o) && Parser__ast_simple(self, i))) {
#line 1585 "src/parser.tr"
            return plain;
        }
#line 1586 "src/parser.tr"
        Expr* o2 = o;
#line 1587 "src/parser.tr"
        Expr* i2 = i;
#line 1588 "src/parser.tr"
        if ((!Parser__ast_simple(self, o))) {
#line 1589 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1590 "src/parser.tr"
            TrStr on = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1591 "src/parser.tr"
            ({ __auto_type _ord_t224 = (Ownership_make_Own()); __auto_type _ord_t225 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(on, _ord_t224, true, false, false, _ord_t225, o))); });
#line 1592 "src/parser.tr"
            o2 = box_expr(Expr_ctor_EIdent(on));
            _tr_str_release(on);
        }
#line 1593 "src/parser.tr"
        if ((!Parser__ast_simple(self, i))) {
#line 1594 "src/parser.tr"
            self->aug_ctr = (self->aug_ctr + 1LL);
#line 1595 "src/parser.tr"
            TrStr inm = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1596 "src/parser.tr"
            ({ __auto_type _ord_t226 = (Ownership_make_Own()); __auto_type _ord_t227 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(inm, _ord_t226, true, false, false, _ord_t227, i))); });
#line 1597 "src/parser.tr"
            i2 = box_expr(Expr_ctor_EIdent(inm));
            _tr_str_release(inm);
        }
#line 1598 "src/parser.tr"
        Expr* t_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1599 "src/parser.tr"
        Expr* r_node = box_expr(Expr_ctor_EIndex(o2, i2));
#line 1600 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node, box_expr(Expr_ctor_EBinOp(op, r_node, rhs)))));
    } else if (_t223.tag == Expr_EPropAccess) {
        __auto_type o = _t223.data.EPropAccess.obj;
__auto_type f = _t223.data.EPropAccess.prop;
#line 1602 "src/parser.tr"
        if (Parser__ast_simple(self, o)) {
#line 1602 "src/parser.tr"
            Stmt* _trret36 = plain;
            return _trret36;
        }
#line 1603 "src/parser.tr"
        self->aug_ctr = (self->aug_ctr + 1LL);
#line 1604 "src/parser.tr"
        TrStr pn = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->aug_ctr)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("_aug", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1605 "src/parser.tr"
        ({ __auto_type _ord_t228 = (Ownership_make_Own()); __auto_type _ord_t229 = ((AstType**)(0LL)); List_ptr_append(stmts, box_stmt(Stmt_ctor_SLet(pn, _ord_t228, true, false, false, _ord_t229, o))); });
#line 1606 "src/parser.tr"
        Expr* o3 = box_expr(Expr_ctor_EIdent(pn));
#line 1607 "src/parser.tr"
        Expr* t_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1608 "src/parser.tr"
        Expr* r_node2 = box_expr(Expr_ctor_EPropAccess(o3, f));
#line 1609 "src/parser.tr"
        List_ptr_append(stmts, box_stmt(Stmt_ctor_SAssign(t_node2, box_expr(Expr_ctor_EBinOp(op, r_node2, rhs)))));
        _tr_str_release(pn);
    } else if (1) {
        __auto_type _ = _t223;
        return plain;
    }
#line 1611 "src/parser.tr"
    Block* blk = Block_init();
#line 1612 "src/parser.tr"
    long long si = 0LL;
#line 1613 "src/parser.tr"
    while ((si < stmts->len)) {
#line 1614 "src/parser.tr"
        ({ __auto_type _ord_t230 = (((Stmt*)List_ptr_get(stmts, si))); List_ptr_append(blk->stmts, _ord_t230); });
#line 1615 "src/parser.tr"
        si = (si + 1LL);
    }
#line 1616 "src/parser.tr"
    Stmt* _trret37 = ({ __auto_type _ord_t231 = (box_expr(Expr_ctor_ELitBool(true))); __auto_type _ord_t232 = ((void*)List_ptr_new()); __auto_type _ord_t233 = (Block_init()); __auto_type _wr = (box_stmt(Stmt_ctor_SIf(_ord_t231, blk, _ord_t232, _ord_t233))); _wr; });
    return _trret37;
}

__attribute__((hot)) List_ptr* Parser_parse_comp_generators(Parser* self) {
#line 1620 "src/parser.tr"
    List_ptr* generators = (void*)List_ptr_new();
#line 1621 "src/parser.tr"
    while (({ __auto_type _ord_t234 = (Parser_peek(self)); __auto_type _wr = ((_ord_t234.tag == Token_make_KwFor().tag)); _wr; })) {
#line 1622 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1623 "src/parser.tr"
        TrStr target = Parser_consume_ident(self);
#line 1624 "src/parser.tr"
        __auto_type _t235 = Parser_peek(self);
        if (_t235.tag == Token_KwIn) {
#line 1626 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t235;
#line 1628 "src/parser.tr"
            /* pass */
        }
#line 1629 "src/parser.tr"
        Expr* iter = Parser_parse_or_expr(self);
#line 1630 "src/parser.tr"
        List_ptr* ifs = (void*)List_ptr_new();
#line 1631 "src/parser.tr"
        while (({ __auto_type _ord_t236 = (Parser_peek(self)); __auto_type _wr = ((_ord_t236.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1632 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1633 "src/parser.tr"
            List_ptr_append(ifs, Parser_parse_or_expr(self));
        }
#line 1634 "src/parser.tr"
        Comprehension* c = Comprehension_init(target, iter);
#line 1635 "src/parser.tr"
        c->ifs = ifs;
#line 1636 "src/parser.tr"
        /* unsafe block */
#line 1637 "src/parser.tr"
        Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 1638 "src/parser.tr"
        (*c_ptr = c);
#line 1639 "src/parser.tr"
        List_ptr_append(generators, c_ptr);
        _tr_str_release(target);
    }
#line 1640 "src/parser.tr"
    return generators;
}

__attribute__((hot)) Expr* Parser_desugar_collection_comp(Parser* self, Expr* key, Expr* val, List_ptr* gens) {
#line 1645 "src/parser.tr"
    TrStr tmp = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->pos)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("__comp", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 1646 "src/parser.tr"
    Stmt* body_stmt = box_stmt(Stmt_make_SPass());
#line 1647 "src/parser.tr"
    Expr* empty = box_expr(Expr_ctor_ESet((void*)List_ptr_new()));
#line 1648 "src/parser.tr"
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 1649 "src/parser.tr"
        empty = ({ __auto_type _ord_t237 = ((void*)List_ptr_new()); __auto_type _ord_t238 = ((void*)List_ptr_new()); __auto_type _wr = (box_expr(Expr_ctor_EDict(_ord_t237, _ord_t238))); _wr; });
#line 1650 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SAssign(box_expr(Expr_ctor_EIndex(box_expr(Expr_ctor_EIdent(tmp)), key)), val));
    } else {
#line 1652 "src/parser.tr"
        List_ptr* margs = (void*)List_ptr_new();
#line 1653 "src/parser.tr"
        List_ptr_append(margs, key);
#line 1654 "src/parser.tr"
        body_stmt = box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EMethodCall(box_expr(Expr_ctor_EIdent(tmp)), _tr_str_lit_len("add", 3LL), margs))));
    }
#line 1655 "src/parser.tr"
    long long gi = (gens->len - 1LL);
#line 1656 "src/parser.tr"
    while ((gi >= 0LL)) {
#line 1657 "src/parser.tr"
        Comprehension* g = (*((Comprehension**)List_ptr_get(gens, gi)));
#line 1658 "src/parser.tr"
        Block* inner = Block_init();
#line 1659 "src/parser.tr"
        Block_push(inner, body_stmt);
#line 1660 "src/parser.tr"
        Stmt* cur = box_stmt(Stmt_make_SPass());
#line 1661 "src/parser.tr"
        long long ci = (g->ifs->len - 1LL);
#line 1662 "src/parser.tr"
        Block* guarded = inner;
#line 1663 "src/parser.tr"
        while ((ci >= 0LL)) {
#line 1664 "src/parser.tr"
            Block* outer = Block_init();
#line 1665 "src/parser.tr"
            ({ __auto_type _ord_t239 = (((Expr*)List_ptr_get(g->ifs, ci))); __auto_type _ord_t240 = ((void*)List_ptr_new()); __auto_type _ord_t241 = (Block_init()); Block_push(outer, box_stmt(Stmt_ctor_SIf(_ord_t239, guarded, _ord_t240, _ord_t241))); });
#line 1666 "src/parser.tr"
            guarded = outer;
#line 1667 "src/parser.tr"
            ci = (ci - 1LL);
        }
#line 1668 "src/parser.tr"
        cur = ({ __auto_type _ord_t242 = (g->target); __auto_type _ord_t243 = (g->iter); __auto_type _ord_t244 = ((void*)List_ptr_new()); __auto_type _wr = (box_stmt(Stmt_ctor_SFor(_ord_t242, _ord_t243, guarded, _ord_t244, false))); _wr; });
#line 1669 "src/parser.tr"
        body_stmt = cur;
#line 1670 "src/parser.tr"
        gi = (gi - 1LL);
    }
#line 1671 "src/parser.tr"
    Block* blk = Block_init();
#line 1672 "src/parser.tr"
    ({ __auto_type _ord_t245 = (Ownership_make_Own()); __auto_type _ord_t246 = ((AstType**)(0LL)); Block_push(blk, box_stmt(Stmt_ctor_SLet(tmp, _ord_t245, true, false, false, _ord_t246, empty))); });
#line 1673 "src/parser.tr"
    Block_push(blk, body_stmt);
#line 1674 "src/parser.tr"
    Block_push(blk, box_stmt(Stmt_ctor_SExpr(box_expr(Expr_ctor_EIdent(tmp)))));
#line 1675 "src/parser.tr"
    Expr* _trret38 = box_expr(Expr_ctor_EDo(blk));
    _tr_str_release(tmp);
    return _trret38;
}

__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self) {
#line 1678 "src/parser.tr"
    __auto_type lhs = Parser_parse_expr(self);
#line 1680 "src/parser.tr"
    if (({ __auto_type _ord_t247 = (Parser_peek(self)); __auto_type _wr = ((_ord_t247.tag == Token_make_Comma().tag)); _wr; })) {
#line 1681 "src/parser.tr"
        __auto_type _t248 = (*lhs);
        if (_t248.tag == Expr_EIdent) {
            __auto_type mn0 = _t248.data.EIdent.name;
#line 1683 "src/parser.tr"
            List_TrStr* mnames = (void*)List_TrStr_new();
#line 1684 "src/parser.tr"
            List_TrStr_append(mnames, mn0);
#line 1685 "src/parser.tr"
            while (({ __auto_type _ord_t249 = (Parser_peek(self)); __auto_type _wr = ((_ord_t249.tag == Token_make_Comma().tag)); _wr; })) {
#line 1686 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1687 "src/parser.tr"
                ({ TrStr _at_t250 = (Parser_consume_ident(self)); List_TrStr_append(mnames, _at_t250); _tr_str_release(_at_t250); });
            }
#line 1688 "src/parser.tr"
            if (({ __auto_type _ord_t251 = (Parser_peek(self)); __auto_type _wr = ((_ord_t251.tag == Token_make_Eq().tag)); _wr; })) {
#line 1689 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1690 "src/parser.tr"
                Expr* mrhs = Parser_parse_expr(self);
#line 1691 "src/parser.tr"
                if (({ __auto_type _ord_t252 = (Parser_peek(self)); __auto_type _wr = ((_ord_t252.tag == Token_make_Comma().tag)); _wr; })) {
#line 1692 "src/parser.tr"
                    List_ptr* mitems = (void*)List_ptr_new();
#line 1693 "src/parser.tr"
                    List_ptr_append(mitems, mrhs);
#line 1694 "src/parser.tr"
                    while (({ __auto_type _ord_t253 = (Parser_peek(self)); __auto_type _wr = ((_ord_t253.tag == Token_make_Comma().tag)); _wr; })) {
#line 1695 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 1696 "src/parser.tr"
                        List_ptr_append(mitems, Parser_parse_expr(self));
                    }
#line 1697 "src/parser.tr"
                    mrhs = box_expr(Expr_ctor_ETuple(mitems));
                }
#line 1698 "src/parser.tr"
                Parser_expect_newline(self);
#line 1699 "src/parser.tr"
                Stmt* _trret39 = box_stmt(Stmt_ctor_SMultiLet(mnames, false, mrhs));
                return _trret39;
            }
        } else if (1) {
            __auto_type _ = _t248;
            /* pass */
        }
    }
#line 1701 "src/parser.tr"
    __auto_type _t254 = Parser_peek(self);
    if (_t254.tag == Token_Colon) {
#line 1704 "src/parser.tr"
        __auto_type _t255 = (*lhs);
        if (_t255.tag == Expr_EIdent) {
            __auto_type name = _t255.data.EIdent.name;
#line 1706 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1707 "src/parser.tr"
            AstType* ty = Parser_parse_type(self);
#line 1708 "src/parser.tr"
            Expr* val = (Expr*)(0LL);
#line 1709 "src/parser.tr"
            if (({ __auto_type _ord_t256 = (Parser_peek(self)); __auto_type _wr = ((_ord_t256.tag == Token_make_Eq().tag)); _wr; })) {
#line 1710 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 1711 "src/parser.tr"
                val = Parser_parse_expr(self);
            }
#line 1712 "src/parser.tr"
            Parser_expect_newline(self);
#line 1713 "src/parser.tr"
            Stmt* _trret40 = ({ __auto_type _ord_t257 = (Ownership_make_Own()); __auto_type _ord_t258 = (box_asttype(ty)); __auto_type _wr = (box_stmt(Stmt_ctor_SLet(name, _ord_t257, false, false, false, _ord_t258, val))); _wr; });
            return _trret40;
        } else if (1) {
            __auto_type _ = _t255;
            /* pass */
        }
    } else if (_t254.tag == Token_Eq) {
#line 1716 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1717 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1718 "src/parser.tr"
        Parser_expect_newline(self);
#line 1719 "src/parser.tr"
        return box_stmt(Stmt_ctor_SAssign(lhs, rhs));
    } else if (_t254.tag == Token_PlusEq) {
#line 1721 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1722 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1723 "src/parser.tr"
        Parser_expect_newline(self);
#line 1724 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("+", 1LL), rhs);
    } else if (_t254.tag == Token_MinusEq) {
#line 1726 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1727 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1728 "src/parser.tr"
        Parser_expect_newline(self);
#line 1729 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("-", 1LL), rhs);
    } else if (_t254.tag == Token_StarEq) {
#line 1731 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1732 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1733 "src/parser.tr"
        Parser_expect_newline(self);
#line 1734 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("*", 1LL), rhs);
    } else if (_t254.tag == Token_SlashEq) {
#line 1736 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1737 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1738 "src/parser.tr"
        Parser_expect_newline(self);
#line 1739 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("/", 1LL), rhs);
    } else if (_t254.tag == Token_PercentEq) {
#line 1741 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1742 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1743 "src/parser.tr"
        Parser_expect_newline(self);
#line 1744 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("%", 1LL), rhs);
    } else if (_t254.tag == Token_FloorDivEq) {
#line 1746 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1747 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1748 "src/parser.tr"
        Parser_expect_newline(self);
#line 1749 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("//", 2LL), rhs);
    } else if (_t254.tag == Token_StarStarEq) {
#line 1751 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1752 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1753 "src/parser.tr"
        Parser_expect_newline(self);
#line 1754 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("**", 2LL), rhs);
    } else if (_t254.tag == Token_AmpEq) {
#line 1756 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1757 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1758 "src/parser.tr"
        Parser_expect_newline(self);
#line 1759 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("&", 1LL), rhs);
    } else if (_t254.tag == Token_PipeEq) {
#line 1761 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1762 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1763 "src/parser.tr"
        Parser_expect_newline(self);
#line 1764 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("|", 1LL), rhs);
    } else if (_t254.tag == Token_CaretEq) {
#line 1766 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1767 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1768 "src/parser.tr"
        Parser_expect_newline(self);
#line 1769 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("^", 1LL), rhs);
    } else if (_t254.tag == Token_LtLtEq) {
#line 1771 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1772 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1773 "src/parser.tr"
        Parser_expect_newline(self);
#line 1774 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len("<<", 2LL), rhs);
    } else if (_t254.tag == Token_GtGtEq) {
#line 1776 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1777 "src/parser.tr"
        Expr* rhs = Parser_parse_expr(self);
#line 1778 "src/parser.tr"
        Parser_expect_newline(self);
#line 1779 "src/parser.tr"
        return Parser_mk_aug(self, lhs, _tr_str_lit_len(">>", 2LL), rhs);
    } else if (1) {
        __auto_type _ = _t254;
#line 1781 "src/parser.tr"
        /* pass */
    }
#line 1782 "src/parser.tr"
    Parser_expect_newline(self);
#line 1783 "src/parser.tr"
    return box_stmt(Stmt_ctor_SExpr(lhs));
}

__attribute__((hot)) Expr* Parser_parse_expr(Parser* self) {
#line 1789 "src/parser.tr"
    if (({ __auto_type _ord_t259 = (Parser_peek(self)); __auto_type _wr = ((_ord_t259.tag == Token_make_KwDo().tag)); _wr; })) {
#line 1790 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1791 "src/parser.tr"
        if (({ __auto_type _ord_t260 = (Parser_peek(self)); __auto_type _wr = ((_ord_t260.tag == Token_make_Colon().tag)); _wr; })) {
#line 1791 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1792 "src/parser.tr"
        Block* do_body = Parser_parse_block(self);
#line 1793 "src/parser.tr"
        Expr* _trret41 = box_expr(Expr_ctor_EDo(do_body));
        return _trret41;
    }
#line 1800 "src/parser.tr"
    if (({ __auto_type _ord_t261 = (Parser_peek(self)); __auto_type _wr = ((_ord_t261.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1801 "src/parser.tr"
        return Parser_parse_if_expr(self);
    }
#line 1805 "src/parser.tr"
    if (({ __auto_type _ord_t262 = (Parser_peek(self)); __auto_type _wr = ((_ord_t262.tag == Token_make_KwMatch().tag)); _wr; })) {
#line 1806 "src/parser.tr"
        return Parser_parse_match_expr(self);
    }
#line 1810 "src/parser.tr"
    if (({ __auto_type _ord_t263 = (Parser_peek(self)); __auto_type _wr = ((_ord_t263.tag == Token_make_KwLoop().tag)); _wr; })) {
#line 1811 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1812 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1813 "src/parser.tr"
        if (({ __auto_type _ord_t264 = (Parser_peek(self)); __auto_type _wr = ((_ord_t264.tag == Token_make_Colon().tag)); _wr; })) {
#line 1813 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1814 "src/parser.tr"
        Block* loop_body = Parser_parse_block(self);
#line 1815 "src/parser.tr"
        Expr* _trret42 = box_expr(Expr_ctor_ELoop(loop_body));
        return _trret42;
    }
#line 1816 "src/parser.tr"
    if (({ __auto_type _ord_t265 = (Parser_peek(self)); __auto_type _wr = ((_ord_t265.tag == Token_make_KwWhile().tag)); _wr; })) {
#line 1817 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1818 "src/parser.tr"
        Expr* we_cond = Parser_parse_expr(self);
#line 1819 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1820 "src/parser.tr"
        if (({ __auto_type _ord_t266 = (Parser_peek(self)); __auto_type _wr = ((_ord_t266.tag == Token_make_Colon().tag)); _wr; })) {
#line 1820 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1821 "src/parser.tr"
        Block* we_body = Parser_parse_block(self);
#line 1822 "src/parser.tr"
        Block* we_else = Block_init();
#line 1823 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1824 "src/parser.tr"
        if (({ __auto_type _ord_t267 = (Parser_peek(self)); __auto_type _wr = ((_ord_t267.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1825 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1826 "src/parser.tr"
            Parser_skip_newlines(self);
#line 1827 "src/parser.tr"
            if (({ __auto_type _ord_t268 = (Parser_peek(self)); __auto_type _wr = ((_ord_t268.tag == Token_make_Colon().tag)); _wr; })) {
#line 1827 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 1828 "src/parser.tr"
            we_else = Parser_parse_block(self);
        }
#line 1829 "src/parser.tr"
        Expr* _trret43 = box_expr(Expr_ctor_EWhileExpr(we_cond, we_body, we_else));
        return _trret43;
    }
#line 1830 "src/parser.tr"
    return Parser_parse_ternary(self);
}

__attribute__((hot)) Expr* Parser_parse_match_expr(Parser* self) {
#line 1833 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1834 "src/parser.tr"
    Expr* subj = Parser_parse_expr(self);
#line 1835 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1836 "src/parser.tr"
    if (({ __auto_type _ord_t269 = (Parser_peek(self)); __auto_type _wr = ((_ord_t269.tag == Token_make_Colon().tag)); _wr; })) {
#line 1836 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else {
#line 1837 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected ':' after the subject of a match expression", 52LL), _tr_str_lit_len("write 'match value:' followed by an indented block of 'case' arms.", 66LL));
    }
#line 1838 "src/parser.tr"
    List_ptr* arms = Parser_parse_match_arms(self);
#line 1839 "src/parser.tr"
    return box_expr(Expr_ctor_EMatch(subj, arms));
}

__attribute__((hot)) Expr* Parser_parse_if_expr(Parser* self) {
#line 1846 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 1847 "src/parser.tr"
    Expr* cond = Parser_parse_expr(self);
#line 1848 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1849 "src/parser.tr"
    if (({ __auto_type _ord_t270 = (Parser_peek(self)); __auto_type _wr = ((_ord_t270.tag == Token_make_Colon().tag)); _wr; })) {
#line 1849 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 1850 "src/parser.tr"
    Block* then_b = Parser_parse_block(self);
#line 1851 "src/parser.tr"
    Expr* then_e = box_expr(Expr_ctor_EDo(then_b));
#line 1852 "src/parser.tr"
    Parser_skip_newlines(self);
#line 1853 "src/parser.tr"
    Expr* else_e = box_expr(Expr_ctor_EDo(Block_init()));
#line 1854 "src/parser.tr"
    __auto_type _t271 = Parser_peek(self);
    if (_t271.tag == Token_KwElif) {
#line 1856 "src/parser.tr"
        else_e = Parser_parse_if_expr(self);
    } else if (_t271.tag == Token_KwElse) {
#line 1858 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1859 "src/parser.tr"
        Parser_skip_newlines(self);
#line 1860 "src/parser.tr"
        if (({ __auto_type _ord_t272 = (Parser_peek(self)); __auto_type _wr = ((_ord_t272.tag == Token_make_Colon().tag)); _wr; })) {
#line 1860 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1861 "src/parser.tr"
        Block* else_b = Parser_parse_block(self);
#line 1862 "src/parser.tr"
        else_e = box_expr(Expr_ctor_EDo(else_b));
    } else if (1) {
        __auto_type _ = _t271;
        /* pass */
    }
#line 1864 "src/parser.tr"
    Expr* _trret44 = box_expr(Expr_ctor_EIfElse(cond, then_e, else_e));
    return _trret44;
}

__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self) {
#line 1867 "src/parser.tr"
    Expr* left = Parser_parse_or_expr(self);
#line 1868 "src/parser.tr"
    if (({ __auto_type _ord_t273 = (Parser_peek(self)); __auto_type _wr = ((_ord_t273.tag == Token_make_Newline().tag)); _wr; })) {
#line 1869 "src/parser.tr"
        return left;
    }
#line 1872 "src/parser.tr"
    __auto_type _t274 = (*left);
    if (_t274.tag == Expr_EClosure) {
#line 1874 "src/parser.tr"
        return left;
    } else if (1) {
        __auto_type _ = _t274;
        /* pass */
    }
#line 1876 "src/parser.tr"
    if (({ __auto_type _ord_t275 = (Parser_peek(self)); __auto_type _wr = ((_ord_t275.tag == Token_make_KwIf().tag)); _wr; })) {
#line 1877 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1878 "src/parser.tr"
        Expr* cond = Parser_parse_or_expr(self);
#line 1879 "src/parser.tr"
        if (({ __auto_type _ord_t276 = (Parser_peek(self)); __auto_type _wr = ((_ord_t276.tag == Token_make_KwElse().tag)); _wr; })) {
#line 1880 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 1881 "src/parser.tr"
        Expr* else_e = Parser_parse_ternary(self);
#line 1882 "src/parser.tr"
        return box_expr(Expr_ctor_EIfElse(cond, left, else_e));
    }
#line 1883 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self) {
#line 1886 "src/parser.tr"
    Expr* left = Parser_parse_and_expr(self);
#line 1887 "src/parser.tr"
    while (({ __auto_type _ord_t277 = (Parser_peek(self)); __auto_type _wr = ((_ord_t277.tag == Token_make_KwOr().tag)); _wr; })) {
#line 1888 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1889 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1890 "src/parser.tr"
        __auto_type right = Parser_parse_and_expr(self);
#line 1891 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("or", 2LL), left, right));
    }
#line 1892 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self) {
#line 1895 "src/parser.tr"
    Expr* left = Parser_parse_not_expr(self);
#line 1896 "src/parser.tr"
    while (({ __auto_type _ord_t278 = (Parser_peek(self)); __auto_type _wr = ((_ord_t278.tag == Token_make_KwAnd().tag)); _wr; })) {
#line 1897 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1898 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 1899 "src/parser.tr"
        __auto_type right = Parser_parse_not_expr(self);
#line 1900 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), left, right));
    }
#line 1901 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self) {
#line 1904 "src/parser.tr"
    __auto_type _t279 = Parser_peek(self);
    if (_t279.tag == Token_KwNot) {
#line 1906 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1907 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), Parser_parse_not_expr(self)));
    } else if (1) {
        __auto_type _ = _t279;
#line 1909 "src/parser.tr"
        /* pass */
    }
#line 1910 "src/parser.tr"
    return Parser_parse_comparison(self);
}

__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self) {
#line 1913 "src/parser.tr"
    Expr* left = Parser_parse_bitor_expr(self);
#line 1915 "src/parser.tr"
    if (({ __auto_type _ord_t280 = (Parser_peek(self)); __auto_type _wr = ((_ord_t280.tag == Token_make_KwNot().tag)); _wr; })) {
#line 1916 "src/parser.tr"
        long long saved_pos = self->pos;
#line 1917 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1918 "src/parser.tr"
        if (({ __auto_type _ord_t281 = (Parser_peek(self)); __auto_type _wr = ((_ord_t281.tag == Token_make_KwIn().tag)); _wr; })) {
#line 1919 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1920 "src/parser.tr"
            return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("not", 3LL), box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("in", 2LL), left, Parser_parse_bitor_expr(self)))));
        }
#line 1921 "src/parser.tr"
        self->pos = saved_pos;
    }
#line 1924 "src/parser.tr"
    List_ptr* operands = (void*)List_ptr_new();
#line 1925 "src/parser.tr"
    List_ptr_append(operands, left);
#line 1926 "src/parser.tr"
    List_TrStr* ops = (void*)List_TrStr_new();
#line 1927 "src/parser.tr"
    while (true) {
#line 1928 "src/parser.tr"
        TrStr op = Parser__peek_cmp_op(self);
#line 1929 "src/parser.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("", 0LL)))) {
#line 1929 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
#line 1930 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1931 "src/parser.tr"
        List_TrStr_append(ops, op);
#line 1932 "src/parser.tr"
        List_ptr_append(operands, Parser_parse_bitor_expr(self));
#line 1933 "src/parser.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("is", 2LL))) || _tr_str_eqv((op), (_tr_str_lit_len("in", 2LL))))) {
#line 1933 "src/parser.tr"
            _tr_str_release(op);
            break;
        }
        _tr_str_release(op);
    }
#line 1934 "src/parser.tr"
    if ((ops->len == 0LL)) {
#line 1934 "src/parser.tr"
        Expr* _trret45 = left;
        List_TrStr_free(ops);
        return _trret45;
    }
#line 1935 "src/parser.tr"
    Expr* result = ({ TrStr _ord_t282 = (List_TrStr_get(ops, 0LL)); __auto_type _ord_t283 = (((Expr*)List_ptr_get(operands, 0LL))); __auto_type _ord_t284 = (((Expr*)List_ptr_get(operands, 1LL))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t282, _ord_t283, _ord_t284))); _tr_str_release(_ord_t282); _wr; });
#line 1936 "src/parser.tr"
    long long ci = 1LL;
#line 1937 "src/parser.tr"
    while ((ci < ops->len)) {
#line 1938 "src/parser.tr"
        Expr* cmp = ({ TrStr _ord_t285 = (List_TrStr_get(ops, ci)); __auto_type _ord_t286 = (((Expr*)List_ptr_get(operands, ci))); __auto_type _ord_t287 = (((Expr*)List_ptr_get(operands, (ci + 1LL)))); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_ord_t285, _ord_t286, _ord_t287))); _tr_str_release(_ord_t285); _wr; });
#line 1939 "src/parser.tr"
        result = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("and", 3LL), result, cmp));
#line 1940 "src/parser.tr"
        ci = (ci + 1LL);
    }
#line 1941 "src/parser.tr"
    Expr* _trret46 = result;
    List_TrStr_free(ops);
    return _trret46;
}

__attribute__((hot)) TrStr Parser__peek_cmp_op(Parser* self) {
#line 1944 "src/parser.tr"
    __auto_type _t288 = Parser_peek(self);
    if (_t288.tag == Token_EqEq) {
        return _tr_str_lit_len("==", 2LL);
    } else if (_t288.tag == Token_NotEq) {
        return _tr_str_lit_len("!=", 2LL);
    } else if (_t288.tag == Token_Lt) {
        return _tr_str_lit_len("<", 1LL);
    } else if (_t288.tag == Token_Gt) {
        return _tr_str_lit_len(">", 1LL);
    } else if (_t288.tag == Token_LtEq) {
        return _tr_str_lit_len("<=", 2LL);
    } else if (_t288.tag == Token_GtEq) {
        return _tr_str_lit_len(">=", 2LL);
    } else if (_t288.tag == Token_KwIs) {
        return _tr_str_lit_len("is", 2LL);
    } else if (_t288.tag == Token_KwIn) {
        return _tr_str_lit_len("in", 2LL);
    } else if (1) {
        __auto_type _ = _t288;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self) {
#line 1956 "src/parser.tr"
    Expr* left = Parser_parse_bitxor_expr(self);
#line 1957 "src/parser.tr"
    while (({ __auto_type _ord_t289 = (Parser_peek(self)); __auto_type _wr = ((_ord_t289.tag == Token_make_Pipe().tag)); _wr; })) {
#line 1958 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1959 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("|", 1LL), left, Parser_parse_bitxor_expr(self)));
    }
#line 1960 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self) {
#line 1963 "src/parser.tr"
    Expr* left = Parser_parse_bitand_expr(self);
#line 1964 "src/parser.tr"
    while (({ __auto_type _ord_t290 = (Parser_peek(self)); __auto_type _wr = ((_ord_t290.tag == Token_make_Caret().tag)); _wr; })) {
#line 1965 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1966 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("^", 1LL), left, Parser_parse_bitand_expr(self)));
    }
#line 1967 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self) {
#line 1970 "src/parser.tr"
    Expr* left = Parser_parse_shift_expr(self);
#line 1971 "src/parser.tr"
    while (({ __auto_type _ord_t291 = (Parser_peek(self)); __auto_type _wr = ((_ord_t291.tag == Token_make_Amp().tag)); _wr; })) {
#line 1972 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 1973 "src/parser.tr"
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("&", 1LL), left, Parser_parse_shift_expr(self)));
    }
#line 1974 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self) {
#line 1977 "src/parser.tr"
    Expr* left = Parser_parse_additive(self);
#line 1978 "src/parser.tr"
    while (true) {
#line 1979 "src/parser.tr"
        __auto_type _t292 = Parser_peek(self);
        if (_t292.tag == Token_LtLt) {
#line 1981 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1982 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("<<", 2LL), left, Parser_parse_additive(self)));
        } else if (_t292.tag == Token_GtGt) {
#line 1984 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1985 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len(">>", 2LL), left, Parser_parse_additive(self)));
        } else if (1) {
            __auto_type _ = _t292;
            break;
        }
    }
#line 1987 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_additive(Parser* self) {
#line 1990 "src/parser.tr"
    Expr* left = Parser_parse_multiplicative(self);
#line 1991 "src/parser.tr"
    while (true) {
#line 1992 "src/parser.tr"
        __auto_type _t293 = Parser_peek(self);
        if (_t293.tag == Token_Plus) {
#line 1994 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1995 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 1996 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("+", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (_t293.tag == Token_Minus) {
#line 1998 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 1999 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 2000 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("-", 1LL), left, Parser_parse_multiplicative(self)));
        } else if (1) {
            __auto_type _ = _t293;
            break;
        }
    }
#line 2002 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self) {
#line 2005 "src/parser.tr"
    Expr* left = Parser_parse_unary(self);
#line 2006 "src/parser.tr"
    while (true) {
#line 2007 "src/parser.tr"
        __auto_type _t294 = Parser_peek(self);
        if (_t294.tag == Token_Star) {
#line 2009 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2010 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("*", 1LL), left, Parser_parse_unary(self)));
        } else if (_t294.tag == Token_Slash) {
#line 2012 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2013 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("/", 1LL), left, Parser_parse_unary(self)));
        } else if (_t294.tag == Token_Percent) {
#line 2015 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2016 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("%", 1LL), left, Parser_parse_unary(self)));
        } else if (_t294.tag == Token_FloorDiv) {
#line 2018 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2019 "src/parser.tr"
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("//", 2LL), left, Parser_parse_unary(self)));
        } else if (1) {
            __auto_type _ = _t294;
            break;
        }
    }
#line 2021 "src/parser.tr"
    return left;
}

__attribute__((hot)) Expr* Parser_parse_power(Parser* self) {
#line 2024 "src/parser.tr"
    Expr* base = Parser_parse_postfix(self);
#line 2025 "src/parser.tr"
    if (({ __auto_type _ord_t295 = (Parser_peek(self)); __auto_type _wr = ((_ord_t295.tag == Token_make_StarStar().tag)); _wr; })) {
#line 2026 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2028 "src/parser.tr"
        Expr* exp = Parser_parse_unary(self);
#line 2029 "src/parser.tr"
        return box_expr(Expr_ctor_EBinOp(_tr_str_lit_len("**", 2LL), base, exp));
    }
#line 2030 "src/parser.tr"
    return base;
}

__attribute__((hot)) Expr* Parser_parse_unary(Parser* self) {
#line 2033 "src/parser.tr"
    __auto_type _t296 = Parser_peek(self);
    if (_t296.tag == Token_Minus) {
#line 2035 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2036 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("-", 1LL), Parser_parse_unary(self)));
    } else if (_t296.tag == Token_Tilde) {
#line 2038 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2039 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("~", 1LL), Parser_parse_unary(self)));
    } else if (_t296.tag == Token_Amp) {
#line 2041 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2042 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("&", 1LL), Parser_parse_unary(self)));
    } else if (_t296.tag == Token_Star) {
#line 2044 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2045 "src/parser.tr"
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit_len("*", 1LL), Parser_parse_unary(self)));
    } else if (1) {
        __auto_type _ = _t296;
#line 2047 "src/parser.tr"
        /* pass */
    }
#line 2048 "src/parser.tr"
    return Parser_parse_power(self);
}

__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self) {
#line 2051 "src/parser.tr"
    Expr* e = Parser_parse_primary(self);
#line 2052 "src/parser.tr"
    while (true) {
#line 2053 "src/parser.tr"
        __auto_type _t297 = Parser_peek(self);
        if (_t297.tag == Token_Dot) {
#line 2055 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2056 "src/parser.tr"
            TrStr attr = Parser_consume_method_ident(self);
#line 2057 "src/parser.tr"
            if (_tr_str_eqv((attr), (_tr_str_lit_len("", 0LL)))) {
#line 2058 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a field or method name after '.'", 41LL), _tr_str_lit_len("write the member name right after the dot, e.g. 'value.len' or 'value.method()'.", 80LL));
            }
#line 2059 "src/parser.tr"
            __auto_type _t298 = Parser_peek(self);
            if (_t298.tag == Token_LParen) {
#line 2061 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2062 "src/parser.tr"
                e = box_expr(Expr_ctor_EMethodCall(e, attr, Parser_parse_arg_list(self)));
            } else if (1) {
                __auto_type _ = _t298;
#line 2064 "src/parser.tr"
                e = box_expr(Expr_ctor_EPropAccess(e, attr));
            }
            _tr_str_release(attr);
        } else if (_t297.tag == Token_LParen) {
#line 2066 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2071 "src/parser.tr"
            __auto_type _t299 = (*e);
            if (_t299.tag == Expr_EIdent) {
                __auto_type _pf_nm = _t299.data.EIdent.name;
#line 2073 "src/parser.tr"
                if (_tr_dict_contains(self->import_aliases, _tr_strz(_pf_nm))) {
#line 2074 "src/parser.tr"
                    e = ({ TrStr _at_t300 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->import_aliases, _tr_strz(_pf_nm))))); __auto_type _wr = (box_expr(Expr_ctor_EIdent(_at_t300))); _tr_str_release(_at_t300); _wr; });
                }
            } else if (1) {
                __auto_type _ = _t299;
#line 2076 "src/parser.tr"
                /* pass */
            }
#line 2077 "src/parser.tr"
            e = box_expr(Expr_ctor_ECall(e, Parser_parse_arg_list(self)));
        } else if (_t297.tag == Token_LBracket) {
#line 2079 "src/parser.tr"
            long long ix_oln = Parser_cur_line(self);
#line 2080 "src/parser.tr"
            long long ix_ocol = Parser_cur_col(self);
#line 2081 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2086 "src/parser.tr"
            Expr* idx = ((Expr*)(0LL));
#line 2087 "src/parser.tr"
            if (({ __auto_type _ord_t301 = (Parser_peek(self)); __auto_type _wr = ((_ord_t301.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2088 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self))));
            } else {
#line 2090 "src/parser.tr"
                idx = Parser_parse_expr(self);
            }
#line 2100 "src/parser.tr"
            List_ptr* _ti_extra = (void*)List_ptr_new();
#line 2101 "src/parser.tr"
            while (({ __auto_type _ord_t302 = (Parser_peek(self)); __auto_type _wr = ((_ord_t302.tag == Token_make_Comma().tag)); _wr; })) {
#line 2102 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2103 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
#line 2104 "src/parser.tr"
                if (({ __auto_type _ord_t303 = (Parser_peek(self)); __auto_type _wr = ((_ord_t303.tag == Token_make_KwDef().tag)); _wr; })) {
#line 2105 "src/parser.tr"
                    List_ptr_append(_ti_extra, box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self)))));
                } else {
#line 2107 "src/parser.tr"
                    List_ptr_append(_ti_extra, Parser_parse_expr(self));
                }
            }
#line 2108 "src/parser.tr"
            Parser_expect_rbracket(self, ix_oln, ix_ocol, _tr_str_lit_len("this index", 10LL));
#line 2109 "src/parser.tr"
            if ((_ti_extra->len > 0LL)) {
#line 2110 "src/parser.tr"
                List_ptr* _ti_all = (void*)List_ptr_new();
#line 2111 "src/parser.tr"
                List_ptr_append(_ti_all, idx);
#line 2112 "src/parser.tr"
                long long _tj = 0LL;
#line 2113 "src/parser.tr"
                while ((_tj < _ti_extra->len)) {
#line 2114 "src/parser.tr"
                    List_ptr_append(_ti_all, ((Expr*)List_ptr_get(_ti_extra, _tj)));
#line 2115 "src/parser.tr"
                    _tj = (_tj + 1LL);
                }
#line 2116 "src/parser.tr"
                idx = box_expr(Expr_ctor_ETuple(_ti_all));
            }
#line 2117 "src/parser.tr"
            e = box_expr(Expr_ctor_EIndex(e, idx));
        } else if (_t297.tag == Token_Question) {
#line 2119 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2120 "src/parser.tr"
            e = box_expr(Expr_ctor_ETryExpr(e));
        } else if (_t297.tag == Token_KwAs) {
#line 2122 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2123 "src/parser.tr"
            e = box_expr(Expr_ctor_ECast(e, box_asttype(Parser_parse_type(self))));
        } else if (1) {
            __auto_type _ = _t297;
            break;
        }
    }
#line 2125 "src/parser.tr"
    return e;
}

__attribute__((hot)) void Parser_emit_diag_at(Parser* self, long long ln, long long col, TrStr msg, TrStr hint) {
#line 2131 "src/parser.tr"
    ({ __auto_type _ord_t304 = (({ TrStr _tr_cl = (c_red(_tr_str_lit_len("error", 5LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ ({ TrStr _ps = ({ TrStr _tr_cl = (_ord_t304); TrStr _tr_cr = (c_bold(msg)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); }); });
#line 2132 "src/parser.tr"
    TrStr loc = _tr_str_lit_len("", 0LL);
#line 2133 "src/parser.tr"
    if ((_tr_str_lenv((self->current_file)) > 0LL)) {
#line 2134 "src/parser.tr"
        TrStr _strtmp_t305 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t305;
    }
#line 2135 "src/parser.tr"
    TrStr head = ({ TrStr _tr_cl = (_tr_strx_concatv((loc), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 2136 "src/parser.tr"
    if ((col > 0LL)) {
#line 2137 "src/parser.tr"
        ({ TrStr _ap_t306 = (_tr_str_lit_len(":", 1LL)); TrStr _ap_t307 = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); _tr_str_append(&head, _ap_t306); _tr_str_append(&head, _ap_t307); _tr_str_release(_ap_t307); });
    }
#line 2138 "src/parser.tr"
    ({ ({ TrStr _ps = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_cyan(_tr_str_lit_len("-->", 3LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (head)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
#line 2139 "src/parser.tr"
    if (((_tr_str_lenv((self->src_text)) > 0LL) && (ln > 0LL))) {
#line 2140 "src/parser.tr"
        TrStr srcline = ({ TrStr _apt_t308 = _tr_str_retain(self->src_text); __auto_type _wr = (_nth_source_line(_apt_t308, ln)); _tr_str_release(_apt_t308); _wr; });
#line 2141 "src/parser.tr"
        long long full_col2 = col;
#line 2142 "src/parser.tr"
        long long col2 = _clip_col(srcline, full_col2);
#line 2143 "src/parser.tr"
        TrStr _strtmp_t309 = _clip_line(srcline, full_col2);
        _tr_str_release(srcline);
        srcline = _strtmp_t309;
#line 2144 "src/parser.tr"
        if ((_tr_str_lenv((srcline)) > 0LL)) {
#line 2145 "src/parser.tr"
            TrStr gnum = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("   ", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" | ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2146 "src/parser.tr"
            TrStr gbar = ({ TrStr _tr_cl = (_spaces((_tr_str_lenv((gnum)) - 2LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("| ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 2147 "src/parser.tr"
            ({ ({ TrStr _ps = ({ TrStr _tr_cl = (c_dim(gnum)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (srcline)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
#line 2148 "src/parser.tr"
            if ((col > 0LL)) {
#line 2149 "src/parser.tr"
                ({ __auto_type _ord_t310 = (c_dim(gbar)); __auto_type _ord_t311 = (({ TrStr _tr_cl = (_ord_t310); TrStr _tr_cr = (_spaces((col2 - 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); ({ ({ TrStr _ps = ({ TrStr _tr_cl = (_ord_t311); TrStr _tr_cr = (c_red(_tr_str_lit_len("^", 1LL))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); }); });
            }
            _tr_str_release(gnum);
        }
    }
#line 2150 "src/parser.tr"
    if ((_tr_str_lenv((hint)) > 0LL)) {
#line 2151 "src/parser.tr"
        ({ ({ TrStr _ps = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (c_green(_tr_str_lit_len("= help", 6LL))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (hint)); _tr_str_release(_tr_cl); _tr_cres; }); printf("%s", _tr_strz(_ps)); _tr_str_release(_ps); }); printf("\n"); });
    }
#line 2152 "src/parser.tr"
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) void Parser_expect_rparen(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2159 "src/parser.tr"
    __auto_type _t312 = Parser_peek(self);
    if (_t312.tag == Token_RParen) {
#line 2161 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t312;
#line 2163 "src/parser.tr"
        ({ TrStr _at_t313 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '(' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t313, _tr_str_lit_len("add the matching ')'.", 21LL)); _tr_str_release(_at_t313); });
    }
}

__attribute__((hot)) void Parser_expect_rbracket(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2165 "src/parser.tr"
    __auto_type _t314 = Parser_peek(self);
    if (_t314.tag == Token_RBracket) {
#line 2167 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t314;
#line 2169 "src/parser.tr"
        ({ TrStr _at_t315 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t315, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t315); });
    }
}

__attribute__((hot)) void Parser_expect_rbrace(Parser* self, long long oln, long long ocol, TrStr what) {
#line 2171 "src/parser.tr"
    __auto_type _t316 = Parser_peek(self);
    if (_t316.tag == Token_RBrace) {
#line 2173 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t316;
#line 2175 "src/parser.tr"
        ({ TrStr _at_t317 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '{' — ", 17LL)), (what))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is never closed", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t317, _tr_str_lit_len("add the matching '}'.", 21LL)); _tr_str_release(_at_t317); });
    }
}

__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self) {
#line 2180 "src/parser.tr"
    long long oln = 0LL;
#line 2181 "src/parser.tr"
    long long ocol = 0LL;
#line 2182 "src/parser.tr"
    if ((self->pos > 0LL)) {
#line 2183 "src/parser.tr"
        oln = List_i64_get(self->lines, (self->pos - 1LL));
#line 2184 "src/parser.tr"
        ocol = List_i64_get(self->cols, (self->pos - 1LL));
    }
#line 2185 "src/parser.tr"
    List_ptr* el = (void*)List_ptr_new();
#line 2186 "src/parser.tr"
    bool going = true;
#line 2187 "src/parser.tr"
    while (going) {
#line 2188 "src/parser.tr"
        Parser_skip_newlines_and_indent(self);
#line 2189 "src/parser.tr"
        __auto_type _t318 = Parser_peek(self);
        if ((_t318.tag == Token_RParen || _t318.tag == Token_Eof)) {
#line 2191 "src/parser.tr"
            going = false;
        } else if (1) {
            __auto_type _ = _t318;
#line 2193 "src/parser.tr"
            /* pass */
        }
#line 2194 "src/parser.tr"
        if (going) {
#line 2195 "src/parser.tr"
            List_ptr_append(el, Parser_parse_expr(self));
#line 2196 "src/parser.tr"
            __auto_type _t319 = Parser_peek(self);
            if (_t319.tag == Token_Comma) {
#line 2198 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2199 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            } else if (1) {
                __auto_type _ = _t319;
#line 2201 "src/parser.tr"
                going = false;
            }
        }
    }
#line 2202 "src/parser.tr"
    Parser_expect_rparen(self, oln, ocol, _tr_str_lit_len("this call's argument list", 25LL));
#line 2203 "src/parser.tr"
    return el;
}

__attribute__((hot)) Expr* Parser_parse_primary(Parser* self) {
#line 2206 "src/parser.tr"
    __auto_type _t320 = Parser_peek(self);
    if (_t320.tag == Token_IntLit) {
        __auto_type v = _t320.data.IntLit.val;
#line 2208 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2209 "src/parser.tr"
        return box_expr(Expr_ctor_ELitInt(v));
    } else if (_t320.tag == Token_FloatLit) {
        __auto_type v = _t320.data.FloatLit.val;
#line 2211 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2212 "src/parser.tr"
        return box_expr(Expr_ctor_ELitFloat(v));
    } else if (_t320.tag == Token_StrLit) {
        __auto_type s = _t320.data.StrLit.val;
__auto_type blen = _t320.data.StrLit.blen;
#line 2214 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2215 "src/parser.tr"
        Expr* _trret47 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret47;
    } else if (_t320.tag == Token_TripleStrLit) {
        __auto_type s = _t320.data.TripleStrLit.val;
__auto_type blen = _t320.data.TripleStrLit.blen;
#line 2217 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2218 "src/parser.tr"
        Expr* _trret48 = box_expr(Expr_ctor_ELitStr(s, blen));
        return _trret48;
    } else if (_t320.tag == Token_RawStrLit) {
        __auto_type s = _t320.data.RawStrLit.val;
__auto_type blen = _t320.data.RawStrLit.blen;
#line 2220 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2221 "src/parser.tr"
        Expr* _trret49 = box_expr(Expr_ctor_ERawStr(s, blen));
        return _trret49;
    } else if (_t320.tag == Token_ByteStrLit) {
        __auto_type s = _t320.data.ByteStrLit.val;
__auto_type blen = _t320.data.ByteStrLit.blen;
#line 2223 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2224 "src/parser.tr"
        Expr* _trret50 = box_expr(Expr_ctor_ELitBytes(s, blen));
        return _trret50;
    } else if (_t320.tag == Token_FStrLit) {
        __auto_type s = _t320.data.FStrLit.val;
__auto_type blen = _t320.data.FStrLit.blen;
#line 2226 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2227 "src/parser.tr"
        Expr* _trret51 = Parser_parse_fstring(self, s, blen);
        return _trret51;
    } else if (_t320.tag == Token_CharLit) {
        __auto_type v = _t320.data.CharLit.val;
#line 2229 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2230 "src/parser.tr"
        return box_expr(Expr_ctor_ELitChar(v));
    } else if (_t320.tag == Token_BoolLit) {
        __auto_type v = _t320.data.BoolLit.val;
#line 2232 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2233 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(v));
    } else if (_t320.tag == Token_KwTrue) {
#line 2235 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2236 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(true));
    } else if (_t320.tag == Token_KwFalse) {
#line 2238 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2239 "src/parser.tr"
        return box_expr(Expr_ctor_ELitBool(false));
    } else if (_t320.tag == Token_KwNone) {
#line 2241 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2242 "src/parser.tr"
        return box_expr(Expr_make_ELitNone());
    } else if (_t320.tag == Token_KwInt) {
#line 2244 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2245 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("int", 3LL)));
    } else if (_t320.tag == Token_KwFloat) {
#line 2247 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2248 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("float", 5LL)));
    } else if (_t320.tag == Token_KwBool) {
#line 2250 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2251 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("bool", 4LL)));
    } else if ((_t320.tag == Token_KwStr || _t320.tag == Token_KwString)) {
#line 2253 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2254 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("str", 3LL)));
    } else if (_t320.tag == Token_KwChar) {
#line 2256 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2257 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("char", 4LL)));
    } else if (_t320.tag == Token_KwVoid) {
#line 2259 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2260 "src/parser.tr"
        return box_expr(Expr_ctor_EIdent(_tr_str_lit_len("void", 4LL)));
    } else if (_t320.tag == Token_KwSuper) {
#line 2262 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2263 "src/parser.tr"
        TrStr super_base = _tr_str_lit_len("", 0LL);
#line 2264 "src/parser.tr"
        if (({ __auto_type _ord_t321 = (Parser_peek(self)); __auto_type _wr = ((_ord_t321.tag == Token_make_LParen().tag)); _wr; })) {
#line 2265 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2266 "src/parser.tr"
            TrStr _strtmp_t322 = Parser_consume_ident(self);
            _tr_str_release(super_base);
            super_base = _strtmp_t322;
#line 2267 "src/parser.tr"
            if (({ __auto_type _ord_t323 = (Parser_peek(self)); __auto_type _wr = ((_ord_t323.tag == Token_make_RParen().tag)); _wr; })) {
#line 2268 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
#line 2269 "src/parser.tr"
            if (({ __auto_type _ord_t324 = (Parser_peek(self)); __auto_type _wr = ((_ord_t324.tag == Token_make_Dot().tag)); _wr; })) {
#line 2270 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2271 "src/parser.tr"
                TrStr super_method = Parser_consume_ident(self);
#line 2272 "src/parser.tr"
                if (({ __auto_type _ord_t325 = (Parser_peek(self)); __auto_type _wr = ((_ord_t325.tag == Token_make_LParen().tag)); _wr; })) {
#line 2273 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2274 "src/parser.tr"
                    List_ptr* super_args = Parser_parse_arg_list(self);
#line 2275 "src/parser.tr"
                    Expr* _trret52 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method, super_args));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret52;
                } else {
#line 2277 "src/parser.tr"
                    Expr* _trret53 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method));
                    _tr_str_release(super_base);
                    _tr_str_release(super_method);
                    return _trret53;
                }
            }
        }
#line 2278 "src/parser.tr"
        if (({ __auto_type _ord_t326 = (Parser_peek(self)); __auto_type _wr = ((_ord_t326.tag == Token_make_Dot().tag)); _wr; })) {
#line 2279 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2280 "src/parser.tr"
            TrStr super_next = Parser_consume_ident(self);
#line 2281 "src/parser.tr"
            if (({ __auto_type _ord_t327 = (Parser_peek(self)); __auto_type _wr = ((_ord_t327.tag == Token_make_Dot().tag)); _wr; })) {
#line 2282 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2283 "src/parser.tr"
                TrStr _strtmp_t328 = _tr_str_retain(super_next);
                _tr_str_release(super_base);
                super_base = _strtmp_t328;
#line 2284 "src/parser.tr"
                TrStr super_method2 = Parser_consume_ident(self);
#line 2285 "src/parser.tr"
                if (({ __auto_type _ord_t329 = (Parser_peek(self)); __auto_type _wr = ((_ord_t329.tag == Token_make_LParen().tag)); _wr; })) {
#line 2286 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2287 "src/parser.tr"
                    List_ptr* super_args2 = Parser_parse_arg_list(self);
#line 2288 "src/parser.tr"
                    Expr* _trret54 = box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method2, super_args2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret54;
                } else {
#line 2290 "src/parser.tr"
                    Expr* _trret55 = box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method2));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    _tr_str_release(super_method2);
                    return _trret55;
                }
            } else {
#line 2292 "src/parser.tr"
                if (({ __auto_type _ord_t330 = (Parser_peek(self)); __auto_type _wr = ((_ord_t330.tag == Token_make_LParen().tag)); _wr; })) {
#line 2293 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2294 "src/parser.tr"
                    List_ptr* super_args3 = Parser_parse_arg_list(self);
#line 2295 "src/parser.tr"
                    Expr* _trret56 = box_expr(Expr_ctor_ESuperMethodCall(_tr_str_lit_len("", 0LL), super_next, super_args3));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret56;
                } else {
#line 2297 "src/parser.tr"
                    Expr* _trret57 = box_expr(Expr_ctor_ESuperPropAccess(_tr_str_lit_len("", 0LL), super_next));
                    _tr_str_release(super_base);
                    _tr_str_release(super_next);
                    return _trret57;
                }
            }
        }
#line 2298 "src/parser.tr"
        Expr* _trret58 = box_expr(Expr_ctor_EIdent(_tr_str_lit_len("super", 5LL)));
        _tr_str_release(super_base);
        return _trret58;
    } else if (_t320.tag == Token_Ident) {
        __auto_type name = _t320.data.Ident.name;
#line 2300 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2303 "src/parser.tr"
        if (({ __auto_type _ord_t331 = (Parser_peek(self)); __auto_type _wr = ((_ord_t331.tag == Token_make_Bang().tag)); _wr; })) {
#line 2304 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2305 "src/parser.tr"
            if (({ __auto_type _ord_t332 = (Parser_peek(self)); __auto_type _wr = ((_ord_t332.tag == Token_make_LParen().tag)); _wr; })) {
#line 2306 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2307 "src/parser.tr"
                List_ptr* mac_args = Parser_parse_arg_list(self);
#line 2308 "src/parser.tr"
                Expr* _trret59 = box_expr(Expr_ctor_EMacroCall(name, mac_args));
                return _trret59;
            }
        }
#line 2309 "src/parser.tr"
        Expr* _trret60 = box_expr(Expr_ctor_EIdent(name));
        return _trret60;
    } else if (_t320.tag == Token_KwSizeOf) {
#line 2311 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2312 "src/parser.tr"
        __auto_type _t333 = Parser_peek(self);
        if (_t333.tag == Token_LParen) {
#line 2314 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t333;
#line 2316 "src/parser.tr"
            /* pass */
        }
#line 2317 "src/parser.tr"
        AstType** ty = box_asttype(Parser_parse_type(self));
#line 2318 "src/parser.tr"
        __auto_type _t334 = Parser_peek(self);
        if (_t334.tag == Token_RParen) {
#line 2320 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t334;
#line 2322 "src/parser.tr"
            /* pass */
        }
#line 2323 "src/parser.tr"
        return box_expr(Expr_ctor_ESizeOf(ty));
    } else if (_t320.tag == Token_LParen) {
#line 2325 "src/parser.tr"
        long long p_oln = Parser_cur_line(self);
#line 2326 "src/parser.tr"
        long long p_ocol = Parser_cur_col(self);
#line 2327 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2328 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2329 "src/parser.tr"
        if (({ __auto_type _ord_t335 = (Parser_peek(self)); __auto_type _wr = ((_ord_t335.tag == Token_make_RParen().tag)); _wr; })) {
#line 2330 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2331 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple((void*)List_ptr_new()));
        }
#line 2332 "src/parser.tr"
        Expr* e = Parser_parse_expr(self);
#line 2333 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2334 "src/parser.tr"
        if (({ __auto_type _ord_t336 = (Parser_peek(self)); __auto_type _wr = ((_ord_t336.tag == Token_make_Comma().tag)); _wr; })) {
#line 2335 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2336 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2337 "src/parser.tr"
            List_ptr* items = (void*)List_ptr_new();
#line 2338 "src/parser.tr"
            List_ptr_append(items, e);
#line 2339 "src/parser.tr"
            while ((({ __auto_type _ord_t337 = (Parser_peek(self)); __auto_type _wr = ((_ord_t337.tag != Token_make_RParen().tag)); _wr; }) && ({ __auto_type _ord_t338 = (Parser_peek(self)); __auto_type _wr = ((_ord_t338.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2340 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2341 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2342 "src/parser.tr"
                if (({ __auto_type _ord_t339 = (Parser_peek(self)); __auto_type _wr = ((_ord_t339.tag == Token_make_Comma().tag)); _wr; })) {
#line 2343 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2344 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2346 "src/parser.tr"
                    break;
                }
            }
#line 2347 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this tuple", 10LL));
#line 2348 "src/parser.tr"
            return box_expr(Expr_ctor_ETuple(items));
        } else {
#line 2350 "src/parser.tr"
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit_len("this parenthesized expression", 29LL));
#line 2351 "src/parser.tr"
            return e;
        }
    } else if (_t320.tag == Token_LBracket) {
#line 2353 "src/parser.tr"
        long long lb_oln = Parser_cur_line(self);
#line 2354 "src/parser.tr"
        long long lb_ocol = Parser_cur_col(self);
#line 2355 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2356 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2357 "src/parser.tr"
        if (({ __auto_type _ord_t340 = (Parser_peek(self)); __auto_type _wr = ((_ord_t340.tag == Token_make_RBracket().tag)); _wr; })) {
#line 2358 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2359 "src/parser.tr"
            return box_expr(Expr_ctor_EList((void*)List_ptr_new()));
        }
#line 2360 "src/parser.tr"
        Expr* first = Parser_parse_or_expr(self);
#line 2361 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2362 "src/parser.tr"
        if (({ __auto_type _ord_t341 = (Parser_peek(self)); __auto_type _wr = ((_ord_t341.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2363 "src/parser.tr"
            List_ptr* generators = (void*)List_ptr_new();
#line 2364 "src/parser.tr"
            while (({ __auto_type _ord_t342 = (Parser_peek(self)); __auto_type _wr = ((_ord_t342.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2365 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2366 "src/parser.tr"
                TrStr target = Parser_consume_ident(self);
#line 2367 "src/parser.tr"
                __auto_type _t343 = Parser_peek(self);
                if (_t343.tag == Token_KwIn) {
#line 2369 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t343;
#line 2371 "src/parser.tr"
                    /* pass */
                }
#line 2372 "src/parser.tr"
                Expr* iter = Parser_parse_or_expr(self);
#line 2373 "src/parser.tr"
                List_ptr* ifs = (void*)List_ptr_new();
#line 2374 "src/parser.tr"
                while (({ __auto_type _ord_t344 = (Parser_peek(self)); __auto_type _wr = ((_ord_t344.tag == Token_make_KwIf().tag)); _wr; })) {
#line 2375 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2376 "src/parser.tr"
                    List_ptr_append(ifs, Parser_parse_or_expr(self));
                }
#line 2377 "src/parser.tr"
                Comprehension* c = Comprehension_init(target, iter);
#line 2378 "src/parser.tr"
                c->ifs = ifs;
#line 2379 "src/parser.tr"
                /* unsafe block */
#line 2380 "src/parser.tr"
                Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
#line 2381 "src/parser.tr"
                (*c_ptr = c);
#line 2382 "src/parser.tr"
                List_ptr_append(generators, c_ptr);
                _tr_str_release(target);
            }
#line 2383 "src/parser.tr"
            Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list comprehension", 23LL));
#line 2384 "src/parser.tr"
            return box_expr(Expr_ctor_EListComp(first, generators));
        }
#line 2385 "src/parser.tr"
        List_ptr* items = (void*)List_ptr_new();
#line 2386 "src/parser.tr"
        List_ptr_append(items, first);
#line 2387 "src/parser.tr"
        if (({ __auto_type _ord_t345 = (Parser_peek(self)); __auto_type _wr = ((_ord_t345.tag == Token_make_Comma().tag)); _wr; })) {
#line 2388 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2389 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2390 "src/parser.tr"
            while ((({ __auto_type _ord_t346 = (Parser_peek(self)); __auto_type _wr = ((_ord_t346.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t347 = (Parser_peek(self)); __auto_type _wr = ((_ord_t347.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2391 "src/parser.tr"
                List_ptr_append(items, Parser_parse_expr(self));
#line 2392 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2393 "src/parser.tr"
                if (({ __auto_type _ord_t348 = (Parser_peek(self)); __auto_type _wr = ((_ord_t348.tag == Token_make_Comma().tag)); _wr; })) {
#line 2394 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 2395 "src/parser.tr"
                    Parser_skip_newlines(self);
                } else {
#line 2397 "src/parser.tr"
                    break;
                }
            }
        }
#line 2398 "src/parser.tr"
        Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit_len("this list", 9LL));
#line 2399 "src/parser.tr"
        return box_expr(Expr_ctor_EList(items));
    } else if (_t320.tag == Token_LBrace) {
#line 2401 "src/parser.tr"
        long long br_oln = Parser_cur_line(self);
#line 2402 "src/parser.tr"
        long long br_ocol = Parser_cur_col(self);
#line 2403 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2404 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2405 "src/parser.tr"
        List_ptr* keys = (void*)List_ptr_new();
#line 2406 "src/parser.tr"
        List_ptr* vals = (void*)List_ptr_new();
#line 2407 "src/parser.tr"
        List_ptr* set_items = (void*)List_ptr_new();
#line 2408 "src/parser.tr"
        bool is_set = false;
#line 2409 "src/parser.tr"
        if (({ __auto_type _ord_t349 = (Parser_peek(self)); __auto_type _wr = ((_ord_t349.tag != Token_make_RBrace().tag)); _wr; })) {
#line 2410 "src/parser.tr"
            Expr* first = Parser_parse_expr(self);
#line 2411 "src/parser.tr"
            Parser_skip_newlines(self);
#line 2412 "src/parser.tr"
            if (({ __auto_type _ord_t350 = (Parser_peek(self)); __auto_type _wr = ((_ord_t350.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2413 "src/parser.tr"
                List_ptr* sgens = Parser_parse_comp_generators(self);
#line 2414 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2415 "src/parser.tr"
                Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this set comprehension", 22LL));
#line 2416 "src/parser.tr"
                return Parser_desugar_collection_comp(self, first, (Expr*)(0LL), sgens);
            }
#line 2417 "src/parser.tr"
            if (({ __auto_type _ord_t351 = (Parser_peek(self)); __auto_type _wr = ((_ord_t351.tag == Token_make_Colon().tag)); _wr; })) {
#line 2418 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2419 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2420 "src/parser.tr"
                Expr* value = Parser_parse_expr(self);
#line 2421 "src/parser.tr"
                if (({ __auto_type _ord_t352 = (Parser_peek(self)); __auto_type _wr = ((_ord_t352.tag == Token_make_KwFor().tag)); _wr; })) {
#line 2422 "src/parser.tr"
                    List_ptr* dgens = Parser_parse_comp_generators(self);
#line 2423 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2424 "src/parser.tr"
                    Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict comprehension", 23LL));
#line 2425 "src/parser.tr"
                    return Parser_desugar_collection_comp(self, first, value, dgens);
                }
#line 2426 "src/parser.tr"
                List_ptr_append(keys, first);
#line 2427 "src/parser.tr"
                List_ptr_append(vals, value);
#line 2428 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2429 "src/parser.tr"
                if (({ __auto_type _ord_t353 = (Parser_peek(self)); __auto_type _wr = ((_ord_t353.tag == Token_make_Comma().tag)); _wr; })) {
#line 2430 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2431 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2432 "src/parser.tr"
                while ((({ __auto_type _ord_t354 = (Parser_peek(self)); __auto_type _wr = ((_ord_t354.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t355 = (Parser_peek(self)); __auto_type _wr = ((_ord_t355.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2433 "src/parser.tr"
                    Expr* key = Parser_parse_expr(self);
#line 2434 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2435 "src/parser.tr"
                    __auto_type _t356 = Parser_peek(self);
                    if (_t356.tag == Token_Colon) {
#line 2437 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t356;
#line 2439 "src/parser.tr"
                        /* pass */
                    }
#line 2440 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2441 "src/parser.tr"
                    Expr* v = Parser_parse_expr(self);
#line 2442 "src/parser.tr"
                    List_ptr_append(keys, key);
#line 2443 "src/parser.tr"
                    List_ptr_append(vals, v);
#line 2444 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2445 "src/parser.tr"
                    if (({ __auto_type _ord_t357 = (Parser_peek(self)); __auto_type _wr = ((_ord_t357.tag == Token_make_Comma().tag)); _wr; })) {
#line 2446 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2448 "src/parser.tr"
                        break;
                    }
#line 2449 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            } else {
#line 2451 "src/parser.tr"
                is_set = true;
#line 2452 "src/parser.tr"
                List_ptr_append(set_items, first);
#line 2453 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2454 "src/parser.tr"
                if (({ __auto_type _ord_t358 = (Parser_peek(self)); __auto_type _wr = ((_ord_t358.tag == Token_make_Comma().tag)); _wr; })) {
#line 2455 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
#line 2456 "src/parser.tr"
                Parser_skip_newlines(self);
#line 2457 "src/parser.tr"
                while ((({ __auto_type _ord_t359 = (Parser_peek(self)); __auto_type _wr = ((_ord_t359.tag != Token_make_RBrace().tag)); _wr; }) && ({ __auto_type _ord_t360 = (Parser_peek(self)); __auto_type _wr = ((_ord_t360.tag != Token_make_Eof().tag)); _wr; }))) {
#line 2458 "src/parser.tr"
                    List_ptr_append(set_items, Parser_parse_expr(self));
#line 2459 "src/parser.tr"
                    Parser_skip_newlines(self);
#line 2460 "src/parser.tr"
                    if (({ __auto_type _ord_t361 = (Parser_peek(self)); __auto_type _wr = ((_ord_t361.tag == Token_make_Comma().tag)); _wr; })) {
#line 2461 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else {
#line 2463 "src/parser.tr"
                        break;
                    }
#line 2464 "src/parser.tr"
                    Parser_skip_newlines(self);
                }
            }
        }
#line 2465 "src/parser.tr"
        Parser_skip_newlines(self);
#line 2466 "src/parser.tr"
        Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit_len("this dict/set literal", 21LL));
#line 2467 "src/parser.tr"
        if (is_set) {
#line 2468 "src/parser.tr"
            return box_expr(Expr_ctor_ESet(set_items));
        } else {
#line 2470 "src/parser.tr"
            return box_expr(Expr_ctor_EDict(keys, vals));
        }
    } else if (_t320.tag == Token_KwTry) {
#line 2472 "src/parser.tr"
        Stmt* st = Parser_parse_try_stmt(self);
#line 2473 "src/parser.tr"
        __auto_type _t362 = (*st);
        if (_t362.tag == Stmt_STry) {
            __auto_type try_body = _t362.data.STry.try_body;
__auto_type catches = _t362.data.STry.catches;
__auto_type finally_b = _t362.data.STry.finally_b;
#line 2475 "src/parser.tr"
            Expr* _trret61 = box_expr(Expr_ctor_ETry(try_body, catches, finally_b));
            return _trret61;
        } else if (1) {
            __auto_type _ = _t362;
#line 2477 "src/parser.tr"
            /* pass */
        }
    } else if (_t320.tag == Token_KwAwait) {
#line 2479 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2480 "src/parser.tr"
        return box_expr(Expr_ctor_EAwait(Parser_parse_expr(self)));
    } else if (_t320.tag == Token_KwYield) {
#line 2482 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2483 "src/parser.tr"
        return box_expr(Expr_ctor_EYield(Parser_parse_expr(self)));
    } else if ((_t320.tag == Token_KwDef || _t320.tag == Token_KwAsync)) {
#line 2485 "src/parser.tr"
        bool is_async = false;
#line 2486 "src/parser.tr"
        if (({ __auto_type _ord_t363 = (Parser_peek(self)); __auto_type _wr = ((_ord_t363.tag == Token_make_KwAsync().tag)); _wr; })) {
#line 2487 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2488 "src/parser.tr"
            is_async = true;
        }
#line 2489 "src/parser.tr"
        __auto_type _t364 = Parser_peek(self);
        if (_t364.tag == Token_KwDef) {
#line 2491 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t364;
#line 2493 "src/parser.tr"
            /* pass */
        }
#line 2494 "src/parser.tr"
        __auto_type _t365 = Parser_peek(self);
        if (_t365.tag == Token_LParen) {
#line 2496 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t365;
#line 2498 "src/parser.tr"
            /* pass */
        }
#line 2499 "src/parser.tr"
        List_ptr* params = Parser_parse_param_list(self);
#line 2500 "src/parser.tr"
        Parser_close_params(self);
#line 2501 "src/parser.tr"
        AstType** return_type = (AstType**)(0LL);
#line 2502 "src/parser.tr"
        AstType** clo_throws = (AstType**)(0LL);
#line 2503 "src/parser.tr"
        if (({ __auto_type _ord_t366 = (Parser_peek(self)); __auto_type _wr = ((_ord_t366.tag == Token_make_KwThrows().tag)); _wr; })) {
#line 2504 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2505 "src/parser.tr"
            clo_throws = box_asttype(Parser_parse_type(self));
        }
#line 2506 "src/parser.tr"
        if (({ __auto_type _ord_t367 = (Parser_peek(self)); __auto_type _wr = ((_ord_t367.tag == Token_make_Arrow().tag)); _wr; })) {
#line 2507 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2508 "src/parser.tr"
            return_type = box_asttype(Parser_parse_type(self));
        }
#line 2509 "src/parser.tr"
        if ((((unsigned long long)(clo_throws)) != ((unsigned long long)(0LL)))) {
#line 2510 "src/parser.tr"
            AstType* clo_rt = AstType_init(_tr_str_lit_len("void", 4LL));
#line 2511 "src/parser.tr"
            if ((((unsigned long long)(return_type)) != ((unsigned long long)(0LL)))) {
#line 2511 "src/parser.tr"
                clo_rt = (*return_type);
            }
#line 2512 "src/parser.tr"
            return_type = box_asttype(throws_result_type(clo_rt, clo_throws));
        }
#line 2513 "src/parser.tr"
        __auto_type _t368 = Parser_peek(self);
        if (_t368.tag == Token_Colon) {
#line 2515 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t368;
#line 2517 "src/parser.tr"
            /* pass */
        }
#line 2518 "src/parser.tr"
        Block* body = Parser_parse_block(self);
#line 2519 "src/parser.tr"
        Expr* _trret62 = box_expr(Expr_ctor_EClosure(params, return_type, body, is_async));
        return _trret62;
    } else if (_t320.tag == Token_KwLambda) {
#line 2523 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2524 "src/parser.tr"
        List_ptr* lparams = (void*)List_ptr_new();
#line 2525 "src/parser.tr"
        while (((({ __auto_type _ord_t369 = (Parser_peek(self)); __auto_type _wr = ((_ord_t369.tag != Token_make_Colon().tag)); _wr; }) && ({ __auto_type _ord_t370 = (Parser_peek(self)); __auto_type _wr = ((_ord_t370.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t371 = (Parser_peek(self)); __auto_type _wr = ((_ord_t371.tag != Token_make_Newline().tag)); _wr; }))) {
#line 2526 "src/parser.tr"
            TrStr lpn = Parser_consume_ident(self);
#line 2527 "src/parser.tr"
            if (_tr_str_eqv((lpn), (_tr_str_lit_len("", 0LL)))) {
#line 2527 "src/parser.tr"
                _tr_str_release(lpn);
                break;
            }
#line 2528 "src/parser.tr"
            List_ptr_append(lparams, Param_init(lpn, (AstType**)(0LL)));
#line 2529 "src/parser.tr"
            if (({ __auto_type _ord_t372 = (Parser_peek(self)); __auto_type _wr = ((_ord_t372.tag == Token_make_Comma().tag)); _wr; })) {
#line 2530 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(lpn);
        }
#line 2531 "src/parser.tr"
        __auto_type _t373 = Parser_peek(self);
        if (_t373.tag == Token_Colon) {
#line 2533 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t373;
#line 2535 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected ':' after the parameters of a 'lambda'", 47LL), _tr_str_lit_len("write `lambda x, y: expr`", 25LL));
        }
#line 2536 "src/parser.tr"
        Expr* lbody_e = Parser_parse_expr(self);
#line 2537 "src/parser.tr"
        Block* lbody = Block_init();
#line 2538 "src/parser.tr"
        Block_push(lbody, box_stmt(Stmt_ctor_SReturn(lbody_e)));
#line 2539 "src/parser.tr"
        Expr* _trret63 = box_expr(Expr_ctor_EClosure(lparams, (AstType**)(0LL), lbody, false));
        return _trret63;
    } else if (_t320.tag == Token_FStrLit) {
        __auto_type raw = _t320.data.FStrLit.val;
__auto_type raw_blen = _t320.data.FStrLit.blen;
#line 2541 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2542 "src/parser.tr"
        Expr* _trret64 = Parser_parse_fstring(self, raw, raw_blen);
        return _trret64;
    } else if (1) {
        __auto_type _ = _t320;
#line 2544 "src/parser.tr"
        /* pass */
    }
#line 2545 "src/parser.tr"
    if ((!Parser_at_end(self))) {
#line 2546 "src/parser.tr"
        TrStr tok_desc = _tr_str_lit_len("token", 5LL);
#line 2547 "src/parser.tr"
        TrStr hint = _tr_str_lit_len("check for a typo, a missing ':' or unbalanced parentheses/brackets near this point.", 83LL);
#line 2548 "src/parser.tr"
        __auto_type _t374 = Parser_peek(self);
        if (_t374.tag == Token_Newline) {
#line 2550 "src/parser.tr"
            TrStr _strtmp_t375 = _tr_str_lit_len("end of line", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t375;
#line 2551 "src/parser.tr"
            TrStr _strtmp_t376 = _tr_str_lit_len("an expression was expected before the end of this line - check for a missing value or trailing operator.", 104LL);
            _tr_str_release(hint);
            hint = _strtmp_t376;
        } else if (_t374.tag == Token_Indent) {
#line 2553 "src/parser.tr"
            TrStr _strtmp_t377 = _tr_str_lit_len("indentation", 11LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t377;
#line 2554 "src/parser.tr"
            TrStr _strtmp_t378 = _tr_str_lit_len("check that this line's indentation matches the surrounding block.", 65LL);
            _tr_str_release(hint);
            hint = _strtmp_t378;
        } else if (_t374.tag == Token_Dedent) {
#line 2556 "src/parser.tr"
            TrStr _strtmp_t379 = _tr_str_lit_len("dedent", 6LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t379;
#line 2557 "src/parser.tr"
            TrStr _strtmp_t380 = _tr_str_lit_len("check that this block is properly indented and closed.", 54LL);
            _tr_str_release(hint);
            hint = _strtmp_t380;
        } else if (_t374.tag == Token_Ident) {
            __auto_type n = _t374.data.Ident.name;
#line 2559 "src/parser.tr"
            TrStr _strtmp_t381 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("identifier '", 12LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t381;
#line 2560 "src/parser.tr"
            TrStr _strtmp_t382 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("an operator, ':' or end of statement was expected before '", 58LL)), (n))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'.", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(hint);
            hint = _strtmp_t382;
        } else if (_t374.tag == Token_KwMut) {
            TrStr _strtmp_t383 = _tr_str_lit_len("keyword 'mut'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t383;
        } else if (_t374.tag == Token_KwConst) {
            TrStr _strtmp_t384 = _tr_str_lit_len("keyword 'const'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t384;
        } else if (_t374.tag == Token_KwPub) {
            TrStr _strtmp_t385 = _tr_str_lit_len("keyword 'pub'", 13LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t385;
        } else if (_t374.tag == Token_KwReturn) {
            TrStr _strtmp_t386 = _tr_str_lit_len("keyword 'return'", 16LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t386;
        } else if (_t374.tag == Token_KwIf) {
            TrStr _strtmp_t387 = _tr_str_lit_len("keyword 'if'", 12LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t387;
        } else if (_t374.tag == Token_KwWhile) {
            TrStr _strtmp_t388 = _tr_str_lit_len("keyword 'while'", 15LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t388;
        } else if (_t374.tag == Token_Comma) {
#line 2568 "src/parser.tr"
            TrStr _strtmp_t389 = _tr_str_lit_len("','", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t389;
#line 2569 "src/parser.tr"
            TrStr _strtmp_t390 = _tr_str_lit_len("remove the extra ',' or add the missing item before it.", 55LL);
            _tr_str_release(hint);
            hint = _strtmp_t390;
        } else if (_t374.tag == Token_Colon) {
#line 2571 "src/parser.tr"
            TrStr _strtmp_t391 = _tr_str_lit_len("':'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t391;
#line 2572 "src/parser.tr"
            TrStr _strtmp_t392 = _tr_str_lit_len("remove the extra ':' or check the statement before it is complete.", 66LL);
            _tr_str_release(hint);
            hint = _strtmp_t392;
        } else if (_t374.tag == Token_RParen) {
#line 2574 "src/parser.tr"
            TrStr _strtmp_t393 = _tr_str_lit_len("')'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t393;
#line 2575 "src/parser.tr"
            TrStr _strtmp_t394 = _tr_str_lit_len("check for an extra ')' or a missing matching '('.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t394;
        } else if (_t374.tag == Token_RBracket) {
#line 2577 "src/parser.tr"
            TrStr _strtmp_t395 = _tr_str_lit_len("']'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t395;
#line 2578 "src/parser.tr"
            TrStr _strtmp_t396 = _tr_str_lit_len("check for an extra ']' or a missing matching '['.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t396;
        } else if (_t374.tag == Token_RBrace) {
#line 2580 "src/parser.tr"
            TrStr _strtmp_t397 = _tr_str_lit_len("'}'", 3LL);
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t397;
#line 2581 "src/parser.tr"
            TrStr _strtmp_t398 = _tr_str_lit_len("check for an extra '}' or a missing matching '{'.", 49LL);
            _tr_str_release(hint);
            hint = _strtmp_t398;
        } else if (_t374.tag == Token_Error) {
            __auto_type em = _t374.data.Error.msg;
#line 2585 "src/parser.tr"
            Parser_emit_diag(self, em, _tr_str_lit_len("", 0LL));
#line 2586 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2587 "src/parser.tr"
            Expr* _trret65 = box_expr(Expr_make_ELitNone());
            return _trret65;
        } else if (1) {
            __auto_type _ = _t374;
            /* pass */
        }
#line 2589 "src/parser.tr"
        if (_tr_str_eqv((tok_desc), (_tr_str_lit_len("token", 5LL)))) {
#line 2592 "src/parser.tr"
            TrStr kw = _tr_str_lit_len("", 0LL);
#line 2593 "src/parser.tr"
            long long kln = Parser_cur_line(self);
#line 2594 "src/parser.tr"
            long long kcol = Parser_cur_col(self);
#line 2595 "src/parser.tr"
            if ((((_tr_str_lenv((self->src_text)) > 0LL) && (kln > 0LL)) && (kcol > 0LL))) {
#line 2596 "src/parser.tr"
                TrStr ksrc = ({ TrStr _apt_t399 = _tr_str_retain(self->src_text); __auto_type _wr = (_nth_source_line(_apt_t399, kln)); _tr_str_release(_apt_t399); _wr; });
#line 2597 "src/parser.tr"
                long long kend = (kcol - 1LL);
#line 2598 "src/parser.tr"
                while (((kend < _tr_str_lenv((ksrc))) && (((_tr_str_char_at_codev((ksrc), kend) >= 97LL) && (_tr_str_char_at_codev((ksrc), kend) <= 122LL)) || (_tr_str_char_at_codev((ksrc), kend) == 95LL)))) {
#line 2599 "src/parser.tr"
                    kend = (kend + 1LL);
                }
#line 2600 "src/parser.tr"
                if ((kend > (kcol - 1LL))) {
#line 2600 "src/parser.tr"
                    TrStr _strtmp_t400 = _tr_str_slicev((ksrc), (kcol - 1LL), kend);
                    _tr_str_release(kw);
                    kw = _strtmp_t400;
                }
            }
#line 2601 "src/parser.tr"
            bool kw_ok = ((_tr_str_lenv((kw)) > 1LL) && (_tr_str_lenv((kw)) < 16LL));
#line 2602 "src/parser.tr"
            long long ki = 0LL;
#line 2603 "src/parser.tr"
            while ((kw_ok && (ki < _tr_str_lenv((kw))))) {
#line 2604 "src/parser.tr"
                long long kc = _tr_str_char_at_codev((kw), ki);
#line 2605 "src/parser.tr"
                if ((!(((kc >= 97LL) && (kc <= 122LL)) || (kc == 95LL)))) {
#line 2605 "src/parser.tr"
                    kw_ok = false;
                }
#line 2606 "src/parser.tr"
                ki = (ki + 1LL);
            }
#line 2607 "src/parser.tr"
            if (kw_ok) {
#line 2608 "src/parser.tr"
                TrStr _strtmp_t401 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("keyword '", 9LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(tok_desc);
                tok_desc = _strtmp_t401;
#line 2609 "src/parser.tr"
                TrStr _strtmp_t402 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (kw))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a reserved word and cannot be used as a name or value here - rename it (for example '", 90LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kw)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_').", 4LL))); _tr_str_release(_tr_cl); _tr_cres; });
                _tr_str_release(hint);
                hint = _strtmp_t402;
            }
        }
#line 2610 "src/parser.tr"
        ({ TrStr _at_t403 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unexpected ", 11LL)), (tok_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" in expression", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t403, hint); _tr_str_release(_at_t403); });
#line 2611 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 2612 "src/parser.tr"
    return box_expr(Expr_make_ELitNone());
}

__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, TrStr raw, long long raw_blen) {
#line 2634 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 2635 "src/parser.tr"
    char* p = ((char*)(_tr_strz(raw)));
#line 2636 "src/parser.tr"
    long long i = 0LL;
#line 2637 "src/parser.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 2639 "src/parser.tr"
    while ((i < raw_blen)) {
#line 2640 "src/parser.tr"
        long long c = ((long long)((*_TR_PTR_OFF(p, i))));
#line 2641 "src/parser.tr"
        long long c_next = 0LL;
#line 2642 "src/parser.tr"
        if (((i + 1LL) < raw_blen)) {
#line 2642 "src/parser.tr"
            c_next = ((long long)((*_TR_PTR_OFF(p, (i + 1LL)))));
        }
#line 2644 "src/parser.tr"
        if ((((c == 123LL) && (c_next == 123LL)) || ((c == 125LL) && (c_next == 125LL)))) {
#line 2646 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2647 "src/parser.tr"
            i = (i + 2LL);
        } else if ((c == 123LL)) {
#line 2650 "src/parser.tr"
            if ((sb->buf->len > 0LL)) {
#line 2651 "src/parser.tr"
                ({ TrStr _ord_t404 = (StringBuilder_to_owned(sb)); __auto_type _ord_t405 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t404, _ord_t405)); _tr_str_release(_ord_t404); });
#line 2652 "src/parser.tr"
                sb = StringBuilder_init(64LL);
            }
#line 2654 "src/parser.tr"
            i = (i + 1LL);
#line 2655 "src/parser.tr"
            StringBuilder* expr_sb = StringBuilder_init(64LL);
#line 2656 "src/parser.tr"
            long long depth = 1LL;
#line 2657 "src/parser.tr"
            while (((depth > 0LL) && (i < raw_blen))) {
#line 2658 "src/parser.tr"
                long long ec = ((long long)((*_TR_PTR_OFF(p, i))));
#line 2659 "src/parser.tr"
                if (((ec == 34LL) || (ec == 39LL))) {
#line 2662 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
#line 2663 "src/parser.tr"
                    i = (i + 1LL);
#line 2664 "src/parser.tr"
                    while (((i < raw_blen) && (((long long)((*_TR_PTR_OFF(p, i)))) != ec))) {
#line 2665 "src/parser.tr"
                        long long sc = ((long long)((*_TR_PTR_OFF(p, i))));
#line 2666 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, sc);
#line 2667 "src/parser.tr"
                        i = (i + 1LL);
#line 2668 "src/parser.tr"
                        if (((sc == 92LL) && (i < raw_blen))) {
#line 2669 "src/parser.tr"
                            StringBuilder_append_char(expr_sb, ((long long)((*_TR_PTR_OFF(p, i)))));
#line 2670 "src/parser.tr"
                            i = (i + 1LL);
                        }
                    }
#line 2671 "src/parser.tr"
                    if ((i < raw_blen)) {
#line 2672 "src/parser.tr"
                        StringBuilder_append_char(expr_sb, ec);
#line 2673 "src/parser.tr"
                        i = (i + 1LL);
                    }
#line 2674 "src/parser.tr"
                    continue;
                }
#line 2675 "src/parser.tr"
                if ((ec == 123LL)) {
#line 2675 "src/parser.tr"
                    depth = (depth + 1LL);
                }
#line 2676 "src/parser.tr"
                if ((ec == 125LL)) {
#line 2676 "src/parser.tr"
                    depth = (depth - 1LL);
                }
#line 2677 "src/parser.tr"
                if ((depth > 0LL)) {
#line 2677 "src/parser.tr"
                    StringBuilder_append_char(expr_sb, ec);
                }
#line 2678 "src/parser.tr"
                i = (i + 1LL);
            }
#line 2682 "src/parser.tr"
            TrStr expr_str = StringObj_as_str(StringBuilder_to_string(expr_sb));
#line 2683 "src/parser.tr"
            TrStr fmt_spec = _tr_str_lit_len("", 0LL);
#line 2684 "src/parser.tr"
            long long colon_pos = _find_fmt_colon(expr_str);
#line 2685 "src/parser.tr"
            if ((colon_pos >= 0LL)) {
#line 2686 "src/parser.tr"
                TrStr _fs = _tr_str_slicev((expr_str), (colon_pos + 1LL), _tr_str_lenv((expr_str)));
#line 2687 "src/parser.tr"
                TrStr _strtmp_t406 = _tr_str_stripv((_fs));
                _tr_str_release(fmt_spec);
                fmt_spec = _strtmp_t406;
#line 2688 "src/parser.tr"
                TrStr _es = _tr_str_slicev((expr_str), 0LL, colon_pos);
#line 2689 "src/parser.tr"
                TrStr _strtmp_t407 = _tr_str_stripv((_es));
                _tr_str_release(expr_str);
                expr_str = _strtmp_t407;
                _tr_str_release(_fs);
                _tr_str_release(_es);
            }
#line 2690 "src/parser.tr"
            Lexer* lexer = Lexer_init(expr_str);
#line 2691 "src/parser.tr"
            List_Token* _fstr_tokens = Lexer_tokenize(lexer);
#line 2692 "src/parser.tr"
            if ((_fstr_tokens->len > 0LL)) {
#line 2693 "src/parser.tr"
                Parser* parser = Parser_init(_fstr_tokens, lexer->token_lines);
#line 2694 "src/parser.tr"
                Expr* e = Parser_parse_expr(parser);
#line 2695 "src/parser.tr"
                if ((_tr_str_lenv((fmt_spec)) > 0LL)) {
#line 2696 "src/parser.tr"
                    FStringPart* fpart = FStringPart_init_expr_fmt(e, fmt_spec);
#line 2698 "src/parser.tr"
                    long long sk = 0LL;
#line 2699 "src/parser.tr"
                    while ((sk < _tr_str_lenv((fmt_spec)))) {
#line 2700 "src/parser.tr"
                        if ((_tr_str_char_at_codev((fmt_spec), sk) == 123LL)) {
#line 2701 "src/parser.tr"
                            long long se = (sk + 1LL);
#line 2702 "src/parser.tr"
                            while (((se < _tr_str_lenv((fmt_spec))) && (_tr_str_char_at_codev((fmt_spec), se) != 125LL))) {
#line 2702 "src/parser.tr"
                                se = (se + 1LL);
                            }
#line 2703 "src/parser.tr"
                            if ((se >= _tr_str_lenv((fmt_spec)))) {
#line 2703 "src/parser.tr"
                                break;
                            }
#line 2704 "src/parser.tr"
                            TrStr sub_raw = _tr_str_slicev((fmt_spec), (sk + 1LL), se);
#line 2705 "src/parser.tr"
                            TrStr sub_src = _tr_str_stripv((sub_raw));
#line 2706 "src/parser.tr"
                            Lexer* slex = Lexer_init(sub_src);
#line 2707 "src/parser.tr"
                            List_Token* stoks = Lexer_tokenize(slex);
#line 2708 "src/parser.tr"
                            if ((stoks->len > 0LL)) {
#line 2709 "src/parser.tr"
                                Parser* sparser = Parser_init(stoks, slex->token_lines);
#line 2710 "src/parser.tr"
                                ({ __auto_type _ord_t408 = (Parser_parse_expr(sparser)); List_ptr_append(fpart->spec_args, _ord_t408); });
                                _tr_obj_release(sparser, _trdrop_Parser);
                            }
#line 2711 "src/parser.tr"
                            sk = (se + 1LL);
                            _tr_str_release(sub_raw);
                            _tr_str_release(sub_src);
                            _tr_obj_release(slex, _trdrop_Lexer);
                        } else {
#line 2713 "src/parser.tr"
                            sk = (sk + 1LL);
                        }
                    }
#line 2714 "src/parser.tr"
                    List_ptr_append(fl, _tr_obj_retain(fpart));
                } else {
#line 2716 "src/parser.tr"
                    List_ptr_append(fl, FStringPart_init_expr(e));
                }
                _tr_obj_release(parser, _trdrop_Parser);
            }
            _tr_str_release(expr_str);
            StringBuilder__tr_fn_free(expr_sb);
            _tr_obj_release(lexer, _trdrop_Lexer);
        } else if (((c == 92LL) && ((i + 1LL) < raw_blen))) {
#line 2718 "src/parser.tr"
            long long esc = ((long long)((*_TR_PTR_OFF(p, (i + 1LL)))));
#line 2719 "src/parser.tr"
            if ((esc == 110LL)) {
#line 2719 "src/parser.tr"
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
#line 2720 "src/parser.tr"
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
#line 2721 "src/parser.tr"
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
#line 2722 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
#line 2723 "src/parser.tr"
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
#line 2724 "src/parser.tr"
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
#line 2725 "src/parser.tr"
                StringBuilder_append_char(sb, 0LL);
            } else if (((((esc == 120LL) || (esc == 117LL)) || (esc == 85LL)) && _fstr_hex_ok(p, (i + 2LL), raw_blen, esc))) {
#line 2728 "src/parser.tr"
                long long fnd = 2LL;
#line 2729 "src/parser.tr"
                if ((esc == 117LL)) {
#line 2729 "src/parser.tr"
                    fnd = 4LL;
                }
#line 2730 "src/parser.tr"
                if ((esc == 85LL)) {
#line 2730 "src/parser.tr"
                    fnd = 8LL;
                }
#line 2731 "src/parser.tr"
                long long fhv = 0LL;
#line 2732 "src/parser.tr"
                long long fk = 0LL;
#line 2733 "src/parser.tr"
                while ((fk < fnd)) {
#line 2734 "src/parser.tr"
                    fhv = ((fhv * 16LL) + lex_hex_val(((long long)((*_TR_PTR_OFF(p, ((i + 2LL) + fk)))))));
#line 2735 "src/parser.tr"
                    fk = (fk + 1LL);
                }
#line 2736 "src/parser.tr"
                if ((esc == 120LL)) {
#line 2736 "src/parser.tr"
                    StringBuilder_append_char(sb, fhv);
                } else {
#line 2737 "src/parser.tr"
                    lex_append_utf8(sb, fhv);
                }
#line 2738 "src/parser.tr"
                i = (i + fnd);
            } else {
#line 2742 "src/parser.tr"
                StringBuilder_append_char(sb, 92LL);
#line 2743 "src/parser.tr"
                StringBuilder_append_char(sb, esc);
            }
#line 2744 "src/parser.tr"
            i = (i + 2LL);
        } else {
#line 2746 "src/parser.tr"
            StringBuilder_append_char(sb, c);
#line 2747 "src/parser.tr"
            i = (i + 1LL);
        }
    }
#line 2749 "src/parser.tr"
    if ((sb->buf->len > 0LL)) {
#line 2750 "src/parser.tr"
        ({ TrStr _ord_t409 = (StringBuilder_to_owned(sb)); __auto_type _ord_t410 = (sb->buf->len); List_ptr_append(fl, FStringPart_init_text(_ord_t409, _ord_t410)); _tr_str_release(_ord_t409); });
    }
#line 2752 "src/parser.tr"
    StringBuilder__tr_fn_free(sb);
    return box_expr(Expr_ctor_EFString(fl));
}

__attribute__((hot)) long long Parser_nesting_too_deep(Parser* self) {
#line 2758 "src/parser.tr"
    long long depth = 0LL;
#line 2759 "src/parser.tr"
    long long indent = 0LL;
#line 2760 "src/parser.tr"
    long long run = 0LL;
#line 2761 "src/parser.tr"
    long long lam = 0LL;
#line 2762 "src/parser.tr"
    bool prev_colon = false;
#line 2763 "src/parser.tr"
    long long i = 0LL;
#line 2764 "src/parser.tr"
    while ((i < self->tokens->len)) {
#line 2765 "src/parser.tr"
        bool is_prefix = false;
#line 2766 "src/parser.tr"
        __auto_type _t411 = List_Token_get(self->tokens, i);
        if (_t411.tag == Token_KwLambda) {
#line 2768 "src/parser.tr"
            if (prev_colon) {
#line 2768 "src/parser.tr"
                lam = (lam + 1LL);
            } else {
#line 2769 "src/parser.tr"
                lam = 0LL;
            }
        } else if (_t411.tag == Token_Colon) {
            /* pass */
        } else if ((_t411.tag == Token_Newline || _t411.tag == Token_Comma || _t411.tag == Token_RParen || _t411.tag == Token_RBracket || _t411.tag == Token_RBrace)) {
            lam = 0LL;
        } else if (1) {
            __auto_type _ = _t411;
            /* pass */
        }
#line 2773 "src/parser.tr"
        prev_colon = ({ __auto_type _ord_t412 = (List_Token_get(self->tokens, i)); __auto_type _wr = ((_ord_t412.tag == Token_make_Colon().tag)); _wr; });
#line 2774 "src/parser.tr"
        __auto_type _t413 = List_Token_get(self->tokens, i);
        if ((_t413.tag == Token_LParen || _t413.tag == Token_LBracket || _t413.tag == Token_LBrace)) {
#line 2776 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((_t413.tag == Token_RParen || _t413.tag == Token_RBracket || _t413.tag == Token_RBrace)) {
#line 2778 "src/parser.tr"
            if ((depth > 0LL)) {
#line 2778 "src/parser.tr"
                depth = (depth - 1LL);
            }
        } else if (_t413.tag == Token_Indent) {
#line 2780 "src/parser.tr"
            indent = (indent + 1LL);
        } else if (_t413.tag == Token_Dedent) {
#line 2782 "src/parser.tr"
            if ((indent > 0LL)) {
#line 2782 "src/parser.tr"
                indent = (indent - 1LL);
            }
        } else if ((_t413.tag == Token_Minus || _t413.tag == Token_Tilde || _t413.tag == Token_KwNot || _t413.tag == Token_KwAwait || _t413.tag == Token_Bang)) {
#line 2784 "src/parser.tr"
            is_prefix = true;
        } else if (1) {
            __auto_type _ = _t413;
#line 2786 "src/parser.tr"
            /* pass */
        }
#line 2787 "src/parser.tr"
        if (is_prefix) {
#line 2787 "src/parser.tr"
            run = (run + 1LL);
        } else {
#line 2788 "src/parser.tr"
            run = 0LL;
        }
#line 2789 "src/parser.tr"
        if (((((depth > 120LL) || (indent > 100LL)) || (run > 120LL)) || (lam > 100LL))) {
#line 2789 "src/parser.tr"
            return i;
        }
#line 2790 "src/parser.tr"
        i = (i + 1LL);
    }
#line 2791 "src/parser.tr"
    return (-1LL);
}

__attribute__((hot)) Program* Parser_parse_program(Parser* self) {
#line 2794 "src/parser.tr"
    Program* prog = Program_init();
#line 2795 "src/parser.tr"
    long long deep_at = Parser_nesting_too_deep(self);
#line 2796 "src/parser.tr"
    if ((deep_at >= 0LL)) {
#line 2797 "src/parser.tr"
        self->pos = deep_at;
#line 2798 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("this construct is nested too deeply", 35LL), _tr_str_lit_len("the compiler supports at most 120 nested brackets, 100 nested blocks or 120 chained prefix operators - split the expression or move the inner part into a helper function.", 170LL));
#line 2799 "src/parser.tr"
        self->pos = self->tokens->len;
#line 2800 "src/parser.tr"
        return prog;
    }
#line 2801 "src/parser.tr"
    Parser_skip_newlines_and_indent(self);
#line 2802 "src/parser.tr"
    while ((!Parser_at_end(self))) {
#line 2807 "src/parser.tr"
        bool sk = true;
#line 2808 "src/parser.tr"
        while (sk) {
#line 2809 "src/parser.tr"
            sk = false;
#line 2810 "src/parser.tr"
            __auto_type _t414 = Parser_peek(self);
            if ((_t414.tag == Token_Newline || _t414.tag == Token_Dedent)) {
#line 2812 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2813 "src/parser.tr"
                sk = true;
            } else if (_t414.tag == Token_Indent) {
#line 2815 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("unexpected indentation", 22LL), _tr_str_lit_len("this line is indented but no block was opened above it (a statement ending in ':' and its own line) — remove the extra indentation.", 133LL));
#line 2816 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2817 "src/parser.tr"
                sk = true;
            } else if (1) {
                __auto_type _ = _t414;
#line 2819 "src/parser.tr"
                /* pass */
            }
        }
#line 2820 "src/parser.tr"
        if ((!Parser_at_end(self))) {
#line 2821 "src/parser.tr"
            Program_push(prog, Parser_parse_decl(self));
        }
    }
#line 2822 "src/parser.tr"
    return prog;
}

__attribute__((hot)) Decl* Parser_parse_decl(Parser* self) {
#line 2825 "src/parser.tr"
    List_ptr* decorators = (void*)List_ptr_new();
#line 2826 "src/parser.tr"
    while (({ __auto_type _ord_t415 = (Parser_peek(self)); __auto_type _wr = ((_ord_t415.tag == Token_make_At().tag)); _wr; })) {
#line 2827 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2828 "src/parser.tr"
        TrStr name = Parser_consume_ident(self);
#line 2829 "src/parser.tr"
        Decorator* d = Decorator_init(name);
#line 2830 "src/parser.tr"
        if (({ __auto_type _ord_t416 = (Parser_peek(self)); __auto_type _wr = ((_ord_t416.tag == Token_make_LParen().tag)); _wr; })) {
#line 2831 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2832 "src/parser.tr"
            d->args = Parser_parse_arg_list(self);
        }
#line 2833 "src/parser.tr"
        List_ptr_append(decorators, _tr_obj_retain(d));
#line 2834 "src/parser.tr"
        Parser_expect_newline(self);
        _tr_str_release(name);
        _tr_obj_release(d, _trdrop_Decorator);
    }
#line 2836 "src/parser.tr"
    bool is_public = false;
#line 2837 "src/parser.tr"
    bool is_export = false;
#line 2840 "src/parser.tr"
    bool _scan_mods = true;
#line 2841 "src/parser.tr"
    while (_scan_mods) {
#line 2842 "src/parser.tr"
        _scan_mods = false;
#line 2843 "src/parser.tr"
        __auto_type _t417 = Parser_peek(self);
        if (_t417.tag == Token_KwPub) {
#line 2845 "src/parser.tr"
            is_public = true;
#line 2846 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2847 "src/parser.tr"
            _scan_mods = true;
        } else if (_t417.tag == Token_KwExport) {
#line 2849 "src/parser.tr"
            is_export = true;
#line 2850 "src/parser.tr"
            is_public = true;
#line 2851 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2852 "src/parser.tr"
            _scan_mods = true;
        } else if (1) {
            __auto_type _ = _t417;
#line 2854 "src/parser.tr"
            /* pass */
        }
    }
#line 2855 "src/parser.tr"
    __auto_type _t418 = Parser_peek(self);
    if (_t418.tag == Token_KwFrom) {
#line 2857 "src/parser.tr"
        Decl* _trret66 = Parser_parse_from_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret66;
    } else if (_t418.tag == Token_KwImport) {
#line 2859 "src/parser.tr"
        Decl* _trret67 = Parser_parse_import(self);
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return _trret67;
    } else if (_t418.tag == Token_Ident) {
        __auto_type type_kw = _t418.data.Ident.name;
#line 2862 "src/parser.tr"
        if (_tr_str_eqv((type_kw), (_tr_str_lit_len("type", 4LL)))) {
#line 2863 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 2864 "src/parser.tr"
            TrStr alias_name = Parser_consume_ident(self);
#line 2865 "src/parser.tr"
            if (({ __auto_type _ord_t419 = (Parser_peek(self)); __auto_type _wr = ((_ord_t419.tag == Token_make_Eq().tag)); _wr; })) {
#line 2866 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 2867 "src/parser.tr"
                AstType* target_ty = Parser_parse_type(self);
#line 2868 "src/parser.tr"
                Parser_expect_newline(self);
#line 2869 "src/parser.tr"
                Decl* _trret68 = box_decl(Decl_ctor_DTypeAlias(alias_name, box_asttype(target_ty)));
                _tr_str_release(alias_name);
                List_ptr_free_obj(decorators, _trdrop_Decorator);
                return _trret68;
            }
        }
    } else if (_t418.tag == Token_KwDef) {
#line 2871 "src/parser.tr"
        long long _fline = Parser_cur_line(self);
#line 2872 "src/parser.tr"
        long long _fpos = (self->pos + 1LL);
#line 2873 "src/parser.tr"
        FunctionDef* f = Parser_parse_function_def(self, false);
#line 2874 "src/parser.tr"
        if (((f->body->stmts->len == 0LL) && (!_tr_str_eqv((f->name), (_tr_str_lit_len("", 0LL)))))) {
#line 2877 "src/parser.tr"
            long long _fend = self->pos;
#line 2878 "src/parser.tr"
            self->pos = _fpos;
#line 2879 "src/parser.tr"
            ({ TrStr _at_t420 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("function '", 10LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' has no body", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t420, _tr_str_lit_len("add ':' and an indented body (use 'pass' for an empty one), or declare it inside an 'extern \"C\":' block if it is defined in C.", 126LL)); _tr_str_release(_at_t420); });
#line 2880 "src/parser.tr"
            self->pos = _fend;
        }
#line 2881 "src/parser.tr"
        f->line = _fline;
#line 2882 "src/parser.tr"
        f->is_public = is_public;
#line 2883 "src/parser.tr"
        f->is_export = is_export;
#line 2884 "src/parser.tr"
        f->decorators = decorators;
#line 2885 "src/parser.tr"
        Decl* _trret69 = box_decl(Decl_ctor_DFunction(f));
        _tr_obj_release(f, _trdrop_FunctionDef);
        return _trret69;
    } else if (_t418.tag == Token_KwClass) {
#line 2887 "src/parser.tr"
        long long _cline = Parser_cur_line(self);
#line 2888 "src/parser.tr"
        Decl* c_ptr = Parser_parse_class_decl(self);
#line 2889 "src/parser.tr"
        __auto_type _t421 = (*c_ptr);
        if (_t421.tag == Decl_DClass) {
            __auto_type c = _t421.data.DClass.cls;
#line 2891 "src/parser.tr"
            c->line = _cline;
#line 2892 "src/parser.tr"
            c->is_public = is_public;
#line 2899 "src/parser.tr"
            bool _is_vt = false;
#line 2900 "src/parser.tr"
            long long _dvi = 0LL;
#line 2901 "src/parser.tr"
            while ((_dvi < decorators->len)) {
#line 2902 "src/parser.tr"
                TrStr _dvn = _tr_str_retain(((Decorator*)List_ptr_get(decorators, _dvi))->name);
#line 2903 "src/parser.tr"
                if ((((_tr_str_eqv((_dvn), (_tr_str_lit_len("value_type", 10LL))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("packed", 6LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("aligned", 7LL)))) || _tr_str_eqv((_dvn), (_tr_str_lit_len("union", 5LL))))) {
#line 2903 "src/parser.tr"
                    _is_vt = true;
                }
#line 2904 "src/parser.tr"
                _dvi = (_dvi + 1LL);
                _tr_str_release(_dvn);
            }
#line 2905 "src/parser.tr"
            c->is_class = (!_is_vt);
#line 2906 "src/parser.tr"
            c->decorators = decorators;
#line 2907 "src/parser.tr"
            /* unsafe block */
#line 2907 "src/parser.tr"
            (*c_ptr = Decl_ctor_DClass(c));
        } else if (1) {
            __auto_type _ = _t421;
#line 2909 "src/parser.tr"
            /* pass */
        }
#line 2910 "src/parser.tr"
        return c_ptr;
    } else if (_t418.tag == Token_KwEnum) {
#line 2912 "src/parser.tr"
        long long _eline = Parser_cur_line(self);
#line 2913 "src/parser.tr"
        Decl* e_ptr = Parser_parse_enum_decl(self);
#line 2914 "src/parser.tr"
        __auto_type _t422 = (*e_ptr);
        if (_t422.tag == Decl_DEnum) {
            __auto_type e = _t422.data.DEnum.enm;
#line 2916 "src/parser.tr"
            e->line = _eline;
#line 2917 "src/parser.tr"
            e->is_public = is_public;
#line 2918 "src/parser.tr"
            e->decorators = decorators;
#line 2919 "src/parser.tr"
            /* unsafe block */
#line 2919 "src/parser.tr"
            (*e_ptr = Decl_ctor_DEnum(e));
        } else if (1) {
            __auto_type _ = _t422;
#line 2921 "src/parser.tr"
            /* pass */
        }
#line 2922 "src/parser.tr"
        return e_ptr;
    } else if (_t418.tag == Token_KwInterface) {
#line 2924 "src/parser.tr"
        long long _iline = Parser_cur_line(self);
#line 2925 "src/parser.tr"
        Decl* i_ptr = Parser_parse_interface_decl(self);
#line 2926 "src/parser.tr"
        __auto_type _t423 = (*i_ptr);
        if (_t423.tag == Decl_DInterface) {
            __auto_type i = _t423.data.DInterface.iface;
#line 2928 "src/parser.tr"
            i->line = _iline;
#line 2929 "src/parser.tr"
            i->is_public = is_public;
#line 2930 "src/parser.tr"
            i->decorators = decorators;
#line 2931 "src/parser.tr"
            /* unsafe block */
#line 2931 "src/parser.tr"
            (*i_ptr = Decl_ctor_DInterface(i));
        } else if (1) {
            __auto_type _ = _t423;
#line 2933 "src/parser.tr"
            /* pass */
        }
#line 2934 "src/parser.tr"
        return i_ptr;
    } else if (_t418.tag == Token_KwExtend) {
#line 2936 "src/parser.tr"
        return Parser_parse_extend_decl(self);
    } else if (_t418.tag == Token_KwDecorator) {
#line 2938 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2939 "src/parser.tr"
        long long _dl = Parser_cur_line(self);
#line 2940 "src/parser.tr"
        FunctionDef* _df = Parser_parse_function_def(self, false);
#line 2941 "src/parser.tr"
        _df->line = _dl;
#line 2942 "src/parser.tr"
        _df->is_public = is_public;
#line 2943 "src/parser.tr"
        Decl* _trret70 = box_decl(Decl_ctor_DDecoratorDef(_df));
        _tr_obj_release(_df, _trdrop_FunctionDef);
        return _trret70;
    } else if (_t418.tag == Token_KwMacro) {
#line 2948 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2949 "src/parser.tr"
        long long _ml = Parser_cur_line(self);
#line 2950 "src/parser.tr"
        FunctionDef* _mf = Parser_parse_function_def(self, false);
#line 2951 "src/parser.tr"
        _mf->line = _ml;
#line 2952 "src/parser.tr"
        _mf->is_public = is_public;
#line 2953 "src/parser.tr"
        _mf->is_macro = true;
#line 2954 "src/parser.tr"
        Decl* _trret71 = box_decl(Decl_ctor_DDecoratorDef(_mf));
        _tr_obj_release(_mf, _trdrop_FunctionDef);
        return _trret71;
    } else if (_t418.tag == Token_KwExtern) {
#line 2956 "src/parser.tr"
        return Parser_parse_extern_decl(self);
    } else if (_t418.tag == Token_KwAsync) {
#line 2958 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2959 "src/parser.tr"
        __auto_type _t424 = Parser_peek(self);
        if (_t424.tag == Token_KwDef) {
#line 2961 "src/parser.tr"
            FunctionDef* f = Parser_parse_function_def(self, false);
#line 2962 "src/parser.tr"
            f->is_async = true;
#line 2963 "src/parser.tr"
            f->is_public = is_public;
#line 2964 "src/parser.tr"
            f->is_export = is_export;
#line 2965 "src/parser.tr"
            f->decorators = decorators;
#line 2966 "src/parser.tr"
            Decl* _trret72 = box_decl(Decl_ctor_DFunction(f));
            _tr_obj_release(f, _trdrop_FunctionDef);
            return _trret72;
        } else if (1) {
            __auto_type _ = _t424;
#line 2968 "src/parser.tr"
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t418;
#line 2970 "src/parser.tr"
        /* pass */
    }
#line 2971 "src/parser.tr"
    return box_decl(Decl_ctor_DTopLevelStmt(Parser_parse_stmt(self)));
}

__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self) {
#line 2984 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 2985 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 2986 "src/parser.tr"
    while (({ __auto_type _ord_t425 = (Parser_peek(self)); __auto_type _wr = ((_ord_t425.tag == Token_make_Dot().tag)); _wr; })) {
#line 2987 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2988 "src/parser.tr"
        ({ TrStr _ap_t426 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t427 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t426); _tr_str_append(&path, _ap_t427); _tr_str_release(_ap_t427); });
    }
#line 2989 "src/parser.tr"
    __auto_type _t428 = Parser_peek(self);
    if (_t428.tag == Token_KwImport) {
#line 2991 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t428;
#line 2993 "src/parser.tr"
        /* pass */
    }
#line 2994 "src/parser.tr"
    List_ptr* il = (void*)List_ptr_new();
#line 2995 "src/parser.tr"
    bool multi = false;
#line 2996 "src/parser.tr"
    __auto_type _t429 = Parser_peek(self);
    if ((_t429.tag == Token_LParen || _t429.tag == Token_LBracket)) {
#line 2998 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 2999 "src/parser.tr"
        multi = true;
    } else if (1) {
        __auto_type _ = _t429;
#line 3001 "src/parser.tr"
        /* pass */
    }
#line 3002 "src/parser.tr"
    bool going = true;
#line 3003 "src/parser.tr"
    while (going) {
#line 3006 "src/parser.tr"
        if (multi) {
#line 3007 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        } else {
#line 3009 "src/parser.tr"
            Parser_skip_newlines(self);
        }
#line 3010 "src/parser.tr"
        __auto_type _t430 = Parser_peek(self);
        if ((_t430.tag == Token_RParen || _t430.tag == Token_RBracket || _t430.tag == Token_Eof)) {
#line 3012 "src/parser.tr"
            going = false;
        } else if ((_t430.tag == Token_Newline || _t430.tag == Token_Dedent)) {
#line 3014 "src/parser.tr"
            if ((!multi)) {
#line 3015 "src/parser.tr"
                going = false;
            }
        } else if (1) {
            __auto_type _ = _t430;
#line 3017 "src/parser.tr"
            /* pass */
        }
#line 3018 "src/parser.tr"
        if (going) {
#line 3022 "src/parser.tr"
            if (({ __auto_type _ord_t431 = (Parser_peek(self)); __auto_type _wr = ((_ord_t431.tag == Token_make_Star().tag)); _wr; })) {
#line 3023 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3024 "src/parser.tr"
                List_ptr_append(il, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 3025 "src/parser.tr"
                if (({ __auto_type _ord_t432 = (Parser_peek(self)); __auto_type _wr = ((_ord_t432.tag == Token_make_Comma().tag)); _wr; })) {
#line 3026 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else if ((!multi)) {
#line 3028 "src/parser.tr"
                    going = false;
                }
            } else {
#line 3030 "src/parser.tr"
                TrStr item_name = Parser_consume_ident(self);
#line 3031 "src/parser.tr"
                if ((!_tr_str_eqv((item_name), (_tr_str_lit_len("", 0LL))))) {
#line 3032 "src/parser.tr"
                    ImportItem* item = ImportItem_init(item_name);
#line 3034 "src/parser.tr"
                    if (({ __auto_type _ord_t433 = (Parser_peek(self)); __auto_type _wr = ((_ord_t433.tag == Token_make_KwAs().tag)); _wr; })) {
#line 3035 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 3036 "src/parser.tr"
                        TrStr _sfo_t434 = item->alias;
                        item->alias = Parser_consume_ident(self);
                        _tr_str_release(_sfo_t434);
#line 3037 "src/parser.tr"
                        if ((!_tr_str_eqv((item->alias), (_tr_str_lit_len("", 0LL))))) {
#line 3040 "src/parser.tr"
                            _tr_dict_set_str(self->import_aliases, _tr_strz(item->alias), _tr_str_box(_tr_str_retain(item->name)));
                        }
                    }
#line 3041 "src/parser.tr"
                    List_ptr_append(il, _tr_obj_retain(item));
#line 3042 "src/parser.tr"
                    if (({ __auto_type _ord_t435 = (Parser_peek(self)); __auto_type _wr = ((_ord_t435.tag == Token_make_Comma().tag)); _wr; })) {
#line 3043 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    } else if ((!multi)) {
#line 3045 "src/parser.tr"
                        going = false;
                    }
                } else {
#line 3047 "src/parser.tr"
                    going = false;
                }
            }
        }
    }
#line 3049 "src/parser.tr"
    if (multi) {
#line 3050 "src/parser.tr"
        __auto_type _t436 = Parser_peek(self);
        if ((_t436.tag == Token_RParen || _t436.tag == Token_RBracket)) {
#line 3052 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t436;
#line 3054 "src/parser.tr"
            /* pass */
        }
    }
#line 3055 "src/parser.tr"
    Parser_expect_newline(self);
#line 3056 "src/parser.tr"
    Decl* _trret73 = box_decl(Decl_ctor_DFromImport(path, il));
    _tr_str_release(path);
    return _trret73;
}

__attribute__((hot)) Decl* Parser_parse_import(Parser* self) {
#line 3059 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3060 "src/parser.tr"
    TrStr path = Parser_consume_module_ident(self);
#line 3061 "src/parser.tr"
    while (({ __auto_type _ord_t437 = (Parser_peek(self)); __auto_type _wr = ((_ord_t437.tag == Token_make_Dot().tag)); _wr; })) {
#line 3062 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3065 "src/parser.tr"
        if (({ __auto_type _ord_t438 = (Parser_peek(self)); __auto_type _wr = ((_ord_t438.tag == Token_make_Star().tag)); _wr; })) {
#line 3066 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3067 "src/parser.tr"
            Parser_expect_newline(self);
#line 3068 "src/parser.tr"
            List_ptr* ils = (void*)List_ptr_new();
#line 3069 "src/parser.tr"
            List_ptr_append(ils, ImportItem_init(_tr_str_lit_len("*", 1LL)));
#line 3070 "src/parser.tr"
            Decl* _trret74 = box_decl(Decl_ctor_DFromImport(path, ils));
            _tr_str_release(path);
            return _trret74;
        }
#line 3071 "src/parser.tr"
        ({ TrStr _ap_t439 = (_tr_str_lit_len(".", 1LL)); TrStr _ap_t440 = (Parser_consume_module_ident(self)); _tr_str_append(&path, _ap_t439); _tr_str_append(&path, _ap_t440); _tr_str_release(_ap_t440); });
    }
#line 3072 "src/parser.tr"
    TrStr alias = _tr_str_lit_len("", 0LL);
#line 3073 "src/parser.tr"
    if (({ __auto_type _ord_t441 = (Parser_peek(self)); __auto_type _wr = ((_ord_t441.tag == Token_make_KwAs().tag)); _wr; })) {
#line 3074 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3075 "src/parser.tr"
        TrStr _strtmp_t442 = Parser_consume_ident(self);
        _tr_str_release(alias);
        alias = _strtmp_t442;
    }
#line 3076 "src/parser.tr"
    Parser_expect_newline(self);
#line 3077 "src/parser.tr"
    Decl* _trret75 = box_decl(Decl_ctor_DImport(path, alias));
    _tr_str_release(path);
    _tr_str_release(alias);
    return _trret75;
}

__attribute__((hot)) void Parser_parse_generic_bound(Parser* self, TrStr gname, List_ptr* constraints) {
#line 3083 "src/parser.tr"
    if (({ __auto_type _ord_t443 = (Parser_peek(self)); __auto_type _wr = ((_ord_t443.tag == Token_make_Colon().tag)); _wr; })) {
#line 3084 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3085 "src/parser.tr"
        GenericConstraint* gc = GenericConstraint_init(gname);
#line 3086 "src/parser.tr"
        ({ __auto_type _ord_t444 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t444); });
#line 3087 "src/parser.tr"
        while (({ __auto_type _ord_t445 = (Parser_peek(self)); __auto_type _wr = ((_ord_t445.tag == Token_make_Plus().tag)); _wr; })) {
#line 3088 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3089 "src/parser.tr"
            ({ __auto_type _ord_t446 = (box_asttype(Parser_parse_type(self))); List_ptr_append(gc->bounds, _ord_t446); });
        }
#line 3090 "src/parser.tr"
        List_ptr_append(constraints, _tr_obj_retain(gc));
    }
}

__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method) {
#line 3093 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3094 "src/parser.tr"
    if ((!is_method)) {
#line 3095 "src/parser.tr"
        __auto_type _t447 = Parser_peek(self);
        if (_t447.tag == Token_Ident) {
            __auto_type _ = _t447.data.Ident.name;
            /* pass */
        } else if (1) {
            __auto_type _ = _t447;
#line 3098 "src/parser.tr"
            Parser_emit_diag(self, _tr_str_lit_len("expected a function name after 'def'", 36LL), _tr_str_lit_len("write 'def name(params) -> Type:' - the name must be an identifier, not a keyword or symbol.", 92LL));
        }
    }
#line 3099 "src/parser.tr"
    TrStr name = ((is_method) ? (({
#line 3099 "src/parser.tr"
__auto_type _dores = (Parser_consume_method_ident(self));
_dores;
})) : (({
#line 3099 "src/parser.tr"
__auto_type _dores = (Parser_consume_ident(self));
_dores;
})));
#line 3100 "src/parser.tr"
    FunctionDef* f = FunctionDef_init(name);
#line 3101 "src/parser.tr"
    if (({ __auto_type _ord_t448 = (Parser_peek(self)); __auto_type _wr = ((_ord_t448.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3102 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3103 "src/parser.tr"
        while ((((({ __auto_type _ord_t449 = (Parser_peek(self)); __auto_type _wr = ((_ord_t449.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t450 = (Parser_peek(self)); __auto_type _wr = ((_ord_t450.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t451 = (Parser_peek(self)); __auto_type _wr = ((_ord_t451.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t452 = (Parser_peek(self)); __auto_type _wr = ((_ord_t452.tag != Token_make_LParen().tag)); _wr; }))) {
#line 3104 "src/parser.tr"
            TrStr _gpn = Parser_consume_ident(self);
#line 3105 "src/parser.tr"
            if (_tr_str_eqv((_gpn), (_tr_str_lit_len("", 0LL)))) {
#line 3106 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'def f[T](...)'.", 34LL));
#line 3107 "src/parser.tr"
                _tr_str_release(_gpn);
                break;
            }
#line 3108 "src/parser.tr"
            List_TrStr_append(f->generics, _gpn);
#line 3109 "src/parser.tr"
            Parser_parse_generic_bound(self, _gpn, f->constraints);
#line 3110 "src/parser.tr"
            if (({ __auto_type _ord_t453 = (Parser_peek(self)); __auto_type _wr = ((_ord_t453.tag == Token_make_Comma().tag)); _wr; })) {
#line 3111 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_gpn);
        }
#line 3112 "src/parser.tr"
        if (({ __auto_type _ord_t454 = (Parser_peek(self)); __auto_type _wr = ((_ord_t454.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3113 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3115 "src/parser.tr"
            ({ TrStr _at_t455 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of '", 47LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t455, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t455); });
        }
    }
#line 3116 "src/parser.tr"
    if (({ __auto_type _ord_t456 = (Parser_peek(self)); __auto_type _wr = ((_ord_t456.tag == Token_make_LParen().tag)); _wr; })) {
#line 3117 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3118 "src/parser.tr"
        f->params = Parser_parse_param_list(self);
#line 3119 "src/parser.tr"
        Parser_close_params(self);
    }
#line 3120 "src/parser.tr"
    __auto_type _t457 = Parser_peek(self);
    if (_t457.tag == Token_KwThrows) {
#line 3122 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3123 "src/parser.tr"
        f->throws_ty = box_asttype(Parser_parse_type(self));
    } else if (1) {
        __auto_type _ = _t457;
#line 3125 "src/parser.tr"
        /* pass */
    }
#line 3126 "src/parser.tr"
    if (({ __auto_type _ord_t458 = (Parser_peek(self)); __auto_type _wr = ((_ord_t458.tag == Token_make_Arrow().tag)); _wr; })) {
#line 3127 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3128 "src/parser.tr"
        AstType* ret_t = Parser_parse_type(self);
#line 3129 "src/parser.tr"
        if (({ __auto_type _ord_t459 = (Parser_peek(self)); __auto_type _wr = ((_ord_t459.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3133 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3134 "src/parser.tr"
            ret_t->from_param = Parser_consume_ident(self);
#line 3135 "src/parser.tr"
            ({ TrStr _apt_t460 = _tr_str_retain(ret_t->from_param); List_TrStr_append(ret_t->from_regions, _apt_t460); _tr_str_release(_apt_t460); });
#line 3136 "src/parser.tr"
            while (({ __auto_type _ord_t461 = (Parser_peek(self)); __auto_type _wr = ((_ord_t461.tag == Token_make_Comma().tag)); _wr; })) {
#line 3137 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3138 "src/parser.tr"
                TrStr rgn2 = Parser_consume_ident(self);
#line 3139 "src/parser.tr"
                ret_t->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((ret_t->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3140 "src/parser.tr"
                List_TrStr_append(ret_t->from_regions, rgn2);
                _tr_str_release(rgn2);
            }
        }
#line 3141 "src/parser.tr"
        f->ret_ty = box_asttype(ret_t);
    }
#line 3149 "src/parser.tr"
    bool w_is = false;
#line 3150 "src/parser.tr"
    __auto_type _t462 = Parser_peek(self);
    if (_t462.tag == Token_Ident) {
        __auto_type wkw0 = _t462.data.Ident.name;
#line 3152 "src/parser.tr"
        if (_tr_str_eqv((wkw0), (_tr_str_lit_len("where", 5LL)))) {
#line 3152 "src/parser.tr"
            w_is = true;
        }
    } else if (1) {
        __auto_type _ = _t462;
        /* pass */
    }
#line 3154 "src/parser.tr"
    if (((!w_is) && ({ __auto_type _ord_t463 = (Parser_peek(self)); __auto_type _wr = ((_ord_t463.tag == Token_make_Newline().tag)); _wr; }))) {
#line 3158 "src/parser.tr"
        long long scan = self->pos;
#line 3159 "src/parser.tr"
        bool scanning = true;
#line 3160 "src/parser.tr"
        while (scanning) {
#line 3161 "src/parser.tr"
            __auto_type _t464 = List_Token_get(self->tokens, scan);
            if ((_t464.tag == Token_Newline || _t464.tag == Token_Indent)) {
#line 3163 "src/parser.tr"
                scan = (scan + 1LL);
            } else if (1) {
                __auto_type _ = _t464;
#line 3165 "src/parser.tr"
                scanning = false;
            }
        }
#line 3166 "src/parser.tr"
        __auto_type _t465 = List_Token_get(self->tokens, scan);
        if (_t465.tag == Token_Ident) {
            __auto_type wkw1 = _t465.data.Ident.name;
#line 3168 "src/parser.tr"
            if (_tr_str_eqv((wkw1), (_tr_str_lit_len("where", 5LL)))) {
#line 3169 "src/parser.tr"
                w_is = true;
#line 3170 "src/parser.tr"
                self->pos = scan;
            }
        } else if (1) {
            __auto_type _ = _t465;
            /* pass */
        }
    }
#line 3172 "src/parser.tr"
    if (w_is) {
#line 3173 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3174 "src/parser.tr"
        bool w_paren = false;
#line 3175 "src/parser.tr"
        if (({ __auto_type _ord_t466 = (Parser_peek(self)); __auto_type _wr = ((_ord_t466.tag == Token_make_LParen().tag)); _wr; })) {
#line 3176 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3177 "src/parser.tr"
            w_paren = true;
#line 3178 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
        }
#line 3179 "src/parser.tr"
        bool w_more = true;
#line 3180 "src/parser.tr"
        while (w_more) {
#line 3181 "src/parser.tr"
            if (w_paren) {
#line 3181 "src/parser.tr"
                Parser_skip_newlines_and_indent(self);
            }
#line 3182 "src/parser.tr"
            if ((w_paren && ({ __auto_type _ord_t467 = (Parser_peek(self)); __auto_type _wr = ((_ord_t467.tag == Token_make_RParen().tag)); _wr; }))) {
#line 3183 "src/parser.tr"
                w_more = false;
            } else {
#line 3185 "src/parser.tr"
                TrStr w_a = Parser_consume_ident(self);
#line 3186 "src/parser.tr"
                TrStr w_kw = Parser_consume_ident(self);
#line 3187 "src/parser.tr"
                TrStr w_b = Parser_consume_ident(self);
#line 3188 "src/parser.tr"
                List_TrStr_append(f->outlives_a, w_a);
#line 3189 "src/parser.tr"
                List_TrStr_append(f->outlives_b, w_b);
#line 3190 "src/parser.tr"
                if (w_paren) {
#line 3190 "src/parser.tr"
                    Parser_skip_newlines_and_indent(self);
                }
#line 3191 "src/parser.tr"
                if ((w_paren && ({ __auto_type _ord_t468 = (Parser_peek(self)); __auto_type _wr = ((_ord_t468.tag == Token_make_Comma().tag)); _wr; }))) {
#line 3192 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                } else {
#line 3194 "src/parser.tr"
                    w_more = false;
                }
            }
        }
#line 3195 "src/parser.tr"
        if (w_paren) {
#line 3196 "src/parser.tr"
            Parser_skip_newlines_and_indent(self);
#line 3197 "src/parser.tr"
            if (({ __auto_type _ord_t469 = (Parser_peek(self)); __auto_type _wr = ((_ord_t469.tag == Token_make_RParen().tag)); _wr; })) {
#line 3198 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3199 "src/parser.tr"
    if (({ __auto_type _ord_t470 = (Parser_peek(self)); __auto_type _wr = ((_ord_t470.tag == Token_make_Colon().tag)); _wr; })) {
#line 3200 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3201 "src/parser.tr"
        f->body = Parser_parse_block(self);
    } else {
#line 3203 "src/parser.tr"
        if (((({ __auto_type _ord_t471 = (Parser_peek(self)); __auto_type _wr = ((_ord_t471.tag == Token_make_Newline().tag)); _wr; }) && ((self->pos + 1LL) < self->tokens->len)) && ({ __auto_type _ord_t472 = (List_Token_get(self->tokens, (self->pos + 1LL))); __auto_type _wr = ((_ord_t472.tag == Token_make_Indent().tag)); _wr; }))) {
#line 3204 "src/parser.tr"
            ({ TrStr _at_t473 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("add ':' after the parameter list / return type, e.g. `def ", 58LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) -> int:`.", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag_prev(self, _tr_str_lit_len("missing ':' at the end of this function header", 46LL), _at_t473); _tr_str_release(_at_t473); });
        }
#line 3205 "src/parser.tr"
        Parser_expect_newline(self);
#line 3206 "src/parser.tr"
        f->body = Block_init();
    }
#line 3207 "src/parser.tr"
    _tr_str_release(name);
    return f;
}

__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self) {
#line 3210 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3211 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3212 "src/parser.tr"
    if (_tr_str_eqv((name), (_tr_str_lit_len("", 0LL)))) {
#line 3212 "src/parser.tr"
        Parser_emit_diag(self, _tr_str_lit_len("expected a class name after 'class'", 35LL), _tr_str_lit_len("write 'class Name:'.", 20LL));
    }
#line 3213 "src/parser.tr"
    ClassDef* c = ClassDef_init(name);
#line 3214 "src/parser.tr"
    if (({ __auto_type _ord_t474 = (Parser_peek(self)); __auto_type _wr = ((_ord_t474.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3215 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3216 "src/parser.tr"
        while ((((({ __auto_type _ord_t475 = (Parser_peek(self)); __auto_type _wr = ((_ord_t475.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t476 = (Parser_peek(self)); __auto_type _wr = ((_ord_t476.tag != Token_make_Eof().tag)); _wr; })) && ({ __auto_type _ord_t477 = (Parser_peek(self)); __auto_type _wr = ((_ord_t477.tag != Token_make_Newline().tag)); _wr; })) && ({ __auto_type _ord_t478 = (Parser_peek(self)); __auto_type _wr = ((_ord_t478.tag != Token_make_Colon().tag)); _wr; }))) {
#line 3217 "src/parser.tr"
            TrStr _cgpn = Parser_consume_ident(self);
#line 3218 "src/parser.tr"
            if (_tr_str_eqv((_cgpn), (_tr_str_lit_len("", 0LL)))) {
#line 3219 "src/parser.tr"
                Parser_emit_diag(self, _tr_str_lit_len("expected a generic parameter name", 33LL), _tr_str_lit_len("write generics as 'class Box[T]:'.", 34LL));
#line 3220 "src/parser.tr"
                _tr_str_release(_cgpn);
                break;
            }
#line 3221 "src/parser.tr"
            List_TrStr_append(c->generics, _cgpn);
#line 3222 "src/parser.tr"
            Parser_parse_generic_bound(self, _cgpn, c->constraints);
#line 3223 "src/parser.tr"
            if (({ __auto_type _ord_t479 = (Parser_peek(self)); __auto_type _wr = ((_ord_t479.tag == Token_make_Comma().tag)); _wr; })) {
#line 3224 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_cgpn);
        }
#line 3225 "src/parser.tr"
        if (({ __auto_type _ord_t480 = (Parser_peek(self)); __auto_type _wr = ((_ord_t480.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3226 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else {
#line 3228 "src/parser.tr"
            ({ TrStr _at_t481 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unclosed '[' in the generic parameter list of class '", 53LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t481, _tr_str_lit_len("add the matching ']'.", 21LL)); _tr_str_release(_at_t481); });
        }
    }
#line 3232 "src/parser.tr"
    if (({ __auto_type _ord_t482 = (Parser_peek(self)); __auto_type _wr = ((_ord_t482.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3233 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3234 "src/parser.tr"
        ({ TrStr _ord_t483 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t483); _tr_str_release(_ord_t483); });
#line 3235 "src/parser.tr"
        while (({ __auto_type _ord_t484 = (Parser_peek(self)); __auto_type _wr = ((_ord_t484.tag == Token_make_Comma().tag)); _wr; })) {
#line 3236 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3237 "src/parser.tr"
            ({ TrStr _ord_t485 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _ord_t485); _tr_str_release(_ord_t485); });
        }
    }
#line 3239 "src/parser.tr"
    if (({ __auto_type _ord_t486 = (Parser_peek(self)); __auto_type _wr = ((_ord_t486.tag == Token_make_LParen().tag)); _wr; })) {
#line 3240 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3241 "src/parser.tr"
        ({ TrStr _ord_t487 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t487); _tr_str_release(_ord_t487); });
#line 3242 "src/parser.tr"
        while (({ __auto_type _ord_t488 = (Parser_peek(self)); __auto_type _wr = ((_ord_t488.tag == Token_make_Comma().tag)); _wr; })) {
#line 3243 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3244 "src/parser.tr"
            ({ TrStr _ord_t489 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t489); _tr_str_release(_ord_t489); });
        }
#line 3245 "src/parser.tr"
        if (({ __auto_type _ord_t490 = (Parser_peek(self)); __auto_type _wr = ((_ord_t490.tag == Token_make_RParen().tag)); _wr; })) {
#line 3246 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3247 "src/parser.tr"
    if (({ __auto_type _ord_t491 = (Parser_peek(self)); __auto_type _wr = ((_ord_t491.tag == Token_make_KwExtends().tag)); _wr; })) {
#line 3248 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3249 "src/parser.tr"
        ({ TrStr _ord_t492 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t492); _tr_str_release(_ord_t492); });
#line 3250 "src/parser.tr"
        while (({ __auto_type _ord_t493 = (Parser_peek(self)); __auto_type _wr = ((_ord_t493.tag == Token_make_Comma().tag)); _wr; })) {
#line 3251 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3252 "src/parser.tr"
            ({ TrStr _ord_t494 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _ord_t494); _tr_str_release(_ord_t494); });
        }
    }
#line 3253 "src/parser.tr"
    if (({ __auto_type _ord_t495 = (Parser_peek(self)); __auto_type _wr = ((_ord_t495.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3254 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3255 "src/parser.tr"
        while (true) {
#line 3256 "src/parser.tr"
            TrStr _im_name = Parser_consume_ident(self);
#line 3257 "src/parser.tr"
            List_TrStr_append(c->iface_names, _im_name);
#line 3258 "src/parser.tr"
            AstType* _im_ty = AstType_init(_im_name);
#line 3259 "src/parser.tr"
            if (({ __auto_type _ord_t496 = (Parser_peek(self)); __auto_type _wr = ((_ord_t496.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3260 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3261 "src/parser.tr"
                while ((({ __auto_type _ord_t497 = (Parser_peek(self)); __auto_type _wr = ((_ord_t497.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t498 = (Parser_peek(self)); __auto_type _wr = ((_ord_t498.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3262 "src/parser.tr"
                    ({ __auto_type _ord_t499 = (box_asttype(Parser_parse_type(self))); List_ptr_append(_im_ty->args, _ord_t499); });
#line 3263 "src/parser.tr"
                    if (({ __auto_type _ord_t500 = (Parser_peek(self)); __auto_type _wr = ((_ord_t500.tag == Token_make_Comma().tag)); _wr; })) {
#line 3263 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
                    }
                }
#line 3264 "src/parser.tr"
                if (({ __auto_type _ord_t501 = (Parser_peek(self)); __auto_type _wr = ((_ord_t501.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3264 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
                }
            }
#line 3265 "src/parser.tr"
            ({ __auto_type _ord_t502 = (box_asttype(_im_ty)); List_ptr_append(c->iface_targs, _ord_t502); });
#line 3266 "src/parser.tr"
            if (({ __auto_type _ord_t503 = (Parser_peek(self)); __auto_type _wr = ((_ord_t503.tag == Token_make_Comma().tag)); _wr; })) {
#line 3267 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3268 "src/parser.tr"
                _tr_str_release(_im_name);
                break;
            }
            _tr_str_release(_im_name);
        }
    }
#line 3273 "src/parser.tr"
    bool inline_body = false;
#line 3274 "src/parser.tr"
    if (({ __auto_type _ord_t504 = (Parser_peek(self)); __auto_type _wr = ((_ord_t504.tag == Token_make_Colon().tag)); _wr; })) {
#line 3275 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3276 "src/parser.tr"
        __auto_type _t505 = Parser_peek(self);
        if ((_t505.tag == Token_Newline || _t505.tag == Token_Eof)) {
#line 3278 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t505;
#line 3280 "src/parser.tr"
            inline_body = true;
        }
    } else {
#line 3282 "src/parser.tr"
        ({ TrStr _at_t506 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("expected ':' after the header of class '", 40LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t506, _tr_str_lit_len("a class header is 'class Name[T]:' (optionally followed by '(Base)'), with the members indented on the following lines.", 119LL)); _tr_str_release(_at_t506); });
#line 3283 "src/parser.tr"
        while (true) {
#line 3284 "src/parser.tr"
            __auto_type _t507 = Parser_peek(self);
            if ((_t507.tag == Token_Newline || _t507.tag == Token_Eof || _t507.tag == Token_Colon)) {
                break;
            } else if (1) {
                __auto_type _ = _t507;
                self->pos = (self->pos + 1LL);
            }
        }
    }
#line 3287 "src/parser.tr"
    if (inline_body) {
#line 3288 "src/parser.tr"
        bool inl = true;
#line 3289 "src/parser.tr"
        while (inl) {
#line 3290 "src/parser.tr"
            bool in_pub = false;
#line 3291 "src/parser.tr"
            __auto_type _t508 = Parser_peek(self);
            if (_t508.tag == Token_KwPub) {
#line 3293 "src/parser.tr"
                in_pub = true;
#line 3294 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t508;
#line 3296 "src/parser.tr"
                /* pass */
            }
#line 3297 "src/parser.tr"
            __auto_type _t509 = Parser_peek(self);
            if (_t509.tag == Token_KwPass) {
#line 3299 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t509.tag == Token_KwDef) {
#line 3301 "src/parser.tr"
                long long _iml = Parser_cur_line(self);
#line 3302 "src/parser.tr"
                FunctionDef* im = Parser_parse_function_def(self, true);
#line 3303 "src/parser.tr"
                im->line = _iml;
#line 3304 "src/parser.tr"
                im->is_public = in_pub;
#line 3305 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(im));
                _tr_obj_release(im, _trdrop_FunctionDef);
            } else if (_t509.tag == Token_Ident) {
                __auto_type ifn = _t509.data.Ident.name;
#line 3307 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3308 "src/parser.tr"
                AstType** ity = (AstType**)(0LL);
#line 3309 "src/parser.tr"
                if (({ __auto_type _ord_t510 = (Parser_peek(self)); __auto_type _wr = ((_ord_t510.tag == Token_make_Colon().tag)); _wr; })) {
#line 3310 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3311 "src/parser.tr"
                    ity = box_asttype(Parser_parse_type(self));
                }
#line 3312 "src/parser.tr"
                ({ FieldDef* _ord_t511 = (FieldDef_init(ifn, ity)); List_ptr_append(c->fields, _tr_obj_retain(_ord_t511)); _tr_obj_release(_ord_t511, _trdrop_FieldDef); });
            } else if (1) {
                __auto_type _ = _t509;
#line 3314 "src/parser.tr"
                inl = false;
            }
#line 3315 "src/parser.tr"
            __auto_type _t512 = Parser_peek(self);
            if (_t512.tag == Token_Semicolon) {
#line 3317 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t512;
#line 3319 "src/parser.tr"
                inl = false;
            }
        }
#line 3320 "src/parser.tr"
        if (({ __auto_type _ord_t513 = (Parser_peek(self)); __auto_type _wr = ((_ord_t513.tag == Token_make_Newline().tag)); _wr; })) {
#line 3321 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3322 "src/parser.tr"
        Decl* _trret76 = box_decl(Decl_ctor_DClass(c));
        _tr_str_release(name);
        _tr_obj_release(c, _trdrop_ClassDef);
        return _trret76;
    }
#line 3323 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3324 "src/parser.tr"
    __auto_type _t514 = Parser_peek(self);
    if (_t514.tag == Token_Indent) {
#line 3326 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t514;
#line 3328 "src/parser.tr"
        /* pass */
    }
#line 3329 "src/parser.tr"
    bool going = true;
#line 3330 "src/parser.tr"
    while (going) {
#line 3331 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3334 "src/parser.tr"
        if (_tr_str_eqv((c->docstring), (_tr_str_lit_len("", 0LL)))) {
#line 3335 "src/parser.tr"
            __auto_type _t515 = Parser_peek(self);
            if (_t515.tag == Token_TripleStrLit) {
                __auto_type _ds = _t515.data.TripleStrLit.val;
#line 3337 "src/parser.tr"
                TrStr _sfo_t516 = c->docstring;
                c->docstring = _tr_str_retain(_ds);
                _tr_str_release(_sfo_t516);
#line 3338 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3339 "src/parser.tr"
                Parser_skip_newlines(self);
            } else if (1) {
                __auto_type _ = _t515;
#line 3341 "src/parser.tr"
                /* pass */
            }
        }
#line 3342 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3343 "src/parser.tr"
        while (({ __auto_type _ord_t517 = (Parser_peek(self)); __auto_type _wr = ((_ord_t517.tag == Token_make_At().tag)); _wr; })) {
#line 3344 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3345 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3346 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3347 "src/parser.tr"
            if (({ __auto_type _ord_t518 = (Parser_peek(self)); __auto_type _wr = ((_ord_t518.tag == Token_make_LParen().tag)); _wr; })) {
#line 3348 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3349 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3350 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3351 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3353 "src/parser.tr"
        bool is_p = false;
#line 3354 "src/parser.tr"
        __auto_type _t519 = Parser_peek(self);
        if (_t519.tag == Token_KwPub) {
#line 3356 "src/parser.tr"
            is_p = true;
#line 3357 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t519;
#line 3359 "src/parser.tr"
            /* pass */
        }
#line 3360 "src/parser.tr"
        __auto_type _t520 = Parser_peek(self);
        if ((_t520.tag == Token_Dedent || _t520.tag == Token_Eof)) {
#line 3362 "src/parser.tr"
            going = false;
        } else if (_t520.tag == Token_KwDef) {
#line 3364 "src/parser.tr"
            long long _mline = Parser_cur_line(self);
#line 3365 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3366 "src/parser.tr"
            m->line = _mline;
#line 3367 "src/parser.tr"
            m->is_public = is_p;
#line 3368 "src/parser.tr"
            m->decorators = decorators;
#line 3369 "src/parser.tr"
            List_ptr_append(c->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t520.tag == Token_KwAsync) {
#line 3371 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3372 "src/parser.tr"
            __auto_type _t521 = Parser_peek(self);
            if (_t521.tag == Token_KwDef) {
#line 3374 "src/parser.tr"
                long long _mline = Parser_cur_line(self);
#line 3375 "src/parser.tr"
                FunctionDef* m = Parser_parse_function_def(self, true);
#line 3376 "src/parser.tr"
                m->line = _mline;
#line 3377 "src/parser.tr"
                m->is_async = true;
#line 3378 "src/parser.tr"
                m->is_public = is_p;
#line 3379 "src/parser.tr"
                m->decorators = decorators;
#line 3380 "src/parser.tr"
                List_ptr_append(c->methods, _tr_obj_retain(m));
                _tr_obj_release(m, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t521;
#line 3382 "src/parser.tr"
                /* pass */
            }
        } else if (_t520.tag == Token_Ident) {
            __auto_type fname = _t520.data.Ident.name;
#line 3384 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3385 "src/parser.tr"
            AstType** ty_ptr = (AstType**)(0LL);
#line 3386 "src/parser.tr"
            if (({ __auto_type _ord_t522 = (Parser_peek(self)); __auto_type _wr = ((_ord_t522.tag == Token_make_Colon().tag)); _wr; })) {
#line 3387 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3388 "src/parser.tr"
                AstType* fty = Parser_parse_type(self);
#line 3393 "src/parser.tr"
                if (({ __auto_type _ord_t523 = (Parser_peek(self)); __auto_type _wr = ((_ord_t523.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3394 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3395 "src/parser.tr"
                    fty->from_param = Parser_consume_ident(self);
#line 3396 "src/parser.tr"
                    ({ TrStr _apt_t524 = _tr_str_retain(fty->from_param); List_TrStr_append(fty->from_regions, _apt_t524); _tr_str_release(_apt_t524); });
#line 3397 "src/parser.tr"
                    while (({ __auto_type _ord_t525 = (Parser_peek(self)); __auto_type _wr = ((_ord_t525.tag == Token_make_Comma().tag)); _wr; })) {
#line 3398 "src/parser.tr"
                        self->pos = (self->pos + 1LL);
#line 3399 "src/parser.tr"
                        TrStr frgn2 = Parser_consume_ident(self);
#line 3400 "src/parser.tr"
                        fty->from_param = ({ TrStr _tr_cl = (_tr_strx_concatv((fty->from_param), (_tr_str_lit_len(",", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (frgn2)); _tr_str_release(_tr_cl); _tr_cres; });
#line 3401 "src/parser.tr"
                        List_TrStr_append(fty->from_regions, frgn2);
                        _tr_str_release(frgn2);
                    }
                }
#line 3402 "src/parser.tr"
                ty_ptr = box_asttype(fty);
            }
#line 3403 "src/parser.tr"
            FieldDef* fld = FieldDef_init(fname, ty_ptr);
#line 3404 "src/parser.tr"
            List_ptr_append(c->fields, _tr_obj_retain(fld));
#line 3405 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(fld, _trdrop_FieldDef);
        } else if (_t520.tag == Token_KwPass) {
#line 3407 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3408 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t520;
#line 3410 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3411 "src/parser.tr"
    if (({ __auto_type _ord_t526 = (Parser_peek(self)); __auto_type _wr = ((_ord_t526.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3412 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3413 "src/parser.tr"
    Decl* _trret77 = box_decl(Decl_ctor_DClass(c));
    _tr_str_release(name);
    _tr_obj_release(c, _trdrop_ClassDef);
    return _trret77;
}

__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self) {
#line 3416 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3417 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3418 "src/parser.tr"
    EnumDef* e = EnumDef_init(name);
#line 3422 "src/parser.tr"
    if (({ __auto_type _ord_t527 = (Parser_peek(self)); __auto_type _wr = ((_ord_t527.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3423 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3424 "src/parser.tr"
        ({ TrStr _ord_t528 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t528); _tr_str_release(_ord_t528); });
#line 3425 "src/parser.tr"
        while (({ __auto_type _ord_t529 = (Parser_peek(self)); __auto_type _wr = ((_ord_t529.tag == Token_make_Comma().tag)); _wr; })) {
#line 3426 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3427 "src/parser.tr"
            ({ TrStr _ord_t530 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _ord_t530); _tr_str_release(_ord_t530); });
        }
    }
#line 3428 "src/parser.tr"
    if (({ __auto_type _ord_t531 = (Parser_peek(self)); __auto_type _wr = ((_ord_t531.tag == Token_make_KwImplements().tag)); _wr; })) {
#line 3429 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3430 "src/parser.tr"
        while (true) {
#line 3431 "src/parser.tr"
            ({ TrStr _ord_t532 = (Parser_consume_ident(self)); List_TrStr_append(e->iface_names, _ord_t532); _tr_str_release(_ord_t532); });
#line 3432 "src/parser.tr"
            if (({ __auto_type _ord_t533 = (Parser_peek(self)); __auto_type _wr = ((_ord_t533.tag == Token_make_Comma().tag)); _wr; })) {
#line 3433 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else {
#line 3434 "src/parser.tr"
                break;
            }
        }
    }
#line 3438 "src/parser.tr"
    bool e_inline = false;
#line 3439 "src/parser.tr"
    if (({ __auto_type _ord_t534 = (Parser_peek(self)); __auto_type _wr = ((_ord_t534.tag == Token_make_Colon().tag)); _wr; })) {
#line 3440 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3441 "src/parser.tr"
        __auto_type _t535 = Parser_peek(self);
        if ((_t535.tag == Token_Newline || _t535.tag == Token_Eof)) {
#line 3443 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t535;
#line 3445 "src/parser.tr"
            e_inline = true;
        }
    }
#line 3446 "src/parser.tr"
    if (e_inline) {
#line 3447 "src/parser.tr"
        bool einl = true;
#line 3448 "src/parser.tr"
        while (einl) {
#line 3449 "src/parser.tr"
            __auto_type _t536 = Parser_peek(self);
            if (_t536.tag == Token_KwPass) {
#line 3451 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t536.tag == Token_Ident) {
                __auto_type evn = _t536.data.Ident.name;
#line 3453 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3454 "src/parser.tr"
                VariantDef* evd = VariantDef_init(evn);
#line 3455 "src/parser.tr"
                if (({ __auto_type _ord_t537 = (Parser_peek(self)); __auto_type _wr = ((_ord_t537.tag == Token_make_LParen().tag)); _wr; })) {
#line 3456 "src/parser.tr"
                    self->pos = (self->pos + 1LL);
#line 3457 "src/parser.tr"
                    evd->fields = Parser_parse_param_list(self);
#line 3458 "src/parser.tr"
                    Parser_close_params(self);
                }
#line 3459 "src/parser.tr"
                List_ptr_append(e->variants, _tr_obj_retain(evd));
                _tr_obj_release(evd, _trdrop_VariantDef);
            } else if (1) {
                __auto_type _ = _t536;
#line 3461 "src/parser.tr"
                einl = false;
            }
#line 3462 "src/parser.tr"
            __auto_type _t538 = Parser_peek(self);
            if ((_t538.tag == Token_Comma || _t538.tag == Token_Semicolon)) {
#line 3464 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t538;
#line 3466 "src/parser.tr"
                einl = false;
            }
        }
#line 3467 "src/parser.tr"
        if (({ __auto_type _ord_t539 = (Parser_peek(self)); __auto_type _wr = ((_ord_t539.tag == Token_make_Newline().tag)); _wr; })) {
#line 3468 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3469 "src/parser.tr"
        Decl* _trret78 = box_decl(Decl_ctor_DEnum(e));
        _tr_str_release(name);
        _tr_obj_release(e, _trdrop_EnumDef);
        return _trret78;
    }
#line 3470 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3471 "src/parser.tr"
    __auto_type _t540 = Parser_peek(self);
    if (_t540.tag == Token_Indent) {
#line 3473 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t540;
#line 3475 "src/parser.tr"
        /* pass */
    }
#line 3476 "src/parser.tr"
    bool going = true;
#line 3477 "src/parser.tr"
    while (going) {
#line 3478 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3479 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3480 "src/parser.tr"
        while (({ __auto_type _ord_t541 = (Parser_peek(self)); __auto_type _wr = ((_ord_t541.tag == Token_make_At().tag)); _wr; })) {
#line 3481 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3482 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3483 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3484 "src/parser.tr"
            if (({ __auto_type _ord_t542 = (Parser_peek(self)); __auto_type _wr = ((_ord_t542.tag == Token_make_LParen().tag)); _wr; })) {
#line 3485 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3486 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3487 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3488 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3490 "src/parser.tr"
        bool is_p = false;
#line 3491 "src/parser.tr"
        __auto_type _t543 = Parser_peek(self);
        if (_t543.tag == Token_KwPub) {
#line 3493 "src/parser.tr"
            is_p = true;
#line 3494 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t543;
#line 3496 "src/parser.tr"
            /* pass */
        }
#line 3497 "src/parser.tr"
        __auto_type _t544 = Parser_peek(self);
        if ((_t544.tag == Token_Dedent || _t544.tag == Token_Eof)) {
#line 3499 "src/parser.tr"
            going = false;
        } else if (_t544.tag == Token_Ident) {
            __auto_type vname = _t544.data.Ident.name;
#line 3501 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3502 "src/parser.tr"
            VariantDef* vd = VariantDef_init(vname);
#line 3503 "src/parser.tr"
            if (({ __auto_type _ord_t545 = (Parser_peek(self)); __auto_type _wr = ((_ord_t545.tag == Token_make_LParen().tag)); _wr; })) {
#line 3504 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3505 "src/parser.tr"
                vd->fields = Parser_parse_param_list(self);
#line 3506 "src/parser.tr"
                Parser_close_params(self);
            }
#line 3507 "src/parser.tr"
            List_ptr_append(e->variants, _tr_obj_retain(vd));
#line 3508 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_obj_release(vd, _trdrop_VariantDef);
        } else if (_t544.tag == Token_KwDef) {
#line 3510 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3511 "src/parser.tr"
            m->is_public = is_p;
#line 3512 "src/parser.tr"
            m->decorators = decorators;
#line 3513 "src/parser.tr"
            List_ptr_append(e->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t544.tag == Token_KwPass) {
#line 3515 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3516 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t544;
#line 3518 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3519 "src/parser.tr"
    if (({ __auto_type _ord_t546 = (Parser_peek(self)); __auto_type _wr = ((_ord_t546.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3520 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3521 "src/parser.tr"
    Decl* _trret79 = box_decl(Decl_ctor_DEnum(e));
    _tr_str_release(name);
    _tr_obj_release(e, _trdrop_EnumDef);
    return _trret79;
}

__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self) {
#line 3524 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3525 "src/parser.tr"
    TrStr name = Parser_consume_ident(self);
#line 3526 "src/parser.tr"
    InterfaceDef* i = InterfaceDef_init(name);
#line 3528 "src/parser.tr"
    if (({ __auto_type _ord_t547 = (Parser_peek(self)); __auto_type _wr = ((_ord_t547.tag == Token_make_LBracket().tag)); _wr; })) {
#line 3529 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3530 "src/parser.tr"
        while ((({ __auto_type _ord_t548 = (Parser_peek(self)); __auto_type _wr = ((_ord_t548.tag != Token_make_RBracket().tag)); _wr; }) && ({ __auto_type _ord_t549 = (Parser_peek(self)); __auto_type _wr = ((_ord_t549.tag != Token_make_Eof().tag)); _wr; }))) {
#line 3531 "src/parser.tr"
            TrStr gname = Parser_consume_ident(self);
#line 3532 "src/parser.tr"
            if ((!_tr_str_eqv((gname), (_tr_str_lit_len("", 0LL))))) {
#line 3532 "src/parser.tr"
                List_TrStr_append(i->generics, gname);
            }
#line 3533 "src/parser.tr"
            if (({ __auto_type _ord_t550 = (Parser_peek(self)); __auto_type _wr = ((_ord_t550.tag == Token_make_Comma().tag)); _wr; })) {
#line 3533 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(gname);
        }
#line 3534 "src/parser.tr"
        if (({ __auto_type _ord_t551 = (Parser_peek(self)); __auto_type _wr = ((_ord_t551.tag == Token_make_RBracket().tag)); _wr; })) {
#line 3534 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3537 "src/parser.tr"
    if (({ __auto_type _ord_t552 = (Parser_peek(self)); __auto_type _wr = ((_ord_t552.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3538 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3539 "src/parser.tr"
        ({ TrStr _ord_t553 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t553); _tr_str_release(_ord_t553); });
#line 3540 "src/parser.tr"
        while (({ __auto_type _ord_t554 = (Parser_peek(self)); __auto_type _wr = ((_ord_t554.tag == Token_make_Comma().tag)); _wr; })) {
#line 3541 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3542 "src/parser.tr"
            ({ TrStr _ord_t555 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _ord_t555); _tr_str_release(_ord_t555); });
        }
    }
#line 3545 "src/parser.tr"
    bool i_inline = false;
#line 3546 "src/parser.tr"
    if (({ __auto_type _ord_t556 = (Parser_peek(self)); __auto_type _wr = ((_ord_t556.tag == Token_make_Colon().tag)); _wr; })) {
#line 3547 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3548 "src/parser.tr"
        __auto_type _t557 = Parser_peek(self);
        if ((_t557.tag == Token_Newline || _t557.tag == Token_Eof)) {
#line 3550 "src/parser.tr"
            /* pass */
        } else if (1) {
            __auto_type _ = _t557;
#line 3552 "src/parser.tr"
            i_inline = true;
        }
    }
#line 3553 "src/parser.tr"
    if (i_inline) {
#line 3554 "src/parser.tr"
        bool iinl = true;
#line 3555 "src/parser.tr"
        while (iinl) {
#line 3556 "src/parser.tr"
            bool i_pub = false;
#line 3557 "src/parser.tr"
            __auto_type _t558 = Parser_peek(self);
            if (_t558.tag == Token_KwPub) {
#line 3559 "src/parser.tr"
                i_pub = true;
#line 3560 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t558;
#line 3562 "src/parser.tr"
                /* pass */
            }
#line 3563 "src/parser.tr"
            __auto_type _t559 = Parser_peek(self);
            if (_t559.tag == Token_KwPass) {
#line 3565 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (_t559.tag == Token_KwDef) {
#line 3567 "src/parser.tr"
                FunctionDef* im2 = Parser_parse_function_def(self, true);
#line 3568 "src/parser.tr"
                im2->is_public = i_pub;
#line 3569 "src/parser.tr"
                List_ptr_append(i->methods, _tr_obj_retain(im2));
                _tr_obj_release(im2, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t559;
#line 3571 "src/parser.tr"
                iinl = false;
            }
#line 3572 "src/parser.tr"
            __auto_type _t560 = Parser_peek(self);
            if (_t560.tag == Token_Semicolon) {
#line 3574 "src/parser.tr"
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t560;
#line 3576 "src/parser.tr"
                iinl = false;
            }
        }
#line 3577 "src/parser.tr"
        if (({ __auto_type _ord_t561 = (Parser_peek(self)); __auto_type _wr = ((_ord_t561.tag == Token_make_Newline().tag)); _wr; })) {
#line 3578 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
#line 3579 "src/parser.tr"
        Decl* _trret80 = box_decl(Decl_ctor_DInterface(i));
        _tr_str_release(name);
        _tr_obj_release(i, _trdrop_InterfaceDef);
        return _trret80;
    }
#line 3580 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3581 "src/parser.tr"
    __auto_type _t562 = Parser_peek(self);
    if (_t562.tag == Token_Indent) {
#line 3583 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t562;
#line 3585 "src/parser.tr"
        /* pass */
    }
#line 3586 "src/parser.tr"
    bool going = true;
#line 3587 "src/parser.tr"
    while (going) {
#line 3588 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3589 "src/parser.tr"
        bool is_p = false;
#line 3590 "src/parser.tr"
        __auto_type _t563 = Parser_peek(self);
        if (_t563.tag == Token_KwPub) {
#line 3592 "src/parser.tr"
            is_p = true;
#line 3593 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t563;
#line 3595 "src/parser.tr"
            /* pass */
        }
#line 3596 "src/parser.tr"
        __auto_type _t564 = Parser_peek(self);
        if ((_t564.tag == Token_Dedent || _t564.tag == Token_Eof)) {
#line 3598 "src/parser.tr"
            going = false;
        } else if (_t564.tag == Token_KwDef) {
#line 3600 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3601 "src/parser.tr"
            m->is_public = is_p;
#line 3602 "src/parser.tr"
            List_ptr_append(i->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t564.tag == Token_KwPass) {
#line 3604 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3605 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t564;
#line 3607 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3608 "src/parser.tr"
    if (({ __auto_type _ord_t565 = (Parser_peek(self)); __auto_type _wr = ((_ord_t565.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3609 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3610 "src/parser.tr"
    Decl* _trret81 = box_decl(Decl_ctor_DInterface(i));
    _tr_str_release(name);
    _tr_obj_release(i, _trdrop_InterfaceDef);
    return _trret81;
}

__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self) {
#line 3613 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3614 "src/parser.tr"
    TrStr target = Parser_consume_ident(self);
#line 3619 "src/parser.tr"
    if (({ __auto_type _ord_t566 = (Parser_peek(self)); __auto_type _wr = ((_ord_t566.tag == Token_make_KwFrom().tag)); _wr; })) {
#line 3620 "src/parser.tr"
        self->pos = (self->pos + 1LL);
#line 3621 "src/parser.tr"
        TrStr _erg = Parser_consume_ident(self);
#line 3622 "src/parser.tr"
        while (({ __auto_type _ord_t567 = (Parser_peek(self)); __auto_type _wr = ((_ord_t567.tag == Token_make_Comma().tag)); _wr; })) {
#line 3623 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3624 "src/parser.tr"
            TrStr _strtmp_t568 = Parser_consume_ident(self);
            _tr_str_release(_erg);
            _erg = _strtmp_t568;
        }
    }
#line 3625 "src/parser.tr"
    if (({ __auto_type _ord_t569 = (Parser_peek(self)); __auto_type _wr = ((_ord_t569.tag == Token_make_Colon().tag)); _wr; })) {
#line 3626 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3627 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3628 "src/parser.tr"
    __auto_type _t570 = Parser_peek(self);
    if (_t570.tag == Token_Indent) {
#line 3630 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t570;
#line 3632 "src/parser.tr"
        /* pass */
    }
#line 3633 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3634 "src/parser.tr"
    bool going = true;
#line 3635 "src/parser.tr"
    while (going) {
#line 3636 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3637 "src/parser.tr"
        List_ptr* decorators = (void*)List_ptr_new();
#line 3638 "src/parser.tr"
        while (({ __auto_type _ord_t571 = (Parser_peek(self)); __auto_type _wr = ((_ord_t571.tag == Token_make_At().tag)); _wr; })) {
#line 3639 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3640 "src/parser.tr"
            TrStr dname = Parser_consume_ident(self);
#line 3641 "src/parser.tr"
            Decorator* d = Decorator_init(dname);
#line 3642 "src/parser.tr"
            if (({ __auto_type _ord_t572 = (Parser_peek(self)); __auto_type _wr = ((_ord_t572.tag == Token_make_LParen().tag)); _wr; })) {
#line 3643 "src/parser.tr"
                self->pos = (self->pos + 1LL);
#line 3644 "src/parser.tr"
                d->args = Parser_parse_arg_list(self);
            }
#line 3645 "src/parser.tr"
            List_ptr_append(decorators, _tr_obj_retain(d));
#line 3646 "src/parser.tr"
            Parser_expect_newline(self);
            _tr_str_release(dname);
            _tr_obj_release(d, _trdrop_Decorator);
        }
#line 3648 "src/parser.tr"
        bool is_p = false;
#line 3649 "src/parser.tr"
        __auto_type _t573 = Parser_peek(self);
        if (_t573.tag == Token_KwPub) {
#line 3651 "src/parser.tr"
            is_p = true;
#line 3652 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t573;
#line 3654 "src/parser.tr"
            /* pass */
        }
#line 3655 "src/parser.tr"
        __auto_type _t574 = Parser_peek(self);
        if ((_t574.tag == Token_Dedent || _t574.tag == Token_Eof)) {
#line 3657 "src/parser.tr"
            going = false;
        } else if (_t574.tag == Token_KwDef) {
#line 3659 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, true);
#line 3660 "src/parser.tr"
            m->is_public = is_p;
#line 3661 "src/parser.tr"
            m->decorators = decorators;
#line 3662 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t574.tag == Token_KwAsync) {
#line 3675 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3676 "src/parser.tr"
            __auto_type _t575 = Parser_peek(self);
            if (_t575.tag == Token_KwDef) {
#line 3678 "src/parser.tr"
                FunctionDef* am = Parser_parse_function_def(self, true);
#line 3679 "src/parser.tr"
                am->is_async = true;
#line 3680 "src/parser.tr"
                am->is_public = is_p;
#line 3681 "src/parser.tr"
                am->decorators = decorators;
#line 3682 "src/parser.tr"
                List_ptr_append(fl, _tr_obj_retain(am));
                _tr_obj_release(am, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t575;
#line 3684 "src/parser.tr"
                /* pass */
            }
        } else if (_t574.tag == Token_KwPass) {
#line 3686 "src/parser.tr"
            self->pos = (self->pos + 1LL);
#line 3687 "src/parser.tr"
            Parser_expect_newline(self);
        } else if (_t574.tag == Token_Ident) {
            __auto_type _xn = _t574.data.Ident.name;
#line 3689 "src/parser.tr"
            if (is_p) {
#line 3690 "src/parser.tr"
                ({ TrStr _at_t576 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("expected 'def' to declare a method of '", 39LL)), (target))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', found '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_xn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t577 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("write 'pub def ", 15LL)), (_xn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(self, ...)'; an extend block holds only methods.", 49LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t576, _at_t577); _tr_str_release(_at_t576); _tr_str_release(_at_t577); });
            }
#line 3691 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t574;
#line 3693 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3694 "src/parser.tr"
    if (({ __auto_type _ord_t578 = (Parser_peek(self)); __auto_type _wr = ((_ord_t578.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3695 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3696 "src/parser.tr"
    Decl d = Decl_ctor_DExtend(target, fl);
#line 3697 "src/parser.tr"
    Decl* _trret82 = box_decl(d);
    _tr_str_release(target);
    return _trret82;
}

__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self) {
#line 3700 "src/parser.tr"
    self->pos = (self->pos + 1LL);
#line 3701 "src/parser.tr"
    TrStr abi = _tr_str_lit_len("C", 1LL);
#line 3702 "src/parser.tr"
    __auto_type _t579 = Parser_peek(self);
    if (_t579.tag == Token_StrLit) {
        __auto_type s = _t579.data.StrLit.val;
#line 3704 "src/parser.tr"
        TrStr _strtmp_t580 = _tr_str_retain(s);
        _tr_str_release(abi);
        abi = _strtmp_t580;
#line 3705 "src/parser.tr"
        if ((((((!_tr_str_eqv((abi), (_tr_str_lit_len("C", 1LL)))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("C++", 3LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("CPP", 3LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("c", 1LL))))) && (!_tr_str_eqv((abi), (_tr_str_lit_len("cpp", 3LL)))))) {
#line 3706 "src/parser.tr"
            ({ TrStr _at_t581 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unknown extern ABI \"", 20LL)), (abi))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); Parser_emit_diag(self, _at_t581, _tr_str_lit_len("only extern \"C\" (and \"C++\"/\"CPP\", which also bind through C) are supported - expose the function with a C ABI.", 110LL)); _tr_str_release(_at_t581); });
        }
#line 3707 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t579;
#line 3709 "src/parser.tr"
        /* pass */
    }
#line 3710 "src/parser.tr"
    if (({ __auto_type _ord_t582 = (Parser_peek(self)); __auto_type _wr = ((_ord_t582.tag == Token_make_Colon().tag)); _wr; })) {
#line 3711 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3712 "src/parser.tr"
    Parser_skip_newlines(self);
#line 3713 "src/parser.tr"
    __auto_type _t583 = Parser_peek(self);
    if (_t583.tag == Token_Indent) {
#line 3715 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t583;
#line 3717 "src/parser.tr"
        /* pass */
    }
#line 3718 "src/parser.tr"
    List_ptr* fl = (void*)List_ptr_new();
#line 3719 "src/parser.tr"
    bool going = true;
#line 3720 "src/parser.tr"
    while (going) {
#line 3721 "src/parser.tr"
        Parser_skip_newlines(self);
#line 3722 "src/parser.tr"
        bool is_p = false;
#line 3723 "src/parser.tr"
        __auto_type _t584 = Parser_peek(self);
        if (_t584.tag == Token_KwPub) {
#line 3725 "src/parser.tr"
            is_p = true;
#line 3726 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t584;
#line 3728 "src/parser.tr"
            /* pass */
        }
#line 3729 "src/parser.tr"
        __auto_type _t585 = Parser_peek(self);
        if ((_t585.tag == Token_Dedent || _t585.tag == Token_Eof)) {
#line 3731 "src/parser.tr"
            going = false;
        } else if (_t585.tag == Token_KwDef) {
#line 3733 "src/parser.tr"
            FunctionDef* m = Parser_parse_function_def(self, false);
#line 3734 "src/parser.tr"
            m->is_public = is_p;
#line 3738 "src/parser.tr"
            if ((m->params->len > 0LL)) {
#line 3739 "src/parser.tr"
                Param* _ext_last = ((Param*)List_ptr_get(m->params, (m->params->len - 1LL)));
#line 3740 "src/parser.tr"
                if (_ext_last->is_variadic) {
#line 3741 "src/parser.tr"
                    ((Param*)List_ptr_pop(m->params));
#line 3742 "src/parser.tr"
                    m->is_variadic = true;
                }
            }
#line 3743 "src/parser.tr"
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (1) {
            __auto_type _ = _t585;
#line 3745 "src/parser.tr"
            self->pos = (self->pos + 1LL);
        }
    }
#line 3746 "src/parser.tr"
    if (({ __auto_type _ord_t586 = (Parser_peek(self)); __auto_type _wr = ((_ord_t586.tag == Token_make_Dedent().tag)); _wr; })) {
#line 3747 "src/parser.tr"
        self->pos = (self->pos + 1LL);
    }
#line 3748 "src/parser.tr"
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
    ({ __auto_type _ord_t587 = (box_asttype(ret)); List_ptr_append(rt->args, _ord_t587); });
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
        c = ((long long)((*_TR_PTR_OFF(p, i))));
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
        TrStr _strtmp_t588 = _tr_strx_concatv((_tr_str_lit_len("...", 3LL)), (out));
        _tr_str_release(out);
        out = _strtmp_t588;
    }
#line 82 "src/parser.tr"
    if ((en < _tr_str_lenv((line)))) {
#line 82 "src/parser.tr"
        ({ TrStr _ap_t589 = (_tr_str_lit_len("...", 3LL)); _tr_str_append(&out, _ap_t589); });
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
#line 3755 "src/parser.tr"
    long long nd = 2LL;
#line 3756 "src/parser.tr"
    if ((esc == 117LL)) {
#line 3756 "src/parser.tr"
        nd = 4LL;
    }
#line 3757 "src/parser.tr"
    if ((esc == 85LL)) {
#line 3757 "src/parser.tr"
        nd = 8LL;
    }
#line 3758 "src/parser.tr"
    if (((at + nd) > n)) {
#line 3758 "src/parser.tr"
        return false;
    }
#line 3759 "src/parser.tr"
    long long v = 0LL;
#line 3760 "src/parser.tr"
    long long k = 0LL;
#line 3761 "src/parser.tr"
    while ((k < nd)) {
#line 3762 "src/parser.tr"
        long long h = lex_hex_val(((long long)((*_TR_PTR_OFF(p, (at + k))))));
#line 3763 "src/parser.tr"
        if ((h < 0LL)) {
#line 3763 "src/parser.tr"
            return false;
        }
#line 3764 "src/parser.tr"
        v = ((v * 16LL) + h);
#line 3765 "src/parser.tr"
        k = (k + 1LL);
    }
#line 3766 "src/parser.tr"
    if (((esc != 120LL) && (v > 1114111LL))) {
#line 3766 "src/parser.tr"
        return false;
    }
#line 3767 "src/parser.tr"
    return true;
}

__attribute__((hot)) long long _find_fmt_colon(TrStr s) {
#line 3770 "src/parser.tr"
    long long i = 0LL;
#line 3771 "src/parser.tr"
    long long n = _tr_str_lenv((s));
#line 3772 "src/parser.tr"
    long long depth = 0LL;
#line 3773 "src/parser.tr"
    while ((i < n)) {
#line 3774 "src/parser.tr"
        long long c = _tr_str_char_at_codev((s), i);
#line 3775 "src/parser.tr"
        if (((c == 34LL) || (c == 39LL))) {
#line 3777 "src/parser.tr"
            long long qc = c;
#line 3778 "src/parser.tr"
            i = (i + 1LL);
#line 3779 "src/parser.tr"
            while (((i < n) && (_tr_str_char_at_codev((s), i) != qc))) {
#line 3780 "src/parser.tr"
                if ((_tr_str_char_at_codev((s), i) == 92LL)) {
#line 3780 "src/parser.tr"
                    i = (i + 1LL);
                }
#line 3781 "src/parser.tr"
                i = (i + 1LL);
            }
        } else if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
#line 3782 "src/parser.tr"
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
#line 3783 "src/parser.tr"
            depth = (depth - 1LL);
        } else if (((c == 58LL) && (depth == 0LL))) {
#line 3784 "src/parser.tr"
            return i;
        }
#line 3785 "src/parser.tr"
        i = (i + 1LL);
    }
#line 3786 "src/parser.tr"
    return (-1LL);
}

