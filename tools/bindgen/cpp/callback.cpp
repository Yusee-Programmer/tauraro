#include "callback.hpp"
#include <cstring>
namespace lib {
int apply(Transform fn, int v, void* user){ return fn ? fn(v, user) : v; }
void memcopy(void* dst, const void* src, int n){ memcpy(dst, src, n); }
}
