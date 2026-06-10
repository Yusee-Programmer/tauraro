#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines) {
    /* pass */
    Parser* p = ((Parser*)_tr_checked_alloc(sizeof(Parser)));
    /* pass */
    p->tokens = tokens;
    /* pass */
    p->lines = lines;
    /* pass */
    p->pos = 0LL;
    /* pass */
    p->error_count = 0LL;
    /* pass */
    p->current_file = "";
    /* pass */
    return p;
}

__attribute__((hot)) Token Parser_peek(Parser* self) {
    /* pass */
    if ((self->pos < self->tokens->len)) {
        /* pass */
        return List_Token_get(self->tokens, self->pos);
    }
    /* pass */
    return Token_make_Eof();
}

__attribute__((hot)) Token Parser_advance(Parser* self) {
    /* pass */
    Token tok = Parser_peek(self);
    /* pass */
    if ((self->pos < self->tokens->len)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return tok;
}

__attribute__((hot)) void Parser_skip_newlines(Parser* self) {
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        __auto_type _t5 = Parser_peek(self);
        if (_t5.tag == Token_Newline) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            going = true;
        } else if (1) {
            __auto_type _ = _t5;
            /* pass */
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self) {
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        __auto_type _t6 = Parser_peek(self);
        if ((_t6.tag == Token_Newline || _t6.tag == Token_Indent || _t6.tag == Token_Dedent)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            going = true;
        } else if (1) {
            __auto_type _ = _t6;
            /* pass */
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_expect_newline(Parser* self) {
    /* pass */
    __auto_type _t7 = Parser_peek(self);
    if ((_t7.tag == Token_Newline || _t7.tag == Token_Semicolon)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t7;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) bool Parser_at_end(Parser* self) {
    /* pass */
    __auto_type _t8 = Parser_peek(self);
    if (_t8.tag == Token_Eof) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t8;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long Parser_cur_line(Parser* self) {
    /* pass */
    if ((self->pos < self->lines->len)) {
        /* pass */
        return List_i64_get(self->lines, self->pos);
    }
    /* pass */
    if ((self->lines->len > 0LL)) {
        /* pass */
        return List_i64_get(self->lines, (self->lines->len - 1LL));
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) char* Parser_consume_ident(Parser* self) {
    /* pass */
    __auto_type _t9 = Parser_peek(self);
    if (_t9.tag == Token_Ident) {
        __auto_type name = _t9.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return name;
    } else if (_t9.tag == Token_KwInt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "int";
    } else if (_t9.tag == Token_KwFloat) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "float";
    } else if (_t9.tag == Token_KwBool) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "bool";
    } else if ((_t9.tag == Token_KwStr || _t9.tag == Token_KwString)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "str";
    } else if (_t9.tag == Token_KwChar) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "char";
    } else if (_t9.tag == Token_KwVoid) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "void";
    } else if (_t9.tag == Token_KwExtend) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "extend";
    } else if (_t9.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "None";
    } else if (_t9.tag == Token_KwLambda) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "lambda";
    } else if (_t9.tag == Token_KwSpawn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "spawn";
    } else if (_t9.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "async";
    } else if (1) {
        __auto_type _ = _t9;
        /* pass */
        /* pass */
    }
    /* pass */
    return "";
}

__attribute__((hot)) char* Parser_consume_module_ident(Parser* self) {
    /* pass */
    __auto_type _t10 = Parser_peek(self);
    if (_t10.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "async";
    } else if (_t10.tag == Token_KwMatch) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "match";
    } else if (_t10.tag == Token_KwFor) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "for";
    } else if (_t10.tag == Token_KwIn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "in";
    } else if (_t10.tag == Token_KwIs) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return "is";
    } else if (1) {
        __auto_type _ = _t10;
        /* pass */
        return Parser_consume_ident(self);
    }
    /* pass */
    return "";
}

__attribute__((hot)) AstType* Parser_parse_type(Parser* self) {
    /* pass */
    char* name = "";
    /* pass */
    __auto_type _t11 = Parser_peek(self);
    if (_t11.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return AstType_init("void");
    } else if (_t11.tag == Token_LParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* tup_t = AstType_init("Tuple");
        /* pass */
        while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
            /* pass */
            AstType* elem_t = Parser_parse_type(self);
            /* pass */
            List_ptr_append(tup_t->args, box_asttype(elem_t));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        return tup_t;
    } else if (_t11.tag == Token_KwDef) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* dfn_t = AstType_init("def");
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                AstType* dpt = Parser_parse_type(self);
                /* pass */
                List_ptr_append(dfn_t->args, box_asttype(dpt));
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        AstType* dret = AstType_init("void");
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            dret = Parser_parse_type(self);
        }
        /* pass */
        List_ptr_append(dfn_t->args, box_asttype(dret));
        /* pass */
        return dfn_t;
    } else if (_t11.tag == Token_KwMut) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        char* mr_name = Parser_consume_ident(self);
        /* pass */
        if ((strcmp((char*)mr_name, (char*)"ref") == 0)) {
            /* pass */
            AstType* mr_inner = Parser_parse_type(self);
            /* pass */
            AstType* mr_t = AstType_init("mut_ref");
            /* pass */
            List_ptr_append(mr_t->args, box_asttype(mr_inner));
            /* pass */
            return mr_t;
        }
        /* pass */
        return AstType_init("void");
    } else if (1) {
        __auto_type _ = _t11;
        /* pass */
        name = Parser_consume_ident(self);
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"") == 0)) {
        /* pass */
        return AstType_init("void");
    }
    /* pass */
    if ((strcmp((char*)name, (char*)"ref") == 0)) {
        /* pass */
        AstType* ref_inner = Parser_parse_type(self);
        /* pass */
        AstType* ref_t = AstType_init("ref");
        /* pass */
        List_ptr_append(ref_t->args, box_asttype(ref_inner));
        /* pass */
        return ref_t;
    }
    /* pass */
    __auto_type _t12 = Parser_peek(self);
    if (_t12.tag == Token_LBracket) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* t = AstType_init(name);
        /* pass */
        bool going = true;
        /* pass */
        while (going) {
            /* pass */
            __auto_type _t13 = Parser_peek(self);
            if ((_t13.tag == Token_RBracket || _t13.tag == Token_Eof)) {
                /* pass */
                going = false;
            } else if (1) {
                __auto_type _ = _t13;
                /* pass */
                /* pass */
            }
            /* pass */
            if (going) {
                /* pass */
                __auto_type arg_t = Parser_parse_type(self);
                /* pass */
                List_ptr_append(t->args, box_asttype(arg_t));
                /* pass */
                __auto_type _t14 = Parser_peek(self);
                if (_t14.tag == Token_Comma) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t14;
                    /* pass */
                    going = false;
                }
            }
        }
        /* pass */
        __auto_type _t15 = Parser_peek(self);
        if (_t15.tag == Token_RBracket) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t15;
            /* pass */
            /* pass */
        }
        /* pass */
        return t;
    } else if (1) {
        __auto_type _ = _t12;
        /* pass */
        /* pass */
    }
    /* pass */
    return AstType_init(name);
}

__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self) {
    /* pass */
    List_ptr* pl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type _t16 = Parser_peek(self);
        if ((_t16.tag == Token_RParen || _t16.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t16;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            __auto_type item_name = Parser_consume_ident(self);
            /* pass */
            if ((strcmp((char*)item_name, (char*)"") != 0)) {
                /* pass */
                AstType** ty_ptr = (AstType**)(0LL);
                /* pass */
                __auto_type _t17 = Parser_peek(self);
                if (_t17.tag == Token_Colon) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    __auto_type ty = Parser_parse_type(self);
                    /* pass */
                    ty_ptr = box_asttype(ty);
                } else if (1) {
                    __auto_type _ = _t17;
                    /* pass */
                    /* pass */
                }
                /* pass */
                __auto_type p = Param_init(item_name, ty_ptr);
                /* pass */
                if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    if ((strcmp((char*)(*ty_ptr)->name, (char*)"ref") == 0)) {
                        /* pass */
                        p->is_ref = true;
                    }
                    /* pass */
                    if ((strcmp((char*)(*ty_ptr)->name, (char*)"mut_ref") == 0)) {
                        /* pass */
                        p->is_mut_ref = true;
                    }
                }
                /* pass */
                List_ptr_append(pl, p);
                /* pass */
                __auto_type _t18 = Parser_peek(self);
                if (_t18.tag == Token_Comma) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    Parser_skip_newlines_and_indent(self);
                } else if (1) {
                    __auto_type _ = _t18;
                    /* pass */
                    going = false;
                }
            } else {
                /* pass */
                going = false;
            }
        }
    }
    /* pass */
    return pl;
}

