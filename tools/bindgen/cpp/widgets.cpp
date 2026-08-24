#include "widgets.hpp"
int Base::compute() const { return 1; }
void Base::connect(void (Base::*)(int)) {}
Widget::Widget() {}
int Widget::sum(const int vals[], int n) const { int s=0; for(int i=0;i<n;i++) s+=vals[i]; return s; }
