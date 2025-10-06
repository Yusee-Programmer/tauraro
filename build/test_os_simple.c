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

// Super() runtime support
void* tauraro_super() {
    return (void*)0x1; // Special value for super calls
}

void* tauraro_super_method_call(const char* method_name, void* self_obj, void** args, int arg_count) {
    // In a full implementation, this would resolve the method according to the MRO
    // For now, we'll just return NULL to indicate the method wasn't found
    printf("Super method call: %s\n", method_name);
    return NULL;
}

int32_t tauraro_main(void) {
    TauraroValue tmp_2;
    TauraroValue tmp_3;
    int64_t tmp_1;
    int64_t files;
    int64_t tmp_0;

    tmp_0 = os_getcwd(os);
    printf("%s\n", "Current directory:");
    tmp_1 = os_listdir(os, ".");
    files = tmp_1;
    printf("%s\n", "Directory contents:");
    tmp_2 = os.name;  // Get attribute
    printf("%s\n", "OS name:");
    tmp_3 = os.sep;  // Get attribute
    printf("%s\n", "Path separator:");
    return 0;
}

int main(int argc, char* argv[]) {
    return tauraro_main();
}
