#include <engine/raytracer/shader/shdr_FlatColorShader.h>

namespace eng::rt::shdr {

Vector3DF FlatColor::GetColor(RayCaster* pRc, int depth, const HitStruct&) { return m_color; }

const Material* FlatColor::GetMaterial() const {
    static constexpr Material material {
        .ambientLight = Vector3DF::Zero(),
        .diffuse = Vector3DF::Zero(),
        .specular = Vector3DF::Zero(),
        .shininess = 0
    };
    return &material;
}

} // namespace eng::rt::shdr
