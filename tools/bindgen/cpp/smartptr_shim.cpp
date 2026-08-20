// Auto-generated C++ -> C shim for smartptr.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c smartptr_shim.cpp
#include "smartptr.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace w;
extern "C" {
std::shared_ptr<w::Thing>* w_make(int v) { return new std::shared_ptr<w::Thing>(w::make(v)); }
std::shared_ptr<w::Thing>* std__shared_ptr_w__Thing__new() { return new std::shared_ptr<w::Thing>(); }
std::shared_ptr<w::Thing>* std__shared_ptr_w__Thing__new_2(std::nullptr_t *a0) { return new std::shared_ptr<w::Thing>(*a0); }
w::Thing* std__shared_ptr_w__Thing__get(const std::shared_ptr<w::Thing>* self) { return (w::Thing*)(self->get()); }
}
