#include "tauraro_types.h"

long long _argreg_modrm(long long idx);
long long _argreg_rex(long long idx);
long long _round16(long long n);
long long _vreg_disp(long long id);
long long _var_disp(LFunc* lf, TrStr name);
void _st_rax(ByteBuf* c, long long disp);
void _ld_rax(ByteBuf* c, long long disp);
void _ld_rcx(ByteBuf* c, long long disp);
void _ld_argreg(ByteBuf* c, long long idx, long long disp);
void _st_argreg(ByteBuf* c, long long idx, long long disp);
void _mov_rax_imm64(ByteBuf* c, long long v);
void _ld_xmm0(ByteBuf* c, long long disp);
void _ld_xmm1(ByteBuf* c, long long disp);
void _st_xmm0(ByteBuf* c, long long disp);
void _emit_farith(ByteBuf* c, TrStr op);
long long _fsetcc(TrStr op);
bool _is_cmp(TrStr op);
long long _setcc(TrStr op);
void _emit_arith(ByteBuf* c, TrStr op);
void _emit_return(ByteBuf* c, long long rv);

__attribute__((malloc,returns_nonnull,hot)) EncodedFunc* EncodedFunc_init(TrStr name) {
#line 34 "src/codegen/native/isa_x64.tr"
    EncodedFunc* e = ((EncodedFunc*)_tr_obj_alloc(sizeof(EncodedFunc)));
#line 35 "src/codegen/native/isa_x64.tr"
    e->name = _tr_str_retain(name);
#line 36 "src/codegen/native/isa_x64.tr"
    e->is_main = false;
#line 37 "src/codegen/native/isa_x64.tr"
    ByteBuf* _cltmp_t7194 = ByteBuf_init();
    _tr_obj_release(e->code, _trdrop_ByteBuf);
    e->code = _cltmp_t7194;
#line 38 "src/codegen/native/isa_x64.tr"
    e->relocs = (void*)List_ptr_new();
#line 39 "src/codegen/native/isa_x64.tr"
    return e;
}

__attribute__((hot)) long long _argreg_modrm(long long idx) {
#line 45 "src/codegen/native/isa_x64.tr"
    if ((idx == 0LL)) {
#line 45 "src/codegen/native/isa_x64.tr"
        return 189LL;
    }
#line 46 "src/codegen/native/isa_x64.tr"
    if ((idx == 1LL)) {
#line 46 "src/codegen/native/isa_x64.tr"
        return 181LL;
    }
#line 47 "src/codegen/native/isa_x64.tr"
    if ((idx == 2LL)) {
#line 47 "src/codegen/native/isa_x64.tr"
        return 149LL;
    }
#line 48 "src/codegen/native/isa_x64.tr"
    if ((idx == 3LL)) {
#line 48 "src/codegen/native/isa_x64.tr"
        return 141LL;
    }
#line 49 "src/codegen/native/isa_x64.tr"
    if ((idx == 4LL)) {
#line 49 "src/codegen/native/isa_x64.tr"
        return 133LL;
    }
#line 50 "src/codegen/native/isa_x64.tr"
    return 141LL;
}

__attribute__((hot)) long long _argreg_rex(long long idx) {
#line 54 "src/codegen/native/isa_x64.tr"
    if ((idx >= 4LL)) {
#line 54 "src/codegen/native/isa_x64.tr"
        return 76LL;
    }
#line 55 "src/codegen/native/isa_x64.tr"
    return 72LL;
}

__attribute__((hot)) long long _round16(long long n) {
#line 58 "src/codegen/native/isa_x64.tr"
    long long r = n;
#line 59 "src/codegen/native/isa_x64.tr"
    while ((_tr_imod((long long)(r), (long long)(16LL)) != 0LL)) {
#line 59 "src/codegen/native/isa_x64.tr"
        r = (r + 1LL);
    }
#line 60 "src/codegen/native/isa_x64.tr"
    return r;
}

__attribute__((hot)) long long _vreg_disp(long long id) {
#line 63 "src/codegen/native/isa_x64.tr"
    return (0LL - (8LL * (id + 1LL)));
}

