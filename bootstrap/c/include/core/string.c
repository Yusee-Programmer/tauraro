#include "../../tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) StringObj* StringObj_init(TrStr s) {
    /* pass */
    StringObj* obj = ((StringObj*)_tr_obj_alloc(sizeof(StringObj)));
    /* pass */
    long long slen = _tr_strlen(_tr_strz(s));
    /* pass */
    obj->len = slen;
    /* pass */
    obj->capacity = (slen + 8LL);
    /* pass */
    obj->data = _tr_checked_alloc(obj->capacity);
    /* pass */
    if ((slen > 0LL)) {
        /* pass */
        _tr_c_memcpy(obj->data, ((char*)(_tr_strz(s))), slen);
    }
    /* pass */
    /* unsafe block */
    /* pass */
    (*(obj->data + slen) = '\0');
    /* pass */
    return obj;
}

__attribute__((hot)) TrStr StringObj_as_str(StringObj* self) {
    /* pass */
    return _tr_str_wrap(_tr_str_lit(self->data));
}

__attribute__((hot)) void StringObj_append(StringObj* self, TrStr other) {
    /* pass */
    long long slen = _tr_strlen(_tr_strz(other));
    /* pass */
    if ((slen <= 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    if (((self->len + slen) >= self->capacity)) {
        /* pass */
        self->capacity = (((self->len + slen) * 2LL) + 8LL);
        /* pass */
        self->data = _tr_c_realloc(self->data, self->capacity);
    }
    /* pass */
    _tr_c_memcpy((self->data + self->len), ((char*)(_tr_strz(other))), slen);
    /* pass */
    self->len = (self->len + slen);
    /* pass */
    /* unsafe block */
    /* pass */
    (*(self->data + self->len) = '\0');
}

__attribute__((hot)) void StringObj_destroy(StringObj* self) {
    /* pass */
    _tr_c_free(self->data);
    /* pass */
    self->data = ((char*)(0LL));
    /* pass */
    self->len = 0LL;
    /* pass */
    self->capacity = 0LL;
}

__attribute__((malloc,returns_nonnull,hot)) StringBuilder* StringBuilder_init(long long initial_capacity) {
    /* pass */
    long long cap = initial_capacity;
    /* pass */
    if ((cap < 16LL)) {
        /* pass */
        cap = 16LL;
    }
    /* pass */
    StringBuilder* sb = ((StringBuilder*)_tr_obj_alloc(sizeof(StringBuilder)));
    /* pass */
    sb->buf = ((StringObj*)_tr_obj_alloc(sizeof(StringObj)));
    /* pass */
    sb->buf->len = 0LL;
    /* pass */
    sb->buf->capacity = (cap + 1LL);
    /* pass */
    sb->buf->data = _tr_checked_alloc(sb->buf->capacity);
    /* pass */
    /* unsafe block */
    /* pass */
    (*(sb->buf->data + 0LL) = '\0');
    /* pass */
    return sb;
}

__attribute__((hot)) void StringBuilder_append(StringBuilder* self, TrStr s) {
    /* pass */
    StringObj_append(self->buf, s);
}

__attribute__((hot)) void StringBuilder_append_char(StringBuilder* self, long long c) {
    /* pass */
    if (((self->buf->len + 1LL) >= self->buf->capacity)) {
        /* pass */
        self->buf->capacity = ((self->buf->capacity * 2LL) + 8LL);
        /* pass */
        self->buf->data = _tr_c_realloc(self->buf->data, self->buf->capacity);
    }
    /* pass */
    /* unsafe block */
    /* pass */
    (*(self->buf->data + self->buf->len) = ((char)(c)));
    /* pass */
    self->buf->len = (self->buf->len + 1LL);
    /* pass */
    /* unsafe block */
    /* pass */
    (*(self->buf->data + self->buf->len) = '\0');
}

__attribute__((hot)) void StringBuilder_append_int(StringBuilder* self, long long n) {
    /* pass */
    bool neg = false;
    /* pass */
    long long x = n;
    /* pass */
    if ((x == 0LL)) {
        /* pass */
        StringBuilder_append_char(self, 48LL);
        /* pass */
        return;
    }
    /* pass */
    if ((x < 0LL)) {
        /* pass */
        neg = true;
        /* pass */
        x = (0LL - x);
    }
    /* pass */
    long long start = self->buf->len;
    /* pass */
    while ((x > 0LL)) {
        /* pass */
        long long d = (x - ((x / 10LL) * 10LL));
        /* pass */
        StringBuilder_append_char(self, (48LL + d));
        /* pass */
        x = (x / 10LL);
    }
    /* pass */
    if (neg) {
        /* pass */
        StringBuilder_append_char(self, 45LL);
    }
    /* pass */
    long long lo = start;
    /* pass */
    long long hi = (self->buf->len - 1LL);
    /* pass */
    while ((lo < hi)) {
        /* pass */
        /* unsafe block */
        /* pass */
        long long a = (*(self->buf->data + lo));
        /* pass */
        long long b = (*(self->buf->data + hi));
        /* pass */
        (*(self->buf->data + lo) = b);
        /* pass */
        (*(self->buf->data + hi) = a);
        /* pass */
        lo = (lo + 1LL);
        /* pass */
        hi = (hi - 1LL);
    }
}

__attribute__((hot)) void StringBuilder_append_float(StringBuilder* self, double f) {
    /* pass */
    TrStr s = _tr_str_wrap(_tr_float_to_str((double)(f)));
    /* pass */
    StringBuilder_append(self, s);
    _tr_str_release(s);
}

__attribute__((hot)) long long StringBuilder_len(StringBuilder* self) {
    /* pass */
    return self->buf->len;
}

__attribute__((hot)) StringObj* StringBuilder_to_string(StringBuilder* self) {
    /* pass */
    return ({ TrStr _at_t1 = (StringObj_as_str(self->buf)); __auto_type _wr = (StringObj_init(_at_t1)); _tr_str_release(_at_t1); _wr; });
}

__attribute__((hot)) TrStr StringBuilder_to_owned(StringBuilder* self) {
    /* pass */
    long long sz = (self->buf->len + 1LL);
    /* pass */
    char* out = _tr_checked_alloc(sz);
    /* pass */
    _tr_c_memcpy(out, self->buf->data, sz);
    /* pass */
    return _tr_str_wrap((char*)(out));
}

__attribute__((hot)) TrStr StringBuilder_as_str(StringBuilder* self) {
    /* pass */
    return StringObj_as_str(self->buf);
}

__attribute__((hot)) void StringBuilder_clear(StringBuilder* self) {
    /* pass */
    self->buf->len = 0LL;
    /* pass */
    /* unsafe block */
    /* pass */
    (*(self->buf->data + 0LL) = '\0');
}

__attribute__((hot)) void StringBuilder__tr_fn_free(StringBuilder* self) {
    /* pass */
    StringObj_destroy(self->buf);
    /* pass */
    /* unsafe block */
    /* pass */
    _tr_c_free(((char*)(self->buf)));
    /* pass */
    _tr_c_free(((char*)(self)));
}

