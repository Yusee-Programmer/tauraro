#include "cpp_realworld.hpp"
Shape::~Shape() {}
Square::Square(int s): side(s) {}
int Square::area() const { return side*side; }
Shape* make_square(int s) { return new Square(s); }
int display_depth() { return 32; }
const int LIB_VERSION = 5;
