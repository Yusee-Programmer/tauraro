// Complex C++ API surface for tauraro-bindgen -h cpp regression: overloaded ctors/methods/free
// functions, nested classes, single inheritance + virtual, const/static methods, references,
// by-value class returns, std:: params/returns, operator overloads, scoped+plain enums, and a
// template (correctly skipped). Drives all the C++ generator hardening.
#pragma once
#include <string>
namespace media {
enum class Codec { H264 = 0, H265, AV1 };
enum Flags { FLAG_NONE = 0, FLAG_FAST = 1, FLAG_HQ = 2 };
class Buffer {
public:
    Buffer();
    Buffer(int capacity);
    ~Buffer();
    int size() const;
    void append(int byte);
    void appendAll(const int* data, int n);
    int at(int i) const;
    static Buffer withCapacity(int cap);
    Buffer clone() const;
private:
    int* data_; int size_;
};
int encode(const Buffer& b, Codec c);
int encode(const Buffer& b, Codec c, int quality);
class Encoder {
public:
    Encoder(Codec c);
    Encoder(Codec c, int threads);
    virtual ~Encoder();
    virtual int process(Buffer& in, Buffer& out);
    void setFlag(Flags f);
    const std::string& name() const;
    void setName(const std::string& n);
    double ratio() const;
    Encoder& operator=(const Encoder& o);
    bool operator==(const Encoder& o) const;
    class Stats { public: Stats(); long frames() const; };
    Stats stats() const;
};
class FastEncoder : public Encoder {
public:
    FastEncoder();
    int process(Buffer& in, Buffer& out);
    int turbo();
};
template<typename T> class Ring { public: Ring(int n); void push(T v); T pop(); };
double distance(double a, double b);
}
