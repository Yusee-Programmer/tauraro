#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init() {
    /* pass */
    LlvmGenerator* g = ((LlvmGenerator*)_tr_obj_alloc(sizeof(LlvmGenerator)));
    /* pass */
    g->buf = StringBuilder_init(1024LL);
    /* pass */
    g->temp = 0LL;
    /* pass */
    g->classes = _tr_dict_new(64LL);
    /* pass */
    g->enums = _tr_dict_new(32LL);
    /* pass */
    g->functions = _tr_dict_new(128LL);
    /* pass */
    return g;
}

__attribute__((hot)) void LlvmGenerator_w(LlvmGenerator* self, TrStr s) {
    /* pass */
    StringBuilder_append(self->buf, s);
}

__attribute__((hot)) TrStr LlvmGenerator_next_reg(LlvmGenerator* self) {
    /* pass */
    self->temp = (self->temp + 1LL);
    /* pass */
    return ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->temp)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("%_")), _cr.data); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) void LlvmGenerator__tr_fn_register(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        _tr_dict_set(self->enums, _tr_strz(e->name), _tr_obj_retain(e));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        _tr_dict_set(self->functions, _tr_strz(f->name), _tr_obj_retain(f));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void LlvmGenerator_emit_type_decls(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("%struct."));
        /* pass */
        LlvmGenerator_w(self, c->name);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = type { "));
        /* pass */
        bool first = true;
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < c->fields->len)) {
            /* pass */
            if ((!first)) {
                /* pass */
                LlvmGenerator_w(self, _tr_str_lit(", "));
            }
            /* pass */
            ({ TrStr _at_t2149 = (llvm_type(((HirField*)List_ptr_get(c->fields, fi))->ty)); LlvmGenerator_w(self, _at_t2149); _tr_str_release(_at_t2149); });
            /* pass */
            first = false;
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        if (first) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("i8"));
        }
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" }\n"));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->enums->len)) {
        /* pass */
        HirEnum* e = ((HirEnum*)List_ptr_get(prog->enums, i));
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("%enum."));
        /* pass */
        LlvmGenerator_w(self, e->name);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = type { i32, [64 x i8] }\n"));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((prog->classes->len > 0LL) || (prog->enums->len > 0LL))) {
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
    }
}

__attribute__((hot)) void LlvmGenerator_emit_func_sig(LlvmGenerator* self, HirFunction* f, TrStr class_name) {
    /* pass */
    TrStr ret_ty = llvm_type(f->ret_ty);
    /* pass */
    TrStr fname = f->name;
    /* pass */
    if ((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit("main"))) == 0)) {
        /* pass */
        fname = _tr_str_lit("_tr_main");
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("define "));
    /* pass */
    if ((!f->is_public)) {
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("internal "));
    }
    /* pass */
    LlvmGenerator_w(self, ret_ty);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(" @"));
    /* pass */
    LlvmGenerator_w(self, fname);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("("));
    /* pass */
    bool first = true;
    /* pass */
    if ((strcmp(_tr_strz(class_name), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        bool has_self = false;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < f->params->len)) {
            /* pass */
            if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(f->params, i))->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
                /* pass */
                has_self = true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if ((has_self && (!f->is_static))) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("ptr %self"));
            /* pass */
            first = false;
        }
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, i));
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(", "));
        }
        /* pass */
        ({ TrStr _at_t2150 = (llvm_type(p->ty)); LlvmGenerator_w(self, _at_t2150); _tr_str_release(_at_t2150); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" %"));
        /* pass */
        LlvmGenerator_w(self, p->name);
        /* pass */
        first = false;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(") {\nentry:\n"));
    _tr_str_release(ret_ty);
}

