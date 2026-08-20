#pragma once
template<typename T> class Box {
  T v_;
public:
  Box(T v);
  T get() const;
  void set(T v);
  int tag() const;
};
namespace app {
  Box<int> makeBox(int x);           // uses Box<int> in the API
}
