#include "../../tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) StringObj* StringObj_init(TrStr s) {
#line 30 "std/core/string.tr"
    StringObj* obj = ((StringObj*)_tr_obj_alloc(sizeof(StringObj)));
#line 31 "std/core/string.tr"
    long long slen = _tr_str_lenv((s));
#line 32 "std/core/string.tr"
    obj->len = slen;
#line 33 "std/core/string.tr"
    obj->capacity = (slen + 8LL);
#line 34 "std/core/string.tr"
    obj->data = _tr_checked_alloc(obj->capacity);
#line 35 "std/core/string.tr"
    if ((slen > 0LL)) {
#line 36 "std/core/string.tr"
        _tr_c_memcpy(obj->data, ((char*)(_tr_strz(s))), slen);
    }
#line 37 "std/core/string.tr"
    /* unsafe block */
#line 37 "std/core/string.tr"
    (*(obj->data + slen) = '\0');
#line 38 "std/core/string.tr"
    return obj;
}

__attribute__((hot)) TrStr StringObj_as_str(StringObj* self) {
#line 41 "std/core/string.tr"
    return _tr_str_wrap(_tr_str_lit_len(self->data, self->len));
}

__attribute__((hot)) void StringObj_append(StringObj* self, TrStr other) {
#line 44 "std/core/string.tr"
    long long slen = _tr_str_lenv((other));
#line 45 "std/core/string.tr"
    if ((slen <= 0LL)) {
#line 45 "std/core/string.tr"
        return;
    }
#line 46 "std/core/string.tr"
    if (((self->len + slen) >= self->capacity)) {
#line 47 "std/core/string.tr"
        self->capacity = (((self->len + slen) * 2LL) + 8LL);
#line 48 "std/core/string.tr"
        self->data = _tr_c_realloc(self->data, self->capacity);
    }
#line 49 "std/core/string.tr"
    _tr_c_memcpy((self->data + self->len), ((char*)(_tr_strz(other))), slen);
#line 50 "std/core/string.tr"
    self->len = (self->len + slen);
#line 51 "std/core/string.tr"
    /* unsafe block */
#line 51 "std/core/string.tr"
    (*(self->data + self->len) = '\0');
}

__attribute__((hot)) void StringObj_destroy(StringObj* self) {
#line 54 "std/core/string.tr"
    _tr_c_free(self->data);
#line 55 "std/core/string.tr"
    self->data = ((char*)(0LL));
#line 56 "std/core/string.tr"
    self->len = 0LL;
#line 57 "std/core/string.tr"
    self->capacity = 0LL;
}

__attribute__((malloc,returns_nonnull,hot)) StringBuilder* StringBuilder_init(long long initial_capacity) {
#line 64 "std/core/string.tr"
    long long cap = initial_capacity;
#line 65 "std/core/string.tr"
    if ((cap < 16LL)) {
#line 65 "std/core/string.tr"
        cap = 16LL;
    }
#line 66 "std/core/string.tr"
    StringBuilder* sb = ((StringBuilder*)_tr_obj_alloc(sizeof(StringBuilder)));
#line 67 "std/core/string.tr"
    sb->buf = ((StringObj*)_tr_obj_alloc(sizeof(StringObj)));
#line 68 "std/core/string.tr"
    sb->buf->len = 0LL;
#line 69 "std/core/string.tr"
    sb->buf->capacity = (cap + 1LL);
#line 70 "std/core/string.tr"
    sb->buf->data = _tr_checked_alloc(sb->buf->capacity);
#line 71 "std/core/string.tr"
    /* unsafe block */
#line 71 "std/core/string.tr"
    (*(sb->buf->data + 0LL) = '\0');
#line 72 "std/core/string.tr"
    return sb;
}

__attribute__((hot)) void StringBuilder_append(StringBuilder* self, TrStr s) {
#line 75 "std/core/string.tr"
    StringObj_append(self->buf, s);
}

__attribute__((hot)) void StringBuilder_append_char(StringBuilder* self, long long c) {
#line 78 "std/core/string.tr"
    if (((self->buf->len + 1LL) >= self->buf->capacity)) {
#line 79 "std/core/string.tr"
        self->buf->capacity = ((self->buf->capacity * 2LL) + 8LL);
#line 80 "std/core/string.tr"
        self->buf->data = _tr_c_realloc(self->buf->data, self->buf->capacity);
    }
#line 81 "std/core/string.tr"
    /* unsafe block */
#line 81 "std/core/string.tr"
    (*(self->buf->data + self->buf->len) = ((char)(c)));
#line 82 "std/core/string.tr"
    self->buf->len = (self->buf->len + 1LL);
#line 83 "std/core/string.tr"
    /* unsafe block */
#line 83 "std/core/string.tr"
    (*(self->buf->data + self->buf->len) = '\0');
}

