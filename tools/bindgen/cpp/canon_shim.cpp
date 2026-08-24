// Auto-generated C++ -> C shim for canon.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c canon_shim.cpp
#include "canon.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace media;
extern "C" {
media::Buffer* media_Buffer_new(int cap) { return new media::Buffer(cap); }
int media_Buffer_at(const media::Buffer* self, int i) { return self->at(i); }
media::Rect media_Buffer_bounds(const media::Buffer* self) { return self->bounds(); }
char* media_Buffer_name(const media::Buffer* self) { return _tr_cpp_strdup(self->name()); }
void media_Buffer_setName(media::Buffer* self, const char* n) { self->setName(std::string(n)); }
long media_process(media::Buffer *b, media::Codec c) { return media::process(*b, c); }
}
