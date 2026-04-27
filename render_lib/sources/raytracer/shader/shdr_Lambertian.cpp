#include <engine/raytracer/shader/shdr_Lambertian.h>
#include <engine/raytracer/eng_RayCaster.h>
#include <engine/eng_Scene.h>
#include <print>

namespace eng::rt::shdr {

Vector3DF Lambertian::GetColor(RayCaster* pRc, int depth, const HitStruct& rec) {

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

            light_sum += (m_Material.diffuse * Vector3DF( nl, nl, nl ) * light.intensity);
        }
    }

    return light_sum;
}

} // namespace eng::shdr
