/* Benchmark 7: Sieve of Eratosthenes — primes up to 50,000,000 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    int n = 50000000;
    unsigned char *sieve = (unsigned char *)calloc(n + 1, 1);
    sieve[0] = sieve[1] = 1;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (!sieve[i]) {
            for (int j = i * i; j <= n; j += i)
                sieve[j] = 1;
        }
    }
    long long count = 0;
    for (int i = 2; i <= n; i++) {
        if (!sieve[i]) count++;
    }
    free(sieve);

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);
    printf("%lld\n", count);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
