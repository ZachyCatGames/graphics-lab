#pragma once
#include <engine/eng_ObjectBase.h>
#include <engine/shader/shdr_IShader.h>

#include <engine/shader/detail/shdr_LambertianImpl.h>

namespace eng {

class BlinnPhongShader : public IShader, public ObjectBase<BlinnPhongShader>, private detail::LambertianShaderImpl {
public:
    constexpr BlinnPhongShader(ray pt_light, float exp) : detail::LambertianShaderImpl(pt_light), m_exp(exp) {}

    virtual Vector3DF GetColor(const HitStruct& rec) override;
private:
    float m_exp;
}; // class BlinnPhongShader

} // namespace eng
