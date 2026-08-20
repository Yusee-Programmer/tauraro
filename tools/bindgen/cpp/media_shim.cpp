// Auto-generated C++ -> C shim for media.hpp (tauraro-bindgen -h cpp).
// Compile:  c++ -c media_shim.cpp
#include "media.hpp"
using namespace media;
extern "C" {
media::Buffer* media_Buffer_new() { return new media::Buffer(); }
media::Buffer* media_Buffer_new_2(int capacity) { return new media::Buffer(capacity); }
void media_Buffer_delete(media::Buffer* self) { delete self; }
int media_Buffer_size(const media::Buffer* self) { return self->size(); }
void media_Buffer_append(media::Buffer* self, int byte) { self->append(byte); }
void media_Buffer_appendAll(media::Buffer* self, int* data, int n) { self->appendAll(data, n); }
int media_Buffer_at(const media::Buffer* self, int i) { return self->at(i); }
media::Buffer* media_Buffer_withCapacity(int cap) { return new media::Buffer(media::Buffer::withCapacity(cap)); }
media::Buffer* media_Buffer_clone(const media::Buffer* self) { return new media::Buffer(self->clone()); }
int media_encode(media::Buffer* b, Codec c) { return media::encode(*b, c); }
int media_encode_2(media::Buffer* b, Codec c, int quality) { return media::encode(*b, c, quality); }
media::Encoder* media_Encoder_new(Codec c) { return new media::Encoder(c); }
media::Encoder* media_Encoder_new_2(Codec c, int threads) { return new media::Encoder(c, threads); }
void media_Encoder_delete(media::Encoder* self) { delete self; }
int media_Encoder_process(media::Encoder* self, media::Buffer* in, media::Buffer* out) { return self->process(*in, *out); }
void media_Encoder_setFlag(media::Encoder* self, Flags f) { self->setFlag(f); }
std::string* media_Encoder_name(const media::Encoder* self) { return (std::string*)(&(self->name())); }
void media_Encoder_setName(media::Encoder* self, std::string* n) { self->setName(*n); }
double media_Encoder_ratio(const media::Encoder* self) { return self->ratio(); }
media::Encoder::Stats* media_Encoder_Stats_new() { return new media::Encoder::Stats(); }
long media_Encoder_Stats_frames(const media::Encoder::Stats* self) { return self->frames(); }
media::Encoder::Stats* media_Encoder_stats(const media::Encoder* self) { return new media::Encoder::Stats(self->stats()); }
media::FastEncoder* media_FastEncoder_new() { return new media::FastEncoder(); }
int media_FastEncoder_process(media::FastEncoder* self, media::Buffer* in, media::Buffer* out) { return self->process(*in, *out); }
int media_FastEncoder_turbo(media::FastEncoder* self) { return self->turbo(); }
double media_distance(double a, double b) { return media::distance(a, b); }
}
