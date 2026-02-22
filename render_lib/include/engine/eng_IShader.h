#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_Vector3D.h>

namespace eng {

class Scene;

class IShader {
public:
    virtual Vector3DF GetColor(Scene* p_scene, const HitStruct& rec) = 0;
}; // class IShader

} // namespace eng
