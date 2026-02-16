#include "BlinnPhong.h"

namespace eng {

Vector3DF BlinnPhongShader::GetColor(const HitStruct& rec) {
    /* Use lambertian shader to get the base color. */
    const auto base_color = detail::LambertianShaderImpl::GetColor(rec);

    const auto half_vector = (rec.r.direction() + m_point_light.direction()).normalize();

    return base_color + std::pow(std::max(0.0, dot(rec.normal, half_vector)), m_exp);
}

} // namespace eng
