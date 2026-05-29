/* Benchmark 10: Matrix Multiply — naive 400x400 f64 (A * B = C) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    clock_t t0 = clock();

    int n = 400;
    double *a = (double *)malloc(n * n * sizeof(double));
    double *b = (double *)malloc(n * n * sizeof(double));
    double *c = (double *)calloc(n * n, sizeof(double));

    for (int i = 0; i < n * n; i++) {
        a[i] = (double)i * 0.000001;
        b[i] = (double)(n * n - i) * 0.000001;
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            double aik = a[i * n + k];
            for (int j = 0; j < n; j++) {
                c[i * n + j] += aik * b[k * n + j];
            }
        }
    }

    double trace = 0.0;
    for (int i = 0; i < n; i++) trace += c[i * n + i];

    free(a); free(b); free(c);

    clock_t t1 = clock();
    long long ms = (long long)((double)(t1 - t0) / CLOCKS_PER_SEC * 1000.0);
    printf("%.6f\n", trace);
    printf("TIME_MS:%lld\n", ms);
    return 0;
}
