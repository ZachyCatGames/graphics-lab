#pragma once
#include <engine/eng_Vector3D.h>

namespace eng {

struct Material {
    Vector3DF ambientLight;
    Vector3DF diffuse;
    Vector3DF specular;
    float shininess;
}; // struct Material

} // namespace eng
