// Test-fixture implementation of media.hpp.
#include "media.hpp"
namespace media {
Buffer::Buffer(): data_(nullptr), size_(0) {}
Buffer::Buffer(int c): data_(new int[c]), size_(0) {}
Buffer::~Buffer(){ delete[] data_; }
int Buffer::size() const { return size_; }
void Buffer::append(int b){ size_++; }
void Buffer::appendAll(const int* d, int n){ size_ += n; }
int Buffer::at(int i) const { return i; }
Buffer Buffer::withCapacity(int c){ return Buffer(c); }
Buffer Buffer::clone() const { return Buffer(); }
int encode(const Buffer& b, Codec c){ return (int)c; }
int encode(const Buffer& b, Codec c, int q){ return (int)c + q; }
Encoder::Encoder(Codec c){} Encoder::Encoder(Codec c,int t){}
Encoder::~Encoder(){}
int Encoder::process(Buffer& i, Buffer& o){ return 1; }
void Encoder::setFlag(Flags f){}
static std::string g_name = "enc";
const std::string& Encoder::name() const { return g_name; }
void Encoder::setName(const std::string& n){ g_name = n; }
double Encoder::ratio() const { return 0.5; }
Encoder& Encoder::operator=(const Encoder& o){ return *this; }
bool Encoder::operator==(const Encoder& o) const { return true; }
Encoder::Stats::Stats(){}
long Encoder::Stats::frames() const { return 99; }
Encoder::Stats Encoder::stats() const { return Stats(); }
FastEncoder::FastEncoder(): Encoder(Codec::H264){}
int FastEncoder::process(Buffer& i, Buffer& o){ return 2; }
int FastEncoder::turbo(){ return 7; }
double distance(double a, double b){ return b - a; }
}
