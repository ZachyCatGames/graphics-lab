#pragma once
#include <engine/raytracer/eng_HitStruct.h>
#include <engine/eng_Vector3D.h>

namespace eng::rt {

class RayCaster;

class IShader {
public:
    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) = 0;
}; // class IShader

} // namespace eng
