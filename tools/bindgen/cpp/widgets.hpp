#pragma once
// Reproduces patterns surfaced by testing against real wxWidgets widget headers:
//  - array parameters (`const int vals[]`) -> must lower to a pointer, not `T[] *`
//  - a member-function-pointer parameter -> can't cross a C ABI, must be skipped
//  - a base method re-declared PROTECTED via `using` in a derived class -> excluded from binding
class Base {
public:
    int compute() const;                 // public here
    void connect(void (Base::*cb)(int)); // member-fn-pointer param -> method skipped
};
class Widget : public Base {
public:
    Widget();
    int sum(const int vals[], int n) const;   // array parameter
protected:
    using Base::compute;                       // re-declares compute() as PROTECTED -> excluded
};
