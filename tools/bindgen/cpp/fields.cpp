#include "fields.hpp"
namespace n {
Point::Point(int x_, int y_): x(x_), y(y_), weight(1.5) {}
int Point::sum() const { return x + y; }
}
