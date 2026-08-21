#pragma once
namespace lib {
typedef int (*Transform)(int x, void* user);
int apply(Transform fn, int v, void* user);   // callback + void* userdata
void memcopy(void* dst, const void* src, int n);  // void* params
}
