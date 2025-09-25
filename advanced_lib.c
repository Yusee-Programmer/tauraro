#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Export functions for Windows DLL
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

// String manipulation functions
EXPORT char* reverse_string(const char* input) {
    if (!input) return NULL;
    
    int len = strlen(input);
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL;
    
    for (int i = 0; i < len; i++) {
        result[i] = input[len - 1 - i];
    }
    result[len] = '\0';
    
    return result;
}

EXPORT int string_length(const char* str) {
    return str ? strlen(str) : 0;
}

EXPORT char* concatenate_strings(const char* str1, const char* str2) {
    if (!str1 || !str2) return NULL;
    
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);
    if (!result) return NULL;
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

// Array operations
EXPORT int sum_array(int* arr, int size) {
    if (!arr || size <= 0) return 0;
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

EXPORT double average_array(double* arr, int size) {
    if (!arr || size <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

EXPORT int find_max(int* arr, int size) {
    if (!arr || size <= 0) return 0;
    
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Mathematical functions
EXPORT double power_function(double base, double exponent) {
    return pow(base, exponent);
}

EXPORT double square_root(double number) {
    return number >= 0 ? sqrt(number) : -1.0;
}

EXPORT int factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

EXPORT int fibonacci(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Utility functions
EXPORT int is_prime(int number) {
    if (number < 2) return 0;
    if (number == 2) return 1;
    if (number % 2 == 0) return 0;
    
    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) return 0;
    }
    return 1;
}

EXPORT double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

EXPORT double fahrenheit_to_celsius(double fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

// Memory management helper
EXPORT void free_string(char* str) {
    if (str) {
        free(str);
    }
}

// Random number generation
EXPORT int random_int(int min, int max) {
    if (min > max) return min;
    return min + rand() % (max - min + 1);
}

EXPORT double random_double(double min, double max) {
    if (min > max) return min;
    double scale = rand() / (double) RAND_MAX;
    return min + scale * (max - min);
}

// Date/Time utilities (simplified)
EXPORT int get_current_year() {
    // Simplified - would normally use time functions
    return 2024;
}

EXPORT const char* get_library_version() {
    return "Advanced FFI Library v1.0";
}