__attribute__((hot)) long long _var_disp(LFunc* lf, TrStr name) {
#line 66 "src/codegen/native/isa_x64.tr"
    long long idx = LFunc_var_index(lf, name);
#line 67 "src/codegen/native/isa_x64.tr"
    if ((idx < 0LL)) {
#line 67 "src/codegen/native/isa_x64.tr"
        idx = 0LL;
    }
#line 68 "src/codegen/native/isa_x64.tr"
    return (0LL - (8LL * ((lf->n_vregs + idx) + 1LL)));
}

__attribute__((hot)) void _st_rax(ByteBuf* c, long long disp) {
#line 71 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 72LL);
#line 72 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 137LL);
#line 73 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 133LL);
#line 74 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rax(ByteBuf* c, long long disp) {
#line 77 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 72LL);
#line 78 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 139LL);
#line 79 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 133LL);
#line 80 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rcx(ByteBuf* c, long long disp) {
#line 83 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 72LL);
#line 84 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 139LL);
#line 85 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 141LL);
#line 86 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_argreg(ByteBuf* c, long long idx, long long disp) {
#line 89 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, _argreg_rex(idx));
#line 90 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 139LL);
#line 91 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, _argreg_modrm(idx));
#line 92 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _st_argreg(ByteBuf* c, long long idx, long long disp) {
#line 95 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, _argreg_rex(idx));
#line 96 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 137LL);
#line 97 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, _argreg_modrm(idx));
#line 98 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _mov_rax_imm64(ByteBuf* c, long long v) {
#line 101 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 72LL);
#line 102 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 184LL);
#line 103 "src/codegen/native/isa_x64.tr"
    ByteBuf_u64(c, v);
}

__attribute__((hot)) void _ld_xmm0(ByteBuf* c, long long disp) {
#line 107 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 242LL);
#line 108 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 15LL);
#line 109 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 16LL);
#line 110 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 133LL);
#line 111 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_xmm1(ByteBuf* c, long long disp) {
#line 114 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 242LL);
#line 115 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 15LL);
#line 116 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 16LL);
#line 117 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 141LL);
#line 118 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _st_xmm0(ByteBuf* c, long long disp) {
#line 121 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 242LL);
#line 122 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 15LL);
#line 123 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 17LL);
#line 124 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 133LL);
#line 125 "src/codegen/native/isa_x64.tr"
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _emit_farith(ByteBuf* c, TrStr op) {
#line 129 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 242LL);
#line 130 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 15LL);
#line 131 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 131 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 88LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 132 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 92LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 133 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 89LL);
    } else {
#line 134 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 94LL);
    }
#line 135 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 193LL);
}

__attribute__((hot)) long long _fsetcc(TrStr op) {
#line 139 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 139 "src/codegen/native/isa_x64.tr"
        return 146LL;
    }
#line 140 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 140 "src/codegen/native/isa_x64.tr"
        return 151LL;
    }
#line 141 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 141 "src/codegen/native/isa_x64.tr"
        return 148LL;
    }
#line 142 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 142 "src/codegen/native/isa_x64.tr"
        return 149LL;
    }
#line 143 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 143 "src/codegen/native/isa_x64.tr"
        return 150LL;
    }
#line 144 "src/codegen/native/isa_x64.tr"
    return 147LL;
}

__attribute__((hot)) bool _is_cmp(TrStr op) {
#line 147 "src/codegen/native/isa_x64.tr"
    return (((((_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) || _tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL))));
}

__attribute__((hot)) long long _setcc(TrStr op) {
#line 151 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 151 "src/codegen/native/isa_x64.tr"
        return 156LL;
    }
#line 152 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 152 "src/codegen/native/isa_x64.tr"
        return 159LL;
    }
#line 153 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 153 "src/codegen/native/isa_x64.tr"
        return 148LL;
    }
#line 154 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 154 "src/codegen/native/isa_x64.tr"
        return 149LL;
    }
#line 155 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 155 "src/codegen/native/isa_x64.tr"
        return 158LL;
    }
#line 156 "src/codegen/native/isa_x64.tr"
    return 157LL;
}

