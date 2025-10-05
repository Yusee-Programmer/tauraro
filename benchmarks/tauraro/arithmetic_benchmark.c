#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

int32_t tauraro_main(void) {
    int32_t a;
    int32_t b;
    int32_t i;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    
    a = 100000;
    b = 0;
    
    for (i = 0; i < a; i++) {
        b = b + i;
        b = b * 2;
        b = b - i;
        b = b / 2;  // Integer division
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("%s\n", "Tauraro arithmetic benchmark completed");
    printf("%s %d\n", "Final result:", b);
    printf("%s %d\n", "Iterations completed:", a);
    printf("%s %f %s\n", "Execution time:", cpu_time_used, "seconds");
    return 0;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}