#include "tauraro_types.h"

TrStr _cov_obj_base(TrStr c_path);
void _cov_rm(TrStr path);
TrStr _cov_tool(TrStr cc);
long long _cov_int(TrStr s);
bool _cov_is_std(TrStr path);
TrStr _cov_ranges(List_i64* lines);
TrStr _cov_pad(TrStr s, long long w);
TrStr _cov_lpad(TrStr s, long long w);

__attribute__((hot)) TrStr _cov_obj_base(TrStr c_path) {
#line 19 "src/coverage.tr"
    long long n = _tr_str_lenv((c_path));
#line 20 "src/coverage.tr"
    if (((n > 2LL) && ({ TrStr _cmpa_t2214 = (_tr_str_slicev((c_path), (n - 2LL), n)); TrStr _cmpb_t2215 = (_tr_str_lit_len(".c", 2LL)); _Bool _cmpr_t2216 = _tr_str_eqv(_cmpa_t2214, _cmpb_t2215); _tr_str_release(_cmpa_t2214); _cmpr_t2216; }))) {
#line 20 "src/coverage.tr"
        return _tr_str_slicev((c_path), 0LL, (n - 2LL));
    }
#line 21 "src/coverage.tr"
    return _tr_str_retain(c_path);
}

__attribute__((hot)) void _cov_rm(TrStr path) {
#line 24 "src/coverage.tr"
    if (_tr_is_windows()) {
#line 24 "src/coverage.tr"
        ({ TrStr _aet_t2217 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_replacev((path), (_tr_str_lit_len("/", 1LL)), (_tr_str_lit_len("\\", 1LL)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("del /Q \"", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" 2>nul >nul", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2217.data); _tr_str_release(_aet_t2217); });
    } else {
#line 25 "src/coverage.tr"
        ({ TrStr _aet_t2218 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("rm -f \"", 7LL)), (path))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_system(_aet_t2218.data); _tr_str_release(_aet_t2218); });
    }
}

