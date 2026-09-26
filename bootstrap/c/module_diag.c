#include "tauraro_types.h"


__attribute__((hot)) bool color_enabled() {
    /* pass */
    if ((_tr_env_set(_tr_strz(_tr_str_lit_len("NO_COLOR", 8LL))) == 1LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((_tr_env_set(_tr_strz(_tr_str_lit_len("CLICOLOR_FORCE", 14LL))) == 1LL)) {
        /* pass */
        return true;
    }
    /* pass */
    return (_tr_stdout_supports_ansi() == 1LL);
}

__attribute__((hot)) TrStr esc() {
    /* pass */
    return _tr_str_wrap(_tr_ansi_esc());
}

__attribute__((hot)) TrStr paint(TrStr s, TrStr code) {
    /* pass */
    if ((!color_enabled())) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    TrStr e = esc();
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((e), (_tr_str_lit_len("[", 1LL)))); TrStr _cres = _tr_strx_concatv(_cl, (code)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("m", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (s)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (e)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("[0m", 3LL))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr c_red(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("1;31", 4LL));
}

__attribute__((hot)) TrStr c_yellow(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("1;33", 4LL));
}

__attribute__((hot)) TrStr c_green(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("1;32", 4LL));
}

__attribute__((hot)) TrStr c_cyan(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("1;36", 4LL));
}

__attribute__((hot)) TrStr c_dim(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("2", 1LL));
}

__attribute__((hot)) TrStr c_bold(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit_len("1", 1LL));
}

__attribute__((hot)) TrStr spaces(long long n) {
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t19 = _tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t19;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr repeat_char(TrStr ch, long long n) {
    /* pass */
    TrStr s = _tr_str_lit_len("", 0LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    long long k = n;
    /* pass */
    if ((k < 1LL)) {
        /* pass */
        k = 1LL;
    }
    /* pass */
    while ((i < k)) {
        /* pass */
        TrStr _strtmp_t20 = _tr_strx_concatv((s), (ch));
        _tr_str_release(s);
        s = _strtmp_t20;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr first_quoted(TrStr msg) {
    /* pass */
    long long a = _tr_str_index_of((msg).data, (_tr_str_lit_len("'", 1LL)).data);
    /* pass */
    if ((a < 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice((msg).data, (a + 1LL), _tr_str_lenv((msg))));
    /* pass */
    long long b = _tr_str_index_of((rest).data, (_tr_str_lit_len("'", 1LL)).data);
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice((rest).data, 0LL, b));
}

__attribute__((hot)) long long col_of(TrStr line, TrStr needle) {
    /* pass */
    if ((_tr_str_lenv((needle)) == 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long idx = _tr_str_index_of((line).data, (needle).data);
    /* pass */
    if ((idx < 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return (idx + 1LL);
}

__attribute__((hot)) TrStr loc_file(TrStr head) {
    /* pass */
    long long last = _tr_str_last_index_of((head).data, (_tr_str_lit_len(":", 1LL)).data);
    /* pass */
    if ((last < 0LL)) {
        /* pass */
        return _tr_str_lit_len("", 0LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice((head).data, 0LL, last));
}

__attribute__((hot)) long long loc_line(TrStr head) {
    /* pass */
    long long last = _tr_str_last_index_of((head).data, (_tr_str_lit_len(":", 1LL)).data);
    /* pass */
    TrStr numstr = _tr_str_retain(head);
    /* pass */
    if ((last >= 0LL)) {
        /* pass */
        TrStr _strtmp_t21 = _tr_str_wrap(_tr_str_slice((head).data, (last + 1LL), _tr_str_lenv((head))));
        _tr_str_release(numstr);
        numstr = _strtmp_t21;
    }
    /* pass */
    return _tr_str_to_int(_tr_strz(numstr));
}

