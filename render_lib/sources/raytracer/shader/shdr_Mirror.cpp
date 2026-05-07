#include <engine/raytracer/shader/shdr_Mirror.h>
#include <engine/raytracer/eng_RayCaster.h>
#include <engine/eng_Scene.h>

namespace eng::rt::shdr {

Vector3DF Mirror::GetColor(RayCaster* pRc, int depth, const HitStruct& rec) {
    const auto dir = rec.r.direction().normalize();
    const auto nor = rec.normal;
    const Ray r {
        rec.position,
        (dir - 2 * dot(dir, nor) * nor).normalize(),
    };

    return pRc->CastRayIntoScene(r, {0.1, std::numeric_limits<float>::infinity()}, depth+1);
}

Material Mirror::GetMaterial() const {
    static constexpr Material material {
        .ambientLight = Vector3DF::Zero(),
        .diffuse = Vector3DF::Zero(),
        .specular = Vector3DF::Zero(),
        .shininess = std::numeric_limits<float>::infinity()
    };
    return material;
}

} // namespace eng
