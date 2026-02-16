#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_Interval.h>
#include <engine/eng_ray.h>

namespace eng {

class IShape {
public:
    virtual bool Intersect(const ray& r, Interval<float> t_range, HitStruct* p_hit_info_out) const = 0;
}; // class IShape

} // namespace eng
