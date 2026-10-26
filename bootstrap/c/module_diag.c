#include "tauraro_types.h"


__attribute__((hot)) bool color_enabled() {
#line 18 "src/diag.tr"
    if ((_tr_env_set(_tr_strz(_tr_str_lit_len("NO_COLOR", 8LL))) == 1LL)) {
#line 19 "src/diag.tr"
        return false;
    }
#line 20 "src/diag.tr"
    if ((_tr_env_set(_tr_strz(_tr_str_lit_len("CLICOLOR_FORCE", 14LL))) == 1LL)) {
#line 21 "src/diag.tr"
        return true;
    }
#line 22 "src/diag.tr"
    return (_tr_stdout_supports_ansi() == 1LL);
}

__attribute__((hot)) TrStr esc() {
#line 26 "src/diag.tr"
    return _tr_str_wrap(_tr_ansi_esc());
}

__attribute__((hot)) TrStr paint(TrStr s, TrStr code) {
#line 30 "src/diag.tr"
    if ((!color_enabled())) {
#line 31 "src/diag.tr"
        return _tr_str_retain(s);
    }
#line 32 "src/diag.tr"
    TrStr e = esc();
#line 33 "src/diag.tr"
    TrStr _trret8 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((e), (_tr_str_lit_len("[", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (code)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("m", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (e)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[0m", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
    _tr_str_release(e);
    return _trret8;
}

__attribute__((hot)) TrStr c_red(TrStr s) {
#line 35 "src/diag.tr"
    return paint(s, _tr_str_lit_len("1;31", 4LL));
}

__attribute__((hot)) TrStr c_yellow(TrStr s) {
#line 36 "src/diag.tr"
    return paint(s, _tr_str_lit_len("1;33", 4LL));
}

__attribute__((hot)) TrStr c_green(TrStr s) {
#line 37 "src/diag.tr"
    return paint(s, _tr_str_lit_len("1;32", 4LL));
}

__attribute__((hot)) TrStr c_cyan(TrStr s) {
#line 38 "src/diag.tr"
    return paint(s, _tr_str_lit_len("1;36", 4LL));
}

__attribute__((hot)) TrStr c_dim(TrStr s) {
#line 39 "src/diag.tr"
    return paint(s, _tr_str_lit_len("2", 1LL));
}

__attribute__((hot)) TrStr c_bold(TrStr s) {
#line 40 "src/diag.tr"
    return paint(s, _tr_str_lit_len("1", 1LL));
}

__attribute__((hot)) TrStr spaces(long long n) {
#line 44 "src/diag.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 45 "src/diag.tr"
    long long i = 0LL;
#line 46 "src/diag.tr"
    while ((i < n)) {
#line 47 "src/diag.tr"
        TrStr _strtmp_t29 = _tr_strx_concatv((s), (_tr_str_lit_len(" ", 1LL)));
        _tr_str_release(s);
        s = _strtmp_t29;
#line 48 "src/diag.tr"
        i = (i + 1LL);
    }
#line 49 "src/diag.tr"
    TrStr _trret9 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret9;
}

__attribute__((hot)) TrStr repeat_char(TrStr ch, long long n) {
#line 53 "src/diag.tr"
    TrStr s = _tr_str_lit_len("", 0LL);
#line 54 "src/diag.tr"
    long long i = 0LL;
#line 55 "src/diag.tr"
    long long k = n;
#line 56 "src/diag.tr"
    if ((k < 1LL)) {
#line 56 "src/diag.tr"
        k = 1LL;
    }
#line 57 "src/diag.tr"
    while ((i < k)) {
#line 58 "src/diag.tr"
        ({ TrStr _ap_t30 = (ch); _tr_str_append(&s, _ap_t30); });
#line 59 "src/diag.tr"
        i = (i + 1LL);
    }
#line 60 "src/diag.tr"
    TrStr _trret10 = _tr_str_retain(s);
    _tr_str_release(s);
    return _trret10;
}

__attribute__((hot)) TrStr first_quoted(TrStr msg) {
#line 66 "src/diag.tr"
    long long a = _tr_str_index_ofv((msg), (_tr_str_lit_len("'", 1LL)));
#line 67 "src/diag.tr"
    if ((a < 0LL)) {
#line 68 "src/diag.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 69 "src/diag.tr"
    TrStr rest = _tr_str_slicev((msg), (a + 1LL), _tr_str_lenv((msg)));
#line 70 "src/diag.tr"
    long long b = _tr_str_index_ofv((rest), (_tr_str_lit_len("'", 1LL)));
#line 71 "src/diag.tr"
    if ((b < 0LL)) {
#line 72 "src/diag.tr"
        _tr_str_release(rest);
        return _tr_str_lit_len("", 0LL);
    }
#line 73 "src/diag.tr"
    TrStr _trret11 = _tr_str_slicev((rest), 0LL, b);
    _tr_str_release(rest);
    return _trret11;
}

__attribute__((hot)) long long col_of(TrStr line, TrStr needle) {
#line 77 "src/diag.tr"
    if ((_tr_str_lenv((needle)) == 0LL)) {
#line 78 "src/diag.tr"
        return 0LL;
    }
#line 79 "src/diag.tr"
    long long idx = _tr_str_index_ofv((line), (needle));
#line 80 "src/diag.tr"
    if ((idx < 0LL)) {
#line 81 "src/diag.tr"
        return 0LL;
    }
#line 82 "src/diag.tr"
    return (idx + 1LL);
}

__attribute__((hot)) TrStr loc_file(TrStr head) {
#line 87 "src/diag.tr"
    long long last = _tr_str_last_index_ofv((head), (_tr_str_lit_len(":", 1LL)));
#line 88 "src/diag.tr"
    if ((last < 0LL)) {
#line 89 "src/diag.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 90 "src/diag.tr"
    return _tr_str_slicev((head), 0LL, last);
}

__attribute__((hot)) long long loc_line(TrStr head) {
#line 94 "src/diag.tr"
    long long last = _tr_str_last_index_ofv((head), (_tr_str_lit_len(":", 1LL)));
#line 95 "src/diag.tr"
    TrStr numstr = _tr_str_retain(head);
#line 96 "src/diag.tr"
    if ((last >= 0LL)) {
#line 97 "src/diag.tr"
        TrStr _strtmp_t31 = _tr_str_slicev((head), (last + 1LL), _tr_str_lenv((head)));
        _tr_str_release(numstr);
        numstr = _strtmp_t31;
    }
#line 98 "src/diag.tr"
    long long _trret12 = _tr_str_to_int(_tr_strz(numstr));
    _tr_str_release(numstr);
    return _trret12;
}

