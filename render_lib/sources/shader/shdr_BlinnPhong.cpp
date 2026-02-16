#include <engine/shader/shdr_BlinnPhong.h>

namespace eng::shdr {

Vector3DF BlinnPhong::GetColor(const HitStruct& rec) {
    /* Use lambertian shader to get the base color. */
    const auto base_color = detail::LambertianImpl::GetColor(rec);

    const auto half_vector = (rec.r.direction() + m_point_light.direction()).normalize();

    return base_color + std::pow(std::max(0.0, dot(rec.normal, half_vector)), m_exp);
}

} // namespace eng::shdr
