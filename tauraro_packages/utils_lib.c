#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Simple utility functions that can be called from Tauraro

// Get current timestamp
__declspec(dllexport) long long get_timestamp() {
    return (long long)time(NULL);
}

// Calculate factorial
__declspec(dllexport) long long factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Reverse a string
__declspec(dllexport) void reverse_string(char* str) {
    if (!str) return;
    
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

// Generate random number between min and max
__declspec(dllexport) int random_range(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    
    srand((unsigned int)time(NULL));
    return min + rand() % (max - min + 1);
}

// Calculate power
__declspec(dllexport) double power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}