__attribute__((hot)) Block* Parser_parse_block(Parser* self) {
    /* pass */
    Block* b = Block_init();
    /* pass */
    bool is_inline = false;
    /* pass */
    __auto_type _t19 = Parser_peek(self);
    if (_t19.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (_t19.tag == Token_Newline) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
    } else if (1) {
        __auto_type _ = _t19;
        /* pass */
        is_inline = true;
    }
    /* pass */
    if (is_inline) {
        /* pass */
        Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
        /* pass */
        Block_push(b, Parser_parse_stmt(self));
        /* pass */
        bool semi_going = true;
        /* pass */
        while (semi_going) {
            /* pass */
            semi_going = false;
            /* pass */
            __auto_type _t20 = Parser_peek(self);
            if (_t20.tag == Token_Semicolon) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                __auto_type _t21 = Parser_peek(self);
                if ((_t21.tag == Token_Newline || _t21.tag == Token_Eof || _t21.tag == Token_Dedent)) {
                    /* pass */
                    /* pass */
                } else if (1) {
                    __auto_type _ = _t21;
                    /* pass */
                    Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
                    /* pass */
                    Block_push(b, Parser_parse_stmt(self));
                    /* pass */
                    semi_going = true;
                }
            } else if (1) {
                __auto_type _ = _t20;
                /* pass */
                if ((self->pos > 0LL)) {
                    /* pass */
                    __auto_type _t22 = List_Token_get(self->tokens, (self->pos - 1LL));
                    if (_t22.tag == Token_Semicolon) {
                        /* pass */
                        __auto_type _t23 = Parser_peek(self);
                        if ((_t23.tag == Token_Newline || _t23.tag == Token_Eof || _t23.tag == Token_Dedent)) {
                            /* pass */
                            /* pass */
                        } else if (1) {
                            __auto_type _ = _t23;
                            /* pass */
                            Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
                            /* pass */
                            Block_push(b, Parser_parse_stmt(self));
                            /* pass */
                            semi_going = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t22;
                        /* pass */
                        /* pass */
                    }
                }
            }
        }
        /* pass */
        return b;
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        __auto_type _t24 = Parser_peek(self);
        if ((_t24.tag == Token_Dedent || _t24.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t24;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t25 = Parser_peek(self);
            if ((_t25.tag == Token_Dedent || _t25.tag == Token_Eof)) {
                /* pass */
                going = false;
            } else if (1) {
                __auto_type _ = _t25;
                /* pass */
                /* pass */
            }
            /* pass */
            if (going) {
                /* pass */
                long long _sline = Parser_cur_line(self);
                /* pass */
                __auto_type s = Parser_parse_stmt(self);
                /* pass */
                Block_push(b, box_stmt(Stmt_ctor_SLine(_sline)));
                /* pass */
                Block_push(b, s);
            }
        }
    }
    /* pass */
    __auto_type _t26 = Parser_peek(self);
    if (_t26.tag == Token_Dedent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t26;
        /* pass */
        /* pass */
    }
    /* pass */
    return b;
}

__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self) {
    /* pass */
    __auto_type _t27 = Parser_peek(self);
    if (_t27.tag == Token_KwReturn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t28 = Parser_peek(self);
        if ((_t28.tag == Token_Newline || _t28.tag == Token_Dedent || _t28.tag == Token_Eof)) {
            /* pass */
            return box_stmt(Stmt_ctor_SReturn((Expr*)(0LL)));
        } else if (1) {
            __auto_type _ = _t28;
            /* pass */
            /* pass */
        }
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SReturn(e));
    } else if (_t27.tag == Token_KwPass) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (_t27.tag == Token_KwBreak) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_make_SBreak());
    } else if (_t27.tag == Token_KwContinue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_make_SContinue());
    } else if (_t27.tag == Token_KwRaise) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SRaise(e));
    } else if (_t27.tag == Token_KwAsm) {
        /* pass */
        return Parser_parse_asm_stmt(self);
    } else if (_t27.tag == Token_KwSpawn) {
        /* pass */
        return Parser_parse_spawn_stmt(self);
    } else if (_t27.tag == Token_KwTaskGroup) {
        /* pass */
        return Parser_parse_taskgroup_stmt(self);
    } else if (_t27.tag == Token_KwGpu) {
        /* pass */
        return Parser_parse_gpu_stmt(self);
    } else if (_t27.tag == Token_KwIf) {
        /* pass */
        return Parser_parse_if_stmt(self);
    } else if (_t27.tag == Token_KwWhile) {
        /* pass */
        return Parser_parse_while_stmt(self);
    } else if (_t27.tag == Token_KwFor) {
        /* pass */
        return Parser_parse_for_stmt(self);
    } else if (_t27.tag == Token_KwMatch) {
        /* pass */
        return Parser_parse_match_stmt(self);
    } else if (_t27.tag == Token_KwMut) {
        /* pass */
        return Parser_parse_let_stmt(self, true);
    } else if (_t27.tag == Token_KwShared) {
        /* pass */
        return Parser_parse_shared_let_stmt(self);
    } else if (_t27.tag == Token_KwConst) {
        /* pass */
        return Parser_parse_const_let_stmt(self);
    } else if (_t27.tag == Token_KwUnsafe) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t29 = Parser_peek(self);
        if (_t29.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t29;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type body = Parser_parse_block(self);
        /* pass */
        return box_stmt(Stmt_ctor_SUnsafe(body));
    } else if (_t27.tag == Token_KwDefer) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Stmt* deferred = Parser_parse_stmt(self);
        /* pass */
        return box_stmt(Stmt_ctor_SDefer(deferred));
    } else if (_t27.tag == Token_KwWith) {
        /* pass */
        return Parser_parse_with_stmt(self);
    } else if (_t27.tag == Token_KwTry) {
        /* pass */
        return Parser_parse_try_stmt(self);
    } else if (_t27.tag == Token_KwAssert) {
        /* pass */
        return Parser_parse_assert_stmt(self);
    } else if (_t27.tag == Token_KwFrom) {
        /* pass */
        Parser_parse_from_import(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (_t27.tag == Token_KwImport) {
        /* pass */
        Parser_parse_import(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (1) {
        __auto_type _ = _t27;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type _t30 = Parser_peek(self);
    if (_t30.tag == Token_Ident) {
        __auto_type cs_nm = _t30.data.Ident.name;
        /* pass */
        if ((strcmp((char*)cs_nm, (char*)"type") == 0)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            char* _ta_name = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType* _ta_ty = Parser_parse_type(self);
                /* pass */
                Parser_expect_newline(self);
                /* pass */
                return box_stmt(Stmt_make_SPass());
            }
        }
        /* pass */
        if ((strcmp((char*)cs_nm, (char*)"chan_select") == 0)) {
            /* pass */
            return Parser_parse_chan_select_stmt(self);
        }
    } else if (1) {
        __auto_type _ = _t30;
        /* pass */
    }
    /* pass */
    return Parser_parse_assign_or_expr_stmt(self);
}

__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t31 = Parser_peek(self);
    if (_t31.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t31;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type try_body = Parser_parse_block(self);
    /* pass */
    List_ptr* catches = (void*)List_ptr_new();
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwExcept().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType** err_type = (AstType**)(0LL);
        /* pass */
        char* err_name = "";
        /* pass */
        if ((Parser_peek(self).tag != Token_make_Colon().tag)) {
            /* pass */
            bool is_bare_var = false;
            /* pass */
            __auto_type _t32 = Parser_peek(self);
            if (_t32.tag == Token_Ident) {
                __auto_type _ = _t32.data.Ident.name;
                /* pass */
                long long save_pos = self->pos;
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                    /* pass */
                    is_bare_var = true;
                }
                /* pass */
                self->pos = save_pos;
            } else if (1) {
                __auto_type _ = _t32;
                /* pass */
                /* pass */
            }
            /* pass */
            if (is_bare_var) {
                /* pass */
                err_name = Parser_consume_ident(self);
            } else {
                /* pass */
                err_type = box_asttype(Parser_parse_type(self));
                /* pass */
                if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    err_name = Parser_consume_ident(self);
                }
            }
        }
        /* pass */
        __auto_type _t33 = Parser_peek(self);
        if (_t33.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t33;
            /* pass */
            /* pass */
        }
        /* pass */
        Block* body = Parser_parse_block(self);
        /* pass */
        Block** body_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
        /* pass */
        (*body_ptr = body);
        /* pass */
        CatchClause* c = CatchClause_init(err_name, body_ptr);
        /* pass */
        c->err_type = err_type;
        /* pass */
        CatchClause** c_ptr = ((CatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(CatchClause*)));
        /* pass */
        (*c_ptr = c);
        /* pass */
        List_ptr_append(catches, c_ptr);
    }
    /* pass */
    Block* finally_b = Block_init();
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFinally().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t34 = Parser_peek(self);
        if (_t34.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t34;
            /* pass */
            /* pass */
        }
        /* pass */
        finally_b = Parser_parse_block(self);
    }
    /* pass */
    return box_stmt(Stmt_ctor_STry(try_body, catches, finally_b));
}

