#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_Ray.h>

namespace eng::shdr::detail {

class LambertianImpl {
public:
    constexpr LambertianImpl(Ray point_light) : m_point_light(point_light) {}

    [[nodiscard]] Vector3DF GetColor(const HitStruct& rec);
protected:
    Ray m_point_light;
}; // class LambertianImpl

} // namespace eng::shdr::detail
