#pragma once
#include "../ray.h"
#include "../Interval.h"

namespace eng {

struct HitStruct {
    Vector3DF normal;
    Vector3DF position;
    float t;
}; // struct HitStruct

class IShape {
public:
    virtual bool Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) = 0;
}; // class IShape

} // namespace eng
