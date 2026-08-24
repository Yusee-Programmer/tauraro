#include "operators.hpp"
namespace vm {
Vec2::Vec2(int x_, int y_): x(x_), y(y_) {}
Vec2 Vec2::operator+(const Vec2& o) const { return Vec2(x+o.x, y+o.y); }
Vec2 Vec2::operator-(const Vec2& o) const { return Vec2(x-o.x, y-o.y); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
int  Vec2::operator[](int i) const { return i==0 ? x : y; }
bool Vec2::operator==(const Vec2& o) const { return x==o.x && y==o.y; }
bool Vec2::operator<(const Vec2& o) const { return (x*x+y*y) < (o.x*o.x+o.y*o.y); }
Vec2& Vec2::operator+=(const Vec2& o) { x+=o.x; y+=o.y; return *this; }
}
