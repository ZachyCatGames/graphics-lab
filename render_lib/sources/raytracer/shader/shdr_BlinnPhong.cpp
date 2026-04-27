#include <engine/raytracer/shader/shdr_BlinnPhong.h>
#include <engine/raytracer/eng_RayCaster.h>
#include <engine/eng_Scene.h>

namespace eng::rt::shdr {

Vector3DF BlinnPhong::GetColor(RayCaster* pRc, int depth, const HitStruct& rec) {

    Vector3DF light_sum;

    for (const auto& light : m_lights) {
        /* Calculate ray from position to the light. */
        const Ray r {
            rec.position,
            light.position - rec.position
        };

        if (!pRc->IsObjectInPath(r, {0.001, 1.0})) {
            const auto dir = light.GetDirection(rec.position);
            const auto nl  = std::max(0.0, dot(rec.normal, dir));

            const auto half_vector = (-rec.r.direction() + light.GetDirection(rec.position)).normalize();

            light_sum += 
                (m_Material.specular * Vector3DF( nl, nl, nl ) * light.intensity) +
                (m_Material.specular * std::pow(std::max(0.0, dot(rec.normal, half_vector)), m_Material.shininess));
        }
    }

    return light_sum;
}

} // namespace eng::shdr
