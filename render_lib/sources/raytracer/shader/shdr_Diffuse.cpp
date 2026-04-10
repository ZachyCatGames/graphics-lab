#include <engine/raytracer/shader/shdr_Diffuse.h>
#include <engine/raytracer/eng_Scene.h>

namespace eng::rt::shdr {

Vector3DF Diffuse::GetColor(Scene* p_scene, int depth, const HitStruct& rec) {
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

    /* Obtain base color from base shader. */
    const auto base_color = m_shader->GetColor(p_scene, depth, rec);

    const Ray r(rec.position, direction + rec.normal);

    return base_color * p_scene->GetRayColor(r, {0.001, std::numeric_limits<float>::infinity()}, depth+1);
}

} // namespace eng::shdr
