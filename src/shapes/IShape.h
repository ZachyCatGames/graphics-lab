#pragma once
#include "../ray.h"

namespace eng {

struct HitStruct {

}; // struct HitStruct

class IShape {
public:
    virtual bool Intersect(const ray& r, float tmin, float* p_tmax, HitStruct* p_hit_info_out) = 0;
}; // class IShape

} // namespace eng
