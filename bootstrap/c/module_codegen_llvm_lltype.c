#include "tauraro_types.h"


__attribute__((hot)) TrStr _ll_ty(long long tag) {
#line 15 "src/codegen/llvm/lltype.tr"
    if ((tag == 5LL)) {
#line 15 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 16 "src/codegen/llvm/lltype.tr"
    if ((tag == 1LL)) {
#line 16 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 17 "src/codegen/llvm/lltype.tr"
    if ((tag == 2LL)) {
#line 17 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 18 "src/codegen/llvm/lltype.tr"
    if ((tag == 3LL)) {
#line 18 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 19 "src/codegen/llvm/lltype.tr"
    if (((tag >= 6LL) && (tag <= 9LL))) {
#line 19 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 20 "src/codegen/llvm/lltype.tr"
    if ((tag == 10LL)) {
#line 20 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 21 "src/codegen/llvm/lltype.tr"
    if ((tag == 11LL)) {
#line 21 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 22 "src/codegen/llvm/lltype.tr"
    if ((tag == 12LL)) {
#line 22 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 23 "src/codegen/llvm/lltype.tr"
    if ((tag == 15LL)) {
#line 23 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 24 "src/codegen/llvm/lltype.tr"
    if (((tag == 13LL) || (tag == 16LL))) {
#line 24 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 25 "src/codegen/llvm/lltype.tr"
    if (((tag == 14LL) || (tag == 19LL))) {
#line 25 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 26 "src/codegen/llvm/lltype.tr"
    if (((tag == 17LL) || (tag == 18LL))) {
#line 26 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 27 "src/codegen/llvm/lltype.tr"
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) TrStr _ll_ty_name(TrStr n) {
#line 32 "src/codegen/llvm/lltype.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL))))) {
#line 32 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("void", 4LL);
    }
#line 33 "src/codegen/llvm/lltype.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 33 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("i64", 3LL);
    }
#line 34 "src/codegen/llvm/lltype.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL))))) {
#line 34 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 35 "src/codegen/llvm/lltype.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("String", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL))))) {
#line 35 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("ptr", 3LL);
    }
#line 36 "src/codegen/llvm/lltype.tr"
    if (((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))))) {
#line 36 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("i64", 3LL);
    }
#line 37 "src/codegen/llvm/lltype.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL))))) {
#line 37 "src/codegen/llvm/lltype.tr"
        return _tr_str_lit_len("i64", 3LL);
    }
#line 38 "src/codegen/llvm/lltype.tr"
    return _tr_str_lit_len("ptr", 3LL);
}

__attribute__((hot)) long long _ll_hexdigit(long long n) {
#line 41 "src/codegen/llvm/lltype.tr"
    if ((n < 10LL)) {
#line 41 "src/codegen/llvm/lltype.tr"
        return (48LL + n);
    }
#line 42 "src/codegen/llvm/lltype.tr"
    return (65LL + (n - 10LL));
}

__attribute__((hot)) TrStr _ll_hexpad16(long long v) {
#line 47 "src/codegen/llvm/lltype.tr"
    StringBuilder* sb = StringBuilder_init(16LL);
#line 48 "src/codegen/llvm/lltype.tr"
    long long k = 15LL;
#line 49 "src/codegen/llvm/lltype.tr"
    while ((k >= 0LL)) {
#line 50 "src/codegen/llvm/lltype.tr"
        long long nib = ((v >> (4LL * k)) & 15LL);
#line 51 "src/codegen/llvm/lltype.tr"
        StringBuilder_append_char(sb, _ll_hexdigit(nib));
#line 52 "src/codegen/llvm/lltype.tr"
        k = (k - 1LL);
    }
#line 53 "src/codegen/llvm/lltype.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _ll_str_escape(TrStr s, long long blen) {
#line 63 "src/codegen/llvm/lltype.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 64 "src/codegen/llvm/lltype.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 65 "src/codegen/llvm/lltype.tr"
    long long i = 0LL;
#line 66 "src/codegen/llvm/lltype.tr"
    while ((i < blen)) {
#line 67 "src/codegen/llvm/lltype.tr"
        long long c = ((long long)((*(p + i))));
#line 68 "src/codegen/llvm/lltype.tr"
        long long b = (c & 255LL);
#line 69 "src/codegen/llvm/lltype.tr"
        if (((((b >= 32LL) && (b <= 126LL)) && (b != 34LL)) && (b != 92LL))) {
#line 70 "src/codegen/llvm/lltype.tr"
            StringBuilder_append_char(sb, b);
        } else {
#line 72 "src/codegen/llvm/lltype.tr"
            StringBuilder_append(sb, _tr_str_lit_len("\\", 1LL));
#line 73 "src/codegen/llvm/lltype.tr"
            StringBuilder_append_char(sb, _ll_hexdigit(((b >> 4LL) & 15LL)));
#line 74 "src/codegen/llvm/lltype.tr"
            StringBuilder_append_char(sb, _ll_hexdigit((b & 15LL)));
        }
#line 75 "src/codegen/llvm/lltype.tr"
        i = (i + 1LL);
    }
#line 76 "src/codegen/llvm/lltype.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