__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Expr* msg = (Expr*)(0LL);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        msg = Parser_parse_expr(self);
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SAssert(cond, msg));
}

__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    List_ptr* items = (void*)List_ptr_new();
    /* pass */
    List_str* aliases = (void*)List_str_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Expr* item_e = Parser_parse_expr(self);
        /* pass */
        char* alias = "";
        /* pass */
        if ((((unsigned long long)(item_e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t35 = (*item_e);
            if (_t35.tag == Expr_ECast) {
                __auto_type cast_inner = _t35.data.ECast.expr;
__auto_type cast_ty_ptr = _t35.data.ECast.ty;
                /* pass */
                if ((((unsigned long long)(cast_ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    alias = (*cast_ty_ptr)->name;
                }
                /* pass */
                item_e = cast_inner;
            } else if (1) {
                __auto_type _ = _t35;
                /* pass */
            }
        }
        /* pass */
        List_ptr_append(items, item_e);
        /* pass */
        List_str_append(aliases, alias);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else {
            /* pass */
            going = false;
        }
    }
    /* pass */
    __auto_type _t36 = Parser_peek(self);
    if (_t36.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t36;
        /* pass */
        /* pass */
    }
    /* pass */
    Block* body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SWith(items, aliases, body));
}

__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t37 = Parser_peek(self);
    if (_t37.tag == Token_LParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t37;
        /* pass */
        /* pass */
    }
    /* pass */
    List_str* parts = (void*)List_str_new();
    /* pass */
    bool reading = true;
    /* pass */
    while (reading) {
        /* pass */
        __auto_type _t38 = Parser_peek(self);
        if (_t38.tag == Token_StrLit) {
            __auto_type s = _t38.data.StrLit.val;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            List_str_append(parts, s);
            /* pass */
            __auto_type _t39 = Parser_peek(self);
            if (_t39.tag == Token_Comma) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t39;
                reading = false;
            }
        } else if (1) {
            __auto_type _ = _t38;
            reading = false;
        }
    }
    /* pass */
    __auto_type _t40 = Parser_peek(self);
    if (_t40.tag == Token_RParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t40;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    char* code = "";
    /* pass */
    char* outs = "";
    /* pass */
    char* ins = "";
    /* pass */
    char* clob = "";
    /* pass */
    if ((parts->len > 0LL)) {
        /* pass */
        code = List_str_get(parts, 0LL);
    }
    /* pass */
    if ((parts->len > 1LL)) {
        /* pass */
        outs = List_str_get(parts, 1LL);
    }
    /* pass */
    if ((parts->len > 2LL)) {
        /* pass */
        ins = List_str_get(parts, 2LL);
    }
    /* pass */
    if ((parts->len > 3LL)) {
        /* pass */
        clob = List_str_get(parts, 3LL);
    }
    /* pass */
    return box_stmt(Stmt_ctor_SAsm(code, outs, ins, clob));
}

__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type e = Parser_parse_expr(self);
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SSpawn(e));
}

__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t41 = Parser_peek(self);
    if (_t41.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t41;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_STaskGroup(body));
}

__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Indent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    List_ptr* cases = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        __auto_type _t42 = Parser_peek(self);
        if (_t42.tag == Token_KwCase) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            long long arm_kind = (-1LL);
            /* pass */
            char* var_nm = "";
            /* pass */
            Expr* chan_e = (Expr*)(0LL);
            /* pass */
            Expr* val_e = (Expr*)(0LL);
            /* pass */
            Expr* timeout_e = (Expr*)(0LL);
            /* pass */
            __auto_type _t43 = Parser_peek(self);
            if (_t43.tag == Token_Ident) {
                __auto_type arm_nm = _t43.data.Ident.name;
                /* pass */
                if ((strcmp((char*)arm_nm, (char*)"default") == 0)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    arm_kind = 3LL;
                } else if ((strcmp((char*)arm_nm, (char*)"timeout") == 0)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    timeout_e = Parser_parse_expr(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    arm_kind = 2LL;
                } else {
                    /* pass */
                    long long saved = self->pos;
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    __auto_type _t44 = Parser_peek(self);
                    if (_t44.tag == Token_Eq) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                        /* pass */
                        chan_e = Parser_parse_expr(self);
                        /* pass */
                        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                            /* pass */
                            self->pos = (self->pos + 1LL);
                        }
                        /* pass */
                        var_nm = arm_nm;
                        /* pass */
                        arm_kind = 0LL;
                    } else if (1) {
                        __auto_type _ = _t44;
                        /* pass */
                        self->pos = saved;
                        /* pass */
                        chan_e = Parser_parse_expr(self);
                        /* pass */
                        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                            /* pass */
                            self->pos = (self->pos + 1LL);
                        }
                        /* pass */
                        arm_kind = 1LL;
                    }
                }
            } else if (1) {
                __auto_type _ = _t43;
                /* pass */
                chan_e = Parser_parse_expr(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                arm_kind = 1LL;
            }
            /* pass */
            Block* arm_body = Parser_parse_block(self);
            /* pass */
            ChanSelectArm** arm_p = ((ChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(ChanSelectArm*)));
            /* pass */
            if ((arm_kind == 0LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_recv(chan_e, var_nm, arm_body));
            } else if ((arm_kind == 1LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_send(chan_e, (Expr*)(0LL), arm_body));
            } else if ((arm_kind == 2LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_timeout(timeout_e, arm_body));
            } else {
                /* pass */
                (*arm_p = ChanSelectArm_init_default(arm_body));
            }
            /* pass */
            List_ptr_append(cases, arm_p);
        } else if ((_t42.tag == Token_Dedent || _t42.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t42.tag == Token_Newline) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t42;
            /* pass */
            going = false;
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_stmt(Stmt_ctor_SChanSelect(cases));
}

__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t45 = Parser_peek(self);
    if (_t45.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t45;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SGpuBlock(body));
}

__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut) {
    /* pass */
    if (is_mut) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        List_str* names = (void*)List_str_new();
        /* pass */
        List_str_append(names, name);
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            List_str_append(names, Parser_consume_ident(self));
        }
        /* pass */
        __auto_type _t46 = Parser_peek(self);
        if (_t46.tag == Token_Eq) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t46;
            /* pass */
            /* pass */
        }
        /* pass */
        Expr* val_ptr = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SMultiLet(names, is_mut, val_ptr));
    }
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t47 = Parser_peek(self);
    if (_t47.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t47;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t48 = Parser_peek(self);
    if (_t48.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t48;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), is_mut, false, false, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t49 = Parser_peek(self);
    if (_t49.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t49;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t50 = Parser_peek(self);
    if (_t50.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t50;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Shared(), true, false, true, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t51 = Parser_peek(self);
    if (_t51.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t51;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t52 = Parser_peek(self);
    if (_t52.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t52;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, true, false, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t53 = Parser_peek(self);
    if (_t53.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t53;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type then_b = Parser_parse_block(self);
    /* pass */
    List_ptr* elifs = (void*)List_ptr_new();
    /* pass */
    Block* else_b = Block_init();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        __auto_type _t54 = Parser_peek(self);
        if (_t54.tag == Token_KwElif) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type ec = Parser_parse_expr(self);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t55 = Parser_peek(self);
            if (_t55.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t55;
                /* pass */
                /* pass */
            }
            /* pass */
            __auto_type eb = Parser_parse_block(self);
            /* pass */
            Block** eb_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
            /* pass */
            (*eb_ptr = eb);
            /* pass */
            List_ptr_append(elifs, ElifClause_init(ec, eb_ptr));
            /* pass */
            going = true;
        } else if (_t54.tag == Token_KwElse) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t56 = Parser_peek(self);
            if (_t56.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t56;
                /* pass */
                /* pass */
            }
            /* pass */
            else_b = Parser_parse_block(self);
        } else if (1) {
            __auto_type _ = _t54;
            /* pass */
            /* pass */
        }
    }
    /* pass */
    return box_stmt(Stmt_ctor_SIf(cond, then_b, elifs, else_b));
}

__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t57 = Parser_peek(self);
    if (_t57.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t57;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SWhile(cond, body, (void*)List_ptr_new()));
}

__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type var = Parser_consume_ident(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        List_str* unpack_vars = (void*)List_str_new();
        /* pass */
        List_str_append(unpack_vars, var);
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            List_str_append(unpack_vars, Parser_consume_ident(self));
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwIn().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Expr* fu_iter = Parser_parse_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* fu_body = Parser_parse_block(self);
        /* pass */
        return box_stmt(Stmt_ctor_SForUnpack(unpack_vars, fu_iter, fu_body));
    }
    /* pass */
    __auto_type _t58 = Parser_peek(self);
    if (_t58.tag == Token_KwIn) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t58;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type iter_e = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t59 = Parser_peek(self);
    if (_t59.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t59;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SFor(var, iter_e, body, (void*)List_ptr_new()));
}

