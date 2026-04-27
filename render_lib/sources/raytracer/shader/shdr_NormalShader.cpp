#include <engine/raytracer/shader/shdr_NormalShader.h>

namespace eng::rt::shdr {

Vector3DF Normal::GetColor(RayCaster*, int depth, const HitStruct& rec) {
    return Vector3DF(0.5 * (rec.normal.normalize() + Vector3DF{1, 1, 1}));
}

Material Normal::GetMaterial() const {
    static constexpr Material material {
        .ambientLight = Vector3DF::Zero(),
        .diffuse = Vector3DF::Zero(),
        .specular = Vector3DF::Zero(),
        .shininess = 0
    };
    return material;
}

} // namespace eng::shdr
