// Auto-generated C++ -> C shim for shapes.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c shapes_shim.cpp
#include "shapes.hpp"
using namespace geo;
extern "C" {
geo::Shape* geo_Shape_new(double x, double y) { return new geo::Shape(x, y); }
void geo_Shape_delete(geo::Shape* self) { delete self; }
double geo_Shape_area(const geo::Shape* self) { return self->area(); }
void geo_Shape_move(geo::Shape* self, double dx, double dy) { self->move(dx, dy); }
Shape* geo_Shape_unit() { return geo::Shape::unit(); }
double geo_distance(Shape* a, Shape* b) { return geo::distance(a, b); }
}
