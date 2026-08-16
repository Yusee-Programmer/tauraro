// Implementation of the geo::Shape API declared in shapes.hpp (test fixture).
#include "shapes.hpp"
namespace geo {
Shape::Shape(double x, double y) : x_(x), y_(y) {}
Shape::~Shape() {}
double Shape::area() const { return x_ * y_; }
void Shape::move(double dx, double dy) { x_ += dx; y_ += dy; }
Shape* Shape::unit() { return new Shape(1.0, 1.0); }
double Shape::secret() { return 42.0; }
double distance(const Shape* a, const Shape* b) { return a->area() + b->area(); }
}
