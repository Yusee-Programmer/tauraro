#define _TR_MAIN
#include "tauraro_rt.h"
typedef struct Set Set;
typedef struct std_core_map_MapEntry_bool std_core_map_MapEntry_bool;
typedef struct std_core_map_Map_bool std_core_map_Map_bool;
typedef struct Option Option;
typedef struct Result Result;

static void _tr_init_std_core_map();


struct Set {
    std_core_map_Map_bool* data;
    long long count;
};

struct std_core_map_MapEntry_bool {
    char* key;
    bool val;
    bool used;
};

struct std_core_map_Map_bool {
    char* buckets;
    long long count;
    long long capacity;
};


extern char* _tr_checked_alloc(long long size);
extern bool _tr_str_eq(char* a, char* b);
extern long long _tr_strlen(char* s);
__attribute__((hot)) long long std_core_map__map_hash(char* key, long long cap);
__attribute__((hot,malloc,returns_nonnull)) Set* Set_init(long long cap);
__attribute__((hot)) void Set_add(Set* self, char* key);
__attribute__((hot)) void Set_remove(Set* self, char* key);
__attribute__((hot)) bool Set_contains(Set* self, char* key);
__attribute__((hot)) bool Set_is_empty(Set* self);
__attribute__((hot)) long long Set_len(Set* self);
__attribute__((hot)) std_core_map_Map_bool* std_core_map_Map_bool_init(long long cap);
__attribute__((hot)) std_core_map_MapEntry_bool** std_core_map_Map_bool__bucket(std_core_map_Map_bool* self, long long i);
__attribute__((hot)) void std_core_map_Map_bool_insert(std_core_map_Map_bool* self, char* key, bool val);
__attribute__((hot)) bool std_core_map_Map_bool_get(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) bool std_core_map_Map_bool_contains(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) void std_core_map_Map_bool_remove(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) long long std_core_map_Map_bool_len(std_core_map_Map_bool* self);
__attribute__((hot)) bool std_core_map_Map_bool_is_empty(std_core_map_Map_bool* self);
__attribute__((hot)) void std_core_map_Map_bool__rehash(std_core_map_Map_bool* self);
__attribute__((hot)) void std_core_map_Map_bool_free(std_core_map_Map_bool* self);

__attribute__((hot)) void _tr_init_std_core_map(void) {
}

__attribute__((hot,malloc,returns_nonnull)) Set* Set_init(long long cap) {
     Set* s = ((Set*)_tr_checked_alloc(sizeof(Set)));
    s->data = std_core_map_Map_bool_init(cap);
    s->count = 0;
    return s;
    if (s) { free(s); }
}

__attribute__((hot)) void Set_add(Set* self, char* key) {
    if ((!std_core_map_Map_bool_contains(self->data, key))) {
        std_core_map_Map_bool_insert(self->data, key, true);
        self->count = (self->count + 1);
    }
}

__attribute__((hot)) void Set_remove(Set* self, char* key) {
    if (std_core_map_Map_bool_contains(self->data, key)) {
        std_core_map_Map_bool_remove(self->data, key);
        self->count = (self->count - 1);
    }
}

__attribute__((hot)) bool Set_contains(Set* self, char* key) {
    return std_core_map_Map_bool_contains(self->data, key);
}

__attribute__((hot)) bool Set_is_empty(Set* self) {
    return (self->count == 0);
}

__attribute__((hot)) long long Set_len(Set* self) {
    return self->count;
}