__attribute__((hot)) void _emit_arith(ByteBuf* c, TrStr op) {
#line 160 "src/codegen/native/isa_x64.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 161 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 162 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 1LL);
#line 163 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 200LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 165 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 166 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 41LL);
#line 167 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 200LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 169 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 170 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 15LL);
#line 171 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 175LL);
#line 172 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 193LL);
    } else if ((_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL))) || _tr_str_eqv((op), (_tr_str_lit_len("//", 2LL))))) {
#line 174 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 175 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 153LL);
#line 176 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 177 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 247LL);
#line 178 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 249LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
#line 180 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 181 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 153LL);
#line 182 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 183 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 247LL);
#line 184 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 249LL);
#line 185 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 186 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 137LL);
#line 187 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 208LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 189 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 190 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 33LL);
#line 191 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 200LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("|", 1LL)))) {
#line 193 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 194 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 9LL);
#line 195 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 200LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) {
#line 197 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 198 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 49LL);
#line 199 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 200LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) {
#line 201 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 202 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 211LL);
#line 203 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 224LL);
    } else if (_tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL)))) {
#line 205 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 206 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 211LL);
#line 207 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 248LL);
    }
}

__attribute__((hot)) void _emit_return(ByteBuf* c, long long rv) {
#line 210 "src/codegen/native/isa_x64.tr"
    if ((rv == 0LL)) {
#line 211 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 49LL);
#line 212 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 192LL);
    } else {
#line 214 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 184LL);
#line 215 "src/codegen/native/isa_x64.tr"
        ByteBuf_u32(c, rv);
    }
#line 216 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 201LL);
#line 217 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 195LL);
}

