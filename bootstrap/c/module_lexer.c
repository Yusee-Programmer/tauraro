#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(char* source) {
    /* pass */
    Lexer* lx = ((Lexer*)_tr_checked_alloc(sizeof(Lexer)));
    /* pass */
    /* unsafe block */
    /* pass */
    lx->src = ((char*)(source));
    /* pass */
    long long src_len = 0LL;
    /* pass */
    /* unsafe block */
    /* pass */
    char* p = lx->src;
    /* pass */
    while ((((long long)((*(p + src_len)))) != 0LL)) {
        /* pass */
        src_len = (src_len + 1LL);
    }
    /* pass */
    lx->len = src_len;
    /* pass */
    lx->pos = 0LL;
    /* pass */
    lx->line = 1LL;
    /* pass */
    lx->indent_stack = (void*)List_i64_new();
    /* pass */
    List_i64_append(lx->indent_stack, 0LL);
    /* pass */
    lx->pending_dedents = 0LL;
    /* pass */
    lx->token_lines = (void*)List_i64_new();
    /* pass */
    return lx;
}

__attribute__((hot)) long long Lexer_peek(Lexer* self) {
    /* pass */
    if ((self->pos >= self->len)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    /* unsafe block */
    /* pass */
    return ((long long)((*(self->src + self->pos))));
}

__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset) {
    /* pass */
    long long p = (self->pos + offset);
    /* pass */
    if ((p >= self->len)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    /* unsafe block */
    /* pass */
    return ((long long)((*(self->src + p))));
}

__attribute__((hot)) long long Lexer_advance(Lexer* self) {
    /* pass */
    long long c = Lexer_peek(self);
    /* pass */
    if ((c == 10LL)) {
        /* pass */
        self->line = (self->line + 1LL);
    }
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    return c;
}

__attribute__((hot)) bool Lexer_at_end(Lexer* self) {
    /* pass */
    return (self->pos >= self->len);
}

__attribute__((hot)) void Lexer_skip_spaces(Lexer* self) {
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        long long c = Lexer_peek(self);
        /* pass */
        if (char_is_space(c)) {
            /* pass */
            Lexer_advance(self);
        } else {
            /* pass */
            break;
        }
    }
}

__attribute__((hot)) void Lexer_skip_comment(Lexer* self) {
    /* pass */
    while (((!Lexer_at_end(self)) && (!char_is_newline(Lexer_peek(self))))) {
        /* pass */
        Lexer_advance(self);
    }
}

