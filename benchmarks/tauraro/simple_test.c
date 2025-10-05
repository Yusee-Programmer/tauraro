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
    printf("%s\n", "Hello, Tauraro!");
    return;
}

int main(int argc, char* argv[]) {
    tauraro_main();
    return 0;
}