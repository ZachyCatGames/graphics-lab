#pragma once
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>

namespace eng {

struct HitStruct {
    Ray r;
    Vector3DF normal;
    Vector3DF position;
    float t;
}; // struct HitStruct

} // namespace eng
