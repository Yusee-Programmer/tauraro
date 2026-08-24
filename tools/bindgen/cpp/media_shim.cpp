// Auto-generated C++ -> C shim for media.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c media_shim.cpp
#include "media.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
// std::string return -> heap char* copy (caller owns it; free with the runtime free).
static char* _tr_cpp_strdup(const std::string& s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.c_str(), s.size()); p[s.size()]=0; } return p; }
using namespace media;
extern "C" {
media::Buffer* media_Buffer_new() { return new media::Buffer(); }
media::Buffer* media_Buffer_new_2(int capacity) { return new media::Buffer(capacity); }
void media_Buffer_delete(media::Buffer* self) { delete self; }
int media_Buffer_size(const media::Buffer* self) { return self->size(); }
void media_Buffer_append(media::Buffer* self, int byte) { self->append(byte); }
void media_Buffer_appendAll(media::Buffer* self, int *data, int n) { self->appendAll(data, n); }
int media_Buffer_at(const media::Buffer* self, int i) { return self->at(i); }
media::Buffer* media_Buffer_withCapacity(int cap) { return new media::Buffer(media::Buffer::withCapacity(cap)); }
media::Buffer* media_Buffer_clone(const media::Buffer* self) { return new media::Buffer(self->clone()); }
int media_encode(media::Buffer *b, media::Codec c) { return media::encode(*b, c); }
int media_encode_2(media::Buffer *b, media::Codec c, int quality) { return media::encode(*b, c, quality); }
media::Encoder* media_Encoder_new(media::Codec c) { return new media::Encoder(c); }
media::Encoder* media_Encoder_new_2(media::Codec c, int threads) { return new media::Encoder(c, threads); }
void media_Encoder_delete(media::Encoder* self) { delete self; }
int media_Encoder_process(media::Encoder* self, media::Buffer *in, media::Buffer *out) { return self->process(*in, *out); }
void media_Encoder_setFlag(media::Encoder* self, media::Flags f) { self->setFlag(f); }
char* media_Encoder_name(const media::Encoder* self) { return _tr_cpp_strdup(self->name()); }
void media_Encoder_setName(media::Encoder* self, const char* n) { self->setName(std::string(n)); }
double media_Encoder_ratio(const media::Encoder* self) { return self->ratio(); }
media::Encoder::Stats* media_Encoder_stats(const media::Encoder* self) { return new media::Encoder::Stats(self->stats()); }
media::Encoder::Stats* media_Encoder_Stats_new() { return new media::Encoder::Stats(); }
long media_Encoder_Stats_frames(const media::Encoder::Stats* self) { return self->frames(); }
media::FastEncoder* media_FastEncoder_new() { return new media::FastEncoder(); }
int media_FastEncoder_process(media::FastEncoder* self, media::Buffer *in, media::Buffer *out) { return self->process(*in, *out); }
int media_FastEncoder_turbo(media::FastEncoder* self) { return self->turbo(); }
void media_FastEncoder_setFlag(media::FastEncoder* self, media::Flags f) { self->setFlag(f); }
char* media_FastEncoder_name(const media::FastEncoder* self) { return _tr_cpp_strdup(self->name()); }
void media_FastEncoder_setName(media::FastEncoder* self, const char* n) { self->setName(std::string(n)); }
double media_FastEncoder_ratio(const media::FastEncoder* self) { return self->ratio(); }
media::Encoder::Stats* media_FastEncoder_stats(const media::FastEncoder* self) { return new media::Encoder::Stats(self->stats()); }
double media_distance(double a, double b) { return media::distance(a, b); }
}
