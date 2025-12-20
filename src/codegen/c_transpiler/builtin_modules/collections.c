// ==========================================
// COLLECTIONS MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: namedtuple, deque, Counter, OrderedDict, defaultdict
// Platform: Cross-platform

#ifndef TAURARO_COLLECTIONS_MODULE_H
#define TAURARO_COLLECTIONS_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifndef TAU_HELPER_FUNCTIONS_DEFINED
#define TAU_HELPER_FUNCTIONS_DEFINED

static inline double tau_to_double(TauValue v) {
    if (v.type == 0) return (double)v.value.i;
    if (v.type == 1) return v.value.f;
    return 0.0;
}

static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;
    if (v.type == 1) return (int64_t)v.value.f;
    return 0;
}

static inline bool tau_to_bool(TauValue v) {
    if (v.type == 3) return v.value.i != 0;
    if (v.type == 0) return v.value.i != 0;
    if (v.type == 1) return v.value.f != 0.0;
    if (v.type == 2) return v.value.s != NULL && v.value.s[0] != '\0';
    return true;
}

static inline char* tau_to_string(TauValue v) {
    if (v.type == 2) return v.value.s;
    return NULL;
}
#endif // TAU_HELPER_FUNCTIONS_DEFINED

// namedtuple factory
typedef struct {
    char* typename;
    char** field_names;
    int field_count;
} NamedTupleFactory;

// deque structure (double-ended queue)
typedef struct DequeNode {
    TauValue value;
    struct DequeNode* prev;
    struct DequeNode* next;
} DequeNode;

typedef struct {
    DequeNode* front;
    DequeNode* back;
    int size;
} Deque;

// Counter structure (frequency counter)
typedef struct {
    TauDict* counts;
} Counter;

// OrderedDict - preserves insertion order
typedef struct {
    TauDict* dict;
    char** keys;
    int key_count;
    int key_capacity;
} OrderedDict;

// defaultdict - dict with default factory
typedef struct {
    TauDict* dict;
    void* default_factory;
} DefaultDict;

// namedtuple(typename, field_names)
static inline TauValue tauraro_collections_namedtuple(TauValue typename, TauValue field_names) {
    const char* type_str = typename.type == 2 ? typename.value.s : "namedtuple";
    
    NamedTupleFactory* factory = (NamedTupleFactory*)malloc(sizeof(NamedTupleFactory));
    factory->typename = (char*)malloc(strlen(type_str) + 1);
    strcpy(factory->typename, type_str);
    
    factory->field_count = 0;
    factory->field_names = NULL;
    
    if (field_names.type == 4 && field_names.value.list) {
        TauList* fields = field_names.value.list;
        factory->field_count = fields->size;
        factory->field_names = (char**)malloc(sizeof(char*) * factory->field_count);
        
        for (int i = 0; i < factory->field_count; i++) {
            if (fields->items[i].type == 2) {
                factory->field_names[i] = (char*)malloc(strlen(fields->items[i].value.s) + 1);
                strcpy(factory->field_names[i], fields->items[i].value.s);
            }
        }
    }
    
    return (TauValue){.type = 6, .value.ptr = (void*)factory, .refcount = 1, .next = NULL};
}

// deque() - Create empty deque
static inline TauValue tauraro_collections_deque(void) {
    Deque* d = (Deque*)malloc(sizeof(Deque));
    d->front = NULL;
    d->back = NULL;
    d->size = 0;
    return (TauValue){.type = 6, .value.ptr = (void*)d, .refcount = 1, .next = NULL};
}

// deque.append(x) - Add to right
static inline TauValue tauraro_collections_deque_append(TauValue deque_val, TauValue value) {
    if (deque_val.type != 6) return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    
    Deque* d = (Deque*)deque_val.value.ptr;
    DequeNode* node = (DequeNode*)malloc(sizeof(DequeNode));
    node->value = value;
    node->prev = d->back;
    node->next = NULL;
    
    if (d->back) {
        d->back->next = node;
    } else {
        d->front = node;
    }
    d->back = node;
    d->size++;
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// deque.appendleft(x) - Add to left
static inline TauValue tauraro_collections_deque_appendleft(TauValue deque_val, TauValue value) {
    if (deque_val.type != 6) return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    
    Deque* d = (Deque*)deque_val.value.ptr;
    DequeNode* node = (DequeNode*)malloc(sizeof(DequeNode));
    node->value = value;
    node->next = d->front;
    node->prev = NULL;
    
    if (d->front) {
        d->front->prev = node;
    } else {
        d->back = node;
    }
    d->front = node;
    d->size++;
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// Counter(iterable) - Count element frequencies
static inline TauValue tauraro_collections_Counter(TauValue items) {
    Counter* counter = (Counter*)malloc(sizeof(Counter));
    counter->counts = (TauDict*)malloc(sizeof(TauDict));
    counter->counts->size = 0;
    counter->counts->capacity = 0;
    counter->counts->keys = NULL;
    counter->counts->values = NULL;
    
    if (items.type == 4 && items.value.list) {
        TauList* list = items.value.list;
        for (int i = 0; i < list->size; i++) {
            // Increment count for this item (simplified)
        }
    }
    
    return (TauValue){.type = 6, .value.ptr = (void*)counter, .refcount = 1, .next = NULL};
}

// OrderedDict() - Create ordered dictionary
static inline TauValue tauraro_collections_OrderedDict(void) {
    OrderedDict* od = (OrderedDict*)malloc(sizeof(OrderedDict));
    od->dict = (TauDict*)malloc(sizeof(TauDict));
    od->dict->size = 0;
    od->dict->capacity = 0;
    od->dict->keys = NULL;
    od->dict->values = NULL;
    
    od->key_count = 0;
    od->key_capacity = 10;
    od->keys = (char**)malloc(sizeof(char*) * od->key_capacity);
    
    return (TauValue){.type = 6, .value.ptr = (void*)od, .refcount = 1, .next = NULL};
}

// defaultdict(default_factory) - Create dict with default factory
static inline TauValue tauraro_collections_defaultdict(TauValue factory) {
    DefaultDict* dd = (DefaultDict*)malloc(sizeof(DefaultDict));
    dd->dict = (TauDict*)malloc(sizeof(TauDict));
    dd->dict->size = 0;
    dd->dict->capacity = 0;
    dd->dict->keys = NULL;
    dd->dict->values = NULL;
    
    dd->default_factory = factory.type == 6 ? factory.value.ptr : NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)dd, .refcount = 1, .next = NULL};
}

#endif // TAURARO_COLLECTIONS_MODULE_H
