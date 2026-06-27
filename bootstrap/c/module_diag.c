#include "tauraro_types.h"


__attribute__((hot)) bool color_enabled() {
    /* pass */
    if ((_tr_env_set(_tr_strz(_tr_str_lit("NO_COLOR"))) == 1LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((_tr_env_set(_tr_strz(_tr_str_lit("CLICOLOR_FORCE"))) == 1LL)) {
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
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(e), _tr_strz(_tr_str_lit("[")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(code)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("m"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(s)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("[0m"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr c_red(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("1;31"));
}

__attribute__((hot)) TrStr c_yellow(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("1;33"));
}

__attribute__((hot)) TrStr c_green(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("1;32"));
}

__attribute__((hot)) TrStr c_cyan(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("1;36"));
}

__attribute__((hot)) TrStr c_dim(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("2"));
}

__attribute__((hot)) TrStr c_bold(TrStr s) {
    /* pass */
    return paint(s, _tr_str_lit("1"));
}

__attribute__((hot)) TrStr spaces(long long n) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t16 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" ")));
        _tr_str_release(s);
        s = _strtmp_t16;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr repeat_char(TrStr ch, long long n) {
    /* pass */
    TrStr s = _tr_str_lit("");
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
        TrStr _strtmp_t17 = _tr_strx_concat(_tr_strz(s), _tr_strz(ch));
        _tr_str_release(s);
        s = _strtmp_t17;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr first_quoted(TrStr msg) {
    /* pass */
    long long a = _tr_str_index_of(_tr_strz(msg), _tr_strz(_tr_str_lit("'")));
    /* pass */
    if ((a < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(msg), (a + 1LL), _tr_strlen(_tr_strz(msg))));
    /* pass */
    long long b = _tr_str_index_of(_tr_strz(rest), _tr_strz(_tr_str_lit("'")));
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, b));
}

__attribute__((hot)) long long col_of(TrStr line, TrStr needle) {
    /* pass */
    if ((_tr_strlen(_tr_strz(needle)) == 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long idx = _tr_str_index_of(_tr_strz(line), _tr_strz(needle));
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
    long long last = _tr_str_last_index_of(_tr_strz(head), _tr_strz(_tr_str_lit(":")));
    /* pass */
    if ((last < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(head), 0LL, last));
}

__attribute__((hot)) long long loc_line(TrStr head) {
    /* pass */
    long long last = _tr_str_last_index_of(_tr_strz(head), _tr_strz(_tr_str_lit(":")));
    /* pass */
    TrStr numstr = _tr_str_retain(head);
    /* pass */
    if ((last >= 0LL)) {
        /* pass */
        TrStr _strtmp_t18 = _tr_str_wrap(_tr_str_slice(_tr_strz(head), (last + 1LL), _tr_strlen(_tr_strz(head))));
        _tr_str_release(numstr);
        numstr = _strtmp_t18;
    }
    /* pass */
    return _tr_str_to_int(_tr_strz(numstr));
}

