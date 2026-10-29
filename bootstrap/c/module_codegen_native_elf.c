#include "tauraro_types.h"

long long _index_of(List_TrStr* names, TrStr s);
void _shdr(ByteBuf* out, long long name, long long ty, long long flags, long long addr, long long offset, long long size, long long link, long long info, long long align, long long entsize);
long long _align8(long long n);

__attribute__((hot)) long long _index_of(List_TrStr* names, TrStr s) {
#line 23 "src/codegen/native/elf.tr"
    long long i = 0LL;
#line 24 "src/codegen/native/elf.tr"
    while ((i < names->len)) {
#line 25 "src/codegen/native/elf.tr"
        if (({ TrStr _cmpa_t7162 = (List_TrStr_get(names, i)); TrStr _cmpb_t7163 = (s); _Bool _cmpr_t7164 = _tr_str_eqv(_cmpa_t7162, _cmpb_t7163); _tr_str_release(_cmpa_t7162); _cmpr_t7164; })) {
#line 25 "src/codegen/native/elf.tr"
            return i;
        }
#line 26 "src/codegen/native/elf.tr"
        i = (i + 1LL);
    }
#line 27 "src/codegen/native/elf.tr"
    return (-1LL);
}

__attribute__((hot)) void _shdr(ByteBuf* out, long long name, long long ty, long long flags, long long addr, long long offset, long long size, long long link, long long info, long long align, long long entsize) {
#line 30 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, name);
#line 31 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, ty);
#line 32 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, flags);
#line 33 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, addr);
#line 34 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, offset);
#line 35 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, size);
#line 36 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, link);
#line 37 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, info);
#line 38 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, align);
#line 39 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, entsize);
}

__attribute__((hot)) long long _align8(long long n) {
#line 42 "src/codegen/native/elf.tr"
    long long r = n;
#line 43 "src/codegen/native/elf.tr"
    while ((_tr_imod((long long)(r), (long long)(8LL)) != 0LL)) {
#line 43 "src/codegen/native/elf.tr"
        r = (r + 1LL);
    }
#line 44 "src/codegen/native/elf.tr"
    return r;
}

