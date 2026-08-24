// Auto-generated C++ -> C shim for box.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c box_shim.cpp
#include "box.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace app;
extern "C" {
Box<int>* app_makeBox(int x) { return new Box<int>(app::makeBox(x)); }
Box<int>* Box_int__new(int v) { return new Box<int>(v); }
int Box_int__get(const Box<int>* self) { return self->get(); }
void Box_int__set(Box<int>* self, int v) { self->set(v); }
int Box_int__tag(const Box<int>* self) { return self->tag(); }
}
