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
int64_t add_numbers(int64_t x, int64_t y);

int32_t tauraro_main(void) {
    int64_t a;
    int64_t b;
    int64_t result;
    bool x;
    bool y;
    int64_t counter;
    int64_t i;
    bool cond;
    int64_t tmp_val;

    printf("%s\n", "Starting comprehensive Tauraro test...");
    printf("%s\n", "Testing arithmetic operations...");
    a = 10;
    b = 3;
    tmp_val = a + b;
    printf("%s %lld\n", "Addition:", tmp_val);
    tmp_val = a - b;
    printf("%s %lld\n", "Subtraction:", tmp_val);
    tmp_val = a * b;
    printf("%s %lld\n", "Multiplication:", tmp_val);
    tmp_val = a / b;
    printf("%s %lld\n", "Division:", tmp_val);
    tmp_val = a % b;
    printf("%s %lld\n", "Modulo:", tmp_val);
    
    printf("%s\n", "Testing comparison operations...");
    printf("%s %s\n", "Equal:", (a == b) ? "true" : "false");
    printf("%s %s\n", "Not equal:", (a != b) ? "true" : "false");
    printf("%s %s\n", "Less than:", (a < b) ? "true" : "false");
    printf("%s %s\n", "Greater than:", (a > b) ? "true" : "false");
    printf("%s %s\n", "Less or equal:", (a <= b) ? "true" : "false");
    printf("%s %s\n", "Greater or equal:", (a >= b) ? "true" : "false");
    
    printf("%s\n", "Testing logical operations...");
    x = true;
    y = false;
    printf("%s %s\n", "And:", (x && y) ? "true" : "false");
    printf("%s %s\n", "Or:", (x || y) ? "true" : "false");
    printf("%s %s\n", "Not:", (!x) ? "true" : "false");
    
    printf("%s\n", "Testing control flow...");
    printf("%s\n", "For loop iterations:");
    for (i = 0; i < 5; i++) {
        printf("%s %lld\n", "For loop iteration:", i);
    }
    
    printf("%s\n", "While loop iterations:");
    counter = 0;
    while (counter < 3) {
        printf("%s %lld\n", "While loop iteration:", counter);
        counter = counter + 1;
    }
    
    printf("%s\n", "Conditional statements:");
    if (a > b) {
        printf("%s\n", "a is greater than b");
    } else if (a == b) {
        printf("%s\n", "a is equal to b");
    } else {
        printf("%s\n", "a is less than b");
    }
    
    printf("%s\n", "Testing function calls...");
    result = add_numbers(5, 7);
    printf("%s %lld\n", "Function call result:", result);
    
    printf("%s\n", "Comprehensive Tauraro test completed!");
    return 0;
}

int64_t add_numbers(int64_t x, int64_t y) {
    return x + y;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}