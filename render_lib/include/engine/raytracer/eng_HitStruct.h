#pragma once
#include <engine/eng_Material.h>
#include <engine/eng_Ray.h>
#include <engine/eng_Vector3D.h>

namespace eng::rt {

class IShader;

struct HitStruct {
    Ray r;
    Vector3DF normal;
    Vector3DF position;
    IShader* shader;
    const Material* pMaterial;
    float t;
}; // struct HitStruct

} // namespace eng
