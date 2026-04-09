#pragma once
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>

namespace eng {

class IShader;

struct HitStruct {
    Ray r;
    Vector3DF normal;
    Vector3DF position;
    Handle<IShader> shader;
    float t;
}; // struct HitStruct

} // namespace eng