__attribute__((hot)) TrStr LlvmGenerator_gen_expr(LlvmGenerator* self, HirExpr* e_ptr) {
    /* pass */
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("null");
    }
    /* pass */
    __auto_type e = (*e_ptr);
    /* pass */
    __auto_type _t2151 = e;
    if (_t2151.tag == HirExpr_ELitInt) {
        __auto_type v = _t2151.data.ELitInt.val;
__auto_type ty = _t2151.data.ELitInt.ty;
        /* pass */
        TrStr reg = LlvmGenerator_next_reg(self);
        /* pass */
        TrStr lty = llvm_type(ty);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    "));
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = add "));
        /* pass */
        LlvmGenerator_w(self, lty);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" 0, "));
        /* pass */
        ({ TrStr _at_t2152 = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); LlvmGenerator_w(self, _at_t2152); _tr_str_release(_at_t2152); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        _tr_str_release(lty);
        return reg;
    } else if (_t2151.tag == HirExpr_ELitBool) {
        __auto_type v = _t2151.data.ELitBool.val;
        /* pass */
        TrStr reg = LlvmGenerator_next_reg(self);
        /* pass */
        TrStr bval = _tr_str_lit("0");
        /* pass */
        if (v) {
            /* pass */
            TrStr _strtmp_t2153 = _tr_str_lit("1");
            _tr_str_release(bval);
            bval = _strtmp_t2153;
        }
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    "));
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = add i1 0, "));
        /* pass */
        LlvmGenerator_w(self, bval);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        _tr_str_release(bval);
        return reg;
    } else if (_t2151.tag == HirExpr_ELitFloat) {
        __auto_type v = _t2151.data.ELitFloat.val;
__auto_type ty = _t2151.data.ELitFloat.ty;
        /* pass */
        TrStr reg = LlvmGenerator_next_reg(self);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    "));
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = fadd double 0.0, "));
        /* pass */
        ({ TrStr _at_t2154 = (_tr_str_wrap(_tr_float_to_str((double)(v)))); LlvmGenerator_w(self, _at_t2154); _tr_str_release(_at_t2154); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        return reg;
    } else if (_t2151.tag == HirExpr_ELitStr) {
        __auto_type v = _t2151.data.ELitStr.val;
        /* pass */
        TrStr reg = LlvmGenerator_next_reg(self);
        /* pass */
        TrStr gname = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->temp)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@.str.")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        long long len = (_tr_strlen(_tr_strz(v)) + 1LL);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    "));
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = getelementptr ["));
        /* pass */
        ({ TrStr _at_t2155 = (_tr_str_wrap(_tr_int_to_str((long long)(len)))); LlvmGenerator_w(self, _at_t2155); _tr_str_release(_at_t2155); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" x i8], ptr "));
        /* pass */
        LlvmGenerator_w(self, gname);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(", i32 0, i32 0\n"));
        /* pass */
        _tr_str_release(gname);
        return reg;
    } else if (_t2151.tag == HirExpr_ELitNone) {
        __auto_type _ = _t2151.data.ELitNone.ty;
        /* pass */
        return _tr_str_lit("null");
    } else if (_t2151.tag == HirExpr_EIdent) {
        __auto_type name = _t2151.data.EIdent.name;
__auto_type is_move = _t2151.data.EIdent.is_move;
        /* pass */
        TrStr reg = LlvmGenerator_next_reg(self);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    "));
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = load ptr, ptr %"));
        /* pass */
        LlvmGenerator_w(self, name);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        return reg;
    } else if (_t2151.tag == HirExpr_EBinOp) {
        __auto_type op = _t2151.data.EBinOp.op;
__auto_type left = _t2151.data.EBinOp.left;
__auto_type right = _t2151.data.EBinOp.right;
__auto_type ty = _t2151.data.EBinOp.ty;
        /* pass */
        return LlvmGenerator_gen_binop_llvm(self, op, left, right, ty);
    } else if (_t2151.tag == HirExpr_ECall) {
        __auto_type callee = _t2151.data.ECall.callee;
__auto_type args = _t2151.data.ECall.args;
        /* pass */
        return LlvmGenerator_gen_call_llvm(self, callee, args);
    } else if (1) {
        __auto_type _ = _t2151;
        /* pass */
        return _tr_str_lit("undef");
    }
}

