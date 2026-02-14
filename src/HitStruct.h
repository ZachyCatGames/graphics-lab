#pragma once
#include "vec.h"

namespace eng {

struct HitStruct {
    Vector3DF normal;
    Vector3DF position;
    float t;
}; // struct HitStruct

} // namespace eng
