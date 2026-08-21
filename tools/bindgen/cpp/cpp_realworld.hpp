#pragma once
// Reproduces the patterns surfaced by testing against real wxWidgets headers:
//  - an abstract base class (pure virtual) -> its constructor must NOT be emitted
//  - a forward-declared (incomplete) type used by value -> that wrapper must be skipped
//  - TRUE global-scope free functions + a global variable -> wrappers must not collide by name
struct Incomplete;                                  // forward-declared, never defined here
class Shape {                                       // abstract (pure virtual)
public:
    virtual int area() const = 0;
    virtual ~Shape();
    Incomplete makeThing() const;                   // by-value incomplete -> skipped
};
class Square : public Shape {
public:
    Square(int s);
    int area() const override;
private:
    int side;
};
Shape* make_square(int s);                          // global free fn returning base*
int display_depth();                                // global free fn (bare name would collide)
extern const int LIB_VERSION;                       // global variable (name would collide)
