#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_ray.h>

namespace eng::shdr::detail {

class LambertianImpl {
public:
    constexpr LambertianImpl(ray point_light) : m_point_light(point_light) {}

    Vector3DF GetColor(const HitStruct& rec);
protected:
    ray m_point_light;
}; // class LambertianImpl

} // namespace eng::shdr::detail
