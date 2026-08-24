// Canonical-type resolution regression for tauraroc bindgen -h cpp: a typedef'd scalar return
// (HRESULT-style `long`), a POD value-struct returned/passed by value, an `std::string`
// return/param (auto-converted to a native C string), and an enum crossed by value.
#pragma once
#include <string>
typedef long HRESULT;
namespace media {
enum class Codec { H264, H265 };
struct Rect { int x; int y; int w; int h; };
class Buffer {
public:
    Buffer(int cap);
    int at(int i) const;
    Rect bounds() const;                 // POD struct by value
    std::string name() const;            // std::string -> native string
    void setName(const std::string& n);
};
HRESULT process(const Buffer& b, Codec c);   // typedef'd scalar (long) + enum by value
}