__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type subj = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t60 = Parser_peek(self);
    if (_t60.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t60;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t61 = Parser_peek(self);
    if (_t61.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t61;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* arms = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        __auto_type _t62 = Parser_peek(self);
        if ((_t62.tag == Token_Dedent || _t62.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t62;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            Pattern pat = Parser_parse_pattern(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
                /* pass */
                List_Pattern* or_pats = (void*)List_Pattern_new();
                /* pass */
                List_Pattern_append(or_pats, pat);
                /* pass */
                while ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_Pattern_append(or_pats, Parser_parse_pattern(self));
                }
                /* pass */
                pat = Pattern_ctor_POr(or_pats);
            }
            /* pass */
            Expr* guard_expr = (Expr*)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                guard_expr = Parser_parse_expr(self);
            }
            /* pass */
            __auto_type _t63 = Parser_peek(self);
            if (_t63.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t63;
                /* pass */
                /* pass */
            }
            /* pass */
            Block* arm_block = Block_init();
            /* pass */
            bool is_block_arm = false;
            /* pass */
            __auto_type _t64 = Parser_peek(self);
            if ((_t64.tag == Token_Newline || _t64.tag == Token_Indent)) {
                /* pass */
                arm_block = Parser_parse_block(self);
                /* pass */
                is_block_arm = true;
            } else if (1) {
                __auto_type _ = _t64;
                /* pass */
                /* pass */
            }
            /* pass */
            if ((!is_block_arm)) {
                /* pass */
                Block_push(arm_block, Parser_parse_stmt(self));
            }
            /* pass */
            Block** ab_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
            /* pass */
            (*ab_ptr = arm_block);
            /* pass */
            MatchArm* new_arm = MatchArm_init(pat, ab_ptr);
            /* pass */
            new_arm->guard = guard_expr;
            /* pass */
            List_ptr_append(arms, new_arm);
        }
    }
    /* pass */
    __auto_type _t65 = Parser_peek(self);
    if (_t65.tag == Token_Dedent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t65;
        /* pass */
        /* pass */
    }
    /* pass */
    return box_stmt(Stmt_ctor_SMatch(subj, arms));
}

__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self) {
    /* pass */
    __auto_type _t66 = Parser_peek(self);
    if (_t66.tag == Token_KwCase) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t66;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type _t67 = Parser_peek(self);
    if (_t67.tag == Token_Ident) {
        __auto_type type_name = _t67.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t68 = Parser_peek(self);
        if (_t68.tag == Token_Dot) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type variant = Parser_consume_ident(self);
            /* pass */
            __auto_type _t69 = Parser_peek(self);
            if (_t69.tag == Token_LParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                List_str* fields = (void*)List_str_new();
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    char* fname = Parser_consume_ident(self);
                    /* pass */
                    if ((strcmp((char*)fname, (char*)"") == 0)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        List_str_append(fields, fname);
                    }
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                }
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                if ((fields->len == 1LL)) {
                    /* pass */
                    return Pattern_ctor_PVariantBind(type_name, variant, List_str_get(fields, 0LL));
                }
                /* pass */
                return Pattern_ctor_PVariantBindMany(type_name, variant, fields);
            } else if (1) {
                __auto_type _ = _t69;
                /* pass */
                /* pass */
            }
            /* pass */
            return Pattern_ctor_PVariant(type_name, variant);
        } else if (1) {
            __auto_type _ = _t68;
            /* pass */
            /* pass */
        }
        /* pass */
        return Pattern_ctor_PBind(type_name);
    } else if (_t67.tag == Token_IntLit) {
        __auto_type v = _t67.data.IntLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitInt(v);
    } else if (_t67.tag == Token_KwTrue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitBool(true);
    } else if (_t67.tag == Token_KwFalse) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitBool(false);
    } else if (_t67.tag == Token_StrLit) {
        __auto_type s = _t67.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitStr(s);
    } else if (1) {
        __auto_type _ = _t67;
        /* pass */
        /* pass */
    }
    /* pass */
    return Pattern_make_PWild();
}

__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self) {
    /* pass */
    __auto_type lhs = Parser_parse_expr(self);
    /* pass */
    __auto_type _t70 = Parser_peek(self);
    if (_t70.tag == Token_Colon) {
        /* pass */
        __auto_type _t71 = (*lhs);
        if (_t71.tag == Expr_EIdent) {
            __auto_type name = _t71.data.EIdent.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            AstType* ty = Parser_parse_type(self);
            /* pass */
            Expr* val = (Expr*)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                val = Parser_parse_expr(self);
            }
            /* pass */
            Parser_expect_newline(self);
            /* pass */
            return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, false, false, box_asttype(ty), val));
        } else if (1) {
            __auto_type _ = _t71;
            /* pass */
        }
    } else if (_t70.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, rhs));
    } else if (_t70.tag == Token_PlusEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("+", lhs, rhs))));
    } else if (_t70.tag == Token_MinusEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("-", lhs, rhs))));
    } else if (_t70.tag == Token_StarEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("*", lhs, rhs))));
    } else if (_t70.tag == Token_SlashEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("/", lhs, rhs))));
    } else if (_t70.tag == Token_PercentEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("%", lhs, rhs))));
    } else if (_t70.tag == Token_FloorDivEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("//", lhs, rhs))));
    } else if (_t70.tag == Token_StarStarEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("**", lhs, rhs))));
    } else if (_t70.tag == Token_AmpEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("&", lhs, rhs))));
    } else if (_t70.tag == Token_PipeEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("|", lhs, rhs))));
    } else if (_t70.tag == Token_CaretEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("^", lhs, rhs))));
    } else if (_t70.tag == Token_LtLtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp("<<", lhs, rhs))));
    } else if (_t70.tag == Token_GtGtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(">>", lhs, rhs))));
    } else if (1) {
        __auto_type _ = _t70;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SExpr(lhs));
}

__attribute__((hot)) Expr* Parser_parse_expr(Parser* self) {
    /* pass */
    return Parser_parse_ternary(self);
}

__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_or_expr(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Newline().tag)) {
        /* pass */
        return left;
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* cond = Parser_parse_or_expr(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwElse().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Expr* else_e = Parser_parse_ternary(self);
        /* pass */
        return box_expr(Expr_ctor_EIfElse(cond, left, else_e));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_and_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwOr().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type right = Parser_parse_and_expr(self);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("or", left, right));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_not_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwAnd().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type right = Parser_parse_not_expr(self);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("and", left, right));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self) {
    /* pass */
    __auto_type _t72 = Parser_peek(self);
    if (_t72.tag == Token_KwNot) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp("not", Parser_parse_not_expr(self)));
    } else if (1) {
        __auto_type _ = _t72;
        /* pass */
        /* pass */
    }
    /* pass */
    return Parser_parse_comparison(self);
}

