#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init() {
    /* pass */
    LlvmGenerator* g = ((LlvmGenerator*)_tr_checked_alloc(sizeof(LlvmGenerator)));
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

__attribute__((hot)) void LlvmGenerator_w(LlvmGenerator* self, char* s) {
    /* pass */
    StringBuilder_append(self->buf, s);
}

__attribute__((hot)) char* LlvmGenerator_next_reg(LlvmGenerator* self) {
    /* pass */
    self->temp = (self->temp + 1LL);
    /* pass */
    return _tr_str_concat("%_", _tr_int_to_str((long long)(self->temp)));
}

__attribute__((hot)) void LlvmGenerator__tr_fn_register(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(prog->classes, i));
        /* pass */
        _tr_dict_set(self->classes, c->name, c);
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
        _tr_dict_set(self->enums, e->name, e);
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
        _tr_dict_set(self->functions, f->name, f);
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
        LlvmGenerator_w(self, "%struct.");
        /* pass */
        LlvmGenerator_w(self, c->name);
        /* pass */
        LlvmGenerator_w(self, " = type { ");
        /* pass */
        bool first = true;
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < c->fields->len)) {
            /* pass */
            if ((!first)) {
                /* pass */
                LlvmGenerator_w(self, ", ");
            }
            /* pass */
            LlvmGenerator_w(self, llvm_type(((HirField*)List_ptr_get(c->fields, fi))->ty));
            /* pass */
            first = false;
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        if (first) {
            /* pass */
            LlvmGenerator_w(self, "i8");
        }
        /* pass */
        LlvmGenerator_w(self, " }\n");
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
        LlvmGenerator_w(self, "%enum.");
        /* pass */
        LlvmGenerator_w(self, e->name);
        /* pass */
        LlvmGenerator_w(self, " = type { i32, [64 x i8] }\n");
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((prog->classes->len > 0LL) || (prog->enums->len > 0LL))) {
        /* pass */
        LlvmGenerator_w(self, "\n");
    }
}