__attribute__((hot)) bool write_elf_object(TrStr out_path, List_ptr* funcs, List_TrStr* externs, List_TrStr* strings, List_i64* string_lens, long long n_globals) {
#line 48 "src/codegen/native/elf.tr"
    ByteBuf* text = ByteBuf_init();
#line 49 "src/codegen/native/elf.tr"
    List_ptr* trelocs = (void*)List_ptr_new();
#line 50 "src/codegen/native/elf.tr"
    List_i64* func_base = (void*)List_i64_new();
#line 51 "src/codegen/native/elf.tr"
    long long fi = 0LL;
#line 52 "src/codegen/native/elf.tr"
    while ((fi < funcs->len)) {
#line 53 "src/codegen/native/elf.tr"
        EncodedFunc* f = ((EncodedFunc*)List_ptr_get(funcs, fi));
#line 54 "src/codegen/native/elf.tr"
        long long base = text->len;
#line 55 "src/codegen/native/elf.tr"
        List_i64_append(func_base, base);
#line 56 "src/codegen/native/elf.tr"
        ByteBuf_append_buf(text, f->code);
#line 57 "src/codegen/native/elf.tr"
        long long ri = 0LL;
#line 58 "src/codegen/native/elf.tr"
        while ((ri < f->relocs->len)) {
#line 59 "src/codegen/native/elf.tr"
            Reloc* r = ((Reloc*)List_ptr_get(f->relocs, ri));
#line 60 "src/codegen/native/elf.tr"
            TextReloc* tr = ((TextReloc*)_tr_obj_alloc(sizeof(TextReloc)));
#line 61 "src/codegen/native/elf.tr"
            tr->roff = (base + r->offset);
#line 62 "src/codegen/native/elf.tr"
            tr->sym = _tr_str_retain(r->symbol);
#line 63 "src/codegen/native/elf.tr"
            tr->kind = r->kind;
#line 64 "src/codegen/native/elf.tr"
            tr->str_idx = r->str_idx;
#line 65 "src/codegen/native/elf.tr"
            List_ptr_append(trelocs, _tr_obj_retain(tr));
#line 66 "src/codegen/native/elf.tr"
            ri = (ri + 1LL);
            _tr_obj_release(tr, _trdrop_TextReloc);
        }
#line 67 "src/codegen/native/elf.tr"
        fi = (fi + 1LL);
    }
#line 70 "src/codegen/native/elf.tr"
    ByteBuf* rodata = ByteBuf_init();
#line 71 "src/codegen/native/elf.tr"
    List_i64* str_off = (void*)List_i64_new();
#line 72 "src/codegen/native/elf.tr"
    long long sxi = 0LL;
#line 73 "src/codegen/native/elf.tr"
    while ((sxi < strings->len)) {
#line 74 "src/codegen/native/elf.tr"
        List_i64_append(str_off, rodata->len);
#line 75 "src/codegen/native/elf.tr"
        ({ TrStr _ord_t7165 = (List_TrStr_get(strings, sxi)); __auto_type _ord_t7166 = (List_i64_get(string_lens, sxi)); ByteBuf_cstr_len(rodata, _ord_t7165, _ord_t7166); _tr_str_release(_ord_t7165); });
#line 76 "src/codegen/native/elf.tr"
        sxi = (sxi + 1LL);
    }
#line 81 "src/codegen/native/elf.tr"
    ByteBuf* symtab = ByteBuf_init();
#line 82 "src/codegen/native/elf.tr"
    ByteBuf* strtab = ByteBuf_init();
#line 83 "src/codegen/native/elf.tr"
    List_TrStr* sym_names = (void*)List_TrStr_new();
#line 84 "src/codegen/native/elf.tr"
    ByteBuf_zeros(symtab, 24LL);
#line 85 "src/codegen/native/elf.tr"
    ByteBuf_u8(strtab, 0LL);
#line 87 "src/codegen/native/elf.tr"
    ByteBuf_u32(symtab, 0LL);
#line 88 "src/codegen/native/elf.tr"
    ByteBuf_u8(symtab, 3LL);
#line 89 "src/codegen/native/elf.tr"
    ByteBuf_u8(symtab, 0LL);
#line 90 "src/codegen/native/elf.tr"
    ByteBuf_u16(symtab, 2LL);
#line 91 "src/codegen/native/elf.tr"
    ByteBuf_u64(symtab, 0LL);
#line 92 "src/codegen/native/elf.tr"
    ByteBuf_u64(symtab, 0LL);
#line 94 "src/codegen/native/elf.tr"
    ByteBuf_u32(symtab, 0LL);
#line 95 "src/codegen/native/elf.tr"
    ByteBuf_u8(symtab, 3LL);
#line 96 "src/codegen/native/elf.tr"
    ByteBuf_u8(symtab, 0LL);
#line 97 "src/codegen/native/elf.tr"
    ByteBuf_u16(symtab, 3LL);
#line 98 "src/codegen/native/elf.tr"
    ByteBuf_u64(symtab, 0LL);
#line 99 "src/codegen/native/elf.tr"
    ByteBuf_u64(symtab, 0LL);
#line 101 "src/codegen/native/elf.tr"
    fi = 0LL;
#line 102 "src/codegen/native/elf.tr"
    while ((fi < funcs->len)) {
#line 103 "src/codegen/native/elf.tr"
        EncodedFunc* f2 = ((EncodedFunc*)List_ptr_get(funcs, fi));
#line 104 "src/codegen/native/elf.tr"
        long long noff = strtab->len;
#line 105 "src/codegen/native/elf.tr"
        ByteBuf_cstr(strtab, f2->name);
#line 106 "src/codegen/native/elf.tr"
        ByteBuf_u32(symtab, noff);
#line 107 "src/codegen/native/elf.tr"
        ByteBuf_u8(symtab, 18LL);
#line 108 "src/codegen/native/elf.tr"
        ByteBuf_u8(symtab, 0LL);
#line 109 "src/codegen/native/elf.tr"
        ByteBuf_u16(symtab, 1LL);
#line 110 "src/codegen/native/elf.tr"
        ByteBuf_u64(symtab, List_i64_get(func_base, fi));
#line 111 "src/codegen/native/elf.tr"
        ByteBuf_u64(symtab, f2->code->len);
#line 112 "src/codegen/native/elf.tr"
        List_TrStr_append(sym_names, f2->name);
#line 113 "src/codegen/native/elf.tr"
        fi = (fi + 1LL);
    }
#line 115 "src/codegen/native/elf.tr"
    long long ei = 0LL;
#line 116 "src/codegen/native/elf.tr"
    while ((ei < externs->len)) {
#line 117 "src/codegen/native/elf.tr"
        TrStr ex = List_TrStr_get(externs, ei);
#line 118 "src/codegen/native/elf.tr"
        long long noff2 = strtab->len;
#line 119 "src/codegen/native/elf.tr"
        ByteBuf_cstr(strtab, ex);
#line 120 "src/codegen/native/elf.tr"
        ByteBuf_u32(symtab, noff2);
#line 121 "src/codegen/native/elf.tr"
        ByteBuf_u8(symtab, 16LL);
#line 122 "src/codegen/native/elf.tr"
        ByteBuf_u8(symtab, 0LL);
#line 123 "src/codegen/native/elf.tr"
        ByteBuf_u16(symtab, 0LL);
#line 124 "src/codegen/native/elf.tr"
        ByteBuf_u64(symtab, 0LL);
#line 125 "src/codegen/native/elf.tr"
        ByteBuf_u64(symtab, 0LL);
#line 126 "src/codegen/native/elf.tr"
        List_TrStr_append(sym_names, ex);
#line 127 "src/codegen/native/elf.tr"
        ei = (ei + 1LL);
        _tr_str_release(ex);
    }
#line 130 "src/codegen/native/elf.tr"
    ByteBuf* rela = ByteBuf_init();
#line 131 "src/codegen/native/elf.tr"
    long long tri = 0LL;
#line 132 "src/codegen/native/elf.tr"
    while ((tri < trelocs->len)) {
#line 133 "src/codegen/native/elf.tr"
        TextReloc* trr = ((TextReloc*)List_ptr_get(trelocs, tri));
#line 134 "src/codegen/native/elf.tr"
        if ((trr->kind == 1LL)) {
#line 136 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, trr->roff);
#line 137 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, ((1LL * 4294967296LL) + 2LL));
#line 138 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, (List_i64_get(str_off, trr->str_idx) - 4LL));
        } else if ((trr->kind == 3LL)) {
#line 141 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, trr->roff);
#line 142 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, ((2LL * 4294967296LL) + 2LL));
#line 143 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, ((trr->str_idx * 8LL) - 4LL));
        } else if ((trr->kind == 4LL)) {
#line 146 "src/codegen/native/elf.tr"
            long long fsidx = (_index_of(sym_names, trr->sym) + 3LL);
#line 147 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, trr->roff);
#line 148 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, ((fsidx * 4294967296LL) + 2LL));
#line 149 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, (0LL - 4LL));
        } else {
#line 152 "src/codegen/native/elf.tr"
            long long sidx = (_index_of(sym_names, trr->sym) + 3LL);
#line 153 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, trr->roff);
#line 154 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, ((sidx * 4294967296LL) + 4LL));
#line 155 "src/codegen/native/elf.tr"
            ByteBuf_u64(rela, (0LL - 4LL));
        }
#line 156 "src/codegen/native/elf.tr"
        tri = (tri + 1LL);
    }
#line 159 "src/codegen/native/elf.tr"
    ByteBuf* shstr = ByteBuf_init();
#line 160 "src/codegen/native/elf.tr"
    ByteBuf_u8(shstr, 0LL);
#line 161 "src/codegen/native/elf.tr"
    long long n_text = shstr->len;
#line 162 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".text", 5LL));
#line 163 "src/codegen/native/elf.tr"
    long long n_rodata = shstr->len;
#line 164 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".rodata", 7LL));
#line 165 "src/codegen/native/elf.tr"
    long long n_bss = shstr->len;
#line 166 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".bss", 4LL));
#line 167 "src/codegen/native/elf.tr"
    long long n_rela = shstr->len;
#line 168 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".rela.text", 10LL));
#line 169 "src/codegen/native/elf.tr"
    long long n_symtab = shstr->len;
#line 170 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".symtab", 7LL));
#line 171 "src/codegen/native/elf.tr"
    long long n_strtab = shstr->len;
#line 172 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".strtab", 7LL));
#line 173 "src/codegen/native/elf.tr"
    long long n_shstr = shstr->len;
#line 174 "src/codegen/native/elf.tr"
    ByteBuf_cstr(shstr, _tr_str_lit_len(".shstrtab", 9LL));
#line 177 "src/codegen/native/elf.tr"
    long long text_off = 64LL;
#line 178 "src/codegen/native/elf.tr"
    long long off = _align8((text_off + text->len));
#line 179 "src/codegen/native/elf.tr"
    long long rodata_off = off;
#line 180 "src/codegen/native/elf.tr"
    off = _align8((off + rodata->len));
#line 181 "src/codegen/native/elf.tr"
    long long bss_off = off;
#line 182 "src/codegen/native/elf.tr"
    long long rela_off = off;
#line 183 "src/codegen/native/elf.tr"
    off = (off + rela->len);
#line 184 "src/codegen/native/elf.tr"
    long long symtab_off = off;
#line 185 "src/codegen/native/elf.tr"
    off = (off + symtab->len);
#line 186 "src/codegen/native/elf.tr"
    long long strtab_off = off;
#line 187 "src/codegen/native/elf.tr"
    off = (off + strtab->len);
#line 188 "src/codegen/native/elf.tr"
    long long shstr_off = off;
#line 189 "src/codegen/native/elf.tr"
    off = _align8((off + shstr->len));
#line 190 "src/codegen/native/elf.tr"
    long long shoff = off;
#line 193 "src/codegen/native/elf.tr"
    ByteBuf* out = ByteBuf_init();
#line 194 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 127LL);
#line 195 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 69LL);
#line 196 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 76LL);
#line 197 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 70LL);
#line 198 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 2LL);
#line 199 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 1LL);
#line 200 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 1LL);
#line 201 "src/codegen/native/elf.tr"
    ByteBuf_u8(out, 0LL);
#line 202 "src/codegen/native/elf.tr"
    ByteBuf_zeros(out, 8LL);
#line 203 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 1LL);
#line 204 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 62LL);
#line 205 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, 1LL);
#line 206 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, 0LL);
#line 207 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, 0LL);
#line 208 "src/codegen/native/elf.tr"
    ByteBuf_u64(out, shoff);
#line 209 "src/codegen/native/elf.tr"
    ByteBuf_u32(out, 0LL);
#line 210 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 64LL);
#line 211 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 0LL);
#line 212 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 0LL);
#line 213 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 64LL);
#line 214 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 8LL);
#line 215 "src/codegen/native/elf.tr"
    ByteBuf_u16(out, 7LL);
#line 217 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, text);
#line 218 "src/codegen/native/elf.tr"
    ByteBuf_align_to(out, 8LL);
#line 219 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, rodata);
#line 220 "src/codegen/native/elf.tr"
    ByteBuf_align_to(out, 8LL);
#line 221 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, rela);
#line 222 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, symtab);
#line 223 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, strtab);
#line 224 "src/codegen/native/elf.tr"
    ByteBuf_append_buf(out, shstr);
#line 225 "src/codegen/native/elf.tr"
    ByteBuf_align_to(out, 8LL);
#line 228 "src/codegen/native/elf.tr"
    long long bss_size = (n_globals * 8LL);
#line 231 "src/codegen/native/elf.tr"
    _shdr(out, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL);
#line 232 "src/codegen/native/elf.tr"
    _shdr(out, n_text, 1LL, 6LL, 0LL, text_off, text->len, 0LL, 0LL, 16LL, 0LL);
#line 233 "src/codegen/native/elf.tr"
    _shdr(out, n_rodata, 1LL, 2LL, 0LL, rodata_off, rodata->len, 0LL, 0LL, 1LL, 0LL);
#line 234 "src/codegen/native/elf.tr"
    _shdr(out, n_bss, 8LL, 3LL, 0LL, bss_off, bss_size, 0LL, 0LL, 8LL, 0LL);
#line 235 "src/codegen/native/elf.tr"
    _shdr(out, n_rela, 4LL, 64LL, 0LL, rela_off, rela->len, 5LL, 1LL, 8LL, 24LL);
#line 236 "src/codegen/native/elf.tr"
    _shdr(out, n_symtab, 2LL, 0LL, 0LL, symtab_off, symtab->len, 6LL, 3LL, 8LL, 24LL);
#line 237 "src/codegen/native/elf.tr"
    _shdr(out, n_strtab, 3LL, 0LL, 0LL, strtab_off, strtab->len, 0LL, 0LL, 1LL, 0LL);
#line 238 "src/codegen/native/elf.tr"
    _shdr(out, n_shstr, 3LL, 0LL, 0LL, shstr_off, shstr->len, 0LL, 0LL, 1LL, 0LL);
#line 240 "src/codegen/native/elf.tr"
    bool _trret1891 = ByteBuf_write_file(out, out_path);
    _tr_obj_release(text, _trdrop_ByteBuf);
    List_ptr_free_obj(trelocs, _trdrop_TextReloc);
    List_i64_free(func_base);
    _tr_obj_release(rodata, _trdrop_ByteBuf);
    List_i64_free(str_off);
    _tr_obj_release(symtab, _trdrop_ByteBuf);
    _tr_obj_release(strtab, _trdrop_ByteBuf);
    _tr_obj_release(rela, _trdrop_ByteBuf);
    _tr_obj_release(shstr, _trdrop_ByteBuf);
    _tr_obj_release(out, _trdrop_ByteBuf);
    return _trret1891;
}

