#include <engine/shader/shdr_Mirror.h>
#include <engine/eng_Scene.h>

namespace eng::shdr {

Vector3DF Mirror::GetColor(Scene* p_scene, int depth, const HitStruct& rec) {
    const auto dir = rec.r.direction().normalize();
    const auto nor = rec.normal.normalize();
    const Ray r {
        rec.position,
        (dir - 2 * dot(dir, nor) * nor).normalize(),
    };

    return p_scene->GetRayColor(r, {0.001, std::numeric_limits<float>::infinity()}, depth+1);
}

} // namespace eng
