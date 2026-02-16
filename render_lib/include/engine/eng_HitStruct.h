#pragma once
#include <engine/eng_ray.h>
#include <engine/eng_vec.h>

namespace eng {

struct HitStruct {
    ray r;
    Vector3DF normal;
    Vector3DF position;
    float t;
}; // struct HitStruct

} // namespace eng