__attribute__((hot)) Token Lexer_read_int(Lexer* self) {
    /* pass */
    if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 120LL) || (Lexer_peek_at(self, 1LL) == 88LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((char_is_hex(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                unsigned long long digit = ((unsigned long long)(0LL));
                /* pass */
                if (((c >= 48LL) && (c <= 57LL))) {
                    /* pass */
                    digit = ((unsigned long long)((c - 48LL)));
                } else if (((c >= 65LL) && (c <= 70LL))) {
                    /* pass */
                    digit = ((unsigned long long)(((c - 65LL) + 10LL)));
                } else {
                    /* pass */
                    digit = ((unsigned long long)(((c - 97LL) + 10LL)));
                }
                /* pass */
                val = ((val * ((unsigned long long)(16LL))) + digit);
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 98LL) || (Lexer_peek_at(self, 1LL) == 66LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((((Lexer_peek(self) == 48LL) || (Lexer_peek(self) == 49LL)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(2LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 111LL) || (Lexer_peek_at(self, 1LL) == 79LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((((Lexer_peek(self) >= 48LL) && (Lexer_peek(self) <= 55LL)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(8LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else {
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(10LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        if (((Lexer_peek(self) == 46LL) && char_is_digit(Lexer_peek_at(self, 1LL)))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            double frac = 0;
            /* pass */
            double divisor = 10;
            /* pass */
            while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
                /* pass */
                long long c = Lexer_advance(self);
                /* pass */
                if ((c != 95LL)) {
                    /* pass */
                    frac = (frac + ((double)((c - 48LL)) / divisor));
                    /* pass */
                    divisor = (divisor * 10);
                }
            }
            /* pass */
            double result = ((double)(((long long)(val))) + frac);
            /* pass */
            if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                bool exp_neg = false;
                /* pass */
                if ((Lexer_peek(self) == 45LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    exp_neg = true;
                } else if ((Lexer_peek(self) == 43LL)) {
                    /* pass */
                    Lexer_advance(self);
                }
                /* pass */
                long long exp = 0LL;
                /* pass */
                while (char_is_digit(Lexer_peek(self))) {
                    /* pass */
                    exp = ((exp * 10LL) + (Lexer_advance(self) - 48LL));
                }
                /* pass */
                long long ei = 0LL;
                /* pass */
                if (exp_neg) {
                    /* pass */
                    while ((ei < exp)) {
                        /* pass */
                        result = (result / 10);
                        /* pass */
                        ei = (ei + 1LL);
                    }
                } else {
                    /* pass */
                    while ((ei < exp)) {
                        /* pass */
                        result = (result * 10);
                        /* pass */
                        ei = (ei + 1LL);
                    }
                }
            }
            /* pass */
            return Token_ctor_FloatLit(result);
        }
        /* pass */
        if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            bool exp_neg = false;
            /* pass */
            if ((Lexer_peek(self) == 45LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                exp_neg = true;
            } else if ((Lexer_peek(self) == 43LL)) {
                /* pass */
                Lexer_advance(self);
            }
            /* pass */
            long long exp = 0LL;
            /* pass */
            while (char_is_digit(Lexer_peek(self))) {
                /* pass */
                exp = ((exp * 10LL) + (Lexer_advance(self) - 48LL));
            }
            /* pass */
            __auto_type result = (double)(((long long)(val)));
            /* pass */
            long long ei = 0LL;
            /* pass */
            if (exp_neg) {
                /* pass */
                while ((ei < exp)) {
                    /* pass */
                    result = (result / 10);
                    /* pass */
                    ei = (ei + 1LL);
                }
            } else {
                /* pass */
                while ((ei < exp)) {
                    /* pass */
                    result = (result * 10);
                    /* pass */
                    ei = (ei + 1LL);
                }
            }
            /* pass */
            return Token_ctor_FloatLit(result);
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    }
}

__attribute__((hot)) Token Lexer_read_triple_string(Lexer* self, long long quote) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(128LL);
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        if ((((Lexer_peek(self) == quote) && (Lexer_peek_at(self, 1LL) == quote)) && (Lexer_peek_at(self, 2LL) == quote))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            return Token_ctor_TripleStrLit(StringObj_as_str(StringBuilder_to_string(sb)));
        }
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    return Token_ctor_TripleStrLit(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            long long esc = Lexer_advance(self);
            /* pass */
            if ((esc == 110LL)) {
                /* pass */
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
                /* pass */
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
                /* pass */
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
                /* pass */
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
                /* pass */
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
                /* pass */
                StringBuilder_append_char(sb, 0LL);
            } else {
                /* pass */
                StringBuilder_append_char(sb, esc);
            }
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_StrLit(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) Token Lexer_read_char(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long val = 0LL;
    /* pass */
    if ((Lexer_peek(self) == 92LL)) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        long long esc = Lexer_advance(self);
        /* pass */
        if ((esc == 110LL)) {
            /* pass */
            val = 10LL;
        } else if ((esc == 116LL)) {
            /* pass */
            val = 9LL;
        } else if ((esc == 114LL)) {
            /* pass */
            val = 13LL;
        } else if ((esc == 92LL)) {
            /* pass */
            val = 92LL;
        } else if ((esc == 39LL)) {
            /* pass */
            val = 39LL;
        } else if ((esc == 48LL)) {
            /* pass */
            val = 0LL;
        } else {
            /* pass */
            val = esc;
        }
    } else {
        /* pass */
        val = Lexer_advance(self);
    }
    /* pass */
    if ((Lexer_peek(self) == 39LL)) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_CharLit(val);
}

__attribute__((hot)) Token Lexer_read_fstring(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        StringBuilder_append_char(sb, c);
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_FStrLit(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_RawStrLit(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            long long esc = Lexer_advance(self);
            /* pass */
            if ((esc == 110LL)) {
                /* pass */
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
                /* pass */
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
                /* pass */
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
                /* pass */
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
                /* pass */
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
                /* pass */
                StringBuilder_append_char(sb, 0LL);
            } else {
                /* pass */
                StringBuilder_append_char(sb, esc);
            }
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_ByteStrLit(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) Token Lexer_read_ident(Lexer* self) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (char_is_alnum(Lexer_peek(self))) {
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    return keyword_to_token(StringObj_as_str(StringBuilder_to_string(sb)));
}

__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self) {
    /* pass */
    if (((self->pos == 0LL) && (self->len >= 3LL))) {
        /* pass */
        if ((((Lexer_peek_at(self, 0LL) == 239LL) && (Lexer_peek_at(self, 1LL) == 187LL)) && (Lexer_peek_at(self, 2LL) == 191LL))) {
            /* pass */
            self->pos = (self->pos + 3LL);
        }
    }
    /* pass */
    List_Token* tokens = (void*)List_Token_new();
    /* pass */
    bool at_line_start = true;
    /* pass */
    long long nesting = 0LL;
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        if ((self->pending_dedents > 0LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Dedent());
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            self->pending_dedents = (self->pending_dedents - 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if (at_line_start) {
            /* pass */
            at_line_start = false;
            /* pass */
            long long indent = 0LL;
            /* pass */
            while ((!Lexer_at_end(self))) {
                /* pass */
                long long c_peek = Lexer_peek(self);
                /* pass */
                if ((c_peek == 32LL)) {
                    /* pass */
                    indent = (indent + 1LL);
                    /* pass */
                    Lexer_advance(self);
                } else if ((c_peek == 9LL)) {
                    /* pass */
                    indent = (indent + 4LL);
                    /* pass */
                    Lexer_advance(self);
                } else {
                    /* pass */
                    break;
                }
            }
            /* pass */
            if (Lexer_at_end(self)) {
                /* pass */
                break;
            }
            /* pass */
            long long next_c = Lexer_peek(self);
            /* pass */
            if ((char_is_newline(next_c) || (next_c == 35LL))) {
                /* pass */
                if ((next_c == 35LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    Lexer_skip_comment(self);
                } else {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    if (((next_c == 13LL) && (Lexer_peek(self) == 10LL))) {
                        /* pass */
                        Lexer_advance(self);
                    }
                }
                /* pass */
                at_line_start = true;
                /* pass */
                continue;
            }
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                if ((next_c != 46LL)) {
                    /* pass */
                    long long cur_indent = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
                    /* pass */
                    if ((indent > cur_indent)) {
                        /* pass */
                        List_i64_append(self->indent_stack, indent);
                        /* pass */
                        List_Token_append(tokens, Token_make_Indent());
                        /* pass */
                        List_i64_append(self->token_lines, self->line);
                    } else if ((indent < cur_indent)) {
                        /* pass */
                        while ((self->indent_stack->len > 1LL)) {
                            /* pass */
                            long long top = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
                            /* pass */
                            if ((top <= indent)) {
                                /* pass */
                                break;
                            }
                            /* pass */
                            self->indent_stack->len = (self->indent_stack->len - 1LL);
                            /* pass */
                            List_Token_append(tokens, Token_make_Dedent());
                            /* pass */
                            List_i64_append(self->token_lines, self->line);
                        }
                    }
                }
            }
        }
        /* pass */
        long long c = Lexer_peek(self);
        /* pass */
        if (char_is_space(c)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_newline(c)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            if (((c == 13LL) && (Lexer_peek(self) == 10LL))) {
                /* pass */
                Lexer_advance(self);
            }
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
                    /* pass */
                    List_Token_append(tokens, Token_make_Newline());
                    /* pass */
                    List_i64_append(self->token_lines, self->line);
                }
                /* pass */
                at_line_start = true;
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 35LL)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_skip_comment(self);
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
                    /* pass */
                    List_Token_append(tokens, Token_make_Newline());
                    /* pass */
                    List_i64_append(self->token_lines, self->line);
                }
                /* pass */
                at_line_start = true;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_digit(c)) {
            /* pass */
            List_Token_append(tokens, Lexer_read_int(self));
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 34LL) || (c == 39LL))) {
            /* pass */
            if ((((c == 34LL) && (Lexer_peek_at(self, 1LL) == 34LL)) && (Lexer_peek_at(self, 2LL) == 34LL))) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                Lexer_advance(self);
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Lexer_read_triple_string(self, 34LL));
                /* pass */
                List_i64_append(self->token_lines, self->line);
            } else if (((c == 39LL) && ((Lexer_peek_at(self, 2LL) == 39LL) || (Lexer_peek_at(self, 1LL) == 92LL)))) {
                /* pass */
                List_Token_append(tokens, Lexer_read_char(self));
                /* pass */
                List_i64_append(self->token_lines, self->line);
            } else {
                /* pass */
                List_Token_append(tokens, Lexer_read_string(self, c));
                /* pass */
                List_i64_append(self->token_lines, self->line);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 102LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_fstring(self));
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 114LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_raw_string(self));
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 98LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_byte_string(self));
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_alpha(c)) {
            /* pass */
            List_Token_append(tokens, Lexer_read_ident(self));
            /* pass */
            List_i64_append(self->token_lines, self->line);
            /* pass */
            continue;
        }
        /* pass */
        Lexer_advance(self);
        /* pass */
        if ((c == 61LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_EqEq());
            } else if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_FatArrow());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Eq());
            }
        } else if ((c == 33LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_NotEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_ctor_Error("!"));
            }
        } else if ((c == 60LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_LtEq());
            } else if ((nc == 60LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_LtLtEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_LtLt());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Lt());
            }
        } else if ((c == 62LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_GtEq());
            } else if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_GtGtEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_GtGt());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Gt());
            }
        } else if ((c == 43LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PlusEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Plus());
            }
        } else if ((c == 45LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_Arrow());
            } else if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_MinusEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Minus());
            }
        } else if ((c == 42LL)) {
            /* pass */
            if ((Lexer_peek(self) == 42LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_StarStarEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_StarStar());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_StarEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Star());
            }
        } else if ((c == 47LL)) {
            /* pass */
            if ((Lexer_peek(self) == 47LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_FloorDivEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_FloorDiv());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_SlashEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Slash());
            }
        } else if ((c == 37LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PercentEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Percent());
            }
        } else if ((c == 46LL)) {
            /* pass */
            if ((Lexer_peek(self) == 46LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 46LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDotDot());
                } else if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDotEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDot());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Dot());
            }
        } else if ((c == 40LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LParen());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 41LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RParen());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 91LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LBracket());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 93LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RBracket());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 123LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LBrace());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 125LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RBrace());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 58LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Colon());
        } else if ((c == 44LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Comma());
        } else if ((c == 59LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Semicolon());
        } else if ((c == 64LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_At());
        } else if ((c == 63LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Question());
        } else if ((c == 126LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Tilde());
        } else if ((c == 38LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_AmpEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Amp());
            }
        } else if ((c == 124LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PipeEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Pipe());
            }
        } else if ((c == 94LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_CaretEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Caret());
            }
        } else {
            /* pass */
            Lexer_advance(self);
            /* pass */
            List_Token_append(tokens, Token_ctor_Error("?"));
        }
        /* pass */
        List_i64_append(self->token_lines, self->line);
    }
    /* pass */
    while ((self->indent_stack->len > 1LL)) {
        /* pass */
        self->indent_stack->len = (self->indent_stack->len - 1LL);
        /* pass */
        List_Token_append(tokens, Token_make_Dedent());
        /* pass */
        List_i64_append(self->token_lines, self->line);
    }
    /* pass */
    List_Token_append(tokens, Token_make_Eof());
    /* pass */
    List_i64_append(self->token_lines, self->line);
    /* pass */
    return tokens;
}

