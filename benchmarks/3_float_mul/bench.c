/* Benchmark 3: Float Multiply — 1B f64 multiplications */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    double x = 1.0;
    for (int i = 0; i < 1000000000; i++) {
        x *= 1.000001;
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);

    printf("%.6f\n", x);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
