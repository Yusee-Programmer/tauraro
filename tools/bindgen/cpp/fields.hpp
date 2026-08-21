#pragma once
namespace n {
// Non-POD (has a ctor) but with public data members — their accessors must be synthesized.
class Point {
public:
    int x, y;
    double weight;
    Point(int x_, int y_);
    int sum() const;
};
}
