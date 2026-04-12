#pragma once
#include <engine/raytracer/eng_IShader.h>
#include <engine/eng_ObjectBase.h>
#include <engine/eng_ObjectManager.h>

#include <engine/shader/shdr_PointLight.h>

namespace eng::rt::shdr {

class BlinnPhong : public IShader, public ObjectBase<BlinnPhong> {
public:
    template<typename R>
    constexpr BlinnPhong(Vector3DF baseColor, R&& lights, float exp)
        : m_baseColor(baseColor),
          m_lights(std::from_range, std::forward<R>(lights)),
          m_exp(exp) {}

    virtual Vector3DF GetColor(RayCaster* pRc, int depth, const HitStruct& rec) override;
private:
    Vector3DF m_baseColor;
    std::vector<eng::shdr::PointLight> m_lights;
    float m_exp;
}; // class BlinnPhong

} // namespace eng::shdr
