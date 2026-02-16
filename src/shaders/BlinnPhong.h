#pragma once
#include "IShader.h"
#include "detail/LambertianImpl.h"
#include "../ObjectBase.h"

namespace eng {

class BlinnPhongShader : public IShader, public ObjectBase<BlinnPhongShader>, private detail::LambertianShaderImpl {
public:
    constexpr BlinnPhongShader(ray pt_light, float exp) : detail::LambertianShaderImpl(pt_light), m_exp(exp) {}

    virtual Vector3DF GetColor(const HitStruct& rec) override;
private:
    float m_exp;
}; // class BlinnPhongShader

} // namespace eng
