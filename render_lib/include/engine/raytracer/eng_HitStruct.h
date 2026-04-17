#pragma once
#include <engine/eng_ObjectManager.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>
#include <engine/eng_Material.h>

namespace eng::rt {

class IShader;

struct HitStruct {
    Ray r;
    Vector3DF normal;
    Vector3DF position;
    IShader* shader;
    Material* material;
    float t;
}; // struct HitStruct

} // namespace eng