__attribute__((hot)) void coverage_reset(List_TrStr* c_files) {
#line 29 "src/coverage.tr"
    long long i = 0LL;
#line 30 "src/coverage.tr"
    while ((i < c_files->len)) {
#line 31 "src/coverage.tr"
        TrStr g = ({ TrStr _at_t2219 = (List_TrStr_get(c_files, i)); __auto_type _wr = (({ TrStr _tr_cl = (_cov_obj_base(_at_t2219)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".gcda", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); _tr_str_release(_at_t2219); _wr; });
#line 32 "src/coverage.tr"
        if (file_exists(g)) {
#line 32 "src/coverage.tr"
            _cov_rm(g);
        }
#line 33 "src/coverage.tr"
        i = (i + 1LL);
        _tr_str_release(g);
    }
}

__attribute__((hot)) TrStr _cov_tool(TrStr cc) {
#line 38 "src/coverage.tr"
    TrStr null_dev = _tr_str_lit_len("/dev/null", 9LL);
#line 39 "src/coverage.tr"
    if (_tr_is_windows()) {
#line 39 "src/coverage.tr"
        TrStr _strtmp_t2220 = _tr_str_lit_len("nul", 3LL);
        _tr_str_release(null_dev);
        null_dev = _strtmp_t2220;
    }
#line 40 "src/coverage.tr"
    if ((_tr_str_index_ofv((cc), (_tr_str_lit_len("clang", 5LL))) >= 0LL)) {
#line 41 "src/coverage.tr"
        if (({ TrStr _aet_t2221 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("llvm-cov --version >", 20LL)), (null_dev))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 2>&1", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = ((_tr_system(_aet_t2221.data) == 0LL)); _tr_str_release(_aet_t2221); _wr; })) {
#line 41 "src/coverage.tr"
            _tr_str_release(null_dev);
            return _tr_str_lit_len("llvm-cov gcov", 13LL);
        }
    }
#line 42 "src/coverage.tr"
    _tr_str_release(null_dev);
    return _tr_str_lit_len("gcov", 4LL);
}

__attribute__((hot)) long long _cov_int(TrStr s) {
#line 45 "src/coverage.tr"
    TrStr t = _tr_str_stripv((s));
#line 46 "src/coverage.tr"
    long long v = 0LL;
#line 47 "src/coverage.tr"
    long long i = 0LL;
#line 48 "src/coverage.tr"
    bool any = false;
#line 49 "src/coverage.tr"
    while ((i < _tr_str_lenv((t)))) {
#line 50 "src/coverage.tr"
        TrStr c = _tr_str_slicev((t), i, (i + 1LL));
#line 51 "src/coverage.tr"
        if (((strcmp(_tr_strz(c), _tr_strz(_tr_str_lit_len("0", 1LL))) >= 0) && (strcmp(_tr_strz(c), _tr_strz(_tr_str_lit_len("9", 1LL))) <= 0))) {
#line 52 "src/coverage.tr"
            v = ((v * 10LL) + _tr_str_to_int(_tr_strz(c)));
#line 53 "src/coverage.tr"
            any = true;
        } else if (_tr_str_eqv((c), (_tr_str_lit_len("*", 1LL)))) {
#line 55 "src/coverage.tr"
            /* pass */
        } else {
#line 57 "src/coverage.tr"
            _tr_str_release(c);
            break;
        }
#line 58 "src/coverage.tr"
        i = (i + 1LL);
        _tr_str_release(c);
    }
#line 59 "src/coverage.tr"
    if ((!any)) {
#line 59 "src/coverage.tr"
        _tr_str_release(t);
        return (-1LL);
    }
#line 60 "src/coverage.tr"
    long long _trret517 = v;
    _tr_str_release(t);
    return _trret517;
}

__attribute__((hot)) bool _cov_is_std(TrStr path) {
#line 63 "src/coverage.tr"
    return (_tr_str_starts_withv((path), (_tr_str_lit_len("std/", 4LL))) || (_tr_str_index_ofv((path), (_tr_str_lit_len("/std/", 5LL))) >= 0LL));
}

__attribute__((hot)) TrStr _cov_ranges(List_i64* lines) {
#line 66 "src/coverage.tr"
    StringBuilder* sb = StringBuilder_init(64LL);
#line 67 "src/coverage.tr"
    long long i = 0LL;
#line 68 "src/coverage.tr"
    while ((i < lines->len)) {
#line 69 "src/coverage.tr"
        long long a = List_i64_get(lines, i);
#line 70 "src/coverage.tr"
        long long b = a;
#line 71 "src/coverage.tr"
        while ((((i + 1LL) < lines->len) && (List_i64_get(lines, (i + 1LL)) == (b + 1LL)))) {
#line 72 "src/coverage.tr"
            i = (i + 1LL);
#line 73 "src/coverage.tr"
            b = List_i64_get(lines, i);
        }
#line 74 "src/coverage.tr"
        if ((sb->buf->len > 0LL)) {
#line 74 "src/coverage.tr"
            StringBuilder_append(sb, _tr_str_lit_len(", ", 2LL));
        }
#line 75 "src/coverage.tr"
        if ((a == b)) {
#line 75 "src/coverage.tr"
            ({ TrStr _sbt_t2222 = (_tr_str_wrap(_tr_int_to_str((long long)(a)))); StringBuilder_append(sb, _sbt_t2222); _tr_str_release(_sbt_t2222); });
        } else {
#line 76 "src/coverage.tr"
            ({ __auto_type _ord_t2223 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(a)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2224 = (({ TrStr _tr_cl = (_ord_t2223); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(b)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); StringBuilder_append(sb, _sbt_t2224); _tr_str_release(_sbt_t2224); });
        }
#line 77 "src/coverage.tr"
        i = (i + 1LL);
    }
#line 78 "src/coverage.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _cov_pad(TrStr s, long long w) {
#line 81 "src/coverage.tr"
    TrStr out = _tr_str_retain(s);
#line 82 "src/coverage.tr"
    while ((_tr_str_lenv((out)) < w)) {
#line 82 "src/coverage.tr"
        ({ TrStr _ap_t2225 = (_tr_str_lit_len(" ", 1LL)); _tr_str_append(&out, _ap_t2225); });
    }
#line 83 "src/coverage.tr"
    TrStr _trret518 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret518;
}

__attribute__((hot)) TrStr _cov_lpad(TrStr s, long long w) {
#line 86 "src/coverage.tr"
    TrStr out = _tr_str_retain(s);
#line 87 "src/coverage.tr"
    while ((_tr_str_lenv((out)) < w)) {
#line 87 "src/coverage.tr"
        TrStr _strtmp_t2226 = _tr_strx_concatv((_tr_str_lit_len(" ", 1LL)), (out));
        _tr_str_release(out);
        out = _strtmp_t2226;
    }
#line 88 "src/coverage.tr"
    TrStr _trret519 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret519;
}

__attribute__((hot)) long long coverage_report(List_TrStr* c_files, TrStr cc, TrStr lcov_path, bool include_std) {
#line 93 "src/coverage.tr"
    TrStr tool = _cov_tool(cc);
#line 94 "src/coverage.tr"
    TrMap* counts = _tr_dict_new(1024LL);
#line 95 "src/coverage.tr"
    List_TrStr* files = (void*)List_TrStr_new();
#line 96 "src/coverage.tr"
    TrMap* max_line = _tr_dict_new(16LL);
#line 97 "src/coverage.tr"
    TrStr tmp = _tr_str_lit_len("_tr_gcov_out.txt", 16LL);
#line 98 "src/coverage.tr"
    long long i = 0LL;
#line 99 "src/coverage.tr"
    while ((i < c_files->len)) {
#line 100 "src/coverage.tr"
        TrStr cpath = List_TrStr_get(c_files, i);
#line 101 "src/coverage.tr"
        TrStr base = _cov_obj_base(cpath);
#line 102 "src/coverage.tr"
        if (({ TrStr _at_t2227 = (_tr_strx_concatv((base), (_tr_str_lit_len(".gcda", 5LL)))); __auto_type _wr = (file_exists(_at_t2227)); _tr_str_release(_at_t2227); _wr; })) {
#line 103 "src/coverage.tr"
            TrStr cmd = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((tool), (_tr_str_lit_len(" -t -o \"", 8LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (base)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".o\" \"", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cpath)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" > \"", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tmp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\" 2>&1", 6LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 104 "src/coverage.tr"
            _tr_system(_tr_strz(cmd));
#line 105 "src/coverage.tr"
            TrStr text = _tr_str_lit_len("", 0LL);
#line 106 "src/coverage.tr"
            if (file_exists(tmp)) {
#line 106 "src/coverage.tr"
                TrStr _strtmp_t2228 = read_file(tmp);
                _tr_str_release(text);
                text = _strtmp_t2228;
            }
#line 107 "src/coverage.tr"
            TrStr src = _tr_str_lit_len("", 0LL);
#line 108 "src/coverage.tr"
            long long pos = 0LL;
#line 109 "src/coverage.tr"
            while ((pos < _tr_str_lenv((text)))) {
#line 110 "src/coverage.tr"
                long long nl = _tr_str_index_ofv((_tr_str_slicev((text), pos, _tr_str_lenv((text)))), (_tr_str_lit_len("\n", 1LL)));
#line 111 "src/coverage.tr"
                TrStr line = _tr_str_lit_len("", 0LL);
#line 112 "src/coverage.tr"
                if ((nl < 0LL)) {
#line 113 "src/coverage.tr"
                    TrStr _strtmp_t2229 = _tr_str_slicev((text), pos, _tr_str_lenv((text)));
                    _tr_str_release(line);
                    line = _strtmp_t2229;
#line 114 "src/coverage.tr"
                    pos = _tr_str_lenv((text));
                } else {
#line 116 "src/coverage.tr"
                    TrStr _strtmp_t2230 = _tr_str_slicev((text), pos, (pos + nl));
                    _tr_str_release(line);
                    line = _strtmp_t2230;
#line 117 "src/coverage.tr"
                    pos = ((pos + nl) + 1LL);
                }
#line 118 "src/coverage.tr"
                long long c1 = _tr_str_index_ofv((line), (_tr_str_lit_len(":", 1LL)));
#line 119 "src/coverage.tr"
                if ((c1 < 0LL)) {
#line 119 "src/coverage.tr"
                    _tr_str_release(line);
                    continue;
                }
#line 120 "src/coverage.tr"
                TrStr rest = _tr_str_slicev((line), (c1 + 1LL), _tr_str_lenv((line)));
#line 121 "src/coverage.tr"
                long long c2 = _tr_str_index_ofv((rest), (_tr_str_lit_len(":", 1LL)));
#line 122 "src/coverage.tr"
                if ((c2 < 0LL)) {
#line 122 "src/coverage.tr"
                    _tr_str_release(line);
                    _tr_str_release(rest);
                    continue;
                }
#line 123 "src/coverage.tr"
                long long lno = ({ TrStr _at_t2231 = (_tr_str_slicev((rest), 0LL, c2)); __auto_type _wr = (_cov_int(_at_t2231)); _tr_str_release(_at_t2231); _wr; });
#line 124 "src/coverage.tr"
                TrStr cnt_s = _tr_str_stripv((_tr_str_slicev((line), 0LL, c1)));
#line 125 "src/coverage.tr"
                if ((lno == 0LL)) {
#line 126 "src/coverage.tr"
                    TrStr tail = _tr_str_slicev((rest), (c2 + 1LL), _tr_str_lenv((rest)));
#line 127 "src/coverage.tr"
                    if (_tr_str_starts_withv((tail), (_tr_str_lit_len("Source:", 7LL)))) {
#line 127 "src/coverage.tr"
                        TrStr _strtmp_t2232 = _tr_str_stripv((_tr_str_slicev((tail), 7LL, _tr_str_lenv((tail)))));
                        _tr_str_release(src);
                        src = _strtmp_t2232;
                    }
#line 128 "src/coverage.tr"
                    _tr_str_release(line);
                    _tr_str_release(rest);
                    _tr_str_release(cnt_s);
                    _tr_str_release(tail);
                    continue;
                }
#line 129 "src/coverage.tr"
                if ((((lno < 0LL) || _tr_str_eqv((src), (_tr_str_lit_len("", 0LL)))) || (!_tr_str_ends_withv((src), (_tr_str_lit_len(".tr", 3LL)))))) {
#line 129 "src/coverage.tr"
                    _tr_str_release(line);
                    _tr_str_release(rest);
                    _tr_str_release(cnt_s);
                    continue;
                }
#line 130 "src/coverage.tr"
                if (_tr_str_eqv((cnt_s), (_tr_str_lit_len("-", 1LL)))) {
#line 130 "src/coverage.tr"
                    _tr_str_release(line);
                    _tr_str_release(rest);
                    _tr_str_release(cnt_s);
                    continue;
                }
#line 131 "src/coverage.tr"
                long long cnt = 0LL;
#line 132 "src/coverage.tr"
                if ((_tr_str_starts_withv((cnt_s), (_tr_str_lit_len("#####", 5LL))) || _tr_str_starts_withv((cnt_s), (_tr_str_lit_len("=====", 5LL))))) {
#line 132 "src/coverage.tr"
                    cnt = 0LL;
                } else {
#line 134 "src/coverage.tr"
                    cnt = _cov_int(cnt_s);
#line 135 "src/coverage.tr"
                    if ((cnt < 0LL)) {
#line 135 "src/coverage.tr"
                        _tr_str_release(line);
                        _tr_str_release(rest);
                        _tr_str_release(cnt_s);
                        continue;
                    }
                }
#line 136 "src/coverage.tr"
                if (((!include_std) && _cov_is_std(src))) {
#line 136 "src/coverage.tr"
                    _tr_str_release(line);
                    _tr_str_release(rest);
                    _tr_str_release(cnt_s);
                    continue;
                }
#line 137 "src/coverage.tr"
                TrStr key = ({ TrStr _tr_cl = (_tr_strx_concatv((src), (_tr_str_lit_len("\t", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(lno)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 138 "src/coverage.tr"
                if (_tr_dict_contains(counts, _tr_strz(key))) {
#line 138 "src/coverage.tr"
                    _tr_dict_set(counts, _tr_strz(key), (((long long)(uintptr_t)_tr_dict_get(counts, _tr_strz(key))) + cnt));
                } else {
#line 140 "src/coverage.tr"
                    _tr_dict_set(counts, _tr_strz(key), cnt);
#line 141 "src/coverage.tr"
                    if ((!_tr_dict_contains(max_line, _tr_strz(src)))) {
#line 142 "src/coverage.tr"
                        _tr_dict_set(max_line, _tr_strz(src), 0LL);
#line 143 "src/coverage.tr"
                        List_TrStr_append(files, src);
                    }
#line 144 "src/coverage.tr"
                    if ((lno > ((long long)(uintptr_t)_tr_dict_get(max_line, _tr_strz(src))))) {
#line 144 "src/coverage.tr"
                        _tr_dict_set(max_line, _tr_strz(src), lno);
                    }
                }
                _tr_str_release(line);
                _tr_str_release(rest);
                _tr_str_release(cnt_s);
                _tr_str_release(key);
            }
            _tr_str_release(cmd);
        }
#line 145 "src/coverage.tr"
        i = (i + 1LL);
        _tr_str_release(cpath);
        _tr_str_release(base);
    }
#line 146 "src/coverage.tr"
    _cov_rm(tmp);
#line 147 "src/coverage.tr"
    if ((files->len == 0LL)) {
#line 148 "src/coverage.tr"
        ({ printf("%s", _tr_strz(_tr_str_lit_len("coverage: no line data found (did the program run to completion?)", 65LL))); printf("\n"); });
#line 149 "src/coverage.tr"
        _tr_str_release(tool);
        Dict_free(counts);
        List_TrStr_free(files);
        Dict_free(max_line);
        _tr_str_release(tmp);
        return 1LL;
    }
#line 152 "src/coverage.tr"
    long long fi = 1LL;
#line 153 "src/coverage.tr"
    while ((fi < files->len)) {
#line 154 "src/coverage.tr"
        TrStr cur = List_TrStr_get(files, fi);
#line 155 "src/coverage.tr"
        long long fj = (fi - 1LL);
#line 156 "src/coverage.tr"
        while (((fj >= 0LL) && (strcmp(_tr_strz(List_TrStr_get(files, fj)), _tr_strz(cur)) > 0))) {
#line 157 "src/coverage.tr"
            ({ TrStr _at_t2233 = (List_TrStr_get(files, fj)); List_TrStr_set(files, (fj + 1LL), _at_t2233); _tr_str_release(_at_t2233); });
#line 158 "src/coverage.tr"
            fj = (fj - 1LL);
        }
#line 159 "src/coverage.tr"
        List_TrStr_set(files, (fj + 1LL), cur);
#line 160 "src/coverage.tr"
        fi = (fi + 1LL);
        _tr_str_release(cur);
    }
#line 162 "src/coverage.tr"
    StringBuilder* lcov = StringBuilder_init(4096LL);
#line 163 "src/coverage.tr"
    long long width = 4LL;
#line 164 "src/coverage.tr"
    long long k = 0LL;
#line 165 "src/coverage.tr"
    while ((k < files->len)) {
#line 166 "src/coverage.tr"
        if ((_tr_str_lenv((List_TrStr_get(files, k))) > width)) {
#line 166 "src/coverage.tr"
            width = _tr_str_lenv((List_TrStr_get(files, k)));
        }
#line 167 "src/coverage.tr"
        k = (k + 1LL);
    }
#line 168 "src/coverage.tr"
    ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_cov_pad(_tr_str_lit_len("File", 4LL), width)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("   Lines   Hit  Cover  Missing", 30LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 169 "src/coverage.tr"
    long long tot_lines = 0LL;
#line 170 "src/coverage.tr"
    long long tot_hit = 0LL;
#line 171 "src/coverage.tr"
    k = 0LL;
#line 172 "src/coverage.tr"
    while ((k < files->len)) {
#line 173 "src/coverage.tr"
        TrStr f = List_TrStr_get(files, k);
#line 174 "src/coverage.tr"
        long long nlines = 0LL;
#line 175 "src/coverage.tr"
        long long nhit = 0LL;
#line 176 "src/coverage.tr"
        List_i64* missing = (void*)List_i64_new();
#line 177 "src/coverage.tr"
        ({ TrStr _sbt_t2234 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("SF:", 3LL)), (f))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(lcov, _sbt_t2234); _tr_str_release(_sbt_t2234); });
#line 178 "src/coverage.tr"
        long long ln = 1LL;
#line 179 "src/coverage.tr"
        long long last = ((long long)(uintptr_t)_tr_dict_get(max_line, _tr_strz(f)));
#line 180 "src/coverage.tr"
        while ((ln <= last)) {
#line 181 "src/coverage.tr"
            TrStr key = ({ TrStr _tr_cl = (_tr_strx_concatv((f), (_tr_str_lit_len("\t", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; });
#line 182 "src/coverage.tr"
            if (_tr_dict_contains(counts, _tr_strz(key))) {
#line 183 "src/coverage.tr"
                long long c = ((long long)(uintptr_t)_tr_dict_get(counts, _tr_strz(key)));
#line 184 "src/coverage.tr"
                nlines = (nlines + 1LL);
#line 185 "src/coverage.tr"
                if ((c > 0LL)) {
#line 185 "src/coverage.tr"
                    nhit = (nhit + 1LL);
                } else {
#line 186 "src/coverage.tr"
                    List_i64_append(missing, ln);
                }
#line 187 "src/coverage.tr"
                ({ __auto_type _ord_t2235 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("DA:", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(",", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2236 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2235); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(c)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(lcov, _sbt_t2236); _tr_str_release(_sbt_t2236); });
            }
#line 188 "src/coverage.tr"
            ln = (ln + 1LL);
            _tr_str_release(key);
        }
#line 189 "src/coverage.tr"
        ({ __auto_type _ord_t2237 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nlines)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("LF:", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\nLH:", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _sbt_t2238 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2237); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(nhit)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\nend_of_record\n", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); StringBuilder_append(lcov, _sbt_t2238); _tr_str_release(_sbt_t2238); });
#line 190 "src/coverage.tr"
        double pct = 100.0;
#line 191 "src/coverage.tr"
        if ((nlines > 0LL)) {
#line 191 "src/coverage.tr"
            pct = ((((double)(nhit)) * 100.0) / ((double)(nlines)));
        }
#line 192 "src/coverage.tr"
        TrStr pct_s = ({ __auto_type _ord_t2239 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(_tr_idiv((long long)(((long long)(((pct * 10.0) + 0.5)))), (long long)(10LL)))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2239); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_tr_imod((long long)(((long long)(((pct * 10.0) + 0.5)))), (long long)(10LL)))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("%", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 193 "src/coverage.tr"
        ({ __auto_type _ord_t2241 = (_cov_pad(f, width)); TrStr _at_t2240 = (_tr_str_wrap(_tr_int_to_str((long long)(nlines)))); __auto_type _ord_t2243 = (({ TrStr _tr_cl = (_ord_t2241); TrStr _tr_cr = (_cov_lpad(_at_t2240, 8LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _at_t2242 = (_tr_str_wrap(_tr_int_to_str((long long)(nhit)))); __auto_type _ord_t2244 = (({ TrStr _tr_cl = (_ord_t2243); TrStr _tr_cr = (_cov_lpad(_at_t2242, 6LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _ord_t2245 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2244); TrStr _tr_cr = (_cov_lpad(pct_s, 7LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("  ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t2245); TrStr _tr_cr = (_cov_ranges(missing)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); _tr_str_release(_at_t2240); _tr_str_release(_at_t2242); });
#line 194 "src/coverage.tr"
        tot_lines = (tot_lines + nlines);
#line 195 "src/coverage.tr"
        tot_hit = (tot_hit + nhit);
#line 196 "src/coverage.tr"
        k = (k + 1LL);
        _tr_str_release(f);
        _tr_str_release(pct_s);
    }
#line 197 "src/coverage.tr"
    double tpct = 100.0;
#line 198 "src/coverage.tr"
    if ((tot_lines > 0LL)) {
#line 198 "src/coverage.tr"
        tpct = ((((double)(tot_hit)) * 100.0) / ((double)(tot_lines)));
    }
#line 199 "src/coverage.tr"
    TrStr tpct_s = ({ __auto_type _ord_t2246 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(_tr_idiv((long long)(((long long)(((tpct * 10.0) + 0.5)))), (long long)(10LL)))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2246); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(_tr_imod((long long)(((long long)(((tpct * 10.0) + 0.5)))), (long long)(10LL)))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("%", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
#line 200 "src/coverage.tr"
    ({ __auto_type _ord_t2248 = (_cov_pad(_tr_str_lit_len("TOTAL", 5LL), width)); TrStr _at_t2247 = (_tr_str_wrap(_tr_int_to_str((long long)(tot_lines)))); __auto_type _ord_t2250 = (({ TrStr _tr_cl = (_ord_t2248); TrStr _tr_cr = (_cov_lpad(_at_t2247, 8LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _at_t2249 = (_tr_str_wrap(_tr_int_to_str((long long)(tot_hit)))); __auto_type _ord_t2251 = (({ TrStr _tr_cl = (_ord_t2250); TrStr _tr_cr = (_cov_lpad(_at_t2249, 6LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_ord_t2251); TrStr _tr_cr = (_cov_lpad(tpct_s, 7LL)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; }))); printf("\n"); }); _tr_str_release(_at_t2247); _tr_str_release(_at_t2249); });
#line 201 "src/coverage.tr"
    if ((!_tr_str_eqv((lcov_path), (_tr_str_lit_len("", 0LL))))) {
#line 202 "src/coverage.tr"
        ({ TrStr _at_t2252 = (StringObj_as_str(StringBuilder_to_string(lcov))); write_file(lcov_path, _at_t2252); _tr_str_release(_at_t2252); });
#line 203 "src/coverage.tr"
        ({ printf("%s", _tr_strz(_tr_strx_concatv((_tr_str_lit_len("coverage: LCOV data written to ", 31LL)), (lcov_path)))); printf("\n"); });
    }
#line 204 "src/coverage.tr"
    _tr_str_release(tool);
    Dict_free(counts);
    List_TrStr_free(files);
    Dict_free(max_line);
    _tr_str_release(tmp);
    _tr_str_release(tpct_s);
    StringBuilder__tr_fn_free(lcov);
    return 0LL;
}

