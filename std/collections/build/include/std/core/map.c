#include "../../../tauraro_types.h"

__attribute__((hot)) long long std_core_map__map_hash(char* key, long long cap);
__attribute__((hot,malloc,returns_nonnull)) std_core_map_Map_bool* std_core_map_Map_bool_init(long long cap);
__attribute__((hot)) std_core_map_MapEntry_bool** std_core_map_Map_bool__bucket(std_core_map_Map_bool* self, long long i);
__attribute__((hot)) void std_core_map_Map_bool_insert(std_core_map_Map_bool* self, char* key, bool val);
__attribute__((hot)) bool std_core_map_Map_bool_get(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) bool std_core_map_Map_bool_contains(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) void std_core_map_Map_bool_remove(std_core_map_Map_bool* self, char* key);
__attribute__((hot)) long long std_core_map_Map_bool_len(std_core_map_Map_bool* self);
__attribute__((hot)) bool std_core_map_Map_bool_is_empty(std_core_map_Map_bool* self);
__attribute__((hot)) void std_core_map_Map_bool__rehash(std_core_map_Map_bool* self);
__attribute__((hot)) void std_core_map_Map_bool_free(std_core_map_Map_bool* self);

__attribute__((hot)) long long std_core_map__map_hash(char* key, long long cap) {
     unsigned long long h = ((unsigned long long)0);
     char* p = ((char*)key);
     long long i = 0;
    while (true) {
         unsigned long long c = ((unsigned long long)(*(char*)(p + i)));
        if ((c == ((unsigned long long)0))) {
            break;
        }
        h = ((h ^ c) * ((unsigned long long)1099511628211));
        i = (i + 1);
    }
    return ((long long)(((long long)h)) % (long long)(cap));
}

__attribute__((hot,malloc,returns_nonnull)) std_core_map_Map_bool* std_core_map_Map_bool_init(long long cap) {
     long long cap2 = cap;
    if ((cap2 < 8)) {
        cap2 = 8;
    }
     long long p = 1;
    while ((p < cap2)) {
        p = (p * 2);
    }
    cap2 = p;
     std_core_map_Map_bool* m = ((std_core_map_Map_bool*)_tr_checked_alloc(sizeof(std_core_map_Map_bool)));
     long long sz = (cap2 * sizeof(std_core_map_MapEntry_bool*));
    m->buckets = _tr_checked_alloc(sz);
    _tr_c_memset(m->buckets, 0, sz);
    m->count = 0;
    m->capacity = cap2;
    return m;
    std_core_map_Map_bool_free(m);
}

__attribute__((hot)) std_core_map_MapEntry_bool** std_core_map_Map_bool__bucket(std_core_map_Map_bool* self, long long i) {
    return (((std_core_map_MapEntry_bool**)self->buckets) + i);
}

__attribute__((hot)) void std_core_map_Map_bool_insert(std_core_map_Map_bool* self, char* key, bool val) {
    if (((self->count * 4) >= (self->capacity * 3))) {
        std_core_map_Map_bool__rehash(self);
    }
     long long h = std_core_map__map_hash(key, self->capacity);
     long long i = h;
    while (true) {
         std_core_map_MapEntry_bool** entry = std_core_map_Map_bool__bucket(self, i);
        if ((!(*(std_core_map_MapEntry_bool**)entry)->used)) {
             std_core_map_MapEntry_bool* e = ((std_core_map_MapEntry_bool*)_tr_checked_alloc(sizeof(std_core_map_MapEntry_bool)));
            e->key = key;
            e->val = val;
            e->used = true;
            (*(std_core_map_MapEntry_bool**)entry = e);
            self->count = (self->count + 1);
            return;
        }
        if (_tr_str_eq((*(std_core_map_MapEntry_bool**)entry)->key, key)) {
             std_core_map_MapEntry_bool* e = (*(std_core_map_MapEntry_bool**)entry);
            e->val = val;
            (*(std_core_map_MapEntry_bool**)entry = e);
            return;
        }
        i = ((long long)((i + 1)) % (long long)(self->capacity));
    }
}

