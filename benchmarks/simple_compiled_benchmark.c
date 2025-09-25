#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <ucontext.h>
#include <dlfcn.h>

// Type definitions
typedef struct {
    int type_tag;
    union {
        int64_t int_val;
        double float_val;
        char* string_val;
        bool bool_val;
        void* ptr_val;
    } data;
} TauraroValue;

typedef struct {
    int state;
    void* locals;
    ucontext_t context;
} AsyncContext;

// Global variables
int64_t a = 10;
int64_t b = 5;
TauraroValue* result1 = tmp_1;
TauraroValue* result2 = tmp_2;
TauraroValue* result3 = tmp_3;
TauraroValue* result4 = tmp_4;


// Async runtime support
static AsyncContext* create_async_context() {
    AsyncContext* ctx = malloc(sizeof(AsyncContext));
    ctx->state = 0;
    ctx->locals = NULL;
    return ctx;
}

static void destroy_async_context(AsyncContext* ctx) {
    if (ctx) {
        free(ctx->locals);
        free(ctx);
    }
}

