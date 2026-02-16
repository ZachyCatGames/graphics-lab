#pragma once
#include <engine/eng_HitStruct.h>
#include <engine/eng_ray.h>

namespace eng::detail {

class LambertianShaderImpl {
public:
    constexpr LambertianShaderImpl(ray point_light) : m_point_light(point_light) {}

    Vector3DF GetColor(const HitStruct& rec);
protected:
    ray m_point_light;
}; // class LambertianShaderImpl

} // namespace eng::detail