__attribute__((hot)) bool std_core_map_Map_bool_get(std_core_map_Map_bool* self, char* key) {
     long long h = std_core_map__map_hash(key, self->capacity);
     long long i = h;
    while (true) {
         std_core_map_MapEntry_bool** entry = std_core_map_Map_bool__bucket(self, i);
        if ((!(*(std_core_map_MapEntry_bool**)entry)->used)) {
             std_core_map_MapEntry_bool* dummy = ((std_core_map_MapEntry_bool*)_tr_checked_alloc(sizeof(std_core_map_MapEntry_bool)));
            return dummy->val;
            if (dummy) { free(dummy); }
        }
        if (_tr_str_eq((*(std_core_map_MapEntry_bool**)entry)->key, key)) {
            return (*(std_core_map_MapEntry_bool**)entry)->val;
        }
        i = ((long long)((i + 1)) % (long long)(self->capacity));
    }
     std_core_map_MapEntry_bool* dummy = ((std_core_map_MapEntry_bool*)_tr_checked_alloc(sizeof(std_core_map_MapEntry_bool)));
    return dummy->val;
    if (dummy) { free(dummy); }
}

__attribute__((hot)) bool std_core_map_Map_bool_contains(std_core_map_Map_bool* self, char* key) {
     long long h = std_core_map__map_hash(key, self->capacity);
     long long i = h;
    while (true) {
         std_core_map_MapEntry_bool** entry = std_core_map_Map_bool__bucket(self, i);
        if ((!(*(std_core_map_MapEntry_bool**)entry)->used)) {
            return false;
        }
        if (_tr_str_eq((*(std_core_map_MapEntry_bool**)entry)->key, key)) {
            return true;
        }
        i = ((long long)((i + 1)) % (long long)(self->capacity));
    }
    return false;
}

__attribute__((hot)) void std_core_map_Map_bool_remove(std_core_map_Map_bool* self, char* key) {
     long long h = std_core_map__map_hash(key, self->capacity);
     long long i = h;
    while (true) {
         std_core_map_MapEntry_bool** entry = std_core_map_Map_bool__bucket(self, i);
        if ((!(*(std_core_map_MapEntry_bool**)entry)->used)) {
            return;
        }
        if (_tr_str_eq((*(std_core_map_MapEntry_bool**)entry)->key, key)) {
             std_core_map_MapEntry_bool* e = (*(std_core_map_MapEntry_bool**)entry);
            e->used = false;
            (*(std_core_map_MapEntry_bool**)entry = e);
            self->count = (self->count - 1);
            return;
        }
        i = ((long long)((i + 1)) % (long long)(self->capacity));
    }
}

__attribute__((hot)) long long std_core_map_Map_bool_len(std_core_map_Map_bool* self) {
    return self->count;
}

__attribute__((hot)) bool std_core_map_Map_bool_is_empty(std_core_map_Map_bool* self) {
    return (self->count == 0);
}

__attribute__((hot)) void std_core_map_Map_bool__rehash(std_core_map_Map_bool* self) {
     long long new_cap = (self->capacity * 2);
     long long new_sz = (new_cap * sizeof(std_core_map_MapEntry_bool*));
     char* new_bkt = _tr_checked_alloc(new_sz);
    _tr_c_memset(new_bkt, 0, new_sz);
     char* old_bkt = self->buckets;
     long long old_cap = self->capacity;
    self->buckets = new_bkt;
    self->capacity = new_cap;
    self->count = 0;
     long long i = 0;
    while ((i < old_cap)) {
         std_core_map_MapEntry_bool** entry = (((std_core_map_MapEntry_bool**)old_bkt) + i);
        if ((*(std_core_map_MapEntry_bool**)entry)->used) {
            std_core_map_Map_bool_insert(self, (*(std_core_map_MapEntry_bool**)entry)->key, (*(std_core_map_MapEntry_bool**)entry)->val);
        }
        i = (i + 1);
    }
    _tr_c_free(old_bkt);
}

__attribute__((hot)) void std_core_map_Map_bool_free(std_core_map_Map_bool* self) {
    _tr_c_free(self->buckets);
    self->buckets = ((char*)0);
    self->count = 0;
    self->capacity = 0;
}

