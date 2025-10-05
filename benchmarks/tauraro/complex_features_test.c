#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void print(void* args);

int32_t tauraro_main(void) {
    int64_t x;
    int64_t y;
    int64_t a;
    int64_t b;
    int64_t c;
    int64_t counter;
    int64_t i;
    int64_t tmp_val;
    bool tmp_bool;

    printf("%s\n", "Testing complex Tauraro features...");
    
    // Test arithmetic operations
    printf("%s\n", "Testing arithmetic operations:");
    x = 100;
    y = 7;
    tmp_val = x + y;
    printf("%s %lld\n", "100 + 7 =", tmp_val);
    tmp_val = x - y;
    printf("%s %lld\n", "100 - 7 =", tmp_val);
    tmp_val = x * y;
    printf("%s %lld\n", "100 * 7 =", tmp_val);
    tmp_val = x / y;
    printf("%s %lld\n", "100 / 7 =", tmp_val);
    tmp_val = x % y;
    printf("%s %lld\n", "100 % 7 =", tmp_val);
    
    // Test comparison operations
    printf("%s\n", "Testing comparison operations:");
    printf("%s %s\n", "100 == 7:", (x == y) ? "true" : "false");
    printf("%s %s\n", "100 != 7:", (x != y) ? "true" : "false");
    printf("%s %s\n", "100 > 7:", (x > y) ? "true" : "false");
    printf("%s %s\n", "100 < 7:", (x < y) ? "true" : "false");
    printf("%s %s\n", "100 >= 7:", (x >= y) ? "true" : "false");
    printf("%s %s\n", "100 <= 7:", (x <= y) ? "true" : "false");
    
    // Test logical operations
    printf("%s\n", "Testing logical operations:");
    a = 1;  // true
    b = 0;  // false
    c = 1;  // true
    printf("%s %s\n", "True and False =", (a && b) ? "true" : "false");
    printf("%s %s\n", "True or False =", (a || b) ? "true" : "false");
    printf("%s %s\n", "not True =", (!a) ? "true" : "false");
    tmp_bool = (a && b) || ((!b) && c);
    printf("%s %s\n", "(True and False) or (not False and True) =", tmp_bool ? "true" : "false");
    
    // Test control flow
    printf("%s\n", "Testing control flow:");
    printf("%s\n", "For loop:");
    for (i = 0; i < 3; i++) {
        printf("%s %lld\n", "Iteration", i);
    }
    
    printf("%s\n", "While loop:");
    counter = 0;
    while (counter < 3) {
        printf("%s %lld\n", "While iteration", counter);
        counter = counter + 1;
    }
    
    // Test conditionals
    printf("%s\n", "Testing conditionals:");
    if (x > y) {
        printf("%s\n", "100 is greater than 7");
    } else if (x == y) {
        printf("%s\n", "100 is equal to 7");
    } else {
        printf("%s\n", "100 is less than 7");
    }
    
    printf("%s\n", "Complex Tauraro features test completed!");
    return 0;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}