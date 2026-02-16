#pragma once
#include "../vec.h"
#include "../HitStruct.h"

namespace eng {

class IShader {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) = 0;
}; // class IShader

} // namespace eng
