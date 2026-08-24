#pragma once
#include <memory>
namespace w { struct Thing { int v; }; std::shared_ptr<Thing> make(int v); }
