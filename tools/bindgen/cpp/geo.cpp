#include "geo.hpp"
namespace geo {
  Vec3 add(Vec3 a, Vec3 b){ Vec3 r; r.x=a.x+b.x; r.y=a.y+b.y; r.z=a.z+b.z; return r; }
  float length(Vec3 v){ return v.x*v.x+v.y*v.y+v.z*v.z; }
}
