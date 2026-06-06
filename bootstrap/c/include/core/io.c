#include "../../tauraro_types.h"


__attribute__((hot)) char* read_file(char* path) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(path, "rb");
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return "";
    }
    /* pass */
    _tr_c_fseek(fp, 0LL, 2LL);
    /* pass */
    long long size = _tr_c_ftell(fp);
    /* pass */
    _tr_c_fseek(fp, 0LL, 0LL);
    /* pass */
    char* buffer = ((char*)(_tr_c_malloc((size + 1LL))));
    /* pass */
    _tr_c_fread(((void*)(buffer)), 1LL, size, fp);
    /* pass */
    (*(buffer + size) = '\0');
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return ((char*)(buffer));
}

__attribute__((hot)) bool file_exists(char* path) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(path, "rb");
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

__attribute__((hot)) bool write_file(char* path, char* content) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(path, "wb");
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long length = 0LL;
    /* pass */
    char* p = ((char*)(content));
    /* pass */
    while ((((long long)((*(p + length)))) != 0LL)) {
        /* pass */
        length = (length + 1LL);
    }
    /* pass */
    _tr_c_fwrite(((void*)(content)), 1LL, length, fp);
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return true;
}

__attribute__((hot)) bool append_file(char* path, char* content) {
    /* pass */
    /* unsafe block */
    /* pass */
    void* fp = _tr_c_fopen(path, "ab");
    /* pass */
    if ((((unsigned long long)(fp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long length = 0LL;
    /* pass */
    char* p = ((char*)(content));
    /* pass */
    while ((((long long)((*(p + length)))) != 0LL)) {
        /* pass */
        length = (length + 1LL);
    }
    /* pass */
    _tr_c_fwrite(((void*)(content)), 1LL, length, fp);
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return true;
}

