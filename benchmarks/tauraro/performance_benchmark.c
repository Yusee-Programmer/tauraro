#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void print(void* args);

int32_t tauraro_main(void) {
    int64_t a;
    int64_t b;
    int64_t i;
    clock_t start, end;
    double cpu_time_used;

    printf("%s\n", "Starting Tauraro performance benchmark...");
    
    start = clock();
    
    a = 1000000;
    b = 0;
    
    for (i = 0; i < a; i++) {
        b = b + i;
        b = b * 2;
        b = b - i;
        b = b / 2;
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("%s %lld\n", "Tauraro arithmetic benchmark result:", b);
    printf("%s %lld\n", "Iterations completed:", a);
    printf("%s %f %s\n", "Execution time:", cpu_time_used, "seconds");
    printf("%s\n", "Tauraro performance benchmark completed!");
    return 0;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}