#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Type definitions
typedef struct {
    int type_tag;
    union {
        int64_t int_val;
        double float_val;
        char* string_val;
        bool bool_val;
        void* ptr_val;
    } data;
} TauraroValue;

void print(void* args);

void tauraro_main(void) {
    int32_t e;
    int32_t tmp_0;
    int32_t tmp_1;
    TauraroValue b;
    int32_t tmp_3;
    TauraroValue a;
    int32_t c;
    int32_t d;
    int32_t tmp_2;
    int32_t f;
    int64_t g;

    printf("%s\n", "Tauraro Performance Benchmark");
    a = 100;
    b = 50;
    tmp_0 = a + b;
    c = tmp_0;
    tmp_1 = a - b;
    d = tmp_1;
    tmp_2 = a * b;
    e = tmp_2;
    tmp_3 = a / b;
    f = tmp_3;
    tmp_4 = a % b;
    g = tmp_4;
    printf("%s\n", "Addition:");
    printf("%s\n", "Subtraction:");
    printf("%s\n", "Multiplication:");
    printf("%s\n", "Division:");
    printf("%s\n", "Modulo:");
    printf("%s\n", "Benchmark completed");
    return;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}
