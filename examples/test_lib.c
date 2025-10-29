// Simple C library for testing FFI functionality

// Function that adds two integers
int add_integers(int a, int b) {
    return a + b;
}

// Function that multiplies two doubles
double multiply_doubles(double a, double b) {
    return a * b;
}

// Function that takes a string and returns its length
int string_length(const char* str) {
    if (str == NULL) {
        return 0;
    }
    
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Function that takes a string and returns a new string
const char* greet(const char* name) {
    if (name == NULL) {
        return "Hello, World!";
    }
    
    static char buffer[256];
    snprintf(buffer, sizeof(buffer), "Hello, %s!", name);
    return buffer;
}

// Function that takes no arguments and returns void
void say_hello() {
    // In a real implementation, this might print to stdout
    // For testing purposes, we'll just return
}