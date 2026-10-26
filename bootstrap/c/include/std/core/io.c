#include "../../../tauraro_types.h"


__attribute__((hot)) TrStr read_file(TrStr path) {
#line 19 "./std/core/io.tr"
    /* unsafe block */
#line 20 "./std/core/io.tr"
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit_len("rb", 2LL)));
#line 21 "./std/core/io.tr"
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
#line 22 "./std/core/io.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 23 "./std/core/io.tr"
    _tr_c_fseek(fp, 0LL, 2LL);
#line 24 "./std/core/io.tr"
    long long size = _tr_c_ftell(fp);
#line 25 "./std/core/io.tr"
    _tr_c_fseek(fp, 0LL, 0LL);
#line 26 "./std/core/io.tr"
    char* buffer = _tr_c_malloc((size + 1LL));
#line 27 "./std/core/io.tr"
    _tr_c_fread(((void*)(buffer)), 1LL, size, fp);
#line 28 "./std/core/io.tr"
    (*(buffer + size) = '\0');
#line 29 "./std/core/io.tr"
    _tr_c_fclose(fp);
#line 30 "./std/core/io.tr"
    return _tr_str_wrap(_tr_str_wrap_len(buffer, size));
}

__attribute__((hot)) bool file_exists(TrStr path) {
#line 33 "./std/core/io.tr"
    /* unsafe block */
#line 34 "./std/core/io.tr"
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit_len("rb", 2LL)));
#line 35 "./std/core/io.tr"
    if ((((unsigned long long)(fp)) != ((unsigned long long)(0LL)))) {
#line 36 "./std/core/io.tr"
        _tr_c_fclose(fp);
#line 37 "./std/core/io.tr"
        return true;
    }
#line 38 "./std/core/io.tr"
    return false;
}

__attribute__((hot)) bool write_file(TrStr path, TrStr content) {
#line 41 "./std/core/io.tr"
    /* unsafe block */
#line 42 "./std/core/io.tr"
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit_len("wb", 2LL)));
#line 43 "./std/core/io.tr"
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
#line 44 "./std/core/io.tr"
        return false;
    }
#line 60 "./std/core/io.tr"
    long long length = _tr_str_lenv((content));
#line 61 "./std/core/io.tr"
    _tr_c_fwrite(((void*)(_tr_strz(content))), 1LL, length, fp);
#line 62 "./std/core/io.tr"
    _tr_c_fclose(fp);
#line 63 "./std/core/io.tr"
    return true;
}

__attribute__((hot)) bool append_file(TrStr path, TrStr content) {
#line 66 "./std/core/io.tr"
    /* unsafe block */
#line 67 "./std/core/io.tr"
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit_len("ab", 2LL)));
#line 68 "./std/core/io.tr"
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
#line 69 "./std/core/io.tr"
        return false;
    }
#line 71 "./std/core/io.tr"
    long long length = _tr_str_lenv((content));
#line 72 "./std/core/io.tr"
    _tr_c_fwrite(((void*)(_tr_strz(content))), 1LL, length, fp);
#line 73 "./std/core/io.tr"
    _tr_c_fclose(fp);
#line 74 "./std/core/io.tr"
    return true;
}

