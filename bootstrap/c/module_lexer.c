#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(TrStr source) {
#line 159 "src/lexer.tr"
    Lexer* lx = ((Lexer*)_tr_obj_alloc(sizeof(Lexer)));
#line 160 "src/lexer.tr"
    /* unsafe block */
#line 161 "src/lexer.tr"
    lx->src = ((char*)(_tr_strz(source)));
#line 162 "src/lexer.tr"
    long long src_len = 0LL;
#line 163 "src/lexer.tr"
    /* unsafe block */
#line 164 "src/lexer.tr"
    char* p = lx->src;
#line 165 "src/lexer.tr"
    while ((((long long)((*(p + src_len)))) != 0LL)) {
#line 166 "src/lexer.tr"
        src_len = (src_len + 1LL);
    }
#line 167 "src/lexer.tr"
    lx->len = src_len;
#line 168 "src/lexer.tr"
    lx->pos = 0LL;
#line 169 "src/lexer.tr"
    lx->line = 1LL;
#line 170 "src/lexer.tr"
    lx->line_start = 0LL;
#line 171 "src/lexer.tr"
    lx->tok_col = 1LL;
#line 172 "src/lexer.tr"
    lx->indent_stack = (void*)List_i64_new();
#line 173 "src/lexer.tr"
    List_i64_append(lx->indent_stack, 0LL);
#line 174 "src/lexer.tr"
    lx->pending_dedents = 0LL;
#line 175 "src/lexer.tr"
    lx->token_lines = (void*)List_i64_new();
#line 176 "src/lexer.tr"
    lx->token_cols = (void*)List_i64_new();
#line 177 "src/lexer.tr"
    lx->record_comments = false;
#line 178 "src/lexer.tr"
    lx->comment_lines = (void*)List_i64_new();
#line 179 "src/lexer.tr"
    lx->comment_texts = (void*)List_TrStr_new();
#line 180 "src/lexer.tr"
    lx->comment_trailing = (void*)List_bool_new();
#line 181 "src/lexer.tr"
    return lx;
}

__attribute__((hot)) void Lexer_push_loc(Lexer* self) {
#line 189 "src/lexer.tr"
    List_i64_append(self->token_lines, self->line);
#line 190 "src/lexer.tr"
    List_i64_append(self->token_cols, self->tok_col);
}

__attribute__((hot)) long long Lexer_peek(Lexer* self) {
#line 195 "src/lexer.tr"
    if ((self->pos >= self->len)) {
#line 196 "src/lexer.tr"
        return 0LL;
    }
#line 197 "src/lexer.tr"
    /* unsafe block */
#line 198 "src/lexer.tr"
    return ((long long)((*(self->src + self->pos))));
}

__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset) {
#line 201 "src/lexer.tr"
    long long p = (self->pos + offset);
#line 202 "src/lexer.tr"
    if ((p >= self->len)) {
#line 203 "src/lexer.tr"
        return 0LL;
    }
#line 204 "src/lexer.tr"
    /* unsafe block */
#line 205 "src/lexer.tr"
    return ((long long)((*(self->src + p))));
}

__attribute__((hot)) bool Lexer__at_eol_after_ws(Lexer* self) {
#line 211 "src/lexer.tr"
    long long k = 0LL;
#line 212 "src/lexer.tr"
    while (true) {
#line 213 "src/lexer.tr"
        long long ch = Lexer_peek_at(self, k);
#line 214 "src/lexer.tr"
        if (((ch == 32LL) || (ch == 9LL))) {
#line 215 "src/lexer.tr"
            k = (k + 1LL);
        } else {
#line 217 "src/lexer.tr"
            break;
        }
    }
#line 218 "src/lexer.tr"
    long long ch2 = Lexer_peek_at(self, k);
#line 219 "src/lexer.tr"
    if ((ch2 == 0LL)) {
#line 219 "src/lexer.tr"
        return true;
    }
#line 220 "src/lexer.tr"
    if ((ch2 == 35LL)) {
#line 220 "src/lexer.tr"
        return true;
    }
#line 221 "src/lexer.tr"
    if (char_is_newline(ch2)) {
#line 221 "src/lexer.tr"
        return true;
    }
#line 222 "src/lexer.tr"
    return false;
}

__attribute__((hot)) long long Lexer_advance(Lexer* self) {
#line 225 "src/lexer.tr"
    long long c = Lexer_peek(self);
#line 226 "src/lexer.tr"
    self->pos = (self->pos + 1LL);
#line 227 "src/lexer.tr"
    if ((c == 10LL)) {
#line 228 "src/lexer.tr"
        self->line = (self->line + 1LL);
#line 229 "src/lexer.tr"
        self->line_start = self->pos;
    }
#line 230 "src/lexer.tr"
    return c;
}

__attribute__((hot)) bool Lexer_at_end(Lexer* self) {
#line 233 "src/lexer.tr"
    return (self->pos >= self->len);
}

__attribute__((hot)) void Lexer_skip_spaces(Lexer* self) {
#line 238 "src/lexer.tr"
    while ((!Lexer_at_end(self))) {
#line 239 "src/lexer.tr"
        long long c = Lexer_peek(self);
#line 240 "src/lexer.tr"
        if (char_is_space(c)) {
#line 241 "src/lexer.tr"
            Lexer_advance(self);
        } else {
#line 243 "src/lexer.tr"
            break;
        }
    }
}

__attribute__((hot)) void Lexer_skip_comment(Lexer* self, bool trailing) {
#line 246 "src/lexer.tr"
    if (self->record_comments) {
#line 247 "src/lexer.tr"
        StringBuilder* sb = StringBuilder_init(32LL);
#line 248 "src/lexer.tr"
        while (((!Lexer_at_end(self)) && (!char_is_newline(Lexer_peek(self))))) {
#line 249 "src/lexer.tr"
            StringBuilder_append_char(sb, Lexer_advance(self));
        }
#line 250 "src/lexer.tr"
        List_i64_append(self->comment_lines, self->line);
#line 251 "src/lexer.tr"
        ({ TrStr _ord_t6 = (StringObj_as_str(StringBuilder_to_string(sb))); List_TrStr_append(self->comment_texts, _ord_t6); _tr_str_release(_ord_t6); });
#line 252 "src/lexer.tr"
        List_bool_append(self->comment_trailing, trailing);
        StringBuilder__tr_fn_free(sb);
    } else {
#line 254 "src/lexer.tr"
        while (((!Lexer_at_end(self)) && (!char_is_newline(Lexer_peek(self))))) {
#line 255 "src/lexer.tr"
            Lexer_advance(self);
        }
    }
}

