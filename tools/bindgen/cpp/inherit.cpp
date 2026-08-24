#include "inherit.hpp"
namespace w {
Base::Base(int i){} 
int Base::id() const { return 100; }
int Base::kind() const { return 1; }
Mid::Mid():Base(5){}  int Mid::midv() const { return 20; }
Leaf::Leaf():Mid(){}  int Leaf::leafv() const { return 30; }
}
