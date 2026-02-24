#include <engine/shader/detail/shdr_LambertianImpl.h>
#include <print>

namespace eng::shdr::detail {

Vector3DF LambertianImpl::GetColor(Scene* p_scene, const HitStruct& rec) {
    /* Request color from base shader. */
    const auto base_color = m_base ? m_base->GetColor(p_scene, rec) : Vector3DF(1,1,1);

    const auto dir = this->GetDirection(rec.position).normalize();

    // TODO: Don't hardcode intensity coefficient.
    const auto nl = std::max(0.0, dot(rec.normal, dir));
    //std::print("{}\n", nl);
    return Vector3DF( nl, nl, nl ) * m_light_intensity * base_color;
}

} // namespace eng::shdr::detail