__attribute__((hot)) void StringBuilder_append_int(StringBuilder* self, long long n) {
#line 86 "std/core/string.tr"
    bool neg = false;
#line 87 "std/core/string.tr"
    long long x = n;
#line 88 "std/core/string.tr"
    if ((x == 0LL)) {
#line 89 "std/core/string.tr"
        StringBuilder_append_char(self, 48LL);
#line 90 "std/core/string.tr"
        return;
    }
#line 91 "std/core/string.tr"
    long long start = self->buf->len;
#line 92 "std/core/string.tr"
    if ((x < 0LL)) {
#line 96 "std/core/string.tr"
        neg = true;
#line 97 "std/core/string.tr"
        while ((x < 0LL)) {
#line 98 "std/core/string.tr"
            long long q = _tr_idiv((long long)(x), (long long)(10LL));
#line 99 "std/core/string.tr"
            StringBuilder_append_char(self, (48LL + ((q * 10LL) - x)));
#line 100 "std/core/string.tr"
            x = q;
        }
    } else {
#line 102 "std/core/string.tr"
        while ((x > 0LL)) {
#line 103 "std/core/string.tr"
            long long d = (x - (_tr_idiv((long long)(x), (long long)(10LL)) * 10LL));
#line 104 "std/core/string.tr"
            StringBuilder_append_char(self, (48LL + d));
#line 105 "std/core/string.tr"
            x = _tr_idiv((long long)(x), (long long)(10LL));
        }
    }
#line 106 "std/core/string.tr"
    if (neg) {
#line 106 "std/core/string.tr"
        StringBuilder_append_char(self, 45LL);
    }
#line 107 "std/core/string.tr"
    long long lo = start;
#line 108 "std/core/string.tr"
    long long hi = (self->buf->len - 1LL);
#line 109 "std/core/string.tr"
    while ((lo < hi)) {
#line 110 "std/core/string.tr"
        /* unsafe block */
#line 111 "std/core/string.tr"
        long long a = (*(self->buf->data + lo));
#line 112 "std/core/string.tr"
        long long b = (*(self->buf->data + hi));
#line 113 "std/core/string.tr"
        (*(self->buf->data + lo) = b);
#line 114 "std/core/string.tr"
        (*(self->buf->data + hi) = a);
#line 115 "std/core/string.tr"
        lo = (lo + 1LL);
#line 116 "std/core/string.tr"
        hi = (hi - 1LL);
    }
}

__attribute__((hot)) void StringBuilder_append_float(StringBuilder* self, double f) {
#line 119 "std/core/string.tr"
    TrStr s = _tr_str_wrap(_tr_float_to_str((double)(f)));
#line 120 "std/core/string.tr"
    StringBuilder_append(self, s);
    _tr_str_release(s);
}

__attribute__((hot)) long long StringBuilder_len(StringBuilder* self) {
#line 123 "std/core/string.tr"
    return self->buf->len;
}

__attribute__((hot)) StringObj* StringBuilder_to_string(StringBuilder* self) {
#line 126 "std/core/string.tr"
    return ({ TrStr _at_t1 = (StringObj_as_str(self->buf)); __auto_type _wr = (StringObj_init(_at_t1)); _tr_str_release(_at_t1); _wr; });
}

__attribute__((hot)) TrStr StringBuilder_to_owned(StringBuilder* self) {
#line 129 "std/core/string.tr"
    long long sz = (self->buf->len + 1LL);
#line 130 "std/core/string.tr"
    char* out = _tr_checked_alloc(sz);
#line 131 "std/core/string.tr"
    _tr_c_memcpy(out, self->buf->data, sz);
#line 132 "std/core/string.tr"
    return _tr_str_wrap(_tr_str_wrap_len(out, self->buf->len));
}

__attribute__((hot)) TrStr StringBuilder_as_str(StringBuilder* self) {
#line 135 "std/core/string.tr"
    return StringObj_as_str(self->buf);
}

__attribute__((hot)) void StringBuilder_clear(StringBuilder* self) {
#line 138 "std/core/string.tr"
    self->buf->len = 0LL;
#line 139 "std/core/string.tr"
    /* unsafe block */
#line 139 "std/core/string.tr"
    (*(self->buf->data + 0LL) = '\0');
}

__attribute__((hot)) void StringBuilder__tr_fn_free(StringBuilder* self) {
#line 146 "std/core/string.tr"
    StringObj_destroy(self->buf);
#line 147 "std/core/string.tr"
    /* unsafe block */
#line 148 "std/core/string.tr"
    _tr_c_free(((char*)(self->buf)));
#line 149 "std/core/string.tr"
    _tr_c_free(((char*)(self)));
}

