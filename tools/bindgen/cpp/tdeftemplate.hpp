#pragma once
template<typename T> class Ring { T buf; int n; public: Ring(int cap); void push(T v); T pop(); int size() const; };
typedef Ring<int> IntRing;      // typedef-only — no function references it
