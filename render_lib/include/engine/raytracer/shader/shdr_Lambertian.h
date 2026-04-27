#pragma once
#include <engine/raytracer/eng_ShaderBase.h>
#include <ranges>

#include <engine/shader/shdr_PointLight.h>

namespace eng::rt::shdr {

class Lambertian : public ShaderBase {
public:
    template<std::ranges::input_range R>
    constexpr Lambertian(const Material& material, R&& lights)
        : ShaderBase(material),
          m_lights(std::from_range, std::forward<R>(lights)) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
public:
    std::vector<eng::shdr::PointLight> m_lights;
}; // class Lambertian

} // namespace eng::shdr
