#include "tauraro_types.h"

void ByteBuf_grow(ByteBuf* self);

__attribute__((malloc,returns_nonnull,hot)) ByteBuf* ByteBuf_init() {
#line 20 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 21 "src/codegen/native/bytebuf.tr"
    ByteBuf* b = ((ByteBuf*)_tr_obj_alloc(sizeof(ByteBuf)));
#line 22 "src/codegen/native/bytebuf.tr"
    b->cap = 65536LL;
#line 23 "src/codegen/native/bytebuf.tr"
    b->data = ((unsigned char*)_tr_c_calloc((size_t)(b->cap), sizeof(unsigned char)));
#line 24 "src/codegen/native/bytebuf.tr"
    b->len = 0LL;
#line 25 "src/codegen/native/bytebuf.tr"
    return b;
}

__attribute__((hot)) void ByteBuf_grow(ByteBuf* self) {
#line 29 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 30 "src/codegen/native/bytebuf.tr"
    long long ncap = (self->cap * 2LL);
#line 31 "src/codegen/native/bytebuf.tr"
    self->data = ((unsigned char*)(raw_realloc(((char*)(self->data)), ncap)));
#line 32 "src/codegen/native/bytebuf.tr"
    raw_zero(((char*)((self->data + self->cap))), (ncap - self->cap));
#line 33 "src/codegen/native/bytebuf.tr"
    self->cap = ncap;
}

__attribute__((hot)) void ByteBuf_u8(ByteBuf* self, long long v) {
#line 36 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 37 "src/codegen/native/bytebuf.tr"
    if ((self->len >= self->cap)) {
#line 38 "src/codegen/native/bytebuf.tr"
        ByteBuf_grow(self);
    }
#line 39 "src/codegen/native/bytebuf.tr"
    (*(self->data + self->len) = ((unsigned char)((v & 255LL))));
#line 40 "src/codegen/native/bytebuf.tr"
    self->len = (self->len + 1LL);
}

__attribute__((hot)) void ByteBuf_u16(ByteBuf* self, long long v) {
#line 43 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, (v & 255LL));
#line 44 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, ((v >> 8LL) & 255LL));
}

__attribute__((hot)) void ByteBuf_u32(ByteBuf* self, long long v) {
#line 47 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, (v & 255LL));
#line 48 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, ((v >> 8LL) & 255LL));
#line 49 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, ((v >> 16LL) & 255LL));
#line 50 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, ((v >> 24LL) & 255LL));
}

__attribute__((hot)) void ByteBuf_u64(ByteBuf* self, long long v) {
#line 53 "src/codegen/native/bytebuf.tr"
    ByteBuf_u32(self, (v & 4294967295LL));
#line 54 "src/codegen/native/bytebuf.tr"
    ByteBuf_u32(self, ((v >> 32LL) & 4294967295LL));
}

__attribute__((hot)) void ByteBuf_cstr(ByteBuf* self, TrStr s) {
#line 58 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 59 "src/codegen/native/bytebuf.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 60 "src/codegen/native/bytebuf.tr"
    long long i = 0LL;
#line 61 "src/codegen/native/bytebuf.tr"
    while ((((long long)((*(p + i)))) != 0LL)) {
#line 62 "src/codegen/native/bytebuf.tr"
        ByteBuf_u8(self, ((long long)((*(p + i)))));
#line 63 "src/codegen/native/bytebuf.tr"
        i = (i + 1LL);
    }
#line 64 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, 0LL);
}

__attribute__((hot)) void ByteBuf_cstr_len(ByteBuf* self, TrStr s, long long blen) {
#line 75 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 76 "src/codegen/native/bytebuf.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 77 "src/codegen/native/bytebuf.tr"
    long long i = 0LL;
#line 78 "src/codegen/native/bytebuf.tr"
    while ((i < blen)) {
#line 79 "src/codegen/native/bytebuf.tr"
        ByteBuf_u8(self, ((long long)((*(p + i)))));
#line 80 "src/codegen/native/bytebuf.tr"
        i = (i + 1LL);
    }
#line 81 "src/codegen/native/bytebuf.tr"
    ByteBuf_u8(self, 0LL);
}

__attribute__((hot)) void ByteBuf_zeros(ByteBuf* self, long long n) {
#line 84 "src/codegen/native/bytebuf.tr"
    long long i = 0LL;
#line 85 "src/codegen/native/bytebuf.tr"
    while ((i < n)) {
#line 86 "src/codegen/native/bytebuf.tr"
        ByteBuf_u8(self, 0LL);
#line 87 "src/codegen/native/bytebuf.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ByteBuf_append_buf(ByteBuf* self, ByteBuf* o) {
#line 91 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 92 "src/codegen/native/bytebuf.tr"
    long long i = 0LL;
#line 93 "src/codegen/native/bytebuf.tr"
    while ((i < o->len)) {
#line 94 "src/codegen/native/bytebuf.tr"
        ByteBuf_u8(self, ((long long)((*(o->data + i)))));
#line 95 "src/codegen/native/bytebuf.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ByteBuf_patch_u32(ByteBuf* self, long long off, long long v) {
#line 99 "src/codegen/native/bytebuf.tr"
    /* unsafe block */
#line 100 "src/codegen/native/bytebuf.tr"
    (*(self->data + off) = ((unsigned char)((v & 255LL))));
#line 101 "src/codegen/native/bytebuf.tr"
    (*(self->data + (off + 1LL)) = ((unsigned char)(((v >> 8LL) & 255LL))));
#line 102 "src/codegen/native/bytebuf.tr"
    (*(self->data + (off + 2LL)) = ((unsigned char)(((v >> 16LL) & 255LL))));
#line 103 "src/codegen/native/bytebuf.tr"
    (*(self->data + (off + 3LL)) = ((unsigned char)(((v >> 24LL) & 255LL))));
}

__attribute__((hot)) void ByteBuf_align_to(ByteBuf* self, long long align) {
#line 107 "src/codegen/native/bytebuf.tr"
    while ((_tr_imod((long long)(self->len), (long long)(align)) != 0LL)) {
#line 108 "src/codegen/native/bytebuf.tr"
        ByteBuf_u8(self, 0LL);
    }
}

__attribute__((hot)) bool ByteBuf_write_file(ByteBuf* self, TrStr path) {
#line 111 "src/codegen/native/bytebuf.tr"
    char* fp = _tr_c_fopen(_tr_strz(path), _tr_strz(_tr_str_lit_len("wb", 2LL)));
#line 112 "src/codegen/native/bytebuf.tr"
    if ((((long long)(fp)) == 0LL)) {
#line 112 "src/codegen/native/bytebuf.tr"
        return false;
    }
#line 113 "src/codegen/native/bytebuf.tr"
    _tr_c_fwrite(((char*)(self->data)), 1LL, self->len, fp);
#line 114 "src/codegen/native/bytebuf.tr"
    _tr_c_fclose(fp);
#line 115 "src/codegen/native/bytebuf.tr"
    return true;
}

