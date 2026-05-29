/* Benchmark 9: Collatz — total steps for all n in 1..10,000,000 */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    long long total = 0;
    for (long long n = 1; n <= 10000000; n++) {
        long long x = n;
        while (x != 1) {
            if (x % 2 == 0)
                x /= 2;
            else
                x = 3 * x + 1;
            total++;
        }
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);
    printf("%lld\n", total);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