__attribute__((hot)) TrStr LlvmGenerator_gen_binop_llvm(LlvmGenerator* self, TrStr op, HirExpr* left, HirExpr* right, AstType* ty) {
    /* pass */
    TrStr ls = LlvmGenerator_gen_expr(self, left);
    /* pass */
    TrStr rs = LlvmGenerator_gen_expr(self, right);
    /* pass */
    TrStr lty = llvm_type(ty);
    /* pass */
    TrStr reg = LlvmGenerator_next_reg(self);
    /* pass */
    TrStr instr = _tr_str_lit("add");
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2156 = _tr_str_lit("add");
        _tr_str_release(instr);
        instr = _strtmp_t2156;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2157 = _tr_str_lit("sub");
        _tr_str_release(instr);
        instr = _strtmp_t2157;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2158 = _tr_str_lit("mul");
        _tr_str_release(instr);
        instr = _strtmp_t2158;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2159 = _tr_str_lit("sdiv");
        _tr_str_release(instr);
        instr = _strtmp_t2159;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2160 = _tr_str_lit("srem");
        _tr_str_release(instr);
        instr = _strtmp_t2160;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2161 = _tr_str_lit("and");
        _tr_str_release(instr);
        instr = _strtmp_t2161;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("|"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2162 = _tr_str_lit("or");
        _tr_str_release(instr);
        instr = _strtmp_t2162;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("^"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2163 = _tr_str_lit("xor");
        _tr_str_release(instr);
        instr = _strtmp_t2163;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<<"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2164 = _tr_str_lit("shl");
        _tr_str_release(instr);
        instr = _strtmp_t2164;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">>"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2165 = _tr_str_lit("ashr");
        _tr_str_release(instr);
        instr = _strtmp_t2165;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        TrStr _strtmp_t2166 = _tr_str_lit("icmp eq");
        _tr_str_release(instr);
        instr = _strtmp_t2166;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        TrStr _strtmp_t2167 = _tr_str_lit("icmp ne");
        _tr_str_release(instr);
        instr = _strtmp_t2167;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2168 = _tr_str_lit("icmp slt");
        _tr_str_release(instr);
        instr = _strtmp_t2168;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        TrStr _strtmp_t2169 = _tr_str_lit("icmp sle");
        _tr_str_release(instr);
        instr = _strtmp_t2169;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        TrStr _strtmp_t2170 = _tr_str_lit("icmp sgt");
        _tr_str_release(instr);
        instr = _strtmp_t2170;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        TrStr _strtmp_t2171 = _tr_str_lit("icmp sge");
        _tr_str_release(instr);
        instr = _strtmp_t2171;
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("    "));
    /* pass */
    LlvmGenerator_w(self, reg);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(" = "));
    /* pass */
    LlvmGenerator_w(self, instr);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(" "));
    /* pass */
    LlvmGenerator_w(self, lty);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(" "));
    /* pass */
    LlvmGenerator_w(self, ls);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(", "));
    /* pass */
    LlvmGenerator_w(self, rs);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("\n"));
    /* pass */
    _tr_str_release(ls);
    _tr_str_release(rs);
    _tr_str_release(lty);
    _tr_str_release(instr);
    return reg;
}

__attribute__((hot)) TrStr LlvmGenerator_gen_call_llvm(LlvmGenerator* self, HirExpr* callee, List_ptr* args) {
    /* pass */
    TrStr reg = LlvmGenerator_next_reg(self);
    /* pass */
    TrStr callee_name = _tr_str_lit("");
    /* pass */
    __auto_type _t2172 = (*callee);
    if (_t2172.tag == HirExpr_EIdent) {
        __auto_type n = _t2172.data.EIdent.name;
__auto_type is_move = _t2172.data.EIdent.is_move;
        TrStr _strtmp_t2173 = _tr_str_retain(n);
        _tr_str_release(callee_name);
        callee_name = _strtmp_t2173;
    } else if (1) {
        __auto_type _ = _t2172;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((strcmp(_tr_strz(callee_name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t2174 = _tr_str_lit("unknown");
        _tr_str_release(callee_name);
        callee_name = _strtmp_t2174;
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("    "));
    /* pass */
    LlvmGenerator_w(self, reg);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(" = call ptr @"));
    /* pass */
    LlvmGenerator_w(self, callee_name);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("("));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(", "));
        }
        /* pass */
        TrStr arg_reg = LlvmGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, i)));
        /* pass */
        AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(args, i)));
        /* pass */
        ({ TrStr _at_t2175 = (llvm_type(arg_ty)); LlvmGenerator_w(self, _at_t2175); _tr_str_release(_at_t2175); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" "));
        /* pass */
        LlvmGenerator_w(self, arg_reg);
        /* pass */
        i = (i + 1LL);
        _tr_str_release(arg_reg);
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit(")\n"));
    /* pass */
    _tr_str_release(callee_name);
    return reg;
}

