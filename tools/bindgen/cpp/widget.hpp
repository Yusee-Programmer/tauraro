#pragma once
namespace ui {
class Widget {
public:
    Widget(int w, int h);
    ~Widget();
    int area() const;
    void setSize(int w, int h);
    static Widget* create();
};
int version();
}
extern "C" {
    int plain_c_add(int a, int b);   /* a C-ABI function inside a C++ header */
}
