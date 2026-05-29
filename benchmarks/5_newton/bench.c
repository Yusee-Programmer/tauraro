/* Benchmark 5: Newton Sqrt — 1B Newton's method iterations */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    double x = 1.5;
    for (int i = 0; i < 1000000000; i++) {
        x = (x + 2.0 / x) * 0.5;
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);

    printf("%.15f\n", x);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