__attribute__((hot)) void LlvmGenerator_gen_stmt(LlvmGenerator* self, HirStmt* s_ptr) {
    /* pass */
    if ((((unsigned long long)(s_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type s = (*s_ptr);
    /* pass */
    __auto_type _t2176 = s;
    if (_t2176.tag == HirStmt_SExpr) {
        __auto_type e = _t2176.data.SExpr.expr;
        /* pass */
        LlvmGenerator_gen_expr(self, e);
    } else if (_t2176.tag == HirStmt_SReturn) {
        __auto_type e = _t2176.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("    ret void\n"));
        } else {
            /* pass */
            TrStr v = LlvmGenerator_gen_expr(self, e);
            /* pass */
            AstType* ty = hir_expr_type(e);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("    ret "));
            /* pass */
            ({ TrStr _at_t2177 = (llvm_type(ty)); LlvmGenerator_w(self, _at_t2177); _tr_str_release(_at_t2177); });
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(" "));
            /* pass */
            LlvmGenerator_w(self, v);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("\n"));
        }
    } else if (_t2176.tag == HirStmt_SLet) {
        __auto_type name = _t2176.data.SLet.name;
__auto_type ownership = _t2176.data.SLet.ownership;
__auto_type is_mut = _t2176.data.SLet.is_mut;
__auto_type is_const = _t2176.data.SLet.is_const;
__auto_type is_shared = _t2176.data.SLet.is_shared;
__auto_type ty = _t2176.data.SLet.ty;
__auto_type val = _t2176.data.SLet.val;
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    %"));
        /* pass */
        LlvmGenerator_w(self, name);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(" = alloca "));
        /* pass */
        ({ TrStr _at_t2178 = (llvm_type(ty)); LlvmGenerator_w(self, _at_t2178); _tr_str_release(_at_t2178); });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr v = LlvmGenerator_gen_expr(self, val);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("    store "));
            /* pass */
            ({ TrStr _at_t2179 = (llvm_type(ty)); LlvmGenerator_w(self, _at_t2179); _tr_str_release(_at_t2179); });
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(" "));
            /* pass */
            LlvmGenerator_w(self, v);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(", ptr %"));
            /* pass */
            LlvmGenerator_w(self, name);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("\n"));
        }
    } else if (_t2176.tag == HirStmt_SIf) {
        __auto_type cond = _t2176.data.SIf.cond;
__auto_type then_b = _t2176.data.SIf.then_b;
__auto_type else_b = _t2176.data.SIf.else_b;
        /* pass */
        TrStr cond_v = LlvmGenerator_gen_expr(self, cond);
        /* pass */
        TrStr then_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("then_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        TrStr else_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("else_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        TrStr end_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("end_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br i1 "));
        /* pass */
        LlvmGenerator_w(self, cond_v);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(", label %"));
        /* pass */
        LlvmGenerator_w(self, then_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(", label %"));
        /* pass */
        LlvmGenerator_w(self, else_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, then_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        /* pass */
        LlvmGenerator_gen_block(self, then_b);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br label %"));
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, else_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        /* pass */
        LlvmGenerator_gen_block(self, else_b);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br label %"));
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        _tr_str_release(cond_v);
        _tr_str_release(then_lbl);
        _tr_str_release(else_lbl);
        _tr_str_release(end_lbl);
    } else if (_t2176.tag == HirStmt_SWhile) {
        __auto_type cond = _t2176.data.SWhile.cond;
__auto_type body = _t2176.data.SWhile.body;
        /* pass */
        TrStr cond_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("while_cond_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        TrStr body_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("while_body_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        TrStr end_lbl = ({ TrStr _cr = (LlvmGenerator_next_reg(self)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("while_end_")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br label %"));
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        /* pass */
        TrStr cond_v = LlvmGenerator_gen_expr(self, cond);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br i1 "));
        /* pass */
        LlvmGenerator_w(self, cond_v);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(", label %"));
        /* pass */
        LlvmGenerator_w(self, body_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(", label %"));
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, body_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        /* pass */
        LlvmGenerator_gen_block(self, body);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    br label %"));
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("\n"));
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit(":\n"));
        _tr_str_release(cond_lbl);
        _tr_str_release(body_lbl);
        _tr_str_release(end_lbl);
        _tr_str_release(cond_v);
    } else if (_t2176.tag == HirStmt_SBreak) {
        __auto_type _ = _t2176.data.SBreak.val;
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    ; break (unresolved in stub)\n"));
    } else if (_t2176.tag == HirStmt_SContinue) {
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    ; continue (unresolved in stub)\n"));
    } else if (_t2176.tag == HirStmt_SPass) {
        /* pass */
        /* pass */
    } else if (1) {
        __auto_type _ = _t2176;
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    ; TODO stmt\n"));
    }
}

