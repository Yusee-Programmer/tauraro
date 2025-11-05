// Tauraro High-Performance Runtime Optimizations
// Memory pools, hash tables, and optimized data structures

#ifndef TAURARO_RUNTIME_OPTIMIZED_H
#define TAURARO_RUNTIME_OPTIMIZED_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// MEMORY POOL ALLOCATOR
// ============================================================================

// Memory pool configuration
#define TAURARO_POOL_BLOCK_SIZE 64
#define TAURARO_POOL_BLOCKS_PER_CHUNK 1024
#define TAURARO_MAX_POOLS 8

// Pool node for free list
typedef struct tauraro_pool_node {
    struct tauraro_pool_node* next;
} tauraro_pool_node_t;

// Memory pool for fixed-size allocations
typedef struct {
    void* memory;                    // Large allocated memory chunk
    tauraro_pool_node_t* free_list;  // Linked list of free blocks
    size_t block_size;               // Size of each block
    size_t total_blocks;             // Total blocks in this pool
    size_t used_blocks;              // Currently used blocks
    struct tauraro_pool_chunk* chunks; // Linked list of chunks
} tauraro_pool_t;

// Chunk in a pool (for growing pools)
typedef struct tauraro_pool_chunk {
    void* memory;
    struct tauraro_pool_chunk* next;
} tauraro_pool_chunk_t;

// Global memory pools for different allocation sizes
static tauraro_pool_t g_memory_pools[TAURARO_MAX_POOLS];
static bool g_pools_initialized = false;

// Pool size categories (powers of 2 for optimal fitting)
static const size_t g_pool_sizes[] = {
    16, 32, 64, 128, 256, 512, 1024, 2048
};

// Initialize memory pools
static void tauraro_init_memory_pools(void) {
    if (g_pools_initialized) return;

    for (int i = 0; i < TAURARO_MAX_POOLS; i++) {
        size_t block_size = g_pool_sizes[i];
        size_t chunk_size = block_size * TAURARO_POOL_BLOCKS_PER_CHUNK;

        g_memory_pools[i].memory = malloc(chunk_size);
        g_memory_pools[i].block_size = block_size;
        g_memory_pools[i].total_blocks = TAURARO_POOL_BLOCKS_PER_CHUNK;
        g_memory_pools[i].used_blocks = 0;
        g_memory_pools[i].chunks = NULL;

        // Initialize free list
        g_memory_pools[i].free_list = NULL;
        char* ptr = (char*)g_memory_pools[i].memory;
        for (size_t j = 0; j < TAURARO_POOL_BLOCKS_PER_CHUNK; j++) {
            tauraro_pool_node_t* node = (tauraro_pool_node_t*)(ptr + j * block_size);
            node->next = g_memory_pools[i].free_list;
            g_memory_pools[i].free_list = node;
        }
    }

    g_pools_initialized = true;
}

// Find appropriate pool for allocation size
static inline int tauraro_find_pool(size_t size) {
    for (int i = 0; i < TAURARO_MAX_POOLS; i++) {
        if (size <= g_pool_sizes[i]) {
            return i;
        }
    }
    return -1; // Too large for pools
}

// Allocate from memory pool
static inline void* tauraro_pool_alloc(size_t size) {
    if (!g_pools_initialized) {
        tauraro_init_memory_pools();
    }

    int pool_idx = tauraro_find_pool(size);
    if (pool_idx < 0) {
        // Too large for pools, use regular malloc
        return malloc(size);
    }

    tauraro_pool_t* pool = &g_memory_pools[pool_idx];

    // If no free blocks, allocate new chunk
    if (pool->free_list == NULL) {
        size_t chunk_size = pool->block_size * TAURARO_POOL_BLOCKS_PER_CHUNK;
        tauraro_pool_chunk_t* chunk = (tauraro_pool_chunk_t*)malloc(sizeof(tauraro_pool_chunk_t));
        chunk->memory = malloc(chunk_size);
        chunk->next = pool->chunks;
        pool->chunks = chunk;

        // Add new blocks to free list
        char* ptr = (char*)chunk->memory;
        for (size_t i = 0; i < TAURARO_POOL_BLOCKS_PER_CHUNK; i++) {
            tauraro_pool_node_t* node = (tauraro_pool_node_t*)(ptr + i * pool->block_size);
            node->next = pool->free_list;
            pool->free_list = node;
        }
        pool->total_blocks += TAURARO_POOL_BLOCKS_PER_CHUNK;
    }

    // Pop from free list
    tauraro_pool_node_t* node = pool->free_list;
    pool->free_list = node->next;
    pool->used_blocks++;

    return node;
}