__attribute__((hot)) void Lexer_skip_float_exponent(Lexer* self) {
#line 259 "src/lexer.tr"
    Lexer_advance(self);
#line 260 "src/lexer.tr"
    if (((Lexer_peek(self) == 45LL) || (Lexer_peek(self) == 43LL))) {
#line 261 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 262 "src/lexer.tr"
    while (char_is_digit(Lexer_peek(self))) {
#line 263 "src/lexer.tr"
        Lexer_advance(self);
    }
}

__attribute__((hot)) double Lexer_float_from_text(Lexer* self, long long start) {
#line 270 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(32LL);
#line 271 "src/lexer.tr"
    long long i = start;
#line 272 "src/lexer.tr"
    while ((i < self->pos)) {
#line 273 "src/lexer.tr"
        long long c = ((long long)((*(self->src + i))));
#line 274 "src/lexer.tr"
        if ((c != 95LL)) {
#line 274 "src/lexer.tr"
            StringBuilder_append_char(sb, c);
        }
#line 275 "src/lexer.tr"
        i = (i + 1LL);
    }
#line 276 "src/lexer.tr"
    return ({ TrStr _aet_t7 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (_tr_str_to_float(_aet_t7.data)); _tr_str_release(_aet_t7); _wr; });
}

__attribute__((hot)) Token Lexer_read_int(Lexer* self) {
#line 280 "src/lexer.tr"
    if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 120LL) || (Lexer_peek_at(self, 1LL) == 88LL)))) {
#line 281 "src/lexer.tr"
        Lexer_advance(self);
#line 281 "src/lexer.tr"
        Lexer_advance(self);
#line 282 "src/lexer.tr"
        unsigned long long val = ((unsigned long long)(0LL));
#line 283 "src/lexer.tr"
        while ((char_is_hex(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
#line 284 "src/lexer.tr"
            long long c = Lexer_advance(self);
#line 285 "src/lexer.tr"
            if ((c != 95LL)) {
#line 286 "src/lexer.tr"
                unsigned long long digit = ((unsigned long long)(0LL));
#line 287 "src/lexer.tr"
                if (((c >= 48LL) && (c <= 57LL))) {
#line 288 "src/lexer.tr"
                    digit = ((unsigned long long)((c - 48LL)));
                } else if (((c >= 65LL) && (c <= 70LL))) {
#line 290 "src/lexer.tr"
                    digit = ((unsigned long long)(((c - 65LL) + 10LL)));
                } else {
#line 292 "src/lexer.tr"
                    digit = ((unsigned long long)(((c - 97LL) + 10LL)));
                }
#line 293 "src/lexer.tr"
                val = ((val * ((unsigned long long)(16LL))) + digit);
            }
        }
#line 294 "src/lexer.tr"
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 98LL) || (Lexer_peek_at(self, 1LL) == 66LL)))) {
#line 297 "src/lexer.tr"
        Lexer_advance(self);
#line 297 "src/lexer.tr"
        Lexer_advance(self);
#line 298 "src/lexer.tr"
        unsigned long long val = ((unsigned long long)(0LL));
#line 299 "src/lexer.tr"
        while ((((Lexer_peek(self) == 48LL) || (Lexer_peek(self) == 49LL)) || (Lexer_peek(self) == 95LL))) {
#line 300 "src/lexer.tr"
            long long c = Lexer_advance(self);
#line 301 "src/lexer.tr"
            if ((c != 95LL)) {
#line 302 "src/lexer.tr"
                val = ((val * ((unsigned long long)(2LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
#line 303 "src/lexer.tr"
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 111LL) || (Lexer_peek_at(self, 1LL) == 79LL)))) {
#line 306 "src/lexer.tr"
        Lexer_advance(self);
#line 306 "src/lexer.tr"
        Lexer_advance(self);
#line 307 "src/lexer.tr"
        unsigned long long val = ((unsigned long long)(0LL));
#line 308 "src/lexer.tr"
        while ((((Lexer_peek(self) >= 48LL) && (Lexer_peek(self) <= 55LL)) || (Lexer_peek(self) == 95LL))) {
#line 309 "src/lexer.tr"
            long long c = Lexer_advance(self);
#line 310 "src/lexer.tr"
            if ((c != 95LL)) {
#line 311 "src/lexer.tr"
                val = ((val * ((unsigned long long)(8LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
#line 312 "src/lexer.tr"
        return Token_ctor_IntLit(((long long)(val)));
    } else {
#line 316 "src/lexer.tr"
        long long lit_start = self->pos;
#line 317 "src/lexer.tr"
        unsigned long long val = ((unsigned long long)(0LL));
#line 318 "src/lexer.tr"
        while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
#line 319 "src/lexer.tr"
            long long c = Lexer_advance(self);
#line 320 "src/lexer.tr"
            if ((c != 95LL)) {
#line 321 "src/lexer.tr"
                val = ((val * ((unsigned long long)(10LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
#line 323 "src/lexer.tr"
        if (((Lexer_peek(self) == 46LL) && char_is_digit(Lexer_peek_at(self, 1LL)))) {
#line 324 "src/lexer.tr"
            Lexer_advance(self);
#line 325 "src/lexer.tr"
            while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
#line 326 "src/lexer.tr"
                Lexer_advance(self);
            }
#line 328 "src/lexer.tr"
            if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
#line 329 "src/lexer.tr"
                Lexer_skip_float_exponent(self);
            }
#line 330 "src/lexer.tr"
            return Token_ctor_FloatLit(Lexer_float_from_text(self, lit_start));
        }
#line 332 "src/lexer.tr"
        if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
#line 333 "src/lexer.tr"
            Lexer_skip_float_exponent(self);
#line 334 "src/lexer.tr"
            return Token_ctor_FloatLit(Lexer_float_from_text(self, lit_start));
        }
#line 335 "src/lexer.tr"
        return Token_ctor_IntLit(((long long)(val)));
    }
}

__attribute__((hot)) Token Lexer_read_triple_string(Lexer* self, long long quote) {
#line 339 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(128LL);
#line 340 "src/lexer.tr"
    while ((!Lexer_at_end(self))) {
#line 341 "src/lexer.tr"
        if ((((Lexer_peek(self) == quote) && (Lexer_peek_at(self, 1LL) == quote)) && (Lexer_peek_at(self, 2LL) == quote))) {
#line 342 "src/lexer.tr"
            Lexer_advance(self);
#line 342 "src/lexer.tr"
            Lexer_advance(self);
#line 342 "src/lexer.tr"
            Lexer_advance(self);
#line 343 "src/lexer.tr"
            return ({ TrStr _ord_t8 = (StringBuilder_to_owned(sb)); __auto_type _ord_t9 = (sb->buf->len); __auto_type _wr = (Token_ctor_TripleStrLit(_ord_t8, _ord_t9)); _tr_str_release(_ord_t8); _wr; });
        }
#line 344 "src/lexer.tr"
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
#line 345 "src/lexer.tr"
    return ({ TrStr _ord_t10 = (StringBuilder_to_owned(sb)); __auto_type _ord_t11 = (sb->buf->len); __auto_type _wr = (Token_ctor_TripleStrLit(_ord_t10, _ord_t11)); _tr_str_release(_ord_t10); _wr; });
}

__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote) {
#line 348 "src/lexer.tr"
    Lexer_advance(self);
#line 349 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 350 "src/lexer.tr"
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
#line 351 "src/lexer.tr"
        long long c = Lexer_advance(self);
#line 352 "src/lexer.tr"
        if ((c == 92LL)) {
#line 353 "src/lexer.tr"
            long long esc = Lexer_advance(self);
#line 354 "src/lexer.tr"
            if ((esc == 110LL)) {
#line 354 "src/lexer.tr"
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
#line 355 "src/lexer.tr"
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
#line 356 "src/lexer.tr"
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
#line 357 "src/lexer.tr"
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
#line 358 "src/lexer.tr"
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
#line 359 "src/lexer.tr"
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
#line 360 "src/lexer.tr"
                StringBuilder_append_char(sb, 0LL);
            } else if (((((esc == 120LL) || (esc == 117LL)) || (esc == 85LL)) && Lexer_hex_run_ok(self, esc))) {
#line 363 "src/lexer.tr"
                long long ndig = 2LL;
#line 364 "src/lexer.tr"
                if ((esc == 117LL)) {
#line 364 "src/lexer.tr"
                    ndig = 4LL;
                }
#line 365 "src/lexer.tr"
                if ((esc == 85LL)) {
#line 365 "src/lexer.tr"
                    ndig = 8LL;
                }
#line 366 "src/lexer.tr"
                long long hv = 0LL;
#line 367 "src/lexer.tr"
                long long hk = 0LL;
#line 368 "src/lexer.tr"
                while ((hk < ndig)) {
#line 369 "src/lexer.tr"
                    hv = ((hv * 16LL) + lex_hex_val(Lexer_advance(self)));
#line 370 "src/lexer.tr"
                    hk = (hk + 1LL);
                }
#line 371 "src/lexer.tr"
                if ((esc == 120LL)) {
#line 371 "src/lexer.tr"
                    StringBuilder_append_char(sb, hv);
                } else {
#line 372 "src/lexer.tr"
                    lex_append_utf8(sb, hv);
                }
            } else {
#line 381 "src/lexer.tr"
                StringBuilder_append_char(sb, 92LL);
#line 382 "src/lexer.tr"
                StringBuilder_append_char(sb, esc);
            }
        } else {
#line 384 "src/lexer.tr"
            StringBuilder_append_char(sb, c);
        }
    }
#line 385 "src/lexer.tr"
    if ((!Lexer_at_end(self))) {
#line 385 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 386 "src/lexer.tr"
    return ({ TrStr _ord_t12 = (StringBuilder_to_owned(sb)); __auto_type _ord_t13 = (sb->buf->len); __auto_type _wr = (Token_ctor_StrLit(_ord_t12, _ord_t13)); _tr_str_release(_ord_t12); _wr; });
}

__attribute__((hot)) bool Lexer_hex_run_ok(Lexer* self, long long esc) {
#line 392 "src/lexer.tr"
    long long ndig = 2LL;
#line 393 "src/lexer.tr"
    if ((esc == 117LL)) {
#line 393 "src/lexer.tr"
        ndig = 4LL;
    }
#line 394 "src/lexer.tr"
    if ((esc == 85LL)) {
#line 394 "src/lexer.tr"
        ndig = 8LL;
    }
#line 395 "src/lexer.tr"
    long long k = 0LL;
#line 396 "src/lexer.tr"
    long long v = 0LL;
#line 397 "src/lexer.tr"
    while ((k < ndig)) {
#line 398 "src/lexer.tr"
        long long h = lex_hex_val(Lexer_peek_at(self, k));
#line 399 "src/lexer.tr"
        if ((h < 0LL)) {
#line 399 "src/lexer.tr"
            return false;
        }
#line 400 "src/lexer.tr"
        v = ((v * 16LL) + h);
#line 401 "src/lexer.tr"
        k = (k + 1LL);
    }
#line 402 "src/lexer.tr"
    if (((esc != 120LL) && (v > 1114111LL))) {
#line 402 "src/lexer.tr"
        return false;
    }
#line 403 "src/lexer.tr"
    return true;
}

__attribute__((hot)) Token Lexer_read_char(Lexer* self) {
#line 406 "src/lexer.tr"
    Lexer_advance(self);
#line 407 "src/lexer.tr"
    long long val = 0LL;
#line 408 "src/lexer.tr"
    if ((Lexer_peek(self) == 92LL)) {
#line 409 "src/lexer.tr"
        Lexer_advance(self);
#line 409 "src/lexer.tr"
        long long esc = Lexer_advance(self);
#line 410 "src/lexer.tr"
        if ((esc == 110LL)) {
#line 410 "src/lexer.tr"
            val = 10LL;
        } else if ((esc == 116LL)) {
#line 411 "src/lexer.tr"
            val = 9LL;
        } else if ((esc == 114LL)) {
#line 412 "src/lexer.tr"
            val = 13LL;
        } else if ((esc == 92LL)) {
#line 413 "src/lexer.tr"
            val = 92LL;
        } else if ((esc == 39LL)) {
#line 414 "src/lexer.tr"
            val = 39LL;
        } else if ((esc == 48LL)) {
#line 415 "src/lexer.tr"
            val = 0LL;
        } else if ((((esc == 120LL) && (lex_hex_val(Lexer_peek(self)) >= 0LL)) && (lex_hex_val(Lexer_peek_at(self, 1LL)) >= 0LL))) {
#line 417 "src/lexer.tr"
            val = (lex_hex_val(Lexer_advance(self)) * 16LL);
#line 418 "src/lexer.tr"
            val = (val + lex_hex_val(Lexer_advance(self)));
        } else {
#line 419 "src/lexer.tr"
            val = esc;
        }
    } else {
#line 421 "src/lexer.tr"
        val = Lexer_advance(self);
    }
#line 422 "src/lexer.tr"
    if ((Lexer_peek(self) == 39LL)) {
#line 422 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 423 "src/lexer.tr"
    return Token_ctor_CharLit(val);
}

__attribute__((hot)) Token Lexer_read_fstring(Lexer* self) {
#line 430 "src/lexer.tr"
    Lexer_advance(self);
#line 430 "src/lexer.tr"
    long long quote = Lexer_advance(self);
#line 431 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 432 "src/lexer.tr"
    long long depth = 0LL;
#line 433 "src/lexer.tr"
    while ((!Lexer_at_end(self))) {
#line 434 "src/lexer.tr"
        long long c = Lexer_peek(self);
#line 435 "src/lexer.tr"
        if (((depth == 0LL) && (c == quote))) {
#line 435 "src/lexer.tr"
            break;
        }
#line 436 "src/lexer.tr"
        if ((c == 92LL)) {
#line 437 "src/lexer.tr"
            StringBuilder_append_char(sb, Lexer_advance(self));
#line 438 "src/lexer.tr"
            if ((!Lexer_at_end(self))) {
#line 438 "src/lexer.tr"
                StringBuilder_append_char(sb, Lexer_advance(self));
            }
#line 439 "src/lexer.tr"
            continue;
        }
#line 440 "src/lexer.tr"
        if ((((depth == 0LL) && (c == 123LL)) && (Lexer_peek_at(self, 1LL) == 123LL))) {
#line 441 "src/lexer.tr"
            StringBuilder_append_char(sb, Lexer_advance(self));
#line 442 "src/lexer.tr"
            StringBuilder_append_char(sb, Lexer_advance(self));
#line 443 "src/lexer.tr"
            continue;
        }
#line 444 "src/lexer.tr"
        if ((c == 123LL)) {
#line 445 "src/lexer.tr"
            depth = (depth + 1LL);
        } else if ((c == 125LL)) {
#line 447 "src/lexer.tr"
            if ((depth > 0LL)) {
#line 447 "src/lexer.tr"
                depth = (depth - 1LL);
            }
        } else if (((depth > 0LL) && ((c == 34LL) || (c == 39LL)))) {
#line 449 "src/lexer.tr"
            long long q = Lexer_advance(self);
#line 450 "src/lexer.tr"
            StringBuilder_append_char(sb, q);
#line 451 "src/lexer.tr"
            while (((!Lexer_at_end(self)) && (Lexer_peek(self) != q))) {
#line 452 "src/lexer.tr"
                long long ch = Lexer_advance(self);
#line 453 "src/lexer.tr"
                StringBuilder_append_char(sb, ch);
#line 454 "src/lexer.tr"
                if (((ch == 92LL) && (!Lexer_at_end(self)))) {
#line 454 "src/lexer.tr"
                    StringBuilder_append_char(sb, Lexer_advance(self));
                }
            }
#line 455 "src/lexer.tr"
            if ((!Lexer_at_end(self))) {
#line 455 "src/lexer.tr"
                StringBuilder_append_char(sb, Lexer_advance(self));
            }
#line 456 "src/lexer.tr"
            continue;
        }
#line 457 "src/lexer.tr"
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
#line 458 "src/lexer.tr"
    if ((!Lexer_at_end(self))) {
#line 458 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 459 "src/lexer.tr"
    return ({ TrStr _ord_t14 = (StringBuilder_to_owned(sb)); __auto_type _ord_t15 = (sb->buf->len); __auto_type _wr = (Token_ctor_FStrLit(_ord_t14, _ord_t15)); _tr_str_release(_ord_t14); _wr; });
}

__attribute__((hot)) Token Lexer_read_triple_fstring(Lexer* self, long long quote) {
#line 466 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(128LL);
#line 467 "src/lexer.tr"
    while ((!Lexer_at_end(self))) {
#line 468 "src/lexer.tr"
        if ((((Lexer_peek(self) == quote) && (Lexer_peek_at(self, 1LL) == quote)) && (Lexer_peek_at(self, 2LL) == quote))) {
#line 469 "src/lexer.tr"
            Lexer_advance(self);
#line 469 "src/lexer.tr"
            Lexer_advance(self);
#line 469 "src/lexer.tr"
            Lexer_advance(self);
#line 470 "src/lexer.tr"
            return ({ TrStr _ord_t16 = (StringBuilder_to_owned(sb)); __auto_type _ord_t17 = (sb->buf->len); __auto_type _wr = (Token_ctor_FStrLit(_ord_t16, _ord_t17)); _tr_str_release(_ord_t16); _wr; });
        }
#line 471 "src/lexer.tr"
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
#line 472 "src/lexer.tr"
    return ({ TrStr _ord_t18 = (StringBuilder_to_owned(sb)); __auto_type _ord_t19 = (sb->buf->len); __auto_type _wr = (Token_ctor_FStrLit(_ord_t18, _ord_t19)); _tr_str_release(_ord_t18); _wr; });
}

__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self) {
#line 475 "src/lexer.tr"
    Lexer_advance(self);
#line 475 "src/lexer.tr"
    long long quote = Lexer_advance(self);
#line 476 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 477 "src/lexer.tr"
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
#line 478 "src/lexer.tr"
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
#line 479 "src/lexer.tr"
    if ((!Lexer_at_end(self))) {
#line 479 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 480 "src/lexer.tr"
    return ({ TrStr _ord_t20 = (StringBuilder_to_owned(sb)); __auto_type _ord_t21 = (sb->buf->len); __auto_type _wr = (Token_ctor_RawStrLit(_ord_t20, _ord_t21)); _tr_str_release(_ord_t20); _wr; });
}

__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self) {
#line 483 "src/lexer.tr"
    Lexer_advance(self);
#line 483 "src/lexer.tr"
    long long quote = Lexer_advance(self);
#line 484 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 485 "src/lexer.tr"
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
#line 486 "src/lexer.tr"
        long long c = Lexer_advance(self);
#line 487 "src/lexer.tr"
        if ((c == 92LL)) {
#line 488 "src/lexer.tr"
            long long esc = Lexer_advance(self);
#line 489 "src/lexer.tr"
            if ((esc == 110LL)) {
#line 489 "src/lexer.tr"
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
#line 490 "src/lexer.tr"
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
#line 491 "src/lexer.tr"
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
#line 492 "src/lexer.tr"
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
#line 493 "src/lexer.tr"
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
#line 494 "src/lexer.tr"
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
#line 495 "src/lexer.tr"
                StringBuilder_append_char(sb, 0LL);
            } else if (((((esc == 120LL) || (esc == 117LL)) || (esc == 85LL)) && Lexer_hex_run_ok(self, esc))) {
#line 498 "src/lexer.tr"
                long long ndig = 2LL;
#line 499 "src/lexer.tr"
                if ((esc == 117LL)) {
#line 499 "src/lexer.tr"
                    ndig = 4LL;
                }
#line 500 "src/lexer.tr"
                if ((esc == 85LL)) {
#line 500 "src/lexer.tr"
                    ndig = 8LL;
                }
#line 501 "src/lexer.tr"
                long long hv = 0LL;
#line 502 "src/lexer.tr"
                long long hk = 0LL;
#line 503 "src/lexer.tr"
                while ((hk < ndig)) {
#line 504 "src/lexer.tr"
                    hv = ((hv * 16LL) + lex_hex_val(Lexer_advance(self)));
#line 505 "src/lexer.tr"
                    hk = (hk + 1LL);
                }
#line 506 "src/lexer.tr"
                if ((esc == 120LL)) {
#line 506 "src/lexer.tr"
                    StringBuilder_append_char(sb, hv);
                } else {
#line 507 "src/lexer.tr"
                    lex_append_utf8(sb, hv);
                }
            } else {
#line 516 "src/lexer.tr"
                StringBuilder_append_char(sb, 92LL);
#line 517 "src/lexer.tr"
                StringBuilder_append_char(sb, esc);
            }
        } else {
#line 519 "src/lexer.tr"
            StringBuilder_append_char(sb, c);
        }
    }
#line 520 "src/lexer.tr"
    if ((!Lexer_at_end(self))) {
#line 520 "src/lexer.tr"
        Lexer_advance(self);
    }
#line 521 "src/lexer.tr"
    return ({ TrStr _ord_t22 = (StringBuilder_to_owned(sb)); __auto_type _ord_t23 = (sb->buf->len); __auto_type _wr = (Token_ctor_ByteStrLit(_ord_t22, _ord_t23)); _tr_str_release(_ord_t22); _wr; });
}

__attribute__((hot)) Token Lexer_read_ident(Lexer* self) {
#line 524 "src/lexer.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 525 "src/lexer.tr"
    while (char_is_alnum(Lexer_peek(self))) {
#line 526 "src/lexer.tr"
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
#line 527 "src/lexer.tr"
    return ({ TrStr _at_t24 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (keyword_to_token(_at_t24)); _tr_str_release(_at_t24); _wr; });
}

__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self) {
#line 533 "src/lexer.tr"
    if (((self->pos == 0LL) && (self->len >= 3LL))) {
#line 534 "src/lexer.tr"
        if (((((Lexer_peek_at(self, 0LL) & 255LL) == 239LL) && ((Lexer_peek_at(self, 1LL) & 255LL) == 187LL)) && ((Lexer_peek_at(self, 2LL) & 255LL) == 191LL))) {
#line 535 "src/lexer.tr"
            self->pos = (self->pos + 3LL);
        }
    }
#line 537 "src/lexer.tr"
    List_Token* tokens = (void*)List_Token_new();
#line 538 "src/lexer.tr"
    bool at_line_start = true;
#line 539 "src/lexer.tr"
    bool trailing_dot = false;
#line 540 "src/lexer.tr"
    long long nesting = 0LL;
#line 546 "src/lexer.tr"
    List_i64* susp_nesting = (void*)List_i64_new();
#line 547 "src/lexer.tr"
    List_i64* susp_baseline = (void*)List_i64_new();
#line 548 "src/lexer.tr"
    bool block_kw_seen = false;
#line 550 "src/lexer.tr"
    while ((!Lexer_at_end(self))) {
#line 551 "src/lexer.tr"
        if ((self->pending_dedents > 0LL)) {
#line 552 "src/lexer.tr"
            self->tok_col = 1LL;
#line 553 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Dedent());
#line 554 "src/lexer.tr"
            Lexer_push_loc(self);
#line 555 "src/lexer.tr"
            self->pending_dedents = (self->pending_dedents - 1LL);
#line 556 "src/lexer.tr"
            continue;
        }
#line 558 "src/lexer.tr"
        if (at_line_start) {
#line 559 "src/lexer.tr"
            at_line_start = false;
#line 562 "src/lexer.tr"
            long long indent = 0LL;
#line 563 "src/lexer.tr"
            while ((!Lexer_at_end(self))) {
#line 564 "src/lexer.tr"
                long long c_peek = Lexer_peek(self);
#line 565 "src/lexer.tr"
                if ((c_peek == 32LL)) {
#line 566 "src/lexer.tr"
                    indent = (indent + 1LL);
#line 567 "src/lexer.tr"
                    Lexer_advance(self);
                } else if ((c_peek == 9LL)) {
#line 569 "src/lexer.tr"
                    indent = (indent + 4LL);
#line 570 "src/lexer.tr"
                    Lexer_advance(self);
                } else {
#line 572 "src/lexer.tr"
                    break;
                }
            }
#line 574 "src/lexer.tr"
            if (Lexer_at_end(self)) {
#line 574 "src/lexer.tr"
                break;
            }
#line 577 "src/lexer.tr"
            long long next_c = Lexer_peek(self);
#line 578 "src/lexer.tr"
            if ((char_is_newline(next_c) || (next_c == 35LL))) {
#line 579 "src/lexer.tr"
                if ((next_c == 35LL)) {
#line 580 "src/lexer.tr"
                    Lexer_advance(self);
#line 581 "src/lexer.tr"
                    Lexer_skip_comment(self, false);
                } else {
#line 583 "src/lexer.tr"
                    Lexer_advance(self);
#line 584 "src/lexer.tr"
                    if (((next_c == 13LL) && (Lexer_peek(self) == 10LL))) {
#line 584 "src/lexer.tr"
                        Lexer_advance(self);
                    }
                }
#line 586 "src/lexer.tr"
                at_line_start = true;
#line 587 "src/lexer.tr"
                continue;
            }
#line 597 "src/lexer.tr"
            if ((nesting == 0LL)) {
#line 598 "src/lexer.tr"
                self->tok_col = ((self->pos - self->line_start) + 1LL);
#line 602 "src/lexer.tr"
                if (((next_c != 46LL) && (!trailing_dot))) {
#line 603 "src/lexer.tr"
                    long long cur_indent = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
#line 604 "src/lexer.tr"
                    if ((indent > cur_indent)) {
#line 605 "src/lexer.tr"
                        List_i64_append(self->indent_stack, indent);
#line 606 "src/lexer.tr"
                        List_Token_append(tokens, Token_make_Indent());
#line 607 "src/lexer.tr"
                        Lexer_push_loc(self);
                    } else if ((indent < cur_indent)) {
#line 609 "src/lexer.tr"
                        while ((self->indent_stack->len > 1LL)) {
#line 610 "src/lexer.tr"
                            long long top = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
#line 611 "src/lexer.tr"
                            if ((top <= indent)) {
#line 611 "src/lexer.tr"
                                break;
                            }
#line 612 "src/lexer.tr"
                            self->indent_stack->len = (self->indent_stack->len - 1LL);
#line 613 "src/lexer.tr"
                            List_Token_append(tokens, Token_make_Dedent());
#line 614 "src/lexer.tr"
                            Lexer_push_loc(self);
                        }
#line 619 "src/lexer.tr"
                        long long newtop = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
#line 620 "src/lexer.tr"
                        if ((newtop != indent)) {
#line 621 "src/lexer.tr"
                            self->tok_col = ((self->pos - self->line_start) + 1LL);
#line 622 "src/lexer.tr"
                            List_Token_append(tokens, Token_ctor_Error(_tr_str_lit_len("inconsistent indentation: this line does not line up with any enclosing block (check for mixed tabs and spaces, or an off-by-one indent)", 136LL)));
#line 623 "src/lexer.tr"
                            Lexer_push_loc(self);
                        }
                    }
                }
#line 624 "src/lexer.tr"
                trailing_dot = false;
#line 629 "src/lexer.tr"
                while ((susp_nesting->len > 0LL)) {
#line 630 "src/lexer.tr"
                    long long _base = List_i64_get(susp_baseline, (susp_baseline->len - 1LL));
#line 631 "src/lexer.tr"
                    if ((indent > _base)) {
#line 631 "src/lexer.tr"
                        break;
                    }
#line 632 "src/lexer.tr"
                    nesting = List_i64_get(susp_nesting, (susp_nesting->len - 1LL));
#line 633 "src/lexer.tr"
                    susp_nesting->len = (susp_nesting->len - 1LL);
#line 634 "src/lexer.tr"
                    susp_baseline->len = (susp_baseline->len - 1LL);
#line 635 "src/lexer.tr"
                    block_kw_seen = false;
                }
            }
        }
#line 637 "src/lexer.tr"
        long long c = Lexer_peek(self);
#line 642 "src/lexer.tr"
        self->tok_col = ((self->pos - self->line_start) + 1LL);
#line 643 "src/lexer.tr"
        if (char_is_space(c)) {
#line 644 "src/lexer.tr"
            Lexer_advance(self);
#line 645 "src/lexer.tr"
            continue;
        }
#line 646 "src/lexer.tr"
        if (char_is_newline(c)) {
#line 647 "src/lexer.tr"
            Lexer_advance(self);
#line 648 "src/lexer.tr"
            if (((c == 13LL) && (Lexer_peek(self) == 10LL))) {
#line 648 "src/lexer.tr"
                Lexer_advance(self);
            }
#line 649 "src/lexer.tr"
            if ((nesting == 0LL)) {
#line 654 "src/lexer.tr"
                if (_last_tok_is_dot(tokens)) {
#line 655 "src/lexer.tr"
                    trailing_dot = true;
                } else if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
#line 657 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_Newline());
#line 658 "src/lexer.tr"
                    Lexer_push_loc(self);
                }
#line 659 "src/lexer.tr"
                at_line_start = true;
            }
#line 660 "src/lexer.tr"
            continue;
        }
#line 661 "src/lexer.tr"
        if ((c == 35LL)) {
#line 662 "src/lexer.tr"
            Lexer_advance(self);
#line 663 "src/lexer.tr"
            Lexer_skip_comment(self, true);
#line 664 "src/lexer.tr"
            if ((nesting == 0LL)) {
#line 665 "src/lexer.tr"
                if (_last_tok_is_dot(tokens)) {
#line 666 "src/lexer.tr"
                    trailing_dot = true;
                } else if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
#line 668 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_Newline());
#line 669 "src/lexer.tr"
                    Lexer_push_loc(self);
                }
#line 670 "src/lexer.tr"
                at_line_start = true;
            }
#line 671 "src/lexer.tr"
            continue;
        }
#line 673 "src/lexer.tr"
        if (char_is_digit(c)) {
#line 674 "src/lexer.tr"
            List_Token_append(tokens, Lexer_read_int(self));
#line 675 "src/lexer.tr"
            Lexer_push_loc(self);
#line 676 "src/lexer.tr"
            continue;
        }
#line 677 "src/lexer.tr"
        if (((c == 34LL) || (c == 39LL))) {
#line 678 "src/lexer.tr"
            if ((((c == 34LL) && (Lexer_peek_at(self, 1LL) == 34LL)) && (Lexer_peek_at(self, 2LL) == 34LL))) {
#line 679 "src/lexer.tr"
                Lexer_advance(self);
#line 679 "src/lexer.tr"
                Lexer_advance(self);
#line 679 "src/lexer.tr"
                Lexer_advance(self);
#line 680 "src/lexer.tr"
                List_Token_append(tokens, Lexer_read_triple_string(self, 34LL));
#line 681 "src/lexer.tr"
                Lexer_push_loc(self);
            } else if (((c == 39LL) && ((Lexer_peek_at(self, 2LL) == 39LL) || (Lexer_peek_at(self, 1LL) == 92LL)))) {
#line 683 "src/lexer.tr"
                List_Token_append(tokens, Lexer_read_char(self));
#line 684 "src/lexer.tr"
                Lexer_push_loc(self);
            } else {
#line 686 "src/lexer.tr"
                List_Token_append(tokens, Lexer_read_string(self, c));
#line 687 "src/lexer.tr"
                Lexer_push_loc(self);
            }
#line 688 "src/lexer.tr"
            continue;
        }
#line 689 "src/lexer.tr"
        if (((((c == 102LL) && (Lexer_peek_at(self, 1LL) == 34LL)) && (Lexer_peek_at(self, 2LL) == 34LL)) && (Lexer_peek_at(self, 3LL) == 34LL))) {
#line 690 "src/lexer.tr"
            Lexer_advance(self);
#line 690 "src/lexer.tr"
            Lexer_advance(self);
#line 690 "src/lexer.tr"
            Lexer_advance(self);
#line 690 "src/lexer.tr"
            Lexer_advance(self);
#line 691 "src/lexer.tr"
            List_Token_append(tokens, Lexer_read_triple_fstring(self, 34LL));
#line 692 "src/lexer.tr"
            Lexer_push_loc(self);
#line 693 "src/lexer.tr"
            continue;
        }
#line 694 "src/lexer.tr"
        if (((c == 102LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
#line 695 "src/lexer.tr"
            List_Token_append(tokens, Lexer_read_fstring(self));
#line 696 "src/lexer.tr"
            Lexer_push_loc(self);
#line 697 "src/lexer.tr"
            continue;
        }
#line 698 "src/lexer.tr"
        if (((c == 114LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
#line 699 "src/lexer.tr"
            List_Token_append(tokens, Lexer_read_raw_string(self));
#line 700 "src/lexer.tr"
            Lexer_push_loc(self);
#line 701 "src/lexer.tr"
            continue;
        }
#line 702 "src/lexer.tr"
        if (((c == 98LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
#line 703 "src/lexer.tr"
            List_Token_append(tokens, Lexer_read_byte_string(self));
#line 704 "src/lexer.tr"
            Lexer_push_loc(self);
#line 705 "src/lexer.tr"
            continue;
        }
#line 706 "src/lexer.tr"
        if (char_is_alpha(c)) {
#line 707 "src/lexer.tr"
            Token _idtok = Lexer_read_ident(self);
#line 715 "src/lexer.tr"
            __auto_type _t25 = _idtok;
            if (_t25.tag == Token_KwDo) {
                block_kw_seen = true;
            } else if (_t25.tag == Token_KwIf) {
                block_kw_seen = true;
            } else if (_t25.tag == Token_KwMatch) {
                block_kw_seen = true;
            } else if (_t25.tag == Token_KwLoop) {
                block_kw_seen = true;
            } else if (1) {
                __auto_type _ = _t25;
                /* pass */
            }
#line 721 "src/lexer.tr"
            List_Token_append(tokens, _idtok);
#line 722 "src/lexer.tr"
            Lexer_push_loc(self);
#line 723 "src/lexer.tr"
            continue;
        }
#line 725 "src/lexer.tr"
        Lexer_advance(self);
#line 726 "src/lexer.tr"
        if ((c == 61LL)) {
#line 727 "src/lexer.tr"
            long long nc = Lexer_peek(self);
#line 728 "src/lexer.tr"
            if ((nc == 61LL)) {
#line 729 "src/lexer.tr"
                Lexer_advance(self);
#line 730 "src/lexer.tr"
                List_Token_append(tokens, Token_make_EqEq());
            } else if ((nc == 62LL)) {
#line 732 "src/lexer.tr"
                Lexer_advance(self);
#line 733 "src/lexer.tr"
                List_Token_append(tokens, Token_make_FatArrow());
            } else {
#line 735 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Eq());
            }
        } else if ((c == 33LL)) {
#line 737 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 738 "src/lexer.tr"
                Lexer_advance(self);
#line 739 "src/lexer.tr"
                List_Token_append(tokens, Token_make_NotEq());
            } else {
#line 742 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Bang());
            }
        } else if ((c == 60LL)) {
#line 744 "src/lexer.tr"
            long long nc = Lexer_peek(self);
#line 745 "src/lexer.tr"
            if ((nc == 61LL)) {
#line 746 "src/lexer.tr"
                Lexer_advance(self);
#line 747 "src/lexer.tr"
                List_Token_append(tokens, Token_make_LtEq());
            } else if ((nc == 60LL)) {
#line 749 "src/lexer.tr"
                Lexer_advance(self);
#line 750 "src/lexer.tr"
                if ((Lexer_peek(self) == 61LL)) {
#line 751 "src/lexer.tr"
                    Lexer_advance(self);
#line 752 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_LtLtEq());
                } else {
#line 754 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_LtLt());
                }
            } else {
#line 756 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Lt());
            }
        } else if ((c == 62LL)) {
#line 758 "src/lexer.tr"
            long long nc = Lexer_peek(self);
#line 759 "src/lexer.tr"
            if ((nc == 61LL)) {
#line 760 "src/lexer.tr"
                Lexer_advance(self);
#line 761 "src/lexer.tr"
                List_Token_append(tokens, Token_make_GtEq());
            } else if ((nc == 62LL)) {
#line 763 "src/lexer.tr"
                Lexer_advance(self);
#line 764 "src/lexer.tr"
                if ((Lexer_peek(self) == 61LL)) {
#line 765 "src/lexer.tr"
                    Lexer_advance(self);
#line 766 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_GtGtEq());
                } else {
#line 768 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_GtGt());
                }
            } else {
#line 770 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Gt());
            }
        } else if ((c == 43LL)) {
#line 772 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 773 "src/lexer.tr"
                Lexer_advance(self);
#line 774 "src/lexer.tr"
                List_Token_append(tokens, Token_make_PlusEq());
            } else {
#line 776 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Plus());
            }
        } else if ((c == 45LL)) {
#line 778 "src/lexer.tr"
            long long nc = Lexer_peek(self);
#line 779 "src/lexer.tr"
            if ((nc == 62LL)) {
#line 780 "src/lexer.tr"
                Lexer_advance(self);
#line 781 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Arrow());
            } else if ((nc == 61LL)) {
#line 783 "src/lexer.tr"
                Lexer_advance(self);
#line 784 "src/lexer.tr"
                List_Token_append(tokens, Token_make_MinusEq());
            } else {
#line 786 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Minus());
            }
        } else if ((c == 42LL)) {
#line 788 "src/lexer.tr"
            if ((Lexer_peek(self) == 42LL)) {
#line 789 "src/lexer.tr"
                Lexer_advance(self);
#line 790 "src/lexer.tr"
                if ((Lexer_peek(self) == 61LL)) {
#line 791 "src/lexer.tr"
                    Lexer_advance(self);
#line 792 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_StarStarEq());
                } else {
#line 794 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_StarStar());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
#line 796 "src/lexer.tr"
                Lexer_advance(self);
#line 797 "src/lexer.tr"
                List_Token_append(tokens, Token_make_StarEq());
            } else {
#line 799 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Star());
            }
        } else if ((c == 47LL)) {
#line 801 "src/lexer.tr"
            if ((Lexer_peek(self) == 47LL)) {
#line 802 "src/lexer.tr"
                Lexer_advance(self);
#line 803 "src/lexer.tr"
                if ((Lexer_peek(self) == 61LL)) {
#line 804 "src/lexer.tr"
                    Lexer_advance(self);
#line 805 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_FloorDivEq());
                } else {
#line 807 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_FloorDiv());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
#line 809 "src/lexer.tr"
                Lexer_advance(self);
#line 810 "src/lexer.tr"
                List_Token_append(tokens, Token_make_SlashEq());
            } else {
#line 812 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Slash());
            }
        } else if ((c == 37LL)) {
#line 814 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 815 "src/lexer.tr"
                Lexer_advance(self);
#line 816 "src/lexer.tr"
                List_Token_append(tokens, Token_make_PercentEq());
            } else {
#line 818 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Percent());
            }
        } else if ((c == 46LL)) {
#line 820 "src/lexer.tr"
            if ((Lexer_peek(self) == 46LL)) {
#line 821 "src/lexer.tr"
                Lexer_advance(self);
#line 822 "src/lexer.tr"
                if ((Lexer_peek(self) == 46LL)) {
#line 823 "src/lexer.tr"
                    Lexer_advance(self);
#line 824 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_DotDotDot());
                } else if ((Lexer_peek(self) == 61LL)) {
#line 826 "src/lexer.tr"
                    Lexer_advance(self);
#line 827 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_DotDotEq());
                } else {
#line 829 "src/lexer.tr"
                    List_Token_append(tokens, Token_make_DotDot());
                }
            } else {
#line 831 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Dot());
            }
        } else if ((c == 40LL)) {
#line 833 "src/lexer.tr"
            List_Token_append(tokens, Token_make_LParen());
#line 834 "src/lexer.tr"
            nesting = (nesting + 1LL);
        } else if ((c == 41LL)) {
#line 836 "src/lexer.tr"
            List_Token_append(tokens, Token_make_RParen());
#line 837 "src/lexer.tr"
            if ((nesting > 0LL)) {
#line 838 "src/lexer.tr"
                nesting = (nesting - 1LL);
            }
        } else if ((c == 91LL)) {
#line 840 "src/lexer.tr"
            List_Token_append(tokens, Token_make_LBracket());
#line 841 "src/lexer.tr"
            nesting = (nesting + 1LL);
        } else if ((c == 93LL)) {
#line 843 "src/lexer.tr"
            List_Token_append(tokens, Token_make_RBracket());
#line 844 "src/lexer.tr"
            if ((nesting > 0LL)) {
#line 845 "src/lexer.tr"
                nesting = (nesting - 1LL);
            }
        } else if ((c == 123LL)) {
#line 847 "src/lexer.tr"
            List_Token_append(tokens, Token_make_LBrace());
#line 848 "src/lexer.tr"
            nesting = (nesting + 1LL);
        } else if ((c == 125LL)) {
#line 850 "src/lexer.tr"
            List_Token_append(tokens, Token_make_RBrace());
#line 851 "src/lexer.tr"
            if ((nesting > 0LL)) {
#line 852 "src/lexer.tr"
                nesting = (nesting - 1LL);
            }
        } else if ((c == 58LL)) {
#line 854 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Colon());
#line 857 "src/lexer.tr"
            if ((((nesting > 0LL) && block_kw_seen) && Lexer__at_eol_after_ws(self))) {
#line 858 "src/lexer.tr"
                List_i64_append(susp_nesting, nesting);
#line 859 "src/lexer.tr"
                List_i64_append(susp_baseline, List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL)));
#line 860 "src/lexer.tr"
                nesting = 0LL;
            }
#line 861 "src/lexer.tr"
            block_kw_seen = false;
        } else if ((c == 44LL)) {
#line 862 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Comma());
        } else if ((c == 59LL)) {
#line 863 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Semicolon());
        } else if ((c == 64LL)) {
#line 864 "src/lexer.tr"
            List_Token_append(tokens, Token_make_At());
        } else if ((c == 63LL)) {
#line 865 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Question());
        } else if ((c == 126LL)) {
#line 866 "src/lexer.tr"
            List_Token_append(tokens, Token_make_Tilde());
        } else if ((c == 38LL)) {
#line 868 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 869 "src/lexer.tr"
                Lexer_advance(self);
#line 870 "src/lexer.tr"
                List_Token_append(tokens, Token_make_AmpEq());
            } else {
#line 872 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Amp());
            }
        } else if ((c == 124LL)) {
#line 874 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 875 "src/lexer.tr"
                Lexer_advance(self);
#line 876 "src/lexer.tr"
                List_Token_append(tokens, Token_make_PipeEq());
            } else {
#line 878 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Pipe());
            }
        } else if ((c == 94LL)) {
#line 880 "src/lexer.tr"
            if ((Lexer_peek(self) == 61LL)) {
#line 881 "src/lexer.tr"
                Lexer_advance(self);
#line 882 "src/lexer.tr"
                List_Token_append(tokens, Token_make_CaretEq());
            } else {
#line 884 "src/lexer.tr"
                List_Token_append(tokens, Token_make_Caret());
            }
        } else {
#line 886 "src/lexer.tr"
            Lexer_advance(self);
#line 887 "src/lexer.tr"
            List_Token_append(tokens, Token_ctor_Error(_tr_str_lit_len("?", 1LL)));
        }
#line 888 "src/lexer.tr"
        Lexer_push_loc(self);
    }
#line 890 "src/lexer.tr"
    self->tok_col = 1LL;
#line 891 "src/lexer.tr"
    while ((self->indent_stack->len > 1LL)) {
#line 892 "src/lexer.tr"
        self->indent_stack->len = (self->indent_stack->len - 1LL);
#line 893 "src/lexer.tr"
        List_Token_append(tokens, Token_make_Dedent());
#line 894 "src/lexer.tr"
        Lexer_push_loc(self);
    }
#line 895 "src/lexer.tr"
    List_Token_append(tokens, Token_make_Eof());
#line 896 "src/lexer.tr"
    Lexer_push_loc(self);
#line 897 "src/lexer.tr"
    List_i64_free(susp_nesting);
    List_i64_free(susp_baseline);
    return tokens;
}

__attribute__((hot)) Token keyword_to_token(TrStr s) {
#line 15 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("def", 3LL)))) {
#line 15 "src/lexer.tr"
        return Token_make_KwDef();
    }
#line 16 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("class", 5LL)))) {
#line 16 "src/lexer.tr"
        return Token_make_KwClass();
    }
#line 17 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("enum", 4LL)))) {
#line 17 "src/lexer.tr"
        return Token_make_KwEnum();
    }
#line 18 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("interface", 9LL)))) {
#line 18 "src/lexer.tr"
        return Token_make_KwInterface();
    }
#line 19 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("extend", 6LL)))) {
#line 19 "src/lexer.tr"
        return Token_make_KwExtend();
    }
#line 20 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("struct", 6LL)))) {
#line 20 "src/lexer.tr"
        return Token_make_KwStruct();
    }
#line 21 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("if", 2LL)))) {
#line 21 "src/lexer.tr"
        return Token_make_KwIf();
    }
#line 22 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("elif", 4LL)))) {
#line 22 "src/lexer.tr"
        return Token_make_KwElif();
    }
#line 23 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("else", 4LL)))) {
#line 23 "src/lexer.tr"
        return Token_make_KwElse();
    }
#line 24 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("for", 3LL)))) {
#line 24 "src/lexer.tr"
        return Token_make_KwFor();
    }
#line 25 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("while", 5LL)))) {
#line 25 "src/lexer.tr"
        return Token_make_KwWhile();
    }
#line 26 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("loop", 4LL)))) {
#line 26 "src/lexer.tr"
        return Token_make_KwLoop();
    }
