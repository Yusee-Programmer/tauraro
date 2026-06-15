#include "../../tauraro_types.h"


__attribute__((hot)) TrStr read_file(TrStr path) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit("rb")));
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    _tr_c_fseek(fp, 0LL, 2LL);
    /* pass */
    long long size = _tr_c_ftell(fp);
    /* pass */
    _tr_c_fseek(fp, 0LL, 0LL);
    /* pass */
    char* buffer = _tr_c_malloc((size + 1LL));
    /* pass */
    _tr_c_fread(((void*)(buffer)), 1LL, size, fp);
    /* pass */
    (*(buffer + size) = '\0');
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return _tr_str_wrap(_tr_str_wrap(buffer));
}

__attribute__((hot)) bool file_exists(TrStr path) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit("rb")));
    /* pass */
    if ((((unsigned long long)(fp)) != ((unsigned long long)(0LL)))) {
        /* pass */
        _tr_c_fclose(fp);
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool write_file(TrStr path, TrStr content) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit("wb")));
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long length = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(content)));
    /* pass */
    while ((((long long)((*(p + length)))) != 0LL)) {
        /* pass */
        length = (length + 1LL);
    }
    /* pass */
    _tr_c_fwrite(((void*)(_tr_strz(content))), 1LL, length, fp);
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return true;
}

__attribute__((hot)) bool append_file(TrStr path, TrStr content) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit("ab")));
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long length = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(content)));
    /* pass */
    while ((((long long)((*(p + length)))) != 0LL)) {
        /* pass */
        length = (length + 1LL);
    }
    /* pass */
    _tr_c_fwrite(((void*)(_tr_strz(content))), 1LL, length, fp);
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return true;
}

