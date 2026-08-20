#include "smartptr.hpp"
namespace w { std::shared_ptr<Thing> make(int v){ auto t=std::make_shared<Thing>(); t->v=v; return t; } }
