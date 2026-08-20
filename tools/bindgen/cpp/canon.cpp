#include "canon.hpp"
namespace media {
Buffer::Buffer(int cap){}
int Buffer::at(int i) const { return i*2; }
Rect Buffer::bounds() const { Rect r; r.x=1; r.y=2; r.w=30; r.h=40; return r; }
std::string Buffer::name() const { return std::string("hello-cpp"); }
void Buffer::setName(const std::string& n){}
HRESULT process(const Buffer& b, Codec c){ return 1000 + (long)c; }
}
