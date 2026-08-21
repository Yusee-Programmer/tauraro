#pragma once
namespace m {
// Header-only math-utility free-function templates (all-T shape) — the shim's explicit
// ns::maxof<int>(...) instantiates them at shim-compile time, so no separate .cpp is needed.
template<class T> T maxof(T a, T b) { return a > b ? a : b; }
template<class T> T clamp(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }
}
