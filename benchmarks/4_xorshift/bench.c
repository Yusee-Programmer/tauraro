/* Benchmark 4: XOR Shift PRNG — 1B xorshift64 steps */
#include <stdio.h>
#include <stdint.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    uint64_t s = 0x123456789ABCDEF0ULL;
    for (int i = 0; i < 1000000000; i++) {
        s ^= s << 13;
        s ^= s >> 7;
        s ^= s << 17;
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);

    printf("%llu\n", (unsigned long long)s);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