__attribute__((hot)) Token keyword_to_token(char* s) {
    /* pass */
    if (((strcmp((char*)s, (char*)"def") == 0) || (strcmp((char*)s, (char*)"aiki") == 0))) {
        /* pass */
        return Token_make_KwDef();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"class") == 0) || (strcmp((char*)s, (char*)"aji") == 0))) {
        /* pass */
        return Token_make_KwClass();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"enum") == 0)) {
        /* pass */
        return Token_make_KwEnum();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"interface") == 0)) {
        /* pass */
        return Token_make_KwInterface();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"extend") == 0)) {
        /* pass */
        return Token_make_KwExtend();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"struct") == 0) || (strcmp((char*)s, (char*)"tsari") == 0))) {
        /* pass */
        return Token_make_KwStruct();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"if") == 0) || (strcmp((char*)s, (char*)"idan") == 0))) {
        /* pass */
        return Token_make_KwIf();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"elif") == 0) || (strcmp((char*)s, (char*)"koidan") == 0))) {
        /* pass */
        return Token_make_KwElif();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"else") == 0) || (strcmp((char*)s, (char*)"sai") == 0))) {
        /* pass */
        return Token_make_KwElse();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"for") == 0) || (strcmp((char*)s, (char*)"ga") == 0))) {
        /* pass */
        return Token_make_KwFor();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"while") == 0) || (strcmp((char*)s, (char*)"yayinda") == 0))) {
        /* pass */
        return Token_make_KwWhile();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"return") == 0) || (strcmp((char*)s, (char*)"dawo") == 0))) {
        /* pass */
        return Token_make_KwReturn();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"break") == 0) || (strcmp((char*)s, (char*)"tsaya") == 0))) {
        /* pass */
        return Token_make_KwBreak();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"continue") == 0) || (strcmp((char*)s, (char*)"ci_gaba") == 0))) {
        /* pass */
        return Token_make_KwContinue();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"pass") == 0) || (strcmp((char*)s, (char*)"wuce") == 0))) {
        /* pass */
        return Token_make_KwPass();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"match") == 0) || (strcmp((char*)s, (char*)"duba") == 0))) {
        /* pass */
        return Token_make_KwMatch();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"case") == 0) || (strcmp((char*)s, (char*)"hali") == 0))) {
        /* pass */
        return Token_make_KwCase();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"try") == 0) || (strcmp((char*)s, (char*)"gwada") == 0))) {
        /* pass */
        return Token_make_KwTry();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"except") == 0) || (strcmp((char*)s, (char*)"kama") == 0))) {
        /* pass */
        return Token_make_KwExcept();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"finally") == 0) || (strcmp((char*)s, (char*)"karshe") == 0))) {
        /* pass */
        return Token_make_KwFinally();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"raise") == 0) || (strcmp((char*)s, (char*)"jefa") == 0))) {
        /* pass */
        return Token_make_KwRaise();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"assert") == 0) || (strcmp((char*)s, (char*)"tabbatar") == 0))) {
        /* pass */
        return Token_make_KwAssert();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"with") == 0) || (strcmp((char*)s, (char*)"tare") == 0))) {
        /* pass */
        return Token_make_KwWith();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"defer") == 0)) {
        /* pass */
        return Token_make_KwDefer();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"asm") == 0)) {
        /* pass */
        return Token_make_KwAsm();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"taskgroup") == 0) || (strcmp((char*)s, (char*)"task_group") == 0))) {
        /* pass */
        return Token_make_KwTaskGroup();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"gpu") == 0)) {
        /* pass */
        return Token_make_KwGpu();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"implements") == 0)) {
        /* pass */
        return Token_make_KwImplements();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"import") == 0)) {
        /* pass */
        return Token_make_KwImport();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"from") == 0)) {
        /* pass */
        return Token_make_KwFrom();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"as") == 0)) {
        /* pass */
        return Token_make_KwAs();
    }
    /* pass */
    if ((((strcmp((char*)s, (char*)"in") == 0) || (strcmp((char*)s, (char*)"ciki") == 0)) || (strcmp((char*)s, (char*)"a_cikin") == 0))) {
        /* pass */
        return Token_make_KwIn();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"mut") == 0)) {
        /* pass */
        return Token_make_KwMut();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"shared") == 0)) {
        /* pass */
        return Token_make_KwShared();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"extern") == 0)) {
        /* pass */
        return Token_make_KwExtern();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"unsafe") == 0)) {
        /* pass */
        return Token_make_KwUnsafe();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"spawn") == 0)) {
        /* pass */
        return Token_make_KwSpawn();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"throws") == 0)) {
        /* pass */
        return Token_make_KwThrows();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"extends") == 0)) {
        /* pass */
        return Token_make_KwExtends();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"async") == 0) || (strcmp((char*)s, (char*)"ba_jira") == 0))) {
        /* pass */
        return Token_make_KwAsync();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"await") == 0) || (strcmp((char*)s, (char*)"jira") == 0))) {
        /* pass */
        return Token_make_KwAwait();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"yield") == 0) || (strcmp((char*)s, (char*)"bayar") == 0))) {
        /* pass */
        return Token_make_KwYield();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"pub") == 0) || (strcmp((char*)s, (char*)"fito") == 0))) {
        /* pass */
        return Token_make_KwPub();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"static") == 0)) {
        /* pass */
        return Token_make_KwStatic();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"const") == 0)) {
        /* pass */
        return Token_make_KwConst();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"actor") == 0)) {
        /* pass */
        return Token_make_KwActor();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"super") == 0)) {
        /* pass */
        return Token_make_KwSuper();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"export") == 0)) {
        /* pass */
        return Token_make_KwExport();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"lambda") == 0) || (strcmp((char*)s, (char*)"dan_aiki") == 0))) {
        /* pass */
        return Token_make_KwLambda();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"decorator") == 0)) {
        /* pass */
        return Token_make_KwDecorator();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"sizeof") == 0)) {
        /* pass */
        return Token_make_KwSizeOf();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"is") == 0)) {
        /* pass */
        return Token_make_KwIs();
    }
    /* pass */
    if ((((strcmp((char*)s, (char*)"true") == 0) || (strcmp((char*)s, (char*)"gaskiya") == 0)) || (strcmp((char*)s, (char*)"True") == 0))) {
        /* pass */
        return Token_make_KwTrue();
    }
    /* pass */
    if ((((strcmp((char*)s, (char*)"false") == 0) || (strcmp((char*)s, (char*)"karya") == 0)) || (strcmp((char*)s, (char*)"False") == 0))) {
        /* pass */
        return Token_make_KwFalse();
    }
    /* pass */
    if ((((strcmp((char*)s, (char*)"none") == 0) || (strcmp((char*)s, (char*)"babu") == 0)) || (strcmp((char*)s, (char*)"None") == 0))) {
        /* pass */
        return Token_make_KwNone();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"and") == 0) || (strcmp((char*)s, (char*)"da") == 0))) {
        /* pass */
        return Token_make_KwAnd();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"or") == 0) || (strcmp((char*)s, (char*)"ko") == 0))) {
        /* pass */
        return Token_make_KwOr();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"not") == 0) || (strcmp((char*)s, (char*)"ba") == 0))) {
        /* pass */
        return Token_make_KwNot();
    }
    /* pass */
    if ((((strcmp((char*)s, (char*)"int") == 0) || (strcmp((char*)s, (char*)"lamba") == 0)) || (strcmp((char*)s, (char*)"i64") == 0))) {
        /* pass */
        return Token_make_KwInt();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"float") == 0) || (strcmp((char*)s, (char*)"f64") == 0))) {
        /* pass */
        return Token_make_KwFloat();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"bool") == 0)) {
        /* pass */
        return Token_make_KwBool();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"char") == 0)) {
        /* pass */
        return Token_make_KwChar();
    }
    /* pass */
    if (((strcmp((char*)s, (char*)"str") == 0) || (strcmp((char*)s, (char*)"zuwa_rubutu") == 0))) {
        /* pass */
        return Token_make_KwStr();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"String") == 0)) {
        /* pass */
        return Token_make_KwString();
    }
    /* pass */
    if ((strcmp((char*)s, (char*)"void") == 0)) {
        /* pass */
        return Token_make_KwVoid();
    }
    /* pass */
    return Token_ctor_Ident(s);
}

