#pragma once
namespace geo {
enum class Kind { Circle = 0, Square = 2, Triangle };
class Shape {
public:
    Shape(double x, double y);
    ~Shape();
    double area() const;
    void move(double dx, double dy);
    static Shape* unit();
private:
    double secret();
    double x_, y_;
};
double distance(const Shape* a, const Shape* b);
}
