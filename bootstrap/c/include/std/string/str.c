#include "../../../tauraro_types.h"


__attribute__((hot)) long long Str_len(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long n = 0LL;
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    return n;
}

__attribute__((hot)) bool Str_starts_with(TrStr s, TrStr prefix) {
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* pp = ((char*)(_tr_strz(prefix)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long pc = ((long long)((*(pp + i))));
        /* pass */
        if ((pc == 0LL)) {
            /* pass */
            return true;
        }
        /* pass */
        long long sc = ((long long)((*(sp + i))));
        /* pass */
        if (((sc == 0LL) || (sc != pc))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Str_ends_with(TrStr s, TrStr suffix) {
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long el = Str_len(suffix);
    /* pass */
    if ((el > sl)) {
        /* pass */
        return false;
    }
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* ep = ((char*)(_tr_strz(suffix)));
    /* pass */
    long long off = (sl - el);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < el)) {
        /* pass */
        if ((((long long)((*(sp + (off + i))))) != ((long long)((*(ep + i)))))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_contains_char(TrStr s, long long c) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long ch = ((long long)((*(p + i))));
        /* pass */
        if ((ch == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((ch == c)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Str_contains(TrStr s, TrStr sub) {
    /* pass */
    return (Str_index_of(s, sub) >= 0LL);
}

__attribute__((hot)) bool Str_eq(TrStr a, TrStr b) {
    /* pass */
    char* pa = ((char*)(_tr_strz(a)));
    /* pass */
    char* pb = ((char*)(_tr_strz(b)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long ca = ((long long)((*(pa + i))));
        /* pass */
        long long cb = ((long long)((*(pb + i))));
        /* pass */
        if ((ca != cb)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((ca == 0LL)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_digit(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        if (((c < 48LL) || (c > 57LL))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_alpha(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        if ((!(((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_alnum(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        bool ok = ((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL)));
        /* pass */
        if ((!ok)) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_space(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        if (((((c != 32LL) && (c != 9LL)) && (c != 10LL)) && (c != 13LL))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_upper(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        if (((c >= 97LL) && (c <= 122LL))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Str_is_lower(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long c = ((long long)((*(p + i))));
    /* pass */
    if ((c == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    while ((c != 0LL)) {
        /* pass */
        if (((c >= 65LL) && (c <= 90LL))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c = ((long long)((*(p + i))));
    }
    /* pass */
    return true;
}

__attribute__((hot)) TrStr Str_slice(TrStr s, long long start, long long end) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    long long real_s = start;
    /* pass */
    long long real_e = end;
    /* pass */
    if ((real_s < 0LL)) {
        /* pass */
        real_s = 0LL;
    }
    /* pass */
    if ((real_e > slen)) {
        /* pass */
        real_e = slen;
    }
    /* pass */
    if ((real_s >= real_e)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    long long sz = (real_e - real_s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((sz + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = real_s;
    /* pass */
    while ((i < real_e)) {
        /* pass */
        StringBuilder_append_char(sb, ((long long)((*(p + i)))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_repeat(TrStr s, long long n) {
    /* pass */
    if ((n <= 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(((Str_len(s) * n) + 1LL));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        StringBuilder_append(sb, s);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_reverse(TrStr s) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((slen + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = (slen - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        StringBuilder_append_char(sb, ((long long)((*(p + i)))));
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_to_upper(TrStr s) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((slen + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long ch = ((long long)((*(p + i))));
        /* pass */
        if (((ch >= 97LL) && (ch <= 122LL))) {
            /* pass */
            ch = (ch - 32LL);
        }
        /* pass */
        StringBuilder_append_char(sb, ch);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_to_lower(TrStr s) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((slen + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long ch = ((long long)((*(p + i))));
        /* pass */
        if (((ch >= 65LL) && (ch <= 90LL))) {
            /* pass */
            ch = (ch + 32LL);
        }
        /* pass */
        StringBuilder_append_char(sb, ch);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_capitalize(TrStr s) {
    /* pass */
    TrStr low = Str_to_lower(s);
    /* pass */
    if ((Str_len(low) == 0LL)) {
        /* pass */
        return low;
    }
    /* pass */
    char* p = ((char*)(_tr_strz(low)));
    /* pass */
    long long ch = ((long long)((*(p + 0LL))));
    /* pass */
    if (((ch >= 97LL) && (ch <= 122LL))) {
        /* pass */
        TrStr result = ({ TrStr _at_t23 = (Str_slice(low, 0LL, 1LL)); __auto_type _wr = (({ TrStr _cl = (Str_to_upper(_at_t23)); TrStr _cr = (Str_slice(low, 1LL, Str_len(low))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); _tr_str_release(_at_t23); _wr; });
        /* pass */
        /* unsafe block */
        /* pass */
        _tr_str_release(low);
        /* pass */
        return result;
    }
    /* pass */
    return low;
}

__attribute__((hot)) TrStr Str_title(TrStr s) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((slen + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long prev = 32LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long ch = ((long long)((*(p + i))));
        /* pass */
        if ((((prev == 32LL) && (ch >= 97LL)) && (ch <= 122LL))) {
            /* pass */
            StringBuilder_append_char(sb, (ch - 32LL));
        } else if ((((prev != 32LL) && (ch >= 65LL)) && (ch <= 90LL))) {
            /* pass */
            StringBuilder_append_char(sb, (ch + 32LL));
        } else {
            /* pass */
            StringBuilder_append_char(sb, ch);
        }
        /* pass */
        prev = ch;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_trim_left(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if (((c == 0LL) || ((((c != 32LL) && (c != 9LL)) && (c != 10LL)) && (c != 13LL)))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return Str_slice(s, i, Str_len(s));
}

__attribute__((hot)) TrStr Str_trim_right(TrStr s) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = (slen - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if (((((c != 32LL) && (c != 9LL)) && (c != 10LL)) && (c != 13LL))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return Str_slice(s, 0LL, (i + 1LL));
}

__attribute__((hot)) TrStr Str_trim(TrStr s) {
    /* pass */
    return ({ TrStr _at_t24 = (Str_trim_right(s)); __auto_type _wr = (Str_trim_left(_at_t24)); _tr_str_release(_at_t24); _wr; });
}

__attribute__((hot)) long long Str_char_at(TrStr s, long long i) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    return ((long long)((*(p + i))));
}

__attribute__((hot)) TrStr Str_lpad(TrStr s, long long width, long long pad_char) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    if ((slen >= width)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init((width + 1LL));
    /* pass */
    long long i = slen;
    /* pass */
    while ((i < width)) {
        /* pass */
        StringBuilder_append_char(sb, pad_char);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder_append(sb, s);
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_rpad(TrStr s, long long width, long long pad_char) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    if ((slen >= width)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init((width + 1LL));
    /* pass */
    StringBuilder_append(sb, s);
    /* pass */
    long long i = slen;
    /* pass */
    while ((i < width)) {
        /* pass */
        StringBuilder_append_char(sb, pad_char);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_center(TrStr s, long long width) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    if ((slen >= width)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    long long total = (width - slen);
    /* pass */
    long long left = (total / 2LL);
    /* pass */
    long long right = (total - left);
    /* pass */
    StringBuilder* sb = StringBuilder_init((width + 1LL));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < left)) {
        /* pass */
        StringBuilder_append_char(sb, 32LL);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    StringBuilder_append(sb, s);
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < right)) {
        /* pass */
        StringBuilder_append_char(sb, 32LL);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) long long Str_index_of(TrStr s, TrStr sub) {
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long subl = Str_len(sub);
    /* pass */
    if ((subl == 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    if ((subl > sl)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* subp = ((char*)(_tr_strz(sub)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i <= (sl - subl))) {
        /* pass */
        long long j = 0LL;
        /* pass */
        bool found = true;
        /* pass */
        while ((j < subl)) {
            /* pass */
            if ((((long long)((*(sp + (i + j))))) != ((long long)((*(subp + j)))))) {
                /* pass */
                found = false;
                /* pass */
                j = subl;
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        if (found) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long Str_last_index_of(TrStr s, TrStr sub) {
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long subl = Str_len(sub);
    /* pass */
    if ((subl == 0LL)) {
        /* pass */
        return sl;
    }
    /* pass */
    if ((subl > sl)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    char* subp = ((char*)(_tr_strz(sub)));
    /* pass */
    long long i = (sl - subl);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        long long j = 0LL;
        /* pass */
        bool found = true;
        /* pass */
        while ((j < subl)) {
            /* pass */
            if ((((long long)((*(sp + (i + j))))) != ((long long)((*(subp + j)))))) {
                /* pass */
                found = false;
                /* pass */
                j = subl;
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        if (found) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long Str__tr_fn_count(TrStr s, TrStr sub) {
    /* pass */
    long long subl = Str_len(sub);
    /* pass */
    if ((subl == 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long n = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while (true) {
        /* pass */
        TrStr rest = Str_slice(s, i, sl);
        /* pass */
        long long pos = Str_index_of(rest, sub);
        /* pass */
        /* unsafe block */
        /* pass */
        _tr_str_release(rest);
        /* pass */
        if ((pos < 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        n = (n + 1LL);
        /* pass */
        i = ((i + pos) + subl);
    }
    /* pass */
    return n;
}

__attribute__((hot)) List_TrStr* Str_split(TrStr s, TrStr sep) {
    /* pass */
    return _tr_str_split(_tr_strz(s), _tr_strz(sep));
}

__attribute__((hot)) List_TrStr* Str_split_to_vec(TrStr s, TrStr sep) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long sepl = Str_len(sep);
    /* pass */
    if ((sepl == 0LL)) {
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < sl)) {
            /* pass */
            ({ TrStr _at_t25 = (Str_slice(s, i, (i + 1LL))); List_TrStr_append(out, _at_t25); _tr_str_release(_at_t25); });
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return out;
    }
    /* pass */
    long long start = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i <= (sl - sepl))) {
        /* pass */
        long long j = 0LL;
        /* pass */
        bool match_ = true;
        /* pass */
        while ((j < sepl)) {
            /* pass */
            if ((Str_char_at(s, (i + j)) != Str_char_at(sep, j))) {
                /* pass */
                match_ = false;
                /* pass */
                j = sepl;
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        if (match_) {
            /* pass */
            ({ TrStr _at_t26 = (Str_slice(s, start, i)); List_TrStr_append(out, _at_t26); _tr_str_release(_at_t26); });
            /* pass */
            start = (i + sepl);
            /* pass */
            i = start;
        } else {
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    ({ TrStr _at_t27 = (Str_slice(s, start, sl)); List_TrStr_append(out, _at_t27); _tr_str_release(_at_t27); });
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_join(List_TrStr* parts, TrStr sep) {
    /* pass */
    return _tr_str_wrap(_tr_strx_join_trstr(parts, _tr_strz(sep)));
}

__attribute__((hot)) TrStr Str_replace(TrStr s, TrStr old, TrStr new_) {
    /* pass */
    StringBuilder* result = StringBuilder_init((Str_len(s) + 1LL));
    /* pass */
    long long oldl = Str_len(old);
    /* pass */
    if ((oldl == 0LL)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < sl)) {
        /* pass */
        bool found = true;
        /* pass */
        if (((i + oldl) > sl)) {
            /* pass */
            found = false;
        }
        /* pass */
        if (found) {
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < oldl)) {
                /* pass */
                if ((((long long)((*(sp + (i + j))))) != ((long long)((*(((char*)(_tr_strz(old))) + j)))))) {
                    /* pass */
                    found = false;
                    /* pass */
                    j = oldl;
                }
                /* pass */
                j = (j + 1LL);
            }
        }
        /* pass */
        if (found) {
            /* pass */
            StringBuilder_append(result, new_);
            /* pass */
            i = (i + oldl);
        } else {
            /* pass */
            StringBuilder_append_char(result, ((long long)((*(sp + i)))));
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(result);
    /* pass */
    StringBuilder__tr_fn_free(result);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_replace_first(TrStr s, TrStr old, TrStr new_) {
    /* pass */
    long long pos = Str_index_of(s, old);
    /* pass */
    if ((pos < 0LL)) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init((Str_len(s) + Str_len(new_)));
    /* pass */
    long long sl = Str_len(s);
    /* pass */
    long long tail_start = (pos + Str_len(old));
    /* pass */
    TrStr head = Str_slice(s, 0LL, pos);
    /* pass */
    TrStr tail = Str_slice(s, tail_start, sl);
    /* pass */
    StringBuilder_append(sb, head);
    /* pass */
    StringBuilder_append(sb, new_);
    /* pass */
    StringBuilder_append(sb, tail);
    /* pass */
    /* unsafe block */
    /* pass */
    if ((pos > 0LL)) {
        /* pass */
        _tr_str_release(head);
    }
    /* pass */
    if ((tail_start < sl)) {
        /* pass */
        _tr_str_release(tail);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_format(TrStr template, List_TrStr* values) {
    /* pass */
    TrStr result = _tr_str_retain(template);
    /* pass */
    bool owned = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < values->len)) {
        /* pass */
        if ((Str_index_of(result, _tr_str_lit("{}")) >= 0LL)) {
            /* pass */
            TrStr next = ({ TrStr _at_t28 = (List_TrStr_get(values, i)); __auto_type _wr = (Str_replace_first(result, _tr_str_lit("{}"), _at_t28)); _tr_str_release(_at_t28); _wr; });
            /* pass */
            if (owned) {
                /* pass */
                /* unsafe block */
                /* pass */
                _tr_str_release(result);
            }
            /* pass */
            TrStr _strtmp_t29 = _tr_str_retain(next);
            _tr_str_release(result);
            result = _strtmp_t29;
            /* pass */
            /* unsafe block */
            /* pass */
            _tr_str_release(next);
            /* pass */
            owned = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return result;
}

__attribute__((hot)) long long Str_parse_int(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    bool neg = false;
    /* pass */
    long long c = ((long long)((*(p + 0LL))));
    /* pass */
    if ((c == 45LL)) {
        /* pass */
        neg = true;
        /* pass */
        i = 1LL;
    } else if ((c == 43LL)) {
        /* pass */
        i = 1LL;
    }
    /* pass */
    long long result = 0LL;
    /* pass */
    long long c2 = ((long long)((*(p + i))));
    /* pass */
    while (((c2 >= 48LL) && (c2 <= 57LL))) {
        /* pass */
        result = ((result * 10LL) + (c2 - 48LL));
        /* pass */
        i = (i + 1LL);
        /* pass */
        c2 = ((long long)((*(p + i))));
    }
    /* pass */
    if (neg) {
        /* pass */
        return (0LL - result);
    }
    /* pass */
    return result;
}

__attribute__((hot)) double Str_parse_float(TrStr s) {
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    bool neg = false;
    /* pass */
    long long c = ((long long)((*(p + 0LL))));
    /* pass */
    if ((c == 45LL)) {
        /* pass */
        neg = true;
        /* pass */
        i = 1LL;
    } else if ((c == 43LL)) {
        /* pass */
        i = 1LL;
    }
    /* pass */
    long long int_part = 0LL;
    /* pass */
    long long frac_part = 0LL;
    /* pass */
    long long frac_div = 1LL;
    /* pass */
    bool in_frac = false;
    /* pass */
    long long c2 = ((long long)((*(p + i))));
    /* pass */
    while ((c2 != 0LL)) {
        /* pass */
        if (((c2 >= 48LL) && (c2 <= 57LL))) {
            /* pass */
            if ((!in_frac)) {
                /* pass */
                int_part = ((int_part * 10LL) + (c2 - 48LL));
            } else {
                /* pass */
                frac_part = ((frac_part * 10LL) + (c2 - 48LL));
                /* pass */
                frac_div = (frac_div * 10LL);
            }
        } else if ((c2 == 46LL)) {
            /* pass */
            in_frac = true;
        } else {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        c2 = ((long long)((*(p + i))));
    }
    /* pass */
    double result = (((double)(int_part)) + (((double)(frac_part)) / ((double)(frac_div))));
    /* pass */
    if (neg) {
        /* pass */
        return (0.0 - result);
    }
    /* pass */
    return result;
}

__attribute__((hot)) bool Str_parse_bool(TrStr s) {
    /* pass */
    if (Str_eq(s, _tr_str_lit("true"))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Str_eq(s, _tr_str_lit("1"))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Str_eq(s, _tr_str_lit("yes"))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) List_TrStr* Str_lines(TrStr s) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    long long start = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long c = ((long long)((*(sp + i))));
        /* pass */
        if ((c == 10LL)) {
            /* pass */
            ({ TrStr _at_t30 = (Str_slice(s, start, i)); List_TrStr_append(out, _at_t30); _tr_str_release(_at_t30); });
            /* pass */
            start = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((start < slen)) {
        /* pass */
        ({ TrStr _at_t31 = (Str_slice(s, start, slen)); List_TrStr_append(out, _at_t31); _tr_str_release(_at_t31); });
    }
    /* pass */
    return out;
}

__attribute__((hot)) List_TrStr* Str_words(TrStr s) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    char* sp = ((char*)(_tr_strz(s)));
    /* pass */
    long long start = (-1LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long c = ((long long)((*(sp + i))));
        /* pass */
        bool ws = ((((c == 32LL) || (c == 9LL)) || (c == 10LL)) || (c == 13LL));
        /* pass */
        if (((!ws) && (start < 0LL))) {
            /* pass */
            start = i;
        } else if ((ws && (start >= 0LL))) {
            /* pass */
            ({ TrStr _at_t32 = (Str_slice(s, start, i)); List_TrStr_append(out, _at_t32); _tr_str_release(_at_t32); });
            /* pass */
            start = (-1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((start >= 0LL)) {
        /* pass */
        ({ TrStr _at_t33 = (Str_slice(s, start, slen)); List_TrStr_append(out, _at_t33); _tr_str_release(_at_t33); });
    }
    /* pass */
    return out;
}

__attribute__((hot)) TrStr Str_strip_prefix(TrStr s, TrStr prefix) {
    /* pass */
    if (Str_starts_with(s, prefix)) {
        /* pass */
        return Str_slice(s, Str_len(prefix), Str_len(s));
    }
    /* pass */
    return _tr_str_retain(s);
}

__attribute__((hot)) TrStr Str_strip_suffix(TrStr s, TrStr suffix) {
    /* pass */
    if (Str_ends_with(s, suffix)) {
        /* pass */
        return Str_slice(s, 0LL, (Str_len(s) - Str_len(suffix)));
    }
    /* pass */
    return _tr_str_retain(s);
}

__attribute__((hot)) TrStr Str_remove_char(TrStr s, long long c) {
    /* pass */
    long long slen = Str_len(s);
    /* pass */
    StringBuilder* sb = StringBuilder_init((slen + 1LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < slen)) {
        /* pass */
        long long ch = ((long long)((*(p + i))));
        /* pass */
        if ((ch != c)) {
            /* pass */
            StringBuilder_append_char(sb, ch);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr out = StringBuilder_to_owned(sb);
    /* pass */
    StringBuilder__tr_fn_free(sb);
    /* pass */
    return out;
}

__attribute__((hot)) StrView StrView_of(TrStr s, long long start, long long _tr_v_count) {
    /* pass */
    StrView v = (StrView){0};
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    v.data = (p + start);
    /* pass */
    v.len = _tr_v_count;
    /* pass */
    return v;
}

__attribute__((hot)) StrView StrView_all(TrStr s) {
    /* pass */
    StrView v = (StrView){0};
    /* pass */
    v.data = ((char*)(_tr_strz(s)));
    /* pass */
    v.len = Str_len(s);
    /* pass */
    return v;
}

__attribute__((hot)) long long StrView_length(StrView self) {
    /* pass */
    return self.len;
}

__attribute__((hot)) char StrView_char_at(StrView self, long long i) {
    /* pass */
    return (*(self.data + i));
}

__attribute__((hot)) StrView StrView_slice(StrView self, long long start, long long _tr_v_count) {
    /* pass */
    StrView v = (StrView){0};
    /* pass */
    v.data = (self.data + start);
    /* pass */
    v.len = _tr_v_count;
    /* pass */
    return v;
}

__attribute__((hot)) TrStr StrView_to_str(StrView self) {
    /* pass */
    return _tr_str_wrap(_tr_str_slice(self.data, 0LL, self.len));
}

__attribute__((hot)) bool StrView_eq(StrView self, TrStr other) {
    /* pass */
    if ((self.len != Str_len(other))) {
        /* pass */
        return false;
    }
    /* pass */
    char* op = ((char*)(_tr_strz(other)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self.len)) {
        /* pass */
        if ((((long long)((*(self.data + i)))) != ((long long)((*(op + i)))))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool StrView_starts_with(StrView self, TrStr prefix) {
    /* pass */
    long long pl = Str_len(prefix);
    /* pass */
    if ((pl > self.len)) {
        /* pass */
        return false;
    }
    /* pass */
    char* pp = ((char*)(_tr_strz(prefix)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < pl)) {
        /* pass */
        if ((((long long)((*(self.data + i)))) != ((long long)((*(pp + i)))))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) StrView str_view(TrStr s, long long start, long long _tr_v_count) {
    /* pass */
    return StrView_of(s, start, _tr_v_count);
}