__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitor_expr(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwNot().tag)) {
        /* pass */
        long long saved_pos = self->pos;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwIn().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_EUnaryOp("not", box_expr(Expr_ctor_EBinOp("in", left, Parser_parse_bitor_expr(self)))));
        }
        /* pass */
        self->pos = saved_pos;
    }
    /* pass */
    __auto_type _t73 = Parser_peek(self);
    if (_t73.tag == Token_EqEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("==", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_NotEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("!=", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_Lt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("<", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_Gt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(">", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_LtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("<=", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_GtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(">=", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_KwIs) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("is", left, Parser_parse_bitor_expr(self)));
    } else if (_t73.tag == Token_KwIn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("in", left, Parser_parse_bitor_expr(self)));
    } else if (1) {
        __auto_type _ = _t73;
        /* pass */
        /* pass */
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitxor_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("|", left, Parser_parse_bitxor_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitand_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Caret().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("^", left, Parser_parse_bitand_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_shift_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Amp().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp("&", left, Parser_parse_shift_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_additive(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t74 = Parser_peek(self);
        if (_t74.tag == Token_LtLt) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("<<", left, Parser_parse_additive(self)));
        } else if (_t74.tag == Token_GtGt) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(">>", left, Parser_parse_additive(self)));
        } else if (1) {
            __auto_type _ = _t74;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_additive(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_multiplicative(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t75 = Parser_peek(self);
        if (_t75.tag == Token_Plus) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines_and_indent(self);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("+", left, Parser_parse_multiplicative(self)));
        } else if (_t75.tag == Token_Minus) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines_and_indent(self);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("-", left, Parser_parse_multiplicative(self)));
        } else if (1) {
            __auto_type _ = _t75;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_power(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t76 = Parser_peek(self);
        if (_t76.tag == Token_Star) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("*", left, Parser_parse_power(self)));
        } else if (_t76.tag == Token_Slash) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("/", left, Parser_parse_power(self)));
        } else if (_t76.tag == Token_Percent) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("%", left, Parser_parse_power(self)));
        } else if (_t76.tag == Token_FloorDiv) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp("//", left, Parser_parse_power(self)));
        } else if (1) {
            __auto_type _ = _t76;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_power(Parser* self) {
    /* pass */
    Expr* base = Parser_parse_unary(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_StarStar().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* exp = Parser_parse_power(self);
        /* pass */
        return box_expr(Expr_ctor_EBinOp("**", base, exp));
    }
    /* pass */
    return base;
}

__attribute__((hot)) Expr* Parser_parse_unary(Parser* self) {
    /* pass */
    __auto_type _t77 = Parser_peek(self);
    if (_t77.tag == Token_Minus) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp("-", Parser_parse_unary(self)));
    } else if (_t77.tag == Token_Tilde) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp("~", Parser_parse_unary(self)));
    } else if (_t77.tag == Token_Amp) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp("&", Parser_parse_unary(self)));
    } else if (_t77.tag == Token_Star) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp("*", Parser_parse_unary(self)));
    } else if (1) {
        __auto_type _ = _t77;
        /* pass */
        /* pass */
    }
    /* pass */
    return Parser_parse_postfix(self);
}

__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self) {
    /* pass */
    Expr* e = Parser_parse_primary(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t78 = Parser_peek(self);
        if (_t78.tag == Token_Dot) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            char* attr = Parser_consume_ident(self);
            /* pass */
            __auto_type _t79 = Parser_peek(self);
            if (_t79.tag == Token_LParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                e = box_expr(Expr_ctor_EMethodCall(e, attr, Parser_parse_arg_list(self)));
            } else if (1) {
                __auto_type _ = _t79;
                /* pass */
                e = box_expr(Expr_ctor_EPropAccess(e, attr));
            }
        } else if (_t78.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            e = box_expr(Expr_ctor_ECall(e, Parser_parse_arg_list(self)));
        } else if (_t78.tag == Token_LBracket) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Expr* idx = Parser_parse_expr(self);
            /* pass */
            while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines_and_indent(self);
                /* pass */
                Parser_parse_expr(self);
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            /* pass */
            e = box_expr(Expr_ctor_EIndex(e, idx));
        } else if (_t78.tag == Token_Question) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            e = box_expr(Expr_ctor_ETryExpr(e));
        } else if (_t78.tag == Token_KwAs) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            e = box_expr(Expr_ctor_ECast(e, box_asttype(Parser_parse_type(self))));
        } else if (1) {
            __auto_type _ = _t78;
            break;
        }
    }
    /* pass */
    return e;
}

__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self) {
    /* pass */
    List_ptr* el = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type _t80 = Parser_peek(self);
        if ((_t80.tag == Token_RParen || _t80.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t80;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            List_ptr_append(el, Parser_parse_expr(self));
            /* pass */
            __auto_type _t81 = Parser_peek(self);
            if (_t81.tag == Token_Comma) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines_and_indent(self);
            } else if (1) {
                __auto_type _ = _t81;
                /* pass */
                going = false;
            }
        }
    }
    /* pass */
    __auto_type _t82 = Parser_peek(self);
    if (_t82.tag == Token_RParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t82;
        /* pass */
        /* pass */
    }
    /* pass */
    return el;
}