__attribute__((hot)) void LlvmGenerator_emit_func_sig(LlvmGenerator* self, HirFunction* f, char* class_name) {
    /* pass */
    char* ret_ty = llvm_type(f->ret_ty);
    /* pass */
    char* fname = f->name;
    /* pass */
    if ((strcmp((char*)fname, (char*)"main") == 0)) {
        /* pass */
        fname = "_tr_main";
    }
    /* pass */
    LlvmGenerator_w(self, "define ");
    /* pass */
    if ((!f->is_public)) {
        /* pass */
        LlvmGenerator_w(self, "internal ");
    }
    /* pass */
    LlvmGenerator_w(self, ret_ty);
    /* pass */
    LlvmGenerator_w(self, " @");
    /* pass */
    LlvmGenerator_w(self, fname);
    /* pass */
    LlvmGenerator_w(self, "(");
    /* pass */
    bool first = true;
    /* pass */
    if ((strcmp((char*)class_name, (char*)"") != 0)) {
        /* pass */
        bool has_self = false;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < f->params->len)) {
            /* pass */
            if ((strcmp((char*)((HirParam*)List_ptr_get(f->params, i))->name, (char*)"self") == 0)) {
                /* pass */
                has_self = true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if ((has_self && (!f->is_static))) {
            /* pass */
            LlvmGenerator_w(self, "ptr %self");
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
        if ((strcmp((char*)p->name, (char*)"self") == 0)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            LlvmGenerator_w(self, ", ");
        }
        /* pass */
        LlvmGenerator_w(self, llvm_type(p->ty));
        /* pass */
        LlvmGenerator_w(self, " %");
        /* pass */
        LlvmGenerator_w(self, p->name);
        /* pass */
        first = false;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, ") {\nentry:\n");
}

__attribute__((hot)) char* LlvmGenerator_gen_expr(LlvmGenerator* self, HirExpr* e_ptr) {
    /* pass */
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return "null";
    }
    /* pass */
    __auto_type e = (*e_ptr);
    /* pass */
    __auto_type _t256 = e;
    if (_t256.tag == HirExpr_ELitInt) {
        __auto_type v = _t256.data.ELitInt.val;
__auto_type ty = _t256.data.ELitInt.ty;
        /* pass */
        char* reg = LlvmGenerator_next_reg(self);
        /* pass */
        char* lty = llvm_type(ty);
        /* pass */
        LlvmGenerator_w(self, "    ");
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, " = add ");
        /* pass */
        LlvmGenerator_w(self, lty);
        /* pass */
        LlvmGenerator_w(self, " 0, ");
        /* pass */
        LlvmGenerator_w(self, _tr_int_to_str((long long)(v)));
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        return reg;
    } else if (_t256.tag == HirExpr_ELitBool) {
        __auto_type v = _t256.data.ELitBool.val;
        /* pass */
        char* reg = LlvmGenerator_next_reg(self);
        /* pass */
        char* bval = "0";
        /* pass */
        if (v) {
            /* pass */
            bval = "1";
        }
        /* pass */
        LlvmGenerator_w(self, "    ");
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, " = add i1 0, ");
        /* pass */
        LlvmGenerator_w(self, bval);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        return reg;
    } else if (_t256.tag == HirExpr_ELitFloat) {
        __auto_type v = _t256.data.ELitFloat.val;
__auto_type ty = _t256.data.ELitFloat.ty;
        /* pass */
        char* reg = LlvmGenerator_next_reg(self);
        /* pass */
        LlvmGenerator_w(self, "    ");
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, " = fadd double 0.0, ");
        /* pass */
        LlvmGenerator_w(self, _tr_float_to_str((double)(v)));
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        return reg;
    } else if (_t256.tag == HirExpr_ELitStr) {
        __auto_type v = _t256.data.ELitStr.val;
        /* pass */
        char* reg = LlvmGenerator_next_reg(self);
        /* pass */
        char* gname = _tr_str_concat("@.str.", _tr_int_to_str((long long)(self->temp)));
        /* pass */
        long long len = (_tr_strlen(v) + 1LL);
        /* pass */
        LlvmGenerator_w(self, "    ");
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, " = getelementptr [");
        /* pass */
        LlvmGenerator_w(self, _tr_int_to_str((long long)(len)));
        /* pass */
        LlvmGenerator_w(self, " x i8], ptr ");
        /* pass */
        LlvmGenerator_w(self, gname);
        /* pass */
        LlvmGenerator_w(self, ", i32 0, i32 0\n");
        /* pass */
        return reg;
    } else if (_t256.tag == HirExpr_ELitNone) {
        __auto_type _ = _t256.data.ELitNone.ty;
        /* pass */
        return "null";
    } else if (_t256.tag == HirExpr_EIdent) {
        __auto_type name = _t256.data.EIdent.name;
__auto_type is_move = _t256.data.EIdent.is_move;
        /* pass */
        char* reg = LlvmGenerator_next_reg(self);
        /* pass */
        LlvmGenerator_w(self, "    ");
        /* pass */
        LlvmGenerator_w(self, reg);
        /* pass */
        LlvmGenerator_w(self, " = load ptr, ptr %");
        /* pass */
        LlvmGenerator_w(self, name);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        return reg;
    } else if (_t256.tag == HirExpr_EBinOp) {
        __auto_type op = _t256.data.EBinOp.op;
__auto_type left = _t256.data.EBinOp.left;
__auto_type right = _t256.data.EBinOp.right;
__auto_type ty = _t256.data.EBinOp.ty;
        /* pass */
        return LlvmGenerator_gen_binop_llvm(self, op, left, right, ty);
    } else if (_t256.tag == HirExpr_ECall) {
        __auto_type callee = _t256.data.ECall.callee;
__auto_type args = _t256.data.ECall.args;
        /* pass */
        return LlvmGenerator_gen_call_llvm(self, callee, args);
    } else if (1) {
        __auto_type _ = _t256;
        /* pass */
        return "undef";
    }
}

