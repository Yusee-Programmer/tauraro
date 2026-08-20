// Auto-generated C++ -> C shim for vec.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c vec_shim.cpp
#include "vec.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace nums;
extern "C" {
std::vector<int>* nums_range(int n) { return new std::vector<int>(nums::range(n)); }
std::vector<int>* std__vector_int__new() { return new std::vector<int>(); }
std::vector<int>* std__vector_int__new_2(std::allocator<int> *__a) { return new std::vector<int>(*__a); }
std::vector<int>* std__vector_int__new_3(unsigned long long __n, std::allocator<int> *__a) { return new std::vector<int>(__n, *__a); }
std::vector<int>* std__vector_int__new_4(unsigned long long __n) { return new std::vector<int>(__n); }
std::vector<int>* std__vector_int__new_5(unsigned long long __n, int __value, std::allocator<int> *__a) { return new std::vector<int>(__n, __value, *__a); }
std::vector<int>* std__vector_int__new_6(unsigned long long __n, int __value) { return new std::vector<int>(__n, __value); }
void std__vector_int__assign(std::vector<int>* self, unsigned long long __n, int __val) { self->assign(__n, __val); }
unsigned long long std__vector_int__size(const std::vector<int>* self) { return self->size(); }
unsigned long long std__vector_int__max_size(const std::vector<int>* self) { return self->max_size(); }
void std__vector_int__resize(std::vector<int>* self, unsigned long long __new_size) { self->resize(__new_size); }
void std__vector_int__resize_2(std::vector<int>* self, unsigned long long __new_size, int __x) { self->resize(__new_size, __x); }
void std__vector_int__shrink_to_fit(std::vector<int>* self) { self->shrink_to_fit(); }
unsigned long long std__vector_int__capacity(const std::vector<int>* self) { return self->capacity(); }
bool std__vector_int__empty(const std::vector<int>* self) { return self->empty(); }
void std__vector_int__reserve(std::vector<int>* self, unsigned long long __n) { self->reserve(__n); }
int * std__vector_int__at(std::vector<int>* self, unsigned long long __n) { return (int *)(&(self->at(__n))); }
int * std__vector_int__at_2(const std::vector<int>* self, unsigned long long __n) { return (int *)(&(self->at(__n))); }
int * std__vector_int__front(std::vector<int>* self) { return (int *)(&(self->front())); }
int * std__vector_int__front_2(const std::vector<int>* self) { return (int *)(&(self->front())); }
int * std__vector_int__back(std::vector<int>* self) { return (int *)(&(self->back())); }
int * std__vector_int__back_2(const std::vector<int>* self) { return (int *)(&(self->back())); }
int * std__vector_int__data(std::vector<int>* self) { return (int *)(self->data()); }
int * std__vector_int__data_2(const std::vector<int>* self) { return (int *)(self->data()); }
void std__vector_int__push_back(std::vector<int>* self, int __x) { self->push_back(__x); }
void std__vector_int__push_back_2(std::vector<int>* self, int *__x) { self->push_back(*__x); }
void std__vector_int__pop_back(std::vector<int>* self) { self->pop_back(); }
void std__vector_int__clear(std::vector<int>* self) { self->clear(); }
}
