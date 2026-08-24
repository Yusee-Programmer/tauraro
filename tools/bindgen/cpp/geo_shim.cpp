// Auto-generated C++ -> C shim for geo.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c geo_shim.cpp
#include "geo.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace geo;
extern "C" {
Vec3 geo_add(Vec3 a, Vec3 b) { return geo::add(a, b); }
float geo_length(Vec3 v) { return geo::length(v); }
}