__attribute__((hot)) char* LlvmGenerator_gen_binop_llvm(LlvmGenerator* self, char* op, HirExpr* left, HirExpr* right, AstType* ty) {
    /* pass */
    char* ls = LlvmGenerator_gen_expr(self, left);
    /* pass */
    char* rs = LlvmGenerator_gen_expr(self, right);
    /* pass */
    char* lty = llvm_type(ty);
    /* pass */
    char* reg = LlvmGenerator_next_reg(self);
    /* pass */
    char* instr = "add";
    /* pass */
    if ((strcmp((char*)op, (char*)"+") == 0)) {
        /* pass */
        instr = "add";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"-") == 0)) {
        /* pass */
        instr = "sub";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"*") == 0)) {
        /* pass */
        instr = "mul";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"/") == 0)) {
        /* pass */
        instr = "sdiv";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"%") == 0)) {
        /* pass */
        instr = "srem";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"&") == 0)) {
        /* pass */
        instr = "and";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"|") == 0)) {
        /* pass */
        instr = "or";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"^") == 0)) {
        /* pass */
        instr = "xor";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"<<") == 0)) {
        /* pass */
        instr = "shl";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)">>") == 0)) {
        /* pass */
        instr = "ashr";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"==") == 0)) {
        /* pass */
        instr = "icmp eq";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"!=") == 0)) {
        /* pass */
        instr = "icmp ne";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"<") == 0)) {
        /* pass */
        instr = "icmp slt";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)"<=") == 0)) {
        /* pass */
        instr = "icmp sle";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)">") == 0)) {
        /* pass */
        instr = "icmp sgt";
    }
    /* pass */
    if ((strcmp((char*)op, (char*)">=") == 0)) {
        /* pass */
        instr = "icmp sge";
    }
    /* pass */
    LlvmGenerator_w(self, "    ");
    /* pass */
    LlvmGenerator_w(self, reg);
    /* pass */
    LlvmGenerator_w(self, " = ");
    /* pass */
    LlvmGenerator_w(self, instr);
    /* pass */
    LlvmGenerator_w(self, " ");
    /* pass */
    LlvmGenerator_w(self, lty);
    /* pass */
    LlvmGenerator_w(self, " ");
    /* pass */
    LlvmGenerator_w(self, ls);
    /* pass */
    LlvmGenerator_w(self, ", ");
    /* pass */
    LlvmGenerator_w(self, rs);
    /* pass */
    LlvmGenerator_w(self, "\n");
    /* pass */
    return reg;
}

