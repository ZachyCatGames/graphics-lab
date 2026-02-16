#include "LambertianImpl.h"
#include <print>

namespace eng::detail {

Vector3DF LambertianShaderImpl::GetColor(const HitStruct& rec) {
    // TODO: Don't hardcode intensity coefficient.
    const auto nl = std::max(0.0, dot(m_point_light.direction(), rec.normal)) / 11.0;
    //std::print("{}\n", nl);
    return Vector3DF ( nl, nl, nl );
}

} // namespace eng::detail
