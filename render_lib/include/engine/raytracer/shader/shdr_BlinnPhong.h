#pragma once
#include <engine/raytracer/eng_ShaderBase.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng::rt::shdr {

class BlinnPhong : public ShaderBase {
public:
    template<typename R>
    constexpr BlinnPhong(const Material& material, R&& lights)
        : ShaderBase(material),
          m_lights(std::from_range, std::forward<R>(lights)) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
private:
    std::vector<eng::shdr::PointLight> m_lights;
}; // class BlinnPhong

} // namespace eng::shdr