#line 27 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("return", 6LL)))) {
#line 27 "src/lexer.tr"
        return Token_make_KwReturn();
    }
#line 28 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("break", 5LL)))) {
#line 28 "src/lexer.tr"
        return Token_make_KwBreak();
    }
#line 29 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("continue", 8LL)))) {
#line 29 "src/lexer.tr"
        return Token_make_KwContinue();
    }
#line 30 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("pass", 4LL)))) {
#line 30 "src/lexer.tr"
        return Token_make_KwPass();
    }
#line 31 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("match", 5LL)))) {
#line 31 "src/lexer.tr"
        return Token_make_KwMatch();
    }
#line 32 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("case", 4LL)))) {
#line 32 "src/lexer.tr"
        return Token_make_KwCase();
    }
#line 33 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("try", 3LL)))) {
#line 33 "src/lexer.tr"
        return Token_make_KwTry();
    }
#line 34 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("except", 6LL)))) {
#line 34 "src/lexer.tr"
        return Token_make_KwExcept();
    }
#line 35 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("finally", 7LL)))) {
#line 35 "src/lexer.tr"
        return Token_make_KwFinally();
    }
#line 36 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("raise", 5LL)))) {
#line 36 "src/lexer.tr"
        return Token_make_KwRaise();
    }
#line 37 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("assert", 6LL)))) {
#line 37 "src/lexer.tr"
        return Token_make_KwAssert();
    }
