#pragma once
namespace w {
class Base { public: Base(int id); int id() const; virtual int kind() const; };
class Mid : public Base { public: Mid(); int midv() const; };
class Leaf : public Mid { public: Leaf(); int leafv() const; };   // 2-level inheritance
}
