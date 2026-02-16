#pragma once
#include "ray.h"
#include "vec.h"

namespace eng {

struct HitStruct {
    ray r;
    Vector3DF normal;
    Vector3DF position;
    float t;
}; // struct HitStruct

} // namespace eng
