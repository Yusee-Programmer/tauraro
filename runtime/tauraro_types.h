#pragma once
#include "tauraro_rt.h"



__attribute__((hot)) void sizeof_demo();
__attribute__((hot)) void bit_demo();
__attribute__((hot)) void pointer_demo();
__attribute__((hot)) void unsafe_demo();
__attribute__((hot)) void asm_demo();
__attribute__((hot)) void gpu_demo();


/* === Module-prefixed typedef aliases (auto-generated) === */
/* Maps module-qualified C names to short-name types in tauraro_types.h */

typedef StringObj core_string_StringObj;
typedef StringBuilder core_string_StringBuilder;

/* Primitive vec/map types for include/core/*.c */
struct core_vec_Vec_str { char** data; long long len; long long capacity; };
typedef struct core_vec_Vec_str core_vec_Vec_str;
__attribute__((hot)) char** core_alloc_alloc_str(long long count);
__attribute__((hot)) char** core_alloc_resize_str(char** ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_str(char** ptr);
struct core_vec_Vec_i64 { long long* data; long long len; long long capacity; };
typedef struct core_vec_Vec_i64 core_vec_Vec_i64;
__attribute__((hot)) long long* core_alloc_alloc_i64(long long count);
__attribute__((hot)) long long* core_alloc_resize_i64(long long* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_i64(long long* ptr);
struct core_map_MapNode_str_bool { char* key; bool value; struct core_map_MapNode_str_bool* next; };
typedef struct core_map_MapNode_str_bool core_map_MapNode_str_bool;
struct core_map_Map_str_bool { core_map_MapNode_str_bool** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_bool core_map_Map_str_bool;
__attribute__((hot)) core_map_MapNode_str_bool** core_alloc_alloc_core_map_MapNode_str_bool(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_bool(core_map_MapNode_str_bool** ptr);
struct core_map_MapNode_str_str { char* key; char* value; struct core_map_MapNode_str_str* next; };
typedef struct core_map_MapNode_str_str core_map_MapNode_str_str;
struct core_map_Map_str_str { core_map_MapNode_str_str** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_str core_map_Map_str_str;
__attribute__((hot)) core_map_MapNode_str_str** core_alloc_alloc_core_map_MapNode_str_str(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_str(core_map_MapNode_str_str** ptr);
struct core_map_MapNode_str_i64 { char* key; long long value; struct core_map_MapNode_str_i64* next; };
typedef struct core_map_MapNode_str_i64 core_map_MapNode_str_i64;
struct core_map_Map_str_i64 { core_map_MapNode_str_i64** buckets; long long capacity; long long len; };
typedef struct core_map_Map_str_i64 core_map_Map_str_i64;
__attribute__((hot)) core_map_MapNode_str_i64** core_alloc_alloc_core_map_MapNode_str_i64(long long count);
__attribute__((hot)) void core_alloc_dealloc_core_map_MapNode_str_i64(core_map_MapNode_str_i64** ptr);
__attribute__((hot)) char* core_alloc_alloc_char(long long count);
__attribute__((hot)) void core_alloc_copy_char(char* dst, char* src, long long count);
__attribute__((hot)) char* core_alloc_resize_char(char* ptr, long long new_count);
__attribute__((hot)) void core_alloc_dealloc_char(char* ptr);