#line 38 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("with", 4LL)))) {
#line 38 "src/lexer.tr"
        return Token_make_KwWith();
    }
#line 39 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("defer", 5LL)))) {
#line 39 "src/lexer.tr"
        return Token_make_KwDefer();
    }
#line 40 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("asm", 3LL)))) {
#line 40 "src/lexer.tr"
        return Token_make_KwAsm();
    }
#line 41 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("taskgroup", 9LL))) || _tr_str_eqv((s), (_tr_str_lit_len("task_group", 10LL))))) {
#line 41 "src/lexer.tr"
        return Token_make_KwTaskGroup();
    }
#line 45 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("implements", 10LL)))) {
#line 45 "src/lexer.tr"
        return Token_make_KwImplements();
    }
#line 46 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("import", 6LL)))) {
#line 46 "src/lexer.tr"
        return Token_make_KwImport();
    }
#line 47 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("from", 4LL)))) {
#line 47 "src/lexer.tr"
        return Token_make_KwFrom();
    }
#line 48 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("as", 2LL)))) {
#line 48 "src/lexer.tr"
        return Token_make_KwAs();
    }
#line 49 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("in", 2LL)))) {
#line 49 "src/lexer.tr"
        return Token_make_KwIn();
    }
#line 50 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("mut", 3LL)))) {
#line 50 "src/lexer.tr"
        return Token_make_KwMut();
    }