__attribute__((hot)) EncodedFunc* encode_func(LFunc* lf) {
#line 220 "src/codegen/native/isa_x64.tr"
    EncodedFunc* e = EncodedFunc_init(lf->name);
#line 221 "src/codegen/native/isa_x64.tr"
    e->is_main = lf->is_main;
#line 222 "src/codegen/native/isa_x64.tr"
    ByteBuf* c = _tr_obj_retain(e->code);
#line 224 "src/codegen/native/isa_x64.tr"
    long long framesize = _round16(((lf->n_vregs + lf->vars->len) * 8LL));
#line 227 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 85LL);
#line 228 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 72LL);
#line 229 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 137LL);
#line 230 "src/codegen/native/isa_x64.tr"
    ByteBuf_u8(c, 229LL);
#line 231 "src/codegen/native/isa_x64.tr"
    if ((framesize > 0LL)) {
#line 232 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 72LL);
#line 233 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 129LL);
#line 234 "src/codegen/native/isa_x64.tr"
        ByteBuf_u8(c, 236LL);
#line 235 "src/codegen/native/isa_x64.tr"
        ByteBuf_u32(c, framesize);
    }
#line 238 "src/codegen/native/isa_x64.tr"
    long long ppi = 0LL;
#line 239 "src/codegen/native/isa_x64.tr"
    while ((ppi < lf->params->len)) {
#line 240 "src/codegen/native/isa_x64.tr"
        ({ TrStr _at_t7195 = (List_TrStr_get(lf->params, ppi)); _st_argreg(c, ppi, _var_disp(lf, _at_t7195)); _tr_str_release(_at_t7195); });
#line 241 "src/codegen/native/isa_x64.tr"
        ppi = (ppi + 1LL);
    }
#line 243 "src/codegen/native/isa_x64.tr"
    List_i64* block_start = (void*)List_i64_new();
#line 244 "src/codegen/native/isa_x64.tr"
    List_ptr* jumps = (void*)List_ptr_new();
#line 246 "src/codegen/native/isa_x64.tr"
    long long bi = 0LL;
#line 247 "src/codegen/native/isa_x64.tr"
    while ((bi < lf->blocks->len)) {
#line 248 "src/codegen/native/isa_x64.tr"
        LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
#line 249 "src/codegen/native/isa_x64.tr"
        List_i64_append(block_start, c->len);
#line 251 "src/codegen/native/isa_x64.tr"
        long long ii = 0LL;
#line 252 "src/codegen/native/isa_x64.tr"
        while ((ii < blk->insts->len)) {
#line 253 "src/codegen/native/isa_x64.tr"
            __auto_type _t7196 = (*((LInst*)List_ptr_get(blk->insts, ii)));
            if (_t7196.tag == LInst_IConst) {
                __auto_type dst = _t7196.data.IConst.dst;
__auto_type v = _t7196.data.IConst.v;
#line 255 "src/codegen/native/isa_x64.tr"
                _mov_rax_imm64(c, v);
#line 256 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IStr) {
                __auto_type dst = _t7196.data.IStr.dst;
__auto_type sidx = _t7196.data.IStr.str_idx;
#line 258 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 259 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 141LL);
#line 260 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 5LL);
#line 261 "src/codegen/native/isa_x64.tr"
                Reloc* sr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 262 "src/codegen/native/isa_x64.tr"
                sr->offset = c->len;
#line 263 "src/codegen/native/isa_x64.tr"
                sr->symbol = _tr_str_lit_len("", 0LL);
#line 264 "src/codegen/native/isa_x64.tr"
                sr->kind = 1LL;
#line 265 "src/codegen/native/isa_x64.tr"
                sr->str_idx = sidx;
#line 266 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(sr));
#line 267 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 268 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(sr, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_ILoadVar) {
                __auto_type dst = _t7196.data.ILoadVar.dst;
__auto_type name = _t7196.data.ILoadVar.name;
#line 270 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _var_disp(lf, name));
#line 271 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IStoreVar) {
                __auto_type name = _t7196.data.IStoreVar.name;
__auto_type src = _t7196.data.IStoreVar.src;
#line 273 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(src));
#line 274 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _var_disp(lf, name));
            } else if (_t7196.tag == LInst_ILoadGlobal) {
                __auto_type dst = _t7196.data.ILoadGlobal.dst;
__auto_type gidx = _t7196.data.ILoadGlobal.gidx;
#line 276 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 277 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 139LL);
#line 278 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 5LL);
#line 279 "src/codegen/native/isa_x64.tr"
                Reloc* glr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 280 "src/codegen/native/isa_x64.tr"
                glr->offset = c->len;
#line 281 "src/codegen/native/isa_x64.tr"
                glr->symbol = _tr_str_lit_len("", 0LL);
#line 282 "src/codegen/native/isa_x64.tr"
                glr->kind = 3LL;
#line 283 "src/codegen/native/isa_x64.tr"
                glr->str_idx = gidx;
#line 284 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(glr));
#line 285 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 286 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(glr, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IStoreGlobal) {
                __auto_type gidx = _t7196.data.IStoreGlobal.gidx;
__auto_type src = _t7196.data.IStoreGlobal.src;
#line 288 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(src));
#line 289 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 290 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 137LL);
#line 291 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 5LL);
#line 292 "src/codegen/native/isa_x64.tr"
                Reloc* gsr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 293 "src/codegen/native/isa_x64.tr"
                gsr->offset = c->len;
#line 294 "src/codegen/native/isa_x64.tr"
                gsr->symbol = _tr_str_lit_len("", 0LL);
#line 295 "src/codegen/native/isa_x64.tr"
                gsr->kind = 3LL;
#line 296 "src/codegen/native/isa_x64.tr"
                gsr->str_idx = gidx;
#line 297 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(gsr));
#line 298 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
                _tr_obj_release(gsr, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IBinOp) {
                __auto_type dst = _t7196.data.IBinOp.dst;
__auto_type op = _t7196.data.IBinOp.op;
__auto_type a = _t7196.data.IBinOp.a;
__auto_type b = _t7196.data.IBinOp.b;
#line 300 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(a));
#line 301 "src/codegen/native/isa_x64.tr"
                _ld_rcx(c, _vreg_disp(b));
#line 302 "src/codegen/native/isa_x64.tr"
                if (_is_cmp(op)) {
#line 303 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 72LL);
#line 304 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 57LL);
#line 305 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 200LL);
#line 306 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 15LL);
#line 307 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, _setcc(op));
#line 308 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 192LL);
#line 309 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 72LL);
#line 310 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 15LL);
#line 311 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 182LL);
#line 312 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 192LL);
                } else {
#line 314 "src/codegen/native/isa_x64.tr"
                    _emit_arith(c, op);
                }
#line 315 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_ICall) {
                __auto_type dst = _t7196.data.ICall.dst;
__auto_type callee = _t7196.data.ICall.callee;
__auto_type args = _t7196.data.ICall.args;
#line 317 "src/codegen/native/isa_x64.tr"
                long long ai = 0LL;
#line 318 "src/codegen/native/isa_x64.tr"
                while ((ai < args->len)) {
#line 319 "src/codegen/native/isa_x64.tr"
                    _ld_argreg(c, ai, _vreg_disp(List_i64_get(args, ai)));
#line 320 "src/codegen/native/isa_x64.tr"
                    ai = (ai + 1LL);
                }
#line 321 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 232LL);
#line 322 "src/codegen/native/isa_x64.tr"
                Reloc* r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 323 "src/codegen/native/isa_x64.tr"
                r->offset = c->len;
#line 324 "src/codegen/native/isa_x64.tr"
                r->symbol = _tr_str_retain(callee);
#line 325 "src/codegen/native/isa_x64.tr"
                r->kind = 0LL;
#line 326 "src/codegen/native/isa_x64.tr"
                r->str_idx = 0LL;
#line 327 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(r));
#line 328 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 329 "src/codegen/native/isa_x64.tr"
                if ((dst >= 0LL)) {
#line 330 "src/codegen/native/isa_x64.tr"
                    _st_rax(c, _vreg_disp(dst));
                }
                _tr_obj_release(r, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IFBinOp) {
                __auto_type dst = _t7196.data.IFBinOp.dst;
__auto_type op = _t7196.data.IFBinOp.op;
__auto_type a = _t7196.data.IFBinOp.a;
__auto_type b = _t7196.data.IFBinOp.b;
#line 332 "src/codegen/native/isa_x64.tr"
                _ld_xmm0(c, _vreg_disp(a));
#line 333 "src/codegen/native/isa_x64.tr"
                _ld_xmm1(c, _vreg_disp(b));
#line 334 "src/codegen/native/isa_x64.tr"
                if (_is_cmp(op)) {
#line 335 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 102LL);
#line 336 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 15LL);
#line 337 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 47LL);
#line 338 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 193LL);
#line 339 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 15LL);
#line 340 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, _fsetcc(op));
#line 341 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 192LL);
#line 342 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 72LL);
#line 343 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 15LL);
#line 344 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 182LL);
#line 345 "src/codegen/native/isa_x64.tr"
                    ByteBuf_u8(c, 192LL);
#line 346 "src/codegen/native/isa_x64.tr"
                    _st_rax(c, _vreg_disp(dst));
                } else {
#line 348 "src/codegen/native/isa_x64.tr"
                    _emit_farith(c, op);
#line 349 "src/codegen/native/isa_x64.tr"
                    _st_xmm0(c, _vreg_disp(dst));
                }
            } else if (_t7196.tag == LInst_IIToF) {
                __auto_type dst = _t7196.data.IIToF.dst;
__auto_type src = _t7196.data.IIToF.src;
#line 351 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(src));
#line 352 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 242LL);
#line 353 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 354 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 15LL);
#line 355 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 42LL);
#line 356 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 192LL);
#line 357 "src/codegen/native/isa_x64.tr"
                _st_xmm0(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IFToI) {
                __auto_type dst = _t7196.data.IFToI.dst;
__auto_type src = _t7196.data.IFToI.src;
#line 359 "src/codegen/native/isa_x64.tr"
                _ld_xmm0(c, _vreg_disp(src));
#line 360 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 242LL);
#line 361 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 362 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 15LL);
#line 363 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 44LL);
#line 364 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 192LL);
#line 365 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IFCall1) {
                __auto_type dst = _t7196.data.IFCall1.dst;
__auto_type callee = _t7196.data.IFCall1.callee;
__auto_type arg = _t7196.data.IFCall1.arg;
#line 367 "src/codegen/native/isa_x64.tr"
                _ld_xmm0(c, _vreg_disp(arg));
#line 368 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 232LL);
#line 369 "src/codegen/native/isa_x64.tr"
                Reloc* fr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 370 "src/codegen/native/isa_x64.tr"
                fr->offset = c->len;
#line 371 "src/codegen/native/isa_x64.tr"
                fr->symbol = _tr_str_retain(callee);
#line 372 "src/codegen/native/isa_x64.tr"
                fr->kind = 0LL;
#line 373 "src/codegen/native/isa_x64.tr"
                fr->str_idx = 0LL;
#line 374 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(fr));
#line 375 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 376 "src/codegen/native/isa_x64.tr"
                if ((dst >= 0LL)) {
#line 377 "src/codegen/native/isa_x64.tr"
                    _st_rax(c, _vreg_disp(dst));
                }
                _tr_obj_release(fr, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IFCallF) {
                __auto_type dst = _t7196.data.IFCallF.dst;
__auto_type callee = _t7196.data.IFCallF.callee;
__auto_type arg = _t7196.data.IFCallF.arg;
#line 379 "src/codegen/native/isa_x64.tr"
                _ld_xmm0(c, _vreg_disp(arg));
#line 380 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 232LL);
#line 381 "src/codegen/native/isa_x64.tr"
                Reloc* ffr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 382 "src/codegen/native/isa_x64.tr"
                ffr->offset = c->len;
#line 383 "src/codegen/native/isa_x64.tr"
                ffr->symbol = _tr_str_retain(callee);
#line 384 "src/codegen/native/isa_x64.tr"
                ffr->kind = 0LL;
#line 385 "src/codegen/native/isa_x64.tr"
                ffr->str_idx = 0LL;
#line 386 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(ffr));
#line 387 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 388 "src/codegen/native/isa_x64.tr"
                _st_xmm0(c, _vreg_disp(dst));
                _tr_obj_release(ffr, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IFCall2F) {
                __auto_type dst = _t7196.data.IFCall2F.dst;
__auto_type callee = _t7196.data.IFCall2F.callee;
__auto_type a = _t7196.data.IFCall2F.a;
__auto_type b = _t7196.data.IFCall2F.b;
#line 390 "src/codegen/native/isa_x64.tr"
                _ld_xmm0(c, _vreg_disp(a));
#line 391 "src/codegen/native/isa_x64.tr"
                _ld_xmm1(c, _vreg_disp(b));
#line 392 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 232LL);
#line 393 "src/codegen/native/isa_x64.tr"
                Reloc* f2r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 394 "src/codegen/native/isa_x64.tr"
                f2r->offset = c->len;
#line 395 "src/codegen/native/isa_x64.tr"
                f2r->symbol = _tr_str_retain(callee);
#line 396 "src/codegen/native/isa_x64.tr"
                f2r->kind = 0LL;
#line 397 "src/codegen/native/isa_x64.tr"
                f2r->str_idx = 0LL;
#line 398 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(f2r));
#line 399 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 400 "src/codegen/native/isa_x64.tr"
                _st_xmm0(c, _vreg_disp(dst));
                _tr_obj_release(f2r, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_IBitsF) {
                __auto_type dst = _t7196.data.IBitsF.dst;
__auto_type src = _t7196.data.IBitsF.src;
#line 404 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(src));
#line 405 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IFBits) {
                __auto_type dst = _t7196.data.IFBits.dst;
__auto_type src = _t7196.data.IFBits.src;
#line 407 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(src));
#line 408 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IAddrVar) {
                __auto_type dst = _t7196.data.IAddrVar.dst;
__auto_type name = _t7196.data.IAddrVar.name;
#line 410 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 411 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 141LL);
#line 412 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 133LL);
#line 413 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, _var_disp(lf, name));
#line 414 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IAddrGlobal) {
#line 419 "src/codegen/native/isa_x64.tr"
                _emit_return(c, 0LL);
            } else if (_t7196.tag == LInst_IFuncAddr) {
                __auto_type dst = _t7196.data.IFuncAddr.dst;
__auto_type fname = _t7196.data.IFuncAddr.fname;
#line 421 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 422 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 141LL);
#line 423 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 5LL);
#line 424 "src/codegen/native/isa_x64.tr"
                Reloc* fnaddr_r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
#line 425 "src/codegen/native/isa_x64.tr"
                fnaddr_r->offset = c->len;
#line 426 "src/codegen/native/isa_x64.tr"
                fnaddr_r->symbol = _tr_str_retain(fname);
#line 427 "src/codegen/native/isa_x64.tr"
                fnaddr_r->kind = 4LL;
#line 428 "src/codegen/native/isa_x64.tr"
                fnaddr_r->str_idx = 0LL;
#line 429 "src/codegen/native/isa_x64.tr"
                List_ptr_append(e->relocs, _tr_obj_retain(fnaddr_r));
#line 430 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, 0LL);
#line 431 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(fnaddr_r, _trdrop_Reloc);
            } else if (_t7196.tag == LInst_ICallInd) {
                __auto_type dst = _t7196.data.ICallInd.dst;
__auto_type fnreg = _t7196.data.ICallInd.fnreg;
__auto_type iargs = _t7196.data.ICallInd.args;
#line 433 "src/codegen/native/isa_x64.tr"
                long long ci = 0LL;
#line 434 "src/codegen/native/isa_x64.tr"
                while ((ci < iargs->len)) {
#line 435 "src/codegen/native/isa_x64.tr"
                    _ld_argreg(c, ci, _vreg_disp(List_i64_get(iargs, ci)));
#line 436 "src/codegen/native/isa_x64.tr"
                    ci = (ci + 1LL);
                }
#line 437 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(fnreg));
#line 438 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 255LL);
#line 439 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 208LL);
#line 440 "src/codegen/native/isa_x64.tr"
                if ((dst >= 0LL)) {
#line 441 "src/codegen/native/isa_x64.tr"
                    _st_rax(c, _vreg_disp(dst));
                }
            } else if (_t7196.tag == LInst_IAsm) {
                __auto_type acode = _t7196.data.IAsm.code;
__auto_type acons = _t7196.data.IAsm.cons;
#line 443 "src/codegen/native/isa_x64.tr"
                /* pass */
            } else if (_t7196.tag == LInst_ILoad) {
                __auto_type dst = _t7196.data.ILoad.dst;
__auto_type base = _t7196.data.ILoad.base;
__auto_type off = _t7196.data.ILoad.off;
#line 446 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(base));
#line 447 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 448 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 139LL);
#line 449 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 128LL);
#line 450 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, off);
#line 451 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IStore) {
                __auto_type base = _t7196.data.IStore.base;
__auto_type off = _t7196.data.IStore.off;
__auto_type src = _t7196.data.IStore.src;
#line 453 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(base));
#line 454 "src/codegen/native/isa_x64.tr"
                _ld_rcx(c, _vreg_disp(src));
#line 455 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 72LL);
#line 456 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 137LL);
#line 457 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 136LL);
#line 458 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, off);
            } else if (_t7196.tag == LInst_ILoadB) {
                __auto_type dst = _t7196.data.ILoadB.dst;
__auto_type base = _t7196.data.ILoadB.base;
__auto_type off = _t7196.data.ILoadB.off;
#line 460 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(base));
#line 461 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 15LL);
#line 462 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 182LL);
#line 463 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 128LL);
#line 464 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, off);
#line 465 "src/codegen/native/isa_x64.tr"
                _st_rax(c, _vreg_disp(dst));
            } else if (_t7196.tag == LInst_IStoreB) {
                __auto_type base = _t7196.data.IStoreB.base;
__auto_type off = _t7196.data.IStoreB.off;
__auto_type src = _t7196.data.IStoreB.src;
#line 467 "src/codegen/native/isa_x64.tr"
                _ld_rax(c, _vreg_disp(base));
#line 468 "src/codegen/native/isa_x64.tr"
                _ld_rcx(c, _vreg_disp(src));
#line 469 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 136LL);
#line 470 "src/codegen/native/isa_x64.tr"
                ByteBuf_u8(c, 136LL);
#line 471 "src/codegen/native/isa_x64.tr"
                ByteBuf_u32(c, off);
            }
#line 472 "src/codegen/native/isa_x64.tr"
            ii = (ii + 1LL);
        }
#line 474 "src/codegen/native/isa_x64.tr"
        __auto_type _t7197 = blk->term;
        if (_t7197.tag == LTerm_TRetInt) {
            __auto_type rv = _t7197.data.TRetInt.v;
#line 476 "src/codegen/native/isa_x64.tr"
            _emit_return(c, rv);
        } else if (_t7197.tag == LTerm_TRetVal) {
            __auto_type rvreg = _t7197.data.TRetVal.v;
#line 478 "src/codegen/native/isa_x64.tr"
            _ld_rax(c, _vreg_disp(rvreg));
#line 479 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 201LL);
#line 480 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 195LL);
        } else if (_t7197.tag == LTerm_TRetVoid) {
#line 482 "src/codegen/native/isa_x64.tr"
            _emit_return(c, 0LL);
        } else if (_t7197.tag == LTerm_TBr) {
            __auto_type target = _t7197.data.TBr.target;
#line 484 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 233LL);
#line 485 "src/codegen/native/isa_x64.tr"
            Jump* jp = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
#line 486 "src/codegen/native/isa_x64.tr"
            jp->patch_off = c->len;
#line 487 "src/codegen/native/isa_x64.tr"
            jp->target = target;
#line 488 "src/codegen/native/isa_x64.tr"
            List_ptr_append(jumps, _tr_obj_retain(jp));
#line 489 "src/codegen/native/isa_x64.tr"
            ByteBuf_u32(c, 0LL);
            _tr_obj_release(jp, _trdrop_Jump);
        } else if (_t7197.tag == LTerm_TCondBr) {
            __auto_type cond = _t7197.data.TCondBr.cond;
__auto_type tb = _t7197.data.TCondBr.then_b;
__auto_type eb = _t7197.data.TCondBr.else_b;
#line 491 "src/codegen/native/isa_x64.tr"
            _ld_rax(c, _vreg_disp(cond));
#line 492 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 72LL);
#line 493 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 133LL);
#line 494 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 192LL);
#line 495 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 15LL);
#line 496 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 133LL);
#line 497 "src/codegen/native/isa_x64.tr"
            Jump* jp2 = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
#line 498 "src/codegen/native/isa_x64.tr"
            jp2->patch_off = c->len;
#line 499 "src/codegen/native/isa_x64.tr"
            jp2->target = tb;
#line 500 "src/codegen/native/isa_x64.tr"
            List_ptr_append(jumps, _tr_obj_retain(jp2));
#line 501 "src/codegen/native/isa_x64.tr"
            ByteBuf_u32(c, 0LL);
#line 502 "src/codegen/native/isa_x64.tr"
            ByteBuf_u8(c, 233LL);
#line 503 "src/codegen/native/isa_x64.tr"
            Jump* jp3 = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
#line 504 "src/codegen/native/isa_x64.tr"
            jp3->patch_off = c->len;
#line 505 "src/codegen/native/isa_x64.tr"
            jp3->target = eb;
#line 506 "src/codegen/native/isa_x64.tr"
            List_ptr_append(jumps, _tr_obj_retain(jp3));
#line 507 "src/codegen/native/isa_x64.tr"
            ByteBuf_u32(c, 0LL);
            _tr_obj_release(jp2, _trdrop_Jump);
            _tr_obj_release(jp3, _trdrop_Jump);
        } else if (1) {
            __auto_type _ = _t7197;
#line 509 "src/codegen/native/isa_x64.tr"
            _emit_return(c, 0LL);
        }
#line 510 "src/codegen/native/isa_x64.tr"
        bi = (bi + 1LL);
    }
#line 513 "src/codegen/native/isa_x64.tr"
    long long ji = 0LL;
#line 514 "src/codegen/native/isa_x64.tr"
    while ((ji < jumps->len)) {
#line 515 "src/codegen/native/isa_x64.tr"
        Jump* j = ((Jump*)List_ptr_get(jumps, ji));
#line 516 "src/codegen/native/isa_x64.tr"
        long long rel = ({ __auto_type _ord_t7198 = (List_i64_get(block_start, j->target)); __auto_type _wr = ((_ord_t7198 - (j->patch_off + 4LL))); _wr; });
#line 517 "src/codegen/native/isa_x64.tr"
        ByteBuf_patch_u32(c, j->patch_off, rel);
#line 518 "src/codegen/native/isa_x64.tr"
        ji = (ji + 1LL);
    }
#line 520 "src/codegen/native/isa_x64.tr"
    _tr_obj_release(c, _trdrop_ByteBuf);
    List_i64_free(block_start);
    List_ptr_free_obj(jumps, _trdrop_Jump);
    return e;
}

