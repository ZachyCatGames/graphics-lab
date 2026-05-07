#include <engine/raytracer/shader/shdr_Diffuse.h>
#include <engine/raytracer/eng_RayCaster.h>
#include <engine/eng_Scene.h>

namespace eng::rt::shdr {

Vector3DF Diffuse::GetColor(RayCaster* pRc, int depth, const HitStruct& rec) {
    Vector3DF direction;
    while (true) {
        /* Sample a random point in the unit circle. */
        direction = Vector3DF::random_in_unit_sphere();

        /* Make sure the vector is non-zero (or close to zero). */
        if (direction.length_squared() > 0) {
            direction = direction.normalize();

            break;
        }
    }

    const Ray r(rec.position, direction + rec.normal);

    return m_Material.diffuse * pRc->CastRayIntoScene(r, {0.001, std::numeric_limits<float>::infinity()}, depth+1);
}

} // namespace eng::shdr