#line 51 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("shared", 6LL)))) {
#line 51 "src/lexer.tr"
        return Token_make_KwShared();
    }
#line 52 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("extern", 6LL)))) {
#line 52 "src/lexer.tr"
        return Token_make_KwExtern();
    }
#line 53 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("unsafe", 6LL)))) {
#line 53 "src/lexer.tr"
        return Token_make_KwUnsafe();
    }
#line 54 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("spawn", 5LL)))) {
#line 54 "src/lexer.tr"
        return Token_make_KwSpawn();
    }
#line 55 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("throws", 6LL)))) {
#line 55 "src/lexer.tr"
        return Token_make_KwThrows();
    }
#line 56 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("extends", 7LL)))) {
#line 56 "src/lexer.tr"
        return Token_make_KwExtends();
    }
#line 57 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("async", 5LL)))) {
#line 57 "src/lexer.tr"
        return Token_make_KwAsync();
    }
#line 58 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("await", 5LL)))) {
#line 58 "src/lexer.tr"
        return Token_make_KwAwait();
    }
#line 59 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("yield", 5LL)))) {
#line 59 "src/lexer.tr"
        return Token_make_KwYield();
    }
#line 60 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("pub", 3LL)))) {
#line 60 "src/lexer.tr"
        return Token_make_KwPub();
    }