__attribute__((hot)) char* LlvmGenerator_gen_call_llvm(LlvmGenerator* self, HirExpr* callee, List_ptr* args) {
    /* pass */
    char* reg = LlvmGenerator_next_reg(self);
    /* pass */
    char* callee_name = "";
    /* pass */
    __auto_type _t257 = (*callee);
    if (_t257.tag == HirExpr_EIdent) {
        __auto_type n = _t257.data.EIdent.name;
__auto_type is_move = _t257.data.EIdent.is_move;
        callee_name = n;
    } else if (1) {
        __auto_type _ = _t257;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((strcmp((char*)callee_name, (char*)"") == 0)) {
        /* pass */
        callee_name = "unknown";
    }
    /* pass */
    LlvmGenerator_w(self, "    ");
    /* pass */
    LlvmGenerator_w(self, reg);
    /* pass */
    LlvmGenerator_w(self, " = call ptr @");
    /* pass */
    LlvmGenerator_w(self, callee_name);
    /* pass */
    LlvmGenerator_w(self, "(");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            LlvmGenerator_w(self, ", ");
        }
        /* pass */
        char* arg_reg = LlvmGenerator_gen_expr(self, ((HirExpr*)List_ptr_get(args, i)));
        /* pass */
        AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(args, i)));
        /* pass */
        LlvmGenerator_w(self, llvm_type(arg_ty));
        /* pass */
        LlvmGenerator_w(self, " ");
        /* pass */
        LlvmGenerator_w(self, arg_reg);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, ")\n");
    /* pass */
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
    __auto_type _t258 = s;
    if (_t258.tag == HirStmt_SExpr) {
        __auto_type e = _t258.data.SExpr.expr;
        /* pass */
        LlvmGenerator_gen_expr(self, e);
    } else if (_t258.tag == HirStmt_SReturn) {
        __auto_type e = _t258.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
            /* pass */
            LlvmGenerator_w(self, "    ret void\n");
        } else {
            /* pass */
            char* v = LlvmGenerator_gen_expr(self, e);
            /* pass */
            AstType* ty = hir_expr_type(e);
            /* pass */
            LlvmGenerator_w(self, "    ret ");
            /* pass */
            LlvmGenerator_w(self, llvm_type(ty));
            /* pass */
            LlvmGenerator_w(self, " ");
            /* pass */
            LlvmGenerator_w(self, v);
            /* pass */
            LlvmGenerator_w(self, "\n");
        }
    } else if (_t258.tag == HirStmt_SLet) {
        __auto_type name = _t258.data.SLet.name;
__auto_type ownership = _t258.data.SLet.ownership;
__auto_type is_mut = _t258.data.SLet.is_mut;
__auto_type is_const = _t258.data.SLet.is_const;
__auto_type is_shared = _t258.data.SLet.is_shared;
__auto_type ty = _t258.data.SLet.ty;
__auto_type val = _t258.data.SLet.val;
        /* pass */
        LlvmGenerator_w(self, "    %");
        /* pass */
        LlvmGenerator_w(self, name);
        /* pass */
        LlvmGenerator_w(self, " = alloca ");
        /* pass */
        LlvmGenerator_w(self, llvm_type(ty));
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            char* v = LlvmGenerator_gen_expr(self, val);
            /* pass */
            LlvmGenerator_w(self, "    store ");
            /* pass */
            LlvmGenerator_w(self, llvm_type(ty));
            /* pass */
            LlvmGenerator_w(self, " ");
            /* pass */
            LlvmGenerator_w(self, v);
            /* pass */
            LlvmGenerator_w(self, ", ptr %");
            /* pass */
            LlvmGenerator_w(self, name);
            /* pass */
            LlvmGenerator_w(self, "\n");
        }
    } else if (_t258.tag == HirStmt_SIf) {
        __auto_type cond = _t258.data.SIf.cond;
__auto_type then_b = _t258.data.SIf.then_b;
__auto_type else_b = _t258.data.SIf.else_b;
        /* pass */
        char* cond_v = LlvmGenerator_gen_expr(self, cond);
        /* pass */
        char* then_lbl = _tr_str_concat("then_", LlvmGenerator_next_reg(self));
        /* pass */
        char* else_lbl = _tr_str_concat("else_", LlvmGenerator_next_reg(self));
        /* pass */
        char* end_lbl = _tr_str_concat("end_", LlvmGenerator_next_reg(self));
        /* pass */
        LlvmGenerator_w(self, "    br i1 ");
        /* pass */
        LlvmGenerator_w(self, cond_v);
        /* pass */
        LlvmGenerator_w(self, ", label %");
        /* pass */
        LlvmGenerator_w(self, then_lbl);
        /* pass */
        LlvmGenerator_w(self, ", label %");
        /* pass */
        LlvmGenerator_w(self, else_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, then_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
        /* pass */
        LlvmGenerator_gen_block(self, then_b);
        /* pass */
        LlvmGenerator_w(self, "    br label %");
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, else_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
        /* pass */
        LlvmGenerator_gen_block(self, else_b);
        /* pass */
        LlvmGenerator_w(self, "    br label %");
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
    } else if (_t258.tag == HirStmt_SWhile) {
        __auto_type cond = _t258.data.SWhile.cond;
__auto_type body = _t258.data.SWhile.body;
        /* pass */
        char* cond_lbl = _tr_str_concat("while_cond_", LlvmGenerator_next_reg(self));
        /* pass */
        char* body_lbl = _tr_str_concat("while_body_", LlvmGenerator_next_reg(self));
        /* pass */
        char* end_lbl = _tr_str_concat("while_end_", LlvmGenerator_next_reg(self));
        /* pass */
        LlvmGenerator_w(self, "    br label %");
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
        /* pass */
        char* cond_v = LlvmGenerator_gen_expr(self, cond);
        /* pass */
        LlvmGenerator_w(self, "    br i1 ");
        /* pass */
        LlvmGenerator_w(self, cond_v);
        /* pass */
        LlvmGenerator_w(self, ", label %");
        /* pass */
        LlvmGenerator_w(self, body_lbl);
        /* pass */
        LlvmGenerator_w(self, ", label %");
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, body_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
        /* pass */
        LlvmGenerator_gen_block(self, body);
        /* pass */
        LlvmGenerator_w(self, "    br label %");
        /* pass */
        LlvmGenerator_w(self, cond_lbl);
        /* pass */
        LlvmGenerator_w(self, "\n");
        /* pass */
        LlvmGenerator_w(self, end_lbl);
        /* pass */
        LlvmGenerator_w(self, ":\n");
    } else if (_t258.tag == HirStmt_SBreak) {
        /* pass */
        LlvmGenerator_w(self, "    ; break (unresolved in stub)\n");
    } else if (_t258.tag == HirStmt_SContinue) {
        /* pass */
        LlvmGenerator_w(self, "    ; continue (unresolved in stub)\n");
    } else if (_t258.tag == HirStmt_SPass) {
        /* pass */
        /* pass */
    } else if (1) {
        __auto_type _ = _t258;
        /* pass */
        LlvmGenerator_w(self, "    ; TODO stmt\n");
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

__attribute__((hot)) char* LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    LlvmGenerator__tr_fn_register(self, prog);
    /* pass */
    LlvmGenerator_w(self, "; Tauraro LLVM IR - generated by compiler/src/codegen/llvm.tr\n");
    /* pass */
    LlvmGenerator_w(self, "source_filename = \"tauraro_module\"\n");
    /* pass */
    LlvmGenerator_w(self, "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n");
    /* pass */
    LlvmGenerator_w(self, "target triple = \"x86_64-unknown-linux-gnu\"\n\n");
    /* pass */
    LlvmGenerator_w(self, "declare void @printf(ptr, ...)\n");
    /* pass */
    LlvmGenerator_w(self, "declare ptr @malloc(i64)\n");
    /* pass */
    LlvmGenerator_w(self, "declare void @free(ptr)\n");
    /* pass */
    LlvmGenerator_w(self, "declare ptr @memcpy(ptr, ptr, i64)\n\n");
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
            LlvmGenerator_w(self, "declare ");
            /* pass */
            LlvmGenerator_w(self, llvm_type(f->ret_ty));
            /* pass */
            LlvmGenerator_w(self, " @");
            /* pass */
            LlvmGenerator_w(self, f->name);
            /* pass */
            LlvmGenerator_w(self, "(...)\n");
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        LlvmGenerator_emit_func_sig(self, f, "");
        /* pass */
        LlvmGenerator_gen_block(self, f->body);
        /* pass */
        LlvmGenerator_w(self, "    ret void\n}\n\n");
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
            LlvmGenerator_w(self, "    ret void\n}\n\n");
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    LlvmGenerator_w(self, "define internal void @_tr_init() {\nentry:\n");
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
    LlvmGenerator_w(self, "    ret void\n}\n\n");
    /* pass */
    LlvmGenerator_w(self, "define i32 @main(i32 %argc, ptr %argv) {\nentry:\n");
    /* pass */
    LlvmGenerator_w(self, "    call void @_tr_init()\n");
    /* pass */
    bool has_main = false;
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        if ((strcmp((char*)((HirFunction*)List_ptr_get(prog->functions, i))->name, (char*)"main") == 0)) {
            /* pass */
            has_main = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (has_main) {
        /* pass */
        LlvmGenerator_w(self, "    call void @_tr_main()\n");
    }
    /* pass */
    LlvmGenerator_w(self, "    ret i32 0\n}\n");
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->buf));
}

