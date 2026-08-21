#pragma once
#include <stdexcept>
namespace e {
// Throws std::runtime_error when x < 0 — must NOT crash the process across the FFI boundary.
int risky(int x);
}
