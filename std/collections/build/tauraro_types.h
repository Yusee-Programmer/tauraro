#pragma once
#include "tauraro_rt.h"

typedef struct Set Set;
typedef struct std_core_map_MapEntry_bool std_core_map_MapEntry_bool;
typedef struct std_core_map_Map_bool std_core_map_Map_bool;
typedef struct Option Option;
typedef struct Result Result;


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

/* --- Function Prototypes --- */
char* _tr_checked_alloc(long long size);
bool _tr_str_eq(char* a, char* b);
long long _tr_strlen(char* s);
long long std_core_map__map_hash(char* key, long long cap);
void _tr_init_std_core_map(void);
/* --- Method Prototypes --- */
Set* Set_init(long long cap);
void Set_add(Set* self, char* key);
void Set_remove(Set* self, char* key);
bool Set_contains(Set* self, char* key);
bool Set_is_empty(Set* self);
long long Set_len(Set* self);
std_core_map_Map_bool* std_core_map_Map_bool_init(long long cap);
std_core_map_MapEntry_bool** std_core_map_Map_bool__bucket(std_core_map_Map_bool* self, long long i);
void std_core_map_Map_bool_insert(std_core_map_Map_bool* self, char* key, bool val);
bool std_core_map_Map_bool_get(std_core_map_Map_bool* self, char* key);
bool std_core_map_Map_bool_contains(std_core_map_Map_bool* self, char* key);
void std_core_map_Map_bool_remove(std_core_map_Map_bool* self, char* key);
long long std_core_map_Map_bool_len(std_core_map_Map_bool* self);
bool std_core_map_Map_bool_is_empty(std_core_map_Map_bool* self);
void std_core_map_Map_bool__rehash(std_core_map_Map_bool* self);
void std_core_map_Map_bool_free(std_core_map_Map_bool* self);

/* --- Spawn Wrappers --- */
