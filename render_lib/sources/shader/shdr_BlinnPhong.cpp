#include <engine/shader/shdr_BlinnPhong.h>
#include <engine/eng_Scene.h>

namespace eng::shdr {

Vector3DF BlinnPhong::GetColor(Scene* p_scene, int depth, const HitStruct& rec) {
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

            const auto half_vector = (rec.r.direction() + light.GetDirection(rec.position)).normalize();

            light_sum += Vector3DF( nl, nl, nl ) * light.intensity + std::pow(std::max(0.0, dot(rec.normal, half_vector)), m_exp);
        }
    }

    return light_sum * base_color;
}

} // namespace eng::shdr
