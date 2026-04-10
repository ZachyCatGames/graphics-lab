#include <engine/raytracer/shader/shdr_Lambertian.h>
#include <engine/raytracer/eng_Scene.h>
#include <print>

namespace eng::rt::shdr {

Vector3DF Lambertian::GetColor(Scene* p_scene, int depth, const HitStruct& rec) {
    /* Request color from base shader. */
    const auto base_color = m_base ? m_base->GetColor(p_scene, depth, rec) : Vector3DF(1,1,1);

    Vector3DF light_sum;
    for (const auto& light : m_lights) {
        /* Calculate ray from position to the light. */
        const Ray r {
            rec.position,
            light.position - rec.position
        };

        if (!p_scene->IsObjectInPath(r, {0.001, 1.0})) {
            const auto dir = light.GetDirection(rec.position);
            const auto nl  = std::max(0.0, dot(rec.normal, dir));

            light_sum += Vector3DF( nl, nl, nl ) * light.intensity;
        }
    }

    return light_sum * base_color;
}

} // namespace eng::shdr