__attribute__((hot)) void LlvmGenerator_gen_block(LlvmGenerator* self, HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        LlvmGenerator_gen_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    LlvmGenerator__tr_fn_register(self, prog);
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("; Tauraro LLVM IR - generated by compiler/src/codegen/llvm.tr\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("source_filename = \"tauraro_module\"\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("target triple = \"x86_64-unknown-linux-gnu\"\n\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("declare void @printf(ptr, ...)\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("declare ptr @malloc(i64)\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("declare void @free(ptr)\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("declare ptr @memcpy(ptr, ptr, i64)\n\n"));
    /* pass */
    LlvmGenerator_emit_type_decls(self, prog);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (f->is_extern) {
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("declare "));
            /* pass */
            ({ TrStr _at_t2180 = (llvm_type(f->ret_ty)); LlvmGenerator_w(self, _at_t2180); _tr_str_release(_at_t2180); });
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit(" @"));
            /* pass */
            LlvmGenerator_w(self, f->name);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("(...)\n"));
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        LlvmGenerator_emit_func_sig(self, f, _tr_str_lit(""));
        /* pass */
        LlvmGenerator_gen_block(self, f->body);
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    ret void\n}\n\n"));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
            /* pass */
            LlvmGenerator_emit_func_sig(self, m, c->name);
            /* pass */
            LlvmGenerator_gen_block(self, m->body);
            /* pass */
            LlvmGenerator_w(self, _tr_str_lit("    ret void\n}\n\n"));
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("define internal void @_tr_init() {\nentry:\n"));
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->top_level_stmts->len)) {
        /* pass */
        LlvmGenerator_gen_stmt(self, ((HirStmt*)List_ptr_get(prog->top_level_stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("    ret void\n}\n\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("define i32 @main(i32 %argc, ptr %argv) {\nentry:\n"));
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("    call void @_tr_init()\n"));
    /* pass */
    bool has_main = false;
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((HirFunction*)List_ptr_get(prog->functions, i))->name), _tr_strz(_tr_str_lit("main"))) == 0)) {
            /* pass */
            has_main = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (has_main) {
        /* pass */
        LlvmGenerator_w(self, _tr_str_lit("    call void @_tr_main()\n"));
    }
    /* pass */
    LlvmGenerator_w(self, _tr_str_lit("    ret i32 0\n}\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->buf));
}

__attribute__((hot)) TrStr llvm_type(AstType* ty) {
    /* pass */
    __auto_type n = ty->name;
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("i1");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0)) {
        /* pass */
        return _tr_str_lit("i8");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0)) {
        /* pass */
        return _tr_str_lit("i8");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0)) {
        /* pass */
        return _tr_str_lit("i16");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0)) {
        /* pass */
        return _tr_str_lit("i16");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0)) {
        /* pass */
        return _tr_str_lit("i32");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) {
        /* pass */
        return _tr_str_lit("i32");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i64"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u64"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("isize"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("usize"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f32"))) == 0)) {
        /* pass */
        return _tr_str_lit("float");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("String"))) == 0)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("char"))) == 0)) {
        /* pass */
        return _tr_str_lit("i8");
    }
    /* pass */
    return _tr_str_lit("ptr");
}