// Free back to memory pool
static inline void tauraro_pool_free(void* ptr, size_t size) {
    if (ptr == NULL) return;

    int pool_idx = tauraro_find_pool(size);
    if (pool_idx < 0) {
        // Was allocated with malloc
        free(ptr);
        return;
    }

    tauraro_pool_t* pool = &g_memory_pools[pool_idx];

    // Add back to free list
    tauraro_pool_node_t* node = (tauraro_pool_node_t*)ptr;
    node->next = pool->free_list;
    pool->free_list = node;
    pool->used_blocks--;
}

// ============================================================================
// HASH TABLE FOR DICTIONARIES
// ============================================================================

#define TAURARO_HASH_INITIAL_SIZE 16
#define TAURARO_HASH_LOAD_FACTOR 0.75

// Hash table entry
typedef struct tauraro_hash_entry {
    char* key;
    void* value;
    uint32_t hash;
    struct tauraro_hash_entry* next; // Chaining for collisions
} tauraro_hash_entry_t;

// Hash table structure
typedef struct {
    tauraro_hash_entry_t** buckets;
    size_t size;        // Number of buckets
    size_t count;       // Number of entries
    size_t threshold;   // Resize threshold
} tauraro_hash_table_t;

// FNV-1a hash function (fast and good distribution)
static inline uint32_t tauraro_hash_string(const char* str) {
    uint32_t hash = 2166136261u;
    while (*str) {
        hash ^= (uint8_t)(*str++);
        hash *= 16777619u;
    }
    return hash;
}

// Create hash table
static tauraro_hash_table_t* tauraro_hash_create(void) {
    tauraro_hash_table_t* table = (tauraro_hash_table_t*)tauraro_pool_alloc(sizeof(tauraro_hash_table_t));
    table->size = TAURARO_HASH_INITIAL_SIZE;
    table->count = 0;
    table->threshold = (size_t)(table->size * TAURARO_HASH_LOAD_FACTOR);
    table->buckets = (tauraro_hash_entry_t**)calloc(table->size, sizeof(tauraro_hash_entry_t*));
    return table;
}

// Resize hash table
static void tauraro_hash_resize(tauraro_hash_table_t* table) {
    size_t old_size = table->size;
    tauraro_hash_entry_t** old_buckets = table->buckets;

    // Double size
    table->size *= 2;
    table->threshold = (size_t)(table->size * TAURARO_HASH_LOAD_FACTOR);
    table->buckets = (tauraro_hash_entry_t**)calloc(table->size, sizeof(tauraro_hash_entry_t*));

    // Rehash all entries
    for (size_t i = 0; i < old_size; i++) {
        tauraro_hash_entry_t* entry = old_buckets[i];
        while (entry) {
            tauraro_hash_entry_t* next = entry->next;

            size_t bucket = entry->hash % table->size;
            entry->next = table->buckets[bucket];
            table->buckets[bucket] = entry;

            entry = next;
        }
    }

    free(old_buckets);
}

// Insert or update hash table entry
static void tauraro_hash_set(tauraro_hash_table_t* table, const char* key, void* value) {
    uint32_t hash = tauraro_hash_string(key);
    size_t bucket = hash % table->size;

    // Check if key exists
    tauraro_hash_entry_t* entry = table->buckets[bucket];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            // Update existing
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    // Insert new entry
    tauraro_hash_entry_t* new_entry = (tauraro_hash_entry_t*)tauraro_pool_alloc(sizeof(tauraro_hash_entry_t));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->hash = hash;
    new_entry->next = table->buckets[bucket];
    table->buckets[bucket] = new_entry;
    table->count++;

    // Check if resize needed
    if (table->count >= table->threshold) {
        tauraro_hash_resize(table);
    }
}

