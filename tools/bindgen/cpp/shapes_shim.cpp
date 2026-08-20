// Auto-generated C++ -> C shim for shapes.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c shapes_shim.cpp
#include "shapes.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace geo;
extern "C" {
geo::Shape* geo_Shape_new(double x, double y) { return new geo::Shape(x, y); }
void geo_Shape_delete(geo::Shape* self) { delete self; }
double geo_Shape_area(const geo::Shape* self) { return self->area(); }
void geo_Shape_move(geo::Shape* self, double dx, double dy) { self->move(dx, dy); }
geo::Shape* geo_Shape_unit() { return (geo::Shape*)(geo::Shape::unit()); }
double geo_distance(geo::Shape *a, geo::Shape *b) { return geo::distance(a, b); }
}
