#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_Vector3D.h>

namespace eng {

class IShader {
public:
    virtual Vector3DF GetColor(const HitStruct& rec) = 0;
}; // class IShader

} // namespace eng