__attribute__((hot)) char* llvm_type(AstType* ty) {
    /* pass */
    __auto_type n = ty->name;
    /* pass */
    if ((strcmp((char*)n, (char*)"void") == 0)) {
        /* pass */
        return "void";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"bool") == 0)) {
        /* pass */
        return "i1";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i8") == 0)) {
        /* pass */
        return "i8";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u8") == 0)) {
        /* pass */
        return "i8";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i16") == 0)) {
        /* pass */
        return "i16";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u16") == 0)) {
        /* pass */
        return "i16";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i32") == 0)) {
        /* pass */
        return "i32";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u32") == 0)) {
        /* pass */
        return "i32";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"int") == 0)) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"i64") == 0)) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"u64") == 0)) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"isize") == 0)) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"usize") == 0)) {
        /* pass */
        return "i64";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"f32") == 0)) {
        /* pass */
        return "float";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"float") == 0)) {
        /* pass */
        return "double";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"f64") == 0)) {
        /* pass */
        return "double";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"str") == 0)) {
        /* pass */
        return "ptr";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"String") == 0)) {
        /* pass */
        return "ptr";
    }
    /* pass */
    if ((strcmp((char*)n, (char*)"char") == 0)) {
        /* pass */
        return "i8";
    }
    /* pass */
    return "ptr";
}

