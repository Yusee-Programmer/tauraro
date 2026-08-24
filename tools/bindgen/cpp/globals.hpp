#pragma once
namespace cfg {
extern const int VERSION;            // extern global (defined in the .cpp)
const double PI = 3.14159;           // in-header constant
class Limits {
public:
    static const int MAX = 100;      // static class constant
    static int current();
};
}