// Get value from hash table
static inline void* tauraro_hash_get(tauraro_hash_table_t* table, const char* key) {
    uint32_t hash = tauraro_hash_string(key);
    size_t bucket = hash % table->size;

    tauraro_hash_entry_t* entry = table->buckets[bucket];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL; // Not found
}

// Check if key exists
static inline bool tauraro_hash_contains(tauraro_hash_table_t* table, const char* key) {
    return tauraro_hash_get(table, key) != NULL;
}

// Delete entry from hash table
static bool tauraro_hash_delete(tauraro_hash_table_t* table, const char* key) {
    uint32_t hash = tauraro_hash_string(key);
    size_t bucket = hash % table->size;

    tauraro_hash_entry_t* entry = table->buckets[bucket];
    tauraro_hash_entry_t* prev = NULL;

    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                table->buckets[bucket] = entry->next;
            }

            free(entry->key);
            tauraro_pool_free(entry, sizeof(tauraro_hash_entry_t));
            table->count--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }

    return false; // Not found
}

// Free hash table
static void tauraro_hash_free(tauraro_hash_table_t* table) {
    for (size_t i = 0; i < table->size; i++) {
        tauraro_hash_entry_t* entry = table->buckets[i];
        while (entry) {
            tauraro_hash_entry_t* next = entry->next;
            free(entry->key);
            tauraro_pool_free(entry, sizeof(tauraro_hash_entry_t));
            entry = next;
        }
    }
    free(table->buckets);
    tauraro_pool_free(table, sizeof(tauraro_hash_table_t));
}

// ============================================================================
// STRING INTERNING (Cache common strings)
// ============================================================================

#define TAURARO_STRING_CACHE_SIZE 1024

static tauraro_hash_table_t* g_string_cache = NULL;

// Initialize string cache
static void tauraro_init_string_cache(void) {
    if (g_string_cache == NULL) {
        g_string_cache = tauraro_hash_create();
    }
}

// Get interned string (or add to cache)
static const char* tauraro_intern_string(const char* str) {
    tauraro_init_string_cache();

    const char* cached = (const char*)tauraro_hash_get(g_string_cache, str);
    if (cached) {
        return cached; // Return cached version
    }

    // Add to cache
    char* new_str = strdup(str);
    tauraro_hash_set(g_string_cache, str, new_str);
    return new_str;
}

// ============================================================================
// VALUE POOL (Pre-allocated value objects)
// ============================================================================

#define TAURARO_VALUE_POOL_SIZE 256

typedef struct tauraro_value tauraro_value_t;

static tauraro_value_t* g_value_pool = NULL;
static tauraro_pool_node_t* g_value_free_list = NULL;
static size_t g_value_pool_size = 0;

// Initialize value pool
static void tauraro_init_value_pool(void) {
    if (g_value_pool != NULL) return;

    g_value_pool_size = TAURARO_VALUE_POOL_SIZE;
    g_value_pool = (tauraro_value_t*)malloc(sizeof(tauraro_value_t) * g_value_pool_size);

    // Link all values into free list
    g_value_free_list = NULL;
    for (size_t i = 0; i < g_value_pool_size; i++) {
        tauraro_pool_node_t* node = (tauraro_pool_node_t*)&g_value_pool[i];
        node->next = g_value_free_list;
        g_value_free_list = node;
    }
}

// Allocate value from pool
static inline tauraro_value_t* tauraro_value_alloc(void) {
    if (g_value_free_list == NULL) {
        // Pool exhausted, use regular malloc
        return (tauraro_value_t*)tauraro_pool_alloc(sizeof(tauraro_value_t));
    }

    // Pop from free list
    tauraro_pool_node_t* node = g_value_free_list;
    g_value_free_list = node->next;

    return (tauraro_value_t*)node;
}

