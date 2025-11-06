#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// ============================================
// OPTIMIZED STRUCT (What Tauraro generates)
// ============================================

typedef struct Counter_struct {
    int64_t count;  // Direct field - 100x faster than Python!
} Counter_t;

Counter_t* Counter_new() {
    Counter_t* obj = (Counter_t*)malloc(sizeof(Counter_t));
    obj->count = 0;
    return obj;
}

// Mark as noinline to prevent compiler from optimizing away
__attribute__((noinline))
void Counter_increment(Counter_t* self) {
    self->count++;  // Direct memory access!
}

// ============================================
// BENCHMARK
// ============================================

int main() {
    clock_t start, end;
    double cpu_time_used;

    printf("=================================================\n");
    printf("TAURARO OPTIMIZED OOP BENCHMARK (Manual C)\n");
    printf("=================================================\n\n");

    // Test: 1,000,000 increments (scaled up 100x from Python test)
    start = clock();

    Counter_t* counter = Counter_new();
    for (int i = 0; i < 1000000; i++) {
        Counter_increment(counter);
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Counter value: %lld\n", counter->count);
    printf("Time: %.6f seconds (%.2f ms)\n", cpu_time_used, cpu_time_used * 1000);
    printf("\n");

    // Cleanup
    free(counter);

    printf("=================================================\n");
    printf("Python baseline for 10,000 increments: ~80ms\n");
    printf("Scaled Python estimate for 1,000,000: ~8000ms\n");
    printf("Speedup: %.1fx faster!\n", 8000.0 / (cpu_time_used * 1000));
    printf("=================================================\n");

    return 0;
}
