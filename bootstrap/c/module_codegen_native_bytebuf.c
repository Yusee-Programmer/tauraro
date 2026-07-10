#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) ByteBuf* ByteBuf_init() {
    /* pass */
    /* unsafe block */
    /* pass */
    ByteBuf* b = ((ByteBuf*)_tr_obj_alloc(sizeof(ByteBuf)));
    /* pass */
    b->cap = 65536LL;
    /* pass */
    b->data = ((unsigned char*)_tr_c_calloc((size_t)(b->cap), sizeof(unsigned char)));
    /* pass */
    b->len = 0LL;
    /* pass */
    return b;
    _tr_obj_release(b, _trdrop_ByteBuf);
}

__attribute__((hot)) void ByteBuf_u8(ByteBuf* self, long long v) {
    /* pass */
    /* unsafe block */
    /* pass */
    if ((self->len < self->cap)) {
        /* pass */
        (*(self->data + self->len) = ((unsigned char)((v & 255LL))));
        /* pass */
        self->len = (self->len + 1LL);
    }
}

__attribute__((hot)) void ByteBuf_u16(ByteBuf* self, long long v) {
    /* pass */
    ByteBuf_u8(self, (v & 255LL));
    /* pass */
    ByteBuf_u8(self, ((v >> 8LL) & 255LL));
}

__attribute__((hot)) void ByteBuf_u32(ByteBuf* self, long long v) {
    /* pass */
    ByteBuf_u8(self, (v & 255LL));
    /* pass */
    ByteBuf_u8(self, ((v >> 8LL) & 255LL));
    /* pass */
    ByteBuf_u8(self, ((v >> 16LL) & 255LL));
    /* pass */
    ByteBuf_u8(self, ((v >> 24LL) & 255LL));
}

__attribute__((hot)) void ByteBuf_u64(ByteBuf* self, long long v) {
    /* pass */
    ByteBuf_u32(self, (v & 4294967295LL));
    /* pass */
    ByteBuf_u32(self, ((v >> 32LL) & 4294967295LL));
}

__attribute__((hot)) void ByteBuf_cstr(ByteBuf* self, TrStr s) {
    /* pass */
    /* unsafe block */
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((((long long)((*(p + i)))) != 0LL)) {
        /* pass */
        ByteBuf_u8(self, ((long long)((*(p + i)))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    ByteBuf_u8(self, 0LL);
}

__attribute__((hot)) void ByteBuf_zeros(ByteBuf* self, long long n) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        ByteBuf_u8(self, 0LL);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ByteBuf_append_buf(ByteBuf* self, ByteBuf* o) {
    /* pass */
    /* unsafe block */
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < o->len)) {
        /* pass */
        ByteBuf_u8(self, ((long long)((*(o->data + i)))));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ByteBuf_patch_u32(ByteBuf* self, long long off, long long v) {
    /* pass */
    /* unsafe block */
    /* pass */
    (*(self->data + off) = ((unsigned char)((v & 255LL))));
    /* pass */
    (*(self->data + (off + 1LL)) = ((unsigned char)(((v >> 8LL) & 255LL))));
    /* pass */
    (*(self->data + (off + 2LL)) = ((unsigned char)(((v >> 16LL) & 255LL))));
    /* pass */
    (*(self->data + (off + 3LL)) = ((unsigned char)(((v >> 24LL) & 255LL))));
}

__attribute__((hot)) void ByteBuf_align_to(ByteBuf* self, long long align) {
    /* pass */
    while (((self->len % align) != 0LL)) {
        /* pass */
        ByteBuf_u8(self, 0LL);
    }
}

__attribute__((hot)) bool ByteBuf_write_file(ByteBuf* self, TrStr path) {
    /* pass */
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit("wb")));
    /* pass */
    if ((((long long)(fp)) == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_c_fwrite(((char*)(self->data)), 1LL, self->len, fp);
    /* pass */
    _tr_c_fclose(fp);
    /* pass */
    return true;
}