#line 61 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("static", 6LL)))) {
#line 61 "src/lexer.tr"
        return Token_make_KwStatic();
    }
#line 62 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("const", 5LL)))) {
#line 62 "src/lexer.tr"
        return Token_make_KwConst();
    }
#line 63 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("actor", 5LL)))) {
#line 63 "src/lexer.tr"
        return Token_make_KwActor();
    }
#line 64 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("super", 5LL)))) {
#line 64 "src/lexer.tr"
        return Token_make_KwSuper();
    }
#line 65 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("export", 6LL)))) {
#line 65 "src/lexer.tr"
        return Token_make_KwExport();
    }
#line 66 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("lambda", 6LL)))) {
#line 66 "src/lexer.tr"
        return Token_make_KwLambda();
    }
#line 67 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("decorator", 9LL)))) {
#line 67 "src/lexer.tr"
        return Token_make_KwDecorator();
    }
#line 68 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("macro", 5LL)))) {
#line 68 "src/lexer.tr"
        return Token_make_KwMacro();
    }
#line 69 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("do", 2LL)))) {
#line 69 "src/lexer.tr"
        return Token_make_KwDo();
    }
#line 70 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("sizeof", 6LL)))) {
#line 70 "src/lexer.tr"
        return Token_make_KwSizeOf();
    }