// Free value back to pool
static inline void tauraro_value_free(tauraro_value_t* value) {
    // Check if from pool
    if (value >= g_value_pool && value < g_value_pool + g_value_pool_size) {
        // Return to pool
        tauraro_pool_node_t* node = (tauraro_pool_node_t*)value;
        node->next = g_value_free_list;
        g_value_free_list = node;
    } else {
        // Was malloc'd
        tauraro_pool_free(value, sizeof(tauraro_value_t));
    }
}

// ============================================================================
// INLINE OPERATIONS (Avoid function call overhead)
// ============================================================================

// Fast integer addition (inline)
static inline int64_t tauraro_add_int_fast(int64_t a, int64_t b) {
    return a + b;
}

// Fast integer subtraction (inline)
static inline int64_t tauraro_sub_int_fast(int64_t a, int64_t b) {
    return a - b;
}

// Fast integer multiplication (inline)
static inline int64_t tauraro_mul_int_fast(int64_t a, int64_t b) {
    return a * b;
}

// Fast integer division (inline, with zero check)
static inline int64_t tauraro_div_int_fast(int64_t a, int64_t b) {
    return (b != 0) ? (a / b) : 0;
}

// Fast float addition (inline)
static inline double tauraro_add_float_fast(double a, double b) {
    return a + b;
}

// Fast float subtraction (inline)
static inline double tauraro_sub_float_fast(double a, double b) {
    return a - b;
}

// Fast float multiplication (inline)
static inline double tauraro_mul_float_fast(double a, double b) {
    return a * b;
}

// Fast float division (inline, with zero check)
static inline double tauraro_div_float_fast(double a, double b) {
    return (b != 0.0) ? (a / b) : 0.0;
}

// ============================================================================
// STATISTICS AND DIAGNOSTICS
// ============================================================================

typedef struct {
    size_t total_allocations;
    size_t total_frees;
    size_t pool_hits;
    size_t pool_misses;
    size_t current_memory_usage;
    size_t peak_memory_usage;
    size_t hash_lookups;
    size_t hash_collisions;
} tauraro_memory_stats_t;

static tauraro_memory_stats_t g_memory_stats = {0};

// Print memory statistics
static void tauraro_print_memory_stats(void) {
    printf("\n=== Tauraro Memory Statistics ===\n");
    printf("Total allocations: %zu\n", g_memory_stats.total_allocations);
    printf("Total frees: %zu\n", g_memory_stats.total_frees);
    printf("Pool hits: %zu\n", g_memory_stats.pool_hits);
    printf("Pool misses: %zu\n", g_memory_stats.pool_misses);
    printf("Pool hit rate: %.2f%%\n",
           100.0 * g_memory_stats.pool_hits /
           (g_memory_stats.pool_hits + g_memory_stats.pool_misses + 1));
    printf("Current memory usage: %zu bytes\n", g_memory_stats.current_memory_usage);
    printf("Peak memory usage: %zu bytes\n", g_memory_stats.peak_memory_usage);
    printf("Hash lookups: %zu\n", g_memory_stats.hash_lookups);
    printf("Hash collisions: %zu\n", g_memory_stats.hash_collisions);
    printf("================================\n\n");
}

// Cleanup all memory pools and caches
static void tauraro_cleanup_runtime(void) {
    // Free string cache
    if (g_string_cache) {
        tauraro_hash_free(g_string_cache);
        g_string_cache = NULL;
    }

    // Free value pool
    if (g_value_pool) {
        free(g_value_pool);
        g_value_pool = NULL;
    }

    // Free memory pools
    for (int i = 0; i < TAURARO_MAX_POOLS; i++) {
        free(g_memory_pools[i].memory);

        tauraro_pool_chunk_t* chunk = g_memory_pools[i].chunks;
        while (chunk) {
            tauraro_pool_chunk_t* next = chunk->next;
            free(chunk->memory);
            free(chunk);
            chunk = next;
        }
    }

    g_pools_initialized = false;
}

#endif // TAURARO_RUNTIME_OPTIMIZED_H