__attribute__((hot)) bool char_is_digit(long long c) {
    /* pass */
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool char_is_alpha(long long c) {
    /* pass */
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool char_is_alnum(long long c) {
    /* pass */
    return (char_is_alpha(c) || char_is_digit(c));
}

__attribute__((hot)) bool char_is_space(long long c) {
    /* pass */
    return ((c == 32LL) || (c == 9LL));
}

__attribute__((hot)) bool char_is_newline(long long c) {
    /* pass */
    return ((c == 10LL) || (c == 13LL));
}

__attribute__((hot)) bool char_is_hex(long long c) {
    /* pass */
    return ((char_is_digit(c) || ((c >= 65LL) && (c <= 70LL))) || ((c >= 97LL) && (c <= 102LL)));
}

__attribute__((hot)) bool _peek_next_line_dot(char* src, long long pos, long long src_len) {
    /* pass */
    long long i = pos;
    /* pass */
    while ((i < src_len)) {
        /* pass */
        long long c = ((long long)((*(src + i))));
        /* pass */
        if (((c == 32LL) || (c == 9LL))) {
            /* pass */
            i = (i + 1LL);
        } else if ((c == 35LL)) {
            /* pass */
            while (((i < src_len) && (((long long)((*(src + i)))) != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
        } else if ((c == 10LL)) {
            /* pass */
            i = (i + 1LL);
        } else if ((c == 13LL)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            if (((i < src_len) && (((long long)((*(src + i)))) == 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
        } else {
            /* pass */
            return (c == 46LL);
        }
    }
    /* pass */
    return false;
}

