// Auto-generated C++ -> C shim for mapdemo.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c mapdemo_shim.cpp
#include "mapdemo.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace db;
extern "C" {
std::map<int, double>* db_prices() { return new std::map<int, double>(db::prices()); }
std::map<int, double>* std__map_int__double__new() { return new std::map<int, double>(); }
std::map<int, double>* std__map_int__double__new_2(std::less<int> *__comp, std::allocator<std::pair<const int, double>> *__a) { return new std::map<int, double>(*__comp, *__a); }
std::map<int, double>* std__map_int__double__new_3(std::less<int> *__comp) { return new std::map<int, double>(*__comp); }
std::map<int, double>* std__map_int__double__new_4(std::allocator<std::pair<const int, double>> *__a) { return new std::map<int, double>(*__a); }
std::allocator<std::pair<const int, double>>* std__map_int__double__get_allocator(const std::map<int, double>* self) { return new std::allocator<std::pair<const int, double>>(self->get_allocator()); }
bool std__map_int__double__empty(const std::map<int, double>* self) { return self->empty(); }
size_t std__map_int__double__size(const std::map<int, double>* self) { return self->size(); }
size_t std__map_int__double__max_size(const std::map<int, double>* self) { return self->max_size(); }
double * std__map_int__double__at(std::map<int, double>* self, int __k) { return (double *)(&(self->at(__k))); }
double * std__map_int__double__at_2(const std::map<int, double>* self, int __k) { return (double *)(&(self->at(__k))); }
size_t std__map_int__double__erase(std::map<int, double>* self, int __x) { return self->erase(__x); }
void std__map_int__double__clear(std::map<int, double>* self) { self->clear(); }
std::less<int>* std__map_int__double__key_comp(const std::map<int, double>* self) { return new std::less<int>(self->key_comp()); }
size_t std__map_int__double__count(const std::map<int, double>* self, int __x) { return self->count(__x); }
}
