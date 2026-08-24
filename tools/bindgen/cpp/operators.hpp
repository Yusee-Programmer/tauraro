#pragma once
namespace vm {
class Vec2 {
public:
    int x, y;
    Vec2(int x_, int y_);
    Vec2 operator+(const Vec2& o) const;   // op_add
    Vec2 operator-(const Vec2& o) const;   // op_sub
    Vec2 operator-() const;                // op_neg (unary)
    int  operator[](int i) const;          // op_index
    bool operator==(const Vec2& o) const;  // op_eq
    bool operator<(const Vec2& o) const;   // op_lt
    Vec2& operator+=(const Vec2& o);       // op_iadd (returns self ref)
};
}