__attribute__((hot)) Expr* Parser_parse_primary(Parser* self) {
    /* pass */
    __auto_type _t83 = Parser_peek(self);
    if (_t83.tag == Token_IntLit) {
        __auto_type v = _t83.data.IntLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitInt(v));
    } else if (_t83.tag == Token_FloatLit) {
        __auto_type v = _t83.data.FloatLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitFloat(v));
    } else if (_t83.tag == Token_StrLit) {
        __auto_type s = _t83.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitStr(s));
    } else if (_t83.tag == Token_TripleStrLit) {
        __auto_type s = _t83.data.TripleStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitStr(s));
    } else if (_t83.tag == Token_RawStrLit) {
        __auto_type s = _t83.data.RawStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ERawStr(s));
    } else if (_t83.tag == Token_ByteStrLit) {
        __auto_type s = _t83.data.ByteStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBytes(s));
    } else if (_t83.tag == Token_FStrLit) {
        __auto_type s = _t83.data.FStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Parser_parse_fstring(self, s);
    } else if (_t83.tag == Token_CharLit) {
        __auto_type v = _t83.data.CharLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitChar(v));
    } else if (_t83.tag == Token_BoolLit) {
        __auto_type v = _t83.data.BoolLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(v));
    } else if (_t83.tag == Token_KwTrue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(true));
    } else if (_t83.tag == Token_KwFalse) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(false));
    } else if (_t83.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_make_ELitNone());
    } else if (_t83.tag == Token_KwInt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("int"));
    } else if (_t83.tag == Token_KwFloat) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("float"));
    } else if (_t83.tag == Token_KwBool) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("bool"));
    } else if ((_t83.tag == Token_KwStr || _t83.tag == Token_KwString)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("str"));
    } else if (_t83.tag == Token_KwChar) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("char"));
    } else if (_t83.tag == Token_KwVoid) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent("void"));
    } else if (_t83.tag == Token_KwSuper) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        char* super_base = "";
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            super_base = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                char* super_method = Parser_consume_ident(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args = Parser_parse_arg_list(self);
                    /* pass */
                    return box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method, super_args));
                } else {
                    /* pass */
                    return box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method));
                }
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            char* super_next = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                super_base = super_next;
                /* pass */
                char* super_method2 = Parser_consume_ident(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args2 = Parser_parse_arg_list(self);
                    /* pass */
                    return box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method2, super_args2));
                } else {
                    /* pass */
                    return box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method2));
                }
            } else {
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args3 = Parser_parse_arg_list(self);
                    /* pass */
                    return box_expr(Expr_ctor_ESuperMethodCall("", super_next, super_args3));
                } else {
                    /* pass */
                    return box_expr(Expr_ctor_ESuperPropAccess("", super_next));
                }
            }
        }
        /* pass */
        return box_expr(Expr_ctor_EIdent("super"));
    } else if (_t83.tag == Token_Ident) {
        __auto_type name = _t83.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(name));
    } else if (_t83.tag == Token_KwSizeOf) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t84 = Parser_peek(self);
        if (_t84.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t84;
            /* pass */
            /* pass */
        }
        /* pass */
        AstType** ty = box_asttype(Parser_parse_type(self));
        /* pass */
        __auto_type _t85 = Parser_peek(self);
        if (_t85.tag == Token_RParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t85;
            /* pass */
            /* pass */
        }
        /* pass */
        return box_expr(Expr_ctor_ESizeOf(ty));
    } else if (_t83.tag == Token_LParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_ETuple((void*)List_ptr_new()));
        }
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            List_ptr* items = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(items, e);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                List_ptr_append(items, Parser_parse_expr(self));
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
            }
            /* pass */
            __auto_type _t86 = Parser_peek(self);
            if (_t86.tag == Token_RParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t86;
                /* pass */
                /* pass */
            }
            /* pass */
            return box_expr(Expr_ctor_ETuple(items));
        } else {
            /* pass */
            __auto_type _t87 = Parser_peek(self);
            if (_t87.tag == Token_RParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t87;
                /* pass */
                /* pass */
            }
            /* pass */
            return e;
        }
    } else if (_t83.tag == Token_LBracket) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_EList((void*)List_ptr_new()));
        }
        /* pass */
        Expr* first = Parser_parse_or_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwFor().tag)) {
            /* pass */
            List_ptr* generators = (void*)List_ptr_new();
            /* pass */
            while ((Parser_peek(self).tag == Token_make_KwFor().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                char* target = Parser_consume_ident(self);
                /* pass */
                __auto_type _t88 = Parser_peek(self);
                if (_t88.tag == Token_KwIn) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t88;
                    /* pass */
                    /* pass */
                }
                /* pass */
                Expr* iter = Parser_parse_or_expr(self);
                /* pass */
                List_ptr* ifs = (void*)List_ptr_new();
                /* pass */
                while ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr_append(ifs, Parser_parse_or_expr(self));
                }
                /* pass */
                Comprehension* c = Comprehension_init(target, iter);
                /* pass */
                c->ifs = ifs;
                /* pass */
                Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
                /* pass */
                (*c_ptr = c);
                /* pass */
                List_ptr_append(generators, c_ptr);
            }
            /* pass */
            __auto_type _t89 = Parser_peek(self);
            if (_t89.tag == Token_RBracket) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t89;
                /* pass */
                /* pass */
            }
            /* pass */
            return box_expr(Expr_ctor_EListComp(first, generators));
        }
        /* pass */
        List_ptr* items = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(items, first);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                List_ptr_append(items, Parser_parse_expr(self));
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
            }
        }
        /* pass */
        __auto_type _t90 = Parser_peek(self);
        if (_t90.tag == Token_RBracket) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t90;
            /* pass */
            /* pass */
        }
        /* pass */
        return box_expr(Expr_ctor_EList(items));
    } else if (_t83.tag == Token_LBrace) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* keys = (void*)List_ptr_new();
        /* pass */
        List_ptr* vals = (void*)List_ptr_new();
        /* pass */
        List_ptr* set_items = (void*)List_ptr_new();
        /* pass */
        bool is_set = false;
        /* pass */
        if ((Parser_peek(self).tag != Token_make_RBrace().tag)) {
            /* pass */
            Expr* first = Parser_parse_expr(self);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                Expr* value = Parser_parse_expr(self);
                /* pass */
                List_ptr_append(keys, first);
                /* pass */
                List_ptr_append(vals, value);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBrace().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    Expr* key = Parser_parse_expr(self);
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    __auto_type _t91 = Parser_peek(self);
                    if (_t91.tag == Token_Colon) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t91;
                        /* pass */
                        /* pass */
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    Expr* v = Parser_parse_expr(self);
                    /* pass */
                    List_ptr_append(keys, key);
                    /* pass */
                    List_ptr_append(vals, v);
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        break;
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                }
            } else {
                /* pass */
                is_set = true;
                /* pass */
                List_ptr_append(set_items, first);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBrace().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    List_ptr_append(set_items, Parser_parse_expr(self));
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        break;
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                }
            }
        }
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        __auto_type _t92 = Parser_peek(self);
        if (_t92.tag == Token_RBrace) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t92;
            /* pass */
            /* pass */
        }
        /* pass */
        if (is_set) {
            /* pass */
            return box_expr(Expr_ctor_ESet(set_items));
        } else {
            /* pass */
            return box_expr(Expr_ctor_EDict(keys, vals));
        }
    } else if (_t83.tag == Token_KwTry) {
        /* pass */
        Stmt* st = Parser_parse_try_stmt(self);
        /* pass */
        __auto_type _t93 = (*st);
        if (_t93.tag == Stmt_STry) {
            __auto_type try_body = _t93.data.STry.try_body;
__auto_type catches = _t93.data.STry.catches;
__auto_type finally_b = _t93.data.STry.finally_b;
            /* pass */
            return box_expr(Expr_ctor_ETry(try_body, catches, finally_b));
        } else if (1) {
            __auto_type _ = _t93;
            /* pass */
            /* pass */
        }
    } else if (_t83.tag == Token_KwAwait) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EAwait(Parser_parse_expr(self)));
    } else if (_t83.tag == Token_KwYield) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EYield(Parser_parse_expr(self)));
    } else if ((_t83.tag == Token_KwDef || _t83.tag == Token_KwAsync)) {
        /* pass */
        bool is_async = false;
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwAsync().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            is_async = true;
        }
        /* pass */
        __auto_type _t94 = Parser_peek(self);
        if (_t94.tag == Token_KwDef) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t94;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t95 = Parser_peek(self);
        if (_t95.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t95;
            /* pass */
            /* pass */
        }
        /* pass */
        List_ptr* params = Parser_parse_param_list(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        AstType** return_type = (AstType**)(0LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return_type = box_asttype(Parser_parse_type(self));
        }
        /* pass */
        __auto_type _t96 = Parser_peek(self);
        if (_t96.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t96;
            /* pass */
            /* pass */
        }
        /* pass */
        Block* body = Parser_parse_block(self);
        /* pass */
        return box_expr(Expr_ctor_EClosure(params, return_type, body, is_async));
    } else if (_t83.tag == Token_FStrLit) {
        __auto_type raw = _t83.data.FStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Parser_parse_fstring(self, raw);
    } else if (1) {
        __auto_type _ = _t83;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((!Parser_at_end(self))) {
        /* pass */
        char* tok_desc = "token";
        /* pass */
        char* hint = "check for a typo, a missing ':' or unbalanced parentheses/brackets near this point.";
        /* pass */
        __auto_type _t97 = Parser_peek(self);
        if (_t97.tag == Token_Newline) {
            /* pass */
            tok_desc = "end of line";
            /* pass */
            hint = "an expression was expected before the end of this line - check for a missing value or trailing operator.";
        } else if (_t97.tag == Token_Indent) {
            /* pass */
            tok_desc = "indentation";
            /* pass */
            hint = "check that this line's indentation matches the surrounding block.";
        } else if (_t97.tag == Token_Dedent) {
            /* pass */
            tok_desc = "dedent";
            /* pass */
            hint = "check that this block is properly indented and closed.";
        } else if (_t97.tag == Token_Ident) {
            __auto_type n = _t97.data.Ident.name;
            /* pass */
            tok_desc = _tr_str_concat(_tr_str_concat("identifier '", n), "'");
            /* pass */
            hint = _tr_str_concat(_tr_str_concat("an operator, ':' or end of statement was expected before '", n), "'.");
        } else if (_t97.tag == Token_KwMut) {
            tok_desc = "keyword 'mut'";
        } else if (_t97.tag == Token_KwConst) {
            tok_desc = "keyword 'const'";
        } else if (_t97.tag == Token_KwPub) {
            tok_desc = "keyword 'pub'";
        } else if (_t97.tag == Token_KwReturn) {
            tok_desc = "keyword 'return'";
        } else if (_t97.tag == Token_KwIf) {
            tok_desc = "keyword 'if'";
        } else if (_t97.tag == Token_KwWhile) {
            tok_desc = "keyword 'while'";
        } else if (_t97.tag == Token_Comma) {
            /* pass */
            tok_desc = "','";
            /* pass */
            hint = "remove the extra ',' or add the missing item before it.";
        } else if (_t97.tag == Token_Colon) {
            /* pass */
            tok_desc = "':'";
            /* pass */
            hint = "remove the extra ':' or check the statement before it is complete.";
        } else if (_t97.tag == Token_RParen) {
            /* pass */
            tok_desc = "')'";
            /* pass */
            hint = "check for an extra ')' or a missing matching '('.";
        } else if (_t97.tag == Token_RBracket) {
            /* pass */
            tok_desc = "']'";
            /* pass */
            hint = "check for an extra ']' or a missing matching '['.";
        } else if (_t97.tag == Token_RBrace) {
            /* pass */
            tok_desc = "'}'";
            /* pass */
            hint = "check for an extra '}' or a missing matching '{'.";
        } else if (1) {
            __auto_type _ = _t97;
            /* pass */
        }
        /* pass */
        char* loc = "";
        /* pass */
        if ((_tr_strlen((char*)self->current_file) > 0LL)) {
            /* pass */
            loc = _tr_str_concat(self->current_file, ":");
        }
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat(_tr_str_concat(_tr_str_concat(_tr_str_concat(loc, _tr_int_to_str((long long)(Parser_cur_line(self)))), ": error: unexpected "), tok_desc), " in expression")));
        /* pass */
        printf("%s\n", (char*)(_tr_str_concat("       FIX: ", hint)));
        /* pass */
        self->error_count = (self->error_count + 1LL);
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_expr(Expr_make_ELitNone());
}

