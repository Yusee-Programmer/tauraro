/* Benchmark 2: Fibonacci — 1B iterative steps */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    long long a = 0, b = 1;
    for (int i = 0; i < 1000000000; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);

    printf("%lld\n", b);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
