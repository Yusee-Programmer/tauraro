/* Benchmark 6: Mandelbrot — 800x800 grid, 1000 max iterations */
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    long long count = 0;
    for (int py = 0; py < 800; py++) {
        double cy = (py / 800.0) * 2.0 - 1.0;
        for (int px = 0; px < 800; px++) {
            double cx = (px / 800.0) * 3.5 - 2.5;
            double x = 0.0, y = 0.0;
            int iter = 0;
            while (iter < 1000 && x*x + y*y < 4.0) {
                double tx = x*x - y*y + cx;
                y = 2.0*x*y + cy;
                x = tx;
                iter++;
            }
            if (iter == 1000) count++;
        }
    }

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);
    printf("%lld\n", count);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
