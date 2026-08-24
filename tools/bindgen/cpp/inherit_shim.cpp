// Auto-generated C++ -> C shim for inherit.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c inherit_shim.cpp
#include "inherit.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace w;
extern "C" {
w::Base* w_Base_new(int id) { return new w::Base(id); }
int w_Base_id(const w::Base* self) { return self->id(); }
int w_Base_kind(const w::Base* self) { return self->kind(); }
w::Mid* w_Mid_new() { return new w::Mid(); }
int w_Mid_midv(const w::Mid* self) { return self->midv(); }
int w_Mid_id(const w::Mid* self) { return self->id(); }
int w_Mid_kind(const w::Mid* self) { return self->kind(); }
w::Leaf* w_Leaf_new() { return new w::Leaf(); }
int w_Leaf_leafv(const w::Leaf* self) { return self->leafv(); }
int w_Leaf_id(const w::Leaf* self) { return self->id(); }
int w_Leaf_kind(const w::Leaf* self) { return self->kind(); }
int w_Leaf_midv(const w::Leaf* self) { return self->midv(); }
}
