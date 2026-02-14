#pragma once
#include "../ray.h"
#include "../Interval.h"
#include "../HitStruct.h"

namespace eng {

class IShape {
public:
    virtual bool Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) = 0;
}; // class IShape

} // namespace eng
