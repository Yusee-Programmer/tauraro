/* Benchmark 1: Integer Sum — sum 0..999_999_999 (1B additions) */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    long long sum = 0;
    for (long long i = 0; i < 1000000000LL; i++) {
        sum += i;
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);

    printf("%lld\n", sum);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
