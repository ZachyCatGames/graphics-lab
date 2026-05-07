#pragma once
#include <engine/eng_Vector3D.h>

namespace eng {

// borrowed from model_obj
struct Vertex {
    Vector3DF position;
    Vector3DF normal;
    Vector2DF texCoord;
    //float tangent[4];
    //float bitangent[3];
};

} // namespace eng