__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, char* raw) {
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    char* p = ((char*)(raw));
    /* pass */
    long long i = 0LL;
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if ((c == 123LL)) {
            /* pass */
            if ((sb->buf->len > 0LL)) {
                /* pass */
                List_ptr_append(fl, FStringPart_init_text(StringObj_as_str(StringBuilder_to_string(sb))));
                /* pass */
                sb = StringBuilder_init(64LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            StringBuilder* expr_sb = StringBuilder_init(64LL);
            /* pass */
            long long depth = 1LL;
            /* pass */
            while ((depth > 0LL)) {
                /* pass */
                long long ec = ((long long)((*(p + i))));
                /* pass */
                if ((ec == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                if ((ec == 123LL)) {
                    /* pass */
                    depth = (depth + 1LL);
                }
                /* pass */
                if ((ec == 125LL)) {
                    /* pass */
                    depth = (depth - 1LL);
                }
                /* pass */
                if ((depth > 0LL)) {
                    /* pass */
                    StringBuilder_append_char(expr_sb, ec);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            char* expr_str = StringObj_as_str(StringBuilder_to_string(expr_sb));
            /* pass */
            char* fmt_spec = "";
            /* pass */
            long long colon_pos = _find_fmt_colon(expr_str);
            /* pass */
            if ((colon_pos >= 0LL)) {
                /* pass */
                char* _fs = _tr_str_slice(expr_str, (colon_pos + 1LL), _tr_strlen((char*)expr_str));
                /* pass */
                fmt_spec = _tr_str_strip(_fs);
                /* pass */
                char* _es = _tr_str_slice(expr_str, 0LL, colon_pos);
                /* pass */
                expr_str = _tr_str_strip(_es);
            }
            /* pass */
            Lexer* lexer = Lexer_init(expr_str);
            /* pass */
            List_Token* _fstr_tokens = Lexer_tokenize(lexer);
            /* pass */
            if ((_fstr_tokens->len > 0LL)) {
                /* pass */
                Parser* parser = Parser_init(_fstr_tokens, lexer->token_lines);
                /* pass */
                Expr* e = Parser_parse_expr(parser);
                /* pass */
                if ((_tr_strlen((char*)fmt_spec) > 0LL)) {
                    /* pass */
                    List_ptr_append(fl, FStringPart_init_expr_fmt(e, fmt_spec));
                } else {
                    /* pass */
                    List_ptr_append(fl, FStringPart_init_expr(e));
                }
            }
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    if ((sb->buf->len > 0LL)) {
        /* pass */
        List_ptr_append(fl, FStringPart_init_text(StringObj_as_str(StringBuilder_to_string(sb))));
    }
    /* pass */
    return box_expr(Expr_ctor_EFString(fl));
}

__attribute__((hot)) Program* Parser_parse_program(Parser* self) {
    /* pass */
    Program* prog = Program_init();
    /* pass */
    Parser_skip_newlines_and_indent(self);
    /* pass */
    while ((!Parser_at_end(self))) {
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        if ((!Parser_at_end(self))) {
            /* pass */
            Program_push(prog, Parser_parse_decl(self));
        }
    }
    /* pass */
    return prog;
}

__attribute__((hot)) Decl* Parser_parse_decl(Parser* self) {
    /* pass */
    List_ptr* decorators = (void*)List_ptr_new();
    /* pass */
    while ((Parser_peek(self).tag == Token_make_At().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type name = Parser_consume_ident(self);
        /* pass */
        Decorator* d = Decorator_init(name);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            d->args = Parser_parse_arg_list(self);
        }
        /* pass */
        List_ptr_append(decorators, d);
        /* pass */
        Parser_expect_newline(self);
    }
    /* pass */
    bool is_public = false;
    /* pass */
    __auto_type _t98 = Parser_peek(self);
    if (_t98.tag == Token_KwPub) {
        /* pass */
        is_public = true;
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t98;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type _t99 = Parser_peek(self);
    if (_t99.tag == Token_KwFrom) {
        /* pass */
        return Parser_parse_from_import(self);
    } else if (_t99.tag == Token_KwImport) {
        /* pass */
        return Parser_parse_import(self);
    } else if (_t99.tag == Token_Ident) {
        __auto_type type_kw = _t99.data.Ident.name;
        /* pass */
        if ((strcmp((char*)type_kw, (char*)"type") == 0)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            char* alias_name = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType* target_ty = Parser_parse_type(self);
                /* pass */
                Parser_expect_newline(self);
                /* pass */
                return box_decl(Decl_ctor_DTypeAlias(alias_name, box_asttype(target_ty)));
            }
        }
    } else if (_t99.tag == Token_KwDef) {
        /* pass */
        long long _fline = Parser_cur_line(self);
        /* pass */
        FunctionDef* f = Parser_parse_function_def(self, false);
        /* pass */
        f->line = _fline;
        /* pass */
        f->is_public = is_public;
        /* pass */
        f->decorators = decorators;
        /* pass */
        return box_decl(Decl_ctor_DFunction(f));
    } else if (_t99.tag == Token_KwClass) {
        /* pass */
        long long _cline = Parser_cur_line(self);
        /* pass */
        Decl* c_ptr = Parser_parse_class_decl(self);
        /* pass */
        __auto_type _t100 = (*c_ptr);
        if (_t100.tag == Decl_DClass) {
            __auto_type c = _t100.data.DClass.cls;
            /* pass */
            c->line = _cline;
            /* pass */
            c->is_public = is_public;
            /* pass */
            c->is_class = true;
            /* pass */
            c->decorators = decorators;
            /* pass */
            (*c_ptr = Decl_ctor_DClass(c));
        } else if (1) {
            __auto_type _ = _t100;
            /* pass */
            /* pass */
        }
        /* pass */
        return c_ptr;
    } else if (_t99.tag == Token_KwEnum) {
        /* pass */
        long long _eline = Parser_cur_line(self);
        /* pass */
        Decl* e_ptr = Parser_parse_enum_decl(self);
        /* pass */
        __auto_type _t101 = (*e_ptr);
        if (_t101.tag == Decl_DEnum) {
            __auto_type e = _t101.data.DEnum.enm;
            /* pass */
            e->line = _eline;
            /* pass */
            e->is_public = is_public;
            /* pass */
            e->decorators = decorators;
            /* pass */
            (*e_ptr = Decl_ctor_DEnum(e));
        } else if (1) {
            __auto_type _ = _t101;
            /* pass */
            /* pass */
        }
        /* pass */
        return e_ptr;
    } else if (_t99.tag == Token_KwInterface) {
        /* pass */
        long long _iline = Parser_cur_line(self);
        /* pass */
        Decl* i_ptr = Parser_parse_interface_decl(self);
        /* pass */
        __auto_type _t102 = (*i_ptr);
        if (_t102.tag == Decl_DInterface) {
            __auto_type i = _t102.data.DInterface.iface;
            /* pass */
            i->line = _iline;
            /* pass */
            i->is_public = is_public;
            /* pass */
            i->decorators = decorators;
            /* pass */
            (*i_ptr = Decl_ctor_DInterface(i));
        } else if (1) {
            __auto_type _ = _t102;
            /* pass */
            /* pass */
        }
        /* pass */
        return i_ptr;
    } else if (_t99.tag == Token_KwExtend) {
        /* pass */
        return Parser_parse_extend_decl(self);
    } else if (_t99.tag == Token_KwDecorator) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        long long _dl = Parser_cur_line(self);
        /* pass */
        FunctionDef* _df = Parser_parse_function_def(self, false);
        /* pass */
        _df->line = _dl;
        /* pass */
        _df->is_public = is_public;
        /* pass */
        return box_decl(Decl_ctor_DDecoratorDef(_df));
    } else if (_t99.tag == Token_KwExtern) {
        /* pass */
        return Parser_parse_extern_decl(self);
    } else if (_t99.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t103 = Parser_peek(self);
        if (_t103.tag == Token_KwDef) {
            /* pass */
            FunctionDef* f = Parser_parse_function_def(self, false);
            /* pass */
            f->is_async = true;
            /* pass */
            f->is_public = is_public;
            /* pass */
            f->decorators = decorators;
            /* pass */
            return box_decl(Decl_ctor_DFunction(f));
        } else if (1) {
            __auto_type _ = _t103;
            /* pass */
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t99;
        /* pass */
        /* pass */
    }
    /* pass */
    return box_decl(Decl_ctor_DTopLevelStmt(Parser_parse_stmt(self)));
}

__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* path = Parser_consume_module_ident(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Dot().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        path = _tr_str_concat(_tr_str_concat(path, "."), Parser_consume_module_ident(self));
    }
    /* pass */
    __auto_type _t104 = Parser_peek(self);
    if (_t104.tag == Token_KwImport) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t104;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* il = (void*)List_ptr_new();
    /* pass */
    bool multi = false;
    /* pass */
    __auto_type _t105 = Parser_peek(self);
    if ((_t105.tag == Token_LParen || _t105.tag == Token_LBracket)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        multi = true;
    } else if (1) {
        __auto_type _ = _t105;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        if (multi) {
            /* pass */
            Parser_skip_newlines_and_indent(self);
        } else {
            /* pass */
            Parser_skip_newlines(self);
        }
        /* pass */
        __auto_type _t106 = Parser_peek(self);
        if ((_t106.tag == Token_RParen || _t106.tag == Token_RBracket || _t106.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if ((_t106.tag == Token_Newline || _t106.tag == Token_Dedent)) {
            /* pass */
            if ((!multi)) {
                /* pass */
                going = false;
            }
        } else if (1) {
            __auto_type _ = _t106;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            char* item_name = Parser_consume_ident(self);
            /* pass */
            if ((strcmp((char*)item_name, (char*)"") != 0)) {
                /* pass */
                ImportItem* item = ImportItem_init(item_name);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    item->alias = Parser_consume_ident(self);
                }
                /* pass */
                List_ptr_append(il, item);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if ((!multi)) {
                    /* pass */
                    going = false;
                }
            } else {
                /* pass */
                going = false;
            }
        }
    }
    /* pass */
    if (multi) {
        /* pass */
        __auto_type _t107 = Parser_peek(self);
        if ((_t107.tag == Token_RParen || _t107.tag == Token_RBracket)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t107;
            /* pass */
            /* pass */
        }
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_decl(Decl_ctor_DFromImport(path, il));
}

__attribute__((hot)) Decl* Parser_parse_import(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* path = Parser_consume_module_ident(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Dot().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        path = _tr_str_concat(_tr_str_concat(path, "."), Parser_consume_module_ident(self));
    }
    /* pass */
    char* alias = "";
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        alias = Parser_consume_ident(self);
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_decl(Decl_ctor_DImport(path, alias));
}

__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* name = Parser_consume_ident(self);
    /* pass */
    FunctionDef* f = FunctionDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while ((Parser_peek(self).tag != Token_make_RBracket().tag)) {
            /* pass */
            List_str_append(f->generics, Parser_consume_ident(self));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->params = Parser_parse_param_list(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    __auto_type _t108 = Parser_peek(self);
    if (_t108.tag == Token_KwThrows) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->throws_ty = box_asttype(Parser_parse_type(self));
    } else if (1) {
        __auto_type _ = _t108;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* ret_t = Parser_parse_type(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ret_t->from_param = Parser_consume_ident(self);
        }
        /* pass */
        f->ret_ty = box_asttype(ret_t);
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->body = Parser_parse_block(self);
    } else {
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        f->body = Block_init();
    }
    /* pass */
    return f;
}

__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* name = Parser_consume_ident(self);
    /* pass */
    ClassDef* c = ClassDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while ((Parser_peek(self).tag != Token_make_RBracket().tag)) {
            /* pass */
            List_str_append(c->generics, Parser_consume_ident(self));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwExtends().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        List_str_append(c->base_classes, Parser_consume_ident(self));
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            List_str_append(c->base_classes, Parser_consume_ident(self));
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwImplements().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (true) {
            /* pass */
            List_str_append(c->iface_names, Parser_consume_ident(self));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else {
                /* pass */
                break;
            }
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t109 = Parser_peek(self);
    if (_t109.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t109;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, d);
            /* pass */
            Parser_expect_newline(self);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t110 = Parser_peek(self);
        if (_t110.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t110;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t111 = Parser_peek(self);
        if ((_t111.tag == Token_Dedent || _t111.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t111.tag == Token_KwDef) {
            /* pass */
            long long _mline = Parser_cur_line(self);
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->line = _mline;
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(c->methods, m);
        } else if (_t111.tag == Token_KwAsync) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type _t112 = Parser_peek(self);
            if (_t112.tag == Token_KwDef) {
                /* pass */
                long long _mline = Parser_cur_line(self);
                /* pass */
                FunctionDef* m = Parser_parse_function_def(self, true);
                /* pass */
                m->line = _mline;
                /* pass */
                m->is_async = true;
                /* pass */
                m->is_public = is_p;
                /* pass */
                m->decorators = decorators;
                /* pass */
                List_ptr_append(c->methods, m);
            } else if (1) {
                __auto_type _ = _t112;
                /* pass */
                /* pass */
            }
        } else if (_t111.tag == Token_Ident) {
            __auto_type fname = _t111.data.Ident.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            AstType** ty_ptr = (AstType**)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                ty_ptr = box_asttype(Parser_parse_type(self));
            }
            /* pass */
            FieldDef* fld = FieldDef_init(fname, ty_ptr);
            /* pass */
            List_ptr_append(c->fields, fld);
            /* pass */
            Parser_expect_newline(self);
        } else if (_t111.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t111;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_decl(Decl_ctor_DClass(c));
}

__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* name = Parser_consume_ident(self);
    /* pass */
    EnumDef* e = EnumDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwImplements().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (true) {
            /* pass */
            List_str_append(e->iface_names, Parser_consume_ident(self));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else {
                /* pass */
                break;
            }
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t113 = Parser_peek(self);
    if (_t113.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t113;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, d);
            /* pass */
            Parser_expect_newline(self);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t114 = Parser_peek(self);
        if (_t114.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t114;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t115 = Parser_peek(self);
        if ((_t115.tag == Token_Dedent || _t115.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t115.tag == Token_Ident) {
            __auto_type vname = _t115.data.Ident.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            VariantDef* vd = VariantDef_init(vname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                vd->fields = Parser_parse_param_list(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            List_ptr_append(e->variants, vd);
            /* pass */
            Parser_expect_newline(self);
        } else if (_t115.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(e->methods, m);
        } else if (_t115.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t115;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_decl(Decl_ctor_DEnum(e));
}

__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* name = Parser_consume_ident(self);
    /* pass */
    InterfaceDef* i = InterfaceDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
            /* pass */
            char* gname = Parser_consume_ident(self);
            /* pass */
            if ((strcmp((char*)gname, (char*)"") != 0)) {
                /* pass */
                List_str_append(i->generics, gname);
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t116 = Parser_peek(self);
    if (_t116.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t116;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t117 = Parser_peek(self);
        if (_t117.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t117;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t118 = Parser_peek(self);
        if ((_t118.tag == Token_Dedent || _t118.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t118.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            List_ptr_append(i->methods, m);
        } else if (_t118.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t118;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_decl(Decl_ctor_DInterface(i));
}

__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* target = Parser_consume_ident(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t119 = Parser_peek(self);
    if (_t119.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t119;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, d);
            /* pass */
            Parser_expect_newline(self);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t120 = Parser_peek(self);
        if (_t120.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t120;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t121 = Parser_peek(self);
        if ((_t121.tag == Token_Dedent || _t121.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t121.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(fl, m);
        } else if (_t121.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t121;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Decl d = Decl_ctor_DExtend(target, fl);
    /* pass */
    return box_decl(d);
}

__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    char* abi = "C";
    /* pass */
    __auto_type _t122 = Parser_peek(self);
    if (_t122.tag == Token_StrLit) {
        __auto_type s = _t122.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        abi = s;
    } else if (1) {
        __auto_type _ = _t122;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t123 = Parser_peek(self);
    if (_t123.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t123;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t124 = Parser_peek(self);
        if (_t124.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t124;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t125 = Parser_peek(self);
        if ((_t125.tag == Token_Dedent || _t125.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t125.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, false);
            /* pass */
            m->is_public = is_p;
            /* pass */
            List_ptr_append(fl, m);
        } else if (1) {
            __auto_type _ = _t125;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_decl(Decl_ctor_DExtern(abi, fl));
}

__attribute__((hot)) Expr* box_expr(Expr e) {
    /* pass */
    Expr* p = ((Expr*)_tr_c_calloc((size_t)(1LL), sizeof(Expr)));
    /* pass */
    (*p = e);
    /* pass */
    return p;
}

__attribute__((hot)) Stmt* box_stmt(Stmt s) {
    /* pass */
    Stmt* p = ((Stmt*)_tr_c_calloc((size_t)(1LL), sizeof(Stmt)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

__attribute__((hot)) Decl* box_decl(Decl d) {
    /* pass */
    Decl* p = ((Decl*)_tr_c_calloc((size_t)(1LL), sizeof(Decl)));
    /* pass */
    (*p = d);
    /* pass */
    return p;
}

__attribute__((hot)) AstType** box_asttype(AstType* t) {
    /* pass */
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
    /* pass */
    (*p = t);
    /* pass */
    return p;
}

__attribute__((hot)) long long _find_fmt_colon(char* s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen((char*)s);
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = _tr_str_char_at_code(s, i);
        /* pass */
        if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
            /* pass */
            depth = (depth - 1LL);
        } else if (((c == 58LL) && (depth == 0LL))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