#line 71 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("is", 2LL)))) {
#line 71 "src/lexer.tr"
        return Token_make_KwIs();
    }
#line 73 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("true", 4LL))) || _tr_str_eqv((s), (_tr_str_lit_len("True", 4LL))))) {
#line 73 "src/lexer.tr"
        return Token_make_KwTrue();
    }
#line 74 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("false", 5LL))) || _tr_str_eqv((s), (_tr_str_lit_len("False", 5LL))))) {
#line 74 "src/lexer.tr"
        return Token_make_KwFalse();
    }
#line 75 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("none", 4LL))) || _tr_str_eqv((s), (_tr_str_lit_len("None", 4LL))))) {
#line 75 "src/lexer.tr"
        return Token_make_KwNone();
    }
#line 76 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("and", 3LL)))) {
#line 76 "src/lexer.tr"
        return Token_make_KwAnd();
    }
#line 77 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("or", 2LL)))) {
#line 77 "src/lexer.tr"
        return Token_make_KwOr();
    }
#line 78 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("not", 3LL)))) {
#line 78 "src/lexer.tr"
        return Token_make_KwNot();
    }
#line 81 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((s), (_tr_str_lit_len("i64", 3LL))))) {
#line 81 "src/lexer.tr"
        return Token_make_KwInt();
    }
