#include "box.hpp"
template<typename T> Box<T>::Box(T v): v_(v) {}
template<typename T> T Box<T>::get() const { return v_; }
template<typename T> void Box<T>::set(T v){ v_ = v; }
template<typename T> int Box<T>::tag() const { return 7; }
template class Box<int>;
namespace app { Box<int> makeBox(int x){ return Box<int>(x*10); } }
