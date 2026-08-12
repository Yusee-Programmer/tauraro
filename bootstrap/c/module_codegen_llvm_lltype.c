#include "tauraro_types.h"


__attribute__((hot)) TrStr _ll_ty(long long tag) {
    /* pass */
    if ((tag == 5LL)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((tag == 1LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 2LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 3LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if (((tag >= 6LL) && (tag <= 9LL))) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 10LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 11LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 12LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((tag == 15LL)) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if (((tag == 13LL) || (tag == 16LL))) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if (((tag == 14LL) || (tag == 19LL))) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if (((tag == 17LL) || (tag == 18LL))) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    return _tr_str_lit("i64");
}

__attribute__((hot)) TrStr _ll_ty_name(TrStr n) {
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f32"))) == 0))) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("String"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("char"))) == 0))) {
        /* pass */
        return _tr_str_lit("ptr");
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0))) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u64"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("usize"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("isize"))) == 0))) {
        /* pass */
        return _tr_str_lit("i64");
    }
    /* pass */
    return _tr_str_lit("ptr");
}

__attribute__((hot)) long long _ll_hexdigit(long long n) {
    /* pass */
    if ((n < 10LL)) {
        /* pass */
        return (48LL + n);
    }
    /* pass */
    return (65LL + (n - 10LL));
}

__attribute__((hot)) TrStr _ll_hexpad16(long long v) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(16LL);
    /* pass */
    long long k = 15LL;
    /* pass */
    while ((k >= 0LL)) {
        /* pass */
        long long nib = ((v >> (4LL * k)) & 15LL);
        /* pass */
        StringBuilder_append_char(sb, _ll_hexdigit(nib));
        /* pass */
        k = (k - 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) long long _ll_str_bytelen(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        if ((((long long)((*(p + i)))) == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return i;
}

__attribute__((hot)) TrStr _ll_str_escape(TrStr s) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
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
        long long b = (c & 255LL);
        /* pass */
        if (((((b >= 32LL) && (b <= 126LL)) && (b != 34LL)) && (b != 92LL))) {
            /* pass */
            StringBuilder_append_char(sb, b);
        } else {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\"));
            /* pass */
            StringBuilder_append_char(sb, _ll_hexdigit(((b >> 4LL) & 15LL)));
            /* pass */
            StringBuilder_append_char(sb, _ll_hexdigit((b & 15LL)));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