#line 82 "src/lexer.tr"
    if ((_tr_str_eqv((s), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((s), (_tr_str_lit_len("f64", 3LL))))) {
#line 82 "src/lexer.tr"
        return Token_make_KwFloat();
    }
#line 83 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("bool", 4LL)))) {
#line 83 "src/lexer.tr"
        return Token_make_KwBool();
    }
#line 84 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("char", 4LL)))) {
#line 84 "src/lexer.tr"
        return Token_make_KwChar();
    }
#line 85 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("str", 3LL)))) {
#line 85 "src/lexer.tr"
        return Token_make_KwStr();
    }
#line 86 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("String", 6LL)))) {
#line 86 "src/lexer.tr"
        return Token_make_KwString();
    }
#line 87 "src/lexer.tr"
    if (_tr_str_eqv((s), (_tr_str_lit_len("void", 4LL)))) {
#line 87 "src/lexer.tr"
        return Token_make_KwVoid();
    }
#line 89 "src/lexer.tr"
    return Token_ctor_Ident(s);
}

__attribute__((hot)) long long lex_hex_val(long long c) {
#line 95 "src/lexer.tr"
    if (((c >= 48LL) && (c <= 57LL))) {
#line 95 "src/lexer.tr"
        return (c - 48LL);
    }
#line 96 "src/lexer.tr"
    if (((c >= 97LL) && (c <= 102LL))) {
#line 96 "src/lexer.tr"
        return (c - 87LL);
    }
#line 97 "src/lexer.tr"
    if (((c >= 65LL) && (c <= 70LL))) {
#line 97 "src/lexer.tr"
        return (c - 55LL);
    }
#line 98 "src/lexer.tr"
    return (-1LL);
}

__attribute__((hot)) void lex_append_utf8(StringBuilder* sb, long long cp) {
#line 102 "src/lexer.tr"
    if ((cp < 128LL)) {
#line 103 "src/lexer.tr"
        StringBuilder_append_char(sb, cp);
    } else if ((cp < 2048LL)) {
#line 105 "src/lexer.tr"
        StringBuilder_append_char(sb, (192LL | (cp >> 6LL)));
#line 106 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | (cp & 63LL)));
    } else if ((cp < 65536LL)) {
#line 108 "src/lexer.tr"
        StringBuilder_append_char(sb, (224LL | (cp >> 12LL)));
#line 109 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | ((cp >> 6LL) & 63LL)));
#line 110 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | (cp & 63LL)));
    } else {
#line 112 "src/lexer.tr"
        StringBuilder_append_char(sb, (240LL | (cp >> 18LL)));
#line 113 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | ((cp >> 12LL) & 63LL)));
#line 114 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | ((cp >> 6LL) & 63LL)));
#line 115 "src/lexer.tr"
        StringBuilder_append_char(sb, (128LL | (cp & 63LL)));
    }
}

__attribute__((hot)) bool char_is_digit(long long c) {
#line 118 "src/lexer.tr"
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool char_is_alpha(long long c) {
#line 121 "src/lexer.tr"
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool char_is_alnum(long long c) {
#line 124 "src/lexer.tr"
    return (char_is_alpha(c) || char_is_digit(c));
}

__attribute__((hot)) bool char_is_space(long long c) {
#line 127 "src/lexer.tr"
    return ((c == 32LL) || (c == 9LL));
}

__attribute__((hot)) bool char_is_newline(long long c) {
#line 130 "src/lexer.tr"
    return ((c == 10LL) || (c == 13LL));
}

__attribute__((hot)) bool char_is_hex(long long c) {
#line 133 "src/lexer.tr"
    return ((char_is_digit(c) || ((c >= 65LL) && (c <= 70LL))) || ((c >= 97LL) && (c <= 102LL)));
}

__attribute__((hot)) bool _peek_next_line_dot(char* src, long long pos, long long src_len) {
#line 905 "src/lexer.tr"
    long long i = pos;
#line 906 "src/lexer.tr"
    while ((i < src_len)) {
#line 907 "src/lexer.tr"
        long long c = ((long long)((*(src + i))));
#line 908 "src/lexer.tr"
        if (((c == 32LL) || (c == 9LL))) {
#line 909 "src/lexer.tr"
            i = (i + 1LL);
        } else if ((c == 35LL)) {
#line 911 "src/lexer.tr"
            while (((i < src_len) && (((long long)((*(src + i)))) != 10LL))) {
#line 912 "src/lexer.tr"
                i = (i + 1LL);
            }
        } else if ((c == 10LL)) {
#line 914 "src/lexer.tr"
            i = (i + 1LL);
        } else if ((c == 13LL)) {
#line 916 "src/lexer.tr"
            i = (i + 1LL);
#line 917 "src/lexer.tr"
            if (((i < src_len) && (((long long)((*(src + i)))) == 10LL))) {
#line 918 "src/lexer.tr"
                i = (i + 1LL);
            }
        } else {
#line 920 "src/lexer.tr"
            return (c == 46LL);
        }
    }
#line 921 "src/lexer.tr"
    return false;
}

__attribute__((hot)) bool _last_tok_is_dot(List_Token* tokens) {
#line 933 "src/lexer.tr"
    if ((tokens->len == 0LL)) {
#line 933 "src/lexer.tr"
        return false;
    }
#line 934 "src/lexer.tr"
    __auto_type _t26 = List_Token_get(tokens, (tokens->len - 1LL));
    if (_t26.tag == Token_Dot) {
#line 939 "src/lexer.tr"
        if ((tokens->len < 2LL)) {
#line 939 "src/lexer.tr"
            return false;
        }
#line 940 "src/lexer.tr"
        __auto_type _t27 = List_Token_get(tokens, (tokens->len - 2LL));
        if (_t27.tag == Token_IntLit) {
            __auto_type _ = _t27.data.IntLit.val;
            return false;
        } else if (_t27.tag == Token_FloatLit) {
            __auto_type _ = _t27.data.FloatLit.val;
            return false;
        } else if (1) {
            __auto_type _ = _t27;
            return true;
        }
    } else if (1) {
        __auto_type _ = _t26;
        return false;
    }
}

