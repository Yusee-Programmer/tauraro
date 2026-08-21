#include "exceptions.hpp"
namespace e {
int risky(int x){ if(x<0) throw std::runtime_error("negative input"); return x*2; }
}
