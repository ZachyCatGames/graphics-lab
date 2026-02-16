#pragma once
#include <engine/eng_IShader.h>
#include <engine/eng_ObjectBase.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng::shdr {

class BlinnPhong : public IShader, public ObjectBase<BlinnPhong>, private detail::LambertianImpl {
public:
    constexpr BlinnPhong(ray pt_light, float exp) : detail::LambertianImpl(pt_light), m_exp(exp) {}

    virtual Vector3DF GetColor(const HitStruct& rec) override;
private:
    float m_exp;
}; // class BlinnPhong

} // namespace eng::shdr
